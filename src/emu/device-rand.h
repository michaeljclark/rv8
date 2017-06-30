//
//  device-rand.h
//

#ifndef rv_device_rand_h
#define rv_device_rand_h

namespace riscv {

	/* RAND MMIO device */

	template <typename P>
	struct rand_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			total_size = page_size
		};

		P &proc;
		host_cpu &cpu;

		/* RAND constructor */

		rand_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("RAND", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read),
			proc(proc), cpu(host_cpu::get_instance())
		{}

		/* RAND MMIO */

		buserror_t load_8 (UX va, u8  &val)
		{
			u8 r = cpu.get_random_seed();
			val = (va < total_size) ? r : 0;
			if (proc.log & proc_log_mmio) {
				printf("rand_mmio:0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_16(UX va, u16 &val)
		{
			u16 r = cpu.get_random_seed();
			val = (va < total_size - 1) ? r : 0;
			if (proc.log & proc_log_mmio) {
				printf("rand_mmio:0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_32(UX va, u32 &val)
		{
			u32 r = cpu.get_random_seed();
			val = (va < total_size - 3) ? r : 0;
			if (proc.log & proc_log_mmio) {
				printf("rand_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			u64 r = (u64(cpu.get_random_seed()) << 32) | u64(cpu.get_random_seed());
			val = (va < total_size - 7) ? r : 0;
			if (proc.log & proc_log_mmio) {
				printf("rand_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}
	};

}

#endif
