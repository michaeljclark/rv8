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

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-fmt.h"
#include "riscv-bits.h"
#include "riscv-sha512.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-color.h"
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
#include "riscv-amo.h"
#include "riscv-processor-logging.h"
#include "riscv-processor-base.h"
#include "riscv-processor-impl.h"
#include "riscv-user-memory.h"
#include "riscv-tlb-soft.h"
#include "riscv-mmu-soft.h"
#include "riscv-interp.h"
#include "riscv-processor-model.h"
#include "riscv-queue.h"
#include "riscv-device-rom-boot.h"
#include "riscv-device-rom-sbi.h"
#include "riscv-device-config.h"
#include "riscv-device-time.h"
#include "riscv-device-plic.h"
#include "riscv-device-uart.h"
#include "riscv-device-mipi.h"
#include "riscv-device-gpio.h"
#include "riscv-device-rand.h"
#include "riscv-processor-priv-1.9.h"
#include "riscv-debug-cli.h"
#include "riscv-processor-runloop.h"

#if defined (ENABLE_GPERFTOOL)
#include "gperftools/profiler.h"
#endif

using namespace riscv;

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

	static const uintmax_t default_ram_base = 0x80000000ULL; /* 2GiB */
	static const uintmax_t default_ram_size = 0x40000000ULL; /* 1GiB */

	elf_file elf;
	std::string elf_filename;
	config cfg;
	std::string config_filename;
	host_cpu &cpu;
	int proc_logs = 0;
	bool help_or_error = false;
	uint64_t initial_seed = 0;
	int ext = rv_isa_imafdc;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

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
		proc.mmu.mem->add_mmap(phdr.p_vaddr, addr_t(addr), phdr.p_memsz,
			pma_type_main | elf_pma_flags(phdr.p_flags));
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
			{ "-v", "--log-pagewalks", cmdline_arg_type_none,
				"Log Pagewalks",
				[&](std::string s) { return (proc_logs |= proc_log_pagewalk); } },
			{ "-t", "--log-traps", cmdline_arg_type_none,
				"Log Traps",
				[&](std::string s) { return (proc_logs |= proc_log_trap); } },
			{ "-d", "--debug", cmdline_arg_type_none,
				"Start up in debugger",
				[&](std::string s) { return (proc_logs |= proc_log_ebreak_cli); } },
			{ "-T", "--debug-trap", cmdline_arg_type_none,
				"Start up in debugger and enter debugger on trap",
				[&](std::string s) { return (proc_logs |= (proc_log_ebreak_cli | proc_log_trap_cli)); } },
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

	/* Start the execuatable with the given privileged processor template */
	template <typename P>
	void start_priv()
	{
		/* setup floating point exception mask */
		fenv_init();

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.log = proc_logs;
		proc.mmu.mem->log = (proc.log & proc_log_memory);

		/* randomise integer register state with 512 bits of entropy */
		proc.seed_registers(cpu, initial_seed, 512);

		/* Find the ELF executable PT_LOAD segments and map them into the emulator mmu */
		typename P::ux rom_base = 0;
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & (PT_LOAD | PT_DYNAMIC)) {
				map_load_segment_priv(proc, elf_filename.c_str(), phdr);
				if (rom_base == 0) rom_base = phdr.p_vaddr;
			}
		}

		/* Add 1GB RAM to the mmu (TODO - read from config string) */
		proc.mmu.mem->add_ram(default_ram_base, default_ram_size);

		/* Initialize interpreter */
		proc.init();
		proc.reset(); /* Reset code calls mapped ROM image */
		proc.device_config->num_harts = 1;
		proc.device_config->time_base = 1000000000;
		proc.device_config->rom_base = rom_base;
		proc.device_config->rom_entry = elf.ehdr.e_entry;
		proc.device_config->ram_base = default_ram_base;
		proc.device_config->ram_size = default_ram_size;

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
			#if ENABLE_RVIMA
					case rv_isa_ima: start_priv<priv_emulator_rv32ima>(); break;
					case rv_isa_imac: start_priv<priv_emulator_rv32imac>(); break;
					case rv_isa_imafd: start_priv<priv_emulator_rv32imafd>(); break;
			#endif
					case rv_isa_imafdc: start_priv<priv_emulator_rv32imafdc>(); break;
					case rv_isa_none: panic("illegal isa extension"); break;
				}
				break;
			case ELFCLASS64:
				switch (ext) {
			#if ENABLE_RVIMA
					case rv_isa_ima: start_priv<priv_emulator_rv64ima>(); break;
					case rv_isa_imac: start_priv<priv_emulator_rv64imac>(); break;
					case rv_isa_imafd: start_priv<priv_emulator_rv64imafd>(); break;
			#endif
					case rv_isa_imafdc: start_priv<priv_emulator_rv64imafdc>(); break;
					case rv_isa_none: panic("illegal isa extension"); break;
				}
				break;
			default: panic("illegal elf class");
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
