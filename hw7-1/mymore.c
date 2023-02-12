#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
pipe를 만들고 fd[0], fd[1]을 STDIN과 STDOUT으로 duplicate를 함
ls -l은 STDOUT인데 fd[1]로 dup을 해서 그 결과가 pipe에 쓰여지고
fd[0]으로 pipe에서 읽어와 more의 input 값으로 pipe의 쓰여진, ls -l이 됨
*/
int 
main() 
{
	int fd[2];
	pid_t child;

	if(pipe(fd) == -1)  {
		perror("pipe");
		exit(1);
	}
 
	child = fork();
	if(child == (pid_t)(-1))  {
    perror("fork");
		exit(1);
	}
	else if(child == (pid_t)0) {
    close(1);
		close(fd[0]);
        	
    if(dup(fd[1]) == -1)  { // 사용하지 않는 fild descriptor와 duplicate
      perror("dup");        // close(1)을 했으므로 fd[1]을 STDOUTPUT과 dup
			exit(1);              // 출력 결과가 콘솔이 아닌 fd[1]이 가리키는 pipe에 쓰여짐
		}

    if((execlp("ls", "ls", "-l", NULL)) == -1)  {
      perror("execlp");
			exit(1);
		}
	} 
	else {
    close(0);
		close(fd[1]);
        	
    if(dup2(fd[0], 0) == -1)  { // dup2는 그냥 지정을 함, 이때도 0번은 close를 해야함
      perror("dup");            // 0을 fd[0]으로 복사 -> 입력을 STDIN이 아닌 fd[0]을 통해 받아오겠다.
			exit(1);
		}

    if((execlp("more", "more", NULL)) == -1)  {
      perror("execlp");
			exit(1);
		}
	}
 
	return 0;
}