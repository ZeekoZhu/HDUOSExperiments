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
}




void CommandInit()
{
	Register("ls", &MyLS);
	Register("exit", &MyExit);
	Register("mkdir", &MyMkdir);
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