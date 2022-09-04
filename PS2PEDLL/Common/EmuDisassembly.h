/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuDisassembly.h
// Version: 1.001
// Desc: Main Header File for PS2 disassembly
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

#ifndef __EMU_DISASSEMBLY_H__
#define __EMU_DISASSEMBLY_H__

#include "EmuMain.h"


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Structures and Types
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef const char * (*EMUDISASSEMBLYFUNCTION)( EMU_U32, EMU_U32 );

typedef struct
{
    char                    Name[ 16 ];
    EMU_U32                 Code;
    EMU_U32                 Mask;
    bool                    Implemented;
    EMUDISASSEMBLYFUNCTION  Disassembly;
} stDisassembly;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Exported variables
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Contains a list of all PS2 instructions for disassembly
extern stDisassembly EmuInstructions[];


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Returns the address of a Label if it exists
DLLEXPORT BOOL CALLBACK EmuFindLabel( char * Label, EMU_U32 * LabelIndex );
// Returns a Label if Address has a symbol associated
DLLEXPORT void CALLBACK EmuGetDisassemblySymbol( EMU_U32 Address, char * Buffer, EMU_U32 BufferSize );
// Return an disassembly text for Address
DLLEXPORT void CALLBACK EmuDisassembly( EMU_U32 Address, char * Buffer, EMU_U32 BufferSize );

// Returns a Label if Address has a symbol associated
const char * EmuDisassemblySymbol( EMU_U32 Address );
// Returns a Name for a COP0 reg index
const char * EmuDis_GetCOP0RegName( EMU_U32 Reg );
// Returns a Name for a COP1 reg index
const char * EmuDis_GetCOP1RegName( EMU_U32 Reg );
// Returns a Name for a COP2 FP reg index
const char * EmuDis_GetCOP2FPRegName( EMU_U32 Reg );
// Returns a Name for a COP2 IP reg index
const char * EmuDis_GetCOP2IPRegName( EMU_U32 Reg );
// Returns a Name for a Main reg index
const char * EmuDis_GetRegName( EMU_U32 Reg );

// This are functions related to the disassembly process
const char * EmuDis_Nothing( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS_RT_I16( EMU_U32 Address, EMU_U32 Code );
const char * EmuDis_I26( EMU_U32 Address, EMU_U32 Code );
const char * EmuDis_0_RT_I16( EMU_U32, EMU_U32 Code );
const char * EmuDis_BASE_RT_OFF16( EMU_U32, EMU_U32 Code );
const char * EmuDis_SYSCALL( EMU_U32, EMU_U32 Code );
const char * EmuDis_0_RT_RD_SA( EMU_U32, EMU_U32 Code );
const char * EmuDis_REGISTERED( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS_RT_BRANCH16( EMU_U32 Address, EMU_U32 Code );
const char * EmuDis_RS_0_BRANCH16( EMU_U32 Address, EMU_U32 Code );
const char * EmuDis_PREF( EMU_U32, EMU_U32 Code );
const char * EmuDis_JR( EMU_U32, EMU_U32 Code );
const char * EmuDis_JALR( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS_RT_RD( EMU_U32, EMU_U32 Code );
const char * EmuDis_RT_FS( EMU_U32, EMU_U32 Code );
const char * EmuDis_FS_FD( EMU_U32, EMU_U32 Code );
const char * EmuDis_FT_FS_FD( EMU_U32, EMU_U32 Code );
const char * EmuDis_BASE_FT_OFF16( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS_RT( EMU_U32, EMU_U32 Code );
const char * EmuDis_RD( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS( EMU_U32, EMU_U32 Code );
const char * EmuDis_SHIFT_V( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS_I16( EMU_U32, EMU_U32 Code );
const char * EmuDis_RD11( EMU_U32, EMU_U32 Code );
const char * EmuDis_0_RT_RD( EMU_U32, EMU_U32 Code );
const char * EmuDis_FT_FS( EMU_U32, EMU_U32 Code );
const char * EmuDis_FT_0_FD( EMU_U32, EMU_U32 Code );
const char * EmuDis_BRANCH16( EMU_U32 Address, EMU_U32 Code );
const char * EmuDis_SYNC_STYPE( EMU_U32, EMU_U32 Code );
const char * EmuDis_RS_0_RD( EMU_U32, EMU_U32 Code );
const char * EmuDis_MC0( EMU_U32, EMU_U32 Code );
const char * EmuDis_VU_RT_FD_INTERLOCK( EMU_U32, EMU_U32 Code );
const char * EmuDis_VU_RT_ID_INTERLOCK( EMU_U32, EMU_U32 Code );
const char * EmuDis_VU_DEST_FT_FS( EMU_U32, EMU_U32 Code );
const char * EmuDis_BASE_VFT_OFF16( EMU_U32, EMU_U32 Code );
const char * EmuDis_VU_DEST_FT_FS_FD( EMU_U32, EMU_U32 Code );
const char * EmuDis_VU_DEST_FT_FS_BC( EMU_U32, EMU_U32 Code );

#endif // __EMU_DISASSEMBLY_H__
