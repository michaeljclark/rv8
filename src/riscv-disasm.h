//
//  riscv-disasm.h
//

#ifndef riscv_disasm_h
#define riscv_disasm_h

namespace riscv {

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
		{ riscv_op_unknown, riscv_op_unknown,  rva_none  },
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

}

typedef std::function<const char*(uintptr_t, bool nearest)> riscv_symbol_name_fn;
typedef std::function<const char*(const char *type)> riscv_symbol_colorize_fn;

const char* riscv_null_symbol_lookup(uintptr_t, bool nearest);
const char* riscv_null_symbol_colorize(const char *type);
void riscv_disasm_insn(riscv_disasm &dec, std::deque<riscv_disasm> &dec_hist,
	uintptr_t pc, uintptr_t next_pc, uintptr_t pc_offset, uintptr_t gp,
	riscv_symbol_name_fn symlookup = riscv_null_symbol_lookup,
	riscv_symbol_colorize_fn colorize = riscv_null_symbol_colorize);

#endif
