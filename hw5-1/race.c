// wait with race condition
// 공유 자원을 서로 쓰려는 상황
// 공유 자원의 상태가 비정상적으로 됨
// 비정상적인 상태를 critical section problem (synchronization problem)
// 보호를 위해 lock, semaphore, monitor가 있음
// race condition이 안일어나려면 공유자원을 순차적으로 써야함

// 여기선 shared resource가 콘솔, 서로 콘솔에 메세지를 보내려고함
// loop 999999..를 돌리는 과정에서 자신의 time quantum을 다 쓰기 때문에
// 그러면 context switch가 일어나서 다른 프로세스가 실행
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
		putc(c, stdout);
	}
}

main()
{
	pid_t	pid;

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		CharAtaTime("output from child\n");
	} 
	else  {
		CharAtaTime("output from parent\n");
	}
}