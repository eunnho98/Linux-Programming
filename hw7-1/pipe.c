// pipe.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

main()
{
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	if (pipe(fd) < 0)  {
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  { // fork를 하면 부모, 자식 모두 ready 상태
		perror("fork");          // 스케줄링 알고리즘에 의해 부모나 자식이 먼저 실행됨
		exit(1);
	}
// 자식이 먼저 실행될 때
	else if (pid == 0)  {
		close(fd[1]); // 사용하지 않으므로 close
		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd[0], buf, MAX_BUF)) < 0)  { // I/O이므로 OS가 실행됨 & pipe가 비어있
			perror("read");                           // 으므로 waiting 상태가 됨
			exit(1);  
		}
		printf("Child : Received data from parent: ");
    // fflush로 버퍼에있는 값을 콘솔에 출력하고 버퍼를 비움
		fflush(stdout);
		// STDOUT에 write를 하면 printf와 같은 역할
		// n bytes를 읽었으므로 n bytes를 쓰기 위해 write로 출력을 했다함
		write(STDOUT_FILENO, buf, n);
	}
	else  { 
		close(fd[0]); // 사용하지 않으므로 close
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		write(fd[1], buf, strlen(buf)+1); // 부모가 write를 하고 종료됨
	}

	exit(0);
}