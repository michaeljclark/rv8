//
//  gen.h
//

#ifndef rv_gen_h
#define rv_gen_h

extern const char* kCHeader;
extern const ssize_t kMaxInstructionWidth;

struct rv_gen;
struct rv_gen_abstract;
typedef std::shared_ptr<rv_gen_abstract> rv_gen_abstract_ptr;
typedef std::vector<rv_gen_abstract_ptr> rv_gen_abstract_list;

struct rv_gen_abstract
{
	rv_gen *gen;
	rv_gen_abstract(rv_gen *gen) : gen(gen) {}
	virtual ~rv_gen_abstract() {}
	virtual std::vector<cmdline_option> get_cmdline_options() = 0;
	virtual void generate() = 0;
};

struct rv_gen_cc : rv_gen_abstract
{
	rv_gen_cc(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_constraints : rv_gen_abstract
{
	rv_gen_constraints(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_fpu_test : rv_gen_abstract
{
	rv_gen_fpu_test(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_interp : rv_gen_abstract
{
	rv_gen_interp(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_jit : rv_gen_abstract
{
	rv_gen_jit(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_latex : rv_gen_abstract
{
	rv_gen_latex(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_latex_alt : rv_gen_abstract
{
	rv_gen_latex_alt(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_map : rv_gen_abstract
{
	rv_gen_map(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_markdown : rv_gen_abstract
{
	rv_gen_markdown(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_meta : rv_gen_abstract
{
	rv_gen_meta(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_operands : rv_gen_abstract
{
	rv_gen_operands(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_strings : rv_gen_abstract
{
	rv_gen_strings(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_switch : rv_gen_abstract
{
	rv_gen_switch(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_gen_tablegen : rv_gen_abstract
{
	rv_gen_tablegen(rv_gen *gen) : rv_gen_abstract(gen) {}
	std::vector<cmdline_option> get_cmdline_options();
	void generate();
};

struct rv_codec_node
{
	std::vector<ssize_t> bits;
	std::vector<ssize_t> vals;
	std::map<ssize_t,rv_opcode_list> val_opcodes;
	std::map<ssize_t,rv_codec_node> val_decodes;

	void clear();
};

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

struct rv_gen : rv_meta_model
{
	rv_extension_list     ext_subset;
	rv_codec_node         root_node;
	rv_opcode_ptr         unknown;
	std::set<std::string>    opts;
	rv_gen_abstract_list  generators;

	rv_gen();

	void generate(int argc, const char *argv[]);

	void generate_map();
	void generate_codec();
	void generate_codec_node(rv_codec_node &node, rv_opcode_list &opcode_list);

	bool set_option(std::string o) { opts.insert(o); return true; }
	bool has_option(std::string o) { return opts.find(o) != opts.end(); }
};

#endif
