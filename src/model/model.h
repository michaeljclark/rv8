//
//  model.h
//

#ifndef rv_model_h
#define rv_model_h

struct rv_bitrange;
struct rv_bitspec;
struct rv_operand;
struct rv_enum;
struct rv_type;
struct rv_codec;
struct rv_extension;
struct rv_format;
struct rv_register;
struct rv_csr;
struct rv_csr_field;
struct rv_opcode_major;
struct rv_opcode_class;
struct rv_opcode;
struct rv_constraint;
struct rv_compressed;
struct rv_pseudo;
struct rv_bitrange;
struct rv_latex_row;

typedef unsigned long long inst_t;

typedef std::pair<rv_bitspec,std::string> rv_named_bitspec;
typedef std::shared_ptr<rv_operand> rv_operand_ptr;
typedef std::vector<rv_operand_ptr> rv_operand_list;
typedef std::map<std::string,rv_operand_ptr> rv_operand_map;
typedef std::shared_ptr<rv_enum> rv_enum_ptr;
typedef std::vector<rv_enum_ptr> rv_enum_list;
typedef std::map<std::string,rv_enum_ptr> rv_enum_map;
typedef std::shared_ptr<rv_type> rv_type_ptr;
typedef std::vector<rv_type_ptr> rv_type_list;
typedef std::map<std::string,rv_type_ptr> rv_type_map;
typedef std::shared_ptr<rv_codec> rv_codec_ptr;
typedef std::vector<rv_codec_ptr> rv_codec_list;
typedef std::map<std::string,rv_codec_ptr> rv_codec_map;
typedef std::shared_ptr<rv_extension> rv_extension_ptr;
typedef std::vector<rv_extension_ptr> rv_extension_list;
typedef std::map<std::string,rv_extension_ptr> rv_extension_map;
typedef std::shared_ptr<rv_format> rv_format_ptr;
typedef std::vector<rv_format_ptr> rv_format_list;
typedef std::map<std::string,rv_format_ptr> rv_format_map;
typedef std::shared_ptr<rv_register> rv_register_ptr;
typedef std::vector<rv_register_ptr> rv_register_list;
typedef std::map<std::string,rv_register_ptr> rv_register_map;
typedef std::shared_ptr<rv_csr> rv_csr_ptr;
typedef std::vector<rv_csr_ptr> rv_csr_list;
typedef std::map<std::string,rv_csr_ptr> rv_csr_map;
typedef std::shared_ptr<rv_csr_field> rv_csr_field_ptr;
typedef std::vector<rv_csr_field_ptr> rv_csr_field_list;
typedef std::pair<rv_bitrange,size_t> rv_opcode_mask;
typedef std::vector<rv_opcode_mask> rv_opcode_mask_list;
typedef std::shared_ptr<rv_opcode_class> rv_opcode_class_ptr;
typedef std::vector<rv_opcode_class_ptr> rv_opcode_class_list;
typedef std::map<std::string,rv_opcode_class_ptr> rv_opcode_class_map;
typedef std::shared_ptr<rv_opcode_major> rv_opcode_major_ptr;
typedef std::vector<rv_opcode_major_ptr> rv_opcode_major_list;
typedef std::shared_ptr<rv_opcode> rv_opcode_ptr;
typedef std::vector<rv_opcode_ptr> rv_opcode_list;
typedef std::shared_ptr<rv_constraint> rv_constraint_ptr;
typedef std::map<std::string,rv_constraint_ptr> rv_constraint_map;
typedef std::vector<rv_constraint_ptr> rv_constraint_list;
typedef std::shared_ptr<rv_compressed> rv_compressed_ptr;
typedef std::vector<rv_compressed_ptr> rv_compressed_list;
typedef std::map<std::string,rv_opcode_ptr> rv_opcode_map;
typedef std::map<std::string,rv_opcode_list> rv_opcode_list_map;
typedef std::set<rv_opcode_ptr> rv_opcode_set;
typedef std::shared_ptr<rv_pseudo> rv_pseudo_ptr;
typedef std::map<std::string,rv_pseudo_ptr> rv_pseudo_map;
typedef std::vector<rv_pseudo_ptr> rv_pseudo_list;

int64_t rv_parse_value(const char* valstr);

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
	rvt_s128,
	rvt_u128,
	rvt_f32,
	rvt_f64,
	rvt_f128,
	rvt_none,
};

enum rvs
{
	rvs_std,
	rvs_ext,
};

struct rv_primitive_type
{
	rvs std_type;
	rvt enum_type;
	const char* spec_type;
	const char* meta_type;
	const char* asm_type;
	const char* c_fmt;
	const char* c_hex_fmt;
	const char* c_suffix;
	const char* c_type;
};

extern const rv_primitive_type rv_primitive_type_table[];
extern const rv_primitive_type* rv_lookup_primitive_by_spec_type(std::string spec_type, rvt default_type = rvt_sx);
extern const rv_primitive_type* rv_lookup_primitive_by_meta_type(std::string meta_type, rvt default_type = rvt_sx);

struct rv_bitrange
{
	ssize_t msb;
	ssize_t lsb;

	rv_bitrange() : msb(0), lsb(0) {}
	rv_bitrange(std::string bitrange);
	rv_bitrange(ssize_t msb, ssize_t lsb) : msb(msb), lsb(lsb) {}
	rv_bitrange(const rv_bitrange &o) : msb(o.msb), lsb(o.lsb) {}

	std::string to_string(std::string sep, bool collapse_single_bit_range = true);
};

struct rv_bitspec
{
	typedef std::vector<rv_bitrange> rv_bitrange_list;
	typedef std::pair<rv_bitrange,rv_bitrange_list> rv_bitseg;

	std::vector<rv_bitseg> segments;

	rv_bitspec() : segments() {}
	rv_bitspec(std::string bitspec);
	rv_bitspec(const rv_bitspec &o) : segments(o.segments) {}

	bool matches_bit(ssize_t bit);
	size_t decoded_msb();
	std::string to_string();
	std::string to_template();
};

struct rv_version : std::tuple<int,int,int>
{
	rv_version()
		: rv_version(0) {}
	rv_version(int x)
		: std::tuple<int,int,int>(x, x, x) {}
	rv_version(int major, int minor, int patchlevel)
		: std::tuple<int,int,int>(major, minor, patchlevel) {}
};

struct rv_version_spec
{
	rv_version start;
	rv_version end;

	rv_version_spec()
		: start(), end() {}
	rv_version_spec(rv_version start, rv_version end)
		: start(start), end(end) {}
};

struct rv_operand
{
	std::string name;
	rv_bitspec bitspec;
	std::string type;
	std::string label;

	rv_operand(std::string name, std::string bitspec, std::string type,
		  std::string label)
		: name(name), bitspec(bitspec), type(type), label(label) {}

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

	std::string fg_color() {
		if (name.find("rd") != std::string::npos ||
			name.find("rs1") != std::string::npos ||
			name.find("rs2") != std::string::npos ||
			name.find("rs2") != std::string::npos)
		{
			return "white";
		} else {
			return "green";
		}
	}

	std::string bg_color() {
		if (name.find("rd") != std::string::npos) {
			return "magenta";
		} else if (name.find("rs1") != std::string::npos) {
			return "cyan";
		} else if (name.find("rs2") != std::string::npos) {
			return "green";
		} else if (name.find("rs3") != std::string::npos) {
			return "yellow";
		} else {
			return "none";
		}
	}
};

struct rv_enum
{
	std::string group;
	std::string name;
	int64_t value;
	std::string description;
	rv_version_spec version;

	rv_enum(std::string group, std::string name, std::string value, std::string description, rv_version_spec version)
		: group(group), name(name), value(rv_parse_value(value.c_str())), description(description), version(version) {}
};

struct rv_type
{
	std::string name;
	std::string description;
	std::vector<rv_named_bitspec> parts;

	rv_type(std::string name, std::string description) : name(name), description(description) {}

	rv_named_bitspec find_named_bitspec(ssize_t bit) {
		for (auto ent : parts) {
			if (ent.first.matches_bit(bit)) return ent;
		}
		return rv_named_bitspec();
	}
};

struct rv_codec
{
	std::string name;
	std::string format;
	std::string codec_key;
	rv_operand_list operands;

	rv_codec(std::string name, std::string format) : name(name), format(format) {}
};

struct rv_extension
{
	std::string name;
	std::string prefix;
	size_t isa_width;
	char alpha_code;
	ssize_t inst_width;
	std::string description;
	rv_opcode_list opcodes;

	rv_extension(std::string prefix, std::string isa_width,
		  std::string alpha_code, std::string inst_width, std::string description)
		: name(prefix + isa_width + alpha_code), prefix(prefix),
		  isa_width(strtoull(isa_width.c_str(), NULL, 10)),
		  alpha_code(alpha_code.length() > 0 ? alpha_code[0] : '?'),
		  inst_width(strtoull(inst_width.c_str(), NULL, 10)),
		  description(description) {}
};

struct rv_format
{
	std::string operands;
	std::string description;

	rv_format(std::string operands, std::string description)
		: operands(operands), description(description) {}
};

struct rv_register
{
	std::string name;
	std::string alias;
	std::string type;
	std::string save;
	std::string description;

	rv_register(std::string name, std::string alias, std::string type, std::string save, std::string description)
		: name(name), alias(alias), type(type), save(save), description(description) {}
};

struct rv_csr
{
	std::string number;
	std::string access;
	std::string name;
	std::string description;
	rv_version_spec version;

	rv_csr(std::string number, std::string access, std::string name, std::string description, rv_version_spec version)
		: number(number), access(access), name(name), description(description), version(version) {}
};

struct rv_csr_field
{
	std::string name;
	std::string field;
	rv_bitspec bitspec;
	std::string modes;
	std::string description;
	rv_version_spec version;

	rv_csr_field(std::string name, std::string field, rv_bitspec bitspec, std::string modes, std::string description, rv_version_spec version)
		: name(name), field(field), bitspec(bitspec), modes(modes), description(description), version(version) {}
};

struct rv_opcode_major
{
	rv_opcode_mask_list masks;
	std::string name;
};

struct rv_opcode_class
{
	std::string name;
	rv_opcode_list opcodes;

	rv_opcode_class(std::string name) : name(name) {}
};

struct rv_opcode
{
	std::string name;
	std::string fullname;
	std::string description;
	std::string pseudocode_c;
	std::string pseudocode_alt;
	rv_operand_list operands;
	rv_opcode_class_list classes;
	rv_opcode_mask_list masks;
	rv_codec_ptr codec;
	rv_format_ptr format;
	rv_type_ptr type;
	rv_extension_list extensions;
	rv_compressed_ptr compressed;
	rv_compressed_list compressions;
	rv_pseudo_ptr pseudo;
	rv_pseudo_list pseudos;

	size_t num;
	inst_t mask;
	inst_t match;
	inst_t done;
	bool suffix;

	rv_opcode(std::string name)
		: name(name), num(0), mask(0), match(0), done(0), suffix(false) {}

	bool is_pseudo() { return name.find("@") == 0; }

	bool match_extension(rv_extension_list &s) {
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

	rv_operand_ptr find_operand(ssize_t bit) {
		for (auto operand : operands) {
			if (operand->bitspec.matches_bit(bit)) return operand;
		}
		return rv_operand_ptr();
	}

	rv_named_bitspec find_named_bitspec(ssize_t bit) {
		for (auto ent : type->parts) {
			if (ent.first.matches_bit(bit)) return ent;
		}
		return rv_named_bitspec();
	}
};

struct rv_constraint
{
	std::string name;
	std::string expression;
	std::string hint;

	rv_constraint(std::string name, std::string expression)
		: name(name), expression(expression) {}
};

struct rv_compressed
{
	rv_opcode_ptr comp_opcode;
	rv_opcode_ptr decomp_opcode;
	rv_constraint_list constraint_list;

	rv_compressed(rv_opcode_ptr comp_opcode, rv_opcode_ptr decomp_opcode, rv_constraint_list constraint_list)
		: comp_opcode(comp_opcode), decomp_opcode(decomp_opcode), constraint_list(constraint_list) {}
};

struct rv_pseudo
{
	std::string name;
	rv_opcode_ptr pseudo_opcode;
	rv_opcode_ptr real_opcode;
	rv_format_ptr format;
	rv_constraint_list constraint_list;

	rv_pseudo(std::string name, rv_opcode_ptr pseudo_opcode, rv_opcode_ptr real_opcode, rv_format_ptr format, rv_constraint_list constraint_list)
		: name(name), pseudo_opcode(pseudo_opcode), real_opcode(real_opcode), format(format), constraint_list(constraint_list) {}
};

struct rv_meta_model
{
	static const ssize_t DEFAULT;

	rv_operand_list       operands;
	rv_operand_map        operands_by_name;
	rv_enum_list          enums;
	rv_enum_map           enums_by_name;
	rv_type_list          types;
	rv_type_map           types_by_name;
	rv_codec_list         codecs;
	rv_codec_map          codecs_by_name;
	rv_codec_map          codecs_by_key;
	rv_extension_list     extensions;
	rv_extension_map      extensions_by_name;
	rv_format_list        formats;
	rv_format_map         formats_by_name;
	rv_register_list      registers;
	rv_register_map       registers_by_name;
	rv_csr_list           csrs;
	rv_csr_map            csrs_by_name;
	rv_csr_field_list     csr_fields;
	rv_opcode_major_list  opcode_majors;
	rv_opcode_class_list  opcode_classes;
	rv_opcode_class_map   opcode_classes_byname;
	rv_opcode_list        opcodes;
	rv_opcode_list        all_opcodes;
	rv_opcode_list_map    opcodes_by_name;
	rv_constraint_list    constraints;
	rv_constraint_map     constraints_by_name;
	rv_compressed_list    compressions;
	rv_pseudo_list        pseudos;
	rv_pseudo_map         pseudos_by_name;

	static rv_opcode_mask decode_mask(std::string bit_spec);
	static rv_version decode_version(std::string version, rv_version default_version);
	static rv_version_spec decode_version_spec(std::string version_spec);
	static std::string format_type(rv_operand_ptr operand);
	static std::string format_codec(std::string prefix, rv_codec_ptr codec, std::string dot, bool strip_suffix = true);
	static std::string format_format(std::string prefix, rv_format_ptr format, char special);
	static std::string opcode_format(std::string prefix, rv_opcode_ptr opcode, std::string dot, bool suffix = false);
	static std::string opcode_codec_key(rv_opcode_ptr opcode);
	static std::string opcode_comment(rv_opcode_ptr opcode, bool no_comment);
	static std::string opcode_isa_shortname(rv_opcode_ptr opcode);
	static std::string codec_type_name(rv_codec_ptr codec);
	static std::vector<rv_bitrange> bitmask_to_bitrange(std::vector<ssize_t> &bits);
	static std::string format_bitmask(std::vector<ssize_t> &bits, std::string var, bool comment);
	static const rv_primitive_type* infer_operand_primitive(rv_opcode_ptr &opcode, rv_extension_ptr &ext, rv_operand_ptr &operand, size_t i);
	static std::vector<std::string> parse_line(std::string line);
	static std::vector<std::vector<std::string>> read_file(std::string filename);

	std::vector<std::string> get_unique_codecs();
	std::vector<std::string> get_inst_mnemonics(bool isa_widths, bool isa_extensions);
	std::vector<std::pair<size_t,std::string>> isa_width_prefixes();
	rv_extension_list decode_isa_extensions(std::string isa_spec);
	rv_opcode_ptr create_opcode(std::string opcode_name, std::string extension);
	rv_opcode_list lookup_opcode_by_name(std::string opcode_name);
	rv_opcode_list opcode_list_by_width(size_t isa_width);

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
	void parse_csr_field(std::vector<std::string> &part);
	void parse_opcode_major(std::vector<std::string> &part);
	void parse_opcode(std::vector<std::string> &part);
	void parse_constraint(std::vector<std::string> &part);
	void parse_compression(std::vector<std::string> &part);
	void parse_pseudo(std::vector<std::string> &part);
	void parse_opcode_classes(std::vector<std::string> &part);
	void parse_opcode_fullname(std::vector<std::string> &part);
	void parse_opcode_description(std::vector<std::string> &part);
	void parse_opcode_pseudocode_c(std::vector<std::string> &part);
	void parse_opcode_pseudocode_alt(std::vector<std::string> &part);

	bool read_metadata(std::string dirname);
};

#endif
