#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	1024

long
filesize(FILE *fp)
{
	long	cur, size;

	cur = ftell(fp); // 현재 커서 포인터 위치 반환, 최초 진입이라면 맨 처음
	fseek(fp, 0L, SEEK_END); // 맨 처음 ~ 맨 끝: 파일의 크기
	size = ftell(fp);
	fseek(fp, cur, SEEK_SET); // 커서를 다시 현재 위치로
	return(size);
}

main(int argc, char *argv[])
{
	FILE	*src, *dst1, *dst2;
	char	buf[MAX_BUF];
	int		count, size;

	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	size = filesize(src) / 2;

// filesize의 반만큼 읽고 dst1에 씀
	while (size > 0)  {
		count = (size > MAX_BUF) ?  MAX_BUF : size; // size, MAX_BUF중 작은 값을 count로
		fread(buf, 1, count, src); 
		fwrite(buf, 1, count, dst1);
		size -= count;
	}

// 현재 커서위치(반 지난 곳)부터 끝까지 읽고 dst2에 씀
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  {
		fwrite(buf, 1, count, dst2);
	}

	fclose(src);
	fclose(dst1);
	fclose(dst2);
}