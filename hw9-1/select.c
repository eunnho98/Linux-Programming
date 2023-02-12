// select.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"


// 서버들이 사용하는 소켓들
int	TcpSockfd;
int	UdpSockfd;
int	UcoSockfd;
int	UclSockfd;

void // signal handler
CloseServer()
{
	close(TcpSockfd);
	close(UdpSockfd);
	close(UcoSockfd);
	close(UclSockfd);
	if (remove(UNIX_STR_PATH) < 0)  { // 실제로 FIFO이므로 시스템에 남아서 지워야함
		perror("remove");
	}
	if (remove(UNIX_DG_PATH) < 0)  {
		perror("remove");
	}

	printf("\nServer daemon exit.....\n");

	exit(0);
}

void
MakeTcpSocket()
{
	struct sockaddr_in	servAddr;

	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5);
}

void
MakeUdpSocket()
{
	struct sockaddr_in	servAddr;

	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

}

void
MakeUcoSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if (bind(UcoSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(UcoSockfd, 5);

}

void
MakeUclSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if (bind(UclSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
		perror("bind");
		exit(1);
	}

}

void
ProcessTcpRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
  // 연걸 맺어지기를 기다림
	newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
	if (newSockfd < 0)  {
		perror("accept");
		exit(1);
	}
		
  // TCP Client에서 보내온 request 읽기
	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
  // Client에게 응답
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd);
}

void
ProcessUdpRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
  // UDP Client에서 받은 request 읽기
	if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 
				0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
  // 응답
	if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

void
ProcessUcoRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	newSockfd = accept(UcoSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
	if (newSockfd < 0)  {
		perror("accept");
		exit(1);
	}
		
	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received UNIX-domain CO request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd);
}

void
ProcessUclRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	if ((n = recvfrom(UclSockfd, (char *)&msg, sizeof(msg), 
				0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UNIX-domain CL request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (sendto(UclSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

main(int argc, char *argv[])
{
	fd_set	fdvar; // file descriptor 저장하는 구조체
	int		count;

	signal(SIGINT, CloseServer);

	MakeTcpSocket();
	MakeUdpSocket();
	MakeUcoSocket();
	MakeUclSocket();

	printf("Server daemon started.....\n");

	while (1)  {
		FD_ZERO(&fdvar); // 전체를 0으로 일단 초기화
		FD_SET(TcpSockfd, &fdvar); // Setting, TcpSockfd 부분이 1로 세팅됨
		FD_SET(UdpSockfd, &fdvar);
		FD_SET(UcoSockfd, &fdvar);
		FD_SET(UclSockfd, &fdvar);
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, //read에 대해서만 기다림, 총 fd개수 반환
			(struct timeval *)NULL)) < 0)  {
			perror("select");
			exit(1);
		}
		while (count--)  {
			if (FD_ISSET(TcpSockfd, &fdvar))  { // 어떤 소켓에서 왔는지 검사
				ProcessTcpRequest();
			}
			else if (FD_ISSET(UdpSockfd, &fdvar))  {
				ProcessUdpRequest();
			}
			else if (FD_ISSET(UcoSockfd, &fdvar))  {
				ProcessUcoRequest();
			}
			else if (FD_ISSET(UclSockfd, &fdvar))  {
				ProcessUclRequest();
			}
		}
	}
}