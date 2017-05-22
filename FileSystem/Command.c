#include "Command.h"
#include "Utils.h"
#include <stdio.h>
#include <string.h>

/**
 * \brief 现在开始你的命令行之旅~
 */
void Commander()
{
	char command[50];
	while (1)
	{
		printf("$ ");
		scanf("%s", command);
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
 * \brief 注册一条命令
 * \param commandName 命令名称
 * \param func 要执行的函数
 */
void Register(const char* commandName, void(*func)())
{
	//printf("%d", strlen(commandName));
	Entries[CommandCnt].Command = malloc(sizeof(char) * strlen(commandName) + 1);
	Entries[CommandCnt].Func = func;
	strcpy(Entries[CommandCnt].Command, commandName);
	CommandCnt++;
}
