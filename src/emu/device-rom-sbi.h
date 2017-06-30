//
//  device-sbi.h
//

#ifndef rv_device_sbi_h
#define rv_device_sbi_h

#include "sbi-rom-bin.h"

namespace riscv {

	/* SBI MMIO device */

	template <typename P>
	struct sbi_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;
		typedef memory_segment<typename P::ux> seg_type;

		P &proc;

		constexpr u8* as_u8() { return (u8*)&build_riscv64_unknown_elf_bin_sbi_rom_bin[0]; }
		constexpr u16* as_u16() { return (u16*)&build_riscv64_unknown_elf_bin_sbi_rom_bin[0]; }
		constexpr u32* as_u32() { return (u32*)&build_riscv64_unknown_elf_bin_sbi_rom_bin[0]; }
		constexpr u64* as_u64() { return (u64*)&build_riscv64_unknown_elf_bin_sbi_rom_bin[0]; }

		/* SBI constructor */

		sbi_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>("SBI", mpa, /*uva*/0,
				/*size*/build_riscv64_unknown_elf_bin_sbi_rom_bin_len,
				pma_type_main | pma_prot_read | pma_prot_execute), proc(proc) {}

		/* SBI MMIO */

		buserror_t load_8 (UX va, u8  &val)
		{
			val = (va < seg_type::size) ? *(as_u8() + va) : 0;
			if (proc.log & proc_log_mmio) {
				printf("sbi_mmio :0x%04llx -> 0x%02hhx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_16(UX va, u16 &val)
		{
			val = (va < seg_type::size - 1) ? *(as_u16() + (va>>1)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("sbi_mmio :0x%04llx -> 0x%04hx\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_32(UX va, u32 &val)
		{
			val = (va < seg_type::size - 3) ? *(as_u32() + (va>>2)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("sbi_mmio :0x%04llx -> 0x%08x\n", addr_t(va), val);
			}
			return 0;
		}

		buserror_t load_64(UX va, u64 &val)
		{
			val = (va < seg_type::size - 7) ? *(as_u64() + (va>>3)) : 0;
			if (proc.log & proc_log_mmio) {
				printf("sbi_mmio :0x%04llx -> 0x%016llx\n", addr_t(va), val);
			}
			return 0;
		}

	};

}

#endif
