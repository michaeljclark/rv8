//
//  riscv-disasm.h
//

#ifndef riscv_disasm_h
#define riscv_disasm_h

typedef std::function<const char*(uintptr_t, bool nearest)> riscv_symbol_name_fn;
typedef std::function<const char*(const char *type)> riscv_symbol_colorize_fn;

const char* riscv_null_symbol_lookup(uintptr_t, bool nearest);
const char* riscv_null_symbol_colorize(const char *type);
void riscv_disasm_insn(riscv_disasm &dec, std::deque<riscv_disasm> &dec_hist,
	uintptr_t pc, uintptr_t next_pc, uintptr_t pc_offset, uintptr_t gp,
	riscv_symbol_name_fn symlookup = riscv_null_symbol_lookup,
	riscv_symbol_colorize_fn colorize = riscv_null_symbol_colorize);

#endif
