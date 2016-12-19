//
//  assembler.h
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <algorithm>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "host-endian.h"
#include "types.h"
#include "bits.h"
#include "format.h"
#include "meta.h"
#include "util.h"
#include "cmdline.h"
#include "color.h"
#include "codec.h"
#include "strings.h"
#include "disasm.h"
#include "assembler.h"
#include "jit.h"

using namespace riscv;

void assembler::load_imm_r(ireg5 rd, s64 val)
{
	if ((val & ~s64(0x7fffffff)) == 0 || (val & ~s64(0x7fffffff)) == ~s64(0x7fffffff)) {
		add_inst(emit_lui(rd, s32(val >> 12)));
		val -= (val >> 12) << 12;
		if (val != 0) {
			add_inst(emit_addi(rd, rd, bitextend(val, 12)));
		}
	}
	else {
		s64 lo = bitextend(val, 12), hi = val - lo;
		int shift = 12 + ctz(u64(hi) >> 12);
		hi >>= shift;
		load_imm_r(rd, hi);
		add_inst(emit_slli_rv64i(rd, rd, shift));
		if (lo != 0) {
			add_inst(emit_addi(rd, rd, s32(bitextend(lo, 12))));
		}
	}
}

void assembler::load_imm(ireg5 rd, s64 val)
{
	if (val >= -2048 && val <= 2047) {
		add_inst(emit_addi(rd, rv_ireg_zero, s32(val)));
	} else if (ispow2(val)) {
		add_inst(emit_addi(rd, rv_ireg_zero, 1));
		add_inst(emit_slli_rv64i(rd, rd, ctz(val)));
	} else {
		load_imm_r(rd, val);
	}
}
