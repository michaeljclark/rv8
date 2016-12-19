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
		std::string name;
		std::vector<u8> buf;

		section(std::string name) :
			name(name) {}
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
		std::map<std::string,section_ptr> sections;

		std::map<section_offset,label_ptr> labels_byoffset;
		std::map<std::string,label_ptr> labels_byname;
		std::map<section_offset,reloc_ptr> relocs_byoffset;
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

		section_offset current_offset()
		{
			return section_offset(current->index, current->buf.size());
		}

		section_ptr get_section(std::string name)
		{
			auto si = sections.find(name);
			if (si == sections.end()) {
				auto s = std::make_shared<section>(".text");
				s->index = sections.size();
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

		void load_imm_r(ireg5 rd, s64 val);
		void load_imm(ireg5 rd, s64 val);

		label_ptr lookup_label(std::string label_name)
		{
			auto li = labels_byname.find(label_name);
			return (li != labels_byname.end()) ? li->second : label_ptr();
		}

		label_ptr lookup_label_f(reloc_ptr reloc, s64 num)
		{
			auto li = labels_byoffset.upper_bound(reloc->offset);
			while (li != labels_byoffset.end()) {
				if (li->second->num == num) return li->second;
				li++;
			}
			return label_ptr();
		}

		label_ptr lookup_label_b(reloc_ptr reloc, s64 num)
		{
			auto li = labels_byoffset.lower_bound(reloc->offset);
			while (li != labels_byoffset.begin()) {
				if (li->second->num == num) return li->second;
				li--;
			}
			return label_ptr();
		}

		label_ptr add_label(std::string label_name)
		{
			if (labels_byname.find(label_name) != labels_byname.end()) {
				return label_ptr();
			}
			auto l = std::make_shared<label>(label_name, current_offset());
			labels_byname[label_name] = l;
			labels_byoffset[l->offset] = l;
			return l;
		}

		label_ptr add_label(s64 num)
		{
			size_t i = 0;
			for (;;) {
				std::string num_label = ".L" + std::to_string(num) + std::to_string(i);
				auto li = labels_byname.find(num_label);
				if (li == labels_byname.end()) {
					auto l = add_label(num_label);
					l->num = num;
					return l;
				}
				i++;
			}
		}

		reloc_ptr lookup_reloc(section_offset offset)
		{
			auto ri = relocs_byoffset.find(offset);
			return (ri != relocs_byoffset.end()) ? ri->second : reloc_ptr();
		}

		reloc_ptr add_reloc(std::string label_name, int rela_type)
		{
			if (relocs_byoffset.find(current_offset()) != relocs_byoffset.end()) {
				return reloc_ptr();
			}
			auto r = std::make_shared<reloc>(current_offset(), label_name, rela_type);
			relocs_byoffset[r->offset] = r;
			return r;
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
