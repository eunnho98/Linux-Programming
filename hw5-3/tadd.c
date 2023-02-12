// 1부터 100까지 합, 두 개의 thread를 생성하여 각각
// 1~50, 51~100까지의 합을 구하고
// 메인 스레드가 각 스레드가 종료되기를 기다린 후 각각의 합을 더하여 출력
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void
getSum50(void* sum1) {
  int i;
  for(i = 1; i <= 50; i++)
    *(int*)sum1 += i; // *(int*): int 포인터 형으로 바꾸고 그 값을 가리키기
  pthread_exit(NULL);
}

void
getSum100(void* sum2) {
  int i;
  for(i = 51; i <= 100; i++)
    *(int*)sum2 += i;
  pthread_exit(NULL);
}

main() {
  pthread_t tid1, tid2;
  int sum1 = 0;
  int sum2 = 0;

// c에는 Call by Ref가 없으므로 주소값을 넘김
  if(pthread_create(&tid1, NULL, (void *)getSum50, (void *)&sum1) < 0) {
    perror('pthread_create');
    exit(1);
  }

  if(pthread_create(&tid2, NULL, (void *)getSum100, (void *)&sum2) < 0) {
    perror('pthread_create');
    exit(1);
  }

// Wait
  if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

  printf("1 to 100 Sum=%d\n", sum1+sum2 );
}
