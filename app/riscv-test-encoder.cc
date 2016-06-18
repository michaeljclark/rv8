#include <cstdio>
#include <cinttypes>
#undef NDEBUG
#include <cassert>

#include "riscv-types.h"
#include "riscv-meta.h"
#include "riscv-endian.h"
#include "riscv-decode.h"

using namespace riscv;

template <typename T> void test_imm(bool r, T imm) {
	static const char *pf = "%s ptr%llu[valid=%d, imm=0x%lx]\n";
	static const char *of = "%s offset%llu[valid=%d, imm=%ld\n";
	static const char *sf = "%s simm%llu[valid=%d, imm=%lld]\n";
	static const char *uf = "%s uimm%llu[valid=%d, imm=%llu]\n";
	printf(T::is_pointer ? pf : T::is_offset ? of : T::is_signed ? sf : uf,
		r == imm.valid() ? "PASS" : "FAIL",
		T::width, imm.valid(), typename T::value_type(imm)
	);
}

/* example instruction encoding using width-typed immediate argument template aliases */

inline uint64_t emit_bne(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	riscv_decode dec;

	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */

	dec.op = riscv_op_bne;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;

	return riscv_encode_insn(dec);
}

inline uint64_t print_bne(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	printf("bne %s,%s,%ld",
		riscv_i_registers[rs1],
		riscv_i_registers[rs2],
		offset13::value_type(sbimm12));

	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) printf(" /* illegal instruction */");

	return emit_bne(rs1, rs2, sbimm12);
}

int main()
{
	test_imm<simm20>(0, -524289);
	test_imm<simm20>(1, -524288);
	test_imm<simm20>(1, -1);
	test_imm<simm20>(1, 0);
	test_imm<simm20>(1, 1);
	test_imm<simm20>(1, 524287);
	test_imm<simm20>(0, 524288);

	test_imm<uimm20>(0, -1048577);
	test_imm<uimm20>(0, -1048576);
	test_imm<uimm20>(0, -1);
	test_imm<uimm20>(1, 0);
	test_imm<uimm20>(1, 1);
	test_imm<uimm20>(1, 1048575);
	test_imm<uimm20>(0, 1048576);

	test_imm<offset12>(0, -2049);
	test_imm<offset12>(1, -2048);
	test_imm<offset12>(1, 0);
	test_imm<offset12>(1, 2047);
	test_imm<offset12>(0, 2048);

	test_imm<offset13>(0, -4097);
	test_imm<offset13>(1, -4096);
	test_imm<offset13>(1, 0);
	test_imm<offset13>(1, 4095);
	test_imm<offset13>(0, 4096);

	test_imm<offset21>(0, -1048577);
	test_imm<offset21>(1, -1048576);
	test_imm<offset21>(1, 0);
	test_imm<offset21>(1, 1048575);
	test_imm<offset21>(0, 1048576);

	test_imm<ptr32>(0, -0x100000000LL);
	test_imm<ptr32>(0, -0xffffffffLL);
	test_imm<ptr32>(0, -1LL);
	test_imm<ptr32>(1, 0LL);
	test_imm<ptr32>(1, 1LL);
	test_imm<ptr32>(1, 0xffffffffLL);
	test_imm<ptr32>(0, 0x100000000LL);

	test_imm<ptr64>(1, 0);
	test_imm<ptr64>(1, 1);
	test_imm<ptr64>(1, 0xffffffffffffffffLL);

	printf(" # 0x%08" PRIx64 "\n", print_bne(riscv_ireg_a5, riscv_ireg_zero, -16));
	printf(" # 0x%08" PRIx64 "\n", print_bne(riscv_ireg_a4, riscv_ireg_a5, 100));
	printf(" # 0x%08" PRIx64 "\n", print_bne(riscv_ireg_a4, riscv_ireg_a5, 4096)); /* illegal instruciton */

	assert(emit_bne(riscv_ireg_a5, riscv_ireg_zero, -16) == 0xfe0798e3);
	assert(emit_bne(riscv_ireg_a4, riscv_ireg_a5, 100) == 0x06f71263);
	assert(emit_bne(riscv_ireg_a4, riscv_ireg_a5, 4096) == 0); /* illegal instruciton */
}
