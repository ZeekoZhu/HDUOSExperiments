#pragma once
#ifndef _FILESYS_STRUCT
#define _FILESYS_STRUCT

#define DISK_BLOCK_SIZE 4096
#define DISKSIZE 4096 * 2048
#define FILE_CNT_MAX 1216
#define FAT_CNT_MAX 2048


/**
* FAT ��¼��С 2B ���λ��ʶ�Ƿ�հף������ʶ���̿��
* ����� 0 �Ŵ��̿�
* �� 2K ����¼
* ��ౣ�� 8M �ļ�
*/

#define FAT_AVALIABLE -1
#define FAT_NULL -2

/**
* �ļ�Ŀ¼�� ��¼��С 64B
* ����� 1 ~ 19 �Ŵ��̿�
* �� 1216 ����¼
* ��ౣ�� 1216 ���ļ�
*/

/**
* \brief �߼���¼ 4K
*/
typedef struct
{
	short BlockIndex;
	char* Data; //DISK_BLOCK_SIZE
} LogicRecord;

LogicRecord* GetEmptyDiskBlock();


/**
* \brief �ļ�������Ϣ 16B
*/
typedef struct
{
	/**
	* \brief ����ȡʱ��
	*/
	long long LastRead;
	/**
	* \brief ���д��ʱ��
	*/
	long long LastWrite;
} TimeInfo;


/**
* \brief �ļ����ƿ� 64B
*	34B		1B		1B		16B		4B		2B		2B		2B		2B
*	Name	Type	Mode	Time	Length	Address	Sibling	Child	Parent
*/
typedef struct
{
	/**
	* \brief �ļ���
	*/
	char FileName[34];

	/**
	* \brief �ļ���ʶ�ţ�ֱ�Ӳ��������±�ȷ��
	*/
	int Id;

	/**
	* \brief �ļ�����
	*/
	char Type;
	/**
	* \brief �ļ����Ȩ
	*/
	char Mode;
	/**
	* \brief �ļ���ȡʱ��
	*/
	TimeInfo TimeInfo;
	/**
	* \brief �ļ�����
	*/
	int Size;
	/**
	* \brief �ļ���ʼ���̺�
	*/
	short Address;
	/**
	* \brief ��һ���ֵܽڵ��¼��
	*/
	short Sibling;
	/**
	* \brief ��һ���ӽڵ�
	*/
	short Child;
	/**
	 * \brief ����Ŀ¼
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
* \brief ��ͨ�ļ�
*/
#define FT_R 0
/**
* \brief Ŀ¼�ļ�
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
