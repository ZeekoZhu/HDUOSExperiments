#pragma once
#ifndef _FILESYS_STRUCT
#define _FILESYS_STRUCT

#define DISK_BLOCK_SIZE 4096
#define DISKSIZE 4096 * 2048
#define FILE_CNT_MAX 1216
#define FAT_CNT_MAX 2048


/**
* FAT 记录大小 2B 最高位标识是否空白，其余标识磁盘块号
* 存放在 0 号磁盘块
* 共 2K 个记录
* 最多保存 8M 文件
*/

#define FAT_AVALIABLE -1
#define FAT_NULL -2

/**
* 文件目录表 记录大小 64B
* 存放在 1 ~ 19 号磁盘块
* 共 1216 个记录
* 最多保存 1216 个文件
*/

/**
* \brief 逻辑记录 4K
*/
typedef struct
{
	short BlockIndex;
	char* Data; //DISK_BLOCK_SIZE
} LogicRecord;

LogicRecord* GetEmptyDiskBlock();


/**
* \brief 文件日期信息 16B
*/
typedef struct
{
	/**
	* \brief 最后读取时间
	*/
	long long LastRead;
	/**
	* \brief 最后写入时间
	*/
	long long LastWrite;
} TimeInfo;


/**
* \brief 文件控制块 64B
*	34B		1B		1B		16B		4B		2B		2B		2B		2B
*	Name	Type	Mode	Time	Length	Address	Sibling	Child	Parent
*/
typedef struct
{
	/**
	* \brief 文件名
	*/
	char FileName[34];

	/**
	* \brief 文件标识号，直接采用数组下表确定
	*/
	int Id;

	/**
	* \brief 文件类型
	*/
	char Type;
	/**
	* \brief 文件许可权
	*/
	char Mode;
	/**
	* \brief 文件存取时间
	*/
	TimeInfo TimeInfo;
	/**
	* \brief 文件长度
	*/
	int Size;
	/**
	* \brief 文件起始磁盘号
	*/
	short Address;
	/**
	* \brief 下一个兄弟节点记录号
	*/
	short Sibling;
	/**
	* \brief 第一个子节点
	*/
	short Child;
	/**
	 * \brief 所在目录
	 */
	short Parent;
} Fcb;

char GetReadMode(const Fcb* fcb);
char GetWriteMode(const Fcb* fcb);
void GetLastWriteStr(const Fcb* fcb, char* res, int len);
void GetLastReadStr(const Fcb* fcb, char* res, int len);
void FcbToString(const Fcb* fcb, char* res, int len);
Fcb* CreateFile(const char* fileName, char mode, char type);
Fcb* FindChild(Fcb* parent, const char* childName);
Fcb* ParsePath(const char* absPath);
char GetFileType(const Fcb* fcb);
void GetAbsolutePath(char* path, int len, const Fcb* fcb);
void WriteString(Fcb* fcb, const char* content);
char* ReadString(const Fcb* fcb);
void DeleteFile(Fcb* fcb);

/**
* \brief 普通文件
*/
#define FT_R 0
/**
* \brief 目录文件
*/
#define FT_D 1

/**
* R - read W - write X - execute
*/
#define FM_X 1
#define FM_W 2
#define FM_R 4


#define FCB_NULL 0
#define FCB_NAN -1


Fcb* root;
Fcb* cwd;
Fcb FileCategory[FILE_CNT_MAX];
short Fat[FAT_CNT_MAX];
char Vhd[DISKSIZE];
char cwdPath[1000];

#endif // !FILESYS_STRUCT
