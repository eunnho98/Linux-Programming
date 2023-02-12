// sync.c
#include <stdio.h>
#include "synclib.h"

#define	NLOOPS	5


main()
{
	int		i;
	pid_t	pid;

	TELL_WAIT(); // 라이브러리 함수, 라이브러리 초기화 목적 & 파이프 생성
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  { // parent
		for (i = 0 ; i < NLOOPS ; i++)  {
		  /* Do Something */
			TELL_CHILD(); // 부모 동작을 다 해서 Child에게 작업을 하라고 말해줌
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD();
		}
	}
	else  { // Child
		for (i = 0 ; i < NLOOPS ; i++)  {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT();
			/* Do Something */
			TELL_PARENT(); // Parent에게 나 다했으니 너 하라고 알려줌
			printf("Child: Tell to parent\n");
		}
	}
}