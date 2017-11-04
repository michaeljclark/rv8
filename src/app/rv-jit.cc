//
//  rv-jit.cc
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

using namespace riscv;

/* Parameterized ABI proxy JIT processor models */

using proxy_model_rv32imafdc = processor_rv32imafdc_model<
	jit_decode, processor_rv32imafd, mmu_proxy_rv32>;
using proxy_model_rv64imafdc = processor_rv64imafdc_model<
	jit_decode, processor_rv64imafd, mmu_proxy_rv64>;

using proxy_jit_rv32imafdc_fusion = jit_runloop<
	processor_proxy<proxy_model_rv32imafdc>,
	jit_fusion<jit_tracer<proxy_model_rv32imafdc,jit_isa_rv32>>,
	jit_emitter_rv32<proxy_model_rv32imafdc>>;
using proxy_jit_rv64imafdc_fusion = jit_runloop<
	processor_proxy<proxy_model_rv64imafdc>,
	jit_fusion<jit_tracer<proxy_model_rv64imafdc,jit_isa_rv64>>,
	jit_emitter_rv64<proxy_model_rv64imafdc>>;

using proxy_jit_rv32imafdc = jit_runloop<
	processor_proxy<proxy_model_rv32imafdc>,
	jit_tracer<proxy_model_rv32imafdc,jit_isa_rv32>,
	jit_emitter_rv32<proxy_model_rv32imafdc>>;
using proxy_jit_rv64imafdc = jit_runloop<
	processor_proxy<proxy_model_rv64imafdc>,
	jit_tracer<proxy_model_rv64imafdc,jit_isa_rv64>,
	jit_emitter_rv64<proxy_model_rv64imafdc>>;


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

/* RISC-V User Mode Emulator and Binary Translator */

struct rv_jit_emulator
{
	/*
		ABI/AEE RISC-V user mode interpreter and binary translator.
		Code is interpreted and profiled for hot paths which are translated
		to native x86-64 machine code and executed from a trace cache.

		A subset of linux syscalls are proxied to the host operating system

		(ABI) application binary interface
		(AEE) application execution environment
	*/

	enum jit_mode {
		jit_mode_none,
		jit_mode_trace,
		jit_mode_audit,
	};

	jit_mode mode = jit_mode_trace;
	host_cpu &cpu;
	int proc_logs = 0;
	int trace_iters = 100;
	int trace_length = 0;
	bool disable_fusion = false;
	bool memory_registers = false;
	bool update_instret = false;
	bool help_or_error = false;
	bool symbolicate = false;
	uint64_t initial_seed = 0;
	std::string elf_filename;
	std::string stats_dirname;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

	rv_jit_emulator() : cpu(host_cpu::get_instance()) {}

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
			{ "-S", "--symbolicate", cmdline_arg_type_none,
				"Symbolicate addresses in instruction log",
				[&](std::string s) { return (symbolicate = true); } },
			{ "-m", "--log-memory-map", cmdline_arg_type_none,
				"Log Memory Map Information",
				[&](std::string s) { return (proc_logs |= proc_log_memory); } },
			{ "-c", "--log-syscalls", cmdline_arg_type_none,
				"Log System Calls",
				[&](std::string s) { return (proc_logs |= proc_log_syscall); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers (defaults to integer registers)",
				[&](std::string s) { return (proc_logs |= proc_log_int_reg); } },
			{ "-T", "--log-jit-trace", cmdline_arg_type_none,
				"Log JIT trace",
				[&](std::string s) { return (proc_logs |= proc_log_jit_trace); } },
			{ "-T", "--log-jit-regalloc", cmdline_arg_type_none,
				"Log JIT register allocation",
				[&](std::string s) { return (proc_logs |= proc_log_jit_regalloc); } },
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
				"Start up in debugger CLI",
				[&](std::string s) { return (proc_logs |= proc_log_ebreak_cli); } },
			{ "-x", "--no-pseudo", cmdline_arg_type_none,
				"Disable Pseudoinstruction decoding",
				[&](std::string s) { return (proc_logs |= proc_log_no_pseudo); } },
			{ "-N", "--no-fusion", cmdline_arg_type_none,
				"Disable JIT macro-op fusion",
				[&](std::string s) { return (disable_fusion = true); } },
			{ "-M", "--memory-mapped-registers", cmdline_arg_type_none,
				"Disable JIT host register mapping",
				[&](std::string s) { return (memory_registers = true); } },
			{ "-i", "--update-instret", cmdline_arg_type_none,
				"Update instret in JIT code",
				[&](std::string s) { return (update_instret = true); } },
			{ "-t", "--no-trace", cmdline_arg_type_none,
				"Disable JIT tracer",
				[&](std::string s) { mode = jit_mode_none; return true; } },
			{ "-a", "--audit", cmdline_arg_type_none,
				"Enable JIT audit",
				[&](std::string s) { mode = jit_mode_audit; return true; } },
			{ "-I", "--trace-iters", cmdline_arg_type_string,
				"Trace iterations",
				[&](std::string s) { trace_iters = strtoull(s.c_str(), nullptr, 10); return true; } },
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
			printf("usage: %s [<emulator_options>] [--] <elf_file> [<options>]\n", argv[0]);
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
	}

	/* Start the execuatable with the given proxy processor template */
	template <typename P>
	void start_jit()
	{
		/* setup floating point exception mask */
		fenv_init();

		/* JIT mode */
		switch (mode) {
			case jit_mode_none:
				break;
			case jit_mode_trace:
				proc_logs |= proc_log_hist_pc | proc_log_jit_trap;
				break;
			case jit_mode_audit:
				proc_logs |= proc_log_jit_audit;
				break;
		}

		/* instantiate processor and set log options */
		P proc;
		proc.log = proc_logs;
		proc.mmu.mem->log = (proc.log & proc_log_memory);
		proc.stats_dirname = stats_dirname;
		if (symbolicate) proc.symlookup = [&](addr_t va) { return proc.symlookup_elf(va); };

		/* set JIT options */
		proc.trace_iters = trace_iters;
		proc.update_instret = update_instret;
		proc.memory_registers = memory_registers;

		/* randomise integer register state with 512 bits of entropy */
		proc.seed_registers(cpu, initial_seed, 512);

		/* Map ELF executable and setup the stack */
		proc.map_executable(elf_filename, host_cmdline, symbolicate);
		proc.map_proxy_stack(P::mmu_type::memory_top, P::mmu_type::stack_size);
		proc.setup_proxy_stack(cpu, host_cmdline, host_env,
			P::mmu_type::memory_top, P::mmu_type::stack_size);

		/* Initialize and run the processor */
		proc.init();
		proc.run(proc.log & proc_log_ebreak_cli ? exit_cause_cli : exit_cause_continue);
		proc.destroy();
	}

	/* Start a specific processor implementation based on ELF type */
	void exec()
	{
		elf_file elf;
		elf.load(elf_filename, elf_load_exec);

		/* check for RDTSCP on X86 */
		#if X86_USE_RDTSCP
		if (cpu.caps.size() > 0 && cpu.caps.find("RDTSCP") == cpu.caps.end()) {
			panic("error: x86 host without RDTSCP. Recompile with -DX86_NO_RDTSCP");
		}
		#endif

		/* execute */
		if (disable_fusion) {
			switch (elf.ei_class) {
				case ELFCLASS32:
					start_jit<proxy_jit_rv32imafdc>(); break;
					break;
				case ELFCLASS64:
					start_jit<proxy_jit_rv64imafdc>(); break;
					break;
				default: panic("illegal elf class");
			}
		} else {
			switch (elf.ei_class) {
				case ELFCLASS32:
					start_jit<proxy_jit_rv32imafdc_fusion>(); break;
					break;
				case ELFCLASS64:
					start_jit<proxy_jit_rv64imafdc_fusion>(); break;
					break;
				default: panic("illegal elf class");
			}
		}
	}
};


/* program main */

int main(int argc, const char *argv[], const char* envp[])
{
	rv_jit_emulator jit;
	jit.parse_commandline(argc, argv, envp);
	jit.exec();
	return 0;
}
