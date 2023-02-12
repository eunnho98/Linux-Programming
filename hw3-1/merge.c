#include <stdio.h>

#define	MAX_BUF	1024
// split한 binary file을 다시 합침
main(int argc, char *argv[])
{
	FILE	*src1, *src2, *dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}

	if ((src1 = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((src2 = fopen(argv[2], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

// binary file인 src1을 읽고 dst에 씀
	while ((count = fread(buf, 1, MAX_BUF, src1)) > 0)  {
		fwrite(buf, 1, count, dst);
	}

// binary file인 src2을 읽고 dst에 씀
	while ((count = fread(buf, 1, MAX_BUF, src2)) > 0)  {
		fwrite(buf, 1, count, dst);
	}

	fclose(src1);
	fclose(src2);
	fclose(dst);
}
