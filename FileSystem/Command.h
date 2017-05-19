#pragma once
#ifndef _COMMAND_MODULE
#define _COMMAND_MODULE
typedef struct
{
	char* Command;
	void(*Func)();
} CommandEntry;

int CommandCnt;

CommandEntry Entries[20];
void Commander();
void Register(const char* commandName, void(*func)());
#endif
