Term detectors
====================

Lift and translate epimorphisms

- Address term (auipc)
- Constant term (lui/shift/add/sub)
- Nop (many nop encodings)
- Useless branch term (branch to next, branch to branch)
- Spin term detection (unconditional jump to self)
- Implicit Function continuation term (branch target)
- Function ABI lift - Epilog/Prolog detection
- Stack spill term (translate into VLIW SSA form)
- Rotate (sub/shift/shift/or) -,>>,<<,| -> >>>,<<<
- Predication (lift branches; many forms of predication)
