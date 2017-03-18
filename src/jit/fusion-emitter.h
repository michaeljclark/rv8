//
//  fusion-emitter.h
//

#ifndef rv_fusion_emitter_h
#define rv_fusion_emitter_h

namespace riscv {

	using namespace asmjit;

	template <typename P>
	struct fusion_emitter : public ErrorHandler
	{
		typedef typename P::decode_type decode_type;

		P &proc;
		X86Assembler as;
		Label term;
		std::map<addr_t,Label> labels;
		addr_t term_pc;

		fusion_emitter(P &proc, CodeHolder &code)
			: proc(proc), as(&code), term_pc(0)
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

		const X86Mem frame_reg(int reg)
		{
			return x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, ireg) + reg * (P::xlen >> 3));
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
			as.mov(x86::rdx, frame_reg(rv_ireg_ra));
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
			if (term_pc) {
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), Imm(term_pc));
				log_trace("\t# 0x%016llx", term_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), term_pc);
			}
			log_trace("\t\tterm:");
			as.bind(term);
			as.mov(frame_reg(rv_ireg_ra), x86::rdx);
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

		bool emit_auipc(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tli\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", x86_reg_str(rdx), dec.pc + dec.imm);
				} else {
					as.mov(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", frame_reg_str(dec.rd), dec.pc + dec.imm);
				}
			}
			return true;
		}

		void emit_mv_rd_rs1(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg(dec.rs1));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(frame_reg(dec.rd), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs1x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(frame_reg(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
					log_trace("\t\tmov %s, rax", frame_reg_str(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs2(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (rdx > 0) {
				if (rs2x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg(dec.rs2));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.mov(frame_reg(dec.rd), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs2));
					as.mov(frame_reg(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs2));
					log_trace("\t\tmov %s, rax", frame_reg_str(dec.rd));
				}
			}
		}

		bool emit_add(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.mov(x86::gpq(rdx), frame_reg(dec.rs2));
						log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.mov(frame_reg(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg(dec.rs2));
						as.mov(frame_reg(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tmov %s, rax", frame_reg_str(dec.rd));
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
						log_trace("\t\tadd %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.add(x86::gpq(rdx), frame_reg(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(frame_reg(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg(dec.rs2));
						as.add(frame_reg(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tadd %s, rax", frame_reg_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.add(x86::gpq(rdx), frame_reg(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(frame_reg(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg(dec.rs2));
						as.add(frame_reg(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tadd %s, rax", frame_reg_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sub(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
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
					log_trace("\t\tneg %s", x86_reg_str(rdx));
				} else {
					as.neg(frame_reg(dec.rd));
					log_trace("\t\tneg %s", frame_reg_str(dec.rd));
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
						log_trace("\t\tsub %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.sub(x86::gpq(rdx), frame_reg(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(frame_reg(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg(dec.rs2));
						as.sub(frame_reg(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tsub %s, rax", frame_reg_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sub rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.sub(x86::gpq(rdx), frame_reg(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str(rdx), frame_reg_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(frame_reg(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", frame_reg_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg(dec.rs2));
						as.sub(frame_reg(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tsub %s, rax", frame_reg_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sll(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, 0
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(0));
					log_trace("\t\tmov %s, 0", x86_reg_str(rdx));
				} else {
					as.mov(frame_reg(dec.rd), Imm(0));
					log_trace("\t\tmov %s, 0", frame_reg_str(dec.rd));
				}
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shl(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_str(dec.rd));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shl(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shl(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_str(dec.rd));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shl(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_srl(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, 0
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(0));
					log_trace("\t\tmov %s, 0", x86_reg_str(rdx));
				} else {
					as.mov(frame_reg(dec.rd), Imm(0));
					log_trace("\t\tmov %s, 0", frame_reg_str(dec.rd));
				}
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shr(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_str(dec.rd));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shr(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.shr(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_str(dec.rd));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.shr(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sra(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, 0
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(0));
					log_trace("\t\tmov %s, 0", x86_reg_str(rdx));
				} else {
					as.mov(frame_reg(dec.rd), Imm(0));
					log_trace("\t\tmov %s, 0", frame_reg_str(dec.rd));
				}
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.sar(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_str(dec.rd));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.sar(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov rcx, cl", frame_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::rcx, x86::gpq(rs2x));
						as.sar(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_str(dec.rd));
					} else {
						as.mov(x86::rcx, frame_reg(dec.rs2));
						as.sar(frame_reg(dec.rd), x86::cl);
						log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_addi(decode_type &dec)
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
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.add(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.add(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", frame_reg_str(dec.rd), dec.imm);
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
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shl(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shl(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", frame_reg_str(dec.rd), dec.imm);
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
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shr(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shr(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", frame_reg_str(dec.rd), dec.imm);
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
				// mov rd, imm
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.sar(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.sar(frame_reg(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", frame_reg_str(dec.rd), dec.imm);
				}
			}
			return true;
		}

		void emit_cmp(decode_type &dec)
		{
			int rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rs1 == rv_ireg_zero) {
				if (rs2x > 0) {
					as.cmp(x86::gpq(rs2x), 0);
					log_trace("\t\tcmp %s, 0", x86_reg_str(rs2x));
				} else {
					as.cmp(frame_reg(dec.rs2), 0);
					log_trace("\t\tcmp %s, 0", frame_reg_str(dec.rs2));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), 0);
					log_trace("\t\tcmp %s, 0", x86_reg_str(rs1x));
				} else {
					as.cmp(frame_reg(dec.rs1), 0);
					log_trace("\t\tcmp %s, 0", frame_reg_str(dec.rs1));
				}
			}
			else if (rs1x > 0) {
				if (rs2x > 0) {
					as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", x86_reg_str(rs1x), x86_reg_str(rs2x));
				} else {
					as.cmp(x86::gpq(rs1x), frame_reg(dec.rs2));
					log_trace("\t\tcmp %s, %s", x86_reg_str(rs1x), frame_reg_str(dec.rs2));
				}
			}
			else {
				if (rs2x > 0) {
					as.cmp(frame_reg(dec.rs1), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", frame_reg_str(dec.rs1), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.cmp(x86::rax, frame_reg(dec.rs2));
					log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
					log_trace("\t\tcmp rax, %s", frame_reg_str(dec.rs1));
				}
			}
		}

		bool emit_bne(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val;
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.jne(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\tjne 0x%016llx", branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				as.jne(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), Imm(cont_pc));
				as.jmp(term);
				log_trace("\t\tjne 0x%016llx", branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				as.je(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), Imm(branch_pc));
				as.jmp(term);
				log_trace("\t\tje 0x%016llx", cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			} else {
				Label l = as.newLabel();
				as.jne(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), Imm(cont_pc));
				as.jmp(term);
				as.bind(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), Imm(branch_pc));
				as.jmp(term);
				log_trace("\t\tjne 1f");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			}

			return true;
		}

		bool emit_beq(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val;
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.je(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\tje 0x%016llx", branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				as.je(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				log_trace("\t\tje 0x%016llx", branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				as.jne(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjne 0x%016llx", cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			} else {
				Label l = as.newLabel();
				as.je(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tje 1f");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			}

			return true;
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
						log_trace("\t\tmov %s, qword ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg(dec.rs1));
						as.mov(x86::gpq(rdx), x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
						log_trace("\t\tmov %s, qword ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.mov(x86::rax, x86::qword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov rax, qword ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg(dec.rs1));
						as.mov(x86::rax, x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
						log_trace("\t\tmov rax, qword ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_str(dec.rd));
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
					log_trace("\t\tmov qword ptr [%s + %lld], 0", x86_reg_str(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, frame_reg(dec.rs1));
					as.mov(x86::qword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
					log_trace("\t\tmov qword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov qword ptr [%s + %lld], %s", x86_reg_str(rs1x), dec.imm, x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg(dec.rs1));
						as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
						log_trace("\t\tmov qword ptr [rax + %lld], %s", dec.imm, x86_reg_str(rs2x));
					}
				} else {
					as.mov(x86::rcx, frame_reg(dec.rs2));
					log_trace("\t\tmov rcx, %s", frame_reg_str(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						printf("\t\tmov qword ptr [%s + %lld], rcx", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg(dec.rs1));
						as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", frame_reg_str(dec.rs1));
						log_trace("\t\tmov qword ptr [rax + %lld], rcx", dec.imm);
					}
				}
			}
			return true;
		}

		bool emit_li(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tli\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(x86::rax, Imm(dec.imm));
					as.mov(frame_reg(dec.rd), x86::rax);
					log_trace("\t\tmov %s, %lld", frame_reg_str(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_lui(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tli\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					as.mov(x86::gpq(rdx), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg(dec.rd), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", frame_reg_str(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_la(decode_type &dec)
		{
			return false;
		}

		bool emit_call(decode_type &dec)
		{
			return false;
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
				case rv_op_sll: return emit_sll(dec);
				case rv_op_srl: return emit_srl(dec);
				case rv_op_sra: return emit_sra(dec);
				case rv_op_addi: return emit_addi(dec);
				case rv_op_slli: return emit_slli(dec);
				case rv_op_srli: return emit_srli(dec);
				case rv_op_srai: return emit_srai(dec);
				case rv_op_bne: return emit_bne(dec);
				case rv_op_beq: return emit_beq(dec);
				case rv_op_ld: return emit_ld(dec);
				case rv_op_sd: return emit_sd(dec);
				case rv_op_lui: return emit_lui(dec);
				case fusion_op_li: return emit_li(dec);
				case fusion_op_la: return emit_la(dec);
				case fusion_op_call: return emit_call(dec);
			}
			return false;
		}
	};

}

#endif
