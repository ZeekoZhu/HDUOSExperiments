#ifndef MY_COMMON
#pragma once
#include <fcntl.h> 
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include "Utils.h"
#define MY_COMMON
#define SHMSIZE _SC_PAGESIZE * 10	// 共享内存大小，设置为系统内存页面大小的整数倍以减少浪费
#define SHMNAME "/myshm"
sem_t* Ssnd;	// 发送消息用的信号量
sem_t* Srcv; // 接受消息用的信号量

int ShmId;
void* ShmPtr;

void Init();
#endif
