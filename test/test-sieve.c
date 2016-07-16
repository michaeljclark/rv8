#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int n = 10000;

	int *prime = malloc(n * n * sizeof(int));
	if (!prime) {
		printf("malloc failed\n");
		return 0;
	}

	for (int i = 0; i < n; i++) {
		prime[i] = i;
	}

	for (int i = 2; i < n; i++) {
		if (prime[i]!=0) { 
			for(int j = 2; j < n; j++) {
				prime[j * prime[i]] = 0;
				if(prime[i] * j > n) break;
			}
		}
	}

	int largest_prime = 0;
	for(int i = 0; i < n; i++) {
		if(prime[i] != 0) largest_prime = i;
	}

	printf("%d\n", largest_prime);

	return 0;
}