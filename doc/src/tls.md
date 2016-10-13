Thread Local Storage
=========================

There are 4 TLS models:

- local-exec
- initial-exec
- local-dynamic
- global-dynamic

## Local Exec

Use for non-PIC static executables.

- Linker errors:  Fail when symbol is imported.
- Runtime errors: Fail when symbol is imported. Fail when the symbol is not in the executable.
- Compiler flag `-ftls-model=local-exec`
- Variable attribute: `__thread int i __attribute__((tls_model("local-exec")));`
- Constant: TLS_MODEL_LOCAL_EXEC

## Initial Exec

Use for PIC dynamic executables.

- Linker errors: None
- Runtime errors: Fail when symbol is imported after load. e.g. `dlopen`
- Compiler flag `-ftls-model=inital-exec`
- Variable attribute: `__thread int i __attribute__((tls_model("inital-exec")));`
- Constant: TLS_MODEL_INITIAL_EXEC
- Asm: `la.tls.ie; tp-relative add`
- ELF flags: DF_STATIC_TLS

## Local Dynamic

Use for PIC Shared Libraries (and `-Bsymbolic`?).

- Linker errors: Fail when symbol is imported.
- Runtime: Fail when symbol is imported.
- Compiler flag `-ftls-model=local-dynamic`
- Variable attribute: `__thread int i __attribute__((tls_model("local-dynamic")));`
- Constant: TLS_MODEL_LOCAL_DYNAMIC

## Global Dynamic

Use for PIC Shared Libraries.

- Linker errors: None.
- Runtime errors: None.
- Compiler flag `-ftls-model=global-dynamic`
- Variable attribute: `__thread int i __attribute__((tls_model("global-dynamic")));`
- Constant: TLS_MODEL_GLOBAL_DYNAMIC
- Asm: `la.tls.gd`

