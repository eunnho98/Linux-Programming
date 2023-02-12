#include <pthread.h>
// Mutex & Condition Variable로 Semaphore 구현
typedef struct  {
	int		sval; // 사용 가능한 자원의 개수를 나타내므로
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}
	sem_t;

int		sem_init(sem_t *sem, int pshared, int value);
int		sem_wait(sem_t *sem);
int		sem_trywait(sem_t *sem);
int		sem_post(sem_t *sem);
int		sem_getvalue(sem_t *sem, int *sval);
int		sem_destroy(sem_t *sem);