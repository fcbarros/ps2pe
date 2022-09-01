/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: Elf.h
// Version: 1.001
// Desc: Main Header File for ELF loading
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Included comments
//		- Organized file
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __ELF_READER_H__
#define __ELF_READER_H__

// Just make sure BOOL is defined
#ifndef BOOL
    #define BOOL   int
#endif
#ifndef TRUE
    #define TRUE   1
    #define FALSE  0
#endif


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Elf Defines
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// e_ident
#define EI_MAG0         0   // File identification
#define EI_MAG1         1   // File identification
#define EI_MAG2         2   // File identification
#define EI_MAG3         3   // File identification
#define EI_CLASS        4   // File class
#define EI_DATA         5   // Data encoding
#define EI_VERSION      6   // File version
#define EI_PAD          7   // Start of padding bytes
#define EI_NIDENT       16  // Size of e_ident[]

// e_type
#define ET_NONE         0       // No file type
#define ET_REL          1       // Relocatable file
#define ET_EXEC         2       // Executable file
#define ET_DYN          3       // Shared object file
#define ET_CORE         4       // Core file
#define ET_LOPROC       0xff00  // Processor-specific
#define ET_HIPROC       0xffff  // Processor-specific

// e_machine
#define ET_NONE         0   // No machine
#define EM_M32          1   // AT&T WE 32100
#define EM_SPARC        2   // SPARC
#define EM_386          3   // Intel Architecture
#define EM_68K          4   // Motorola 68000
#define EM_88K          5   // Motorola 88000
#define EM_860          7   // Intel 80860
#define EM_MIPS         8   // MIPS RS3000 Big-Endian
#define EM_MIPS_RS4_BE  10  // MIPS RS4000 Big-Endian

// e_version
#define EV_NONE         0   // Invalid versionn
#define EV_CURRENT      1   // Current version

// e_ident values
#define ELFMAG0         0x7f    // e_ident[EI_MAG0]
#define ELFMAG1         'E'     // e_ident[EI_MAG1]
#define ELFMAG2         'L'     // e_ident[EI_MAG2]
#define ELFMAG3         'F'     // e_ident[EI_MAG3]

#define ELFCLASSNONE    0       // Invalid class
#define ELFCLASS32      1       // 32-bit objects
#define ELFCLASS64      2       // 64-bit objects

#define ELFDATANONE     0       // Invalid data encoding
#define ELFDATA2LSB     1       // Little-endian
#define ELFDATA2MSB     2       // Big-endian

// Special Section Indexes
#define SHN_UNDEF           0
#define SHN_LORESERVE       0xff00
#define SHN_LOPROC          0xff00
#define SHN_HIPROC          0xff1f
#define SHN_ABS             0xfff1
#define SHN_COMMON          0xfff2
#define SHN_HIRESERVE       0xffff

// Section Types, sh_type
#define SHT_NULL            0
#define SHT_PROGBITS        1
#define SHT_SYMTAB          2
#define SHT_STRTAB          3
#define SHT_RELA            4
#define SHT_HASH            5
#define SHT_DYNAMIC         6
#define SHT_NOTE            7
#define SHT_NOBITS          8
#define SHT_REL             9
#define SHT_SHLIB           10
#define SHT_DYNSYM          11
#define SHT_LOPROC          0x70000000
#define SHT_HIPROC          0x7fffffff
#define SHT_LOUSER          0x80000000
#define SHT_HIUSER          0xffffffff

// Section Attribute Flags, sh_flags
#define SHF_WRITE           0x1
#define SHF_ALLOC           0x2
#define SHF_EXECINSTR       0x4
#define SHF_MASKPROC        0xf0000000

#define PT_NULL         0
#define PT_LOAD         1
#define PT_DYNAMIC      2
#define PT_INTERP       3
#define PT_NOTE         4
#define PT_SHLIB        5
#define PT_PHDR         6
#define PT_LOPROC       0x70000000
#define PT_HIPROC       0x7fffffff

#define ELF32_ST_BIND(i)    ((i)>>4)
#define ELF32_ST_TYPE(i)    ((i)&0xf)
#define ELF32_ST_INFO(b,t)  (((b)<<4)+((t)&0xf))

#define ELF32_R_SYM(i)      ((i)>>8)
#define ELF32_R_TYPE(i)     ((unsigned char)(i))
#define ELF32_R_INFO(s,t)   (((s)<<8)+(unsigned char)(t))

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Elf types and structures
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef unsigned int   Elf32_Addr;      // 4 bytes - Unsigned program address
typedef unsigned short Elf32_Half;      // 2 bytes - Unsigned medium integer
typedef unsigned int   Elf32_Off;       // 4 bytes - Unsigned file offset
typedef int            Elf32_Sword;     // 4 bytes - Signed large integer
typedef unsigned int   Elf32_Word;      // 4 bytes - Unsigned large integer

// ELF Header
typedef struct
{
    unsigned char   e_ident[EI_NIDENT];
    Elf32_Half      e_type;
    Elf32_Half      e_machine;
    Elf32_Word      e_version;
    Elf32_Addr      e_entry;
    Elf32_Off       e_phoff;
    Elf32_Off       e_shoff;
    Elf32_Word      e_flags;
    Elf32_Half      e_ehsize;
    Elf32_Half      e_phentsize;
    Elf32_Half      e_phnum;
    Elf32_Half      e_shentsize;
    Elf32_Half      e_shnum;
    Elf32_Half      e_shstrndx;
} Elf32_Ehdr;

// Section Header
typedef struct
{
    Elf32_Word  sh_name;
    Elf32_Word  sh_type;
    Elf32_Word  sh_flags;
    Elf32_Addr  sh_addr;
    Elf32_Off   sh_offset;
    Elf32_Word  sh_size;
    Elf32_Word  sh_link;
    Elf32_Word  sh_info;
    Elf32_Word  sh_addralign;
    Elf32_Word  sh_entsize;
} Elf32_Shdr;

typedef struct
{
    Elf32_Word  p_type;
    Elf32_Off   p_offset;
    Elf32_Addr  p_vaddr;
    Elf32_Addr  p_paddr;
    Elf32_Word  p_filesz;
    Elf32_Word  p_memsz;
    Elf32_Word  p_flags;
    Elf32_Word  p_align;
} Elf32_Phdr;

typedef struct
{
    Elf32_Addr r_offset;
    Elf32_Word r_info;
} Elf32_Rel;

typedef struct
{
    Elf32_Addr r_offset;
    Elf32_Word r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct
{
    Elf32_Word      st_name;
    Elf32_Addr      st_value;
    Elf32_Word      st_size;
    unsigned char   st_info;
    unsigned char   st_other;
    Elf32_Half      st_shndx;
} Elf32_Sym;

typedef struct
{
    Elf32_Ehdr  Header;
    Elf32_Phdr  * Program;
    Elf32_Shdr  * Session;
    Elf32_Sym   * Symbol;
    Elf32_Word  Symbol_num;
    char        * SessionSTable;
    char        * SymbolSTable;
} Elf32_File;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

Elf32_File * CreateElf( void );
void DestroyElf( Elf32_File * ElfFile );
BOOL ReadElf( const char * FileName, Elf32_File * ElfFile );

#endif