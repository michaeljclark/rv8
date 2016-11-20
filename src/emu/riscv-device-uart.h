//
//  riscv-device-uart.h
//

#ifndef riscv_device_uart_h
#define riscv_device_uart_h

namespace riscv {

	/* UART MMIO device */

	template <typename P>
	struct uart_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		P &proc;

		/* UART Registers */

		struct {
			u8 RBR; /* (R  [0]) Recieve Buffer Register        */
			u8 THR; /* (W  [0]) Transmit Holding Register      */
			u8 IER; /* (RW [1]) Interrupt Enable Register      */
			u8 IIR; /* (R  [2]) Interrupt Identity Register    */
			u8 FCR; /* (W  [2]) FIFO Control Register          */
			u8 LCR; /* (RW [3]) Line Control Register          */
			u8 MCR; /* (RW [4]) MODEM Control Register         */
			u8 LSR; /* (RW [5]) Line Status Register           */
			u8 MSR; /* (RW [6]) MODEM Status Register          */
			u8 SCR; /* (RW [7]) Scratch Register               */
			u8 DLL; /* (RW [0]) Divisor Latch LSB (LCR.DLAB=1) */
			u8 DLM; /* (RW [0]) Divisor Latch MSB (LCR.DLAB=1) */
		} regs;

		/* Register MMIO offsets in brackets (Based on 16550)  */

		/* TODO implement UART MMIO access and functionality */

		uart_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>(mpa, /*uva*/0, /*size*/8,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc) {}

		void load_8 (UX va, u8  &val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx -> invalid\n", addr_t(va)); val = 0; }
		void load_16(UX va, u16 &val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx -> invalid\n", addr_t(va)); val = 0; }
		void load_32(UX va, u32 &val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx -> invalid\n", addr_t(va)); val = 0; }
		void load_64(UX va, u64 &val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx -> invalid\n", addr_t(va)); val = 0; }

		void store_8 (UX va, u8  val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx <- invalid\n", addr_t(va)); }
		void store_16(UX va, u16 val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx <- invalid\n", addr_t(va)); }
		void store_32(UX va, u32 val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx <- invalid\n", addr_t(va)); }
		void store_64(UX va, u64 val) { if (proc.log & proc_log_mmio) printf("uart_mmio:0x%04llx <- invalid\n", addr_t(va)); }
	};

}

#endif
