#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define	DEBUG

#define	MAX_CLIENT	5
#define	MAX_ID		32
#define	MAX_BUF		256

typedef	struct  {
	int			sockfd;
	int			inUse;
	pthread_t	tid;
	char		uid[MAX_ID];
}
	ClientType;

int				Sockfd;
pthread_mutex_t	Mutex;

ClientType		Client[MAX_CLIENT];


int
GetID()
{
	int	i;

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (! Client[i].inUse)  { // Client가 0일 때
			Client[i].inUse = 1;
			return i;
		}
	}
}

void
SendToOtherClients(int id, char *buf)
{
	int		i;
	char	msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG // DEBUG가 정의되었다면
	printf("%s", msg);
	fflush(stdout);
#endif

	pthread_mutex_lock(&Mutex);
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse && (i != id))  { // Client가 동작 중이고 내 자신이 아닐 때
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  { // 메시지 전달
				perror("send");
				exit(1);
			}
		}
	}
	pthread_mutex_unlock(&Mutex);
}
	
// 서버로 클라이언트 몇 번이 들어왔는지, 나갔는지 알림
void
ProcessClient(int id)
{
	char	buf[MAX_BUF];
	int		n;

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  { // thread에게 취소 요청을 보냄
		perror("pthread_setcancelstate");
		exit(1);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  { // 취소 요청 중 thread를 바로 종료
		perror("pthread_setcanceltype");
		exit(1);
	}

	if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  { // 파라미터로 넘긴 id의 클라이언트로부터 receive
		perror("recv");
		exit(1);
	}
	printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);

	while (1)  {
		if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0)  {
			perror("recv");
			exit(1);
		}
		if (n == 0)  { // 아무것도 안보내면(ctrl c) 종료
			printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);

			pthread_mutex_lock(&Mutex);
			close(Client[id].sockfd);
			Client[id].inUse = 0; // logout하면 그 Client 상태를 0으로
			pthread_mutex_unlock(&Mutex);

			strcpy(buf, "log-out.....\n");
			SendToOtherClients(id, buf);

			pthread_exit(NULL);
		}

		SendToOtherClients(id, buf); // Client가 입력한 것을 다른 클라이언트들에게 전달
	}
}


void
CloseServer(int signo)
{
	int		i;

	close(Sockfd);

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse)  {
			if (pthread_cancel(Client[i].tid))  { // 해당 요청을 받으면 스레드는 exit 수행
				perror("pthread_cancel");
				exit(1);
			}
			if (pthread_join(Client[i].tid, NULL))  {
				perror("pthread_join");
				exit(1);
			}
			close(Client[i].sockfd);
		}
	}
	if (pthread_mutex_destroy(&Mutex) < 0)  {
		perror("pthread_mutex_destroy");
		exit(1);
	}

	printf("\nChat server terminated.....\n");

	exit(0);
}

main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, id, one = 1;
	struct sockaddr_in	cliAddr, servAddr;

	signal(SIGINT, CloseServer); // Ctrl + c : Server 종료
	if (pthread_mutex_init(&Mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

// 이미 사용된 주소를 재사용 (bind) 하도록
	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
		perror("setsockopt");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); // 최대 소켓 5개 처리

	printf("Chat server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}

		id = GetID();
		Client[id].sockfd = newSockfd;
		
		if (pthread_create(&Client[id].tid, NULL, (void *)ProcessClient, (void *)id) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}
}