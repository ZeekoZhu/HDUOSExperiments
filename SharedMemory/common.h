#ifndef MY_COMMON
#pragma once
#include <fcntl.h> 
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include "Utils.h"
#define MY_COMMON
#define SHMSIZE _SC_PAGESIZE * 10	// �����ڴ��С������Ϊϵͳ�ڴ�ҳ���С���������Լ����˷�
#define SHMNAME "/myshm"
sem_t* Ssnd;	// ������Ϣ�õ��ź���
sem_t* Srcv; // ������Ϣ�õ��ź���

int ShmId;
void* ShmPtr;

void Init();
#endif
