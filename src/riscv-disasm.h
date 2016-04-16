//
//  riscv-disasm.h
//

#ifndef riscv_disasm_h
#define riscv_disasm_h

typedef std::function<const char*(riscv_ptr)> riscv_symbol_name_fn;
typedef std::function<const char*(char *buf, size_t buflen, const char *symbol, const char *type)> riscv_symbol_colorizer_fn;

extern const char* riscv_null_symbol_lookup(riscv_ptr);
extern const char* riscv_null_symbol_colorizer(char *buf, size_t buflen, const char *symbol, const char *type);
extern void riscv_disasm_instruction(riscv_decode &dec, riscv_proc_state *proc,
	riscv_ptr pc, riscv_ptr next_pc, riscv_ptr pc_offset,
	riscv_symbol_name_fn symlookup = riscv_null_symbol_lookup,
	riscv_symbol_colorizer_fn colorizer = riscv_null_symbol_colorizer);

#endif
