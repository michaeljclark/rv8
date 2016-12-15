//
//  elf-file.h
//

#ifndef rv_elf_file_h
#define rv_elf_file_h

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
	std::map<size_t,std::vector<Elf64_Rela>> relocations;
	std::map<Elf64_Addr,size_t> addr_symbol_map;
	std::map<const char*,size_t,cmp_str> name_symbol_map;
	size_t shstrtab;
	size_t symtab;
	size_t strtab;
	std::vector<elf_section> sections;

	elf_file();
	elf_file(std::string filename);

	void clear();
	void load(std::string filename, bool headers_only = false);
	void save(std::string filename);

	void byteswap_symbol_table(ELFENDIAN endian);
	void copy_from_symbol_table_sections();
	void copy_to_symbol_table_sections();
	void recalculate_section_offsets();

	uint8_t* offset(size_t offset);
	elf_section* section(size_t offset);

	const char* shdr_name(size_t i);
	const char* sym_name(size_t i);
	const char* sym_name(const Elf64_Sym *sym);
	const Elf64_Sym* sym_by_nearest_addr(Elf64_Addr addr);
	const Elf64_Sym* sym_by_addr(Elf64_Addr addr);
	const Elf64_Sym* sym_by_name(const char *name);
	const size_t section_offset_by_type(Elf64_Word sh_type);

	void update_sym_addr(Elf64_Addr old_addr, Elf64_Addr new_addr);
};

#endif
