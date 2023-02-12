// shared memory가 내 virtual space 어디에 할당되어 있는가?
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE];

main()
{
	int		shmid;
	char	*ptr, *shmptr;

	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  {
		perror("malloc");
		exit(1);
	}

// IPC_PRIVATE: 이 Shared memory는 이 프로세스만 쓰겠다 -> 이 프로세스를 종료하면 자동으로 반환됨, IPC_RMID할 필요가 없다.
// 이 프로세스에서만 쓰므로 사실상 IPC가 아니지만 Fork를 하면 똑같은 프로세스가 생성되므로 Shared memory 사용 가능
	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]); // 전역변수, data segment에 저장
	printf("Stack around %p\n", &shmid); // 지역변수, stack에 저장
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE); // 동적할당한 메모리, heap에 저장
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE); // shared memory는 heap과 stack 사이에 저장

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { // shared memory 반환
		perror("shmctl");
		exit(1);
	}
}