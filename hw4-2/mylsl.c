#include <stdio.h> //std io
#include <sys/types.h> //fork
#include <stdlib.h> //char *
#include <pwd.h> //passwd
#include <dirent.h> //dirent
#include <grp.h> //g
#include <sys/stat.h> //stat
#include <time.h> //tm

/* 파일의 종류와 접근권한을 표시 */
void
permission(mode) {
  if(S_ISDIR(mode))  // 디렉토리
    printf("d");
  else  // 파일
    printf("-");

// USER Permission
  if(mode & S_IRUSR) 
    printf("r");
  else 
    printf("-");
  if(mode & S_IWUSR)
    printf("w");
  else 
    printf("-");
  if(mode & S_IXUSR)
    printf("x");
  else 
    printf("-");

  // GROUP Permission
  if(mode & S_IRGRP) 
    printf("r");
  else 
    printf("-");
  if(mode & S_IWGRP)
    printf("w");
  else 
    printf("-");
  if(mode & S_IXGRP)
    printf("x");
  else 
    printf("-");

  // OTHER Permission
  if(mode & S_IROTH) 
    printf("r");
  else 
    printf("-");
  if(mode & S_IWOTH)
    printf("w");
  else 
    printf("-");
  if(mode & S_IXOTH)
    printf("x");
  else 
    printf("-");

}

void JustDoit(char *path){
   struct passwd *pwd; // uid
   DIR *dp; // 현재 디렉토리
   struct dirent *dep; 
   struct group *g; //gid
   struct stat statbuf; 
   mode_t mode; //파일의 종류 및 모드
   struct tm *time_info; // 파일 최종 수정 시간 출력
   char buf[100]; // 파일 수정 시간 저장
   int total = 0; // total값 누적

   uid_t uid = 1012;
   gid_t gid = 1050;

   if((dp =opendir(path)) == NULL) {
    perror("opendir");
    exit(1);
   }

   while(dep = readdir(dp)){
      if(stat(dep->d_name,&statbuf)==-1){ // 현재 경로를 읽어오지 못하면 에러
        perror("Error : ");
        exit(0);  
       }

      /* 경로가 . 또는 ..이면 넘어감 */ 
      if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
        continue;

    /* 파일의 모드를 출력 */
      mode=statbuf.st_mode;
      permission(mode);

      /* 파일의 링크수 출력 */
      printf("  %d  ", statbuf.st_nlink);

      /* 파일의 소유자와 그룹을 */
      pwd=getpwuid(uid);
      g=getgrgid(gid);
      printf("%s", pwd->pw_name);
      printf(" %s ", g->gr_name);

      /* 파일 사이즈*/
      printf(" %d ", statbuf.st_size);
      total += statbuf.st_size;

      /* 파일의 생성 시간*/
      time_info=localtime(&(statbuf.st_mtime));
      strftime(buf, sizeof(buf), "%b %d %H:%M ",time_info);
      printf(" %s ", buf);

      /* 파일 이름을 출력 */
      printf(" %s\n", dep->d_name);
   }
   printf("total %d\n", total/1000);
   closedir(dp);
}

main() {
  JustDoit(".");
}

