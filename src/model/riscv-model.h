//
//  riscv-model.h
//

#ifndef riscv_model_h
#define riscv_model_h

struct riscv_bitrange;
struct riscv_bitspec;
struct riscv_operand;
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
struct riscv_pseudo;
struct riscv_bitrange;
struct riscv_latex_row;

typedef std::pair<riscv_bitspec,std::string> riscv_named_bitspec;
typedef std::shared_ptr<riscv_operand> riscv_operand_ptr;
typedef std::vector<riscv_operand_ptr> riscv_operand_list;
typedef std::map<std::string,riscv_operand_ptr> riscv_operand_map;
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
typedef std::shared_ptr<riscv_pseudo> riscv_pseudo_ptr;
typedef std::map<std::string,riscv_pseudo_ptr> riscv_pseudo_map;
typedef std::vector<riscv_pseudo_ptr> riscv_pseudo_list;

int64_t riscv_parse_value(const char* valstr);

enum rvt
{
	rvt_sx,
	rvt_ux,
	rvt_s8,
	rvt_u8,
	rvt_s16,
	rvt_u16,
	rvt_s32,
	rvt_u32,
	rvt_s64,
	rvt_u64,
	rvt_f32,
	rvt_f64,
	rvt_none,
};

struct riscv_primitive_type
{
	rvt enum_type;
	const char* spec_type;
	const char* meta_type;
	const char* asm_type;
	const char* c_fmt;
	const char* c_suffix;
	const char* c_type;
};

extern const riscv_primitive_type riscv_primitive_type_table[];
extern const riscv_primitive_type* riscv_lookup_primitive_by_spec_type(std::string spec_type, rvt default_type = rvt_sx);
extern const riscv_primitive_type* riscv_lookup_primitive_by_meta_type(std::string meta_type, rvt default_type = rvt_sx);

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
	size_t decoded_msb();
	std::string to_string();
	std::string to_template();
};

struct riscv_operand
{
	std::string name;
	riscv_bitspec bitspec;
	std::string type;
	std::string label;
	std::string fg_color;
	std::string bg_color;

	riscv_operand(std::string name, std::string bitspec, std::string type,
		  std::string label, std::string fg_color, std::string bg_color)
		: name(name), bitspec(bitspec), type(type),
		  label(label), fg_color(fg_color), bg_color(bg_color) {}

	std::string char_code() {
		if (type == "ireg") return "R";
		else if (type == "freg") return "F";
		else if (type == "creg") return "R";
		else if (type == "arg") return "A";
		else if (type == "simm") return "S";
		else if (type == "uimm") return "U";
		else if (type == "offset") return "O";
		else return "?";
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
	std::string codec_key;
	riscv_operand_list operands;

	riscv_codec(std::string name, std::string format) : name(name), format(format) {}
};

struct riscv_extension
{
	std::string name;
	std::string prefix;
	size_t isa_width;
	char alpha_code;
	ssize_t inst_width;
	std::string description;
	riscv_opcode_list opcodes;

	riscv_extension(std::string prefix, std::string isa_width,
		  std::string alpha_code, std::string inst_width, std::string description)
		: name(prefix + isa_width + alpha_code), prefix(prefix),
		  isa_width(strtoull(isa_width.c_str(), NULL, 10)),
		  alpha_code(alpha_code.length() > 0 ? alpha_code[0] : '?'),
		  inst_width(strtoull(inst_width.c_str(), NULL, 10)),
		  description(description) {}
};

struct riscv_format
{
	std::string operands;
	std::string description;

	riscv_format(std::string operands, std::string description)
		: operands(operands), description(description) {}
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
	riscv_operand_list operands;
	riscv_opcode_mask_list masks;
	riscv_codec_ptr codec;
	riscv_format_ptr format;
	riscv_type_ptr type;
	riscv_extension_list extensions;
	riscv_compressed_ptr compressed;
	riscv_compressed_list compressions;
	riscv_pseudo_ptr pseudo;
	riscv_pseudo_list pseudos;

	size_t num;
	uint64_t mask;
	uint64_t match;
	uint64_t done;

	riscv_opcode(std::string key, std::string name)
		: key(key), name(name), num(0), mask(0), match(0), done(0) {}

	bool is_pseudo() { return name.find("@") == 0; }

	bool match_extension(riscv_extension_list &s) {
		if (s.size() == 0) return true;
		for (auto ext : extensions) {
			if (std::find(s.begin(), s.end(), ext) != s.end()) return true;
		}
		return false;
	}

	bool include_isa(size_t width) {
		bool include_isa = false;
		for (auto &ext : extensions) {
			if (ext->isa_width == width) include_isa = true;
		}
		return include_isa;
	}

	riscv_operand_ptr find_operand(ssize_t bit) {
		for (auto operand : operands) {
			if (operand->bitspec.matches_bit(bit)) return operand;
		}
		return riscv_operand_ptr();
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
	std::string hint;

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

struct riscv_pseudo
{
	std::string name;
	riscv_opcode_ptr pseudo_opcode;
	riscv_opcode_ptr real_opcode;
	riscv_format_ptr format;
	riscv_constraint_list constraint_list;

	riscv_pseudo(std::string name, riscv_opcode_ptr pseudo_opcode, riscv_opcode_ptr real_opcode, riscv_format_ptr format, riscv_constraint_list constraint_list)
		: name(name), pseudo_opcode(pseudo_opcode), real_opcode(real_opcode), format(format), constraint_list(constraint_list) {}
};

struct riscv_meta_model
{
	static const ssize_t DEFAULT;

	riscv_operand_list       operands;
	riscv_operand_map        operands_by_name;
	riscv_enum_list          enums;
	riscv_enum_map           enums_by_name;
	riscv_type_list          types;
	riscv_type_map           types_by_name;
	riscv_codec_list         codecs;
	riscv_codec_map          codecs_by_name;
	riscv_codec_map          codecs_by_key;
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
	riscv_pseudo_list        pseudos;
	riscv_pseudo_map         pseudos_by_name;

	static riscv_opcode_mask decode_mask(std::string bit_spec);
	static std::string opcode_mask(riscv_opcode_ptr opcode);
	static std::string format_type(riscv_operand_ptr operand);
	static std::string format_codec(std::string prefix, riscv_codec_ptr codec, std::string dot, bool strip_suffix = true);
	static std::string format_format(std::string prefix, riscv_format_ptr format, char special);
	static std::string opcode_format(std::string prefix, riscv_opcode_ptr opcode, std::string dot, bool use_key = true);
	static std::string opcode_codec_key(riscv_opcode_ptr opcode);
	static std::string opcode_comment(riscv_opcode_ptr opcode, bool no_comment, bool key = true);
	static std::string opcode_isa_shortname(riscv_opcode_ptr opcode);
	static std::string codec_type_name(riscv_codec_ptr codec);
	static std::vector<riscv_bitrange> bitmask_to_bitrange(std::vector<ssize_t> &bits);
	static std::string format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment);
	static std::vector<std::string> parse_line(std::string line);
	static std::vector<std::vector<std::string>> read_file(std::string filename);

	std::vector<std::string> get_unique_codecs();
	std::vector<std::string> get_inst_mnemonics(bool isa_widths, bool isa_extensions);
	std::vector<std::pair<size_t,std::string>> isa_width_prefixes();
	riscv_extension_list decode_isa_extensions(std::string isa_spec);
	riscv_opcode_ptr create_opcode(std::string opcode_name, std::string extension);
	riscv_opcode_ptr lookup_opcode_by_key(std::string opcode_name);
	riscv_opcode_list lookup_opcode_by_name(std::string opcode_name);

	bool is_operand(std::string mnem);
	bool is_ignore(std::string mnem);
	bool is_mask(std::string mnem);
	bool is_codec(std::string mnem);
	bool is_extension(std::string mnem);

	void parse_operand(std::vector<std::string> &part);
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
	void parse_pseudo(std::vector<std::string> &part);
	void parse_instruction(std::vector<std::string> &part);
	void parse_description(std::vector<std::string> &part);

	bool read_metadata(std::string dirname);
};

#endif
