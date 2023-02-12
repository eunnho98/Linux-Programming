#include <stdio.h>
#include <signal.h>
#include <pwd.h>

/*
getpwnam(*pw) {
	statis struct passwd pw;
	...
	return &pw;
}
처음엔 pw가 cjs인데 알람때문에 핸들러가 실행, 이땐 pw가 root
root 퀀텀이 끝나 다시 cjs에서 중단된 곳부터 실행하는데 이때 pw가 root로 된 상태임
-> 다 빠져나오지 않은 상태에서 다시 들어가서 문제 발생
*/

void
MyAlarmHandler(int signo)
{
	struct passwd	*rootptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1);

	printf("in signal handler\n");

	if ((rootptr = getpwnam("root")) == NULL)  {
		perror("getpwnam");
		exit(1);
	}

	return;
}

main()
{
	struct passwd	*ptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1);

	for ( ; ; )  {
		if ((ptr = getpwnam("cjs")) == NULL)  {
			perror("getpwnam");
			exit(1);
		}

		if (strcmp(ptr->pw_name, "cjs") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
}

