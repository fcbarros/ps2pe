/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuMain.h
// Version: 1.001
// Desc: Main Header File for PS2 emulator
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Included comments
//		- Removed unused functions ( EmuSwap, EmuOptimizeList )
//		- Included Defines
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#pragma once

#define EMU_LOG

#include "../StdAfx.h"
#include <vector>

#include "EmuPS2Core.h"
#include "Elf.h"

/////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////

typedef void (*EMUCONSOLECALLBACK)(const char*);

extern EMU_PS2_Regs PS2Regs;

/////////////////////////////////////////////////////////////////////
// Main Functions
/////////////////////////////////////////////////////////////////////

DLLEXPORT void CALLBACK EmuReset();
DLLEXPORT EMU_U32 CALLBACK EmuInstructionIndex(EMU_U32 tInst);
DLLEXPORT void CALLBACK EmuLog(const char* Format, ...);
DLLEXPORT void CALLBACK EmuConsole(const char* Format, ...);
DLLEXPORT double CALLBACK EmuGetClock();
DLLEXPORT void CALLBACK EmuSetByte(EMU_U32 Address, EMU_U08 Data);
DLLEXPORT void CALLBACK EmuSetShort(EMU_U32 Address, EMU_U16 Data);
DLLEXPORT void CALLBACK EmuSetWord(EMU_U32 Address, EMU_U32 Data);
DLLEXPORT void CALLBACK EmuSetDWord(EMU_U32 Address, EMU_U64 Data);
DLLEXPORT EMU_U08 CALLBACK EmuGetByte(EMU_U32 Address);
DLLEXPORT EMU_U16 CALLBACK EmuGetShort(EMU_U32 Address);
DLLEXPORT EMU_U32 CALLBACK EmuGetWord(EMU_U32 Address);
DLLEXPORT EMU_U64 CALLBACK EmuGetDWord(EMU_U32 Address);

void EmuLoadConfig();
void EmuLoadPlugins();
void EmuReleasePlugins();
void EmuSetDir(const char* Dir, int size);
void EmuBiosSyscall();
