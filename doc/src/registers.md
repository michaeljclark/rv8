# Register Allocation

The translator maps RISC-V registers to host registers
and the remaining registers are assigned to static spill slots.
Dynamic register allocation is too expensive to perform during
runtime translation as the graph colouring problem is NP Complete.
Instead the translator takes advantage of the static location
of the spill frame such that spilt registers remain in L1 cache.

## Mapping Randomisation

The register mapping may eventually be randomised by the translator.

## Register Mapping Sketch

The following is a sketch of a static register mapping for x86:

 - rax, translator temporary (IMUL/DIV, temporary for destructive ALU ops)
 - rdx, translator temporary (IMUL/DIV)
 - rsp, translator static spill slots ([rsp + 0] pointer to translator private area)

Reg | Reg  | Target register (or static spill slot)
:-- | :--  | :--
x0  | zero |
x1  | ra   | rcx
x2  | sp   | rbp
x3  | gp   | [rsp + 8]
x4  | tp   | [rsp + 16]
x5  | t0   | rsi
x6  | t1   | rdi
x7  | t2   | rbx
x8  | s0   | [rsp + 24]
x9  | s1   | [rsp + 32]
x10 | a0   | r8
x11 | a1   | r9
x12 | a2   | r10
x13 | a3   | r11
x14 | a4   | r12
x15 | a5   | r13
x16 | a6   | r14
x17 | a7   | r15
x18 | s2   | [rsp + 40]
x19 | s3   | [rsp + 48]
x20 | s4   | [rsp + 56]
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

## Dynamic Register Usage

Dynamic register usage histogram booting linux kernel to a shell:

```
    1. a5         22.78% [46017546] ###############################################################################
    2. a1         10.16% [20517817] ###################################
    3. a3          9.56% [19311041] #################################
    4. zero        8.64% [17457690] #############################
    5. a4          7.76% [15683487] ##########################
    6. a2          5.83% [11784465] ####################
    7. a6          3.65% [7368631] ############
    8. a0          3.40% [6865335] ###########
    9. sp          3.23% [6530200] ###########
   10. t1          3.10% [6271867] ##########
   11. t0          3.08% [6231171] ##########
   12. a7          2.88% [5820932] #########
   13. s0          2.77% [5598309] #########
   14. s1          1.77% [3570413] ######
   15. ra          1.66% [3345125] #####
   16. s10         1.49% [3010741] #####
   17. s3          1.23% [2491997] ####
   18. s11         1.06% [2139253] ###
   19. s2          0.86% [1746755] ##
   20. t4          0.72% [1451744] ##
   21. t5          0.69% [1393237] ##
   22. t2          0.64% [1300304] ##
   23. t6          0.50% [1005260] #
   24. s7          0.48% [968546] #
   25. s5          0.44% [881136] #
   26. s8          0.38% [776592] #
   27. s6          0.38% [764312] #
   28. s4          0.28% [572383] 
   29. s9          0.27% [551745] 
   30. t3          0.26% [520438] 
   31. tp          0.02% [40218 ] 
   32. gp          0.01% [11215 ] 
```
