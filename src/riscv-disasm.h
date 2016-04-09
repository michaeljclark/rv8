//
//  riscv-disasm.h
//

#ifndef riscv_disasm_h
#define riscv_disasm_h

extern void riscv_disasm_instruction(riscv_decode &dec, riscv_proc_state *proc, riscv_ptr pc, riscv_ptr pc_offset);

#endif
