//
//  riscv-test-emulate.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-cmdline.h"
#include "riscv-codec.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-strings.h"
#include "riscv-disasm.h"
#include "riscv-processor.h"
#include "riscv-abi.h"
#include "riscv-proxy.h"

inline float f32_sqrt(float a) { return std::sqrt(a); }
inline double f64_sqrt(double a) { return std::sqrt(a); }
inline float f32_classify(float a) { return 0; } /* unimplemented */
inline double f64_classify(double a) { return 0; } /* unimplemented */

namespace riscv {
	#include "riscv-interp.h"
}

using namespace riscv;

struct riscv_proc_proxy_rv64 : riscv_processor_rv64
{
	uintptr_t heap_begin;
	uintptr_t heap_end;
	std::vector<std::pair<void*,size_t>> mapped_segments;

	riscv_proc_proxy_rv64() : riscv_processor_rv64(), heap_begin(0), heap_end(0) {}
};

struct riscv_emulator
{
	const size_t stack_top =  0x78000000; // 1920 MiB
	const size_t stack_size = 0x01000000; //   16 MiB

	elf_file elf;
	std::string filename;

	bool memory_debug = false;
	bool emulator_debug = false;
	bool log_registers = false;
	bool log_instructions = false;
	bool help_or_error = false;

	/*
		This simple proof of concept machine emulator uses a 
		machine generated interpreter in src/asm/riscv-interp.h
		created by parse-meta using C-psuedo code in meta/instructions
	*/

	inline static const int elf_p_flags_mmap(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= PROT_EXEC;
		if (v & PF_W) prot |= PROT_WRITE;
		if (v & PF_R) prot |= PROT_READ;
		return prot;
	}

	template <typename P>
	void print_registers(P &proc)
	{
		for (size_t i = 0; i < 32; i++) {
			char fmt[32];
			snprintf(fmt, sizeof(fmt), "%%-4s: 0x%%0%ux%%s", (P::xlen >> 2));
			printf(fmt, riscv_ireg_name_sym[i], proc.ireg[i], (i + 1) % 4 == 0 ? "\n" : " ");
		}
	}

	std::string format_inst(uintptr_t pc)
	{
		char buf[20];
		uintptr_t inst_length;
		uint64_t inst = riscv_inst_fetch(pc, &inst_length);
		switch (inst_length) {
			case 2:  snprintf(buf, sizeof(buf), "0x%04tx", inst); break;
			case 4:  snprintf(buf, sizeof(buf), "0x%08tx", inst); break;
			case 6:  snprintf(buf, sizeof(buf), "0x%012tx", inst); break;
			case 8:  snprintf(buf, sizeof(buf), "0x%016tx", inst); break;
			default: snprintf(buf, sizeof(buf), "(invalid)"); break;
		}
		return buf;
	}

	template <typename T, typename P>
	void print_disassembly(T &dec, P &proc)
	{
		std::string args = riscv_disasm_inst_simple(dec);
		printf("core %3zu: 0x%016tx (%s) %-30s\n", proc.hart_id, proc.pc, format_inst(proc.pc).c_str(), args.c_str());
	}

	// Simple RV64 Linux syscall emulation (write, exit)
	template <typename P>
	void emulate_ecall(riscv_decode &dec, P &proc, uintptr_t inst_length)
	{
		switch (proc.ireg[riscv_ireg_a7]) {
			case riscv_syscall_close:  riscv_sys_close(proc); break;
			case riscv_syscall_write:  riscv_sys_write(proc); break;
			case riscv_syscall_fstat:  riscv_sys_fstat(proc); break;
			case riscv_syscall_exit:   riscv_sys_exit(proc);  break;
			case riscv_syscall_brk:    riscv_sys_brk(proc);   break;
			default: panic("unknown syscall: %d", proc.ireg[riscv_ireg_a7]);
		}
		proc.pc += inst_length;
	}

	// Simple RV64 emulator main loop
	template <typename P>
	void rv64_run(P &proc)
	{
		riscv_decode dec;
		size_t inst_length;
		uint64_t inst;
		while (true) {
			inst = riscv_inst_fetch(proc.pc, &inst_length);
			riscv_decode_inst_rv64(dec, inst);
			if (log_registers) print_registers(proc);
			if (log_instructions) print_disassembly(dec, proc);
			if (riscv::rv64_exec(dec, proc, inst_length)) continue;
			if (dec.op == riscv_op_ecall) emulate_ecall(dec, proc, inst_length);
			else {
				panic("illegal instruciton: pc=0x%" PRIx64 " inst=0x%" PRIx64,
					proc.pc, inst);
			}
		}
	}

	// Simple code to map a single stack segment
	template <typename P>
	void map_stack(P &proc, uintptr_t stack_top, uintptr_t stack_size)
	{
		void *addr = mmap((void*)(stack_top - stack_size), stack_size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			panic("map_stack: error: mmap: %s", strerror(errno));
		}

		// keep track of the mapped segment and set the stack_top
		proc.mapped_segments.push_back(std::pair<void*,size_t>((void*)(stack_top - stack_size), stack_size));
		proc.ireg[riscv_ireg_sp] = stack_top - 0x8;

		if (emulator_debug) {
			debug("sp : mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				(stack_top - stack_size), stack_top);
		}
	}

	// Simple code currently maps all segments copy-on-write RWX
	template <typename P>
	void map_load_segment(P &proc, const char* filename, Elf64_Phdr &phdr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}
		void *addr = mmap((void*)phdr.p_vaddr, phdr.p_memsz,
			elf_p_flags_mmap(phdr.p_flags), MAP_FIXED | MAP_PRIVATE, fd, phdr.p_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		// keep track of the mapped segment and set the heap_end
		proc.mapped_segments.push_back(std::pair<void*,size_t>((void*)phdr.p_vaddr, phdr.p_memsz));
		uintptr_t seg_end = uintptr_t(phdr.p_vaddr + phdr.p_memsz);
		if (proc.heap_begin < seg_end) proc.heap_begin = proc.heap_end = seg_end;

		if (emulator_debug) {
			debug("elf: mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " %s",
				uintptr_t(phdr.p_vaddr), seg_end, elf_p_flags_name(phdr.p_flags).c_str());
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-m", "--memory-debug", cmdline_arg_type_none,
				"Memory debug",
				[&](std::string s) { return (memory_debug = true); } },
			{ "-d", "--emulator-debug", cmdline_arg_type_none,
				"Emulator debug",
				[&](std::string s) { return (emulator_debug = true); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers",
				[&](std::string s) { return (log_registers = true); } },
			{ "-l", "--log-instructions", cmdline_arg_type_none,
				"Log Instructions",
				[&](std::string s) { return (log_instructions = true); } },
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error)
		{
			printf("usage: %s [<options>] <elf_file>\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		filename = result.first[0];
	}

	// print approximate location of host text, heap and stack
	void memory_info(int argc, const char *argv[])
	{
		static const char *textptr = nullptr;
		void *heapptr = malloc(8);
		debug("text : ~0x%016" PRIxPTR, (uintptr_t)&textptr);
		debug("heap : ~0x%016" PRIxPTR, (uintptr_t)heapptr);
		debug("stack: ~0x%016" PRIxPTR, (uintptr_t)argv);
		free(heapptr);
	}

	void exec(int argc, const char *argv[])
	{
		// print process information
		if (memory_debug) {
			memory_info(argc, argv);
		}

		// load ELF (headers only)
		elf.load(filename, true);

		// Processor
		riscv_proc_proxy_rv64 proc;
		proc.flags = emulator_debug ? riscv_processor_flag_emulator_debug : 0;

		// Find the PT_LOAD segments and mmap then into memory
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & PT_LOAD) {
				map_load_segment(proc, filename.c_str(), phdr);
			}
		}

		// Map a stack and set the stack pointer
		map_stack(proc, stack_top, stack_size);

		// Set the program counter to the entry address
		proc.pc = elf.ehdr.e_entry;

		// Start the emulator
		rv64_run(proc);

		// Unmap segments
		for (auto &seg: proc.mapped_segments) {
			munmap(seg.first, seg.second);
		}
	}
};

int main(int argc, const char *argv[])
{
	riscv_emulator emulator;
	emulator.parse_commandline(argc, argv);
	emulator.exec(argc, argv);
	return 0;
}
