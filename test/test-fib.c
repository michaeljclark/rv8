#include <stdio.h>
#include <assert.h>

int main()
{
	typedef long long u64;
	u64 first = 0, second = 1, next = 0;
	while (next < (1ULL << 62)) {
		next = first + second;
		first = second;
		second = next;
	}
	assert(next == 4660046610375530309ULL);
	return 0;
}
