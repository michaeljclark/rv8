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
#include "riscv-color.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-decode-switch.h"
#include "riscv-csr.h"
#include "riscv-compression.h"
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

#define ADDRESS_BEGIN S_COLOR F_YELLOW B_BLACK
#define SYMBOL_BEGIN  S_COLOR F_WHITE B_BLACK

const char* riscv_disasm_symbol_colorizer(char *buf, size_t buflen, const char *symbol, const char *type)
{
	if (strcmp(type, "address") == 0) {
		snprintf(buf, buflen, "%s%s%s", ADDRESS_BEGIN, symbol, T_RESET);
		return buf;
	} else if (strcmp(type, "symbol") == 0) {
		snprintf(buf, buflen, "%s%s%s", SYMBOL_BEGIN, symbol, T_RESET);
		return buf;
	}
	return symbol;
}

void decode_rv64(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset, riscv_symbol_name_fn symlookup)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;
	proc.pc = start;
	while (proc.pc < end) {
		riscv_ptr next_pc = riscv_decode_instruction(dec, &proc);
		riscv_disasm_instruction(dec, &proc, proc.pc, next_pc, pc_offset,
			symlookup, riscv_disasm_symbol_colorizer);
		proc.pc = next_pc;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) panic("usage: %s <elf_file>", argv[0]);
	elf_file elf(argv[1]);
	elf_print_info(elf);

	auto symloopup = [&elf] (riscv_ptr addr)->const char* {
		auto sym = elf_sym(elf, (Elf64_Addr)addr);
		if (!sym) return nullptr;
		return elf_sym_name(elf, sym);
	};

	for (size_t i = 0; i < elf.shdrs.size(); i++) {
		Elf64_Shdr &shdr = elf.shdrs[i];
		if (shdr.sh_flags & SHF_EXECINSTR) {
			printf("Section[%2lu] %s\n", i, elf_shdr_name(elf, i));
			decode_rv64(elf.buf.data() + shdr.sh_offset,
				elf.buf.data() + shdr.sh_offset + shdr.sh_size,
				elf.buf.data() + shdr.sh_offset - shdr.sh_addr,
				symloopup);
			printf("\n");
		}
	}
	return 0;
}
