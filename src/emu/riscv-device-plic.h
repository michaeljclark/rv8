//
//  riscv-device-plic.h
//

#ifndef riscv_device_plic_h
#define riscv_device_plic_h

namespace riscv {

	/* PLIC MMIO device */

	template <typename P, const int NUM_IRQS = 64>
	struct plic_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			num_irqs = NUM_IRQS,
			priority_bits = 2,
			bits_per_word = sizeof(UX) << 3,
			word_shift = ctz_pow2(bits_per_word),
			irq_words = num_irqs / bits_per_word,
			pending_size = sizeof(UX) * irq_words,
			enabled_size = sizeof(UX) * irq_words,
			priority_size = sizeof(UX) * irq_words * priority_bits,
			total_size = pending_size + enabled_size + priority_size
		};

		P &proc;

		/* PLIC registers */

		UX pending[irq_words];
		UX enabled[irq_words];
		UX priority0[irq_words];
		UX priority1[irq_words];

		constexpr u8* as_u8() { return (u8*)&pending[0]; }
		constexpr u16* as_u16() { return (u16*)&pending[0]; }
		constexpr u32* as_u32() { return (u32*)&pending[0]; }
		constexpr u64* as_u64() { return (u64*)&pending[0]; }

		/* PLIC constructor */

		plic_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("PLIC", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc),
				pending{}, enabled{}, priority0{}, priority1{} {}

		/* PLIC interface */

		void signal_irq(UX irq)
		{
			irq &= (num_irqs-1);
			pending[irq >> word_shift] |= (1ULL << (irq & (bits_per_word-1)));
		}

		bool irq_pending(UX mode)
		{
			/*
			 * implements 4 priority levels using 2 bits ([p0][p1])
			 *
			 * highest M (11)
			 *         H (10,11)
			 *         S (01,10,11)
			 * lowest  U (00,01,10,11)
			 */
			UX is_pending = 0;
			switch (mode) {
				case 0:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[i]);
					}
					break;
				case 1:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[i] & (priority0[i] | priority1[i]));
					}
					break;
				case 2:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[i] & priority0[i]);
					}
					break;
				case 3:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[i] & priority0[i] & priority1[i]);
					}
					break;
			}
			return is_pending > 0;
		}

		/* PLIC MMIO */

		void load_8 (UX va, u8  &val)
		{
			val = (va < total_size) ? *(as_u8() + va) : 0;
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
		}

		void load_16(UX va, u16 &val)
		{
			val = (va < total_size - 1) ? *(as_u16() + (va>>1)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
		}

		void load_32(UX va, u32 &val)
		{
			val = (va < total_size - 3) ? *(as_u32() + (va>>2)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
		}

		void load_64(UX va, u64 &val)
		{
			val = (va < total_size - 7) ? *(as_u64() + (va>>3)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
		}

		void store_8 (UX va, u8  val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val);
			}
			if (va < total_size) *(as_u8() + va) = val;
		}

		void store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val);
			}
			if (va < total_size - 1) *(as_u16() + (va>>1)) = val;
		}

		void store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va < total_size - 3) *(as_u32() + (va>>2)) = val;
		}

		void store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va < total_size - 7) *(as_u64() + (va>>3)) = val;
		}

	};

}

#endif
