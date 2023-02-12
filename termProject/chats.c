#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "chat.h"

#define	DEBUG
#define	MAX_CLIENT	5
#define	MAX_ID		32
#define MAX_BUF 100

typedef	struct  {
	int			sockfd;
	int			inUse;
	pthread_t	tid;
	char		uid[MAX_ID];
}
	ClientType;

ClientType		Client[MAX_CLIENT];
int Sockfd;

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

void SendToOtherClients(int id, char *buf) {
	int		i;
	char	msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG // DEBUG가 정의되었다면
	printf("%s", msg);
	fflush(stdout);
#endif
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse && (i != id))  { // Client가 동작 중이고 내 자신이 아닐 때
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  { // 메시지 전달
				perror("send");
				exit(1);
			}
		}
	}
}

void
CloseServer(int signo)
{
	int i;
	close(Sockfd);
	for(i = 0; i < MAX_CLIENT; i++) {
		if(Client[i].inUse) {
			close(Client[i].sockfd);
		}
	}
	printf("\nChat server terminated.....\n");

	exit(0);
}

int main(void)
{

    struct sockaddr_in servAddr;
    struct sockaddr_in cliAddr;

    fd_set fdvar, temps;
    int fd_max;
    int binded;
    int id, i;
    char buf[MAX_BUF];

    int fd, n;
    int newSockfd, cliAddrLen;
    int one = 1;

    signal(SIGINT,  CloseServer); // 서버 종료

    Sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// 이미 사용된 주소를 재사용 (bind) 하도록
    setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one));
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(SERV_TCP_PORT);

    binded = bind(Sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if(binded != 0)
    {
      perror("bind error");
      exit(-1);
    }

    binded = listen(Sockfd, 5);
    if(binded != 0)
    {
      perror("listen error");
      exit(-1);
    }
    printf("Chat server started.....\n");

    FD_ZERO(&fdvar); 
    FD_SET(Sockfd, &fdvar); 
    fd_max = Sockfd; 

    while(1)
    {
      temps = fdvar;
      if(select(fd_max + 1, &temps, 0, 0, (struct timeval *)NULL) < 0) 
      {
        error("select error");
        exit(1);
      }
 
      for(fd = 0; fd < fd_max+1; fd++) 
      {
        if(FD_ISSET(fd, &temps)) 
        {
          if(fd == Sockfd) // 연결요청인경우
          {
            cliAddrLen = sizeof(cliAddr);
            newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
            // 해당 클라이언트와 연결
            id = GetID();
            FD_SET(newSockfd, &fdvar); 
            Client[id].sockfd = newSockfd;
            if(fd_max < newSockfd) 
            {
              fd_max = newSockfd;
            }
            n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0);
          	printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
          }
          else{
            for(i = 0; i < MAX_CLIENT; i++) {
					    if(Client[i].sockfd == fd) {
						  break;
					    }
				    }
            n = recv(Client[i].sockfd, buf, MAX_BUF, 0); 
            if(n == 0) // 아무것도 안보내면(ctrl c) 종료
            {
            	printf("Client %d log-out(ID: %s).....\n", i, Client[i].uid);
              FD_CLR(fd, &fdvar); // 해당 파일디스크립터 fd를 0으로 변경
              close(Client[i].sockfd);
            	Client[i].inUse = 0;
					    strcpy(buf, "log-out.....\n");
    					SendToOtherClients(i, buf); // Client가 입력한 것을 다른 클라이언트들에게 전달
            }
            else{ 
              SendToOtherClients(i, buf);     
            }
          }
        }
      }
    }
  return 0;
}
