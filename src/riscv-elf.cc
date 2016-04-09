//
//  riscv-elf.cc
//

#include <cstdint>
#include <cstring>

#include "riscv-elf.h"
#include "riscv-endian.h"

bool elf_check_magic(uint8_t *e_ident)
{
	return e_ident[EI_MAG0] == ELFMAG0 && e_ident[EI_MAG1] == ELFMAG1 &&
		e_ident[EI_MAG2] == ELFMAG2 && e_ident[EI_MAG3] == ELFMAG3;
}

void elf_bswap_ehdr32(Elf32_Ehdr *ehdr32, int ei_data)
{
	switch (ei_data) {
		case ELFDATA2LSB:
			ehdr32->e_type = le16toh(ehdr32->e_type);
			ehdr32->e_machine = le16toh(ehdr32->e_machine);
			ehdr32->e_version = le32toh(ehdr32->e_version);
			ehdr32->e_entry = le32toh(ehdr32->e_entry);
			ehdr32->e_phoff = le32toh(ehdr32->e_phoff);
			ehdr32->e_shoff = le32toh(ehdr32->e_shoff);
			ehdr32->e_flags = le32toh(ehdr32->e_flags);
			ehdr32->e_ehsize = le16toh(ehdr32->e_ehsize);
			ehdr32->e_phentsize = le16toh(ehdr32->e_phentsize);
			ehdr32->e_phnum = le16toh(ehdr32->e_phnum);
			ehdr32->e_shentsize = le16toh(ehdr32->e_shentsize);
			ehdr32->e_shnum = le16toh(ehdr32->e_shnum);
			ehdr32->e_shstrndx = le16toh(ehdr32->e_shstrndx);
			break;
		case ELFDATA2MSB:
			ehdr32->e_type = be16toh(ehdr32->e_type);
			ehdr32->e_machine = be16toh(ehdr32->e_machine);
			ehdr32->e_version = be32toh(ehdr32->e_version);
			ehdr32->e_entry = be32toh(ehdr32->e_entry);
			ehdr32->e_phoff = be32toh(ehdr32->e_phoff);
			ehdr32->e_shoff = be32toh(ehdr32->e_shoff);
			ehdr32->e_flags = be32toh(ehdr32->e_flags);
			ehdr32->e_ehsize = be16toh(ehdr32->e_ehsize);
			ehdr32->e_phentsize = be16toh(ehdr32->e_phentsize);
			ehdr32->e_phnum = be16toh(ehdr32->e_phnum);
			ehdr32->e_shentsize = be16toh(ehdr32->e_shentsize);
			ehdr32->e_shnum = be16toh(ehdr32->e_shnum);
			ehdr32->e_shstrndx = be16toh(ehdr32->e_shstrndx);
			break;
		default:
			break;
	}
}

void elf_bswap_phdr32(Elf32_Phdr *phdr32, int ei_data)
{
	switch (ei_data) {
		case ELFDATA2LSB:
			phdr32->p_type = le32toh(phdr32->p_type);
			phdr32->p_offset = le32toh(phdr32->p_offset);
			phdr32->p_vaddr = le32toh(phdr32->p_vaddr);
			phdr32->p_paddr = le32toh(phdr32->p_paddr);
			phdr32->p_filesz = le32toh(phdr32->p_filesz);
			phdr32->p_memsz = le32toh(phdr32->p_memsz);
			phdr32->p_flags = le32toh(phdr32->p_flags);
			phdr32->p_align = le32toh(phdr32->p_align);
			break;
		case ELFDATA2MSB:
			phdr32->p_type = be32toh(phdr32->p_type);
			phdr32->p_offset = be32toh(phdr32->p_offset);
			phdr32->p_vaddr = be32toh(phdr32->p_vaddr);
			phdr32->p_paddr = be32toh(phdr32->p_paddr);
			phdr32->p_filesz = be32toh(phdr32->p_filesz);
			phdr32->p_memsz = be32toh(phdr32->p_memsz);
			phdr32->p_flags = be32toh(phdr32->p_flags);
			phdr32->p_align = be32toh(phdr32->p_align);
			break;
		default:
			break;
	}
}

void elf_bswap_shdr32(Elf32_Shdr *shdr32, int ei_data)
{
	switch (ei_data) {
		case ELFDATA2LSB:
			shdr32->sh_name = le32toh(shdr32->sh_name);
			shdr32->sh_type = le32toh(shdr32->sh_type);
			shdr32->sh_flags = le32toh(shdr32->sh_flags);
			shdr32->sh_addr = le32toh(shdr32->sh_addr);
			shdr32->sh_offset = le32toh(shdr32->sh_offset);
			shdr32->sh_size = le32toh(shdr32->sh_size);
			shdr32->sh_link = le32toh(shdr32->sh_link);
			shdr32->sh_info = le32toh(shdr32->sh_info);
			shdr32->sh_addralign = le32toh(shdr32->sh_addralign);
			shdr32->sh_entsize = le32toh(shdr32->sh_entsize);
			break;
		case ELFDATA2MSB:
			shdr32->sh_name = be32toh(shdr32->sh_name);
			shdr32->sh_type = be32toh(shdr32->sh_type);
			shdr32->sh_flags = be32toh(shdr32->sh_flags);
			shdr32->sh_addr = be32toh(shdr32->sh_addr);
			shdr32->sh_offset = be32toh(shdr32->sh_offset);
			shdr32->sh_size = be32toh(shdr32->sh_size);
			shdr32->sh_link = be32toh(shdr32->sh_link);
			shdr32->sh_info = be32toh(shdr32->sh_info);
			shdr32->sh_addralign = be32toh(shdr32->sh_addralign);
			shdr32->sh_entsize = be32toh(shdr32->sh_entsize);
			break;
		default:
			break;
	}
}

void elf_bswap_ehdr64(Elf64_Ehdr *ehdr64, int ei_data)
{
	switch (ei_data) {
		case ELFDATA2LSB:
			ehdr64->e_type = le16toh(ehdr64->e_type);
			ehdr64->e_machine = le16toh(ehdr64->e_machine);
			ehdr64->e_version = le32toh(ehdr64->e_version);
			ehdr64->e_entry = le32toh(ehdr64->e_entry);
			ehdr64->e_phoff = le32toh(ehdr64->e_phoff);
			ehdr64->e_shoff = le32toh(ehdr64->e_shoff);
			ehdr64->e_flags = le32toh(ehdr64->e_flags);
			ehdr64->e_ehsize = le16toh(ehdr64->e_ehsize);
			ehdr64->e_phentsize = le16toh(ehdr64->e_phentsize);
			ehdr64->e_phnum = le16toh(ehdr64->e_phnum);
			ehdr64->e_shentsize = le16toh(ehdr64->e_shentsize);
			ehdr64->e_shnum = le16toh(ehdr64->e_shnum);
			ehdr64->e_shstrndx = le16toh(ehdr64->e_shstrndx);
			break;
		case ELFDATA2MSB:
			ehdr64->e_type = be16toh(ehdr64->e_type);
			ehdr64->e_machine = be16toh(ehdr64->e_machine);
			ehdr64->e_version = be32toh(ehdr64->e_version);
			ehdr64->e_entry = be64toh(ehdr64->e_entry);
			ehdr64->e_phoff = be64toh(ehdr64->e_phoff);
			ehdr64->e_shoff = be64toh(ehdr64->e_shoff);
			ehdr64->e_flags = be32toh(ehdr64->e_flags);
			ehdr64->e_ehsize = be16toh(ehdr64->e_ehsize);
			ehdr64->e_phentsize = be16toh(ehdr64->e_phentsize);
			ehdr64->e_phnum = be16toh(ehdr64->e_phnum);
			ehdr64->e_shentsize = be16toh(ehdr64->e_shentsize);
			ehdr64->e_shnum = be16toh(ehdr64->e_shnum);
			ehdr64->e_shstrndx = be16toh(ehdr64->e_shstrndx);
			break;
		default:
			break;
	}
}

void elf_bswap_phdr64(Elf64_Phdr *phdr64, int ei_data)
{
	switch (ei_data) {
		case ELFDATA2LSB:
			phdr64->p_type = le32toh(phdr64->p_type);
			phdr64->p_flags = le32toh(phdr64->p_flags);
			phdr64->p_offset = le64toh(phdr64->p_offset);
			phdr64->p_vaddr = le64toh(phdr64->p_vaddr);
			phdr64->p_paddr = le64toh(phdr64->p_paddr);
			phdr64->p_filesz = le64toh(phdr64->p_filesz);
			phdr64->p_memsz = le64toh(phdr64->p_memsz);
			phdr64->p_align = le64toh(phdr64->p_align);
			break;
		case ELFDATA2MSB:
			phdr64->p_type = be32toh(phdr64->p_type);
			phdr64->p_flags = be32toh(phdr64->p_flags);
			phdr64->p_offset = be64toh(phdr64->p_offset);
			phdr64->p_vaddr = be64toh(phdr64->p_vaddr);
			phdr64->p_paddr = be64toh(phdr64->p_paddr);
			phdr64->p_filesz = be64toh(phdr64->p_filesz);
			phdr64->p_memsz = be64toh(phdr64->p_memsz);
			phdr64->p_align = be64toh(phdr64->p_align);
			break;
		default:
			break;
	}
}

void elf_bswap_shdr64(Elf64_Shdr *shdr64, int ei_data)
{
	switch (ei_data) {
		case ELFDATA2LSB:
			shdr64->sh_name = le32toh(shdr64->sh_name);
			shdr64->sh_type = le32toh(shdr64->sh_type);
			shdr64->sh_flags = le64toh(shdr64->sh_flags);
			shdr64->sh_addr = le64toh(shdr64->sh_addr);
			shdr64->sh_offset = le64toh(shdr64->sh_offset);
			shdr64->sh_size = le64toh(shdr64->sh_size);
			shdr64->sh_link = le32toh(shdr64->sh_link);
			shdr64->sh_info = le32toh(shdr64->sh_info);
			shdr64->sh_addralign = le64toh(shdr64->sh_addralign);
			shdr64->sh_entsize = le64toh(shdr64->sh_entsize);
			break;
		case ELFDATA2MSB:
			shdr64->sh_name = be32toh(shdr64->sh_name);
			shdr64->sh_type = be32toh(shdr64->sh_type);
			shdr64->sh_flags = be64toh(shdr64->sh_flags);
			shdr64->sh_addr = be64toh(shdr64->sh_addr);
			shdr64->sh_offset = be64toh(shdr64->sh_offset);
			shdr64->sh_size = be64toh(shdr64->sh_size);
			shdr64->sh_link = be32toh(shdr64->sh_link);
			shdr64->sh_info = be32toh(shdr64->sh_info);
			shdr64->sh_addralign = be64toh(shdr64->sh_addralign);
			shdr64->sh_entsize = be64toh(shdr64->sh_entsize);
			break;
	}
}

void elf_convert_to_ehdr64(Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32)
{
	memcpy(ehdr64->e_ident, ehdr32->e_ident, EI_NIDENT);
	ehdr64->e_type = ehdr32->e_type;
	ehdr64->e_machine = ehdr32->e_machine;
	ehdr64->e_version = ehdr32->e_version;
	ehdr64->e_entry = ehdr32->e_entry;
	ehdr64->e_phoff = ehdr32->e_phoff;
	ehdr64->e_shoff = ehdr32->e_shoff;
	ehdr64->e_flags = ehdr32->e_flags;
	ehdr64->e_ehsize = ehdr32->e_ehsize;
	ehdr64->e_phentsize = ehdr32->e_phentsize;
	ehdr64->e_phnum = ehdr32->e_phnum;
	ehdr64->e_shentsize = ehdr32->e_shentsize;
	ehdr64->e_shnum = ehdr32->e_shnum;
	ehdr64->e_shstrndx = ehdr32->e_shstrndx;
}

void elf_convert_to_phdr64(Elf64_Phdr *phdr64, Elf32_Phdr *phdr32)
{
	phdr64->p_type = phdr32->p_type;
	phdr64->p_flags = phdr32->p_flags;
	phdr64->p_offset = phdr32->p_offset;
	phdr64->p_vaddr = phdr32->p_vaddr;
	phdr64->p_paddr = phdr32->p_paddr;
	phdr64->p_filesz = phdr32->p_filesz;
	phdr64->p_memsz = phdr32->p_memsz;
	phdr64->p_align = phdr32->p_align;
}

void elf_convert_to_shdr64(Elf64_Shdr *shdr64, Elf32_Shdr *shdr32)
{
	shdr64->sh_name = shdr32->sh_name;
	shdr64->sh_type = shdr32->sh_type;
	shdr64->sh_flags = shdr32->sh_flags;
	shdr64->sh_addr = shdr32->sh_addr;
	shdr64->sh_offset = shdr32->sh_offset;
	shdr64->sh_size = shdr32->sh_size;
	shdr64->sh_link = shdr32->sh_link;
	shdr64->sh_info = shdr32->sh_info;
	shdr64->sh_addralign = shdr32->sh_addralign;
	shdr64->sh_entsize = shdr32->sh_entsize;
}
