# CLIC

This (unofficial) _draft_ document describes a sketch for platform-level and core-level interrupt controllers.

- PLIC (Platform Level Interrupt Controller)
- CLIC (Core Level Interrupt Controller)

This current [MMIO](mmio.md) layout for `rv-sim` has a MIPI aperture for interprocessor interrupts, a TIME aperture for mtime and mtimecmp as per the specification, and a UART for console boot. The remaining pieces are completion of SBI stubs or alternatively a model where OS drivers interact directly with well-documented MMIO regions.

This docuument proposes something a little more different to the current MMIO layout by the addition of a CLIC (Core Level Interrupt Controller) MMIO aperture which contains the timers, timer compare registers and interprocessor interrupt registers.

The thoughts that out of the process of implementing version 1.9.1 of the privileged ISA spec:

- PLIC layout is vague. 
- MIPI hart bit field or integer MMIO register for sending IPIs, mtime and mtimecmp could be coalesced into a CLIC (Core Level Interrupt Controller) device
- Question whether MIPI would be better placed in the PLIC as a bit field, given it has wires to cores in the cluster or if there are IPI fields in each CLIC
- Prefer {M,H,S,U}IPI is an integer per CLIC that can indicate a XLEN-bit message communicated during the IPI (sketched below e.g. 1 = TLB SHOOT-DOWN, etc)
- CLIC is per hart, and each hart requires their own timer compare registers for each priority/privilege level (same as a typical linux SMP setup).

## PLIC (Platform Level Interrupt Controller) MMIO Region (per node)

- pending bitfield (per IRQ), whether an interrupt has been triggered, clear to acknowledge.
- priority bitfield (per IRQ), 4 priority levels that map to masks for the 4 privilege levels. i.e. 2-bits per IRQ
- enabled bitfield (per NODE, per HART, per IRQ) that designates whether the destination hart is masked for each IRQ.
- things we have forgotten or are unaware of

```
u64 pending[irq_words]
u64 pritority0[irq_words]
u64 pritority1[irq_words]
u64 enabled[enabled_words]
```
More detail is available in PLIC section of the current [MMIO](mmio.md) layout document.

## CLIC (Core Level Interrupt Controller) MMIO Region (per core)

- mipi,hipi,sipi,uipi, 4-words (M mandatory) each corresponding to raising software interrupts at M,H,S,U levels. e.g. msip, hsip, ssip, usip
- mtime, mtimecmp, htimecmp, stimecmp, utimecmp
- mtie, htie, stie, utie would control which timers are active for the “core” raising timer interrupts at M,H,S,U levels e.g. mtip, htip, stip, utip
- the mipi region would be accessible from other cores to raise interprocessor interrupts.
- if mipi,hipi,sipi,uipi are words, then the word can store a message signal (0 means no interrupt pending)
- the IPI word is communicated along with the level m_software, h_software, s_software, u_software
- e.g. a hart receiving s_software can read sipi to get message dispatch info (e.g. TLB shoot-down vs queue wakeup) 
- things we have forgotten or are unaware of

```
u64 mtime
u64 mtimecmp
u64 htimecmp /* reserved */
u64 stimecmp /* reserved */
u64 utimecmp /* reserved */
u64 mipi
u64 hipi /* reserved */
u64 sipi /* reserved */
u64 uipi /* reserved */
```

## Comments

It seems logical that per core timers and IPI be combined together into a CLIC (Core Level Interrupt Controller).

The document does not suggest to follow these memory layouts verbatim, rather raises the issue of conceptual separation of the node level PLIC and the hart level CLIC. This document contains a little more details on specific layout than the current work-in-progress privileged specification… 

The current the current [MMIO](mmio.md) layout document more closely matches the current privileged specification however this document proposes a sketch that has consideration of SMP timers, prioritoes, and (optional) S-mode autonomy for timers, for implementations with S-mode and of course the optional H-mode and U-mode timers with the H and N extensions in a full scale implementation.

## UART

Suggest the platform spec mentions a de-facto standard such as an 8550 or 16550 style UART for console. The UART was present in the config string example in one of the earlier versions of the privileged spec. I also see 16550 UART in PowerPC Linux and MIPS Linux ports as an early boot console devices. I am not sure whether there are IP issues with the use of 16550 2x5 headers and register layout (essentially a struct). A UART makes early boot logging much much easier for bring up. Ethernet, Framebuffer and VirtIO are clearly more complicated for early bring up stage.

## Interrupt Routing

Interrupt routing needs consideration. Routing can be static or dynamic. The current model wires the UART to IRQ 3 on the PLIC. The intent is that static interrupt routing is described in configuration or an interface is provided in the PLIC for dynamic routing (PCI bus pin to IRQ mappings). Physical Interrupt wires on various devices can be statically or dynamically assigned to IRQs. It may be such that the wiring is indicated in the config. This is so that interrupt trap entry knows which device driver bottom half ISR routines to call without requiring IRQs to be hardcoded. Static mapping is sufficient for many purposes, but PCI may require dynamic bus interrupt pin to IRQ routing.