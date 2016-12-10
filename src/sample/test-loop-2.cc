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
	int *ent = arr;
	while (*ent != 0) {
		total += *ent;
		ent++;
	}
	printf("total=%d", total);
}
