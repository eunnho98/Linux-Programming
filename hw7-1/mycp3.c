// using memory-mapped file
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int argc, char *argv[])
{
	int			fdin, fdout;
	char		*src, *dst;
	struct stat	statbuf;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0)  { // 열린 파일의 상태정보를 얻음
		perror("fstat");
		exit(1);
	}

// fdout은 현재 빈 파일이므로 가상으로 만들어 놓기 위한 코드
// 시작점부터 statbuf.st_size -1만큼 이동하여 \0을 쓰면 그 앞도 메모리에서 임의로 \0으로 채움 -> st_size만큼의 virtual한 파일이 생김
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1); // null string은 1바이트, \0이 있으므로

	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}

	// *src부터 statbuf.st_size만큼 *dst부터 복사
	// 그러면 fdin에서 읽어 fdout에 쓰는 것이됨, mmap에서 src는 fdin과, dst는 fdout과 매핑을 해서
	memcpy(dst, src, statbuf.st_size);

	close(fdin);
	close(fdout);
}