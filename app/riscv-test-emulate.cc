#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <vector>
#include <map>
#include <string>

#include <unistd.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-regs.h"
#include "riscv-processor.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-util.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-decode-switch.h"
#include "riscv-csr.h"
#include "riscv-compression.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

void riscv_execute_instruction(riscv_decode &dec, riscv_proc_state *proc)
{
	memset(&dec, 0, sizeof(dec));
	riscv_lu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		dec.sz = 4;
	} else {
		dec.sz = 2;
	}
	riscv_decode_opcode(dec, inst);
	riscv_decode_type(dec, inst);
	switch (dec.op) {
		case riscv_op_addi:
			proc->i_reg[dec.rd].lu = proc->i_reg[dec.rs1].lu + dec.imm;
			proc->pc += dec.sz;
			break;
		case riscv_op_lui:
			proc->i_reg[dec.rd].lu = dec.imm;
			proc->pc += dec.sz;
			break;
		case riscv_op_scall:
			switch (proc->i_reg[riscv_ireg_a7].lu) {
				case 64: /* sys_write */
					proc->i_reg[riscv_ireg_a0].lu = write(proc->i_reg[riscv_ireg_a0].lu,
						proc->mem + proc->i_reg[riscv_ireg_a1].lu - proc->vaddr, proc->i_reg[riscv_ireg_a2].lu);
					break;
				case 93: /* sys_exit */
					exit(proc->i_reg[riscv_ireg_a0].lu);
					break;
				default:
					panic("illegal syscall: %d", proc->i_reg[riscv_ireg_a7].lu);
			}
			proc->pc += dec.sz;
			break;
		default:
			panic("illegal instruciton: %s", riscv_instruction_name[dec.op]);
	}
}

void run_rv64(riscv_ptr mem, riscv_lu vaddr, riscv_lu entry)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;
	proc.mem = mem;
	proc.vaddr = vaddr;
	proc.pc = mem + entry - vaddr;
	while (true) {
		riscv_execute_instruction(dec, &proc);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) panic("usage: %s <elf_file>", argv[0]);
	elf_file elf(argv[1]);
	for (size_t i = 0; i < elf.phdrs.size(); i++) {
		Elf64_Phdr &phdr = elf.phdrs[i];
		if (phdr.p_flags & PT_LOAD) {
			run_rv64(elf.buf.data(), phdr.p_vaddr, elf.ehdr.e_entry);
			break;
		}
	}
	panic("could not execute");
	return 0;
}
