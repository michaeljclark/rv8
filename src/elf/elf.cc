//
//  elf.cc
//

#include <cstdint>
#include <cstring>

#include "elf.h"
#include "host-endian.h"

struct ehdr32_bswap
{
	typedef Elf32_Ehdr type_name;

	template <typename S> static void bswap(type_name *ehdr32)
	{
		ehdr32->e_type =       S::bswap(ehdr32->e_type);
		ehdr32->e_machine =    S::bswap(ehdr32->e_machine);
		ehdr32->e_version =    S::bswap(ehdr32->e_version);
		ehdr32->e_entry =      S::bswap(ehdr32->e_entry);
		ehdr32->e_phoff =      S::bswap(ehdr32->e_phoff);
		ehdr32->e_shoff =      S::bswap(ehdr32->e_shoff);
		ehdr32->e_flags =      S::bswap(ehdr32->e_flags);
		ehdr32->e_ehsize =     S::bswap(ehdr32->e_ehsize);
		ehdr32->e_phentsize =  S::bswap(ehdr32->e_phentsize);
		ehdr32->e_phnum =      S::bswap(ehdr32->e_phnum);
		ehdr32->e_shentsize =  S::bswap(ehdr32->e_shentsize);
		ehdr32->e_shnum =      S::bswap(ehdr32->e_shnum);
		ehdr32->e_shstrndx =   S::bswap(ehdr32->e_shstrndx);
	}
};

struct phdr32_bswap
{
	typedef Elf32_Phdr type_name;

	template <typename S> static void bswap(type_name *phdr32)
	{
		phdr32->p_type =       S::bswap(phdr32->p_type);
		phdr32->p_offset =     S::bswap(phdr32->p_offset);
		phdr32->p_vaddr =      S::bswap(phdr32->p_vaddr);
		phdr32->p_paddr =      S::bswap(phdr32->p_paddr);
		phdr32->p_filesz =     S::bswap(phdr32->p_filesz);
		phdr32->p_memsz =      S::bswap(phdr32->p_memsz);
		phdr32->p_flags =      S::bswap(phdr32->p_flags);
		phdr32->p_align =      S::bswap(phdr32->p_align);
	}
};

struct shdr32_bswap
{
	typedef Elf32_Shdr type_name;

	template <typename S> static void bswap(type_name *shdr32)
	{
		shdr32->sh_name =      S::bswap(shdr32->sh_name);
		shdr32->sh_type =      S::bswap(shdr32->sh_type);
		shdr32->sh_flags =     S::bswap(shdr32->sh_flags);
		shdr32->sh_addr =      S::bswap(shdr32->sh_addr);
		shdr32->sh_offset =    S::bswap(shdr32->sh_offset);
		shdr32->sh_size =      S::bswap(shdr32->sh_size);
		shdr32->sh_link =      S::bswap(shdr32->sh_link);
		shdr32->sh_info =      S::bswap(shdr32->sh_info);
		shdr32->sh_addralign = S::bswap(shdr32->sh_addralign);
		shdr32->sh_entsize =   S::bswap(shdr32->sh_entsize);
	}
};

struct sym32_bswap
{
	typedef Elf32_Sym type_name;

	template <typename S> static void bswap(type_name *sym32)
	{
		sym32->st_name =       S::bswap(sym32->st_name);
		sym32->st_value =      S::bswap(sym32->st_value);
		sym32->st_size =       S::bswap(sym32->st_size);
		sym32->st_shndx =      S::bswap(sym32->st_shndx);
	}
};

struct rela32_bswap
{
	typedef Elf32_Rela type_name;

	template <typename S> static void bswap(type_name *rela32)
	{
		rela32->r_offset =     S::bswap(rela32->r_offset);
		rela32->r_info =       S::bswap(rela32->r_info);
	}
};

struct ehdr64_bswap
{
	typedef Elf64_Ehdr type_name;

	template <typename S> static void bswap(type_name *ehdr64)
	{
		ehdr64->e_type =       S::bswap(ehdr64->e_type);
		ehdr64->e_machine =    S::bswap(ehdr64->e_machine);
		ehdr64->e_version =    S::bswap(ehdr64->e_version);
		ehdr64->e_entry =      S::bswap(ehdr64->e_entry);
		ehdr64->e_phoff =      S::bswap(ehdr64->e_phoff);
		ehdr64->e_shoff =      S::bswap(ehdr64->e_shoff);
		ehdr64->e_flags =      S::bswap(ehdr64->e_flags);
		ehdr64->e_ehsize =     S::bswap(ehdr64->e_ehsize);
		ehdr64->e_phentsize =  S::bswap(ehdr64->e_phentsize);
		ehdr64->e_phnum =      S::bswap(ehdr64->e_phnum);
		ehdr64->e_shentsize =  S::bswap(ehdr64->e_shentsize);
		ehdr64->e_shnum =      S::bswap(ehdr64->e_shnum);
		ehdr64->e_shstrndx =   S::bswap(ehdr64->e_shstrndx);
	}
};

struct phdr64_bswap
{
	typedef Elf64_Phdr type_name;

	template <typename S> static void bswap(type_name *phdr64)
	{
		phdr64->p_type =       S::bswap(phdr64->p_type);
		phdr64->p_flags =      S::bswap(phdr64->p_flags);
		phdr64->p_offset =     S::bswap(phdr64->p_offset);
		phdr64->p_vaddr =      S::bswap(phdr64->p_vaddr);
		phdr64->p_paddr =      S::bswap(phdr64->p_paddr);
		phdr64->p_filesz =     S::bswap(phdr64->p_filesz);
		phdr64->p_memsz =      S::bswap(phdr64->p_memsz);
		phdr64->p_align =      S::bswap(phdr64->p_align);
	}
};

struct shdr64_bswap
{
	typedef Elf64_Shdr type_name;

	template <typename S> static void bswap(type_name *shdr64)
	{
		shdr64->sh_name =      S::bswap(shdr64->sh_name);
		shdr64->sh_type =      S::bswap(shdr64->sh_type);
		shdr64->sh_flags =     S::bswap(shdr64->sh_flags);
		shdr64->sh_addr =      S::bswap(shdr64->sh_addr);
		shdr64->sh_offset =    S::bswap(shdr64->sh_offset);
		shdr64->sh_size =      S::bswap(shdr64->sh_size);
		shdr64->sh_link =      S::bswap(shdr64->sh_link);
		shdr64->sh_info =      S::bswap(shdr64->sh_info);
		shdr64->sh_addralign = S::bswap(shdr64->sh_addralign);
		shdr64->sh_entsize =   S::bswap(shdr64->sh_entsize);
	}
};

struct sym64_bswap
{
	typedef Elf64_Sym type_name;

	template <typename S> static void bswap(type_name *sym64)
	{
		sym64->st_name =       S::bswap(sym64->st_name);
		sym64->st_shndx =      S::bswap(sym64->st_shndx);
		sym64->st_value =      S::bswap(sym64->st_value);
		sym64->st_size =       S::bswap(sym64->st_size);
	}
};

struct rela64_bswap
{
	typedef Elf64_Rela type_name;

	template <typename S> static void bswap(type_name *rela64)
	{
		rela64->r_offset =     S::bswap(rela64->r_offset);
		rela64->r_info =       S::bswap(rela64->r_info);
		rela64->r_addend =     S::bswap(rela64->r_addend);
	}
};

template <typename T> void do_swap(int ei_data, int endian, typename T::type_name *data)
{
	switch (endian) {
		case ELFENDIAN_HOST:
			switch (ei_data) {
				case ELFDATA2LSB: T::template bswap<bswap_letoh>(data); break;
				case ELFDATA2MSB: T::template bswap<bswap_betoh>(data); break;
				default: break;
			} break;
		case ELFENDIAN_TARGET:
			switch (ei_data) {
				case ELFDATA2LSB: T::template bswap<bswap_htole>(data); break;
				case ELFDATA2MSB: T::template bswap<bswap_htobe>(data); break;
				default: break;
			} break;
	}
};

template <typename A, typename B> void ehdr_convert(A *ehdr_a, B *ehdr_b)
{
	ehdr_a->e_type =       ehdr_b->e_type;
	ehdr_a->e_machine =    ehdr_b->e_machine;
	ehdr_a->e_version =    ehdr_b->e_version;
	ehdr_a->e_entry =      ehdr_b->e_entry;
	ehdr_a->e_phoff =      ehdr_b->e_phoff;
	ehdr_a->e_shoff =      ehdr_b->e_shoff;
	ehdr_a->e_flags =      ehdr_b->e_flags;
	ehdr_a->e_ehsize =     ehdr_b->e_ehsize;
	ehdr_a->e_phentsize =  ehdr_b->e_phentsize;
	ehdr_a->e_phnum =      ehdr_b->e_phnum;
	ehdr_a->e_shentsize =  ehdr_b->e_shentsize;
	ehdr_a->e_shnum =      ehdr_b->e_shnum;
	ehdr_a->e_shstrndx =   ehdr_b->e_shstrndx;
}

template <typename A, typename B> void phdr_convert(A *phdr_a, B *phdr_b)
{
	phdr_a->p_type =       phdr_b->p_type;
	phdr_a->p_flags =      phdr_b->p_flags;
	phdr_a->p_offset =     phdr_b->p_offset;
	phdr_a->p_vaddr =      phdr_b->p_vaddr;
	phdr_a->p_paddr =      phdr_b->p_paddr;
	phdr_a->p_filesz =     phdr_b->p_filesz;
	phdr_a->p_memsz =      phdr_b->p_memsz;
	phdr_a->p_align =      phdr_b->p_align;
}

template <typename A, typename B> void shdr_convert(A *shdr_a, B *shdr_b)
{
	shdr_a->sh_name =      shdr_b->sh_name;
	shdr_a->sh_type =      shdr_b->sh_type;
	shdr_a->sh_flags =     shdr_b->sh_flags;
	shdr_a->sh_addr =      shdr_b->sh_addr;
	shdr_a->sh_offset =    shdr_b->sh_offset;
	shdr_a->sh_size =      shdr_b->sh_size;
	shdr_a->sh_link =      shdr_b->sh_link;
	shdr_a->sh_info =      shdr_b->sh_info;
	shdr_a->sh_addralign = shdr_b->sh_addralign;
	shdr_a->sh_entsize =   shdr_b->sh_entsize;
}

template <typename A, typename B> void sym_convert(A *sym_a, B *sym_b)
{
	sym_a->st_name =       sym_b->st_name;
	sym_a->st_value =      sym_b->st_value;
	sym_a->st_size =       sym_b->st_size;
	sym_a->st_info =       sym_b->st_info;
	sym_a->st_other =      sym_b->st_other;
	sym_a->st_shndx =      sym_b->st_shndx;
}

template <typename A, typename B> void rela_convert_32_64(A *rela_a, B *rela_b)
{
	rela_a->r_offset = rela_b->r_offset;
	rela_a->r_info = ELF64_R_INFO(ELF32_R_SYM(rela_b->r_info), ELF32_R_TYPE(rela_b->r_info));
	rela_a->r_addend = 0;
}

template <typename A, typename B> void rela_convert_64_32(A *rela_a, B *rela_b)
{
	rela_a->r_offset = rela_b->r_offset;
	rela_a->r_info = ELF32_R_INFO(ELF64_R_SYM(rela_b->r_info), ELF64_R_TYPE(rela_b->r_info));
}

bool elf_check_magic(uint8_t *e_ident)
{
	return e_ident[EI_MAG0] == ELFMAG0 && e_ident[EI_MAG1] == ELFMAG1 &&
		e_ident[EI_MAG2] == ELFMAG2 && e_ident[EI_MAG3] == ELFMAG3;
}

void elf_bswap_ehdr32(Elf32_Ehdr *ehdr32, int ei_data, ELFENDIAN endian)
{
	do_swap<ehdr32_bswap>(ei_data, endian, ehdr32);
}

void elf_bswap_phdr32(Elf32_Phdr *phdr32, int ei_data, ELFENDIAN endian)
{
	do_swap<phdr32_bswap>(ei_data, endian, phdr32);
}

void elf_bswap_shdr32(Elf32_Shdr *shdr32, int ei_data, ELFENDIAN endian)
{
	do_swap<shdr32_bswap>(ei_data, endian, shdr32);
}

void elf_bswap_sym32(Elf32_Sym *sym32, int ei_data, ELFENDIAN endian)
{
	do_swap<sym32_bswap>(ei_data, endian, sym32);
}

void elf_bswap_rela32(Elf32_Rela *rela32, int ei_data, ELFENDIAN endian)
{
	do_swap<rela32_bswap>(ei_data, endian, rela32);
}

void elf_bswap_ehdr64(Elf64_Ehdr *ehdr64, int ei_data, ELFENDIAN endian)
{
	do_swap<ehdr64_bswap>(ei_data, endian, ehdr64);
}

void elf_bswap_phdr64(Elf64_Phdr *phdr64, int ei_data, ELFENDIAN endian)
{
	do_swap<phdr64_bswap>(ei_data, endian, phdr64);
}

void elf_bswap_shdr64(Elf64_Shdr *shdr64, int ei_data, ELFENDIAN endian)
{
	do_swap<shdr64_bswap>(ei_data, endian, shdr64);
}

void elf_bswap_sym64(Elf64_Sym *sym64, int ei_data, ELFENDIAN endian)
{
	do_swap<sym64_bswap>(ei_data, endian, sym64);
}

void elf_bswap_rela64(Elf64_Rela *rela64, int ei_data, ELFENDIAN endian)
{
	do_swap<rela64_bswap>(ei_data, endian, rela64);
}

void elf_ehdr32_to_ehdr64(Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32)
{
	memcpy(ehdr64->e_ident, ehdr32->e_ident, EI_NIDENT);
	ehdr_convert(ehdr64, ehdr32);
}

void elf_phdr32_to_phdr64(Elf64_Phdr *phdr64, Elf32_Phdr *phdr32)
{
	phdr_convert(phdr64, phdr32);
}

void elf_shdr32_to_shdr64(Elf64_Shdr *shdr64, Elf32_Shdr *shdr32)
{
	shdr_convert(shdr64, shdr32);
}

void elf_sym32_to_sym64(Elf64_Sym *sym64, Elf32_Sym *sym32)
{
	sym_convert(sym64, sym32);
}

void elf_rela32_to_rela64(Elf64_Rela *rela64, Elf32_Rela *rela32)
{
	rela_convert_32_64(rela64, rela32);
}

void elf_ehdr64_to_ehdr32(Elf32_Ehdr *ehdr32, Elf64_Ehdr *ehdr64)
{
	memcpy(ehdr32->e_ident, ehdr64->e_ident, EI_NIDENT);
	ehdr_convert(ehdr32, ehdr64);
}

void elf_phdr64_to_phdr32(Elf32_Phdr *phdr32, Elf64_Phdr *phdr64)
{
	phdr_convert(phdr32, phdr64);
}

void elf_shdr64_to_shdr32(Elf32_Shdr *shdr32, Elf64_Shdr *shdr64)
{
	shdr_convert(shdr32, shdr64);
}

void elf_sym64_to_sym32(Elf32_Sym *sym32, Elf64_Sym *sym64)
{
	sym_convert(sym32, sym64);
}

void elf_rela64_to_rela32(Elf32_Rela *rela64, Elf64_Rela *rela32)
{
	rela_convert_64_32(rela32, rela64);
}
