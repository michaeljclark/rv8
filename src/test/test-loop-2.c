#include <stdio.h>

long arr[] = {
	1, 2, 3,
	4, 5, 6,
	7, 8, 9,
	10, 11, 12,
	13, 14, 15,
	16, 17, 18,
	19, 20, 21,
	22, 23, 24,
	0
};

int main()
{
	long total = 0;
	for (size_t i = 0; i < 100000; i++) {
		for (size_t j = 0; j < (sizeof(arr)/sizeof(long)); j++) {
			total += arr[j];
		}
	}
	printf("total=%ld\n", total);
}
