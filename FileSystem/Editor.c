#include "Editor.h"
#include <stdio.h>

void Exec()
{
	while (1)
	{
		if (EditorMode == ED_C)
		{
			printf("> ");
		}

	}
}

void InitEditor()
{
	EditorMode = ED_C;
}