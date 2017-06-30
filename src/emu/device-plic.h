//
//  device-plic.h
//

#ifndef rv_device_plic_h
#define rv_device_plic_h

namespace riscv {

	/* PLIC MMIO device */

	template <typename P, const int NUM_IRQS = 64>
	struct plic_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		P &proc;

		/* PLIC data registers */

		u32 pending;
		u32 served;

		/* PLIC constructor */

		plic_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("PLIC", mpa, /*uva*/0, /*size*/8,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc),
				pending(0),
				served(0)
			{}

		/* PLIC interface */

		void print_registers()
		{
			debug("plic_mmio:pending          0x%016llx", pending);
			debug("plic_mmio:served           0b%016llx", served);
		}

		void set_irq(UX irq, int val)
		{
			if (val) {
				pending |= (1 << irq);
			} else {
				pending &= ~(1 << irq);
			}
		}

		bool irq_pending()
		{
			return (pending & ~served) > 0;
		}

		/* PLIC MMIO */

		buserror_t load_32(UX va, u32 &val)
		{
			if (va == 4) {
				u32 mask = pending & ~served;
				if (mask != 0) {
					int i = ctz(mask);
					served |= 1 << i;
					val = i + 1;
				} else {
					val = 0;
				}
			} else {
				val = 0;
			}
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			if (va == 0) {
				u32 mask = pending & ~served;
				if (mask != 0) {
					int i = ctz(mask);
					served |= 1 << i;
					val = i + 1;
				} else {
					val = 0;
				}
			} else {
				val = 0;
			}
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx -> 0x%08llx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va == 4) {
				val--;
				if (val < 32) {
					served &= ~(1 << val);
				}
			}
			return 0;
		}

		buserror_t store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va == 0) {
				val--;
				if (val < 32) {
					served &= ~(1 << val);
				}
			}
			return 0;
		}

	};

}

#endif
