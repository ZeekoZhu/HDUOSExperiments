#include "common.h"
#include "Utils.h"

void Init()
{
	ShmPtr = NULL;
	Ssnd = sem_open("/Ssnd", O_CREAT, O_RDWR, 0);
	TRYCATCH(Ssnd == SEM_FAILED, "Init sem send faild");
	Srcv = sem_open("/Srcv", O_CREAT, O_RDWR, 0);
	TRYCATCH(Srcv == SEM_FAILED, "Init sem rcv faild");

	ShmId = shm_open(SHMNAME, O_RDWR | O_CREAT, 0666);
	TRYCATCH(ShmId == -1, "Can not open shared memory");
	TRYCATCH(ftruncate(ShmId, SHMSIZE) == -1, "Arrange memory size faild");
	ShmPtr = mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, ShmId, 0);
	TRYCATCH(ShmPtr == MAP_FAILED, "Map memory faild");
}
