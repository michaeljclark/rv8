//
//  riscv-device-plic.h
//

#ifndef riscv_device_plic_h
#define riscv_device_plic_h

namespace riscv {

	/* PLIC MMIO device */

	template <typename P, const int NUM_IRQS = 64, const int NUM_NODES = 4, const int NUM_HARTS = 64>
	struct plic_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			priority_bits = 2,
			bits_per_word = sizeof(u32) << 3,
			word_shift = ctz_pow2(bits_per_word),
			irq_words = NUM_IRQS / bits_per_word,
			pending_size = sizeof(u32) * irq_words,
			priority_size = sizeof(u32) * irq_words * priority_bits,
			enabled_words = irq_words * NUM_NODES * NUM_HARTS,
			enabled_size = sizeof(u32) * enabled_words,
			total_size = 12 * sizeof(u32) + pending_size + priority_size + enabled_size
		};

		P &proc;

		/* PLIC config registers */

		u32 num_irqs;
		u32 num_nodes;
		u32 num_harts;
		u32 num_priorities;
		u32 pending_offset;
		u32 pending_length;
		u32 priority0_offset;
		u32 priority0_length;
		u32 priority1_offset;
		u32 priority1_length;
		u32 enabled_offset;
		u32 enabled_length;

		/* PLIC data registers */

		u32 pending[irq_words];
		u32 priority0[irq_words];
		u32 priority1[irq_words];
		u32 enabled[enabled_words];

		/* PLIC data access */

		constexpr u8* as_u8() { return (u8*)&num_irqs; }
		constexpr u16* as_u16() { return (u16*)&num_irqs; }
		constexpr u32* as_u32() { return (u32*)&num_irqs; }
		constexpr u64* as_u64() { return (u64*)&num_irqs; }

		/* PLIC constructor */

		plic_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("PLIC", mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc),
				num_irqs(NUM_IRQS),
				num_nodes(NUM_NODES),
				num_harts(NUM_HARTS),
				num_priorities(4),
				pending_offset(12 * sizeof(u32)),
				pending_length(irq_words * sizeof(u32)),
				priority0_offset(pending_offset + pending_length),
				priority0_length(irq_words * sizeof(u32)),
				priority1_offset(priority0_offset + priority0_length),
				priority1_length(irq_words * sizeof(u32)),
				enabled_offset(priority1_offset + priority1_length),
				enabled_length(enabled_words * sizeof(u32)),
				pending{},
				priority0{},
				priority1{},
				enabled{} {}

		/* PLIC interface */

		void signal_irq(UX irq)
		{
			irq &= (num_irqs-1);
			pending[irq >> word_shift] |= (1ULL << (irq & (bits_per_word-1)));
		}

		bool irq_pending(UX mode, UX node_id, UX hart_id)
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
						UX en_off = (node_id * NUM_NODES * NUM_HARTS) + (hart_id * NUM_HARTS);
						is_pending |= (pending[i] & enabled[en_off + i]);
					}
					break;
				case 1:
					for (size_t i = 0; i < irq_words; i++) {
						UX en_off = (node_id * NUM_NODES * NUM_HARTS) + (hart_id * NUM_HARTS);
						is_pending |= (pending[i] & enabled[en_off + i] & (priority0[i] | priority1[i]));
					}
					break;
				case 2:
					for (size_t i = 0; i < irq_words; i++) {
						UX en_off = (node_id * NUM_NODES * NUM_HARTS) + (hart_id * NUM_HARTS);
						is_pending |= (pending[i] & enabled[en_off + i] & priority0[i]);
					}
					break;
				case 3:
					for (size_t i = 0; i < irq_words; i++) {
						UX en_off = (node_id * NUM_NODES * NUM_HARTS) + (hart_id * NUM_HARTS);
						is_pending |= (pending[i] & enabled[en_off + i] & priority0[i] & priority1[i]);
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
			if (va >= pending_offset && va < total_size) *(as_u8() + va) = val;
		}

		void store_16(UX va, u16 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%04hx\n", addr_t(va), val);
			}
			if (va >= pending_offset && va < total_size - 1) *(as_u16() + (va>>1)) = val;
		}

		void store_32(UX va, u32 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%08x\n", addr_t(va), val);
			}
			if (va >= pending_offset && va < total_size - 3) *(as_u32() + (va>>2)) = val;
		}

		void store_64(UX va, u64 val)
		{
			if (proc.log & proc_log_mmio) {
				printf("plic_mmio:0x%04llx <- 0x%016llx\n", addr_t(va), val);
			}
			if (va >= pending_offset && va < total_size - 7) *(as_u64() + (va>>3)) = val;
		}

	};

}

#endif
