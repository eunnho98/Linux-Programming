// tcp client.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"


main(int argc, char *argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr; // socket address 구조체
	MsgType				msg;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // TCP IPv4 Socket 생성
		perror("socket");
		exit(1);
	}

// bzero(void *s, size_t n); 메모리 s를 n만큼 0으로 초기화
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET; // IPv4 socket address를 사용
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // inet_addr은 자동으로 network order로 바꿔줌
	servAddr.sin_port = htons(SERV_TCP_PORT); // host to network order(short)

// tcp client에서 server에게 연결 요청, sockfd에 servAddr을 매핑함
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  { // request
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  { // 서버 reply를 읽음
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
}