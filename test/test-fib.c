#include <stdio.h>

int main()
{
	typedef long long u64;

	u64 first = 0, second = 1, next = 0;
	while (next < (1ULL << 62)) {
		next = first + second;
		first = second;
		second = next;
		printf("%llu\n",next);
	}

	return 0;
}
