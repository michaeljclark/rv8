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
    1. a5        [48330328] ###############################################################################
    2. a1        [20518445] #################################
    3. a3        [19311163] ###############################
    4. zero      [18614032] ##############################
    5. a4        [15683870] #########################
    6. a2        [11784550] ###################
    7. a6        [7368641] ############
    8. a0        [6865477] ###########
    9. sp        [6626824] ##########
   10. t1        [6271881] ##########
   11. t0        [6231735] ##########
   12. a7        [5820946] #########
   13. s0        [5791189] #########
   14. ra        [3923221] ######
   15. s1        [3666804] #####
   16. s10       [3010765] ####
   17. s3        [2492030] ####
   18. s11       [2139280] ###
   19. s2        [1746801] ##
   20. t4        [1451744] ##
   21. t5        [1393237] ##
   22. t2        [1300304] ##
   23. t6        [1005260] #
   24. s7        [968568] #
   25. s5        [881161] #
   26. s8        [776605] #
   27. s6        [764332] #
   28. s4        [572425] 
   29. s9        [551768] 
   30. t3        [520445] 
   31. tp        [40222 ] 
   32. gp        [11215 ] 
```
