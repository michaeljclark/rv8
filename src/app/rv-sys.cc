//
//  rv-sys.cc
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
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <type_traits>

#include "dense_hash_map"

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "host-endian.h"
#include "types.h"
#include "fmt.h"
#include "bits.h"
#include "sha512.h"
#include "format.h"
#include "meta.h"
#include "util.h"
#include "color.h"
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
#include "pte.h"
#include "pma.h"
#include "amo.h"
#include "processor-logging.h"
#include "processor-base.h"
#include "processor-impl.h"
#include "mmu-memory.h"
#include "tlb-soft.h"
#include "mmu-soft.h"
#include "interp.h"
#include "processor-model.h"
#include "queue.h"
#include "console.h"
#include "device-rom-boot.h"
#include "device-rom-sbi.h"
#include "device-rom-string.h"
#include "device-config.h"
#include "device-rtc.h"
#include "device-timer.h"
#include "device-plic.h"
#include "device-uart.h"
#include "device-mipi.h"
#include "device-gpio.h"
#include "device-rand.h"
#include "device-htif.h"
#include "processor-histogram.h"
#include "processor-priv-1.9.h"
#include "debug-cli.h"
#include "processor-runloop.h"

#if defined (ENABLE_GPERFTOOL)
#include "gperftools/profiler.h"
#endif

using namespace riscv;

/* Parameterized privileged soft-mmu processor models */

using priv_emulator_rv32imafdc = processor_runloop<processor_privileged<processor_rv32imafdc_model<decode,processor_priv_rv32imafd,mmu_soft_rv32>>>;
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

	static const uintmax_t default_ram_base = 0x80000000ULL; /* 2GiB */
	static const uintmax_t default_ram_size = 0x40000000ULL; /* 1GiB */

	elf_file elf;
	host_cpu &cpu;
	int proc_logs = 0;
	bool help_or_error = false;
	addr_t map_physical = 0;
	s64 ram_boot = 0;
	uint64_t initial_seed = 0;
	std::string boot_filename;
	std::string stats_dirname;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

	rv_emulator() : cpu(host_cpu::get_instance()) {}

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
	void map_load_segment_priv(P &proc, const char* filename, Elf64_Phdr &phdr, addr_t map_addr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}

		addr_t map_delta = phdr.p_offset & (page_size-1);
		addr_t map_offset = phdr.p_offset - map_delta;
		addr_t map_vaddr = phdr.p_vaddr - map_delta;
		addr_t map_len = round_up(phdr.p_memsz + map_delta, page_size);

		void *addr = mmap(nullptr, map_len,
			elf_p_flags_mmap(phdr.p_flags), MAP_PRIVATE, fd, map_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		/* zero bss */
		if ((phdr.p_flags & PF_W) && phdr.p_memsz > phdr.p_filesz) {
			memset((void*)((uintptr_t)addr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz - 1);
		}

		/* add the mmap to the emulator soft_mmu */
		proc.mmu.mem->add_mmap(map_vaddr, addr_t(addr), map_len,
			pma_type_main | elf_pma_flags(phdr.p_flags));
	}

	void parse_commandline(int argc, const char* argv[], const char* envp[])
	{
		cmdline_option options[] =
		{
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
			{ "-S", "--log-smode-csr", cmdline_arg_type_none,
				"Log Supervisor Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_smode); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers (defaults to integer registers)",
				[&](std::string s) { return (proc_logs |= proc_log_int_reg); } },
			{ "-v", "--log-pagewalks", cmdline_arg_type_none,
				"Log Pagewalks",
				[&](std::string s) { return (proc_logs |= proc_log_pagewalk); } },
			{ "-c", "--log-config", cmdline_arg_type_none,
				"Log Config",
				[&](std::string s) { return (proc_logs |= proc_log_config); } },
			{ "-t", "--log-traps", cmdline_arg_type_none,
				"Log Traps",
				[&](std::string s) { return (proc_logs |= proc_log_trap); } },
			{ "-E", "--log-exit-stats", cmdline_arg_type_none,
				"Log Registers and Statistics at Exit",
				[&](std::string s) { return (proc_logs |= proc_log_exit_log_stats); } },
			{ "-D", "--save-exit-stats", cmdline_arg_type_string,
				"Save Registers and Statistics at Exit",
				[&](std::string s) { stats_dirname = s; return (proc_logs |= proc_log_exit_save_stats); } },
			{ "-P", "--pc-usage-histogram", cmdline_arg_type_none,
				"Record program counter usage",
				[&](std::string s) { return (proc_logs |= proc_log_hist_pc); } },
			{ "-R", "--register-usage-histogram", cmdline_arg_type_none,
				"Record register usage",
				[&](std::string s) { return (proc_logs |= proc_log_hist_reg); } },
			{ "-I", "--instruction-usage-histogram", cmdline_arg_type_none,
				"Record instruction usage",
				[&](std::string s) { return (proc_logs |= proc_log_hist_inst); } },
			{ "-d", "--debug", cmdline_arg_type_none,
				"Start up in debugger",
				[&](std::string s) { return (proc_logs |= proc_log_ebreak_cli); } },
			{ "-T", "--debug-trap", cmdline_arg_type_none,
				"Start up in debugger and enter debugger on trap",
				[&](std::string s) { return (proc_logs |= (proc_log_ebreak_cli | proc_log_trap_cli)); } },
			{ "-x", "--no-pseudo", cmdline_arg_type_none,
				"Disable Pseudoinstruction decoding",
				[&](std::string s) { return (proc_logs |= proc_log_no_pseudo); } },
			{ "-p", "--map-physical", cmdline_arg_type_string,
				"Map execuatable at physical address",
				[&](std::string s) { return parse_integral(s, map_physical); } },
			{ "-b", "--binary", cmdline_arg_type_string,
				"Boot Binary ( 32, 64 )",
				[&](std::string s) { return parse_integral(s, ram_boot); } },
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
		boot_filename = result.first[0];
		for (size_t i = 0; i < result.first.size(); i++) {
			host_cmdline.push_back(result.first[i]);
		}

		/* filter host environment */
		for (const char** env = envp; *env != 0; env++) {
			if (allow_env_var(*env)) {
				host_env.push_back(*env);
			}
		}

		/* load ELF */
		if (ram_boot == 0) {
			elf.load(boot_filename, elf_load_headers);
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
		proc.mmu.mem->log = (proc.log & proc_log_memory);
		proc.stats_dirname = stats_dirname;

		/* randomise integer register state with 512 bits of entropy */
		proc.seed_registers(cpu, initial_seed, 512);

		/* ROM/FLASH exposed in the Config MMIO region */
		typename P::ux rom_base = 0, rom_size = 0, rom_entry = 0;

		if (ram_boot == 32 || ram_boot == 64) {
			struct stat statbuf;
			FILE *file = nullptr;
			memory_segment<typename P::ux> *segment = nullptr;

			/* Add 1GB RAM to the mmu */
			proc.mmu.mem->add_ram(default_ram_base, default_ram_size);

			addr_t ram_base = proc.mmu.mem->mpa_to_uva(segment, default_ram_base);
			if (segment == nullptr) {
				panic("unable to locate ram");
			}
			if (stat(boot_filename.c_str(), &statbuf) < 0) {
				panic("unable to stat boot file: %s", boot_filename.c_str());
			}
			if (!(file = fopen(boot_filename.c_str(), "r"))) {
				panic("unable to open boot file: %s", boot_filename.c_str());
			}
			ssize_t len = fread((void*)ram_base, 1, statbuf.st_size, file);
			if (len != statbuf.st_size) {
				panic("unable to read boot file: %s", boot_filename.c_str());
			}
			fclose(file);
			rom_base = default_ram_base;
			rom_size = statbuf.st_size;
			rom_entry = default_ram_base;
		} else {
			/* Find the ELF executable PT_LOAD segment base address */
			for (size_t i = 0; i < elf.phdrs.size(); i++) {
				Elf64_Phdr &phdr = elf.phdrs[i];
				if (phdr.p_flags & (PT_LOAD | PT_DYNAMIC)) {
					if (rom_base == 0) rom_base = phdr.p_vaddr;
					rom_size = phdr.p_vaddr + phdr.p_memsz - rom_base;
				}
			}

			/* Map the ELF executable PT_LOAD segments into the emulator mmu */
			typename P::ux map_offset = map_physical == 0 ? 0 : rom_base - map_physical;
			for (size_t i = 0; i < elf.phdrs.size(); i++) {
				Elf64_Phdr &phdr = elf.phdrs[i];
				if (phdr.p_flags & (PT_LOAD | PT_DYNAMIC)) {
					map_load_segment_priv(proc, boot_filename.c_str(), phdr, phdr.p_vaddr - map_offset);
				}
			}
			rom_base = rom_base - map_offset;
			rom_entry = elf.ehdr.e_entry - map_offset;

			/* Add 1GB RAM to the mmu */
			proc.mmu.mem->add_ram(default_ram_base, default_ram_size);
		}

		/* Initialize interpreter */
		proc.init();
		proc.reset(); /* Reset code calls mapped ROM image */
		proc.device_config->num_harts = 1;
		proc.device_config->time_base = 1000000000;
		proc.device_config->rom_base = rom_base;
		proc.device_config->rom_size = rom_size;
		proc.device_config->rom_entry = rom_entry;
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
		if (ram_boot == 0) {
			switch (elf.ei_class) {
				case ELFCLASS32:
					start_priv<priv_emulator_rv32imafdc>(); break;
				case ELFCLASS64:
					start_priv<priv_emulator_rv64imafdc>(); break;
			}
		}
		else if (ram_boot == 32) {
			start_priv<priv_emulator_rv32imafdc>();
		}
		else if (ram_boot == 64) {
			start_priv<priv_emulator_rv64imafdc>();
		} else {
			panic("--boot option must be 32 or 64");
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
