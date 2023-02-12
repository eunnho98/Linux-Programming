#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int	Pfd1[2], Pfd2[2];
// Pfd1: 부모프로세스가 자식프로세스에게 
// Pfd2: 자식프로세스가 부모프로세스에게

/*
pipe를 만들고 pipe에 데이터를 쓰면 CPU 권한을 넘겨주는 것
pipe에서 데이터를 읽으면 그 프로세스가 CPU를 잡고 동작을 함
*/
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