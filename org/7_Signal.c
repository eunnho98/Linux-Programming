Signal: 소프트웨어 인터럽트, signal handler을 등록해서 시그널이 오면 handler가 동작하도록
-> 핸들러를 등록하지 않으면 디폴트 핸들러가 등록, 프로세스를 다 죽이는 핸들러가 동작

void (*signal(int signo, void (*func)(int)))(int);
* signo는 SIGNAL들, SIGINT, SIGHUP 등
* 핸들러는 signo를 파라미터로 가짐
* 반환하는 것은 이전에 등록한 핸들러, 그런데 대부분은 새로 핸들러를 등록하는 것이므로
  사실 리턴값은 의미가 없음

Ex)
#include <signal.h>
void SigIntHandler(int signo) {
	/* Do Something*/
}

main() {
	signal(SIGINT, SigIntHandler);
}

-> signo에는 SIGINT의 int number가 들어감
-> handler내에서 signo == SIGINT 하면 true
-> 다른 일을 하다가(비동기) SIGINT이므로 ctrl^c를 누르면 그때 핸들러가 수행됨

//############################

Send a Signal

int kill(pid_t pid, int signo);
* signal을 보내면 프로세스가 종료되므로 kill이라고 함
* pid > 0: 그 pid에만 보냄
* pid == 0: 자신과 그룹 ID가 같은 모든 프로세스에게 보냄
* pid < 0: |pid|를 가진 모든 프로세스에 보냄
kill(13418, SIGUSR1) // 13418 프로세스에게 SIGUSR1을 보내라

int raise(int signo);
* 자기 자신에게 signal을 보냄

// ##########################

System Calls related with Signals

unsigned int sleep(unsigned int seconds);
* 나타낸 초 동안 프로세스의 상태가 wait됨, 다 끝나면 다시 ready queue로

unsigned int alarm(unsigned int seconds);
* 나타낸 초 이후에 ALARM Signal을 보냄

int pause();
* Signal이 발생되는 것을 기다림

Signal이 프로세스에 전달되면 그 signal은 main thread에서 처리함

// #########################

Kill Another Thread
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

int pthread_cancel(pthread_t tid);
* 현재 실행되고 있는 스레드를 종료시킴

int pthread_setcancelstate(int state, int *oldstate);
* state: PTHREAD_CANCEL_DISABLE | PTHREAD_CANCEL_ENABLE (default)
* enable하면 다른 스레드에서 이 스레드를 죽이는 것 허용

int pthread_setcanceltype(int type, int *oldtype);
* type: PTHREAD_CANCEL_ASYNCHRONOUS, PTHRAED_CANCEL_DEFERRED (default)
* DEFERRED하면 데드락 방지를 위해 thread가 죽는데 critical section 이후에 죽도록 함



























