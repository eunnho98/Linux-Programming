#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"


main()
{
	char		fname[MAX_FIFO_NAME];
	int			fd, sfd, n; // client, server, n
	MsgType		msg;

  // Client-specific FIFO 생성
	sprintf(fname, ".fifo%d", getpid());
	if (mkfifo(fname, 0600) < 0)  {
		perror("mkfifo");
		exit(1);
	}

  // 응답을 읽어오기 위해 Client-specific FIFO open
	if ((fd = open(fname, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}
  // 서버로 메시지를 날리기 위해 서버 FIFO open
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d.", getpid());
  // 서버 FIFO에 데이터 작성(request)
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request.....");

// 서버로의 응답을 기다림
	if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd);
	close(sfd);
  // 통신이 완료되면 Client-specific FIFO를 삭제
	if (remove(fname) < 0)  {
		perror("remove");
		exit(1);
	}
}