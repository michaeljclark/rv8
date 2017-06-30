//
//  device-time.h
//

#ifndef rv_device_time_h
#define rv_device_time_h

namespace riscv {

	/* TIME MMIO device */

	template <typename P>
	struct rtc_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			total_size = sizeof(u64)
		};

		P &proc;

		/* Timer registers */

		u64 mtime;

		/* Timer constructor */

		rtc_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("RTC", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write),
			proc(proc),
			mtime(0)
		{}

		/* Timer interface */

		void print_registers()
		{
			debug("rtc_mmio:time              0x%llx", mtime);
		}

		void update_time(UX time)
		{
			mtime = time;
		}

		/* Timer MMIO */

		buserror_t load_32(UX va, u32 &val)
		{
			if (va == 0) {
				val = u32(mtime);
			}
			else if (va == 4) {
				val = u32(mtime >> 32);
			}
			if (proc.log & proc_log_mmio) {
				printf("rtc_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			if (va == 0) {
				val = mtime;
			}
			if (proc.log & proc_log_mmio) {
				printf("rtc_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("rtc_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va == 0) {
				mtime = (mtime & (u64(-1) << 32)) | val;
			}
			else if (va == 4) {
				mtime = (mtime & (u64(-1) >> 32)) | (u64(val) << 32);
			}
			return 0;
		}

		buserror_t store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("rtc_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va == 0) {
				mtime = val;
			}
			return 0;
		}
	};

}

#endif
