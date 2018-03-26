#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
	printf("Our UTC time = %d\n", utctime());
	int t;
	time(&t);
	printf("Standard implemented time = %d\n", t);
}
