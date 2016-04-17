//
//  riscv-elf-format.cc
//

#include <cstdint>
#include <cinttypes>
#include <vector>
#include <map>
#include <string>

#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-util.h"

const char* elf_null_symbol_colorize(const char *type) { return ""; }

const char* elf_ei_class_name(int v)
{
	switch (v) {
		case ELFCLASS32: return "ELF32";
		case ELFCLASS64: return "ELF64";
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
};

const char* elf_e_machine_name(int v)
{
	switch (v) {
		case EM_NONE: return "none";
		case EM_X86_64: return "x86-64";
		case EM_RISCV: return "RISC-V";
		default: return "unknown";
	}
};

const char* elf_p_type_name(int v)
{
	switch (v) {
		case PT_NULL: return "NULL";
		case PT_LOAD: return "LOAD";
		case PT_DYNAMIC: return "DYNAMIC";
		case PT_INTERP: return "INTERP";
		case PT_NOTE: return "NOTE";
		case PT_SHLIB: return "SHLIB";
		case PT_PHDR: return "PHDR";
		default: return "UNKNOWN";
	}
}

const std::string elf_p_flags_name(int v)
{
	std::string s;
	if (v & PF_X) s+= "+X";
	if (v & PF_W) s+= "+W";
	if (v & PF_R) s+= "+R";
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
		case SHT_HASH: return "HASH" ;
		case SHT_DYNAMIC: return "DYNAMIC";
		case SHT_NOTE: return "NOTE";
		case SHT_NOBITS: return "NOBITS";
		case SHT_REL: return "REL";
		case SHT_SHLIB: return "SHLIB";
		case SHT_DYNSYM: return "DYNSYM";
		case SHT_INIT_ARRAY: return "INIT_ARRAY";
		case SHT_FINI_ARRAY: return "FINI_ARRAY";
		default: return "UNKNOWN";
	}
};

const std::string elf_sh_shndx_name(int v)
{
	switch (v) {
		case SHN_UNDEF: return "UNDEF";
		case SHN_ABS: return "ABS";
		case SHN_COMMON: return "COMMON";
		default: return format_string("%d", v);
	}
}

const std::string elf_sh_flags_name(int v)
{
	std::string s;
	if (v & SHF_WRITE) s+= "+WRITE";
	if (v & SHF_ALLOC) s+= "+ALLOC";
	if (v & SHF_EXECINSTR) s+= "+EXEC";
	return s;
};

const char* elf_st_bind_name(int v)
{
	switch (v) {
		case STB_LOCAL: return "LOCAL";
		case STB_GLOBAL: return "GLOBAL";
		case STB_WEAK: return "WEAK";
		default: return "UNKNOWN";
	}
};

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
};

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
	return format_string("%s[%2lu]%s %-7s %-9s %s0x%016x 0x%016x 0x%016x 0x%016x 0x%016x%s %5lu",
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
	return format_string("%s[%2lu]%s %-20s %-12s %-12s %s0x%016x 0x%016x 0x%016x%s %4d %4d %4d",
		colorize("legend"),
		i,
		colorize("reset"),
		elf_shdr_name(elf, i),
		elf_sh_type_name(shdr.sh_type),
		elf_sh_flags_name(shdr.sh_flags).c_str(),
		colorize("address"),
		shdr.sh_addr,
		shdr.sh_offset,
		shdr.sh_size,
		colorize("reset"),
		shdr.sh_entsize,
		shdr.sh_link,
		shdr.sh_info);
}

const std::string elf_sym_info(elf_file &elf, int i, elf_symbol_colorize_fn colorize)
{
	Elf64_Sym &sym = elf.symbols[i];
	return format_string("%s[%4lu]%s %s0x%016x%s %5d %-8s %-8s %-8s %6s %s%s%s",
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
		elf_sym_name(elf, i),
		colorize("reset"));
}

const char* elf_shdr_name(elf_file &elf, int i)
{
	return elf.shstrtab ? (const char*)(elf.buf.data() +
		elf.shstrtab->sh_offset + elf.shdrs[i].sh_name) : "";
}

const char* elf_sym_name(elf_file &elf, int i)
{
	return elf.strtab ? (const char*)(elf.buf.data() +
		elf.strtab->sh_offset + elf.symbols[i].st_name) : "";
}

const char* elf_sym_name(elf_file &elf, const Elf64_Sym *sym)
{
	return elf.strtab ? (const char*)(elf.buf.data() +
		elf.strtab->sh_offset + sym->st_name) : "";
}

const Elf64_Sym* elf_sym(elf_file &elf, Elf64_Addr addr)
{
	size_t i = elf.addr_symbol_map[addr];
	if (i == 0 || i >= elf.symbols.size()) return nullptr;
	return &elf.symbols[i];
}

void elf_print_header_info(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%sFile       %s%s\n", colorize("legend"), colorize("reset"), elf.filename.c_str());
	printf("%sClass      %s%s\n", colorize("legend"), colorize("reset"), elf_ei_class_name(elf.ei_class));
	printf("%sMachine    %s%s\n", colorize("legend"), colorize("reset"), elf_e_machine_name(elf.ehdr.e_machine));
	printf("%sType       %s%s\n", colorize("legend"), colorize("reset"), elf_e_type_name(elf.ehdr.e_type));
	printf("%sData       %s%s\n", colorize("legend"), colorize("reset"), elf_ei_data_name(elf.ei_data));
	printf("%sEntryAddr  %s0x%016" PRIx64 "\n", colorize("legend"), colorize("reset"), elf.ehdr.e_entry);
}

void elf_print_section_headers(elf_file &elf, elf_symbol_colorize_fn colorize)
{
	printf("%sShdr %-20s %-12s %-12s %-18s %-18s %-18s %4s %4s %4s%s\n",
		colorize("title"), "Name", "Type", "Flags", "Addr", "Offset", "Size",
		"Ents", "Link", "Info", colorize("reset"));
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
