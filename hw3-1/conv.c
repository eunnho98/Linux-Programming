#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "record.h"


main(int argc, char* argv[]) {
	FILE *src, *dst;
	Record rec;
	
	if(argc != 3) {
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	if((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}
	
	if((dst = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	}

// 한 줄씩 읽고 \n의 포인터를 찾음
// binary file로의 변환
	while(fgets(rec.stud, SMAX, src) != NULL) {
		*strchr(rec.stud, '\n') = '\0';
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';

// dst에 rec 구조체를 씀
		fwrite(&rec, sizeof(rec), 1, dst);
	}
	fclose(src);
	fclose(dst);
}
