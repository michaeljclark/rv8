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
	{ 22, 23, 24 },
	{ 0, 0, 0 }
};

int main()
{
	int total = 0;
	struct astruct *ent = arr;
	while (ent->a != 0) {
		total += ent->a + ent->b + ent->c;
		ent++;
	}
	printf("total=%d", total);
}
