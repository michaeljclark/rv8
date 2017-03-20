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

		const char* frame_reg_32_str(int reg)
		{
			static char buf[32];
			snprintf(buf, sizeof(buf), "dword ptr [rbp + %lu]", offsetof(processor_rv64imafd, ireg) + reg * (P::xlen >> 3));
			return buf;
		}

		const X86Mem frame_reg_32(int reg)
		{
			return x86::dword_ptr(x86::rbp, offsetof(processor_rv64imafd, ireg) + reg * (P::xlen >> 3));
		}

		const char* frame_reg_64_str(int reg)
		{
			static char buf[32];
			snprintf(buf, sizeof(buf), "qword ptr [rbp + %lu]", offsetof(processor_rv64imafd, ireg) + reg * (P::xlen >> 3));
			return buf;
		}

		const X86Mem frame_reg_64(int reg)
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
			as.mov(x86::rdx, frame_reg_64(rv_ireg_ra));
			as.mov(x86::rbx, frame_reg_64(rv_ireg_sp));
			as.mov(x86::rsi, frame_reg_64(rv_ireg_t0));
			as.mov(x86::rdi, frame_reg_64(rv_ireg_t1));
			as.mov(x86::r8,  frame_reg_64(rv_ireg_a0));
			as.mov(x86::r9,  frame_reg_64(rv_ireg_a1));
			as.mov(x86::r10, frame_reg_64(rv_ireg_a2));
			as.mov(x86::r11, frame_reg_64(rv_ireg_a3));
			as.mov(x86::r12, frame_reg_64(rv_ireg_a4));
			as.mov(x86::r13, frame_reg_64(rv_ireg_a5));
			as.mov(x86::r14, frame_reg_64(rv_ireg_a6));
			as.mov(x86::r15, frame_reg_64(rv_ireg_a7));
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
			as.mov(frame_reg_64(rv_ireg_ra), x86::rdx);
			as.mov(frame_reg_64(rv_ireg_sp), x86::rbx);
			as.mov(frame_reg_64(rv_ireg_t0), x86::rsi);
			as.mov(frame_reg_64(rv_ireg_t1), x86::rdi);
			as.mov(frame_reg_64(rv_ireg_a0), x86::r8);
			as.mov(frame_reg_64(rv_ireg_a1), x86::r9);
			as.mov(frame_reg_64(rv_ireg_a2), x86::r10);
			as.mov(frame_reg_64(rv_ireg_a3), x86::r11);
			as.mov(frame_reg_64(rv_ireg_a4), x86::r12);
			as.mov(frame_reg_64(rv_ireg_a5), x86::r13);
			as.mov(frame_reg_64(rv_ireg_a6), x86::r14);
			as.mov(frame_reg_64(rv_ireg_a7), x86::r15);
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

		void emit_sign_extend_32(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd);
			if (rdx > 0) {
				as.movsxd(x86::gpq(rdx), x86::gpd(rdx));
				log_trace("\t\tmovsxd %s, %s", x86_reg_str(dec.rd), x86_reg_str(dec.rd));
			} else {
				as.mov(x86::ecx, frame_reg_32(dec.rd));
				as.movsxd(x86::rcx, x86::ecx);
				as.mov(frame_reg_64(dec.rd), x86::rcx);
				log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rd));
				log_trace("\t\tmovsxd rcx, ecx");
				log_trace("\t\tmov %s, rcx", frame_reg_64_str(dec.rd));
			}
		}

		void emit_zero_rd(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd);
			if (rdx > 0) {
				as.xor_(x86::gpq(rdx), x86::gpq(rdx));
				log_trace("\t\tmov %s, 0", x86_reg_str(rdx));
			} else {
				as.mov(frame_reg_64(dec.rd), Imm(0));
				log_trace("\t\tmov %s, 0", frame_reg_64_str(dec.rd));
			}
		}

		void emit_mv_rd_rs1(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.mov(x86::gpq(rdx), frame_reg_64(dec.rs1));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(frame_reg_64(dec.rd), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs1x));
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs1_32(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpd(rdx), x86::gpd(rs1x));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.mov(x86::gpd(rdx), frame_reg_32(dec.rs1));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_32_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(frame_reg_32(dec.rd), x86::gpd(rs1x));
					log_trace("\t\tmov %s, %s", frame_reg_32_str(dec.rd), x86_reg_str(rs1x));
				} else {
					as.mov(x86::eax, frame_reg_32(dec.rs1));
					as.mov(frame_reg_32(dec.rd), x86::eax);
					log_trace("\t\tmov eax, %s", frame_reg_32_str(dec.rs1));
					log_trace("\t\tmov %s, eax", frame_reg_32_str(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs1_sx_32(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.movsxd(x86::gpq(rdx), x86::gpd(rs1x));
					log_trace("\t\tmovsxd %s, %s", x86_reg_str(rdx), x86_reg_str(rs1x));
				} else {
					as.movsxd(x86::gpq(rdx), frame_reg_32(dec.rs1));
					log_trace("\t\tmovsxd %s, %s", x86_reg_str(rdx), frame_reg_32_str(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.movsxd(x86::rax, x86::gpd(rs1x));
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, %s", x86_reg_str(rs1x));
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
				} else {
					as.movsxd(x86::rax, frame_reg_32(dec.rs1));
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, %s", frame_reg_32_str(dec.rs1));
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
					as.mov(x86::gpq(rdx), frame_reg_64(dec.rs2));
					log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.mov(frame_reg_64(dec.rd), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs2));
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
				}
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
					as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", x86_reg_str(rdx), dec.pc + dec.imm);
				} else {
					as.mov(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", frame_reg_64_str(dec.rd), dec.pc + dec.imm);
				}
			}
			return true;
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
						as.mov(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tmov %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.mov(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.mov(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						as.add(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.add(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tadd %s, rax", frame_reg_64_str(dec.rd));
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
						as.add(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tadd %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.add(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tadd %s, rax", frame_reg_64_str(dec.rd));
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
					as.neg(frame_reg_64(dec.rd));
					log_trace("\t\tneg %s", frame_reg_64_str(dec.rd));
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
						as.sub(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.sub(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tsub %s, rax", frame_reg_64_str(dec.rd));
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
						as.sub(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tsub %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.sub(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tsub %s, rax", frame_reg_64_str(dec.rd));
					}
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
					log_trace("\t\tcmp %s, %s", x86_reg_str(rs1x), x86_reg_str(rs2x));
				}
				else {
					// mov rax, rs1
					if (dec.rs1 == rv_ireg_zero) {
						as.xor_(x86::rax, x86::rax);
						log_trace("\t\txor rax, rax");
					} else if (rs1x > 0) {
						as.mov(x86::rax, x86::gpq(rs1x));
						log_trace("\t\tmov rax, %s", x86_reg_str(rs1x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					}
					// cmp rax, rs2
					if (dec.rs2 == rv_ireg_zero) {
						as.cmp(x86::rax, Imm(0));
					} else if (rs2x > 0) {
						as.cmp(x86::rax, x86::gpq(rs2x));
						log_trace("\t\tcmp rax, %s", x86_reg_str(rs2x));
					} else {
						as.cmp(x86::rax, frame_reg_64(dec.rs2));
						log_trace("\t\tcmp rax, %s", frame_reg_64_str(dec.rs2));
					}
				}
				// setl rd
				as.setl(x86::al);
				log_trace("\t\tsetl al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, rax", x86_reg_str(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
					log_trace("\t\tcmp %s, %s", x86_reg_str(rs1x), x86_reg_str(rs2x));
				}
				else {
					// mov rax, rs1
					if (dec.rs1 == rv_ireg_zero) {
						as.xor_(x86::rax, x86::rax);
						log_trace("\t\txor rax, rax");
					} else if (rs1x > 0) {
						as.mov(x86::rax, x86::gpq(rs1x));
						log_trace("\t\tmov rax, %s", x86_reg_str(rs1x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					}
					// cmp rax, rs2
					if (dec.rs2 == rv_ireg_zero) {
						as.cmp(x86::rax, Imm(0));
					} else if (rs2x > 0) {
						as.cmp(x86::rax, x86::gpq(rs2x));
						log_trace("\t\tcmp rax, %s", x86_reg_str(rs2x));
					} else {
						as.cmp(x86::rax, frame_reg_64(dec.rs2));
						log_trace("\t\tcmp rax, %s", frame_reg_64_str(dec.rs2));
					}
				}
				// setb rd
				as.setb(x86::al);
				log_trace("\t\tsetb al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, rax", x86_reg_str(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
				}
			}
			return true;
		}

		bool emit_and(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
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
						log_trace("\t\tand %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.and_(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tand %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.and_(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.and_(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tand %s, rax", frame_reg_64_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// and rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.and_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.and_(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tand %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.and_(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.and_(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tand %s, rax", frame_reg_64_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_or(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
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
						log_trace("\t\tor %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.or_(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tor %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.or_(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.or_(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tor %s, rax", frame_reg_64_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// or rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.or_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.or_(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\tor %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.or_(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.or_(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tor %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\txor %s, %s", x86_reg_str(rdx), x86_reg_str(rs2x));
					} else {
						as.xor_(x86::gpq(rdx), frame_reg_64(dec.rs2));
						log_trace("\t\txor %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.xor_(frame_reg_64(dec.rd), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs2));
						as.xor_(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\txor %s, rax", frame_reg_64_str(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// xor rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						as.xor_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\txor %s, %s", x86_reg_str(rdx), x86_reg_str(rs1x));
					} else {
						as.xor_(x86::gpq(rdx), frame_reg_64(dec.rs1));
						log_trace("\t\txor %s, %s", x86_reg_str(rdx), frame_reg_64_str(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.xor_(frame_reg_64(dec.rd), x86::gpq(rs1x));
						log_trace("\t\txor %s, %s", frame_reg_64_str(dec.rd), x86_reg_str(rs1x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.xor_(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\txor %s, rax", frame_reg_64_str(dec.rd));
					}
				}
			}
			else {
				// mov rax, rs1
				if (rs1x > 0) {
					as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", x86_reg_str(rs1x));
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
				}

				// xor rax, rs2
				// mov rs, rax
				if (rdx > 0) {
					if (rs2x > 0) {
						as.xor_(x86::rax, x86::gpq(rs2x));
						as.mov(x86::gpq(rdx), x86::rax);
						log_trace("\t\txor rax, %s", x86_reg_str(rs2x));
						log_trace("\t\tmov %s, rax", x86_reg_str(rdx));
					} else {
						as.xor_(x86::rax, frame_reg_64(dec.rs2));
						as.mov(x86::gpq(rdx), x86::rax);
						log_trace("\t\txor rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tmov %s, rax", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.xor_(x86::rax, x86::gpq(rs2x));
						as.mov(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\txor rax, %s", x86_reg_str(rs2x));
						log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
					} else {
						as.xor_(x86::rax, frame_reg_64(dec.rs2));
						as.mov(frame_reg_64(dec.rd), x86::rax);
						log_trace("\t\txor rax, %s", frame_reg_64_str(dec.rs2));
						log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_64_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_64_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_64_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_64_str(dec.rd));
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
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shr rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_64_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_64_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_64_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_64_str(dec.rd));
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
				emit_zero_rd(dec);
			}
			else if (dec.rs2 == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_64_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_64_str(dec.rd));
					}
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, cl", frame_reg_64_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_64_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(frame_reg_64(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_64_str(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sllw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
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
				// shl rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_32_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_32_str(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shl rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shl(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_32_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shl(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshl %s, cl", frame_reg_32_str(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_srlw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
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
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_32_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_32_str(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shr rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.shr(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_32_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.shr(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tshr %s, cl", frame_reg_32_str(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_sraw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
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
				// add rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_32_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_32_str(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// sar rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, cl", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", x86_reg_str(rdx));
					}
				} else {
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
						as.sar(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", x86_reg_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_32_str(dec.rd));
					} else {
						as.mov(x86::ecx, frame_reg_32(dec.rs2));
						as.sar(frame_reg_32(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", frame_reg_32_str(dec.rs2));
						log_trace("\t\tsar %s, cl", frame_reg_32_str(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
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
					as.mov(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_64_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.add(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					as.add(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), result);
				} else {
					as.mov(frame_reg_64(dec.rd), Imm(result));
					log_trace("\t\tmov %s, %d", frame_reg_64_str(dec.rd), result);
				}
			}
			else {
				// cmp rs1, imm
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", x86_reg_str(rs1x), dec.imm);
				} else {
					as.cmp(frame_reg_64(dec.rs1), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", frame_reg_64_str(dec.rs1), dec.imm);
				}
				// setl rd
				as.setl(x86::al);
				log_trace("\t\tsetl al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, rax", x86_reg_str(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), result);
				} else {
					as.mov(frame_reg_64(dec.rd), Imm(result));
					log_trace("\t\tmov %s, %d", frame_reg_64_str(dec.rd), result);
				}
			}
			else {
				// cmp rs1, imm
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", x86_reg_str(rs1x), dec.imm);
				} else {
					as.cmp(frame_reg_64(dec.rs1), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", frame_reg_64_str(dec.rs1), dec.imm);
				}
				// setb rd
				as.setb(x86::al);
				log_trace("\t\tsetb al");
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, rax", x86_reg_str(rdx));
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
					log_trace("\t\tand %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.and_(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tand %s, %d", frame_reg_64_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// and rd, imm
				if (rdx > 0) {
					as.and_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tand %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.and_(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tand %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_64_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// or rd, imm
				if (rdx > 0) {
					as.or_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tor %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.or_(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tor %s, %d", frame_reg_64_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// or rd, imm
				if (rdx > 0) {
					as.or_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tor %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.or_(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tor %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_64_str(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// xor rd, imm
				if (rdx > 0) {
					as.xor_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\txor %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.xor_(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\txor %s, %d", frame_reg_64_str(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// xor rd, imm
				if (rdx > 0) {
					as.xor_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\txor %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.xor_(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\txor %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tshl %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shl(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					as.shl(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tshr %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shr(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					as.shr(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tsar %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.sar(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					as.sar(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", frame_reg_64_str(dec.rd), dec.imm);
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
					log_trace("\t\tmov %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", frame_reg_32_str(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.add(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", frame_reg_32_str(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_sx_32(dec);

				// add rd, imm
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.add(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", frame_reg_32_str(dec.rd), dec.imm);
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
					log_trace("\t\tshl %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shl(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", frame_reg_32_str(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shl rd, imm
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shl(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", frame_reg_32_str(dec.rd), dec.imm);
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
					log_trace("\t\tshr %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shr(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", frame_reg_32_str(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shr rd, imm
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.shr(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", frame_reg_32_str(dec.rd), dec.imm);
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
					log_trace("\t\tsar %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.sar(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", frame_reg_32_str(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// sar rd, imm
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", x86_reg_str(rdx), dec.imm);
				} else {
					as.sar(frame_reg_32(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", frame_reg_32_str(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
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
					as.cmp(frame_reg_64(dec.rs2), 0);
					log_trace("\t\tcmp %s, 0", frame_reg_64_str(dec.rs2));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.cmp(x86::gpq(rs1x), 0);
					log_trace("\t\tcmp %s, 0", x86_reg_str(rs1x));
				} else {
					as.cmp(frame_reg_64(dec.rs1), 0);
					log_trace("\t\tcmp %s, 0", frame_reg_64_str(dec.rs1));
				}
			}
			else if (rs1x > 0) {
				if (rs2x > 0) {
					as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", x86_reg_str(rs1x), x86_reg_str(rs2x));
				} else {
					as.cmp(x86::gpq(rs1x), frame_reg_64(dec.rs2));
					log_trace("\t\tcmp %s, %s", x86_reg_str(rs1x), frame_reg_64_str(dec.rs2));
				}
			}
			else {
				if (rs2x > 0) {
					as.cmp(frame_reg_64(dec.rs1), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", frame_reg_64_str(dec.rs1), x86_reg_str(rs2x));
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					as.cmp(x86::rax, frame_reg_64(dec.rs2));
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					log_trace("\t\tcmp rax, %s", frame_reg_64_str(dec.rs1));
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

		bool emit_blt(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.jl(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\tjl 0x%016llx", branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				as.jl(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				log_trace("\t\tjl 0x%016llx", branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				as.jge(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjge 0x%016llx", cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			} else {
				Label l = as.newLabel();
				as.jl(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjl 1f");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			}

			return true;
		}

		bool emit_bge(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val;
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.jge(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\tjge 0x%016llx", branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				as.jge(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				log_trace("\t\tjge 0x%016llx", branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				as.jl(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjl 0x%016llx", cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			} else {
				Label l = as.newLabel();
				as.jge(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjge 1f");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			}

			return true;
		}

		bool emit_bltu(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.jb(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\tjb 0x%016llx", branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				as.jb(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				log_trace("\t\tjb 0x%016llx", branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				as.jae(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjae 0x%016llx", cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			} else {
				Label l = as.newLabel();
				as.jb(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjb 1f");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			}

			return true;
		}

		bool emit_bgeu(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val;
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);
			term_pc = 0;

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.jae(branch_i->second);
				as.jmp(cont_i->second);
				log_trace("\t\tjae 0x%016llx", branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				as.jae(branch_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				log_trace("\t\tjae 0x%016llx", branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				as.jb(cont_i->second);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjb 0x%016llx", cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", offsetof(processor_rv64imafd, pc), branch_pc);
				log_trace("\t\tjmp term");
			} else {
				Label l = as.newLabel();
				as.jae(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)cont_pc);
				as.jmp(term);
				as.bind(l);
				as.mov(x86::qword_ptr(x86::rbp, offsetof(processor_rv64imafd, pc)), (unsigned)branch_pc);
				as.jmp(term);
				log_trace("\t\tjae 1f");
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
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::gpq(rdx), x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov %s, qword ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.mov(x86::rax, x86::qword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov rax, qword ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::rax, x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov rax, qword ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\tmovsxd %s, dword ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movsxd(x86::gpq(rdx), x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmovsxd %s, dword ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movsxd(x86::rax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsxd rax, dword ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movsxd(x86::rax, x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmovsxd rax, dword ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\tmov %s, dword ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::gpd(rdx), x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov %s, dword ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.mov(x86::eax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, dword ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::eax, x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov eax, dword ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\tmovsx %s, word ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movsx(x86::gpq(rdx), x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmovsx %s, word ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movsx(x86::rax, x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx rax, word ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movsx(x86::rax, x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmovsx rax, word ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\tmov %s, word ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movzx(x86::gpd(rdx), x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov %s, word ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movzx(x86::eax, x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, word ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movzx(x86::eax, x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov eax, word ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\tmovsx %s, byte ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movsx(x86::gpq(rdx), x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmovsx %s, byte ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movsx(x86::rax, x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx rax, byte ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movsx(x86::rax, x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmovsx rax, byte ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
						log_trace("\t\tmov %s, byte ptr [%s + %lld]", x86_reg_str(rdx), x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov %s, byte ptr [rax + %lld]", x86_reg_str(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						as.movzx(x86::eax, x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, byte ptr [%s + %lld]", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.movzx(x86::eax, x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov eax, byte ptr [rax + %lld]", dec.imm);
					}
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					as.mov(x86::qword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					log_trace("\t\tmov qword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov qword ptr [%s + %lld], %s", x86_reg_str(rs1x), dec.imm, x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov qword ptr [rax + %lld], %s", dec.imm, x86_reg_str(rs2x));
					}
				} else {
					as.mov(x86::rcx, frame_reg_64(dec.rs2));
					log_trace("\t\tmov rcx, %s", frame_reg_64_str(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						log_trace("\t\tmov qword ptr [%s + %lld], rcx", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
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
					log_trace("\t\tmov dword ptr [%s + %lld], 0", x86_reg_str(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					as.mov(x86::dword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					log_trace("\t\tmov dword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov dword ptr [%s + %lld], %s", x86_reg_str(rs1x), dec.imm, x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::dword_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov dword ptr [rax + %lld], %s", dec.imm, x86_reg_str(rs2x));
					}
				} else {
					as.mov(x86::rcx, frame_reg_64(dec.rs2));
					log_trace("\t\tmov rcx, %s", frame_reg_64_str(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						log_trace("\t\tmov dword ptr [%s + %lld], rcx", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::dword_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov dword ptr [rax + %lld], rcx", dec.imm);
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
					log_trace("\t\tmov word ptr [%s + %lld], 0", x86_reg_str(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					as.mov(x86::word_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					log_trace("\t\tmov word ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov word ptr [%s + %lld], %s", x86_reg_str(rs1x), dec.imm, x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::word_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov word ptr [rax + %lld], %s", dec.imm, x86_reg_str(rs2x));
					}
				} else {
					as.mov(x86::rcx, frame_reg_64(dec.rs2));
					log_trace("\t\tmov rcx, %s", frame_reg_64_str(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						log_trace("\t\tmov word ptr [%s + %lld], rcx", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::word_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov word ptr [rax + %lld], rcx", dec.imm);
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
					log_trace("\t\tmov byte ptr [%s + %lld], 0", x86_reg_str(rs1x), dec.imm);
				} else {
					as.mov(x86::rax, frame_reg_64(dec.rs1));
					as.mov(x86::byte_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
					log_trace("\t\tmov byte ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov byte ptr [%s + %lld], %s", x86_reg_str(rs1x), dec.imm, x86_reg_str(rs2x));
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::byte_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov byte ptr [rax + %lld], %s", dec.imm, x86_reg_str(rs2x));
					}
				} else {
					as.mov(x86::rcx, frame_reg_64(dec.rs2));
					log_trace("\t\tmov rcx, %s", frame_reg_64_str(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						log_trace("\t\tmov byte ptr [%s + %lld], rcx", x86_reg_str(rs1x), dec.imm);
					} else {
						as.mov(x86::rax, frame_reg_64(dec.rs1));
						as.mov(x86::byte_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", frame_reg_64_str(dec.rs1));
						log_trace("\t\tmov byte ptr [rax + %lld], rcx", dec.imm);
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
				} else if (dec.imm >= std::numeric_limits<int>::min() && dec.imm <= std::numeric_limits<int>::max()) {
					as.mov(frame_reg_64(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", frame_reg_64_str(dec.rd), dec.imm);
				} else {
					as.mov(x86::rax, Imm(dec.imm));
					as.mov(frame_reg_64(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %lld",  dec.imm);
					log_trace("\t\tmov %s, rax", frame_reg_64_str(dec.rd));
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
					log_trace("\t\tmov %s, %lld", x86_reg_str(rdx), dec.imm);
				} else {
					as.mov(frame_reg_64(dec.rd), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", frame_reg_64_str(dec.rd), dec.imm);
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
				case rv_op_slt: return emit_slt(dec);
				case rv_op_sltu: return emit_sltu(dec);
				case rv_op_and: return emit_and(dec);
				case rv_op_or: return emit_or(dec);
				case rv_op_xor: return emit_xor(dec);
				case rv_op_sll: return emit_sll(dec);
				case rv_op_srl: return emit_srl(dec);
				case rv_op_sra: return emit_sra(dec);
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
				case fusion_op_li: return emit_li(dec);
				case fusion_op_la: return emit_la(dec);
				case fusion_op_call: return emit_call(dec);
			}
			return false;
		}
	};

}

#endif
