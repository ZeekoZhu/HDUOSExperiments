#include "Utils.h"
#include "Category.h"
#include "Command.h"





void Init()
{
	memset(Vhd, 0, sizeof Vhd);
	memset(Fat, -1, sizeof Fat);
	ARRAYINIT(Fcb, FileCategory, FILE_CNT_MAX, _it->Id = -1;);
	root = CreateFile("/", FM_R, FT_D);
}

void MyLS()
{
	char str[100];
	int it = cwd->Child;
	while (it != FCB_NULL && it != FCB_NAN)
	{
		FcbToString(&FileCategory[it], str, 100);
		printf("%s\n", str);
		//memset(str, 0, sizeof str);
		it = FileCategory[it].Sibling;
	}
}

void MyExit()
{
	printf("Exiting...\n");
	exit(0);
}

void MyMkdir()
{
	char dirName[36];
	scanf_s("%s", dirName, 36);
	if (strchr(dirName, '/') != NULL)
	{
		printf("File name can not contains '/'\n");
		return;
	}
	if (FindChild(cwd, dirName) != NULL)
	{
		printf("File is already exist\n");
		return;
	}
	Fcb* fcb = CreateFile(dirName, FM_W | FM_R, FT_D);
	fcb->Sibling = cwd->Child;
	cwd->Child = fcb->Id;
	if (cwd == NULL)
	{
		fcb->Parent = 0;
	}
	else
	{
		fcb->Parent = cwd->Id;
	}
}

void MyCd()
{
	char input[1000];
	scanf_s("%s", input, 1000);
	char newCwd[1000];
	if (input[0] != '/')
	{
		if (cwd != root)
		{
			sprintf_s(newCwd, 1000, "%s%c%s", cwdPath, '/', input);
		}
		else
		{
			sprintf_s(newCwd, 1000, "%s%s", cwdPath, input);
		}
		printf(newCwd);
		printf("%c", '\n');
	}
	else
	{
		strcpy_s(newCwd, 1000, input);
	}
	char pathTmp[1000];
	strcpy_s(pathTmp, 1000, newCwd);
	Fcb* next = ParsePath(pathTmp);
	if (next == NULL)
	{
		printf("Directory is not exist");
		return;
	}
	cwd = next;
	strcpy_s(cwdPath, 1000, newCwd);
}

void MyRmDir()
{
	char dir[40];
	scanf_s("%s", dir, 40);
	Fcb* fcb = FindChild(cwd, dir);
	if (fcb == NULL)
	{
		printf("No such directory\n");
		return;
	}
	if (fcb->Mode & FM_W != FM_W)
	{
		printf("Permission denied\n");
		return;
	}
	if (fcb->Child != FCB_NULL)
	{
		printf("Directory is not empty\n");
		return;
	}
	Fcb* parent = &FileCategory[fcb->Parent];
	parent->Child = fcb->Sibling;
	fcb->Id = -1;
}

void MyCwd()
{
	printf("%s\n", cwdPath);
}




void CommandInit()
{
	Register("ls", &MyLS);
	Register("exit", &MyExit);
	Register("mkdir", &MyMkdir);
	Register("cd", &MyCd);
	Register("rmdir", &MyRmDir);
	Register("cwd", &MyCwd);
}

int main()
{
	CommandInit();
	Init();
	cwd = root;
	Commander();
	printf("hello from FileSystem!\n");
	return 0;
}