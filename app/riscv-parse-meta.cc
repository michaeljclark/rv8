#include <cstdio>
#include <cstdlib>
#include <cinttypes>
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
static const ssize_t kLatexTableRows = 52;
static const ssize_t kLatexReserveRows = 5;

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

struct riscv_codec_node
{
	std::vector<ssize_t> bits;
	std::vector<ssize_t> vals;
	std::map<ssize_t,riscv_opcode_list> val_opcodes;
	std::map<ssize_t,riscv_codec_node> val_decodes;

	void clear();
};

struct riscv_parse_meta : riscv_meta_model
{
	riscv_extension_list     ext_subset;
	riscv_codec_node         root_node;
	riscv_opcode_ptr         unknown;

	void generate_map();
	void generate_codec(bool include_pseudo);

	std::string colorize_args(riscv_opcode_ptr opcode);
	std::vector<std::string> get_unique_codecs();

	static std::string latex_utf_substitute(std::string str);

	void print_latex_row(riscv_latex_row &row, std::string ts, bool remove_question_marks);
	void print_latex(bool remove_question_marks);
	void print_map(bool print_map_instructions);
	void print_c_header(std::string filename);
	void print_args_h();
	void print_jit_h();
	void print_jit_cc();
	void print_meta_h(bool no_comment = false, bool zero_not_oh = false);
	void print_meta_cc(bool no_comment = false, bool zero_not_oh = false);
	void print_strings_h(bool no_comment = false, bool zero_not_oh = false);
	void print_strings_cc(bool no_comment = false, bool zero_not_oh = false);
	void print_switch_h(bool no_comment = false, bool zero_not_oh = false);

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

void riscv_codec_node::clear()
{
	bits.clear();
	vals.clear();
	val_opcodes.clear();
	val_decodes.clear();
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

void riscv_parse_meta::generate_codec(bool include_pseudo)
{
	// make list of opcodes to include
	riscv_opcode_list opcodes_copy;
	for (auto &opcode : opcodes) {
		if (include_pseudo || opcode->key.find("@") == std::string::npos) {
			opcodes_copy.push_back(opcode);
		}
	}

	// generate decode
	root_node.clear();
	generate_codec_node(root_node, opcodes_copy);
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
		std::string codec_name = format_codec("", codec, "_");
		if (std::find(codec_names.begin(), codec_names.end(), codec_name) == codec_names.end()) {
			codec_names.push_back(codec_name);
		}
	}
	return codec_names;
}

std::string riscv_parse_meta::latex_utf_substitute(std::string str)
{
	str = replace(str, "∞", "\\infty");
	str = replace(str, "{", "\\left\\{");
	str = replace(str, "}", "\\right\\}");
	str = replace(str, "≠", "\\neq");
	str = replace(str, "≤", "\\leq");
	str = replace(str, "≥", "\\geq");
	str = replace(str, "⊂", "\\subset");
	str = replace(str, "⊃", "\\supset");
	str = replace(str, "⊆", "\\subseteq");
	str = replace(str, "⊇", "\\supseteq");
	str = replace(str, "⊕", "\\oplus");
	str = replace(str, "⊗", "\\otimes");
	str = replace(str, "⊖", "\\ominus");
	str = replace(str, "→", "\\rightarrow");
	str = replace(str, "←", "\\leftarrow");
	str = replace(str, "∨", "\\vee");
	str = replace(str, "∧", "\\wedge");
	str = replace(str, "⊻", "\\veebar");
	str = replace(str, "⊼", "\\wedgebar");
	str = replace(str, "×", "\\times");
	str = replace(str, "÷", "\\div");
	str = replace(str, "«", "\\ll");
	str = replace(str, "»", "\\gg");
	str = replace(str, "∥", "\\parallel");
	return str;
}

void riscv_parse_meta::print_latex_row(riscv_latex_row &row, std::string ts, bool remove_question_marks)
{
	switch (row.row_type) {
		case riscv_latex_type_empty:
			printf("\\tabularnewline\n");
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
			ssize_t bit_width = opcode->extensions[0]->inst_width;

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
					if (arg->type == "simm" || arg->type == "uimm" || arg->type == "offset") {
						auto spec = arg->bitspec;
						if (spec.segments.size() == 1 && spec.segments.front().second.size() == 0) {
							// indicate size for immediates with no custom bit decoding spec
							str += std::to_string(size);
						} else {
							// indicate pattern for immediates with custom bit decoding spec
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
					}
					if (opcode->compressed) {
						for (auto &constraint : opcode->compressed->constraint_list) {
							if (constraint->hint.size() == 0) continue;
							std::string arg = split(constraint->name, "_", false, false)[0];
							if (str.find(arg) != str.length() - arg.length()) continue;
							std::string render_hint = replace(constraint->hint, arg, "");
							str += "$" + latex_utf_substitute(render_hint) + "$";
						}
					}
				}
				if (remove_question_marks) {
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
			if (arg_comps.size() == 1 && arg_comps[0] == "none") arg_comps[0] = ""; 

			// print this row
			printf("%s & \\scriptsize{%s %s} \\\\\n\\cline{1-%ld}\n",
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
				auto &spec = named_bitspec.first;
				auto str = named_bitspec.second;
				ssize_t msb = spec.segments.front().first.msb;
				ssize_t lsb = spec.segments.back().first.lsb;
				ssize_t size = msb - lsb + 1;

				if (str == "imm" || str == "offset") {
					if (spec.segments.size() == 1 && spec.segments.front().second.size() == 0) {
						// indicate size for immediates with no custom bit decoding spec
						str += std::to_string(size);
					} else {
						// indicate pattern for immediates with custom bit decoding spec
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
			printf("%s & \\scriptsize{\\bf %s} \\\\\n\\cline{1-%ld}\n",
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
					ls << "\\multicolumn{" << tsize << "}{c}{\\scriptsize{" << msb << "}} & ";
				} else {
					auto lsize = size >> 1;
					auto rsize = tsize - lsize;
					ls << "\\multicolumn{" << lsize << "}{l}{\\scriptsize{" << msb << "}} & "
					   << "\\multicolumn{" << rsize << "}{r}{\\scriptsize{" << lsb << "}} & ";
				}
			}
			// print this row
			printf("%s \\\\\n\\cline{1-%ld}\n",
				ls.str().c_str(), kLatexTableColumns);
			break;
		}
	}
}

void riscv_parse_meta::print_latex(bool remove_question_marks)
{
	// paginate opcodes ordered by extension
	// adding type and extension headings, page breaks and continuations
	size_t line = 0;
	ssize_t inst_width = 0;
	std::vector<riscv_latex_page> pages;
	for (auto &ext : extensions) {
		// check if this extension is in the selected subset
		if (ext->opcodes.size() == 0 || (ext_subset.size() > 0 &&
			std::find(ext_subset.begin(), ext_subset.end(), ext) == ext_subset.end())) {
			continue;
		}

		// check we have opcodes in this section (excluding pseudo opcodes)
		size_t count = 0;
		for (auto opcode : ext->opcodes) {
			count += opcode->name.find("@") == 0 ? 0 : 1;
		}
		if (count == 0) continue;

		// add new page
		if (pages.size() == 0) pages.push_back(riscv_latex_page());

		// break to a new page if the instruction width changes
		if (inst_width != 0 && inst_width != ext->inst_width) {
			pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_page_break));
			pages.push_back(riscv_latex_page());
			line = 0;
		}

		// don't start new extension heading unless we have reserved space
		if (line % kLatexTableRows > kLatexTableRows - kLatexReserveRows) {
			pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_page_break));
			pages.push_back(riscv_latex_page());
			line = 0;
		}

		// add extension heading
		pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_empty));
		pages.back().rows.push_back(riscv_latex_row(riscv_latex_type_extension_heading, ext));
		line++;

		// add opcodes
		for (auto &opcode : ext->opcodes) {
			// skip psuedo opcode
			if (opcode->name.find("@") == 0) continue;
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

		inst_width = ext->inst_width;
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
	for (ssize_t i = 0 ; i < kLatexTableColumns; i++) ts << "p{0.05mm}";
	ts << "p{50mm}l}";
	for (ssize_t i = 0 ; i < kLatexTableColumns; i++) ts << "& ";
	ts << "& \\\\\n";

	// print document header
	printf("%s", kLatexDocumentBegin);
	printf("%s%s", kLatexTableBegin, ts.str().c_str());

	// iterate through pages and rows and printing them
	for (auto &page : pages) {
		for (auto &row : page.rows) {
			print_latex_row(row, ts.str(), remove_question_marks);
		}
	}

	// print document trailer
	printf("%s", kLatexTableEnd);
	printf("%s", kLatexDocumentEnd);
}

void riscv_parse_meta::print_map(bool print_map_instructions)
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
		ssize_t bit_width = opcode->extensions[0]->inst_width;
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
		ssize_t len = 30 - (opcode->name.length() + opcode->format->args.length());
		std::string ws;
		for (ssize_t i = 0; i < len; i++) ws += ' ';
		printf("%s%s# %s%s%s\n",
			ws.c_str(),
			enable_colorize ? _COLOR_EXT : "",
			opcode->extensions.front()->name.c_str(),
			enable_colorize ? _COLOR_RESET : "",
			print_map_instructions ? format_string("   %s", opcode->instruction.c_str()).c_str() : "");
	}
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

static void print_array_unknown_uint64(uint64_t num, bool no_comment)
{
	printf("\t%s0x%016" PRIx64 ",\n", no_comment ? "" : unknown_op_comment, num);
}

void riscv_parse_meta::print_c_header(std::string filename)
{
	printf("//\n");
	printf("//  %s\n", filename.c_str());
	printf("//\n");
	printf("//  DANGER - This is machine generated code\n");
	printf("//\n\n");
}

void riscv_parse_meta::print_args_h()
{
	print_c_header("riscv-codec-args.h");
	printf("#ifndef riscv_codec_args_h\n");
	printf("#define riscv_codec_args_h\n");
	printf("\n");

	// print immediate decoders
	for (auto arg: args) {
		printf("typedef %s%-60s arg_%s;\n",
			(arg->type == "simm" || arg->type == "offset") ? "simm_arg_t" : "uimm_arg_t",
			arg->bitspec.to_template().c_str(),
			arg->name.c_str());
	}
	printf("\n");
	printf("#endif\n");
}

void riscv_parse_meta::print_jit_h()
{
	print_c_header("riscv-jit.h");
	printf("#ifndef riscv_jit_h\n");
	printf("#define riscv_jit_h\n");
	printf("\n");
	printf("namespace riscv\n{\n");
	for (auto &opcode : opcodes) {
		// exclude compressed and psuedo instructions
		if (opcode->compressed || opcode->key.find("@") == 0) continue;

		// create emit interface
		std::string emit_name = opcode_format("emit_", opcode, "_");
		std::vector<std::string> arg_list;
		for (auto &arg : opcode->codec->args) {
			auto type_name = format_type(arg) + " " + arg->name;
			arg_list.push_back(type_name);
		}

		// output emit interface
		printf("\tuint64_t %s(%s);\n",
			emit_name.c_str(), join(arg_list, ", ").c_str());
	}
	printf("}\n");
	printf("\n");
	printf("#endif\n");
}

void riscv_parse_meta::print_jit_cc()
{
	print_c_header("riscv-jit.cc");

	printf("#include <cstdint>\n");
	printf("#include <cstdlib>\n");
	printf("#include <cassert>\n");
	printf("\n");
	printf("#include \"riscv-types.h\"\n");
	printf("#include \"riscv-endian.h\"\n");
	printf("#include \"riscv-jit.h\"\n");
	printf("#include \"riscv-meta.h\"\n");
	printf("#include \"riscv-codec.h\"\n");
	printf("\n");

	for (auto &opcode : opcodes) {
		// exclude compressed and psuedo instructions
		if (opcode->compressed || opcode->key.find("@") == 0) continue;

		// create emit interface
		std::string emit_name = opcode_format("emit_", opcode, "_");
		std::vector<std::string> arg_list;
		for (auto &arg : opcode->codec->args) {
			auto type_name = format_type(arg) + " " + arg->name;
			arg_list.push_back(type_name);
		}

		// output emit interface
		printf("uint64_t riscv::%s(%s)\n{\n",
			emit_name.c_str(), join(arg_list, ", ").c_str());
		printf("\triscv_decode dec;\n");
		if (opcode->codec->args.size() > 0) {
			std::vector<std::string> check_list;
			for (auto &arg : opcode->codec->args) {
				auto check_name = arg->name + ".valid()";
				check_list.push_back(check_name);
			}
			printf("\tif (!(%s)) return 0; /* illegal instruction */\n",
				join(check_list, " && ").c_str());
		}
		printf("\tdec.op = %s;\n", opcode_format("riscv_op_", opcode, "_").c_str());
		for (auto &arg : opcode->codec->args) {
			if (arg->type == "offset" || arg->type == "simm" || arg->type == "uimm") {
				printf("\tdec.imm = %s;\n", arg->name.c_str());
			} else if (arg->type == "ireg") {
				printf("\tdec.%s = %s;\n", arg->name.c_str(), arg->name.c_str());
			} else if (arg->type == "freg") {
				printf("\tdec.%s = %s;\n", arg->name.substr(1).c_str(), arg->name.c_str());
			} else if (arg->type == "arg") {
				printf("\tdec.%s = %s;\n", arg->name.c_str(), arg->name.c_str());
			} else {
				printf("/* dec.? = %s unhandled */\n", arg->name.c_str());
			}
		}
		printf("\treturn riscv_encode_inst(dec);\n");
		printf("}\n\n");
	}
	printf("\n");
}

void riscv_parse_meta::print_meta_h(bool no_comment, bool zero_not_oh)
{
	print_c_header("riscv-meta.h");
	printf("#ifndef riscv_meta_h\n");
	printf("#define riscv_meta_h\n");
	printf("\n");

	printf("#ifdef __cplusplus\n");
	printf("extern \"C\" {\n");
	printf("#endif\n");
	printf("\n");

	// Enums
	std::string last_group;
	for (auto &enumv : enums) {
		if (last_group != enumv->group) {
			if (last_group.size() != 0) printf("};\n\n");
			printf("enum riscv_%s\n{\n", enumv->group.c_str());
		}
		printf("\triscv_%s_%s = %" PRId64 ",%s\n",
			enumv->group.c_str(), enumv->name.c_str(), enumv->value,
			no_comment || enumv->description.size() == 0 ? "" :
				format_string("\t/* %s */", enumv->description.c_str()).c_str());
		last_group = enumv->group;
	}
	if (last_group.size() != 0) printf("};\n\n");

	// Constraint enum
	printf("enum rvc_constraint\n{\n");
	printf("\trvc_end,\n");
	for (auto &constraint : constraints) {
		printf("\trvc_%s,%s\n", constraint->name.c_str(),
			no_comment ? "" : format_string("\t/* %s */", constraint->expression.c_str()).c_str());
	}
	printf("};\n\n");

	// CSR enum
	printf("enum riscv_csr\n{\n");
	for (auto &csr : csrs) {
		printf("\triscv_csr_%s = %s,%s\n", csr->name.c_str(), csr->number.c_str(),
			no_comment || csr->description.size() == 0 ? "" :
				format_string("\t/* %s */", csr->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Integer register number enum
	printf("enum riscv_ireg_num\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,%s\n", reg->name.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Integer register ABI name enum
	printf("enum riscv_ireg_name\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,%s\n", reg->alias.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Floating Point register number enum
	printf("enum riscv_freg_num\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,%s\n", reg->name.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Floating Point register ABI name enum
	printf("enum riscv_freg_name\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,%s\n", reg->alias.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Instruction codec enum
	printf("enum riscv_codec\n{\n");
	printf("\triscv_codec_unknown,\n");
	for (auto &codec : get_unique_codecs()) {
		printf("\triscv_codec_%s,\n", codec.c_str());
	}
	printf("};\n\n");

	// Instruction argument name enum
	printf("enum riscv_arg_name\n{\n");
	printf("\triscv_arg_name_none,\n");
	for (auto &arg : args) {
		printf("\triscv_arg_name_%s,\n", arg->name.c_str());
	}
	printf("};\n\n");

	// instruction argument type enum
	std::vector<std::string> arg_types;
	for (auto &arg : args) {
		auto type_name = format_type(arg);
		if (std::find(arg_types.begin(), arg_types.end(), type_name) == arg_types.end()) {
			arg_types.push_back(type_name);
		}
	}
	printf("enum riscv_arg_type\n{\n");
	printf("\triscv_arg_type_none,\n");
	for (auto &arg_type : arg_types) {
		printf("\triscv_arg_type_%s,\n", arg_type.c_str());
	}
	printf("};\n\n");

	// Instruction opcode enum
	printf("enum riscv_op\n{\n");
	printf("\triscv_op_unknown = 0,\n");
	for (auto &opcode : opcodes) {
		printf("\t%s = %lu,%s\n", opcode_format("riscv_op_", opcode, "_").c_str(), opcode->num,
			no_comment || opcode->long_name.size() == 0 ? "" :
				format_string("\t/* %s */", opcode->long_name.c_str()).c_str());
	}
	printf("};\n\n");

	// Instruction compression data structure
	printf("struct riscv_comp_data\n{\n");
	printf("\tconst int op;\n");
	printf("\tconst rvc_constraint* constraints;\n");
	printf("};\n\n");

	// Instruction arg structure
	printf("struct riscv_arg_data\n{\n");
	printf("\tconst riscv_arg_name arg_name;\n");
	printf("\tconst riscv_arg_type arg_type;\n");
	printf("\tconst riscv_type type;\n");
	printf("\tconst unsigned int width;\n");
	printf("};\n\n");

	// Opcode metadata tables
	printf("extern const riscv_codec riscv_inst_codec[];\n");
	printf("extern const char* riscv_inst_format[];\n");
	printf("extern const riscv_arg_data* riscv_inst_arg_data[];\n");
	printf("extern const uint64_t riscv_inst_match[];\n");
	printf("extern const uint64_t riscv_inst_mask[];\n");
	for (auto isa_width : isa_width_prefixes()) {
		printf("extern const riscv_comp_data* riscv_inst_comp_%s[];\n",
			isa_width.second.c_str());
	}
	for (auto isa_width : isa_width_prefixes()) {
		printf("extern const int riscv_inst_decomp_%s[];\n",
			isa_width.second.c_str());
	}
	printf("\n");
	printf("#ifdef __cplusplus\n");
	printf("}\n");
	printf("#endif\n");
	printf("\n");
	printf("#endif\n");
}

void riscv_parse_meta::print_meta_cc(bool no_comment, bool zero_not_oh)
{
	print_c_header("riscv-meta.cc");

	printf("#include \"riscv-types.h\"\n");
	printf("#include \"riscv-format.h\"\n");
	printf("#include \"riscv-meta.h\"\n");
	printf("\n");	

	// Compression constraints
	for (auto &opcode : opcodes) {
		if (!opcode->compressed) continue;
		std::string cop_name = "rvcc_" + opcode_format("", opcode, "_") + "[] =";
		printf("const rvc_constraint %s {\n", cop_name.c_str());
		for (auto &constraint : opcode->compressed->constraint_list) {
			printf("\trvc_%s,\n", constraint->name.c_str());
		}
		printf("\trvc_end\n};\n\n");
	}
	printf("\n");

	// Compression data
	std::set<std::string> rvcd_set;
	for (auto isa_width : isa_width_prefixes()) {
		std::string isa_prefix = "rvcd_" + isa_width.second + "_";
		for (auto &opcode : opcodes) {
			riscv_compressed_list include_compressions;
			for (auto comp : opcode->compressions) {
				bool include_isa = false;
				for (auto &ext_a : comp->comp_opcode->extensions) {
					if (ext_a->isa_width != isa_width.first) continue;
					for (auto &ext_b : opcode->extensions) {
						if (ext_b->isa_width != isa_width.first) continue;
						include_isa = true;
					}
				}
				if (include_isa) include_compressions.push_back(comp);
			}
			if (include_compressions.size() == 0) continue;
			std::string rvcd_name = opcode_format(isa_prefix, opcode, "_");
			rvcd_set.insert(rvcd_name);
			printf("const riscv_comp_data %s[] = {\n", rvcd_name.c_str());
			for (auto &comp : include_compressions) {
				printf("\t{ %s, %s },\n",
					(zero_not_oh ?
						format_string("%lu", comp->comp_opcode->num).c_str() :
						opcode_format("riscv_op_", comp->comp_opcode, "_").c_str()),
					opcode_format("rvcc_", comp->comp_opcode, "_").c_str());
			}
			printf("\t{ %s, nullptr }\n};\n\n",
				(zero_not_oh ?
						"0" :
						"riscv_op_unknown"));
		}
		printf("\n");
	}

	// Codec argument data
	for (auto &codec : codecs) {
		printf("const riscv_arg_data riscv_codec_%s_args[] = {\n", format_codec("", codec, "_", false).c_str());
		for (auto &arg : codec->args) {
			size_t width = arg->bitspec.decoded_msb() > 0 ?
				arg->bitspec.decoded_msb() + 1 :
				arg->bitspec.segments.front().first.msb - arg->bitspec.segments.back().first.lsb + 1;
			printf("\t{ riscv_arg_name_%s, riscv_arg_type_%s, riscv_type_%s, %lu },\n",
				arg->name.c_str(), format_type(arg).c_str(), arg->type.c_str(), width);
		}
		printf("\t{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }\n};\n\n");
	}

	// Instruction codecs
	printf("const riscv_codec riscv_inst_codec[] = {\n");
	print_array_unknown_enum("riscv_codec_unknown", no_comment);
	for (auto &opcode : opcodes) {
		std::string codec_name = format_codec("", opcode->codec, "_");
		printf("\t%sriscv_codec_%s,\n",\
			opcode_comment(opcode, no_comment).c_str(),
			codec_name.c_str());
	}
	printf("};\n\n");

	// Instruction formats
	printf("const char* riscv_inst_format[] = {\n");
	print_array_unknown_enum("riscv_fmt_none", no_comment);
	for (auto &opcode : opcodes) {
		printf("\t%s%s,\n",
			opcode_comment(opcode, no_comment).c_str(),
			format_format("riscv_fmt_", opcode->format, '_').c_str());
	}
	printf("};\n\n");

	// Instruction codecs argument data table
	printf("const riscv_arg_data* riscv_inst_arg_data[] = {\n");
	printf("\triscv_codec_none_args,\n");
	for (auto &opcode : opcodes) {
		printf("\t%sriscv_codec_%s_args,\n",
			opcode_comment(opcode, no_comment).c_str(), format_codec("", opcode->codec, "_", false).c_str());
	}
	printf("};\n\n");

	// Instruction match bits
	printf("const uint64_t riscv_inst_match[] = {\n");
	print_array_unknown_uint64(0ULL, no_comment);
	for (auto &opcode : opcodes) {
		printf("\t%s0x%016" PRIx64 ",\n",
			opcode_comment(opcode, no_comment).c_str(), opcode->match);
	}
	printf("};\n\n");

	// Instruction mask bits
	printf("const uint64_t riscv_inst_mask[] = {\n");
	print_array_unknown_uint64(0ULL, no_comment);
	for (auto &opcode : opcodes) {
		printf("\t%s0x%016" PRIx64 ",\n",
			opcode_comment(opcode, no_comment).c_str(), opcode->mask);
	}
	printf("};\n\n");

	// RVC compression table (per isa width)
	for (auto isa_width : isa_width_prefixes()) {
		printf("const riscv_comp_data* riscv_inst_comp_%s[] = {\n", isa_width.second.c_str());
		print_array_unknown_enum("nullptr", no_comment);
		std::string isa_prefix = "rvcd_" + isa_width.second + "_";
		for (auto &opcode : opcodes) {
			std::string opcode_key = opcode_format("", opcode, ".");
			std::string rvcd_name = opcode_format(isa_prefix, opcode, "_");
			bool include_isa = rvcd_set.find(rvcd_name) != rvcd_set.end();
			printf("\t%s%s,\n",
				opcode_comment(opcode, no_comment).c_str(),
				include_isa && opcode->compressions.size() > 0 ? rvcd_name.c_str() : "nullptr");
		}
		printf("};\n\n");
	}

	// RVC decompression table
	for (auto isa_width : isa_width_prefixes()) {
		printf("const int riscv_inst_decomp_%s[] = {\n", isa_width.second.c_str());
		if (zero_not_oh) print_array_unknown_enum("0", no_comment);
		else print_array_unknown_enum("riscv_op_unknown", no_comment);
		for (auto &opcode : opcodes) {
			bool include_isa = false;
			for (auto &ext : opcode->extensions) {
				if (ext->isa_width == isa_width.first) include_isa = true;
			}
			std::string opcode_key = opcode_format("", opcode, ".");
			printf("\t%s%s,\n",
				opcode_comment(opcode, no_comment).c_str(),
				include_isa && opcode->compressed ?
					(zero_not_oh ?
						format_string("%lu", opcode->compressed->decomp_opcode->num).c_str() :
						opcode_format("riscv_op_", opcode->compressed->decomp_opcode, "_").c_str()) :
					(zero_not_oh ?
						"0" :
						"riscv_op_unknown"));
		}
		printf("};\n\n");
	}
}

void riscv_parse_meta::print_strings_h(bool no_comment, bool zero_not_oh)
{
	print_c_header("riscv-strings.h");
	printf("#ifndef riscv_strings_h\n");
	printf("#define riscv_strings_h\n");
	printf("\n");
	printf("#ifdef __cplusplus\n");
	printf("extern \"C\" {\n");
	printf("#endif\n");
	printf("\n");
	printf("extern const char* riscv_ireg_name_sym[];\n");
	printf("extern const char* riscv_freg_name_sym[];\n");
	printf("extern const char* riscv_inst_name_sym[];\n");
	printf("extern const char* riscv_arg_name_sym[];\n");
	printf("extern const char* riscv_arg_type_sym[];\n");
	printf("extern const char* riscv_csr_name_sym[];\n");
	printf("\n");
	printf("#ifdef __cplusplus\n");
	printf("}\n");
	printf("#endif\n");
	printf("\n");
	printf("#endif\n");
}

void riscv_parse_meta::print_strings_cc(bool no_comment, bool zero_not_oh)
{
	print_c_header("riscv-strings.cc");

	printf("#include \"riscv-strings.h\"\n");
	printf("\n");

	// Integer register names
	printf("const char* riscv_ireg_name_sym[] = {\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Floating Point register names
	printf("const char* riscv_freg_name_sym[] = {\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Instruction names
	printf("const char* riscv_inst_name_sym[] = {\n");
	print_array_unknown_str("unknown", no_comment);
	for (auto &opcode : opcodes) {
		std::string opcode_name = opcode_format("", opcode, ".", false);
		printf("\t%s\"%s\",\n",
			opcode_comment(opcode, no_comment).c_str(),
			opcode_name.c_str());
	}
	printf("};\n\n");

	// Instruction argument names
	printf("const char* riscv_arg_name_sym[] = {\n");
	printf("\t\"none\",\n");
	for (auto &arg : args) {
		printf("\t\"%s\",\n", arg->name.c_str());
	}
	printf("};\n\n");

	// Instruction argument type names
	std::vector<std::string> arg_types;
	for (auto &arg : args) {
		auto type_name = format_type(arg);
		if (std::find(arg_types.begin(), arg_types.end(), type_name) == arg_types.end()) {
			arg_types.push_back(type_name);
		}
	}
	printf("const char* riscv_arg_type_sym[] = {\n");
	printf("\t\"none\",\n");
	for (auto &arg_type : arg_types) {
		printf("\t\"%s\",\n", arg_type.c_str());
	}
	printf("};\n\n");

	// CSR names
	std::map<int,riscv_csr_ptr> csr_map;
	for (auto &csr : csrs) {
		int csr_num = riscv_parse_value(csr->number.c_str());
		csr_map[csr_num] = csr;
	}
	printf("const char* riscv_csr_name_sym[] = {\n");
	for (int i = 0; i < 4096; i++) {
		auto ci = csr_map.find(i);
		printf("\t%s%s%s,\n",
			(ci != csr_map.end()) ? "\"" : "",
			(ci != csr_map.end()) ? ci->second->name.c_str() : "nullptr",
			(ci != csr_map.end()) ? "\"" : ""
		);
	}
	printf("};\n\n");
}

void riscv_parse_meta::print_switch_h(bool no_comment, bool zero_not_oh)
{
	print_c_header("riscv-codec-switch.h");
	printf("#ifndef riscv_codec_switch_h\n");
	printf("#define riscv_codec_switch_h\n");
	printf("\n");

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
	printf("/* Decode Instruction Opcode */\n\n");
	printf("template <");
	for (auto mi = mnems.begin(); mi != mnems.end(); mi++) {
		if (mi != mnems.begin()) printf(", ");
		printf("bool %s", mi->c_str());
	}
	printf(">\n");
	printf("inline uint64_t riscv_decode_inst_op(uint64_t inst)\n");
	printf("{\n");
	printf("\tuint64_t op = riscv_op_unknown;\n");
	print_switch_decoder_node(root_node, 1);
	printf("\treturn op;\n");
	printf("}\n\n");

	// print type decoder
	printf("/* Decode Instruction Type */\n\n");
	printf("template <typename T>\n");
	printf("inline void riscv_decode_inst_type(T &dec, uint64_t inst)\n");
	printf("{\n");
	printf("\tdec.codec = riscv_inst_codec[dec.op];\n");
	printf("\tswitch (dec.codec) {\n");
	for (auto &codec : get_unique_codecs()) {
		printf("\t\tcase %-26s %-50s break;\n",
			format_string("riscv_codec_%s:", codec.c_str()).c_str(),
			format_string("riscv::decode_%s(dec, inst);", codec.c_str()).c_str());
	}
	printf("\t};\n");
	printf("}\n\n");

	// print encoder
	printf("/* Encode Instruction */\n\n");
	printf("template <typename T>\n");
	printf("inline uint64_t riscv_encode_inst(T &dec)\n");
	printf("{\n");
	printf("\tdec.codec = riscv_inst_codec[dec.op];\n");
	printf("\tuint64_t inst = riscv_inst_match[dec.op];\n");
	printf("\tswitch (dec.codec) {\n");
	for (auto &codec : get_unique_codecs()) {
		printf("\t\tcase %-26s %-50s break;\n",
			format_string("riscv_codec_%s:", codec.c_str()).c_str(),
			format_string("return inst |= riscv::encode_%s(dec);", codec.c_str()).c_str());
	}
	printf("\t};\n");
	printf("\treturn inst;\n");
	printf("}\n");
	printf("\n");
	printf("#endif\n");
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
					printf("\t\t%sif (%s && rv%lu) op = %s;\n",
						oi != opcode_list.begin() ? "else " : "",
						opcode_isa_shortname(opcode).c_str(), opcode->extensions.front()->isa_width,
						opcode_format("riscv_op_", opcode, "_").c_str());
				}
				for (size_t i = 0; i < indent; i++) printf("\t");
				printf("\t\tbreak;\n");
			}
			else
			{
				// if ambiguous, chooses first opcode
				if (opcode_widths.size() == 1) {
					printf("if (%s && rv%lu) op = %s; break;",
						opcode_isa_shortname(opcode).c_str(), opcode->extensions.front()->isa_width,
						opcode_format("riscv_op_", opcode, "_").c_str());
				} else {
					printf("if (%s) op = %s; break;",
						opcode_isa_shortname(opcode).c_str(),
						opcode_format("riscv_op_", opcode, "_").c_str());
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
	bool print_map_instructions = false;
	bool no_comment = false;
	bool zero_not_oh = false;
	bool remove_question_marks = false;
	bool include_pseudo = false;
	bool print_args_h = false;
	bool print_switch_h = false;
	bool print_jit_h = false;
	bool print_jit_cc = false;
	bool print_meta_h = false;
	bool print_meta_cc = false;
	bool print_strings_h = false;
	bool print_strings_cc = false;
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
		{ "-?", "--substitute-question-marks", cmdline_arg_type_none,
			"Substitute question marks for zeros in LaTeX output",
			[&](std::string s) { return (remove_question_marks = true); } },
		{ "-m", "--print-map", cmdline_arg_type_none,
			"Print map",
			[&](std::string s) { return (print_map = true); } },
		{ "-x", "--print-map-instructions", cmdline_arg_type_none,
			"Print map with instructions",
			[&](std::string s) { return (print_map = print_map_instructions = true); } },
		{ "-N", "--no-comment", cmdline_arg_type_none,
			"Don't emit comments in generated source",
			[&](std::string s) { return (no_comment = true); } },
		{ "-0", "--numeric-constants", cmdline_arg_type_none,
			"Use numeric constants in generated source",
			[&](std::string s) { return (zero_not_oh = true); } },
		{ "-p", "--include-pseudo", cmdline_arg_type_none,
			"Include pseudo opcodes in switch decoder",
			[&](std::string s) { return (include_pseudo = true); } },
		{ "-H", "--print-meta-h", cmdline_arg_type_none,
			"Print metadata header",
			[&](std::string s) { return (print_meta_h = true); } },
		{ "-C", "--print-meta-cc", cmdline_arg_type_none,
			"Print metadata source",
			[&](std::string s) { return (print_meta_cc = true); } },
		{ "-SH", "--print-strings-h", cmdline_arg_type_none,
			"Print strings header",
			[&](std::string s) { return (print_strings_h = true); } },
		{ "-SC", "--print-strings-cc", cmdline_arg_type_none,
			"Print strings source",
			[&](std::string s) { return (print_strings_cc = true); } },
		{ "-A", "--print-args-h", cmdline_arg_type_none,
			"Print args header",
			[&](std::string s) { return (print_args_h = true); } },
		{ "-S", "--print-switch-h", cmdline_arg_type_none,
			"Print switch header",
			[&](std::string s) { return (print_switch_h = true); } },
		{ "-J", "--print-jit-h", cmdline_arg_type_none,
			"Print jit header",
			[&](std::string s) { return (print_jit_h = true); } },
		{ "-K", "--print-jit-cc", cmdline_arg_type_none,
			"Print jit source",
			[&](std::string s) { return (print_jit_cc = true); } },
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
		!print_switch_h && !print_args_h &&
		!print_jit_h && !print_jit_cc &&
		!print_meta_h && !print_meta_cc &&
		!print_strings_h && !print_strings_cc))
	{
		printf("usage: %s [<options>]\n", argv[0]);
		cmdline_option::print_options(options);
		return false;
	}

	inst_set.ext_subset = inst_set.decode_isa_extensions(isa_spec);

	inst_set.generate_map();

	if (print_latex) {
		inst_set.print_latex(remove_question_marks);
	}

	if (print_map) {
		inst_set.print_map(print_map_instructions);
	}

	if (print_meta_h) {
		inst_set.print_meta_h(no_comment, zero_not_oh);
	}

	if (print_meta_cc) {
		inst_set.print_meta_cc(no_comment, zero_not_oh);
	}

	if (print_strings_h) {
		inst_set.print_strings_h(no_comment, zero_not_oh);
	}

	if (print_strings_cc) {
		inst_set.print_strings_cc(no_comment, zero_not_oh);
	}

	if (print_args_h) {
		inst_set.print_args_h();
	}

	if (print_switch_h) {
		inst_set.generate_codec(include_pseudo);
		inst_set.print_switch_h(no_comment, zero_not_oh);
	}

	if (print_jit_h) {
		inst_set.print_jit_h();
	}

	if (print_jit_cc) {
		inst_set.print_jit_cc();
	}

	exit(0);
}
