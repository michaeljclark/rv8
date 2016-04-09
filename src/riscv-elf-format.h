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
const std::string elf_sh_flags_name(int v);
const char* elf_st_info_name(int v);
const std::string elf_file_info(elf_file &elf);
const std::string elf_phdr_info(Elf64_Phdr &phdr);
const std::string elf_shdr_info(Elf64_Shdr &shdr);
void elf_print_info(elf_file &elf);

#endif