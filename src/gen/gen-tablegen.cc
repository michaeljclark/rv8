//
//  gen-tablegen.cc
//

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "util.h"
#include "cmdline.h"
#include "model.h"
#include "gen.h"
#include "fmt.h"

using namespace riscv;

std::vector<cmdline_option> rv_gen_tablegen::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-TG", "--print-tablegen-h", cmdline_arg_type_none,
			"Print LLVM TableGen",
			[&](std::string s) { return gen->set_option("print_tablegen_td"); } },
	};
}

static void print_tablegen_td(rv_gen *gen)
{
	for (auto codec : gen->codecs) {
		if (split(codec->name,"+").size() > 1) continue;
		std::string name = join(split(codec->name, "·"), "_");
		printf("class %s\n{\n", name.c_str());
		for (auto operand : codec->operands) {
			std::string operand_name = operand->name;
			if (operand_name.find("rs1rd") != std::string::npos) operand_name = "rdrs1";
			else if (operand_name.find("rd") != std::string::npos) operand_name = "rd";
			else if (operand_name.find("rs1") != std::string::npos) operand_name = "rs1";
			else if (operand_name.find("rs2") != std::string::npos) operand_name = "rs2";
			else if (operand_name.find("rs3") != std::string::npos) operand_name = "rs3";
			else if (operand_name.find("imm") != std::string::npos) operand_name = "imm";
			std::string suffix;
			if (operand->name.find("rs1q") != std::string::npos ||
				operand->name.find("rs2q") != std::string::npos ||
				operand->name.find("rdq") != std::string::npos)
			{
				suffix = " - 8";
			}
			std::map<int,std::string> prlist;
			for (auto segment : operand->bitspec.segments) {
				auto inst_range = segment.first;
				auto decode_range = segment.second;
				size_t inst_seg_offset = inst_range.msb;
				if (decode_range.size() == 0) {
					std::string buf;
					size_t seg_len = (inst_range.msb - inst_range.lsb + 1);
					if (seg_len == 1) {
						sprintf(buf, "Inst{%zu} = %s%s;",
							inst_seg_offset,
							operand_name,
							suffix);
					} else {
						sprintf(buf, "Inst{%zu-%zu} = %s%s;",
							inst_seg_offset, inst_seg_offset - seg_len + 1,
							operand_name.c_str(),
							suffix.c_str());
					}
					prlist[inst_seg_offset] = buf;
				} else {
					for (auto dec_seg : decode_range) {
						std::string buf;
						size_t seg_len = (dec_seg.msb - dec_seg.lsb + 1);
						if (seg_len == 1) {
							sprintf(buf, "Inst{%zu} = %s{%zu}%s;",
								inst_seg_offset,
								operand_name.c_str(),
								dec_seg.msb,
								suffix.c_str());
						} else {
							sprintf(buf, "Inst{%zu-%zu} = %s{%zu-%zu}%s;",
								inst_seg_offset, inst_seg_offset - seg_len + 1,
								operand_name.c_str(),
								dec_seg.msb, dec_seg.lsb,
								suffix.c_str());
						}
						prlist[inst_seg_offset] = buf;
						inst_seg_offset -= seg_len;
					}
				}
			}
			for (auto &ent : prlist) {
				printf("\t%s\n", ent.second.c_str());
			}
		}
		printf("}\n\n");
	}
}

void rv_gen_tablegen::generate()
{
	if (gen->has_option("print_tablegen_td")) {
		print_tablegen_td(gen);
	}
}
