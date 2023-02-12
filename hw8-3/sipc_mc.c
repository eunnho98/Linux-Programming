#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>

// sipc Mutex & Condition Variable
typedef struct{
	char data[1024];
	int num;
} Message;

Message Msg;
pthread_cond_t From1;
pthread_cond_t From2;
pthread_mutex_t Mutex;

void sipc1(void *dummy) {
	if(pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	while(Msg.num != 2) { // Msg number가 2가 아니면(2에서 데이터가 안오면) 
		if(pthread_cond_wait(&From2, &Mutex) < 0) {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}
	printf("sipc1 Received request: %s....\n", Msg.data);
	sprintf(Msg.data, "This is reply from sipc1");
	printf("sipc1 Replied\n");
	Msg.num = 1;

	if(pthread_cond_signal(&From1) < 0) { // 1에서 데이터를 썼다고 알림
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}
	if(pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);	
}

void sipc2(void *dummy) {
	if(pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	sprintf(Msg.data, "This is request from sipc2");
	printf("Sent a request from sipc2\n");
	Msg.num = 2; // Msg 번호를 2번으로 바꿈
	if(pthread_cond_signal(&From2) < 0) { // 2번에서 썼다고 알림
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}
	while(Msg.num != 1) { // 1에서 데이터가 안오면
		if(pthread_cond_wait(&From1, &Mutex) < 0) {
			perror("pthraed_cond_wait");
			pthread_exit(NULL);
		}	
	}
	printf("sipc2 Received reply: %s\n", Msg.data);

	if(pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void
main() {
	pthread_t tid1, tid2;

	if(pthread_cond_init(&From1, NULL) < 0) {
		perror("pthraed_cond_init");
		pthread_exit(NULL);
	}
	
	if(pthread_cond_init(&From2, NULL) < 0) {
		perror("pthraed_cond_init");
		pthread_exit(NULL);
	}

	if(pthread_mutex_init(&Mutex, NULL) < 0) {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	if(pthread_create(&tid1, NULL, (void *)sipc1, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if(pthread_create(&tid2, NULL, (void *)sipc2, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	if(pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	if(pthread_cond_destroy(&From1) < 0) {
		perror("pthread_cond_destroy");
	}

	if(pthread_cond_destroy(&From2) < 0) {
		perror("pthread_cond_destroy");
	}

	if(pthread_mutex_destroy(&Mutex) < 0) {
		perror("pthread_Mutex_destroy");
	}
}
