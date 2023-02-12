#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "msg.h"

// sipc POISX Semaphore
Message Msg;
sem_t From1Sem, From2Sem, MutexSem;

void
sipc1(void* dummy) {
	if(sem_wait(&From2Sem) < 0) { // sipc2에서 데이터 올 때까지 대기
		perror("sem_wait");
		pthread_exit(NULL);
	}

	if(sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("sipc1 Received data: %s\n", Msg.data);
	sprintf(Msg.data, "sipc1 replied msg");
	printf("sipc1 Replied data\n");
	if(sem_post(&MutexSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	
	if(sem_post(&From1Sem) < 0) { // sipc1에서 데이터를 썼다고 알림
		perror("sem_post");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void
sipc2(void* dummy) {
	if(sem_wait(&MutexSem) < 0) { // Msg에 대한 접근권한 얻기
		perror("sem_wait");
		pthread_exit(NULL);
	}

	sprintf(Msg.data, "sipc2 request msg");
	printf("Sipc2 sent a request\n");
	
	if(sem_post(&From2Sem) < 0) { // sipc2에서 다 썼다고 알림
		perror("sem_post");
		pthread_exit(NULL);
	}

	if(sem_post(&MutexSem) < 0) { // MSG에 대한 접근권한 놓기
		perror("sem_post");
		pthread_exit(NULL);
	}

	if(sem_wait(&From1Sem) < 0) { // sipc1에서 응답 올 때까지 대기
		perror("sem_wait");
		pthread_exit(NULL);
	}	
	
	if(sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("sipc2 Received Reply: %s\n", Msg.data);
	pthread_exit(NULL);
}

void
main() {
	
	pthread_t	tid1, tid2;

	if (sem_init(&From1Sem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&From2Sem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)sipc1, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)sipc2, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	if (sem_destroy(&From1Sem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&From2Sem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  {
		perror("sem_destroy");
	}
}









