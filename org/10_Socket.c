1-1 TCP_Server
// IPv4 TCP 소켓 생성
int Sockid = socket(PF_INET, SOCK_STREAM, 0)
																  																						
// Sockfd에 servAddr을 매핑
bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0

// 형식적, 몇 개의 소켓을 동시에 처리할 것인가
listen(Sockfd, 5);

// TCP 연결이 하나 맺어지면 그 맺어진 TCP connection을 가리키는 새로운 sd가 생기고 리턴함
// 파라미터로 준 sd는 서버에서 연결을 기다리는 sd
// peer: input 파라미터가 아닌 나랑 어떤 클라이언트가 연결되었는지를 나타내줌
int accept(int sd, struct sockaddr *peer, int *addrlen);

1-2 TCP_Client
int Sockfd = socket(PF_INET, SOCK_STREAM, 0)

connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr))

1-3 read write, 구조체
ssize_t recv(newSockfd, (char *)&msg, sizeof(msg), int flags);
ssize_t read(newSockfd, (char *)&msg, sizeof(msg));

ssize_t send(newSockfd, (char *)&msg, sizeof(msg), int flags);
ssize_t write(newSockfd, (char *)&msg, sizeof(msg));

struct sockaddr_in	servAddr;
  servAddr.sin_family = PF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Server, INADDR_ANY라고 지정하면 어느 주소로 오던 내가 다 처리하겠다
  servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // Client, inet_addr은 자동으로 network order로 바꿔줌
  servAddr.sin_port = htons(SERV_TCP_PORT); // Port 번호 지정, Ip와 Port 모두 network order로 바꿔야함

2-1 UDP_Server
Sockfd = socket(PF_INET, SOCK_DGRAM, 0) // DGRAM: UDP
bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr))

2-2 UDP_Client
sockfd = socket(PF_INET, SOCK_DGRAM, 0)

2-3 read, wrtie, 구조체
// UDP는 connectionless이므로 바로 recvfrom
recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)

// sendto를 할 때 cliAddr 정보가 쓰임
sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen)
recvfrom(sockfd, (char *)&msg, sizeof(msg),0, (struct sockaddr *)&peerAddr, &peerAddrLen)
struct sockaddr_in servAddr;
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Server
  servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // Client
	servAddr.sin_port = htons(SERV_UDP_PORT);

3. Ordering
* Little-Endian에서만 사용

u_long htonl(u_long hostlong);
* host to network, 자기 byte order을 network order로 바꿈
* l이라서 long integer
* 네트워크에 데이터를 보내기 전에 반드시

u_short htons(u_short hostshort);

u_long ntohl(u_long netlong);

u_short ntohs(u_short netshort);

4. Address Conversion
unsigned long inet_addr(char *str);
* 32bits의 string IPv4를 4bytes integer로 바꿔줌
* 이때 network order로 바꿈

char *inet_ntoa(struct in_addr inaddr);
위 함수의 역

5. DNS
#include <netdb.h>
struct hostent *gethostbyname(char *name);
// ex) gethostbyname("www.naver.com");

hostent 구조체에서는 h_addr에 ip주소가 담긴다는 것만 알면 됨
// hp = gethostbyname(argv[1]), hp->h_addr, hp->h_length

6. multiplex
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