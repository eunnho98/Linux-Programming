#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h" // SYSTEM-V를 POSIX처럼 사용하기 위해
#include "prodcons.h"


main()
{
	int shmid;
	int reqMutexSemid, resMutexSemid; // request할 때와 response할 때의 semaphore
	char *pData; // shared memory를 가리키는 포인터

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((pData = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	if ((reqMutexSemid = semInit(REQ_MUTEX_SEM_KEY)) < 0)  { // Semaphore 초기화
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if ((resMutexSemid = semInit(RES_MUTEX_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}	

// sipc2에서 데이터를 다 쓰는 것을 기다림
	if(semWait(reqMutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	printf("Received request: %s.....\n", pData);
	sprintf(pData, "This is a reply from %d", getpid());
	printf("Replied\n");

// sipc2가 wait을 빠져나올 수 있도록
	if(semPost(resMutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}	

// req Semaphore을 다 써서 지움
	if (semDestroy(reqMutexSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}

// shared memory를 다 써서 해제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}	
}
