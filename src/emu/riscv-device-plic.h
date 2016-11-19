//
//  riscv-device-plic.h
//

#ifndef riscv_device_plic_h
#define riscv_device_plic_h

namespace riscv {

	/* PLIC MMIO device */

	template <typename P>
	struct plic_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		P &proc;

		/* TODO - implement PLIC state */

		plic_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>(mpa, 0x1000, 0x1000, pma_type_io | pma_prot_read | pma_prot_write), proc(proc) {}

		~plic_mmio_device() {}

		void load_8 (UX va, u8  &val) { printf("plic_mmio:0x%llx -> 0\n", addr_t(va)); val = 0; }
		void load_16(UX va, u16 &val) { printf("plic_mmio:0x%llx -> 0\n", addr_t(va)); val = 0; }
		void load_32(UX va, u32 &val) { printf("plic_mmio:0x%llx -> 0\n", addr_t(va)); val = 0; }
		void load_64(UX va, u64 &val) { printf("plic_mmio:0x%llx -> 0\n", addr_t(va)); val = 0; }

		void store_8 (UX va, u8  val) { printf("plic_mmio:0x%llx <- 0x%02hhx\n", addr_t(va), val); }
		void store_16(UX va, u16 val) { printf("plic_mmio:0x%llx <- 0x%04hx\n", addr_t(va), val); }
		void store_32(UX va, u32 val) { printf("plic_mmio:0x%llx <- 0x%08x\n", addr_t(va), val); }
		void store_64(UX va, u64 val) { printf("plic_mmio:0x%llx <- 0x%016llx\n", addr_t(va), val); }
	};

}

#endif
