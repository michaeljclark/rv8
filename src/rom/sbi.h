#ifndef _ASM_RISCV_SBI_H
#define _ASM_RISCV_SBI_H

typedef struct {
  unsigned long base;
  unsigned long size;
  unsigned long node_id;
} memory_block_info;

unsigned long sbi_query_memory(unsigned long id, memory_block_info *p);

unsigned long sbi_hart_id(void);
unsigned long sbi_num_harts(void);
unsigned long sbi_timebase(void);
void sbi_set_timer(unsigned long long stime_value);
void sbi_send_ipi(unsigned long hart_id);
unsigned long sbi_clear_ipi(void);
void sbi_shutdown(void);

void sbi_console_putchar(unsigned char ch);
int sbi_console_getchar(void);

void sbi_remote_sfence_vm(unsigned long hart_mask_ptr, unsigned long asid);
void sbi_remote_sfence_vm_range(unsigned long hart_mask_ptr, unsigned long asid, unsigned long start, unsigned long size);
void sbi_remote_fence_i(unsigned long hart_mask_ptr);

unsigned long sbi_mask_interrupt(unsigned long which);
unsigned long sbi_unmask_interrupt(unsigned long which);

#endif
