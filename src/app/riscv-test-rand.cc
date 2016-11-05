//
//  riscv-test-rand.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <map>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"
#include "riscv-host.h"
#include "riscv-util.h"

using namespace riscv;


int main(int argc, const char * argv[])
{
	host_cpu &cpu = host_cpu::get_instance();
	uint32_t seed = cpu.get_random_seed();
	debug("seed=%08x", seed);
}
