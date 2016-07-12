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
#include "riscv-linux.h"
#include "riscv-syscalls.h"

inline float f32_sqrt(float a) { return std::sqrt(a); }
inline double f64_sqrt(double a) { return std::sqrt(a); }
inline float f32_classify(float a) { return 0; } /* unimplemented */
inline double f64_classify(double a) { return 0; } /* unimplemented */

namespace riscv {
	#include "riscv-interp.h"
}

using namespace riscv;

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
		std::string args;
		const char *fmt = riscv_inst_format[dec.op];
		while (*fmt) {
			switch (*fmt) {
				case 'O': args += riscv_inst_name_sym[dec.op]; break;
				case '(': args += "("; break;
				case ',': args += ","; break;
				case ')': args += ")"; break;
				case '0': args += riscv_ireg_name_sym[dec.rd]; break;
				case '1': args += riscv_ireg_name_sym[dec.rs1]; break;
				case '2': args += riscv_ireg_name_sym[dec.rs2]; break;
				case '3': args += riscv_freg_name_sym[dec.rd]; break;
				case '4': args += riscv_freg_name_sym[dec.rs1]; break;
				case '5': args += riscv_freg_name_sym[dec.rs2]; break;
				case '6': args += riscv_freg_name_sym[dec.rs3]; break;
				case '7': args += format_string("%d", dec.rs1); break;
				case 'i': args += format_string("%lld", dec.imm); break;
				case 'o': args += format_string("%lld", dec.imm); break;
				case 'c': {
					const char * csr_name = riscv_csr_name_sym[dec.imm & 0xfff];
					if (csr_name) args += format_string("%s", csr_name);
					else args += format_string("0x%03x", dec.imm & 0xfff);
					break;
				}
				case 'r':
					switch(dec.rm) {
						case riscv_rm_rne: args += "rne"; break;
						case riscv_rm_rtz: args += "rtz"; break;
						case riscv_rm_rdn: args += "rdn"; break;
						case riscv_rm_rup: args += "rup"; break;
						case riscv_rm_rmm: args += "rmm"; break;
						default:           args += "unk"; break;
					}
					break;
				case '\t': while (args.length() < 12) args += " "; break;
				case 'A': if (dec.aq) args += ".aq"; break;
				case 'R': if (dec.rl) args += ".rl"; break;
				default:
					break;
			}
			fmt++;
		}
		printf("0x%016tx %-30s\n", pc, args.c_str());
	}

	// Simple RV64 Linux syscall emulation (write, exit)
	template <typename T>
	void emulate_ecall(riscv_decode &dec, T &proc, uintptr_t next_pc)
	{
		switch (proc.ireg[riscv_ireg_a7]) {
			case 64:  riscv_linux_sys_write(proc); break;
			case 80:  riscv_linux_sys_fstat(proc); break;
			case 93:  riscv_linux_sys_exit(proc); break;
			default: panic("unknown syscall: %d", proc.ireg[riscv_ireg_a7]);
		}
		proc.pc = next_pc;
	}

	// Simple RV64 emulator main loop
	void rv64_run(uintptr_t entry, uintptr_t stack_top)
	{
		riscv_decode dec;
		riscv_proc_rv64 proc;
		proc.pc = entry;
		proc.ireg[riscv_ireg_sp] = stack_top - 0x8;

		while (true) {
			uintptr_t next_pc;
			uint64_t inst = riscv_get_inst(proc.pc, &next_pc);
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
	void* map_stack(uintptr_t stack_top, uintptr_t stack_size)
	{
		void *addr = mmap((void*)(stack_top - stack_size), stack_size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			panic("map_stack: error: mmap: %s", strerror(errno));
		}
		printf("guest stack =  0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W\n",
			(stack_top - stack_size), stack_top);

		return addr;
	}

	inline const int elf_p_flags_mmap(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= PROT_EXEC;
		if (v & PF_W) prot |= PROT_WRITE;
		if (v & PF_R) prot |= PROT_READ;
		return prot;
	}

	// Simple code currently maps all segments copy-on-write RWX
	void* map_pt_load_segment(const char* filename, Elf64_Phdr &phdr)
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
		printf("guest text  =  0x%016" PRIxPTR " - 0x%016" PRIxPTR " %s\n",
			uintptr_t(phdr.p_vaddr), uintptr_t(phdr.p_vaddr + phdr.p_memsz),
			elf_p_flags_name(phdr.p_flags).c_str());

		return addr;
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

	void run()
	{
		// load ELF (headers only)
		elf.load(filename, true);

		// Find the PT_LOAD segments and mmap then into memory
		std::vector<std::pair<void*,size_t>> pt_load_segments;
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & PT_LOAD) {
				pt_load_segments.push_back(std::pair<void*,size_t>((void*)phdr.p_vaddr,phdr.p_memsz));
				map_pt_load_segment(filename.c_str(), phdr);
			}
		}

		// Create a stack
		void* stack_addr = map_stack(stack_top, stack_size);

		// Run the emulator
		rv64_run(uintptr_t(elf.ehdr.e_entry), stack_top);

		// Unmap segments
		munmap(stack_addr, stack_size);
		for (auto &seg: pt_load_segments) {
			munmap(seg.first, seg.second);
		}
	}
};

int main(int argc, const char *argv[])
{
	riscv_emulator emulator;
	emulator.parse_commandline(argc, argv);
	emulator.process_info(argv);
	emulator.run();
	return 0;
}
