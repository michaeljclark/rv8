//
//  riscv-test-operators.cc
//

#undef NDEBUG

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <random>
#include <type_traits>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-operators.h"

using namespace riscv;

int main()
{
	assert(u128(0,0) == u128(0,0));
	assert(!(u128(1,1) == u128(0,0)));
	assert(u128(1,1) == u128(1,1));
	assert(!(u128(1,1) == u128(2,2)));

	assert(!(u128(0,0) != u128(0,0)));
	assert(u128(1,1) != u128(0,0));
	assert(!(u128(1,1) != u128(1,1)));
	assert(u128(1,1) != u128(2,2));

	assert(s128(0,0) == s128(0,0));
	assert(!(s128(1,1) == s128(0,0)));
	assert(s128(-1,1) == s128(-1,1));
	assert(!(s128(-1,1) == s128(-2,2)));

	assert(!(s128(0,0) != s128(0,0)));
	assert(s128(1,1) != s128(0,0));
	assert(!(s128(-1,1) != s128(-1,1)));
	assert(s128(-1,1) != s128(-2,2));

	return 0;
}
