#include <stdio.h>
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
	printf("reply: %d\n", getpid());

	pInt = (int *)ptr;
	while ((*pInt) == 0) // 아직 안썼으므로 계속 대기, 최초에 Time Quantum을 다 써서 2로 Context Switching
		;

	pData = ptr + sizeof(int);

	printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	*pInt = 0;
	printf("Replied.\n");

	sleep(1); // sleep을 안하면 다른 프로세스가 응답을 받기도 전에 반환될 수도 있음

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { // 반환
		perror("shmctl");
		exit(1);
	}
}