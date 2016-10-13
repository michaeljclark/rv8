Assembler
==============

## Absolute addressing example

```
	lui a1, %hi(msg)            # load msg(hi)
	addi a1, a1, %lo(msg)       # load msg(lo)
```

## Relative addressing example

```
1:	auipc a1, %pcrel_hi(msg)    # load msg(hi)
	addi a1, a1, %pcrel_lo(1b)  # load msg(lo)
```

## Addressing expressions

- %hi(symbol)
- %lo(symbol)
- %pcrel_hi(symbol)
- %pcrel_lo(label)
- %tls_ie_pcrel_hi(symbol)       # Thread local storage with TLS model "Initial Exec"
- %tls_gd_pcrel_hi(symbol)       # Thread local storage with TLD model "Global Dynamic"
- %tprel_hi(symbol)
- %tprel_lo(label)
- %tprel_add(x)
