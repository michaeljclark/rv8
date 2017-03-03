## Direct versus Indirect Jumps

In static analysis of large static binaries, many library calls are in
the form of AUIPC+JALR versus JAL (which has +/-1MiB range).

JALR is a register indirect jump and link return address instruction,
and interestingly register indirect calls and returns are particularly
hard for dynamic binary translation. A translator typically needs to
inject a stub at the translation point that looks up the address of
the translation for the ‘dynamic’ target address, as the target address
is not known at the time of translation. There are some interesting
techniques in the literature, such that these stubs can learn the static
address and later rewrite the indirect jump as a direct jump.

Indirect jumps are also likely slightly harder for microarchitectures
due to requiring a register read to learn the target address for
instruction prefetch. i.e. higher latency to get the target address
further down the pipeline versus as an immediate during decode.

While JALR is technically a register indirect jump, the fused adjacent
combination of AUIPC+JALR can technically be seen as a direct PC
relative jump and link with load target address (as a side effect)
and on the contrary can be efficiently translated. In a microarchitecture
the target address could be decoded and prefetched before the register
value has been comitted.

The observation (thought experiment) is that one of these AUIPC+JALR
can later be split, and the JALR can potentially be used as a ROP gadget
given enough diversity of offsets one might be able to get a return
address onto the stack pointing to an adjacent function given a known
value for the temporary (the `t1` temporary from the last indirect call
in code that is being exploited). From a binary translation perspective,
the trace for the basic block target address for the split entry point
would not exist and would be re-translated starting with JALR, and the
JALR would be treated as an unfused indirect JALR and require a stub.

```
	auipc t1, pc + 1589248
1:
	jalr ra, t1, 324 # <memset>
	…
	auipc t1, pc + 1576960
	jalr  ra, t1, -164 # <strcmp>
	ret

	…

	jal x0, 1b # or ra value restored from xyz(sp)
```

This is pseudo assembly that shows a possible translation:

```
	lea rcx, [label - RIP]  ; rcx is an alias for ra
	mov rax, target ; populate temporary with target address (but no dependency)
	jmp target ; we know target is fixed from the AUIPC+JALR instruction pair
label:
```

or the technically correct register indirect jump which might have a higher latency:

```
	lea rcx, [label - RIP]  ; rcx is an alias for ra
	mov rax, target
	jmp rax ; depends on the previous instruction
label:
```

and if ra is zero:

```
	mov rax, target 
	jmp target ; direct jump, could be RIP relative
```

instead of:

```
	mov rax, target
	jmp rax ; indirect jump
```

which leads to threaded implementations that indirect through a constant
pool offset table that could contain stubs:

```
	lea rcx, [label - RIP]  ; rcx is an alias for ra
	mov rax, QWORD PTR [target_slot] ; populate temporary (no register dependency)
	jmp rax
label:
```

and if ra is zero:

```
	mov rax, QWORD PTR [target_slot]
	jmp rax
label:
```
