#pragma once
#ifndef _COMMAND_MODULE
#define _COMMAND_MODULE
typedef struct
{
	char* Command;
	int(*Func)();
} CommandEntry;

/**
 * \brief �洢������ָ��������ģ�ʹ��ǰ��Ҫ�ֶ���ʼ��
 */
typedef struct
{
	int CommandCnt;
	CommandEntry* Entries;
} CommandContext;


void Commander(CommandContext* context, char indecate);
/**
 * \brief ��ָ����ָ����������ע��ָ��
 * \param context ������ָ��������
 * \param commandName ָ������
 * \param func ָ��Ҫִ�еĺ��������� 0 ��ʾ������ȡָ����� 1 ��ʾ���ٽ���ָ��
 */
void Register(CommandContext* context, const char* commandName, int(*func)());
#endif
