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

using proxy_jit_rv32imafdc_memreg = jit_runloop<
	processor_proxy<proxy_model_rv32imafdc>,
	jit_tracer<proxy_model_rv32imafdc,jit_isa_rv32>,
	jit_emitter_rv32<proxy_model_rv32imafdc,true>>;
using proxy_jit_rv64imafdc_memreg = jit_runloop<
	processor_proxy<proxy_model_rv64imafdc>,
	jit_tracer<proxy_model_rv64imafdc,jit_isa_rv64>,
	jit_emitter_rv64<proxy_model_rv64imafdc,true>>;

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

struct rv_jit
{
	enum jit_mode {
		jit_mode_none,
		jit_mode_trace,
		jit_mode_audit,
	};

	jit_mode mode = jit_mode_trace;
	elf_file elf;
	uintptr_t imagebase = 0;
	host_cpu &cpu;
	int proc_logs = 0;
	int trace_iters = 100;
	int trace_length = 0;
	bool disable_fusion = false;
	bool memory_registers = false;
	bool update_instret = false;
	bool help_or_error = false;
	std::string elf_filename;
	std::string stats_dirname;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

	rv_jit() : cpu(host_cpu::get_instance()) {}

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
			{ "-m", "--log-memory-map", cmdline_arg_type_none,
				"Log Memory Map Information",
				[&](std::string s) { return (proc_logs |= proc_log_memory); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers (defaults to integer registers)",
				[&](std::string s) { return (proc_logs |= proc_log_int_reg); } },
			{ "-T", "--log-jit-trace", cmdline_arg_type_none,
				"Log JIT trace",
				[&](std::string s) { return (proc_logs |= proc_log_jit_trace); } },
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
			{ "-L", "--trace-length", cmdline_arg_type_string,
				"Trace length",
				[&](std::string s) { trace_length = strtoull(s.c_str(), nullptr, 10); return true; } },
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

		if (help_or_error)
		{
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

		/* load ELF (headers only) */
		elf.load(elf_filename, true);
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

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.log = proc_logs;
		proc.pc = elf.ehdr.e_entry;
		proc.mmu.mem->log = (proc.log & proc_log_memory);
		proc.stats_dirname = stats_dirname;
		proc.trace_iters = trace_iters;
		proc.trace_length = trace_length;
		proc.update_instret = update_instret;

		/* Find the ELF executable PT_LOAD segments and mmap them into user memory */
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & (PT_LOAD | PT_DYNAMIC)) {
				proc.map_load_segment_user(elf_filename.c_str(), phdr);
			}
		}

		/* Map a stack and set the stack pointer */
		static const size_t stack_size = 0x00100000; // 1 MiB
		proc.map_proxy_stack(P::mmu_type::memory_top, stack_size);
		proc.setup_proxy_stack(elf, cpu, host_cmdline, host_env, P::mmu_type::memory_top, stack_size);

		/* Initialize interpreter */
		proc.init();

		/* Run the CPU until it halts */
		proc.run(proc.log & proc_log_ebreak_cli
			? exit_cause_cli : exit_cause_continue);

		/* Unmap memory segments */
		for (auto &seg: proc.mmu.mem->segments) {
			guest_munmap(seg.first, seg.second);
		}
	}

	void exec()
	{
		/* execute */
		if (memory_registers) {
			switch (elf.ei_class) {
				case ELFCLASS32:
					start_jit<proxy_jit_rv32imafdc_memreg>(); break;
					break;
				case ELFCLASS64:
					start_jit<proxy_jit_rv64imafdc_memreg>(); break;
					break;
				default: panic("illegal elf class");
			}
		}
		else if (disable_fusion) {
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
	rv_jit jit;
	jit.parse_commandline(argc, argv, envp);
	jit.exec();
	return 0;
}
