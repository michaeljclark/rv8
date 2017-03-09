#include <stdio.h>

int arr[] = {
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
	int total = 0;
	for (size_t i = 0; i < 100000; i++) {
		for (size_t j = 0; j < (sizeof(arr)/sizeof(int)); j++) {
			total += arr[j];
		}
	}
	printf("total=%d\n", total);
}
