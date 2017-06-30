//
//  device-ipi.h
//

#ifndef rv_device_gpio_h
#define rv_device_gpio_h

namespace riscv {

	/* GPIO MMIO device */

	template <typename P>
	struct gpio_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;
		typedef std::shared_ptr<plic_mmio_device<P>> plic_mmio_device_ptr;

		P &proc;
		plic_mmio_device_ptr plic;
		UX irq;

		/* GPIO registers */

		struct {
			u32 ie;  /* interrupt enable */
			u32 ip;  /* interrupt pending */
			u32 in;  /* intput buffer */
			u32 out; /* output buffer */
		} gpio;

		enum {
			OUT_POWER_OFF = 1,
			OUT_RESET = 2,
		};

		enum {
			total_size = sizeof(u32) * 4
		};

		constexpr u8* as_u8() { return (u8*)&gpio.ie; }
		constexpr u16* as_u16() { return (u16*)&gpio.ie; }
		constexpr u32* as_u32() { return (u32*)&gpio.ie; }
		constexpr u64* as_u64() { return (u64*)&gpio.ie; }

		/* MIPI constructor */

		gpio_mmio_device(P &proc, UX mpa, plic_mmio_device_ptr plic, UX irq) :
			memory_segment<UX>("GPIO", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write),
			proc(proc),
			plic(plic),
			irq(irq),
			gpio{}
		{}

		/* GPIO interface */

		void print_registers()
		{
			debug("gpio_mmio:ie               0x%08x", gpio.ie);
			debug("gpio_mmio:ip               0x%08x", gpio.ip);
			debug("gpio_mmio:in               0x%08x", gpio.in);
			debug("gpio_mmio:out              0x%08x", gpio.out);
		}

		void service()
		{
			plic->set_irq(irq, (gpio.ie & gpio.ip) ? 1 : 0);
		}

		void trigger()
		{
			if (gpio.out & OUT_POWER_OFF) {
				proc.raise(P::internal_cause_poweroff, proc.pc);
			}
			if (gpio.out & OUT_RESET) {
				proc.reset();
			}
		}

		/* GPIO MMIO */

		buserror_t load_8 (UX va, u8  &val)
		{
			val = (va < total_size) ? *(as_u8() + va) : 0;
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_16(UX va, u16 &val)
		{
			val = (va < total_size - 1) ? *(as_u16() + (va>>1)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_32(UX va, u32 &val)
		{
			val = (va < total_size - 3) ? *(as_u32() + (va>>2)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			val = (va < total_size - 7) ? *(as_u64() + (va>>3)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val);
			}
			if (va < total_size) *(as_u8() + va) = val;
			trigger();
			return 0;
		}

		buserror_t store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val);
			}
			if (va < total_size - 1) *(as_u16() + (va>>1)) = val;
			trigger();
			return 0;
		}

		buserror_t store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va < total_size - 3) *(as_u32() + (va>>2)) = val;
			trigger();
			return 0;
		}

		buserror_t store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("gpio_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va < total_size - 7) *(as_u64() + (va>>3)) = val;
			trigger();
			return 0;
		}

	};

}

#endif
