//
//  test-encoder.cc
//

#include <cstdio>
#include <cinttypes>
#undef NDEBUG
#include <cassert>

#include "host-endian.h"
#include "types.h"
#include "bits.h"
#include "meta.h"
#include "codec.h"
#include "jit.h"

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

	assert(emit_lui(riscv_ireg_a2, 0xfffffffffffff000) == 0xfffff637);
	assert(emit_lui(riscv_ireg_s4, 0x20000) == 0x00020a37);
	assert(emit_lui(riscv_ireg_a2, 0xfff0fffffffff000) == 0); /* illegal instruciton */

	assert(emit_auipc(riscv_ireg_a0, 0xffffffffffef1000) == 0xffef1517);
	assert(emit_auipc(riscv_ireg_t1, 0x117000) == 0x00117317);
	assert(emit_auipc(riscv_ireg_a0, 0xfff0fffffffff000) == 0); /* illegal instruciton */

	assert(emit_jalr(riscv_ireg_ra, riscv_ireg_s3, 0) == 0x000980e7);
	assert(emit_jalr(riscv_ireg_ra, riscv_ireg_t1, 368) == 0x170300e7);
	assert(emit_jalr(riscv_ireg_ra, riscv_ireg_t1, -1720) == 0x948300e7);
	assert(emit_jalr(riscv_ireg_ra, riscv_ireg_t1, 4096) == 0); /* illegal instruciton */

	assert(emit_bne(riscv_ireg_a5, riscv_ireg_zero, -16) == 0xfe0798e3);
	assert(emit_bne(riscv_ireg_a4, riscv_ireg_a5, 100) == 0x06f71263);
	assert(emit_bne(riscv_ireg_a4, riscv_ireg_a5, 4096) == 0); /* illegal instruciton */

	assert(emit_lbu(riscv_ireg_a4, riscv_ireg_a5, 20) == 0x0147c703);
}
