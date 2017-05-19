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
	short Next;
	char Data[DISK_BLOCK_SIZE - 4];
} LogicRecord;

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
*	36B		1B		1B		16B		4B		2B		2B		2B
*	Name	Type	Mode	Time	Length	Address	Sibling	Child
*/
typedef struct
{
	/**
	* \brief �ļ���
	*/
	char FileName[36];

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
} Fcb;

#define FCB_NULL 0
#define FCB_NAN -1




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

#endif // !FILESYS_STRUCT
