#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <vector>
#include <map>
#include <string>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-processor.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-util.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-csr.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

void rv64_exec(riscv_decode &dec, riscv_proc_state *proc)
{
	riscv_ptr next_pc = riscv_decode_instruction(dec, proc->pc);
	switch (dec.op) {
		case riscv_op_addi:
			proc->i_reg[dec.rd].lu = proc->i_reg[dec.rs1].lu + dec.imm;
			proc->pc = next_pc;
			break;
		case riscv_op_auipc:
			proc->i_reg[dec.rd].lu = riscv_lu(proc->pc) + dec.imm;
			proc->pc = next_pc;
			break;
		case riscv_op_lui:
			proc->i_reg[dec.rd].lu = dec.imm;
			proc->pc = next_pc;
			break;
		case riscv_op_scall:
			switch (proc->i_reg[riscv_ireg_a7].lu) {
				case 64: /* sys_write */
					proc->i_reg[riscv_ireg_a0].lu = write(proc->i_reg[riscv_ireg_a0].lu,
						(void*)proc->i_reg[riscv_ireg_a1].lu, proc->i_reg[riscv_ireg_a2].lu);
					break;
				case 93: /* sys_exit */
					exit(proc->i_reg[riscv_ireg_a0].lu);
					break;
				default:
					panic("illegal syscall: %d", proc->i_reg[riscv_ireg_a7].lu);
			}
			proc->pc = next_pc;
			break;
		default:
			panic("illegal instruciton: %s", riscv_instruction_name[dec.op]);
	}
}

void rv64_run(riscv_ptr entry)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;
	proc.pc = entry;
	while (true) {
		rv64_exec(dec, &proc);
	}
}

void* map_executable(const char* filename, void *vaddr, size_t len, size_t offset)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		panic("error: open: %s: %s", filename, strerror(errno));
	}
	void *addr = mmap(vaddr, len, PROT_READ | PROT_EXEC, MAP_FIXED | MAP_PRIVATE, fd, offset);
	if (addr == MAP_FAILED) {
		panic("error: mmap: %s: %s", filename, strerror(errno));
	}
	close(fd);
	return addr;
}

int main(int argc, char *argv[])
{
	elf_file elf;
	if (argc != 2) panic("usage: %s <elf_file>", argv[0]);
	elf.load(argv[1]);
	for (size_t i = 0; i < elf.phdrs.size(); i++) {
		Elf64_Phdr &phdr = elf.phdrs[i];
		if (phdr.p_flags & PT_LOAD) {
			map_executable(argv[1], (void*)phdr.p_vaddr, phdr.p_memsz, phdr.p_offset);
			rv64_run(riscv_ptr(elf.ehdr.e_entry));
			break;
		}
	}
	// TODO : munmap
	return 0;
}
