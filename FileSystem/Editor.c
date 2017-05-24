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
	while (EditorStatus)
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
					printf("%c", tmp);
				}
				if(tmp == 8)
				{
					putc(8, stdout);
					putc(' ', stdout);
					putc(8, stdout);
				}
			}
#ifdef __GNUC__
			resetTermios();
#endif
			printf("\n\n---------Now, You Are In Command Mode---------\n");
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
	EditorMode = ED_E;
	return 0;
}

void InitEditor()
{
	EditorStatus = ED_OPEN;
	EditorMode = ED_E;
	CommandContext context;
	context.Entries = malloc(sizeof(CommandEntry) * 5);
	context.CommandCnt = 0;
	Register(&context, "q", &EditorQuit);

	Exec(&context);
	free(context.Entries);
}