#include "Category.h"
#include "Utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/**
 * \brief ��ѯ�ļ��Ķ�Ȩ��
 * \param fcb Ҫ��ѯ���ļ��Ŀ��ƿ�
 * \return �ļ���Ȩ��״̬
 */
char GetReadMode(const Fcb* fcb)
{
	if (fcb == NULL)
	{
		return '-';
	}
	return fcb->Mode == FM_R ? 'r' : '-';
}

/**
 * \brief ��ѯ�ļ���дȨ��
 * \param fcb fcb Ҫ��ѯ���ļ��Ŀ��ƿ�
 * \return �ļ�дȨ��״̬
 */
char GetWriteMode(const Fcb* fcb)
{
	if (fcb == NULL)
	{
		return '-';
	}
	return fcb->Mode == FM_W ? 'w' : '-';
}

/**
 * \brief ��ȡ�ļ������д�������ַ���
 * \param fcb Ҫ��ѯ���ļ����ƿ�
 * \param res �ļ����д�������ַ���
 * \param len ���������ȣ���Ӧ����20
 */
void GetLastWriteStr(const Fcb* fcb, char* res, int len)
{
	struct tm* tm = localtime(&fcb->TimeInfo.LastWrite);
	strftime(res, len, "%Y/%m/%d %H:%M:%S", tm);
}

/**
* \brief ��ȡ�ļ�������ȡ�����ַ���
* \param fcb Ҫ��ѯ���ļ����ƿ�
* \param res �ļ�����ȡ�����ַ���
* \param len ���������ȣ���Ӧ����22
*/
void GetLastReadStr(const Fcb* fcb, char* res, int len)
{
	struct tm* tm = localtime(&fcb->TimeInfo.LastWrite);
	strftime(res, len, "%Y/%m/d - %H:%M:%S", &tm);
}

/**
 * \brief ��ȡ�ļ����ƿ�������ַ���
 * \param fcb �ļ����ƿ�
 * \param res ����ַ���
 * \param len ���������ȣ���ӦС��84
 */
void FcbToString(const Fcb* fcb, char* res, int len)
{
	char lastRead[30], lastWrite[30];
	GetLastWriteStr(fcb, lastRead, 30);
	GetLastWriteStr(fcb, lastWrite, 30);
	sprintf(res, "%c%c%c %-20s %-20s %10dB %-36s", GetFileType(fcb), GetReadMode(fcb), GetWriteMode(fcb), lastRead, lastWrite, fcb->Size, fcb->FileName);
}

/**
* \brief �� Fcb �����д����µ��ļ�������Ҫ�ֶ��ͷ���Դ
* \param fileName �ļ���
* \param mode ��дȨ��
* \param type �ļ�����
* \return �ļ����ƿ飬����ʧ��
*/
Fcb* CreateFile(const char* fileName, char mode, char type)
{
	Fcb* result = NULL;
	// ��һ���ռ�¼����
	ARRAYFIRST(Fcb, FileCategory, FILE_CNT_MAX, _it->Id < 0, index);
	if (index >= 0)
	{
		result = &FileCategory[index];
		strcpy(result->FileName, fileName);
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
		if (strcmp(FileCategory[it].FileName, childName) == 0)
		{
			result = &FileCategory[it];
			break;
		}
		it = FileCategory[it].Sibling;
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
	Fcb* it = &FileCategory[0];
	if (strcmp(absPath, "/") == 0)
	{
		return it;
	}
	char * filename = strtok(absPath, "/");
	while (filename != NULL && it != NULL)
	{
		it = FindChild(it, filename);
		filename = strtok(NULL, "/");
	}
	return it;
}

/**
 * \brief ��ȡָ���ļ����ַ���ʶ������
 * \param fcb �ļ����ƿ�
 * \return ��ʾ�ļ����͵��ַ�
 */
char GetFileType(const Fcb* fcb)
{
	return fcb->Type == FT_D ? 'D' : '-';
}

/**
 * \brief ��ȡָ���ļ��ľ���·��
 * \param path �ļ��ľ���·��
 * \param len ����������
 * \param fcb �ļ����ƿ�
 */
void GetAbsolutePath(char* path, int len, const Fcb* fcb)
{
	memset(path, 0, len);
}

