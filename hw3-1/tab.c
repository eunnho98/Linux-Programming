#include <stdio.h>

enum { FALSE, TRUE };
// tab 간격을 8이아닌 4로 만들기
void
conv(char *fname)
{
	FILE	*src, *dst;
	char	fdest[40];
	int		ch, first;

	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	}

	strcpy(fdest, fname);
	strcat(fdest, ".t"); // fdest = fdest.t로 만들기
	if ((dst = fopen(fdest, "wt")) == NULL)  { // 쓰기를 위한 파일
		perror("fopen");
		return;
	}

	first = TRUE;
	while ((ch = fgetc(src)) != EOF)  { // 문자를 하나씩 읽음
		if (first && ch == '\t')  { // tab이고 first가 TRUE이면
			fputc(' ', dst); // 공백 네 칸을 dst에 씀
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else  {
			fputc(ch, dst);
			if (first) // tab 이후이므로 first를 FALSE로
				first = FALSE;
			if (ch == '\n') // 줄바꿈이 일어나면 다시 first를 TRUE로
				first = TRUE;
		}
	}

	fclose(src);
	fclose(dst);
}

main(int argc, char *argv[])
{
	while (--argc)  {
		conv(argv[argc]); // 입력한 text 파일들을 binary 파일로 변환
	}
}
