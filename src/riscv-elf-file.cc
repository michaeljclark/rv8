//
//  riscv-elf-file.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include <sys/stat.h>

#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-util.h"


elf_file::elf_file() {}

elf_file::elf_file(std::string filename)
{
	load(filename);
}

void elf_file::clear()
{
	filename = "";
	filesize = 0;
	ei_class = ELFCLASSNONE;
	ei_data = ELFDATANONE;

	memset(&ehdr, 0, sizeof(ehdr));
	phdrs.resize(0);
	shdrs.resize(0);
	symbols.resize(0);
	addr_symbol_map.clear();
	name_symbol_map.clear();
	shstrtab = nullptr;
	symtab = nullptr;
	strtab = nullptr;
	sections.resize(0);
}

void elf_file::load(std::string filename)
{
	FILE *file;
	struct stat stat_buf;
	std::vector<uint8_t> buf;

	// clear current data
	clear();

	// open file
	this->filename = filename;
	file = fopen(filename.c_str(), "r");
	if (!file) {
		panic("error fopen: %s: %s", filename.c_str(), strerror(errno));
	}

	// check file length
	if (fstat(fileno(file), &stat_buf) < 0) {
		fclose(file);
		panic("error fstat: %s: %s", filename.c_str(), strerror(errno));
	}

	// read file magic
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

	// read, byteswap and normalize file header
	switch (ei_class) {
		case ELFCLASS32: buf.resize(sizeof(Elf32_Ehdr)); break;
		case ELFCLASS64: buf.resize(sizeof(Elf64_Ehdr)); break;
		default:
			fclose(file);
			panic("error invalid ELF class: %s", filename.c_str());
	}
	fseek(file, 0, SEEK_SET);
	if (fread(buf.data(), 1, buf.size(), file) != buf.size()) {
		fclose(file);
		panic("error fread: %s", filename.c_str());
	}
	switch (ei_class) {
		case ELFCLASS32:
			elf_bswap_ehdr32((Elf32_Ehdr*)buf.data(), ei_data, ELFENDIAN_HOST);
			elf_ehdr32_to_ehdr64(&ehdr, (Elf32_Ehdr*)buf.data());
			break;
		case ELFCLASS64:
			elf_bswap_ehdr64((Elf64_Ehdr*)buf.data(), ei_data, ELFENDIAN_HOST);
			memcpy(&ehdr, (Elf64_Ehdr*)buf.data(), sizeof(Elf64_Ehdr));
			break;
	}

	// check header version
	if (ehdr.e_version != EV_CURRENT) {
		fclose(file);
		panic("error invalid ELF version: %s", filename.c_str());
	}

	// read, byteswap and normalize program and section headers
	switch (ei_class) {
		case ELFCLASS32:
			buf.resize(sizeof(Elf32_Phdr));
			for (int i = 0; i < ehdr.e_phnum; i++) {
				fseek(file, ehdr.e_phoff + i * sizeof(Elf32_Phdr), SEEK_SET);
				if (fread(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fread: %s", filename.c_str());
				}
				Elf32_Phdr *phdr32 = (Elf32_Phdr*)buf.data();
				Elf64_Phdr phdr64;
				elf_bswap_phdr32(phdr32, ei_data, ELFENDIAN_HOST);
				elf_phdr32_to_phdr64(&phdr64, phdr32);
				phdrs.push_back(phdr64);
			}
			buf.resize(sizeof(Elf32_Shdr));
			for (int i = 0; i < ehdr.e_shnum; i++) {
				fseek(file, ehdr.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
				if (fread(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fread: %s", filename.c_str());
				}
				Elf32_Shdr *shdr32 = (Elf32_Shdr*)buf.data();
				Elf64_Shdr shdr64;
				elf_bswap_shdr32(shdr32, ei_data, ELFENDIAN_HOST);
				elf_shdr32_to_shdr64(&shdr64, shdr32);
				shdrs.push_back(shdr64);
			}
			break;
		case ELFCLASS64:
			buf.resize(sizeof(Elf64_Phdr));
			for (int i = 0; i < ehdr.e_phnum; i++) {
				fseek(file, ehdr.e_phoff + i * sizeof(Elf64_Phdr), SEEK_SET);
				if (fread(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fread: %s", filename.c_str());
				}
				Elf64_Phdr *phdr64 = (Elf64_Phdr*)buf.data();
				elf_bswap_phdr64(phdr64, ei_data, ELFENDIAN_HOST);
				phdrs.push_back(*phdr64);
			}
			buf.resize(sizeof(Elf64_Shdr));
			for (int i = 0; i < ehdr.e_shnum; i++) {
				fseek(file, ehdr.e_shoff + i * sizeof(Elf64_Shdr), SEEK_SET);
				if (fread(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fread: %s", filename.c_str());
				}
				Elf64_Shdr *shdr64 = (Elf64_Shdr*)buf.data();
				elf_bswap_shdr64(shdr64, ei_data, ELFENDIAN_HOST);
				shdrs.push_back(*shdr64);
			}
			break;
	}

	// Find strtab and symtab
	shstrtab = symtab = strtab = nullptr;
	for (size_t i = 0; i < shdrs.size(); i++) {
		if (shstrtab == nullptr && shdrs[i].sh_type == SHT_STRTAB && ehdr.e_shstrndx == i) {
			shstrtab = &shdrs[i];
		} else if (symtab == nullptr && shdrs[i].sh_type == SHT_SYMTAB) {
			symtab = &shdrs[i];
			if (shdrs[i].sh_link > 0) {
				strtab = &shdrs[shdrs[i].sh_link];
			}
		}
	}

	// read section data into buffers
	sections.resize(shdrs.size());
	for (size_t i = 0; i < shdrs.size(); i++) {
		sections[i].offset = shdrs[i].sh_offset;
		sections[i].size = shdrs[i].sh_size;
		if (shdrs[i].sh_type == SHT_NOBITS) continue;
		fseek(file, shdrs[i].sh_offset, SEEK_SET);
		sections[i].buf.resize(shdrs[i].sh_size);
		if (fread(sections[i].buf.data(), 1, shdrs[i].sh_size, file) != shdrs[i].sh_size) {
			fclose(file);
			panic("error fread: %s", filename.c_str());
		}
	}
	fclose(file);
	buf.resize(0);

	// byteswap symbol table
	byteswap_symbol_table(ELFENDIAN_HOST);

	// update symbol maps
	copy_from_symbol_table_sections();
}

void elf_file::save(std::string filename)
{
	FILE *file;
	std::vector<uint8_t> buf;

	// open file
	this->filename = filename;
	file = fopen(filename.c_str(), "w");
	if (!file) {
		panic("error fopen: %s: %s", filename.c_str(), strerror(errno));
	}

	// update symbol table section based on changes to symbols
	copy_to_symbol_table_sections();

	// recompute section offsets based on changes to headers and sections
	recalculate_section_offsets();

	// byteswap, de-normalize and write file header
	switch (ei_class) {
		case ELFCLASS32:
			buf.resize(sizeof(Elf32_Ehdr));
			elf_ehdr64_to_ehdr32((Elf32_Ehdr*)buf.data(), &ehdr);
			elf_bswap_ehdr32((Elf32_Ehdr*)buf.data(), ei_data, ELFENDIAN_TARGET);
			break;
		case ELFCLASS64:
			buf.resize(sizeof(Elf64_Ehdr));
			memcpy((Elf64_Ehdr*)buf.data(), &ehdr, sizeof(Elf64_Ehdr));
			elf_bswap_ehdr64((Elf64_Ehdr*)buf.data(), ei_data, ELFENDIAN_TARGET);
			break;
		default:
			fclose(file);
			panic("error invalid ELF class: %s", filename.c_str());
	}
	if (fwrite(buf.data(), 1, buf.size(), file) != buf.size()) {
		fclose(file);
		panic("error fwrite: %s", filename.c_str());
	}

	// byteswap, de-normalize and write program and section headers
	switch (ei_class) {
		case ELFCLASS32:
			buf.resize(sizeof(Elf32_Phdr));
			for (size_t i = 0; i < phdrs.size(); i++) {
				elf_phdr64_to_phdr32((Elf32_Phdr*)buf.data(), &phdrs[i]);
				elf_bswap_phdr32((Elf32_Phdr*)buf.data(), ei_data, ELFENDIAN_TARGET);
				fseek(file, ehdr.e_phoff + i * sizeof(Elf32_Phdr), SEEK_SET);
				if (fwrite(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fwrite: %s", filename.c_str());
				}
			}
			buf.resize(sizeof(Elf32_Shdr));
			for (size_t i = 0; i < shdrs.size(); i++) {
				elf_shdr64_to_shdr32((Elf32_Shdr*)buf.data(), &shdrs[i]);
				elf_bswap_shdr32((Elf32_Shdr*)buf.data(), ei_data, ELFENDIAN_TARGET);
				fseek(file, ehdr.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
				if (fwrite(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fwrite: %s", filename.c_str());
				}
			}
			break;
		case ELFCLASS64:
			buf.resize(sizeof(Elf64_Phdr));
			for (size_t i = 0; i < phdrs.size(); i++) {
				memcpy((Elf64_Phdr*)buf.data(), &phdrs[i], sizeof(Elf64_Phdr));
				elf_bswap_phdr64((Elf64_Phdr*)buf.data(), ei_data, ELFENDIAN_TARGET);
				fseek(file, ehdr.e_phoff + i * sizeof(Elf64_Phdr), SEEK_SET);
				if (fwrite(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fwrite: %s", filename.c_str());
				}
			}
			buf.resize(sizeof(Elf64_Shdr));
			for (size_t i = 0; i < shdrs.size(); i++) {
				memcpy((Elf64_Shdr*)buf.data(), &shdrs[i], sizeof(Elf64_Shdr));
				elf_bswap_shdr64((Elf64_Shdr*)buf.data(), ei_data, ELFENDIAN_TARGET);
				fseek(file, ehdr.e_shoff + i * sizeof(Elf64_Shdr), SEEK_SET);
				if (fwrite(buf.data(), 1, buf.size(), file) != buf.size()) {
					fclose(file);
					panic("error fwrite: %s", filename.c_str());
				}
			}
			break;
	}

	// byteswap symbol table
	byteswap_symbol_table(ELFENDIAN_TARGET);

	// write section buffers to file
	for (size_t i = 0; i < sections.size(); i++) {
		if (shdrs[i].sh_type == SHT_NOBITS) continue;
		fseek(file, shdrs[i].sh_offset, SEEK_SET);
		if (fwrite(sections[i].buf.data(), 1, shdrs[i].sh_size, file) != shdrs[i].sh_size) {
			fclose(file);
			panic("error fwrite: %s", filename.c_str());
		}
	}

	// byteswap symbol table
	byteswap_symbol_table(ELFENDIAN_HOST);

	fclose(file);
}

void elf_file::byteswap_symbol_table(ELFENDIAN endian)
{
	if (!symtab) return;

	size_t num_symbols = symtab->sh_size / symtab->sh_entsize;
	switch (ei_class) {
		case ELFCLASS32:
			for (size_t i = 0; i < num_symbols; i++) {
				Elf32_Sym *sym32 = (Elf32_Sym*)offset(symtab->sh_offset + i * sizeof(Elf32_Sym));
				elf_bswap_sym32(sym32, ei_data, ELFENDIAN_TARGET);
			}
			break;
		case ELFCLASS64:
			for (size_t i = 0; i < num_symbols; i++) {
				Elf64_Sym *sym64 = (Elf64_Sym*)offset(symtab->sh_offset + i * sizeof(Elf64_Sym));
				elf_bswap_sym64(sym64, ei_data, ELFENDIAN_TARGET);
			}
			break;
	}
}

void elf_file::copy_from_symbol_table_sections()
{
	symbols.clear();
	addr_symbol_map.clear();
	name_symbol_map.clear();

	if (!symtab) return;

	size_t num_symbols = symtab->sh_size / symtab->sh_entsize;
	switch (ei_class) {
		case ELFCLASS32:
			for (size_t i = 0; i < num_symbols; i++) {
				Elf32_Sym *sym32 = (Elf32_Sym*)offset(symtab->sh_offset + i * sizeof(Elf32_Sym));
				Elf64_Sym sym64;
				elf_sym32_to_sym64(&sym64, sym32);
				symbols.push_back(sym64);
			}
			break;
		case ELFCLASS64:
			for (size_t i = 0; i < num_symbols; i++) {
				Elf64_Sym *sym64 = (Elf64_Sym*)offset(symtab->sh_offset + i * sizeof(Elf64_Sym));
				symbols.push_back(*sym64);
			}
			break;
	}

	if (!strtab) return;

	for (size_t i = 0; i < symbols.size(); i++) {
		auto &sym = symbols[i];
		if (sym.st_shndx != SHN_UNDEF && sym.st_info != STT_FILE && sym.st_value != 0) {
			const char* name = (const char*)offset(strtab->sh_offset + sym.st_name);
			if (!strlen(name)) continue;
			name_symbol_map[name] = i;
			addr_symbol_map[sym.st_value] = i;
		}
	}
}

void elf_file::copy_to_symbol_table_sections()
{
	// TODO
}

void elf_file::recalculate_section_offsets()
{
	// TODO
}

uint8_t* elf_file::offset(size_t offset)
{
	for (size_t i = 0; i < sections.size(); i++) {
		if (offset >= sections[i].offset && offset < sections[i].offset + sections[i].buf.size()) {
			return sections[i].buf.data() + (offset - sections[i].offset);
		}
	}
	panic("illegal offset: %lu", offset);
	return nullptr;
}

elf_section* elf_file::section(size_t offset)
{
	for (size_t i = 0; i < sections.size(); i++) {
		if (offset >= sections[i].offset && offset < sections[i].offset + sections[i].buf.size()) {
			return &sections[i];
		}
	}
	return nullptr;
}

const char* elf_file::shdr_name(int i)
{
	return shstrtab ?
		(const char*)offset(shstrtab->sh_offset + shdrs[i].sh_name) : "";
}

const char* elf_file::sym_name(int i)
{
	return strtab ?
		(const char*)offset(strtab->sh_offset + symbols[i].st_name) : "";
}

const char* elf_file::sym_name(const Elf64_Sym *sym)
{
	return strtab ?
		(const char*)offset(strtab->sh_offset + sym->st_name) : "";
}

const Elf64_Sym* elf_file::sym_by_nearest_addr(Elf64_Addr addr)
{
	auto ai = addr_symbol_map.lower_bound(addr);
	if (ai == addr_symbol_map.end()) return nullptr;
	if (ai->second == addr) return &symbols[ai->second];
	if (ai != addr_symbol_map.begin()) ai--;
	return &symbols[ai->second];
}

const Elf64_Sym* elf_file::sym_by_addr(Elf64_Addr addr)
{
	auto ai = addr_symbol_map.find(addr);
	if (ai == addr_symbol_map.end()) return nullptr;
	return &symbols[ai->second];
}

const Elf64_Sym* elf_file::sym_by_name(const char *name)
{
	size_t i = name_symbol_map[name];
	if (i == 0 || i >= symbols.size()) return nullptr;
	return &symbols[i];
}
