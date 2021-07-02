#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cmp(const void*a, const void*b) {
	return strcmp(*(const char**)a, *(const char**)b);
}

int main() {
	char *s[3];
	s[0] = (char*) malloc(sizeof(20));
	strcpy(s[0], "Zamfirescu");
	s[1] = (char*) malloc(sizeof(20));
	strcpy(s[1], "Petric");
	s[2] = (char*) malloc(sizeof(20));
	strcpy(s[2], "Vrancea");
	s[3] = (char*) malloc(sizeof(20));
	strcpy(s[3], "Petruc");
	for (int i = 0; i < 4; i++) {
		printf("%s\n", s[i]);
		printf("%d\n", sizeof(s[i]));
	}
	qsort(s, 4, sizeof(char*), cmp);
	for (int i = 0; i < 4; i++) {
		printf("%s\n", s[i]);
		printf("%d\n", sizeof(s[i]));
	}

}