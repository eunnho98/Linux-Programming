// 두 파일을 열고 한 파일 내용을 다른 파일에 쓰기
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define	MAX_BUF	1024

// argc: argument 개수, argv: 경로
main(int argc, char *argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];

	if (argc != 3)  {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

// Read only
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}

// == int create
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}

// 지정한 file description에 대해 버퍼에다 MAX_BUF만큼 읽기
	while ((count = read(fd1, buf, MAX_BUF)) > 0)  {
		write(fd2, buf, count); // 지정한 file description에다 버퍼에서 count만큼 쓰기
	}

	close(fd1);
	close(fd2);
}