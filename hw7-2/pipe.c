#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

main()
{
	int		n, fd[2], cfd[2]; // cfd: child -> parent
	pid_t	pid;
	char	buf[MAX_BUF];

	if (pipe(fd) || pipe(cfd) < 0)  {
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}

	else if (pid == 0)  {
		close(fd[1]); // fd: 부모 -> 자식, Child에서는 write인 fd[1]이 필요없음
    close(cfd[0]);  // cfd: 자식 -> 부모, Child에서는 read인 cfd[0]이 필요없음
    strcpy(buf, "Hi, World!\n");
    printf("자식프로세스가 부모프로세스에게 데이터 보냄\n");
    write(cfd[1], buf, strlen(buf)+1);

		printf("자식프로세스가 부모프로세스 데이터 대기\n");
		if ((n = read(fd[0], buf, MAX_BUF)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout); 
		write(STDOUT_FILENO, buf, n);
	}
	else  {
		close(fd[0]);
    close(cfd[1]);
		strcpy(buf, "Hello, World!\n");
		printf("부모프로세스가 자식프로세스에게 데이터 보냄\n");
		write(fd[1], buf, strlen(buf)+1);

    printf("부모프로세스가 자식프로세스 데이터 대기\n");
    if((n = read(cfd[0], buf, MAX_BUF)) < 0) {
      perror("read");
      exit(1);
    }
    printf("Parent : Received data from child: ");
    fflush(stdout);
    write(STDERR_FILENO, buf, n);
	}

	exit(0);
}