//tcp server.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"


int	Sockfd; // signal handler에서 사용하려고 전역 변수로 놓음

void
CloseServer()
{
	close(Sockfd);
	printf("\nTCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // 소켓 생성
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP주소 지정, 서버면 IP 주소가 여러개 있는데 INADDR_ANY
																							  // 라고 지정하면 어느 주소로 오던 내가 다 처리하겠다
																								// 특정 주소로만 오게하려면 그 주소를 괄호에 넣기
	servAddr.sin_port = htons(SERV_TCP_PORT); // Port 번호 지정, Ip와 Port 모두 network order로 바꿔야함

// socket에 주소를 할당
	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

// 형식적, 몇 개의 소켓을 동시에 처리할 것인가
	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // 연걸 맺어지기를 기다림
		if (newSockfd < 0)  {                                                  // cliAddr에 맺어진 IP주소 담김     
			perror("accept");
			exit(1);
		}
		
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		usleep(10000);
		close(newSockfd);
	}
}

// 일반적으로는 fork를 해서 child에서 소켓 통신을 하고 부모는 바로 새로운 연결을 기다림