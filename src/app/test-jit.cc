//
//  rv-dump.cc
//

//
//  rv-sim.cc
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cinttypes>
#include <csignal>
#include <csetjmp>
#include <cerrno>
#include <cmath>
#include <cctype>
#include <cwchar>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <limits>
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include <deque>
#include <map>
#include <thread>
#include <atomic>
#include <type_traits>

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "histedit.h"

#include "host-endian.h"
#include "types.h"
#include "fmt.h"
#include "bits.h"
#include "sha512.h"
#include "format.h"
#include "meta.h"
#include "util.h"
#include "host.h"
#include "cmdline.h"
#include "config-parser.h"
#include "config-string.h"
#include "codec.h"
#include "strings.h"
#include "disasm.h"
#include "alu.h"
#include "fpu.h"
#include "pma.h"
#include "amo.h"
#include "processor-logging.h"
#include "processor-base.h"
#include "processor-impl.h"
#include "interp.h"
#include "processor-model.h"
#include "mmu-proxy.h"
#include "unknown-abi.h"
#include "processor-proxy.h"
#include "debug-cli.h"
#include "processor-runloop.h"
#include "assembler.h"
#include "jit.h"

using namespace riscv;

using proxy_emulator_rv64imafdc = processor_runloop<processor_proxy
	<processor_rv64imafdc_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;

struct rv_test_jit
{
	void run()
	{
		assembler as;
		proxy_emulator_rv64imafdc emulator;

		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xde);
		asm_slli_rv64i(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xad);
		asm_slli_rv64i(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xbe);
		asm_slli_rv64i(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xef);

		std::vector<u8> &buf = as.get_section(".text")->buf;
		emulator.pc = (uintptr_t)buf.data();
		emulator.step(7);

		u64 result = emulator.ireg[rv_ireg_a0];
		assert(result = 0xdeadbeef);
	}
};

int main(int argc, char *argv[])
{
	rv_test_jit test;
	test.run();
}
