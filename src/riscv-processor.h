//
//  riscv-processor.h
//

#ifndef riscv_processor_h
#define riscv_processor_h

/* Instruction sets */

enum riscv_proc_type
{
	riscv_proc_type_rv32i,
	riscv_proc_type_rv32e,
	riscv_proc_type_rv64i,
	riscv_proc_type_rv128i
};

/* Processor state */

struct riscv_proc_state
{
	riscv_ptr pc;
	riscv_i_reg i_reg[32];
	riscv_f_reg f_reg[32];
	riscv_proc_type p_type;
};

#endif