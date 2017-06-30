//
//  device-time.h
//

#ifndef rv_device_htif_h
#define rv_device_htif_h

namespace riscv {

	/* HTIF MMIO device */

	template <typename P>
	struct htif_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;
		typedef std::shared_ptr<console_device<P>> console_device_ptr;

		enum {
			total_size = sizeof(u64) * 2
		};

		P &proc;

		/* HTIF registers */

		u64 htif_tohost;
		u64 htif_fromhost;

		console_device_ptr console;

		/* HTIF constructor */

		htif_mmio_device(P &proc, UX mpa, console_device_ptr console) :
			memory_segment<UX>("HTIF", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write),
			proc(proc),
			htif_tohost(0),
			htif_fromhost(0),
			console(console)
		{}

		/* HTIF interface */

		void print_registers()
		{
			debug("htif_mmio:htif_tohost      %llu", htif_tohost);
			debug("htif_mmio:htif_fromhost    %llu", htif_fromhost);
		}

		inline u64 htif_device_command(u8 device, u8 command) {
			return ((u64)device << 56) | ((u64)command << 48);
		}

		inline u8 htif_device(u64 var) { return (var >> 56); }
		inline u8 htif_command(u64 var) { return (var >> 48) & 0xff; }

		void handle_output()
		{
			if (htif_tohost == 1) {
				proc.raise(P::internal_cause_poweroff, proc.pc);
			}
			u8 device = htif_device(htif_tohost);
			u8 command = htif_command(htif_tohost);
			if (device == 1) {
				switch (command) {
					case 0:
						htif_tohost = 0;
						htif_fromhost = htif_device_command(device, command);
						break;
					case 1: 
						console->write_char(htif_tohost & 0xff);
						htif_tohost = 0;
						htif_fromhost = htif_device_command(device, command);
						break;
					default:
						debug("unknown htif command: %d", device);
						break;
				}
			}
		}

		void handle_input()
		{
			if (htif_fromhost == 0) {
				if (console->has_char()) {
					htif_fromhost = htif_device_command(1, 0) | console->read_char();
				}
			}
		}

		/* HTIF MMIO */

		buserror_t load_32(UX va, u32 &val)
		{
			if (va == 0) {
				val = u32(htif_tohost);
			}
			else if (va == 4) {
				val = u32(htif_tohost >> 32);
			}
			else if (va == 8) {
				handle_input();
				val = u32(htif_fromhost);
			}
			else if (va == 12) {
				val = u32(htif_fromhost >> 32);
			}
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			if (va == 0) {
				val = htif_tohost;
			}
			else if (va == 8) {
				/* here */
				handle_input();
				val = htif_fromhost;
			}
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va == 0) {
				htif_tohost = (htif_tohost & (u64(-1) << 32)) | val;
				handle_output();
			}
			else if (va == 4) {
				htif_tohost = (htif_tohost & (u64(-1) >> 32)) | (u64(val) << 32);
				handle_output();
			}
			else if (va == 8) {
				htif_fromhost = (htif_fromhost & (u64(-1) << 32)) | val;
			}
			else if (va == 12) {
				htif_fromhost = (htif_fromhost & (u64(-1) >> 32)) | (u64(val) << 32);
			}
			return 0;
		}

		buserror_t store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va == 0) {
				htif_tohost = val;
				handle_output();
			}
			else if (va == 8) {
				htif_fromhost = val;
			}
			return 0;
		}
	};

}

#endif
