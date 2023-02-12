1-1 POSIX Semaphore
* 스레드간 동기화, 링킹할 때 -lpthraed 옵션 필요
#include <semaphore.h>

// Semaphore 하나 생성
// pshared은 사용하지 않음
// value로 초기값 세팅
int sem_init(sem_t *sem, int pshared, unsigned int value);

// wait: 없으면 기다리는 것, -1
int sem_wait(sem_t *sem);

// 내가 지금 Semaphore을 가질 수 있나 없나만 체크
// 가질 수 있으면 -1, 가질 수 없으면(0이면) fail로 리턴
int sem_trywait(sem_t *sem);

// 자원을 반납하는 것, +1
int sem_post(sem_t *sem);

// 현재 값이 얼마인지 sval에 리턴
int sem_getvalue(sem_t *sem, int *sval);

// Semaphore 삭제
int sem_destroy(sem_t *sem);

1-2 System V Semaphore
* 프로세스간 동기화
* 너무 복잡해서 라이브러리를 임의로 만들어 POSIX처럼 씀

#include "semlib.h"
int semInit(key_t key);

// Semaphore 초기화
int semInitValue(int seemid, int value);

// -1 or 0이면 wait
int semWait(int semid);

// -1 or 0이면 fail
int semTryWait(int semid);

// +1
int semPost(int semid);

// 현재 Semaphore 값 가져오기
int semGetValue(int semid);

// Semaphore 삭제
int semDestroy(int semid);

2-1 Mutex
* 스레드 간에서 사용
* Binary Semaphore와 거의 같음
#include <pthread.h>

// mutex 하나 생성, 최초에 만들면 unlock 상태
int pthread_mutex_init(pthread_mutex_t *mutex, phread_mutexattr_t *mattr);

int pthread_mutex_destroy(pthread_mutex_t *mutex);

// lock을 누가 걸었으면 wait
int pthread_mutex_lock(phread_mutex_t *mutex);

// trywait과 비슷, 누가 lock을 안걸었으면 lock을 진행, 걸었으면 fail 반환
int pthread_mutex_trylock(phread_mutex_t *mutex);

int pthread_mutex_unlock(phread_mutex_t *mutex);

2-2 Condition Variable
* mutex lock을 걸고 unlock을 거는 사이에서 어떤 조건이 발생하기를 기다리는 코드

#include <pthread.h>
// Condition variable 하나를 만듦
int pthread_cond_init(pthread_cond_t *cond, phtread_condattr_t *cattr);

int pthread_cond_destroy(pthread_cond_t *cond);

// lock과 unlock 사이에서 어떤 조건이 발생하기를 기다림
// wait을 하는 상황에서 mutex를 계속 가지고 있으면 다른 스레드가 못들어오므로 unlock을 해줘야함
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

// wait하고 있는 스레드를 깨워줌
int pthread_cond_signal(pthread_cond_t *cond)

// wait하고 있는 모든 스레드에게 신호를 보냄
int pthread_cond_broadcast(pthread_cond_t *cond);

pthread_cond_timedwait(&cond, &mutex, &ts) // 특정 시간까지만 기다리고 그동안 signal하는 애가 없으면 빠져나옴, &ts: 절대시간, Threadusleep을 위해
