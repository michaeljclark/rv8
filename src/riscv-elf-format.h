//
//  riscv-elf-format.h
//

#ifndef riscv_elf_format_h
#define riscv_elf_format_h

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
const std::string elf_file_info(elf_file &elf);
const std::string elf_phdr_info(elf_file &elf, int i);
const std::string elf_shdr_info(elf_file &elf, int i);
const std::string elf_sym_info(elf_file &elf, int i);
const char* elf_shdr_name(elf_file &elf, int i);
const char* elf_sym_name(elf_file &elf, int i);
const char* elf_sym_name(elf_file &elf, const Elf64_Sym *sym);
const Elf64_Sym* elf_sym(elf_file &elf, Elf64_Addr addr);
void elf_print_info(elf_file &elf);

#endif