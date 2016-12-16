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

	struct section
	{
		std::string name;
		std::vector<u8> buf;

		section(std::string name) :
			name(name) {}
	};

	struct label
	{
		std::string name;
		section_ptr section;
		u32 offset;

		label(std::string name, section_ptr section, u32 offset) :
			name(name), section(section), offset(offset) {}
	};

	struct reloc
	{
		u32 offset;
		section_ptr section;
		std::string name;
		int rela_type;

		reloc(u32 offset, section_ptr section, std::string name, int rela_type) :
			offset(offset), section(section), name(name), rela_type(rela_type) {}
	};

	struct assembler
	{
		std::map<std::string,section_ptr> sections;

		std::map<size_t,label_ptr> labels_byoffset;
		std::map<std::string,label_ptr> labels_byname;
		std::map<size_t,reloc_ptr> relocs_byoffset;
		std::map<std::string,reloc_ptr> relocs_byname;
		std::vector<std::string> exports;
		section_ptr current;

		assembler()
		{
			get_section(".text");
		}

		void global(std::string label)
		{
			exports.push_back(label);
		}

		size_t current_offset()
		{
			return current->buf.size();
		}

		section_ptr get_section(std::string name)
		{
			auto si = sections.find(name);
			if (si == sections.end()) {
				auto s = std::make_shared<section>(".text");
				sections.insert(sections.end(), std::pair<std::string,section_ptr>(name, s));
				return (current = s);
			} else {
				return (current = si->second);
			}
		}

		void append(u8 d)
		{
			current->buf.push_back(d);
		}

		void append(u16 d)
		{
			current->buf.push_back(d & 0xff);
			current->buf.push_back(d >> 8);
		}

		void append(u32 d)
		{
			current->buf.push_back(d & 0xff);
			current->buf.push_back((d >> 8) & 0xff);
			current->buf.push_back((d >> 16) & 0xff);
			current->buf.push_back((d >> 24) & 0xff);
		}

		void append(u64 d)
		{
			current->buf.push_back(d & 0xff);
			current->buf.push_back((d >> 8) & 0xff);
			current->buf.push_back((d >> 16) & 0xff);
			current->buf.push_back((d >> 24) & 0xff);
			current->buf.push_back((d >> 32) & 0xff);
			current->buf.push_back((d >> 40) & 0xff);
			current->buf.push_back((d >> 48) & 0xff);
			current->buf.push_back((d >> 56) & 0xff);
		}

		void add_inst(u64 inst)
		{
			size_t len = inst_length(inst);
			switch (len) {
				case 2:
					append(u16(inst));
					break;
				case 4:
					append(u32(inst));
					break;
				case 6:
					append(u32(inst));
					append(u16(inst >> 32));
					break;
				case 8:
					append(inst);
					break;
			}
		}

		void add_label(std::string label_name)
		{
			auto l = std::make_shared<label>(label_name, current, current->buf.size());
			labels_byname[label_name] = l;
			labels_byoffset[current->buf.size()] = l;
		}

		void add_reloc(std::string label_name, int rela_type)
		{
			auto r = std::make_shared<reloc>(current->buf.size(), current, label_name, rela_type);
			relocs_byname[label_name] = r;
			relocs_byoffset[current->buf.size()] = r;
		}

		void balign(size_t align)
		{
			if (!ispow2(align)) return;
			if ((current->buf.size() & (align-1)) == 0) return;
			size_t sz = align - (current->buf.size() & (align-1));
			for(size_t i = 0; i < sz; i++) {
				current->buf.push_back(0);
			}
		}

		void p2align(size_t s)
		{
			balign(1 << s);
		}
	};
}

#endif
