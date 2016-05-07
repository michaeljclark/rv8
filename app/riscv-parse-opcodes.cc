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
#include "riscv-cmdline.h"
#include "riscv-color.h"

#define _COLOR_OPCODE_BEGIN _COLOR_BEGIN _COLOR_UNDERSCORE _COLOR_SEP _COLOR_FG_YELLOW _COLOR_END
#define _COLOR_BITS_BEGIN   _COLOR_BEGIN _COLOR_REVERSE    _COLOR_SEP _COLOR_FG_GREEN  _COLOR_END
#define _COLOR_FORMAT_BEGIN _COLOR_BEGIN _COLOR_BOLD       _COLOR_SEP _COLOR_FG_RED    _COLOR_END
#define _COLOR_LEGEND_BEGIN _COLOR_BEGIN _COLOR_FG_WHITE   _COLOR_SEP _COLOR_BG_BLACK  _COLOR_END
#define _COLOR_EXT_BEGIN    _COLOR_BEGIN _COLOR_FG_RED     _COLOR_END

static bool enable_color = false;

static const char* ARGS_FILE           = "args";
static const char* TYPES_FILE          = "types";
static const char* EXTENSIONS_FILE     = "extensions";
static const char* FORMATS_FILE        = "formats";
static const char* REGISTERS_FILE      = "registers";
static const char* CSRS_FILE           = "csrs";
static const char* OPCODES_FILE        = "opcodes";
static const char* COMPRESSION_FILE    = "compression";
static const char* DESCRIPTIONS_FILE   = "descriptions";

// TODO - make this variable based on extension metadata
static const ssize_t INSN_WIDTH = 32;

struct riscv_bitrange;
struct riscv_bitrange_spec;
struct riscv_arg;
struct riscv_type;
struct riscv_extension;
struct riscv_format;
struct riscv_register;
struct riscv_csr;
struct riscv_opcode;
struct riscv_constraint;
struct riscv_compressed;
struct riscv_bitrange;
struct riscv_decoder_node;

typedef std::shared_ptr<riscv_arg> riscv_arg_ptr;
typedef std::vector<riscv_arg_ptr> riscv_arg_list;
typedef std::map<std::string,riscv_arg_ptr> riscv_arg_map;
typedef std::shared_ptr<riscv_type> riscv_type_ptr;
typedef std::vector<riscv_type_ptr> riscv_type_list;
typedef std::map<std::string,riscv_type_ptr> riscv_type_map;
typedef std::shared_ptr<riscv_extension> riscv_extension_ptr;
typedef std::vector<riscv_extension_ptr> riscv_extension_list;
typedef std::map<std::string,riscv_extension_ptr> riscv_extension_map;
typedef std::shared_ptr<riscv_format> riscv_format_ptr;
typedef std::vector<riscv_format_ptr> riscv_format_list;
typedef std::map<std::string,riscv_format_ptr> riscv_format_map;
typedef std::shared_ptr<riscv_register> riscv_register_ptr;
typedef std::vector<riscv_register_ptr> riscv_register_list;
typedef std::map<std::string,riscv_register_ptr> riscv_register_map;
typedef std::shared_ptr<riscv_csr> riscv_csr_ptr;
typedef std::vector<riscv_csr_ptr> riscv_csr_list;
typedef std::map<std::string,riscv_csr_ptr> riscv_csr_map;
typedef std::pair<riscv_bitrange,size_t> riscv_opcode_mask;
typedef std::vector<riscv_opcode_mask> riscv_opcode_mask_list;
typedef std::shared_ptr<riscv_opcode> riscv_opcode_ptr;
typedef std::vector<riscv_opcode_ptr> riscv_opcode_list;
typedef std::shared_ptr<riscv_constraint> riscv_constraint_ptr;
typedef std::map<std::string,riscv_constraint_ptr> riscv_constraint_map;
typedef std::vector<riscv_constraint_ptr> riscv_constraint_list;
typedef std::shared_ptr<riscv_compressed> riscv_compressed_ptr;
typedef std::vector<riscv_compressed_ptr> riscv_compressed_list;
typedef std::map<std::string,riscv_opcode_ptr> riscv_opcode_map;
typedef std::map<std::string,riscv_opcode_list> riscv_opcode_list_map;
typedef std::set<riscv_opcode_ptr> riscv_opcode_set;

struct riscv_bitrange
{
	ssize_t msb;
	ssize_t lsb;

	riscv_bitrange(std::string bitrange);
	riscv_bitrange(ssize_t msb, ssize_t lsb) : msb(msb), lsb(lsb) {}

	std::string to_string(std::string sep);
};

struct riscv_bitrange_spec
{
	typedef std::vector<riscv_bitrange> riscv_bitrange_list;
	typedef std::pair<riscv_bitrange,riscv_bitrange_list> riscv_bitrange_segment;

	std::vector<riscv_bitrange_segment> segments;

	riscv_bitrange_spec(std::string bitrange_spec);

	bool matches_bit(ssize_t bit);
	std::string to_string();
	std::string to_template();
};

struct riscv_arg
{
	std::string name;
	riscv_bitrange_spec bitrange_spec;
	std::string type;
	std::string label;
	std::string fg_color;
	std::string bg_color;

	riscv_arg(std::string name, std::string bitrange_spec, std::string type, std::string label, std::string fg_color, std::string bg_color)
		: name(name), bitrange_spec(bitrange_spec), type(type), label(label), fg_color(fg_color), bg_color(bg_color) {}

	char char_code() {
		if (type == "ireg") return 'R';
		else if (type == "freg") return 'R';
		else if (type == "arg") return 'A';
		else if (type == "imm") return 'I';
		else if (type == "ipc") return 'J';
		else return '?';
	}
};

struct riscv_type
{
	std::string name;
	std::string format;

	riscv_type(std::string name, std::string format) : name(name), format(format) {}
};

struct riscv_extension
{
	std::string name;
	std::string prefix;
	ssize_t isa_width;
	char alpha_code;
	ssize_t insn_width;
	std::string description;
	riscv_opcode_list opcodes;

	riscv_extension(std::string prefix, std::string isa_width, std::string alpha_code, std::string insn_width, std::string description) :
		name(prefix + isa_width + alpha_code), prefix(prefix),
		isa_width(strtoull(isa_width.c_str(), NULL, 10)),
		alpha_code(alpha_code.length() > 0 ? alpha_code[0] : '?'),
		insn_width(strtoull(insn_width.c_str(), NULL, 10)),
		description(description) {}
};

struct riscv_format
{
	std::string name;
	std::string args;

	riscv_format(std::string name, std::string args) : name(name), args(args) {}
};

struct riscv_register
{
	std::string name;
	std::string alias;
	std::string type;
	std::string save;
	std::string description;

	riscv_register(std::string name, std::string alias, std::string type, std::string save, std::string description)
		: name(name), alias(alias), type(type), save(save), description(description) {}
};

struct riscv_csr
{
	std::string number;
	std::string access;
	std::string name;
	std::string description;

	riscv_csr(std::string number, std::string access, std::string name, std::string description)
		: number(number), access(access), name(name), description(description) {}
};

struct riscv_opcode
{
	ssize_t num;
	std::string key;
	std::string name;
	std::string long_name;
	std::string description;
	riscv_arg_list args;
	riscv_opcode_mask_list masks;
	riscv_type_ptr type;
	riscv_extension_list extensions;
	riscv_compressed_ptr compressed;
	riscv_compressed_list compressions;

	size_t mask;
	size_t match;
	size_t done;

	riscv_opcode(std::string key, std::string name) : key(key), name(name), mask(0), match(0), done(0) {}

	bool match_extension(riscv_extension_list &ext_subset) {
		if (ext_subset.size() == 0) return true;
		for (auto ext : extensions) {
			if (std::find(ext_subset.begin(), ext_subset.end(), ext) != ext_subset.end()) return true;
		}
		return false;
	}

	riscv_arg_ptr find_arg(ssize_t bit) {
		for (auto arg: args) {
			if (arg->bitrange_spec.matches_bit(bit)) return arg;
		}
		return riscv_arg_ptr();
	}
};

struct riscv_constraint
{
	std::string name;

	riscv_constraint(std::string name) : name(name) {}
};

struct riscv_compressed
{
	riscv_opcode_ptr copcode;
	riscv_opcode_ptr opcode;
	riscv_constraint_list constraint_list;

	riscv_compressed(riscv_opcode_ptr copcode, riscv_opcode_ptr opcode, riscv_constraint_list constraint_list)
		: copcode(copcode), opcode(opcode), constraint_list(constraint_list) {}
};

struct riscv_decoder_node
{
	std::vector<ssize_t> bits;
	std::vector<ssize_t> vals;
	std::map<ssize_t,riscv_opcode_list> val_opcodes;
	std::map<ssize_t,riscv_decoder_node> val_decodes;
};

struct riscv_inst_set
{
	const ssize_t DEFAULT = std::numeric_limits<ssize_t>::max();

	riscv_arg_list           args;
	riscv_arg_map            args_by_name;
	riscv_type_list          types;
	riscv_type_map           types_by_name;
	riscv_extension_list     extensions;
	riscv_extension_map      extensions_by_name;
	riscv_format_list        formats;
	riscv_format_map         formats_by_name;
	riscv_register_list      registers;
	riscv_register_map       registers_by_name;
	riscv_csr_list           csrs;
	riscv_csr_map            csrs_by_name;
	riscv_opcode_list        opcodes;
	riscv_opcode_map         opcodes_by_key;
	riscv_opcode_list_map    opcodes_by_name;
	riscv_opcode_list_map    opcodes_by_type;
	riscv_constraint_list    constraints;
	riscv_constraint_map     constraints_by_name;
	riscv_compressed_list    compressions;
	riscv_extension_list     ext_subset;

	riscv_decoder_node node;

	static riscv_opcode_mask decode_mask(std::string bit_spec);
	static std::string opcode_mask(riscv_opcode_ptr opcode);
	static std::string opcode_format(std::string prefix, riscv_opcode_ptr opcode, char dot, bool key = true);
	static std::string opcode_isa_shortname(riscv_opcode_ptr opcode);
	static std::vector<riscv_bitrange> bitmask_to_bitrange(std::vector<ssize_t> &bits);
	static std::string format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment);

	std::vector<std::string> parse_line(std::string line);
	std::vector<std::vector<std::string>> read_file(std::string filename);
	riscv_extension_list decode_isa_extensions(std::string isa_spec);
	riscv_opcode_ptr create_opcode(std::string opcode_name, std::string extension);
	riscv_opcode_ptr lookup_opcode_by_key(std::string opcode_name);
	riscv_opcode_list lookup_opcode_by_name(std::string opcode_name);

	bool is_arg(std::string mnem);
	bool is_ignore(std::string mnem);
	bool is_mask(std::string mnem);
	bool is_type(std::string mnem);
	bool is_extension(std::string mnem);

	void parse_arg(std::vector<std::string> &part);
	void parse_type(std::vector<std::string> &part);
	void parse_extension(std::vector<std::string> &part);
	void parse_format(std::vector<std::string> &part);
	void parse_register(std::vector<std::string> &part);
	void parse_csr(std::vector<std::string> &part);
	void parse_opcode(std::vector<std::string> &part);
	void parse_compression(std::vector<std::string> &part);
	void parse_description(std::vector<std::string> &part);

	bool read_metadata(std::string dirname);

	void generate_map();
	void generate_decoder();

	std::string colorize_args(riscv_opcode_ptr opcode);
	std::vector<std::string> get_unique_types();

	void print_latex();
	void print_map();
	void print_opcodes_h();
	void print_opcodes_c();
	void print_switch_c();

	void generate_decoder_node(riscv_decoder_node &node, riscv_opcode_list &opcode_list);
	void print_switch_decoder_node(riscv_decoder_node &node, size_t indent);
};

static std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

static std::vector<std::string> split(std::string str, std::string separator,
	bool includeEmpty, bool includeSeparator)
{
	size_t last_index = 0, index;
	std::vector<std::string> comps;
	while ((index = str.find_first_of(separator, last_index)) != std::string::npos) {
		if (includeEmpty || index - last_index > 0) {
			comps.push_back(str.substr(last_index, index - last_index));
		}
		if (includeSeparator) {
			comps.push_back(separator);
		}
		last_index = index + separator.length();
	}
	if (includeEmpty || str.size() - last_index > 0) {
		comps.push_back(str.substr(last_index, str.size() - last_index));
	}
	return comps;
}

template <typename T>
std::string join(std::vector<T> list, std::string separator)
{
	std::stringstream ss;
	for (typename std::vector<T>::iterator i = list.begin(); i != list.end(); i++) {
		if (i != list.begin()) ss << separator;
		ss << *i;
	}
	return ss.str();
}

riscv_bitrange::riscv_bitrange(std::string bitrange)
{
	std::vector<std::string> comps = split(bitrange, ":", false, false);
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

std::string riscv_bitrange::to_string(std::string sep)
{
	std::stringstream ss;
	ss << msb;
	if (msb != lsb) {
		ss << sep << lsb;
	}
	return ss.str();
}

riscv_bitrange_spec::riscv_bitrange_spec(std::string bitrange_spec)
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

	std::vector<std::string> comps = split(bitrange_spec, ",", false, false);
	for (std::string comp : comps) {
		size_t bopen = comp.find("[");
		size_t bclose = comp.find("]");
		if (bopen != std::string::npos && bclose != std::string::npos) {
			riscv_bitrange gather(comp.substr(0, bopen));
			std::string scatter_spec = comp.substr(bopen + 1, bclose - bopen - 1);
			riscv_bitrange_list scatter;
			for (auto scatter_comp : split(scatter_spec, "|", false, false)) {
				scatter.push_back(riscv_bitrange(scatter_comp));
			}
			segments.push_back(riscv_bitrange_segment(gather, scatter));
		} else {
			riscv_bitrange gather(comp);
			riscv_bitrange_list scatter({ riscv_bitrange(gather.msb - gather.lsb, 0) });
			segments.push_back(riscv_bitrange_segment(gather, scatter));
		}
	}
}

bool riscv_bitrange_spec::matches_bit(ssize_t bit)
{
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (bit <= si->first.msb && bit >= si->first.lsb) return true;
	}
	return false;
}

std::string riscv_bitrange_spec::to_string()
{
	std::stringstream ss;
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (si != segments.begin()) ss << ",";
		ss << si->first.to_string(":") << "[";
		for (auto ti = si->second.begin(); ti != si->second.end(); ti++) {
			if (ti != si->second.begin()) ss << "|";
			ss << ti->to_string(":");
		}
		ss << "]";
	}
	return ss.str();
}

std::string riscv_bitrange_spec::to_template()
{
	ssize_t msb = 0;
	for (auto si = segments.begin(); si != segments.end(); si++) {
		for (auto ti = si->second.begin(); ti != si->second.end(); ti++) {
			if (ti->msb > msb) msb = ti->msb;
		}
	}
	std::stringstream ss;
	ss << "imm_t<" << (msb + 1) << ", ";
	for (auto si = segments.begin(); si != segments.end(); si++) {
		if (si != segments.begin()) ss << ", ";
		ss << "S<" << si->first.to_string(",") << ", ";
		for (auto ti = si->second.begin(); ti != si->second.end(); ti++) {
			if (ti != si->second.begin()) ss << ",";
			ss << "B<" << ti->to_string(",") << ">";
		}
		ss << ">";
	}
	ss << ">";
	return ss.str();
}

riscv_opcode_mask riscv_inst_set::decode_mask(std::string bit_spec)
{
	std::vector<std::string> spart = split(bit_spec, "=", false, false);
	if (spart.size() != 2) {
		printf("bit range %s must be in form n..m=v\n", bit_spec.c_str());
		exit(1);
	}
	std::vector<std::string> rpart = split(spart[0], "..", false, false);
	ssize_t msb, lsb, val;
	if (rpart.size() == 1) {
		msb = lsb = strtoul(rpart[0].c_str(), nullptr, 10);
	} else if (rpart.size() == 2) {
		msb = strtoul(rpart[0].c_str(), nullptr, 10);
		lsb = strtoul(rpart[1].c_str(), nullptr, 10);
	} else {
		printf("bit range %s must be in form n..m=v\n", bit_spec.c_str());
		exit(1);
	}
	if (spart[1].find("0x") == 0) {
		val = strtoul(spart[1].c_str() + 2, nullptr, 16);
	} else {
		val = strtoul(spart[1].c_str(), nullptr, 10);
	}

	return riscv_opcode_mask(riscv_bitrange(msb, lsb), val);
}

std::vector<riscv_bitrange> riscv_inst_set::bitmask_to_bitrange(std::vector<ssize_t> &bits)
{	
	std::vector<riscv_bitrange> ranges;
	if (bits.size() > 0) {
		ranges.push_back(riscv_bitrange(bits[0], bits[0]));
		for (size_t i = 1; i < bits.size(); i++) {
			if (bits[i] + 1 == ranges.back().lsb) {
				ranges.back().lsb = bits[i];
			} else {
				ranges.push_back(riscv_bitrange(bits[i], bits[i]));
			}
		}
	}
	return ranges;
}

std::string riscv_inst_set::format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment)
{
	std::vector<riscv_bitrange> ranges = bitmask_to_bitrange(bits);
	std::stringstream ss;

	ssize_t total_length = bits.size();
	ssize_t range_start = bits.size();

	for (auto ri = ranges.begin(); ri != ranges.end(); ri++) {
		riscv_bitrange r = *ri;
		ssize_t range_end = range_start - (r.msb - r.lsb);
		ssize_t shift = r.msb - range_start + 1;
		if (ri != ranges.begin()) ss << " | ";
		ss << "((" << var << " >> " << shift << ") & 0b";
		for (ssize_t i = total_length; i > 0; i--) {
			if (i <= range_start && i >= range_end) ss << "1";
			else ss << "0";
		}
		ss << ")";
		range_start -= (r.msb - r.lsb) + 1;
	}

	if (comment) {
		ss << " /* " << var << "[";
		for (auto ri = ranges.begin(); ri != ranges.end(); ri++) {
			riscv_bitrange r = *ri;
			if (ri != ranges.begin()) ss << "|";
			if (r.msb == r.lsb) ss << r.msb;
			else ss << r.msb << ":" << r.lsb;
		}
		ss << "] */";
	}

	return ss.str();
}

std::string riscv_inst_set::opcode_mask(riscv_opcode_ptr opcode)
{
	std::stringstream ss;
	ss << std::left << std::setw(20) << "";
	for (auto &mask : opcode->masks) {
		ss << " " << mask.first.msb << ".." << mask.first.lsb << "=" << mask.second;
	}
	return ss.str();
}

std::string riscv_inst_set::opcode_format(std::string prefix, riscv_opcode_ptr opcode, char dot, bool key)
{
	std::string name = key ? opcode->key : opcode->name;
	if (name.find("@") == 0) name = name.substr(1);
	std::replace(name.begin(), name.end(), '.', dot);
	return prefix + name;
}

std::string riscv_inst_set::opcode_isa_shortname(riscv_opcode_ptr opcode)
{
	auto &ext = opcode->extensions.front();
	std::string short_name = ext->prefix;
	short_name += ext->alpha_code;
	return short_name;
}

std::vector<std::string> riscv_inst_set::parse_line(std::string line)
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

std::vector<std::vector<std::string>> riscv_inst_set::read_file(std::string filename)
{
	std::vector<std::vector<std::string>> data;
	std::ifstream in(filename.c_str());
	std::string line;
	if (!in.is_open()) {
		panic("error opening %s\n", filename.c_str());
	}
	while (in.good())
	{
		std::getline(in, line);
		size_t hoffset = line.find("#");
		if (hoffset != std::string::npos) {
			line = ltrim(rtrim(line.substr(0, hoffset)));
		}
		std::vector<std::string> part = parse_line(line);
		if (part.size() == 0) continue;
		data.push_back(part);
	}
	in.close();
	return data;
}


riscv_extension_list riscv_inst_set::decode_isa_extensions(std::string isa_spec)
{
	riscv_extension_list list;
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
			}
		}
	}
	if (ext_prefix.size() == 0 || ext_isa_width == 0) {
		panic("illegal isa spec: %s", isa_spec.c_str());
	}

	// replace 'g' with 'imafd'
	size_t g_offset = isa_spec.find("g");
	if (g_offset != std::string::npos) {
		isa_spec = isa_spec.replace(isa_spec.begin() + g_offset, isa_spec.begin() + g_offset + 1, "imafd");
	}

	// lookup extensions
	ssize_t ext_offset = ext_prefix.length() + ext_isa_width_str.length();
	for (auto i = isa_spec.begin() + ext_offset; i != isa_spec.end(); i++) {
		std::string ext_name = isa_spec.substr(0, ext_offset) + *i;
		auto ext = extensions_by_name[ext_name];
		if (!ext) {
			panic("illegal isa spec: %s: missing extension: %s", isa_spec.c_str(), ext_name.c_str());
		}
		if (std::find(list.begin(), list.end(), ext) != list.end()) {
			panic("illegal isa spec: %s: duplicate extension: %s", isa_spec.c_str(), ext_name.c_str());
		}
		list.push_back(ext);
	}
	return list;
}

riscv_opcode_ptr riscv_inst_set::create_opcode(std::string opcode_name, std::string extension)
{
	// create key for the opcode
	riscv_opcode_ptr opcode = lookup_opcode_by_key(opcode_name);
	if (opcode) {
		// if the opcode already exists then rename the previous opcode using its isa extension
		opcode->key = opcode_name + std::string(".") + opcode->extensions.front()->name;
		opcodes_by_key.erase(opcode_name);
		opcodes_by_key[opcode->key] = opcode;

		// and add the new opcode with its isa extension
		std::string opcode_key = opcode_name + std::string(".") + extension;
		if (opcodes_by_key.find(opcode_key) != opcodes_by_key.end()) {
			panic("opcode key with same extension already exists: %s", opcode_key.c_str());
		}
		opcode = opcodes_by_key[opcode_key] = std::make_shared<riscv_opcode>(opcode_key, opcode_name);
		opcode->num = opcodes.size();
		opcodes.push_back(opcode);
	} else {
		opcode = opcodes_by_key[opcode_name] = std::make_shared<riscv_opcode>(opcode_name, opcode_name);
		opcode->num = opcodes.size();
		opcodes.push_back(opcode);
	}

	// add opcode to the opcode by name list, creating a new list if one doesn't exist
	auto opcode_list_i  = opcodes_by_name.find(opcode_name);
	if (opcode_list_i == opcodes_by_name.end()) {
		opcodes_by_name[opcode_name] = { opcode };
	} else {
		opcode_list_i->second.push_back(opcode);
	}

	return opcode;
}

riscv_opcode_ptr riscv_inst_set::lookup_opcode_by_key(std::string opcode_key)
{
	auto i = opcodes_by_key.find(opcode_key);
	if (i != opcodes_by_key.end()) return i->second;
	return riscv_opcode_ptr();
}

riscv_opcode_list riscv_inst_set::lookup_opcode_by_name(std::string opcode_name)
{
	auto i = opcodes_by_name.find(opcode_name);
	if (i != opcodes_by_name.end()) return i->second;
	return riscv_opcode_list();
}

bool riscv_inst_set::is_arg(std::string mnem)
{
	return (args_by_name.find(mnem) != args_by_name.end());
}

bool riscv_inst_set::is_ignore(std::string mnem)
{
	return (mnem.find("=ignore") != std::string::npos);
}

bool riscv_inst_set::is_mask(std::string mnem)
{
	return (mnem.find("=") != std::string::npos);
}

bool riscv_inst_set::is_type(std::string mnem)
{
	return (types_by_name.find(mnem) != types_by_name.end());
}

bool riscv_inst_set::is_extension(std::string mnem)
{
	return (extensions_by_name.find(mnem) != extensions_by_name.end());
}

void riscv_inst_set::parse_arg(std::vector<std::string> &part)
{
	if (part.size() < 6) {
		panic("invalid args file requires 6 parameters: %s", join(part, " ").c_str());
	}
	auto arg = args_by_name[part[0]] = std::make_shared<riscv_arg>(part[0], part[1], part[2], part[3], part[4], part[5]);
	args.push_back(arg);
}

void riscv_inst_set::parse_type(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("invalid types file requires 2 parameters: %s", join(part, " ").c_str());
	}
	auto type = types_by_name[part[0]] = std::make_shared<riscv_type>(part[0], part[1]);
	types.push_back(type);
}

void riscv_inst_set::parse_extension(std::vector<std::string> &part)
{
	if (part.size() < 5) {
		panic("invalid extensions file requires 5 parameters: %s", join(part, " ").c_str());
	}
	auto extension = extensions_by_name[part[0]+part[1]+part[2]] = std::make_shared<riscv_extension>(part[0], part[1], part[2], part[3], part[4]);
	extensions.push_back(extension);
}

void riscv_inst_set::parse_format(std::vector<std::string> &part)
{
	if (part.size() < 1) {
		panic("invalid formats file requires at least 1 parameters: %s", join(part, " ").c_str());
	}
	auto format = formats_by_name[part[0]] = std::make_shared<riscv_format>(part[0], part.size() > 1 ? part[1] : "");
	formats.push_back(format);
}

void riscv_inst_set::parse_register(std::vector<std::string> &part)
{
	if (part.size() < 5) {
		panic("invalid registers file requires 5 parameters: %s", join(part, " ").c_str());
	}
	auto reg = registers_by_name[part[0]] = std::make_shared<riscv_register>(part[0], part[1], part[2], part[3], part[4]);
	registers.push_back(reg);
}

void riscv_inst_set::parse_csr(std::vector<std::string> &part)
{
	if (part.size() < 4) {
		panic("invalid csrs file requires 4 parameters: %s", join(part, " ").c_str());
	}
	auto csr = csrs_by_name[part[2]] = std::make_shared<riscv_csr>(part[0], part[1], part[2], part[3]);
	csrs.push_back(csr);
}

void riscv_inst_set::parse_opcode(std::vector<std::string> &part)
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
		if (is_arg(mnem)) {
			opcode->args.push_back(args_by_name[mnem]);
		} else if (is_ignore(mnem)) {
			// presently we ignore masks labeled as ignore
		} else if (is_mask(mnem)) {
			opcode->masks.push_back(decode_mask(mnem));
		} else if (is_type(mnem)) {
			opcode->type = types_by_name[mnem];
		} else if (is_extension(mnem)) {
			auto extension = extensions_by_name[mnem];
			opcode->extensions.push_back(extension);
			if (opcode->extensions.size() == 1) {
				extension->opcodes.push_back(opcode);
			}
		} else {
			debug("opcode %s: unknown arg: %s", opcode_name.c_str(), mnem.c_str());
		}
	}

	if (!opcode->type) {
		panic("opcode has no type: %s", opcode_name.c_str());
	}
	if (opcode->extensions.size() == 0) {
		panic("opcode has no extensions: %s", opcode_name.c_str());
	}
}

void riscv_inst_set::parse_compression(std::vector<std::string> &part)
{
	if (part.size() < 2) {
		panic("invalid compression file requires at least 2 parameters: %s", join(part, " ").c_str());
	}
	for (auto copcode : lookup_opcode_by_name(part[0])) {
		for (auto opcode : lookup_opcode_by_name(part[1])) {
			riscv_constraint_list constraint_list;
			for (size_t i = 2; i < part.size(); i++) {
				auto ci = constraints_by_name.find(part[i]);
				if (ci == constraints_by_name.end()) {
					auto constraint = std::make_shared<riscv_constraint>(part[i]);
					constraints_by_name[part[i]] = constraint;
					auto ci2 = std::find_if(constraints.begin(), constraints.end(),
						[&constraint] (const riscv_constraint_ptr &c) {
							return constraint->name < c->name;
					});
					constraints.insert(ci2, constraint);
					constraint_list.push_back(constraint);
				} else {
					constraint_list.push_back(ci->second);
				}
			}
			auto comp = std::make_shared<riscv_compressed>(copcode, opcode, constraint_list);
			copcode->compressed = comp;
			opcode->compressions.push_back(comp);
			compressions.push_back(comp);
		}
	}
}

void riscv_inst_set::parse_description(std::vector<std::string> &part)
{
	if (part.size() < 2) return;
	std::string opcode_name = part[0];
	std::string opcode_long_name = part[1];
	std::string opcode_description = part.size() > 2 ? part[2] : "";
	for (auto opcode : lookup_opcode_by_name(opcode_name)) {
		opcode->long_name = opcode_long_name;
		opcode->description = opcode_description;
	}
}

bool riscv_inst_set::read_metadata(std::string dirname)
{
	for (auto part : read_file(dirname + std::string("/") + ARGS_FILE)) parse_arg(part);
	for (auto part : read_file(dirname + std::string("/") + TYPES_FILE)) parse_type(part);
	for (auto part : read_file(dirname + std::string("/") + EXTENSIONS_FILE)) parse_extension(part);
	for (auto part : read_file(dirname + std::string("/") + FORMATS_FILE)) parse_format(part);
	for (auto part : read_file(dirname + std::string("/") + REGISTERS_FILE)) parse_register(part);
	for (auto part : read_file(dirname + std::string("/") + CSRS_FILE)) parse_csr(part);
	for (auto part : read_file(dirname + std::string("/") + OPCODES_FILE)) parse_opcode(part);
	for (auto part : read_file(dirname + std::string("/") + COMPRESSION_FILE)) parse_compression(part);
	for (auto part : read_file(dirname + std::string("/") + DESCRIPTIONS_FILE)) parse_description(part);
	return true;
}

void riscv_inst_set::generate_map()
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

void riscv_inst_set::generate_decoder()
{
	generate_decoder_node(node, opcodes);
}

std::string riscv_inst_set::colorize_args(riscv_opcode_ptr opcode)
{
	auto format = formats_by_name[opcode->type->format];
	auto args = format->args;

	std::vector<char> token;
	std::vector<std::string> comps;

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
			auto new_comp = riscv_colors_to_ansi_escape_sequence(arg->fg_color, arg->bg_color, ansi_color_normal);
			new_comp.append(comp);
			new_comp.append(_COLOR_RESET);
			comps[i] = new_comp;
		}
	}

	return join(comps, "");
}


std::vector<std::string> riscv_inst_set::get_unique_types()
{
	std::vector<std::string> type_names;
	for (auto &type : types) {
		std::string type_name = split(type->name, "+", false, false)[0];
		if (std::find(type_names.begin(), type_names.end(), type_name) == type_names.end()) {
			type_names.push_back(type_name);
		}
	}
	return type_names;
}

void riscv_inst_set::print_latex()
{
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

	// table row data
	struct riscv_table_row
	{
		riscv_extension_ptr extension; // set if this is a title row
		riscv_opcode_ptr opcode;       // set if this is an opcode row
	};

	// create list of opcodes ordered by extension, with blank entries between extension sections
	std::vector<riscv_table_row> rows;
	for (auto &ext : extensions) {
		if (ext_subset.size() > 0 && std::find(ext_subset.begin(), ext_subset.end(), ext) == ext_subset.end()) continue;
		if (ext->opcodes.size() == 0) continue;
		if (rows.size() != 0) rows.push_back(riscv_table_row{ riscv_extension_ptr(), riscv_opcode_ptr() });
		rows.push_back(riscv_table_row{ ext, riscv_opcode_ptr() });
		for (auto &opcode : ext->opcodes) {
			rows.push_back(riscv_table_row{ riscv_extension_ptr(), opcode });
		}
	}

	// create the table width specification
	std::stringstream ts;
	ts << "{";
	for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) ts << "p{0.02in}";
	ts << "p{2.0in}l}";
	for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) ts << "& ";
	ts << "& \\\\\n";

	// print document header
	printf("%s", kLatexDocumentBegin);

	// iterate through the table rows
	size_t line = 0;
	for (auto &row : rows) {

		auto &extension = row.extension;
		auto &opcode = row.opcode;

		// print table header and page breaks
		if (line % 64 == 0) {
			if (line != 0) {
				printf("%s", kLatexTableEnd);
			}
			printf("%s%s", kLatexTableBegin, ts.str().c_str());
			if (opcode) {
				printf("\\cline{1-32}\n");
			}
		}

		// format an opcode line
		if (opcode) {

			// calculate the column spans for this row
			riscv_arg_ptr arg, larg;
			typedef std::tuple<riscv_opcode_ptr,riscv_arg_ptr,ssize_t,std::string> arg_tuple;
			std::vector<arg_tuple> arg_parts;
			for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
				char c = ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : '?');
				arg = opcode->find_arg(bit);
				if (arg_parts.size() == 0 || std::get<1>(arg_parts.back()) != arg) {
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
				larg = arg;
			}

			// update labels for segments with args
			ssize_t msb = INSN_WIDTH-1;
			for (size_t i = 0; i < arg_parts.size(); i++) {
				auto &arg = std::get<1>(arg_parts[i]);
				auto size = std::get<2>(arg_parts[i]);
				auto &str = std::get<3>(arg_parts[i]);
				if (arg) {
					str = arg->label;
					if (str == "imm") {
						auto spec = arg->bitrange_spec;
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
				auto &opcode = std::get<0>(arg_parts[i]);
				auto size = std::get<2>(arg_parts[i]);
				auto &str = std::get<3>(arg_parts[i]);
				bool insn16 = (opcode->extensions[0]->insn_width == INSN_WIDTH/2);
				if (i == 0 && insn16) continue;
				ls << (i != (insn16 ? 1 : 0) ? " & " : "")
				   << "\\multicolumn{" << (size * (insn16 ? 2 : 1)) << "}"
				   << "{" << (i == (insn16 ? 1 : 0) ? "|" : "") << "c|}"
				   << "{" << str << "}";
			}

			// format the opcode name and arguments
			auto name = opcode->name;
			auto format = formats_by_name[opcode->type->format];
			auto arg_comps = split(format->args, ",", false, false);
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);

			// print this row
			printf("%s & %s %s \\\\\n\\cline{1-32}\n",
				ls.str().c_str(), name.c_str(), join(arg_comps, ", ").c_str());
		}

		// format an extension heading
		else if (extension) {
			printf("\\multicolumn{32}{c}{\\bf %s} & \\\\\n\\cline{1-32}\n",
				extension->description.c_str());
		}

		// format an empty line
		else {
			printf("\\multicolumn{32}{c}{} & \\\\\n");
		}

		line++;
	}

	// print table and document trailer
	printf("%s", kLatexTableEnd);
	printf("%s", kLatexDocumentEnd);
}

void riscv_inst_set::print_map()
{
	bool enable_colorize = enable_color && isatty(fileno(stdout));

	int i = 0;
	for (auto &opcode : opcodes) {
		if (i % 22 == 0) {
			printf("// %s", enable_colorize ? _COLOR_LEGEND_BEGIN : "");
			for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
				char c = (bit / 10) + '0';
				printf("%c", c);
			}
			printf("%s\n", enable_colorize ? _COLOR_RESET : "");
			printf("// %s", enable_colorize ? _COLOR_LEGEND_BEGIN : "");
			for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
				char c = (bit % 10) + '0';
				printf("%c", c);
			}
			printf("%s\n", enable_colorize ? _COLOR_RESET : "");
		}
		if (!opcode->match_extension(ext_subset)) continue;
		i++;
		printf("// ");
		for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
			char c = ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : '.');
			switch (c) {
				case '0':
				case '1':
				{
					printf("%s%c%s", enable_colorize ? _COLOR_BITS_BEGIN : "", c, enable_colorize ? _COLOR_RESET : "");
					break;
				}
				default:
				{
					riscv_arg_ptr arg = opcode->find_arg(bit);
					if (arg) {
						printf("%s%c%s",
							enable_colorize ? riscv_colors_to_ansi_escape_sequence(arg->fg_color, arg->bg_color).c_str() : "",
							arg->char_code(), enable_colorize ? _COLOR_RESET : "");
					} else {
						printf("%c", c);
					}
					break;
				}
			}
		}
		auto format = formats_by_name[opcode->type->format];
		if (enable_colorize) {
			printf(" %s%s%s %s",
				_COLOR_OPCODE_BEGIN, opcode->name.c_str(), _COLOR_RESET, colorize_args(opcode).c_str());
		} else {
			printf(" %s %s",
				opcode->name.c_str(), format->args.c_str());
		}
		ssize_t len = 34 - (opcode->name.length() + format->args.length());
		std::string ws;
		for (ssize_t i = 0; i < len; i++) ws += ' ';
		printf("%s%s# %s%s\n",
			ws.c_str(),
			enable_colorize ? _COLOR_EXT_BEGIN : "",
			opcode->extensions.front()->name.c_str(),
			enable_colorize ? _COLOR_RESET : "");
	}
}

void riscv_inst_set::print_opcodes_h()
{
	printf("//\n");
	printf("//  riscv-opcodes.h\n");
	printf("//\n");
	printf("//  DANGER - This is machine generated code\n");
	printf("//\n");
	printf("\n");
	printf("#ifndef riscv_opcodes_h\n");
	printf("#define riscv_opcodes_h\n");
	printf("\n");
	printf("enum rvc_constraint\n{\n");
	printf("\trvc_end,\n");
	for (auto &constraint : constraints) {
		printf("\trvc_%s,\n", constraint->name.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_csr\n{\n");
	for (auto &csr : csrs) {
		printf("\triscv_csr_%s = %s,\n", csr->name.c_str(), csr->number.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_ireg_name\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,\n", reg->name.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_ireg_alias\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,\n", reg->alias.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_freg_name\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,\n", reg->name.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_freg_alias\n{\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,\n", reg->alias.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_inst_type\n{\n");
	printf("\triscv_inst_type_unknown,\n");
	for (auto &type : get_unique_types()) {
		printf("\triscv_inst_type_%s,\n", type.c_str());
	}
	printf("};\n\n");
	printf("enum riscv_op\n{\n");
	printf("\triscv_op_unknown,\n");
	for (auto &opcode : opcodes) {
		printf("\t%s,\n", opcode_format("riscv_op_", opcode, '_').c_str());
	}
	printf("};\n\n");
	printf("extern const char* riscv_instruction_name[];\n");
	printf("extern const riscv_inst_type riscv_instruction_type[];\n");
	printf("extern const riscv_wu riscv_instruction_match[];\n");
	printf("extern const riscv_wu riscv_instruction_mask[];\n");
	printf("extern const rvf* riscv_instruction_format[];\n");
	printf("extern const char* riscv_i_registers[];\n");
	printf("extern const char* riscv_f_registers[];\n");
	printf("extern const rvc_constraint** riscv_instruction_comp[];\n");
	printf("extern const riscv_op riscv_instruction_decomp[];\n");
	printf("\n");
	printf("#endif\n");
}

void riscv_inst_set::print_opcodes_c()
{
	printf("//\n");
	printf("//  riscv-opcodes.cc\n");
	printf("//\n");
	printf("//  DANGER - This is machine generated code\n");
	printf("//\n");
	printf("\n");
	printf("#include \"riscv-types.h\"\n");
	printf("#include \"riscv-format.h\"\n");
	printf("#include \"riscv-opcodes.h\"\n");
	printf("\n");
	printf("const char* riscv_instruction_name[] = {\n");
	printf("\t/*              unknown */ \"unknown\",\n");
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		std::string opcode_name = opcode_format("", opcode, '.', false);
		printf("\t/* %20s */ \"%s\",\n", opcode_key.c_str(), opcode_name.c_str());
	}
	printf("};\n\n");
	printf("const riscv_inst_type riscv_instruction_type[] = {\n");
	printf("\t/*              unknown */ riscv_inst_type_unknown,\n");
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		std::string type_name = split(opcode->type->name, "+", false, false)[0];
		printf("\t/* %20s */ riscv_inst_type_%s,\n", opcode_key.c_str(), type_name.c_str());
	}
	printf("};\n\n");
	printf("const riscv_wu riscv_instruction_match[] = {\n");
	printf("\t/*              unknown */ 0x%08x,\n", 0);
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		printf("\t/* %20s */ 0x%08x,\n", opcode_key.c_str(), (uint32_t)opcode->match);
	}
	printf("};\n\n");
	printf("const riscv_wu riscv_instruction_mask[] = {\n");
	printf("\t/*              unknown */ 0x%08x,\n", 0);
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		printf("\t/* %20s */ 0x%08x,\n", opcode_key.c_str(), (uint32_t)opcode->mask);
	}
	printf("};\n\n");
	printf("const rvf* riscv_instruction_format[] = {\n");
	printf("\t/*              unknown */ riscv_fmt_none,\n");
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		auto format = formats_by_name[opcode->type->format];
		printf("\t/* %20s */ riscv_fmt_%s,\n", opcode_key.c_str(), format->name.c_str());
	}
	printf("};\n\n");
	printf("const char* riscv_i_registers[] = {\n");
	for (auto &reg : registers) {
		if (reg->type != "ireg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");
	printf("const char* riscv_f_registers[] = {\n");
	for (auto &reg : registers) {
		if (reg->type != "freg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// compression meta data
	for (auto &opcode : opcodes) {
		if (!opcode->compressed) continue;
		std::string cop_name = "rvc_" + opcode_format("", opcode, '_') + "[] =";
		printf("const rvc_constraint %-30s { ", cop_name.c_str());
		for (auto &constraint : opcode->compressed->constraint_list) {
			printf("rvc_%s, ", constraint->name.c_str());
		}
		printf("rvc_end };\n");
	}
	printf("\n");
	for (auto &opcode : opcodes) {
		if (opcode->compressions.size() == 0) continue;
		std::string op_name = "rvcl_" + opcode_format("", opcode, '_') + "[] =";
		printf("const rvc_constraint* %-30s { ", op_name.c_str());
		for (auto comp : opcode->compressions) {
			printf("rvc_%s, ", opcode_format("", comp->copcode, '_').c_str());
		}
		printf("nullptr };\n");
	}
	printf("\n");
	printf("const rvc_constraint** riscv_instruction_comp[] = {\n");
	printf("\t/*              unknown */ nullptr,\n");
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		printf("\t/* %20s */ %s,\n", opcode_key.c_str(),
			opcode->compressions.size() > 0 ? opcode_format("rvcl_", opcode, '_').c_str() : "nullptr");
	}
	printf("};\n\n");
	printf("const riscv_op riscv_instruction_decomp[] = {\n");
	printf("\t/*              unknown */ riscv_op_unknown,\n");
	for (auto &opcode : opcodes) {
		std::string opcode_key = opcode_format("", opcode, '.');
		printf("\t/* %20s */ %s,\n", opcode_key.c_str(),
			opcode->compressed ? opcode_format("riscv_op_", opcode->compressed->opcode, '_').c_str() : "riscv_op_unknown");
	}
	printf("};\n\n");
}

void riscv_inst_set::print_switch_c()
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
	print_switch_decoder_node(node, 1);
	printf("}\n\n");

	// print type decoder
	printf("void riscv_decode_type(riscv_decode &dec, riscv_lu inst)\n");
	printf("{\n");
	printf("\tdec.type = riscv_instruction_type[dec.op];\n");
	printf("\tswitch (dec.type) {\n");
	for (auto &type : get_unique_types()) {
		printf("\t\tcase %-30s %-40s break;\n",
			format_string("riscv_inst_type_%s:", type.c_str()).c_str(),
			format_string("riscv_decode_%s(dec, inst);", type.c_str()).c_str());
	}
	printf("\t};\n");
	printf("}\n");
}

void riscv_inst_set::generate_decoder_node(riscv_decoder_node &node, riscv_opcode_list &opcode_list)
{
	// calculate row coverage for each column
	std::vector<ssize_t> sum;
	sum.resize(32);
	for (auto &opcode : opcode_list) {
		for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
			if ((opcode->mask & (1 << bit)) && !(opcode->done & (1 << bit))) sum[bit]++;
		}
	}

	// find column with maximum row coverage
	ssize_t max_rows = 0;
	for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
		if (sum[bit] > max_rows) max_rows = sum[bit];
	}

	if (max_rows == 0) return; // no bits to match

	// select bits that cover maximum number of rows
	for (ssize_t bit = INSN_WIDTH-1; bit >= 0; bit--) {
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
				std::pair<ssize_t,riscv_decoder_node>(val, riscv_decoder_node()));
		}
		generate_decoder_node(node.val_decodes[val], node.val_opcodes[val]);
	}
}

void riscv_inst_set::print_switch_decoder_node(riscv_decoder_node &node, size_t indent)
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
					if (std::find(opcode_widths.begin(), opcode_widths.end(), ext->isa_width) == opcode_widths.end()) {
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
	riscv_inst_set inst_set;

	bool print_latex = false;
	bool print_map = false;
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
		inst_set.print_opcodes_h();
	}

	if (print_opcodes_c) {
		inst_set.print_opcodes_c();
	}

	if (print_switch_c) {
		inst_set.generate_decoder();
		inst_set.print_switch_c();
	}

	exit(0);
}
