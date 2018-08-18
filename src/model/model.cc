//
//  model.cc
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

#include "util.h"
#include "model.h"

static const char* OPERANDS_FILE              = "operands";
static const char* ENUMS_FILE                 = "enums";
static const char* TYPES_FILE                 = "types";
static const char* FORMATS_FILE               = "formats";
static const char* CODECS_FILE                = "codecs";
static const char* EXTENSIONS_FILE            = "extensions";
static const char* REGISTERS_FILE             = "registers";
static const char* CSRS_FILE                  = "csrs";
static const char* CSR_FIELDS_FILE            = "csr-fields";
static const char* OPCODES_FILE               = "opcodes";
static const char* CONSTRAINTS_FILE           = "constraints";
static const char* COMPRESSION_FILE           = "compression";
static const char* PSEUDO_FILE                = "pseudos";
static const char* OPCODE_MAJORS_FILE         = "opcode-majors";
static const char* OPCODE_CLASSES_FILE        = "opcode-classes";
static const char* OPCODE_FULLNAMES_FILE      = "opcode-fullnames";
static const char* OPCODE_DESCRIPTIONS_FILE   = "opcode-descriptions";
static const char* OPCODE_PSEUDOCODE_C_FILE   = "opcode-pseudocode-c";
static const char* OPCODE_PSEUDOCODE_ALT_FILE = "opcode-pseudocode-alt";

const rv_primitive_type rv_primitive_type_table[] = {
	{ rvs_ext, rvt_sx,   "x",      "sx",   "r", "%ld",   "%lx",   "l",     "signed long" },       /* LP64 and ILP32, not LLP64 */
	{ rvs_ext, rvt_ux,   "xu",     "ux",   "r", "%lu",   "%lx",   "ul",    "unsigned long" },     /* LP64 and ILP32, not LLP64 */
	{ rvs_std, rvt_s8,   "b",      "s8",   "r", "%hhd",  "%hhx",  "",      "signed char" },
	{ rvs_std, rvt_u8,   "bu",     "u8",   "r", "%hhu",  "%hhx",  "",      "unsigned char" },
	{ rvs_std, rvt_s16,  "h",      "s16",  "r", "%hd",   "%hx",   "",      "signed short" },
	{ rvs_std, rvt_u16,  "hu",     "u16",  "r", "%hu",   "%hx",   "",      "unsigned short" },
	{ rvs_std, rvt_s32,  "w",      "s32",  "r", "%d",    "%x",    "",      "signed int" },
	{ rvs_std, rvt_u32,  "wu",     "u32",  "r", "%u",    "%x",    "u",     "unsigned int" },
	{ rvs_std, rvt_s64,  "l",      "s64",  "r", "%lld",  "%llx",  "ll",    "signed long long" },
	{ rvs_std, rvt_u64,  "lu",     "u64",  "r", "%llu",  "%llx",  "ull",   "unsigned long long" },
	{ rvs_ext, rvt_s128, "c", /*?*/"s128", "r", nullptr, nullptr, nullptr, "signed __int128" },   /* Clang/GCC type */
	{ rvs_ext, rvt_u128, "cu",/*?*/"u128", "r", nullptr, nullptr, nullptr, "unsigned __int128" }, /* Clang/GCC type */
	{ rvs_std, rvt_f32,  "s",      "f32",  "f", "%.9e",  "%.9e",  "f",     "float" },
	{ rvs_std, rvt_f64,  "d",      "f64",  "f", "%.17e", "%.17e", "",      "double" },
	{ rvs_ext, rvt_f128, "q",      "f128", "f", nullptr, nullptr, nullptr, "__float128" },        /* Clang/GCC type */
	{ rvs_ext, rvt_none, nullptr,  nullptr, nullptr, nullptr, nullptr }
};

const rv_primitive_type* rv_lookup_primitive_by_spec_type(std::string spec_type, rvt default_type)
{
	struct rv_primitive_spec_type_map : std::map<std::string,const rv_primitive_type*>
	{
		rv_primitive_spec_type_map() {
			for (const auto *ent = rv_primitive_type_table; ent->enum_type != rvt_none; ent++)
				(*this)[ent->spec_type] = ent;
		}
	};
	static rv_primitive_spec_type_map map;
	auto ti = map.find(spec_type);
	return (ti == map.end()) ? &rv_primitive_type_table[default_type] : ti->second;
}

const rv_primitive_type* rv_lookup_primitive_by_meta_type(std::string meta_type, rvt default_type)
{
	struct rv_primitive_meta_type_map : std::map<std::string,const rv_primitive_type*>
	{
		rv_primitive_meta_type_map() {
			for (const auto *ent = rv_primitive_type_table; ent->enum_type != rvt_none; ent++)
				(*this)[ent->meta_type] = ent;
		}
	};
	static rv_primitive_meta_type_map map;
	auto ti = map.find(meta_type);
	return (ti == map.end()) ? &rv_primitive_type_table[default_type] : ti->second;
}

template <typename T>
std::string join(std::vector<T> list, std::string sep)
{
	std::string s;
	for (auto i = list.begin(); i != list.end(); i++) {
		if (i != list.begin()) s.append(sep);
		s.append(*i);
	}
	return s;
}

int64_t rv_parse_value(const char* valstr)
{
	int64_t val;
	char *endptr = nullptr;
	if (strncmp(valstr, "0x", 2) == 0) {
		val = strtoull(valstr + 2, &endptr, 16);
	} else if (strncmp(valstr, "0b", 2) == 0) {
		val = strtoull(valstr + 2, &endptr, 2);
	} else {
		val = strtoull(valstr, &endptr, 10);
	}
	if (*endptr != '\0') {
		panic("rv_parse_value: invalid value: %s", valstr);
	}
	return val;
}

rv_bitrange::rv_bitrange(std::string bitrange)
{
	std::vector<std::string> comps = split(bitrange, ":");
	if (comps.size() < 1 || comps.size() > 2) {
		panic("invalid bitrange: %s", bitrange.c_str());
	}
	if (comps.size() == 2) {
		msb = strtoul(comps[0].c_str(), nullptr, 10);
		lsb = strtoul(comps[1].c_str(), nullptr, 10);
	} else {
		msb = lsb = strtoul(comps[0].c_str(), nullptr, 10);
	}
}

std::string rv_bitrange::to_string(std::string sep, bool collapse_single_bit_range)
{
	std::string s;
	s.append(std::to_string(msb));
	if (!collapse_single_bit_range || msb != lsb) {
		s.append(sep);
		s.append(std::to_string(lsb));
	}
	return s;
}

rv_bitspec::rv_bitspec(std::string bitspec)
{
	/*
	 * example bitrange specs in gather[scatter](,...) format
	 *
	 *     0
	 *     11:7
	 *     12[5],6:2[4:0]
	 *     31:25[12|10:5],11:7[4:1|11]
	 *
	 * when [scatter] is ommitted, bits are right justified from bit 0
	 */

	std::vector<std::string> comps = split(bitspec, ",");
	for (std::string comp : comps) {
		size_t bopen = comp.find("[");
		size_t bclose = comp.find("]");
		if (bopen != std::string::npos && bclose != std::string::npos) {
			rv_bitrange gather(comp.substr(0, bopen));
			std::string scatter_spec = comp.substr(bopen + 1, bclose - bopen - 1);
			rv_bitrange_list scatter;
			for (auto scatter_comp : split(scatter_spec, "|")) {
				scatter.push_back(rv_bitrange(scatter_comp));
			}
			segments.push_back(rv_bitseg(gather, scatter));
		} else {
			rv_bitrange gather(comp);
			rv_bitrange_list scatter;
			segments.push_back(rv_bitseg(gather, scatter));
		}
	}
}

bool rv_bitspec::matches_bit(ssize_t bit)
{
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (bit <= si->first.msb && bit >= si->first.lsb) return true;
	}
	return false;
}

size_t rv_bitspec::decoded_msb()
{
	ssize_t msb = 0;
	for (auto si = segments.begin(); si != segments.end(); si++) {
		for (auto di = si->second.begin(); di != si->second.end(); di++) {
			if (di->msb > msb) msb = di->msb;
		}
	}
	return msb;
}

std::string rv_bitspec::to_string()
{
	std::string s;
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (si != segments.begin()) s.append(",");
		s.append(si->first.to_string(":"));
		s.append("[");
		for (auto ti = si->second.begin(); ti != si->second.end(); ti++) {
			if (ti != si->second.begin()) s.append("|");
			s.append(ti->to_string(":"));
		}
		s.append("]");
	}
	return s;
}

std::string rv_bitspec::to_template()
{
	ssize_t msb = 0;
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (si->second.size() == 0) {
			msb = si->first.msb - si->first.lsb;
		} else {
			for (auto ti = si->second.begin(); ti != si->second.end(); ti++) {
				if (ti->msb > msb) msb = ti->msb;
			}
		}
	}
	std::string s;
	s.append("<");
	s.append(std::to_string(msb + 1));
	s.append(", ");
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (si != segments.begin()) s.append(", ");
		s.append("S<");
		s.append(si->first.to_string(",", false));
		s.append(", ");
		if (si->second.size() == 0) {
			s.append("B<");
			s.append(std::to_string(si->first.msb - si->first.lsb));
			s.append(",0>");
		} else {
			for (auto ti = si->second.begin(); ti != si->second.end(); ti++) {
				if (ti != si->second.begin()) s.append(",");
				s.append("B<");
				s.append(ti->to_string(","));
				s.append(">");
			}
		}
		s.append(">");
	}
	s.append(">");
	return s;
}

const ssize_t rv_meta_model::DEFAULT = std::numeric_limits<ssize_t>::max();

rv_opcode_mask rv_meta_model::decode_mask(std::string bit_spec)
{
	std::vector<std::string> spart = split(bit_spec, "=");
	if (spart.size() != 2) {
		panic("bit range %s must be in form n..m=v\n", bit_spec.c_str());
	}
	std::vector<std::string> rpart = split(spart[0], "..");
	ssize_t msb = 0, lsb = 0, val = 0;
	if (rpart.size() == 1) {
		msb = lsb = strtoul(rpart[0].c_str(), nullptr, 10);
	} else if (rpart.size() == 2) {
		msb = strtoul(rpart[0].c_str(), nullptr, 10);
		lsb = strtoul(rpart[1].c_str(), nullptr, 10);
	} else {
		panic("bit range %s must be in form n..m=v\n", bit_spec.c_str());
	}
	if (spart[1].find("0x") == 0) {
		val = strtoul(spart[1].c_str() + 2, nullptr, 16);
	} else {
		val = strtoul(spart[1].c_str(), nullptr, 10);
	}

	return rv_opcode_mask(rv_bitrange(msb, lsb), val);
}

rv_version rv_meta_model::decode_version(std::string version, rv_version default_version)
{
	std::vector<std::string> vpart = split(version, ".");
	if (vpart.size() == 1 && vpart[0].size() > 0) {
		return rv_version(strtoul(vpart[0].c_str(), nullptr, 10));
	} else if (vpart.size() == 2) {
		return rv_version(strtoul(vpart[0].c_str(), nullptr, 10),
			strtoul(vpart[1].c_str(), nullptr, 10), 0);
	} else if (vpart.size() == 3) {
		return rv_version(strtoul(vpart[0].c_str(), nullptr, 10),
			strtoul(vpart[1].c_str(), nullptr, 10),
			strtoul(vpart[2].c_str(), nullptr, 10));
	}
	return default_version;
}

rv_version_spec rv_meta_model::decode_version_spec(std::string version_spec)
{
	std::vector<std::string> spart = split(split(version_spec, ",")[0], "-");
	if (spart.size() == 1) {
		rv_version start_end = decode_version(spart[0], rv_version(0));
		return rv_version_spec(start_end, start_end);
	} else if (spart.size() >= 2) {
		return rv_version_spec(decode_version(spart[0], rv_version(0)),
			decode_version(spart[1], rv_version(std::numeric_limits<int>::max())));
	} else {
		return rv_version_spec(rv_version(0), rv_version(0));
	}
}

std::vector<rv_bitrange> rv_meta_model::bitmask_to_bitrange(std::vector<ssize_t> &bits)
{
	std::vector<rv_bitrange> v;
	if (bits.size() > 0) {
		v.push_back(rv_bitrange(bits[0], bits[0]));
		for (size_t i = 1; i < bits.size(); i++) {
			if (bits[i] + 1 == v.back().lsb) {
				v.back().lsb = bits[i];
			} else {
				v.push_back(rv_bitrange(bits[i], bits[i]));
			}
		}
	}
	return v;
}

std::string rv_meta_model::format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment)
{
	std::vector<rv_bitrange> v = bitmask_to_bitrange(bits);
	std::string s;

	ssize_t total_length = bits.size();
	ssize_t range_start = bits.size();

	for (auto ri = v.begin(); ri != v.end(); ri++) {
		rv_bitrange r = *ri;
		ssize_t range_end = range_start - (r.msb - r.lsb);
		ssize_t shift = r.msb - range_start + 1;
		if (ri != v.begin()) s.append(" | ");
		s.append("((");
		s.append(var);
		s.append(" >> ");
		s.append(std::to_string(shift));
		s.append(") & 0b");
		for (ssize_t i = total_length; i > 0; i--) {
			if (i <= range_start && i >= range_end) s.append("1");
			else s.append("0");
		}
		s.append(")");
		range_start -= (r.msb - r.lsb) + 1;
	}

	if (comment) {
		s.append(" /* ");
		s.append(var);
		s.append("[");
		for (auto ri = v.begin(); ri != v.end(); ri++) {
			rv_bitrange r = *ri;
			if (ri != v.begin()) s.append("|");
			if (r.msb == r.lsb) s.append(std::to_string(r.msb));
			else {
				s.append(std::to_string(r.msb));
				s.append(":");
				s.append(std::to_string(r.lsb));
			}
		}
		s.append("] */");
	}

	return s;
}

std::string rv_meta_model::format_type(rv_operand_ptr operand)
{
	return operand->type + std::to_string((operand->type == "offset" || operand->type == "simm" || operand->type == "uimm") ?
		operand->bitspec.decoded_msb() + 1 :
		operand->bitspec.segments.front().first.msb - operand->bitspec.segments.front().first.lsb + 1);
}

std::string rv_meta_model::format_codec(std::string prefix, rv_codec_ptr codec, std::string dot, bool strip_suffix)
{
	std::string name = strip_suffix ? split(codec->name, "+")[0] : codec->name;
	name = replace(name, "·", dot);
	name = replace(name, "+", dot);
	return prefix + name;
}

std::string rv_meta_model::format_format(std::string prefix, rv_format_ptr format, char special)
{
	std::string operands;
	for (auto i = format->operands.begin(); i != format->operands.end(); i++) {
		if (std::ispunct(*i)) {
			if (i != format->operands.begin() && i + 1 != format->operands.end() && !std::ispunct(*(i - 1))) {
				operands += special;
			}
		} else {
			operands += *i;
		}
	}
	return prefix + ltrim(rtrim(operands, std::ispunct), std::ispunct);
}

std::string rv_meta_model::opcode_format(std::string prefix, rv_opcode_ptr opcode, std::string dot, bool suffix)
{
	std::string name = opcode->name;
	if (name.find("@") == 0) name = name.substr(1);
	if (suffix && opcode->suffix) {
		return prefix + replace(name, ".", dot) + dot + opcode->extensions.front()->name;
	} else {
		return prefix + replace(name, ".", dot);
	}
}

std::string rv_meta_model::opcode_codec_key(rv_opcode_ptr opcode)
{
	if (opcode->operands.size() == 0) {
		return "none";
	}
	std::vector<std::string> codec_operands;
	for (auto operand : opcode->operands) {
		codec_operands.push_back(operand->name);
	}
	return join(codec_operands, "·");
}

std::string rv_meta_model::opcode_comment(rv_opcode_ptr opcode, bool no_comment)
{
	std::string opcode_name = opcode_format("", opcode, ".");
	return no_comment ? "" : format_string("/* %20s */ ", opcode_name.c_str());
}

std::string rv_meta_model::opcode_isa_shortname(rv_opcode_ptr opcode)
{
	auto &ext = opcode->extensions.front();
	std::string short_name = ext->prefix;
	short_name += ext->alpha_code;
	return short_name;
}

std::string rv_meta_model::codec_type_name(rv_codec_ptr codec)
{
	size_t o = codec->name.find("·");
	if (o == std::string::npos) o = codec->name.find("+");
	if (o == std::string::npos) return codec->name;
	return codec->name.substr(0, o);
}

const rv_primitive_type* rv_meta_model::infer_operand_primitive(rv_opcode_ptr &opcode, rv_extension_ptr &ext, rv_operand_ptr &operand, size_t i)
{
	// infer operand primitive type using heuristics based on RISC-V opcode names
	// NOTE:- heuristic is designed for RISC-V FPU opcode naming convention
	std::string opcode_name = opcode->name;
	if (opcode_name.find("c.") == 0) opcode_name = replace(opcode_name, "c.", "c_");
	std::vector<std::string> opcode_parts = split(opcode_name, ".");
	const rv_primitive_type *primitive = &rv_primitive_type_table[rvt_sx];
	if (operand->type == "ireg") {
		if (i == 0 && opcode_parts.size() > 2) {
			primitive = rv_lookup_primitive_by_spec_type(opcode_parts[1], rvt_sx);
		} else if (i == 1 && opcode_parts.size() > 2) {
			primitive = rv_lookup_primitive_by_spec_type(opcode_parts[2], rvt_sx);
		}
	} else if (operand->type == "freg") {
		if (opcode_parts.size() == 2) {
			primitive = rv_lookup_primitive_by_spec_type(opcode_parts[1]);
		} else if (i == 0 && opcode_parts.size() > 2) {
			primitive = rv_lookup_primitive_by_spec_type(opcode_parts[1]);
		} else if (i == 1 && opcode_parts.size() > 2) {
			primitive = rv_lookup_primitive_by_spec_type(opcode_parts[2]);
		} else {
			if (ext->alpha_code == 's') {
				primitive = &rv_primitive_type_table[rvt_f32];
			} else if (ext->alpha_code == 'd') {
				primitive = &rv_primitive_type_table[rvt_f64];
			} else if (ext->alpha_code == 'q') {
				primitive = &rv_primitive_type_table[rvt_f128];
			} else /* if (ext->alpha_code == 'c') */ {
				primitive = &rv_primitive_type_table[rvt_f32];
			}
		}
	}
	return primitive;
}

std::vector<std::string> rv_meta_model::parse_line(std::string line)
{
	// simple parsing routine that handles tokens separated by whitespace,
	// double quoted tokens containing whitespace and # comments

	std::vector<char> token;
	std::vector<std::string> comps;
	enum {
		whitespace,
		quoted_token,
		unquoted_token,
		comment
	} state = whitespace;

	size_t i = 0;
	while (i < line.size()) {
		char c = line[i];
		switch (state) {
			case whitespace:
				if (::isspace(c)) {
					i++;
				} else if (c == '#') {
					state = comment;
				} else if (c == '"') {
					state = quoted_token;
					i++;
				} else {
					state = unquoted_token;
				}
				break;
			case quoted_token:
				if (c == '"') {
					comps.push_back(std::string(token.begin(), token.end()));
					token.resize(0);
					state = whitespace;
				} else {
					token.push_back(c);
				}
				i++;
				break;
			case unquoted_token:
				if (::isspace(c)) {
					comps.push_back(std::string(token.begin(), token.end()));
					token.resize(0);
					state = whitespace;
				} else {
					token.push_back(c);
				}
				i++;
				break;
			case comment:
				i++;
				break;
		}
	}
	if (token.size() > 0) {
		comps.push_back(std::string(token.begin(), token.end()));
	}
	return comps;
}

std::vector<std::vector<std::string>> rv_meta_model::read_file(std::string filename)
{
	char buf[256], *s;
	std::vector<std::vector<std::string>> data;
	FILE *file = fopen(filename.c_str(), "r");
	if (!file) {
		panic("error opening %s\n", filename.c_str());
	}
	while ((s = fgets(buf, sizeof(buf), file)))
	{
		std::string line = s;
		size_t hoffset = line.find("#");
		if (hoffset != std::string::npos) {
			line = ltrim(rtrim(line.substr(0, hoffset)));
		}
		std::vector<std::string> part = parse_line(line);
		if (part.size() == 0) continue;
		data.push_back(part);
	}
	fclose(file);
	return data;
}

std::vector<std::string> rv_meta_model::get_unique_codecs()
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

std::vector<std::string> rv_meta_model::get_inst_mnemonics(bool isa_widths, bool isa_extensions)
{
	std::vector<std::string> mnems;

	// create mnemonics for instruction set widths
	if (isa_widths) {
		for (auto &ext : extensions) {
			std::string mnem = ext->prefix + std::to_string(ext->isa_width);
			if (std::find(mnems.begin(), mnems.end(), mnem) == mnems.end())
				mnems.push_back(mnem);
		}
	}

	// create mnemonics for instruction set extensions
	if (isa_extensions) {
		for (auto &ext : extensions) {
			std::string mnem = ext->prefix + ext->alpha_code;
			if (std::find(mnems.begin(), mnems.end(), mnem) == mnems.end())
				mnems.push_back(mnem);
		}
	}

	return mnems;
}

std::vector<std::pair<size_t,std::string>> rv_meta_model::isa_width_prefixes()
{
	std::vector<std::pair<size_t,std::string>> widths;
	for (auto &extension : extensions) {
		auto width = std::pair<size_t,std::string>(extension->isa_width,
			extension->prefix + std::to_string(extension->isa_width));
		if (std::find(widths.begin(), widths.end(), width) == widths.end()) {
			widths.push_back(width);
		}
	}
	return widths;
}

rv_extension_list rv_meta_model::decode_isa_extensions(std::string isa_spec)
{
	rv_extension_list list;
	if (isa_spec.size() == 0) {
		return list;
	}

	// canonicalise isa spec to lower case
	std::transform(isa_spec.begin(), isa_spec.end(), isa_spec.begin(), ::tolower);

	// find isa prefix and width
	ssize_t ext_isa_width = 0;
	std::string ext_prefix, ext_isa_width_str;
	for (auto &ext : extensions) {
		if (isa_spec.find(ext->prefix) == 0) {
			ext_prefix = ext->prefix;
		}
		if (ext_prefix.size() > 0) {
			ext_isa_width_str = std::to_string(ext->isa_width);
			if (isa_spec.find(ext_isa_width_str) == ext_prefix.size()) {
				ext_isa_width = ext->isa_width;
				break;
			}
		}
	}
	if (ext_prefix.size() == 0 || ext_isa_width == 0) {
		panic("illegal isa spec: %s", isa_spec.c_str());
	}

	// replace 'g' with 'imafd'
	size_t g_offset = isa_spec.find("g");
	if (g_offset != std::string::npos) {
		isa_spec = isa_spec.replace(isa_spec.begin() + g_offset,
			isa_spec.begin() + g_offset + 1, "imafd");
	}

	// lookup extensions
	ssize_t ext_offset = ext_prefix.length() + ext_isa_width_str.length();
	for (auto i = isa_spec.begin() + ext_offset; i != isa_spec.end(); i++) {
		std::string ext_name = isa_spec.substr(0, ext_offset) + *i;
		auto ext = extensions_by_name[ext_name];
		if (!ext) {
			panic("illegal isa spec: %s: missing extension: %s",
				isa_spec.c_str(), ext_name.c_str());
		}
		if (std::find(list.begin(), list.end(), ext) != list.end()) {
			panic("illegal isa spec: %s: duplicate extension: %s",
				isa_spec.c_str(), ext_name.c_str());
		}
		list.push_back(ext);
	}
	return list;
}

rv_opcode_ptr rv_meta_model::create_opcode(std::string opcode_name, std::string extension)
{
	// add opcode to the opcode by name list, creating a new list if one doesn't exist

	// all_opcodes contains a list of all opcodes including opcodes
	// with the same name but different bidwidth encodings

	// opcodes contains the last opcode (widest encoding)

	auto opcode = std::make_shared<rv_opcode>(opcode_name);
	all_opcodes.push_back(opcode);
	auto opcode_list_i  = opcodes_by_name.find(opcode_name);
	if (opcode_list_i == opcodes_by_name.end()) {
		opcodes.push_back(opcode);
		opcodes_by_name[opcode_name] = { opcode };
		opcode->num = opcodes.size();
	} else {
		auto first = opcode_list_i->second.front();
		opcodes[first->num - 1] = opcode;
		opcode_list_i->second.push_back(opcode);
		opcode->num = first->num;
		opcode->suffix = first->suffix = true;
	}

	return opcode;
}

rv_opcode_list rv_meta_model::lookup_opcode_by_name(std::string opcode_name)
{
	auto i = opcodes_by_name.find(opcode_name);
	if (i != opcodes_by_name.end()) return i->second;
	return rv_opcode_list();
}

rv_opcode_list rv_meta_model::opcode_list_by_width(size_t isa_width)
{
	rv_opcode_list list;
	for (auto opcode : opcodes) {
		if (opcode->suffix) {
			bool found = false;
			for (auto find_opcode : lookup_opcode_by_name(opcode->name)) {
				if (find_opcode->extensions.front()->isa_width == isa_width) {
					list.push_back(find_opcode);
					found = true;
					break;
				}
			}
			if (!found) {
				list.push_back(opcode);
			}
		} else {
			list.push_back(opcode);
		}
	}
	return list;
}

bool rv_meta_model::is_operand(std::string mnem)
{
	return (operands_by_name.find(mnem) != operands_by_name.end());
}

bool rv_meta_model::is_ignore(std::string mnem)
{
	return (mnem.find("=ignore") != std::string::npos);
}

bool rv_meta_model::is_mask(std::string mnem)
{
	return (mnem.find("=") != std::string::npos);
}

bool rv_meta_model::is_codec(std::string mnem)
{
	return (codecs_by_name.find(mnem) != codecs_by_name.end());
}

bool rv_meta_model::is_extension(std::string mnem)
{
	return (extensions_by_name.find(mnem) != extensions_by_name.end());
}

void rv_meta_model::parse_operand(std::vector<std::string> &part)
{
	if (part.size() < 4) {
		panic("operands requires 4 parameters: %s", join(part, " ").c_str());
	}
	auto operand = operands_by_name[part[0]] = std::make_shared<rv_operand>(
		part[0], part[1], part[2], part[3]
	);
	operands.push_back(operand);
}

void rv_meta_model::parse_enum(std::vector<std::string> &part)
{
	if (part.size() < 5) {
		panic("enums requires 5 parameters: %s", join(part, " ").c_str());
	}
	auto enumv = enums_by_name[part[0]] = std::make_shared<rv_enum>(
		part[0], part[1], part[2], part[3], decode_version_spec(part[4])
	);
	enums.push_back(enumv);
}

void rv_meta_model::parse_type(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("types requires 2 or more parameters: %s", join(part, " ").c_str());
	}
	auto type = types_by_name[part[0]] = std::make_shared<rv_type>(
		part[0], part[1]
	);
	for (size_t i = 2; i < part.size(); i++) {
		std::vector<std::string> spec = split(part[i], "=");
		type->parts.push_back(rv_named_bitspec(rv_bitspec(spec[0]), spec.size() > 1 ? spec[1] : ""));
	}
	types.push_back(type);
}

void rv_meta_model::parse_codec(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("codecs requires 2 parameters: %s", join(part, " ").c_str());
	}
	auto codec = codecs_by_name[part[0]] = std::make_shared<rv_codec>(
		part[0], part[1]
	);
	std::string codec_key;
	std::vector<std::string> codec_operands;
	for (size_t i = 2; i < part.size(); i++) {
		auto operand = operands_by_name[part[i]];
		if (!operand) {
			panic("codec %s has unknown operand: %s",
				codec->name.c_str(), part[i].c_str());
		}
		codec->operands.push_back(operand);
		codec_operands.push_back(part[i]);
	}
	codec->codec_key = join(codec_operands, "·");
	if (codec->codec_key.size() == 0) {
		codec->codec_key = "none";
	}
	codecs_by_key[codec->codec_key] = codec;
	codecs.push_back(codec);
}

void rv_meta_model::parse_extension(std::vector<std::string> &part)
{
	if (part.size() < 5) {
		panic("extensions requires 5 parameters: %s", join(part, " ").c_str());
	}
	std::string isa = part[0] + part[1] + part[2];
	auto extension = extensions_by_name[isa] = std::make_shared<rv_extension>(
		part[0], part[1], part[2], part[3], part[4]
	);
	extensions.push_back(extension);
}

void rv_meta_model::parse_format(std::vector<std::string> &part)
{
	if (part.size() < 1) {
		panic("formats requires at least 1 parameters: %s", join(part, " ").c_str());
	}
	auto format = formats_by_name[part[0]] = std::make_shared<rv_format>(
		part[0], part.size() > 1 ? part[1] : ""
	);
	formats.push_back(format);
}

void rv_meta_model::parse_register(std::vector<std::string> &part)
{
	if (part.size() < 5) {
		panic("registers requires 5 parameters: %s", join(part, " ").c_str());
	}
	auto reg = registers_by_name[part[0]] = std::make_shared<rv_register>(
		part[0], part[1], part[2], part[3], part[4]
	);
	registers.push_back(reg);
}

void rv_meta_model::parse_csr(std::vector<std::string> &part)
{
	if (part.size() < 5) {
		panic("csrs requires 5 parameters: %s", join(part, " ").c_str());
	}
	auto csr = csrs_by_name[part[2]] = std::make_shared<rv_csr>(
		part[0], part[1], part[2], part[3], decode_version_spec(part[4])
	);
	csrs.push_back(csr);
}

void rv_meta_model::parse_csr_field(std::vector<std::string> &part)
{
	if (part.size() < 6) {
		panic("csr-fields requires 6 parameters: %s", join(part, " ").c_str());
	}
	auto csr_field = std::make_shared<rv_csr_field>(
		part[0], part[1], part[2], part[3], part[4], decode_version_spec(part[5])
	);
	csr_fields.push_back(csr_field);
}

void rv_meta_model::parse_opcode_major(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("opcode-majors requires at least 2 parameters: %s", join(part, " ").c_str());
	}
	auto opcode_major = std::make_shared<rv_opcode_major>();
	for (auto mnem : part) {
		if (is_mask(mnem)) {
			opcode_major->masks.push_back(decode_mask(mnem));
		} else if (opcode_major->name.size() == 0) {
			opcode_major->name = mnem;
		} else {
			panic("opcode-majors: unexpected token: %s", mnem.c_str());
		}
	}
	opcode_majors.push_back(opcode_major);
}

void rv_meta_model::parse_opcode(std::vector<std::string> &part)
{
	std::vector<std::string> extensions;
	for (size_t i = 1; i < part.size(); i++) {
		std::string mnem = part[i];
		std::transform(mnem.begin(), mnem.end(), mnem.begin(), ::tolower);
		if (is_extension(mnem)) {
			extensions.push_back(mnem);
		}
	}

	std::string opcode_name = part[0];
	if (extensions.size() == 0) {
		panic("no extension assigned for opcode: %s", opcode_name.c_str());
	}
	auto opcode = create_opcode(opcode_name, extensions.front());

	for (size_t i = 1; i < part.size(); i++) {
		std::string mnem = part[i];
		std::transform(mnem.begin(), mnem.end(), mnem.begin(), ::tolower);
		if (is_operand(mnem)) {
			opcode->operands.push_back(operands_by_name[mnem]);
		} else if (is_ignore(mnem)) {
			// presently we ignore masks labeled as ignore
		} else if (is_mask(mnem)) {
			opcode->masks.push_back(decode_mask(mnem));
		} else if (is_codec(mnem)) {
			opcode->codec = codecs_by_name[mnem];
			opcode->format = formats_by_name[opcode->codec->format];
			if (!opcode->format) {
				panic("opcode %s codec %s has unknown format: %s",
					opcode_name.c_str(), opcode->codec->name.c_str(),
					opcode->codec->format.c_str());
			}
			std::string type_name = codec_type_name(opcode->codec);
			opcode->type = types_by_name[type_name];
			if (!opcode->type) {
				panic("opcode %s codec %s has unknown type: %s",
					opcode_name.c_str(), opcode->codec->name.c_str(),
					type_name.c_str());
			}
		} else if (is_extension(mnem)) {
			auto extension = extensions_by_name[mnem];
			opcode->extensions.push_back(extension);
			if (opcode->extensions.size() == 1) {
				extension->opcodes.push_back(opcode);
			}
		} else {
			debug("opcode %s: unknown operand: %s",
				opcode_name.c_str(), mnem.c_str());
		}
	}

	// lookup codec by key and log warning if it does not exist
	if (!opcode->is_pseudo()) {
		std::string codec_key = opcode_codec_key(opcode);
		auto codec = codecs_by_key[codec_key];
		if (!codec) {
			debug("WARNING: codec deduction failure: %-20s  codec_key: %s",
				opcode->name.c_str(), codec_key.c_str());
		} else if (codec->codec_key != codec_key) {
			debug("WARNING: codec deduction mismatch: %-20s  codec_key: %s "
				"(opcode:%s != deduced:%s)",
				opcode->name.c_str(), codec_key.c_str(),
				opcode->codec->name.c_str(),
				codec->name.c_str());
		}
	}

	if (!opcode->codec) {
		panic("opcode has no codec: %s", opcode_name.c_str());
	}
	if (opcode->extensions.size() == 0) {
		panic("opcode has no extensions: %s", opcode_name.c_str());
	}
}

void rv_meta_model::parse_constraint(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("constraints requires 2 parameters: %s", join(part, " ").c_str());
	}
	auto constraint = constraints_by_name[part[0]] = std::make_shared<rv_constraint>(
		part[0], part[1]
	);
	if (part.size() >= 3) {
		constraint->hint = part[2];
	}
	constraints.push_back(constraint);
}

void rv_meta_model::parse_compression(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("invalid compression file requires at least 2 parameters: %s",
			join(part, " ").c_str());
	}
	size_t processed = 0;
	for (auto comp_opcode : lookup_opcode_by_name(part[0])) {
		for (auto opcode : lookup_opcode_by_name(part[1])) {

			// skip opcodes that don't match isa width
			if (opcode->extensions.size() == 1 && comp_opcode->extensions.size() == 1 &&
				opcode->extensions[0]->isa_width != comp_opcode->extensions[0]->isa_width) continue;

			// create compression for this pair of opcodes
			rv_constraint_list constraint_list;
			for (size_t i = 2; i < part.size(); i++) {
				auto ci = constraints_by_name.find(part[i]);
				if (ci == constraints_by_name.end()) {
					panic("compressed opcode %s references unknown constraint %s",
						part[0].c_str(), part[i].c_str());
				}
				constraint_list.push_back(ci->second);
			}
			auto comp = std::make_shared<rv_compressed>(
				comp_opcode, opcode, constraint_list
			);
			comp_opcode->compressed = comp;
			opcode->compressions.push_back(comp);
			compressions.push_back(comp);

			processed++;
		}
	}
	if (processed == 0) {
		debug("WARNING: compressed opcode: %s missing matching opcode: %s",
			part[0].c_str(), part[1].c_str());
	}
}

void rv_meta_model::parse_pseudo(std::vector<std::string> &part)
{
	if (part.size() < 3) {
		panic("invalid pseudo file requires at least 3 parameters: %s",
			join(part, " ").c_str());
	}

	auto pseudo_name = part[0];
	auto opcode_list_i  = opcodes_by_name.find(part[1]);
	if (opcode_list_i == opcodes_by_name.end()) {
		panic("pseudo %s has unknown opcode: %s",
			pseudo_name.c_str(), part[1].c_str());
	}
	auto real_opcode = opcode_list_i->second.front();
	auto format = formats_by_name[part[2]];
	if (!format) {
		panic("pseudo %s has unknown format: %s",
			pseudo_name.c_str(), part[2].c_str());
	}
	rv_constraint_list constraint_list;
	for (size_t i = 3; i < part.size(); i++) {
		auto ci = constraints_by_name.find(part[i]);
		if (ci == constraints_by_name.end()) {
			panic("psuedo opcode %s references unknown constraint %s",
				pseudo_name.c_str(), part[i].c_str());
		}
		constraint_list.push_back(ci->second);
	}

	// create opcode (if needed)
	std::string pseudo_opcode_name = "@" + pseudo_name;
	auto existing_opcode_list = lookup_opcode_by_name(pseudo_name);
	auto pseudo_opcode_list = lookup_opcode_by_name(pseudo_opcode_name);
	rv_opcode_ptr pseudo_opcode;
	if (existing_opcode_list.size() > 0) {
		pseudo_opcode = existing_opcode_list.front();
	} else if (pseudo_opcode_list.size() == 0) {
		// create a opcode for this pseudo opcode
		std::string pseudo_opcode_name = "@" + pseudo_name;
		pseudo_opcode = create_opcode(pseudo_opcode_name, "rv32p");

		// use the format from the meta/pseudo definition
		pseudo_opcode->format = format;
		
		// derive operands from the format
		std::vector<std::string> operand_names = split(format->operands, ",");
		for (auto operand_name : operand_names) {
			// TODO - abstract this inference hack
			if (operand_name == "none") continue;
			if (operand_name == "offset") operand_name = "oimm20";
			rv_operand_ptr operand = operands_by_name[operand_name];
			if (!operand) {
				panic("psuedo opcode %s references unknown operand %s",
					pseudo_name.c_str(), operand_name.c_str());
			}
			pseudo_opcode->operands.push_back(operand);
		}
		// use the real opcode extensions
		pseudo_opcode->extensions = real_opcode->extensions;
	} else {
		pseudo_opcode = pseudo_opcode_list.front();
	}

	// create pseudo
	auto pseudo = std::make_shared<rv_pseudo>(
		pseudo_name, pseudo_opcode, real_opcode, format, constraint_list
	);
	real_opcode->pseudos.push_back(pseudo);
	pseudos.push_back(pseudo);
	pseudos_by_name[pseudo_name] = pseudo;
	pseudo_opcode->pseudo = pseudo;
	pseudo_opcode->codec = real_opcode->codec;
}

void rv_meta_model::parse_opcode_classes(std::vector<std::string> &part)
{
	if (part.size() < 2) return;
	std::string opcode_name = part[0];
	std::vector<std::string> op_class_names = split(part[1], ",");
	rv_opcode_class_list op_classes;

	for (auto op_class_name : op_class_names) {
		rv_opcode_class_ptr opcode_class;
		auto ci = opcode_classes_byname.find(op_class_name);
		if (ci != opcode_classes_byname.end()) {
			opcode_class = ci->second;
		} else {
			opcode_class = std::make_shared<rv_opcode_class>(op_class_name);
			opcode_classes.push_back(opcode_class);
			opcode_classes_byname[op_class_name] = opcode_class;
		}
		op_classes.push_back(opcode_class);
	}

	for (auto opcode : lookup_opcode_by_name(opcode_name)) {
		for (auto opcode_class : op_classes) {
			opcode->classes.push_back(opcode_class);
			if (std::find(opcode_class->opcodes.begin(), opcode_class->opcodes.end(),
					opcode) == opcode_class->opcodes.end()) {
				opcode_class->opcodes.push_back(opcode);
			}
		}
	}
}

void rv_meta_model::parse_opcode_fullname(std::vector<std::string> &part)
{
	if (part.size() < 1) return;
	std::string opcode_name = part[0];
	std::string opcode_fullname = part.size() > 1 ? part[1] : "";
	for (auto opcode : lookup_opcode_by_name(opcode_name)) {
		opcode->fullname = opcode_fullname;
	}
	for (auto opcode : lookup_opcode_by_name(std::string("@") + opcode_name)) {
		opcode->fullname = opcode_fullname;
	}
}

void rv_meta_model::parse_opcode_description(std::vector<std::string> &part)
{
	if (part.size() < 1) return;
	std::string opcode_name = part[0];
	std::string opcode_description = part.size() > 1 ? part[1] : "";
	for (auto opcode : lookup_opcode_by_name(opcode_name)) {
		opcode->description = opcode_description;
	}
	for (auto opcode : lookup_opcode_by_name(std::string("@") + opcode_name)) {
		opcode->description = opcode_description;
	}
}

void rv_meta_model::parse_opcode_pseudocode_c(std::vector<std::string> &part)
{
	if (part.size() < 1) return;
	std::string opcode_name = part[0];
	std::string opcode_pseudocode_c = part.size() > 1 ? part[1] : "";
	for (auto opcode : lookup_opcode_by_name(opcode_name)) {
		opcode->pseudocode_c = opcode_pseudocode_c;
	}
	for (auto opcode : lookup_opcode_by_name(std::string("@") + opcode_name)) {
		opcode->pseudocode_c = opcode_pseudocode_c;
	}
}

void rv_meta_model::parse_opcode_pseudocode_alt(std::vector<std::string> &part)
{
	if (part.size() < 1) return;
	std::string opcode_name = part[0];
	std::string opcode_pseudocode_alt = part.size() > 1 ? part[1] : "";
	for (auto opcode : lookup_opcode_by_name(opcode_name)) {
		opcode->pseudocode_alt = opcode_pseudocode_alt;
	}
	for (auto opcode : lookup_opcode_by_name(std::string("@") + opcode_name)) {
		opcode->pseudocode_alt = opcode_pseudocode_alt;
	}
}

bool rv_meta_model::read_metadata(std::string dirname)
{
	for (auto part : read_file(dirname + std::string("/") + OPERANDS_FILE)) parse_operand(part);
	for (auto part : read_file(dirname + std::string("/") + ENUMS_FILE)) parse_enum(part);
	for (auto part : read_file(dirname + std::string("/") + TYPES_FILE)) parse_type(part);
	for (auto part : read_file(dirname + std::string("/") + FORMATS_FILE)) parse_format(part);
	for (auto part : read_file(dirname + std::string("/") + CODECS_FILE)) parse_codec(part);
	for (auto part : read_file(dirname + std::string("/") + EXTENSIONS_FILE)) parse_extension(part);
	for (auto part : read_file(dirname + std::string("/") + REGISTERS_FILE)) parse_register(part);
	for (auto part : read_file(dirname + std::string("/") + CSRS_FILE)) parse_csr(part);
	for (auto part : read_file(dirname + std::string("/") + CSR_FIELDS_FILE)) parse_csr_field(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODE_MAJORS_FILE)) parse_opcode_major(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODES_FILE)) parse_opcode(part);
	for (auto part : read_file(dirname + std::string("/") + CONSTRAINTS_FILE)) parse_constraint(part);
	for (auto part : read_file(dirname + std::string("/") + COMPRESSION_FILE)) parse_compression(part);
	for (auto part : read_file(dirname + std::string("/") + PSEUDO_FILE)) parse_pseudo(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODE_CLASSES_FILE)) parse_opcode_classes(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODE_FULLNAMES_FILE)) parse_opcode_fullname(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODE_DESCRIPTIONS_FILE)) parse_opcode_description(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODE_PSEUDOCODE_C_FILE)) parse_opcode_pseudocode_c(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODE_PSEUDOCODE_ALT_FILE)) parse_opcode_pseudocode_alt(part);
	return true;
}
