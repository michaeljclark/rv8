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
#include "riscv-processor.h"
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
#include "riscv-abi.h"
#include "riscv-syscalls.h"

inline float f32_sqrt(float a) { return std::sqrt(a); }
inline double f64_sqrt(double a) { return std::sqrt(a); }
inline float f32_classify(float a) { return 0; } /* unimplemented */
inline double f64_classify(double a) { return 0; } /* unimplemented */

namespace riscv {
	#include "riscv-interp.h"
}

using namespace riscv;

struct riscv_proc_proxy_rv64 : riscv_proc_rv64
{
	uintptr_t heap_begin;
	uintptr_t heap_end;
	std::vector<std::pair<void*,size_t>> mapped_segments;

	riscv_proc_proxy_rv64() : riscv_proc_rv64(), heap_begin(0), heap_end(0) {}
};

struct riscv_emulator
{
	const size_t stack_top = 0x40000000;
	const size_t stack_size = 0x10000;

	elf_file elf;
	std::string filename;

	bool registers = false;
	bool disassebly = false;
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

	// Register dump
	template <typename T>
	void print_registers(T &proc)
	{
		for (size_t i = 0; i < 32; i++) {
			char fmt[32];
			snprintf(fmt, sizeof(fmt), "%%4s=0x%%0%ux%%s", (T::xlen >> 2));
			printf(fmt, riscv_ireg_name_sym[i], proc.ireg[i], (i + 1) % 4 == 0 ? "\n" : " ");
		}
	}

	// Disassembler
	template <typename T>
	void print_disassembly(T &dec, uintptr_t pc)
	{
		std::string args = riscv_disasm_inst_simple(dec);
		printf("0x%016tx %-30s\n", pc, args.c_str());
	}

	// Simple RV64 Linux syscall emulation (write, exit)
	template <typename T>
	void emulate_ecall(riscv_decode &dec, T &proc, uintptr_t next_pc)
	{
		switch (proc.ireg[riscv_ireg_a7]) {
			case 57:  riscv_sys_close(proc); break;
			case 64:  riscv_sys_write(proc); break;
			case 80:  riscv_sys_fstat(proc); break;
			case 93:  riscv_sys_exit(proc); break;
			case 214: riscv_sys_brk(proc); break;
			default: panic("unknown syscall: %d", proc.ireg[riscv_ireg_a7]);
		}
		proc.pc = next_pc;
	}

	// Simple RV64 emulator main loop
	template <typename T>
	void rv64_run(T &proc)
	{
		riscv_decode dec;
		uintptr_t inst, next_pc;
		while (true) {
			inst = riscv_get_inst(proc.pc, &next_pc);
			riscv_decode_inst_rv64(dec, inst);
			if (registers) print_registers(proc);
			if (disassebly) print_disassembly(dec, proc.pc);
			if (riscv::rv64_exec(dec, proc, next_pc)) continue;
			if (dec.op == riscv_op_ecall) emulate_ecall(dec, proc, next_pc);
			else {
				panic("illegal instruciton: pc=0x%" PRIx64 " inst=0x%" PRIx64,
					proc.pc, inst);
			}
		}
	}

	// Simple code to map a single stack segment
	template <typename T>
	void map_stack(T &proc, uintptr_t stack_top, uintptr_t stack_size)
	{
		void *addr = mmap((void*)(stack_top - stack_size), stack_size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			panic("map_stack: error: mmap: %s", strerror(errno));
		}

		// keep track of the mapped segment and set the stack_top
		proc.mapped_segments.push_back(std::pair<void*,size_t>((void*)(stack_top - stack_size), stack_size));
		proc.ireg[riscv_ireg_sp] = stack_top - 0x8;

		printf("guest stack =  0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W\n",
			(stack_top - stack_size), stack_top);
	}

	// Simple code currently maps all segments copy-on-write RWX
	template <typename T>
	void map_load_segment(T &proc, const char* filename, Elf64_Phdr &phdr)
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

		printf("guest text  =  0x%016" PRIxPTR " - 0x%016" PRIxPTR " %s\n",
			uintptr_t(phdr.p_vaddr), seg_end,
			elf_p_flags_name(phdr.p_flags).c_str());
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-r", "--print-registers", cmdline_arg_type_none,
				"Print Registers",
				[&](std::string s) { return (registers = true); } },
			{ "-d", "--print-disassembly", cmdline_arg_type_none,
				"Print Disassembly",
				[&](std::string s) { return (disassebly = true); } },
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
	void process_info(const char **argv)
	{
		static const char *textptr = nullptr;
		void *heapptr = malloc(8);
		printf("host  text  = ~0x%016" PRIxPTR "\n", (uintptr_t)&textptr);
		printf("host  heap  = ~0x%016" PRIxPTR "\n", (uintptr_t)heapptr);
		printf("host  stack = ~0x%016" PRIxPTR "\n", (uintptr_t)argv);
		free(heapptr);
	}

	void exec()
	{
		// load ELF (headers only)
		elf.load(filename, true);

		// Processor state
		riscv_proc_proxy_rv64 proc;

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
	emulator.process_info(argv);
	emulator.exec();
	return 0;
}
