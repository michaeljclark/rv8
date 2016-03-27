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

#include "riscv-types.h"
#include "riscv-dsm.h"
#include "riscv-opcodes.h"
#include "riscv-util.h"
#include "riscv-cmdline.h"


static std::string isa_prefix                  = "rv";
static std::vector<size_t> isa_width_bits      = { 32, 64, 128 };
static std::vector<std::string> isa_width_str =  { "rv32", "rv64", "rv128" };

static const char* ARGS_FILE           = "args";
static const char* TYPES_FILE          = "types";
static const char* EXTENSIONS_FILE     = "extensions";
static const char* FORMATS_FILE        = "formats";
static const char* REGISTERS_FILE      = "registers";
static const char* CSRS_FILE           = "csrs";
static const char* OPCODES_FILE        = "opcodes";
static const char* DESCRIPTIONS_FILE   = "descriptions";

struct riscv_bitrange;
struct riscv_bitrange_spec;
struct riscv_arg;
struct riscv_type;
struct riscv_extension;
struct riscv_format;
struct riscv_register;
struct riscv_csr;
struct riscv_opcode;
struct riscv_tag;
struct riscv_bitrange;
struct riscv_decoder_node;
struct riscv_dsm_entry;

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
typedef std::map<std::string,riscv_opcode_ptr> riscv_opcode_map;
typedef std::map<std::string,riscv_opcode_list> riscv_opcode_list_map;
typedef std::set<riscv_opcode_ptr> riscv_opcode_set;
typedef std::shared_ptr<riscv_tag> riscv_tag_ptr;
typedef std::vector<riscv_tag_ptr> riscv_tag_list;
typedef std::map<std::string,riscv_tag_ptr> riscv_tag_map;
typedef std::set<riscv_tag_ptr> riscv_tag_set;
typedef std::vector<riscv_dsm_entry> riscv_dsm_entry_list;

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

	std::string to_string();
	std::string to_template();
};

struct riscv_arg
{
	std::string name;
	riscv_bitrange_spec bitrange_spec;
	std::string type;
	std::string label;

	riscv_arg(std::string name, std::string bitrange_spec, std::string type, std::string label)
		: name(name), bitrange_spec(bitrange_spec), type(type), label(label) {}
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
	std::string description;

	riscv_extension(std::string name, std::string description) : name(name), description(description) {}
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
	riscv_tag_list tags;
	riscv_opcode_mask_list masks;
	riscv_type_ptr type;
	riscv_extension_list extensions;

	size_t mask;
	size_t match;
	size_t done;

	riscv_opcode(std::string key, std::string name) : key(key), name(name), mask(0), match(0), done(0) {}

	bool match_extension(std::set<std::string> &extension_subset) {
		if (extension_subset.size() == 0) return true;
		for (auto extension : extensions) {
			if (extension_subset.find(extension->name) != extension_subset.end()) return true;
		}
		return false;
	}
};

struct riscv_tag
{
	std::string name;
	riscv_opcode_set opcodes;

	riscv_tag(std::string name) : name(name) {}
};

struct riscv_decoder_node
{
	std::vector<ssize_t> bits;
	std::vector<ssize_t> vals;
	std::map<ssize_t,riscv_opcode_list> val_opcodes;
	std::map<ssize_t,riscv_decoder_node> val_decodes;
	riscv_opcode_list less_specific_opcodes;
	ssize_t jump_slot;

	riscv_decoder_node() : jump_slot(-1) {}
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
	riscv_tag_list           tags;
	riscv_tag_map            tags_by_name;
	std::set<std::string>    extension_subset;

	riscv_decoder_node node;

	static std::vector<std::string> decode_isa_extensions(std::string isa_spec);
	static riscv_opcode_mask decode_mask(std::string bit_spec);
	static std::string opcode_spec(riscv_opcode_ptr opcode);
	static std::string opcode_mask(riscv_opcode_ptr opcode);
	static std::string opcode_name(std::string prefix, riscv_opcode_ptr opcode, char dot);
	static std::string opcode_isa_extension(riscv_opcode_ptr opcode);
	static std::vector<size_t> opcode_isa_widths(riscv_opcode_ptr opcode);
	static std::vector<riscv_bitrange> bitmask_to_bitrange(std::vector<ssize_t> &bits);
	static std::string format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment);

	std::vector<std::string> parse_line(std::string line);
	std::vector<std::vector<std::string>> read_file(std::string filename);

	riscv_opcode_ptr create_opcode(std::string opcode_name, std::string extension);
	riscv_opcode_ptr lookup_opcode_by_key(std::string opcode_name);
	riscv_opcode_list lookup_opcode_by_name(std::string opcode_name);
	riscv_tag_ptr lookup_tag(std::string tag);

	bool is_mask(std::string tag);
	bool is_type(std::string tag);
	bool is_extension(std::string tag);

	void parse_arg(std::vector<std::string> &part);
	void parse_type(std::vector<std::string> &part);
	void parse_extension(std::vector<std::string> &part);
	void parse_format(std::vector<std::string> &part);
	void parse_register(std::vector<std::string> &part);
	void parse_csr(std::vector<std::string> &part);
	void parse_opcode(std::vector<std::string> &part);
	void parse_description(std::vector<std::string> &part);

	bool read_metadata(std::string dirname);

	void generate_map();
	void generate_decoder();

	void print_map();
	void print_enum();
	void print_class();
	void print_switch_template_header();
	void print_switch();
	void print_dsm();

	void generate_decoder_node(riscv_decoder_node &node, riscv_opcode_list &opcode_list);
	void print_switch_decoder_node(riscv_decoder_node &node, size_t indent);
	void calc_dsm_decoder_node(riscv_decoder_node &node, riscv_dsm_entry_list &decoder);
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

std::vector<std::string> riscv_inst_set::decode_isa_extensions(std::string isa_spec)
{
	std::vector<std::string> list;
	std::transform(isa_spec.begin(), isa_spec.end(), isa_spec.begin(), ::tolower);
	size_t ext_offset = 0;
	for (std::string isa_width : isa_width_str) {
		if (isa_spec.find(isa_width) == 0) {
			ext_offset = isa_width.size();
		}
	}
	if (ext_offset == 0) {
		panic("illegal isa spec: %s", isa_spec.c_str());
	}
	size_t g_offset = isa_spec.find("g");
	if (g_offset != std::string::npos) {
		isa_spec = isa_spec.replace(isa_spec.begin() + g_offset, isa_spec.begin() + g_offset + 1, "imafd");
	}
	for (auto i = isa_spec.begin() + ext_offset; i != isa_spec.end(); i++) {
		std::string extension = isa_spec.substr(0, ext_offset) + *i;
		if (std::find(list.begin(), list.end(), extension) != list.end()) {
			panic("illegal isa spec: %s: duplicate symbol: %c", isa_spec.c_str(), *i);
		}
		list.push_back(extension);
	}
	return list;
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

std::string riscv_inst_set::opcode_spec(riscv_opcode_ptr opcode)
{
	std::stringstream ss;
	ss << std::left << std::setw(20) << opcode->name << " " << join(opcode->tags, " ");
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

std::string riscv_inst_set::opcode_name(std::string prefix, riscv_opcode_ptr opcode, char dot)
{
	std::string name = opcode->key;
	if (name.find("@") == 0) name = name.substr(1);
	std::replace(name.begin(), name.end(), '.', dot);
	return prefix + name;
}

std::string riscv_inst_set::opcode_isa_extension(riscv_opcode_ptr opcode)
{
	for (auto &extension : opcode->extensions) {
		size_t ext_offset = 0;
		for (std::string isa_width : isa_width_str) {
			if (extension->name.find(isa_width) == 0) {
				ext_offset = isa_width.size();
			}
		}
		if (ext_offset == 0) {
			panic("illegal extension name: %s", extension->name.c_str());
		}
		return isa_prefix + extension->name.substr(ext_offset);
	}
	return "unknown";
}

std::vector<size_t> riscv_inst_set::opcode_isa_widths(riscv_opcode_ptr opcode)
{
	std::vector<size_t> widths;
	for (auto &extension : opcode->extensions) {
		for (size_t i = 0; i < isa_width_str.size(); i++) {
			if (extension->name.find(isa_width_str[i]) == 0) {
				widths.push_back(isa_width_bits[i]);
			}
		}
	}
	return widths;
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

riscv_tag_ptr riscv_inst_set::lookup_tag(std::string tag)
{
	auto i = tags_by_name.find(tag);
	if (i != tags_by_name.end()) return i->second;
	riscv_tag_ptr p = tags_by_name[tag] = std::make_shared<riscv_tag>(tag);
	tags.push_back(p);
	return p;
}

bool riscv_inst_set::is_mask(std::string tag)
{
	return ((tag.find("=") != std::string::npos) &&
		(tag.find("=ignore") == std::string::npos));
}

bool riscv_inst_set::is_type(std::string tag)
{
	return (types_by_name.find(tag) != types_by_name.end());
}

bool riscv_inst_set::is_extension(std::string tag)
{
	return (extensions_by_name.find(tag) != extensions_by_name.end());
}

void riscv_inst_set::parse_arg(std::vector<std::string> &part)
{
	if (part.size() < 4) {
		panic("invalid args file requires 4 parameters: %s", join(part, " ").c_str());
	}
	auto arg = args_by_name[part[0]] = std::make_shared<riscv_arg>(part[0], part[1], part[2], part[3]);
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
	if (part.size() < 2) {
		panic("invalid extensions file requires 2 parameters: %s", join(part, " ").c_str());
	}
	auto extension = extensions_by_name[part[0]] = std::make_shared<riscv_extension>(part[0], part[1]);
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
	auto opcode = create_opcode(opcode_name, extensions.front());

	for (size_t i = 1; i < part.size(); i++) {
		std::string mnem = part[i];
		std::transform(mnem.begin(), mnem.end(), mnem.begin(), ::tolower);
		opcode->tags.push_back(lookup_tag(mnem));
		if (is_mask(mnem)) {
			opcode->masks.push_back(decode_mask(mnem));
		} else if (is_type(mnem)) {
			opcode->type = types_by_name[mnem];
		} else if (is_extension(mnem)) {
			opcode->extensions.push_back(extensions_by_name[mnem]);
		}
	}

	if (!opcode->type) {
		panic("opcode has no type: %s", opcode_name.c_str());
	}
	if (opcode->extensions.size() == 0) {
		panic("opcode has no extensions: %s", opcode_name.c_str());
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

void riscv_inst_set::print_map()
{
	for (auto &opcode : opcodes) {
		if (!opcode->match_extension(extension_subset)) continue;
		printf("// %-16s", opcode->name.c_str());
		for (ssize_t bit = 31; bit >= 0; bit--) {
			printf("%c", ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : ((opcode->done & (1 << bit)) ? 'X' : '?')) );
		}
		printf("\n");
	}
}

void riscv_inst_set::print_enum()
{
	printf("enum riscv_inst_type\n{\n");
	for (auto &type : types) {
		printf("\triscv_inst_type_%s,\n", type->name.c_str());
	}
	printf("};\n");
	printf("enum riscv_op\n{\n");
	for (auto &opcode : opcodes) {
		printf("\t%s,\n", opcode_name("riscv_op_", opcode, '_').c_str());
	}
	printf("};\n\n");
}

void riscv_inst_set::print_class()
{
	printf("{\n");
	for (auto &opcode : opcodes) {
		printf("\t{ %-20s%-30s },\n",
			format_string("\"%s\",", opcode_name("", opcode, '.').c_str()).c_str(),
			format_string("riscv_inst_type_%s ", opcode->type->name.c_str()).c_str());
	}
	printf("};\n");
}

void riscv_inst_set::print_switch_template_header()
{
	std::vector<std::string> mnemonics;
	for (auto &extension : extensions) {
		for (std::string isa_width : isa_width_str) {
			if (extension->name.find(isa_width) == 0) {
				if (std::find(mnemonics.begin(), mnemonics.end(), isa_width) == mnemonics.end()) {
					mnemonics.push_back(isa_width);
				}
			}
		}
	}
	for (auto &extension : extensions) {
		size_t ext_offset = 0;
		for (std::string isa_width : isa_width_str) {
			if (extension->name.find(isa_width) == 0) {
				ext_offset = isa_width.size();
			}
		}
		if (ext_offset == 0) {
			panic("illegal isa spec: %s", extension->name.c_str());
		}
		std::string extension_mnem = std::string("rv") + extension->name.substr(ext_offset);
		if (std::find(mnemonics.begin(), mnemonics.end(), extension_mnem) == mnemonics.end()) {
			mnemonics.push_back(extension_mnem);
		}
	}
	printf("template <");
	for (auto mi = mnemonics.begin(); mi != mnemonics.end(); mi++) {
		if (mi != mnemonics.begin()) printf(", ");
		printf("bool %s", mi->c_str());
	}
	printf(">\n");
}

void riscv_inst_set::print_switch()
{
	print_switch_template_header();
	print_switch_decoder_node(node, 0);

	printf("\t switch (dec.type) {\n");
	for (auto &type : types) {
		printf("\t\tcase %-30s %-40s break;\n",
			format_string("riscv_inst_type_%s:", type->name.c_str()).c_str(),
			format_string("riscv_decode_%s(dec, inst);", type->name.c_str()).c_str());
	}
	printf("\t};\n");
}

void riscv_inst_set::print_dsm()
{
	riscv_dsm_entry_list decoder;
	calc_dsm_decoder_node(node, decoder);

	for (size_t i = 0; i < decoder.size(); i++) {
		riscv_dsm_entry &ent = decoder[i];
		printf("/* %-5lu */", i);
		switch (ent.dsm_op) {
			case riscv_dsm_break: printf("{ riscv_dsm_break, %d },\n", ent.dsm_val); break;
			case riscv_dsm_table_brk: printf("{ riscv_dsm_table_brk, %d },\n", ent.dsm_val); break;
			case riscv_dsm_table_dfl: printf("{ riscv_dsm_table_dfl, %d },\n", ent.dsm_val); break;
			case riscv_dsm_match: printf("{ riscv_dsm_match, %d },\n", ent.dsm_val); break;
			case riscv_dsm_jump: printf("{ riscv_dsm_jump, %d },\n", ent.dsm_val); break;
			case riscv_dsm_select: printf("{ riscv_dsm_select, %s },\n", opcode_name("riscv_op_", opcodes[ent.dsm_val], '_').c_str()); break;
			case riscv_dsm_mask_srl0:
			case riscv_dsm_mask_srl1:
			case riscv_dsm_mask_srl2:
			case riscv_dsm_mask_srl3:
			case riscv_dsm_mask_srl4:
			case riscv_dsm_mask_srl5:
			case riscv_dsm_mask_srl6:
			case riscv_dsm_mask_srl7:
			case riscv_dsm_mask_srl8:
			case riscv_dsm_mask_srl9:
			case riscv_dsm_mask_srl10:
			case riscv_dsm_mask_srl11:
			case riscv_dsm_mask_srl12:
			case riscv_dsm_mask_srl13:
			case riscv_dsm_mask_srl14:
			case riscv_dsm_mask_srl15:
			case riscv_dsm_mask_srl16:
			case riscv_dsm_mask_srl17:
			case riscv_dsm_mask_srl18:
			case riscv_dsm_mask_srl19:
			case riscv_dsm_mask_srl20:
			case riscv_dsm_mask_srl21:
			case riscv_dsm_mask_srl22:
			case riscv_dsm_mask_srl23:
			case riscv_dsm_mask_srl24:
			case riscv_dsm_mask_srl25:
			case riscv_dsm_mask_srl26:
			case riscv_dsm_mask_srl27:
			case riscv_dsm_mask_srl28:
			case riscv_dsm_mask_srl29:
			case riscv_dsm_mask_srl30:
			case riscv_dsm_mask_srl31:
				printf("{ riscv_dsm_mask_srl%d, %d },\n", (ent.dsm_op - riscv_dsm_mask_srl0), ent.dsm_val); break;
				break;
		}
	}
}

void riscv_inst_set::generate_decoder_node(riscv_decoder_node &node, riscv_opcode_list &opcode_list)
{
	// calculate row coverage for each column
	std::vector<ssize_t> sum;
	sum.resize(32);
	for (auto &opcode : opcode_list) {
		for (ssize_t bit = 31; bit >= 0; bit--) {
			if ((opcode->mask & (1 << bit)) && !(opcode->done & (1 << bit))) sum[bit]++;
		}
	}

	// find column with maximum row coverage
	ssize_t max_rows = 0;
	for (ssize_t bit = 31; bit >= 0; bit--) {
		if (sum[bit] > max_rows) max_rows = sum[bit];
	}

	if (max_rows == 0) return; // no bits to match

	// select bits that cover maximum number of rows
	for (ssize_t bit = 31; bit >= 0; bit--) {
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
				for (size_t width : opcode_isa_widths(opcode)) {
					opcode_widths.push_back(width);
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
						opcode_isa_extension(opcode).c_str(), opcode_isa_widths(opcode)[0],
						opcode_name("riscv_op_", opcode, '_').c_str());
				}
				for (size_t i = 0; i < indent; i++) printf("\t");
				printf("\t\tbreak;\n");
			}
			else
			{
				// if ambiguous, chooses first opcode
				if (opcode_widths.size() == 1) {
					printf("if (%s && rv%lu) dec.op = %s; break;",
						opcode_isa_extension(opcode).c_str(), opcode_isa_widths(opcode)[0],
						opcode_name("riscv_op_", opcode, '_').c_str());
				} else {
					printf("if (%s) dec.op = %s; break;",
						opcode_isa_extension(opcode).c_str(),
						opcode_name("riscv_op_", opcode, '_').c_str());
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

void riscv_inst_set::calc_dsm_decoder_node(riscv_decoder_node &node, riscv_dsm_entry_list &decoder)
{
	if (node.jump_slot != -1) {
		decoder[node.jump_slot] = riscv_dsm_entry(riscv_dsm_jump, decoder.size() - node.jump_slot);
	}

	ssize_t range_start = node.bits.size();
	for (riscv_bitrange &r : bitmask_to_bitrange(node.bits)) {
		ssize_t shift = r.msb - range_start + 1;
		ssize_t val = 0;
		for (ssize_t i = range_start; i > 0; i--) {
			val = (val << 1) | (i >= (range_start - (r.msb - r.lsb)) ? 1 : 0);
		}
		range_start -= (r.msb - r.lsb) + 1;
		decoder.push_back(riscv_dsm_entry(riscv_dsm_mask_srl0 + shift, val));
	}

	bool has_default = false;
	ssize_t max_val = 0;
	for (auto &val : node.vals) {
		if (val == DEFAULT) {
			has_default = true;
		} else {
			max_val = val;
		}
	}

	if (max_val < 512) {
		ssize_t table_start = decoder.size() + 1;
		ssize_t table_size = max_val + (has_default ? 2 : 1);
		decoder.push_back(riscv_dsm_entry(has_default ? riscv_dsm_table_dfl : riscv_dsm_table_brk, table_size));
		decoder.resize(decoder.size() + table_size);
		if (has_default) {
			if (node.val_decodes[DEFAULT].bits.size() == 0 &&
				node.val_opcodes[DEFAULT].size() == 1)
			{
				decoder[table_start + table_size - 1] = riscv_dsm_entry(riscv_dsm_select, node.val_opcodes[DEFAULT].front()->num);
			} else {
				decoder[table_start + table_size - 1] = riscv_dsm_entry(riscv_dsm_jump, -1);
				node.val_decodes[DEFAULT].jump_slot = table_start + table_size - 1;
			}
			for (ssize_t i = 0; i < table_size - 1; i++) {
				decoder[table_start + i] = riscv_dsm_entry(riscv_dsm_jump, table_start + table_size - 1);
			}
		}
		for (auto &val : node.vals) {
			if (val == DEFAULT) continue;
			if (node.val_decodes[val].bits.size() == 0 && node.val_opcodes[val].size() >= 1) {
				// if ambiguous, chooses first opcode
				decoder[table_start + val] = riscv_dsm_entry(riscv_dsm_select, node.val_opcodes[val].front()->num);
			} else {
				decoder[table_start + val] = riscv_dsm_entry(riscv_dsm_jump, -1);
				node.val_decodes[val].jump_slot = table_start + val;
			}
		}
		for (auto &val : node.vals) {
			if (node.val_decodes[val].bits.size() > 0) {
				calc_dsm_decoder_node(node.val_decodes[val], decoder);
			}
		}
	} else {
		for (auto &val : node.vals) {
			if (val == DEFAULT) continue;
			if (node.val_decodes[val].bits.size() == 0 && node.val_opcodes[val].size() >= 1) {
				// if ambiguous, chooses first opcode
				decoder.push_back(riscv_dsm_entry(riscv_dsm_match, val));
				decoder.push_back(riscv_dsm_entry(riscv_dsm_select, node.val_opcodes[val].front()->num));
			} else {
				decoder.push_back(riscv_dsm_entry(riscv_dsm_match, val));
				decoder.push_back(riscv_dsm_entry(riscv_dsm_jump, -1));
				node.val_decodes[val].jump_slot = decoder.size() - 1;
			}
		}
		if (has_default) {
			if (node.val_decodes[DEFAULT].bits.size() == 0 && node.val_opcodes[DEFAULT].size() >= 1) {
				// if ambiguous, chooses first opcode
				decoder.push_back(riscv_dsm_entry(riscv_dsm_select, node.val_opcodes[DEFAULT].front()->num));
			} else {
				decoder.push_back(riscv_dsm_entry(riscv_dsm_jump, -1));
				node.val_decodes[DEFAULT].jump_slot = decoder.size() - 1;
			}
		} else {
			decoder.push_back(riscv_dsm_entry(riscv_dsm_break));
		}
		for (auto &val : node.vals) {
			if (node.val_decodes[val].bits.size() > 0) {
				calc_dsm_decoder_node(node.val_decodes[val], decoder);
			}
		}
	}
}


/* main */

int main(int argc, const char *argv[])
{
	riscv_inst_set inst_set;

	bool print_map = false;
	bool print_switch = false;
	bool print_dsm = false;
	bool print_enum = false;
	bool print_class = false;
	bool help_or_error = false;

	cmdline_option options[] =
	{
		{ "-I", "--isa-subset", cmdline_arg_type_string,
			"ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)",
			[&](std::string s) {
				for (std::string isa : riscv_inst_set::decode_isa_extensions(s))
					inst_set.extension_subset.insert(isa);
				return true; }
			},
		{ "-r", "--read-isa", cmdline_arg_type_string,
			"Read instruction set metadata from directory",
			[&](std::string s) { return inst_set.read_metadata(s); } },
		{ "-m", "--print-map", cmdline_arg_type_none,
			"Print map",
			[&](std::string s) { return (print_map = true); } },
		{ "-s", "--print-switch", cmdline_arg_type_none,
			"Print switch",
			[&](std::string s) { return (print_switch = true); } },
		{ "-d", "--print-dsm", cmdline_arg_type_none,
			"Print decoder state machine",
			[&](std::string s) { return (print_dsm = true); } },
		{ "-e", "--print-enum", cmdline_arg_type_none,
			"Print enum",
			[&](std::string s) { return (print_enum = true); } },
		{ "-c", "--print-class", cmdline_arg_type_none,
			"Print instruction classes",
			[&](std::string s) { return (print_class = true); } },
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

	help_or_error |= !print_map && !print_switch &&
		!print_enum && !print_dsm && !print_class;

	if (help_or_error) {
		printf("usage: %s\n", argv[0]);
		cmdline_option::print_options(options);
		return false;
	}

	inst_set.generate_map();

	if (print_map) {
		inst_set.print_map();
	}

	if (print_enum) {
		inst_set.print_enum();
	}

	if (print_class) {
		inst_set.print_class();
	}

	if (print_switch) {
		inst_set.generate_decoder();
		inst_set.print_switch();
	}

	if (print_dsm) {
		inst_set.generate_decoder();
		inst_set.print_dsm();
	}

	exit(0);
}
