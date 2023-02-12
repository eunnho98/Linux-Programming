#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "chat.h"

#define	MAX_BUF		256

int		Sockfd;

void
CloseClient(int signo)
{
	close(Sockfd);
	printf("\nChat client terminated.....\n");

	exit(0);
}

void
ToServer(void *dummy) {
	int n;
	char buf[MAX_BUF];
	while(1) {
		fgets(buf, MAX_BUF, stdin);
		if ((n = send(Sockfd, buf, strlen(buf)+1, 0)) < 0)  {
			perror("send");
			exit(1);
		}
	}
}

void
FromServer(void *dummy) {
	int n;
	char buf[MAX_BUF];
	while(1) {
		n = recv(Sockfd, buf, MAX_BUF, 0);
		if(n == 0) { // 서버에서 Ctrl c하면 종료
			fprintf(stderr, "Server terminated.....\n");
			close(Sockfd);
			exit(1);
			pthread_exit(NULL);
		}
		printf("%s", buf);
	}
}


main(int argc, char *argv[])
{
	struct sockaddr_in	servAddr;
	struct hostent		*hp;
	char buf[MAX_BUF];
	int count, n;
	fd_set fdset;
	pthread_t tid1, tid2;

	signal(SIGINT, CloseClient);

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (isdigit(argv[1][0]))  {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else  {
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

	printf("Enter ID: ");
	fflush(stdout);
	fgets(buf, MAX_BUF, stdin);
	*strchr(buf, '\n') = '\0'; // 문자열 검색, 못찾으면 NULL
	if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  {
		perror("send");
		exit(1);
	}
	printf("Press ^C to exit\n");

	if(pthread_create(&tid1, NULL, (void *)ToServer, NULL) < 0) { // 서버로 메시지 전달
		perror("pthread_create");
		exit(1);
	}

	if(pthread_create(&tid2, NULL, (void *)FromServer, NULL) < 0) { // 서버에서 메시지 얻어옴
		perror("pthread_create");
		exit(1);
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
}