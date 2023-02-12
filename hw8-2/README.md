IPC 실습내용 중 sipc1.c sipc2.c를 다음과 같이 수정
busy waiting을 없애고, semaphore를 통해 동기화시킴
buisy waiting을 위해 사용했던 flag 4 bytes를 제외하고, shared memory 부분은 모두 데이터 전달을 위해 사용
[Hint] Process간 동기화 이므로, System V semaphore를 사용해야 하며, POSIX semaphore API처럼 만든 semlib.c의 함수를 사용하면 됨
