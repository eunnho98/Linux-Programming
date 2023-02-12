#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

main()
{
	DIR				*dp;
	struct dirent	*dep;

	if ((dp = opendir(".")) == NULL)  { // 현재 디렉토리 열기
		perror("opendir");
		exit(0);
	}

// readdir을 호출할 때마다 하나의 파일 엔트리가 넘어옴, 더 없으면 NULL 반환
	while (dep = readdir(dp))  {
		printf("%s\n", dep->d_name);
	}
	
	closedir(dp);
}
