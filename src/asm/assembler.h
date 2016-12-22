//
//  assembler.h
//

#ifndef rv_assembler_h
#define rv_assembler_h

namespace riscv {

	struct section;
	struct label;
	struct reloc;

	typedef std::shared_ptr<section> section_ptr;
	typedef std::shared_ptr<label> label_ptr;
	typedef std::shared_ptr<reloc> reloc_ptr;

	typedef std::pair<size_t,size_t> section_offset;

	struct section
	{
		size_t index;
		size_t offset;
		std::string name;
		std::vector<u8> buf;

		section(std::string name) :
			index(0), offset(0), name(name) {}
	};

	struct label
	{
		std::string name;
		section_offset offset;
		s64 num;

		label(std::string name, section_offset offset) :
			name(name), offset(offset), num(0) {}
	};

	struct reloc
	{
		section_offset offset;
		section_ptr section;
		std::string name;
		int rela_type;

		reloc(section_offset offset, std::string name, int rela_type) :
			offset(offset), name(name), rela_type(rela_type) {}
	};

	struct assembler
	{
		std::vector<section_ptr> sections;
		std::map<std::string,section_ptr> section_map;

		std::map<section_offset,label_ptr> labels_byoffset;
		std::map<std::string,label_ptr> labels_byname;
		std::map<section_offset,reloc_ptr> relocs_byoffset;
		std::vector<std::string> strong_exports;
		std::vector<std::string> weak_exports;
		section_ptr current;

		static bool check_symbol(std::string arg);
		static bool check_private(std::string arg);
		static bool check_local(std::string arg);

		assembler();

		void global(std::string label);
		void weak(std::string label);

		section_offset current_offset();
		section_ptr get_section(std::string name);

		void append(u8 d);
		void append(u16 d);
		void append(u32 d);
		void append(u64 d);

		void add_inst(u64 inst);

		void load_imm_r(ireg5 rd, s64 val);
		void load_imm(ireg5 rd, s64 val);

		label_ptr lookup_label(std::string label_name);
		label_ptr lookup_label_f(reloc_ptr reloc, s64 num);
		label_ptr lookup_label_b(reloc_ptr reloc, s64 num);
		label_ptr lookup_label(reloc_ptr reloc, std::string name);
		label_ptr add_label(std::string label_name);
		label_ptr add_label(s64 num);

		reloc_ptr lookup_reloc(section_offset offset);
		reloc_ptr add_reloc(std::string label_name, int rela_type);

		void balign(size_t align);
		void p2align(size_t s);

		size_t label_offset(label_ptr label);
		u8* label_buffer(label_ptr label);

		size_t reloc_offset(reloc_ptr reloc);
		u8* reloc_buffer(reloc_ptr reloc);

		bool relocate(reloc_ptr reloc);
		void link();
	};
}

#endif
