//
//  riscv-test-emulate.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <cinttypes>
#include <vector>
#include <map>
#include <string>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-processor.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-codec.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-strings.h"

using namespace riscv;

void rv64_exec(riscv_decode &dec, riscv_proc_rv64 &proc)
{
	uintptr_t next_pc;
	uint64_t inst = riscv_get_inst(proc.pc, &next_pc);
	riscv_decode_inst_rv64(dec, inst);
	switch (dec.op) {
		case riscv_op_addi:
			proc.ireg[dec.rd] = proc.ireg[dec.rs1] + dec.imm;
			proc.pc = next_pc;
			break;
		case riscv_op_auipc:
			proc.ireg[dec.rd] = uintptr_t(proc.pc) + dec.imm;
			proc.pc = next_pc;
			break;
		case riscv_op_lui:
			proc.ireg[dec.rd] = dec.imm;
			proc.pc = next_pc;
			break;
		case riscv_op_ecall:
			switch (proc.ireg[riscv_ireg_a7]) {
				case 64: /* sys_write */
					proc.ireg[riscv_ireg_a0] = write(proc.ireg[riscv_ireg_a0],
						(void*)(uintptr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
					break;
				case 93: /* sys_exit */
					exit(proc.ireg[riscv_ireg_a0]);
					break;
				default:
					panic("illegal syscall: %d", proc.ireg[riscv_ireg_a7]);
			}
			proc.pc = next_pc;
			break;
		default:
			panic("illegal instruciton: %s", riscv_inst_name_sym[dec.op]);
	}
}

void rv64_run(uintptr_t entry)
{
	riscv_decode dec;
	riscv_proc_rv64 proc;
	proc.pc = entry;
	while (true) {
		rv64_exec(dec, proc);
	}
}

void* map_executable(const char* filename, void *vaddr, size_t len, size_t offset)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		panic("error: open: %s: %s", filename, strerror(errno));
	}
	void *addr = mmap(vaddr, len, PROT_READ | PROT_EXEC, MAP_FIXED | MAP_PRIVATE, fd, offset);
	if (addr == MAP_FAILED) {
		panic("error: mmap: %s: %s", filename, strerror(errno));
	}
	close(fd);
	return addr;
}

void process_info(char **argv)
{
	static const char *textptr = nullptr;
	void *heapptr = malloc(8);
	printf("text  = ~0x%016" PRIxPTR "\n", (uintptr_t)&textptr);
	printf("heap  = ~0x%016" PRIxPTR "\n", (uintptr_t)heapptr);
	printf("stack = ~0x%016" PRIxPTR "\n", (uintptr_t)argv);
	free(heapptr);
}

int main(int argc, char *argv[])
{
	elf_file elf;

	if (argc != 2) panic("usage: %s <elf_file>", argv[0]);

	process_info(argv);

	// load ELF headers
	// NOTE: This POC code presently loads the whole ELF into RAM.
	// TODO: Add flag to only load headers
	elf.load(argv[1]);

	// Find the LOAD segment and mmap it into memory
	// NOTE: This POC code presently only handles one PT_LOAD segment
	for (size_t i = 0; i < elf.phdrs.size(); i++) {
		Elf64_Phdr &phdr = elf.phdrs[i];
		if (phdr.p_flags & PT_LOAD) {
			map_executable(argv[1], (void*)phdr.p_vaddr, phdr.p_memsz, phdr.p_offset);
			rv64_run(uintptr_t(elf.ehdr.e_entry));
			break;
		}
	}

	// TODO : munmap

	return 0;
}
