//
//  riscv-model.h
//

#ifndef riscv_model_h
#define riscv_model_h

struct riscv_bitrange;
struct riscv_bitspec;
struct riscv_arg;
struct riscv_enum;
struct riscv_type;
struct riscv_codec;
struct riscv_extension;
struct riscv_format;
struct riscv_register;
struct riscv_csr;
struct riscv_opcode;
struct riscv_constraint;
struct riscv_compressed;
struct riscv_bitrange;
struct riscv_codec_node;
struct riscv_latex_row;

typedef std::pair<riscv_bitspec,std::string> riscv_named_bitspec;
typedef std::shared_ptr<riscv_arg> riscv_arg_ptr;
typedef std::vector<riscv_arg_ptr> riscv_arg_list;
typedef std::map<std::string,riscv_arg_ptr> riscv_arg_map;
typedef std::shared_ptr<riscv_enum> riscv_enum_ptr;
typedef std::vector<riscv_enum_ptr> riscv_enum_list;
typedef std::map<std::string,riscv_enum_ptr> riscv_enum_map;
typedef std::shared_ptr<riscv_type> riscv_type_ptr;
typedef std::vector<riscv_type_ptr> riscv_type_list;
typedef std::map<std::string,riscv_type_ptr> riscv_type_map;
typedef std::shared_ptr<riscv_codec> riscv_codec_ptr;
typedef std::vector<riscv_codec_ptr> riscv_codec_list;
typedef std::map<std::string,riscv_codec_ptr> riscv_codec_map;
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

int64_t riscv_parse_value(const char* valstr);

struct riscv_bitrange
{
	ssize_t msb;
	ssize_t lsb;

	riscv_bitrange() : msb(0), lsb(0) {}
	riscv_bitrange(std::string bitrange);
	riscv_bitrange(ssize_t msb, ssize_t lsb) : msb(msb), lsb(lsb) {}
	riscv_bitrange(const riscv_bitrange &o) : msb(o.msb), lsb(o.lsb) {}

	std::string to_string(std::string sep, bool collapse_single_bit_range = true);
};

struct riscv_bitspec
{
	typedef std::vector<riscv_bitrange> riscv_bitrange_list;
	typedef std::pair<riscv_bitrange,riscv_bitrange_list> riscv_bitseg;

	std::vector<riscv_bitseg> segments;

	riscv_bitspec() : segments() {}
	riscv_bitspec(std::string bitspec);
	riscv_bitspec(const riscv_bitspec &o) : segments(o.segments) {}

	bool matches_bit(ssize_t bit);
	std::string to_string();
	std::string to_template();
};

struct riscv_arg
{
	std::string name;
	riscv_bitspec bitspec;
	std::string type;
	std::string label;
	std::string fg_color;
	std::string bg_color;

	riscv_arg(std::string name, std::string bitspec, std::string type,
		  std::string label, std::string fg_color, std::string bg_color)
		: name(name), bitspec(bitspec), type(type),
		  label(label), fg_color(fg_color), bg_color(bg_color) {}

	char char_code() {
		if (type == "ireg") return 'R';
		else if (type == "freg") return 'R';
		else if (type == "creg") return 'R';
		else if (type == "arg") return 'A';
		else if (type == "simm") return 'I';
		else if (type == "uimm") return 'I';
		else return '?';
	}
};

struct riscv_enum
{
	std::string group;
	std::string name;
	int64_t value;
	std::string description;

	riscv_enum(std::string group, std::string name, std::string value, std::string description)
		: group(group), name(name), value(riscv_parse_value(value.c_str())), description(description) {}
};

struct riscv_type
{
	std::string name;
	std::string description;
	std::vector<riscv_named_bitspec> parts;

	riscv_type(std::string name, std::string description) : name(name), description(description) {}

	riscv_named_bitspec find_named_bitspec(ssize_t bit) {
		for (auto ent : parts) {
			if (ent.first.matches_bit(bit)) return ent;
		}
		return riscv_named_bitspec();
	}
};

struct riscv_codec
{
	std::string name;
	std::string format;

	riscv_codec(std::string name, std::string format) : name(name), format(format) {}
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

	riscv_extension(std::string prefix, std::string isa_width,
		  std::string alpha_code, std::string insn_width, std::string description)
		: name(prefix + isa_width + alpha_code), prefix(prefix),
		  isa_width(strtoull(isa_width.c_str(), NULL, 10)),
		  alpha_code(alpha_code.length() > 0 ? alpha_code[0] : '?'),
		  insn_width(strtoull(insn_width.c_str(), NULL, 10)),
		  description(description) {}
};

struct riscv_format
{
	std::string name;
	std::string args;

	riscv_format(std::string name, std::string args)
		: name(name), args(args) {}
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
	std::string key;
	std::string name;
	std::string long_name;
	std::string instruction;
	std::string description;
	riscv_arg_list args;
	riscv_opcode_mask_list masks;
	riscv_codec_ptr codec;
	riscv_format_ptr format;
	riscv_type_ptr type;
	riscv_extension_list extensions;
	riscv_compressed_ptr compressed;
	riscv_compressed_list compressions;

	size_t num;
	size_t mask;
	size_t match;
	size_t done;

	riscv_opcode(std::string key, std::string name)
		: key(key), name(name), num(0), mask(0), match(0), done(0) {}

	bool match_extension(riscv_extension_list &s) {
		if (s.size() == 0) return true;
		for (auto ext : extensions) {
			if (std::find(s.begin(), s.end(), ext) != s.end()) return true;
		}
		return false;
	}

	riscv_arg_ptr find_arg(ssize_t bit) {
		for (auto arg : args) {
			if (arg->bitspec.matches_bit(bit)) return arg;
		}
		return riscv_arg_ptr();
	}

	riscv_named_bitspec find_named_bitspec(ssize_t bit) {
		for (auto ent : type->parts) {
			if (ent.first.matches_bit(bit)) return ent;
		}
		return riscv_named_bitspec();
	}
};

struct riscv_constraint
{
	std::string name;
	std::string expression;

	riscv_constraint(std::string name, std::string expression)
		: name(name), expression(expression) {}
};

struct riscv_compressed
{
	riscv_opcode_ptr comp_opcode;
	riscv_opcode_ptr decomp_opcode;
	riscv_constraint_list constraint_list;

	riscv_compressed(riscv_opcode_ptr comp_opcode, riscv_opcode_ptr decomp_opcode, riscv_constraint_list constraint_list)
		: comp_opcode(comp_opcode), decomp_opcode(decomp_opcode), constraint_list(constraint_list) {}
};

struct riscv_codec_node
{
	std::vector<ssize_t> bits;
	std::vector<ssize_t> vals;
	std::map<ssize_t,riscv_opcode_list> val_opcodes;
	std::map<ssize_t,riscv_codec_node> val_decodes;

	void clear();
};

struct riscv_meta_model
{
	const ssize_t DEFAULT = std::numeric_limits<ssize_t>::max();

	riscv_arg_list           args;
	riscv_arg_map            args_by_name;
	riscv_enum_list          enums;
	riscv_enum_map           enums_by_name;
	riscv_type_list          types;
	riscv_type_map           types_by_name;
	riscv_codec_list         codecs;
	riscv_codec_map          codecs_by_name;
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
	riscv_constraint_list    constraints;
	riscv_constraint_map     constraints_by_name;
	riscv_compressed_list    compressions;
	riscv_extension_list     ext_subset;
	riscv_codec_node         root_node;
	riscv_opcode_ptr         unknown;

	static riscv_opcode_mask decode_mask(std::string bit_spec);
	static std::string opcode_mask(riscv_opcode_ptr opcode);
	static std::string opcode_format(std::string prefix, riscv_opcode_ptr opcode, char dot, bool key = true);
	static std::string opcode_comment(riscv_opcode_ptr opcode, bool no_comment, bool key = true);
	static std::string opcode_isa_shortname(riscv_opcode_ptr opcode);
	static std::string codec_type_name(riscv_codec_ptr codec);
	static std::vector<riscv_bitrange> bitmask_to_bitrange(std::vector<ssize_t> &bits);
	static std::string format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment);
	static std::vector<std::string> parse_line(std::string line);
	static std::vector<std::vector<std::string>> read_file(std::string filename);

	riscv_extension_list decode_isa_extensions(std::string isa_spec);
	riscv_opcode_ptr create_opcode(std::string opcode_name, std::string extension);
	riscv_opcode_ptr lookup_opcode_by_key(std::string opcode_name);
	riscv_opcode_list lookup_opcode_by_name(std::string opcode_name);

	bool is_arg(std::string mnem);
	bool is_ignore(std::string mnem);
	bool is_mask(std::string mnem);
	bool is_codec(std::string mnem);
	bool is_extension(std::string mnem);

	void parse_arg(std::vector<std::string> &part);
	void parse_enum(std::vector<std::string> &part);
	void parse_type(std::vector<std::string> &part);
	void parse_codec(std::vector<std::string> &part);
	void parse_extension(std::vector<std::string> &part);
	void parse_format(std::vector<std::string> &part);
	void parse_register(std::vector<std::string> &part);
	void parse_csr(std::vector<std::string> &part);
	void parse_opcode(std::vector<std::string> &part);
	void parse_constraint(std::vector<std::string> &part);
	void parse_compression(std::vector<std::string> &part);
	void parse_instruction(std::vector<std::string> &part);
	void parse_description(std::vector<std::string> &part);

	bool read_metadata(std::string dirname);
};

#endif