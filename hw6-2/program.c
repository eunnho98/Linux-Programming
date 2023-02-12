#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Handler
void
sigHandler(int signo) {
  printf("Child Process is terminated.\n");
  exit(0);
}

main()
{
	pid_t	pid;
	int		status;

// 자식프로세스가 종료되면 SIGCHLD signal이 보내짐
    if(signal(SIGCHLD, sigHandler) == SIG_ERR) {
      perror("signal");
      exit(1);
    }
  
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		printf("I'm a child\n");
		sleep(2);
	}
	else  {
		while(1); // 부모 프로세스는 계속 자기 일을 함 -> 비동기적
		printf("A child killed with %d status\n", status);
	}
}
