#include <stdio.h>

main(int argc, char *argv[])
{
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

// oldname(argv[1]) 에서 newname(argv[2])로 바꿈, 성공하면 0 반환
	if (rename(argv[1], argv[2]) < 0) { 
		perror("rename");
		exit(1);
	}
}
#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	char	buf[MAX_BUF];

// getcwd(현재 작업 디렉토리를 저장할 버퍼, 버퍼 메모리 크기)
	if (getcwd(buf, MAX_BUF) == NULL)  {
		perror("getcwd");
		exit(1);
	}

	printf("%s\n", buf);
}
