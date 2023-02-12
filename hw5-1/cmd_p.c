#include <stdio.h>

#define	MAX_CMD		256

void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		if ((pid = fork()) < 0)  {
			perror("fork");
			exit(1);
		}
		else if (pid == 0)  { // child
			DoCmd(cmd);
			exit(0);
		}
#if 0 // 컴파일이 안됨
		else  {
			wait(NULL);
		}
#endif
	}
}

// #if 0때문에 fork로 자식프로세스는 작업을 수행하고 부모는 바로 올라와 새로운
// 입력을 기다림, #if 1로 컴파일이 되면 자식프로세스가 종료될 때까지 부모가 wait
// iteration 버전이랑 똑같이됨