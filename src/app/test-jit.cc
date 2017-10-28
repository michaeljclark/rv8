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

#include "dense_hash_map"

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <termios.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <sys/resource.h>

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
#include "color.h"
#include "codec.h"
#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"
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
#include "mmap-core.h"
#include "unknown-abi.h"
#include "processor-histogram.h"
#include "processor-proxy.h"
#include "debug-cli.h"

#include "asmjit.h"

#include "jit-decode.h"
#include "jit-emitter-rv32.h"
#include "jit-emitter-rv64.h"
#include "jit-fusion.h"
#include "jit-tracer.h"
#include "jit-regalloc.h"
#include "jit-runloop.h"

#include "assembler.h"
#include "jit.h"

using namespace riscv;

using proxy_model_rv32imafdc = processor_rv32imafdc_model<
	jit_decode, processor_rv32imafd, mmu_proxy_rv32>;
using proxy_model_rv64imafdc = processor_rv64imafdc_model<
	jit_decode, processor_rv64imafd, mmu_proxy_rv64>;

using proxy_jit_rv32imafdc = jit_runloop<
	processor_proxy<proxy_model_rv32imafdc>,
	jit_tracer<proxy_model_rv32imafdc,jit_isa_rv32>,
	jit_emitter_rv32<proxy_model_rv32imafdc>>;
using proxy_jit_rv64imafdc = jit_runloop<
	processor_proxy<proxy_model_rv64imafdc>,
	jit_tracer<proxy_model_rv64imafdc,jit_isa_rv64>,
	jit_emitter_rv64<proxy_model_rv64imafdc>>;

template <typename P>
struct rv_test_jit
{
	bool memory_registers = false;
	int total_tests = 0;
	int tests_passed = 0;

	rv_test_jit() : total_tests(0), tests_passed(0) {}

	void run_test(const char* test_name, P &proc, addr_t pc, size_t step)
	{
		printf("\n=========================================================\n");
		printf("TEST: %s\n", test_name);
		typename P::ireg_t save_regs[P::ireg_count];
		size_t regfile_size = sizeof(typename P::ireg_t) * P::ireg_count;

		/* create 256MB RAM at 256MB */
		proc.mmu.mem->brk = proc.mmu.mem->heap_begin = proc.mmu.mem->heap_end = 0x10000000;
		proc.ireg[rv_ireg_a0] = 0x20000000;
		abi_sys_brk(proc);

		/* clear registers */
		memset(&proc.ireg[0], 0, regfile_size);

		/* step the interpreter */
		printf("\n--[ interp ]---------------\n");
		proc.log = proc_log_inst;
		proc.pc = pc;
		proc.step(step);

		/* save and reset registers */
		memcpy(&save_regs[0], &proc.ireg[0], regfile_size);
		memset(&proc.ireg[0], 0, regfile_size);

		/* compile the program buffer trace */
		printf("\n--[ jit ]------------------\n");
		proc.memory_registers = memory_registers;
		proc.log = proc_log_jit_trace;
		proc.pc = pc;
		proc.jit_trace();

		/* reset registers */
		memset(&proc.ireg[0], 0, regfile_size);

		/* run compiled trace */
		proc.jit_exec(proc, pc);

		/* print result */
		printf("\n--[ result ]---------------\n");
		bool pass = true;
		for (size_t i = 0; i < P::ireg_count; i++) {
			if (save_regs[i].r.xu.val != proc.ireg[i].r.xu.val) {
				pass = false;
				printf("ERROR interp-%s=0x%016llx jit-%s=0x%016llx\n",
					rv_ireg_name_sym[i], save_regs[i].r.xu.val,
					rv_ireg_name_sym[i], proc.ireg[i].r.xu.val);
			}
		}
		printf("%s\n", pass ? "PASS" : "FAIL");
		if (pass) tests_passed++;
		total_tests++;
	}

	void test_addi_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0xde);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 1);
	}

	void test_addi_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_zero, rv_ireg_a0, 0xde);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 1);
	}

	void test_addi_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_zero, rv_ireg_zero, 0xde);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 1);
	}

	void test_addi_4()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0xde);
		asm_addi(as, rv_ireg_s10, rv_ireg_s11, 0xde);
		asm_addi(as, rv_ireg_s10, rv_ireg_s10, 0xde);
		asm_addi(as, rv_ireg_a0, rv_ireg_s10, 0xde);
		asm_addi(as, rv_ireg_s9, rv_ireg_a0, 0xde);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_slti_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 1);
		asm_slti(as, rv_ireg_a1, rv_ireg_zero, 0);
		asm_slti(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_slti(as, rv_ireg_a3, rv_ireg_a0, 1);
		asm_slti(as, rv_ireg_a4, rv_ireg_a0, 2);
		asm_slti(as, rv_ireg_a5, rv_ireg_a0, -1);
		asm_slti(as, rv_ireg_a6, rv_ireg_a0, -2);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_slti_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 1);
		asm_slti(as, rv_ireg_s1, rv_ireg_zero, 0);
		asm_slti(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_slti(as, rv_ireg_s3, rv_ireg_s0, 1);
		asm_slti(as, rv_ireg_s4, rv_ireg_s0, 2);
		asm_slti(as, rv_ireg_s5, rv_ireg_a0, -1);
		asm_slti(as, rv_ireg_s6, rv_ireg_a0, -2);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_sltiu_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, -1);
		asm_sltiu(as, rv_ireg_a1, rv_ireg_zero, 0);
		asm_sltiu(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_sltiu(as, rv_ireg_a3, rv_ireg_a0, 1);
		asm_sltiu(as, rv_ireg_a4, rv_ireg_a0, 2);
		asm_sltiu(as, rv_ireg_a5, rv_ireg_a0, -1);
		asm_sltiu(as, rv_ireg_a6, rv_ireg_a0, -2);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_sltiu_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, -1);
		asm_sltiu(as, rv_ireg_s1, rv_ireg_zero, 0);
		asm_sltiu(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_sltiu(as, rv_ireg_s3, rv_ireg_s0, 1);
		asm_sltiu(as, rv_ireg_s4, rv_ireg_s0, 2);
		asm_sltiu(as, rv_ireg_s5, rv_ireg_a0, -1);
		asm_sltiu(as, rv_ireg_s6, rv_ireg_a0, -2);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_add_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_add(as, rv_ireg_a0, rv_ireg_zero, rv_ireg_a1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_add_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_add(as, rv_ireg_a0, rv_ireg_a1, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_add_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x1);
		asm_add(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_add_4()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_add(as, rv_ireg_s10, rv_ireg_zero, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_add_5()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_add(as, rv_ireg_s10, rv_ireg_s11, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_add_6()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s11, rv_ireg_zero, 1);
		asm_add(as, rv_ireg_a0, rv_ireg_s9, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_add_7()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s11, rv_ireg_zero, 1);
		asm_add(as, rv_ireg_s9, rv_ireg_a0, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_add_8()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s11, rv_ireg_zero, 1);
		asm_add(as, rv_ireg_s9, rv_ireg_s10, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_sub_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_sub(as, rv_ireg_a0, rv_ireg_zero, rv_ireg_a1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_sub_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_sub(as, rv_ireg_a0, rv_ireg_a1, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_sub_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x1);
		asm_sub(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_sub_4()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_sub(as, rv_ireg_s10, rv_ireg_zero, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_sub_5()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_sub(as, rv_ireg_s10, rv_ireg_s11, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_sub_6()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s11, rv_ireg_zero, 1);
		asm_sub(as, rv_ireg_a0, rv_ireg_s9, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_sub_7()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s11, rv_ireg_zero, 1);
		asm_sub(as, rv_ireg_s9, rv_ireg_a0, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_sub_8()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s11, rv_ireg_zero, 1);
		asm_sub(as, rv_ireg_s9, rv_ireg_s10, rv_ireg_s11);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_slt_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 1);
		asm_addi(as, rv_ireg_a2, rv_ireg_zero, 2);
		asm_addi(as, rv_ireg_a3, rv_ireg_zero, -1);
		asm_addi(as, rv_ireg_a4, rv_ireg_zero, -1);
		asm_slt(as, rv_ireg_t0, rv_ireg_zero, rv_ireg_a0);
		asm_slt(as, rv_ireg_t1, rv_ireg_a0, rv_ireg_a1);
		asm_slt(as, rv_ireg_t2, rv_ireg_a0, rv_ireg_a2);
		asm_slt(as, rv_ireg_s0, rv_ireg_a0, rv_ireg_a3);
		asm_slt(as, rv_ireg_s1, rv_ireg_a0, rv_ireg_a4);
		asm_slt(as, rv_ireg_s2, rv_ireg_a0, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 11);
	}

	void test_slt_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0);
		asm_addi(as, rv_ireg_s1, rv_ireg_zero, 1);
		asm_addi(as, rv_ireg_s2, rv_ireg_zero, 2);
		asm_addi(as, rv_ireg_s3, rv_ireg_zero, -1);
		asm_addi(as, rv_ireg_s4, rv_ireg_zero, -1);
		asm_slt(as, rv_ireg_t0, rv_ireg_zero, rv_ireg_s0);
		asm_slt(as, rv_ireg_t1, rv_ireg_s0, rv_ireg_s1);
		asm_slt(as, rv_ireg_t2, rv_ireg_s0, rv_ireg_s2);
		asm_slt(as, rv_ireg_s0, rv_ireg_s0, rv_ireg_s3);
		asm_slt(as, rv_ireg_s1, rv_ireg_s0, rv_ireg_s4);
		asm_slt(as, rv_ireg_s2, rv_ireg_s0, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 11);
	}

	void test_sltu_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 1);
		asm_addi(as, rv_ireg_a2, rv_ireg_zero, 2);
		asm_addi(as, rv_ireg_a3, rv_ireg_zero, -1);
		asm_addi(as, rv_ireg_a4, rv_ireg_zero, -1);
		asm_sltu(as, rv_ireg_t0, rv_ireg_zero, rv_ireg_a0);
		asm_sltu(as, rv_ireg_t1, rv_ireg_a0, rv_ireg_a1);
		asm_sltu(as, rv_ireg_t2, rv_ireg_a0, rv_ireg_a2);
		asm_sltu(as, rv_ireg_s0, rv_ireg_a0, rv_ireg_a3);
		asm_sltu(as, rv_ireg_s1, rv_ireg_a0, rv_ireg_a4);
		asm_sltu(as, rv_ireg_s2, rv_ireg_a0, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 11);
	}

	void test_sltu_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0);
		asm_addi(as, rv_ireg_s1, rv_ireg_zero, 1);
		asm_addi(as, rv_ireg_s2, rv_ireg_zero, 2);
		asm_addi(as, rv_ireg_s3, rv_ireg_zero, -1);
		asm_addi(as, rv_ireg_s4, rv_ireg_zero, -1);
		asm_sltu(as, rv_ireg_t0, rv_ireg_zero, rv_ireg_s0);
		asm_sltu(as, rv_ireg_t1, rv_ireg_s0, rv_ireg_s1);
		asm_sltu(as, rv_ireg_t2, rv_ireg_s0, rv_ireg_s2);
		asm_sltu(as, rv_ireg_s0, rv_ireg_s0, rv_ireg_s3);
		asm_sltu(as, rv_ireg_s1, rv_ireg_s0, rv_ireg_s4);
		asm_sltu(as, rv_ireg_s2, rv_ireg_s0, rv_ireg_zero);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 11);
	}

	void test_andi_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_andi(as, rv_ireg_a0, rv_ireg_a0, 0x3ff);
		asm_andi(as, rv_ireg_a1, rv_ireg_a0, 0x3ff);
		asm_andi(as, rv_ireg_a2, rv_ireg_zero, 0x3ff);
		asm_andi(as, rv_ireg_zero, rv_ireg_a2, 0x3ff);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_andi_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0x7ff);
		asm_andi(as, rv_ireg_s0, rv_ireg_s0, 0x3ff);
		asm_andi(as, rv_ireg_s1, rv_ireg_s0, 0x3ff);
		asm_andi(as, rv_ireg_s1, rv_ireg_a0, 0x1ff);
		asm_andi(as, rv_ireg_a0, rv_ireg_s1, 0xff);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_ori_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_ori(as, rv_ireg_a0, rv_ireg_a0, 0x3ff);
		asm_ori(as, rv_ireg_a1, rv_ireg_a0, 0x3ff);
		asm_ori(as, rv_ireg_a2, rv_ireg_zero, 0x3ff);
		asm_ori(as, rv_ireg_zero, rv_ireg_a2, 0x3ff);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_ori_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0x7ff);
		asm_ori(as, rv_ireg_s0, rv_ireg_s0, 0x3ff);
		asm_ori(as, rv_ireg_s1, rv_ireg_s0, 0x3ff);
		asm_ori(as, rv_ireg_s1, rv_ireg_a0, 0x1ff);
		asm_ori(as, rv_ireg_a0, rv_ireg_s1, 0xff);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_xori_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_xori(as, rv_ireg_a0, rv_ireg_a0, 0x3ff);
		asm_xori(as, rv_ireg_a1, rv_ireg_a0, 0x3ff);
		asm_xori(as, rv_ireg_a2, rv_ireg_zero, 0x3ff);
		asm_xori(as, rv_ireg_zero, rv_ireg_a2, 0x3ff);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_xori_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0x7ff);
		asm_xori(as, rv_ireg_s0, rv_ireg_s0, 0x3ff);
		asm_xori(as, rv_ireg_s1, rv_ireg_s0, 0x3ff);
		asm_xori(as, rv_ireg_s1, rv_ireg_a0, 0x1ff);
		asm_xori(as, rv_ireg_a0, rv_ireg_s1, 0xff);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_and_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_a3, rv_ireg_zero, 0x3ff);
		asm_and(as, rv_ireg_a0, rv_ireg_a0, rv_ireg_a3);
		asm_and(as, rv_ireg_a1, rv_ireg_a0, rv_ireg_a3);
		asm_and(as, rv_ireg_a2, rv_ireg_zero, rv_ireg_a3);
		asm_and(as, rv_ireg_zero, rv_ireg_a2, rv_ireg_a3);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 6);
	}

	void test_and_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s3, rv_ireg_zero, 0x3ff);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x1ff);
		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0xff);
		asm_and(as, rv_ireg_s0, rv_ireg_s0, rv_ireg_s3);
		asm_and(as, rv_ireg_s1, rv_ireg_s0, rv_ireg_s3);
		asm_and(as, rv_ireg_s1, rv_ireg_a0, rv_ireg_a1);
		asm_and(as, rv_ireg_a0, rv_ireg_s1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 8);
	}

	void test_or_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_a3, rv_ireg_zero, 0x3ff);
		asm_or(as, rv_ireg_a0, rv_ireg_a0, rv_ireg_a3);
		asm_or(as, rv_ireg_a1, rv_ireg_a0, rv_ireg_a3);
		asm_or(as, rv_ireg_a2, rv_ireg_zero, rv_ireg_a3);
		asm_or(as, rv_ireg_zero, rv_ireg_a2, rv_ireg_a3);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 6);
	}

	void test_or_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s3, rv_ireg_zero, 0x3ff);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x1ff);
		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0xff);
		asm_or(as, rv_ireg_s0, rv_ireg_s0, rv_ireg_s3);
		asm_or(as, rv_ireg_s1, rv_ireg_s0, rv_ireg_s3);
		asm_or(as, rv_ireg_s1, rv_ireg_a0, rv_ireg_a1);
		asm_or(as, rv_ireg_a0, rv_ireg_s1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 8);
	}

	void test_xor_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_a3, rv_ireg_zero, 0x3ff);
		asm_xor(as, rv_ireg_a0, rv_ireg_a0, rv_ireg_a3);
		asm_xor(as, rv_ireg_a1, rv_ireg_a0, rv_ireg_a3);
		asm_xor(as, rv_ireg_a2, rv_ireg_zero, rv_ireg_a3);
		asm_xor(as, rv_ireg_zero, rv_ireg_a2, rv_ireg_a3);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 6);
	}

	void test_xor_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0x7ff);
		asm_addi(as, rv_ireg_s3, rv_ireg_zero, 0x3ff);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x1ff);
		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0xff);
		asm_xor(as, rv_ireg_s0, rv_ireg_s0, rv_ireg_s3);
		asm_xor(as, rv_ireg_s1, rv_ireg_s0, rv_ireg_s3);
		asm_xor(as, rv_ireg_s1, rv_ireg_a0, rv_ireg_a1);
		asm_xor(as, rv_ireg_a0, rv_ireg_s1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 8);
	}

	void test_slli_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xde);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xad);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xbe);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xef);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_slli_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xde);
		asm_slli(as, rv_ireg_s0, rv_ireg_s0, 8);
		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xad);
		asm_slli(as, rv_ireg_s0, rv_ireg_s0, 8);
		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xbe);
		asm_slli(as, rv_ireg_s0, rv_ireg_s0, 8);
		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xef);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_slli_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_a0, 0xde);
		asm_slli(as, rv_ireg_a2, rv_ireg_a1, 8);
		asm_addi(as, rv_ireg_a3, rv_ireg_a2, 0xad);
		asm_slli(as, rv_ireg_s0, rv_ireg_a3, 8);
		asm_addi(as, rv_ireg_s1, rv_ireg_s0, 0xbe);
		asm_slli(as, rv_ireg_s2, rv_ireg_s1, 8);
		asm_addi(as, rv_ireg_s3, rv_ireg_s2, 0xef);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_slli_4()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 0xde);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xad);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xbe);
		asm_slli(as, rv_ireg_a0, rv_ireg_a0, 8);
		asm_addi(as, rv_ireg_a0, rv_ireg_a0, 0xef);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_slli_5()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 0xde);
		asm_slli(as, rv_ireg_s0, rv_ireg_s0, 8);
		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xad);
		asm_slli(as, rv_ireg_s0, rv_ireg_s0, 8);
		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xbe);
		asm_slli(as, rv_ireg_s0, rv_ireg_s0, 8);
		asm_addi(as, rv_ireg_s0, rv_ireg_s0, 0xef);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_slli_6()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0xde);
		asm_slli(as, rv_ireg_a2, rv_ireg_a1, 8);
		asm_addi(as, rv_ireg_a3, rv_ireg_a2, 0xad);
		asm_slli(as, rv_ireg_s0, rv_ireg_a3, 8);
		asm_addi(as, rv_ireg_s1, rv_ireg_s0, 0xbe);
		asm_slli(as, rv_ireg_s2, rv_ireg_s1, 8);
		asm_addi(as, rv_ireg_s3, rv_ireg_s2, 0xef);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 7);
	}

	void test_srli_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_a2, rv_ireg_a1, 56);
		asm_srli(as, rv_ireg_a3, rv_ireg_a2, 56);
		asm_srli(as, rv_ireg_a2, rv_ireg_a2, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srli_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_s0, rv_ireg_a1, 56);
		asm_srli(as, rv_ireg_a3, rv_ireg_s0, 56);
		asm_srli(as, rv_ireg_s0, rv_ireg_s0, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srli_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_s2, rv_ireg_s1, 56);
		asm_srli(as, rv_ireg_s3, rv_ireg_s2, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_srli_4()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_a0, rv_ireg_s1, 56);
		asm_srli(as, rv_ireg_s3, rv_ireg_a0, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_srai_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_a2, rv_ireg_a1, 56);
		asm_srai(as, rv_ireg_a3, rv_ireg_a2, 56);
		asm_srai(as, rv_ireg_a2, rv_ireg_a2, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srai_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_s0, rv_ireg_a1, 56);
		asm_srai(as, rv_ireg_a3, rv_ireg_s0, 56);
		asm_srai(as, rv_ireg_s0, rv_ireg_s0, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srai_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_s2, rv_ireg_s1, 56);
		asm_srai(as, rv_ireg_s3, rv_ireg_s2, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}

	void test_srai_4()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s1, rv_ireg_zero, 0x80);
		asm_slli(as, rv_ireg_a0, rv_ireg_s1, 56);
		asm_srai(as, rv_ireg_s3, rv_ireg_a0, 56);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 3);
	}


	void test_addiw_1()
	{
		P proc;
		assembler as;

		asm_addiw(as, rv_ireg_a0, rv_ireg_zero, -2);
		asm_addiw(as, rv_ireg_a0, rv_ireg_a0, 1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_addiw_2()
	{
		P proc;
		assembler as;

		asm_addiw(as, rv_ireg_s0, rv_ireg_zero, -2);
		asm_addiw(as, rv_ireg_s0, rv_ireg_s0, 1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_addiw_3()
	{
		P proc;
		assembler as;

		asm_addiw(as, rv_ireg_s0, rv_ireg_zero, -2);
		asm_addiw(as, rv_ireg_s0, rv_ireg_a0, 1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_addiw_4()
	{
		P proc;
		assembler as;

		asm_addiw(as, rv_ireg_a0, rv_ireg_zero, -2);
		asm_addiw(as, rv_ireg_a0, rv_ireg_s0, 1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_slliw_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_slliw(as, rv_ireg_a1, rv_ireg_a1, 12);
		asm_slliw(as, rv_ireg_a2, rv_ireg_a1, 12);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_slliw_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_slliw(as, rv_ireg_s11, rv_ireg_s11, 12);
		asm_slliw(as, rv_ireg_a0, rv_ireg_s11, 12);
		asm_slliw(as, rv_ireg_s10, rv_ireg_a0, 12);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_srliw_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_srliw(as, rv_ireg_a1, rv_ireg_a1, 12);
		asm_srliw(as, rv_ireg_a2, rv_ireg_a1, 12);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srliw_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_srliw(as, rv_ireg_s11, rv_ireg_s11, 12);
		asm_srliw(as, rv_ireg_a0, rv_ireg_s11, 12);
		asm_srliw(as, rv_ireg_s10, rv_ireg_a0, 12);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sraiw_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sraiw(as, rv_ireg_a1, rv_ireg_a1, 12);
		asm_sraiw(as, rv_ireg_a2, rv_ireg_a1, 12);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sraiw_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_sraiw(as, rv_ireg_s11, rv_ireg_s11, 12);
		asm_sraiw(as, rv_ireg_a0, rv_ireg_s11, 12);
		asm_sraiw(as, rv_ireg_s10, rv_ireg_a0, 12);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sll_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_sll(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_sll(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sll_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, 0x7ff);
		asm_sll(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_s0);
		asm_sll(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_s0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sll_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s9, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_s10, rv_ireg_zero, 0x7ff);
		asm_sll(as, rv_ireg_s11, rv_ireg_s11, rv_ireg_s9);
		asm_sll(as, rv_ireg_a0, rv_ireg_s11, rv_ireg_s9);
		asm_sll(as, rv_ireg_s10, rv_ireg_a0, rv_ireg_s9);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_srl_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_srl(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_srl(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srl_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_srl(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_s0);
		asm_srl(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_s0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srl_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s9, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_srl(as, rv_ireg_s11, rv_ireg_s11, rv_ireg_s9);
		asm_srl(as, rv_ireg_a0, rv_ireg_s11, rv_ireg_s9);
		asm_srl(as, rv_ireg_s10, rv_ireg_a0, rv_ireg_s9);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sra_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sra(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_sra(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sra_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sra(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_s0);
		asm_sra(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_s0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sra_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s9, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_sra(as, rv_ireg_s11, rv_ireg_s11, rv_ireg_s9);
		asm_sra(as, rv_ireg_a0, rv_ireg_s11, rv_ireg_s9);
		asm_sra(as, rv_ireg_s10, rv_ireg_a0, rv_ireg_s9);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sllw_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sllw(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_sllw(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sllw_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sllw(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_s0);
		asm_sllw(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_s0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sllw_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s9, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_sllw(as, rv_ireg_s11, rv_ireg_s11, rv_ireg_s9);
		asm_sllw(as, rv_ireg_a0, rv_ireg_s11, rv_ireg_s9);
		asm_sllw(as, rv_ireg_s10, rv_ireg_a0, rv_ireg_s9);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_srlw_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_srlw(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_srlw(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srlw_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_srlw(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_s0);
		asm_srlw(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_s0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_srlw_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s9, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_srlw(as, rv_ireg_s11, rv_ireg_s11, rv_ireg_s9);
		asm_srlw(as, rv_ireg_a0, rv_ireg_s11, rv_ireg_s9);
		asm_srlw(as, rv_ireg_s10, rv_ireg_a0, rv_ireg_s9);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sraw_1()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_a0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sraw(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_a0);
		asm_sraw(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_a0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sraw_2()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s0, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_a1, rv_ireg_zero, -1);
		asm_sraw(as, rv_ireg_a1, rv_ireg_a1, rv_ireg_s0);
		asm_sraw(as, rv_ireg_a2, rv_ireg_a1, rv_ireg_s0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 4);
	}

	void test_sraw_3()
	{
		P proc;
		assembler as;

		asm_addi(as, rv_ireg_s9, rv_ireg_zero, 12);
		asm_addi(as, rv_ireg_s10, rv_ireg_zero, -1);
		asm_sraw(as, rv_ireg_s11, rv_ireg_s11, rv_ireg_s9);
		asm_sraw(as, rv_ireg_a0, rv_ireg_s11, rv_ireg_s9);
		asm_sraw(as, rv_ireg_s10, rv_ireg_a0, rv_ireg_s9);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_lui_1()
	{
		P proc;
		assembler as;

		asm_lui(as, rv_ireg_a0, 0x7fffffff);
		asm_lui(as, rv_ireg_a1, -1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_lui_2()
	{
		P proc;
		assembler as;

		asm_lui(as, rv_ireg_s0, 0x7fffffff);
		asm_lui(as, rv_ireg_s1, -1);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 2);
	}

	void test_load_imm_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0xfeedcafebabe);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 6);
	}

	void test_load_imm_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0xfeedcafebabe);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 6);
	}

	void test_sd_ld_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sd(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_ld(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sd_ld_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sd(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_ld(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sd_ld_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sd(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_ld(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sd_ld_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sd(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_ld(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lw_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sw(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_lw(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lw_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sw(as, rv_ireg_s0, rv_ireg_s1, 0);
		asm_lw(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lw_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sw(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_lw(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lw_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sw(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_lw(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lwu_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sw(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_lwu(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lwu_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sw(as, rv_ireg_s0, rv_ireg_s1, 0);
		asm_lwu(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lwu_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sw(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_lwu(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sw_lwu_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sw(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_lwu(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lh_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sh(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_lh(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lh_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sh(as, rv_ireg_s0, rv_ireg_s1, 0);
		asm_lh(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lh_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sh(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_lh(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lh_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sh(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_lh(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lhu_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sh(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_lhu(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lhu_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sh(as, rv_ireg_s0, rv_ireg_s1, 0);
		asm_lhu(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lhu_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sh(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_lhu(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sh_lhu_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sh(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_lhu(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lb_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sb(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_lb(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lb_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sb(as, rv_ireg_s0, rv_ireg_s1, 0);
		asm_lb(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lb_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sb(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_lb(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lb_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sb(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_lb(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lbu_1()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sb(as, rv_ireg_a0, rv_ireg_a1, 0);
		asm_lbu(as, rv_ireg_a2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lbu_2()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sb(as, rv_ireg_s0, rv_ireg_s1, 0);
		asm_lbu(as, rv_ireg_s2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lbu_3()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_a0, 0x10000000);
		as.load_imm(rv_ireg_s1, -1);
		asm_sb(as, rv_ireg_a0, rv_ireg_s1, 0);
		asm_lbu(as, rv_ireg_s2, rv_ireg_a0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void test_sb_lbu_4()
	{
		P proc;
		assembler as;

		as.load_imm(rv_ireg_s0, 0x10000000);
		as.load_imm(rv_ireg_a1, -1);
		asm_sb(as, rv_ireg_s0, rv_ireg_a1, 0);
		asm_lbu(as, rv_ireg_a2, rv_ireg_s0, 0);
		asm_ebreak(as);
		as.link();

		run_test(__func__, proc, (addr_t)as.get_section(".text")->buf.data(), 5);
	}

	void print_summary()
	{
		printf("\n%d/%d tests successful\n", tests_passed, total_tests);
	}
};

template <typename T>
void test(T &test)
{
	test.test_addi_1();
	test.test_addi_2();
	test.test_addi_3();
	test.test_addi_4();
	test.test_slti_1();
	test.test_slti_2();
	test.test_sltiu_1();
	test.test_sltiu_2();
	test.test_add_1();
	test.test_add_2();
	test.test_add_3();
	test.test_add_4();
	test.test_add_5();
	test.test_add_6();
	test.test_add_7();
	test.test_add_8();
	test.test_sub_1();
	test.test_sub_2();
	test.test_sub_3();
	test.test_sub_4();
	test.test_sub_5();
	test.test_sub_6();
	test.test_sub_7();
	test.test_sub_8();
	test.test_slt_1();
	test.test_slt_2();
	test.test_sltu_1();
	test.test_sltu_2();
	test.test_andi_1();
	test.test_andi_2();
	test.test_ori_1();
	test.test_ori_2();
	test.test_xori_1();
	test.test_xori_2();
	test.test_and_1();
	test.test_and_2();
	test.test_or_1();
	test.test_or_2();
	test.test_xor_1();
	test.test_xor_2();
	test.test_slli_1();
	test.test_slli_2();
	test.test_slli_3();
	test.test_slli_4();
	test.test_slli_5();
	test.test_slli_6();
	test.test_srli_1();
	test.test_srli_2();
	test.test_srli_3();
	test.test_srli_4();
	test.test_srai_1();
	test.test_srai_2();
	test.test_srai_3();
	test.test_srai_4();
	test.test_addiw_1();
	test.test_addiw_2();
	test.test_addiw_3();
	test.test_addiw_4();
	test.test_slliw_1();
	test.test_slliw_2();
	test.test_srliw_1();
	test.test_srliw_2();
	test.test_sraiw_1();
	test.test_sraiw_2();
	test.test_sll_1();
	test.test_sll_2();
	test.test_sll_3();
	test.test_srl_1();
	test.test_srl_2();
	test.test_srl_3();
	test.test_sra_1();
	test.test_sra_2();
	test.test_sra_3();
	test.test_sllw_1();
	test.test_sllw_2();
	test.test_sllw_3();
	test.test_srlw_1();
	test.test_srlw_2();
	test.test_srlw_3();
	test.test_sraw_1();
	test.test_sraw_2();
	test.test_sraw_3();
	test.test_lui_1();
	test.test_lui_2();
	test.test_load_imm_1();
	test.test_load_imm_2();
	test.test_sd_ld_1();
	test.test_sd_ld_2();
	test.test_sd_ld_3();
	test.test_sd_ld_4();
	test.test_sw_lw_1();
	test.test_sw_lw_2();
	test.test_sw_lw_3();
	test.test_sw_lw_4();
	test.test_sw_lwu_1();
	test.test_sw_lwu_2();
	test.test_sw_lwu_3();
	test.test_sw_lwu_4();
	test.test_sh_lh_1();
	test.test_sh_lh_2();
	test.test_sh_lh_3();
	test.test_sh_lh_4();
	test.test_sh_lhu_1();
	test.test_sh_lhu_2();
	test.test_sh_lhu_3();
	test.test_sh_lhu_4();
	test.test_sb_lb_1();
	test.test_sb_lb_2();
	test.test_sb_lb_3();
	test.test_sb_lb_4();
	test.test_sb_lbu_1();
	test.test_sb_lbu_2();
	test.test_sb_lbu_3();
	test.test_sb_lbu_4();
	test.print_summary();
}

int main(int argc, char *argv[])
{
	rv_test_jit<proxy_jit_rv64imafdc> proc;
	if (argc == 2 && strcmp(argv[1], "-M") == 0) {
		proc.memory_registers = true;
	}
	test(proc);
}
