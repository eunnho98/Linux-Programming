#include <stdio.h>
#include <signal.h>
#include <pthread.h>


pthread_t	ThreadId[2];

void
SigIntHandler(int signo)
{
	int		i;

	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	for (i = 0 ; i < 2 ; i++)  {
		/*
    int pthread_cancel(pthread_t tid);
    현재 실행되고 있는 스레드를 종료시키는 시스템 콜
    DEFERRED하면 데드락 방지를 위해 thread가 죽는데 critical section 이후에 죽도록
    */
		if (pthread_cancel(ThreadId[i]))  {
			perror("pthread_cancel");
			exit(1);
		}
	}
	for (i = 0 ; i < 2 ; i++)  {
		if (pthread_join(ThreadId[i], NULL))  { // process의 wait 역할
			perror("pthread_join");
			exit(1);
		}
	}
	printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

void
Thread1(void *dummy)
{
	printf("Thread %d created.....\n", pthread_self());
/*
state: PTHREAD_CANCEL_DISABLE, PTHREAD_CANCEL_ENABLE(Default)
ENABLE하면 다른 스레드에서 이 스레드를 죽이는 것 허용

int pthread_setcanceltype(int type, int *oldtype);
type: PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_DEFERRED(Default)
DEFERRED하면 데드락 방지를 위해 thread가 죽는데 critical section 이후에 죽도록
*/
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

void
Thread2(void *dummy)
{
	printf("Thread %d created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

main()
{
	if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();
}