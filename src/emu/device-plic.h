//
//  device-plic.h
//

#ifndef riscv_device_plic_h
#define riscv_device_plic_h

namespace riscv {

	/* PLIC MMIO device */

	/*
		## PLIC MMIO Layout

		Offset         | Type | Name             | Value
		:------------- | :--- | :--------------  | :-----
		0                u32    num_irqs           NUM_IRQS
		4                u32    num_nodes          NUM_NODES
		8                u32    num_harts          NUM_HARTS
		12               u32    num_priorities     4
		16               u32    pending_offset     12 * sizeof(u32)
		20               u32    priority0_offset   pending_offset + pending_length
		24               u32    priority1_offset   priority0_offset + priority0_length
		28               u32    enabled_offset     priority1_offset + priority1_length
		pending_offset   u32    pending            irq_words * sizeof(u32)
		priority0_offset u32    priority0          irq_words * sizeof(u32)
		priority1_offset u32    priority1          irq_words * sizeof(u32)
		enabled_offset   u32    enabled            enabled_words * sizeof(u32)

		The following constants describe the length of the MMIO apetures:

        Constant         | Expression
        :--------------- | :----------------
		irq_words        | num_irqs / bits_per_word
		pending_length   | irq_words * sizeof(u32)
		priority0_length | irq_words * sizeof(u32)
		priority1_length | irq_words * sizeof(u32)
		enabled_words    | num_irqs * num_nodes * num_harts / bits_per_word
		enabled_length   | enabled_words * sizeof(u32)

		The enabled bit field order is [node_id][hart_id][irq_word] so that
		the offset into the enabled bit field can be calculated using the
		node_id and hart_id, which locates the start of the bit field that
		is encoded in the same order as the pending and priority bit fields.

		The offset into the pending, priority0 and priority1 bit fields
		is based on the least significant IRQ (e.g. irq 0) being in the
		least significant bit of the first word:

			pending[irq >> word_shift] & (1ULL << (irq & (bits_per_word-1)))

		where word_shift = 5 and bits_per_word = 32.

		The enabled bit field for a given node_id and hart_id combination
		starts at an offset based on the node_id and hart_id and the
		encoding then follows the same scheme as the pending bit field:

			(node_id * NUM_HARTS * irq_words) + (hart_id * irq_words)

		## Priority Encoding

		implements 4 priority levels using 2 bits ([p0][p1])

		Priority | Mode | Enabled ([p0][p1])
		:------  | ---- | :------
		highest  | M    | (11)
		         | H    | (10,11)
		         | S    | (01,10,11)
		lowest   | U    | (00,01,10,11)
	*/

	template <typename P, const int NUM_IRQS = 64, const int NUM_NODES = 1, const int NUM_HARTS = 8>
	struct plic_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			priority_bits = 2,
			bits_per_word = sizeof(u32) << 3,
			word_shift = ctz_pow2(bits_per_word),
			irq_words = NUM_IRQS / bits_per_word,
			pending_length = sizeof(u32) * irq_words,
			priority_length = sizeof(u32) * irq_words,
			enabled_words = (NUM_IRQS * NUM_NODES * NUM_HARTS) / bits_per_word,
			enabled_length = sizeof(u32) * enabled_words,
			total_size = 8 * sizeof(u32) + pending_length + (2 * priority_length) + enabled_length
		};

		P &proc;

		/* PLIC config registers */

		u32 num_irqs;
		u32 num_nodes;
		u32 num_harts;
		u32 num_priorities;
		u32 pending_offset;
		u32 priority0_offset;
		u32 priority1_offset;
		u32 enabled_offset;

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
				pending_offset(8 * sizeof(u32)),
				priority0_offset(pending_offset + pending_length),
				priority1_offset(priority0_offset + priority_length),
				enabled_offset(priority1_offset + priority_length),
				pending{},
				priority0{},
				priority1{},
				enabled{}
			{}

		/* PLIC interface */

		void print_registers()
		{
			debug("plic_mmio:num_irqs         %d", num_irqs);
			debug("plic_mmio:num_nodes        %d", num_nodes);
			debug("plic_mmio:num_harts        %d", num_harts);
			debug("plic_mmio:pending_offset   %d", pending_offset);
			debug("plic_mmio:priority0_offset %d", priority0_offset);
			debug("plic_mmio:priority1_offset %d", priority1_offset);
			debug("plic_mmio:enabled_offset   %d", enabled_offset);
			debug("plic_mmio:total_size       %d", total_size);
			std::string pending_bstr;
			std::string priority0_bstr;
			std::string priority1_bstr;
			for (ssize_t i = irq_words - 1; i >= 0; --i) {
				pending_bstr += to_binary(pending[i]);
				priority0_bstr += to_binary(priority0[i]);
				priority1_bstr += to_binary(priority1[i]);
			}
			debug("plic_mmio:pending          0b%s", pending_bstr.c_str());
			debug("plic_mmio:priority0        0b%s", priority0_bstr.c_str());
			debug("plic_mmio:priority1        0b%s", priority1_bstr.c_str());
			for (size_t n = 0; n < NUM_NODES; n++) {
				for (size_t h = 0; h < NUM_HARTS; h++) {
					std::string enabled_bstr;
					UX en_off = (n * NUM_HARTS * irq_words) + (h * irq_words);
					for (ssize_t i = irq_words - 1; i >= 0; --i) {
						enabled_bstr += to_binary(enabled[en_off + i]);
					}
					debug("plic_mmio:enabled[%02d][%02d]  0b%s", n, h, enabled_bstr.c_str());
					fflush(stdout);
				}
			}
		}

		void signal_irq(UX irq)
		{
			irq &= (num_irqs-1);
			pending[irq >> word_shift] |= (1ULL << (irq & (bits_per_word-1)));
		}

		bool irq_pending(UX mode, UX node_id, UX hart_id)
		{
			UX is_pending = 0;
			UX en_off = (node_id * NUM_HARTS * irq_words) + (hart_id * irq_words);
			switch (mode) {
				case 0:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[en_off + i]);
					}
					break;
				case 1:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[en_off + i] & (priority0[i] | priority1[i]));
					}
					break;
				case 2:
					for (size_t i = 0; i < irq_words; i++) {
						is_pending |= (pending[i] & enabled[en_off + i] & priority0[i]);
					}
					break;
				case 3:
					for (size_t i = 0; i < irq_words; i++) {
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
