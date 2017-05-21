#include "Category.h"
#include "Utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/**
 * \brief 查询文件的读权限
 * \param fcb 要查询的文件的控制块
 * \return 文件读权限状态
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
 * \brief 查询文件的写权限
 * \param fcb fcb 要查询的文件的控制块
 * \return 文件写权限状态
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
 * \brief 获取文件的最后写入日期字符串
 * \param fcb 要查询的文件控制块
 * \param res 文件最后写入日期字符串
 * \param len 缓冲区长度，不应低于20
 */
void GetLastWriteStr(const Fcb* fcb, char* res, int len)
{
	struct tm tm;
	localtime_s(&tm, &fcb->TimeInfo.LastWrite);
	strftime(res, len, "%Y/%m/%d %H:%M:%S", &tm);
}

/**
* \brief 获取文件的最后读取日期字符串
* \param fcb 要查询的文件控制块
* \param res 文件最后读取日期字符串
* \param len 缓冲区长度，不应低于22
*/
void GetLastReadStr(const Fcb* fcb, char* res, int len)
{
	struct tm tm;
	localtime_s(&tm, &fcb->TimeInfo.LastWrite);
	strftime(res, len, "%Y/%m/d - %H:%M:%S", &tm);
}

/**
 * \brief 获取文件控制块的描述字符串
 * \param fcb 文件控制块
 * \param res 结果字符串
 * \param len 缓冲区长度，不应小于22
 */
void FcbToString(const Fcb* fcb, char* res, int len)
{
	char lastRead[30], lastWrite[30];
	GetLastWriteStr(fcb, lastRead, 30);
	GetLastWriteStr(fcb, lastWrite, 30);
	sprintf_s(res, len, "%c%c%c %-20s %-20s %10dB %-36s", GetFileType(fcb), GetReadMode(fcb), GetWriteMode(fcb), lastRead, lastWrite, fcb->Size, fcb->FileName);
}

/**
* \brief 从 Fcb 数组中创建新的文件，不需要手动释放资源
* \param fileName 文件名
* \param mode 读写权限
* \param type 文件类型
* \return 文件控制块，创建失败
*/
Fcb* CreateFile(const char* fileName, char mode, char type)
{
	Fcb* result = NULL;
	// 找一个空记录出来
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
		// 空文件分配一个磁盘块
		if (type == FT_R)
		{
			ARRAYFIRST(short, Fat, FAT_CNT_MAX, *_it == FAT_AVALIABLE, fatId);
			result->Address = fatId;
			Fat[fatId] = FAT_NULL;
			// 文件只能是叶子节点
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
* \brief 在指定目录下寻找直接子文件
* \param parent 要寻找的目录
* \param childName 文件名
* \return 目标子文件的控制块
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
* \brief 将指定的绝对路径转换为文件控制块
* \param absPath 文件的绝对路径
* \return 绝对路径指向的文件的控制块
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
			result = FindChild(result, fileName); // 任意一级无法找到子文件，就退出并返回 NULL
			if (result == NULL)
			{
				break;
			}
			index = i + 1;
		}
	}
	return result;
}

/**
 * \brief 获取指定文件用字符标识的类型
 * \param fcb 文件控制块
 * \return 表示文件类型的字符
 */
char GetFileType(const Fcb* fcb)
{
	return fcb->Type == FT_D ? 'D' : '-';
}

