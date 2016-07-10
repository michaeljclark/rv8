//
//  riscv-config.h
//

#ifndef riscv_config_h
#define riscv_config_h

/* Generic configuration model */

struct riscv_config;
struct riscv_config_record;
struct riscv_block_record;

typedef std::shared_ptr<riscv_config> riscv_config_ptr;
typedef std::vector<std::string> riscv_config_line;
typedef std::vector<riscv_config_line> riscv_block_stack;
typedef std::function<void(riscv_config_line&)>riscv_config_function;
typedef std::map<std::string,riscv_config_record> riscv_config_function_map;
typedef std::function<void(riscv_config_line&)>riscv_block_function;
typedef std::map<std::string,riscv_block_record> riscv_block_function_map;

/* RISC-V configuration model */

struct riscv_address_range;
struct riscv_platform;
struct riscv_plic;
struct riscv_plic_priority;
struct riscv_plic_pending;
struct riscv_plic_node;
struct riscv_plic_hart;
struct riscv_plic_mode;
struct riscv_pcie;
struct riscv_pcie_bus;
struct riscv_pcie_bridge;
struct riscv_leds;
struct riscv_rtc;
struct riscv_ram;
struct riscv_ram_node;
struct riscv_uart;
struct riscv_core;
struct riscv_core_node;
struct riscv_core_hart;

typedef std::shared_ptr<riscv_address_range> riscv_address_range_ptr;
typedef std::vector<riscv_address_range_ptr> riscv_address_range_list;
typedef std::shared_ptr<riscv_platform> riscv_platform_ptr;
typedef std::shared_ptr<riscv_plic_priority> riscv_plic_priority_ptr;
typedef std::shared_ptr<riscv_plic_pending> riscv_plic_pending_ptr;
typedef std::shared_ptr<riscv_plic> riscv_plic_ptr;
typedef std::vector<riscv_plic_ptr> riscv_plic_list;
typedef std::shared_ptr<riscv_plic_node> riscv_plic_node_ptr;
typedef std::vector<riscv_plic_node_ptr> riscv_plic_node_list;
typedef std::shared_ptr<riscv_plic_hart> riscv_plic_hart_ptr;
typedef std::vector<riscv_plic_hart_ptr> riscv_plic_hart_list;
typedef std::shared_ptr<riscv_plic_mode> riscv_plic_mode_ptr;
typedef std::vector<riscv_plic_mode_ptr> riscv_plic_mode_list;
typedef std::shared_ptr<riscv_pcie> riscv_pcie_ptr;
typedef std::vector<riscv_pcie_ptr> riscv_pcie_list;
typedef std::shared_ptr<riscv_pcie_bus> riscv_pcie_bus_ptr;
typedef std::vector<riscv_pcie_bus_ptr> riscv_pcie_bus_list;
typedef std::shared_ptr<riscv_pcie_bridge> riscv_pcie_bridge_ptr;
typedef std::vector<riscv_pcie_bridge_ptr> riscv_pcie_bridge_list;
typedef std::shared_ptr<riscv_leds> riscv_leds_ptr;
typedef std::vector<riscv_leds_ptr> riscv_leds_list;
typedef std::shared_ptr<riscv_rtc> riscv_rtc_ptr;
typedef std::vector<riscv_rtc_ptr> riscv_rtc_list;
typedef std::shared_ptr<riscv_ram> riscv_ram_ptr;
typedef std::vector<riscv_ram_ptr> riscv_ram_list;
typedef std::shared_ptr<riscv_ram_node> riscv_ram_node_ptr;
typedef std::vector<riscv_ram_node_ptr> riscv_ram_node_list;
typedef std::shared_ptr<riscv_uart> riscv_uart_ptr;
typedef std::vector<riscv_uart_ptr> riscv_uart_list;
typedef std::shared_ptr<riscv_core> riscv_core_ptr;
typedef std::vector<riscv_core_ptr> riscv_core_list;
typedef std::shared_ptr<riscv_core_node> riscv_core_node_ptr;
typedef std::vector<riscv_core_node_ptr> riscv_core_node_list;
typedef std::shared_ptr<riscv_core_hart> riscv_core_hart_ptr;
typedef std::vector<riscv_core_hart_ptr> riscv_core_hart_list;

struct riscv_config_record
{
	int minargs;
	int maxargs;
	riscv_config_function fn;
};

struct riscv_block_record
{
	int minargs;
	int maxargs;
	riscv_block_function fn;
};

struct riscv_address_range
{
	uint64_t start;
	uint64_t end;
};

struct riscv_platform
{
	std::string vendor;
	std::string arch;
};

struct riscv_plic
{
	std::string interface;
	uint64_t ndevs;
	riscv_plic_priority_ptr priority;
	riscv_plic_pending_ptr pending;
	riscv_plic_node_list node_list;
};

struct riscv_plic_priority
{
	riscv_address_range_list addr_list;
};

struct riscv_plic_pending
{
	riscv_address_range_list addr_list;
};

struct riscv_plic_node
{
	uint64_t node_id;
	riscv_plic_hart_list hart_list;
};

struct riscv_plic_hart
{
	uint64_t hart_id;
	riscv_plic_mode_list mode_list;
};

struct riscv_plic_mode
{
	std::string mode_name;
	riscv_address_range_list ie_addr_list;
	riscv_address_range_list ctl_addr_list;
};

struct riscv_pcie
{
	std::string interface;
	riscv_pcie_bus_list bus_list;
	riscv_pcie_bridge_list bridge_list;
};

struct riscv_pcie_bus
{
	riscv_address_range_list addr_list;
	std::pair<uint64_t,uint64_t> bus_pair;
};

struct riscv_pcie_bridge
{
	riscv_address_range_list addr_list;
	uint64_t bus_id;
	uint64_t irq_id;
};

struct riscv_leds
{
	std::string interface;
	uint64_t ngpio;
	riscv_address_range_list addr_list;
};

struct riscv_rtc
{
	riscv_address_range_list addr_list;
};

struct riscv_ram
{
	riscv_ram_node_list node_list;
};

struct riscv_ram_node
{
	uint64_t node_id;
	riscv_address_range_list addr_list;
};

struct riscv_uart
{
	riscv_address_range_list addr_list;
};

struct riscv_core
{
	riscv_core_node_list node_list;
};

struct riscv_core_node
{
	uint64_t node_id;
	riscv_core_hart_list hart_list;
};

struct riscv_core_hart
{
	uint64_t hart_id;
	std::string isa;
	uint64_t timecmp;
	uint64_t ipi;
};

struct riscv_config : riscv_config_parser
{
	riscv_config_function_map  config_fn_map;
	riscv_block_function_map   block_start_fn_map;
	riscv_block_function_map   block_end_fn_map;

	static bool parse_value(std::string valstr, uint64_t &val);
	static bool parse_address_range(std::string valstr, riscv_address_range_ptr range);
	static std::string address_range_to_string(riscv_address_range_list &addr_list);

	riscv_config();

	riscv_block_stack          block;
	riscv_config_line          prefix;
	riscv_config_line          line;

	riscv_platform_ptr         platform;
	riscv_plic_list            plic_list;
	riscv_pcie_list            pcie_list;
	riscv_leds_list            leds_list;
	riscv_rtc_list             rtc_list;
	riscv_ram_list             ram_list;
	riscv_uart_list            uart_list;
	riscv_core_list            core_list;

	void read(std::string filename);
	void symbol(const char *value, size_t length);
	void end_statement();
	void start_block();
	void end_block();
	void config_done();

	template <typename T, typename R>
	bool lookup_config_fn(T &map, std::string key, R &record, bool push)
	{
		auto it = map.find(key);
		if (it != map.end()) {
			if (push) prefix.push_back(key);
			record = it->second;
			return true;
		}
		if (prefix.size() > 0) {
			uint64_t num;
			if (parse_value(key, num)) {
				std::string block_key = prefix.back() + ".#";
				it = map.find(block_key);
				if (it != map.end()) {
					if (push) prefix.push_back(block_key);
					record = it->second;
					return true;
				}
			} else {
				std::string block_key = prefix.back() + "." + key;
				it = map.find(block_key);
				if (it != map.end()) {
					if (push) prefix.push_back(block_key);
					record = it->second;
					return true;
				}
			}
		}
		return false;
	}
	
	bool lookup_config_fn(std::string key, riscv_config_record &record) {
		return lookup_config_fn(config_fn_map, key, record, false);
	}
	bool lookup_block_start_fn(std::string key, riscv_block_record &block) {
		return lookup_config_fn(block_start_fn_map, key, block, true);
	}
	bool lookup_block_end_fn(std::string key, riscv_block_record &block) {
		return lookup_config_fn(block_end_fn_map, key, block, true);
	}

	// configuration functions
	void config_platform();
	void config_plic();
	void config_pcie();
	void config_leds();
	void config_rtc();
	void config_ram();
	void config_uart();
	void config_core();

	// output functions
	void output_platform(std::stringstream &ss);
	void output_plic(std::stringstream &ss);
	void output_pcie(std::stringstream &ss);
	void output_leds(std::stringstream &ss);
	void output_rtc(std::stringstream &ss);
	void output_ram(std::stringstream &ss);
	void output_uart(std::stringstream &ss);
	void output_core(std::stringstream &ss);

	std::string to_string();
};

#endif
