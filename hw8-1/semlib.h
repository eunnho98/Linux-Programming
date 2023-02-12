#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// System V Semaphore을 POSIX Semaphore-like로 구현
int		semInit(key_t key);
int		semInitValue(int semid, int value);
int		semWait(int semid);
int		semTryWait(int semid);
int		semPost(int semid);
int		semGetValue(int semid);
int		semDestroy(int semid);