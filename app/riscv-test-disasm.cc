#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <vector>
#include <map>
#include <string>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-regs.h"
#include "riscv-processor.h"
#include "riscv-opcodes.h"
#include "riscv-util.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-decode-switch.h"
#include "riscv-csr.h"
#include "riscv-compression.h"
#include "riscv-format.h"


riscv_wu test_code_1[] = {
	0x0026c197,
	0xd7018193,
	0x800000e7,
	0x04051063,
	0x0026c517,
	0xcd050513,
	0x00000593,
	0x002a3617,
	0x14460613,
	0x40a60633,
	0x00127317,
	0x07c300e7,
	0x00250117,
	0xfd010113,
	0x00259217,
	0xc0020213,
	0x000032b7,
	0x1002b073,
	0x5180006f,
	0x00800593,
	0x02b57663,
	0x0026e597,
	0xfac58593,
	0x00351513,
	0x00b50533,
	0x00053103,
	0xfe010ee3,
	0xffffe237,
	0x00220233,
	0x00023203,
	0x7980106f,
	0x10200073,
	0xffdff06f
};

riscv_hu test_code_2[] = {
	0x6197, 0x001d,
	0x8193, 0xd701,
	0x00e7, 0x8000,
	0xe915,
	0x6517, 0x001d,
	0x0513, 0xcd25,
	0x4581,
	0xd617, 0x0020,
	0x0613, 0x1486,
	0x8e09,
	0xc0ef, 0x527c,
	0xa117, 0x001b,
	0x0113, 0xfda1,
	0x3217, 0x001c,
	0x0213, 0xc0a2,
	0x628d,
	0xb073, 0x1002,
	0x006f, 0x3680,
	0x45a1,
	0x7263, 0x02b5,
	0x8597, 0x001d,
	0x8593, 0xfba5,
	0x050e,
	0x952e,
	0x3103, 0x0005,
	0x0ee3, 0xfe01,
	0x7279,
	0x920a,
	0x3203, 0x0002,
	0x106f, 0x1300,
	0x0073, 0x1020,
	0xbff5
};

void decode_rv64(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	proc.p_type = riscv_proc_type_rv64i;

	proc.pc = start;
	while (proc.pc < end) {
		riscv_ptr pc = proc.pc;
		riscv_decode_instruction(dec, &proc);
		riscv_print_instruction(dec, &proc, pc, pc_offset);
	}
}

#define DECODE_RV64(arr) decode_rv64((riscv_ptr)arr, (riscv_ptr)arr + sizeof(arr), (riscv_ptr)arr - 0xffffffff80000000ULL);

int main(int argc, char *argv[])
{

#if 1
	//DECODE_RV64(test_code_1);
	DECODE_RV64(test_code_2);
#else
	std::vector<char> bin = read_file("bin/add_1.bin");
	decode_rv64((riscv_ptr)bin.data(), (riscv_ptr)bin.data() + bin.size(), (riscv_ptr)bin.data());
#endif

	return 0;
}
