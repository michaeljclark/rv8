//
//  elf-format.h
//

#ifndef rv_elf_format_h
#define rv_elf_format_h

typedef std::function<const char*(const char *type)> elf_symbol_colorize_fn;

const char* elf_null_symbol_colorize(const char *type);
const char* elf_ei_class_name(int v);
const char* elf_ei_data_name(int v);
const char* elf_e_type_name(int v);
const char* elf_e_machine_name(int v);
const char* elf_p_type_name(int v);
const std::string elf_p_flags_name(int v);
const char* elf_sh_type_name(int v);
const std::string elf_sh_shndx_name(int v);
const std::string elf_sh_flags_name(int v);
const char* elf_st_bind_name(int v);
const char* elf_st_type_name(int v);
const char* elf_st_other_name(int v);
const char* elf_rela_type_name(int v);
const std::string elf_file_info(elf_file &elf);
const std::string elf_phdr_info(elf_file &elf, int i,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
const std::string elf_shdr_info(elf_file &elf, int i,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
const std::string elf_sym_info(elf_file &elf, int i,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
void elf_print_header_info(elf_file &elf,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
void elf_print_header_ext_info(elf_file &elf,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
void elf_print_section_headers(elf_file &elf,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
void elf_print_program_headers(elf_file &elf,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
void elf_print_symbol_table(elf_file &elf,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);
void elf_print_relocations(elf_file &elf,
	elf_symbol_colorize_fn colorize = elf_null_symbol_colorize);

#endif
