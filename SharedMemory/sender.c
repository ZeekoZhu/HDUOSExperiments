#include <fcntl.h> 
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include "common.h"

void Dispose()
{
	sem_close(Ssnd);
	sem_close(Srcv);
	sem_unlink("Ssnd");
	sem_unlink("Srcv");
}

void Sender()
{
	char input[SHMSIZE];
	// get input str
	fgets(input, SHMSIZE - 1, stdin);
	memcpy(ShmPtr, input, SHMSIZE);
	sem_post(Srcv);
	sem_wait(Ssnd);
	memset(input, 0, sizeof(char) * SHMSIZE);
	memcpy(input, ShmPtr, 4);
	printf("%s", input);
	close(ShmId);
	shm_unlink(SHMNAME);
	TRYCATCH(munmap(ShmPtr, SHMSIZE)==-1,"Deallocate shared memory faild");
}

int main()
{
	Init();
	Sender();
	Dispose();
	return 0;
}