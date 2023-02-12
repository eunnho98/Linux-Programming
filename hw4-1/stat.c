#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	struct stat	statbuf; 
	char		*mode;
	int			i;

	for (i = 1 ; i < argc ; i++)  { // 입력한 모든 파일들에 대해 수행
		printf("%s: ", argv[i]);
		if (lstat(argv[i], &statbuf) < 0)  { // error 발생시 lstat은 -1 반환
			perror("lstat");
			continue;
		}
		if (S_ISREG(statbuf.st_mode)) // stat의 type을 반환
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";
		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode); // type과 mode
		printf("\tst_ino = %d\n", statbuf.st_ino); // i-node 번호
		printf("\tst_dev = %d\n", statbuf.st_dev); // 이 파일에 저장된 디바이스 번호
		printf("\tst_rdev = %d\n", statbuf.st_rdev);
		printf("\tst_nlink = %d\n", statbuf.st_nlink); // link의 수
		printf("\tst_uid = %d\n", statbuf.st_uid); // 유저 ID
		printf("\tst_gid = %d\n", statbuf.st_gid); // 그룹 ID
		printf("\tst_size = %d\n", statbuf.st_size); // 이 파일의 크기 저장
		printf("\tst_atime = %d\n", statbuf.st_atime); // 마지막으로 파일 접근된 시간
		printf("\tst_mtime = %d\n", statbuf.st_mtime); // 마지막으로 수정된 시간
		printf("\tst_ctime = %d\n", statbuf.st_ctime); // 마지막으로 파일 status가 변경된 시간
		printf("\tst_blksize = %d\n", statbuf.st_blksize); // 하드디스크일 때만 사용
		printf("\tst_blocks = %d\n", statbuf.st_blocks); // 블럭의 개수
	}
}
