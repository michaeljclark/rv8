//
//  config.h
//

#ifndef rv_config_h
#define rv_config_h

namespace riscv {

	/* Generic configuration model */

	struct config_string;
	struct config_record;
	struct block_record;

	typedef std::shared_ptr<config_string> config_ptr;
	typedef std::vector<std::string> config_line;
	typedef std::vector<config_line> block_stack;
	typedef std::function<void(config_line&)>config_function;
	typedef std::map<std::string,config_record> config_function_map;
	typedef std::function<void(config_line&)>block_function;
	typedef std::map<std::string,block_record> block_function_map;

	struct config_record
	{
		int minargs;
		int maxargs;
		config_function fn;
	};

	struct block_record
	{
		int minargs;
		int maxargs;
		block_function fn;
	};

	/* RISC-V configuration model */

	struct address_range;
	struct platform;
	struct plic;
	struct plic_priority;
	struct plic_pending;
	struct plic_node;
	struct plic_hart;
	struct plic_mode;
	struct pcie;
	struct pcie_bus;
	struct pcie_bridge;
	struct leds;
	struct rtc;
	struct ram;
	struct ram_node;
	struct uart;
	struct core;
	struct core_node;
	struct core_hart;

	typedef std::shared_ptr<address_range> address_range_ptr;
	typedef std::vector<address_range_ptr> address_range_list;
	typedef std::shared_ptr<platform> platform_ptr;
	typedef std::shared_ptr<plic_priority> plic_priority_ptr;
	typedef std::shared_ptr<plic_pending> plic_pending_ptr;
	typedef std::shared_ptr<plic> plic_ptr;
	typedef std::vector<plic_ptr> plic_list_t;
	typedef std::shared_ptr<plic_node> plic_node_ptr;
	typedef std::vector<plic_node_ptr> plic_node_list;
	typedef std::shared_ptr<plic_hart> plic_hart_ptr;
	typedef std::vector<plic_hart_ptr> plic_hart_list;
	typedef std::shared_ptr<plic_mode> plic_mode_ptr;
	typedef std::vector<plic_mode_ptr> plic_mode_list;
	typedef std::shared_ptr<pcie> pcie_ptr;
	typedef std::vector<pcie_ptr> pcie_list_t;
	typedef std::shared_ptr<pcie_bus> pcie_bus_ptr;
	typedef std::vector<pcie_bus_ptr> pcie_bus_list;
	typedef std::shared_ptr<pcie_bridge> pcie_bridge_ptr;
	typedef std::vector<pcie_bridge_ptr> pcie_bridge_list;
	typedef std::shared_ptr<leds> leds_ptr;
	typedef std::vector<leds_ptr> leds_list_t;
	typedef std::shared_ptr<rtc> rtc_ptr;
	typedef std::vector<rtc_ptr> rtc_list_t;
	typedef std::shared_ptr<ram> ram_ptr;
	typedef std::vector<ram_ptr> ram_list_t;
	typedef std::shared_ptr<ram_node> ram_node_ptr;
	typedef std::vector<ram_node_ptr> ram_node_list;
	typedef std::shared_ptr<uart> uart_ptr;
	typedef std::vector<uart_ptr> uart_list_t;
	typedef std::shared_ptr<core> core_ptr;
	typedef std::vector<core_ptr> core_list_t;
	typedef std::shared_ptr<core_node> core_node_ptr;
	typedef std::vector<core_node_ptr> core_node_list;
	typedef std::shared_ptr<core_hart> core_hart_ptr;
	typedef std::vector<core_hart_ptr> core_hart_list;

	struct address_range
	{
		addr_t start;
		addr_t end;
	};

	struct platform
	{
		std::string vendor;
		std::string arch;
	};

	struct plic
	{
		std::string interface;
		integral_t ndevs;
		plic_priority_ptr priority;
		plic_pending_ptr pending;
		plic_node_list node_list;
	};

	struct plic_priority
	{
		address_range_list addr_list;
	};

	struct plic_pending
	{
		address_range_list addr_list;
	};

	struct plic_node
	{
		integral_t node_id;
		plic_hart_list hart_list;
	};

	struct plic_hart
	{
		integral_t hart_id;
		plic_mode_list mode_list;
	};

	struct plic_mode
	{
		std::string mode_name;
		address_range_list ie_addr_list;
		address_range_list ctl_addr_list;
	};

	struct pcie
	{
		std::string interface;
		pcie_bus_list bus_list;
		pcie_bridge_list bridge_list;
	};

	struct pcie_bus
	{
		address_range_list addr_list;
		std::pair<integral_t,integral_t> bus_pair;
	};

	struct pcie_bridge
	{
		address_range_list addr_list;
		integral_t bus_id;
		integral_t irq_id;
	};

	struct leds
	{
		std::string interface;
		integral_t ngpio;
		address_range_list addr_list;
	};

	struct rtc
	{
		address_range_list addr_list;
	};

	struct ram
	{
		ram_node_list node_list;
	};

	struct ram_node
	{
		integral_t node_id;
		address_range_list addr_list;
	};

	struct uart
	{
		address_range_list addr_list;
	};

	struct core
	{
		core_node_list node_list;
	};

	struct core_node
	{
		integral_t node_id;
		core_hart_list hart_list;
	};

	struct core_hart
	{
		integral_t hart_id;
		std::string isa;
		addr_t timecmp;
		addr_t ipi;
	};

	struct config_string : config_parser
	{
		// Generic configuration model
		config_function_map  config_fn_map;
		block_function_map   block_start_fn_map;
		block_function_map   block_end_fn_map;

		block_stack          block;
		config_line          prefix;
		config_line          line;

		// RISC-V configuration model
		platform_ptr         platform;
		plic_list_t          plic_list;
		pcie_list_t          pcie_list;
		leds_list_t          leds_list;
		rtc_list_t           rtc_list;
		ram_list_t           ram_list;
		uart_list_t          uart_list;
		core_list_t          core_list;

		static bool parse_address_range(std::string valstr, address_range_ptr range);
		static std::string address_range_to_string(address_range_list &addr_list);

		config_string();
		std::string to_string();
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
				integral_t num;
				if (parse_integral(key, num)) {
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
		
		bool lookup_config_fn(std::string key, config_record &record) {
			return lookup_config_fn(config_fn_map, key, record, false);
		}
		bool lookup_block_start_fn(std::string key, block_record &block) {
			return lookup_config_fn(block_start_fn_map, key, block, true);
		}
		bool lookup_block_end_fn(std::string key, block_record &block) {
			return lookup_config_fn(block_end_fn_map, key, block, true);
		}

		// configuration parser lambdas
		void config_platform();
		void config_plic();
		void config_pcie();
		void config_leds();
		void config_rtc();
		void config_ram();
		void config_uart();
		void config_core();

		// configuration output functions
		void output_platform(std::string &s);
		void output_plic(std::string &s);
		void output_pcie(std::string &s);
		void output_leds(std::string &s);
		void output_rtc(std::string &s);
		void output_ram(std::string &s);
		void output_uart(std::string &s);
		void output_core(std::string &s);
	};

}

#endif
