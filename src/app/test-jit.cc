//
//  test-jit.cc
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
#include <cstddef>
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

#include "asmjit.h"

#include "fusion-decode.h"
#include "fusion-emitter.h"
#include "fusion-tracer.h"
#include "fusion-runloop.h"

#include "assembler.h"
#include "jit.h"

using namespace riscv;

using proxy_jit_rv64imafdc = fusion_runloop<processor_proxy
	<processor_rv64imafdc_model<fusion_decode,processor_rv64imafd,mmu_proxy_rv64>>>;

struct rv_test_jit
{
	void test_1()
	{
		assembler as;
		proxy_jit_rv64imafdc emulator;

		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xde);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xad);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xbe);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xef);
		asm_ebreak(as);

		as.link(); /* required for unresolved labels */
		std::vector<u8> &buf = as.get_section(".text")->buf;

		/* step the interpreter */
		emulator.pc = (addr_t)buf.data();
		emulator.step(7);

		assert(emulator.ireg[rv_ireg_a0] == 0xdeadbeef);
	}

	void test_2()
	{
		assembler as;
		proxy_jit_rv64imafdc emulator;

		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xde);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xad);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xbe);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xef);
		asm_ebreak(as);

		as.link(); /* required for unresolved labels */
		std::vector<u8> &buf = as.get_section(".text")->buf;

		/* compile the program buffer trace */
		emulator.log = proc_log_jit_trace;
		emulator.pc = (addr_t)buf.data();
		emulator.start_trace();

		/* reset registers and run compiled trace */
		memset(&emulator.ireg[0], 0, sizeof(emulator.ireg));
		auto fn = emulator.trace_cache[(addr_t)buf.data()];
		fn(static_cast<processor_rv64imafd*>(&emulator));

		assert(emulator.ireg[rv_ireg_a0] == 0xdeadbeef);
	}
};

int main(int argc, char *argv[])
{
	rv_test_jit test;
	test.test_1();
	test.test_2();
}
