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
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

struct riscv_histogram_elf
{
	elf_file elf;
	std::string filename;

	std::string use_char = "#";
	size_t max_chars = 80;
	bool help_or_error = false;
	bool hash_bars = false;
	bool reverse_sort = false;

	typedef std::map<size_t,size_t> map_t;
	typedef std::pair<size_t,size_t> pair_t;

	void histogram(map_t &hist, riscv_ptr start, riscv_ptr end)
	{
		riscv_decode dec;
		riscv_ptr pc = start, next_pc;
		while (pc < end) {
			riscv_lu inst = riscv_get_instruction(pc, &next_pc);
			riscv_decode_instruction<riscv_decode>(dec, inst);
			auto hi = hist.find(dec.op);
			if (hi == hist.end()) hist.insert(pair_t(dec.op, 1));
			else hi->second++;
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
				uint8_t *offset = elf.offset(shdr.sh_offset);
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
				++i, riscv_instruction_name[ent.first], ent.second,
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
		} else if (result.first.size() != 1) {
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
