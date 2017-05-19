#include "Command.h"
#include "Utils.h"
#include <stdio.h>
#include <string.h>

/**
 * \brief ���ڿ�ʼ���������֮��~
 */
void Commander()
{
	char command[50];
	while (1)
	{
		printf("$ ");
		scanf_s("%s", command, 50);
		ARRAYFIRST(CommandEntry, Entries, CommandCnt, strcmp(_it->Command, command) == 0, index);
		if (index == -1)
		{
			printf("%s is not a command\n", command);
			continue;
		}
		Entries[index].Func();
	}
}

/**
 * \brief ע��һ������
 * \param commandName ��������
 * \param func Ҫִ�еĺ���
 */
void Register(const char* commandName, void(*func)())
{
	//printf("%d", strlen(commandName));
	Entries[CommandCnt].Command = malloc(sizeof(char) * strlen(commandName) + 1);
	Entries[CommandCnt].Func = func;
	strcpy_s(Entries[CommandCnt].Command, strlen(commandName) + 1, commandName);
	CommandCnt++;
}
