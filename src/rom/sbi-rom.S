#
# sbi-rom.S
#

# MCALL definitions

.equ MCALL_HART_ID,                 0
.equ MCALL_NUM_HARTS,               10
.equ MCALL_QUERY_MEMORY,            11
.equ MCALL_CONSOLE_PUTCHAR,         1
.equ MCALL_CONSOLE_GETCHAR,         2
.equ MCALL_HTIF_SYSCALL,            3
.equ MCALL_SEND_IPI,                4
.equ MCALL_CLEAR_IPI,               5
.equ MCALL_TIMEBASE,                12
.equ MCALL_SHUTDOWN,                6
.equ MCALL_SET_TIMER,               7
.equ MCALL_MASK_INTERRUPT,          13
.equ MCALL_UNMASK_INTERRUPT,        14
.equ MCALL_REMOTE_SFENCE_VM,        8
.equ MCALL_REMOTE_SFENCE_VM_RANGE,  15
.equ MCALL_REMOTE_FENCE_I,          9

  .section .text
  .option norvc
  .globl _start
_start:

  # TODO: figure out something better to do with this space.  It's not
  # protected from the OS, so beware.
  .skip 2048

  # hart_id          -2048
  .p2align 4
  .globl sbi_hart_id
  li a7, MCALL_HART_ID
  ecall
  ret

  # num_harts        -2032
  .p2align 4
  .globl sbi_num_harts
  li a7, MCALL_NUM_HARTS
  ecall
  ret

  # query_memory     -2016
  .p2align 4
  .globl sbi_query_memory
  li a7, MCALL_QUERY_MEMORY
  ecall
  ret

  # console_putchar  -2000
  .p2align 4
  .globl sbi_console_putchar
  li a7, MCALL_CONSOLE_PUTCHAR
  ecall
  ret

  # console_getchar  -1984
  .p2align 4
  .globl sbi_console_getchar
  li a7, MCALL_CONSOLE_GETCHAR
  ecall
  ret

 # htif_syscall      -1968
  .p2align 4
  .globl sbi_htif_syscall
  li a7, MCALL_HTIF_SYSCALL
  ecall
  ret

  # send ipi          -1952
  .p2align 4
  .globl sbi_send_ipi
  li a7, MCALL_SEND_IPI
  ecall
  ret

  # clear ipi         -1936
  .p2align 4
  .globl sbi_clear_ipi
  li a7, MCALL_CLEAR_IPI
  ecall
  ret

  # timebase          -1920
  .p2align 4
  .globl sbi_timebase
  li a7, MCALL_TIMEBASE
  ecall
  ret

  # shutdown          -1904
  .p2align 4
  .globl sbi_shutdown
  li a7, MCALL_SHUTDOWN
  ecall
  ret

  # set_timer         -1888
  .p2align 4
  .globl sbi_set_timer
  li a7, MCALL_SET_TIMER
  ecall
  ret

  # mask_interrupt    -1872
  .p2align 4
  .globl sbi_mask_interrupt
  li a7, MCALL_MASK_INTERRUPT
  ecall
  ret

  # unmask_interrupt  -1856
  .p2align 4
  .globl sbi_unmask_interrupt
  li a7, MCALL_UNMASK_INTERRUPT
  ecall
  ret

  # remote_sfence_vm  -1840
  .p2align 4
  .globl sbi_remote_sfence_vm
  li a7, MCALL_REMOTE_SFENCE_VM
  ecall
  ret

  # remote_sfence_vm_range -1824
  .p2align 4
  .globl sbi_remote_sfence_vm_range
  li a7, MCALL_REMOTE_SFENCE_VM_RANGE
  ecall
  ret

  # remote_fence_i    -1808
  .p2align 4
  .globl sbi_remote_fence_i
  li a7, MCALL_REMOTE_FENCE_I
  ecall
  ret

  # end of SBI trampolines

  .globl do_mcall
do_mcall:
  mv a7, a0
  mv a0, a1
  mv a1, a2
  ecall
  ret

.p2align 12
  .globl sbi_end
sbi_end:
