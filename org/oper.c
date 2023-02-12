scanf("%c %s", &c, s);
// cin과 같음, 주소값을 줘야함

int strlen(char *s);
// 문자열 길이 반환

char* strcpy(char* dest, char* src)
// string copy

char* strcat(char* dst, char src);
// dst에 src를 붙임

int strcmp(char* s1, char* s2);
// 문자열 비교

int atoi(char* s);
// 문자열을 정수로

int toupper(int c);
// <ctype.h> 포함 

// ========================================================================

int open(char* pathname, int oflag, ... /* */);
// file descriptor 반환, -1이면 error

int close(int fd)
// -1이면 error

int read(int fd, void* buf, int nbytes);
// fd 내용을 nbytes만큼 buf에 씀
char buf[100]; count = read(fd1, buf, 100);

int write(int fd, void* buf, int nbytes);
// fd에 buf 내용을 nbytes만큼 씀

int lseek(int fd, int offset, int whence);
// fd의 커서를 whence 기준으로 offset만큼 옮김
fseek와 동일

int creat(char* pathname, mode_t mode);
// pathname에 mode로 파일 생성

// ========

FILE* fopen(char* pathname, char* type);
// pathname을 type으로 읽고 FILE 구조체 반환, type = "rt" 등

int fclose(FILE* fp);
// 0이면 닫김

int getc(FILE* fp) or int fgetc(FILE* fp);
// 문자열 받아옴

int putc(int ch, FILE* fp) or int fputc(int ch, FILE* fp);
// 문자열을 fp에 씀

char* fgets(char* buf, int max, FILE* fp);
// buf에 최대 max만큼 fp에서 읽어온 것 담음, '\n'까지

int fputs(char* str, FILE* fp);
// 음이 아닌 정수면 ok

int fread(void* ptr, int size, size nobj, FILE* fp);
// ptr: 파일에서 읽어온 것을 저장할 block을 가리키는 포인터
// size: block의 사이즈
// nobj: 개수 -> size * nobj만큼 bytes를 읽음
char buf[100]; count = fread(buf, 1, 100, src);

int fwrite(void* ptr, int size, int nobj, FILE* fp);
// *ptr에 담긴 것을 size만큼 nobj번 fp에 씀

long ftell(FILE *fp), void rewind(FILE* fp)

// ==========================================================================

int stat(char* pathname, struct stat *buf)
// buf에 pathname에 대한 status를 담음
struct stat statbuf; stat(argv[i], &statbuf)

int fstat(int fd, struct stat *buf);
// file descriptor 줌

int lstat(char* pathname, struct stat *buf);
// pathname 자체에 대한 status
// 모두 ok이면 0

S_ISDIR(statbuf.st_mode) // directory인지 판단

umask(S_IRUSR | ...)

int chmod(char* pathname, mode_t mode)
chmod("path", (statbuf.st_mode & ~S_IWGRP) // 그룹 쓰기 끄기
chmod("path", S_IRUSR | S_IWISR); // or이면 키기
int fchmode(int fd, mode_t mode);

int chown(char* pathname, uid_t owner, gid_t group)
// owner을 파라미터로 준 것으로 바꿈

int link(char* existingPath, char* newPath);
// hardlink

int symlink(char* actualPath, char* symPath);
// Symbolic Link

int remove(char* pathname)
int rename(char* old, char* new)

int mkdir(char* pathname, mode_t mode);
int rmdir(char* pathname);

DIR* opendir(char* pathname);
struct dirent* readdir(DIR* dp);
// opendir로 디렉토리를 열고 그 반환 값을 readdir에 넣으면 호출할 때마다
// 하나의 파일 엔트리가 넘어옴

DIR *dp;
struct dirent *dep;
dp = opendir(".");
while(dep = readdir(dp)) dep -> d_name


int chdir(char* pathname);

char* getcwd(char* buf, int size);

// ========================================================================

int getpid()
int getuid()

int fork()

int atexit(void (*func)(void));

int wait(NULL)
int waitpid(pid, NULL, 0)

int execl(char* pathname, char* arg0, ..., (char *)0);
// pathname에서 arg0 ~을 찾아 실행

int execv(char* pathname, char* argv[])
// argv에 다 씀, 단 마지막에 NULL도

execlp이면 PATH에서 그 파일을 찾아 실행

#include <pthread.h>
pthread_t tid1
pthread_create(&tid1, NULL, (void *)함수이름, (void *)파라미터)

pthread_exit(NULL)

pthread_join(tid1, NULL);

// ========================================================================
signal(SIGINT, SigIntHandler)

kill(13418, SIGUSR1)
// Send Signal to Process 13418

int raise(int signo);
// 자기 자신에게 signal

sleep(unsigned int seconds);
// 나타낸 초 동안 프로세스 상태를 wait

unsigned int alarm(unsigned int seconds);
// 나타낸 초 이후 ALARM Signal을 보냄

// ========================================================================

int pipe(int fd[2]);
// 0은 read 1은 write, 사용하지 않는 것은 close

int dup(int oldfd);
 // 할당 가능한 fd 값중 가장 작은 번호를 return
close(1); -> STDOUTPUT을 닫음
close(fd[0]); -> read fd를 닫음
dup(fd[1]) == dup2(fd[1], 1) -> fd[1]에 descriptor을 할당하는데 현재 1번을 닫아 그게 제일 작은 번호

#define SERV_FIFO "./.fifo
mkfifo(SERV_FIFO, 0600);

int shmid = shmget(SHM_KEY, 1024, SHM_MODE);
char* ptr = shmat(shmid, 0, 0); // ptr이 할당한 메모리를 가리키도록
shmctl(shmid, IPC_RMID, 0);

src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);
dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARD, fdout, 0);

// ========================================================================

1-1 POSIX Semaphore
* 스레드간 동기화, 링킹할 때 -lpthraed 옵션 필요
#include <semaphore.h>
sem_t				EmptySem, FullSem, MutexSem;
// Semaphore 하나 생성
// pshared은 사용하지 않음
// value로 초기값 세팅
int sem_init(sem_t *sem, int pshared, unsigned int value);

// wait: 없으면 기다리는 것, -1
int sem_wait(sem_t *sem);

// 내가 지금 Semaphore을 가질 수 있나 없나만 체크
// 가질 수 있으면 -1, 가질 수 없으면(0이면) fail로 리턴
int sem_trywait(sem_t *sem);

// 자원을 반납하는 것, +1
int sem_post(sem_t *sem);

// 현재 값이 얼마인지 sval에 리턴
int sem_getvalue(sem_t *sem, int *sval);

// Semaphore 삭제
int sem_destroy(sem_t *sem);

1-2 System V Semaphore
* 프로세스간 동기화
* 너무 복잡해서 라이브러리를 임의로 만들어 POSIX처럼 씀

#include "semlib.h"
int semInit(key_t key);

// Semaphore 초기화
int semInitValue(int seemid, int value);

// -1 or 0이면 wait
int semWait(int semid);

// -1 or 0이면 fail
int semTryWait(int semid);

// +1
int semPost(int semid);

// 현재 Semaphore 값 가져오기
int semGetValue(int semid);

// Semaphore 삭제
int semDestroy(int semid);

2-1 Mutex
* 스레드 간에서 사용
* Binary Semaphore와 거의 같음
#include <pthread.h>

// mutex 하나 생성, 최초에 만들면 unlock 상태
int pthread_mutex_init(pthread_mutex_t *mutex, phread_mutexattr_t *mattr);

int pthread_mutex_destroy(pthread_mutex_t *mutex);

// lock을 누가 걸었으면 wait
int pthread_mutex_lock(phread_mutex_t *mutex);

// trywait과 비슷, 누가 lock을 안걸었으면 lock을 진행, 걸었으면 fail 반환
int pthread_mutex_trylock(phread_mutex_t *mutex);

int pthread_mutex_unlock(phread_mutex_t *mutex);

2-2 Condition Variable
* mutex lock을 걸고 unlock을 거는 사이에서 어떤 조건이 발생하기를 기다리는 코드

#include <pthread.h>
// Condition variable 하나를 만듦
int pthread_cond_init(pthread_cond_t *cond, phtread_condattr_t *cattr);

int pthread_cond_destroy(pthread_cond_t *cond);

// lock과 unlock 사이에서 어떤 조건이 발생하기를 기다림
// wait을 하는 상황에서 mutex를 계속 가지고 있으면 다른 스레드가 못들어오므로 unlock을 해줘야함
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

// wait하고 있는 스레드를 깨워줌
int pthread_cond_signal(pthread_cond_t *cond)

// wait하고 있는 모든 스레드에게 신호를 보냄
int pthread_cond_broadcast(pthread_cond_t *cond);

pthread_cond_timedwait(&cond, &mutex, &ts) // 특정 시간까지만 기다리고 그동안 signal하는 애가 없으면 빠져나옴, &ts: 절대시간, Threadusleep을 위해

// ========================================================================

// IPv4 TCP 소켓 생성
int Sockid = socket(PF_INET, SOCK_STREAM, 0) // UDP이면 SOCK_DGRAM

// Sockfd에 servAddr을 매핑
bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0

// 형식적, 몇 개의 소켓을 동시에 처리할 것인가
listen(Sockfd, 5);

// TCP 연결이 하나 맺어지면 그 맺어진 TCP connection을 가리키는 새로운 sd가 생기고 리턴함
int accept(int sd, struct sockaddr *peer, int *addrlen);

connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr))

ssize_t recv(newSockfd, (char *)&msg, sizeof(msg), int flags);
ssize_t read(newSockfd, (char *)&msg, sizeof(msg));
recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)

ssize_t send(newSockfd, (char *)&msg, sizeof(msg), int flags);
ssize_t write(newSockfd, (char *)&msg, sizeof(msg));
sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen)

struct sockaddr_in	servAddr;
  servAddr.sin_family = PF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Server, INADDR_ANY라고 지정하면 어느 주소로 오던 내가 다 처리하겠다
  servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // Client, inet_addr은 자동으로 network order로 바꿔줌
  servAddr.sin_port = htons(SERV_TCP_PORT); // Port 번호 지정, Ip와 Port 모두 network order로 바꿔야함

// Little-Endian에서만 사용
u_long htonl(u_long hostlong); // host to network long
u_short htons(u_short hostshort);
u_long ntohl(u_long netlong);
u_short ntohs(u_short netshort);

unsigned long inet_addr(char *str);
// 32bits의 string IPv4를 4bytes integer로 바꿔줌
// 이때 network order로 바꿈

char *inet_ntoa(struct in_addr inaddr);
// 역

#include <netdb.h>
struct hostent *gethostbyname(char *name);
// hp = gethostbyname(argv[1]), hp->h_addr, hp->h_length

fd_set fdvar;
* FD_SET(TcpSockfd, &fdvar); // Setting
* FD_ZERO(&fdvar); // 전체를 0으로 일단 초기화
* FD_CLR(int fd, fd_set *fdset); fd 없앰 
* FD_ISSET(int fd, fd_set *fdset); 여러 개 fd중 어디서 왔는지 체크할 때 사용 -> 나한테 패킷이 왔나 안왔나를 체크

count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL) //read에 대해서만
while (count--)  {
			if (FD_ISSET(TcpSockfd, &fdvar))  { // 어떤 소켓에서 왔는지 검사
				ProcessTcpRequest();
			}
      ...
}