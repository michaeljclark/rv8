#include <cstdio>
#include <cstring>
#include <cstdlib>
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

#include <unistd.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-cmdline.h"
#include "riscv-color.h"
#include "riscv-decode.h"
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-strings.h"

using namespace riscv;

struct riscv_histogram_elf
{
	elf_file elf;
	std::string filename;

	std::string use_char = "#";
	size_t max_chars = 80;
	bool help_or_error = false;
	bool hash_bars = false;
	bool reverse_sort = false;
	bool inst_histogram = false;
	bool regs_histogram = false;
	bool regs_position = false;

	typedef std::map<std::string,size_t> map_t;
	typedef std::pair<std::string,size_t> pair_t;

	void histogram_add(map_t &hist, std::string s)
	{
		auto hi = hist.find(s);
		if (hi == hist.end()) hist.insert(pair_t(s, 1));
		else hi->second++;
	}

	size_t reg_value(riscv_decode &dec, riscv_arg_name arg_name)
	{
		switch (arg_name) {
			case riscv_arg_name_rd: return dec.rd;
			case riscv_arg_name_rs1: return dec.rs1;
			case riscv_arg_name_rs2: return dec.rs2;
			case riscv_arg_name_frd: return dec.rd;
			case riscv_arg_name_frs1: return dec.rs1;
			case riscv_arg_name_frs2: return dec.rs2;
			case riscv_arg_name_frs3: return dec.rs3;
			default: return 0;
		}
	}

	void histogram_add_regs(map_t &hist, riscv_decode &dec)
	{
		std::string key;
		const riscv_arg_data *arg_data = riscv_inst_arg_data[dec.op];
		while (arg_data->type != riscv_type_none) {
			switch (arg_data->type) {
				case riscv_type_ireg:
				case riscv_type_freg:
					key = std::string(arg_data->type == riscv_type_ireg ?
						riscv_ireg_name_sym[reg_value(dec, arg_data->arg_name)] :
						riscv_freg_name_sym[reg_value(dec, arg_data->arg_name)]) +
						(regs_position ? "-" : "") +
						(regs_position ? riscv_arg_name_sym[arg_data->arg_name] : "");
					histogram_add(hist, key);
					break;
				default: break;
			}
			arg_data++;
		}
	}

	void histogram(map_t &hist, uintptr_t start, uintptr_t end)
	{
		riscv_decode dec;
		uintptr_t pc = start, next_pc;
		while (pc < end) {
			uint64_t inst = riscv_get_inst(pc, &next_pc);
			riscv_decode_inst_rv64(dec, inst);
			if (inst_histogram) {
				histogram_add(hist, riscv_inst_name_sym[dec.op]);
			}
			if (regs_histogram) {
				histogram_add_regs(hist, dec);
			}
			pc = next_pc;
		}
	}

	std::string repeat_str(std::string str, size_t count)
	{
		std::string s;
		for (size_t i = 0; i < count; i++) s += str;
		return s;
	}

	void histogram()
	{
		map_t hist;
		std::vector<pair_t> hist_s;

		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				uintptr_t offset = (uintptr_t)elf.offset(shdr.sh_offset);
				histogram(hist, offset, offset + shdr.sh_size);
			}
		}

		size_t max = 0;
		for (auto ent : hist) {
			if (ent.second > max) max = ent.second;
			hist_s.push_back(ent);
		}

		std::sort(hist_s.begin(), hist_s.end(), [&] (const pair_t &a, const pair_t &b) {
			return reverse_sort ? a.second < b.second : a.second > b.second;
		});

		size_t i = 0;
		for (auto ent : hist_s) {
			printf("%5lu. %-10s[%-6lu]%s%s%s\n",
				++i, ent.first.c_str(), ent.second,
				hash_bars ? " " : "",
				hash_bars ? use_char.c_str() : "",
				hash_bars ? repeat_str(use_char, ent.second * (max_chars - 1) / max).c_str() : "");
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ "-c", "--char", cmdline_arg_type_string,
				"Character to use in bars",
				[&](std::string s) { use_char = s; return true; } },
			{ "-b", "--bars", cmdline_arg_type_none,
				"Print bars next to counts",
				[&](std::string s) { return (hash_bars = true); } },
			{ "-I", "--instructions", cmdline_arg_type_none,
				"Instruction Usage Histogram",
				[&](std::string s) { return (inst_histogram = true); } },
			{ "-R", "--registers", cmdline_arg_type_none,
				"Register Usage Histogram",
				[&](std::string s) { return (regs_histogram = true); } },
			{ "-P", "--registers-operands", cmdline_arg_type_none,
				"Register Usage Histogram (with operand positions)",
				[&](std::string s) { return (regs_histogram = regs_position = true); } },
			{ "-m", "--max-chars", cmdline_arg_type_string,
				"Maximum number of characters for bars",
				[&](std::string s) { return (max_chars = strtoull(s.c_str(), nullptr, 10)); } },
			{ "-r", "--reverse-sort", cmdline_arg_type_none,
				"Sort in Reverse",
				[&](std::string s) { return (reverse_sort = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1 || !(inst_histogram || regs_histogram)) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error)
		{
			printf("usage: %s [<options>] <elf_file>\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		filename = result.first[0];
	}

	void run()
	{
		elf.load(filename);
		histogram();
	}
};

int main(int argc, const char *argv[])
{
	riscv_histogram_elf elf_histogram;
	elf_histogram.parse_commandline(argc, argv);
	elf_histogram.run();
	return 0;
}
