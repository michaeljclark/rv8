#include <stdio.h>
#include <stdlib.h>

#define MALLOC_ITERS 1000
#define MALLOC_SIZE 1000

int main()
{
	for (size_t i = 0; i < MALLOC_ITERS; i++) {
		char *b = (char*)malloc(MALLOC_SIZE);
		if (!b) {
			printf("malloc appears to be broken\n");
			exit(9);
		}
		for (size_t j = 0; j < MALLOC_SIZE; j++) {
			b[j] = 0;
		}
		free(b);
	}
	printf("malloc appears to work\n");
	exit(0);
}
