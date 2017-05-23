#pragma once
#ifndef _MY_EDITOR
#define _MY_EDITOR

/**
 * \brief 编辑模式
 */
#define ED_E 0
/**
 * \brief 命令模式
 */
#define ED_C 1

char* Content;
int EditorMode;


void InitEditor();
void Exec();

#endif
