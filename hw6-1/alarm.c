#include <stdio.h>
#include <signal.h>

static unsigned int	AlarmSecs;
// 어쩔 수 없이 전역변수를 쓸 땐 static을 붙임
// 이 파일 외 다른 파일에서는 이 변수를 못쓰게됨

void
SigAlarmHandler(int signo)
{
  // 다시 핸들러를 등록하는 코드, 리눅스에선 필요없음
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	alarm(AlarmSecs);

	/* Do something */
	printf(".");
	fflush(stdout);
  // fflush를 주석처리하면 버퍼캐시에만 써짐, 실제로 출력이 안됨
  // printf는 콘솔에다 쓰므로 fflush로 stdout 버퍼를 비우고
  // 그 내용을 쓰라고 알려줘야함
	return;
}

int
SetPeriodicAlarm(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return -1;
	}

	AlarmSecs = nsecs;

	alarm(nsecs);

	return 0;
}

main()
{
	printf("Doing something every one seconds\n");
// 1초마다 SetPeriodicAlram을 수행
	SetPeriodicAlarm(1);

	for ( ; ; )
		pause();
}