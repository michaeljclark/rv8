# Register Allocation

The translator maps a subset of RISC-V registers to host registers
and the remaining registers are assigned to a spill area.

The optimal register assignment method of lifting registers into
SSA form and solving the graph colouring problem is NP Complete.

The translator may implement a form of dynamic register renames
by setting a subset of `n` bits in a 32-bit word with the bit
position indicating the register assignment for a basic block.

## Mapping Randomisation

The register mapping may eventually be randomised by the translator.

## Register Mapping Sketch

The following is a sketch of a static register mapping for x86:

 - rax, translator temporary
 - rcx, translator temporary (cl used as shift argument)
 - rbp, translator static spill slots ([rbp + 0] holds the program counter)
 - rsp, translator call stack

Reg | Reg  | x86 | Spill Slot
:-- | :--  | :-- | :--
x0  | zero |     |
x1  | ra   | rdx | [rbp + 16]
x2  | sp   | rbx | [rbp + 26]
x3  | gp   |     | [rbp + 32]
x4  | tp   |     | [rbp + 40]
x5  | t0   | rsi | [rbp + 48]
x6  | t1   | rdi | [rbp + 56]
x7  | t2   |     | [rbp + 64]
x8  | s0   |     | [rbp + 72]
x9  | s1   |     | [rbp + 80]
x10 | a0   | r8  | [rbp + 88]
x11 | a1   | r9  | [rbp + 96]
x12 | a2   | r10 | [rbp + 104]
x13 | a3   | r11 | [rbp + 112]
x14 | a4   | r12 | [rbp + 120]
x15 | a5   | r13 | [rbp + 128]
x16 | a6   | r14 | [rbp + 136]
x17 | a7   | r15 | [rbp + 144]
x18 | s2   |     | [rbp + 152]
x19 | s3   |     | [rbp + 160]
x20 | s4   |     | [rbp + 168]
x21 | s5   |     | [rbp + 176]
x22 | s6   |     | [rbp + 184]
x23 | s7   |     | [rbp + 192]
x24 | s8   |     | [rbp + 200]
x25 | s9   |     | [rbp + 208]
x26 | s10  |     | [rbp + 216]
x27 | s11  |     | [rbp + 224]
x28 | t3   |     | [rbp + 232]
x29 | t4   |     | [rbp + 240]
x30 | t5   |     | [rbp + 248]
x31 | t6   |     | [rbp + 256]

## Dynamic Register Usage

Dynamic register usage histogram booting linux kernel to a shell:

```
    1. a5         22.78% [46017546 ] ###############################################################################
    2. a1         10.16% [20517817 ] ###################################
    3. a3          9.56% [19311041 ] #################################
    4. zero        8.64% [17457690 ] #############################
    5. a4          7.76% [15683487 ] ##########################
    6. a2          5.83% [11784465 ] ####################
    7. a6          3.65% [7368631  ] ############
    8. a0          3.40% [6865335  ] ###########
    9. sp          3.23% [6530200  ] ###########
   10. t1          3.10% [6271867  ] ##########
   11. t0          3.08% [6231171  ] ##########
   12. a7          2.88% [5820932  ] #########
   13. s0          2.77% [5598309  ] #########
   14. s1          1.77% [3570413  ] ######
   15. ra          1.66% [3345125  ] #####
   16. s10         1.49% [3010741  ] #####
   17. s3          1.23% [2491997  ] ####
   18. s11         1.06% [2139253  ] ###
   19. s2          0.86% [1746755  ] ##
   20. t4          0.72% [1451744  ] ##
   21. t5          0.69% [1393237  ] ##
   22. t2          0.64% [1300304  ] ##
   23. t6          0.50% [1005260  ] #
   24. s7          0.48% [968546   ] #
   25. s5          0.44% [881136   ] #
   26. s8          0.38% [776592   ] #
   27. s6          0.38% [764312   ] #
   28. s4          0.28% [572383   ] 
   29. s9          0.27% [551745   ] 
   30. t3          0.26% [520438   ] 
   31. tp          0.02% [40218    ] 
   32. gp          0.01% [11215    ] 
```
