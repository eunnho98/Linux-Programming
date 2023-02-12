#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	struct stat	statbuf;

	if (stat("bar", &statbuf) < 0)  { // 파일의 status를 읽어옴
		perror("stat");
		exit(1);
	}

// (statbuf.st_mode & ~S_IWGRP) 현재 모드와 Group Write의 bit 전체를 뒤집은 것을 비트AND 연산 -> Group Write 모드를 삭제하는 것
// | 로 연결하면 그 모드를 키겠다라는 의미
	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0)  {
		perror("chmod");
		exit(1);
	}

	/* S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH : ON */
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH < 0))  {
		perror("chmod");
		exit(1);
	}
}
