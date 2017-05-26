#pragma once
#ifndef _MY_EDITOR
#define _MY_EDITOR
#include "Command.h"

/**
 * \brief �༭ģʽ
 */
#define ED_E 0
/**
 * \brief ����ģʽ
 */
#define ED_C 1

#define ED_CLOSE 0
#define ED_OPEN 1
#define ED_WRITE 2

char* Content;
static int EDContentSize;
static int EDContentLen;
int EditorMode;
int EditorStatus;


void InitEditor();
void Exec(CommandContext* context);

#endif
