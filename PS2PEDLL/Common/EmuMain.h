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

#ifndef __EMU_MAIN_H__
#define __EMU_MAIN_H__

#define EMU_LOG

#include "../StdAfx.h"
#include <vector>

#include "EmuPS2Core.h"
#include "Elf.h"
#include "EmuVU.h"
#include "EmuMemory.h"

/////////////////////////////////////////////////////////////////////
// General Defines
/////////////////////////////////////////////////////////////////////


// PS2 GS clock 147.456 Mhz
#define GS_CORE_CLOCK			147456000	// hz

// Core clock / 16
#define GS_CORE_CLOCK_16		  9216000	// hz
// Core clock / 256
#define GS_CORE_CLOCK_256		   576000	// hz


/////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////

typedef void (*EMUCONSOLECALLBACK)(const char*);

// Struct related to Stats
typedef struct
{
	EMU_U32 Index;
	EMU_U32 Total;
} stEmuInstructionCount;

typedef std::vector<stEmuInstructionCount> sEmutStats;

/////////////////////////////////////////////////////////////////////
// External Variables
/////////////////////////////////////////////////////////////////////

// Current CPU clock
extern double CPUClock;
// Total number of instructions in EmuInstructions.cpp file
extern EMU_U32 TotalInstructions;
// Loaded instructions statistics
extern sEmutStats EmuInstructionsStats;
// Running instructions statistics
extern sEmutStats EmuRunningStats;
// Pointer to the Elf structure current loaded
extern Elf32_File* ElfFile;

extern EMU_PS2_Regs PS2Regs;
//// Core registers
//extern Emu_R5900_Regs R5900Regs;
//// COP0 registers
//extern Emu_COP0_Regs COP0Regs;
//// COP1 registers
//extern Emu_COP1_Regs COP1Regs;
// 
// Registers - Backup for handlers
extern Emu_R5900_Regs R5900RegsBackup[10];
extern Emu_COP0_Regs COP0RegsBackup[10];
extern Emu_COP1_Regs COP1RegsBackup[10];
// Is Emu in Interrupt
extern EMU_U08 EmuInterruptIndex;
// Is Emu in running mode?
extern bool EmuRunning;
// Boolean to stop a running (Run or Exec) program
extern bool EmuStopRun;
// Is current instruction in BranchDelay Slot?
extern bool EmuInBranchDelay;
// Address Breakpoint
extern std::vector<EMU_U32> BreakPoints;
// Instructions Breakpoint
extern std::vector<EMU_U32> InstrBreakPoints;
// ScratchPad Memory
extern EMU_U08 EMemScratchPad[16 * 1024];

extern char BiosFileName[1024];
extern char GSFileName[1024];
extern char PADFileName[1024];
extern char SPUFileName[1024];


/////////////////////////////////////////////////////////////////////
// Main Functions
/////////////////////////////////////////////////////////////////////

// Initialize the Emulator, must be called once
DLLEXPORT void CALLBACK EmuInitialize();
// Release memory related to the emulator
DLLEXPORT void CALLBACK EmuRelease();
// Do an Emulator Reset
DLLEXPORT void CALLBACK EmuReset();
// Set the Bios File to load
DLLEXPORT void CALLBACK EmuSetBiosFile(const char* FileName);
// Load an ELF file into memory
DLLEXPORT EMU_I32 CALLBACK EmuLoad(const char* FileName);
// Get the Index related to the EmuInstructions structure 
DLLEXPORT EMU_U32 CALLBACK EmuInstructionIndex(EMU_U32 tInst);
// Add an Address breakpoint
DLLEXPORT void CALLBACK EmuAddBreakPoint(EMU_U32 Address);
// Remove an Address breakpoint
DLLEXPORT void CALLBACK EmuRemoveBreakPoint(EMU_U32 Address);
// Check to see if the address is a breakpoint
DLLEXPORT BOOL CALLBACK EmuIsBreakPoint(EMU_U32 Address);
DLLEXPORT void CALLBACK EmuLog(const char* Format, ...);
DLLEXPORT void CALLBACK EmuConsole(const char* Format, ...);
DLLEXPORT void CALLBACK EmuSetConsoleCallback(EMUCONSOLECALLBACK function);
DLLEXPORT double CALLBACK EmuGetClock();
// Runs the program, generate running stats and stops at breakpoints
DLLEXPORT void CALLBACK EmuRun(EMU_U32 tAddress);
// Executes the program, does NOT stops at breakpoints or generates running stats
DLLEXPORT void CALLBACK EmuExecute(EMU_U32 tAddress);
// Executes the current instruction, if it is a jump, put a breakpoint
// after the instruction and executes the jump
DLLEXPORT void CALLBACK EmuStepOver(EMU_U32 tAddress);
DLLEXPORT void CALLBACK EmuStepInto(EMU_U32 tAddress);
DLLEXPORT void CALLBACK EmuGetRegs(Emu_R5900_Regs** r5900regs,
	Emu_COP0_Regs** cop0regs,
	Emu_COP1_Regs** cop1regs,
	EMU_VU_Regs** vu0regs,
	EMU_VU_Regs** vu1regs);
DLLEXPORT EMU_U32 CALLBACK EmuGetTotalInstructions();
DLLEXPORT void CALLBACK EmuGetInstructionsStats(EMU_U32* TotalSupportedInstructions,
	EMU_U32* TotalDisassembledInstructions,
	EMU_U32* TotalImplementedInstructions);
DLLEXPORT void CALLBACK EmuGetRunningStats(sEmutStats**);
DLLEXPORT void CALLBACK EmuGetLoadedStats(sEmutStats**);
DLLEXPORT void CALLBACK EmuSetByte(EMU_U32 Address, EMU_U08 Data);
DLLEXPORT void CALLBACK EmuSetShort(EMU_U32 Address, EMU_U16 Data);
DLLEXPORT void CALLBACK EmuSetWord(EMU_U32 Address, EMU_U32 Data);
DLLEXPORT void CALLBACK EmuSetDWord(EMU_U32 Address, EMU_U64 Data);
DLLEXPORT EMU_U08 CALLBACK EmuGetByte(EMU_U32 Address);
DLLEXPORT EMU_U16 CALLBACK EmuGetShort(EMU_U32 Address);
DLLEXPORT EMU_U32 CALLBACK EmuGetWord(EMU_U32 Address);
DLLEXPORT EMU_U64 CALLBACK EmuGetDWord(EMU_U32 Address);
DLLEXPORT void CALLBACK EmuAddInstructionBreakpoint(EMU_U32 InstIndex);
DLLEXPORT void CALLBACK EmuRemoveInstructionBreakpoint(EMU_U32 InstIndex);
DLLEXPORT BOOL CALLBACK EmuIsInstructionBreakpoint(EMU_U32 InstIndex);
DLLEXPORT void CALLBACK EmuGetInstructionInfo(EMU_U32 InstIndex,
	char** Name,
	BOOL* IsDisassembled,
	BOOL* IsImplemented);
DLLEXPORT void CALLBACK EmuConfig();
// Returns a Name for a COP0 reg index
DLLEXPORT void CALLBACK EmuGetCOP0RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
// Returns a Name for a COP1 reg index
DLLEXPORT void CALLBACK EmuGetCOP1RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
// Returns a Name for a COP2 FP reg index
DLLEXPORT void CALLBACK EmuGetCOP2FPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
// Returns a Name for a COP2 IP reg index
DLLEXPORT void CALLBACK EmuGetCOP2IPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
// Returns a Name for a Main reg index
DLLEXPORT void CALLBACK EmuGetR5900RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);

// Clear All statistics
void EmuClearStats();
// Generate Loaded Stats from Start address to End address
void EmuGenStats(EMU_U32 Start, EMU_U32 End);
// Runs the program, generate running stats and stops at breakpoints
void EmuRunDebug(EMU_U32 tAddress, bool InLoop);
// Executes the program, does NOT stops at breakpoints or generates running stats
void EmuExecuteFast(EMU_U32 tAddress, bool InLoop);
void EmuLoadConfig();
void EmuSaveConfig();
void EmuLoadPlugins();
void EmuReleasePlugins();
void EmuSetDir(char* Dir);


#endif //__EMU_MAIN_H__
