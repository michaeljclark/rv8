RISC-V Binary Translation
==============================

Dynamic Binary translation is the process of translating binary code for
one architecture to another on the fly.

Crypto Binary Translation is the process of translating binary code for
one architecture to another while introducing entropy into the translation
to prevent attacks based on static code analysis.

Crypto Binary Translation
-------------------------------

Present day attacks on operating systems and application software are
typically based on static analysis of binaries. The exploitation of
software vulnerabilities uses static information based on known versions
of executables running on the target system. Return Orientated Programming
(ROP) attacks use the location of known executable gadgets in the target
application binary image to bypass present day security defenses such as
non-executable heap and stack. ROP-oriented programming allows escalation
of stack buffer overflow vulnerabilities by injecting the return addresses
of executable gadgets onto the stack, diverting control flow on procedure
return, allowing escalation of a stack buffer overflow into a potential
remote execution vulnerability.

The assumption made by all present zero day shell code or exploits is that
payloads can rely on a known execution environment such as the ability to
call C library routines or system calls to elevate the exploit sequence into
executing payload code on the target system, ultimately escalating privileges.
These assumptions are based on the target system exposing a standard (ABI)
Application Binary Image.

Crypto Binary Translation introduces runtime interposition between
applications and the target operating system by using keyspace in the
ABI (Application Binary Interface) to bond an application to a custom
randomised version of the ABI, thus preventing all exploits that are based
on static analysis of binaries and the target system ABI. Addresses, System
call numbers, register assignments and code layout are randomised into a
state-space whereby each application is bonded to the target operating
system making the target system immune to traditional exploit payloads.
If the state-space of possible ABI combinations is of a high enough order e.g.
2^n where n > 112 bits, then it becomes a quantum cryptographic problem
to find the ABI key. This approach differs radically to code signature
systems that verify executables or traditional anti-virus that work on
pattern recognition. In the case of a cryptographic ABI, to exploit a
vulnerability, the exploit writer must not only exploit a bug that allows
execution of their payload, they must search the state-space of possible
ABI randomisations before their payload will execute.

Present day systems make use of Address Space Layout Randomisation (ASLR)
to introduce a small amount of entropy into the address space making
attacks based on static function addresses more difficult, however ASLR
systems are limited in the amount of address space they can use for
randomisation due to the use of canonical pointers (sign extended pointers).

Canonical Pointers are enforced on present day commodity 64-bit systems
under the guise of allowing for address space growth, and the net result
is that they limit the address space entropy to the order of 20 bits. Also,
the use of arbitrary memory read primitives can be used to leak the offset
used in the randomisation of the address space thereby providing the
ability to bypass ASLR. Brute force can also be used effectively against
the low orders of entropy use in present day ASLR.

The following is a list of entropy vectors that can be introduced during
dynamic binary translation to obfuscate the in-memory image of a translated
executable to prevent present day static analysis techniques:

- Large Address Space Entropy
- Execute Only Memory Pages
- Upcall Trampolines
- Basic Block Reorder Entropy
- Register Assignment Entropy
- Syscall Table Entropy

The combination of these techniques shall be referred to as Cryptographic
Binary Translation using an entropy coded ABI.

## Large Address Space Entropy

Using the large address space introduced by 128-bit systems combined with
the relaxation of canonical pointer requirements, the additional address
space in code pointers can be used for embedding keys. The amount of entropy
in pointers on a system that requires 40-bits of address space increases
from 24 bits to 88 bits when moving from 64-bits to 128-bits. Large Address
Space Entropy needs changes to the target system MMU (Memory Management Unit)
to allow keys to be stored in unused pointer bits.

## Execute Only Memory Pages

With the introduction of execute-only memory pages in microprocessor memory
protection units, there becomes the possibility of loading secrets from memory
in the target application address space that are no longer subject to
exfiltration using exploitation of arbitrary memory read primitives. The
combination of large address space entropy and execute-only memory pages
provides a mechanism for applications to load encrypted pointers containing
a large amount of entropy with limited exfiltration potential.

## Upcall Trampolines

Execute Only Memory Pages located at randomised addresses can contain
upcall keys that are loaded using immediate instructions that can't be
exfiltrated using arbitrary memory read primitives. Upcall keys are loaded
in trampoline code sequences at randomised addresses and are used to call
privileged functions in the operating system or hypervisor.

## Basic Block Reorder Entropy

In compiled code, a basic block is a sequence of code without branches.
Typical exploits against static code such as ROP (Return Orientated
Programming) exploits scan known binaries for known offsets. Basic
Block Reorder Entropy changes the order of each basic block during
dynamic binary translation by randomising the placement either before
or after the current basic block. The amount of entropy rises with
the number of basic blocks in the code. If the number of basic blocks
is above 128, then more than 128 bits of entropy or 2^128 basic block
order combinations can be added to the state space of the target binary
memory image, making the system immune to attacks based on static offsets
in the target binary image.

## Register Assignment Entropy

Similarly to basic block reorder entropy, the register assignment in
the target code can be randomised during translation. With 32 registers
there are 32 factorial combinations of register assignments. The
Cryptographic Binary Translation system can create operating system
stubs or trampolines that translate from the register assignment in
the application state space to the system state space, and the system
state space may also be distinct from the static code state space.

## Syscall Table Entropy

System Call Table Entropy uses the key space provided by larger pointers
to embed system call keys. The runtime link loader is changed such that
code sequences in the target application memory image have per syscall
keys added into the code sequences that call system calls. Any payload
that works on static code assumptions, such as static syscall numbers
will fail when running on the target system. System call trampolines will
be placed in Execute Only Memory Pages so that arbitrary memory read
primitives cannot be used to exfiltrate the system call keys.
