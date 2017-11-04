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
	std::string name;
	size_t offset;
	size_t size;
	std::vector<uint8_t> buf;
};

enum elf_load
{
	elf_load_all,
	elf_load_headers,
	elf_load_exec,
};

struct elf_file
{
	std::string filename;
	std::string interp;
	ssize_t filesize;
	int ei_class;
	int ei_data;

	Elf64_Ehdr ehdr;
	std::vector<Elf64_Phdr> phdrs;
	std::vector<Elf64_Shdr> shdrs;
	std::vector<Elf64_Sym> symbols;
	std::vector<Elf64_Rela> relocations;
	std::map<Elf64_Addr,size_t> addr_symbol_map;
	std::map<const char*,size_t,cmp_str> name_symbol_map;
	std::vector<elf_section> sections;

	size_t text;
	size_t rela_text;
	size_t data;
	size_t bss;
	size_t rodata;
	size_t shstrtab;
	size_t symtab;
	size_t strtab;

	elf_file();
	elf_file(std::string filename);

	void clear();

	void init_object(int ei_class);
	size_t add_section(std::string name, Elf64_Word sh_type, Elf64_Xword sh_flags,
		Elf64_Xword sh_addralign);
	size_t add_symbol(std::string name, Elf32_Word st_bind, Elf32_Word st_type,
		Elf64_Byte st_other, Elf64_Half st_shndx, Elf64_Addr st_value);
	size_t add_reloc(Elf64_Addr r_offset, Elf64_Xword r_sym,
		Elf64_Xword r_type, Elf64_Sxword r_addend);
	size_t section_num(std::string name);

	void load(std::string filename, elf_load load_type = elf_load_all);
	void save(std::string filename);

	void byteswap_symbol_table(ELFENDIAN endian);
	void copy_from_section_names();
	void copy_to_section_names();
	void copy_from_relocation_table_sections();
	void copy_to_relocation_table_sections();
	void copy_from_symbol_table_sections();
	void copy_to_symbol_table_sections();
	void recalculate_section_offsets();

	uint8_t* offset(size_t offset);
	elf_section* section(size_t offset);

	const char* interp_name();
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
