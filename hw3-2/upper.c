#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h> 

#define MAX_BUF 1024

main(int argc, char* argv[]) {
  FILE *f; // FILE 구조체 포인터 
  int ch; // 문자의 아스키코드를 읽음
  
  if(argc != 2) { // 이 실행파일은 파라미터가 자신빼고 하나 필요
    fprintf(stderr, "Usage: %s source", argv[0]);
    exit(1);
  }

  if((f = fopen(argv[1], "rt")) == NULL) { // read text file 모드
    perror("fopen");
    exit(1);
  }

  while((ch = fgetc(f)) != EOF) { // 파일의 끝까지 문자 하나씩 읽음
    printf("%c", toupper(ch)); // 대문자로 바꾸어서 출력
  }

  fclose(f);
}