//
//  riscv-test-emulate.cc
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
#include <type_traits>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-fmt.h"
#include "riscv-bits.h"
#include "riscv-sha512.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-host.h"
#include "riscv-cmdline.h"
#include "riscv-config-parser.h"
#include "riscv-config.h"
#include "riscv-codec.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-strings.h"
#include "riscv-disasm.h"
#include "riscv-alu.h"
#include "riscv-fpu.h"
#include "riscv-pte.h"
#include "riscv-pma.h"
#include "riscv-user-memory.h"
#include "riscv-soft-tlb.h"
#include "riscv-soft-mmu.h"
#include "riscv-interp.h"
#include "riscv-processor-base.h"
#include "riscv-processor-impl.h"
#include "riscv-processor-model.h"
#include "riscv-unknown-abi.h"
#include "riscv-processor-proxy.h"
#include "riscv-device-boot.h"
#include "riscv-device-time.h"
#include "riscv-device-mipi.h"
#include "riscv-device-plic.h"
#include "riscv-device-uart.h"
#include "riscv-processor-priv-1.9.h"
#include "riscv-processor-runloop.h"

#if defined (ENABLE_GPERFTOOL)
#include "gperftools/profiler.h"
#endif

using namespace riscv;

/* Parameterized ABI proxy processor models */

using proxy_emulator_rv32ima = processor_runloop<processor_proxy<processor_rv32ima_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imac = processor_runloop<processor_proxy<processor_rv32imac_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imafd = processor_runloop<processor_proxy<processor_rv32imafd_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imafdc = processor_runloop<processor_proxy<processor_rv32imafdc_model<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv64ima = processor_runloop<processor_proxy<processor_rv64ima_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imac = processor_runloop<processor_proxy<processor_rv64imac_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imafd = processor_runloop<processor_proxy<processor_rv64imafd_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imafdc = processor_runloop<processor_proxy<processor_rv64imafdc_model<decode,processor_rv64imafd,mmu_proxy_rv64>>>;


/* Parameterized privileged soft-mmu processor models */

using priv_emulator_rv32ima = processor_runloop<processor_privileged<processor_rv32ima_model<decode,processor_priv_rv32imafd,mmu_soft_rv32>>>;
using priv_emulator_rv32imac = processor_runloop<processor_privileged<processor_rv32imac_model<decode,processor_priv_rv32imafd,mmu_soft_rv32>>>;
using priv_emulator_rv32imafd = processor_runloop<processor_privileged<processor_rv32imafd_model<decode,processor_priv_rv32imafd,mmu_soft_rv32>>>;
using priv_emulator_rv32imafdc = processor_runloop<processor_privileged<processor_rv32imafdc_model<decode,processor_priv_rv32imafd,mmu_soft_rv32>>>;
using priv_emulator_rv64ima = processor_runloop<processor_privileged<processor_rv64ima_model<decode,processor_priv_rv64imafd,mmu_soft_rv64>>>;
using priv_emulator_rv64imac = processor_runloop<processor_privileged<processor_rv64imac_model<decode,processor_priv_rv64imafd,mmu_soft_rv64>>>;
using priv_emulator_rv64imafd = processor_runloop<processor_privileged<processor_rv64imafd_model<decode,processor_priv_rv64imafd,mmu_soft_rv64>>>;
using priv_emulator_rv64imafdc = processor_runloop<processor_privileged<processor_rv64imafdc_model<decode,processor_priv_rv64imafd,mmu_soft_rv64>>>;


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

struct riscv_emulator
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
	config cfg;
	std::string config_filename;
	host_cpu &cpu;
	int proc_logs = 0;
	bool priv_mode = false;
	bool help_or_error = false;
	uint64_t initial_seed = 0;
	int ext = rv_isa_imafdc;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

	static const int inst_step = 1000000; /* Number of instructions executes in step call */

	riscv_emulator() : cpu(host_cpu::get_instance()) {}

	static rv_isa decode_isa_ext(std::string isa_ext)
	{
		if (strncasecmp(isa_ext.c_str(), "IMA", isa_ext.size()) == 0) return rv_isa_ima;
		else if (strncasecmp(isa_ext.c_str(), "IMAC", isa_ext.size()) == 0) return rv_isa_imac;
		else if (strncasecmp(isa_ext.c_str(), "IMAFD", isa_ext.size()) == 0) return rv_isa_imafd;
		else if (strncasecmp(isa_ext.c_str(), "IMAFDC", isa_ext.size()) == 0) return rv_isa_imafdc;
		else return rv_isa_none;
	}

	static const int elf_p_flags_mmap(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= PROT_EXEC;
		if (v & PF_W) prot |= PROT_WRITE;
		if (v & PF_R) prot |= PROT_READ;
		return prot;
	}

	static const int elf_pma_flags(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= pma_prot_execute;
		if (v & PF_W) prot |= pma_prot_write;
		if (v & PF_R) prot |= pma_prot_read;
		return prot;
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
		proc.mmu.mem.segments.push_back(std::pair<void*,size_t>((void*)(stack_top - stack_size), stack_size));
		*(u64*)(stack_top - sizeof(u64)) = 0xfeedcafebabef00dULL;
		proc.ireg[riscv_ireg_sp] = stack_top - sizeof(u64);

		/* log stack creation */
		if (proc.log & proc_log_memory) {
			debug("mmap-sp  :%016" PRIxPTR "-%016" PRIxPTR " +R+W",
				(stack_top - stack_size), stack_top);
		}
	}

	template <typename P>
	void copy_to_proxy_stack(P &proc, addr_t stack_top, size_t stack_size, void *data, size_t len)
	{
		proc.ireg[riscv_ireg_sp] = proc.ireg[riscv_ireg_sp] - len;
		if (size_t(stack_top - proc.ireg[riscv_ireg_sp]) > stack_size) {
			panic("copy_to_proxy_stack: overflow: %d > %d",
				stack_top - proc.ireg[riscv_ireg_sp], stack_size);
		}
		memcpy((void*)(uintptr_t)proc.ireg[riscv_ireg_sp].r.xu.val, data, len);
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

			enum {
				AT_NULL = 0,         * end of auxiliary vector *
				AT_BASE = 7,         * pointer to image base *
			};

			typedef struct {
				Elf32_Word a_type;
				Elf32_Word a_val;
			} Elf32_auxv;

			typedef struct {
				Elf64_Word a_type;
				Elf64_Word a_val;
			} Elf64_auxv;
		*/


		/* add environment data to stack */
		std::vector<typename P::ux> env_data;
		for (auto &env : host_env) {
			copy_to_proxy_stack(proc, stack_top, stack_size, (void*)env.c_str(), env.size() + 1);
			env_data.push_back(typename P::ux(proc.ireg[riscv_ireg_sp].r.xu.val));
		}
		env_data.push_back(0);

		/* add command line data to stack */
		std::vector<typename P::ux> arg_data;
		for (auto &arg : host_cmdline) {
			copy_to_proxy_stack(proc, stack_top, stack_size, (void*)arg.c_str(), arg.size() + 1);
			arg_data.push_back(typename P::ux(proc.ireg[riscv_ireg_sp].r.xu.val));
		}
		arg_data.push_back(0);

		/* align stack to 16 bytes */
		proc.ireg[riscv_ireg_sp] = proc.ireg[riscv_ireg_sp] & ~15;

		/* TODO - Add auxiliary vector to stack */

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

	/* Map ELF load segments into privileged MMU address space */
	template <typename P>
	void map_load_segment_priv(P &proc, const char* filename, Elf64_Phdr &phdr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}
		void *addr = mmap(nullptr, phdr.p_memsz,
			elf_p_flags_mmap(phdr.p_flags), MAP_PRIVATE, fd, phdr.p_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		/* log elf load segment virtual address range */
		if (proc.log & proc_log_memory) {
			debug("mmap-elf :%016" PRIxPTR "-%016" PRIxPTR " %s",
				addr_t(phdr.p_vaddr), addr_t(phdr.p_vaddr + phdr.p_memsz),
				elf_p_flags_name(phdr.p_flags).c_str());
		}

		/* add the mmap to the emulator soft_mmu */
		proc.mmu.mem.add_mmap(phdr.p_vaddr, addr_t(addr), phdr.p_memsz,
			pma_type_main | elf_pma_flags(phdr.p_flags));
	}

	/* Map ELF load segments into proxy MMU address space */
	template <typename P>
	void map_load_segment_user(P &proc, const char* filename, Elf64_Phdr &phdr)
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

		/* log elf load segment virtual address range */
		if (proc.log & proc_log_memory) {
			debug("mmap-elf :%016" PRIxPTR "-%016" PRIxPTR " %s",
				addr_t(phdr.p_vaddr), addr_t(phdr.p_vaddr + phdr.p_memsz),
				elf_p_flags_name(phdr.p_flags).c_str());
		}

		/* add the mmap to the emulator proxy_mmu */
		proc.mmu.mem.segments.push_back(std::pair<void*,size_t>((void*)phdr.p_vaddr, phdr.p_memsz));
		addr_t seg_end = addr_t(phdr.p_vaddr + phdr.p_memsz);
		if (proc.mmu.mem.heap_begin < seg_end) {
			proc.mmu.mem.heap_begin = proc.mmu.mem.heap_end = seg_end;
		}
	}

	void parse_commandline(int argc, const char* argv[], const char* envp[])
	{
		cmdline_option options[] =
		{
			{ "-c", "--config", cmdline_arg_type_string,
				"Configuration strung",
				[&](std::string s) { config_filename = s; return true; } },
			{ "-i", "--isa", cmdline_arg_type_string,
				"ISA Extensions (IMA, IMAC, IMAFD, IMAFDC)",
				[&](std::string s) { return (ext = decode_isa_ext(s)); } },
			{ "-p", "--privileged", cmdline_arg_type_none,
				"Privileged ISA Emulation",
				[&](std::string s) { return (priv_mode = true); } },
			{ "-l", "--log-instructions", cmdline_arg_type_none,
				"Log Instructions",
				[&](std::string s) { return (proc_logs |= (proc_log_inst | proc_log_trap)); } },
			{ "-o", "--log-operands", cmdline_arg_type_none,
				"Log Instructions and Operands",
				[&](std::string s) { return (proc_logs |= (proc_log_inst | proc_log_trap | proc_log_operands)); } },
			{ "-O", "--log-mmio", cmdline_arg_type_none,
				"Log Memory Mapped IO",
				[&](std::string s) { return (proc_logs |= proc_log_mmio); } },
			{ "-m", "--log-memory-map", cmdline_arg_type_none,
				"Log Memory Map Information",
				[&](std::string s) { return (proc_logs |= proc_log_memory); } },
			{ "-M", "--log-mmode-csr", cmdline_arg_type_none,
				"Log Machine Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_mmode); } },
			{ "-H", "--log-hmode-csr", cmdline_arg_type_none,
				"Log Hypervisor Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_hmode); } },
			{ "-S", "--log-smode-csr", cmdline_arg_type_none,
				"Log Supervisor Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_smode); } },
			{ "-U", "--log-umode-csr", cmdline_arg_type_none,
				"Log User Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_umode); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers (defaults to integer registers)",
				[&](std::string s) { return (proc_logs |= proc_log_int_reg); } },
			{ "-t", "--log-traps", cmdline_arg_type_none,
				"Log Traps",
				[&](std::string s) { return (proc_logs |= proc_log_trap); } },
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
		} else if (result.first.size() < 1) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error) {
			printf("usage: %s [<options>] <elf_file>\n", argv[0]);
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

		/* read config */
		if (config_filename.size() > 0) {
			cfg.read(config_filename);
		}

		/* load ELF (headers only) */
		elf.load(elf_filename, true);
	}

	template <typename P>
	void seed_registers(P &proc, size_t n)
	{
		sha512_ctx_t sha512;
		u8 seed[SHA512_OUTPUT_BYTES];   /* 512-bits random seed */
		u8 random[SHA512_OUTPUT_BYTES]; /* 512-bits hash output */

		/* if 64-bit initial seed is specified, repeat seed 8 times in the seed buffer
		   if no initial seed is specified then fill the seed buffer with 512-bits of random */
		for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
			*(u64*)(seed + i) = initial_seed ? initial_seed
				: (((u64)cpu.get_random_seed()) << 32) | (u64)cpu.get_random_seed() ;
		}

		/* Log initial seed state */
		if (proc.log & proc_log_memory) {
			std::string seed_str;
			for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
				seed_str.append(format_string("%016llx", *(u64*)(seed + i)));
			}
			debug("seed: %s", seed_str.c_str());
		}

		/* Randomize the integer registers */
		size_t rand_bytes = 0;
		std::uniform_int_distribution<typename P::ux> distribution(0, std::numeric_limits<typename P::ux>::max());
		for (size_t i = riscv_ireg_x1; i < P::ireg_count; i++) {
			/* on buffer exhaustion sha-512 hash the seed and xor the hash back into the seed */
			if ((rand_bytes & (SHA512_OUTPUT_BYTES - 1)) == 0) {
				sha512_init(&sha512);
				sha512_update(&sha512, seed, SHA512_OUTPUT_BYTES);
				sha512_final(&sha512, random);
				for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
					*(u64*)(seed + i) ^= *(u64*)(random + i);
				}
			}
			proc.ireg[i].r.xu.val = *(u64*)(random + (rand_bytes & (SHA512_OUTPUT_BYTES - 1)));
			rand_bytes += 8;
		}
	}

	/* Start the execuatable with the given privileged processor template */
	template <typename P>
	void start_priv()
	{
		/* setup floating point exception mask */
		fenv_init();

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.log = proc_logs;
		proc.pc = elf.ehdr.e_entry;
		proc.mmu.mem.log = (proc.log & proc_log_memory);

		/* randomise integer register state with 512 bits of entropy */
		seed_registers(proc, 512);

		/* Find the ELF executable PT_LOAD segments and map them into the emulator mmu */
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & PT_LOAD) {
				map_load_segment_priv(proc, elf_filename.c_str(), phdr);
			}
		}

		/* Add 1GB RAM to the mmu (TODO - read from config string) */
		proc.mmu.mem.add_ram(/*mpa=2GiB*/ 0x80000000ULL, /*size=1GiB*/ 0x40000000ULL);

		/* Initialize interpreter */
		proc.init();

#if defined (ENABLE_GPERFTOOL)
		ProfilerStart("test-emulate.out");
#endif

		/* Step the CPU until it halts */
		while(proc.step(inst_step));

#if defined (ENABLE_GPERFTOOL)
		ProfilerStop();
#endif

		/* Unmap memory segments */

		/* TODO - unmap ELF ROM */
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

		/* randomise integer register state with 512 bits of entropy */
		seed_registers(proc, 512);

		/* Find the ELF executable PT_LOAD segments and mmap them into user memory */
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & PT_LOAD) {
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

		/* Step the CPU until it halts */
		while(proc.step(inst_step));

#if defined (ENABLE_GPERFTOOL)
		ProfilerStop();
#endif

		/* Unmap memory segments */
		for (auto &seg: proc.mmu.mem.segments) {
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
		if (priv_mode) {
			switch (elf.ei_class) {
				case ELFCLASS32:
					switch (ext) {
						case rv_isa_ima: start_priv<priv_emulator_rv32ima>(); break;
						case rv_isa_imac: start_priv<priv_emulator_rv32imac>(); break;
						case rv_isa_imafd: start_priv<priv_emulator_rv32imafd>(); break;
						case rv_isa_imafdc: start_priv<priv_emulator_rv32imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				case ELFCLASS64:
					switch (ext) {
						case rv_isa_ima: start_priv<priv_emulator_rv64ima>(); break;
						case rv_isa_imac: start_priv<priv_emulator_rv64imac>(); break;
						case rv_isa_imafd: start_priv<priv_emulator_rv64imafd>(); break;
						case rv_isa_imafdc: start_priv<priv_emulator_rv64imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				default: panic("illegal elf class");
			}
		} else {
			switch (elf.ei_class) {
				case ELFCLASS32:
					switch (ext) {
						case rv_isa_ima: start_proxy<proxy_emulator_rv32ima>(); break;
						case rv_isa_imac: start_proxy<proxy_emulator_rv32imac>(); break;
						case rv_isa_imafd: start_proxy<proxy_emulator_rv32imafd>(); break;
						case rv_isa_imafdc: start_proxy<proxy_emulator_rv32imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				case ELFCLASS64:
					switch (ext) {
						case rv_isa_ima: start_proxy<proxy_emulator_rv64ima>(); break;
						case rv_isa_imac: start_proxy<proxy_emulator_rv64imac>(); break;
						case rv_isa_imafd: start_proxy<proxy_emulator_rv64imafd>(); break;
						case rv_isa_imafdc: start_proxy<proxy_emulator_rv64imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				default: panic("illegal elf class");
			}
		}
	}
};


/* program main */

int main(int argc, const char* argv[], const char* envp[])
{
	riscv_emulator emulator;
	emulator.parse_commandline(argc, argv, envp);
	emulator.exec();
	return 0;
}
