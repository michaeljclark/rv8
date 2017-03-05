//
//  test-asmjit.cc
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
#include "processor-proxy.h"
#include "debug-cli.h"
#include "processor-runloop.h"

#include "asmjit.h"

using namespace riscv;
using namespace asmjit;

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

struct rv_jit
{
	elf_file elf;
	host_cpu &cpu;
	int proc_logs = 0;
	bool help_or_error = false;
	std::string elf_filename;
	JitRuntime rt;

	std::vector<std::string> host_cmdline;
	std::vector<std::string> host_env;

	rv_jit() : cpu(host_cpu::get_instance()) {}

	static const int elf_p_flags_mmap(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= PROT_EXEC;
		if (v & PF_W) prot |= PROT_WRITE;
		if (v & PF_R) prot |= PROT_READ;
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

	/* Map ELF load segments into proxy MMU address space */
	template <typename P>
	void map_load_segment_user(P &proc, const char* filename, Elf64_Phdr &phdr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}
		addr_t map_delta = phdr.p_offset & (page_size-1);
		addr_t map_offset = phdr.p_offset - map_delta;
		addr_t map_vaddr = phdr.p_vaddr - map_delta;
		addr_t map_len = round_up(phdr.p_memsz + map_delta, page_size);
		void *addr = mmap((void*)map_vaddr, map_len,
			elf_p_flags_mmap(phdr.p_flags), MAP_FIXED | MAP_PRIVATE, fd, map_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		/* log elf load segment virtual address range */
		if (proc.log & proc_log_memory) {
			debug("mmap-elf :%016" PRIxPTR "-%016" PRIxPTR " %s offset=%" PRIxPTR,
				addr_t(map_vaddr), addr_t(map_vaddr + map_len),
				elf_p_flags_name(phdr.p_flags).c_str(), addr_t(map_offset));
		}

		/* add the mmap to the emulator proxy_mmu */
		proc.mmu.mem->segments.push_back(std::pair<void*,size_t>((void*)phdr.p_vaddr, phdr.p_memsz));
		addr_t seg_end = addr_t(map_vaddr + map_len);
		if (proc.mmu.mem->heap_begin < seg_end) {
			proc.mmu.mem->brk = proc.mmu.mem->heap_begin = proc.mmu.mem->heap_end = seg_end;
		}
	}

	void parse_commandline(int argc, const char* argv[], const char* envp[])
	{
		cmdline_option options[] =
		{
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1 && !help_or_error) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error)
		{
			printf("usage: %s [<options>] <asm_file>\n", argv[0]);
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
		elf.load(elf_filename);
	}

	template <typename T>
	struct fusion_matcher
	{
		enum match_state {
			match_state_none,
			match_state_auipc,
			match_state_call,
			match_state_la,
			match_state_li,
			match_state_lui,
		};

		u64 imm;
		int rd = -1;
		std::vector<T> decode_trace;
		match_state state = match_state_none;

		void emit(T &dec)
		{
			decode_pseudo_inst(dec);
			printf("\t%s\n", disasm_inst_simple(dec).c_str());
		}

		void emit_trace()
		{
			for (auto &dec : decode_trace) emit(dec);
			decode_trace.clear();
		}

		void clear_trace()
		{
			state = match_state_none;
			decode_trace.clear();
		}

		void emit_li()
		{
			clear_trace();
			printf("\tli %s, 0x%llx\n", rv_ireg_name_sym[rd], imm);
		}

		void emit_la()
		{
			clear_trace();
			printf("\tla.pcrel %s, 0x%llx\n", rv_ireg_name_sym[rd], imm);
		}

		void emit_call()
		{
			clear_trace();
			printf("\tcall 0x%llx\n", imm);
		}

		void match(T &dec)
		{
		reparse:
			switch(state) {
				case match_state_none:
					switch (dec.op) {
						case rv_op_addi:
							if (dec.rs1 == rv_ireg_zero) {
								rd = dec.rd;
								imm = dec.imm;
								state = match_state_li;
								decode_trace.push_back(dec);
								return;
							}
							break;
						case rv_op_auipc:
							rd = dec.rd;
							imm = dec.imm;
							state = match_state_auipc;
							decode_trace.push_back(dec);
							return;
						case rv_op_lui:
							rd = dec.rd;
							imm = dec.imm;
							state = match_state_lui;
							decode_trace.push_back(dec);
							return;
						default:
							break;
					}
					break;
				case match_state_auipc:
					switch (dec.op) {
						case rv_op_addi: state = match_state_la; goto reparse;
						case rv_op_jalr: state = match_state_call; goto reparse;
						default:
							emit_trace();
							break;
					}
					break;
				case match_state_lui:
					switch (dec.op) {
						case rv_op_slli: state = match_state_li; goto reparse;
						case rv_op_addi: state = match_state_li; goto reparse;
						default:
							emit_trace();
							break;
					}
					break;
				case match_state_li:
					switch (dec.op) {
						case rv_op_addi:
							if (rd == dec.rd && rd == dec.rs1) {
								imm += dec.imm;
								return;
							}
							break;
						case rv_op_slli:
							if (rd == dec.rd && rd == dec.rs1) {
								imm <<= dec.imm;
								return;
							}
							break;
						default:
							break;
					}
					emit_li();
					break;
				case match_state_la:
					if (rd == dec.rd && rd == dec.rs1) {
						imm += dec.imm;
						emit_la();
						return;
					}
					emit_trace();
					break;
				case match_state_call:
					if (rd == dec.rs1 && dec.rd == rv_ireg_ra) {
						imm += dec.imm;
						emit_call();
						return;
					}
					emit_trace();
					break;
			}
			emit(dec);
		}
	};

	template <typename P>
	void trace(P proc)
	{
		fusion_matcher<typename P::decode_type> m;

		for(;;) {
			typename P::decode_type dec;
			addr_t pc_offset, new_offset;
			inst_t inst = proc.mmu.inst_fetch(proc, proc.pc, pc_offset);
			proc.inst_decode(dec, inst);
			m.match(dec);
			if ((new_offset = proc.inst_exec(dec, pc_offset)) == -1) break;
			proc.pc += new_offset;
		}
	}

	/* Start the execuatable with the given proxy processor template */
	template <typename P>
	void start_jit()
	{
		/* setup floating point exception mask */
		fenv_init();

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.log = proc_logs;
		proc.pc = elf.ehdr.e_entry;
		proc.mmu.mem->log = (proc.log & proc_log_memory);

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

		/* Trace */
		trace(proc);
	}

	void exec()
	{
		/* execute */
		switch (elf.ei_class) {
			case ELFCLASS32:
				panic("rv32 not implemented"); break;
				break;
			case ELFCLASS64:
				start_jit<proxy_emulator_rv64imafdc>(); break;
				break;
			default: panic("illegal elf class");
		}
	}
};

int main(int argc, const char *argv[], const char* envp[])
{
	printf("\n");
	printf("rv-jit-0.0.0-prealpha-0\n");
	printf("\n");
	rv_jit jit;
	jit.parse_commandline(argc, argv, envp);
	jit.exec();
	return 0;
}
