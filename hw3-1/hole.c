#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int 	fd;
	char	ch = '\0';

  // file.hole이라는 user이 생성한 read mode 파일 생성
	if ((fd = creat("file.hole", 0400)) < 0)  {
		perror("creat");
		exit(1);
	}

  // 포인터를 처음으로 옮기고 1023바이트만큼 오른쪽으로 이동
  // 만약 EOF라면 \0으로 채움
	if (lseek(fd, 1023, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
  // file description에 ch(버퍼) 1바이트를 써넣음
	write(fd, &ch, 1);

	close(fd);
}