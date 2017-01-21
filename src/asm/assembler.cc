//
//  assembler.h
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <algorithm>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "host-endian.h"
#include "types.h"
#include "bits.h"
#include "format.h"
#include "meta.h"
#include "util.h"
#include "cmdline.h"
#include "color.h"
#include "codec.h"
#include "strings.h"
#include "disasm.h"
#include "elf.h"
#include "assembler.h"
#include "jit.h"

using namespace riscv;


bool assembler::check_symbol(std::string arg)
{
	// [_\w][_\w\d]*
	if (arg.size() < 1 || !(std::isalpha(arg[0]) || arg[0] == '_')) return false;
	for (auto ci = arg.begin() + 1; ci != arg.end(); ci++) {
		if (!(std::isalnum(*ci) || *ci == '_')) return false;
	}
	return true;
}

bool assembler::check_private(std::string arg)
{
	// \.[_\w][_\w\d]*
	if (arg.size() < 2 || arg[0] != '.' ||
		!(std::isalpha(arg[1]) || arg[1] == '_')) return false;
	for (auto ci = arg.begin() + 2; ci != arg.end(); ci++) {
		if (!(std::isalnum(*ci) || *ci == '_')) return false;
	}
	return true;
}

bool assembler::check_local(std::string arg)
{
	// \d+[fb]
	if (arg.size() < 2 || !(arg.back() == 'b' || arg.back() == 'f')) return false;
	for (auto ci = arg.begin(); ci != arg.end() - 1; ci++) {
		if (!std::isdigit(*ci)) return false;
	}
	return true;
}

assembler::assembler()
{
	get_section("");
	get_section(".text");
}

void assembler::global(std::string label)
{
	strong_exports.push_back(label);
}

void assembler::weak(std::string label)
{
	weak_exports.push_back(label);
}

section_offset assembler::current_offset()
{
	return section_offset(current->index, current->buf.size());
}

section_ptr assembler::get_section(std::string name)
{
	auto si = section_map.find(name);
	if (si == section_map.end()) {
		auto s = std::make_shared<section>(".text");
		s->index = sections.size();
		section_map.insert(section_map.end(), std::pair<std::string,section_ptr>(name, s));
		sections.push_back(s);
		return (current = s);
	} else {
		return (current = si->second);
	}
}

void assembler::append(u8 d)
{
	current->buf.push_back(d);
}

void assembler::append(u16 d)
{
	current->buf.push_back(d & 0xff);
	current->buf.push_back(d >> 8);
}

void assembler::append(u32 d)
{
	current->buf.push_back(d & 0xff);
	current->buf.push_back((d >> 8) & 0xff);
	current->buf.push_back((d >> 16) & 0xff);
	current->buf.push_back((d >> 24) & 0xff);
}

void assembler::append(u64 d)
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

void assembler::add_inst(u64 inst)
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

void assembler::load_imm_r(ireg5 rd, s64 val)
{
	if ((val & ~s64(0x7fffffff)) == 0 || (val & ~s64(0x7fffffff)) == ~s64(0x7fffffff)) {
		add_inst(emit_lui(rd, s32(val >> 12)));
		val -= (val >> 12) << 12;
		if (val != 0) {
			add_inst(emit_addi(rd, rd, bitextend(val, 12)));
		}
	}
	else {
		s64 lo = bitextend(val, 12), hi = val - lo;
		int shift = 12 + ctz(u64(hi) >> 12);
		hi >>= shift;
		load_imm_r(rd, hi);
		add_inst(emit_slli(rd, rd, shift));
		if (lo != 0) {
			add_inst(emit_addi(rd, rd, s32(bitextend(lo, 12))));
		}
	}
}

void assembler::load_imm(ireg5 rd, s64 val)
{
	if (val >= -2048 && val <= 2047) {
		add_inst(emit_addi(rd, rv_ireg_zero, s32(val)));
	} else if (ispow2(val)) {
		add_inst(emit_addi(rd, rv_ireg_zero, 1));
		add_inst(emit_slli(rd, rd, ctz(val)));
	} else {
		load_imm_r(rd, val);
	}
}

label_ptr assembler::lookup_label(std::string label_name)
{
	auto li = labels_byname.find(label_name);
	return (li != labels_byname.end()) ? li->second : label_ptr();
}

label_ptr assembler::lookup_label_f(reloc_ptr reloc, s64 num)
{
	bool found = false;
	for (auto ent : labels_byoffset) {
		if (ent.first > reloc->offset) found = true;
		if (found && ent.second->num == num) return ent.second;
	}
	return label_ptr();
}

label_ptr assembler::lookup_label_b(reloc_ptr reloc, s64 num)
{
	label_ptr found;
	for (auto ent : labels_byoffset) {
		if (ent.first > reloc->offset) break;
		if (ent.second->num == num) found = ent.second;
	}
	return found;
}

label_ptr assembler::lookup_label(reloc_ptr reloc, std::string name)
{
	if (check_local(name)) {
		if (name.back() == 'f') {
			name.erase(name.end()-1);
			s64 val = strtoull(name.c_str(), nullptr, 10);
			return lookup_label_f(reloc, val);
		}
		if (name.back() == 'b') {
			name.erase(name.end()-1);
			s64 val = strtoull(name.c_str(), nullptr, 10);
			return lookup_label_b(reloc, val);
		}
	}
	return lookup_label(name);
}

label_ptr assembler::add_label(std::string label_name)
{
	if (labels_byname.find(label_name) != labels_byname.end()) {
		return label_ptr();
	}
	auto l = std::make_shared<label>(label_name, current_offset());
	labels_byname[label_name] = l;
	labels_byoffset[l->offset] = l;
	return l;
}

label_ptr assembler::add_label(s64 num)
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

label_ptr assembler::add_constant(std::string label_name, s64 value)
{
	if (labels_byname.find(label_name) != labels_byname.end()) {
		return label_ptr();
	}
	auto l = std::make_shared<label>(label_name, section_offset(0, value));
	labels_byname[label_name] = l;
	labels_byoffset[l->offset] = l;
	return l;
}

reloc_ptr assembler::lookup_reloc(section_offset offset)
{
	auto ri = relocs_byoffset.find(offset);
	return (ri != relocs_byoffset.end()) ? ri->second : reloc_ptr();
}

reloc_ptr assembler::add_reloc(std::string label_name, int rela_type)
{
	if (relocs_byoffset.find(current_offset()) != relocs_byoffset.end()) {
		return reloc_ptr();
	}
	auto r = std::make_shared<reloc>(current_offset(), label_name, rela_type);
	relocs_byoffset[r->offset] = r;
	return r;
}

void assembler::balign(size_t align)
{
	if (!ispow2(align)) return;
	if ((current->buf.size() & (align-1)) == 0) return;
	size_t sz = align - (current->buf.size() & (align-1));
	for(size_t i = 0; i < sz; i++) {
		current->buf.push_back(0);
	}
}

void assembler::p2align(size_t s)
{
	balign(1 << s);
}

size_t assembler::label_offset(label_ptr label)
{
	return sections[label->offset.first]->offset + label->offset.second;
}

u8* assembler::label_buffer(label_ptr label)
{
	return &sections[label->offset.first]->buf[label->offset.second];
}

size_t assembler::reloc_offset(reloc_ptr reloc)
{
	return sections[reloc->offset.first]->offset + reloc->offset.second;
}

u8* assembler::reloc_buffer(reloc_ptr reloc)
{
	return &sections[reloc->offset.first]->buf[reloc->offset.second];
}

void assembler::reencode_inst(decode &dec, addr_t offset)
{
	inst_t inst = encode_inst(dec);
	size_t len = inst_length(inst);
	switch (len) {
		case 2: *(u16*)offset = le16toh(inst & 0xffff); break;
		case 4: *(u32*)offset = le32toh(inst & 0xffffffff); break;
	}
}

bool assembler::relocate(reloc_ptr reloc)
{
	/* find target symbol */
	label_ptr label = lookup_label(reloc, reloc->name);
	if (!label) return false;
	reloc->name = label->name;
	size_t label_off = label_offset(label);

	/* decode instruction being relocated */
	addr_t pc_offset;
	decode dec;
	size_t reloc_off = reloc_offset(reloc);
	u8* reloc_buf = reloc_buffer(reloc);
	inst_t inst = inst_fetch((addr_t)reloc_buf, pc_offset);
	decode_inst_rv64(dec, inst);
	bool re_encode = false;

	/* apply relocation */
	switch (reloc->rela_type) {
		case R_RISCV_JAL:
		case R_RISCV_BRANCH:
			dec.imm = (label_off - reloc_off);
			re_encode = true;
			break;
		case R_RISCV_HI20: {
			dec.imm = ((label_off + 0x800) >> 12) << 12;
			re_encode = true;
			break;
		}
		case R_RISCV_LO12_I:
		case R_RISCV_LO12_S: {
			int upper = ((label_off + 0x800) >> 12) << 12;
			dec.imm = label_off - upper;
			re_encode = true;
			break;
		}
		case R_RISCV_PCREL_HI20: {
			dec.imm = ((label_off - reloc_off + 0x800) >> 12) << 12;
			re_encode = true;
			break;
		}
		case R_RISCV_PCREL_LO12_I:
		case R_RISCV_PCREL_LO12_S: {
			/*
			 * the reloc label points to an auipc instruction with the %pcrel_hi
			 * reloc entry, so we use the label to look up the previous relocation
			 * entry and use that as the base address for the PC-relative offset
			 */
			auto ri = relocs_byoffset.find(section_offset(reloc->offset.first, label_off));
			if (ri == relocs_byoffset.end()) {
				return false;
			}
			reloc = ri->second;
			reloc_off = reloc_offset(reloc);

			/* find symbol associated with the reloc pointed to by our label */
			label = lookup_label(reloc->name);
			if (!label) return false;
			size_t label_off = label_offset(label);

			/* calculate difference from upper */
			int upper = ((label_off - reloc_off + 0x800) >> 12) << 12;
			dec.imm = label_off - reloc_off - upper;
			re_encode = true;
			break;
		}
		case R_RISCV_CALL: {
			/*
			 * R_RISCV_CALL relocation needs to update two instructions
			 *
			 * relocate auipc immediate
			 */
			int upper = ((label_off - reloc_off + 0x800) >> 12) << 12;
			int lower = label_off - reloc_off - upper;
			dec.imm = upper;
			re_encode = true;

			/* relocate jalr immediate */
			{
				decode dec;
				addr_t ignore_pc_offset;
				inst_t inst = inst_fetch(addr_t(reloc_buf + pc_offset), ignore_pc_offset);
				decode_inst_rv64(dec, inst);
				dec.imm = lower;
				reencode_inst(dec, addr_t(reloc_buf + pc_offset));
			}
			break;
		}
		case R_RISCV_32:
			*(u32*)reloc_buf = u32(label_off);
			break;
		case R_RISCV_64:
			*(u64*)reloc_buf = u64(label_off);
			break;
	}

	/* re-encode instruction being relocated */
	if (re_encode) {
		reencode_inst(dec, addr_t(reloc_buf));
	}

	return true;
}

void assembler::link()
{
	int unresolved_relocs = 0;
	sections.front()->offset = 0;
	for (auto si = sections.begin() + 1; si != sections.end(); si++) {
		(*si)->offset = (*(si-1))->offset + (*(si-1))->buf.size();
	}
	for (auto &ent : relocs_byoffset) {
		if (!relocate(ent.second)) {
			printf("unresolved symbol: %s\n", ent.second->name.c_str());
			unresolved_relocs++;
		}
	}
	if (unresolved_relocs > 0) {
		printf("%d unresolved relocations during link\n", unresolved_relocs);
	}
}
