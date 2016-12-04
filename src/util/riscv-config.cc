//
//  riscv_config.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cassert>
#include <cerrno>
#include <cmath>
#include <cctype>
#include <cwchar>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <limits>
#include <array>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <map>
#include <type_traits>
#include <functional>
#include <algorithm>

#include <sys/stat.h>

#include "riscv-types.h"
#include "riscv-util.h"
#include "riscv-config-parser.h"
#include "riscv-config.h"
#include "riscv-types.h"
#include "riscv-fmt.h"

using namespace riscv;

bool config::parse_address_range(std::string valstr, address_range_ptr range)
{
	auto range_comps = split(valstr, ":");
	if (range_comps.size() == 1) {
		if (!parse_integral(range_comps[0], range->start)) return false;
		range->end = range->start;
	} else if (range_comps.size() == 2) {
		if (!parse_integral(range_comps[0], range->start)) return false;
		if (!parse_integral(range_comps[1], range->end)) return false;
	} else {
		return false;
	}
	return true;
}

std::string config::address_range_to_string(address_range_list &addr_list)
{
	std::string s;
	sprintf(s, "@");
	for (auto &addr : addr_list) {
		sprintf(s, " ");
		if (addr->start == addr->end) sprintf(s, "0x%x", addr->start);
		else sprintf(s, "0x%x:0x%x", addr->start, addr->end);
	}
	sprintf(s, ";");
	return s;
}

config::config()
{
	config_platform();
	config_plic();
	config_pcie();
	config_leds();
	config_rtc();
	config_ram();
	config_uart();
	config_core();
}

std::string config::to_string()
{
	std::string s;
	output_platform(s);
	output_plic(s);
	output_pcie(s);
	output_leds(s);
	output_rtc(s);
	output_ram(s);
	output_uart(s);
	output_core(s);
	return s;
}

void config::read(std::string filename)
{
	FILE *fp;
	struct stat statbuf;
	size_t len;
	std::vector<char> buf;
	
	if ((fp = fopen(filename.c_str(), "r")) == NULL) {
		panic("config: fopen: %s: %s", filename.c_str(), strerror(errno));
	}
	
	if (fstat(fileno(fp), &statbuf) < 0) {
		panic("config: fstat: %s", strerror(errno));
	}
	len = statbuf.st_size;
	
	buf.resize(len + 1);
	if (fread(buf.data(), 1, len, fp) != len) {
		panic("config: short read");
	}
	
	if (!parse(buf.data(), len)) {
		panic("config: parse error");
	}
	
	fclose(fp);
}

void config::symbol(const char *value, size_t vlen)
{
	line.push_back(std::string(value, vlen));
}

void config::start_block()
{
	config_line block_line = line;
	
	// look up  block record
	block_record rec;
	bool found = lookup_block_start_fn(line[0], rec);
	if (found) {
		if (rec.minargs == rec.maxargs && (int)line.size() != rec.minargs) {
			panic("config: %s requires %d argument(s)", line[0].c_str(), rec.minargs);
		} else if (rec.minargs > 0 && (int)line.size() < rec.minargs) {
			panic("config: %s requires at least %d argument(s)", line[0].c_str(), rec.minargs);
		} else if (rec.maxargs > 0 && (int)line.size() > rec.maxargs) {
			panic("config: %s requires no more than %d argument(s)", line[0].c_str(), rec.maxargs);
		}
		block.push_back(block_line);
		line.clear();
		rec.fn(block.back());
	} else {
		panic("config: unrecognized block: %s", line[0].c_str());
	}
}

void config::end_block()
{
	// look up  block record
	block_record rec;
	bool found = lookup_block_end_fn(block.back()[0], rec);
	if (found) {
		rec.fn(block.back());
	}
	block.pop_back();
	while (prefix.size() > block.size()) {
		prefix.pop_back();
	}
}

void config::end_statement()
{
	if (line.size() > 0)
	{
		// look up and invoke config function
		config_record rec;
		bool found = lookup_config_fn(line[0], rec);
		if (found) {
			if (rec.minargs == rec.maxargs && (int)line.size() != rec.minargs) {
				panic("config: %s requires %d argument(s)", line[0].c_str(), rec.minargs);
			} else if (rec.minargs > 0 && (int)line.size() < rec.minargs) {
				panic("config: %s requires at least %d argument(s)", line[0].c_str(), rec.minargs);
			} else if (rec.maxargs > 0 && (int)line.size() > rec.maxargs) {
				panic("config: %s requires no more than %d argument(s)", line[0].c_str(), rec.maxargs);
			}
			rec.fn(line);
		} else {
			panic("config: unrecognized directive: %s", line[0].c_str());
		}
	}
	line.clear();
}

void config::config_done()
{
}

void config::config_platform()
{
	block_start_fn_map["platform"] = {1, 1, [&] (config_line &line) {
		if (platform) panic("config must contain \"platform\" block");
		platform = std::make_shared<riscv::platform>();
	}};

	block_end_fn_map["platform"] = {0, 0, [&] (config_line &line) {
		if (!platform) panic("config must contain \"platform\" block");
	}};

	config_fn_map["platform.vendor"] = {2, 2, [&] (config_line &line) {
		platform->vendor = line[1];
	}};

	config_fn_map["platform.arch"] = {2, 2, [&] (config_line &line) {
		platform->arch = line[1];
	}};
}

void config::config_plic()
{
	block_start_fn_map["plic"] = {1, 1, [&] (config_line &line) {
		plic_list.push_back(std::make_shared<riscv::plic>());
	}};

	config_fn_map["plic.interface"] = {2, 2, [&] (config_line &line) {
		plic_list.back()->interface = line[1];
	}};

	config_fn_map["plic.ndevs"] = {2, 2, [&] (config_line &line) {
		if (!parse_integral(line[1], plic_list.back()->ndevs)) {
			panic("config: invalid ndevs value: plic \"%s\": %s",
				plic_list.back()->interface.c_str(), line[1].c_str());
		}
	}};

	block_start_fn_map["plic.priority"] = {1, 1, [&] (config_line &line) {
		plic_list.back()->priority = std::make_shared<riscv::plic_priority>();
	}};

	config_fn_map["plic.priority.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: plic \"%s\" priority: %s",
				plic_list.back()->interface.c_str(), line[1].c_str());
		}
		plic_list.back()->priority->addr_list.push_back(range);
	}};

	block_start_fn_map["plic.pending"] = {1, 1, [&] (config_line &line) {
		plic_list.back()->pending = std::make_shared<riscv::plic_pending>();
	}};

	config_fn_map["plic.pending.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: plic \"%s\" pending: %s",
				plic_list.back()->interface.c_str(), line[1].c_str());
		}
		plic_list.back()->pending->addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#"] = {1, 1, [&] (config_line &line) {
		auto plic_node = std::make_shared<riscv::plic_node>();
		parse_integral(line[0], plic_node->node_id);
		plic_list.back()->node_list.push_back(plic_node);
	}};

	block_start_fn_map["plic.#.#"] = {1, 1, [&] (config_line &line) {
		auto plic_hart = std::make_shared<riscv::plic_hart>();
		parse_integral(line[0], plic_hart->hart_id);
		plic_list.back()->node_list.back()->hart_list.push_back(plic_hart);
	}};

	block_start_fn_map["plic.#.#.m"] = {1, 1, [&] (config_line &line) {
		auto plic_mode = std::make_shared<riscv::plic_mode>();
		plic_mode->mode_name = "m";
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.push_back(plic_mode);
	}};

	block_start_fn_map["plic.#.#.m.ie"] = {1, 1, [&] (config_line &line) {}};

	config_fn_map["plic.#.#.m.ie.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: plic \"%s\" node id %u hart id %u m mode ie: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ie_addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#.#.m.ctl"] = {1, 1, [&] (config_line &line) {}};

	config_fn_map["plic.#.#.m.ctl.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: plic \"%s\" node id %u hart id %u m mode ctl: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ctl_addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#.#.s"] = {1, 1, [&] (config_line &line) {
		auto plic_mode = std::make_shared<riscv::plic_mode>();
		plic_mode->mode_name = "s";
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.push_back(plic_mode);
	}};

	block_start_fn_map["plic.#.#.s.ie"] = {1, 1, [&] (config_line &line) {}};

	config_fn_map["plic.#.#.s.ie.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: plic \"%s\" node id %u hart id %u s mode ie: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ie_addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#.#.s.ctl"] = {1, 1, [&] (config_line &line) {}};

	config_fn_map["plic.#.#.s.ctl.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: plic \"%s\" node id %u hart id %u s mode ctl: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ctl_addr_list.push_back(range);
	}};
}

void config::config_pcie()
{
	block_start_fn_map["pcie"] = {1, 1, [&] (config_line &line) {
		pcie_list.push_back(std::make_shared<riscv::pcie>());
	}};

	config_fn_map["pcie.interface"] = {2, 2, [&] (config_line &line) {
		pcie_list.back()->interface = line[1];
	}};

	block_start_fn_map["pcie.bus"] = {1, 1, [&] (config_line &line) {
		auto bus = std::make_shared<riscv::pcie_bus>();
		pcie_list.back()->bus_list.push_back(bus);
	}};

	config_fn_map["pcie.bus.@"] = {2, -1, [&] (config_line &line) {
		for (size_t i = 1; i < line.size(); i++) {
			auto range = std::make_shared<riscv::address_range>();
			if (!parse_address_range(line[i], range)) {
				panic("config: invalid bus address range: pcie \"%s\": %s",
					pcie_list.back()->interface.c_str(),
					line[i].c_str());
			}
			pcie_list.back()->bus_list.back()->addr_list.push_back(range);
		}
	}};

	config_fn_map["pcie.bus.bus"] = {2, 2, [&] (config_line &line) {
		auto comps = split(line[1], ":");
		if (comps.size() != 2 ||
			!parse_integral(comps[0], pcie_list.back()->bus_list.back()->bus_pair.first) ||
			!parse_integral(comps[1], pcie_list.back()->bus_list.back()->bus_pair.second)) {
			panic("config: invalid bus spec: pcie \"%s\": %s",
				pcie_list.back()->interface.c_str(),
				line[1].c_str());
		}
	}};

	block_start_fn_map["pcie.bridge"] = {1, 1, [&] (config_line &line) {
		auto bridge = std::make_shared<riscv::pcie_bridge>();
		pcie_list.back()->bridge_list.push_back(bridge);
	}};

	config_fn_map["pcie.bridge.@"] = {2, -1, [&] (config_line &line) {
		for (size_t i = 1; i < line.size(); i++) {
			auto range = std::make_shared<riscv::address_range>();
			if (!parse_address_range(line[i], range)) {
				panic("config: invalid bridge address range: pcie \"%s\": %s",
					pcie_list.back()->interface.c_str(),
					line[i].c_str());
			}
			pcie_list.back()->bridge_list.back()->addr_list.push_back(range);
		}
	}};

	config_fn_map["pcie.bridge.bus"] = {2, 2, [&] (config_line &line) {
		if (!parse_integral(line[1], pcie_list.back()->bridge_list.back()->bus_id)) {
			panic("config: invalid bridge bus number: pcie \"%s\": %s",
				pcie_list.back()->interface.c_str(),
				line[1].c_str());
		}
	}};

	config_fn_map["pcie.bridge.irq"] = {2, 2, [&] (config_line &line) {
		if (!parse_integral(line[1], pcie_list.back()->bridge_list.back()->irq_id)) {
			panic("config: invalid bridge irq number: pcie \"%s\": %s",
				pcie_list.back()->interface.c_str(),
				line[1].c_str());
		}
	}};
}

void config::config_leds()
{
	block_start_fn_map["leds"] = {1, 1, [&] (config_line &line) {
		leds_list.push_back(std::make_shared<riscv::leds>());
	}};

	config_fn_map["leds.interface"] = {2, 2, [&] (config_line &line) {
		leds_list.back()->interface = line[1];
	}};

	config_fn_map["leds.ngpio"] = {2, 2, [&] (config_line &line) {
		if (!parse_integral(line[1], leds_list.back()->ngpio)) {
			panic("config: invalid ngpio value: leds \"%s\": %s",
				leds_list.back()->interface.c_str(), line[1].c_str());
		}
	}};

	config_fn_map["leds.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: leds \"%s\": %s",
				leds_list.back()->interface.c_str(), line[1].c_str());
		}
		leds_list.back()->addr_list.push_back(range);
	}};
}

void config::config_rtc()
{
	block_start_fn_map["rtc"] = {1, 1, [&] (config_line &line) {
		rtc_list.push_back(std::make_shared<riscv::rtc>());
	}};

	config_fn_map["rtc.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: rtc: %s",
				line[1].c_str());
		}
		rtc_list.back()->addr_list.push_back(range);
	}};
}

void config::config_ram()
{
	block_start_fn_map["ram"] = {1, 1, [&] (config_line &line) {
		ram_list.push_back(std::make_shared<riscv::ram>());
	}};

	block_start_fn_map["ram.#"] = {1, 1, [&] (config_line &line) {
		auto ram_node = std::make_shared<riscv::ram_node>();
		parse_integral(line[0], ram_node->node_id);
		ram_list.back()->node_list.push_back(ram_node);
	}};

	config_fn_map["ram.#.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: ram node id %u: %s",
				ram_list.back()->node_list.back()->node_id,
				line[1].c_str());
		}
		ram_list.back()->node_list.back()->addr_list.push_back(range);
	}};
}

void config::config_uart()
{
	block_start_fn_map["uart"] = {1, 1, [&] (config_line &line) {
		uart_list.push_back(std::make_shared<riscv::uart>());
	}};

	config_fn_map["uart.@"] = {2, 2, [&] (config_line &line) {
		auto range = std::make_shared<riscv::address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("config: invalid address range: uart: %s",
				line[1].c_str());
		}
		uart_list.back()->addr_list.push_back(range);
	}};
}

void config::config_core()
{
	block_start_fn_map["core"] = {1, 1, [&] (config_line &line) {
		core_list.push_back(std::make_shared<riscv::core>());
	}};

	block_start_fn_map["core.#"] = {1, 1, [&] (config_line &line) {
		auto core_node = std::make_shared<riscv::core_node>();
		parse_integral(line[0], core_node->node_id);
		core_list.back()->node_list.push_back(core_node);
	}};

	block_start_fn_map["core.#.#"] = {1, 1, [&] (config_line &line) {
		auto core_hart = std::make_shared<riscv::core_hart>();
		parse_integral(line[0], core_hart->hart_id);
		core_list.back()->node_list.back()->hart_list.push_back(core_hart);
	}};

	config_fn_map["core.#.#.isa"] = {2, 2, [&] (config_line &line) {
		core_list.back()->node_list.back()->hart_list.back()->isa = line[1];
	}};

	config_fn_map["core.#.#.timecmp"] = {2, 2, [&] (config_line &line) {
		if (!parse_integral(line[1], core_list.back()->node_list.back()->hart_list.back()->timecmp)) {
			panic("config: invalid timecmp value: core node id %u: hart id %u: %s",
				core_list.back()->node_list.back()->node_id,
				core_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
	}};

	config_fn_map["core.#.#.ipi"] = {2, 2, [&] (config_line &line) {
		if (!parse_integral(line[1], core_list.back()->node_list.back()->hart_list.back()->ipi)) {
			panic("config: invalid ipi value: core node id %u: hart id %u: %s",
				core_list.back()->node_list.back()->node_id,
				core_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
	}};
}

void config::output_platform(std::string &s)
{
	if (!platform) panic("config must contain \"platform\" block");
	sprintf(s, "platform {\n\tvendor %s;\n\tarch   %s;\n};\n",
		platform->vendor, platform->arch);
}

void config::output_plic(std::string &s)
{
	for (auto &plic : plic_list) {
		sprintf(s,
			"plic {\n"
			"\tinterface %s;\n"
			"\tndevs %u;\n",
			plic->interface, plic->ndevs);
		if (plic->priority) {
			sprintf(s, "\tpriority { %s };\n",
				address_range_to_string(plic->priority->addr_list));
		}
		if (plic->pending) {
			sprintf(s, "\tpending  { %s };\n",
				address_range_to_string(plic->pending->addr_list));
		}
		for (auto &node : plic->node_list) {
			sprintf(s, "\t%u {\n", node->node_id);
			for (auto &hart : node->hart_list) {
				sprintf(s, "\t\t%u {\n", hart->hart_id);
				for (auto &mode : hart->mode_list) {
					sprintf(s,
						"\t\t\t%s {\n"
						"\t\t\t\tie  { %s };\n"
						"\t\t\t\tctl { %s };\n"
						"\t\t\t};\n",
						mode->mode_name,
						address_range_to_string(mode->ie_addr_list),
						address_range_to_string(mode->ctl_addr_list));
				}
				sprintf(s, "\t\t};\n");
			}
			sprintf(s, "\t};\n");
		}
		sprintf(s, "};\n");
	}
}

void config::output_pcie(std::string &s)
{
	for (auto &pcie : pcie_list) {
		sprintf(s, "pcie {\n\tinterface %s;\n",
			pcie->interface);
		for (auto &bus : pcie->bus_list) {
			sprintf(s,
				"\tbus {\n"
				"\t\t%s\n"
				"\t\tbus %u:%u;\n"
				"\t};\n",
				address_range_to_string(bus->addr_list),
				bus->bus_pair.first,
				bus->bus_pair.second);
		}
		for (auto &bridge : pcie->bridge_list) {
			sprintf(s,
				"\tbridge {\n"
				"\t\t%s\n"
				"\t\tbus %u;\n"
				"\t\tirq %u;\n"
				"\t};\n",
				address_range_to_string(bridge->addr_list),
				bridge->bus_id,
				bridge->irq_id);
		}
		sprintf(s, "};\n");
	}
}

void config::output_leds(std::string &s)
{
	for (auto &leds : leds_list) {
		sprintf(s,
			"leds {\n"
			"\tinterface %s;\n"
			"\tngpio %u;\n"
			"\t%s\n};\n",
			leds->interface, leds->ngpio,
			address_range_to_string(leds->addr_list));
	}
}

void config::output_rtc(std::string &s)
{
	for (auto &rtc : rtc_list) {
		sprintf(s,
			"rtc {\n"
			"\t%s\n"
			"};\n",
			address_range_to_string(rtc->addr_list));
	}
}

void config::output_ram(std::string &s)
{
	for (auto &ram : ram_list) {
		sprintf(s, "ram {\n");
		for (auto &ram_node : ram->node_list) {
			sprintf(s, "\t%u { %s };\n",
				ram_node->node_id,
				address_range_to_string(ram_node->addr_list));
		}
		sprintf(s, "};\n");
	}
}

void config::output_uart(std::string &s)
{
	for (auto &uart : uart_list) {
		sprintf(s, "uart {\n\t%s\n};\n",
			address_range_to_string(uart->addr_list));
	}
}

void config::output_core(std::string &s)
{
	for (auto &core : core_list) {
		sprintf(s, "core {\n");
		for (auto &core_node : core->node_list) {
			sprintf(s, "\t%u {\n", core_node->node_id);
			for (auto &core_hart : core_node->hart_list) {
				sprintf(s,
					"\t\t%u {\n"
					"\t\t\tisa     %s;\n"
					"\t\t\ttimecmp 0x%x;\n"
					"\t\t\tipi     0x%x;\n"
					"\t\t};\n",
					core_hart->hart_id, core_hart->isa,
					core_hart->timecmp, core_hart->ipi);
			}
			sprintf(s, "\t};\n");
		}
		sprintf(s, "};\n");
	}
}
