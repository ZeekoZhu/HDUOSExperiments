#include "Editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Command.h"

#ifdef __GNUC__
#include <termios.h>
int getkey() {
	int character;
	struct termios orig_term_attr;
	struct termios new_term_attr;

	/* set the terminal to raw mode */
	tcgetattr(fileno(stdin), &orig_term_attr);
	memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
	new_term_attr.c_lflag &= ~(ECHO | ICANON);
	new_term_attr.c_cc[VTIME] = 0;
	new_term_attr.c_cc[VMIN] = 0;
	tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

	/* read a character from the stdin stream without blocking */
	/*   returns EOF (-1) if no character is available */
	character = fgetc(stdin);

	/* restore the original terminal attributes */
	tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

	return character;
}
#define GETKEY getkey()
#endif
#ifdef _MSC_VER
#include <conio.h>
#define GETKEY getche()
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
			while ((tmp =GETKEY) != EOF)
			{
				printf("-%d-\n", tmp);
			}
		}
	}
}

int EditorQuit()
{
	EditorStatus = ED_CLOSE;
	printf("Editor closing...\n");
	return 1;
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