//
//  jit-emitter-rv32.h
//

#ifndef rv_jit_emitter_rv32_h
#define rv_jit_emitter_rv32_h

namespace riscv {

	using namespace asmjit;

	template <typename P>
	struct jit_emitter_rv32
	{
		typedef P processor_type;
		typedef typename P::decode_type decode_type;

		#define proc_offset(member) offsetof(typename P::processor_type, member)

		P &proc;
		X86Assembler as;
		CodeHolder &code;
		mmu_ops ops, ops_wrap;
		TraceLookup lookup_trace_slow;
		TraceLookup lookup_trace_fast;
		std::map<addr_t,Label> labels;
		std::map<addr_t,Label> jmp_tramp_labels;
		std::map<addr_t,Label> exit_tramp_labels;
		std::map<addr_t,std::vector<Label>> jmp_fixup_labels;
		std::vector<addr_t> callstack;
		u32 term_pc;
		bool use_mmu;
		Label start, term;

		jit_emitter_rv32(P &proc, CodeHolder &code, mmu_ops &ops, TraceLookup lookup_trace_slow, TraceLookup lookup_trace_fast)
			: proc(proc), as(&code), code(code), ops(ops),
			  lookup_trace_slow(lookup_trace_slow),
			  lookup_trace_fast(lookup_trace_fast),
			  term_pc(0), use_mmu(false)
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

		void emit_prolog()
		{
			as.push(x86::r12);
			as.push(x86::r13);
			as.push(x86::r14);
			as.push(x86::r15);
			as.push(x86::rbx);
			as.push(x86::rbp);
			as.mov(x86::rbp, x86::rdi);
			as.mov(x86::edx, rbp_reg_d(rv_ireg_ra));
			as.mov(x86::ebx, rbp_reg_d(rv_ireg_sp));
			as.mov(x86::esi, rbp_reg_d(rv_ireg_t0));
			as.mov(x86::edi, rbp_reg_d(rv_ireg_t1));
			as.mov(x86::r8d,  rbp_reg_d(rv_ireg_a0));
			as.mov(x86::r9d,  rbp_reg_d(rv_ireg_a1));
			as.mov(x86::r10d, rbp_reg_d(rv_ireg_a2));
			as.mov(x86::r11d, rbp_reg_d(rv_ireg_a3));
			as.mov(x86::r12d, rbp_reg_d(rv_ireg_a4));
			as.mov(x86::r13d, rbp_reg_d(rv_ireg_a5));
			as.mov(x86::r14d, rbp_reg_d(rv_ireg_a6));
			as.mov(x86::r15d, rbp_reg_d(rv_ireg_a7));
		}

		void emit_epilog()
		{
			as.mov(rbp_reg_d(rv_ireg_ra), x86::edx);
			as.mov(rbp_reg_d(rv_ireg_sp), x86::ebx);
			as.mov(rbp_reg_d(rv_ireg_t0), x86::esi);
			as.mov(rbp_reg_d(rv_ireg_t1), x86::edi);
			as.mov(rbp_reg_d(rv_ireg_a0), x86::r8d);
			as.mov(rbp_reg_d(rv_ireg_a1), x86::r9d);
			as.mov(rbp_reg_d(rv_ireg_a2), x86::r10d);
			as.mov(rbp_reg_d(rv_ireg_a3), x86::r11d);
			as.mov(rbp_reg_d(rv_ireg_a4), x86::r12d);
			as.mov(rbp_reg_d(rv_ireg_a5), x86::r13d);
			as.mov(rbp_reg_d(rv_ireg_a6), x86::r14d);
			as.mov(rbp_reg_d(rv_ireg_a7), x86::r15d);
			as.pop(x86::rbp);
			as.pop(x86::rbx);
			as.pop(x86::r15);
			as.pop(x86::r14);
			as.pop(x86::r13);
			as.pop(x86::r12);
			as.ret();


			for (auto &jtl : jmp_tramp_labels) {
				as.bind(jtl.second);
				emit_pc(jtl.first);
				as.jmp(Imm(func_address(lookup_trace_fast)));
			}

			for (auto &jtl : exit_tramp_labels) {
				as.bind(jtl.second);
				emit_pc(jtl.first);
				as.jmp(term);
			}
		}

		TraceLookup create_trace_lookup(JitRuntime &rt)
		{
			auto lookup_slow = as.newLabel();
			auto lookup_fail = as.newLabel();

			u32 mask = (P::trace_l1_size - 1) << 1;

			/* fast path lookup cache pc -> trace fn */
			as.mov(x86::ecx, x86::dword_ptr(x86::rbp, proc_offset(pc)));
			as.mov(x86::eax, x86::ecx);
			as.and_(x86::ecx, Imm(mask));
			as.cmp(x86::rax, x86::qword_ptr(x86::rbp, x86::rcx, 4, proc_offset(trace_pc)));
			as.jne(lookup_slow);
			as.jmp(x86::qword_ptr(x86::rbp, x86::rcx, 4, proc_offset(trace_fn)));

			/* slow path lookup cache pc -> trace fn */
			as.bind(lookup_slow);
			as.mov(rbp_reg_d(rv_ireg_ra), x86::edx);
			as.mov(rbp_reg_d(rv_ireg_sp), x86::ebx);
			as.mov(rbp_reg_d(rv_ireg_t0), x86::esi);
			as.mov(rbp_reg_d(rv_ireg_t1), x86::edi);
			as.mov(rbp_reg_d(rv_ireg_a0), x86::r8d);
			as.mov(rbp_reg_d(rv_ireg_a1), x86::r9d);
			as.mov(rbp_reg_d(rv_ireg_a2), x86::r10d);
			as.mov(rbp_reg_d(rv_ireg_a3), x86::r11d);
			as.mov(x86::rdi, x86::rax);
			as.call(Imm(func_address(lookup_trace_slow)));
			as.test(x86::rax, x86::rax);
			as.jz(lookup_fail);
			as.mov(x86::ecx, x86::dword_ptr(x86::rbp, proc_offset(pc)));
			as.mov(x86::edx, x86::ecx);
			as.and_(x86::ecx, Imm(mask));
			as.mov(x86::qword_ptr(x86::rbp, x86::rcx, 4, proc_offset(trace_fn)), x86::rax);
			as.mov(x86::qword_ptr(x86::rbp, x86::rcx, 4, proc_offset(trace_pc)), x86::rdx);
			as.mov(x86::edx, rbp_reg_d(rv_ireg_ra));
			as.mov(x86::ebx, rbp_reg_d(rv_ireg_sp));
			as.mov(x86::esi, rbp_reg_d(rv_ireg_t0));
			as.mov(x86::edi, rbp_reg_d(rv_ireg_t1));
			as.mov(x86::r8d, rbp_reg_d(rv_ireg_a0));
			as.mov(x86::r9d, rbp_reg_d(rv_ireg_a1));
			as.mov(x86::r10d, rbp_reg_d(rv_ireg_a2));
			as.mov(x86::r11d, rbp_reg_d(rv_ireg_a3));
			as.jmp(x86::rax);

			/* fail path, return to emulator */
			as.bind(lookup_fail);
			as.mov(rbp_reg_d(rv_ireg_a4), x86::r12d);
			as.mov(rbp_reg_d(rv_ireg_a5), x86::r13d);
			as.mov(rbp_reg_d(rv_ireg_a6), x86::r14d);
			as.mov(rbp_reg_d(rv_ireg_a7), x86::r15d);
			as.pop(x86::rbp);
			as.pop(x86::rbx);
			as.pop(x86::r15);
			as.pop(x86::r14);
			as.pop(x86::r13);
			as.pop(x86::r12);
			as.ret();

			Error err = rt.add(&lookup_trace_fast, &code);
			if (err) panic("failed to create trace lookup function");
			return lookup_trace_fast;
		}

		void save_volatile()
		{
			as.mov(rbp_reg_d(rv_ireg_ra), x86::edx);
			as.mov(rbp_reg_d(rv_ireg_t0), x86::esi);
			as.mov(rbp_reg_d(rv_ireg_t1), x86::edi);
			as.mov(rbp_reg_d(rv_ireg_a0), x86::r8d);
			as.mov(rbp_reg_d(rv_ireg_a1), x86::r9d);
			as.mov(rbp_reg_d(rv_ireg_a2), x86::r10d);
			as.mov(rbp_reg_d(rv_ireg_a3), x86::r11d);
		}

		void restore_volatile()
		{
			as.mov(x86::edx, rbp_reg_d(rv_ireg_ra));
			as.mov(x86::esi, rbp_reg_d(rv_ireg_t0));
			as.mov(x86::edi, rbp_reg_d(rv_ireg_t1));
			as.mov(x86::r8d, rbp_reg_d(rv_ireg_a0));
			as.mov(x86::r9d, rbp_reg_d(rv_ireg_a1));
			as.mov(x86::r10d, rbp_reg_d(rv_ireg_a2));
			as.mov(x86::r11d, rbp_reg_d(rv_ireg_a3));
		}

		mmu_ops create_load_store(JitRuntime &rt)
		{
			Label lb = as.newLabel();
			as.align(kAlignCode, 16);
			as.bind(lb);
			save_volatile();
			as.mov(x86::rdi, x86::rax);
			as.call(Imm(func_address(ops.lb)));
			restore_volatile();
			as.ret();

			Label lh = as.newLabel();
			as.align(kAlignCode, 16);
			as.bind(lh);
			save_volatile();
			as.mov(x86::rdi, x86::rax);
			as.call(Imm(func_address(ops.lh)));
			restore_volatile();
			as.ret();

			Label lw = as.newLabel();
			as.align(kAlignCode, 16);
			as.bind(lw);
			save_volatile();
			as.mov(x86::rdi, x86::rax);
			as.call(Imm(func_address(ops.lw)));
			restore_volatile();
			as.ret();

			Label sb = as.newLabel();
			as.align(kAlignCode, 16);
			as.bind(sb);
			save_volatile();
			as.mov(x86::rdi, x86::rax);
			as.mov(x86::rsi, x86::rcx);
			as.call(Imm(func_address(ops.sb)));
			restore_volatile();
			as.ret();

			Label sh = as.newLabel();
			as.align(kAlignCode, 16);
			as.bind(sh);
			save_volatile();
			as.mov(x86::rdi, x86::rax);
			as.mov(x86::rsi, x86::rcx);
			as.call(Imm(func_address(ops.sh)));
			restore_volatile();
			as.ret();

			Label sw = as.newLabel();
			as.align(kAlignCode, 16);
			as.bind(sw);
			save_volatile();
			as.mov(x86::rdi, x86::rax);
			as.mov(x86::rsi, x86::rcx);
			as.call(Imm(func_address(ops.sw)));
			restore_volatile();
			as.ret();

			TraceFunc fn;
			Error err = rt.add(&fn, &code);
			if (err) panic("failed to load store functions");

			mmu_ops ops = {
				.lb = func_address_offset<lb_fn>(fn, code.getLabelOffset(lb)),
				.lh = func_address_offset<lh_fn>(fn, code.getLabelOffset(lh)),
				.lw = func_address_offset<lw_fn>(fn, code.getLabelOffset(lw)),
				.ld = nullptr,
				.sb = func_address_offset<sb_fn>(fn, code.getLabelOffset(sb)),
				.sh = func_address_offset<sh_fn>(fn, code.getLabelOffset(sh)),
				.sw = func_address_offset<sw_fn>(fn, code.getLabelOffset(sw)),
				.sd = nullptr
			};

			return ops;
		}

		void begin()
		{
			term = as.newLabel();
			start = as.newLabel();
			as.bind(start);
		}

		void end()
		{
			if (term_pc) {
				emit_pc(term_pc);
				log_trace("\t# 0x%016llx", term_pc);
			}
			as.bind(term);
		}

		void emit_pc(uintptr_t new_pc)
		{
			as.mov(x86::qword_ptr(x86::rbp, proc_offset(pc)), Imm(new_pc));
		}

		void emit_zero_rd(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd);
			if (rdx > 0) {
				as.xor_(x86::gpd(rdx), x86::gpd(rdx));
			} else {
				as.mov(rbp_reg_d(dec.rd), Imm(0));
			}
		}

		void emit_mv_eax_rs1(decode_type &dec)
		{
			int rs1x = x86_reg(dec.rs1);
			if (rs1x > 0) {
				as.mov(x86::eax, x86::gpd(rs1x));
			} else {
				as.mov(x86::eax, rbp_reg_d(dec.rs1));
			}
		}

		void emit_mv_rd_rs1(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (rdx > 0) {
				if (rs1x > 0) {
					as.mov(x86::gpd(rdx), x86::gpd(rs1x));
				} else {
					as.mov(x86::gpd(rdx), rbp_reg_d(dec.rs1));
				}
			} else {
				if (rs1x > 0) {
					as.mov(rbp_reg_d(dec.rd), x86::gpd(rs1x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
				}
			}
		}

		void emit_mv_rd_rs2(decode_type &dec)
		{
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (rdx > 0) {
				if (rs2x > 0) {
					as.mov(x86::gpd(rdx), x86::gpd(rs2x));
				} else {
					as.mov(x86::gpd(rdx), rbp_reg_d(dec.rs2));
				}
			} else {
				if (rs2x > 0) {
					as.mov(rbp_reg_d(dec.rd), x86::gpd(rs2x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs2));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
				}
			}
		}

		void emit_mv_cl_rs2(decode_type &dec)
		{
			int rs2x = x86_reg(dec.rs2);
			if (rs2x > 0) {
				as.mov(x86::ecx, x86::gpd(rs2x));
			} else {
				as.mov(x86::ecx, rbp_reg_d(dec.rs2));
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
					as.mov(x86::gpd(rdx), Imm(dec.pc + dec.imm));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(dec.pc + dec.imm));
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
				if (rdx > 0) {
					if (rs2x > 0) {
						as.mov(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.mov(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.mov(rbp_reg_d(dec.rd), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.mov(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.add(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(rbp_reg_d(dec.rd), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.add(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.add(x86::gpd(rdx), x86::gpd(rs1x));
					} else {
						as.add(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.add(rbp_reg_d(dec.rd), x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.add(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (rdx > 0 && rs1x > 0 && rs2x > 0) {
				as.lea(x86::gpd(rdx), x86::dword_ptr(x86::gpd(rs1x), x86::gpd(rs2x)));
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.add(x86::eax, x86::gpd(rs2x));
				} else {
					as.add(x86::eax, rbp_reg_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					if (rs2x > 0) {
						as.add(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.add(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.add(rbp_reg_d(dec.rd), x86::gpd(rs2x));
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
					emit_mv_rd_rs2(dec);
				}
				if (rdx > 0) {
					as.neg(x86::gpd(rdx));
				} else {
					as.neg(rbp_reg_d(dec.rd));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.sub(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(rbp_reg_d(dec.rd), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.sub(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.sub(x86::gpd(rdx), x86::gpd(rs1x));
					} else {
						as.sub(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.sub(rbp_reg_d(dec.rd), x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.sub(rbp_reg_d(dec.rd), x86::eax);
					}
				}
				if (rdx > 0) {
					as.neg(x86::gpd(rdx));
				} else {
					as.neg(rbp_reg_d(dec.rd));
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.sub(x86::eax, x86::gpd(rs2x));
				} else {
					as.sub(x86::eax, rbp_reg_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					if (rs2x > 0) {
						as.sub(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.sub(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.sub(rbp_reg_d(dec.rd), x86::gpd(rs2x));
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
				if (rs2x > 0) {
					as.imul(x86::gpd(rdx), x86::gpd(rs2x));
				} else {
					as.imul(x86::gpd(rdx), rbp_reg_d(dec.rs2));
				}
			}
			else if (dec.rd == dec.rs2 && rdx > 0) {
				if (rs1x > 0) {
					as.imul(x86::gpd(rdx), x86::gpd(rs1x));
				} else {
					as.imul(x86::gpd(rdx), rbp_reg_d(dec.rs1));
				}
			}
			else if (rdx < 0) {
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.imul(x86::eax, x86::gpd(rs2x));
				} else {
					as.imul(x86::eax, rbp_reg_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rs2x > 0) {
					as.imul(x86::gpd(rdx), x86::gpd(rs2x));
				} else {
					as.imul(x86::gpd(rdx), rbp_reg_d(dec.rs2));
				}
			}
			return true;
		}

		bool emit_mulh(decode_type &dec)
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
			else {
				if (rdx != 2 /* x86::edx */) {
					as.mov(x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])), x86::edx);
				}

				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.imul(x86::gpd(rs2x));
				} else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					as.imul(x86::ecx);
				}
				if (rdx > 0) {
					if (rdx != 2 /* x86::edx */) {
						as.mov(x86::gpd(rdx), x86::edx);
					}
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::edx);
				}

				if (rdx != 2 /* x86::edx */) {
					as.mov(x86::edx, x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
				}
			}
			return true;
		}

		bool emit_mulhu(decode_type &dec)
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
			else {
				if (rdx != 2 /* x86::edx */) {
					as.mov(x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])), x86::edx);
				}

				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.mul(x86::gpd(rs2x));
				} else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					as.mul(x86::ecx);
				}
				if (rdx > 0) {
					if (rdx != 2 /* x86::edx */) {
						as.mov(x86::gpd(rdx), x86::edx);
					}
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::edx);
				}

				if (rdx != 2 /* x86::edx */) {
					as.mov(x86::edx, x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
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
				if (rdx != 2 /* x86::edx */ || (rs1x == 2 /* x86::edx */ || rs2x == 2 /* x86::edx */)) {
					as.mov(x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])), x86::edx);
				}

				/* if rs1 is positive branch to umul */
				Label umul = as.newLabel();
				Label out = as.newLabel();
				emit_mv_eax_rs1(dec);
				as.test(x86::eax, x86::eax);
				as.jns(umul);
				as.neg(x86::eax);

				/* multiply, negate and stash result in ecx */
				if (rs2x > 0) {
					as.mul(x86::gpd(rs2x));
				} else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					as.mul(x86::ecx);
				}
				as.xor_(x86::edx, Imm(-1));
				as.mov(x86::ecx, x86::edx);

				/* if necessary restore rdx input operand */
				if (rs1x == 2 || rs2x == 2 /* x86::edx */) {
					as.mov(x86::edx, x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
				}

				/* second multiply */
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.imul(x86::eax, x86::gpd(rs2x));
				} else {
					as.imul(x86::eax, rbp_reg_d(dec.rs2));
				}

				/* handle zero special case */
				as.test(x86::eax, x86::eax);
				as.setz(x86::dl);
				as.movzx(x86::edx, x86::dl);
				as.lea(x86::edx, x86::dword_ptr(x86::edx, x86::ecx));
				as.jmp(out);

				/* unsigned multiply */
				as.bind(umul);
				if (rs2x > 0) {
					as.mul(x86::gpd(rs2x));
				} else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					as.mul(x86::ecx);
				}

				/* high result */
				as.bind(out);
				if (rdx > 0) {
					if (rdx != 2 /* x86::edx */) {
						as.mov(x86::gpd(rdx), x86::edx);
					}
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::edx);
				}

				if (rdx != 2 /* x86::edx */) {
					as.mov(x86::edx, x86::dword_ptr(x86::rbp, proc_offset(ireg[rv_ireg_ra])));
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
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(-1));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(-1));
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
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), x86::eax);
				} else {
					as.cmp(rbp_reg_d(dec.rs1), x86::eax);
				}
				as.jne(div1);

				if (rs2x > 0) {
					as.cmp(x86::gpd(rs2x), Imm(-1));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(-1));
				}
				as.je(out);

				as.bind(div1);
				if (rs2x > 0) {
					as.test(x86::gpd(rs2x), x86::gpd(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
				}
				as.jne(div2);
				as.mov(x86::eax, Imm(-1));
				as.jmp(out);

				as.bind(div2);
				emit_mv_eax_rs1(dec);

				if (rdx != 2 /* edx */) {
					as.mov(x86::ecx, x86::edx);
				}
				as.cqo();
				if (rs2x > 0) {
					as.idiv(x86::gpd(rs2x));
				} else {
					as.idiv(rbp_reg_d(dec.rs2));
				}
				if (rdx != 2 /* edx */) {
					as.mov(x86::edx, x86::ecx);
				}

				as.bind(out);
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), x86::eax);
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(-1));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(-1));
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* edx */) {
					as.mov(x86::ecx, x86::edx);
				}

				Label out = as.newLabel();
				Label div1 = as.newLabel();
				Label div2 = as.newLabel();

				as.mov(x86::eax, std::numeric_limits<int32_t>::min());
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), x86::eax);
				} else {
					as.cmp(rbp_reg_d(dec.rs1), x86::eax);
				}
				as.jne(div1);

				as.xor_(x86::edx, x86::edx);
				if (rs2x > 0) {
					as.cmp(x86::gpd(rs2x), Imm(-1));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(-1));
				}
				as.je(out);

				as.bind(div1);
				emit_mv_eax_rs1(dec);

				if (rs2x > 0) {
					as.test(x86::gpd(rs2x), x86::gpd(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
				}
				as.jne(div2);
				as.mov(x86::edx, x86::eax);
				as.jmp(out);

				as.bind(div2);
				as.cqo();
				if (rs2x > 0) {
					as.idiv(x86::gpd(rs2x));
				} else {
					as.idiv(rbp_reg_d(dec.rs2));
				}

				as.bind(out);
				if (rdx > 0) {
					if (rdx != 2 /* edx */) {
						as.mov(x86::gpd(rdx), x86::edx);
					}
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::edx);
				}

				if (rdx != 2 /* edx */) {
					as.mov(x86::edx, x86::ecx);
				}
			}
			return true;
		}

		bool emit_divu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(-1));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(-1));
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				Label out = as.newLabel();
				Label div1 = as.newLabel();

				if (rs2x > 0) {
					as.test(x86::gpd(rs2x), x86::gpd(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
				}
				as.jne(div1);
				as.mov(x86::eax, Imm(-1));
				as.jmp(out);

				as.bind(div1);
				emit_mv_eax_rs1(dec);

				if (rdx != 2 /* edx */) {
					as.mov(x86::ecx, x86::edx);
				}
				as.xor_(x86::edx, x86::edx);
				if (rs2x > 0) {
					as.div(x86::gpd(rs2x));
				} else {
					as.div(rbp_reg_d(dec.rs2));
				}
				if (rdx != 2 /* edx */) {
					as.mov(x86::edx, x86::ecx);
				}

				as.bind(out);
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), x86::eax);
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::eax);
				}
			}
			return true;
		}

		bool emit_remu(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			term_pc = dec.pc + inst_length(dec.inst);
			int rdx = x86_reg(dec.rd), rs2x = x86_reg(dec.rs2);
			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(-1));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(-1));
				}
			}
			else if (dec.rs1 == rv_ireg_zero) {
				emit_zero_rd(dec);
			}
			else {
				if (rdx != 2 /* edx */) {
					as.mov(x86::ecx, x86::edx);
				}

				Label out = as.newLabel();
				Label div1 = as.newLabel();

				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.test(x86::gpd(rs2x), x86::gpd(rs2x));
				} else {
					as.cmp(rbp_reg_d(dec.rs2), Imm(0));
				}
				as.jne(div1);
				as.mov(x86::edx, x86::eax);
				as.jmp(out);

				as.bind(div1);
				as.xor_(x86::edx, x86::edx);
				if (rs2x > 0) {
					as.div(x86::gpd(rs2x));
				} else {
					as.div(rbp_reg_d(dec.rs2));
				}

				as.bind(out);
				if (rdx > 0) {
					if (rdx != 2 /* edx */) {
						as.mov(x86::gpd(rdx), x86::edx);
					}
				} else {
					as.mov(rbp_reg_d(dec.rd), x86::edx);
				}

				if (rdx != 2 /* edx */) {
					as.mov(x86::edx, x86::ecx);
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
				if (rs1x > 0 && rs2x > 0) {
					as.cmp(x86::gpd(rs1x), x86::gpd(rs2x));
				}
				else {
					if (dec.rs1 == rv_ireg_zero) {
						as.xor_(x86::eax, x86::eax);
					} else if (rs1x > 0) {
						as.mov(x86::eax, x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
					}
					if (dec.rs2 == rv_ireg_zero) {
						as.cmp(x86::eax, Imm(0));
					} else if (rs2x > 0) {
						as.cmp(x86::eax, x86::gpd(rs2x));
					} else {
						as.cmp(x86::eax, rbp_reg_d(dec.rs2));
					}
				}
				as.setl(x86::al);
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (rs1x > 0 && rs2x > 0) {
					as.cmp(x86::gpd(rs1x), x86::gpd(rs2x));
				}
				else {
					if (dec.rs1 == rv_ireg_zero) {
						as.xor_(x86::eax, x86::eax);
					} else if (rs1x > 0) {
						as.mov(x86::eax, x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
					}
					if (dec.rs2 == rv_ireg_zero) {
						as.cmp(x86::eax, Imm(0));
					} else if (rs2x > 0) {
						as.cmp(x86::eax, x86::gpd(rs2x));
					} else {
						as.cmp(x86::eax, rbp_reg_d(dec.rs2));
					}
				}
				as.setb(x86::al);
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (rdx > 0) {
					if (rs2x > 0) {
						as.and_(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.and_(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.and_(rbp_reg_d(dec.rd), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.and_(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.and_(x86::gpd(rdx), x86::gpd(rs1x));
					} else {
						as.and_(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.and_(rbp_reg_d(dec.rd), x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.and_(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.and_(x86::eax, x86::gpd(rs2x));
				} else {
					as.and_(x86::eax, rbp_reg_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					if (rs2x > 0) {
						as.and_(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.and_(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.and_(rbp_reg_d(dec.rd), x86::gpd(rs2x));
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
				emit_mv_rd_rs2(dec);
			}
			else if (dec.rs2 == rv_ireg_zero) {
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					if (rs2x > 0) {
						as.or_(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.or_(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.or_(rbp_reg_d(dec.rd), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.or_(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.or_(x86::gpd(rdx), x86::gpd(rs1x));
					} else {
						as.or_(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.or_(rbp_reg_d(dec.rd), x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.or_(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.or_(x86::eax, x86::gpd(rs2x));
				} else {
					as.or_(x86::eax, rbp_reg_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					if (rs2x > 0) {
						as.or_(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.or_(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.or_(rbp_reg_d(dec.rd), x86::gpd(rs2x));
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
				emit_mv_rd_rs2(dec);
			}
			else if (dec.rs2 == rv_ireg_zero) {
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					if (rs2x > 0) {
						as.xor_(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.xor_(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.xor_(rbp_reg_d(dec.rd), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs2));
						as.xor_(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (dec.rd == dec.rs2) {
				if (rdx > 0) {
					if (rs1x > 0) {
						as.xor_(x86::gpd(rdx), x86::gpd(rs1x));
					} else {
						as.xor_(x86::gpd(rdx), rbp_reg_d(dec.rs1));
					}
				} else {
					if (rs1x > 0) {
						as.xor_(rbp_reg_d(dec.rd), x86::gpd(rs1x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.xor_(rbp_reg_d(dec.rd), x86::eax);
					}
				}
			}
			else if (rdx < 0 && (rs1x < 0 || rs2x < 0)) {
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.xor_(x86::eax, x86::gpd(rs2x));
				} else {
					as.xor_(x86::eax, rbp_reg_d(dec.rs2));
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					if (rs2x > 0) {
						as.xor_(x86::gpd(rdx), x86::gpd(rs2x));
					} else {
						as.xor_(x86::gpd(rdx), rbp_reg_d(dec.rs2));
					}
				} else {
					if (rs2x > 0) {
						as.xor_(rbp_reg_d(dec.rd), x86::gpd(rs2x));
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
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				emit_mv_cl_rs2(dec);
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), x86::cl);
				} else {
					as.shl(rbp_reg_d(dec.rd), x86::cl);
				}
			}
			else if (rdx < 0) {
				emit_mv_cl_rs2(dec);
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.shl(x86::eax, x86::cl);
				} else {
					as.shl(x86::eax, x86::cl);
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_cl_rs2(dec);
				emit_mv_rd_rs1(dec);
				if (rs2x > 0) {
					as.shl(x86::gpd(rdx), x86::cl);
				} else {
					as.shl(x86::gpd(rdx), x86::cl);
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
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				emit_mv_cl_rs2(dec);
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), x86::cl);
				} else {
					as.shr(rbp_reg_d(dec.rd), x86::cl);
				}
			}
			else if (rdx < 0) {
				emit_mv_cl_rs2(dec);
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.shr(x86::eax, x86::cl);
				} else {
					as.shr(x86::eax, x86::cl);
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_cl_rs2(dec);
				emit_mv_rd_rs1(dec);
				if (rs2x > 0) {
					as.shr(x86::gpd(rdx), x86::cl);
				} else {
					as.shr(x86::gpd(rdx), x86::cl);
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
				emit_mv_rd_rs1(dec);
			}
			else if (dec.rd == dec.rs1) {
				emit_mv_cl_rs2(dec);
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), x86::cl);
				} else {
					as.sar(rbp_reg_d(dec.rd), x86::cl);
				}
			}
			else if (rdx < 0) {
				emit_mv_cl_rs2(dec);
				emit_mv_eax_rs1(dec);
				if (rs2x > 0) {
					as.sar(x86::eax, x86::cl);
				} else {
					as.sar(x86::eax, x86::cl);
				}
				as.mov(rbp_reg_d(dec.rd), x86::eax);
			}
			else {
				emit_mv_cl_rs2(dec);
				emit_mv_rd_rs1(dec);
				if (rs2x > 0) {
					as.sar(x86::gpd(rdx), x86::cl);
				} else {
					as.sar(x86::gpd(rdx), x86::cl);
				}
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
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.add(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else if (dec.imm == 0) {
				emit_mv_rd_rs1(dec);
			}
			else if (rdx > 0 && rs1x > 0) {
				as.lea(x86::gpd(rdx), x86::dword_ptr(x86::gpd(rs1x), int32_t(dec.imm)));
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.add(rbp_reg_d(dec.rd), Imm(dec.imm));
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
					as.mov(x86::gpd(rdx), Imm(result));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(result));
				}
			}
			else {
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), Imm(dec.imm));
				} else {
					as.cmp(rbp_reg_d(dec.rs1), Imm(dec.imm));
				}
				as.setl(x86::al);
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
					as.mov(x86::gpd(rdx), Imm(result));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(result));
				}
			}
			else {
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), Imm(dec.imm));
				} else {
					as.cmp(rbp_reg_d(dec.rs1), Imm(dec.imm));
				}
				as.setb(x86::al);
				if (rdx > 0) {
					as.movzx(x86::gpd(rdx), x86::al);
				} else {
					as.movzx(x86::eax, x86::al);
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (rdx > 0) {
					as.and_(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.and_(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.and_(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.and_(rbp_reg_d(dec.rd), Imm(dec.imm));
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
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					as.or_(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.or_(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.or_(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.or_(rbp_reg_d(dec.rd), Imm(dec.imm));
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
				if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else if (dec.rd == dec.rs1) {
				if (rdx > 0) {
					as.xor_(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.xor_(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.xor_(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.xor_(rbp_reg_d(dec.rd), Imm(dec.imm));
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
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.shl(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.shl(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.shl(rbp_reg_d(dec.rd), Imm(dec.imm));
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
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.shr(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.shr(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.shr(rbp_reg_d(dec.rd), Imm(dec.imm));
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
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.sar(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			else {
				emit_mv_rd_rs1(dec);
				if (rdx > 0) {
					as.sar(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.sar(rbp_reg_d(dec.rd), Imm(dec.imm));
				}
			}
			return true;
		}

		void emit_cmp(decode_type &dec)
		{
			int rs1x = x86_reg(dec.rs1), rs2x = x86_reg(dec.rs2);
			if (dec.rs1 == rv_ireg_zero) {
				as.xor_(x86::eax, x86::eax);
				if (rs2x > 0) {
					as.cmp(x86::eax, x86::gpd(rs2x));
				} else {
					as.cmp(x86::eax, rbp_reg_d(dec.rs2));
				}
			}
			else if (dec.rs2 == rv_ireg_zero) {
				if (rs1x > 0) {
					as.cmp(x86::gpd(rs1x), Imm(0));
				} else {
					as.cmp(rbp_reg_d(dec.rs1), Imm(0));
				}
			}
			else if (rs1x > 0) {
				if (rs2x > 0) {
					as.cmp(x86::gpd(rs1x), x86::gpd(rs2x));
				} else {
					as.cmp(x86::gpd(rs1x), rbp_reg_d(dec.rs2));
				}
			}
			else {
				if (rs2x > 0) {
					as.cmp(rbp_reg_d(dec.rs1), x86::gpd(rs2x));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.cmp(x86::eax, rbp_reg_d(dec.rs2));
				}
			}
		}

		inline auto create_exit_tramp(addr_t pc)
		{
			auto jtl = exit_tramp_labels.find(pc);
			if (jtl == exit_tramp_labels.end()) {
				jtl = exit_tramp_labels.insert(exit_tramp_labels.end(),
					std::pair<addr_t,Label>(pc, as.newLabel()));
			}
			return jtl;
		}

		inline auto create_jump_tramp(addr_t pc)
		{
			auto jtl = jmp_tramp_labels.find(pc);
			if (jtl == jmp_tramp_labels.end()) {
				jtl = jmp_tramp_labels.insert(jmp_tramp_labels.end(),
					std::pair<addr_t,Label>(pc, as.newLabel()));
			}
			return jtl;
		}

		inline auto create_jump_fixup(addr_t pc)
		{
			auto jfl = jmp_fixup_labels.find(pc);
			if (jfl == jmp_fixup_labels.end()) {
				jfl = jmp_fixup_labels.insert(jmp_fixup_labels.end(),
					std::pair<addr_t,std::vector<Label>>(pc, std::vector<Label>()));
			}
			return jfl;
		}

		void emit_jump_fixup(addr_t pc)
		{
			auto jtl = create_jump_tramp(pc);
			auto jfl = create_jump_fixup(pc);
			as.jmp(jtl->second);
			Label label = as.newLabel();
			as.bind(label);
			jfl->second.push_back(label);
		}

		void emit_branch_fixup(x86::Cond bf, addr_t pc)
		{
			auto jtl = create_jump_tramp(pc);
			auto jfl = create_jump_fixup(pc);
			as.j(bf, jtl->second);
			Label label = as.newLabel();
			as.bind(label);
			jfl->second.push_back(label);
		}

		bool emit_branch(decode_type &dec, bool cond, x86::Cond bf, x86::Cond ibf)
		{
			addr_t branch_pc = dec.pc + dec.imm;
			addr_t cont_pc = dec.pc + inst_length(dec.inst);
			auto branch_i = labels.find(branch_pc);
			auto cont_i = labels.find(cont_pc);

			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			emit_cmp(dec);

			if (branch_i != labels.end() && cont_i != labels.end()) {
				as.j(bf, branch_i->second);
				as.jmp(cont_i->second);
			}
			else if (cond && branch_i != labels.end()) {
				as.j(bf, branch_i->second);
				uintptr_t cont_addr = lookup_trace_slow(cont_pc);
				if (cont_addr) {
					as.jmp(Imm(cont_addr));
				} else {
					emit_jump_fixup(cont_pc);
				}
				term_pc = 0;
			}
			else if (!cond && cont_i != labels.end()) {
				as.j(ibf, cont_i->second);
				uintptr_t branch_addr = lookup_trace_slow(branch_pc);
				if (branch_addr) {
					as.jmp(Imm(branch_addr));
				} else {
					emit_jump_fixup(branch_pc);
				}
				term_pc = 0;
			} else if (cond) {
				uintptr_t cont_addr = lookup_trace_slow(cont_pc);
				if (cont_addr) {
					as.j(ibf, Imm(cont_addr));
				} else {
					emit_branch_fixup(ibf, cont_pc);
				}
				term_pc = branch_pc;
			} else {
				uintptr_t branch_addr = lookup_trace_slow(branch_pc);
				if (branch_addr) {
					as.j(bf, Imm(branch_addr));
				} else {
					emit_branch_fixup(bf, branch_pc);
				}
				term_pc = cont_pc;
			}
			return true;
		}

		bool emit_bne(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, x86::kCondNE, x86::kCondE);
		}

		bool emit_beq(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, x86::kCondE, x86::kCondNE);
		}

		bool emit_blt(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, x86::kCondL, x86::kCondGE);
		}

		bool emit_bge(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val;
			return emit_branch(dec, cond, x86::kCondGE, x86::kCondL);
		}

		bool emit_bltu(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			return emit_branch(dec, cond, x86::kCondB, x86::kCondAE);
		}

		bool emit_bgeu(decode_type &dec)
		{
			bool cond = proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val;
			return emit_branch(dec, cond, x86::kCondAE, x86::kCondB);
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
				if (use_mmu) {
					if (dec.rs1 == rv_ireg_zero) {
						as.mov(x86::rax, Imm(dec.imm));
					}
					else if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					}
					else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					as.call(Imm(func_address(ops.lw)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
					if (rdx > 0) {
						as.mov(x86::gpd(rdx), x86::eax);
					} else {
						as.mov(rbp_reg_d(dec.rd), x86::eax);
					}
				}
				else if (rdx > 0) {
					if (rs1x > 0) {
						as.movsxd(x86::gpd(rdx), x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movsxd(x86::gpd(rdx), x86::dword_ptr(x86::eax, dec.imm));
					}
				} else {
					if (rs1x > 0) {
						as.movsxd(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movsxd(x86::eax, x86::dword_ptr(x86::eax, dec.imm));
					}
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (use_mmu) {
					if (dec.rs1 == rv_ireg_zero) {
						as.mov(x86::rax, Imm(dec.imm));
					}
					else if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					}
					else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					as.call(Imm(func_address(ops.lh)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
					if (rdx > 0) {
						as.movsx(x86::gpd(rdx), x86::ax);
					} else {
						as.movsx(x86::eax, x86::ax);
						as.mov(rbp_reg_d(dec.rd), x86::eax);
					}
				}
				else if (rdx > 0) {
					if (rs1x > 0) {
						as.movsx(x86::gpd(rdx), x86::word_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movsx(x86::gpd(rdx), x86::word_ptr(x86::eax, dec.imm));
					}
				} else {
					if (rs1x > 0) {
						as.movsx(x86::eax, x86::word_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movsx(x86::eax, x86::word_ptr(x86::eax, dec.imm));
					}
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (use_mmu) {
					if (dec.rs1 == rv_ireg_zero) {
						as.mov(x86::rax, Imm(dec.imm));
					}
					else if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					}
					else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					as.call(Imm(func_address(ops.lh)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
					if (rdx > 0) {
						as.movzx(x86::gpd(rdx), x86::ax);
					} else {
						as.movzx(x86::eax, x86::ax);
						as.mov(rbp_reg_d(dec.rd), x86::eax);
					}
				}
				else if (rdx > 0) {
					if (rs1x > 0) {
						as.movzx(x86::gpd(rdx), x86::word_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movzx(x86::gpd(rdx), x86::word_ptr(x86::eax, dec.imm));
					}
				} else {
					if (rs1x > 0) {
						as.movzx(x86::eax, x86::word_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movzx(x86::eax, x86::word_ptr(x86::eax, dec.imm));
					}
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (use_mmu) {
					if (dec.rs1 == rv_ireg_zero) {
						as.mov(x86::rax, Imm(dec.imm));
					}
					else if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					}
					else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					as.call(Imm(func_address(ops.lb)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
					if (rdx > 0) {
						as.movsx(x86::gpd(rdx), x86::al);
					} else {
						as.movsx(x86::eax, x86::al);
						as.mov(rbp_reg_d(dec.rd), x86::eax);
					}
				}
				else if (rdx > 0) {
					if (rs1x > 0) {
						as.movsx(x86::gpd(rdx), x86::byte_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movsx(x86::gpd(rdx), x86::byte_ptr(x86::eax, dec.imm));
					}
				} else {
					if (rs1x > 0) {
						as.movsx(x86::eax, x86::byte_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movsx(x86::eax, x86::byte_ptr(x86::eax, dec.imm));
					}
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (use_mmu) {
					if (dec.rs1 == rv_ireg_zero) {
						as.mov(x86::rax, Imm(dec.imm));
					}
					else if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					}
					else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					as.call(Imm(func_address(ops.lb)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
					if (rdx > 0) {
						as.movzx(x86::gpd(rdx), x86::al);
					} else {
						as.movzx(x86::eax, x86::al);
						as.mov(rbp_reg_d(dec.rd), x86::eax);
					}
				}
				else if (rdx > 0) {
					if (rs1x > 0) {
						as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movzx(x86::gpd(rdx), x86::byte_ptr(x86::eax, dec.imm));
					}
				} else {
					if (rs1x > 0) {
						as.movzx(x86::eax, x86::byte_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.movzx(x86::eax, x86::byte_ptr(x86::eax, dec.imm));
					}
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				if (use_mmu) {
					if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::ecx, dec.imm));
					}
					as.xor_(x86::ecx, x86::ecx);
					as.call(Imm(func_address(ops.sw)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
				}
				else if (rs1x > 0) {
					as.mov(x86::dword_ptr(x86::gpd(rs1x), dec.imm), Imm(0));
				}
				else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.mov(x86::dword_ptr(x86::eax, dec.imm), Imm(0));
				}
			}
			else {
				if (use_mmu) {
					if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					}
					as.call(Imm(func_address(ops.sw)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
				}
				else if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::dword_ptr(x86::gpd(rs1x), dec.imm), x86::gpd(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.mov(x86::dword_ptr(x86::eax, dec.imm), x86::gpd(rs2x));
					}
				}
				else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::dword_ptr(x86::gpd(rs1x), dec.imm), x86::ecx);
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.mov(x86::dword_ptr(x86::eax, dec.imm), x86::ecx);
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
				if (use_mmu) {
					if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::ecx, dec.imm));
					}
					as.xor_(x86::ecx, x86::ecx);
					as.call(Imm(func_address(ops.sh)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
				}
				else if (rs1x > 0) {
					as.mov(x86::word_ptr(x86::gpd(rs1x), dec.imm), Imm(0));
				}
				else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.mov(x86::word_ptr(x86::eax, dec.imm), Imm(0));
				}
			}
			else {
				if (use_mmu) {
					if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					}
					as.call(Imm(func_address(ops.sh)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
				}
				else if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::word_ptr(x86::gpd(rs1x), dec.imm), x86::gpw(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.mov(x86::word_ptr(x86::eax, dec.imm), x86::gpw(rs2x));
					}
				}
				else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::word_ptr(x86::gpd(rs1x), dec.imm), x86::cx);
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.mov(x86::word_ptr(x86::eax, dec.imm), x86::cx);
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
				if (use_mmu) {
					if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::ecx, dec.imm));
					}
					as.xor_(x86::ecx, x86::ecx);
					as.call(Imm(func_address(ops.sb)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
				}
				else if (rs1x > 0) {
					as.mov(x86::byte_ptr(x86::gpd(rs1x), dec.imm), Imm(0));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.mov(x86::byte_ptr(x86::eax, dec.imm), Imm(0));
				}
			}
			else {
				if (use_mmu) {
					if (rs1x > 0) {
						as.lea(x86::eax, x86::dword_ptr(x86::gpq(rs1x), dec.imm));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs1));
						as.lea(x86::eax, x86::dword_ptr(x86::rcx, dec.imm));
					}
					if (rs2x > 0) {
						as.mov(x86::ecx, x86::gpd(rs2x));
					} else {
						as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					}
					as.call(Imm(func_address(ops.sb)));
					auto okay = as.newLabel();
					as.cmp(x86::dword_ptr(x86::rbp, proc_offset(cause)), Imm(0));
					as.je(okay);
					emit_pc(dec.pc);
					as.jmp(term);
					as.bind(okay);
				}
				else if (rs2x > 0) {
					if (rs1x > 0) {
						as.mov(x86::byte_ptr(x86::gpd(rs1x), dec.imm), x86::gpb_lo(rs2x));
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.mov(x86::byte_ptr(x86::eax, dec.imm), x86::gpb_lo(rs2x));
					}
				}
				else {
					as.mov(x86::ecx, rbp_reg_d(dec.rs2));
					if (rs1x > 0) {
						as.mov(x86::byte_ptr(x86::gpd(rs1x), dec.imm), x86::cl);
					} else {
						as.mov(x86::eax, rbp_reg_d(dec.rs1));
						as.mov(x86::byte_ptr(x86::eax, dec.imm), x86::cl);
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
					as.mov(x86::gpd(rdx), (int)dec.imm);
				} else {
					as.mov(rbp_reg_d(dec.rd), (int)dec.imm);
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
					as.mov(x86::gpd(rdx), Imm(link_addr));
				} else {
					as.mov(x86::eax, Imm(link_addr));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
				}
			}
			return true;
		}

		bool emit_jalr(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\t%s", dec.pc, disasm_inst_simple(dec).c_str());
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);
			if (dec.rd == rv_ireg_zero && dec.rs1 == rv_ireg_ra && callstack.size() > 0) {
				term_pc = 0;
				addr_t link_addr = callstack.back();
				callstack.pop_back();

				auto etl = create_exit_tramp(dec.pc);
				as.cmp(x86::gpq(x86_reg(rv_ireg_ra)), Imm(link_addr));
				as.jne(etl->second);

				return true;
			} else {
				term_pc = 0;
				addr_t link_addr = dec.pc + inst_length(dec.inst);

				if (dec.rs1 == rv_ireg_zero) {
					if (dec.imm == 0) {
						as.xor_(x86::eax, x86::eax);
					} else {
						as.mov(x86::rax, Imm(dec.imm));
					}
				} else if (rs1x > 0) {
					if (dec.imm == 0) {
						as.mov(x86::dword_ptr(x86::rbp, proc_offset(pc)), x86::gpd(rs1x));
					} else {
						as.lea(x86::eax, x86::dword_ptr(x86::gpd(rs1x), dec.imm));
						as.mov(x86::dword_ptr(x86::rbp, proc_offset(pc)), x86::eax);
					}
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rs1));
					as.add(x86::eax, dec.imm);
					as.mov(x86::dword_ptr(x86::rbp, proc_offset(pc)), x86::eax);
				}

				if (dec.rd == rv_ireg_zero) {
					// ret
				}
				else if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(link_addr));
				} else {
					as.mov(x86::eax, Imm(link_addr));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
				}

				as.jmp(Imm(func_address(lookup_trace_fast)));

				return false;
			}
		}

		bool emit_la(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tla\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + inst_length(dec.inst) + 4; /* auipc */;
			int rdx = x86_reg(dec.rd);
			if (dec.rd == rv_ireg_zero) {
				// nop
			} else {
				int32_t addr = dec.pc + dec.imm;
				if (dec.rd == rv_ireg_zero) {
					// nop
				} else if (rdx > 0) {
					as.mov(x86::gpd(rdx), Imm(addr));
				} else {
					as.mov(rbp_reg_d(dec.rd), Imm(addr));
				}
			}
			return true;
		}

		bool emit_call(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tcall\t%s, 0x%llx", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + dec.imm;
			int rdx = x86_reg(rv_ireg_ra), rs1x = x86_reg(dec.rd);
			addr_t link_addr = dec.pc + inst_length(dec.inst) + 4; /* auipc */;
			callstack.push_back(link_addr);

			if (dec.rd == rv_ireg_ra) {
				// nop
			} else if (rs1x > 0) {
				as.mov(x86::gpd(rs1x), Imm(term_pc));
			} else {
				as.mov(rbp_reg_d(dec.rs1), Imm(term_pc));
			}

			if (rdx > 0) {
				as.mov(x86::gpd(rdx), Imm(link_addr));
			} else {
				as.mov(rbp_reg_d(dec.rd), Imm(link_addr));
			}

			return true;
		}

		bool emit_zextw(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\tzext.w\t%s, %s", dec.pc, rv_ireg_name_sym[dec.rd], rv_ireg_name_sym[dec.rs1]);
			term_pc = dec.pc + dec.sz;
			int rdx = x86_reg(dec.rd), rs1x = x86_reg(dec.rs1);

			if (dec.rd == dec.rs1) {
				//
			}
			else {
				if (rdx > 0 && rs1x > 0) {
					as.mov(x86::gpd(rdx), x86::gpd(rs1x));
				} else if (rdx > 0) {
					as.mov(x86::gpd(rdx), rbp_reg_d(dec.rs1));
				} else if (rs1x > 0) {
					as.mov(rbp_reg_d(dec.rd), x86::gpd(rs1x));
				} else {
					as.movzx(x86::eax, rbp_reg_d(dec.rs1));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
				}
			}

			return true;
		}

		bool emit_addiwz(decode_type &dec)
		{
			log_trace("\t# 0x%016llx\taddiw.z\t%s, %d", dec.pc, rv_ireg_name_sym[dec.rd], dec.imm);
			term_pc = dec.pc + dec.sz;
			int rdx = x86_reg(dec.rd);

			if (dec.rd == rv_ireg_zero) {
				// nop
			}
			else {
				if (rdx > 0) {
					as.add(x86::gpd(rdx), Imm(dec.imm));
				} else {
					as.mov(x86::eax, rbp_reg_d(dec.rd));
					as.add(x86::eax, Imm(dec.imm));
					as.mov(rbp_reg_d(dec.rd), x86::eax);
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
				case rv_op_addi: return emit_addi(dec);
				case rv_op_slti: return emit_slti(dec);
				case rv_op_sltiu: return emit_sltiu(dec);
				case rv_op_andi: return emit_andi(dec);
				case rv_op_ori: return emit_ori(dec);
				case rv_op_xori: return emit_xori(dec);
				case rv_op_slli: return emit_slli(dec);
				case rv_op_srli: return emit_srli(dec);
				case rv_op_srai: return emit_srai(dec);
				case rv_op_bne: return emit_bne(dec);
				case rv_op_beq: return emit_beq(dec);
				case rv_op_blt: return emit_blt(dec);
				case rv_op_bge: return emit_bge(dec);
				case rv_op_bltu: return emit_bltu(dec);
				case rv_op_bgeu: return emit_bgeu(dec);
				case rv_op_lw: return emit_lw(dec);
				case rv_op_lh: return emit_lh(dec);
				case rv_op_lhu: return emit_lhu(dec);
				case rv_op_lb: return emit_lb(dec);
				case rv_op_lbu: return emit_lbu(dec);
				case rv_op_sw: return emit_sw(dec);
				case rv_op_sh: return emit_sh(dec);
				case rv_op_sb: return emit_sb(dec);
				case rv_op_lui: return emit_lui(dec);
				case rv_op_jal: return emit_jal(dec);
				case rv_op_jalr: return emit_jalr(dec);
				case jit_op_la: return emit_la(dec);
				case jit_op_call: return emit_call(dec);
				case jit_op_zextw: return emit_zextw(dec);
				case jit_op_addiwz: return emit_addiwz(dec);
			}
			return false;
		}
	};

}

#endif
