#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH	256

void // ls -R, 하위 디렉토리까지 출력
JustDoIt(char *path)
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL)  {
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n", path); // 현재 path 출력
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) // 경로가 . 또는 ..이면 pass
			continue;
		printf("%s\n", dep->d_name);
	}
	// 여기까지 ls

	rewinddir(dp); // 커서를 맨 처음으로 이동
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name); // fullpath = ./d_name
		if (lstat(fullpath, &statbuf) < 0)  { // lstat: 경로 자체를 읽어옴
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode))  { // 디렉토리라면
			JustDoIt(fullpath);
		}
	}
	
	closedir(dp);
}

main()
{
	JustDoIt(".");
}
