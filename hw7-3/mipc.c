// mipc.c 를 shared memory system call을 사용하도록 변경
// [Hint] open, mmap 대신 shmget, shmat를 사용 

#define	MY_ID		1212
#define	SHM_KEY		(0x9000 + MY_ID)
#define	SHM_SIZE	1024
#define	SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)
#define	NLOOPS	10
#define	SIZE	sizeof(long)

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "synclib.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static int	Pfd1[2], Pfd2[2]; // pipe 생성
void
TELL_WAIT(void)
{
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0)  {
		perror("pipe");
		exit(1);
	}
}

void
TELL_PARENT(void)
{
	if (write(Pfd2[1], "c", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

void
WAIT_PARENT(void)
{
	char	c;

	if (read(Pfd1[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'p')  {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

void
TELL_CHILD(void)
{
	if (write(Pfd1[1], "p", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

void
WAIT_CHILD(void)
{
	char	c;

	if (read(Pfd2[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'c')  {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
int
update(long *ptr)
{
	return ((*ptr)++);
}

void
main()
{
  int shmid, counter, i;
  char *ptr;
  int *pint;
  pid_t	pid;

// shared memory 생성
  if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}

// shared memoy mapping
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

  pint = (int *)ptr;

  TELL_WAIT();

 	if ((pid = fork()) < 0)  { 
		perror("fork");
		exit(1);
	}

  else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			if ((counter = update((long *) pint)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD(); // Child에게 하라고 넘김
			WAIT_CHILD(); // Child작업 끝나기까지 기다림
		}
	}
	else  {
		for (i = 1 ; i < NLOOPS ; i += 2)  {
			WAIT_PARENT(); // Parent작업 끝나기까지 기다림
			if ((counter = update((long *) pint)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT(); // Parent에게 하라고 넘김
		}
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { // shared memory 반환
		perror("shmctl");
		exit(1);
	}
}