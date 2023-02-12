Parent process가 child process의 종료시, SIGCHLD signal을 asynchronous하게 처리하는 프로그램을 작성
즉, child process의 종료를 synchronous하게 처리하기 위해서는 wait() system call을 사용하고, asynchronous하게 처리하기 위해서는 signal을 사용함을 익힘
[Hint] Processes and Threads의 wait.c 실습 내용에서 wait 부분을 while (1) ; 로 변경하고, fork() 이전에 signal handler를 등록
