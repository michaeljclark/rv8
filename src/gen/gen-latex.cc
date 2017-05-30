//
//  gen-latex.cc
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

std::vector<cmdline_option> rv_gen_latex::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-?", "--substitute-question-marks", cmdline_arg_type_none,
			"Substitute question marks for zeros in LaTeX output",
			[&](std::string s) { return gen->set_option("remove_question_marks"); } },
		{ "-l", "--print-latex", cmdline_arg_type_none,
			"Print LaTeX instruction listing",
			[&](std::string s) { return gen->set_option("print_latex"); } },
	};
}

static const ssize_t kLatexTableColumns = 32;
static const ssize_t kLatexTableRows = 43;
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


static std::string latex_utf_substitute(std::string str)
{
	// TODO - replace this with a state machine parser/matcher that reads meta/notation
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

static void print_latex_row(rv_gen *gen, rv_latex_row &row, std::string ts, bool remove_question_marks)
{
	switch (row.row_type) {
		case rv_latex_type_empty:
			printf("\\tabularnewline\n");
			break;
		case rv_latex_type_line:
			printf("\\cline{1-%ld}\n", kLatexTableColumns);
			break;
		case rv_latex_type_page_break:
			printf("%s", kLatexTableEnd);
			printf("%s%s", kLatexTableBegin, ts.c_str());
			break;
		case rv_latex_type_extension_heading:
		case rv_latex_type_extension_contd:
		{
			printf("\\multicolumn{%ld}{c}{\\bf %s %s} & \\\\\n\\cline{1-%ld}\n",
				kLatexTableColumns,
				row.extension->description.c_str(),
				row.row_type == rv_latex_type_extension_contd ? " contd" : "",
				kLatexTableColumns);
			break;
		}
		case rv_latex_type_opcode:
		{
			auto &opcode = row.opcode;
			ssize_t bit_width = opcode->extensions[0]->inst_width;

			// calculate the column spans for this row
			rv_operand_ptr operand, loperand;
			bool lbound = false;
			typedef std::tuple<rv_opcode_ptr,rv_operand_ptr,ssize_t,std::string> operand_tuple;
			std::vector<operand_tuple> operand_parts;
			for (ssize_t bit = bit_width-1; bit >= 0; bit--) {
				char c = ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : '?');
				operand = opcode->find_operand(bit);

				// figure out where to break columns
				if (operand_parts.size() == 0 || std::get<1>(operand_parts.back()) != operand || lbound)
				{
					std::string str;
					str += c;
					operand_parts.push_back(operand_tuple(opcode, operand, 1, str));
				} else {
					auto &sz = std::get<2>(operand_parts.back());
					auto &str = std::get<3>(operand_parts.back());
					char lastc = str[str.length()-1];
					if ((lastc == '?' && (c == '1' || c == '0')) ||
						((lastc == '1' || lastc == '0') && c == '?'))
					{
						std::string str;
						str += c;
						operand_parts.push_back(operand_tuple(opcode, operand, 1, str));
					} else {
						sz++;
						str += c;
					}
				}

				// find the type of this opcode for column boundaries
				auto named_bitspec = opcode->find_named_bitspec(bit);
				if (named_bitspec.first.segments.size() == 0) {
					auto default_type = gen->types_by_name[std::to_string(bit_width)];
					if (!default_type) panic("can't find default type: %d", bit_width);
					named_bitspec = default_type->find_named_bitspec(bit);
				}
				lbound = (bit != 0 && bit == named_bitspec.first.segments.back().first.lsb);

				loperand = operand;
			}

			// update labels for segments with operands
			ssize_t msb = bit_width-1;
			for (size_t i = 0; i < operand_parts.size(); i++) {
				auto &operand = std::get<1>(operand_parts[i]);
				auto size = std::get<2>(operand_parts[i]);
				auto &str = std::get<3>(operand_parts[i]);
				if (operand) {
					str = operand->label;
					if (operand->type == "simm" || operand->type == "uimm" || operand->type == "offset") {
						auto spec = operand->bitspec;
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
							std::string operand = split(constraint->name, "_")[0];
							if (str.find(operand) != str.length() - operand.length()) continue;
							std::string render_hint = replace(constraint->hint, operand, "");
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
			std::string ls;
			for (size_t i = 0; i < operand_parts.size(); i++) {
				auto size = std::get<2>(operand_parts[i]);
				auto &str = std::get<3>(operand_parts[i]);
				if (i != 0) ls.append(" & ");
				ls.append("\\multicolumn{");
				ls.append(std::to_string(size * kLatexTableColumns / bit_width));
				ls.append("}{");
				if (i == 0) ls.append("|");
				ls.append("c|}{");
				ls.append(str);
				ls.append("}");
			}

			// format the opcode name and operands
			auto name = opcode->name;
			auto operand_comps = split(opcode->format->operands, ",");
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
			if (operand_comps.size() == 1 && operand_comps[0] == "none") operand_comps[0] = "";

			// print this row
			printf("%s & \\scriptsize{%s %s} \\\\\n\\cline{1-%ld}\n",
				ls.c_str(), name.c_str(), join(operand_comps, ", ").c_str(), kLatexTableColumns);
			break;
		}
		case rv_latex_type_type_spec:
		{
			// construct the LaTeX for this type row
			std::string ls;
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

				if (i != 0) ls.append(" & ");
				ls.append("\\multicolumn{");
				ls.append(std::to_string(size * kLatexTableColumns / bit_width));
				ls.append("}{");
				if (i == 0) ls.append("|");
				ls.append("c|}{");
				ls.append(str);
				ls.append("}");
			}

			// format the type name
			std::string name = type->name;
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
			name = "Type-" + name;

			// print this row
			printf("%s & \\scriptsize{\\bf %s} \\\\\n\\cline{1-%ld}\n",
				ls.c_str(), name.c_str(), kLatexTableColumns);
			break;
		}
		case rv_latex_type_type_bitrange:
		{
			// construct the LaTeX for this type bit range header
			std::string ls;
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
					ls.append("\\multicolumn{");
					ls.append(std::to_string(tsize));
					ls.append("}{c}{\\scriptsize{");
					ls.append(std::to_string(msb));
					ls.append("}} & ");
				} else {
					auto lsize = size >> 1;
					auto rsize = tsize - lsize;
					ls.append("\\multicolumn{");
					ls.append(std::to_string(lsize));
					ls.append("}{l}{\\scriptsize{");
					ls.append(std::to_string(msb));
					ls.append("}} & \\multicolumn{");
					ls.append(std::to_string(rsize));
					ls.append("}{r}{\\scriptsize{");
					ls.append(std::to_string(lsb));
					ls.append("}} & ");
				}
			}
			// print this row
			printf("%s \\\\\n\\cline{1-%ld}\n",
				ls.c_str(), kLatexTableColumns);
			break;
		}
	}
}

static void print_latex(rv_gen *gen)
{
	bool remove_question_marks = gen->has_option("remove_question_marks");

	// paginate opcodes ordered by extension
	// adding type and extension headings, page breaks and continuations
	size_t line = 0;
	ssize_t inst_width = 0;
	std::vector<rv_latex_page> pages;
	for (auto &ext : gen->extensions) {
		// check if this extension is in the selected subset
		if (ext->opcodes.size() == 0 || (gen->ext_subset.size() > 0 &&
			std::find(gen->ext_subset.begin(), gen->ext_subset.end(), ext) == gen->ext_subset.end())) {
			continue;
		}

		// check we have opcodes in this section (excluding pseudo opcodes)
		size_t count = 0;
		for (auto opcode : ext->opcodes) {
			count += opcode->is_pseudo() ? 0 : 1;
		}
		if (count == 0) continue;

		// add new page
		if (pages.size() == 0) pages.push_back(rv_latex_page());

		// break to a new page if the instruction width changes
		if (inst_width != 0 && inst_width != ext->inst_width) {
			pages.back().rows.push_back(rv_latex_row(rv_latex_type_page_break));
			pages.push_back(rv_latex_page());
			line = 0;
		}

		// don't start new extension heading unless we have reserved space
		if (line % kLatexTableRows > kLatexTableRows - kLatexReserveRows) {
			pages.back().rows.push_back(rv_latex_row(rv_latex_type_page_break));
			pages.push_back(rv_latex_page());
			line = 0;
		}

		// add extension heading
		pages.back().rows.push_back(rv_latex_row(rv_latex_type_empty));
		pages.back().rows.push_back(rv_latex_row(rv_latex_type_extension_heading, ext));
		line++;

		// add opcodes
		for (auto &opcode : ext->opcodes) {
			// skip psuedo opcode
			if (opcode->is_pseudo()) continue;
			// add a line to the top of the page if there is a continuation
			if (line % kLatexTableRows == 0) {
				pages.back().rows.push_back(rv_latex_row(rv_latex_type_empty));
				pages.back().rows.push_back(rv_latex_row(rv_latex_type_extension_contd, ext));
				pages.back().rows.push_back(rv_latex_row(rv_latex_type_line));
			}
			pages.back().rows.push_back(rv_latex_row(rv_latex_type_opcode, opcode));
			line++;
			// add page break
			if (line % kLatexTableRows == 0) {
				pages.back().rows.push_back(rv_latex_row(rv_latex_type_page_break));
				pages.push_back(rv_latex_page());
			}
		}

		// add empty line
		pages.back().rows.push_back(rv_latex_row(rv_latex_type_empty));
		line++;

		inst_width = ext->inst_width;
	}

	// iterate through the table rows and add types to 
	for (auto &page : pages) {
		std::vector<rv_type_ptr> types;
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
		auto default_type = gen->types_by_name[std::to_string(bit_width)];
		if (!default_type) panic("can't find default type: %d", bit_width);

		// insert type header rows at top of the page
		for (auto ti = types.rbegin(); ti != types.rend(); ti++) {
			page.rows.insert(page.rows.begin(), rv_latex_row(rv_latex_type_type_spec, *ti));
		}
		page.rows.insert(page.rows.begin(), rv_latex_row(rv_latex_type_type_bitrange, default_type));
	}

	// create the table width specification
	std::string ts;
	ts.append("{");
	for (ssize_t i = 0 ; i < kLatexTableColumns; i++) ts.append("p{0.05mm}");
	ts.append("p{50mm}l}");
	for (ssize_t i = 0 ; i < kLatexTableColumns; i++) ts.append("& ");
	ts.append("& \\\\\n");

	// print document header
	printf("%s", kLatexDocumentBegin);
	printf("%s%s", kLatexTableBegin, ts.c_str());

	// iterate through pages and rows and printing them
	for (auto &page : pages) {
		for (auto &row : page.rows) {
			print_latex_row(gen, row, ts, remove_question_marks);
		}
	}

	// print document trailer
	printf("%s", kLatexTableEnd);
	printf("%s", kLatexDocumentEnd);
}

void rv_gen_latex::generate()
{
	if (gen->has_option("print_latex")) print_latex(gen);
}
