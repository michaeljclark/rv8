//
//  riscv-disasm.h
//

#ifndef riscv_disasm_h
#define riscv_disasm_h

namespace riscv {

	struct disasm : decode
	{
		uintptr_t pc;
		uint64_t  inst;

		disasm() : decode(), pc(0), inst(0) {}
	};

	enum rva {
		rva_none,
		rva_abs,
		rva_pcrel
	};

	struct rvx {
		riscv_op op1;
		riscv_op op2;
		rva addr;
	};

	// instruction pair constraints
	const rvx rvx_constraints[] = {
		{ riscv_op_lui,     riscv_op_addi,     rva_abs   },
		{ riscv_op_auipc,   riscv_op_addi,     rva_pcrel },
		{ riscv_op_auipc,   riscv_op_jalr,     rva_pcrel },
		{ riscv_op_auipc,   riscv_op_ld,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_lb,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_lh,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_lw,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_lbu,      rva_pcrel },
		{ riscv_op_auipc,   riscv_op_lhu,      rva_pcrel },
		{ riscv_op_auipc,   riscv_op_lwu,      rva_pcrel },
		{ riscv_op_auipc,   riscv_op_flw,      rva_pcrel },
		{ riscv_op_auipc,   riscv_op_fld,      rva_pcrel },
		{ riscv_op_auipc,   riscv_op_sd,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_sb,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_sh,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_sw,       rva_pcrel },
		{ riscv_op_auipc,   riscv_op_fsw,      rva_pcrel },
		{ riscv_op_auipc,   riscv_op_fsd,      rva_pcrel },
		{ riscv_op_illegal, riscv_op_illegal,  rva_none  },
	};

	// instruction buffer length
	const size_t rvx_instruction_buffer_len = 16;

	// decode pc relative address
	template <typename T>
	bool decode_pcrel(T &dec, uint64_t &addr, uintptr_t pc, uintptr_t pc_offset)
	{
		switch (dec.codec) {
			case riscv_codec_uj:
			case riscv_codec_sb:
				addr = pc - pc_offset + dec.imm;
				return true;
			default:
				return false;
		}
		return false;
	}

	// decode address using instruction pair constraints
	template <typename T>
	bool decode_pairs(T &dec, uint64_t &addr, std::deque<T> &dec_hist, uintptr_t pc_offset)
	{
		const rvx* rvxi = rvx_constraints;
		while(rvxi->addr != rva_none) {
			if (rvxi->op2 == dec.op) {
				for (auto li = dec_hist.rbegin(); li != dec_hist.rend(); li++) {
					if (rvxi->op1 != li->op && dec.rs1 == li->rd) break; // break: another primitive encountered
					if (rvxi->op1 != li->op || dec.rs1 != li->rd) continue; // continue: not the right pair
					switch (rvxi->addr) {
						case rva_abs:
							addr = li->imm + dec.imm;
							return true;
						case rva_pcrel:
							addr = li->pc - pc_offset + li->imm + dec.imm;
							return true;
						case rva_none:
						default:
							continue;
					}
					break;
				}
			}
			rvxi++;
		}
		return false;
	}

	// decode address for loads and stores from the global pointer
	template <typename T>
	bool deocde_gprel(T &dec, uint64_t &addr, uintptr_t gp)
	{
		if (!gp || dec.rs1 != riscv_ireg_gp) return false;
		switch (dec.op) {
			case riscv_op_addi:
			case riscv_op_lb:
			case riscv_op_lh:
			case riscv_op_lw:
			case riscv_op_ld:
			case riscv_op_lbu:
			case riscv_op_lhu:
			case riscv_op_lwu:
			case riscv_op_flw:
			case riscv_op_fld:
			case riscv_op_sb:
			case riscv_op_sh:
			case riscv_op_sw:
			case riscv_op_sd:
			case riscv_op_fsw:
			case riscv_op_fsd:
				addr = int64_t(gp + dec.imm);
				return true;
			default:
				break;
		}
		return false;
	}

	typedef std::function<const char*(uintptr_t, bool nearest)> symbol_name_fn;
	typedef std::function<const char*(const char *type)> symbol_colorize_fn;

	const char* null_symbol_lookup(uintptr_t, bool nearest);
	const char* null_symbol_colorize(const char *type);

	template <typename T>
	std::string disasm_inst_simple(T &dec)
	{
		std::string args;
		const char *fmt = riscv_inst_format[dec.op];
		while (*fmt) {
			switch (*fmt) {
				case 'O': args += riscv_inst_name_sym[dec.op]; break;
				case '(': args += "("; break;
				case ',': args += ", "; break;
				case ')': args += ")"; break;
				case '0': args += riscv_ireg_name_sym[dec.rd]; break;
				case '1': args += riscv_ireg_name_sym[dec.rs1]; break;
				case '2': args += riscv_ireg_name_sym[dec.rs2]; break;
				case '3': args += riscv_freg_name_sym[dec.rd]; break;
				case '4': args += riscv_freg_name_sym[dec.rs1]; break;
				case '5': args += riscv_freg_name_sym[dec.rs2]; break;
				case '6': args += riscv_freg_name_sym[dec.rs3]; break;
				case '7': args += format_string("%d", dec.rs1); break;
				case 'i': args += format_string("%lld", dec.imm); break;
				case 'o': args += format_string("pc %c %td",
					intptr_t(dec.imm) < 0 ? '-' : '+',
					intptr_t(dec.imm) < 0 ? -intptr_t(dec.imm) : intptr_t(dec.imm)); break;
				case 'c': {
					const char * csr_name = riscv_csr_name_sym[dec.imm & 0xfff];
					if (csr_name) args += format_string("%s", csr_name);
					else args += format_string("0x%03x", dec.imm & 0xfff);
					break;
				}
				case 'r':
					switch(dec.rm) {
						case riscv_rm_rne: args += "rne"; break;
						case riscv_rm_rtz: args += "rtz"; break;
						case riscv_rm_rdn: args += "rdn"; break;
						case riscv_rm_rup: args += "rup"; break;
						case riscv_rm_rmm: args += "rmm"; break;
						case riscv_rm_dyn: args += "dyn"; break;
						default:           args += "inv"; break;
					}
					break;
				case 'p':
					if (dec.pred & riscv_fence_i) args += "i";
					if (dec.pred & riscv_fence_o) args += "o";
					if (dec.pred & riscv_fence_r) args += "r";
					if (dec.pred & riscv_fence_w) args += "w";
					break;
				case 's':
					if (dec.succ & riscv_fence_i) args += "i";
					if (dec.succ & riscv_fence_o) args += "o";
					if (dec.succ & riscv_fence_r) args += "r";
					if (dec.succ & riscv_fence_w) args += "w";
					break;
				case '\t': while (args.length() < 12) args += " "; break;
				case 'A': if (dec.aq) args += ".aq"; break;
				case 'R': if (dec.rl) args += ".rl"; break;
				default:
					break;
			}
			fmt++;
		}
		return args;
	}

	void disasm_inst_print(disasm &dec, std::deque<disasm> &dec_hist,
		uintptr_t pc, uintptr_t pc_offset, uintptr_t gp,
		symbol_name_fn symlookup = null_symbol_lookup,
		symbol_colorize_fn colorize = null_symbol_colorize);

}

#endif
