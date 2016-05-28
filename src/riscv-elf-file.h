//
//  riscv-elf-file.h
//

#ifndef riscv_elf_file_h
#define riscv_elf_file_h

struct cmp_str {
	bool operator()(char const *a, char const *b) const { return std::strcmp(a, b) < 0; }
};

struct elf_section
{
	size_t offset;
	size_t size;
	std::vector<uint8_t> buf;
};

struct elf_file
{
	std::string filename;
	ssize_t filesize;
	int ei_class;
	int ei_data;

	Elf64_Ehdr ehdr;
	std::vector<Elf64_Phdr> phdrs;
	std::vector<Elf64_Shdr> shdrs;
	std::vector<Elf64_Sym> symbols;
	std::map<Elf64_Addr,size_t> addr_symbol_map;
	std::map<const char*,size_t,cmp_str> name_symbol_map;
	Elf64_Shdr *shstrtab;
	Elf64_Shdr *symtab;
	Elf64_Shdr *strtab;
	std::vector<elf_section> sections;

	elf_file();
	elf_file(std::string filename);

	void clear();
	void load(std::string filename);
	uint8_t* offset(size_t offset);
	elf_section* section(size_t offset);

	const char* shdr_name(int i);
	const char* sym_name(int i);
	const char* sym_name(const Elf64_Sym *sym);
	const Elf64_Sym* sym_by_nearest_addr(Elf64_Addr addr);
	const Elf64_Sym* sym_by_addr(Elf64_Addr addr);
	const Elf64_Sym* sym_by_name(const char *name);
};

#endif
