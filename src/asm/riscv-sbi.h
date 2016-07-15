//
//  riscv-sbi.h
//

#ifndef riscv_sbi_h
#define riscv_sbi_h

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Supervisor Binary Interface (SBI) from version 1.9
 * of the RISC-V Privileged Architecture Specification
 */

const char* sbi_get_config(void);
size_t sbi_num_harts(void);
size_t sbi_hart_id(void);
int sbi_send_ipi(size_t hart_id);
bool sbi_clear_ipi(void);
void sbi_shutdown(void);
int sbi_console_putchar(uint8_t ch);
int sbi_console_getchar(void);
void sbi_remote_sfence_vm(const uintptr_t* harts, size_t asid);
void sbi_remote_sfence_vm_range(const uintptr_t* harts, size_t asid,
	uintptr_t start, uintptr_t size);
void sbi_remote_fence_i(const uintptr_t* harts);
int sbi_mask_interrupt(int which);
int sbi_unmask_interrupt(int which);

#ifdef __cplusplus
}
#endif

#endif
