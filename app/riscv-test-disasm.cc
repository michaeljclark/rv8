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

#define HEADER_BEGIN  S_COLOR S_BOLD F_WHITE B_BLACK
#define LEGEND_BEGIN  S_COLOR S_BOLD F_MAGENTA B_BLACK
#define TITLE_BEGIN   S_COLOR S_BOLD F_WHITE B_BLACK
#define OPCODE_BEGIN  S_COLOR S_BOLD F_GREEN B_BLACK
#define ADDRESS_BEGIN S_COLOR F_YELLOW B_BLACK
#define SYMBOL_BEGIN  S_COLOR F_WHITE B_BLACK

const char* disasm_colorize(const char *type)
{
	if (!isatty(fileno(stdout))) {
		return "";
	} else if (strcmp(type, "header") == 0) {
		return HEADER_BEGIN;
	} else if (strcmp(type, "legend") == 0) {
		return LEGEND_BEGIN;
	} else if (strcmp(type, "title") == 0) {
		return TITLE_BEGIN;
	} else if (strcmp(type, "opcode") == 0) {
		return OPCODE_BEGIN;
	} else if (strcmp(type, "address") == 0) {
		return ADDRESS_BEGIN;
	} else if (strcmp(type, "symbol") == 0) {
		return SYMBOL_BEGIN;
	} else if (strcmp(type, "reset") == 0) {
		return T_RESET;
	}
	return "";
}

void label_rv64(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset,
	std::map<riscv_ptr,std::string> &branch_labels)
{
	ssize_t branch_label_num = 1;
	char branch_label_buf[32];

	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;
	proc.pc = start;
	uint64_t addr = 0;
	while (proc.pc < end) {
		riscv_ptr next_pc = riscv_decode_instruction(dec, &proc);
		const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
		if (comp) {
			dec.op = comp->op;
			dec.type = comp->type;
		}
		switch (dec.type) {
			case riscv_inst_type_sb:
			case riscv_inst_type_uj:
			{
				addr = proc.pc - pc_offset + dec.imm;
				snprintf(branch_label_buf, sizeof(branch_label_buf), "label_%06lu", branch_label_num++);
				branch_labels[(riscv_ptr)addr] = branch_label_buf;
				break;
			}
			default:
				break;
		}
		proc.pc = next_pc;
	}
}

void disasm_rv64(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset,
	riscv_symbol_name_fn symlookup)
{
	riscv_decode dec, last_dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;
	proc.pc = start;
	while (proc.pc < end) {
		riscv_ptr next_pc = riscv_decode_instruction(dec, &proc);
		riscv_disasm_instruction(dec, last_dec, &proc, proc.pc, next_pc, pc_offset,
			symlookup, disasm_colorize);
		proc.pc = next_pc;
		last_dec = dec;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) panic("usage: %s <elf_file>", argv[0]);
	elf_file elf(argv[1]);
	printf("%s\n", disasm_colorize("header"));
	printf("---[ ELF Header ]----------------------------------------------------------------------------------------------------------\n");
	printf("%s\n", disasm_colorize("reset"));
	elf_print_header_info(elf, disasm_colorize);
	printf("%s\n", disasm_colorize("header"));
	printf("---[ Section Headers ]-----------------------------------------------------------------------------------------------------\n");
	printf("%s\n", disasm_colorize("reset"));
	elf_print_section_headers(elf, disasm_colorize);
	printf("%s\n", disasm_colorize("header"));
	printf("---[ Program Headers ]-----------------------------------------------------------------------------------------------------\n");
	printf("%s\n", disasm_colorize("reset"));
	elf_print_program_headers(elf, disasm_colorize);
	printf("%s\n", disasm_colorize("header"));
	printf("---[ Symbol Table ]--------------------------------------------------------------------------------------------------------\n");
	printf("%s\n", disasm_colorize("reset"));
	elf_print_symbol_table(elf, disasm_colorize);

	// predecode scanning for branch target addresses
	std::map<riscv_ptr,std::string> branch_labels;
	for (size_t i = 0; i < elf.shdrs.size(); i++) {
		Elf64_Shdr &shdr = elf.shdrs[i];
		if (shdr.sh_flags & SHF_EXECINSTR) {
			label_rv64(elf.buf.data() + shdr.sh_offset,
				elf.buf.data() + shdr.sh_offset + shdr.sh_size,
				elf.buf.data() + shdr.sh_offset - shdr.sh_addr,
				branch_labels);
		}
	}

	auto symloopup = [&elf, &branch_labels] (riscv_ptr addr)->const char* {
		auto sym = elf_sym(elf, (Elf64_Addr)addr);
		if (sym) return elf_sym_name(elf, sym);
		auto branch_label_i = branch_labels.find(addr);
		if (branch_label_i != branch_labels.end()) return branch_label_i->second.c_str();
		return nullptr;
	};

	printf("%s\n", disasm_colorize("header"));
	printf("---[ Disassembly ]---------------------------------------------------------------------------------------------------------\n");
	printf("%s\n", disasm_colorize("reset"));
	for (size_t i = 0; i < elf.shdrs.size(); i++) {
		Elf64_Shdr &shdr = elf.shdrs[i];
		if (shdr.sh_flags & SHF_EXECINSTR) {
			printf("%sSection[%2lu] %-111s%s\n", disasm_colorize("title"), i, elf_shdr_name(elf, i), disasm_colorize("reset"));
			disasm_rv64(elf.buf.data() + shdr.sh_offset,
				elf.buf.data() + shdr.sh_offset + shdr.sh_size,
				elf.buf.data() + shdr.sh_offset - shdr.sh_addr,
				symloopup);
			printf("\n");
		}
	}

	return 0;
}
