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

/* example using width-typed immediate argument template aliases */
inline uint64_t emit_bne(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	assert(rs1.valid() && rs2.valid() && sbimm12.valid());
	printf("bne %s,%s,%ld\n",
		riscv_i_registers[rs1],
		riscv_i_registers[rs2],
		offset13::value_type(sbimm12));
	riscv_decode dec;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return riscv_encode_sb(dec);
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

	emit_bne(riscv_ireg_a0, riscv_ireg_a1, -8);
	emit_bne(riscv_ireg_a2, riscv_ireg_a3, 8);
	emit_bne(riscv_ireg_a4, riscv_ireg_a5, 4096);
}