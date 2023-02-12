IPC
프로세스 소통 메커니즘

1-1 pipe 
* 메세지 패싱
* half-duplex
* 부모-자식간 사용하는 메커니즘

int pipe(int fd[2]);
* fd[0]은 read, fd[1]은 write
* 사용하지 않는 file descriptor은 close 해야함

1-2 dup
* file descriptor을 copy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int dup(int oldfd); // 할당 가능한 fd 값중 가장 작은 번호를 return
int dup2(int oldfd, newfd);

main() {
	// child가 pipe에 쓰고 부모가 pipe에서 읽어옴
	int fd[2];
	pid_t child;

	pipe(fd);
	child = fork();
	
	// child일 경우
	close(1); -> STDOUTPUT을 닫음
	close(fd[0]); -> read fd를 닫음
	dup(fd[1]) -> fd[1]에 descriptor을 할당하는데 현재 1번을 닫아 그게 제일 작은 번호
	-> fd[1]에 STDOUTPUT이 복사됨

	// parent
	close(0); -> STDIN을 닫음
	close(fd[1]); -> write fd를 닫음
	dup2(fd[0], 0);
	-> dup2는 직접 지정을 해주는데 그때도 대상은 close 해줘야함
	-> STDIN을 fd[0]에 복사 -> 입력을 STDIN이 아닌 fd[0]에서 받아오겠다.
}
-> child 프로세스의 출력이 pipe에 쓰여지고, 그 출력이 다시 parent의 입력이 된다.ㅏ

2. FIFOs
* 서로 다른 프로세스 간의 메세지 패싱
* full-duplex
* 두 프로세스 짝을 위해 FIFO queue에 ID 지정 필요
int mkfifo(char* pathname, mode_t mode);
* fifo는 special file, ls -a하면 나옴

헤더파일
#define SERV_FIFO "./.fifo" -> 서버의 FIFO, .fifo가 현재 서버가 기다리는 fifo
typedef struct {
	char returnFifo[30];
	char data[1024];
} MsgType
// Server
main() {
	MsgType msg
	mkfifo(SERV_FIFO, 0600);
	int fd = open(SERV_FIFO, O_RDWR); // fifo는 special file임
	int n = read(fd, (char*)&msg, sizeof(msg))
	int cfd = open (msg.returnFifo, O_WRONLY)
	sprintf(msg.data "Server reply");
	write(cfd, (char*)&msg, sizeof(msg));
	close(cfd);
}

// Client
main() {
	char fifoname[32];
	MsgType msg;
	sprintf(fifoname, ~~);
	mkfifo(fname, 0600); // fifo 생성
	fd = open(fifoname, O_RDWR); // 서버에서 받아오기 위한 FIFO
	sfd = open(SERV_FIFO, O_RDWR); // Server FIFO 오픈
	strcpy(msg.returnFifo, fifoname);
	sprintf(msg.data, ~~);
	write(sfd, (char*)&msg, sizeof(msg)); // 서버 fifo에 write

	n = read(fd, (char*)&msg, sizeof(msg)); // 서버에서 응답 메세지를 이 FIFO에 썼음
	close(fd);
	close(sfd);
	remove(fifoname); // 서버로부터 받으면 내가 만든 fifo를 제거
}

3. Shared Memory
* 동기화 문제 직접 해결 필요
* heap, stack 사이에 저장

int shmget(key_t key, int size, int flag);
* memory identifier을 하나 얻어옴
* shared memory가 없으면 하나 만들고, 없으면 그 메모리를 open
* key_t: int, shared memory의 키(이름)
* size: 메모리의 크기, shared memory를 만든다면 얼마만큼 size를 할 것인지 지정
	* 만약 이미 있는 shared memory라면 자동으로 0이됨
* flag: option

void *shmat(int shnid, void *addr, int flag);
* shmget을 통해 얻어온 메모리를 내 메모리에 붙이는 것
* addr, flag는 그냥 null로 두기

int shmdt(void *addr);
* 다 써서 그 매핑이 필요없을 때 놓는 것

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
* shmid: shmget을 통해 받은 shared memory id
* cmd: 커맨드
* buf: 그 커맨드에 해당하는 operation
*보통 shmctl(shmid, IPC_RMID, 0); 으로 메모리 해제에 사용

Ex)
#define SHM_KEY (~~)
#define SHM_MODE(~~)
// Server
main() {
	int shmid = shmget(SHM_KEY, 1024, SHM_MODE);
	char* ptr = shmat(shmid, 0, 0); // ptr이 할당한 메모리를 가리키도록
	ptr에 쓰면 공유 메모링 데이터를 쓰는 것 (sprintf(ptr, ~~))
	shmctl(shmid, IPC_RMID, 0);
}

//Client
main() {
	int shmid = shmget(SHM_KEY, 1024, SHM_MODE);
	char* ptr = shmat(shmid, 0, 0);
}

4. Mermoy Mapped File
* Virtual Address을 파일처럼 다룸, 메모리에서 read-write를 하는데 실제로 파일에서 read-write가 됨
* read, write 할 필요 없이 메모리 접근만으로 File I/O가 가능

caddr_t mmap(caddr_t addr, size_t len, int prot, int flag, int fd, off_t off);
* start address를 리턴
* addr: 이 Virtual Address에 매핑해달라, 잘 안쓰니 0
* len: file의 크기
* prot: protection: rwxnone
* 메모리 I/O를 파일 I/O처럼 하므로 fd가 필요
* off: 파일의 어느 부분부터 read를 할 것인가
Ex) mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)

main() {
	int fdin, fdout
	char *src, *dst
	struct stat statbuf

	fdin = open(~, O_RDONLY)
	fdout = open(~, ~, 0644);
	fstat(fdin, &statbuf); // fdin의 파일 정보를 얻음
	
	src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);
	dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARD, fdout, 0);

	memcpy(dst, src, statbuf_st_size);
	-> *src부터 stat_buf만큼 *dst부터 복사
	-> fdin에서 읽어 fdout에 쓰는 것, src는 fdin과 dst는 fdout과 매핑을 했으므로
}
