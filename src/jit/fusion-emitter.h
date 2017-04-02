//
//  fusion-emitter.h
//

#ifndef rv_fusion_emitter_h
#define rv_fusion_emitter_h

namespace riscv {

	using namespace asmjit;

	template <typename P>
	struct fusion_emitter : fusion_base<P>
	{
		typedef typename P::decode_type decode_type;
		typedef fusion_base<P> rv;

		P &proc;
		Label term;
		std::map<addr_t,Label> labels;
		std::vector<addr_t> callstack;
		addr_t term_pc;

		fusion_emitter(P &proc, CodeHolder &code)
			: fusion_base<P>(code), proc(proc), term_pc(0)
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

		void begin()
		{
			term = rv::as.newLabel();
		}

		void end()
		{
			if (term_pc) {
				rv::as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), Imm(term_pc));
				log_trace("\t# 0x%016llx", term_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), term_pc);
			}
			log_trace("\t\tterm:");
			rv::as.bind(term);
		}

		void emit_sign_extend_32(decode_type &dec)
		{
			int rdx = rv::x86_reg(dec.rd);
			if (rdx > 0) {
				rv::as.movsxd(x86::gpq(rdx), x86::gpd(rdx));
				log_trace("\t\tmovsxd %s, %s", rv::x86_reg_str_q(dec.rd), rv::x86_reg_str_d(dec.rd));
			} else {
				rv::as.movsxd(x86::rcx, rv::rbp_reg_d(dec.rd));
				rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rcx);
				log_trace("\t\tmovsxd rcx, %s", rv::rbp_reg_str_d(dec.rd));
				log_trace("\t\tmov %s, rcx", rv::rbp_reg_str_q(dec.rd));
			}
		}

		void emit_zero_rd(decode_type &dec)
		{
			int rdx = rv::x86_reg(dec.rd);
			if (rdx > 0) {
				rv::as.xor_(x86::gpd(rdx), x86::gpd(rdx));
				log_trace("\t\txor %s, %s", rv::x86_reg_str_d(rdx), rv::x86_reg_str_d(rdx));
			} else {
				rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(0));
				log_trace("\t\tmov %s, 0", rv::rbp_reg_str_q(dec.rd));
			}
		}

		void emit_mv_rd_rs1(decode_type &dec)
		{
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					rv::as.mov(x86::gpq(rdx), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::gpq(rdx), rv::rbp_reg_q(dec.rs1));
					log_trace("\t\tmov %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::gpq(rs1x));
					log_trace("\t\tmov %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs1_32(decode_type &dec)
		{
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					rv::as.mov(x86::gpd(rdx), x86::gpd(rs1x));
					log_trace("\t\tmov %s, %s", rv::x86_reg_str_d(rdx), rv::x86_reg_str_d(rs1x));
				} else {
					rv::as.mov(x86::gpd(rdx), rv::rbp_reg_d(dec.rs1));
					log_trace("\t\tmov %s, %s", rv::x86_reg_str_d(rdx), rv::rbp_reg_str_d(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					rv::as.mov(rv::rbp_reg_d(dec.rd), x86::gpd(rs1x));
					log_trace("\t\tmov %s, %s", rv::rbp_reg_str_d(dec.rd), rv::x86_reg_str_d(rs1x));
				} else {
					rv::as.mov(x86::eax, rv::rbp_reg_d(dec.rs1));
					rv::as.mov(rv::rbp_reg_d(dec.rd), x86::eax);
					log_trace("\t\tmov eax, %s", rv::rbp_reg_str_d(dec.rs1));
					log_trace("\t\tmov %s, eax", rv::rbp_reg_str_d(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs1_sx_32(decode_type &dec)
		{
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					rv::as.movsxd(x86::gpq(rdx), x86::gpd(rs1x));
					log_trace("\t\tmovsxd %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_d(rs1x));
				} else {
					rv::as.movsxd(x86::gpq(rdx), rv::rbp_reg_d(dec.rs1));
					log_trace("\t\tmovsxd %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_d(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					rv::as.movsxd(x86::rax, x86::gpd(rs1x));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, %s", rv::x86_reg_str_d(rs1x));
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				} else {
					rv::as.movsxd(x86::rax, rv::rbp_reg_d(dec.rs1));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovsxd rax, %s", rv::rbp_reg_str_d(dec.rs1));
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
		}

		void emit_mv_rd_rs2(decode_type &dec)
		{
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
			if (rdx > 0) {
				if (rs2x > 0) {
					rv::as.mov(x86::gpq(rdx), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.mov(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tmov %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
					log_trace("\t\tmov %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
		}

		bool emit_auipc(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(dec.pc + dec.imm));
					log_trace("\t\tmov %s, %lld", rv::x86_reg_str_q(rdx), dec.pc + dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(dec.pc + dec.imm));
					log_trace("\t\tmov %s, %lld", rv::rbp_reg_str_q(dec.rd), dec.pc + dec.imm);
				}
			}
			return true;
		}

		bool emit_add(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.mov(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tmov %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tmov %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
						rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
						log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
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
						rv::as.add(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.add(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tadd %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.add(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
						rv::as.add(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
						log_trace("\t\tadd %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// add rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.add(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tadd %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.add(x86::gpq(rdx), rv::rbp_reg_q(dec.rs1));
						log_trace("\t\tadd %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						rv::as.add(rv::rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tadd %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.add(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tadd %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx > 0 && rs1x > 0 && rs2x > 0) {
				rv::as.lea(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), x86::gpq(rs2x)));
				log_trace("\t\tlea %s, qword ptr [%s + %s]",
					rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x), rv::x86_reg_str_q(rs2x));
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// add rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					rv::as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					rv::as.add(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tadd rax, %s", rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.add(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tadd rax, %s", rv::rbp_reg_str_q(dec.rs2));
				}
				rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.add(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.add(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tadd %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.add(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tadd %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_sub(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
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
					rv::as.neg(x86::gpq(rdx));
					log_trace("\t\tneg %s", rv::x86_reg_str_q(rdx));
				} else {
					rv::as.neg(rv::rbp_reg_q(dec.rd));
					log_trace("\t\tneg %s", rv::rbp_reg_str_q(dec.rd));
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
						rv::as.sub(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.sub(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tsub %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.sub(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
						rv::as.sub(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
						log_trace("\t\tsub %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// sub rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.sub(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tsub %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.sub(x86::gpq(rdx), rv::rbp_reg_q(dec.rs1));
						log_trace("\t\tsub %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						rv::as.sub(rv::rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tsub %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.sub(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tsub %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
				// neg rd
				if (rdx > 0) {
					rv::as.neg(x86::gpq(rdx));
					log_trace("\t\tneg %s", rv::x86_reg_str_q(rdx));
				} else {
					rv::as.neg(rv::rbp_reg_q(dec.rd));
					log_trace("\t\tneg %s", rv::rbp_reg_str_q(dec.rd));
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// sub rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					rv::as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					rv::as.sub(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tsub rax, %s", rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.sub(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tsub rax, %s", rv::rbp_reg_str_q(dec.rs2));
				}
				rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sub rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.sub(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.sub(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tsub %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.sub(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tsub %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_slt(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				// cmp rs1, rs2
				if (rs1x > 0 && rs2x > 0) {
					rv::as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", rv::x86_reg_str_q(rs1x), rv::x86_reg_str_q(rs2x));
				}
				else {
					// mov rax, rs1
					if (dec.rs1 == rv_ireg_zero) {
						rv::as.xor_(x86::eax, x86::eax);
						log_trace("\t\txor eax, eax");
					} else if (rs1x > 0) {
						rv::as.mov(x86::rax, x86::gpq(rs1x));
						log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					}
					// cmp rax, rs2
					if (dec.rs2 == rv_ireg_zero) {
						rv::as.cmp(x86::rax, Imm(0));
					} else if (rs2x > 0) {
						rv::as.cmp(x86::rax, x86::gpq(rs2x));
						log_trace("\t\tcmp rax, %s", rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.cmp(x86::rax, rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tcmp rax, %s", rv::rbp_reg_str_q(dec.rs2));
					}
				}
				// setl rd
				rv::as.setl(x86::al);
				log_trace("\t\tsetl al");
				if (rdx > 0) {
					rv::as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", rv::x86_reg_str_d(rdx));
				} else {
					rv::as.movzx(x86::eax, x86::al);
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_sltu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				// cmp rs1, rs2
				if (rs1x > 0 && rs2x > 0) {
					rv::as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", rv::x86_reg_str_q(rs1x), rv::x86_reg_str_q(rs2x));
				}
				else {
					// mov rax, rs1
					if (dec.rs1 == rv_ireg_zero) {
						rv::as.xor_(x86::eax, x86::eax);
						log_trace("\t\txor eax, eax");
					} else if (rs1x > 0) {
						rv::as.mov(x86::rax, x86::gpq(rs1x));
						log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					}
					// cmp rax, rs2
					if (dec.rs2 == rv_ireg_zero) {
						rv::as.cmp(x86::rax, Imm(0));
					} else if (rs2x > 0) {
						rv::as.cmp(x86::rax, x86::gpq(rs2x));
						log_trace("\t\tcmp rax, %s", rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.cmp(x86::rax, rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tcmp rax, %s", rv::rbp_reg_str_q(dec.rs2));
					}
				}
				// setb rd
				rv::as.setb(x86::al);
				log_trace("\t\tsetb al");
				if (rdx > 0) {
					rv::as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", rv::x86_reg_str_d(rdx));
				} else {
					rv::as.movzx(x86::eax, x86::al);
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_and(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
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
						rv::as.and_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.and_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tand %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.and_(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
						rv::as.and_(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
						log_trace("\t\tand %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// and rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.and_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tand %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.and_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs1));
						log_trace("\t\tand %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						rv::as.and_(rv::rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tand %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.and_(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tand %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// and rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					rv::as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					rv::as.and_(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tand rax, %s", rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.and_(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tand rax, %s", rv::rbp_reg_str_q(dec.rs2));
				}
				rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// and rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.and_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.and_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tand %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.and_(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tand %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_or(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
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
						rv::as.or_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.or_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tor %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.or_(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
						rv::as.or_(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
						log_trace("\t\tor %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// or rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.or_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\tor %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.or_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs1));
						log_trace("\t\tor %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						rv::as.or_(rv::rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\tor %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.or_(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tor %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// or rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					rv::as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					rv::as.or_(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tor rax, %s", rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.or_(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tor rax, %s", rv::rbp_reg_str_q(dec.rs2));
				}
				rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// or rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.or_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.or_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\tor %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.or_(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\tor %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_xor(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
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
						rv::as.xor_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.xor_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\txor %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.xor_(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs2));
						rv::as.xor_(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs2));
						log_trace("\t\txor %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				// xor rd, rs1
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.xor_(x86::gpq(rdx), x86::gpq(rs1x));
						log_trace("\t\txor %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.xor_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs1));
						log_trace("\t\txor %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						rv::as.xor_(rv::rbp_reg_q(dec.rd), x86::gpq(rs1x));
						log_trace("\t\txor %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs1x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.xor_(rv::rbp_reg_q(dec.rd), x86::rax);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\txor %s, rax", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				// mov rax, rs1
				// xor rax, rs2
				// mov rd, rax
				if (rs1x > 0) {
					rv::as.mov(x86::rax, x86::gpq(rs1x));
					log_trace("\t\tmov rax, %s", rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
				}
				if (rs2x > 0) {
					rv::as.xor_(x86::rax, x86::gpq(rs2x));
					log_trace("\t\txor rax, %s", rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.xor_(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\txor rax, %s", rv::rbp_reg_str_q(dec.rs2));
				}
				rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
				log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// xor rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.xor_(x86::gpq(rdx), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.xor_(x86::gpq(rdx), rv::rbp_reg_q(dec.rs2));
						log_trace("\t\txor %s, %s", rv::x86_reg_str_q(rdx), rv::rbp_reg_str_q(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						rv::as.xor_(rv::rbp_reg_q(dec.rd), x86::gpq(rs2x));
						log_trace("\t\txor %s, %s", rv::rbp_reg_str_q(dec.rd), rv::x86_reg_str_q(rs2x));
					}
				}
			}
			return true;
		}

		bool emit_sll(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
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
				// shl rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shl(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_q(dec.rd));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shl(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else {
				// mov ecx, rs2
				if (rs2x > 0) {
					rv::as.mov(x86::ecx, x86::gpd(rs2x));
					log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
				} else {
					rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
					log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
				}

				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.shl(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.shl(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_q(dec.rd));
					} else {
						rv::as.shl(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_srl(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
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
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shr(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_q(dec.rd));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shr(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else {
				// mov ecx, rs2
				if (rs2x > 0) {
					rv::as.mov(x86::ecx, x86::gpd(rs2x));
					log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
				} else {
					rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
					log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
				}

				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.shr(x86::gpq(rdx), x86::cl);
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.shr(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_q(dec.rd));
					} else {
						rv::as.shr(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sra(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
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
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.sar(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_q(dec.rd));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.sar(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			else {
				// mov ecx, rs2
				if (rs2x > 0) {
					rv::as.mov(x86::ecx, x86::gpd(rs2x));
					log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
				} else {
					rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
					log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
				}

				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.sar(x86::gpq(rdx), x86::cl);
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_q(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.sar(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_q(dec.rd));
					} else {
						rv::as.sar(rv::rbp_reg_q(dec.rd), x86::cl);
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_q(dec.rd));
					}
				}
			}
			return true;
		}

		bool emit_sllw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
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
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_q(rdx));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_d(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shl(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_d(dec.rd));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shl(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_d(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov ecx, rs2
				if (rs2x > 0) {
					rv::as.mov(x86::ecx, x86::gpd(rs2x));
					log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
				} else {
					rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
					log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
				}

				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shl rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_d(rdx));
					} else {
						rv::as.shl(x86::gpd(rdx), x86::cl);
						log_trace("\t\tshl %s, cl", rv::x86_reg_str_d(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.shl(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_d(dec.rd));
					} else {
						rv::as.shl(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tshl %s, cl", rv::rbp_reg_str_d(dec.rd));
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
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
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
				// shr rd, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_d(rdx));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_d(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.shr(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_d(dec.rd));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.shr(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_d(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov ecx, rs2
				if (rs2x > 0) {
					rv::as.mov(x86::ecx, x86::gpd(rs2x));
					log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
				} else {
					rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
					log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
				}

				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shr rs, cl
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_d(rdx));
					} else {
						rv::as.shr(x86::gpd(rdx), x86::cl);
						log_trace("\t\tshr %s, cl", rv::x86_reg_str_d(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.shr(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_d(dec.rd));
					} else {
						rv::as.shr(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tshr %s, cl", rv::rbp_reg_str_d(dec.rd));
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
			int rdx = rv::x86_reg(dec.rd), rs2x = rv::x86_reg(dec.rs2);
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
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_d(rdx));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_d(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.mov(x86::ecx, x86::gpd(rs2x));
						rv::as.sar(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_d(dec.rd));
					} else {
						rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
						rv::as.sar(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_d(dec.rd));
					}
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov ecx, rs2
				if (rs2x > 0) {
					rv::as.mov(x86::ecx, x86::gpd(rs2x));
					log_trace("\t\tmov ecx, %s", rv::x86_reg_str_d(dec.rs2));
				} else {
					rv::as.mov(x86::ecx, rv::rbp_reg_d(dec.rs2));
					log_trace("\t\tmov ecx, %s", rv::rbp_reg_str_d(dec.rs2));
				}

				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// sar rs, rs2
				if (rdx > 0) {
					if (rs2x > 0) {
						rv::as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_d(rdx));
					} else {
						rv::as.sar(x86::gpd(rdx), x86::cl);
						log_trace("\t\tsar %s, cl", rv::x86_reg_str_d(rdx));
					}
				} else {
					if (rs2x > 0) {
						rv::as.sar(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_d(dec.rd));
					} else {
						rv::as.sar(rv::rbp_reg_d(dec.rd), x86::cl);
						log_trace("\t\tsar %s, cl", rv::rbp_reg_str_d(dec.rd));
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
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					rv::as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.add(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.imm == 0) {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);
			}
			else if (rdx > 0 && rs1x > 0) {
				// lea rd, rs1, rs2
				rv::as.lea(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), int32_t(dec.imm)));
				log_trace("\t\tlea %s, qword ptr [%s + %d]",
					rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// add rd, imm
				if (rdx > 0) {
					rv::as.add(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.add(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_slti(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				bool result = 0LL < dec.imm;
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(result));
					log_trace("\t\tmov %s, %d", rv::x86_reg_str_q(rdx), result);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(result));
					log_trace("\t\tmov %s, %d", rv::rbp_reg_str_q(dec.rd), result);
				}
			}
			else {
				// cmp rs1, imm
				if (rs1x > 0) {
					rv::as.cmp(x86::gpq(rs1x), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", rv::x86_reg_str_q(rs1x), dec.imm);
				} else {
					rv::as.cmp(rv::rbp_reg_q(dec.rs1), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", rv::rbp_reg_str_q(dec.rs1), dec.imm);
				}
				// setl rd
				rv::as.setl(x86::al);
				log_trace("\t\tsetl al");
				if (rdx > 0) {
					rv::as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", rv::x86_reg_str_d(rdx));
				} else {
					rv::as.movzx(x86::eax, x86::al);
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_sltiu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				bool result = 0ULL < u64(dec.imm);
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(result));
					log_trace("\t\tmov %s, %d", rv::x86_reg_str_q(rdx), result);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(result));
					log_trace("\t\tmov %s, %d", rv::rbp_reg_str_q(dec.rd), result);
				}
			}
			else {
				// cmp rs1, imm
				if (rs1x > 0) {
					rv::as.cmp(x86::gpq(rs1x), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", rv::x86_reg_str_q(rs1x), dec.imm);
				} else {
					rv::as.cmp(rv::rbp_reg_q(dec.rs1), Imm(dec.imm));
					log_trace("\t\tcmp %s, %d", rv::rbp_reg_str_q(dec.rs1), dec.imm);
				}
				// setb rd
				rv::as.setb(x86::al);
				log_trace("\t\tsetb al");
				if (rdx > 0) {
					rv::as.movzx(x86::gpd(rdx), x86::al);
					log_trace("\t\tmovzx %s, al", rv::x86_reg_str_d(rdx));
				} else {
					rv::as.movzx(x86::eax, x86::al);
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmovzx eax, al");
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_andi(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// and rd, imm
				if (rdx > 0) {
					rv::as.and_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tand %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.and_(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tand %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// and rd, imm
				if (rdx > 0) {
					rv::as.and_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tand %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.and_(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tand %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_ori(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// or rd, imm
				if (rdx > 0) {
					rv::as.or_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tor %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.or_(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tor %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// or rd, imm
				if (rdx > 0) {
					rv::as.or_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tor %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.or_(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tor %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_xori(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else if (dec.rd == dec.rs1) {
				// xor rd, imm
				if (rdx > 0) {
					rv::as.xor_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\txor %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.xor_(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\txor %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// xor rd, imm
				if (rdx > 0) {
					rv::as.xor_(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\txor %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.xor_(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\txor %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_slli(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shl rd, imm
				if (rdx > 0) {
					rv::as.shl(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.shl(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shl rd, imm
				if (rdx > 0) {
					rv::as.shl(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.shl(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_srli(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shr rd, imm
				if (rdx > 0) {
					rv::as.shr(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.shr(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// shr rd, imm
				if (rdx > 0) {
					rv::as.shr(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.shr(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_srai(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, imm
				if (rdx > 0) {
					rv::as.sar(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.sar(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1(dec);

				// sar rd, imm
				if (rdx > 0) {
					rv::as.sar(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.sar(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_addiw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				// mov rd, imm
				if (rdx > 0) {
					rv::as.mov(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else if (dec.rd == dec.rs1) {
				// add rd, imm
				if (rdx > 0) {
					rv::as.add(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.add(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_sx_32(dec);

				// add rd, imm
				if (rdx > 0) {
					rv::as.add(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.add(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tadd %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_slliw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shl rd, imm
				if (rdx > 0) {
					rv::as.shl(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.shl(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shl rd, imm
				if (rdx > 0) {
					rv::as.shl(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.shl(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshl %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_srliw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// shr rd, imm
				if (rdx > 0) {
					rv::as.shr(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.shr(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// shr rd, imm
				if (rdx > 0) {
					rv::as.shr(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.shr(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tshr %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		bool emit_sraiw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else if (dec.rd == dec.rs1) {
				// sar rd, imm
				if (rdx > 0) {
					rv::as.sar(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.sar(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			else {
				// mov rd, rs1
				emit_mv_rd_rs1_32(dec);

				// sar rd, imm
				if (rdx > 0) {
					rv::as.sar(x86::gpd(rdx), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::x86_reg_str_d(rdx), dec.imm);
				} else {
					rv::as.sar(rv::rbp_reg_d(dec.rd), Imm(dec.imm));
					log_trace("\t\tsar %s, %d", rv::rbp_reg_str_d(dec.rd), dec.imm);
				}
				emit_sign_extend_32(dec);
			}
			return true;
		}

		void emit_cmp(decode_type &dec)
		{
			int rs1x = rv::x86_reg(dec.rs1), rs2x = rv::x86_reg(dec.rs2);
			if (dec.rs1 == rv_ireg_zero) {
				rv::as.xor_(x86::eax, x86::eax);
				log_trace("\t\txor eax, eax");
				if (rs2x > 0) {
					rv::as.cmp(x86::rax, x86::gpq(rs2x));
					log_trace("\t\tcmp rax, %s", rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.cmp(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tcmp rax, %s", rv::rbp_reg_str_q(dec.rs2));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					rv::as.cmp(x86::gpq(rs1x), Imm(0));
					log_trace("\t\tcmp %s, 0", rv::x86_reg_str_q(rs1x));
				} else {
					rv::as.cmp(rv::rbp_reg_q(dec.rs1), Imm(0));
					log_trace("\t\tcmp %s, 0", rv::rbp_reg_str_q(dec.rs1));
				}
			}
			else if (rs1x > 0) {
				if (rs2x > 0) {
					rv::as.cmp(x86::gpq(rs1x), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", rv::x86_reg_str_q(rs1x), rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.cmp(x86::gpq(rs1x), rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tcmp %s, %s", rv::x86_reg_str_q(rs1x), rv::rbp_reg_str_q(dec.rs2));
				}
			}
			else {
				if (rs2x > 0) {
					rv::as.cmp(rv::rbp_reg_q(dec.rs1), x86::gpq(rs2x));
					log_trace("\t\tcmp %s, %s", rv::rbp_reg_str_q(dec.rs1), rv::x86_reg_str_q(rs2x));
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					rv::as.cmp(x86::rax, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					log_trace("\t\tcmp rax, %s", rv::rbp_reg_str_q(dec.rs1));
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
				(rv::as.*bf)(branch_i->second);
				rv::as.jmp(cont_i->second);
				log_trace("\t\t%s 0x%016llx", bfname, branch_pc);
				log_trace("\t\tjmp 0x%016llx", cont_pc);
			}
			else if (cond && branch_i != labels.end()) {
				(rv::as.*bf)(branch_i->second);
				rv::as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)cont_pc);
				rv::as.jmp(term);
				log_trace("\t\t%s 0x%016llx", bfname, branch_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), cont_pc);
				log_trace("\t\tjmp term");
			}
			else if (!cond && cont_i != labels.end()) {
				(rv::as.*ibf)(cont_i->second);
				rv::as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)branch_pc);
				rv::as.jmp(term);
				log_trace("\t\t%s 0x%016llx", ibfname, cont_pc);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), branch_pc);
				log_trace("\t\tjmp term");
			} else if (cond) {
				Label l = rv::as.newLabel();
				(rv::as.*bf)(l);
				rv::as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)cont_pc);
				rv::as.jmp(term);
				rv::as.bind(l);
				log_trace("\t\t%s 1f", bfname);
				log_trace("\t\tmov [rbp + %lu], 0x%llx", proc_offset(pc), cont_pc);
				log_trace("\t\tjmp term");
				log_trace("\t\t1:");
				term_pc = branch_pc;
			} else {
				Label l = rv::as.newLabel();
				(rv::as.*ibf)(l);
				rv::as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), (unsigned)branch_pc);
				rv::as.jmp(term);
				rv::as.bind(l);
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
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.mov(x86::gpq(rdx), x86::qword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, qword ptr [%s + %lld]", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::gpq(rdx), x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, qword ptr [rax + %lld]", rv::x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.mov(x86::rax, x86::qword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov rax, qword ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::rax, x86::qword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov rax, qword ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.movsxd(x86::gpq(rdx), x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsxd %s, dword ptr [%s + %lld]", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movsxd(x86::gpq(rdx), x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsxd %s, dword ptr [rax + %lld]", rv::x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.movsxd(x86::rax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsxd rax, dword ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movsxd(x86::rax, x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsxd rax, dword ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lwu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.mov(x86::gpd(rdx), x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, dword ptr [%s + %lld]", rv::x86_reg_str_d(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::gpd(rdx), x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, dword ptr [rax + %lld]", rv::x86_reg_str_d(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.mov(x86::eax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, dword ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::eax, x86::dword_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov eax, dword ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lh(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.movsx(x86::gpq(rdx), x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx %s, word ptr [%s + %lld]", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movsx(x86::gpq(rdx), x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx %s, word ptr [rax + %lld]", rv::x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.movsx(x86::rax, x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx rax, word ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movsx(x86::rax, x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx rax, word ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lhu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.movzx(x86::gpd(rdx), x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, word ptr [%s + %lld]", rv::x86_reg_str_d(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movzx(x86::gpd(rdx), x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, word ptr [rax + %lld]", rv::x86_reg_str_d(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.movzx(x86::eax, x86::word_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, word ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movzx(x86::eax, x86::word_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov eax, word ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lb(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.movsx(x86::gpq(rdx), x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx %s, byte ptr [%s + %lld]", rv::x86_reg_str_q(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movsx(x86::gpq(rdx), x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx %s, byte ptr [rax + %lld]", rv::x86_reg_str_q(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.movsx(x86::rax, x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmovsx rax, byte ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movsx(x86::rax, x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmovsx rax, byte ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_lbu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					if (rs1x > 0) {
						rv::as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov %s, byte ptr [%s + %lld]", rv::x86_reg_str_d(rdx), rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov %s, byte ptr [rax + %lld]", rv::x86_reg_str_d(rdx), dec.imm);
					}
				} else {
					if (rs1x > 0) {
						rv::as.movzx(x86::eax, x86::byte_ptr(x86::gpq(rs1x), dec.imm));
						log_trace("\t\tmov eax, byte ptr [%s + %lld]", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.movzx(x86::eax, x86::byte_ptr(x86::rax, dec.imm));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov eax, byte ptr [rax + %lld]", dec.imm);
					}
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_sd(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rs2x = rv::x86_reg(dec.rs2), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					rv::as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov qword ptr [%s + %lld], 0", rv::x86_reg_str_q(rs1x), dec.imm);
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					rv::as.mov(x86::qword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov qword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						rv::as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov qword ptr [%s + %lld], %s", rv::x86_reg_str_q(rs1x), dec.imm, rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::gpq(rs2x));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov qword ptr [rax + %lld], %s", dec.imm, rv::x86_reg_str_q(rs2x));
					}
				} else {
					rv::as.mov(x86::rcx, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rv::rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						rv::as.mov(x86::qword_ptr(x86::gpq(rs1x), dec.imm), x86::rcx);
						log_trace("\t\tmov qword ptr [%s + %lld], rcx", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::qword_ptr(x86::rax, dec.imm), x86::rcx);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
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
			int rs2x = rv::x86_reg(dec.rs2), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					rv::as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov dword ptr [%s + %lld], 0", rv::x86_reg_str_q(rs1x), dec.imm);
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					rv::as.mov(x86::dword_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov dword ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						rv::as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), x86::gpd(rs2x));
						log_trace("\t\tmov dword ptr [%s + %lld], %s", rv::x86_reg_str_d(rs1x), dec.imm, rv::x86_reg_str_q(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::dword_ptr(x86::rax, dec.imm), x86::gpd(rs2x));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov dword ptr [rax + %lld], %s", dec.imm, rv::x86_reg_str_d(rs2x));
					}
				} else {
					rv::as.mov(x86::rcx, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rv::rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						rv::as.mov(x86::dword_ptr(x86::gpq(rs1x), dec.imm), x86::ecx);
						log_trace("\t\tmov dword ptr [%s + %lld], ecx", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::dword_ptr(x86::rax, dec.imm), x86::ecx);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
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
			int rs2x = rv::x86_reg(dec.rs2), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					rv::as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov word ptr [%s + %lld], 0", rv::x86_reg_str_q(rs1x), dec.imm);
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					rv::as.mov(x86::word_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov word ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						rv::as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), x86::gpw(rs2x));
						log_trace("\t\tmov word ptr [%s + %lld], %s", rv::x86_reg_str_q(rs1x), dec.imm, rv::x86_reg_str_w(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::word_ptr(x86::rax, dec.imm), x86::gpw(rs2x));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov word ptr [rax + %lld], %s", dec.imm, rv::x86_reg_str_w(rs2x));
					}
				} else {
					rv::as.mov(x86::rcx, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rv::rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						rv::as.mov(x86::word_ptr(x86::gpq(rs1x), dec.imm), x86::cx);
						log_trace("\t\tmov word ptr [%s + %lld], cx", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::word_ptr(x86::rax, dec.imm), x86::cx);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
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
			int rs2x = rv::x86_reg(dec.rs2), rs1x = rv::x86_reg(dec.rs1);
			if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					rv::as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), Imm(0));
					log_trace("\t\tmov byte ptr [%s + %lld], 0", rv::x86_reg_str_q(rs1x), dec.imm);
				} else {
					rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
					rv::as.mov(x86::byte_ptr(x86::rax, dec.imm), Imm(0));
					log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
					log_trace("\t\tmov byte ptr [rax + %lld], 0", dec.imm);
				}
			}
			else {
				if (rs2x > 0) {
					if (rs1x > 0) {
						rv::as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), x86::gpb_lo(rs2x));
						log_trace("\t\tmov byte ptr [%s + %lld], %s", rv::x86_reg_str_q(rs1x), dec.imm, rv::x86_reg_str_b(rs2x));
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::byte_ptr(x86::rax, dec.imm), x86::gpb_lo(rs2x));
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
						log_trace("\t\tmov byte ptr [rax + %lld], %s", dec.imm, rv::x86_reg_str_b(rs2x));
					}
				} else {
					rv::as.mov(x86::rcx, rv::rbp_reg_q(dec.rs2));
					log_trace("\t\tmov rcx, %s", rv::rbp_reg_str_q(dec.rs2));
					if (rs1x > 0) {
						rv::as.mov(x86::byte_ptr(x86::gpq(rs1x), dec.imm), x86::cl);
						log_trace("\t\tmov byte ptr [%s + %lld], cl", rv::x86_reg_str_q(rs1x), dec.imm);
					} else {
						rv::as.mov(x86::rax, rv::rbp_reg_q(dec.rs1));
						rv::as.mov(x86::byte_ptr(x86::rax, dec.imm), x86::cl);
						log_trace("\t\tmov rax, %s", rv::rbp_reg_str_q(dec.rs1));
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
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", rv::x86_reg_str_q(rdx), dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), (int)dec.imm);
					log_trace("\t\tmov %s, %lld", rv::rbp_reg_str_q(dec.rd), dec.imm);
				}
			}
			return true;
		}

		bool emit_jal(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + dec.imm;
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				addr_t link_addr = dec.pc + inst_length(dec.inst);
				callstack.push_back(link_addr);
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(link_addr));
					log_trace("\t\tmov %s, 0x%llx", rv::x86_reg_str_q(rdx), link_addr);
				} else {
					rv::as.mov(x86::rax, Imm(link_addr));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, 0x%llx",  link_addr);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_jalr(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + dec.imm;
			if (dec.rd == rv_ireg_zero && dec.rs1 == rv_ireg_ra && callstack.size() > 0) {
				addr_t link_addr = callstack.back();
				callstack.pop_back();
				rv::as.cmp(x86::gpq(rv::x86_reg(rv_ireg_ra)), Imm(link_addr));
				Label l = rv::as.newLabel();
				rv::as.je(l);
				rv::as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), Imm(dec.pc));
				rv::as.jmp(term);
				rv::as.bind(l);
				return true;
			}
			return false;
		}

		bool emit_li(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tli\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", rv::x86_reg_str_q(rdx), dec.imm);
				} else if (dec.imm >= std::numeric_limits<int>::min() && dec.imm <= std::numeric_limits<int>::max()) {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(dec.imm));
					log_trace("\t\tmov %s, %lld", rv::rbp_reg_str_q(dec.rd), dec.imm);
				} else {
					rv::as.mov(x86::rax, Imm(dec.imm));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, %lld",  dec.imm);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
				}
			}
			return true;
		}

		bool emit_la(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tla\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(dec.pc + dec.imm));
					log_trace("\t\tmov %s, %lld", rv::x86_reg_str_q(rdx), dec.pc + dec.imm);
				} else {
					rv::as.mov(rv::rbp_reg_q(dec.rd), Imm(dec.pc + dec.imm));
					log_trace("\t\tmov %s, %lld", rv::rbp_reg_str_q(dec.rd), dec.pc + dec.imm);
				}
			}
			return true;
		}

		bool emit_call(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tcall\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + dec.imm;
			int rdx = rv::x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				addr_t link_addr = dec.pc + inst_length(dec.inst);
				if (rdx > 0) {
					rv::as.mov(x86::gpq(rdx), Imm(link_addr));
					log_trace("\t\tmov %s, 0x%llx", rv::x86_reg_str_q(rdx), link_addr);
				} else {
					rv::as.mov(x86::rax, Imm(link_addr));
					rv::as.mov(rv::rbp_reg_q(dec.rd), x86::rax);
					log_trace("\t\tmov rax, 0x%llx",  link_addr);
					log_trace("\t\tmov %s, rax", rv::rbp_reg_str_q(dec.rd));
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
			Label l = rv::as.newLabel();
			labels[dec.pc] = l;
			rv::as.bind(l);
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
				case rv_op_jal: return emit_jal(dec);
				case rv_op_jalr: return emit_jalr(dec);
				case fusion_op_li: return emit_li(dec);
				case fusion_op_la: return emit_la(dec);
				case fusion_op_call: return emit_call(dec);
			}
			return false;
		}
	};

}

#endif
