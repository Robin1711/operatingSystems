#include <stdlib.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[]) {
	printf("starting testy\n");
	int in;
	for (int i = 0; i < 4; i++) {
		scanf("%d", &in);
		printf("%d\n", in);
	}
	return 0;
}
