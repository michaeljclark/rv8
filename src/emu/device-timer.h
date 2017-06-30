//
//  device-timer.h
//

#ifndef rv_device_timer_h
#define rv_device_timer_h

namespace riscv {

	/* Timer MMIO device */

	template <typename P, const int NUM_HARTS = 4>
	struct timer_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			bits_per_word = sizeof(u64) << 3,
			num_harts = NUM_HARTS,
			total_size = bits_per_word * num_harts
		};

		P &proc;

		/* Timer compare registers */

		u64 timecmp[num_harts];
		u64 claimed[num_harts];

		constexpr u8* as_u8() { return (u8*)&timecmp[0]; }
		constexpr u16* as_u16() { return (u16*)&timecmp[0]; }
		constexpr u32* as_u32() { return (u32*)&timecmp[0]; }
		constexpr u64* as_u64() { return (u64*)&timecmp[0]; }

		/* Timer constructor */

		timer_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("TIMER", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc),
				timecmp{}
		{
			for (size_t i = 0; i < num_harts; i++) {
				claimed[i] = std::numeric_limits<u64>::max();
			}
		}

		/* Timer interface */

		void print_registers()
		{
			for (size_t i = 0; i < num_harts; i++) {
				debug("timer_mmio:timecmp[%04d]   0x%llx", i, timecmp[i]);
				debug("timer_mmio:claimed[%04d]   0x%llx", i, claimed[i]);
			}
		}

		bool timer_pending(UX hart_id, u64 time)
		{
			if (hart_id >= num_harts || claimed[hart_id] > 0) return false;
			if (timecmp[hart_id] <= time) {
				claimed[hart_id] = time;
				return true;
			} else {
				return false;
			}
		}

		/* Timer MMIO */

		buserror_t load_8 (UX va, u8  &val)
		{
			val = (va < total_size) ? *(as_u8() + va) : 0;
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_16(UX va, u16 &val)
		{
			val = (va < total_size - 1) ? *(as_u16() + (va>>1)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_32(UX va, u32 &val)
		{
			val = (va < total_size - 3) ? *(as_u32() + (va>>2)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			val = (va < total_size - 7) ? *(as_u64() + (va>>3)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val);
			}
			if (va < total_size) {
				*(as_u8() + va) = val;
				claimed[va >> 3] = 0;
			}
			return 0;
		}

		buserror_t store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val);
			}
			if (va < total_size - 1) {
				*(as_u16() + (va>>1)) = val;
				claimed[va >> 3] = 0;
			}
			return 0;
		}

		buserror_t store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va < total_size - 3) {
				*(as_u32() + (va>>2)) = val;
				claimed[va >> 3] = 0;
			}
			return 0;
		}

		buserror_t store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("timer_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va < total_size - 7) {
				*(as_u64() + (va>>3)) = val;
				claimed[va >> 3] = 0;
			}
			return 0;
		}

	};

}

#endif
