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
			priority_words = (num_irqs / bits_per_word) * priority_bits,
			pending_size = sizeof(UX) * irq_words,
			enabled_size = sizeof(UX) * irq_words,
			priority_size = sizeof(UX) * priority_words,
			total_size = pending_size + enabled_size + priority_size
		};

		P &proc;

		/* PLIC registers */

		UX pending[irq_words];
		UX enabled[irq_words];
		UX priority[priority_words];

		/* TODO implement PLIC MMIO access and functionality */

		plic_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>(mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc) {}

		void signal_irq(UX irq)
		{
			irq &= (num_irqs-1);
			pending[irq >> word_shift] |= (1ULL << (irq & (bits_per_word-1)));
		}

		void load_8 (UX va, u8  &val) { printf("plic_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }
		void load_16(UX va, u16 &val) { printf("plic_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }
		void load_32(UX va, u32 &val) { printf("plic_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }
		void load_64(UX va, u64 &val) { printf("plic_mmio:0x%04llx -> 0\n", addr_t(va)); val = 0; }

		void store_8 (UX va, u8  val) { printf("plic_mmio:0x%04llx <- 0x%02hhx\n", addr_t(va), val); }
		void store_16(UX va, u16 val) { printf("plic_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val); }
		void store_32(UX va, u32 val) { printf("plic_mmio:0x%04llx <- 0x%04x\n", addr_t(va), val); }
		void store_64(UX va, u64 val) { printf("plic_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val); }
	};

}

#endif
