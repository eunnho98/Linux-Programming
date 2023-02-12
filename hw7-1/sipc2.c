#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"


main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;


	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}
	printf("request: %d\n", getpid());

	pInt = (int *)ptr;
	pData = ptr + sizeof(int); // flag 다음부터 가리킴
	sprintf(pData, "This is a request from %d.", getpid());

	*pInt = 1; // flag
	printf("Sent a request.....");

	while ((*pInt) == 1)
		;

	printf("Received reply: %s\n", pData);
}
