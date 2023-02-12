#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int		Var = 6;
char	Buf[] = "a write to stdout\n";

main()
{
	int		var;
	pid_t	pid;

	var = 88;
// 버퍼 내용을 STDOUT_FILENO에 씀
// STDIN, STODUT, STDError은 미리 오픈을 따로 오픈을 안해도 가능
// 즉 printf와 똑같은 기능을 함
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1);
	printf("Before fork\n");

// 무한 프로세스 fork 하면 서버가 다운됨
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		/* child */
		Var++;
		var++;
	}
	else  {
		/* parent */
		sleep(2);
	}

// 자식 프로세스가 먼저 printf를 하고 부모는 2초뒤에 printf
// 자식은 89 7, 부모는 88 6
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}