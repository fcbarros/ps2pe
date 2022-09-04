
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf.h"

// Creates and Elf struct to use
Elf32_File* CreateElf(void)
{
	Elf32_File* NewElf = (Elf32_File*)malloc(sizeof(Elf32_File));
	memset(NewElf, 0, sizeof(Elf32_File));

	return NewElf;
}

// Destroys and Elf struct and releases memory
void DestroyElf(Elf32_File* ElfFile)
{
	if (!ElfFile)
	{
		return;
	}

	if (ElfFile->Program)
	{
		free(ElfFile->Program);
	}
	if (ElfFile->Session)
	{
		free(ElfFile->Session);
	}
	if (ElfFile->SessionSTable)
	{
		free(ElfFile->SessionSTable);
	}
	if (ElfFile->Symbol)
	{
		free(ElfFile->Symbol);
	}
	if (ElfFile->SymbolSTable)
	{
		free(ElfFile->SymbolSTable);
	}
	free(ElfFile);
}

// Fill the Elf struct with info from File
BOOL ReadElf(const char* FileName, Elf32_File* ElfFile)
{
	FILE* pElfFile;
	
	if (fopen_s(&pElfFile, FileName, "rb") != 0)
	{
		return FALSE;
	}

	// Reading Main Header
	fseek(pElfFile, 0, SEEK_SET);
	fread(&ElfFile->Header, sizeof(Elf32_Ehdr), 1, pElfFile);

	// Validating Elf Header
	if ((ElfFile->Header.e_ident[0] != ELFMAG0) ||
		(ElfFile->Header.e_ident[1] != ELFMAG1) ||
		(ElfFile->Header.e_ident[2] != ELFMAG2) ||
		(ElfFile->Header.e_ident[3] != ELFMAG3))
	{
		fclose(pElfFile);
		return FALSE;
	}

	if (ElfFile->Header.e_phoff > 0)
	{
		// Validating the Program Header sizes
		assert(ElfFile->Header.e_phentsize == sizeof(Elf32_Phdr));

		// Freeing allocated structs
		if (ElfFile->Program)
		{
			free(ElfFile->Program);
		}
		// Allocating necessary memory for the Program Headers
		ElfFile->Program = (Elf32_Phdr*)malloc(ElfFile->Header.e_phnum * sizeof(Elf32_Phdr));

		// Reading the Program header
		fseek(pElfFile, ElfFile->Header.e_phoff, SEEK_SET);
		fread(ElfFile->Program, sizeof(Elf32_Phdr), ElfFile->Header.e_phnum, pElfFile);
	}

	if ((ElfFile->Header.e_shoff > 0) &&
		(ElfFile->Header.e_shnum > 0))
	{
		// Validating the Session Header sizes
		assert(ElfFile->Header.e_shentsize == sizeof(Elf32_Shdr));

		// Freeing allocated structs
		if (ElfFile->Session)
		{
			free(ElfFile->Session);
		}
		// Allocating necessary memory for the Sessions
		ElfFile->Session = (Elf32_Shdr*)malloc(ElfFile->Header.e_shnum * sizeof(Elf32_Shdr));

		// Reading the Sessions
		fseek(pElfFile, ElfFile->Header.e_shoff, SEEK_SET);
		fread(ElfFile->Session, sizeof(Elf32_Shdr), ElfFile->Header.e_shnum, pElfFile);

		// Session String Table
		fseek(pElfFile, ElfFile->Session[ElfFile->Header.e_shstrndx].sh_offset, SEEK_SET);
		// Freeing allocated structs
		if (ElfFile->SessionSTable)
		{
			free(ElfFile->SessionSTable);
		}
		ElfFile->SessionSTable = (char*)malloc(ElfFile->Session[ElfFile->Header.e_shstrndx].sh_size);
		fread(ElfFile->SessionSTable, ElfFile->Session[ElfFile->Header.e_shstrndx].sh_size, 1, pElfFile);

		// Looking for a Symbol Table
		for (int i = 0; i < ElfFile->Header.e_shnum; i++)
		{
			if (ElfFile->Session[i].sh_type == SHT_SYMTAB)
			{
				fseek(pElfFile, ElfFile->Session[i].sh_offset, SEEK_SET);
				// Freeing allocated structs
				if (ElfFile->Symbol)
				{
					free(ElfFile->Symbol);
				}
				// Read the Symbol Table
				ElfFile->Symbol = (Elf32_Sym*)malloc(ElfFile->Session[i].sh_size);
				fread(ElfFile->Symbol, ElfFile->Session[i].sh_size, 1, pElfFile);
				ElfFile->Symbol_num = ElfFile->Session[i].sh_size / ElfFile->Session[i].sh_entsize;

				// Read the Symbol String Table
				fseek(pElfFile, ElfFile->Session[ElfFile->Session[i].sh_link].sh_offset, SEEK_SET);
				// Freeing allocated structs
				if (ElfFile->SymbolSTable)
				{
					free(ElfFile->SymbolSTable);
				}
				ElfFile->SymbolSTable = (char*)malloc(ElfFile->Session[ElfFile->Session[i].sh_link].sh_size);
				fread(ElfFile->SymbolSTable, ElfFile->Session[ElfFile->Session[i].sh_link].sh_size, 1, pElfFile);
				break;
			}
		}
	}

	fclose(pElfFile);
	return TRUE;
}

