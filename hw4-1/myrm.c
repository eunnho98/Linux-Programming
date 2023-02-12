#include <stdio.h>

main(int argc, char *argv[])
{
	int		i;

	if (argc == 1)  {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	for (i = 1 ; i < argc ; i++)  {
		if (remove(argv[i]) < 0)  { // argv[i] (경로)에 대한 파일 삭제, 성공하면 0 반환
			perror("remove");
			exit(1);
		}
	}
}
