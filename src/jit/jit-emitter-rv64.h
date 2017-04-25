//
//  jit-emitter-rv64.h
//

#ifndef rv_jit_emitter_rv64_h
#define rv_jit_emitter_rv64_h

namespace riscv {

	using namespace asmjit;

	template <typename P>
	struct jit_emitter_rv64
	{
		typedef P processor_type;
		typedef typename P::decode_type decode_type;

		#define proc_offset(member) offsetof(typename P::processor_type, member)

		P &proc;
		X86Assembler as;
		Label term;
		std::map<addr_t,Label> labels;
		std::vector<addr_t> callstack;
		u64 term_pc;

		jit_emitter_rv64(P &proc, CodeHolder &code)
			:proc(proc),  as(&code), term_pc(0)
		{}

		void log_trace(const char* fmt, ...)
		{
			if (proc.log & proc_log_jit_trace) {
				char buf[128];
				va_list arg;
				va_start(arg, fmt);
				vsnprintf(buf, sizeof(buf), fmt, arg);
				va_end(arg);
				printf("%s\n", buf);
			}
		}

		virtual bool handleError(Error err, const char* message, CodeEmitter* origin) {
			log_trace("%s", message);
			return false;
		}

		const char* x86_reg_str_b(int rdx)
		{
			switch (rdx) {
				case 0: return "al";
				case 1: return "cl";
				case 2: return "dl";
				case 3: return "bl";
				case 4: return "spl";
				case 5: return "bpl";
				case 6: return "sil";
				case 7: return "dil";
				case 8: return "r8b";
				case 9: return "r9b";
				case 10: return "r10b";
				case 11: return "r11b";
				case 12: return "r12b";
				case 13: return "r13b";
				case 14: return "r14b";
				case 15: return "r15b";
			}
			return "";
		}

		const char* x86_reg_str_w(int rdx)
		{
			switch (rdx) {
				case 0: return "ax";
				case 1: return "cx";
				case 2: return "dx";
				case 3: return "bx";
				case 4: return "sp";
				case 5: return "bp";
				case 6: return "si";
				case 7: return "di";
				case 8: return "r8w";
				case 9: return "r9w";
				case 10: return "r10w";
				case 11: return "r11w";
				case 12: return "r12w";
				case 13: return "r13w";
				case 14: return "r14w";
				case 15: return "r15w";
			}
			return "";
		}

		const char* x86_reg_str_d(int rdx)
		{
			switch (rdx) {
				case 0: return "eax";
				case 1: return "ecx";
				case 2: return "edx";
				case 3: return "ebx";
				case 4: return "esp";
				case 5: return "ebp";
				case 6: return "esi";
				case 7: return "edi";
				case 8: return "r8d";
				case 9: return "r9d";
				case 10: return "r10d";
				case 11: return "r11d";
				case 12: return "r12d";
				case 13: return "r13d";
				case 14: return "r14d";
				case 15: return "r15d";
			}
			return "";
		}

		const char* x86_reg_str_q(int rdx)
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

		static int x86_reg(int rd)
		{
			switch (rd) {
				case rv_ireg_zero: return 0;
				case rv_ireg_ra: return 2;  /* rdx */
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

		const char* rbp_reg_str_d(int reg)
		{
			static char buf[32];
			snprintf(buf, sizeof(buf), "dword ptr [rbp + %lu]", proc_offset(ireg) + reg * (P::xlen >> 3));
			return buf;
		}

		const X86Mem rbp_reg_d(int reg)
		{
			return x86::dword_ptr(x86::rbp, proc_offset(ireg) + reg * (P::xlen >> 3));
		}

		const char* rbp_reg_str_q(int reg)
		{
			static char buf[32];
			snprintf(buf, sizeof(buf), "qword ptr [rbp + %lu]", proc_offset(ireg) + reg * (P::xlen >> 3));
			return buf;
		}

		const X86Mem rbp_reg_q(int reg)
		{
			return x86::qword_ptr(x86::rbp, proc_offset(ireg) + reg * (P::xlen >> 3));
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
			as.mov(x86::rdx, rbp_reg_q(rv_ireg_ra));
			as.mov(x86::rbx, rbp_reg_q(rv_ireg_sp));
			as.mov(x86::rsi, rbp_reg_q(rv_ireg_t0));
			as.mov(x86::rdi, rbp_reg_q(rv_ireg_t1));
			as.mov(x86::r8,  rbp_reg_q(rv_ireg_a0));
			as.mov(x86::r9,  rbp_reg_q(rv_ireg_a1));
			as.mov(x86::r10, rbp_reg_q(rv_ireg_a2));
			as.mov(x86::r11, rbp_reg_q(rv_ireg_a3));
			as.mov(x86::r12, rbp_reg_q(rv_ireg_a4));
			as.mov(x86::r13, rbp_reg_q(rv_ireg_a5));
			as.mov(x86::r14, rbp_reg_q(rv_ireg_a6));
			as.mov(x86::r15, rbp_reg_q(rv_ireg_a7));
		}

		void emit_epilog()
		{
			as.mov(rbp_reg_q(rv_ireg_ra), x86::rdx);
			as.mov(rbp_reg_q(rv_ireg_sp), x86::rbx);
			as.mov(rbp_reg_q(rv_ireg_t0), x86::rsi);
			as.mov(rbp_reg_q(rv_ireg_t1), x86::rdi);
			as.mov(rbp_reg_q(rv_ireg_a0), x86::r8);
			as.mov(rbp_reg_q(rv_ireg_a1), x86::r9);
			as.mov(rbp_reg_q(rv_ireg_a2), x86::r10);
			as.mov(rbp_reg_q(rv_ireg_a3), x86::r11);
			as.mov(rbp_reg_q(rv_ireg_a4), x86::r12);
			as.mov(rbp_reg_q(rv_ireg_a5), x86::r13);
			as.mov(rbp_reg_q(rv_ireg_a6), x86::r14);
			as.mov(rbp_reg_q(rv_ireg_a7), x86::r15);
			as.pop(x86::r15);
			as.pop(x86::r14);
			as.pop(x86::r13);
			as.pop(x86::r12);
			as.pop(x86::rbx);
			as.pop(x86::rbp);
			as.ret();
		}

		void begin()
		{
			term = as.newLabel();
		}

		void end()
		{
			if (term_pc) {
				as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), Imm(term_pc));
				log_trace("\t# 0x%016llx", term_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), term_pc);
			}
			log_trace("\t\tterm:");
			as.bind(term);
		}

		void emit_sign_extend_32(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd);
			if (rdx > 0) {
				as.movsxd(x86::gpq(rdx), x86::gpd(rdx));
				log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), x86_reg_str_d(rdx));
			} else {
				as.movsxd(x86::rcx, rbp_reg_d(dec.rd));
				as.mov(rbp_reg_q(dec.rd), x86::rcx);
				log_trace("\t\tmovsxd rcx, %s", rbp_reg_str_d(dec.rd));
				log_trace("\t\tmov %s, rcx", rbp_reg_str_q(dec.rd));
			}
		}

		void emit_zero_rd(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd);
			if (rdx > 0) {
				as.xor_(x86::gpd(rdx), x86::gpd(rdx));
				log_trace("\t\txor %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rdx));
			} else {
				as.mov(rbp_reg_q(dec.rd), Imm(0));
				log_trace("\t\tmov %s, 0", rbp_reg_str_q(dec.rd));
			}
		}

		void emit_mv_rd_rs1(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::gpq(rdx), rbp_reg_q(dec.rs1));
					log_trace("\t\tmov %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(rbp_reg_q(dec.rd), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs1_32(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpd(rdx), x86::gpd(rs1x));
					log_trace("\t\tmov %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					log_trace("\t\tmov %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(rbp_reg_d(dec.rd), x86::gpd(rs1x));
					log_trace("\t\tmov %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
					log_trace("\t\tmov %s, eax", rbp_reg_str_d(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs1_sx_32(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.movsxd(x86::gpq(rdx), x86::gpd(rs1x));
					log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), x86_reg_str_d(rs1x));
				} else {
					as.movsxd(x86::gpq(rdx), rbp_reg_d(dec.rs1));
					log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_d(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.movsxd(x86::rax, x86::gpd(rs1x));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, %s", x86_reg_str_d(rs1x));
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				} else {
					as.movsxd(x86::rax, rbp_reg_d(dec.rs1));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, %s", rbp_reg_str_d(dec.rs1));
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs2(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (rdx > 0) {
				if (rs2x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::gpq(rdx), rbp_reg_q(dec.rs2));
					log_trace("\t\tmov %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.mov(rbp_reg_q(dec.rd), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs2));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
		}

		void emit_mv_cl_rs2(decode_type &dec)
		{
			int rs2x = x86_reg(dec.rs2);
			if (rs2x > 0) {
				as.mov(x86::ecx, x86::gpd(rs2x));
				log_trace("\t\tmov ecx, %s", x86_reg_str_d(dec.rs2));
			} else {
				as.mov(x86::ecx, rbp_reg_d(dec.rs2));
				log_trace("\t\tmov ecx, %s", rbp_reg_str_d(dec.rs2));
			}
		}

		bool emit_auipc(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.pc + dec.imm));
					log_trace("\t\tmov %s, %lld", x86_reg_str_q(rdx), dec.pc + dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(dec.pc + dec.imm));
					log_trace("\t\tmov %s, %lld", rbp_reg_str_q(dec.rd), dec.pc + dec.imm);
				}
			}
			return true;
		}

		bool emit_add(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tmov %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.mov(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs2));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.add(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs2));
						as.add(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
						log_trace("\t\tadd %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// add rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.add(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tadd %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
					} else {
						as.add(x86::gpq(rdx), rbp_reg_q(dec.rs1));
						log_trace("\t\tadd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.add(rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tadd %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.add(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tadd %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx > 0 && rs1x > 0 && rs2x > 0) {
				as.lea(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), x86::gpq(rs2x)));
				log_trace("\t\tlea %s, qword ptr [%s + %s]",
					x86_reg_str_q(rdx), x86_reg_str_q(rs1x), x86_reg_str_q(rs2x));
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// add rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.add(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tadd rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.add(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\tadd rax, %s", rbp_reg_str_q(dec.rs2));
				}
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.add(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_sub(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				if (dec.rd != dec.rs2) {
					// mov rd, rs2
					emit_mv_rd_rs2(dec);
				}
				// neg rd
				if (rdx > 0) {
					as.neg(x86::gpq(rdx));
					log_trace("\t\tneg %s", x86_reg_str_q(rdx));
				} else {
					as.neg(rbp_reg_q(dec.rd));
					log_trace("\t\tneg %s", rbp_reg_str_q(dec.rd));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sub rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.sub(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs2));
						as.sub(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
						log_trace("\t\tsub %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// sub rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.sub(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tsub %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
					} else {
						as.sub(x86::gpq(rdx), rbp_reg_q(dec.rs1));
						log_trace("\t\tsub %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.sub(rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tsub %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.sub(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tsub %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
				// neg rd
				if (rdx > 0) {
					as.neg(x86::gpq(rdx));
					log_trace("\t\tneg %s", x86_reg_str_q(rdx));
				} else {
					as.neg(rbp_reg_q(dec.rd));
					log_trace("\t\tneg %s", rbp_reg_str_q(dec.rd));
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// sub rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.sub(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tsub rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.sub(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\tsub rax, %s", rbp_reg_str_q(dec.rs2));
				}
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sub rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.sub(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_mul(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero || dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1 && rdx > 0) {
				// imul rd, rs2
				if (rs2x > 0) {
					as.imul(x86::gpq(rdx), x86::gpq(rs2x));
					log_trace("\t\timul %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
				} else {
					as.imul(x86::gpq(rdx), rbp_reg_q(dec.rs2));
					log_trace("\t\timul %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
				}
			}
			else if (dec.rd == dec.rs2 && rdx > 0) {
				// imul rd, rs1
				if (rs1x > 0) {
					as.imul(x86::gpq(rdx), x86::gpq(rs1x));
					log_trace("\t\timul %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
				} else {
					as.imul(x86::gpq(rdx), rbp_reg_q(dec.rs1));
					log_trace("\t\timul %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
				}
			}
			else if (rdx < 0) {
				// mov rax, rs1
				// imul rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.imul(x86::rax, x86::gpq(rs2x));
					log_trace("\t\timul rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.imul(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\timul rax, %s", rbp_reg_str_q(dec.rs2));
				}
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// imul rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.imul(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\timul %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.imul(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\timul %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				}
			}
			return true;
		}

		bool emit_mulh(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero || dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				// mov rax, rs1
				// imul rs2
				// mov rd, rdx

				if (rdx != 2 /* x86::rdx */) {
					as.mov(x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(rv_ireg_ra));
				}

				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				if (rs2x > 0) {
					as.imul(x86::gpq(rs2x));
					log_trace("\t\timul %s", x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					as.imul(x86::rcx);
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					log_trace("\t\timul rcx");
				}

				if (rdx > 0) {
					if (rdx != 2 /* x86::rdx */) {
						as.mov(x86::gpq(rdx), x86::rdx);
						log_trace("\t\tmov %s, rdx", x86_reg_str_q(rdx));
					}
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* x86::rdx */) {
					as.mov(x86::rdx, x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
					log_trace("\t\tmov rdx, %s", rbp_reg_str_q(rv_ireg_ra));
				}
			}
			return true;
		}

		bool emit_mulhu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero || dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				// mov rax, rs1
				// mul rs2
				// mov rd, rdx

				if (rdx != 2 /* x86::rdx */) {
					as.mov(x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(rv_ireg_ra));
				}

				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				if (rs2x > 0) {
					as.mul(x86::gpq(rs2x));
					log_trace("\t\tmul %s", x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					as.mul(x86::rcx);
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					log_trace("\t\tmul rcx");
				}

				if (rdx > 0) {
					if (rdx != 2 /* x86::rdx */) {
						as.mov(x86::gpq(rdx), x86::rdx);
						log_trace("\t\tmov %s, rdx", x86_reg_str_q(rdx));
					}
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* x86::rdx */) {
					as.mov(x86::rdx, x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
					log_trace("\t\tmov rdx, %s", rbp_reg_str_q(rv_ireg_ra));
				}
			}
			return true;
		}

		bool emit_mulhsu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero || dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* x86::rdx */ || (rs1x == 2 /* x86::rdx */ || rs2x == 2 /* x86::rdx */)) {
					as.mov(x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(rv_ireg_ra));
				}

				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				/* if rs1 is positive branch to umul */
				Label umul = as.newLabel();
				Label out = as.newLabel();
				as.test(x86::rax, x86::rax);
				as.jns(umul);
				as.neg(x86::rax);
				log_trace("\t\ttest rax, rax");
				log_trace("\t\tjns umul");
				log_trace("\t\tneg rax");

				/* multiply, negate and stash result in rcx */
				if (rs2x > 0) {
					as.mul(x86::gpq(rs2x));
					log_trace("\t\tmul %s", x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					as.mul(x86::rcx);
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					log_trace("\t\tmul rcx");
				}
				as.xor_(x86::rdx, Imm(-1));
				as.mov(x86::rcx, x86::rdx);
				log_trace("\t\txor rdx, -1");
				log_trace("\t\tmov rcx, rdx");

				/* if necessary restore rdx input operand */
				if (rs1x == 2 || rs2x == 2 /* x86::rdx */) {
					as.mov(x86::rdx, x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
					log_trace("\t\tmov rdx, %s", rbp_reg_str_q(rv_ireg_ra));
				}

				/* second multiply */
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.imul(x86::rax, x86::gpq(rs2x));
					log_trace("\t\timul rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.imul(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\timul rax, %s", rbp_reg_str_q(dec.rs2));
				}

				/* handle zero special case */
				as.test(x86::rax, x86::rax);
				as.setz(x86::dl);
				as.movzx(x86::edx, x86::dl);
				as.lea(x86::rdx, x86::qword_ptr(x86::rdx, x86::rcx));
				as.jmp(out);
				log_trace("\t\ttest rax, rax");
				log_trace("\t\tsetz dl");
				log_trace("\t\tmovzx edx, dl");
				log_trace("\t\tlea rdx, qword ptr [rdx + rcx]");
				log_trace("\t\tjmp out");

				/* unsigned multiply */
				log_trace("\t\tumul:");
				as.bind(umul);
				if (rs2x > 0) {
					as.mul(x86::gpq(rs2x));
					log_trace("\t\tmul %s", x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					as.mul(x86::rcx);
					log_trace("\t\tmov", rbp_reg_str_q(dec.rs2));
					log_trace("\t\tmul rcx");
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					if (rdx != 2 /* x86::rdx */) {
						as.mov(x86::gpq(rdx), x86::rdx);
						log_trace("\t\tmov %s, rdx", x86_reg_str_q(rdx));
					}
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* x86::rdx */) {
					as.mov(x86::rdx, x86::qword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
					log_trace("\t\tmov rdx, %s", rbp_reg_str_q(rv_ireg_ra));
				}
			}
			return true;
		}

		bool emit_div(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				Label out = as.newLabel();
				Label div1 = as.newLabel();
				Label div2 = as.newLabel();

				as.mov(x86::rax, std::numeric_limits<int64_t>::min());
				log_trace("\t\tmov rax, %lld", std::numeric_limits<int64_t>::min());
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), x86::rax);
					log_trace("\t\tcmp %s, rax", x86_reg_str_q(rs1x));
				} else {
					as.cmp(rbp_reg_q(dec.rs1), x86::rax);
					log_trace("\t\tcmp %s, rax", rbp_reg_str_q(dec.rs1));
				}
				as.jne(div1);
				log_trace("\t\tjne div1");

				if (rs2x > 0) {
					as.cmp(x86::gpq(rs2x), Imm(-1));
					log_trace("\t\tcmp %s, -1", x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_q(dec.rs2), Imm(-1));
					log_trace("\t\tcmp %s, -1", rbp_reg_str_q(dec.rs2));
				}
				as.je(out);
				log_trace("\t\tje out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_q(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_q(dec.rs2));
				}
				as.jne(div2);
				as.mov(x86::rax, Imm(-1));
				as.jmp(out);
				log_trace("\t\tjne div2");
				log_trace("\t\tmov rax, -1");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv2:");
				as.bind(div2);
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}
				as.cqo();
				log_trace("\t\tcqo");
				if (rs2x > 0) {
					as.idiv(x86::gpq(rs2x));
					log_trace("\t\tidiv %s", x86_reg_str_q(rs2x));
				} else {
					as.idiv(rbp_reg_q(dec.rs2));
					log_trace("\t\tidiv %s", rbp_reg_str_q(dec.rs2));
				}
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), x86::rax);
					log_trace("\t\tmov %s, rax", x86_reg_str_q(rdx));
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_rem(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}

				Label out = as.newLabel();
				Label div1 = as.newLabel();
				Label div2 = as.newLabel();

				as.mov(x86::rax, std::numeric_limits<int64_t>::min());
				log_trace("\t\tmov rax, %lld", std::numeric_limits<int64_t>::min());
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), x86::rax);
					log_trace("\t\tcmp %s, rax", x86_reg_str_q(rs1x));
				} else {
					as.cmp(rbp_reg_q(dec.rs1), x86::rax);
					log_trace("\t\tcmp %s, rax", rbp_reg_str_q(dec.rs1));
				}
				as.jne(div1);
				log_trace("\t\tjne div1");

				as.xor_(x86::edx, x86::edx);
				log_trace("\t\txor edx, edx");
				if (rs2x > 0) {
					as.cmp(x86::gpq(rs2x), Imm(-1));
					log_trace("\t\tcmp %s, -1", x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_q(dec.rs2), Imm(-1));
					log_trace("\t\tcmp %s, -1", rbp_reg_str_q(dec.rs2));
				}
				as.je(out);
				log_trace("\t\tje out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_q(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_q(dec.rs2));
				}
				as.jne(div2);
				as.mov(x86::rdx, x86::rax);
				as.jmp(out);
				log_trace("\t\tjne div2");
				log_trace("\t\tmov rdx, rax");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv2:");
				as.bind(div2);
				as.cqo();
				log_trace("\t\tcqo");
				if (rs2x > 0) {
					as.idiv(x86::gpq(rs2x));
					log_trace("\t\tidiv %s", x86_reg_str_q(rs2x));
				} else {
					as.idiv(rbp_reg_q(dec.rs2));
					log_trace("\t\tidiv %s", rbp_reg_str_q(dec.rs2));
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					if (rdx != 2 /* rdx */) {
						as.mov(x86::gpq(rdx), x86::rdx);
						log_trace("\t\tmov %s, rdx", x86_reg_str_q(rdx));
					}
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}
			}
			return true;
		}

		bool emit_divu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				Label out = as.newLabel();
				Label div1 = as.newLabel();

				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_q(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_q(dec.rs2));
				}
				as.jne(div1);
				as.mov(x86::rax, Imm(-1));
				as.jmp(out);
				log_trace("\t\tjne div1");
				log_trace("\t\tmov rax, -1");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}
				as.xor_(x86::rdx, x86::rdx);
				log_trace("\t\txor rdx, rdx");
				if (rs2x > 0) {
					as.div(x86::gpq(rs2x));
					log_trace("\t\tdiv %s", x86_reg_str_q(rs2x));
				} else {
					as.div(rbp_reg_q(dec.rs2));
					log_trace("\t\tdiv %s", rbp_reg_str_q(dec.rs2));
				}
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), x86::rax);
					log_trace("\t\tmov %s, rax", x86_reg_str_q(rdx));
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_remu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}

				Label out = as.newLabel();
				Label div1 = as.newLabel();

				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_q(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_q(dec.rs2));
				}
				as.jne(div1);
				as.mov(x86::rdx, x86::rax);
				as.jmp(out);
				log_trace("\t\tjne div1");
				log_trace("\t\tmov rdx, rax");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				as.xor_(x86::rdx, x86::rdx);
				log_trace("\t\txor rdx, rdx");
				if (rs2x > 0) {
					as.div(x86::gpq(rs2x));
					log_trace("\t\tdiv %s", x86_reg_str_q(rs2x));
				} else {
					as.div(rbp_reg_q(dec.rs2));
					log_trace("\t\tdiv %s", rbp_reg_str_q(dec.rs2));
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					if (rdx != 2 /* rdx */) {
						as.mov(x86::gpq(rdx), x86::rdx);
						log_trace("\t\tmov %s, rdx", x86_reg_str_q(rdx));
					}
				} else {
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}
			}
			return true;
		}

		bool emit_slt(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				// cmp rs1, rs2
				if (rs1x > 0 && rs2x > 0) {
					as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", x86_reg_str_q(rs1x), x86_reg_str_q(rs2x));
				}
				else {
					// mov rax, rs1
					if (dec.rs1 == rv_ireg_zero) {
						as.xor_(x86::eax, x86::eax);
						log_trace("\t\txor eax, eax");
					} else if (rs1x > 0) {
						as.mov(x86::rax, x86::gpq(rs1x));
						log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					}
					// cmp rax, rs2
					if (dec.rs2 == rv_ireg_zero) {
						as.cmp(x86::rax, Imm(0));
					} else if (rs2x > 0) {
						as.cmp(x86::rax, x86::gpq(rs2x));
						log_trace("\t\tcmp rax, %s", x86_reg_str_q(rs2x));
					} else {
						as.cmp(x86::rax, rbp_reg_q(dec.rs2));
						log_trace("\t\tcmp rax, %s", rbp_reg_str_q(dec.rs2));
					}
				}
				// setl rd
				as.setl(x86::al);
				log_trace("\t\tsetl al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", x86_reg_str_d(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_sltu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				// cmp rs1, rs2
				if (rs1x > 0 && rs2x > 0) {
					as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", x86_reg_str_q(rs1x), x86_reg_str_q(rs2x));
				}
				else {
					// mov rax, rs1
					if (dec.rs1 == rv_ireg_zero) {
						as.xor_(x86::eax, x86::eax);
						log_trace("\t\txor eax, eax");
					} else if (rs1x > 0) {
						as.mov(x86::rax, x86::gpq(rs1x));
						log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					}
					// cmp rax, rs2
					if (dec.rs2 == rv_ireg_zero) {
						as.cmp(x86::rax, Imm(0));
					} else if (rs2x > 0) {
						as.cmp(x86::rax, x86::gpq(rs2x));
						log_trace("\t\tcmp rax, %s", x86_reg_str_q(rs2x));
					} else {
						as.cmp(x86::rax, rbp_reg_q(dec.rs2));
						log_trace("\t\tcmp rax, %s", rbp_reg_str_q(dec.rs2));
					}
				}
				// setb rd
				as.setb(x86::al);
				log_trace("\t\tsetb al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", x86_reg_str_d(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_and(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero || dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// and rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.and_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.and_(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tand %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.and_(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs2));
						as.and_(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
						log_trace("\t\tand %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// and rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.and_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tand %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
					} else {
						as.and_(x86::gpq(rdx), rbp_reg_q(dec.rs1));
						log_trace("\t\tand %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.and_(rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tand %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.and_(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tand %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// and rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.and_(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tand rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.and_(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\tand rax, %s", rbp_reg_str_q(dec.rs2));
				}
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// and rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.and_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.and_(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tand %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.and_(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_or(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero && dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, rs2
				emit_mv_rd_rs2(dec);
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// or rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.or_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.or_(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tor %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.or_(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs2));
						as.or_(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
						log_trace("\t\tor %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// or rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.or_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tor %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
					} else {
						as.or_(x86::gpq(rdx), rbp_reg_q(dec.rs1));
						log_trace("\t\tor %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.or_(rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tor %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.or_(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tor %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// or rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.or_(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tor rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.or_(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\tor rax, %s", rbp_reg_str_q(dec.rs2));
				}
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// or rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.or_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.or_(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\tor %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.or_(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_xor(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero && dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, rs2
				emit_mv_rd_rs2(dec);
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// xor rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.xor_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.xor_(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\txor %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.xor_(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs2));
						as.xor_(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs2));
						log_trace("\t\txor %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// xor rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.xor_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\txor %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs1x));
					} else {
						as.xor_(x86::gpq(rdx), rbp_reg_q(dec.rs1));
						log_trace("\t\txor %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.xor_(rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\txor %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs1x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.xor_(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\txor %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// xor rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					as.xor_(x86::rax, x86::gpq(rs2x));
					log_trace("\t\txor rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.xor_(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\txor rax, %s", rbp_reg_str_q(dec.rs2));
				}
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// xor rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.xor_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", x86_reg_str_q(rdx), x86_reg_str_q(rs2x));
					} else {
						as.xor_(x86::gpq(rdx), rbp_reg_q(dec.rs2));
						log_trace("\t\txor %s, %s", x86_reg_str_q(rdx), rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.xor_(rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", rbp_reg_str_q(dec.rd), x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_sll(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// shl rd, rs2
				if (rdx > 0) {
					as.shl(x86::gpq(rdx), x86::cl);
					log_trace("\t\tshl %s, cl", x86_reg_str_q(rdx));
				} else {
					as.shl(rbp_reg_q(dec.rd), x86::cl);
					log_trace("\t\tshl %s, cl", rbp_reg_str_q(dec.rd));
				}
			}
			else if (rdx < 0) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov eax, rs1
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				// shl eax, cl
				if (rs2x > 0) {
					as.shl(x86::rax, x86::cl);
					log_trace("\t\tshl rax, cl");
				} else {
					as.shl(x86::rax, x86::cl);
					log_trace("\t\tshl rax, cl");
				}

				// mov rd, rax
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				as.mov(rbp_reg_q(dec.rd), x86::rax);
			}
			else {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshl %s, cl", x86_reg_str_q(rdx));
					} else {
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshl %s, cl", x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.shl(rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshl %s, cl", rbp_reg_str_q(dec.rd));
					} else {
						as.shl(rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshl %s, cl", rbp_reg_str_q(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_srl(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// shr rd, rs2
				if (rdx > 0) {
					as.shr(x86::gpq(rdx), x86::cl);
					log_trace("\t\tshr %s, cl", x86_reg_str_q(rdx));
				} else {
					as.shr(rbp_reg_q(dec.rd), x86::cl);
					log_trace("\t\tshr %s, cl", rbp_reg_str_q(dec.rd));
				}
			}
			else if (rdx < 0) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov eax, rs1
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				// shr eax, cl
				if (rs2x > 0) {
					as.shr(x86::rax, x86::cl);
					log_trace("\t\tshr rax, cl");
				} else {
					as.shr(x86::rax, x86::cl);
					log_trace("\t\tshr rax, cl");
				}

				// mov rd, rax
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				as.mov(rbp_reg_q(dec.rd), x86::rax);
			}
			else {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshr %s, cl", x86_reg_str_q(rdx));
					} else {
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshr %s, cl", x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.shr(rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshr %s, cl", rbp_reg_str_q(dec.rd));
					} else {
						as.shr(rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshr %s, cl", rbp_reg_str_q(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sra(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// sar rd, rs2
				if (rdx > 0) {
					as.sar(x86::gpq(rdx), x86::cl);
					log_trace("\t\tsar %s, cl", x86_reg_str_q(rdx));
				} else {
					as.sar(rbp_reg_q(dec.rd), x86::cl);
					log_trace("\t\tsar %s, cl", rbp_reg_str_q(dec.rd));
				}
			}
			else if (rdx < 0) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov eax, rs1
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str_q(rs1x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
				}

				// sar eax, cl
				if (rs2x > 0) {
					as.sar(x86::rax, x86::cl);
					log_trace("\t\tsar rax, cl");
				} else {
					as.sar(x86::rax, x86::cl);
					log_trace("\t\tsar rax, cl");
				}

				// mov rd, rax
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				as.mov(rbp_reg_q(dec.rd), x86::rax);
			}
			else {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tsar %s, cl", x86_reg_str_q(rdx));
					} else {
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tsar %s, cl", x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.sar(rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tsar %s, cl", rbp_reg_str_q(dec.rd));
					} else {
						as.sar(rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tsar %s, cl", rbp_reg_str_q(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_addw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// movsxd rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.movsxd(x86::gpq(rdx), x86::gpd(rs2x));
						log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), x86_reg_str_d(rs2x));
					} else {
						as.mov(x86::gpq(rdx), rbp_reg_d(dec.rs2));
						log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.movsxd(x86::rax, x86::gpd(rs2x));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmovsxd rax, %s", x86_reg_str_d(rs2x));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					} else {
						as.movsxd(x86::rax, rbp_reg_d(dec.rs2));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmovsxd rax, %s", rbp_reg_str_d(dec.rs2));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// movsxd rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movsxd(x86::gpq(rdx), x86::gpd(rs1x));
						log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), x86_reg_str_d(rs1x));
					} else {
						as.mov(x86::gpq(rdx), rbp_reg_d(dec.rs1));
						log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.movsxd(x86::rax, x86::gpd(rs1x));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmovsxd rax, %s", x86_reg_str_d(rs1x));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					} else {
						as.movsxd(x86::rax, rbp_reg_d(dec.rs1));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmovsxd rax, %s", rbp_reg_str_d(dec.rs1));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs1) {
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpd(rdx), x86::gpd(rs2x));
						log_trace("\t\tadd %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs2x));
					} else {
						as.add(x86::gpd(rdx), rbp_reg_d(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(rbp_reg_d(dec.rd), x86::gpd(rs2x));
						log_trace("\t\tadd %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.add(rbp_reg_d(dec.rd), x86::eax);
						log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs2));
						log_trace("\t\tadd %s, eax", rbp_reg_str_d(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rd == dec.rs2) {
				// add rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.add(x86::gpd(rdx), x86::gpd(rs1x));
						log_trace("\t\tadd %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs1x));
					} else {
						as.add(x86::gpd(rdx), rbp_reg_d(dec.rs1));
						log_trace("\t\tadd %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.add(rbp_reg_d(dec.rd), x86::gpd(rs1x));
						log_trace("\t\tadd %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.add(rbp_reg_d(dec.rd), x86::eax);
						log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
						log_trace("\t\tadd %s, eax", rbp_reg_str_d(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else if (rdx > 0 && rs1x > 0 && rs2x > 0) {
				as.lea(x86::gpd(rdx), x86::dword_ptr(x86::gpd(rs1x), x86::gpd(rs2x)));
				log_trace("\t\tlea %s, dword ptr [%s + %s]",
					x86_reg_str_d(rdx), x86_reg_str_d(rs1x), x86_reg_str_d(rs2x));
				emit_sign_extend_32(dec);
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov eax, rs1
				// add eax, rs2
				// movsxd rax, eax
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}
				if (rs2x > 0) {
					as.add(x86::eax, x86::gpd(rs2x));
					log_trace("\t\tadd eax, %s", x86_reg_str_d(rs2x));
				} else {
					as.add(x86::eax, rbp_reg_d(dec.rs2));
					log_trace("\t\tadd eax, %s", rbp_reg_str_d(dec.rs2));
				}
				as.movsxd(x86::rax, x86::eax);
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmovsxd rax, eax");
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpd(rdx), x86::gpd(rs2x));
						log_trace("\t\tadd %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs2x));
					} else {
						as.add(x86::gpd(rdx), rbp_reg_d(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(rbp_reg_d(dec.rd), x86::gpd(rs2x));
						log_trace("\t\tadd %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs2x));
					}
				}

				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_subw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				if (dec.rd != dec.rs2) {
					// mov rd, rs2
					emit_mv_rd_rs2(dec);
				}
				// neg rd
				if (rdx > 0) {
					as.neg(x86::gpd(rdx));
					log_trace("\t\tneg %s", x86_reg_str_d(rdx));
				} else {
					as.neg(rbp_reg_d(dec.rd));
					log_trace("\t\tneg %s", rbp_reg_str_d(dec.rd));
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// movsxd rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movsxd(x86::gpq(rdx), x86::gpd(rs1x));
						log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), x86_reg_str_d(rs1x));
					} else {
						as.mov(x86::gpq(rdx), rbp_reg_d(dec.rs1));
						log_trace("\t\tmovsxd %s, %s", x86_reg_str_q(rdx), rbp_reg_str_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.movsxd(x86::rax, x86::gpd(rs1x));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmovsxd rax, %s", x86_reg_str_d(rs1x));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					} else {
						as.movsxd(x86::rax, rbp_reg_d(dec.rs1));
						as.mov(rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmovsxd rax, %s", rbp_reg_str_d(dec.rs1));
						log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs1) {
				// sub rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpd(rdx), x86::gpd(rs2x));
						log_trace("\t\tsub %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs2x));
					} else {
						as.sub(x86::gpd(rdx), rbp_reg_d(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(rbp_reg_d(dec.rd), x86::gpd(rs2x));
						log_trace("\t\tsub %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.sub(rbp_reg_d(dec.rd), x86::eax);
						log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs2));
						log_trace("\t\tsub %s, eax", rbp_reg_str_d(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rd == dec.rs2) {
				// sub rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.sub(x86::gpd(rdx), x86::gpd(rs1x));
						log_trace("\t\tsub %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs1x));
					} else {
						as.sub(x86::gpd(rdx), rbp_reg_d(dec.rs1));
						log_trace("\t\tsub %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.sub(rbp_reg_d(dec.rd), x86::gpd(rs1x));
						log_trace("\t\tsub %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.sub(rbp_reg_d(dec.rd), x86::eax);
						log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
						log_trace("\t\tsub %s, eax", rbp_reg_str_d(dec.rd));
					}
				}
				// neg rd
				if (rdx > 0) {
					as.neg(x86::gpd(rdx));
					log_trace("\t\tneg %s", x86_reg_str_d(rdx));
				} else {
					as.neg(rbp_reg_d(dec.rd));
					log_trace("\t\tneg %s", rbp_reg_str_d(dec.rd));
				}
				emit_sign_extend_32(dec);
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov eax, rs1
				// sub eax, rs2
				// movsxd rax, eax
				// mov rd, rax
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}
				if (rs2x > 0) {
					as.sub(x86::eax, x86::gpd(rs2x));
					log_trace("\t\tsub eax, %s", x86_reg_str_d(rs2x));
				} else {
					as.sub(x86::eax, rbp_reg_d(dec.rs2));
					log_trace("\t\tsub eax, %s", rbp_reg_str_d(dec.rs2));
				}
				as.movsxd(x86::rax, x86::eax);
				as.mov(rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmovsxd rax, eax");
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sub rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpd(rdx), x86::gpd(rs2x));
						log_trace("\t\tsub %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs2x));
					} else {
						as.sub(x86::gpd(rdx), rbp_reg_d(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(rbp_reg_d(dec.rd), x86::gpd(rs2x));
						log_trace("\t\tsub %s, %s", rbp_reg_str_d(dec.rd), x86_reg_str_d(rs2x));
					}
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_mulw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero || dec.rs2 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1 && rdx > 0) {
				// imul rd, rs2
				if (rs2x > 0) {
					as.imul(x86::gpd(rdx), x86::gpd(rs2x));
					log_trace("\t\timul %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs2x));
				} else {
					as.imul(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					log_trace("\t\timul %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs2));
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rd == dec.rs2 && rdx > 0) {
				// imul rd, rs1
				if (rs1x > 0) {
					as.imul(x86::gpd(rdx), x86::gpd(rs1x));
					log_trace("\t\timul %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs1x));
				} else {
					as.imul(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					log_trace("\t\timul %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs1));
				}
				emit_sign_extend_32(dec);
			}
			else if (rdx < 0) {
				// mov eax, rs1
				// imul eax, rs2
				// mov rd, eax
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}
				if (rs2x > 0) {
					as.imul(x86::eax, x86::gpd(rs2x));
					log_trace("\t\timul eax, %s", x86_reg_str_d(rs2x));
				} else {
					as.imul(x86::eax, rbp_reg_d(dec.rs2));
					log_trace("\t\timul eax, %s", rbp_reg_str_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
				log_trace("\t\tmov %s, eax", rbp_reg_str_d(dec.rd));
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// imul rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.imul(x86::gpd(rdx), x86::gpd(rs2x));
						log_trace("\t\timul %s, %s", x86_reg_str_d(rdx), x86_reg_str_d(rs2x));
					} else {
						as.imul(x86::gpd(rdx), rbp_reg_d(dec.rs2));
						log_trace("\t\timul %s, %s", x86_reg_str_d(rdx), rbp_reg_str_d(dec.rs2));
					}
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_divw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				Label out = as.newLabel();
				Label div1 = as.newLabel();
				Label div2 = as.newLabel();

				as.mov(x86::eax, std::numeric_limits<int32_t>::min());
				log_trace("\t\tmov eax, %d", std::numeric_limits<int32_t>::min());
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), x86::eax);
					log_trace("\t\tcmp %s, eax", x86_reg_str_d(rs1x));
				} else {
					as.cmp(rbp_reg_d(dec.rs1), x86::eax);
					log_trace("\t\tcmp %s, eax", rbp_reg_str_d(dec.rs1));
				}
				as.jne(div1);
				log_trace("\t\tjne div1");

				if (rs2x > 0) {
					as.cmp(x86::gpd(rs2x), Imm(-1));
					log_trace("\t\tcmp %s, -1", x86_reg_str_d(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(-1));
					log_trace("\t\tcmp %s, -1", rbp_reg_str_d(dec.rs2));
				}
				as.je(out);
				log_trace("\t\tje out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_d(dec.rs2));
				}
				as.jne(div2);
				as.mov(x86::eax, Imm(-1));
				as.jmp(out);
				log_trace("\t\tjne div2");
				log_trace("\t\tmov eax, -1");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv2:");
				as.bind(div2);
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}
				as.cdq();
				log_trace("\t\tcdq");
				if (rs2x > 0) {
					as.idiv(x86::gpd(rs2x));
					log_trace("\t\tidiv %s", x86_reg_str_d(rs2x));
				} else {
					as.idiv(rbp_reg_d(dec.rs2));
					log_trace("\t\tidiv %s", rbp_reg_str_d(dec.rs2));
				}
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					as.movsxd(x86::gpq(rdx), x86::eax);
					log_trace("\t\tmovsxd %s, eax", x86_reg_str_q(rdx));
				} else {
					as.movsxd(x86::rax, x86::eax);
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, eax");
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_remw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}

				Label out = as.newLabel();
				Label div1 = as.newLabel();
				Label div2 = as.newLabel();

				as.mov(x86::eax, std::numeric_limits<int32_t>::min());
				log_trace("\t\tmov eax, %d", std::numeric_limits<int32_t>::min());
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), x86::eax);
					log_trace("\t\tcmp %s, eax", x86_reg_str_d(rs1x));
				} else {
					as.cmp(rbp_reg_d(dec.rs1), x86::eax);
					log_trace("\t\tcmp %s, eax", rbp_reg_str_d(dec.rs1));
				}
				as.jne(div1);
				log_trace("\t\tjne div1");

				as.xor_(x86::edx, x86::edx);
				log_trace("\t\txor edx, edx");
				if (rs2x > 0) {
					as.cmp(x86::gpd(rs2x), Imm(-1));
					log_trace("\t\tcmp %s, -1", x86_reg_str_d(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(-1));
					log_trace("\t\tcmp %s, -1", rbp_reg_str_d(dec.rs2));
				}
				as.je(out);
				log_trace("\t\tje out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_d(dec.rs2));
				}
				as.jne(div2);
				as.mov(x86::edx, x86::eax);
				as.jmp(out);
				log_trace("\t\tjne div2");
				log_trace("\t\tmov rdx, eax");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv2:");
				as.bind(div2);
				as.cdq();
				log_trace("\t\tcdq");
				if (rs2x > 0) {
					as.idiv(x86::gpd(rs2x));
					log_trace("\t\tidiv %s", x86_reg_str_d(rs2x));
				} else {
					as.idiv(rbp_reg_d(dec.rs2));
					log_trace("\t\tidiv %s", rbp_reg_str_d(dec.rs2));
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					as.movsxd(x86::gpq(rdx), x86::edx);
					log_trace("\t\tmovsxd %s, edx", x86_reg_str_q(rdx));
				} else {
					as.movsxd(x86::rdx, x86::edx);
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmovsxd rdx, edx");
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}
			}
			return true;
		}

		bool emit_divuw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				Label out = as.newLabel();
				Label div1 = as.newLabel();

				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_d(dec.rs2));
				}
				as.jne(div1);
				as.mov(x86::eax, Imm(-1));
				as.jmp(out);
				log_trace("\t\tjne div1");
				log_trace("\t\tmov eax, -1");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}
				as.xor_(x86::edx, x86::edx);
				log_trace("\t\txor edx, edx");
				if (rs2x > 0) {
					as.div(x86::gpd(rs2x));
					log_trace("\t\tdiv %s", x86_reg_str_d(rs2x));
				} else {
					as.div(rbp_reg_d(dec.rs2));
					log_trace("\t\tdiv %s", rbp_reg_str_d(dec.rs2));
				}
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					as.movsxd(x86::gpq(rdx), x86::eax);
					log_trace("\t\tmovsxd %s, eax", x86_reg_str_q(rdx));
				} else {
					as.movsxd(x86::rax, x86::eax);
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, eax");
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_remuw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				// mov rd, -1
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(-1));
					log_trace("\t\tmov %s, -1", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(-1));
					log_trace("\t\tmov %s, -1", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* rdx */) {
					as.mov(x86::rcx, x86::rdx);
					log_trace("\t\tmov rcx, rdx");
				}

				Label out = as.newLabel();
				Label div1 = as.newLabel();

				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				if (rs2x > 0) {
					as.test(x86::gpq(rs2x), x86::gpq(rs2x));
					log_trace("\t\ttest %s, %s", x86_reg_str_q(rs2x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_d(dec.rs2));
				}
				as.jne(div1);
				as.mov(x86::edx, x86::eax);
				as.jmp(out);
				log_trace("\t\tjne div1");
				log_trace("\t\tmov rdx, eax");
				log_trace("\t\tjmp out");

				log_trace("\t\tdiv1:");
				as.bind(div1);
				as.xor_(x86::edx, x86::edx);
				log_trace("\t\txor edx, edx");
				if (rs2x > 0) {
					as.div(x86::gpd(rs2x));
					log_trace("\t\tdiv %s", x86_reg_str_d(rs2x));
				} else {
					as.div(rbp_reg_d(dec.rs2));
					log_trace("\t\tdiv %s", rbp_reg_str_d(dec.rs2));
				}

				log_trace("\t\tout:");
				as.bind(out);
				if (rdx > 0) {
					as.movsxd(x86::gpq(rdx), x86::edx);
					log_trace("\t\tmovsxd %s, edx", x86_reg_str_q(rdx));
				} else {
					as.movsxd(x86::rdx, x86::edx);
					as.mov(rbp_reg_q(dec.rd), x86::rdx);
					log_trace("\t\tmovsxd rdx, edx");
					log_trace("\t\tmov %s, rdx", rbp_reg_str_q(dec.rd));
				}

				if (rdx != 2 /* rdx */) {
					as.mov(x86::rdx, x86::rcx);
					log_trace("\t\tmov rdx, rcx");
				}
			}
			return true;
		}

		bool emit_sllw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1_sx_32(dec);
			}
			else if (dec.rd == dec.rs1) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// shl rd, rs2
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), x86::cl);
					log_trace("\t\tshl %s, cl", x86_reg_str_q(rdx));
				} else {
					as.shl(rbp_reg_d(dec.rd), x86::cl);
					log_trace("\t\tshl %s, cl", rbp_reg_str_d(dec.rd));
				}
				emit_sign_extend_32(dec);
			}
			else if (rdx < 0) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov eax, rs1
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				// shl eax, cl
				if (rs2x > 0) {
					as.shl(x86::eax, x86::cl);
					log_trace("\t\tshl eax, cl");
				} else {
					as.shl(x86::eax, x86::cl);
					log_trace("\t\tshl eax, cl");
				}

				// movsxd rax, eax
				// mov rd, rax
				log_trace("\t\tmovsxd rax, eax");
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				as.movsxd(x86::rax, x86::eax);
				as.mov(rbp_reg_q(dec.rd), x86::rax);
			}
			else {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shl rs, cl
				if (rs2x > 0) {
					as.shl(x86::gpd(rdx), x86::cl);
					log_trace("\t\tshl %s, cl", x86_reg_str_d(rdx));
				} else {
					as.shl(x86::gpd(rdx), x86::cl);
					log_trace("\t\tshl %s, cl", x86_reg_str_d(rdx));
				}

				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_srlw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1_sx_32(dec);
			}
			else if (dec.rd == dec.rs1) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// shr rd, rs2
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), x86::cl);
					log_trace("\t\tshr %s, cl", x86_reg_str_d(rdx));
				} else {
					as.shr(rbp_reg_d(dec.rd), x86::cl);
					log_trace("\t\tshr %s, cl", rbp_reg_str_d(dec.rd));
				}
				emit_sign_extend_32(dec);
			}
			else if (rdx < 0) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov eax, rs1
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				// shr eax, cl
				if (rs2x > 0) {
					as.shr(x86::eax, x86::cl);
					log_trace("\t\tshr eax, cl");
				} else {
					as.shr(x86::eax, x86::cl);
					log_trace("\t\tshr eax, cl");
				}

				// movsxd rax, eax
				// mov rd, rax
				log_trace("\t\tmovsxd rax, eax");
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				as.movsxd(x86::rax, x86::eax);
				as.mov(rbp_reg_q(dec.rd), x86::rax);
			}
			else {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shr rs, cl
				if (rs2x > 0) {
					as.shr(x86::gpd(rdx), x86::cl);
					log_trace("\t\tshr %s, cl", x86_reg_str_d(rdx));
				} else {
					as.shr(x86::gpd(rdx), x86::cl);
					log_trace("\t\tshr %s, cl", x86_reg_str_d(rdx));
				}

				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_sraw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1_sx_32(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, rs2
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), x86::cl);
					log_trace("\t\tsar %s, cl", x86_reg_str_d(rdx));
				} else {
					as.sar(rbp_reg_d(dec.rd), x86::cl);
					log_trace("\t\tsar %s, cl", rbp_reg_str_d(dec.rd));
				}
				emit_sign_extend_32(dec);
			}
			else if (rdx < 0) {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov eax, rs1
				if (rs1x > 0) {
					as.mov(x86::eax, x86::gpd(rs1x));
					log_trace("\t\tmov eax, %s", x86_reg_str_d(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					log_trace("\t\tmov eax, %s", rbp_reg_str_d(dec.rs1));
				}

				// sar eax, cl
				if (rs2x > 0) {
					as.sar(x86::eax, x86::cl);
					log_trace("\t\tsar eax, cl");
				} else {
					as.sar(x86::eax, x86::cl);
					log_trace("\t\tsar eax, cl");
				}

				// movsxd rax, eax
				// mov rd, rax
				log_trace("\t\tmovsxd rax, eax");
				log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				as.movsxd(x86::rax, x86::eax);
				as.mov(rbp_reg_q(dec.rd), x86::rax);
			}
			else {
				// mov cl, rs2
				emit_mv_cl_rs2(dec);

				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// sar rs, rs2
				if (rs2x > 0) {
					as.sar(x86::gpd(rdx), x86::cl);
					log_trace("\t\tsar %s, cl", x86_reg_str_d(rdx));
				} else {
					as.sar(x86::gpd(rdx), x86::cl);
					log_trace("\t\tsar %s, cl", x86_reg_str_d(rdx));
				}

				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_addi(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.add(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.imm == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (rdx > 0 && rs1x > 0) {
				// lea rd, rs1, rs2
				as.lea(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), int32_t(dec.imm)));
				log_trace("\t\tlea %s, qword ptr [%s + %d]",
					x86_reg_str_q(rdx), x86_reg_str_q(rs1x), dec.imm);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.add(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_slti(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				bool result = 0LL < dec.imm;
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(result));
					log_trace("\t\tmov %s, %d", x86_reg_str_q(rdx), result);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(result));
					log_trace("\t\tmov %s, %d", rbp_reg_str_q(dec.rd), result);
				}
			}
			else {
				// cmp rs1, imm
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", x86_reg_str_q(rs1x), dec.imm);
				} else {
					as.cmp(rbp_reg_q(dec.rs1), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", rbp_reg_str_q(dec.rs1), dec.imm);
				}
				// setl rd
				as.setl(x86::al);
				log_trace("\t\tsetl al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", x86_reg_str_d(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_sltiu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				bool result = 0ULL < u64(dec.imm);
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(result));
					log_trace("\t\tmov %s, %d", x86_reg_str_q(rdx), result);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(result));
					log_trace("\t\tmov %s, %d", rbp_reg_str_q(dec.rd), result);
				}
			}
			else {
				// cmp rs1, imm
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", x86_reg_str_q(rs1x), dec.imm);
				} else {
					as.cmp(rbp_reg_q(dec.rs1), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", rbp_reg_str_q(dec.rs1), dec.imm);
				}
				// setb rd
				as.setb(x86::al);
				log_trace("\t\tsetb al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", x86_reg_str_d(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_andi(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// and rd, imm
				if (rdx > 0) {
					as.and_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tand %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.and_(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tand %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// and rd, imm
				if (rdx > 0) {
					as.and_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tand %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.and_(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tand %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_ori(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// or rd, imm
				if (rdx > 0) {
					as.or_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tor %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.or_(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tor %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// or rd, imm
				if (rdx > 0) {
					as.or_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tor %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.or_(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tor %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_xori(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// xor rd, imm
				if (rdx > 0) {
					as.xor_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\txor %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.xor_(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\txor %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// xor rd, imm
				if (rdx > 0) {
					as.xor_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\txor %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.xor_(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\txor %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_slli(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.shl(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.shl(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_srli(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.shr(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.shr(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_srai(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.sar(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.sar(rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_addiw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.add(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_sx_32(dec);

				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.add(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_slliw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.shl(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.shl(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_srliw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.shr(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.shr(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_sraiw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.sar(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str_d(rdx), dec.imm);
				} else {
					as.sar(rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		void emit_cmp(decode_type &dec)
		{
			int rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rs1 == rv_ireg_zero) {
				as.xor_(x86::eax, x86::eax);
				log_trace("\t\txor eax, eax");
				if (rs2x > 0) {
					as.cmp(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tcmp rax, %s", x86_reg_str_q(rs2x));
				} else {
					as.cmp(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\tcmp rax, %s", rbp_reg_str_q(dec.rs2));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), Imm(0));
					log_trace("\t\tcmp %s, 0", x86_reg_str_q(rs1x));
				} else {
					as.cmp(rbp_reg_q(dec.rs1), Imm(0));
					log_trace("\t\tcmp %s, 0", rbp_reg_str_q(dec.rs1));
				}
			}
			else if (rs1x > 0) {
				if (rs2x > 0) {
					as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", x86_reg_str_q(rs1x), x86_reg_str_q(rs2x));
				} else {
					as.cmp(x86::gpq(rs1x), rbp_reg_q(dec.rs2));
					log_trace("\t\tcmp %s, %s", x86_reg_str_q(rs1x), rbp_reg_str_q(dec.rs2));
				}
			}
			else {
				if (rs2x > 0) {
					as.cmp(rbp_reg_q(dec.rs1), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", rbp_reg_str_q(dec.rs1), x86_reg_str_q(rs2x));
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					as.cmp(x86::rax, rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					log_trace("\t\tcmp rax, %s", rbp_reg_str_q(dec.rs1));
				}
			}
		}

		template <typename T>
		bool emit_branch(decode_type &dec, bool cond,
			Error(T::*bf)(const Label&), const char* bfname,
			Error(T::*ibf)(const Label&), const char* ibfname)
		{
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				(as.*bf)(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\t%s 0x%016llx", bfname, branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				(as.*bf)(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)cont_pc);
				as.jmp(term);
				log_trace("\t\t%s 0x%016llx", bfname, branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				(as.*ibf)(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\t%s 0x%016llx", ibfname, cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), branch_pc);
				log_trace("\t\tjmp term");
			} else if (cond) {
				Label l = as.newLabel();
				(as.*bf)(l);
				as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				log_trace("\t\t%s 1f", bfname);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				term_pc = branch_pc;
			} else {
				Label l = as.newLabel();
				(as.*ibf)(l);
				as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)branch_pc);
				as.jmp(term);
				as.bind(l);
				log_trace("\t\t%s 1f", ibfname);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), branch_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				term_pc = cont_pc;
			}
			return true;
		}

		bool emit_bne(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, &X86Assembler::jne, "jne", &X86Assembler::je, "je");
		}

		bool emit_beq(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, &X86Assembler::je, "je", &X86Assembler::jne, "jne");
		}

		bool emit_blt(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, &X86Assembler::jl, "jl", &X86Assembler::jge, "jge");
		}

		bool emit_bge(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, &X86Assembler::jge, "jge", &X86Assembler::jl, "jl");
		}

		bool emit_bltu(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			return emit_branch(dec, cond, &X86Assembler::jb, "jb", &X86Assembler::jae, "jae");
		}

		bool emit_bgeu(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val;
			return emit_branch(dec, cond, &X86Assembler::jae, "jae", &X86Assembler::jb, "jb");
		}

		bool emit_ld(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.mov(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, qword ptr [%s + %lld]", x86_reg_str_q(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::gpq(rdx), x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, qword ptr [rax + %lld]", x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.mov(x86::rax, x86::qword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov rax, qword ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::rax, x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov rax, qword ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movsxd(x86::gpq(rdx), x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsxd %s, dword ptr [%s + %lld]", x86_reg_str_q(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movsxd(x86::gpq(rdx), x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsxd %s, dword ptr [rax + %lld]", x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movsxd(x86::rax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsxd rax, dword ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movsxd(x86::rax, x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsxd rax, dword ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lwu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.mov(x86::gpd(rdx), x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, dword ptr [%s + %lld]", x86_reg_str_d(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::gpd(rdx), x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, dword ptr [rax + %lld]", x86_reg_str_d(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.mov(x86::eax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, dword ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::eax, x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov eax, dword ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lh(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movsx(x86::gpq(rdx), x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx %s, word ptr [%s + %lld]", x86_reg_str_q(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movsx(x86::gpq(rdx), x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx %s, word ptr [rax + %lld]", x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movsx(x86::rax, x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx rax, word ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movsx(x86::rax, x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx rax, word ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lhu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movzx(x86::gpd(rdx), x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, word ptr [%s + %lld]", x86_reg_str_d(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movzx(x86::gpd(rdx), x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, word ptr [rax + %lld]", x86_reg_str_d(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movzx(x86::eax, x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, word ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movzx(x86::eax, x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov eax, word ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lb(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movsx(x86::gpq(rdx), x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx %s, byte ptr [%s + %lld]", x86_reg_str_q(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movsx(x86::gpq(rdx), x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx %s, byte ptr [rax + %lld]", x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movsx(x86::rax, x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx rax, byte ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movsx(x86::rax, x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx rax, byte ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lbu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, byte ptr [%s + %lld]", x86_reg_str_d(rdx), x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, byte ptr [rax + %lld]", x86_reg_str_d(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movzx(x86::eax, x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, byte ptr [%s + %lld]", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.movzx(x86::eax, x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov eax, byte ptr [rax + %lld]", dec.imm);
					}
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_sd(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rs2x = x86_reg(dec.rs2), rs1x = x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov qword ptr [%s + %lld], 0", x86_reg_str_q(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					as.mov(x86::qword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov qword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov qword ptr [%s + %lld], %s", x86_reg_str_q(rs1x), dec.imm, x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov qword ptr [rax + %lld], %s", dec.imm, x86_reg_str_q(rs2x));
					}
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						log_trace("\t\tmov qword ptr [%s + %lld], rcx", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov qword ptr [rax + %lld], rcx", dec.imm);
					}
				}
			}
			return true;
		}

		bool emit_sw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rs2x = x86_reg(dec.rs2), rs1x = x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov dword ptr [%s + %lld], 0", x86_reg_str_q(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					as.mov(x86::dword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov dword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), x86::gpd(rs2x));
						log_trace("\t\tmov dword ptr [%s + %lld], %s", x86_reg_str_d(rs1x), dec.imm, x86_reg_str_q(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::dword_ptr(x86::rax, dec.imm), x86::gpd(rs2x));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov dword ptr [rax + %lld], %s", dec.imm, x86_reg_str_d(rs2x));
					}
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), x86::ecx);
						log_trace("\t\tmov dword ptr [%s + %lld], ecx", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::dword_ptr(x86::rax, dec.imm), x86::ecx);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov dword ptr [rax + %lld], ecx", dec.imm);
					}
				}
			}
			return true;
		}

		bool emit_sh(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rs2x = x86_reg(dec.rs2), rs1x = x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov word ptr [%s + %lld], 0", x86_reg_str_q(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					as.mov(x86::word_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov word ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), x86::gpw(rs2x));
						log_trace("\t\tmov word ptr [%s + %lld], %s", x86_reg_str_q(rs1x), dec.imm, x86_reg_str_w(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::word_ptr(x86::rax, dec.imm), x86::gpw(rs2x));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov word ptr [rax + %lld], %s", dec.imm, x86_reg_str_w(rs2x));
					}
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), x86::cx);
						log_trace("\t\tmov word ptr [%s + %lld], cx", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::word_ptr(x86::rax, dec.imm), x86::cx);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov word ptr [rax + %lld], cx", dec.imm);
					}
				}
			}
			return true;
		}

		bool emit_sb(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rs2x = x86_reg(dec.rs2), rs1x = x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov byte ptr [%s + %lld], 0", x86_reg_str_q(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, rbp_reg_q(dec.rs1));
					as.mov(x86::byte_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov byte ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), x86::gpb_lo(rs2x));
						log_trace("\t\tmov byte ptr [%s + %lld], %s", x86_reg_str_q(rs1x), dec.imm, x86_reg_str_b(rs2x));
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::byte_ptr(x86::rax, dec.imm), x86::gpb_lo(rs2x));
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov byte ptr [rax + %lld], %s", dec.imm, x86_reg_str_b(rs2x));
					}
				} else {
					as.mov(x86::rcx, rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), x86::cl);
						log_trace("\t\tmov byte ptr [%s + %lld], cl", x86_reg_str_q(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, rbp_reg_q(dec.rs1));
						as.mov(x86::byte_ptr(x86::rax, dec.imm), x86::cl);
						log_trace("\t\tmov rax, %s", rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov byte ptr [rax + %lld], cl", dec.imm);
					}
				}
			}
			return true;
		}

		bool emit_lui(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", x86_reg_str_q(rdx), dec.imm);
				} else {
					as.mov(rbp_reg_q(dec.rd), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_jal(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + dec.imm;
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				addr_t link_addr = dec.pc + inst_length(dec.inst);
				callstack.push_back(link_addr);
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(link_addr));
					log_trace("\t\tmov %s, 0x%llx", x86_reg_str_q(rdx), link_addr);
				} else {
					as.mov(x86::rax, Imm(link_addr));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, 0x%llx",  link_addr);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_jalr(decode_type &dec)
		{
			if (dec.rd == rv_ireg_zero && dec.rs1 == rv_ireg_ra && callstack.size() > 0) {
				term_pc = dec.pc + dec.imm;
				addr_t link_addr = callstack.back();
				callstack.pop_back();
				as.cmp(x86::gpq(x86_reg(rv_ireg_ra)), Imm(link_addr));
				Label l = as.newLabel();
				as.je(l);
				as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), Imm(dec.pc));
				as.jmp(term);
				as.bind(l);
				log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
				log_trace("\t\tcmp %s, 0x%llx", x86_reg_str_q(rv_ireg_ra), link_addr);
				log_trace("\t\tje 1f");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), dec.pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				return true;
			}
			return false;
		}

		bool emit_la(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tla\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				int64_t addr = dec.pc + dec.imm;
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(addr));
					log_trace("\t\tmov %s, %lld", x86_reg_str_q(rdx), dec.pc + dec.imm);
				} else {
					as.mov(x86::rax, Imm(addr));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, 0x%llx", addr);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_call(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tcall\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + dec.imm;
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				addr_t link_addr = dec.pc + inst_length(dec.inst);
				callstack.push_back(link_addr);
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(link_addr));
					log_trace("\t\tmov %s, 0x%llx", x86_reg_str_q(rdx), link_addr);
				} else {
					as.mov(x86::rax, Imm(link_addr));
					as.mov(rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, 0x%llx", link_addr);
					log_trace("\t\tmov %s, rax", rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit(decode_type &dec)
		{
			auto li = labels.find(dec.pc);
			if (li != labels.end()) {
				return false; /* trace complete */
			}
			Label l = as.newLabel();
			labels[dec.pc] = l;
			as.bind(l);
			switch(dec.op) {
				case rv_op_auipc: return emit_auipc(dec);
				case rv_op_add: return emit_add(dec);
				case rv_op_sub: return emit_sub(dec);
				case rv_op_mul: return emit_mul(dec);
				case rv_op_mulh: return emit_mulh(dec);
				case rv_op_mulhu: return emit_mulhu(dec);
				case rv_op_mulhsu: return emit_mulhsu(dec);
				case rv_op_div: return emit_div(dec);
				case rv_op_rem: return emit_rem(dec);
				case rv_op_divu: return emit_divu(dec);
				case rv_op_remu: return emit_remu(dec);
				case rv_op_slt: return emit_slt(dec);
				case rv_op_sltu: return emit_sltu(dec);
				case rv_op_and: return emit_and(dec);
				case rv_op_or: return emit_or(dec);
				case rv_op_xor: return emit_xor(dec);
				case rv_op_sll: return emit_sll(dec);
				case rv_op_srl: return emit_srl(dec);
				case rv_op_sra: return emit_sra(dec);
				case rv_op_addw: return emit_addw(dec);
				case rv_op_subw: return emit_subw(dec);
				case rv_op_mulw: return emit_mulw(dec);
				case rv_op_divw: return emit_divw(dec);
				case rv_op_remw: return emit_remw(dec);
				case rv_op_divuw: return emit_divuw(dec);
				case rv_op_remuw: return emit_remuw(dec);
				case rv_op_sllw: return emit_sllw(dec);
				case rv_op_srlw: return emit_srlw(dec);
				case rv_op_sraw: return emit_sraw(dec);
				case rv_op_addi: return emit_addi(dec);
				case rv_op_slti: return emit_slti(dec);
				case rv_op_sltiu: return emit_sltiu(dec);
				case rv_op_andi: return emit_andi(dec);
				case rv_op_ori: return emit_ori(dec);
				case rv_op_xori: return emit_xori(dec);
				case rv_op_slli: return emit_slli(dec);
				case rv_op_srli: return emit_srli(dec);
				case rv_op_srai: return emit_srai(dec);
				case rv_op_addiw: return emit_addiw(dec);
				case rv_op_slliw: return emit_slliw(dec);
				case rv_op_srliw: return emit_srliw(dec);
				case rv_op_sraiw: return emit_sraiw(dec);
				case rv_op_bne: return emit_bne(dec);
				case rv_op_beq: return emit_beq(dec);
				case rv_op_blt: return emit_blt(dec);
				case rv_op_bge: return emit_bge(dec);
				case rv_op_bltu: return emit_bltu(dec);
				case rv_op_bgeu: return emit_bgeu(dec);
				case rv_op_ld: return emit_ld(dec);
				case rv_op_lw: return emit_lw(dec);
				case rv_op_lwu: return emit_lwu(dec);
				case rv_op_lh: return emit_lh(dec);
				case rv_op_lhu: return emit_lhu(dec);
				case rv_op_lb: return emit_lb(dec);
				case rv_op_lbu: return emit_lbu(dec);
				case rv_op_sd: return emit_sd(dec);
				case rv_op_sw: return emit_sw(dec);
				case rv_op_sh: return emit_sh(dec);
				case rv_op_sb: return emit_sb(dec);
				case rv_op_lui: return emit_lui(dec);
				case rv_op_jal: return emit_jal(dec);
				case rv_op_jalr: return emit_jalr(dec);
				case jit_op_la: return emit_la(dec);
				case jit_op_call: return emit_call(dec);
			}
			return false;
		}
	};

}

#endif
