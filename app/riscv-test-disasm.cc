#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <vector>
#include <map>
#include <string>

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
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

void decode_rv64(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;
	proc.pc = start;
	while (proc.pc < end) {
		riscv_ptr pc = proc.pc;
		riscv_decode_instruction(dec, &proc);
		riscv_disasm_instruction(dec, &proc, pc, pc_offset);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) panic("usage: %s <elf_file>", argv[0]);
	elf_file elf(argv[1]);
	elf_print_info(elf);
	for (size_t i = 0; i < elf.shdrs.size(); i++) {
		Elf64_Shdr &shdr = elf.shdrs[i];
		if (shdr.sh_flags & SHF_EXECINSTR) {
			printf("Section[%2lu] %s\n", i, elf_shdr_name(elf, i));
			decode_rv64(elf.buf.data() + shdr.sh_offset,
				elf.buf.data() + shdr.sh_offset + shdr.sh_size,
				elf.buf.data() + shdr.sh_offset - shdr.sh_addr);
			printf("\n");
		}
	}
	return 0;
}
