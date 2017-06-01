#include "Utils.h"
#include "Category.h"
#include "Command.h"
#include "Editor.h"
#ifdef _MSC_VER
#include <io.h>
#endif
#ifdef __GNUC__
#include <unistd.h>
#endif


#define FILEEXIST(cwd, file) \
if(FindChild(cwd, file) == NULL)\
{\
	printf("No such file or directory\n");\
	return;\
}

#define CHECKREGFILE(permission) \
	if (fcb == NULL)\
	{\
		printf("No such file\n");\
		return 0;\
	}\
	if (fcb->Type != FT_R)\
	{\
		printf("%s is not a regular file\n", input);\
		return 0;\
	}\
	if ((fcb->Mode & permission) != permission)\
	{\
		printf("Permission denied\n");\
		return 0;\
	}

void Init()
{
	memset(Vhd, 0, sizeof Vhd);
	memset(Fat, FAT_AVALIABLE, sizeof Fat);
	// 前 20 块被占用
	memset(Fat, FAT_NULL, sizeof(short) * 20);
	ARRAYINIT(Fcb, FileCategory, FILE_CNT_MAX, _it->Id = -1;);
	root = CreateFile("/", FM_R, FT_D);
}

int MyLS()
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
	return 0;
}

int MyExit()
{
	printf("Exiting...\n");
	return 1;
}

int MyMkdir()
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
		return 0;
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
	return 0;
}

int MyCd()
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
		return 0;
	}
	cwd = next;
	strcpy(cwdPath, newCwd);
	return 0;
}

int MyRmDir()
{
	char dir[40];
	scanf("%s", dir);
	Fcb* fcb = FindChild(cwd, dir);
	if (fcb == NULL)
	{
		printf("No such directory\n");
		return 0;
	}
	if (fcb->Type != FT_D)
	{
		printf("%s is not a directory\n", dir);
		return 0;
	}
	if ((fcb->Mode & FM_W) != FM_W)
	{
		printf("Permission denied\n");
		return 0;
	}
	if (fcb->Child != FCB_NULL)
	{
		printf("Directory is not empty\n");
		return 0;
	}
	Fcb* parent = &FileCategory[fcb->Parent];
	parent->Child = fcb->Sibling;
	fcb->Id = -1;
	return 0;
}

int MyCwd()
{
	printf("%s\n", cwdPath);
	return 0;
}

int MyCreate()
{
	char input[40];
	scanf("%s", input);
	if (strchr(input, '/') != NULL)
	{
		printf("Invalid filename\n");
		return 0;
	}
	if (FindChild(cwd, input) != NULL)
	{
		printf("File is already exist\n");
	}
	Fcb* fcb = CreateFile(input, FM_R | FM_W, FT_R);
	fcb->Sibling = cwd->Child;
	cwd->Child = fcb->Id;
	fcb->Parent = cwd->Id;
	return 0;
}

int MyRm()
{
	char input[40];
	scanf("%s", input);
	Fcb* fcb = FindChild(cwd, input);
	CHECKREGFILE(FM_W);
	DeleteFile(fcb);
	return 0;
}

int MyChmod()
{
	int mode;
	char input[40];
	scanf("%d %s", &mode, input);
	Fcb* fcb = FindChild(cwd, input);
	if (fcb == NULL)
	{
		printf("No such file or directory\n");
		return 0;
	}
	if (mode < 0 || mode > 7)
	{
		printf("Invalid mode\n");
		return 0;
	}
	fcb->Mode = mode;
	return 0;
}

int MyOpen()
{
	char input[40];
	scanf("%s", input);
	Fcb* fcb = FindChild(cwd, input);
	CHECKREGFILE(FM_W);
	printf("(O/o)verride or (A/a)ppend: ");
	scanf("%s", input);
	switch (input[0])
	{
	case 'O':
	case 'o':
		Content = NULL;
		break;
	case 'A':
	case 'a':
		Content = ReadString(fcb);
		EDContentLen = fcb->Size;
		EDContentSize = fcb->Size * 2;
		break;
	default:
		printf("Invalid option\n");
		return 0;
	}
	InitEditor();
	if (ED_WRITE == EditorStatus)
	{
		WriteString(fcb, Content);
	}
	return 0;
}


int MyRead()
{
	char input[40];
	scanf("%s", input);
	Fcb* fcb = FindChild(cwd, input);
	CHECKREGFILE(FM_R);
	if (fcb->Size == 0)
	{
		return 0;
	}
	char* content = ReadString(fcb);
	printf("%s\n----------EOF----------\n", content);
	free(content);
	return 0;
}

int OpenEditor()
{
	InitEditor();
	printf("%s", Content);
	printf("\n-------- EOF --------\n");
	return 0;
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
	Register(entries, "vin", &OpenEditor);
	Register(entries, "open", &MyOpen);
	Register(entries, "cat", &MyRead);
}

FILE* fvhd = NULL;
void LoadVhd()
{

	if (access("fs.vhd", 0) == -1)
	{
		Init();
	}
	else
	{
		fvhd = fopen("fs.vhd", "rb");
		int a = fread(Fat, sizeof(short), FAT_CNT_MAX, fvhd);
		int b = fread(FileCategory, sizeof(Fcb), FILE_CNT_MAX, fvhd);
		int c = fread(Vhd, sizeof(char), DISKSIZE, fvhd);
		root = FileCategory;
		fclose(fvhd);
	}
}

void SaveVhd()
{
	fvhd = fopen("fs.vhd", "wb+");
	fwrite(Fat, sizeof(short), FAT_CNT_MAX, fvhd);
	fwrite(FileCategory, sizeof(Fcb), FILE_CNT_MAX, fvhd);
	fwrite(Vhd, sizeof(char), DISKSIZE, fvhd);
	fclose(fvhd);
}

int main()
{
	CommandContext cmdContext;
	cmdContext.CommandCnt = 0;
	cmdContext.Entries = malloc(sizeof(CommandEntry) * 20);
	CommandInit(&cmdContext);
	LoadVhd();
	cwd = root;
	Commander(&cmdContext, '$');
	free(cmdContext.Entries);
	SaveVhd();
	return 0;
}