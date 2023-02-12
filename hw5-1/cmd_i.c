#include <stdio.h>

#define	MAX_CMD		256

// 어떤 명령어가 시간이 많이 걸리면 그게 다 처리될 때까지 대기해야함
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

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		DoCmd(cmd);
	}
}