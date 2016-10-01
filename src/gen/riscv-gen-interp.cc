//
//  riscv-gen-interp.cc
//

#include <cstdio>
#include <sstream>
#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "riscv-util.h"
#include "riscv-cmdline.h"
#include "riscv-model.h"
#include "riscv-gen.h"

std::vector<cmdline_option> riscv_gen_interp::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-V", "--print-interp-h", cmdline_arg_type_none,
			"Print interpreter header",
			[&](std::string s) { return gen->set_option("print_interp_h"); } },
	};
}

static void print_interp_h(riscv_gen *gen)
{
	printf(kCHeader, "riscv-interp.h");
	printf("#ifndef riscv_interp_h\n");
	printf("#define riscv_interp_h\n");
	printf("\n");
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("/* Execute Instruction RV%lu */\n\n", isa_width.first);
		printf("template <");
		std::vector<std::string> mnems = gen->get_inst_mnemonics(false, true);
		for (auto mi = mnems.begin(); mi != mnems.end(); mi++) {
			printf("bool %s, ", mi->c_str());
		}
		printf("typename T, typename P>\n");
		printf("intptr_t exec_inst_%s(T &dec, P &proc, intptr_t pc_offset)\n",
			isa_width.second.c_str());
		printf("{\n");
		printf("\tenum { xlen = %zu };\n", isa_width.first);
		printf("\ttypedef int%zu_t sx;\n", isa_width.first);
		printf("\ttypedef uint%zu_t ux;\n", isa_width.first);
		printf("\tusing namespace riscv;\n");
		printf("\n");
		printf("\tswitch (dec.op) {\n");
		for (auto &opcode : gen->opcodes) {
			std::string inst = opcode->pseudocode_c;
			if (inst.size() == 0) continue;
			if (!opcode->include_isa(isa_width.first)) continue;
			printf("\t\tcase %s:\n", riscv_meta_model::opcode_format("riscv_op_", opcode, "_").c_str());
			inst = replace(inst, "imm", "dec.imm");
			inst = replace(inst, "ptr", "uintptr_t");
			inst = replace(inst, "fcsr", "proc.fcsr");
			inst = replace(inst, "lr", "proc.lr");
			inst = replace(inst, "pc_offset", "PC_OFFSET");
			inst = replace(inst, "pc", "proc.pc");
			inst = replace(inst, "PC_OFFSET", "pc_offset");
			inst = replace(inst, "length(inst)", "pc_offset");
			inst = replace(inst, "u32(f32(NAN))", "0x7fc00000");
			inst = replace(inst, "u64(f64(NAN))", "0x7ff8000000000000ULL");
			inst = replace(inst, "sx(INT_MIN)", "std::numeric_limits<sx>::min()");
			inst = replace(inst, "s32(INT_MIN)", "std::numeric_limits<s32>::min()");
			inst = replace(inst, "s64(INT_MIN)", "std::numeric_limits<s64>::min()");
			inst = replace(inst, "ux(INT_MIN)", "std::numeric_limits<ux>::min()");
			inst = replace(inst, "u32(INT_MIN)", "std::numeric_limits<u32>::min()");
			inst = replace(inst, "u64(INT_MIN)", "std::numeric_limits<u64>::min()");
			inst = replace(inst, "sx(INT_MAX)", "std::numeric_limits<sx>::max()");
			inst = replace(inst, "s32(INT_MAX)", "std::numeric_limits<s32>::max()");
			inst = replace(inst, "s64(INT_MAX)", "std::numeric_limits<s64>::max()");
			inst = replace(inst, "ux(INT_MAX)", "std::numeric_limits<ux>::max()");
			inst = replace(inst, "u32(INT_MAX)", "std::numeric_limits<u32>::max()");
			inst = replace(inst, "u64(INT_MAX)", "std::numeric_limits<u64>::max()");
			inst = replace(inst, "f32(frd)", "frd.r.s.val");
			inst = replace(inst, "f32(frs1)", "frs1.r.s.val");
			inst = replace(inst, "f32(frs2)", "frs2.r.s.val");
			inst = replace(inst, "f32(frs3)", "frs3.r.s.val");
			inst = replace(inst, "f64(frd)", "frd.r.d.val");
			inst = replace(inst, "f64(frs1)", "frs1.r.d.val");
			inst = replace(inst, "f64(frs2)", "frs2.r.d.val");
			inst = replace(inst, "f64(frs3)", "frs3.r.d.val");
			inst = replace(inst, "u32(frd)", "frd.r.wu.val");
			inst = replace(inst, "u32(frs1)", "frs1.r.wu.val");
			inst = replace(inst, "u32(frs2)", "frs2.r.wu.val");
			inst = replace(inst, "u64(frd)", "frd.r.lu.val");
			inst = replace(inst, "u64(frs1)", "frs1.r.lu.val");
			inst = replace(inst, "u64(frs2)", "frs2.r.lu.val");
			inst = replace(inst, "s32(frd)", "frd.r.w.val");
			inst = replace(inst, "s32(frs1)", "frs1.r.w.val");
			inst = replace(inst, "s32(frs2)", "frs2.r.w.val");
			inst = replace(inst, "s64(frd)", "frd.r.l.val");
			inst = replace(inst, "s64(frs1)", "frs1.r.l.val");
			inst = replace(inst, "s64(frs2)", "frs2.r.l.val");
			inst = replace(inst, "frd", "FRD");
			inst = replace(inst, "frs1", "FRS1");
			inst = replace(inst, "frs2", "FRS2");
			inst = replace(inst, "rd", "if (dec.rd > 0) proc.ireg[dec.rd]");
			inst = replace(inst, "rs1", "proc.ireg[dec.rs1]");
			inst = replace(inst, "rs2", "proc.ireg[dec.rs2]");
			inst = replace(inst, "FRD", "frd");
			inst = replace(inst, "FRS1", "frs1");
			inst = replace(inst, "FRS2", "frs2");
			inst = replace(inst, "frd", "proc.freg[dec.rd]");
			inst = replace(inst, "frs1", "proc.freg[dec.rs1]");
			inst = replace(inst, "frs2", "proc.freg[dec.rs2]");
			inst = replace(inst, "frs3", "proc.freg[dec.rs3]");
			inst = replace(inst, "fenv_setrm(rm)", "fenv_setrm((proc.fcsr >> 5) & 0b111)");
			printf("\t\t\tif (rv%c) {\n", opcode->extensions.front()->alpha_code);
			printf("\t\t\t\t%s;\n",  inst.c_str());
			printf("\t\t\t};\n");
			printf("\t\t\tbreak;\n");
		}
		printf("\t\tdefault: return 0; /* illegal instruction */\n");
		printf("\t}\n");
		printf("\treturn pc_offset;\n");
		printf("}\n\n");
	}
	printf("#endif\n");
}

void riscv_gen_interp::generate()
{
	if (gen->has_option("print_interp_h")) print_interp_h(gen);
}
