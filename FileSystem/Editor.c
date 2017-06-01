#include "Editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Command.h"

#ifdef __GNUC__
#include <termios.h>
static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
	char ch;
	ch = getchar();
	return ch;
}

/* Read 1 character without echo */
char getch(void)
{
	return getch_(0);
}

#define GETKEY getch()
#endif
#ifdef _MSC_VER
#include <conio.h>
char mygetch()
{
	char c = getch();
	if (c == '\r')
	{
		c = '\n';
	}
	return c;
}
#define GETKEY mygetch()
#endif



void Exec(CommandContext* context)
{
	if (EDContentLen > 0 && Content != NULL)
	{
		printf(Content);
	}
	while (EditorStatus == ED_OPEN)
	{
		if (EditorMode == ED_C)
		{
			Commander(context, '>');
		}
		else
		{
			char tmp;
#ifdef __GNUC__
			initTermios(0); // with echo
#endif
			while ((tmp = GETKEY) != EOF)
			{
				if (tmp == 27) // ESC
				{
					break;
				}
				if (32 <= tmp && tmp <= 126 || tmp == '\n')
				{
					// 缓冲区不够就扩充一倍
					if (EDContentLen + 1 >= EDContentSize)
					{
						EDContentSize *= 2;
						char* ptmp = malloc(EDContentSize);
						strcpy(ptmp, Content);
						free(Content);
						Content = ptmp;
					}
					Content[EDContentLen] = tmp;
					Content[EDContentLen + 1] = 0;
					EDContentLen++;
					printf("%c", tmp);
				}
				if (tmp == 8)
				{
					EDContentLen--;
					Content[EDContentLen] = 0;
					putc(8, stdout); //退格
					putc(' ', stdout); //空格
					putc(8, stdout); //空格
				}
			}
#ifdef __GNUC__
			resetTermios();
#endif
			printf("\n---------Now, You Are In Command Mode---------\n");
			printf("i\tSwitch to EDITING mode.\nq\tQuit without save.\nw\tSave file.\n");
			EditorMode = ED_C;
		}
	}
}

int EditorQuit()
{
	EditorStatus = ED_CLOSE;
	printf("Editor closing...\n");
	return 1;
}
int Editing()
{
	printf("\n---------Now, You Are In Editing Mode---------\n");
	EditorMode = ED_E;
	return 1;
}

int EditorWriteSig()
{
	EditorStatus = ED_WRITE;
	return 1;
}

/**
 * \brief 函数退出后，可以从 Content 中读取用户的输入。
 * 如果需要编辑空白文件，Content 需要设置为 NULL。
 * 如果需要追加编辑，Content，EDContentSize，EDContentLen 需要手动设置
 */
void InitEditor()
{
	EditorStatus = ED_OPEN;
	EditorMode = ED_E;
	if (Content == NULL)
	{
		EDContentSize = 512;
		Content = malloc(sizeof(char)*EDContentSize);
		EDContentLen = 0;
	}
	CommandContext context;
	context.Entries = malloc(sizeof(CommandEntry) * 5);
	context.CommandCnt = 0;
	Register(&context, "q", &EditorQuit);
	Register(&context, "i", &Editing);
	Register(&context, "w", &EditorWriteSig);
	printf("\nPress 'ESC' to switch to COMMAND Mode.\n");
	Exec(&context);
	CommandEntry* entries = context.Entries;
	free(entries);
}