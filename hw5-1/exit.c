#include <stdio.h>
#include <stdlib.h>

void
myexit1()
{
	printf("first exit handler\n");
}

void
myexit2()
{
	printf("second exit handler\n");
}

main()
{
	/* Add myexit2 to exit functions */
	if (atexit(myexit2) != 0)  {
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  {
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  {
		perror("atexit");
		exit(1);
	}

	printf("main is done\n");
}

// 호출하면 myexit을 두 번 출력 -> 똑같은 핸들러를 여러 번 등록해도 다 해줌
// 순서가 myexit(1) -> myexit(2)로 나옴: 스택 형태로 핸들러가 관리됨