#include <stdio.h>

struct astruct {
	int a;
	int b;
	int c;
};

struct astruct arr[] = {
	{ 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 9 },
	{ 10, 11, 12 },
	{ 13, 14, 15 },
	{ 16, 17, 18 },
	{ 19, 20, 21 },
	{ 22, 23, 24 }
};

int main()
{
	int total = 0;
	int elems = (sizeof(arr)/sizeof(struct astruct));
	for (int i = 0; i < elems; i++) {
		total += arr[i].a + arr[i].b + arr[i].c;
	}
	printf("total=%d", total);
}
