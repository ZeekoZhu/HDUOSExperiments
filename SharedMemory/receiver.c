#include "Utils.h"
#include "common.h"

void Dispose()
{
	sem_close(Ssnd);
	sem_close(Srcv);
}

void Receiver()
{
	sem_wait(Srcv);
	char str[SHMSIZE];
	memcpy(str, ShmPtr, SHMSIZE);
	printf("%s", str);
	char* over = "over";
	memcpy(ShmPtr, over, strlen(over));
	sem_post(Ssnd);
}

int main()
{
	Init();
	Receiver();
	Dispose();
	return 0;
}