# Register Allocation

The translator maps RISC-V registers to host registers
and the remaining registers are assigned to static spill slots.
Dynamic register allocation is too expensive to perform during
runtime translation as the graph colouring problem is NP Complete.
Instead the translator takes advantage of the static location
of the spill frame such that spilt registers remain in L1 cache.

## Mapping Randomisation

The register mapping may eventually be randomised by the translator.

## Dynamic Register Usage

TODO - analyse dynamic register usage to refine static mapping.

## Register Mapping Sketch

 - rax # translator temporary (IMUL/DIV, temporary for destructive ALU ops)
 - rdx # translator temporary (IMUL/DIV)
 - rsp # translator static spill slots ([rsp + 0] pointer to translator private)

Reg | Reg  | Target register (or static spill slot)
:-- | :--  | :--
x0  | zero |
x1  | ra   | rcx
x2  | sp   | rbp
x3  | gp   | [rsp + 8]
x4  | tp   | [rsp + 16]
x5  | t0   | [rsp + 24]
x6  | t1   | [rsp + 32]
x7  | t2   | [rsp + 40]
x8  | s0   | rbx
x9  | s1   | rdi
x10 | a0   | r8
x11 | a1   | r9
x12 | a2   | r10
x13 | a3   | r11
x14 | a4   | r12
x15 | a5   | r13
x16 | a6   | [rsp + 48]
x17 | a7   | [rsp + 56]
x18 | s2   | rsi
x19 | s3   | r14
x20 | s4   | r15
x21 | s5   | [rsp + 64]
x22 | s6   | [rsp + 72]
x23 | s7   | [rsp + 80]
x24 | s8   | [rsp + 88]
x25 | s9   | [rsp + 96]
x26 | s10  | [rsp + 104]
x27 | s11  | [rsp + 112]
x28 | t3   | [rsp + 120]
x29 | t4   | [rsp + 128]
x30 | t5   | [rsp + 136]
x31 | t6   | [rsp + 144]
