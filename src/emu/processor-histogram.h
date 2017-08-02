//
//  processor-histogram.h
//

#ifndef rv_processor_histogram_h
#define rv_processor_histogram_h

namespace riscv {

	const size_t max_chars = 40;

	static std::string repeat_str(std::string str, size_t count)
	{
		std::string s;
		for (size_t i = 0; i < count; i++) s += str;
		return s;
	}

	template <typename P>
	void histogram_pc_print(P &proc, bool reverse_sort)
	{
		size_t addr_count = proc.hist_pc.size();
		size_t addr_shift = 0;
		while (addr_count > 64) {
			addr_count >>= 1;
			addr_shift++;
		}

		hist_pc_map_t hist_pc_reduce;
		hist_pc_reduce.set_empty_key(0);
		for (auto ent : proc.hist_pc) {
			addr_t key = ent.first >> addr_shift;
			auto hi = hist_pc_reduce.find(key);
			if (hi == hist_pc_reduce.end()) hist_pc_reduce.insert(hist_pc_pair_t(key, ent.second));
			else hi->second += ent.second;
		}

		size_t max = 0, total = 0;
		std::vector<hist_pc_pair_t> hist_pc_s;
		for (auto ent : hist_pc_reduce) {
			if (ent.second > max) max = ent.second;
			total += ent.second;
			hist_pc_s.push_back(hist_pc_pair_t(ent.first << addr_shift, ent.second));
		}

		std::sort(hist_pc_s.begin(), hist_pc_s.end(), [&] (const hist_pc_pair_t &a, const hist_pc_pair_t &b) {
			return reverse_sort ? a.second < b.second : a.second > b.second;
		});

		size_t i = 0;
		for (auto ent : hist_pc_s) {
			printf("%5lu. 0x%016llx %5.2f%% [%-9lu] %s\n",
				++i,
				ent.first,
				(float)ent.second / (float)total * 100.0f,
				ent.second,
				repeat_str("#", ent.second * (max_chars - 1) / max).c_str());
		}
	}

	template <typename P>
	void histogram_pc_save(P &proc, std::string filename)
	{
		std::vector<hist_pc_pair_t> hist_pc_s;
		for (auto ent : proc.hist_pc) {
			hist_pc_s.push_back(ent);
		}

		std::sort(hist_pc_s.begin(), hist_pc_s.end(), [&] (const hist_pc_pair_t &a, const hist_pc_pair_t &b) {
			return a.second > b.second;
		});

		FILE *file;
		if ((file = fopen(filename.c_str(), "w")) == nullptr) {
			panic("histogram_pc_save: unable to open: %s: %s",
				filename.c_str(), strerror(errno));
		}
		fprintf(file, "pc\tcount\n");
		for (auto ent : hist_pc_s) {
			fprintf(file, "0x%016llx\t%lu\n",
				ent.first, ent.second);
		}
		fclose(file);
	}

	template <typename P>
	void histogram_reg_print(P &proc, bool reverse_sort)
	{
		size_t max = 0, total = 0;
		std::vector<hist_reg_pair_t> hist_reg_s;
		for (auto ent : proc.hist_reg) {
			if (ent.second > max) max = ent.second;
			total += ent.second;
			hist_reg_s.push_back(ent);
		}

		std::sort(hist_reg_s.begin(), hist_reg_s.end(), [&] (const hist_reg_pair_t &a, const hist_reg_pair_t &b) {
			return reverse_sort ? a.second < b.second : a.second > b.second;
		});

		size_t i = 0;
		for (auto ent : hist_reg_s) {
			printf("%5lu. %-10s %5.2f%% [%-9lu] %s\n",
				++i,
				ent.first < 32 ?
					rv_ireg_name_sym[ent.first] :
					rv_freg_name_sym[ent.first - 32],
				(float)ent.second / (float)total * 100.0f,
				ent.second,
				repeat_str((ent.first >= 8 && ent.first <= 15) ? "-" : "=",
					ent.second * (max_chars - 1) / max).c_str());
		}
	}

	template <typename P>
	void histogram_reg_save(P &proc, std::string filename)
	{
		std::vector<hist_reg_pair_t> hist_reg_s;
		for (auto ent : proc.hist_reg) {
			hist_reg_s.push_back(ent);
		}

		std::sort(hist_reg_s.begin(), hist_reg_s.end(), [&] (const hist_reg_pair_t &a, const hist_reg_pair_t &b) {
			return a.second > b.second;
		});

		FILE *file;
		if ((file = fopen(filename.c_str(), "w")) == nullptr) {
			panic("histogram_pc_save: unable to open: %s: %s",
				filename.c_str(), strerror(errno));
		}
		fprintf(file, "register\tcount\n");
		for (auto ent : hist_reg_s) {
			fprintf(file, "%s\t%lu\n",
				ent.first < 32 ?
					rv_ireg_name_sym[ent.first] :
					rv_freg_name_sym[ent.first - 32],
				ent.second);
		}
		fclose(file);
	}

	template <typename P>
	void histogram_inst_print(P &proc, bool reverse_sort)
	{
		size_t max = 0, total = 0;
		std::vector<hist_inst_pair_t> hist_inst_s;
		for (auto ent : proc.hist_inst) {
			if (ent.second > max) max = ent.second;
			total += ent.second;
			hist_inst_s.push_back(ent);
		}

		std::sort(hist_inst_s.begin(), hist_inst_s.end(), [&] (const hist_inst_pair_t &a, const hist_inst_pair_t &b) {
			return reverse_sort ? a.second < b.second : a.second > b.second;
		});

		size_t i = 0;
		for (auto ent : hist_inst_s) {
			printf("%5lu. %-10s %5.2f%% [%-9lu] %s\n",
				++i,
				rv_inst_name_sym[ent.first],
				(float)ent.second / (float)total * 100.0f,
				ent.second,
				repeat_str("#", ent.second * (max_chars - 1) / max).c_str());
		}
	}

	template <typename P>
	void histogram_inst_save(P &proc, std::string filename)
	{
		std::vector<hist_inst_pair_t> hist_inst_s;
		for (auto ent : proc.hist_inst) {
			hist_inst_s.push_back(ent);
		}

		std::sort(hist_inst_s.begin(), hist_inst_s.end(), [&] (const hist_inst_pair_t &a, const hist_inst_pair_t &b) {
			return a.second > b.second;
		});

		FILE *file;
		if ((file = fopen(filename.c_str(), "w")) == nullptr) {
			panic("histogram_inst_save: unable to open: %s: %s",
				filename.c_str(), strerror(errno));
		}
		fprintf(file, "opcode\tcount\n");
		for (auto ent : hist_inst_s) {
			fprintf(file, "%s\t%lu\n",
				rv_inst_name_sym[ent.first], ent.second);
		}
		fclose(file);
	}
}

#endif
