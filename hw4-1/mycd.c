#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (chdir(argv[1]) < 0)  {
		perror("chdir");
		exit(1);
	}
}

// ./mycd ..가 안되는 이유 : fork와 관련
// fork한 프로세스에서 cd가 되고 종료되므로 부모 프로세스에서는 경로가 안바뀜