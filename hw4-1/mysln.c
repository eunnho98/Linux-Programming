#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

// argv[2]가 argv[1]에 대한 symbolic link, 성공하면 0, 실패하면 -1 반환
	if (symlink(argv[1], argv[2]) < 0)  {
		perror("link");
		exit(1);
	}
}
