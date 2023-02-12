#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

main()
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		printf("I'm a child\n");
		sleep(2);
	}
	else  {
		wait(&status);
		printf("A child killed with %d status\n", status);
	}
}
// 부모가 먼저 실행되면 자식이 아직 종료를 안했으므로 wait에서 대기
// 자식이 먼저 실행하면 순차적