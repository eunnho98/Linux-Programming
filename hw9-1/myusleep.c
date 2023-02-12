// myusleep.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void
myusleep(int secs, int usecs)
{
	struct timeval	tm;

	tm.tv_sec = secs;
	tm.tv_usec = usecs;

// 내가 지금 기다리고 있는 소켓 번호를 알려줌
// int select(int maxfd, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *tvptr);
// 내가 만든 fd중 maxfd 까지를 기다리는 것으로 상한선으로 tvptr 동안 read, write, except 기다리겠다.
	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0)  {
		perror("select");
		return;
	}
}

main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}