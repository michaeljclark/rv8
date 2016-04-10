//
//  riscv-elf-file.cc
//

#include <cstdint>
#include <cerrno>
#include <string>
#include <vector>

#include <sys/stat.h>

#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-util.h"

elf_file::elf_file(std::string filename) : filename(filename)
{
	struct stat stat_buf;

	// open file
	FILE *file = fopen(filename.c_str(), "r");
	if (!file) {
		panic("error fopen: %s: %s", filename.c_str(), strerror(errno));
	}

	// check file length
	if (fstat(fileno(file), &stat_buf) < 0) {
		fclose(file);
		panic("error fstat: %s: %s", filename.c_str(), strerror(errno));
	}

	// read magic
	if (stat_buf.st_size < EI_NIDENT) {
		fclose(file);
		panic("error invalid ELF file: %s", filename.c_str());
	}
	filesize = stat_buf.st_size;
	buf.resize(EI_NIDENT);
	size_t bytes_read = fread(buf.data(), 1, EI_NIDENT, file);
	if (bytes_read != EI_NIDENT || !elf_check_magic(buf.data())) {
		fclose(file);
		panic("error invalid ELF magic: %s", filename.c_str());
	}
	ei_class = buf[EI_CLASS];
	ei_data = buf[EI_DATA];

	// read remaining data
	buf.resize(filesize);
	bytes_read = fread(buf.data() + EI_NIDENT, 1, filesize - EI_NIDENT, file);
	if (bytes_read != (size_t)filesize - EI_NIDENT) {
		fclose(file);
		panic("error fread: %s", filename.c_str());
	}
	fclose(file);

	// byteswap and normalize file header
	switch (ei_class) {
		case ELFCLASS32:
			elf_bswap_ehdr32((Elf32_Ehdr*)buf.data(), ei_data);
			elf_convert_to_ehdr64(&ehdr, (Elf32_Ehdr*)buf.data());
			break;
		case ELFCLASS64:
			elf_bswap_ehdr64((Elf64_Ehdr*)buf.data(), ei_data);
			memcpy(&ehdr, (Elf64_Ehdr*)buf.data(), sizeof(Elf64_Ehdr));
			break;
		default:
			panic("error invalid ELF class: %s", filename.c_str());
	}

	// check header version
	if (ehdr.e_version != EV_CURRENT) {
		panic("error invalid ELF version: %s", filename.c_str());
	}

	// byteswap and normalize program and section headers
	switch (ei_class) {
		case ELFCLASS32:
			for (int i = 0; i < ehdr.e_phnum; i++) {
				Elf32_Phdr *phdr32 = (Elf32_Phdr*)(buf.data() + ehdr.e_phoff + i * sizeof(Elf32_Phdr));
				Elf64_Phdr phdr64;
				elf_bswap_phdr32(phdr32, ei_data);
				elf_convert_to_phdr64(&phdr64, phdr32);
				phdrs.push_back(phdr64);
			}
			for (int i = 0; i < ehdr.e_shnum; i++) {
				Elf32_Shdr *shdr32 = (Elf32_Shdr*)(buf.data() + ehdr.e_shoff + i * sizeof(Elf32_Shdr));
				Elf64_Shdr shdr64;
				elf_bswap_shdr32(shdr32, ei_data);
				elf_convert_to_shdr64(&shdr64, shdr32);
				shdrs.push_back(shdr64);
			}
			break;
		case ELFCLASS64:
			for (int i = 0; i < ehdr.e_phnum; i++) {
				Elf64_Phdr *phdr64 = (Elf64_Phdr*)(buf.data() + ehdr.e_phoff + i * sizeof(Elf64_Phdr));
				elf_bswap_phdr64(phdr64, ei_data);
				phdrs.push_back(*phdr64);
			}
			for (int i = 0; i < ehdr.e_shnum; i++) {
				Elf64_Shdr *shdr64 = (Elf64_Shdr*)(buf.data() + ehdr.e_shoff + i * sizeof(Elf64_Shdr));
				elf_bswap_shdr64(shdr64, ei_data);
				shdrs.push_back(*shdr64);
			}
			break;
	}

	// Find strtab and symtab
	shstrtab = symtab = strtab = nullptr;
	for (size_t i = 0; i < shdrs.size(); i++) {
		if (shstrtab == nullptr && shdrs[i].sh_type == SHT_STRTAB) {
			shstrtab = &shdrs[i];
		} else if (symtab == nullptr && shdrs[i].sh_type == SHT_SYMTAB) {
			symtab = &shdrs[i];
			if (shdrs[i].sh_link > 0) {
				strtab = &shdrs[shdrs[i].sh_link];
			}
		}
	}

	if (!symtab) return;

	// byteswap and normalize symbol table entries
	size_t num_symbols = symtab->sh_size / symtab->sh_entsize;
	switch (ei_class) {
		case ELFCLASS32:
			for (size_t i = 0; i < num_symbols; i++) {
				Elf32_Sym *sym32 = (Elf32_Sym*)(buf.data() + symtab->sh_offset + i * sizeof(Elf32_Sym));
				Elf64_Sym sym64;
				elf_bswap_sym32(sym32, ei_data);
				elf_convert_to_sym64(&sym64, sym32);
				symbols.push_back(sym64);
			}
			break;
		case ELFCLASS64:
			for (size_t i = 0; i < num_symbols; i++) {
				Elf64_Sym *sym64 = (Elf64_Sym*)(buf.data() + symtab->sh_offset + i * sizeof(Elf64_Sym));
				elf_bswap_sym64(sym64, ei_data);
				symbols.push_back(*sym64);
			}
			break;
	}
}
