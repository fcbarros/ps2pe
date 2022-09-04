/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuBios.h
// Version: 1.001
// Desc: Main Header File for PS2 Bios handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 18/05/2002
// Modifications:
// * Version 1.000 ( 18/05/2002 ): Created
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_BIOS_H__
#define __EMU_BIOS_H__

#include "EmuMemory.h"

typedef void (*EMUBIOSFUNCTIONS)( void );

extern const char * Emu_Bios_Calls_Names[ 256 ];

#define EMU_BIOS_START_ADDR         0x1FC00000
#define EMU_BIOS_END_ADDR           0x1FFFFFFF

extern EMU_U08 * psR;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void Emu_Bios_Load( char * FileName );
void Emu_Bios_Init( void );

EMU_U08 * Emu_Bios_GetPointer( EMU_U32 Address );

void Emu_Bios_Syscall( void );

void Emu_Bios_Dummy( void );
void Emu_Bios_Exit( void );
void Emu_Bios_RFU060( void );
void Emu_Bios_RFU061( void );
void Emu_Bios_EndOfHeap( void );
void Emu_Bios_FlushCache( void );
void Emu_Bios_Deci2Call( void );
void Emu_Bios_ExecPS2( void );

#endif