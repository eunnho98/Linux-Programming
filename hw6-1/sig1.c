#include <stdio.h>
#include <signal.h>

void // signal 핸들러
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}

main()
{
	/* SIGINT signal handler: SigIntHandler */
  // SIGINT: 컨트롤C하면 발생하는 시그널
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();
}