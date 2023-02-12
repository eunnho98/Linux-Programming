#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int mysystem(const char *cmd) {
  pid_t pid;
  int status;

// -c를 붙이면 뒤에 나온 string이 실행 파일이 아니라 실행 코드가 됨
  char *argv[] = {"sh", "-c", cmd, (char*)0};
  if((pid = fork()) < 0) {
    status = -1; // error
    perror("fork error");
  }
  else if (pid == 0) { // child
    execv("/bin/sh", argv); // /bin/sh에 터미널 명령어가 있는 듯
  }
  else { // parent
    waitpid(pid, NULL, 0);
  }

  return status;
}

void
main() {
  int status;
  char cmd[100];
  while(1) {
    // q to exit
    fgets(cmd, 100, stdin);
    if(cmd[0] == 'q')
      break;
    status = mysystem(cmd);
    printf("exit status = %d\n", status);
  }
}
