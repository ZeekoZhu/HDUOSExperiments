#include "Command.h"
#include "Utils.h"
#include <stdio.h>
#include <string.h>

/**
 * \brief ���ڿ�ʼ���������֮��~
 */
void Commander(CommandContext* context)
{
	char command[50];
	while (1)
	{
		printf("$ ");
		scanf("%s", command);
		ARRAYFIRST(CommandEntry, context->Entries, context->CommandCnt, strcmp(_it->Command, command) == 0, index);
		if (index == -1)
		{
			printf("%s is not a command\n", command);
			continue;
		}
		context->Entries[index].Func();
		// ��ջ�����
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
	}
}

/**
 * \brief ע��һ������
 * \param commandName ��������
 * \param func Ҫִ�еĺ���
 */
void Register(CommandContext* context, const char* commandName, void(*func)())
{
	//printf("%d", strlen(commandName));
	context->Entries[context->CommandCnt].Command = malloc(sizeof(char) * strlen(commandName) + 1);
	context->Entries[context->CommandCnt].Func = func;
	strcpy(context->Entries[context->CommandCnt].Command, commandName);
	context->CommandCnt++;
}
