#include "Category.h"
#include "Utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char GetMode(const Fcb* fcb, char mode, char present)
{
	if (fcb == NULL)
	{
		return '-';
	}
	return (fcb->Mode & mode) > 0 ? present : '-';
}

/**
 * \brief 查询文件的读权限
 * \param fcb 要查询的文件的控制块
 * \return 文件读权限状态
 */
char GetReadMode(const Fcb* fcb)
{
	return GetMode(fcb, FM_R, 'r');
}

/**
 * \brief 查询文件的写权限
 * \param fcb fcb 要查询的文件的控制块
 * \return 文件写权限状态
 */
char GetWriteMode(const Fcb* fcb)
{
	return GetMode(fcb, FM_W, 'w');
}

/**
* \brief 查询文件的执行权限
* \param fcb fcb 要查询的文件的控制块
* \return 文件执行权限状态
*/
char GetExecMode(const Fcb* fcb)
{
	return GetMode(fcb, FM_X, 'x');
}


/**
 * \brief 获取文件的最后写入日期字符串
 * \param fcb 要查询的文件控制块
 * \param res 文件最后写入日期字符串
 * \param len 缓冲区长度，不应低于20
 */
void GetLastWriteStr(const Fcb* fcb, char* res, int len)
{
	struct tm* tm = localtime(&fcb->TimeInfo.LastWrite);
	strftime(res, len, "%Y/%m/%d %H:%M:%S", tm);
}

/**
* \brief 获取文件的最后读取日期字符串
* \param fcb 要查询的文件控制块
* \param res 文件最后读取日期字符串
* \param len 缓冲区长度，不应低于22
*/
void GetLastReadStr(const Fcb* fcb, char* res, int len)
{
	struct tm* tm = localtime(&fcb->TimeInfo.LastWrite);
	strftime(res, len, "%Y/%m/d - %H:%M:%S", tm);
}

/**
 * \brief 获取文件控制块的描述字符串
 * \param fcb 文件控制块
 * \param res 结果字符串
 * \param len 缓冲区长度，不应小于84
 */
void FcbToString(const Fcb* fcb, char* res, int len)
{
	char lastRead[30], lastWrite[30];
	GetLastWriteStr(fcb, lastRead, 30);
	GetLastWriteStr(fcb, lastWrite, 30);
	sprintf(res, "%c%c%c%c %-20s %-20s %10dB %-36s", GetFileType(fcb), GetReadMode(fcb), GetWriteMode(fcb), GetExecMode(fcb), lastRead, lastWrite, fcb->Size, fcb->FileName);
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
		strcpy(result->FileName, fileName);
		result->Mode = mode;
		result->Type = type;
		result->Id = index;
		result->Size = 0;
		time(&result->TimeInfo.LastWrite);
		result->TimeInfo.LastRead = -1;
		if (type == FT_R)
		{
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
* \brief 将指定的绝对路径转换为文件控制块
* \param absPath 文件的绝对路径
* \return 绝对路径指向的文件的控制块
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
 * \brief 获取指定文件用字符标识的类型
 * \param fcb 文件控制块
 * \return 表示文件类型的字符
 */
char GetFileType(const Fcb* fcb)
{
	return fcb->Type == FT_D ? 'd' : '-';
}

/**
 * \brief 获取指定文件的绝对路径
 * \param path 文件的绝对路径
 * \param len 缓冲区长度
 * \param fcb 文件控制块
 */
void GetAbsolutePath(char* path, int len, const Fcb* fcb)
{
	memset(path, 0, len);
}

/**
 * \brief 获取一个空的逻辑记录
 * \return 一个空的逻辑记录
 */
LogicRecord* GetEmptyDiskBlock()
{
	ARRAYFIRST(short, Fat, FAT_CNT_MAX, *_it == FAT_AVALIABLE, index);
	if (index == -1)
	{
		return NULL;
	}
	Fat[index] = FAT_NULL;
	LogicRecord* record = malloc(sizeof(LogicRecord));
	record->Data = &Vhd[index * DISK_BLOCK_SIZE];
	record->BlockIndex = index;
	return record;
}

/**
 * \brief 将文本内容写入文件
 * \param fcb 文件控制块
 * \param content 要写入的内容
 */
void WriteString(Fcb* fcb, const char* content)
{
	int len = strlen(content);
	// 计算需要的磁盘块数
	int size = min(len, DISK_BLOCK_SIZE) * sizeof(char);
	int blockCnt = ceil(len / DISK_BLOCK_SIZE) + 1;
	LogicRecord* rec = GetEmptyDiskBlock();
	TRYCATCH(rec->Data == NULL, "Disk space is not enough");
	memcpy(rec->Data, content, size);
	fcb->Address = rec->BlockIndex;
	int it = fcb->Address;
	for (int i = 1; i < blockCnt; i++)
	{
		size = min(len - i*DISK_BLOCK_SIZE, DISK_BLOCK_SIZE) * sizeof(char);
		rec = GetEmptyDiskBlock();
		TRYCATCH(rec->Data == NULL, "Disk space is not enough");

		memcpy(rec->Data, content + (it - 20) * DISK_BLOCK_SIZE, size);
		Fat[it] = rec->BlockIndex;
		it = rec->BlockIndex;
	}
	fcb->Size = len;
	Fat[it] = FAT_NULL;
}

/**
 * \brief 读取文件的内容到字符串
 * \param fcb 文件控制块
 * \return 文件中保存的内容
 */
char* ReadString(const Fcb* fcb)
{
	char* result = malloc(sizeof(char)*fcb->Size + 1);
	short it = fcb->Address;
	int blockCnt = 0;
	while (it != FAT_NULL)
	{
		int size = min(fcb->Size - blockCnt * DISK_BLOCK_SIZE, DISK_BLOCK_SIZE) * sizeof(char);
		memcpy(result + blockCnt*DISK_BLOCK_SIZE, Vhd + it*DISK_BLOCK_SIZE, size);
		it = Fat[it];
		blockCnt++;
	}
	result[fcb->Size] = 0;
	return result;
}

/**
 * \brief 删除指定文件
 * \param fcb 文件控制块
 */
void DeleteFile(Fcb* fcb)
{
	Fcb* parent = FileCategory + fcb->Parent;
	int it = fcb->Address;
	// 清除磁盘块上的内容
	while (fcb->Size > 0 && it != FAT_NULL)
	{
		memset(Vhd + it * DISK_BLOCK_SIZE, 0, DISK_BLOCK_SIZE);
		short tmp = Fat[it];
		Fat[it] = FAT_AVALIABLE;
		it = tmp;
	}
	// 清除文件目录记录项
	it = parent->Child;
	if (it == fcb->Id)
	{
		parent->Child = FCB_NULL;
	}
	else
	{
		while (it != FCB_NAN)
		{
			if (FileCategory[it].Sibling == fcb->Id)
			{
				FileCategory[it].Sibling = fcb->Sibling;
				break;
			}
			it = FileCategory[it].Sibling;
		}
	}
	fcb->Id = FCB_NAN;
}