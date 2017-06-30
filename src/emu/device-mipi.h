//
//  device-ipi.h
//

#ifndef rv_device_ipi_h
#define rv_device_ipi_h

namespace riscv {

	/* MIPI MMIO device */

	template <typename P, const int NUM_HARTS = 4>
	struct mipi_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			bits_per_word = sizeof(u32) << 3,
			num_harts = NUM_HARTS,
			total_size = bits_per_word * num_harts
		};

		P &proc;

		/* MIPI registers */

		u32 hart[NUM_HARTS];

		constexpr u8* as_u8() { return (u8*)&hart[0]; }
		constexpr u16* as_u16() { return (u16*)&hart[0]; }
		constexpr u32* as_u32() { return (u32*)&hart[0]; }
		constexpr u64* as_u64() { return (u64*)&hart[0]; }

		/* MIPI constructor */

		mipi_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("IPI", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc),
				hart{} {}

		/* MIPI interface */

		void print_registers()
		{
			for (size_t i = 0; i < num_harts; i++) {
				debug("mipi_mmio:hart[%04d]       0x%x", i, hart[i]);
			}
		}

		void signal_ipi(UX hart_id, u32 value)
		{
			if (hart_id >= num_harts) return;
			hart[num_harts] |= (1ULL << (hart_id & (bits_per_word-1)));
		}

		bool ipi_pending(UX hart_id)
		{
			if (hart_id >= num_harts) return false;
			return hart[hart_id] > 0;
		}

		/* MIPI MMIO */

		buserror_t load_8 (UX va, u8  &val)
		{
			val = (va < total_size) ? *(as_u8() + va) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_16(UX va, u16 &val)
		{
			val = (va < total_size - 1) ? *(as_u16() + (va>>1)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_32(UX va, u32 &val)
		{
			val = (va < total_size - 3) ? *(as_u32() + (va>>2)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			val = (va < total_size - 7) ? *(as_u64() + (va>>3)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val);
			}
			if (va < total_size) *(as_u8() + va) = val;
			return 0;
		}

		buserror_t store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val);
			}
			if (va < total_size - 1) *(as_u16() + (va>>1)) = val;
			return 0;
		}

		buserror_t store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va < total_size - 3) *(as_u32() + (va>>2)) = val;
			return 0;
		}

		buserror_t store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va < total_size - 7) *(as_u64() + (va>>3)) = val;
			return 0;
		}

	};

}

#endif
