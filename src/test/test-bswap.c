#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

extern uint32_t __bswap32(uint32_t val);
extern uint64_t __bswap64(uint64_t val);

void test_bswap32(uint32_t val)
{
	assert(__builtin_bswap32(val) == __bswap32(val));
}

void test_bswap64(uint64_t val)
{
	assert(__builtin_bswap64(val) == __bswap64(val));
}

int main()
{
	test_bswap32(0x01020304);
	test_bswap64(0x0102030405060708ULL);
}
