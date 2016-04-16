//
//  riscv-disasm.h
//

#ifndef riscv_disasm_h
#define riscv_disasm_h

typedef std::function<const char*(riscv_ptr)> riscv_symbol_name_fn;

extern const char* riscv_null_symbol_lookup(riscv_ptr);
extern void riscv_disasm_instruction(riscv_decode &dec, riscv_proc_state *proc,
	riscv_ptr pc, riscv_ptr next_pc, riscv_ptr pc_offset,
	riscv_symbol_name_fn symlookup = riscv_null_symbol_lookup);

#endif
