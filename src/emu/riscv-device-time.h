//
//  riscv-device-time.h
//

#ifndef riscv_device_time_h
#define riscv_device_time_h

namespace riscv {

	/* TIME MMIO device */

	template <typename P>
	struct time_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			total_size = sizeof(u64) * 2
		};

		P &proc;

		/* Time registers */

		u64 mtime;
		u64 mtimecmp;

		/* TODO implement TIME MMIO access and timer interrupts */

		time_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>(mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc) {}

		void load_8 (UX va, u8  &val) { printf("time_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }
		void load_16(UX va, u16 &val) { printf("time_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }
		void load_32(UX va, u32 &val) { printf("time_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }
		void load_64(UX va, u64 &val) { printf("time_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }

		void store_8 (UX va, u8  val) { printf("time_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val); }
		void store_16(UX va, u16 val) { printf("time_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val); }
		void store_32(UX va, u32 val) { printf("time_mmio:0x%04llx <- 0x%04x\n", addr_t(va), val); }
		void store_64(UX va, u64 val) { printf("time_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val); }
	};

}

#endif
