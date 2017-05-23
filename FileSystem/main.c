#include "Utils.h"
#include "Category.h"
#include "Command.h"

#define FILEEXIST(cwd, file) \
if(FindChild(cwd, file) == NULL)\
{\
	printf("No such file or directory\n");\
	return;\
}

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
	scanf("%s", dirName);
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
	scanf("%s", input);
	char newCwd[1000];
	if (input[0] != '/')
	{
		if (cwd != root)
		{
			sprintf(newCwd, "%s%c%s", cwdPath, '/', input);
		}
		else
		{
			sprintf(newCwd, "%s%s", cwdPath, input);
		}
		printf(newCwd);
		printf("%c", '\n');
	}
	else
	{
		strcpy(newCwd, input);
	}
	char pathTmp[1000];
	strcpy(pathTmp, newCwd);
	Fcb* next = ParsePath(pathTmp);
	if (next == NULL)
	{
		printf("Directory is not exist");
		return;
	}
	cwd = next;
	strcpy(cwdPath, newCwd);
}

void MyRmDir()
{
	char dir[40];
	scanf("%s", dir);
	Fcb* fcb = FindChild(cwd, dir);
	if (fcb == NULL)
	{
		printf("No such directory\n");
		return;
	}
	if (fcb->Type != FT_D)
	{
		printf("%s is not a directory\n", dir);
		return;
	}
	if ((fcb->Mode & FM_W) != FM_W)
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

void MyCreate()
{
	char input[40];
	scanf("%s", input);
	if (strchr(input, '/') != NULL)
	{
		printf("Invalid filename\n");
		return;
	}
	if (FindChild(cwd, input) != NULL)
	{
		printf("File is already exist\n");
	}
	Fcb* fcb = CreateFile(input, FM_R | FM_W, FT_R);
	fcb->Sibling = cwd->Child;
	cwd->Child = fcb->Id;
	fcb->Parent = cwd->Id;
}

void MyRm()
{
	char filename[40];
	scanf("%s", filename);
	Fcb* fcb = FindChild(cwd, filename);
	if (fcb == NULL)
	{
		printf("No such file\n");
		return;
	}
	if (fcb->Type != FT_R)
	{
		printf("%s is not a regular file\n", filename);
		return;
	}
	if ((fcb->Mode & FM_W) != FM_W)
	{
		printf("Permission denied\n");
		return;
	}
	Fcb* parent = &FileCategory[fcb->Parent];
	parent->Child = fcb->Sibling;
	fcb->Id = -1;
}

void MyChmod()
{
	int mode;
	char input[40];
	scanf("%d %s", &mode, input);
	Fcb* fcb = FindChild(cwd, input);
	if (fcb == NULL)
	{
		printf("No such file or directory\n");
		return;
	}
	if (mode < 0 || mode > 7)
	{
		printf("Invalid mode\n");
		return;
	}
	fcb->Mode = mode;
}




void CommandInit(CommandContext* entries)
{
	Register(entries, "ls", &MyLS);
	Register(entries, "exit", &MyExit);
	Register(entries, "mkdir", &MyMkdir);
	Register(entries, "cd", &MyCd);
	Register(entries, "rmdir", &MyRmDir);
	Register(entries, "cwd", &MyCwd);
	Register(entries, "touch", &MyCreate);
	Register(entries, "chmod", &MyChmod);
	Register(entries, "rm", &MyRm);
}

int main()
{
	CommandContext cmdContext;
	cmdContext.CommandCnt = 0;
	cmdContext.Entries = malloc(sizeof(CommandEntry) * 20);
	CommandInit(&cmdContext);
	Init();
	cwd = root;
	Commander(&cmdContext);
	printf("hello from FileSystem!\n");
	free(cmdContext.Entries);
	return 0;
}