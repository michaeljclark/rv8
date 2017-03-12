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

#include "asmjit.h"

using namespace riscv;
using namespace asmjit;

template <typename P>
struct fusion_tracer : public ErrorHandler
{
	struct decode : P::decode_type
	{
		addr_t pc;
		inst_t inst;

		decode(typename P::decode_type decode) : P::decode_type(decode) {}
	};

	enum match_state {
		match_state_none,
		match_state_auipc,
		match_state_call,
		match_state_la,
		match_state_li,
		match_state_lui,
	};

	P &proc;
	X86Assembler as;
	u64 imm;
	int rd;
	addr_t term_pc;
	addr_t pseudo_pc;
	match_state state;
	Label term;
	std::map<addr_t,Label> labels;
	std::vector<decode> decode_trace;

	fusion_tracer(P &proc, CodeHolder &code)
		: proc(proc), as(&code), imm(0), rd(0), state(match_state_none)
	{
		code.setErrorHandler(this);
	}

	virtual bool handleError(Error err, const char* message, CodeEmitter* origin) {
		printf("%s\n", message);
		return false;
	}

	const char* x86_reg_str(int rdx)
	{
		switch (rdx) {
			case 0: return "rax";
			case 1: return "rcx";
			case 2: return "rdx";
			case 3: return "rbx";
			case 4: return "rsp";
			case 5: return "rbp";
			case 6: return "rsi";
			case 7: return "rdi";
			case 8: return "r8";
			case 9: return "r9";
			case 10: return "r10";
			case 11: return "r11";
			case 12: return "r12";
			case 13: return "r13";
			case 14: return "r14";
			case 15: return "r15";
		}
		return "";
	}

	const char* frame_reg_str(int reg)
	{
		static char buf[32];
		snprintf(buf, sizeof(buf), "[rbp + %lu]", offsetof(processor_rv64imafd, ireg) + reg * (P::xlen >> 3));
		return buf;
	}

	static int x86_reg(int rd)
	{
		switch (rd) {
			case rv_ireg_zero: return 0;
			case rv_ireg_ra: return 1;  /* rcx */
			case rv_ireg_sp: return 3;  /* rbx */
			case rv_ireg_t0: return 6;  /* rsi */
			case rv_ireg_t1: return 7;  /* rdi */
			case rv_ireg_a0: return 8;  /* r8  */
			case rv_ireg_a1: return 9;  /* r9  */
			case rv_ireg_a2: return 10; /* r10 */
			case rv_ireg_a3: return 11; /* r11 */
			case rv_ireg_a4: return 12; /* r12 */
			case rv_ireg_a5: return 13; /* r13 */
			case rv_ireg_a6: return 14; /* r14 */
			case rv_ireg_a7: return 15; /* r15 */
		}
		return -1;
	}

	const X86Mem frame_reg(int reg)
	{
		return x86::ptr(x86::rbp, offsetof(processor_rv64imafd, ireg) + reg * (P::xlen >> 3));
	}

	void emit_prolog()
	{
		as.push(x86::rbp);
		as.push(x86::rbx);
		as.push(x86::r12);
		as.push(x86::r13);
		as.push(x86::r14);
		as.push(x86::r15);
		as.mov(x86::rbp, x86::rdi);
		as.mov(x86::rcx, frame_reg(rv_ireg_ra));
		as.mov(x86::rbx, frame_reg(rv_ireg_sp));
		as.mov(x86::rsi, frame_reg(rv_ireg_t0));
		as.mov(x86::rdi, frame_reg(rv_ireg_t1));
		as.mov(x86::r8,  frame_reg(rv_ireg_a0));
		as.mov(x86::r9,  frame_reg(rv_ireg_a1));
		as.mov(x86::r10, frame_reg(rv_ireg_a2));
		as.mov(x86::r11, frame_reg(rv_ireg_a3));
		as.mov(x86::r12, frame_reg(rv_ireg_a4));
		as.mov(x86::r13, frame_reg(rv_ireg_a5));
		as.mov(x86::r14, frame_reg(rv_ireg_a6));
		as.mov(x86::r15, frame_reg(rv_ireg_a7));
		term = as.newLabel();
	}

	void emit_epilog()
	{
		as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)term_pc);
		printf("\t\tmov [rbp + %lu], 0x%llx\n", offsetof(processor_rv64imafd, pc), term_pc);
		printf("\t\tterm:\n");
		as.bind(term);
		as.mov(frame_reg(rv_ireg_ra), x86::rcx);
		as.mov(frame_reg(rv_ireg_sp), x86::rbx);
		as.mov(frame_reg(rv_ireg_t0), x86::rsi);
		as.mov(frame_reg(rv_ireg_t1), x86::rdi);
		as.mov(frame_reg(rv_ireg_a0), x86::r8);
		as.mov(frame_reg(rv_ireg_a1), x86::r9);
		as.mov(frame_reg(rv_ireg_a2), x86::r10);
		as.mov(frame_reg(rv_ireg_a3), x86::r11);
		as.mov(frame_reg(rv_ireg_a4), x86::r12);
		as.mov(frame_reg(rv_ireg_a5), x86::r13);
		as.mov(frame_reg(rv_ireg_a6), x86::r14);
		as.mov(frame_reg(rv_ireg_a7), x86::r15);
		as.pop(x86::r15);
		as.pop(x86::r14);
		as.pop(x86::r13);
		as.pop(x86::r12);
		as.pop(x86::rbx);
		as.pop(x86::rbp);
		as.ret();
	}

	void emit_trace()
	{
		state = match_state_none;
		for (auto &dec : decode_trace) emit(dec);
		decode_trace.clear();
	}

	void clear_trace()
	{
		state = match_state_none;
		decode_trace.clear();
	}

	bool emit_add(decode &dec)
	{
		int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
		if (dec.rd == rv_ireg_zero) {
			// nop
		}
		else if (dec.rs1 == rv_ireg_zero) {
			// mov rd, rs2
			if (rdx > 0) {
				if (rs2x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs2x));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), x86_reg_str(rs2x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg(dec.rs2));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), frame_reg_str(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.mov(frame_reg(dec.rd), x86::gpq(rs2x));
					printf("\t\tmov %s, %s\n", frame_reg_str(dec.rd), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs2));
					as.mov(frame_reg(dec.rd), x86::rax);
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs2));
					printf("\t\tmov %s, rax\n", frame_reg_str(dec.rd));
				}
			}
		}
		else if (dec.rs2 == rv_ireg_zero) {
			// mov rd, rs1
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg(dec.rs1));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), frame_reg_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(frame_reg(dec.rd), x86::gpq(rs1x));
					printf("\t\tmov %s, %s\n", frame_reg_str(dec.rd), x86_reg_str(rs1x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(frame_reg(dec.rd), x86::rax);
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs1));
					printf("\t\tmov %s, rax\n", frame_reg_str(dec.rd));
				}
			}
		}
		else if (dec.rd == dec.rs1) {
			// add rd, rs2
			if (rdx > 0) {
				if (rs2x > 0) {
					as.add(x86::gpq(rdx), x86::gpq(rs2x));
					printf("\t\tadd %s, %s\n", x86_reg_str(rdx), x86_reg_str(rs2x));
				} else {
					as.add(x86::gpq(rdx), frame_reg(dec.rs2));
					printf("\t\tadd %s, %s\n", x86_reg_str(rdx), frame_reg_str(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.add(frame_reg(dec.rd), x86::gpq(rs2x));
					printf("\t\tadd %s, %s\n", frame_reg_str(dec.rd), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs2));
					as.add(frame_reg(dec.rd), x86::rax);
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs2));
					printf("\t\tadd %s, rax\n", frame_reg_str(dec.rd));
				}
			}
		}
		else {
			// mov rd, rs1
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg(dec.rs1));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), frame_reg_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(frame_reg(dec.rd), x86::gpq(rs1x));
					printf("\t\tmov %s, %s\n", frame_reg_str(dec.rd), x86_reg_str(rs1x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(frame_reg(dec.rd), x86::rax);
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs1));
					printf("\t\tadd %s, rax\n", frame_reg_str(dec.rd));
				}
			}
			// add rs, rs2
			if (rdx > 0) {
				if (rs2x > 0) {
					as.add(x86::gpq(rdx), x86::gpq(rs2x));
					printf("\t\tadd %s, %s\n", x86_reg_str(rdx), x86_reg_str(rs2x));
				} else {
					as.add(x86::gpq(rdx), frame_reg(dec.rs2));
					printf("\t\tadd %s, %s\n", x86_reg_str(rdx), frame_reg_str(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.add(frame_reg(dec.rd), x86::gpq(rs2x));
					printf("\t\tadd %s, %s\n", frame_reg_str(dec.rd), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs2));
					as.add(frame_reg(dec.rd), x86::rax);
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs2));
					printf("\t\tadd %s, rax\n", frame_reg_str(dec.rd));
				}
			}
		}
		return true;
	}

	bool emit_addi(decode &dec)
	{
		int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
		if (dec.rd == rv_ireg_zero) {
			// nop
		}
		else if (dec.rs1 == rv_ireg_zero) {
			// mov rd, imm
			if (rdx > 0) {
				as.mov(x86::gpq(rdx), dec.imm);
				printf("\t\tmov %s, %d\n", x86_reg_str(rdx), dec.imm);
			} else {
				as.mov(frame_reg(dec.rd), dec.imm);
				printf("\t\tmov %s, %d\n", frame_reg_str(dec.rd), dec.imm);
			}
		}
		else if (dec.rd == dec.rs1) {
			// add rd, imm
			if (rdx > 0) {
				as.add(x86::gpq(rdx), dec.imm);
				printf("\t\tadd %s, %d\n", x86_reg_str(rdx), dec.imm);
			} else {
				as.add(frame_reg(dec.rd), dec.imm);
				printf("\t\tadd %s, %d\n", frame_reg_str(dec.rd), dec.imm);
			}
		}
		else {
			// mov rd, rs1
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg(dec.rs1));
					printf("\t\tmov %s, %s\n", x86_reg_str(rdx), frame_reg_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(frame_reg(dec.rd), x86::gpq(rs1x));
					printf("\t\tmov %s, %s\n", frame_reg_str(dec.rd), x86_reg_str(rs1x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(frame_reg(dec.rd), x86::rax);
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs1));
					printf("\t\tadd %s, rax\n", frame_reg_str(dec.rd));
				}
			}
			// add rd, imm
			if (rdx > 0) {
				as.add(x86::gpq(rdx), dec.imm);
				printf("\t\tadd %s, %d\n", x86_reg_str(rdx), dec.imm);
			} else {
				as.add(frame_reg(dec.rd), dec.imm);
				printf("\t\tadd %s, %d\n", frame_reg_str(dec.rd), dec.imm);
			}
		}
		return true;
	}

	bool emit_bne(decode &dec)
	{
		int rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
		if (dec.rs1 == rv_ireg_zero) {
			if (rs2x > 0) {
				as.cmp(x86::gpq(rs2x), 0);
				printf("\t\tcmp %s, 0\n", x86_reg_str(rs2x));
			} else {
				as.cmp(frame_reg(dec.rs2), 0);
				printf("\t\tcmp %s, 0\n", frame_reg_str(dec.rs2));
			}
		}
		else if (dec.rs2 == rv_ireg_zero) {
			if (rs1x > 0) {
				as.cmp(x86::gpq(rs1x), 0);
				printf("\t\tcmp %s, 0\n", x86_reg_str(rs1x));
			} else {
				as.cmp(frame_reg(dec.rs1), 0);
				printf("\t\tcmp %s, 0\n", frame_reg_str(dec.rs1));
			}
		}
		else if (rs1x > 0) {
			if (rs2x > 0) {
				as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
				printf("\t\tcmp %s, %s\n", x86_reg_str(rs1x), x86_reg_str(rs2x));
			} else {
				as.cmp(x86::gpq(rs1x), frame_reg(dec.rs2));
				printf("\t\tcmp %s, %s\n", x86_reg_str(rs1x), frame_reg_str(dec.rs2));
			}
		}
		else {
			if (rs2x > 0) {
				as.cmp(frame_reg(dec.rs1), x86::gpq(rs2x));
				printf("\t\tcmp %s, %s\n", frame_reg_str(dec.rs1), x86_reg_str(rs2x));
			} else {
				as.mov(x86::rax, frame_reg(dec.rs1));
				as.cmp(x86::rax, frame_reg(dec.rs2));
				printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs1));
				printf("\t\tcmp rax, %s\n", frame_reg_str(dec.rs1));
			}
		}

		bool cond = proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val;
		addr_t branch_pc = dec.pc + dec.imm;
		addr_t cont_pc = dec.pc + inst_length(dec.inst);
		if (cond) {
			auto li = labels.find(branch_pc);
			if (li == labels.end()) {
				Label l = as.newLabel();
				as.jne(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				printf("\t\tjne 1f\n");
				printf("\t\tmov [rbp + %lu], 0x%llx\n", offsetof(processor_rv64imafd, pc), cont_pc);
				printf("\t\tjmp term\n");
				printf("\t\t1:\n");
			} else {
				as.jne(li->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				printf("\t\tjne pc_0x%016llx\n", branch_pc);
				printf("\t\tmov [rbp + %lu], 0x%llx\n", offsetof(processor_rv64imafd, pc), cont_pc);
				printf("\t\tjmp term\n");
			}
		}
		else {
			auto li = labels.find(cont_pc);
			if (li == labels.end()) {
				Label l = as.newLabel();
				as.je(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				as.bind(l);
				printf("\t\tje 1f\n");
				printf("\t\tmov [rbp + %lu], 0x%llx\n", offsetof(processor_rv64imafd, pc), branch_pc);
				printf("\t\tjmp term\n");
				printf("\t\t1:\n");
			} else {
				as.je(li->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				printf("\t\tje pc_0x%016llx\n", cont_pc);
				printf("\t\tmov [rbp + %lu], 0x%llx\n", offsetof(processor_rv64imafd, pc), branch_pc);
				printf("\t\tjmp term\n");
			}
		}

		return true;
	}

	bool emit_ld(decode &dec)
	{
		int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
		if (dec.rd == rv_ireg_zero) {
			// nop
		}
		else {
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), dec.imm));
					printf("\t\tmov %s, qword ptr [%s + %d]\n", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(x86::gpq(rdx), x86::qword_ptr(x86::rax, dec.imm));
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs1));
					printf("\t\tmov %s, qword ptr [rax + %d]\n", x86_reg_str(rdx), dec.imm);
				}
			} else {
				if (rs1x > 0) {
					as.mov(x86::rax, x86::qword_ptr(x86::gpq(rs1x), dec.imm));
					printf("\t\tmov rax, qword ptr [%s + %d]\n", x86_reg_str(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(x86::rax, x86::qword_ptr(x86::rax, dec.imm));
					printf("\t\tmov rax, %s\n", frame_reg_str(dec.rs1));
					printf("\t\tmov rax, qword ptr [rax + %d]\n", dec.imm);
				}
				as.mov(frame_reg(dec.rd), x86::rax);
				printf("\t\tmov %s, rax\n", frame_reg_str(dec.rd));
			}
		}
		return true;
	}

	bool emit_li()
	{
		int rdx = x86_reg(rd);
		clear_trace();
		printf("\t# 0x%016llx\tli\t%s, 0x%llx\n", pseudo_pc, rv_ireg_name_sym[rd], imm);
		if (rd == rv_ireg_zero) {
			// nop
		} else {
			if (rdx > 0) {
				as.mov(x86::gpq(rdx), (unsigned)imm);
				printf("\t\tmov %s, %lld\n", x86_reg_str(rdx), imm);
			} else {
				as.mov(frame_reg(rd), (unsigned)imm);
				printf("\t\tmov %s, %lld\n", frame_reg_str(rd), imm);
			}
		}
		return true;
	}

	bool emit_la()
	{
		clear_trace();
		/* TODO - emit asm */
		printf("\t# 0x%016llx\tla\t%s, 0x%llx\n", pseudo_pc, rv_ireg_name_sym[rd], imm);
		return false;
	}

	bool emit_call()
	{
		clear_trace();
		/* TODO - emit asm */
		printf("\t# 0x%016llx\tcall\t0x%llx\n", pseudo_pc, imm);
		return false;
	}

	bool emit(decode &dec)
	{
		printf("\t# 0x%016llx\t%s\n", dec.pc, disasm_inst_simple(dec).c_str());
		switch(dec.op) {
			case rv_op_add: return emit_add(dec);
			case rv_op_addi: return emit_addi(dec);
			case rv_op_bne: return emit_bne(dec);
			case rv_op_ld: return emit_ld(dec);
		}
		return false;
	}

	bool trace(addr_t pc, decode dec, inst_t inst)
	{
		dec.pc = pc;
		dec.inst = inst;
		auto li = labels.find(pc);
		if (li != labels.end()) {
			term_pc = pc;
			emit_trace();
			return false; /* trace complete */
		}
		Label l = as.newLabel();
		labels[pc] = l;
		as.bind(l);
		proc.histogram_set_pc(pc, P::hostspot_trace_skip);

	reparse:
		switch(state) {
			case match_state_none:
				switch (dec.op) {
					case rv_op_addi:
						if (dec.rs1 == rv_ireg_zero) {
							rd = dec.rd;
							imm = dec.imm;
							pseudo_pc = dec.pc;
							state = match_state_li;
							decode_trace.push_back(dec);
							return true;
						}
						break;
					case rv_op_auipc:
						rd = dec.rd;
						imm = dec.imm;
						pseudo_pc = dec.pc;
						state = match_state_auipc;
						decode_trace.push_back(dec);
						return true;
					case rv_op_lui:
						rd = dec.rd;
						imm = dec.imm;
						pseudo_pc = dec.pc;
						state = match_state_lui;
						decode_trace.push_back(dec);
						return true;
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
							return true;
						}
						break;
					case rv_op_slli:
						if (rd == dec.rd && rd == dec.rs1) {
							imm <<= dec.imm;
							return true;
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
					return emit_la();
				}
				emit_trace();
				break;
			case match_state_call:
				if (rd == dec.rs1 && dec.rd == rv_ireg_ra) {
					imm += dec.imm;
					return emit_call();
				}
				emit_trace();
				state = match_state_none;
				break;
		}

		term_pc = pc;
		return emit(dec);
	}
};

struct processor_fault
{
	static processor_fault *current;
};

processor_fault* processor_fault::current = nullptr;

template <typename P>
struct processor_runloop : processor_fault, P
{
	static const size_t inst_cache_size = 8191;
	static const int inst_step = 100000;

	typedef int (*TraceFunc)(processor_rv64imafd *proc);

	std::shared_ptr<debug_cli<P>> cli;
	std::map<addr_t,TraceFunc> trace_cache;
	JitRuntime rt;

	struct rv_inst_cache_ent
	{
		inst_t inst;
		typename P::decode_type dec;
	};

	rv_inst_cache_ent inst_cache[inst_cache_size];

	processor_runloop() : cli(std::make_shared<debug_cli<P>>()) {}
	processor_runloop(std::shared_ptr<debug_cli<P>> cli) : cli(cli) {}

	static void signal_handler(int signum, siginfo_t *info, void *)
	{
		static_cast<processor_runloop<P>*>
			(processor_fault::current)->signal_dispatch(signum, info);
	}

	void signal_dispatch(int signum, siginfo_t *info)
	{
		printf("SIGNAL   :%s pc:0x%0llx si_addr:0x%0llx\n",
			signal_name(signum), (addr_t)P::pc, (addr_t)info->si_addr);

		/* let the processor longjmp */
		P::signal(signum, info);
	}

	void init()
	{
		// block signals before so we don't deadlock in signal handlers
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set, SIGSEGV);
		sigaddset(&set, SIGTERM);
		sigaddset(&set, SIGQUIT);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGHUP);
		sigaddset(&set, SIGUSR1);
		if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
			panic("can't set thread signal mask: %s", strerror(errno));
		}

		// disable unwanted signals
		sigset_t sigpipe_set;
		sigemptyset(&sigpipe_set);
		sigaddset(&sigpipe_set, SIGPIPE);
		sigprocmask(SIG_BLOCK, &sigpipe_set, nullptr);

		// install signal handler
		struct sigaction sigaction_handler;
		memset(&sigaction_handler, 0, sizeof(sigaction_handler));
		sigaction_handler.sa_sigaction = &processor_runloop<P>::signal_handler;
		sigaction_handler.sa_flags = SA_SIGINFO;
		sigaction(SIGSEGV, &sigaction_handler, nullptr);
		sigaction(SIGTERM, &sigaction_handler, nullptr);
		sigaction(SIGQUIT, &sigaction_handler, nullptr);
		sigaction(SIGINT, &sigaction_handler, nullptr);
		sigaction(SIGHUP, &sigaction_handler, nullptr);
		sigaction(SIGUSR1, &sigaction_handler, nullptr);
		processor_fault::current = this;

		/* unblock signals */
		if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
			panic("can't set thread signal mask: %s", strerror(errno));
		}

		/* processor initialization */
		P::init();
	}

	void run(exit_cause ex = exit_cause_continue)
	{
		u32 logsave = P::log;
		size_t count = inst_step;
		for (;;) {
			switch (ex) {
				case exit_cause_continue:
					break;
				case exit_cause_cli:
					P::debugging = true;
					count = cli->run(this);
					if (count == size_t(-1)) {
						P::debugging = false;
						P::log = logsave;
						count = inst_step;
					} else {
						P::log |= (proc_log_inst | proc_log_operands | proc_log_trap);
					}
					break;
				case exit_cause_poweroff:
					return;
			}
			ex = step(count);
			if (P::debugging && ex == exit_cause_continue) {
				ex = exit_cause_cli;
			}
		}
	}

	void start_trace()
	{
		CodeHolder code;
		code.init(rt.getCodeInfo());
		fusion_tracer<P> tracer(*this, code);
		typename P::ux trace_pc = P::pc;
		typename P::ux trace_instret = P::instret;

		printf("trace-begin pc=0x%016llx\n", P::pc);

		P::log &= ~proc_log_hotspot_trap;

		tracer.emit_prolog();

		for(;;) {
			typename P::decode_type dec;
			addr_t pc_offset, new_offset;
			inst_t inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
			P::inst_decode(dec, inst);
			if (tracer.trace(P::pc, dec, inst) == false) {
				break;
			}
			if ((new_offset = P::inst_exec(dec, pc_offset)) == -1) {
				break;
			}
			P::pc += new_offset;
			P::cycle++;
			P::instret++;
		}

		tracer.emit_epilog();

		P::log |= proc_log_hotspot_trap;

		printf("trace-end   pc=0x%016llx\n\n", P::pc);

		if (P::instret == trace_instret) return;

		TraceFunc fn;
		Error err = rt.add(&fn, &code);
		if (!err) {
			trace_cache[trace_pc] = fn;
			P::histogram_set_pc(trace_pc, P::hostspot_trace_cached);
		}
	}

	void exec_trace()
	{
		TraceFunc fn = trace_cache[P::pc];
		fn(static_cast<processor_rv64imafd*>(this));
	}

	exit_cause step(size_t count)
	{
		typename P::decode_type dec;
		typename P::ux inststop = P::instret + count;
		addr_t pc_offset, new_offset;
		inst_t inst = 0;

		/* interrupt service routine */
		P::time = cpu_cycle_clock();
		P::isr();

		/* trap return path */
		int cause;
		if (unlikely((cause = setjmp(P::env)) > 0)) {
			cause -= P::internal_cause_offset;
			switch(cause) {
				case P::internal_cause_cli:
					return exit_cause_cli;
				case P::internal_cause_fatal:
					P::print_csr_registers();
					P::print_int_registers();
					return exit_cause_poweroff;
				case P::internal_cause_poweroff:
					return exit_cause_poweroff;
				case P::internal_cause_hotspot:
					start_trace();
					return exit_cause_continue;
				case P::internal_cause_traced:
					exec_trace();
					return exit_cause_continue;
			}
			P::trap(dec, cause);
			if (!P::running) return exit_cause_poweroff;
		}

		/* step the processor */
		while (P::instret < inststop) {
			if (P::pc == P::breakpoint && P::breakpoint != 0) {
				return exit_cause_cli;
			}
			if (P::log & proc_log_hotspot_trap) {
				auto ti = trace_cache.find(P::pc);
				if (ti != trace_cache.end()) {
					ti->second(static_cast<processor_rv64imafd*>(this));
					continue;
				}
			}
			inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
			inst_t inst_cache_key = inst % inst_cache_size;
			if (inst_cache[inst_cache_key].inst == inst) {
				dec = inst_cache[inst_cache_key].dec;
			} else {
				P::inst_decode(dec, inst);
				inst_cache[inst_cache_key].inst = inst;
				inst_cache[inst_cache_key].dec = dec;
			}
			if ((new_offset = P::inst_exec(dec, pc_offset)) != -1  ||
				(new_offset = P::inst_priv(dec, pc_offset)) != -1)
			{
				if (P::log) P::print_log(dec, inst);
				P::pc += new_offset;
				P::cycle++;
				P::instret++;
			} else {
				P::raise(rv_cause_illegal_instruction, P::pc);
			}
		}
		return exit_cause_continue;
	}
};

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
	int trace_iters = 100;
	bool help_or_error = false;
	std::string elf_filename;

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
			{ "-d", "--debug", cmdline_arg_type_none,
				"Start up in debugger CLI",
				[&](std::string s) { return (proc_logs |= proc_log_ebreak_cli); } },
			{ "-x", "--no-pseudo", cmdline_arg_type_none,
				"Disable Pseudoinstruction decoding",
				[&](std::string s) { return (proc_logs |= proc_log_no_pseudo); } },
			{ "-t", "--trace", cmdline_arg_type_none,
				"Enable hotspot tracer",
				[&](std::string s) { proc_logs |= proc_log_hist_pc | proc_log_hotspot_trap; return true; } },
			{ "-l", "--trace-iters", cmdline_arg_type_string,
				"Hotspot trace iterations",
				[&](std::string s) { trace_iters = strtoull(s.c_str(), nullptr, 10); return true; } },
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
		proc.hotspot_iters = trace_iters;

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

		/* Run the CPU until it halts */
		proc.run(proc.log & proc_log_ebreak_cli
			? exit_cause_cli : exit_cause_continue);

		/* Unmap memory segments */
		for (auto &seg: proc.mmu.mem->segments) {
			munmap(seg.first, seg.second);
		}
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
