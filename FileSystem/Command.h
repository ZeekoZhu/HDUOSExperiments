#pragma once
#ifndef _COMMAND_MODULE
#define _COMMAND_MODULE
typedef struct
{
	char* Command;
	void(*Func)();
} CommandEntry;

typedef struct
{
	int CommandCnt;
	CommandEntry* Entries;
} CommandContext;


void Commander(CommandContext* context);
void Register(CommandContext* context, const char* commandName, void(*func)());
#endif
