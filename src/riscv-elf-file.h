//
//  riscv-elf-file.h
//

#ifndef riscv_elf_file_h
#define riscv_elf_file_h

struct elf_file
{
	std::string filename;
	std::vector<uint8_t> buf;
	ssize_t filesize;
	int ei_class;
	int ei_data;

	Elf64_Ehdr ehdr;
	std::vector<Elf64_Phdr> phdrs;
	std::vector<Elf64_Shdr> shdrs;
	std::vector<Elf64_Sym> symbols;
	Elf64_Shdr *shstrtab;
	Elf64_Shdr *symtab;
	Elf64_Shdr *strtab;

	elf_file(std::string filename);
};

#endif
