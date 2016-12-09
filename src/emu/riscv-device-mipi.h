//
//  riscv-device-ipi.h
//

#ifndef riscv_device_ipi_h
#define riscv_device_ipi_h

namespace riscv {

	/* MIPI MMIO device */

	template <typename P, const int NUM_HARTS = 64>
	struct mipi_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			num_harts = NUM_HARTS,
			bits_per_word = sizeof(UX) << 3,
			word_shift = ctz_pow2(bits_per_word),
			hart_words = num_harts / bits_per_word,
			hart_size = sizeof(UX) * hart_words,
			total_size = hart_size
		};

		P &proc;

		/* MIPI registers */

		UX hart[hart_words];

		constexpr u8* as_u8() { return (u8*)&hart[0]; }
		constexpr u16* as_u16() { return (u16*)&hart[0]; }
		constexpr u32* as_u32() { return (u32*)&hart[0]; }
		constexpr u64* as_u64() { return (u64*)&hart[0]; }

		/* MIPI constructor */

		mipi_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("MIPI", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc),
				hart{} {}

		/* MIPI interface */

		void print_registers()
		{
			std::string hart_bstr;
			for (ssize_t i = hart_words - 1; i >= 0; --i) {
				hart_bstr += to_binary(hart[i]);
			}
			debug("mipi_mmio:hart             0b%s", hart_bstr.c_str());
		}

		void signal_ipi(UX hart_id)
		{
			hart_id &= (num_harts-1);
			hart[hart_id >> word_shift] |= (1ULL << (hart_id & (bits_per_word-1)));
		}

		bool ipi_pending(UX hart_id)
		{
			hart_id &= (num_harts-1);
			return (hart[hart_id >> word_shift] & (1ULL << (hart_id & (bits_per_word-1)))) > 0;
		}

		/* MIPI MMIO */

		void load_8 (UX va, u8  &val)
		{
			val = (va < total_size) ? *(as_u8() + va) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
		}

		void load_16(UX va, u16 &val)
		{
			val = (va < total_size - 1) ? *(as_u16() + (va>>1)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
		}

		void load_32(UX va, u32 &val)
		{
			val = (va < total_size - 3) ? *(as_u32() + (va>>2)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
		}

		void load_64(UX va, u64 &val)
		{
			val = (va < total_size - 7) ? *(as_u64() + (va>>3)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
		}

		void store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val);
			}
			if (va < total_size) *(as_u8() + va) = val;
		}

		void store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val);
			}
			if (va < total_size - 1) *(as_u16() + (va>>1)) = val;
		}

		void store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va < total_size - 3) *(as_u32() + (va>>2)) = val;
		}

		void store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("mipi_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va < total_size - 7) *(as_u64() + (va>>3)) = val;
		}

	};

}

#endif
