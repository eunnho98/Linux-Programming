#include <stdio.h>
#include <signal.h>

void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int
mysleep(unsigned int nsecs)
{
	/* SIGALRM signal handler: SigAlarmHandler */
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return nsecs;
	}

	alarm(nsecs);
/*
만약 fork를 했는데 5초안에 자식프로세스가 종료되면 SIGCHD가 와서 pause()에서 받음
(원래 pause는 SIGALRM을 받아야함)
*/
	pause();

	return alarm(0);
  /*
  alarm(0): 이전의 알람부터 지금까지 시간이 얼마나 지났는지 리턴해줌
  정상적이면 5초뒤에 알람이 울리므로 5초가 지남 -> 0 리턴
  만약 3초에 자식프로세스가 종료되면 2리턴
  */
}

main()
{
	printf("Wait for 5 seconds...\n");

}