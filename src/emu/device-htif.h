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

		void handle_output()
		{
			if (htif_tohost == 1) {
				proc.raise(P::internal_cause_poweroff, proc.pc);
			}
			u8 device = htif_tohost >> 56;
			u8 cmd = (htif_tohost >> 48) & 0xff;
			if (device == 1 && cmd == 1) {
				u8 c = htif_tohost & 0xff;
				console->write_char(c);
			}
		}

		void handle_input()
		{
			htif_fromhost = 0;
			if (console->has_char()) {
				htif_fromhost = ((uint64_t)1 << 56) | ((uint64_t)0 << 48) | console->read_char();
			}
		}

		/* HTIF MMIO */

		void load_32(UX va, u32 &val)
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
				printf("htif_mmio:0x%04llx -> 0x%04x\n", addr_t(va), val);
			}
		}

		void load_64(UX va, u64 &val)
		{
			if (va == 0) {
				val = htif_tohost;
			}
			else if (va == 8) {
				handle_input();
				val = htif_fromhost;
			}
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx -> 0x%08llx\n", addr_t(va), val);
			}
		}

		void store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx <- 0x%04x\n", addr_t(va), val);
			}
			if (va == 0) {
				htif_tohost = (htif_tohost & (u64(-1) << 32)) | val;
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
		}

		void store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx <- 0x%08llx\n", addr_t(va), val);
			}
			if (va == 0) {
				htif_tohost = val;
				handle_output();
			}
			else if (va == 8) {
				htif_fromhost = val;
			}
		}
	};

}

#endif
