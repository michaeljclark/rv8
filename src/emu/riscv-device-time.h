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

		/* Timer registers */

		u64 mtime;
		u64 mtimecmp;
		bool claimed;

		/* Timer constructor */

		time_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("TIME", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write),
			proc(proc),
			mtime(0),
			mtimecmp(0),
			claimed(true)
		{}

		/* Timer interface */

		void print_registers()
		{
			debug("time_mmio:mtime            %llu", mtime);
			debug("time_mmio:mtimecmp         %llu", mtimecmp);
		}

		bool timer_pending(UX time)
		{
			mtime = time;
			if (claimed) return false;
			claimed = mtime >= mtimecmp;
			return claimed;
		}

		/* Timer MMIO */

		void load_8 (UX va, u8  &val)
		{
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx -> invalid\n", addr_t(va));
			}
			val = 0;
		}

		void load_16(UX va, u16 &val)
		{
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx -> invalid\n", addr_t(va));
			}
			val = 0;
		}

		void load_32(UX va, u32 &val)
		{
			if (va == 0) {
				val = u32(mtime);
			}
			else if (va == 4) {
				val = u32(mtime >> 32);
			}
			else if (va == 8) {
				val = u32(mtimecmp);
			}
			else if (va == 12) {
				val = u32(mtimecmp >> 32);
			}
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx -> 0x%04x\n", addr_t(va), val);
			}
		}

		void load_64(UX va, u64 &val)
		{
			if (va == 0) {
				val = mtime;
			}
			else if (va == 8) {
				val = mtimecmp;
			}
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx -> 0x%08llx\n", addr_t(va), val);
			}
		}

		void store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx <- invalid\n", addr_t(va));
			}
		}

		void store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx <- invalid\n", addr_t(va));
			}
		}

		void store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx <- 0x%04x\n", addr_t(va), val);
			}
			if (va == 0) {
				mtime = (mtime & (u64(-1) << 32)) | val;
			}
			else if (va == 4) {
				mtime = (mtime & (u64(-1) >> 32)) | (u64(val) << 32);
			}
			else if (va == 8) {
				claimed = false;
				mtimecmp = (mtimecmp & (u64(-1) << 32)) | val;
			}
			else if (va == 12) {
				claimed = false;
				mtimecmp = (mtimecmp & (u64(-1) >> 32)) | (u64(val) << 32);
			}
		}

		void store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("time_mmio:0x%04llx <- 0x%08llx\n", addr_t(va), val);
			}
			if (va == 0) {
				mtime = val;
			}
			else if (va == 8) {
				claimed = false;
				mtimecmp = val;
			}
		}
	};

}

#endif
