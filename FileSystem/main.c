#include "Utils.h"
#include "Category.h"
#include <time.h>
#include "Command.h"

Fcb* root;
Fcb* cwd;
Fcb FileCategory[FILE_CNT_MAX];
short Fat[FAT_CNT_MAX];
char Vhd[DISKSIZE];

/**
 * \brief �����µ��ļ�
 * \param fileName �ļ�����·����
 * \param mode ��дȨ��
 * \param type �ļ�����
 * \return �ļ����ƿ飬����ʧ��
 */
Fcb* CreateFile(const char* fileName, char mode, char type)
{
	Fcb* result = NULL;
	ARRAYFIRST(Fcb, FileCategory, FILE_CNT_MAX, _it->Id < 0, index);
	if (index >= 0)
	{
		result = &FileCategory[index];
		strcpy_s(result->FileName, 24, fileName);
		result->Mode = mode;
		result->Type = type;
		result->Id = index;
		result->Size = 0;
		time(&result->TimeInfo.LastWrite);
		result->TimeInfo.LastRead = -1;
		// ���ļ�����һ�����̿�
		if (type == FT_R)
		{
			ARRAYFIRST(short, Fat, FAT_CNT_MAX, *_it == FAT_AVALIABLE, fatId);
			result->Address = fatId;
			Fat[fatId] = FAT_NULL;
			// �ļ�ֻ����Ҷ�ӽڵ�
			result->Sibling = FCB_NULL;
			result->Child = FCB_NAN;
		}
		if (type == FT_D)
		{
			result->Sibling = FCB_NULL;
			result->Child = FCB_NULL;
		}
	}
	return result;
}

/**
 * \brief ��ָ��Ŀ¼��Ѱ��ֱ�����ļ�
 * \param parent ҪѰ�ҵ�Ŀ¼
 * \param childName �ļ���
 * \return Ŀ�����ļ��Ŀ��ƿ�
 */
Fcb* FindChild(Fcb* parent, const char* childName)
{
	Fcb* result = NULL;
	if (parent->Type == FT_R)
	{
		return result;
	}
	short it = parent->Child;
	while (it != FCB_NULL)
	{
		result = &FileCategory[it];
		if (strcmp(result->FileName, childName) == 0)
		{
			break;
		}
		it = result->Sibling;
	}
	return result;
}

/**
 * \brief ��ָ���ľ���·��ת��Ϊ�ļ����ƿ�
 * \param absPath �ļ��ľ���·��
 * \return ����·��ָ����ļ��Ŀ��ƿ�
 */
Fcb* ParsePath(const char* absPath)
{
	Fcb* result = &FileCategory[0];
	int index = 1;
	char fileName[36];
	memset(fileName, 0, sizeof fileName);
	for (int i = 1; i < strlen(absPath); i++)
	{
		if (absPath[i] == '/')
		{
			strcpy_s(fileName, i - index, absPath);
			result = FindChild(result, fileName); // ����һ���޷��ҵ����ļ������˳������� NULL
			if (result == NULL)
			{
				break;
			}
			index = i + 1;
		}
	}
	return result;
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
	printf("%c %c %-40s\n", cwd->Type == FT_D ? 'D' : '-', cwd->Mode == FM_R ? 'R' : '-', cwd->FileName);
}

void MyExit()
{
	printf("Exiting...\n");
	exit(0);
}

#pragma region COMMAND_MODULE






void CommandInit()
{
	Register("my_ls", &MyLS);
	Register("my_exitsys", &MyExit);
}




#pragma endregion


int main()
{
	CommandInit();
	Init();
	cwd = root;
	Commander();
	printf("hello from FileSystem!\n");
	return 0;
}