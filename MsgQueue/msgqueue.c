#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <semaphore.h>
#define KEY 2333
#define MSG_LENGTH 100
#define MSGTYPE 1
#define MQMAXSIZE 10 // 不能超过文件 /proc/sys/fs/mqueue/msg_max 中的数值，我的机器上面是10
#define MSGMAXLEN 1000 // 不能超过 /proc/sys/fs/mqueue/msgsize_max 的数值
#define TRYCATCH(pred, msg) if(pred) {fprintf(stderr, "ERROR: %s -- %s\n", strerror(errno), msg); exit(1);}
int MQId = 0;
/**
 * \brief 发送消息的信号量，初始值为 0
 */
sem_t Ssend;
/**
 * \brief 接受消息的信号量，初始值为 0
 */
sem_t Srec;
sem_t Sexit;
struct mq_attr MqAttr;

typedef struct
{
	long int message_type;
	char msg[MSG_LENGTH];
}Message;


mqd_t TryOpenMQ()
{
	mqd_t mqId = mq_open("/mymq", O_RDWR | O_CREAT | O_NONBLOCK, 0666, &MqAttr);
	TRYCATCH(mqId == -1, "Create message queue faild");
	return mqId;
}


/**
 * \brief 发送消息，失败则结束进程
 * \param msg 要发送的消息
 * \param prio 消息优先级，普通消息优先级为 1，结束进程消息为 2
 */
void TrySend(char* msg, unsigned prio)
{
	int res = mq_send(MQId, msg, strlen(msg) * sizeof(char), prio);
	TRYCATCH(res == -1, "Send message faild");
}

void TryReceive(char* msg)
{
	int res = mq_receive(MQId, msg, MSGMAXLEN + 1, NULL);
	TRYCATCH(res == -1, "Receive message faild");
}

/**
 * \brief 初始化信号量并创建消息队列
 * \return 新创建的消息队列Id
 */
void Init()
{
	sem_init(&Ssend, 1, 0);
	sem_init(&Sexit, 1, 0);
	sem_init(&Srec, 1, 0);
	MqAttr.mq_flags = O_NONBLOCK;
	MqAttr.mq_maxmsg = MQMAXSIZE;
	MqAttr.mq_msgsize = MSGMAXLEN;
	// 创建消息队列
	MQId = TryOpenMQ();
}


/**
 * \brief 发送消息的函数
 */
void Sender()
{
	char* msg = malloc(MSGMAXLEN);
	struct mq_attr mqAttr;
	int stmp;
	int flag = 1;
	while (flag)
	{
		TRYCATCH(mq_getattr(MQId, &mqAttr), "Can not get messge queue infomation");
		if (mqAttr.mq_curmsgs == MQMAXSIZE - 1)
		{
			sem_wait(&Srec);	// 等待接收者接受
		}
		printf("Enter your message:\n> ");
		scanf("%s", msg);

		// 用户输入结束指令
		if (strcmp(msg, "exit") == 0)
		{
			strcpy(msg, "end");
			TrySend(msg, 2);
			flag = 0;
		}
		else
		{
			TrySend(msg, 1);
		}
		TRYCATCH(sem_getvalue(&Ssend, &stmp), "Can not read sem Send");
		printf("%d Process Waiting...\n", stmp);
		if (stmp <= 0)
		{
			sem_post(&Ssend);	// 声明已经发送消息
		}
	}
	sem_wait(&Sexit);	// 等待退出应答
	printf("Received Response\n");
	TryReceive(msg);
	printf("Exit Response:%s\n", msg);
	TRYCATCH(mq_close(MQId) == -1, "Can not close the message queue");
	free(msg);
}
/**
 * \brief 接受消息的函数
 */
void Receiver()
{
	FILE* out = fopen("rcv.txt", "w");
	char* msg = malloc(MSGMAXLEN);
	struct mq_attr mqAttr;
	int stmp;
	while (1)
	{
		TRYCATCH(mq_getattr(MQId, &mqAttr), "Can not get messge queue infomation");
		if (mqAttr.mq_curmsgs == 0)
		{
			sem_wait(&Ssend); // 等待发送者发送消息
		}
		memset(msg, 0, MSGMAXLEN);
		TryReceive(msg);
		//printf("Received message:\n%s\n\n", msg);
		fprintf(out, "Received message:\n%s\n\n", msg);
		fflush(out);
		TRYCATCH(sem_getvalue(&Srec, &stmp), "Can not read sem rec");
		if (stmp <= 0)
		{
			sem_post(&Srec);
		}
		if (strcmp(msg, "end") == 0)
		{
			break;
		}
	}
	strcpy(msg, "over");
	TrySend(msg, 2);
	sem_post(&Sexit);
	fclose(out);
}
int main()
{
	pthread_t id1, id2;
	Init();

	int res = pthread_create(&id1, NULL, (void *)Sender, NULL);
	if (res != 0)
	{
		printf("Create pthread sender error!\n");
		exit(1);
	}
	int res2 = pthread_create(&id2, NULL, (void *)Receiver, NULL);
	if (res2 != 0)
	{
		printf("Create pthread receive error!\n");
		exit(1);
	}
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	printf("Exit program!\n");
	return 0;
}

