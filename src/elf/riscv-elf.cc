//
//  riscv-elf.cc
//

#include <cstdint>
#include <cstring>

#include "riscv-elf.h"
#include "riscv-endian.h"

#define EHDR32_BSWAP(ehdr32, swap16, swap32, swap64)         \
	ehdr32->e_type =       swap16(ehdr32->e_type);       \
	ehdr32->e_machine =    swap16(ehdr32->e_machine);    \
	ehdr32->e_version =    swap32(ehdr32->e_version);    \
	ehdr32->e_entry =      swap32(ehdr32->e_entry);      \
	ehdr32->e_phoff =      swap32(ehdr32->e_phoff);      \
	ehdr32->e_shoff =      swap32(ehdr32->e_shoff);      \
	ehdr32->e_flags =      swap32(ehdr32->e_flags);      \
	ehdr32->e_ehsize =     swap16(ehdr32->e_ehsize);     \
	ehdr32->e_phentsize =  swap16(ehdr32->e_phentsize);  \
	ehdr32->e_phnum =      swap16(ehdr32->e_phnum);      \
	ehdr32->e_shentsize =  swap16(ehdr32->e_shentsize);  \
	ehdr32->e_shnum =      swap16(ehdr32->e_shnum);      \
	ehdr32->e_shstrndx =   swap16(ehdr32->e_shstrndx);

#define PHDR32_BSWAP(phdr32, swap16, swap32, swap64)         \
	phdr32->p_type =       swap32(phdr32->p_type);       \
	phdr32->p_offset =     swap32(phdr32->p_offset);     \
	phdr32->p_vaddr =      swap32(phdr32->p_vaddr);      \
	phdr32->p_paddr =      swap32(phdr32->p_paddr);      \
	phdr32->p_filesz =     swap32(phdr32->p_filesz);     \
	phdr32->p_memsz =      swap32(phdr32->p_memsz);      \
	phdr32->p_flags =      swap32(phdr32->p_flags);      \
	phdr32->p_align =      swap32(phdr32->p_align);

#define SHDR32_BSWAP(shdr32, swap16, swap32, swap64)         \
	shdr32->sh_name =      swap32(shdr32->sh_name);      \
	shdr32->sh_type =      swap32(shdr32->sh_type);      \
	shdr32->sh_flags =     swap32(shdr32->sh_flags);     \
	shdr32->sh_addr =      swap32(shdr32->sh_addr);      \
	shdr32->sh_offset =    swap32(shdr32->sh_offset);    \
	shdr32->sh_size =      swap32(shdr32->sh_size);      \
	shdr32->sh_link =      swap32(shdr32->sh_link);      \
	shdr32->sh_info =      swap32(shdr32->sh_info);      \
	shdr32->sh_addralign = swap32(shdr32->sh_addralign); \
	shdr32->sh_entsize =   swap32(shdr32->sh_entsize);

#define SYM32_BSWAP(sym32, swap16, swap32, swap64)           \
	sym32->st_name =       swap32(sym32->st_name);       \
	sym32->st_value =      swap32(sym32->st_value);      \
	sym32->st_size =       swap32(sym32->st_size);       \
	sym32->st_shndx =      swap16(sym32->st_shndx);

#define EHDR64_BSWAP(ehdr64, swap16, swap32, swap64)         \
	ehdr64->e_type =       swap16(ehdr64->e_type);       \
	ehdr64->e_machine =    swap16(ehdr64->e_machine);    \
	ehdr64->e_version =    swap32(ehdr64->e_version);    \
	ehdr64->e_entry =      swap64(ehdr64->e_entry);      \
	ehdr64->e_phoff =      swap64(ehdr64->e_phoff);      \
	ehdr64->e_shoff =      swap64(ehdr64->e_shoff);      \
	ehdr64->e_flags =      swap32(ehdr64->e_flags);      \
	ehdr64->e_ehsize =     swap16(ehdr64->e_ehsize);     \
	ehdr64->e_phentsize =  swap16(ehdr64->e_phentsize);  \
	ehdr64->e_phnum =      swap16(ehdr64->e_phnum);      \
	ehdr64->e_shentsize =  swap16(ehdr64->e_shentsize);  \
	ehdr64->e_shnum =      swap16(ehdr64->e_shnum);      \
	ehdr64->e_shstrndx =   swap16(ehdr64->e_shstrndx);   \

#define PHDR64_BSWAP(phdr64, swap16, swap32, swap64)         \
	phdr64->p_type =       swap32(phdr64->p_type);       \
	phdr64->p_flags =      swap32(phdr64->p_flags);      \
	phdr64->p_offset =     swap64(phdr64->p_offset);     \
	phdr64->p_vaddr =      swap64(phdr64->p_vaddr);      \
	phdr64->p_paddr =      swap64(phdr64->p_paddr);      \
	phdr64->p_filesz =     swap64(phdr64->p_filesz);     \
	phdr64->p_memsz =      swap64(phdr64->p_memsz);      \
	phdr64->p_align =      swap64(phdr64->p_align);

#define SHDR64_BSWAP(shdr64, swap16, swap32, swap64)         \
	shdr64->sh_name =      swap32(shdr64->sh_name);      \
	shdr64->sh_type =      swap32(shdr64->sh_type);      \
	shdr64->sh_flags =     swap64(shdr64->sh_flags);     \
	shdr64->sh_addr =      swap64(shdr64->sh_addr);      \
	shdr64->sh_offset =    swap64(shdr64->sh_offset);    \
	shdr64->sh_size =      swap64(shdr64->sh_size);      \
	shdr64->sh_link =      swap32(shdr64->sh_link);      \
	shdr64->sh_info =      swap32(shdr64->sh_info);      \
	shdr64->sh_addralign = swap64(shdr64->sh_addralign); \
	shdr64->sh_entsize =   swap64(shdr64->sh_entsize);

#define SYM64_BSWAP(sym64, swap16, swap32, swap64)           \
	sym64->st_name =       swap32(sym64->st_name);       \
	sym64->st_shndx =      swap32(sym64->st_shndx);      \
	sym64->st_value =      swap64(sym64->st_value);      \
	sym64->st_size =       swap64(sym64->st_size);

#define BSWAP_TEMPLATE(ei_data, endian, swap, data)          \
	switch (endian) { \
		case ELFENDIAN_HOST: \
			switch (ei_data) { \
				case ELFDATA2LSB: swap(data, le16toh, le32toh, le64toh); break; \
				case ELFDATA2MSB: swap(data, be16toh, be32toh, be64toh); break; \
				default: break; \
			} break; \
		case ELFENDIAN_TARGET: \
			switch (ei_data) { \
				case ELFDATA2LSB: swap(data, htole16, htole32, htole64); break; \
				case ELFDATA2MSB: swap(data, htobe16, htobe32, htobe64); break; \
				default: break; \
			} break; \
	}

#define EHDR_CONVERT(ehdr_a, ehdr_b)                         \
	ehdr_a->e_type =       ehdr_b->e_type;               \
	ehdr_a->e_machine =    ehdr_b->e_machine;            \
	ehdr_a->e_version =    ehdr_b->e_version;            \
	ehdr_a->e_entry =      ehdr_b->e_entry;              \
	ehdr_a->e_phoff =      ehdr_b->e_phoff;              \
	ehdr_a->e_shoff =      ehdr_b->e_shoff;              \
	ehdr_a->e_flags =      ehdr_b->e_flags;              \
	ehdr_a->e_ehsize =     ehdr_b->e_ehsize;             \
	ehdr_a->e_phentsize =  ehdr_b->e_phentsize;          \
	ehdr_a->e_phnum =      ehdr_b->e_phnum;              \
	ehdr_a->e_shentsize =  ehdr_b->e_shentsize;          \
	ehdr_a->e_shnum =      ehdr_b->e_shnum;              \
	ehdr_a->e_shstrndx =   ehdr_b->e_shstrndx;

#define PHDR_CONVERT(phdr_a, phdr_b)                         \
	phdr_a->p_type =       phdr_b->p_type;               \
	phdr_a->p_flags =      phdr_b->p_flags;              \
	phdr_a->p_offset =     phdr_b->p_offset;             \
	phdr_a->p_vaddr =      phdr_b->p_vaddr;              \
	phdr_a->p_paddr =      phdr_b->p_paddr;              \
	phdr_a->p_filesz =     phdr_b->p_filesz;             \
	phdr_a->p_memsz =      phdr_b->p_memsz;              \
	phdr_a->p_align =      phdr_b->p_align;

#define SHDR_CONVERT(shdr_a, shdr_b)                         \
	shdr_a->sh_name =      shdr_b->sh_name;              \
	shdr_a->sh_type =      shdr_b->sh_type;              \
	shdr_a->sh_flags =     shdr_b->sh_flags;             \
	shdr_a->sh_addr =      shdr_b->sh_addr;              \
	shdr_a->sh_offset =    shdr_b->sh_offset;            \
	shdr_a->sh_size =      shdr_b->sh_size;              \
	shdr_a->sh_link =      shdr_b->sh_link;              \
	shdr_a->sh_info =      shdr_b->sh_info;              \
	shdr_a->sh_addralign = shdr_b->sh_addralign;         \
	shdr_a->sh_entsize =   shdr_b->sh_entsize;

#define SYM_CONVERT(sym_a, sym_b)                            \
	sym_a->st_name =       sym_b->st_name;               \
	sym_a->st_value =      sym_b->st_value;              \
	sym_a->st_size =       sym_b->st_size;               \
	sym_a->st_info =       sym_b->st_info;               \
	sym_a->st_other =      sym_b->st_other;              \
	sym_a->st_shndx =      sym_b->st_shndx;

bool elf_check_magic(uint8_t *e_ident)
{
	return e_ident[EI_MAG0] == ELFMAG0 && e_ident[EI_MAG1] == ELFMAG1 &&
		e_ident[EI_MAG2] == ELFMAG2 && e_ident[EI_MAG3] == ELFMAG3;
}

void elf_bswap_ehdr32(Elf32_Ehdr *ehdr32, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, EHDR32_BSWAP, ehdr32);
}

void elf_bswap_phdr32(Elf32_Phdr *phdr32, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, PHDR32_BSWAP, phdr32);
}

void elf_bswap_shdr32(Elf32_Shdr *shdr32, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, SHDR32_BSWAP, shdr32);
}

void elf_bswap_sym32(Elf32_Sym *sym32, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, SYM32_BSWAP, sym32);
}

void elf_bswap_ehdr64(Elf64_Ehdr *ehdr64, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, EHDR64_BSWAP, ehdr64);
}

void elf_bswap_phdr64(Elf64_Phdr *phdr64, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, PHDR64_BSWAP, phdr64);
}

void elf_bswap_shdr64(Elf64_Shdr *shdr64, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, SHDR64_BSWAP, shdr64);
}

void elf_bswap_sym64(Elf64_Sym *sym64, int ei_data, ELFENDIAN endian)
{
	BSWAP_TEMPLATE(ei_data, endian, SYM64_BSWAP, sym64);
}

void elf_ehdr32_to_ehdr64(Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32)
{
	memcpy(ehdr64->e_ident, ehdr32->e_ident, EI_NIDENT);
	EHDR_CONVERT(ehdr64, ehdr32);
}

void elf_phdr32_to_phdr64(Elf64_Phdr *phdr64, Elf32_Phdr *phdr32)
{
	PHDR_CONVERT(phdr64, phdr32);
}

void elf_shdr32_to_shdr64(Elf64_Shdr *shdr64, Elf32_Shdr *shdr32)
{
	SHDR_CONVERT(shdr64, shdr32);
}

void elf_sym32_to_sym64(Elf64_Sym *sym64, Elf32_Sym *sym32)
{
	SYM_CONVERT(sym64, sym32);
}

void elf_ehdr64_to_ehdr32(Elf32_Ehdr *ehdr32, Elf64_Ehdr *ehdr64)
{
	memcpy(ehdr32->e_ident, ehdr64->e_ident, EI_NIDENT);
	EHDR_CONVERT(ehdr32, ehdr64);
}

void elf_phdr64_to_phdr32(Elf32_Phdr *phdr32, Elf64_Phdr *phdr64)
{
	PHDR_CONVERT(phdr32, phdr64);
}

void elf_shdr64_to_shdr32(Elf32_Shdr *shdr32, Elf64_Shdr *shdr64)
{
	SHDR_CONVERT(shdr32, shdr64);
}

void elf_sym64_to_sym32(Elf32_Sym *sym32, Elf64_Sym *sym64)
{
	SYM_CONVERT(sym32, sym64);
}
