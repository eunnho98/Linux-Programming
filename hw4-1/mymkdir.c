#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	/* 755 permission */
	// argv[1] 경로에 755 (rwxr_xr_x) 권한인 폴더 생성
	if (mkdir(argv[1], 0755) < 0)  { 
		perror("mkdir");
		exit(1);
	}
}
