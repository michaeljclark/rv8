#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
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

#include "riscv-util.h"
#include "riscv-model.h"
#include "riscv-cmdline.h"
#include "riscv-color.h"

static bool enable_color = false;

#define _COLOR_OPCODE _COLOR_BEGIN _COLOR_UNDERSCORE _COLOR_SEP _COLOR_FG_YELLOW _COLOR_END
#define _COLOR_BITS   _COLOR_BEGIN _COLOR_REVERSE    _COLOR_SEP _COLOR_FG_GREEN  _COLOR_END
#define _COLOR_FORMAT _COLOR_BEGIN _COLOR_BOLD       _COLOR_SEP _COLOR_FG_RED    _COLOR_END
#define _COLOR_LEGEND _COLOR_BEGIN _COLOR_BOLD       _COLOR_END
#define _COLOR_EXT    _COLOR_BEGIN _COLOR_FG_RED     _COLOR_END

static const ssize_t kMaxInstructionWidth = 32;
static const ssize_t kLatexTableColumns = 32;
static const ssize_t kLatexTableRows = 50;

static const char* kLatexDocumentBegin =
R"LaTeX(\documentclass{report}
\usepackage[letterpaper, portrait, margin=0.5in]{geometry}
\usepackage[utf8]{inputenc}
\begin{document}
)LaTeX";

static const char* kLatexDocumentEnd =
R"LaTeX(\end{document}
)LaTeX";

static const char* kLatexTableBegin =
R"LaTeX(\newpage
\begin{table}[p]
\begin{small}
\begin{center}
\begin{tabular})LaTeX";

static const char* kLatexTableEnd =
R"LaTeX(\end{tabular}
\end{center}
\end{small}
\end{table}
)LaTeX";

enum riscv_latex_type {
	riscv_latex_type_empty,
	riscv_latex_type_line,
	riscv_latex_type_page_break,
	riscv_latex_type_extension_heading,
	riscv_latex_type_extension_contd,
	riscv_latex_type_opcode,
	riscv_latex_type_type_bitrange,
	riscv_latex_type_type_spec,
};

struct riscv_latex_row
{
	riscv_latex_type row_type;
	riscv_extension_ptr extension;
	riscv_opcode_ptr opcode;
	riscv_type_ptr type;

	riscv_latex_row(riscv_latex_type row_type)
		: row_type(row_type) {}

	riscv_latex_row(riscv_latex_type row_type, riscv_extension_ptr extension)
		: row_type(row_type), extension(extension) {}

	riscv_latex_row(riscv_latex_type row_type, riscv_opcode_ptr opcode)
		: row_type(row_type), opcode(opcode) {}

	riscv_latex_row(riscv_latex_type row_type, riscv_type_ptr type)
		: row_type(row_type), type(type) {}
};

struct riscv_latex_page
{
	std::vector<riscv_latex_row> rows;
};

struct riscv_parse_meta : riscv_meta_model
{
	void generate_map();
	void generate_codec();

	std::string colorize_args(riscv_opcode_ptr opcode);
	std::vector<std::string> get_unique_codecs();

	void print_latex_row(riscv_latex_row &row, std::string ts);
	void print_latex();
	void print_map();
	void print_opcodes_h(bool no_comment = false, bool zero_not_oh = false);
	void print_opcodes_c(bool no_comment = false, bool zero_not_oh = false);
	void print_switch_c(bool no_comment = false, bool zero_not_oh = false);

	void generate_codec_node(riscv_codec_node &node, riscv_opcode_list &opcode_list);
	void print_switch_decoder_node(riscv_codec_node &node, size_t indent);
};

template <typename T>
std::string join(std::vector<T> list, std::string sep)
{
	std::stringstream ss;
	for (auto i = list.begin(); i != list.end(); i++) {
		ss << (i != list.begin() ? sep : "") << *i;
	}
	return ss.str();
}

void riscv_parse_meta::generate_map()
{
	for (auto &opcode : opcodes) {
		for (auto &mask : opcode->masks) {
			ssize_t msb = mask.first.msb;
			ssize_t lsb = mask.first.lsb;
			ssize_t val = mask.second;
			for (ssize_t bit = msb; bit >= lsb; bit--) {
				opcode->mask |= (1 << bit);
				opcode->match |= (val >> (bit - lsb)) & 1 ? (1 << bit) : 0;
			}
		}
	}
}

void riscv_parse_meta::generate_codec()
{
	generate_codec_node(root_node, opcodes);
}

std::string riscv_parse_meta::colorize_args(riscv_opcode_ptr opcode)
{
	std::vector<char> token;
	std::vector<std::string> comps;
	auto args = opcode->format->args;

	for (size_t i = 0; i < args.length(); i++) {
		char c = args[i];
		if (::isalnum(c)) {
			token.push_back(c);
		} else {
			comps.push_back(std::string(token.begin(), token.end()));
			token.resize(0);
			token.push_back(c);
			comps.push_back(std::string(token.begin(), token.end()));
			token.resize(0);
		}
	}
	if (token.size() > 0) {
		comps.push_back(std::string(token.begin(), token.end()));
	}

	for (size_t i = 0; i < comps.size(); i++) {
		auto comp = comps[i];
		auto arg = args_by_name[comp];
		if (arg) {
			auto new_comp = riscv_colors_to_ansi_escape_sequence(
				arg->fg_color, arg->bg_color, ansi_color_normal
			);
			new_comp.append(comp);
			new_comp.append(_COLOR_RESET);
			comps[i] = new_comp;
		}
	}

	return join(comps, "");
}


std::vector<std::string> riscv_parse_meta::get_unique_codecs()
{
	std::vector<std::string> codec_names;
	for (auto &codec : codecs) {
		std::string codec_name = split(codec->name, "+", false, false)[0];
		if (std::find(codec_names.begin(), codec_names.end(), codec_name) == codec_names.end()) {
			codec_names.push_back(codec_name);
		}
	}
	return codec_names;
}

void riscv_parse_meta::print_latex_row(riscv_latex_row &row, std::string ts)
{
	switch (row.row_type) {
		case riscv_latex_type_empty:
			printf("\\multicolumn{%ld}{c}{} & \\\\\n", kLatexTableColumns);
			break;
		case riscv_latex_type_line:
			printf("\\cline{1-%ld}\n", kLatexTableColumns);
			break;
		case riscv_latex_type_page_break:
			printf("%s", kLatexTableEnd);
			printf("%s%s", kLatexTableBegin, ts.c_str());
			break;
		case riscv_latex_type_extension_heading:
		case riscv_latex_type_extension_contd:
		{
			printf("\\multicolumn{%ld}{c}{\\bf %s %s} & \\\\\n\\cline{1-%ld}\n",
				kLatexTableColumns,
				row.extension->description.c_str(),
				row.row_type == riscv_latex_type_extension_contd ? " contd" : "",
				kLatexTableColumns);
			break;
		}
		case riscv_latex_type_opcode:
		{
			auto &opcode = row.opcode;
			ssize_t bit_width = opcode->extensions[0]->insn_width;

			// calculate the column spans for this row
			riscv_arg_ptr arg, larg;
			bool lbound = false;
			typedef std::tuple<riscv_opcode_ptr,riscv_arg_ptr,ssize_t,std::string> arg_tuple;
			std::vector<arg_tuple> arg_parts;
			for (ssize_t bit = bit_width-1; bit >= 0; bit--) {
				char c = ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : '?');
				arg = opcode->find_arg(bit);

				// figure out where to break columns
				if (arg_parts.size() == 0 || std::get<1>(arg_parts.back()) != arg || lbound)
				{
					std::string str;
					str += c;
					arg_parts.push_back(arg_tuple(opcode, arg, 1, str));
				} else {
					auto &sz = std::get<2>(arg_parts.back());
					auto &str = std::get<3>(arg_parts.back());
					char lastc = str[str.length()-1];
					if ((lastc == '?' && (c == '1' || c == '0')) ||
						((lastc == '1' || lastc == '0') && c == '?'))
					{
						std::string str;
						str += c;
						arg_parts.push_back(arg_tuple(opcode, arg, 1, str));
					} else {
						sz++;
						str += c;
					}
				}

				// find the type of this opcode for column boundaries
				auto named_bitspec = opcode->find_named_bitspec(bit);
				if (named_bitspec.first.segments.size() == 0) {
					auto default_type = types_by_name[std::to_string(bit_width)];
					if (!default_type) panic("can't find default type: %d", bit_width);
					named_bitspec = default_type->find_named_bitspec(bit);
				}
				lbound = (bit != 0 && bit == named_bitspec.first.segments.back().first.lsb);

				larg = arg;
			}

			// update labels for segments with args
			ssize_t msb = bit_width-1;
			for (size_t i = 0; i < arg_parts.size(); i++) {
				auto &arg = std::get<1>(arg_parts[i]);
				auto size = std::get<2>(arg_parts[i]);
				auto &str = std::get<3>(arg_parts[i]);
				if (arg) {
					str = arg->label;
					if (str == "imm") {
						auto spec = arg->bitspec;
						for (auto &seg : spec.segments) {
							if (seg.first.msb == msb && seg.first.lsb == (msb - size) + 1) {
								str += "[";
								for (auto ri = seg.second.begin(); ri != seg.second.end(); ri++) {
									if (ri != seg.second.begin()) str += "$\\vert$";
									str += ri->to_string(":");
								}
								str += "]";
								break;
							}
						}
					}
				} else {
					std::replace(str.begin(), str.end(), '?', '0');
				}
				msb -= size;
			}

			// construct the LaTeX for this row
			std::stringstream ls;
			for (size_t i = 0; i < arg_parts.size(); i++) {
				auto size = std::get<2>(arg_parts[i]);
				auto &str = std::get<3>(arg_parts[i]);
				ls << (i != 0 ? " & " : "")
				   << "\\multicolumn{" << (size * kLatexTableColumns / bit_width) << "}"
				   << "{" << (i == 0 ? "|" : "") << "c|}"
				   << "{" << str << "}";
			}

			// format the opcode name and arguments
			auto name = opcode->name;
			auto arg_comps = split(opcode->format->args, ",", false, false);
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);

			// print this row
			printf("%s & %s %s \\\\\n\\cline{1-%ld}\n",
				ls.str().c_str(), name.c_str(), join(arg_comps, ", ").c_str(), kLatexTableColumns);
			break;
		}
		case riscv_latex_type_type_spec:
		{
			// construct the LaTeX for this type row
			std::stringstream ls;
			auto type = row.type;
			size_t bit_width = type->parts[0].first.segments.front().first.msb + 1;
			for (size_t i = 0; i < type->parts.size(); i++) {
				auto &named_bitspec = type->parts[i];
				auto &range = named_bitspec.first;
				auto str = named_bitspec.second;
				ssize_t msb = range.segments.front().first.msb;
				ssize_t lsb = range.segments.back().first.lsb;
				ssize_t size = msb - lsb + 1;

				if (str == "imm") {
					for (auto &seg : range.segments) {
						if (seg.first.msb == msb && seg.first.lsb == (msb - size) + 1) {
							str += "[";
							for (auto ri = seg.second.begin(); ri != seg.second.end(); ri++) {
								if (ri != seg.second.begin()) str += "$\\vert$";
								str += ri->to_string(":");
							}
							str += "]";
							break;
						}
					}
				}

				ls << (i != 0 ? " & " : "")
				   << "\\multicolumn{" << (size * kLatexTableColumns / bit_width) << "}"
				   << "{" << (i == 0 ? "|" : "") << "c|}"
				   << "{" << str << "}";
			}

			// format the type name
			std::string name = type->name;
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
			name = "Type-" + name;

			// print this row
			printf("%s & %s \\\\\n\\cline{1-%ld}\n",
				ls.str().c_str(), name.c_str(), kLatexTableColumns);
			break;
		}
		case riscv_latex_type_type_bitrange:
		{
			// construct the LaTeX for this type bit range header
			std::stringstream ls;
			auto type = row.type;
			size_t bit_width = type->parts[0].first.segments.front().first.msb + 1;
			for (size_t i = 0; i < type->parts.size(); i++) {
				auto &named_bitspec = type->parts[i];
				auto &range = named_bitspec.first;
				auto msb = range.segments.front().first.msb;
				auto lsb = range.segments.back().first.lsb;
				auto size = msb - lsb + 1;
				auto tsize = size * kLatexTableColumns / bit_width;
				if (size == 1) {
					ls << "\\multicolumn{" << tsize << "}{c}{\\textbf{\\scriptsize{" << msb << "}}} & ";
				} else {
					auto lsize = size >> 1;
					auto rsize = tsize - lsize;
					ls << "\\multicolumn{" << lsize << "}{l}{\\textbf{\\scriptsize{" << msb << "}}} & "
					   << "\\multicolumn{" << rsize << "}{r}{\\textbf{\\scriptsize{" << lsb << "}}} & ";
				}
			}
			// print this row
			printf("%s \\\\\n\\cline{1-%ld}\n",
				ls.str().c_str(), kLatexTableColumns);
			break;
		}
	}
}

void riscv_parse_meta::print_latex()
{
	// paginate opcodes ordered by extension
	// adding type and extension headings, page breaks and continuations
	size_t line = 0;
	ssize_t lwidth = 0;
	std::vector<riscv_latex_page> pages;
	for (auto &ext : extensions) {
		// check if this extension is in the selected subset
		if (ext->opcodes.size() == 0 || (ext_subset.size() > 0 &&
			std::find(ext_subset.begin(), ext_subset.end(), ext) == ext_subset.end())) {
			continue;
		}
		if (pages.size() == 0) pages.push_back(riscv_latex_page());

		// page break if the instruction width has changed
		if (lwidth != 0 && lwidth != ext->insn_width) {
			pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_page_break));
			line = 0;
		} else {
			pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_empty));
		}

		// add extension heading
		pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_extension_heading, ext));
		line++;

		// add opcodes
		for (auto &opcode : ext->opcodes) {
			// add a line to the top of the page if there is a continuation
			if (line % kLatexTableRows == 0) {
				pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_empty));
				pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_extension_contd, ext));
				pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_line));
			}
			pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_opcode, opcode));
			line++;
			// add page break
			if (line % kLatexTableRows == 0) {
				pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_page_break));
				pages.push_back(riscv_latex_page());
			}
		}

		// add empty line
		pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_empty));
		line++;
		lwidth = ext->insn_width;
	}

	// iterate through the table rows and add types to 
	for (auto &page : pages) {
		std::vector<riscv_type_ptr> types;
		for (auto &row : page.rows) {
			// accumulate types on this page
			if (row.opcode) {
				auto type = row.opcode->type;
				// exclude empty type
				if (type->parts.size() > 0 &&
						std::find(types.begin(), types.end(), type) == types.end()) {
					types.push_back(type);
				}
			}
		}
		if (types.size() == 0) continue;
		size_t bit_width = types.front()->parts[0].first.segments.front().first.msb + 1;
		auto default_type = types_by_name[std::to_string(bit_width)];
		if (!default_type) panic("can't find default type: %d", bit_width);

		// insert type header rows at top of the page
		for (auto ti = types.rbegin(); ti != types.rend(); ti++) {
			page.rows.insert(page.rows.begin(), riscv_latex_row(riscv_latex_type_type_spec, *ti));
		}
		page.rows.insert(page.rows.begin(), riscv_latex_row(riscv_latex_type_type_bitrange, default_type));
	}

	// create the table width specification
	std::stringstream ts;
	ts << "{";
	for (ssize_t i = 0 ; i < kLatexTableColumns; i++) ts << "p{0.02in}";
	ts << "p{2.0in}l}";
	for (ssize_t i = 0 ; i < kLatexTableColumns; i++) ts << "& ";
	ts << "& \\\\\n";

	// print document header
	printf("%s", kLatexDocumentBegin);
	printf("%s%s", kLatexTableBegin, ts.str().c_str());

	// iterate through pages and rows and printing them
	for (auto &page : pages) {
		for (auto &row : page.rows) {
			print_latex_row(row, ts.str());
		}
	}

	// print document trailer
	printf("%s", kLatexTableEnd);
	printf("%s", kLatexDocumentEnd);
}

void riscv_parse_meta::print_map()
{
	bool enable_colorize = enable_color && isatty(fileno(stdout));

	int i = 0;
	for (auto &opcode : opcodes) {
		if (i % 22 == 0) {
			printf("// %s", enable_colorize ? _COLOR_LEGEND : "");
			for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
				char c = (bit % 10) == 0 ? (bit / 10) + '0' : ' ';
				printf("%c", c);
			}
			printf("%s\n", enable_colorize ? _COLOR_RESET : "");
			printf("// %s", enable_colorize ? _COLOR_LEGEND : "");
			for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
				char c = (bit % 10) + '0';
				printf("%c", c);
			}
			printf("%s\n", enable_colorize ? _COLOR_RESET : "");
		}
		if (!opcode->match_extension(ext_subset)) continue;
		i++;
		printf("// ");
		ssize_t bit_width = opcode->extensions[0]->insn_width;
		for (ssize_t i = 0; i < kMaxInstructionWidth-bit_width; i++) printf(" ");
		for (ssize_t bit = bit_width-1; bit >= 0; bit--) {
			char c = ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : '.');
			switch (c) {
				case '0':
				case '1':
				{
					printf("%s%c%s",
						enable_colorize ? _COLOR_BITS : "",
						c,
						enable_colorize ? _COLOR_RESET : "");
					break;
				}
				default:
				{
					riscv_arg_ptr arg = opcode->find_arg(bit);
					if (arg) {
						printf("%s%c%s",
							enable_colorize ? riscv_colors_to_ansi_escape_sequence(
								arg->fg_color, arg->bg_color
							).c_str() : "",
							arg->char_code(),
							enable_colorize ? _COLOR_RESET : "");
					} else {
						printf("%c", c);
					}
					break;
				}
			}
		}
		if (enable_colorize) {
			printf(" %s%s%s %s",
				_COLOR_OPCODE, opcode->name.c_str(), _COLOR_RESET, colorize_args(opcode).c_str());
		} else {
			printf(" %s %s",
				opcode->name.c_str(), opcode->format->args.c_str());
		}
		ssize_t len = 34 - (opcode->name.length() + opcode->format->args.length());
		std::string ws;
		for (ssize_t i = 0; i < len; i++) ws += ' ';
		printf("%s%s# %s%s\n",
			ws.c_str(),
			enable_colorize ? _COLOR_EXT : "",
			opcode->extensions.front()->name.c_str(),
			enable_colorize ? _COLOR_RESET : "");
	}
}

void riscv_parse_meta::print_opcodes_h(bool no_comment, bool zero_not_oh)
{
	printf("//\n");
	printf("//  riscv-meta.h\n");
	printf("//\n");
	printf("//  DANGER - This is machine generated code\n");
	printf("//\n\n");

	printf("#ifndef riscv_meta_h\n");
	printf("#define riscv_meta_h\n");
	printf("\n");

	// Constraint enum
	printf("enum rvc_constraint\n{\n");
	printf("\trvc_end,\n");
	for (auto &constraint : constraints) {
		printf("\trvc_%s,\n", constraint->name.c_str());
	}
	printf("};\n\n");

	// CSR enum
	printf("enum riscv_csr\n{\n");
	for (auto &csr : csrs) {
		printf("\triscv_csr_%s = %s,\n", csr->name.c_str(), csr->number.c_str());
	}
	printf("};\n\n");

	// Integer register enum
	printf("enum riscv_ireg_name\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,\n", reg->name.c_str());
	}
	printf("};\n\n");

	// Integer register ABI enum 
	printf("enum riscv_ireg_abi\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Floating Point register enum
	printf("enum riscv_freg_name\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,\n", reg->name.c_str());
	}
	printf("};\n\n");

	// Floating Point register ABI enum
	printf("enum riscv_freg_abi\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Instruction codec enum
	printf("enum riscv_codec\n{\n");
	printf("\triscv_codec_unknown,\n");
	for (auto &codec : get_unique_codecs()) {
		printf("\triscv_codec_%s,\n", codec.c_str());
	}
	printf("};\n\n");

	// Instruction opcode enum
	printf("enum riscv_op\n{\n");
	printf("\triscv_op_unknown = 0,\n");
	for (auto &opcode : opcodes) {
		printf("\t%s = %lu,\n", opcode_format("riscv_op_", opcode, '_').c_str(), opcode->num);
	}
	printf("};\n\n");

	// Instruction compression data structure
	printf("struct riscv_comp_data\n{\n");
	printf("\tconst int op;\n");
	printf("\tconst rvc_constraint* constraints;\n");
	printf("};\n\n");

	// C interfaces
	printf("extern \"C\" {\n");
	printf("\textern const char* riscv_i_registers[];\n");
	printf("\textern const char* riscv_f_registers[];\n");
	printf("\textern const char* riscv_instruction_name[];\n");
	printf("\textern const riscv_codec riscv_instruction_codec[];\n");
	printf("\textern const riscv_wu riscv_instruction_match[];\n");
	printf("\textern const riscv_wu riscv_instruction_mask[];\n");
	printf("\textern const rvf* riscv_instruction_format[];\n");
	printf("\textern const riscv_comp_data* riscv_instruction_comp[];\n");
	printf("\textern const int riscv_instruction_decomp[];\n");
	printf("}\n");
	printf("\n");
	printf("#endif\n");
}

static const char* unknown_op_comment = "/*              unknown */ ";

static void print_array_unknown_str(const char *str, bool no_comment)
{
	printf("\t%s\"%s\",\n", no_comment ? "" : unknown_op_comment, str);
}

static void print_array_unknown_enum(const char *str, bool no_comment)
{
	printf("\t%s%s,\n", no_comment ? "" : unknown_op_comment, str);
}

static void print_array_unknown_int(uint32_t num, bool no_comment)
{
	printf("\t%s0x%08x,\n", no_comment ? "" : unknown_op_comment, num);
}

void riscv_parse_meta::print_opcodes_c(bool no_comment, bool zero_not_oh)
{
	printf("//\n");
	printf("//  riscv-meta.cc\n");
	printf("//\n");
	printf("//  DANGER - This is machine generated code\n");
	printf("//\n\n");

	printf("#include \"riscv-types.h\"\n");
	printf("#include \"riscv-format.h\"\n");
	printf("#include \"riscv-meta.h\"\n");
	printf("\n");

	// Integer register names
	printf("const char* riscv_i_registers[] = {\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Floating Point register names
	printf("const char* riscv_f_registers[] = {\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Instruction names
	printf("const char* riscv_instruction_name[] = {\n");
	print_array_unknown_str("unknown", no_comment);
	for (auto &opcode : opcodes) {
		std::string opcode_name = opcode_format("", opcode, '.', false);
		printf("\t%s\"%s\",\n",
			opcode_comment(opcode, no_comment).c_str(),
			opcode_name.c_str());
	}
	printf("};\n\n");

	// Instruction codecs
	printf("const riscv_codec riscv_instruction_codec[] = {\n");
	print_array_unknown_enum("riscv_codec_unknown", no_comment);
	for (auto &opcode : opcodes) {
		std::string codec_name = split(opcode->codec->name, "+", false, false)[0];
		printf("\t%sriscv_codec_%s,\n",\
			opcode_comment(opcode, no_comment).c_str(),
			codec_name.c_str());
	}
	printf("};\n\n");

	// Instruction match bits
	printf("const riscv_wu riscv_instruction_match[] = {\n");
	print_array_unknown_int(0, no_comment);
	for (auto &opcode : opcodes) {
		printf("\t%s0x%08x,\n",
			opcode_comment(opcode, no_comment).c_str(),
			(uint32_t)opcode->match);
	}
	printf("};\n\n");

	// Instruction mask bits
	printf("const riscv_wu riscv_instruction_mask[] = {\n");
	print_array_unknown_int(0, no_comment);
	for (auto &opcode : opcodes) {
		printf("\t%s0x%08x,\n",
			opcode_comment(opcode, no_comment).c_str(),
			(uint32_t)opcode->mask);
	}
	printf("};\n\n");

	// Instruction formats
	printf("const rvf* riscv_instruction_format[] = {\n");
	print_array_unknown_int(0, no_comment);
	for (auto &opcode : opcodes) {
		printf("\t%sriscv_fmt_%s,\n",
			opcode_comment(opcode, no_comment).c_str(),
			opcode->format->name.c_str());
	}
	printf("};\n\n");

	// Compression constraints
	for (auto &opcode : opcodes) {
		if (!opcode->compressed) continue;
		std::string cop_name = "rvcc_" + opcode_format("", opcode, '_') + "[] =";
		printf("const rvc_constraint %-30s { ", cop_name.c_str());
		for (auto &constraint : opcode->compressed->constraint_list) {
			printf("rvc_%s, ", constraint->name.c_str());
		}
		printf("rvc_end };\n");
	}
	printf("\n");

	// Compression data
	for (auto &opcode : opcodes) {
		if (opcode->compressions.size() == 0) continue;
		std::string op_constraint_data = "rvcd_" + opcode_format("", opcode, '_') + "[] =";
		printf("const riscv_comp_data %-30s { ", op_constraint_data.c_str());
		for (auto comp : opcode->compressions) {
			printf("{ %s, %s }, ",
				(zero_not_oh ?
					format_string("%lu", comp->comp_opcode->num).c_str() :
					opcode_format("riscv_op_", comp->comp_opcode, '_').c_str()),
				opcode_format("rvcc_", comp->comp_opcode, '_').c_str());
		}
		printf("{ riscv_op_unknown, nullptr } };\n");
	}
	printf("\n");

	// RVC compression table
	printf("const riscv_comp_data* riscv_instruction_comp[] = {\n");
	print_array_unknown_enum("nullptr", no_comment);
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		std::string opcode_data = opcode_format("rvcd_", opcode, '_');
		std::string opcode_constraint = "nullptr";
		printf("\t%s%s,\n",
			opcode_comment(opcode, no_comment).c_str(),
			opcode->compressions.size() > 0 ? opcode_data.c_str() : "nullptr");
	}
	printf("};\n\n");

	// RVC decompression table
	printf("const int riscv_instruction_decomp[] = {\n");
	print_array_unknown_enum("riscv_op_unknown", no_comment);
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		printf("\t%s%s,\n",
			opcode_comment(opcode, no_comment).c_str(),
			opcode->compressed ?
				(zero_not_oh ?
					format_string("%lu", opcode->compressed->decomp_opcode->num).c_str() :
					opcode_format("riscv_op_", opcode->compressed->decomp_opcode, '_').c_str()) :
				(zero_not_oh ?
					"0" :
					"riscv_op_unknown"));
	}
	printf("};\n\n");
}

void riscv_parse_meta::print_switch_c(bool no_comment, bool zero_not_oh)
{
	std::vector<std::string> mnems;

	// create mnemonics for instruction set widths
	for (auto &ext : extensions) {
		std::string mnem = ext->prefix + std::to_string(ext->isa_width);
		if (std::find(mnems.begin(), mnems.end(), mnem) == mnems.end())
			mnems.push_back(mnem);
	}

	// create mnemonics for instruction set extensions
	for (auto &ext : extensions) {
		std::string mnem = ext->prefix + ext->alpha_code;
		if (std::find(mnems.begin(), mnems.end(), mnem) == mnems.end())
			mnems.push_back(mnem);
	}

	// print opcode decoder
	printf("template <");
	for (auto mi = mnems.begin(); mi != mnems.end(); mi++) {
		if (mi != mnems.begin()) printf(", ");
		printf("bool %s", mi->c_str());
	}
	printf(">\n");
	printf("void riscv_decode_opcode(riscv_decode &dec, riscv_lu inst)\n");
	printf("{\n");
	print_switch_decoder_node(root_node, 1);
	printf("}\n\n");

	// print type decoder
	printf("void riscv_decode_type(riscv_decode &dec, riscv_lu inst)\n");
	printf("{\n");
	printf("\tdec.codec = riscv_instruction_codec[dec.op];\n");
	printf("\tswitch (dec.codec) {\n");
	for (auto &codec : get_unique_codecs()) {
		printf("\t\tcase %-30s %-40s break;\n",
			format_string("riscv_codec_%s:", codec.c_str()).c_str(),
			format_string("riscv_decode_%s(dec, inst);", codec.c_str()).c_str());
	}
	printf("\t};\n");
	printf("}\n\n");

	// print immediate decoders
	for (auto arg: args) {
		printf("typedef %-67s riscv_arg_%s;\n",
			arg->bitspec.to_template().c_str(), arg->name.c_str());
	}
	printf("\n");
}

void riscv_parse_meta::generate_codec_node(riscv_codec_node &node, riscv_opcode_list &opcode_list)
{
	// calculate row coverage for each column
	std::vector<ssize_t> sum;
	sum.resize(32);
	for (auto &opcode : opcode_list) {
		for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
			if ((opcode->mask & (1 << bit)) && !(opcode->done & (1 << bit))) sum[bit]++;
		}
	}

	// find column with maximum row coverage
	ssize_t max_rows = 0;
	for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
		if (sum[bit] > max_rows) max_rows = sum[bit];
	}

	if (max_rows == 0) return; // no bits to match

	// select bits that cover maximum number of rows
	for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
		if (sum[bit] == max_rows) node.bits.push_back(bit);
	}

	// find distinct values for the chosen bits
	for (auto &opcode : opcode_list) {

		ssize_t val = 0;
		bool partial_match = false;
		for (auto &bit : node.bits) {
			if (!(opcode->mask & (1 << bit))) {
				partial_match = true;
			} else {
				val = (val << 1) | ((opcode->match & (1 << bit)) ? 1 : 0);
			}
		}

		// partial match is default in switch containing more specific masks
		if (partial_match) {
			val = DEFAULT;
		}

		// add value to list
		if (std::find(node.vals.begin(), node.vals.end(), val) == node.vals.end()) {
			node.vals.push_back(val);
		}

		// create opcode list for this value and add opcode to the list
		auto val_opcode_list_i = node.val_opcodes.find(val);
		if (val_opcode_list_i == node.val_opcodes.end()) {
			val_opcode_list_i = node.val_opcodes.insert(node.val_opcodes.begin(),
				std::pair<ssize_t,riscv_opcode_list>(val, riscv_opcode_list()));
		}
		val_opcode_list_i->second.push_back(opcode);
	}

	// sort values
	std::sort(node.vals.begin(), node.vals.end());

	// mark chosen bits as done
	for (auto &opcode : opcode_list) {
		for (auto &bit : node.bits) {
			opcode->done |= (1 << bit);
		}
	}

	// recurse
	for (auto &val : node.vals) {
		if (node.val_decodes.find(val) == node.val_decodes.end()) {
			node.val_decodes.insert(node.val_decodes.begin(),
				std::pair<ssize_t,riscv_codec_node>(val, riscv_codec_node()));
		}
		generate_codec_node(node.val_decodes[val], node.val_opcodes[val]);
	}
}

void riscv_parse_meta::print_switch_decoder_node(riscv_codec_node &node, size_t indent)
{
	for (size_t i = 0; i < indent; i++) printf("\t");
	printf("switch (%s) {\n", format_bitmask(node.bits, "inst", true).c_str());
	for (auto &val : node.vals) {
		auto opcode_list = node.val_opcodes[val];
		if (node.val_decodes[val].bits.size() == 0 && opcode_list.size() >= 1) {
			for (size_t i = 0; i < indent; i++) printf("\t");
			if (val == DEFAULT) {
				printf("\tdefault: ");
			} else {
				printf("\tcase %lu: ", val);
			}

			// resolve distinct number of isa widths for this opcode
			std::vector<size_t> opcode_widths;
			for (auto opcode : opcode_list) {
				for (auto &ext : opcode->extensions) {
					if (std::find(opcode_widths.begin(), opcode_widths.end(),
							ext->isa_width) == opcode_widths.end()) {
						opcode_widths.push_back(ext->isa_width);
					}
				}
			}
			auto opcode = opcode_list.front();

			// different opcodes that share encoding on different isa widths
			if (opcode_list.size() > 1 && opcode_list.size() == opcode_widths.size())
			{
				// conditionals for different opcodes sharing encoding on different isa widths
				printf("\n");
				for (auto oi = opcode_list.begin(); oi != opcode_list.end(); oi++) {
					auto opcode = *oi;
					for (size_t i = 0; i < indent; i++) printf("\t");
					printf("\t\t%sif (%s && rv%lu) dec.op = %s;\n",
						oi != opcode_list.begin() ? "else " : "",
						opcode_isa_shortname(opcode).c_str(), opcode->extensions.front()->isa_width,
						opcode_format("riscv_op_", opcode, '_').c_str());
				}
				for (size_t i = 0; i < indent; i++) printf("\t");
				printf("\t\tbreak;\n");
			}
			else
			{
				// if ambiguous, chooses first opcode
				if (opcode_widths.size() == 1) {
					printf("if (%s && rv%lu) dec.op = %s; break;",
						opcode_isa_shortname(opcode).c_str(), opcode->extensions.front()->isa_width,
						opcode_format("riscv_op_", opcode, '_').c_str());
				} else {
					printf("if (%s) dec.op = %s; break;",
						opcode_isa_shortname(opcode).c_str(),
						opcode_format("riscv_op_", opcode, '_').c_str());
				}

				// if ambiguous, add comment
				if (opcode_list.size() > 1) {
					printf(" //");
					for (auto &opcode : opcode_list) {
						printf(" %s", opcode->name.c_str());
					}
				}
				printf("\n");
			}

		} else {
			for (size_t i = 0; i < indent; i++) printf("\t");
			if (val == DEFAULT) {
				printf("\tdefault: ");
			} else {
				printf("\tcase %lu:\n", val);
			}
			for (size_t i = 0; i < indent; i++) printf("\t");
			printf("\t\t//");
			int count = 0;
			for (auto &opcode : opcode_list) {
				if (count++ == 12) {
					printf(" ..."); break;
				}
					printf(" %s", opcode->name.c_str());
			}
			printf("\n");
			if (node.val_decodes[val].bits.size() > 0) {
				print_switch_decoder_node(node.val_decodes[val], indent + 2);
			}
			for (size_t i = 0; i < indent; i++) printf("\t");
			printf("\t\tbreak;\n");
		}
	}
	for (size_t i = 0; i < indent; i++) printf("\t");
	printf("}\n");
}

/* main */

int main(int argc, const char *argv[])
{
	riscv_parse_meta inst_set;

	bool print_latex = false;
	bool print_map = false;
	bool no_comment = false;
	bool zero_not_oh = false;
	bool print_switch_c = false;
	bool print_opcodes_h = false;
	bool print_opcodes_c = false;
	bool help_or_error = false;
	std::string isa_spec = "";

	cmdline_option options[] =
	{
		{ "-c", "--color", cmdline_arg_type_none,
			"Enable Color",
			[&](std::string s) { return (enable_color = true); } },
		{ "-I", "--isa-subset", cmdline_arg_type_string,
			"ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)",
			[&](std::string s) { isa_spec = s; return true; } },
		{ "-r", "--read-isa", cmdline_arg_type_string,
			"Read instruction set metadata from directory",
			[&](std::string s) { return inst_set.read_metadata(s); } },
		{ "-l", "--print-latex", cmdline_arg_type_none,
			"Print LaTeX",
			[&](std::string s) { return (print_latex = true); } },
		{ "-m", "--print-map", cmdline_arg_type_none,
			"Print map",
			[&](std::string s) { return (print_map = true); } },
		{ "-N", "--no-comment", cmdline_arg_type_none,
			"Don't emit comments in generated source",
			[&](std::string s) { return (no_comment = true); } },
		{ "-0", "--zero-not-oh", cmdline_arg_type_none,
			"Use numeric constants in generated source",
			[&](std::string s) { return (zero_not_oh = true); } },
		{ "-H", "--print-opcodes-h", cmdline_arg_type_none,
			"Print C header",
			[&](std::string s) { return (print_opcodes_h = true); } },
		{ "-C", "--print-opcodes-c", cmdline_arg_type_none,
			"Print C source",
			[&](std::string s) { return (print_opcodes_c = true); } },
		{ "-S", "--print-switch-c", cmdline_arg_type_none,
			"Print C switch",
			[&](std::string s) { return (print_switch_c = true); } },
		{ "-h", "--help", cmdline_arg_type_none,
			"Show help",
			[&](std::string s) { return (help_or_error = true); } },
		{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
	};

	auto result = cmdline_option::process_options(options, argc, argv);
	if (!result.second) {
		help_or_error = true;
	} else if (result.first.size() != 0) {
		printf("%s: wrong number of arguments\n", argv[0]);
		help_or_error = true;
	}

	if ((help_or_error |= !print_latex && !print_map &&
		!print_switch_c && !print_opcodes_h && !print_opcodes_c))
	{
		printf("usage: %s\n", argv[0]);
		cmdline_option::print_options(options);
		return false;
	}

	inst_set.ext_subset = inst_set.decode_isa_extensions(isa_spec);

	inst_set.generate_map();

	if (print_latex) {
		inst_set.print_latex();
	}

	if (print_map) {
		inst_set.print_map();
	}

	if (print_opcodes_h) {
		inst_set.print_opcodes_h(no_comment, zero_not_oh);
	}

	if (print_opcodes_c) {
		inst_set.print_opcodes_c(no_comment, zero_not_oh);
	}

	if (print_switch_c) {
		inst_set.generate_codec();
		inst_set.print_switch_c(no_comment, zero_not_oh);
	}

	exit(0);
}
