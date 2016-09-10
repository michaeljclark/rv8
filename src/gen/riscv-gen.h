//
//  riscv-gen.h
//

#ifndef riscv_gen_h
#define riscv_gen_h

extern const char* kCHeader;
extern const ssize_t kMaxInstructionWidth;

struct riscv_gen;
struct riscv_gen_abstract;
typedef std::shared_ptr<riscv_gen_abstract> riscv_gen_abstract_ptr;
typedef std::vector<riscv_gen_abstract_ptr> riscv_gen_abstract_list;

struct riscv_gen_abstract
{
	riscv_gen *gen;
	riscv_gen_abstract(riscv_gen *gen) : gen(gen) {}
	virtual ~riscv_gen_abstract() {}
	virtual std::vector<cmdline_option> get_cmdline_options() = 0;
	virtual void generate() = 0;
};

struct riscv_gen_constraints : riscv_gen_abstract
{
	riscv_gen_constraints(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_fpu_test : riscv_gen_abstract
{
	riscv_gen_fpu_test(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_interp : riscv_gen_abstract
{
	riscv_gen_interp(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_jit : riscv_gen_abstract
{
	riscv_gen_jit(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_latex : riscv_gen_abstract
{
	riscv_gen_latex(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_map : riscv_gen_abstract
{
	riscv_gen_map(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_meta : riscv_gen_abstract
{
	riscv_gen_meta(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_operands : riscv_gen_abstract
{
	riscv_gen_operands(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_strings : riscv_gen_abstract
{
	riscv_gen_strings(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_gen_switch : riscv_gen_abstract
{
	riscv_gen_switch(riscv_gen *gen) : riscv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct riscv_codec_node
{
	std::vector<ssize_t> bits;
	std::vector<ssize_t> vals;
	std::map<ssize_t,riscv_opcode_list> val_opcodes;
	std::map<ssize_t,riscv_codec_node> val_decodes;

	void clear();
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

struct riscv_gen : riscv_meta_model
{
	riscv_extension_list     ext_subset;
	riscv_codec_node         root_node;
	riscv_opcode_ptr         unknown;
	std::set<std::string>    opts;
	riscv_gen_abstract_list  generators;

	riscv_gen();

	void generate(int argc, const char *argv[]);

	void generate_map();
	void generate_codec();
	void generate_codec_node(riscv_codec_node &node, riscv_opcode_list &opcode_list);

	bool set_option(std::string o) { opts.insert(o); return true; }
	bool has_option(std::string o) { return opts.find(o) != opts.end(); }
};

#endif