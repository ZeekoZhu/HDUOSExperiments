#pragma once
#ifndef _COMMAND_MODULE
#define _COMMAND_MODULE
typedef struct
{
	char* Command;
	int(*Func)();
} CommandEntry;

/**
 * \brief 存储命令行指令的上下文，使用前需要手动初始化
 */
typedef struct
{
	int CommandCnt;
	CommandEntry* Entries;
} CommandContext;


void Commander(CommandContext* context, char indecate);
/**
 * \brief 向指定的指令上下文中注册指令
 * \param context 命令行指令上下文
 * \param commandName 指令名称
 * \param func 指令要执行的函数，返回 0 表示继续读取指令，返回 1 表示不再接受指令
 */
void Register(CommandContext* context, const char* commandName, int(*func)());
#endif
