#include <stdio.h>

// strlen with pointer
int strlen_p(char *str)
{
	int	len = 0;

	while (*(str + len) != '\0')  { // '\0'이 나올 때까지 len을 1씩 늘려 str의 다음 위치를 가리키게함
		len++;
	}
	return len;
}

// strlen with array
int strlen_a(char str[])
{
	int i;

	for (i = 0 ; str[i] != '\0' ; i++) // '\0'이 나올 때까지 i에 1씩 더함
		;
	return i;
}

// strcpy with pointer
void strcpy_p(char *dst, char *src)
{
	while (*src)  {
		*dst = *src; // 첫 번째 src 값을 dst 첫 번째 값에 삽입
    src++; // src의 바로 다음 위치
    dst++; // dst의 바로 다음 위치
	}
	*dst = *src; // '\0'도 복사
}

// strcpy with array
void strcpy_a(char dst[], char src[])
{
	int i;

	for (i = 0 ; src[i] != '\0' ; i++)
		dst[i] = src[i]; // i값에 따라 src i 값을 dst i 위치 값에 복서 
	dst[i] = src[i]; // '\0'도 복사
}

// strcat with pointer
void strcat_p(char *dst, char *src)
{
	while (*dst++) // dst의 마지막 위치까지 이동
		;
	*dst--; // '\0'값 삭제
	while (*src)  {
		*dst++ = *src++; // 후위 연산자이므로 값을 적용한 후 1을 더함
	}
	*dst = *src; // '\0'값 복사
}

// strcat with array
void
strcat_a(char dst[], char src[])
{
	int i, j;

	for (i = 0 ; dst[i] != '\0' ; i++) // dst의 끝-1 까지 이동
		;
	for (j = 0 ; src[j] != '\0' ; j++)
		dst[i+j] = src[j]; // dst 뒤에 src 0번쨰 부터 붙임
	dst[i+j] = '\0'; // '\0'도 붙임
}

// strcmp with pointer
int
strcmp_p(char *dst, char *src) {
	while(*dst != '\0' || *src != '\0') { // 둘 다 '\0'에 다다르면 같은 문자열
		if(*dst > *src) { // 아스키코드 값 비교, 앞의 문자가 더 크면 1
			return 1;
		}
		else if(*dst < *src) { // 아스키코드 값 비교, 앞의 문자가 더 작으면 -1
			return -1;
		}
		else { // 같으면 다음 위치 값 비교
			dst++;
			src++;
		}
	}
	return 0; // 같은 문자열
}

int
strcmp_a(char dst[], char src[]) {
	int i = 0;
	while(dst[i] != '\0' || src[i] != '\0') { // 둘 다 '\0'에 다다르면 같은 문자열
		if(dst[i] > src[i]) { // 아스키코드 값 비교, 앞의 문자가 더 크면 1
			return 1;
		}
		else if (dst[i] < src[i]) { // 아스키코드 값 비교, 앞의 문자가 더 작으면 -1
			return -1;
		}
		else { // 같으면 다음 위치 값 비교
			i++;
		}
	}
	return 0; // 같은 문자열
}

main()
{
	int		len1, len2;
	char	str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);

	printf("strcmp: p=%d, a=%d\n", strcmp_p("Hello", "Hellu"), strcmp_a("Hello", "Hellu"));
}
