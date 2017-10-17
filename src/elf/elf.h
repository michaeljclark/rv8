//
//  elf.h
//

#ifndef rv_elf_h
#define rv_elf_h

/*

 References:

 - TIS Portable Formats Specification, Version 1.1 ELF: Executable and Linkable Format
 - ELF-64 Object File Format, Version 1.5 Draft 2
 - System V Application Binary Interface AMD64 Architecture Processor Supplement Draft 0.99.6

*/

// ELF Types

/*
 * Type aliases are defined here instead of using stdint.h types due to
 * the use of signed long int and unsigned long int for int64_t and uint64_t
 * by some library headers. These definitions are compatible with ILP32, LLP64
 * and LP64, which supports Windows and SVR4 ABIs for x86 and RISC-V.
 */

typedef unsigned char      _elf_u8;
typedef unsigned short int _elf_u16;
typedef signed int         _elf_s32;
typedef unsigned int       _elf_u32;
typedef signed long long   _elf_s64;
typedef unsigned long long _elf_u64;

// Elf32 types
typedef _elf_u32 Elf32_Addr;
typedef _elf_u16 Elf32_Half;
typedef _elf_u32 Elf32_Off;
typedef _elf_s32 Elf32_Sword;
typedef _elf_u32 Elf32_Word;
typedef _elf_u8  Elf32_Byte;

// Elf64 types
typedef _elf_u64 Elf64_Addr;
typedef _elf_u16 Elf64_Half;
typedef _elf_u64 Elf64_Off;
typedef _elf_s32 Elf64_Sword;
typedef _elf_u32 Elf64_Word;
typedef _elf_u64 Elf64_Xword;
typedef _elf_s64 Elf64_Sxword;
typedef _elf_u8  Elf64_Byte;

// ELF Auxilliary table

/*
   STACK TOP
   env data
   arg data
   padding, align 16
   auxv table, AT_NULL terminated
   envp array, null terminated
   argv pointer array, null terminated
   argc <- stack pointer
 */

// a_type
enum {
	AT_NULL = 0,                     /* End of table */
	AT_PHDR = 3,                     /* Address of the ELF program headers (used by interpreter) */
	AT_PHENT = 4,                    /* Size of the ELF program headers (ehdr.e_phentsize) */
	AT_PHNUM = 5,                    /* Number of ELF program headers (ehdr.e_phnum) */
	AT_PAGESZ = 6,                   /* Page size */
	AT_BASE = 7,                     /* Load address of interpreter */
	AT_FLAGS = 8,                    /* Flags (0x0) */
	AT_ENTRY = 9,                    /* Program entry point */
	AT_UID = 11,                     /* Real uid */
	AT_EUID = 12,                    /* Effective uid */
	AT_GID = 13,                     /* Real gid */
	AT_EGID  = 14,                   /* Effective gid */
	AT_CLKTCK = 17,                  /* Frequency of times() */
	AT_SECURE = 23,                  /* Secure, non-zero for suid or guid executable */
	AT_RANDOM = 25                   /* pointer to 16 random bytes */
};

// Elf32_auxv
typedef struct {
	Elf32_Word a_type;              /* Entry type */
	Elf32_Word a_val;               /* Entry value */
} Elf32_auxv;

// Elf32_auxv
typedef struct {
	Elf64_Word a_type;              /* Entry type */
	Elf64_Word a_val;               /* Entry value */
} Elf64_auxv;


// ELF Extension Header

// e_type
enum {
	ET_NONE = 0,                     /* No file type */
	ET_REL = 1,                      /* Relocatable file */
	ET_EXEC = 2,                     /* Executable file */
	ET_DYN = 3,                      /* Shared object file */
	ET_CORE = 4,                     /* Core file */
	ET_LOPROC = 0xff00,              /* Processor-specific */
	ET_HIPROC = 0xffff
};

// e_machine
enum {
	EM_NONE = 0,                     /* Unknown machine */
	EM_X86_64 = 62,                  /* x86-64 */
	EM_RISCV = 243                   /* RISC-V */
};

// e_version
enum {
	EV_NONE = 0,                     /* Invalid version */
	EV_CURRENT = 1                   /* Current version */
};

// e_ident
enum {
	EI_MAG0 = 0,                     /* File identification */
	EI_MAG1 = 1,
	EI_MAG2 = 2,
	EI_MAG3 = 3,
	EI_CLASS = 4,                    /* File class */ 
	EI_DATA = 5,                     /* Data encoding */ 
	EI_VERSION = 6,                  /* File version */ 
	EI_OSABI = 7,                    /* OS/ABI identification */
	EI_ABIVERSION = 8,               /* ABI version */
	EI_PAD = 9,                      /* Start of padding bytes */
	EI_NIDENT = 16                   /* Size of e_ident[] */
};

// e_magic
enum {
	ELFMAG0 = 0x7f,                  /* e_ident[EI_MAG0] == ELFMAG0 */
	ELFMAG1 = 'E',                   /* e_ident[EI_MAG1] == ELFMAG1 */
	ELFMAG2 = 'L',                   /* e_ident[EI_MAG2] == ELFMAG2 */
	ELFMAG3 = 'F'                    /* e_ident[EI_MAG3] == ELFMAG3 */
};

// EI_CLASS
enum {
	ELFCLASSNONE = 0,
	ELFCLASS32 = 1,                  /* e_ident[EI_CLASS] - 32-bit objects */
	ELFCLASS64 = 2,                  /* e_ident[EI_CLASS] - 64-bit objects */
	ELFCLASS128 = 3                  /* e_ident[EI_CLASS] - 128-bit objects */
};

// EI_DATA
enum {
	ELFDATANONE = 0,
	ELFDATA2LSB = 1,                 /* e_ident[EI_DATA] - Data is little-endian */
	ELFDATA2MSB = 2                  /* e_ident[EI_DATA] - Data is big-endian */
};

// EI_OSABI
enum {
	ELFOSABI_SYSV = 0,               /* e_ident[EI_OSABI] == ELFOSABI_SYSV */
	ELFOSABI_LINUX = 3,              /* e_ident[EI_OSABI] == ELFOSABI_LINUX */
	ELFOSABI_SOLARIS = 6,            /* e_ident[EI_OSABI] == ELFOSABI_SOLARIS */
	ELFOSABI_FREEBSD = 9             /* e_ident[EI_OSABI] == ELFOSABI_FREEBSD */
};

// EI_ABIVERSION
enum {
	ELFABIVERSION_NONE = 0
};

// Elf32_Ehdr
typedef struct {
	Elf32_Byte  e_ident[EI_NIDENT];  /* ELF identification */
	Elf32_Half  e_type;              /* Object file type */
	Elf32_Half  e_machine;           /* Machine type */
	Elf32_Word  e_version;           /* Object file version */
	Elf32_Addr  e_entry;             /* Entry point address */
	Elf32_Off   e_phoff;             /* Program header offset */
	Elf32_Off   e_shoff;             /* Section header offset */
	Elf32_Word  e_flags;             /* Processor-specific flags */
	Elf32_Half  e_ehsize;            /* ELF header size */
	Elf32_Half  e_phentsize;         /* Size of program header entry */
	Elf32_Half  e_phnum;             /* Number of program header entries */
	Elf32_Half  e_shentsize;         /* Size of section header entry */
	Elf32_Half  e_shnum;             /* Number of section header entries */
	Elf32_Half  e_shstrndx;          /* Section name string table index */
} Elf32_Ehdr;

// Elf64_Ehdr
typedef struct {
	Elf64_Byte  e_ident[EI_NIDENT];  /* ELF identification */
	Elf64_Half  e_type;              /* Object file type */
	Elf64_Half  e_machine;           /* Machine type */
	Elf64_Word  e_version;           /* Object file version */
	Elf64_Addr  e_entry;             /* Entry point address */
	Elf64_Off   e_phoff;             /* Program header offset */
	Elf64_Off   e_shoff;             /* Section header offset */
	Elf64_Word  e_flags;             /* Processor-specific flags */
	Elf64_Half  e_ehsize;            /* ELF header size */
	Elf64_Half  e_phentsize;         /* Size of program header entry */
	Elf64_Half  e_phnum;             /* Number of program header entries */
	Elf64_Half  e_shentsize;         /* Size of section header entry */
	Elf64_Half  e_shnum;             /* Number of section header entries */
	Elf64_Half  e_shstrndx;          /* Section name string table index */
} Elf64_Ehdr;


// ELF Program Header

// p_type
enum {
	PT_NULL = 0,                     /* Unused entry */
	PT_LOAD = 1,                     /* Loadable segment */
	PT_DYNAMIC = 2,                  /* Dynamic linking tables */
	PT_INTERP = 3,                   /* Program interpreter path name */
	PT_NOTE = 4,                     /* Note sections */ 
	PT_SHLIB = 5,                    /* Reserved */
	PT_PHDR = 6,                     /* Program header table */
	PT_TLS = 7,                      /* Thread local storage header table */
	PT_LOOS = 0x60000000,            /* Environment-specific use */
	PT_GNU_EH_FRAME = 0x6474e550,    /* GCC .eh_frame_hdr segment */
	PT_GNU_STACK = 0x6474e551,       /* GCC stack executability */
	PT_GNU_RELRO = 0x6474e552,       /* GCC Read-only after relocation */
	PT_HIOS = 0x6fffffff,
	PT_LOPROC = 0x70000000,          /* Processor-specific use */
	PT_HIPROC = 0x7fffffff
};

// p_flags
enum {
	PF_X = 0x1,                      /* Execute permission */
	PF_W = 0x2,                      /* Write permission */
	PF_R = 0x4,                      /* Read permission */
	PF_MASKOS = 0x00FF0000,          /* Environment-specific use */
	PF_MASKPROC = 0xFF000000,        /* Processor-specific use */
};

// Elf32_Phdr
typedef struct {
	Elf32_Word  p_type;              /* Type of segment */
	Elf32_Off   p_offset;            /* Offset in file */
	Elf32_Addr  p_vaddr;             /* Virtual address in memory */
	Elf32_Addr  p_paddr;             /* Physical address in memory (if applicable) */
	Elf32_Word  p_filesz;            /* Size of segment in file */
	Elf32_Word  p_memsz;             /* Size of segment in memory */ 
	Elf32_Word  p_flags;             /* Segment attributes */
	Elf32_Word  p_align;             /* Alignment of segment */
} Elf32_Phdr;

// Elf64_Phdr
typedef struct {
	Elf64_Word  p_type;              /* Type of segment */
	Elf64_Word  p_flags;             /* Segment attributes */
	Elf64_Off   p_offset;            /* Offset in file */
	Elf64_Addr  p_vaddr;             /* Virtual address in memory */
	Elf64_Addr  p_paddr;             /* Physical address in memory (if applicable) */
	Elf64_Xword p_filesz;            /* Size of segment in file */
	Elf64_Xword p_memsz;             /* Size of segment in memory */
	Elf64_Xword p_align;             /* Alignment of segment */
} Elf64_Phdr;


// ELF Section Header

// sh_name
enum {
	SHN_UNDEF = 0,                   /* Used to mark an undefined or meaningless section reference */
	SHN_LOPROC = 0xff00,             /* Processor-specific use */
	SHN_HIPROC = 0xff1f,
	SHN_LOOS = 0xff20,               /* Environment-specific use */
	SHN_HIOS = 0xff3f,
	SHN_ABS = 0xfff1,                /* Indicates that the corresponding reference is an absolute value */
	SHN_COMMON = 0xfff2,             /* Indicates a symbol that has been declared as a common block */
	SHN_XINDEX = 0xffff,             /* Indicates section header has additional date in SHT_SYMTAB_SHNDX section */
	SHN_HIRESERVE = 0xffff,          /* Upper bound of the range of reserved indexes */
};

// sh_type
enum {
	SHT_NULL = 0,                    /* Marks an unused section header */
	SHT_PROGBITS = 1,                /* Contains information defined by the program */
	SHT_SYMTAB = 2,                  /* Contains a linker symbol table */
	SHT_STRTAB = 3,                  /* Contains a string table */
	SHT_RELA = 4,                    /* Contains "Rela" type relocation entries */
	SHT_HASH = 5,                    /* Contains a symbol hash table */
	SHT_DYNAMIC = 6,                 /* Contains dynamic linking tables */
	SHT_NOTE = 7,                    /* Contains note information */
	SHT_NOBITS = 8,                  /* Contains uninitialized space; does not take up space in file */
	SHT_REL = 9,                     /* Contains "Rel" type relocation entries */
	SHT_SHLIB = 10,                  /* Reserved */
	SHT_DYNSYM = 11,                 /* Contains a dynamic loader symbol table */
	SHT_INIT_ARRAY = 14,             /* Contains initialization functions */
	SHT_FINI_ARRAY = 15,             /* Contains finalization functions */
	SHT_PREINIT_ARRAY = 16,          /* */
	SHT_GROUP = 17,                  /* Contains an array of Elf32_Word entries, the first entry is a flag entry */
	SHT_SYMTAB_SHNDX = 18,           /* Contains extra symbol table info, sh_link to SHT_SYMTAB with SHN_XINDEX set */
	SHT_LOOS = 0x60000000,           /* Environment-specific use */
	SHT_GNU_VERDEF = 0x6ffffffd,
	SHT_GNU_VERNEED = 0x6ffffffe,
	SHT_GNU_VERSYM = 0x6fffffff,
	SHT_HIOS = 0x6fffffff,
	SHT_LOPROC = 0x70000000,         /* Processor-specific use */
	SHT_HIPROC = 0x7fffffff,
	SHT_LOUSER = 0x80000000,         /* User-specific use */
	SHT_HIUSER = 0xffffffff
};

// sh_flags
enum {
	SHF_WRITE = 0x1,                 /* Section contains writable data */
	SHF_ALLOC = 0x2,                 /* Section is allocated in memory image of program */
	SHF_EXECINSTR = 0x4,             /* Section contains executable instructions */
	SHF_MERGE = 0x10,                /* Section contains data that may be merged to eliminate duplication */
	SHF_STRINGS = 0x20,              /* Section consist of null-terminated character strings */
	SHF_INFO_LINK = 0x40,            /* Section sh_info holds a section header table index */
	SHF_LINK_ORDER = 0x80,           /* Section has link ordering requirement with sh_link pointing to next section */
	SHF_GROUP = 0x200,               /* Section is a member of a group (applicable to ET_REL relocatable files)*/
	SHF_TLS = 0x400,                 /* Section holds thread local storage*/
	SHF_MASKOS = 0x0F000000,         /* Environment-specific use */
	SHF_MASKPROC = 0xf0000000        /* Processor-specific use */
};

// flags entry in the first Elf32_Word of a SHT_GROUP section
enum {
  GRP_COMDAT = 0x1,
  GRP_MASKOS = 0x0ff00000,
  GRP_MASKPROC = 0xf0000000
};

// Elf32_Shdr
typedef struct {
	Elf32_Word  sh_name;             /* Section name */
	Elf32_Word  sh_type;             /* Section type */
	Elf32_Word  sh_flags;            /* Section attributes */
	Elf32_Addr  sh_addr;             /* Virtual memory address */
	Elf32_Off   sh_offset;           /* Offset in file */
	Elf32_Word  sh_size;             /* Size of section */
	Elf32_Word  sh_link;             /* Link to other section */
	Elf32_Word  sh_info;             /* Miscellaneous information */
	Elf32_Word  sh_addralign;        /* Address alignment boundary */
	Elf32_Word  sh_entsize;          /* Size of entries, if section has table */
} Elf32_Shdr;

// Elf64_Shdr
typedef struct {
	Elf64_Word  sh_name;             /* Section name */
	Elf64_Word  sh_type;             /* Section type */
	Elf64_Xword sh_flags;            /* Section attributes */
	Elf64_Addr  sh_addr;             /* Virtual memory address */
	Elf64_Off   sh_offset;           /* Offset in file */
	Elf64_Xword sh_size;             /* Size of section */
	Elf64_Word  sh_link;             /* Link to other section */
	Elf64_Word  sh_info;             /* Miscellaneous information */
	Elf64_Xword sh_addralign;        /* Address alignment boundary */
	Elf64_Xword sh_entsize;          /* Size of entries, if section has table */
} Elf64_Shdr;


// ELF Note Header (PT_NOTE)

// Elf32_Nhdr
typedef struct {
	Elf32_Word n_namesz;             /* Name size */
	Elf32_Word n_descsz;             /* Content size */
	Elf32_Word n_type;               /* Content type */
} Elf32_Nhdr;

// Elf64_Nhdr
typedef struct {
	Elf64_Word n_namesz;             /* Name size */
	Elf64_Word n_descsz;             /* Content size */
	Elf64_Word n_type;               /* Content type */
} Elf64_Nhdr;


// ELF Symbol Table

inline const Elf32_Byte ELF32_ST_BIND(Elf32_Word i) { return i >> 4; }
inline const Elf32_Byte ELF32_ST_TYPE(Elf32_Word i) { return i & 0xf; }
inline const Elf32_Byte ELF32_ST_INFO(Elf32_Word b, Elf32_Word t) { return (b << 4) | (t & 0xf); }

inline const Elf32_Byte ELF64_ST_BIND(Elf32_Word i) { return i >> 4; }
inline const Elf32_Byte ELF64_ST_TYPE(Elf32_Word i) { return i & 0xf; }
inline const Elf32_Byte ELF64_ST_INFO(Elf32_Word b, Elf32_Word t) { return (b << 4) | (t & 0xf); }

// st_info bind
enum {
	STB_LOCAL = 0,                   /* Not visible outside the object file */
	STB_GLOBAL = 1,                  /* Global symbol, visible to all object files */
	STB_WEAK = 2,                    /* Global scope, but with lower precedence than global symbols */
	STB_LOOS = 10,                   /* Environment-specific use */
	STB_HIOS = 12,
	STB_LOPROC = 13,                 /* Processor-specific use */
	STB_HIPROC = 15
};

// st_info type
enum {
	STT_NOTYPE = 0,                  /* No type specified (e.g., an absolute symbol) */
	STT_OBJECT = 1,                  /* Data object */
	STT_FUNC = 2,                    /* Function entry point */
	STT_SECTION = 3,                 /* Symbol is associated with a section */
	STT_FILE = 4,                    /* Source file associated with the object file */
	STT_LOOS = 10,                   /* Environment-specific use */
	STT_HIOS = 12,
	STT_LOPROC = 13,                 /* Processor-specific use */
	STT_HIPROC = 15
};

// st_other visibility
enum {
	STV_DEFAULT = 0,
	STV_INTERNAL = 1,
	STV_HIDDEN = 2,
	STV_PROTECTED = 3
};

// Elf32_Sym
typedef struct {
	Elf32_Word  st_name;             /* Symbol name */
	Elf32_Addr  st_value;            /* Symbol value */
	Elf32_Word  st_size;             /* Size of object */
	Elf32_Byte  st_info;             /* Type and Binding attributes */
	Elf32_Byte  st_other;            /* Visibility */
	Elf32_Half  st_shndx;            /* Section table index */
} Elf32_Sym;

// Elf64_Sym
typedef struct {
	Elf64_Word  st_name;             /* Symbol name */
	Elf64_Byte  st_info;             /* Type and Binding attributes */
	Elf64_Byte  st_other;            /* Visibility */
	Elf64_Half  st_shndx;            /* Section table index */
	Elf64_Addr  st_value;            /* Symbol value */
	Elf64_Xword st_size;             /* Size of object */
} Elf64_Sym;


// Relocations

inline const Elf32_Word ELF32_R_SYM(Elf32_Word i) { return i >> 8; }
inline const Elf32_Word ELF32_R_TYPE(Elf32_Word i) { return i & 0xff; }
inline const Elf32_Word ELF32_R_INFO(Elf32_Word s, Elf32_Word t) { return (s << 8) | (t & 0xff); }

inline const Elf64_Xword ELF64_R_SYM(Elf64_Xword i) { return i >> 32; }
inline const Elf64_Xword ELF64_R_TYPE(Elf64_Xword i) { return i & 0xffffffffUL; }
inline const Elf64_Xword ELF64_R_INFO(Elf64_Xword s, Elf64_Xword t) { return (s << 32) | (t & 0xffffffffUL); }

typedef struct {
	Elf32_Addr  r_offset;            /* Address of reference */
	Elf32_Word  r_info;              /* Symbol index and type of relocation */
} Elf32_Rel;

typedef struct {
	Elf32_Addr  r_offset;            /* Address of reference */
	Elf32_Word  r_info;              /* Symbol index and type of relocation */
	Elf32_Sword r_addend;            /* Constant part of expression */
} Elf32_Rela;

typedef struct
{
	Elf64_Addr  r_offset;            /* Address of reference */
	Elf64_Xword r_info;              /* Symbol index and type of relocation */
} Elf64_Rel;

typedef struct
{
	Elf64_Addr r_offset;             /* Address of reference */
	Elf64_Xword r_info;              /* Symbol index and type of relocation */
	Elf64_Sxword r_addend;           /* Constant part of expression */
} Elf64_Rela;


// x86-64 Relocation Types

/*

System V Application Binary Interface AMD64 Architecture Processor Supplement Draft 0.99.6

- A Represents the addend used to compute the value of the relocatable field.
- B Represents the base address at which a shared object has been loaded into memory
	during execution. Generally, a shared object is built with a 0 base virtual
	address, but the execution address will be different.
- G Represents the offset into the global offset table at which the relocation entry’s
	symbol will reside during execution.
- GOT Represents the address of the global offset table.
- L Represents the place (section offset or address) of the Procedure Linkage Table
	entry for a symbol.
- P Represents the place (section offset or address) of the storage unit being relocated
	(computed using r_offset).
- S Represents the value of the symbol whose index resides in the relocation entry.
- Z Represents the size of the symbol whose index resides in the relocation entry.

*/

enum {
	R_X86_64_NONE = 0,                /* none */
	R_X86_64_64 = 1,                  /* word64 = S + A */
	R_X86_64_PC32 = 2,                /* word32 = S + A - P */
	R_X86_64_GOT32 = 3,               /* word32 = G + A */
	R_X86_64_PLT32 = 4,               /* word32 = L + A - P */
	R_X86_64_COPY = 5,                /* none, symbol must be in executable, not allowed in shared library */
	R_X86_64_GLOB_DAT = 6,            /* wordclass = S */
	R_X86_64_JUMP_SLOT = 7,           /* wordclass = S (handled by PLT unless LD_BIND_NOW) */
	R_X86_64_RELATIVE = 8,            /* wordclass = B + A */
	R_X86_64_GOTPCREL = 9,            /* word32 = G + GOT + A - P */
	R_X86_64_32 = 10,                 /* word32 = S + A */
	R_X86_64_32S = 11,                /* word32 = S + A */
	R_X86_64_16 = 12,                 /* word16 = S + A */
	R_X86_64_PC16 = 13,               /* word16 = S + A - P */
	R_X86_64_8 = 14,                  /* word8  = S + A */
	R_X86_64_PC8 = 15,                /* word8  = S + A - P */
	R_X86_64_DTPMOD64 = 16,           /* word64 */
	R_X86_64_DTPOFF64 = 17,           /* word64 */
	R_X86_64_TPOFF64  = 18,           /* word64 */
	R_X86_64_TLSGD = 19,              /* word32 */
	R_X86_64_TLSLD = 20,              /* word32 */
	R_X86_64_DTPOFF32 = 21,           /* word32 */
	R_X86_64_GOTTPOFF = 22,           /* word32 */
	R_X86_64_TPOFF32 = 23,            /* word32 */
	R_X86_64_PC64 = 24,               /* word64 = S + A - P */
	R_X86_64_GOTOFF64 = 25,           /* word64 = S + A - GOT */
	R_X86_64_GOTPC32 = 26,            /* word32 = GOT + A + P */
	R_X86_64_SIZE32 = 32,             /* word32 = Z + A */
	R_X86_64_SIZE64 = 33,             /* word64 = Z + A */
	R_X86_64_GOTPC32_TLSDESC = 34,    /* word32 */
	R_X86_64_TLSDESC_CALL = 35,       /* none */
	R_X86_64_TLSDESC = 36,            /* word64x2 */
	R_X86_64_IRELATIVE = 37,          /* wordclass = indirect (B + A) */
	R_X86_64_RELATIVE64 = 38          /* word64 = B + A */
};


// RISC-V Relocation Types

enum {
	R_RISCV_NONE = 0,                /* xx,       None                         */
	R_RISCV_32 = 1,                  /* 32,       Runtime relocation                                 word32 = S + A */
	R_RISCV_64 = 2,                  /* 64,       Runtime relocation                                 word64 = S + A */
	R_RISCV_RELATIVE = 3,            /* 32,       Runtime relocation                                 wordclass = B + A */
	R_RISCV_COPY = 4,                /* 0,        Runtime relocation                                 none ;must be in executable; not allowed in shared library */
	R_RISCV_JUMP_SLOT = 5,           /* 64,       Runtime relocation                                 wordclass = S ;handled by PLT unless LD_BIND_NOW */
	R_RISCV_TLS_DTPMOD32 = 6,        /* 32,       TLS relocation                                     word32 = S->TLSINDEX */
	R_RISCV_TLS_DTPMOD64 = 7,        /* 64,       TLS relocation                                     word64 = S->TLSINDEX */
	R_RISCV_TLS_DTPREL32 = 8,        /* 32,       TLS relocation                                     word32 = TLS + S + A - TLS_TP_OFFSET */
	R_RISCV_TLS_DTPREL64 = 9,        /* 64,       TLS relocation                                     word64 = TLS + S + A - TLS_TP_OFFSET */
	R_RISCV_TLS_TPREL32 = 10,        /* 32,       TLS relocation                                     word32 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET */
	R_RISCV_TLS_TPREL64 = 11,        /* 64,       TLS relocation                                     word64 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET */
	R_RISCV_BRANCH = 16,             /* 32, 12:1  PC-relative branch offset                          SB-Type (beq,bne,blt,bge,bltu,bgeu) */
	R_RISCV_JAL = 17,                /* 32, 20:1  PC-relative jump offset                            UJ-Type (jal) */
	R_RISCV_CALL = 18,               /* 64, 31:0  PC-relative function call                          MACRO call (auipc+jalr) */
	R_RISCV_CALL_PLT = 19,           /* 64, 31:0  PC-relative function call                          MACRO tail (auipc+jalr) */
	R_RISCV_GOT_HI20 = 20,           /* 32, 31:12 PC-relative GOT offset                             MACRO la */
	R_RISCV_TLS_GOT_HI20 = 21,       /* 32, 31:12 PC-relative TLS IE GOT ref    %tls_ie_pcrel_hi(x)  MACRO la.tls.ie */
	R_RISCV_TLS_GD_HI20 = 22,        /* 32, 31:12 PC-relative TLS GD GOT ref    %tls_gd_pcrel_hi(x)  MACRO la.tls.gd */
	R_RISCV_PCREL_HI20 = 23,         /* 32, 31:12 PC-relative reference         %pcrel_hi(symbol)    U-Type (auipc) */
	R_RISCV_PCREL_LO12_I = 24,       /* 32, 11:0  PC-relative reference         %pcrel_lo(label)     I-Type (lb,lbu,lh,lhu,lw,lwu,flw,fld,addi,addiw) */
	R_RISCV_PCREL_LO12_S = 25,       /* 32, 11:0  PC-relative reference         %pcrel_lo(label)     S-Type (sb,sh,sw,fsw,fsd) */
	R_RISCV_HI20 = 26,               /* 32, 31:12 Absolute address              %hi(symbol)          U-Type (lui,auipc) */
	R_RISCV_LO12_I = 27,             /* 32, 11:0  Absolute address              %lo(symbol)          I-Type (lb,lbu,lh,lhu,lw,lwu,flw,fld,addi,addiw) */
	R_RISCV_LO12_S = 28,             /* 32, 11:0  Absolute address              %lo(symbol)          S-Type (sb,sh,sw,fsw,fsd) */
	R_RISCV_TPREL_HI20 = 29,         /* 32, 31:12 TLS LE thread pointer offset  %tprel_hi(symbol)    U-Type (auipc) */
	R_RISCV_TPREL_LO12_I = 30,       /* 32, 11:0  TLS LE thread pointer offset  %tprel_lo(label)     I-Type (lb,lbu,lh,lhu,lw,lwu,flw,fld,addi,addiw) */
	R_RISCV_TPREL_LO12_S = 31,       /* 32, 11:0  TLS LE thread pointer offset  %tprel_lo(label)     S-Type (sb,sh,sw,fsw,fsd) */
	R_RISCV_TPREL_ADD = 32,          /* 32, 31:0  TLS LE thread pointer usage   %tprel_add(label)    ;internal assembler expansion */
	R_RISCV_ADD8 = 33,               /* 8,  7:0   8-bit label addition                               word8  = S + A */
	R_RISCV_ADD16 = 34,              /* 16, 15:0  16-bit label addition                              word16 = S + A */
	R_RISCV_ADD32 = 35,              /* 32, 31:0  32-bit label addition                              word32 = S + A */
	R_RISCV_ADD64 = 36,              /* 64, 63:0  64-bit label addition                              word64 = S + A */
	R_RISCV_SUB8 = 37,               /* 8,  7:0   8-bit label subtraction                            word8  = S - A */
	R_RISCV_SUB16 = 38,              /* 16, 15:0  16-bit label subtraction                           word16 = S - A */
	R_RISCV_SUB32 = 39,              /* 32, 31:0  32-bit label subtraction                           word32 = S - A */
	R_RISCV_SUB64 = 40,              /* 64, 63:0  64-bit label subtraction                           word64 = S - A */
	R_RISCV_GNU_VTINHERIT = 41,      /* 32,       GNU C++ vtable hierarchy     */
	R_RISCV_GNU_VTENTRY = 42,        /* 32,       GNU C++ vtable member usage  */
	R_RISCV_ALIGN = 43,              /* xx,       Alignment statement          */
	R_RISCV_RVC_BRANCH = 44,         /* xx, 8:1   PC-relative branch offset                          CB-Type (c.beqz,c.bnez) */
	R_RISCV_RVC_JUMP = 45,           /* xx, 11:1  PC-relative jump offset                            CJ-Type (c.j) */
	R_RISCV_RVC_LUI = 46,            /* xx, 17:12 Absolute address                                   CI-Type (c.lui) */
	R_RISCV_GPREL_I = 47,            /* 32, 11:0  PC-relative reference         %gprel(symbol)       I-Type (lb,lbu,lh,lhu,lw,lwu,flw,fld,addi,addiw)   */
	R_RISCV_GPREL_S = 48,            /* 32, 11:0  PC-relative reference         %gprel(symbol)       S-Type (sb,sh,sw,fsw,fsd) */
	R_RISCV_TPREL_I = 49,            /* 32,       TP-relative TLS LE load */
	R_RISCV_TPREL_S = 50,            /* 32,       TP-relative TLS LE store */
	R_RISCV_RELAX = 51,              /*           Reloc pair can be relaxed */
	R_RISCV_SUB6 = 52,               /*           Local label subtraction */
	R_RISCV_SET6 = 53,               /*           Local label subtraction */
	R_RISCV_SET8 = 54,               /*           Local label subtraction */
	R_RISCV_SET16 = 55,              /*           Local label subtraction */
	R_RISCV_SET32 = 56,              /*           Local label subtraction */
};

// RISC-V Ehdr e_flags

enum {
	EF_RISCV_RVC = 0x1,
	EF_RISCV_FLOAT_ABI_SINGLE = 0x2,
	EF_RISCV_FLOAT_ABI_DOUBLE = 0x4,
	EF_RISCV_FLOAT_ABI_QUAD = 0x6,
	EF_RISCV_RVE = 0x8
};

// Dynamic Flags

// d_flags
enum {
	DF_ORIGIN = 1,                  /* $ORIGIN reference substitution to directory of the object */
	DF_SYMBOLIC = 2,                /* dynamic linker starts search from the object instead of executable */
	DF_TEXTREL = 4,                 /* allow relocations to make modifications to non-writable segments */
	DF_BIND_NOW = 8,                /* process all relocations before passing control to the executable */
	DF_STATIC_TLS = 16              /* executable or shared object has static thread local storage */
};


// Dynamic Section

// d_tag
enum {
	DT_NULL = 0,                     /* d_val - Marks the end of the dynamic array */
	DT_NEEDED = 1,                   /* d_ptr - The string table offset of the name of a needed library */
	DT_PLTRELSZ = 2,                 /* d_ptr - Total size in bytes of the relocation entries associated with PLT */
	DT_PLTGOT = 3,                   /* d_ptr - Contains an address associated with the linkage table */
	DT_HASH = 4,                     /* d_ptr - Address of the symbol hash table, described below. */
	DT_STRTAB = 5,                   /* d_ptr - Address of the dynamic string table */
	DT_SYMTAB = 6,                   /* d_ptr - Address of the dynamic symbol table */
	DT_RELA = 7,                     /* d_ptr - Address of a relocation table with Elf64_Rela entries */
	DT_RELASZ = 8,                   /* d_val - Total size, in bytes, of the DT_RELA relocation table */
	DT_RELAENT = 9,                  /* d_val - Size, in bytes, of each DT_RELA relocation entry */
	DT_STRSZ = 10,                   /* d_val - Total size, in bytes, of the string table */
	DT_SYMENT = 11,                  /* d_val - Size, in bytes, of each symbol table entry */
	DT_INIT = 12,                    /* d_ptr - Address of the initialization function */
	DT_FINI = 13,                    /* d_ptr - Address of the termination function */
	DT_SONAME = 14,                  /* d_val - The string table offset of the name of this shared object */
	DT_RPATH = 15,                   /* d_val - The string table offset of a shared library search path string */
	DT_SYMBOLIC = 16,                /*       - Symbols defined within the library are used over search path */
	DT_REL = 17,                     /* d_ptr - Address of a relocation table with Elf64_Rel entries */
	DT_RELSZ = 18,                   /* d_val - Total size, in bytes, of the DT_REL relocation table */
	DT_RELENT = 19,                  /* d_val - Size, in bytes, of each DT_REL relocation entry */
	DT_PLTREL = 20,                  /* d_val - Type of relocation entry used for PLT (DT_REL or DT_RELA) */
	DT_DEBUG = 21,                   /* d_ptr - Reserved for debugger use */
	DT_TEXTREL = 22,                 /*       - Signals relocations for a non-writable segment */
	DT_JMPREL = 23,                  /* d_ptr - Address of the relocations associated with the PLT */
	DT_BIND_NOW = 24,                /*       - Signals linker should process all relocations program start */
	DT_INIT_ARRAY = 25,              /* d_ptr - Pointer to an array of pointers to initialization functions */
	DT_FINI_ARRAY = 26,              /* d_ptr - Pointer to an array of pointers to termination functions */
	DT_INIT_ARRAYSZ = 27,            /* d_val - Size, in bytes, of the array of initialization functions */
	DT_FINI_ARRAYSZ = 28,            /* d_val - Size, in bytes, of the array of termination functions */
	DT_RUNPATH = 29,                 /* d_val - DT_STRTAB dynamic string table offsetc containing library search paths */
	DT_FLAGS = 30,                   /* d_val - (DF_ORIGIN, DF_SYMBOLIC, DF_TEXTREL, DF_BIND_NOW, DF_STATIC_TLS) */
	DT_LOOS = 0x60000000,            /*       - Defines a range of dynamic table tags that are reserved */
	DT_HIOS = 0x6fffffff,
	DT_LOPROC = 0x70000000,          /*       - Defines a range of dynamic table tags that are reserved */
	DT_HIPROC = 0x7fffffff
};

// Elf32_Dyn
typedef struct {
	Elf32_Sword  d_tag;              /* Dynamic table entry type */
	union {
		Elf32_Word  d_val;           /* Integer value */
		Elf32_Addr  d_ptr;           /* Program virtual address */
	} d_un;
} Elf32_Dyn;

// Elf64_Dyn
typedef struct {
	Elf64_Sxword d_tag;              /* Dynamic table entry type */
	union {
		Elf64_Xword d_val;           /* Integer value */
		Elf64_Addr  d_ptr;           /* Program virtual address */
	} d_un;
} Elf64_Dyn;

enum ELFENDIAN {
	ELFENDIAN_HOST,
	ELFENDIAN_TARGET,
};

bool elf_check_magic(uint8_t *e_ident);
void elf_bswap_ehdr32(Elf32_Ehdr *ehdr32, int ei_data, ELFENDIAN endian);
void elf_bswap_phdr32(Elf32_Phdr *phdr32, int ei_data, ELFENDIAN endian);
void elf_bswap_shdr32(Elf32_Shdr *shdr32, int ei_data, ELFENDIAN endian);
void elf_bswap_sym32(Elf32_Sym *rela32, int ei_data, ELFENDIAN endian);
void elf_bswap_rela32(Elf32_Rela *shdr32, int ei_data, ELFENDIAN endian);
void elf_bswap_ehdr64(Elf64_Ehdr *ehdr64, int ei_data, ELFENDIAN endian);
void elf_bswap_phdr64(Elf64_Phdr *phdr64, int ei_data, ELFENDIAN endian);
void elf_bswap_shdr64(Elf64_Shdr *shdr64, int ei_data, ELFENDIAN endian);
void elf_bswap_sym64(Elf64_Sym *sym64, int ei_data, ELFENDIAN endian);
void elf_bswap_rela64(Elf64_Rela *rela64, int ei_data, ELFENDIAN endian);
void elf_ehdr32_to_ehdr64(Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32);
void elf_phdr32_to_phdr64(Elf64_Phdr *phdr64, Elf32_Phdr *phdr32);
void elf_shdr32_to_shdr64(Elf64_Shdr *shdr64, Elf32_Shdr *shdr32);
void elf_sym32_to_sym64(Elf64_Sym *sym64, Elf32_Sym *sym32);
void elf_rela32_to_rela64(Elf64_Rela *rela64, Elf32_Rela *rela32);
void elf_ehdr64_to_ehdr32(Elf32_Ehdr *ehdr32, Elf64_Ehdr *ehdr64);
void elf_phdr64_to_phdr32(Elf32_Phdr *phdr32, Elf64_Phdr *phdr64);
void elf_shdr64_to_shdr32(Elf32_Shdr *shdr32, Elf64_Shdr *shdr64);
void elf_sym64_to_sym32(Elf32_Sym *sym32, Elf64_Sym *sym64);
void elf_rela64_to_rela32(Elf32_Rela *rela64, Elf64_Rela *rela32);

#endif
