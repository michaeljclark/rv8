//
//  riscv_config.cc
//

#include <cassert>
#include <cstring>
#include <cinttypes>
#include <cerrno>
#include <functional>
#include <algorithm>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <map>

#include <sys/stat.h>

#include "riscv-util.h"
#include "riscv-config-parser.h"
#include "riscv-config.h"


bool riscv_config::parse_value(std::string valstr, uint64_t &val)
{
	char *endptr = nullptr;
	valstr = replace(valstr, "_", "");
	if (valstr.find("0x") == 0) {
		val = strtoull(valstr.c_str() + 2, &endptr, 16);
	} else if (valstr.find("0b") == 0) {
		val = strtoull(valstr.c_str() + 2, &endptr, 2);
	} else if (valstr.find("0") == 0) {
		val = strtoull(valstr.c_str() + 1, &endptr, 8);
	} else {
		val = strtoull(valstr.c_str(), &endptr, 10);
	}
	return (endptr != valstr.c_str());
}

bool riscv_config::parse_address_range(std::string valstr, riscv_address_range_ptr range)
{
	auto range_comps = split(valstr, ":");
	if (range_comps.size() == 1) {
		if (!parse_value(range_comps[0], range->start)) return false;
		range->end = range->start;
	} else if (range_comps.size() == 2) {
		if (!parse_value(range_comps[0], range->start)) return false;
		if (!parse_value(range_comps[1], range->end)) return false;
	} else {
		return false;
	}
	return true;
}

std::string riscv_config::address_range_to_string(riscv_address_range_list &addr_list)
{
	std::stringstream ss;
	ss	<< "@";
	for (auto &addr : addr_list) {
		ss	<< " ";
		if (addr->start == addr->end) ss << format_string("0x%" PRIx64, addr->start);
		else ss << format_string("0x%" PRIx64 ":0x%" PRIx64, addr->start, addr->end);
	}
	ss	<< ";";
	return ss.str();
}

riscv_config::riscv_config()
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

std::string riscv_config::to_string()
{
	std::stringstream ss;
	output_platform(ss);
	output_plic(ss);
	output_pcie(ss);
	output_leds(ss);
	output_rtc(ss);
	output_ram(ss);
	output_uart(ss);
	output_core(ss);
	return ss.str();
}

void riscv_config::read(std::string filename)
{
	FILE *fp;
	struct stat statbuf;
	size_t len;
	std::vector<char> buf;
	
	if ((fp = fopen(filename.c_str(), "r")) == NULL) {
		panic("riscv_config: fopen: %s: %s", filename.c_str(), strerror(errno));
	}
	
	if (fstat(fileno(fp), &statbuf) < 0) {
		panic("riscv_config: fstat: %s", strerror(errno));
	}
	len = statbuf.st_size;
	
	buf.resize(len + 1);
	if (fread(buf.data(), 1, len, fp) != len) {
		panic("riscv_config: short read");
	}
	
	if (!parse(buf.data(), len)) {
		panic("riscv_config: parse error");
	}
	
	fclose(fp);
}

void riscv_config::symbol(const char *value, size_t vlen)
{
	line.push_back(std::string(value, vlen));
}

void riscv_config::start_block()
{
	riscv_config_line block_line = line;
	
	// look up  block record
	riscv_block_record rec;
	bool found = lookup_block_start_fn(line[0], rec);
	if (found) {
		if (rec.minargs == rec.maxargs && (int)line.size() != rec.minargs) {
			panic("riscv_config: %s requires %d argument(s)", line[0].c_str(), rec.minargs);
		} else if (rec.minargs > 0 && (int)line.size() < rec.minargs) {
			panic("riscv_config: %s requires at least %d argument(s)", line[0].c_str(), rec.minargs);
		} else if (rec.maxargs > 0 && (int)line.size() > rec.maxargs) {
			panic("riscv_config: %s requires no more than %d argument(s)", line[0].c_str(), rec.maxargs);
		}
		block.push_back(block_line);
		line.clear();
		rec.fn(block.back());
	} else {
		panic("riscv_config: unrecognized block: %s", line[0].c_str());
	}
}

void riscv_config::end_block()
{
	// look up  block record
	riscv_block_record rec;
	bool found = lookup_block_end_fn(block.back()[0], rec);
	if (found) {
		rec.fn(block.back());
	}
	block.pop_back();
	while (prefix.size() > block.size()) {
		prefix.pop_back();
	}
}

void riscv_config::end_statement()
{
	if (line.size() > 0)
	{
		// look up and invoke riscv_config function
		riscv_config_record rec;
		bool found = lookup_config_fn(line[0], rec);
		if (found) {
			if (rec.minargs == rec.maxargs && (int)line.size() != rec.minargs) {
				panic("riscv_config: %s requires %d argument(s)", line[0].c_str(), rec.minargs);
			} else if (rec.minargs > 0 && (int)line.size() < rec.minargs) {
				panic("riscv_config: %s requires at least %d argument(s)", line[0].c_str(), rec.minargs);
			} else if (rec.maxargs > 0 && (int)line.size() > rec.maxargs) {
				panic("riscv_config: %s requires no more than %d argument(s)", line[0].c_str(), rec.maxargs);
			}
			rec.fn(line);
		} else {
			panic("riscv_config: unrecognized directive: %s", line[0].c_str());
		}
	}
	line.clear();
}

void riscv_config::config_done()
{
}

void riscv_config::config_platform()
{
	block_start_fn_map["platform"] = {1, 1, [&] (riscv_config_line &line) {
		if (platform) panic("config must contain \"platform\" block");
		platform = std::make_shared<riscv_platform>();
	}};

	block_end_fn_map["platform"] = {0, 0, [&] (riscv_config_line &line) {
		if (!platform) panic("config must contain \"platform\" block");
	}};

	config_fn_map["platform.vendor"] = {2, 2, [&] (riscv_config_line &line) {
		platform->vendor = line[1];
	}};

	config_fn_map["platform.arch"] = {2, 2, [&] (riscv_config_line &line) {
		platform->arch = line[1];
	}};
}

void riscv_config::config_plic()
{
	block_start_fn_map["plic"] = {1, 1, [&] (riscv_config_line &line) {
		plic_list.push_back(std::make_shared<riscv_plic>());
	}};

	config_fn_map["plic.interface"] = {2, 2, [&] (riscv_config_line &line) {
		plic_list.back()->interface = line[1];
	}};

	config_fn_map["plic.ndevs"] = {2, 2, [&] (riscv_config_line &line) {
		if (!parse_value(line[1], plic_list.back()->ndevs)) {
			panic("riscv_config: invalid ndevs value: plic \"%s\": %s",
				plic_list.back()->interface.c_str(), line[1].c_str());
		}
	}};

	block_start_fn_map["plic.priority"] = {1, 1, [&] (riscv_config_line &line) {
		plic_list.back()->priority = std::make_shared<riscv_plic_priority>();
	}};

	config_fn_map["plic.priority.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: plic \"%s\" priority: %s",
				plic_list.back()->interface.c_str(), line[1].c_str());
		}
		plic_list.back()->priority->addr_list.push_back(range);
	}};

	block_start_fn_map["plic.pending"] = {1, 1, [&] (riscv_config_line &line) {
		plic_list.back()->pending = std::make_shared<riscv_plic_pending>();
	}};

	config_fn_map["plic.pending.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: plic \"%s\" pending: %s",
				plic_list.back()->interface.c_str(), line[1].c_str());
		}
		plic_list.back()->pending->addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#"] = {1, 1, [&] (riscv_config_line &line) {
		auto plic_node = std::make_shared<riscv_plic_node>();
		parse_value(line[0], plic_node->node_id);
		plic_list.back()->node_list.push_back(plic_node);
	}};

	block_start_fn_map["plic.#.#"] = {1, 1, [&] (riscv_config_line &line) {
		auto plic_hart = std::make_shared<riscv_plic_hart>();
		parse_value(line[0], plic_hart->hart_id);
		plic_list.back()->node_list.back()->hart_list.push_back(plic_hart);
	}};

	block_start_fn_map["plic.#.#.m"] = {1, 1, [&] (riscv_config_line &line) {
		auto plic_mode = std::make_shared<riscv_plic_mode>();
		plic_mode->mode_name = "m";
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.push_back(plic_mode);
	}};

	block_start_fn_map["plic.#.#.m.ie"] = {1, 1, [&] (riscv_config_line &line) {}};

	config_fn_map["plic.#.#.m.ie.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: plic \"%s\" node id %u hart id %u m mode ie: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ie_addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#.#.m.ctl"] = {1, 1, [&] (riscv_config_line &line) {}};

	config_fn_map["plic.#.#.m.ctl.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: plic \"%s\" node id %u hart id %u m mode ctl: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ctl_addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#.#.s"] = {1, 1, [&] (riscv_config_line &line) {
		auto plic_mode = std::make_shared<riscv_plic_mode>();
		plic_mode->mode_name = "s";
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.push_back(plic_mode);
	}};

	block_start_fn_map["plic.#.#.s.ie"] = {1, 1, [&] (riscv_config_line &line) {}};

	config_fn_map["plic.#.#.s.ie.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: plic \"%s\" node id %u hart id %u s mode ie: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ie_addr_list.push_back(range);
	}};

	block_start_fn_map["plic.#.#.s.ctl"] = {1, 1, [&] (riscv_config_line &line) {}};

	config_fn_map["plic.#.#.s.ctl.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: plic \"%s\" node id %u hart id %u s mode ctl: %s",
				plic_list.back()->interface.c_str(),
				plic_list.back()->node_list.back()->node_id,
				plic_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
		plic_list.back()->node_list.back()->hart_list.back()->mode_list.back()->ctl_addr_list.push_back(range);
	}};
}

void riscv_config::config_pcie()
{
	block_start_fn_map["pcie"] = {1, 1, [&] (riscv_config_line &line) {
		pcie_list.push_back(std::make_shared<riscv_pcie>());
	}};

	config_fn_map["pcie.interface"] = {2, 2, [&] (riscv_config_line &line) {
		pcie_list.back()->interface = line[1];
	}};

	block_start_fn_map["pcie.bus"] = {1, 1, [&] (riscv_config_line &line) {
		auto bus = std::make_shared<riscv_pcie_bus>();
		pcie_list.back()->bus_list.push_back(bus);
	}};

	config_fn_map["pcie.bus.@"] = {2, -1, [&] (riscv_config_line &line) {
		for (size_t i = 1; i < line.size(); i++) {
			auto range = std::make_shared<riscv_address_range>();
			if (!parse_address_range(line[i], range)) {
				panic("riscv_config: invalid bus address range: pcie \"%s\": %s",
					pcie_list.back()->interface.c_str(),
					line[i].c_str());
			}
			pcie_list.back()->bus_list.back()->addr_list.push_back(range);
		}
	}};

	config_fn_map["pcie.bus.bus"] = {2, 2, [&] (riscv_config_line &line) {
		auto comps = split(line[1], ":");
		if (comps.size() != 2 ||
			!parse_value(comps[0], pcie_list.back()->bus_list.back()->bus_pair.first) ||
			!parse_value(comps[1], pcie_list.back()->bus_list.back()->bus_pair.second)) {
			panic("riscv_config: invalid bus spec: pcie \"%s\": %s",
				pcie_list.back()->interface.c_str(),
				line[1].c_str());
		}
	}};

	block_start_fn_map["pcie.bridge"] = {1, 1, [&] (riscv_config_line &line) {
		auto bridge = std::make_shared<riscv_pcie_bridge>();
		pcie_list.back()->bridge_list.push_back(bridge);
	}};

	config_fn_map["pcie.bridge.@"] = {2, -1, [&] (riscv_config_line &line) {
		for (size_t i = 1; i < line.size(); i++) {
			auto range = std::make_shared<riscv_address_range>();
			if (!parse_address_range(line[i], range)) {
				panic("riscv_config: invalid bridge address range: pcie \"%s\": %s",
					pcie_list.back()->interface.c_str(),
					line[i].c_str());
			}
			pcie_list.back()->bridge_list.back()->addr_list.push_back(range);
		}
	}};

	config_fn_map["pcie.bridge.bus"] = {2, 2, [&] (riscv_config_line &line) {
		if (!parse_value(line[1], pcie_list.back()->bridge_list.back()->bus_id)) {
			panic("riscv_config: invalid bridge bus number: pcie \"%s\": %s",
				pcie_list.back()->interface.c_str(),
				line[1].c_str());
		}
	}};

	config_fn_map["pcie.bridge.irq"] = {2, 2, [&] (riscv_config_line &line) {
		if (!parse_value(line[1], pcie_list.back()->bridge_list.back()->irq_id)) {
			panic("riscv_config: invalid bridge irq number: pcie \"%s\": %s",
				pcie_list.back()->interface.c_str(),
				line[1].c_str());
		}
	}};
}

void riscv_config::config_leds()
{
	block_start_fn_map["leds"] = {1, 1, [&] (riscv_config_line &line) {
		leds_list.push_back(std::make_shared<riscv_leds>());
	}};

	config_fn_map["leds.interface"] = {2, 2, [&] (riscv_config_line &line) {
		leds_list.back()->interface = line[1];
	}};

	config_fn_map["leds.ngpio"] = {2, 2, [&] (riscv_config_line &line) {
		if (!parse_value(line[1], leds_list.back()->ngpio)) {
			panic("riscv_config: invalid ngpio value: leds \"%s\": %s",
				leds_list.back()->interface.c_str(), line[1].c_str());
		}
	}};

	config_fn_map["leds.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: leds \"%s\": %s",
				leds_list.back()->interface.c_str(), line[1].c_str());
		}
		leds_list.back()->addr_list.push_back(range);
	}};
}

void riscv_config::config_rtc()
{
	block_start_fn_map["rtc"] = {1, 1, [&] (riscv_config_line &line) {
		rtc_list.push_back(std::make_shared<riscv_rtc>());
	}};

	config_fn_map["rtc.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: rtc: %s",
				line[1].c_str());
		}
		rtc_list.back()->addr_list.push_back(range);
	}};
}

void riscv_config::config_ram()
{
	block_start_fn_map["ram"] = {1, 1, [&] (riscv_config_line &line) {
		ram_list.push_back(std::make_shared<riscv_ram>());
	}};

	block_start_fn_map["ram.#"] = {1, 1, [&] (riscv_config_line &line) {
		auto ram_node = std::make_shared<riscv_ram_node>();
		parse_value(line[0], ram_node->node_id);
		ram_list.back()->node_list.push_back(ram_node);
	}};

	config_fn_map["ram.#.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: ram node id %u: %s",
				ram_list.back()->node_list.back()->node_id,
				line[1].c_str());
		}
		ram_list.back()->node_list.back()->addr_list.push_back(range);
	}};
}

void riscv_config::config_uart()
{
	block_start_fn_map["uart"] = {1, 1, [&] (riscv_config_line &line) {
		uart_list.push_back(std::make_shared<riscv_uart>());
	}};

	config_fn_map["uart.@"] = {2, 2, [&] (riscv_config_line &line) {
		auto range = std::make_shared<riscv_address_range>();
		if (!parse_address_range(line[1], range)) {
			panic("riscv_config: invalid address range: uart: %s",
				line[1].c_str());
		}
		uart_list.back()->addr_list.push_back(range);
	}};
}

void riscv_config::config_core()
{
	block_start_fn_map["core"] = {1, 1, [&] (riscv_config_line &line) {
		core_list.push_back(std::make_shared<riscv_core>());
	}};

	block_start_fn_map["core.#"] = {1, 1, [&] (riscv_config_line &line) {
		auto core_node = std::make_shared<riscv_core_node>();
		parse_value(line[0], core_node->node_id);
		core_list.back()->node_list.push_back(core_node);
	}};

	block_start_fn_map["core.#.#"] = {1, 1, [&] (riscv_config_line &line) {
		auto core_hart = std::make_shared<riscv_core_hart>();
		parse_value(line[0], core_hart->hart_id);
		core_list.back()->node_list.back()->hart_list.push_back(core_hart);
	}};

	config_fn_map["core.#.#.isa"] = {2, 2, [&] (riscv_config_line &line) {
		core_list.back()->node_list.back()->hart_list.back()->isa = line[1];
	}};

	config_fn_map["core.#.#.timecmp"] = {2, 2, [&] (riscv_config_line &line) {
		if (!parse_value(line[1], core_list.back()->node_list.back()->hart_list.back()->timecmp)) {
			panic("riscv_config: invalid timecmp value: core node id %u: hart id %u: %s",
				core_list.back()->node_list.back()->node_id,
				core_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
	}};

	config_fn_map["core.#.#.ipi"] = {2, 2, [&] (riscv_config_line &line) {
		if (!parse_value(line[1], core_list.back()->node_list.back()->hart_list.back()->ipi)) {
			panic("riscv_config: invalid ipi value: core node id %u: hart id %u: %s",
				core_list.back()->node_list.back()->node_id,
				core_list.back()->node_list.back()->hart_list.back()->hart_id,
				line[1].c_str());
		}
	}};
}

void riscv_config::output_platform(std::stringstream &ss)
{
	if (!platform) panic("config must contain \"platform\" block");
	ss	<< "platform {\n"
		<< "\tvendor " << platform->vendor << ";\n" 
		<< "\tarch   " << platform->arch << ";\n" 
		<<"};\n";
}

void riscv_config::output_plic(std::stringstream &ss)
{
	for (auto &plic : plic_list) {
		ss	<< "plic {\n"
			<< "\tinterface " << plic->interface << ";\n"
			<< "\tndevs " << plic->ndevs << ";\n";
		if (plic->priority) {
			ss	<< "\tpriority { " << address_range_to_string(plic->priority->addr_list) << " };\n";
		}
		if (plic->pending) {
			ss	<< "\tpending  { " << address_range_to_string(plic->pending->addr_list) << " };\n";
		}
		for (auto &node : plic->node_list) {
			ss	<< "\t" << node->node_id << " {\n";
			for (auto &hart : node->hart_list) {
				ss	<< "\t\t" << hart->hart_id << " {\n";
				for (auto &mode : hart->mode_list) {
					ss	<< "\t\t\t" << mode->mode_name << " {\n";
					ss	<< "\t\t\t\tie  { " << address_range_to_string(mode->ie_addr_list) << " };\n";
					ss	<< "\t\t\t\tctl { " << address_range_to_string(mode->ctl_addr_list) << " };\n";
					ss	<< "\t\t\t}\n";
				}
				ss	<< "\t\t}\n";
			}
			ss	<< "\t}\n";
		}
		ss	<< "};\n";
	}
}

void riscv_config::output_pcie(std::stringstream &ss)
{
	for (auto &pcie : pcie_list) {
		ss	<< "pcie {\n"
			<< "\tinterface " << pcie->interface << ";\n";
		for (auto &bus : pcie->bus_list) {
			ss	<< "\tbus {\n"
				<< "\t\t" << address_range_to_string(bus->addr_list) << "\n"
				<< "\t\tbus " << bus->bus_pair.first << ":" << bus->bus_pair.second << ";\n"
				<< "\t};\n";
		}
		for (auto &bridge : pcie->bridge_list) {
			ss	<< "\tbridge {\n"
				<< "\t\t" << address_range_to_string(bridge->addr_list) << "\n"
				<< "\t\tbus " << bridge->bus_id << ";\n"
				<< "\t\tirq " << bridge->irq_id << ";\n"
				<< "\t};\n";
		}
		ss	<< "};\n";
	}
}

void riscv_config::output_leds(std::stringstream &ss)
{
	for (auto &leds : leds_list) {
		ss	<< "leds {\n"
			<< "\tinterface " << leds->interface << ";\n"
			<< "\tngpio " << leds->ngpio << ";\n" 
			<< "\t" << address_range_to_string(leds->addr_list) << "\n"
			<< "};\n";
	}
}

void riscv_config::output_rtc(std::stringstream &ss)
{
	for (auto &rtc : rtc_list) {
		ss	<< "rtc {\n"
			<< "\t" << address_range_to_string(rtc->addr_list) << "\n"
			<< "};\n";
	}
}

void riscv_config::output_ram(std::stringstream &ss)
{
	for (auto &ram : ram_list) {
		ss	<< "ram {\n";
		for (auto &ram_node : ram->node_list) {
			ss	<< "\t" << ram_node->node_id << " { " << address_range_to_string(ram_node->addr_list) << " };\n";
		}
		ss	<< "};\n";
	}
}

void riscv_config::output_uart(std::stringstream &ss)
{
	for (auto &uart : uart_list) {
		ss	<< "uart {\n"
			<< "\t" << address_range_to_string(uart->addr_list) << "\n"
			<< "};\n";
	}
}

void riscv_config::output_core(std::stringstream &ss)
{
	for (auto &core : core_list) {
		ss	<< "core {\n";
		for (auto &core_node : core->node_list) {
			ss	<< "\t" << core_node->node_id << " {\n";
			for (auto &core_hart : core_node->hart_list) {
				ss	<< "\t\t" << core_hart->hart_id << " {\n"
					<< "\t\t\tisa     " << core_hart->isa << ";\n"
					<< "\t\t\ttimecmp " << format_string("0x%" PRIx64, core_hart->timecmp) << ";\n"
					<< "\t\t\tipi     " << format_string("0x%" PRIx64, core_hart->ipi) << ";\n"
					<< "\t\t};\n";
			}
			ss	<< "\t};\n";
		}
		ss	<< "};\n";
	}
}
