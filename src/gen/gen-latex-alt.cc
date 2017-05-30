//
//  gen-latex-alt.cc
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

std::vector<cmdline_option> rv_gen_latex_alt::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-la", "--print-latex-alt", cmdline_arg_type_none,
			"Print alternative LaTeX instruction listing",
			[&](std::string s) { return gen->set_option("print_latex_alt"); } },
	};
}

static const char* kLatexDocumentBegin =
R"LaTeX(\documentclass{report}
\usepackage[letterpaper, portrait, margin=0.5in]{geometry}
\usepackage[utf8]{inputenc}
\usepackage{tikz}
\usetikzlibrary{graphs} 
\begin{document}
)LaTeX";

static const char* kLatexDocumentEnd =
R"LaTeX(
\end{document}
)LaTeX";

static const char* kLatexPictureBegin =
R"LaTeX(
\begin{tikzpicture}[fill=blue!20,scale=0.5]
\begin{scope}[every node/.style={font=\scriptsize}]
)LaTeX";

static const char* kLatexPictureEnd =
R"LaTeX(
\end{scope}
\end{tikzpicture}
)LaTeX";

enum rv_latex_type {
	rv_latex_type_empty,
	rv_latex_type_line,
	rv_latex_type_page_break,
	rv_latex_type_extension_heading,
	rv_latex_type_extension_contd,
	rv_latex_type_opcode,
	rv_latex_type_type_bitrange,
	rv_latex_type_type_spec,
};

struct rv_latex_row
{
	rv_latex_type row_type;
	rv_extension_ptr extension;
	rv_opcode_ptr opcode;
	rv_type_ptr type;

	rv_latex_row(rv_latex_type row_type)
		: row_type(row_type) {}

	rv_latex_row(rv_latex_type row_type, rv_extension_ptr extension)
		: row_type(row_type), extension(extension) {}

	rv_latex_row(rv_latex_type row_type, rv_opcode_ptr opcode)
		: row_type(row_type), opcode(opcode) {}

	rv_latex_row(rv_latex_type row_type, rv_type_ptr type)
		: row_type(row_type), type(type) {}
};

struct rv_latex_page
{
	std::vector<rv_latex_row> rows;
};

static void print_latex_alt_type(rv_gen *gen, rv_type_ptr type)
{
	typedef std::vector<rv_bitspec> rv_bitspec_list;

	printf("%s", kLatexPictureBegin);

	// find bit ranges for each named decode component
	std::map<std::string,rv_bitspec_list> decode_list;
	std::vector<std::string> decode_names;

	for (auto &part : type->parts) {
		rv_bitspec &bitspec = part.first;
		std::string &name = part.second;
		auto bi = decode_list.find(name);
		if (bi == decode_list.end()) {
			bi = decode_list.insert(decode_list.end(),
				std::pair<std::string,rv_bitspec_list>(name, rv_bitspec_list()));
			decode_names.push_back(name);
		}
		bi->second.push_back(bitspec);
	}

	// assign shades
	std::map<std::string,int> decode_shade;
	int shade = (decode_list.size() -1) * 10;
	for (auto &name : decode_names) {
		decode_shade[name] = shade;
		shade -= 10;
	}

	// draw bit segment fill
	for (auto &part : type->parts) {
		rv_bitspec &bitspec = part.first;
		std::string &name = part.second;
		int msb = bitspec.segments.front().first.msb;
		int lsb = bitspec.segments.back().first.lsb;
		int shade = decode_shade[name];
		if (shade == 0) continue;
		printf("\\fill [black!%d] (%d,-1)  rectangle (%d,-2);\n", shade, 32 - lsb, (32 - msb) - 1);
	}
	printf("\n");

	// draw bit segment borders
	for (auto &part : type->parts) {
		rv_bitspec &bitspec = part.first;
		int msb = bitspec.segments.front().first.msb;
		int lsb = bitspec.segments.back().first.lsb;
		printf("\\draw (%d,-1) rectangle (%d,-2);\n", 32 - lsb, (32 - msb) - 1);
	}
	printf("\n");

	// draw bit segment bit position labels
	for (auto &part : type->parts) {
		rv_bitspec &bitspec = part.first;
		int msb = bitspec.segments.front().first.msb;
		int lsb = bitspec.segments.back().first.lsb;
		if (msb == lsb) {
			printf("\\node (inst_%d)    at (%f ,-1.5) {$%d$};\n", msb, (32 - msb) - 0.5, msb);
		} else {
			printf("\\node (inst_%d)    at (%f ,-1.5) {$%d$};\n", msb, (32 - msb) - 0.5, msb);
			printf("\\node (inst_%d)    at (%f ,-1.5) {$%d$};\n", lsb, (32 - lsb) - 0.5, lsb);
		}
	}
	printf("\n");

	// draw bit segment text labels
	for (auto &part : type->parts) {
		rv_bitspec &bitspec = part.first;
		std::string &name = part.second;

		std::string label = name;
		size_t decode_bits = 0;
		for (auto &segment : bitspec.segments) {
			decode_bits += segment.second.size();
		}
		if (decode_bits > 0) {
			label = label + bitspec.to_string();
		}

		int msb = bitspec.segments.front().first.msb;
		int lsb = bitspec.segments.back().first.lsb;
		float center = lsb + (msb - lsb) / 2;
		printf("\\node (%s) at (%f,-0.5) {$%s$};\n", name.c_str(), (32 - center) - 0.5, label.c_str());
	}
	printf("\n");

	printf("%s", kLatexPictureEnd);
}

static void print_latex_alt(rv_gen *gen)
{
	// print document header
	printf("%s", kLatexDocumentBegin);

	for (auto &type : gen->types) {

		// skip type categories (type name is none or starts with digits)
		if (std::isdigit(type->name[0]) || type->name == "none") continue;

		// format the type name
		std::string type_name = type->name;
		std::transform(type_name.begin(), type_name.end(), type_name.begin(), ::toupper);
		type_name = "Type-" + type_name;
		printf("\n\\bf{%s}\n", type_name.c_str());

		// output the type diagram
		print_latex_alt_type(gen, type);
	}

	// print document trailer
	printf("%s", kLatexDocumentEnd);
}

void rv_gen_latex_alt::generate()
{
	if (gen->has_option("print_latex_alt")) print_latex_alt(gen);
}
