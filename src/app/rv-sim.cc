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

#include "dense_hash_map"

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>

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
#include "unknown-abi.h"
#include "processor-histogram.h"
#include "processor-proxy.h"
#include "debug-cli.h"
#include "processor-runloop.h"

#if defined (ENABLE_GPERFTOOL)
#include "gperftools/profiler.h"
#endif

using namespace riscv;

/* Parameterized ABI proxy processor models */

using proxy_emulator_rv32i = processor_runloop<processor_proxy<processor_rv32i_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32ima = processor_runloop<processor_proxy<processor_rv32ima_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imac = processor_runloop<processor_proxy<processor_rv32imac_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imafd = processor_runloop<processor_proxy<processor_rv32imafd_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imafdc = processor_runloop<processor_proxy<processor_rv32imafdc_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv64i = processor_runloop<processor_proxy<processor_rv64i_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64ima = processor_runloop<processor_proxy<processor_rv64ima_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imac = processor_runloop<processor_proxy<processor_rv64imac_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imafd = processor_runloop<processor_proxy<processor_rv64imafd_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imafdc = processor_runloop<processor_proxy<processor_rv64imafdc_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;


/* environment variables */

static const char* allowed_env_vars[] = {
	"TERM=",
	nullptr
};

static bool allow_env_var(const char *var)
{
	const char **envp = allowed_env_vars;
	while (*envp != nullptr) {
		if (strncmp(*envp, var, strlen(*envp)) == 0) return true;
		envp++;
	}
	return false;
}


/* RISC-V Emulator */

struct rv_emulator
{
	/*
		Simple ABI/AEE RISC-V emulator that uses a machine generated interpreter
		created by parse-meta using the C-psuedo code in meta/instructions

		Currently only a small number of syscalls are implemented

		privileged emulator with soft-mmu is a work in progress

		(ABI) application binary interface
		(AEE) application execution environment
	*/

	elf_file elf;
	std::string elf_filename;
	uintptr_t imagebase = 0;
	host_cpu &cpu;
	int proc_logs = 0;
	bool help_or_error = false;
	bool symbolicate = false;
	uint64_t initial_seed = 0;
	int ext = rv_set_imafdc;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

	rv_emulator() : cpu(host_cpu::get_instance()) {}

	static rv_set decode_isa_ext(std::string ext)
	{
		if (ext == "i") return rv_set_i;
		else if (ext == "ima") return rv_set_ima;
		else if (ext == "imac") return rv_set_imac;
		else if (ext == "imafd") return rv_set_imafd;
		else if (ext == "imafdc") return rv_set_imafdc;
		else return rv_set_none;
	}

	static const int elf_p_flags_mmap(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= PROT_EXEC;
		if (v & PF_W) prot |= PROT_WRITE;
		if (v & PF_R) prot |= PROT_READ;
		return prot;
	}

	const char* symlookup(addr_t addr)
	{
		static char symbol_tmpname[256];
		auto sym = elf.sym_by_addr((Elf64_Addr)addr);
		if (sym) {
			snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"%s", elf.sym_name(sym));
			return symbol_tmpname;
		}
		sym = elf.sym_by_nearest_addr((Elf64_Addr)addr);
		if (sym) {
			int64_t offset = int64_t(addr) - sym->st_value;
			snprintf(symbol_tmpname, sizeof(symbol_tmpname),
				"%s%s0x%" PRIx64, elf.sym_name(sym),
				offset < 0 ? "-" : "+", offset < 0 ? -offset : offset);
			return symbol_tmpname;
		}
		return nullptr;
	}

	/* Map a single stack segment into user address space */
	template <typename P>
	void map_proxy_stack(P &proc, addr_t stack_top, size_t stack_size)
	{
		void *addr = mmap((void*)(stack_top - stack_size), stack_size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			panic("map_proxy_stack: error: mmap: %s", strerror(errno));
		}

		/* keep track of the mapped segment and set the stack_top */
		proc.mmu.mem->segments.push_back(std::pair<void*,size_t>((void*)(stack_top - stack_size), stack_size));
		*(u64*)(stack_top - sizeof(u64)) = 0xfeedcafebabef00dULL;
		proc.ireg[rv_ireg_sp] = stack_top - sizeof(u64);

		/* log stack creation */
		if (proc.log & proc_log_memory) {
			debug("mmap-sp  :%016" PRIxPTR "-%016" PRIxPTR " +R+W",
				(stack_top - stack_size), stack_top);
		}
	}

	template <typename P>
	void copy_to_proxy_stack(P &proc, addr_t stack_top, size_t stack_size, void *data, size_t len)
	{
		proc.ireg[rv_ireg_sp] = proc.ireg[rv_ireg_sp] - len;
		if (size_t(stack_top - proc.ireg[rv_ireg_sp]) > stack_size) {
			panic("copy_to_proxy_stack: overflow: %d > %d",
				stack_top - proc.ireg[rv_ireg_sp], stack_size);
		}
		memcpy((void*)(uintptr_t)proc.ireg[rv_ireg_sp].r.xu.val, data, len);
	}

	template <typename P>
	void setup_proxy_stack(P &proc, addr_t stack_top, size_t stack_size)
	{
		/* set up auxiliary vector, environment and command line at top of stack */

		/*
			STACK TOP
			env data
			arg data
			padding, align 16
			auxv table, AT_NULL terminated
			envp array, null terminated
			argv pointer array, null terminated
			argc <- stack pointer
		*/

		/* set up aux data */
		std::vector<typename P::ux> aux_data = {
			AT_BASE, typename P::ux(imagebase),
			AT_PHDR, typename P::ux(imagebase + elf.ehdr.e_phoff),
			AT_PHNUM, elf.ehdr.e_phnum,
			AT_PHENT, elf.ehdr.e_phentsize,
			AT_PAGESZ, page_size,
			AT_RANDOM, cpu.get_random_seed(),
			AT_UID, getuid(),
			AT_EUID, geteuid(),
			AT_GID, getgid(),
			AT_EGID, getegid(),
			AT_NULL, 0
		};

		/* add environment data to stack */
		std::vector<typename P::ux> env_data;
		for (auto &env : host_env) {
			copy_to_proxy_stack(proc, stack_top, stack_size, (void*)env.c_str(), env.size() + 1);
			env_data.push_back(typename P::ux(proc.ireg[rv_ireg_sp].r.xu.val));
		}
		env_data.push_back(0);

		/* add command line data to stack */
		std::vector<typename P::ux> arg_data;
		for (auto &arg : host_cmdline) {
			copy_to_proxy_stack(proc, stack_top, stack_size, (void*)arg.c_str(), arg.size() + 1);
			arg_data.push_back(typename P::ux(proc.ireg[rv_ireg_sp].r.xu.val));
		}
		arg_data.push_back(0);

		/* align stack to 16 bytes */
		proc.ireg[rv_ireg_sp] = proc.ireg[rv_ireg_sp] & ~15;

		/* add auxiliary vector to stack */
		copy_to_proxy_stack(proc, stack_top, stack_size, (void*)aux_data.data(),
			aux_data.size() * sizeof(typename P::ux));

		/* add environment array to stack */
		copy_to_proxy_stack(proc, stack_top, stack_size, (void*)env_data.data(),
			env_data.size() * sizeof(typename P::ux));

		/* add command line array to stack */
		copy_to_proxy_stack(proc, stack_top, stack_size, (void*)arg_data.data(),
			arg_data.size() * sizeof(typename P::ux));

		/* add argc, argv, envp to stack */
		typename P::ux argc = host_cmdline.size();
		copy_to_proxy_stack(proc, stack_top, stack_size, (void*)&argc, sizeof(argc));
	}

	/* Map ELF load segments into proxy MMU address space */
	template <typename P>
	void map_load_segment_user(P &proc, const char* filename, Elf64_Phdr &phdr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}

		/* round the mmap start address and length to the nearest page size */
		addr_t map_delta = phdr.p_offset & (page_size-1);
		addr_t map_offset = phdr.p_offset - map_delta;
		addr_t map_vaddr = phdr.p_vaddr - map_delta;
		addr_t map_len = round_up(phdr.p_memsz + map_delta, page_size);
		addr_t map_end = map_vaddr + map_len;
		addr_t brk = addr_t(phdr.p_vaddr + phdr.p_memsz);
		if (!imagebase) imagebase = map_vaddr;
		void *addr = mmap((void*)map_vaddr, map_len,
			elf_p_flags_mmap(phdr.p_flags), MAP_FIXED | MAP_PRIVATE, fd, map_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		/* erase trailing bytes past the end of the mapping */
		if ((phdr.p_flags & PF_W) && phdr.p_memsz > phdr.p_filesz) {
			addr_t start = addr_t(phdr.p_vaddr + phdr.p_filesz), len = map_end - start;
			memset((void*)start, 0, len);
		}

		/* log load segment virtual address range */
		if (proc.log & proc_log_memory) {
			debug("mmap-elf :%016" PRIxPTR "-%016" PRIxPTR " %s offset=%" PRIxPTR,
				addr_t(map_vaddr), addr_t(map_vaddr + map_len),
				elf_p_flags_name(phdr.p_flags).c_str(), addr_t(map_offset));
		}

		/* add the mmap to the emulator proxy_mmu */
		proc.mmu.mem->segments.push_back(std::pair<void*,size_t>((void*)phdr.p_vaddr, phdr.p_memsz));

		/* set heap mmap area begin and end */
		if (proc.mmu.mem->heap_begin < map_end) {
			proc.mmu.mem->heap_begin = proc.mmu.mem->heap_end = map_end;
		}

		/* set the program break */
		if (proc.mmu.mem->brk < brk) {
			proc.mmu.mem->brk = brk;
		}
	}

	void parse_commandline(int argc, const char* argv[], const char* envp[])
	{
		cmdline_option options[] =
		{
		#if ENABLE_EXTENSION_SWITCH
			{ "-i", "--isa", cmdline_arg_type_string,
				"ISA Extensions (i, ima, imac, imafd, imafdc)",
				[&](std::string s) { return (ext = decode_isa_ext(s)); } },
		#endif
			{ "-l", "--log-instructions", cmdline_arg_type_none,
				"Log Instructions",
				[&](std::string s) { return (proc_logs |= (proc_log_inst | proc_log_trap)); } },
			{ "-o", "--log-operands", cmdline_arg_type_none,
				"Log Instructions and Operands",
				[&](std::string s) { return (proc_logs |= (proc_log_inst | proc_log_trap | proc_log_operands)); } },
			{ "-S", "--symbolicate", cmdline_arg_type_none,
				"Symbolicate addresses in instruction log",
				[&](std::string s) { return (symbolicate = true); } },
			{ "-m", "--log-memory-map", cmdline_arg_type_none,
				"Log Memory Map Information",
				[&](std::string s) { return (proc_logs |= proc_log_memory); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers (defaults to integer registers)",
				[&](std::string s) { return (proc_logs |= proc_log_int_reg); } },
			{ "-E", "--log-exit", cmdline_arg_type_none,
				"Log Registers at exit",
				[&](std::string s) { return (proc_logs |= proc_log_exit_stats); } },
			{ "-P", "--pc-usage-histogram", cmdline_arg_type_none,
				"Record program counter usage",
				[&](std::string s) { return (proc_logs |= proc_log_hist_pc | proc_log_exit_stats); } },
			{ "-R", "--register-usage-histogram", cmdline_arg_type_none,
				"Record register usage",
				[&](std::string s) { return (proc_logs |= proc_log_hist_reg | proc_log_exit_stats); } },
			{ "-I", "--instruction-usage-histogram", cmdline_arg_type_none,
				"Record instruction usage",
				[&](std::string s) { return (proc_logs |= proc_log_hist_inst | proc_log_exit_stats); } },
			{ "-d", "--debug", cmdline_arg_type_none,
				"Start up in debugger CLI",
				[&](std::string s) { return (proc_logs |= proc_log_ebreak_cli); } },
			{ "-x", "--no-pseudo", cmdline_arg_type_none,
				"Disable Pseudoinstruction decoding",
				[&](std::string s) { return (proc_logs |= proc_log_no_pseudo); } },
			{ "-s", "--seed", cmdline_arg_type_string,
				"Random seed",
				[&](std::string s) { initial_seed = strtoull(s.c_str(), nullptr, 10); return true; } },
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() < 1 && !help_or_error) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error) {
			printf("usage: %s [<options>] <elf_file> [<options>]\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		/* get command line options */
		elf_filename = result.first[0];
		for (size_t i = 0; i < result.first.size(); i++) {
			host_cmdline.push_back(result.first[i]);
		}

		/* filter host environment */
		for (const char** env = envp; *env != 0; env++) {
			if (allow_env_var(*env)) {
				host_env.push_back(*env);
			}
		}

		/* load ELF (headers only unless symbolicating) */
		elf.load(elf_filename, !symbolicate);
	}

	/* Start the execuatable with the given proxy processor template */
	template <typename P>
	void start_proxy()
	{
		/* setup floating point exception mask */
		fenv_init();

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.log = proc_logs;
		proc.pc = elf.ehdr.e_entry;
		proc.mmu.mem->log = (proc.log & proc_log_memory);
		if (symbolicate) proc.symlookup = [&](addr_t va) { return this->symlookup(va); };

		/* randomise integer register state with 512 bits of entropy */
		proc.seed_registers(cpu, initial_seed, 512);

		/* Find the ELF executable PT_LOAD segments and mmap them into user memory */
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & (PT_LOAD | PT_DYNAMIC)) {
				map_load_segment_user(proc, elf_filename.c_str(), phdr);
			}
		}

		/* Map a stack and set the stack pointer */
		static const size_t stack_size = 0x00100000; // 1 MiB
		map_proxy_stack(proc, P::mmu_type::memory_top, stack_size);
		setup_proxy_stack(proc, P::mmu_type::memory_top, stack_size);

		/* Initialize interpreter */
		proc.init();

#if defined (ENABLE_GPERFTOOL)
		ProfilerStart("test-emulate.out");
#endif

		/*
		 * Run the CPU until it halts
		 *
		 * when --debug flag is present we start in the debugger
		 */
		proc.run(proc.log & proc_log_ebreak_cli
			? exit_cause_cli : exit_cause_continue);

#if defined (ENABLE_GPERFTOOL)
		ProfilerStop();
#endif

		/* Unmap memory segments */
		for (auto &seg: proc.mmu.mem->segments) {
			munmap(seg.first, seg.second);
		}
	}

	/* Start a specific processor implementation based on ELF type and ISA extensions */
	void exec()
	{
		/* check for RDTSCP on X86 */
		#if X86_USE_RDTSCP
		if (cpu.caps.size() > 0 && cpu.caps.find("RDTSCP") == cpu.caps.end()) {
			panic("error: x86 host without RDTSCP. Recompile with -DX86_NO_RDTSCP");
		}
		#endif

		/* execute */
		switch (elf.ei_class) {
			case ELFCLASS32:
				switch (ext) {
				#if ENABLE_EXTENSION_SWITCH
					case rv_set_i: start_proxy<proxy_emulator_rv32i>(); break;
					case rv_set_ima: start_proxy<proxy_emulator_rv32ima>(); break;
					case rv_set_imac: start_proxy<proxy_emulator_rv32imac>(); break;
					case rv_set_imafd: start_proxy<proxy_emulator_rv32imafd>(); break;
				#endif
					case rv_set_imafdc: start_proxy<proxy_emulator_rv32imafdc>(); break;
					case rv_set_none: panic("illegal isa extension"); break;
				}
				break;
			case ELFCLASS64:
				switch (ext) {
				#if ENABLE_EXTENSION_SWITCH
					case rv_set_i: start_proxy<proxy_emulator_rv64i>(); break;
					case rv_set_ima: start_proxy<proxy_emulator_rv64ima>(); break;
					case rv_set_imac: start_proxy<proxy_emulator_rv64imac>(); break;
					case rv_set_imafd: start_proxy<proxy_emulator_rv64imafd>(); break;
				#endif
					case rv_set_imafdc: start_proxy<proxy_emulator_rv64imafdc>(); break;
					case rv_set_none: panic("illegal isa extension"); break;
				}
				break;
			default: panic("illegal elf class");
		}
	}
};


/* program main */

int main(int argc, const char* argv[], const char* envp[])
{
	rv_emulator emulator;
	emulator.parse_commandline(argc, argv, envp);
	emulator.exec();
	return 0;
}
