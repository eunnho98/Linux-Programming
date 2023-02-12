#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

// argv[1] 경로 폴더 삭제, 성공하면 0
	if (rmdir(argv[1]) < 0)  {
		perror("rmdir");
		exit(1);
	}
}
