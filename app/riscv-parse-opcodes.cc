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


struct riscv_opcode;
struct riscv_tag;
struct riscv_bitrange;
struct riscv_decoder_node;
struct riscv_dsm_entry;

typedef std::pair<riscv_bitrange,size_t> riscv_opcode_mask;
typedef std::vector<riscv_opcode_mask> riscv_opcode_mask_list;
typedef std::shared_ptr<riscv_opcode> riscv_opcode_ptr;
typedef std::shared_ptr<riscv_tag> riscv_tag_ptr;
typedef std::vector<riscv_opcode_ptr> riscv_opcode_list;
typedef std::vector<riscv_tag_ptr> riscv_tag_list;
typedef std::map<std::string,riscv_opcode_ptr> riscv_opcode_map;
typedef std::map<std::string,riscv_tag_ptr> riscv_tag_map;
typedef std::set<riscv_opcode_ptr> riscv_opcode_set;
typedef std::set<riscv_tag_ptr> riscv_tag_set;
typedef std::vector<riscv_dsm_entry> riscv_dsm_entry_list;

struct riscv_opcode
{
	ssize_t num;
	std::string name;
	riscv_tag_list tags;
	riscv_opcode_mask_list masks;
	std::string isa_class;
	std::vector<std::string> isa_extensions;

	size_t mask;
	size_t match;
	size_t done;

	riscv_opcode(std::string name) : name(name), mask(0), match(0), done(0) {}

	bool match_isa(std::set<std::string> &isa_subset) {
		if (isa_subset.size() == 0) return true;
		for (auto isa : isa_extensions) {
			if (isa_subset.find(isa) != isa_subset.end()) return true;
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

struct riscv_bitrange 
{
	ssize_t msb;
	ssize_t lsb;

	riscv_bitrange(ssize_t msb, ssize_t lsb) : msb(msb), lsb(lsb) {}
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

	riscv_opcode_list opcodes;
	riscv_opcode_map opcodes_bykey;
	riscv_tag_list tags;
	riscv_tag_map tags_byname;
	riscv_decoder_node node;
	std::set<std::string> isa_subset;

	static bool is_mask(std::string tag);
	static bool is_class(std::string tag);
	static bool is_extension(std::string tag);

	static std::vector<std::string> decode_isa_extensions(std::string isa_spec);

	static riscv_opcode_mask decode_mask(std::string bit_spec);
	static std::string decode_class(std::string mc_spec);

	static std::string opcode_spec(riscv_opcode_ptr opcode);
	static std::string opcode_mask(riscv_opcode_ptr opcode);
	static std::string opcode_name(std::string prefix, riscv_opcode_ptr opcode, char dot);

	static std::vector<riscv_bitrange> bitmask_to_bitrange(std::vector<ssize_t> &bits);
	static std::string format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment);

	riscv_opcode_ptr lookup_opcode(std::string opcode_key, std::string opcode_name);
	riscv_tag_ptr lookup_tag(std::string tag);

	void parse_opcode(std::vector<std::string> &part);
	bool read_opcodes(std::string filename);

	void generate_map();
	void generate_decoder();

	void print_map();
	void print_enum();
	void print_class();
	void print_strings();
	void print_switch();
	void print_dsm();

	void generate_decoder_node(riscv_decoder_node &node, riscv_opcode_list &opcode_list);
	void print_switch_decoder_node(riscv_decoder_node &node, size_t indent);
	void calc_dsm_decoder_node(riscv_decoder_node &node, riscv_dsm_entry_list &decoder);
};

std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::vector<std::string> split(std::string str, std::string separator,
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

bool riscv_inst_set::is_mask(std::string tag)
{
	return ((tag.find("=") != std::string::npos) &&
		(tag.find("=ignore") == std::string::npos) &&
		(tag.find("c=") == std::string::npos));
}

bool riscv_inst_set::is_class(std::string tag)
{
	return (tag.find("c=") == 0);
}

bool riscv_inst_set::is_extension(std::string tag)
{
	return (tag.find("rv") == 0);
}

std::vector<std::string> riscv_inst_set::decode_isa_extensions(std::string isa_spec)
{
	std::vector<std::string> list;
	std::transform(isa_spec.begin(), isa_spec.end(),isa_spec.begin(), ::tolower);
	size_t ext_offset = 0;
	if (isa_spec.find("rv32") == 0) {
		ext_offset = 4;
	} else if (isa_spec.find("rv64") == 0) {
		ext_offset = 4;
	} else if (isa_spec.find("rv128") == 0) {
		ext_offset = 5;
	} else {
		panic("illegal isa spec: %s", isa_spec.c_str());
	}
	size_t g_offset = isa_spec.find("g");
	if (g_offset != std::string::npos) {
		isa_spec = isa_spec.replace(isa_spec.begin() + g_offset, isa_spec.begin() + g_offset + 1, "imafd");
	}
	for (auto i = isa_spec.begin() + ext_offset; i != isa_spec.end(); i++) {
		std::stringstream ss;
		ss << isa_spec.substr(0, ext_offset) << *i;
		if (std::find(list.begin(), list.end(), ss.str()) != list.end()) {
			panic("illegal isa spec: %s: duplicate symbol: %c", isa_spec.c_str(), *i);
		}
		list.push_back(ss.str());
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

std::string riscv_inst_set::decode_class(std::string mc_spec)
{
	std::vector<std::string> spart = split(mc_spec, "=", false, false);
	if (spart.size() != 2) {
		printf("class%s must be in form c=value\n", mc_spec.c_str());
		exit(1);
	}
	return spart[1];
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
	std::string name = opcode->name;
	if (name.find("@") == 0) name = name.substr(1);
	std::replace(name.begin(), name.end(), '.', dot);
	return prefix + name;
}

riscv_opcode_ptr riscv_inst_set::lookup_opcode(std::string opcode_key, std::string opcode_name)
{
	auto i = opcodes_bykey.find(opcode_key);
	if (i != opcodes_bykey.end()) return i->second;
	riscv_opcode_ptr p = opcodes_bykey[opcode_key] = std::make_shared<riscv_opcode>(opcode_name);
	p->num = opcodes.size();
	opcodes.push_back(p);
	return p;
}

riscv_tag_ptr riscv_inst_set::lookup_tag(std::string tag)
{
	auto i = tags_byname.find(tag);
	if (i != tags_byname.end()) return i->second;
	riscv_tag_ptr p = tags_byname[tag] = std::make_shared<riscv_tag>(tag);
	tags.push_back(p);
	return p;
}

void riscv_inst_set::parse_opcode(std::vector<std::string> &part)
{
	std::vector<std::string> isa_extensions;
	for (size_t i = 1; i < part.size(); i++) {
		if (is_extension(part[i])) {
			isa_extensions.push_back(part[i]);
		}
	}

	std::string opcode_name = part[0];
	std::string opcode_key = isa_extensions.size() == 1 ?
		opcode_name + std::string(".") + isa_extensions.front() : opcode_name;

	auto opcode = lookup_opcode(opcode_key, opcode_name);

	for (size_t i = 1; i < part.size(); i++) {
		opcode->tags.push_back(lookup_tag(part[i]));
		if (is_mask(part[i])) {
			opcode->masks.push_back(decode_mask(part[i]));
		} else if (is_class(part[i])) {
			opcode->isa_class = decode_class(part[i]);
		} else if (is_extension(part[i])) {
			opcode->isa_extensions.push_back(part[i]);
		}
	}
}

bool riscv_inst_set::read_opcodes(std::string filename)
{
	std::string line;
	std::ifstream in(filename.c_str());
	if (!in.is_open()) {
		printf("error opening %s\n", filename.c_str());
		return false;
	}
	while (in.good())
	{
		std::getline(in, line);
		size_t hoffset = line.find("#");
		if (hoffset != std::string::npos) {
			line = ltrim(rtrim(line.substr(0, hoffset)));
		} 
		std::vector<std::string> part = split(line, " ", false, false);
		if (part.size() == 0) continue;
		parse_opcode(part);
	}
	in.close();
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
		if (!opcode->match_isa(isa_subset)) continue;
		printf("// %-16s", opcode->name.c_str());
		for (ssize_t bit = 31; bit >= 0; bit--) {
			printf("%c", ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : ((opcode->done & (1 << bit)) ? 'X' : '?')) );
		}
		printf("\n");
	}
}

void riscv_inst_set::print_enum()
{
	printf("enum riscv_op {\n");
	for (auto &opcode : opcodes) {
		printf("\t%s,\n", opcode_name("riscv_op_", opcode, '_').c_str());
	}
	printf("};\n");
}

void riscv_inst_set::print_class()
{
	printf("{\n");
	for (auto &opcode : opcodes) {
		printf("\t{ %-20s%-30s },\n",
			format_string("%s,", opcode_name("riscv_op_", opcode, '_').c_str()).c_str(),
			format_string("riscv_inst_type_%s, ", opcode->isa_class.c_str()).c_str());
	}
	printf("};\n");
}

void riscv_inst_set::print_strings()
{
	printf("const char* riscv_instructions[] = {\n");
	for (auto &opcode : opcodes) {
		printf("\t\"%s\",\n", opcode_name("", opcode, '.').c_str());
	}
	printf("};\n");
}

void riscv_inst_set::print_switch()
{
	// template <bool RV32, bool RV64, bool RVI, bool RVM, bool RVA, bool RVF, bool RVD, bool RVS, bool RVC>
	print_switch_decoder_node(node, 0);
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
		if (node.val_decodes[val].bits.size() == 0 && node.val_opcodes[val].size() >= 1) {
			for (size_t i = 0; i < indent; i++) printf("\t");
			if (val == DEFAULT) {
				printf("\tdefault: ");
			} else {
				printf("\tcase %lu: ", val);
			}
			// if ambiguous (@pseudo opcode), chooses first opcode
			auto opcode = node.val_opcodes[val].front();
			printf("dec.op = %s; break;", opcode_name("riscv_op_", opcode, '_').c_str());
			// if ambiguous, add comment
			if (node.val_opcodes[val].size() > 1) {
				printf(" //");
				for (auto &opcode : node.val_opcodes[val]) {
					printf(" %s", opcode->name.c_str());
				}
			}
			printf("\n");
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
			for (auto &opcode : node.val_opcodes[val]) {
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
				// if ambiguous (@pseudo opcode), chooses first opcode
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
				// if ambiguous (@pseudo opcode), chooses first opcode
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
				// if ambiguous (@pseudo opcode), chooses first opcode
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
	bool print_strings = false;
	bool help_or_error = false;

	cmdline_option options[] =
	{
		{ "-I", "--isa-subset", cmdline_arg_type_string,
			"ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)",
			[&](std::string s) {
				for (std::string isa : riscv_inst_set::decode_isa_extensions(s))
					inst_set.isa_subset.insert(isa);
				return true; }
			},
		{ "-r", "--read-opcodes", cmdline_arg_type_string,
			"Read opcodes",
			[&](std::string s) { return inst_set.read_opcodes(s); } },
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
		{ "-i", "--print-strings", cmdline_arg_type_none,
			"Print strings",
			[&](std::string s) { return (print_strings = true); } },
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
		!print_enum && !print_strings &&
		!print_dsm && !print_class;

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

	if (print_strings) {
		inst_set.print_strings();
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
