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

		enum {
			total_size = sizeof(u64) * 2
		};

		P &proc;

		/* Timer registers */

		u64 htif_tohost;
		u64 htif_fromhost;

		/* HTIF constructor */

		htif_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("HTIF", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write),
			proc(proc),
			htif_tohost(0),
			htif_fromhost(0)
		{}

		/* HTIF interface */

		void print_registers()
		{
			debug("htif_mmio:htif_tohost      %llu", htif_tohost);
			debug("htif_mmio:htif_fromhost    %llu", htif_fromhost);
		}

		void handle_command()
		{
			/* TODO */
		}

		/* HTIF MMIO */

		void load_8 (UX va, u8  &val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx -> invalid\n", addr_t(va));
			}
			val = 0;
		}

		void load_16(UX va, u16 &val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx -> invalid\n", addr_t(va));
			}
			val = 0;
		}

		void load_32(UX va, u32 &val)
		{
			if (va == 0) {
				val = u32(htif_tohost);
			}
			else if (va == 4) {
				val = u32(htif_tohost >> 32);
			}
			else if (va == 8) {
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
				val = htif_fromhost;
			}
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx -> 0x%08llx\n", addr_t(va), val);
			}
		}

		void store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx <- invalid\n", addr_t(va));
			}
		}

		void store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("htif_mmio:0x%04llx <- invalid\n", addr_t(va));
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
				handle_command();
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
				handle_command();
			}
			else if (va == 8) {
				htif_fromhost = val;
			}
		}
	};

}

#endif
