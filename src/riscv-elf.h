//
//  riscv-elf.h
//

#ifndef riscv_elf_h
#define riscv_elf_h

/*

 References:

 - TIS Portable Formats Specification, Version 1.1 ELF: Executable and Linkable Format
 - ELF-64 Object File Format, Version 1.5 Draft 2
 - System V Application Binary Interface AMD64 Architecture Processor Supplement Draft 0.99.6

*/

// ELF Types

// Elf32 types
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;
typedef uint8_t	 Elf32_Byte;

// Elf64 types
typedef uint64_t Elf64_Addr;
typedef uint16_t Elf64_Half;
typedef uint64_t Elf64_Off;
typedef int32_t  Elf64_Sword;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;
typedef uint8_t  Elf64_Byte;


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
	ELFCLASS64 = 2                   /* e_ident[EI_CLASS] - 64-bit objects */
};

// EI_DATA
enum {
	ELFDATANONE = 0,
	ELFDATA2LSB = 1,                 /* e_ident[EI_DATA] - Data is little-endian */
	ELFDATA2MSB = 2                  /* e_ident[EI_DATA] - Data is big-endian */
};

// EI_OSABI
enum {
	ELFOSABI_SYSV = 0                /* e_ident[EI_OSABI] == ELFOSABI_SYSV */
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
	PT_LOOS = 0x60000000,            /* Environment-specific use */
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
	SHN_HIRESERVE = 0xffff           /* Upper bound of the range of reserved indexes */
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
	SHT_NOBITS = 8,                  /* Contains uninitialized space; does not space in file */
	SHT_REL = 9,                     /* Contains "Rel" type relocation entries */
	SHT_SHLIB = 10,                  /* Reserved */
	SHT_DYNSYM = 11,                 /* Contains a dynamic loader symbol table */
	SHT_INIT_ARRAY = 14,             /* Contains initialization functions */
	SHT_FINI_ARRAY = 15,             /* Contains finalization functions */
	SHT_LOOS = 0x60000000,           /* Environment-specific use */
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
	SHF_MASKOS = 0x0F000000,         /* Environment-specific use */
	SHF_MASKPROC = 0xf0000000        /* Processor-specific use */
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
inline const Elf32_Word ELF32_R_TYPE(Elf32_Word i) { return i & 0xf; }
inline const Elf32_Word ELF32_R_INFO(Elf32_Word s, Elf32_Word t) { return (s << 4) | (t & 0xf); }

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
- A Represents the addend used to compute the value of the relocatable field.
- B Represents the base address at which a shared object has been loaded into memory
	during execution. Generally, a shared object is built with a 0 base virtual
	address, but the execution address will be different.
- G Represents the offset into the global offset table at which the relocation entry’s
	symbol will reside during execution.
	GOT Represents the address of the global offset table.
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
	R_X86_64_COPY = 5,                /* symbol must be in executable, not allowed in shared library */
	R_X86_64_GLOB_DAT = 6,            /* word64 = S */
	R_X86_64_JUMP_SLOT = 7,           /* word64 = S (handled by PLT unless LD_BIND_NOW) */
	R_X86_64_RELATIVE = 8,            /* word64 = B + A */
	R_X86_64_GOTPCREL = 9,            /* word32 = G + GOT + A - P */
	R_X86_64_32 = 10,                 /* word32 = S + A */
	R_X86_64_32S = 11,                /* word32 = S + A */
	R_X86_64_16 = 12,                 /* word16 = S + A */
	R_X86_64_PC16 = 13,               /* word16 = S + A - P */
	R_X86_64_8 = 14,                  /* word8  = S + A */
	R_X86_64_PC8 = 15,                /* word8  = S + A - P */
	R_X86_64_DTPMOD64 = 16,
	R_X86_64_DTPOFF64 = 17,
	R_X86_64_TPOFF64  = 18,
	R_X86_64_TLSGD = 19,
	R_X86_64_TLSLD = 20,
	R_X86_64_DTPOFF32 = 21,
	R_X86_64_GOTTPOFF = 22,
	R_X86_64_TPOFF32 = 23,
	R_X86_64_PC64 = 24,               /* word64 = S + A - P */
	R_X86_64_GOTOFF64 = 25,           /* word64 = S + A - GOT */
	R_X86_64_GOTPC32 = 26,            /* word32 = GOT + A + P */
	R_X86_64_SIZE32 = 32,             /* word32 = Z + A */
	R_X86_64_SIZE64 = 33              /* word64 = Z + A */
};


// RISC-V Relocation Types

enum {
	R_RISCV_NONE = 0,                /* none */
	R_RISCV_32 = 1,                  /* word32 = S + A */
	R_RISCV_64 = 2,                  /* word64 = S + A */
	R_RISCV_RELATIVE = 3,            /* word(32|64) = B + A */
	R_RISCV_COPY = 4,                /* symbol must be in executable, not allowed in shared library */
	R_RISCV_JUMP_SLOT = 5,           /* word(32|64) = S (handled by PLT unless LD_BIND_NOW) */
	R_RISCV_TLS_DTPMOD32 = 6,        /* word32 = S->TLSINDEX */
	R_RISCV_TLS_DTPMOD64 = 7,        /* word64 = S->TLSINDEX */
	R_RISCV_TLS_DTPREL32 = 8,        /* word32 = TLS + S + A - TLS_TP_OFFSET */
	R_RISCV_TLS_DTPREL64 = 9,        /* word64 = TLS + S + A - TLS_TP_OFFSET */
	R_RISCV_TLS_TPREL32 = 10,        /* word32 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET */
	R_RISCV_TLS_TPREL64 = 11,        /* word64 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET */
	R_RISCV_BRANCH = 16,
	R_RISCV_JAL = 17,
	R_RISCV_CALL = 18,
	R_RISCV_CALL_PLT = 19,
	R_RISCV_GOT_HI20 = 20,
	R_RISCV_TLS_GOT_HI20 = 21,
	R_RISCV_TLS_GD_HI20 = 22,
	R_RISCV_PCREL_HI20 = 23,
	R_RISCV_PCREL_LO12_I = 24,
	R_RISCV_PCREL_LO12_S = 25,
	R_RISCV_HI20 = 26,
	R_RISCV_LO12_I = 27,
	R_RISCV_LO12_S = 28,
	R_RISCV_TPREL_HI20 = 29,
	R_RISCV_TPREL_LO12_I = 30,
	R_RISCV_TPREL_LO12_S = 31,
	R_RISCV_TPREL_ADD = 32,
	R_RISCV_ADD8 = 33,
	R_RISCV_ADD16 = 34,
	R_RISCV_ADD32 = 35,
	R_RISCV_ADD64 = 36,
	R_RISCV_SUB8 = 37,
	R_RISCV_SUB16 = 38,
	R_RISCV_SUB32 = 39,
	R_RISCV_SUB64 = 40,
	R_RISCV_GNU_VTINHERIT = 41,
	R_RISCV_GNU_VTENTRY = 42,
	R_RISCV_ALIGN = 43,
	R_RISCV_RVC_BRANCH = 44,
	R_RISCV_RVC_JUMP = 45
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

bool elf_check_magic(uint8_t *e_ident);
void elf_bswap_ehdr32(Elf32_Ehdr *ehdr32, int ei_data);
void elf_bswap_phdr32(Elf32_Phdr *phdr32, int ei_data);
void elf_bswap_shdr32(Elf32_Shdr *shdr32, int ei_data);
void elf_bswap_sym32(Elf32_Sym *sym32, int ei_data);
void elf_bswap_ehdr64(Elf64_Ehdr *ehdr64, int ei_data);
void elf_bswap_phdr64(Elf64_Phdr *phdr64, int ei_data);
void elf_bswap_shdr64(Elf64_Shdr *shdr64, int ei_data);
void elf_bswap_sym64(Elf64_Sym *sym64, int ei_data);
void elf_convert_to_ehdr64(Elf64_Ehdr *ehdr64, Elf32_Ehdr *ehdr32);
void elf_convert_to_phdr64(Elf64_Phdr *phdr64, Elf32_Phdr *phdr32);
void elf_convert_to_shdr64(Elf64_Shdr *shdr64, Elf32_Shdr *shdr32);
void elf_convert_to_sym64(Elf64_Sym *sym64, Elf32_Sym *sym32);

#endif