//
//  elf-format.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cinttypes>
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"
#include "util.h"

const char* elf_null_symbol_colorize(const char *type) { return ""; }

const char* elf_ei_class_name(int v)
{
	switch (v) {
		case ELFCLASS32: return "ELF32";
		case ELFCLASS64: return "ELF64";
		case ELFCLASS128: return "ELF128";
		default: return "unknown";
	};
}

const char* elf_ei_data_name(int v)
{
	switch (v) {
		case ELFDATA2LSB: return "little-endian";
		case ELFDATA2MSB: return "big-endian";
		default: return "unknown";
	};
}

const char* elf_e_type_name(int v)
{
	switch (v) {
		case ET_NONE: return "none";
		case ET_REL: return "relocatable object";
		case ET_EXEC: return "executable";
		case ET_DYN: return "shared object";
		case ET_CORE: return "core";
		default: return "unknown";
	};
}

const char* elf_e_machine_name(int v)
{
	switch (v) {
		case EM_NONE: return "none";
		case EM_X86_64: return "x86-64";
		case EM_RISCV: return "RISC-V";
		default: return "unknown";
	}
}

const char* elf_p_type_name(int v)
{
	switch (v) {
		case PT_NULL: return "NULL";
		case PT_LOAD: return "LOAD";
		case PT_DYNAMIC: return "DYNAMIC";
		case PT_INTERP: return "INTERP";
		case PT_NOTE: return "NOTE";
		case PT_PHDR: return "PHDR";
		case PT_SHLIB: return "SHLIB";
		case PT_TLS: return "TLS";
		case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
		case PT_GNU_STACK: return "GNU_STACK";
		case PT_GNU_RELRO: return "GNU_RELRO";
		default: return "UNKNOWN";
	}
}

const std::string elf_p_flags_name(int v)
{
	std::string s;
	if (v & PF_R) s+= "+R";
	if (v & PF_W) s+= "+W";
	if (v & PF_X) s+= "+X";
	return s;
}

const char* elf_sh_type_name(int v)
{
	switch (v) {
		case SHT_NULL: return "NULL";
		case SHT_PROGBITS: return "PROGBITS";
		case SHT_SYMTAB: return "SYMTAB";
		case SHT_STRTAB: return "STRTAB";
		case SHT_RELA: return "RELA";
		case SHT_HASH: return "HASH";
		case SHT_DYNAMIC: return "DYNAMIC";
		case SHT_NOTE: return "NOTE";
		case SHT_NOBITS: return "NOBITS";
		case SHT_REL: return "REL";
		case SHT_SHLIB: return "SHLIB";
		case SHT_DYNSYM: return "DYNSYM";
		case SHT_INIT_ARRAY: return "INIT_ARRAY";
		case SHT_FINI_ARRAY: return "FINI_ARRAY";
		case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
		case SHT_GROUP: return "GROUP";
		case SHT_SYMTAB_SHNDX: return "SYMTAB_SHNDX";
		case SHT_GNU_VERDEF: return "GNU_VERDEF";
		case SHT_GNU_VERNEED: return "GNU_VERNEED";
		case SHT_GNU_VERSYM: return "GNU_VERSYM";
		default: return "UNKNOWN";
	}
}

const std::string elf_sh_shndx_name(int v)
{
	switch (v) {
		case SHN_UNDEF: return "UNDEF";
		case SHN_ABS: return "ABS";
		case SHN_COMMON: return "COMMON";
		case SHN_XINDEX: return "XINDEX";
		default: return format_string("%d", v);
	}
}

const std::string elf_sh_flags_name(int v)
{
	std::string s;
	if (v & SHF_WRITE) s+= "+WRITE";
	if (v & SHF_ALLOC) s+= "+ALLOC";
	if (v & SHF_EXECINSTR) s+= "+EXEC";
	if (v & SHF_MERGE) s+= "+MERGE";
	if (v & SHF_STRINGS) s+= "+STRINGS";
	if (v & SHF_INFO_LINK) s+= "+INFO_LINK";
	if (v & SHF_LINK_ORDER) s+= "+LINK_ORDER";
	if (v & SHF_GROUP) s+= "+GROUP";
	if (v & SHF_TLS) s+= "+TLS";
	return s;
}

const char* elf_st_bind_name(int v)
{
	switch (v) {
		case STB_LOCAL: return "LOCAL";
		case STB_GLOBAL: return "GLOBAL";
		case STB_WEAK: return "WEAK";
		default: return "UNKNOWN";
	}
}

const char* elf_st_type_name(int v)
{
	switch (v) {
		case STT_NOTYPE: return "NOTYPE";
		case STT_OBJECT: return "OBJECT";
		case STT_FUNC: return "FUNC";
		case STT_SECTION: return "SECTION";
		case STT_FILE: return "FILE";
		default: return "UNKNOWN";
	}
}

const char* elf_st_other_name(int v)
{
	switch (v) {
		case STV_DEFAULT: return "DEFAULT";
		case STV_INTERNAL: return "INTERNAL";
		case STV_HIDDEN: return "HIDDEN";
		case STV_PROTECTED: return "PROTECTED";
		default: return "UNKNOWN";
	}
}

const char* elf_rela_type_name(int v) {
	switch (v) {
		case R_RISCV_NONE: return "R_RISCV_NONE";
		case R_RISCV_32: return "R_RISCV_32";
		case R_RISCV_64: return "R_RISCV_64";
		case R_RISCV_RELATIVE: return "R_RISCV_RELATIVE";
		case R_RISCV_COPY: return "R_RISCV_COPY";
		case R_RISCV_JUMP_SLOT: return "R_RISCV_JUMP_SLOT";
		case R_RISCV_TLS_DTPMOD32: return "R_RISCV_TLS_DTPMOD32";
		case R_RISCV_TLS_DTPMOD64: return "R_RISCV_TLS_DTPMOD64";
		case R_RISCV_TLS_DTPREL32: return "R_RISCV_TLS_DTPREL32";
		case R_RISCV_TLS_DTPREL64: return "R_RISCV_TLS_DTPREL64";
		case R_RISCV_TLS_TPREL32: return "R_RISCV_TLS_TPREL32";
		case R_RISCV_TLS_TPREL64: return "R_RISCV_TLS_TPREL64";
		case R_RISCV_BRANCH: return "R_RISCV_BRANCH";
		case R_RISCV_JAL: return "R_RISCV_JAL";
		case R_RISCV_CALL: return "R_RISCV_CALL";
		case R_RISCV_CALL_PLT: return "R_RISCV_CALL_PLT";
		case R_RISCV_GOT_HI20: return "R_RISCV_GOT_HI20";
		case R_RISCV_TLS_GOT_HI20: return "R_RISCV_TLS_GOT_HI20";
		case R_RISCV_TLS_GD_HI20: return "R_RISCV_TLS_GD_HI20";
		case R_RISCV_PCREL_HI20: return "R_RISCV_PCREL_HI20";
		case R_RISCV_PCREL_LO12_I: return "R_RISCV_PCREL_LO12_I";
		case R_RISCV_PCREL_LO12_S: return "R_RISCV_PCREL_LO12_S";
		case R_RISCV_HI20: return "R_RISCV_HI20";
		case R_RISCV_LO12_I: return "R_RISCV_LO12_I";
		case R_RISCV_LO12_S: return "R_RISCV_LO12_S";
		case R_RISCV_TPREL_HI20: return "R_RISCV_TPREL_HI20";
		case R_RISCV_TPREL_LO12_I: return "R_RISCV_TPREL_LO12_I";
		case R_RISCV_TPREL_LO12_S: return "R_RISCV_TPREL_LO12_S";
		case R_RISCV_TPREL_ADD: return "R_RISCV_TPREL_ADD";
		case R_RISCV_ADD8: return "R_RISCV_ADD8";
		case R_RISCV_ADD16: return "R_RISCV_ADD16";
		case R_RISCV_ADD32: return "R_RISCV_ADD32";
		case R_RISCV_ADD64: return "R_RISCV_ADD64";
		case R_RISCV_SUB8: return "R_RISCV_SUB8";
		case R_RISCV_SUB16: return "R_RISCV_SUB16";
		case R_RISCV_SUB32: return "R_RISCV_SUB32";
		case R_RISCV_SUB64: return "R_RISCV_SUB64";
		case R_RISCV_GNU_VTINHERIT: return "R_RISCV_GNU_VTINHERIT";
		case R_RISCV_GNU_VTENTRY: return "R_RISCV_GNU_VTENTRY";
		case R_RISCV_ALIGN: return "R_RISCV_ALIGN";
		case R_RISCV_RVC_BRANCH: return "R_RISCV_RVC_BRANCH";
		case R_RISCV_RVC_JUMP: return "R_RISCV_RVC_JUMP";
		case R_RISCV_RVC_LUI: return "R_RISCV_RVC_LUI";
		case R_RISCV_GPREL_I: return "R_RISCV_GPREL_I";
		case R_RISCV_GPREL_S: return "R_RISCV_GPREL_S";
		case R_RISCV_TPREL_I: return "R_RISCV_TPREL_I";
		case R_RISCV_TPREL_S: return "R_RISCV_TPREL_S";
		case R_RISCV_RELAX: return "R_RISCV_RELAX";
		case R_RISCV_SUB6: return "R_RISCV_SUB6";
		case R_RISCV_SET6: return "R_RISCV_SET6";
		case R_RISCV_SET8: return "R_RISCV_SET8";
		case R_RISCV_SET16: return "R_RISCV_SET16";
		case R_RISCV_SET32: return "R_RISCV_SET32";
		default: return "UNKNOWN";
	}
}

const std::string elf_file_info(elf_file &elf)
{
	return format_string("%s: %s %s %s %s",
		elf.filename.c_str(),
		elf_ei_class_name(elf.ei_class),
		elf_ei_data_name(elf.ei_data),
		elf_e_machine_name(elf.ehdr.e_machine),
		elf_e_type_name(elf.ehdr.e_type));
}

const std::string elf_phdr_info(elf_file &elf, int i, elf_symbol_colorize_fn colorize)
{
	Elf64_Phdr &phdr = elf.phdrs[i];
	return format_string("%s[%2lu]%s %-7s %-9s %s0x%-16llx 0x%-16llx 0x%-16llx 0x%-16llx 0x%-16llx%s %5lu",
		colorize("legend"),
		i,
		colorize("reset"),
		elf_p_type_name(phdr.p_type),
		elf_p_flags_name(phdr.p_flags).c_str(),
		colorize("address"),
		phdr.p_offset,
		phdr.p_vaddr,
		phdr.p_paddr,
		phdr.p_filesz,
		phdr.p_memsz,
		colorize("reset"),
		phdr.p_align);
}

const std::string elf_shdr_info(elf_file &elf, int i, elf_symbol_colorize_fn colorize)
{
	Elf64_Shdr &shdr = elf.shdrs[i];
	return format_string("%s[%2lu]%s %-20s %-12s %-20s %s0x%-16llx 0x%-8llx 0x%-8llx 0x%-4llx%s %4d %4d %4d",
		colorize("legend"),
		i,
		colorize("reset"),
		elf.shdr_name(i),
		elf_sh_type_name(shdr.sh_type),
		elf_sh_flags_name(shdr.sh_flags).c_str(),
		colorize("address"),
		shdr.sh_addr,
		shdr.sh_offset,
		shdr.sh_size,
		shdr.sh_addralign,
		colorize("reset"),
		shdr.sh_entsize,
		shdr.sh_link,
		shdr.sh_info);
}

const std::string elf_sym_info(elf_file &elf, int i, elf_symbol_colorize_fn colorize)
{
	Elf64_Sym &sym = elf.symbols[i];
	return format_string("%s[%4lu]%s %s0x%-16llx%s %5d %-8s %-8s %-8s %6s %s%s%s",
		colorize("legend"),
		i,
		colorize("reset"),
		colorize("address"),
		sym.st_value,
		colorize("reset"),
		sym.st_size,
		elf_st_type_name(ELF64_ST_TYPE(sym.st_info)),
		elf_st_bind_name(ELF64_ST_BIND(sym.st_info)),
		elf_st_other_name(sym.st_other),
		elf_sh_shndx_name(sym.st_shndx).c_str(),
		colorize("symbol"),
		elf.sym_name(i),
		colorize("reset"));
}

void elf_print_header_info(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%sFile                   %s%s\n", colorize("legend"), colorize("reset"), elf.filename.c_str());
	printf("%sClass                  %s%s\n", colorize("legend"), colorize("reset"), elf_ei_class_name(elf.ei_class));
	printf("%sData                   %s%s\n", colorize("legend"), colorize("reset"), elf_ei_data_name(elf.ei_data));
	printf("%sType                   %s%s\n", colorize("legend"), colorize("reset"), elf_e_type_name(elf.ehdr.e_type));
	printf("%sMachine                %s%s\n", colorize("legend"), colorize("reset"), elf_e_machine_name(elf.ehdr.e_machine));
	if (elf.interp_name()) {
		printf("%sInterp                 %s%s\n", colorize("legend"), colorize("reset"), elf.interp_name());
	}
}

void elf_print_header_ext_info(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%se_ident[EI_VERSION]    %s%d\n", colorize("legend"), colorize("reset"), (int)elf.ehdr.e_ident[EI_VERSION]);
	printf("%se_ident[EI_OSABI]      %s%d\n", colorize("legend"), colorize("reset"), (int)elf.ehdr.e_ident[EI_OSABI]);
	printf("%se_ident[EI_ABIVERSION] %s%d\n", colorize("legend"), colorize("reset"), (int)elf.ehdr.e_ident[EI_ABIVERSION]);
	printf("%se_version              %s%d\n", colorize("legend"), colorize("reset"), (int)elf.ehdr.e_version);
	printf("%se_flags                %s%d\n", colorize("legend"), colorize("reset"), (int)elf.ehdr.e_flags);
	printf("%se_entry                %s0x%-16llx\n", colorize("legend"), colorize("reset"), elf.ehdr.e_entry);
}

void elf_print_section_headers(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%sShdr %-20s %-12s %-20s %-18s %-10s %-10s %-6s %4s %4s %4s%s\n",
		colorize("title"), "Name", "Type", "Flags", "Addr", "Offset", "Size",
		"Align", "Ents", "Link", "Info", colorize("reset"));
	for (size_t i = 0; i < elf.shdrs.size(); i++) {
		printf("%s\n", elf_shdr_info(elf, i, colorize).c_str());
	}
}

void elf_print_program_headers(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%sPhdr %-7s %-9s %-18s %-18s %-18s %-18s %-18s %-5s%s\n",
		colorize("title"), "Type", "Flags", "Offset", "VirtAddr", "PhysAddr",
		"FileSize", "MemSize", "Align", colorize("reset"));
	for (size_t i = 0; i < elf.phdrs.size(); i++) {
		printf("%s\n", elf_phdr_info(elf, i, colorize).c_str());
	}
}

void elf_print_symbol_table(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%sSymbol %-18s %-5s %-8s %-8s %-8s %6s %-57s%s\n",
		colorize("title"), "Value", "Size", "Type", "Bind", "Vis", "Index",
		"Name", colorize("reset"));
	for (size_t i = 0; i < elf.symbols.size(); i++) {
		printf("%s\n", elf_sym_info(elf, i, colorize).c_str());
	}
}

void elf_print_relocations(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	if (elf.rela_text == 0) return;

	printf("%sSection[%2lu] %-111s%s\n", colorize("title"),
		elf.rela_text, elf.shdr_name(elf.rela_text), colorize("reset"));

	printf("\n%sReloc  %-18s %-20s %-30s %-18s%s\n",
		colorize("title"),
		"Offset", "Symbol", "Type", "Addend",
		colorize("reset"));
	size_t j = 0;
	for (auto &rela : elf.relocations) {
		Elf64_Xword sym = ELF64_R_SYM(rela.r_info);
		Elf64_Xword type = ELF64_R_TYPE(rela.r_info);
		printf("%s[%4lu]%s %s0x%-16llx%s %-20s %-30s %s0x%-16llx%s\n",
			colorize("legend"),
			j++,
			colorize("reset"),
			colorize("address"),
			rela.r_offset,
			colorize("reset"),
			elf.sym_name(sym),
			elf_rela_type_name(type),
			colorize("address"),
			rela.r_addend,
			colorize("reset"));
	}
	printf("\n");
}
