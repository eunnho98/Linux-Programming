#include <stdio.h>
#include <signal.h>

void
SigUsrHandler(int signo)
{
	if (signo == SIGUSR1)  {
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2)  {
		printf("Received a SIGUSR2 signal\n");
	}
	else  {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

main()
{
	/* SIGUSR1 signal handler: SigUsrHandler */
  // 커스텀 signal 전달
  // 터미널에서 kill -USR1 pid하면 pid 프로세스로 SIGUSR1 시그널 전달
  // pid를 알려면 ps -ef | grep sig2 / ps -u username
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	for ( ; ; )
		pause();
}