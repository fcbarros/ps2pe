#include <stdio.h>

#include "Bios.h"
#include "Gs.h"
#include "Ps2Core.h"

//////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////

Common::Ps2Core &ps2Core = Common::Ps2Core::GetInstance();

EMU_PS2_Regs PS2Regs;

// Initialize the Emulator, must be called once
// Initializes Registers and anythings else needed
DLLEXPORT void CALLBACK EmuInitialize()
{
	ps2Core.Initialize();
}

// Do an Emulator Reset
DLLEXPORT void CALLBACK EmuReset()
{
	ps2Core.Reset();
}

// Set the Bios File to load
DLLEXPORT void CALLBACK EmuSetBiosFile(const char* FileName)
{
	ps2Core.SetBiosFile(FileName);
}

// Release memory related to the emulator
DLLEXPORT void CALLBACK EmuRelease()
{
	ps2Core.Release();
}

// Load an ELF file into memory
DLLEXPORT EMU_I32 CALLBACK EmuLoad(const char* FileName)
{
	return ps2Core.Load(FileName);
}

DLLEXPORT void CALLBACK EmuAddBreakPoint(EMU_U32 Address)
{
	ps2Core.AddBreakPoint(Address);
}

DLLEXPORT void CALLBACK EmuRemoveBreakPoint(EMU_U32 Address)
{
	ps2Core.RemoveBreakPoint(Address);
}

DLLEXPORT BOOL CALLBACK EmuIsBreakPoint(EMU_U32 Address)
{
	return ps2Core.IsBreakPoint(Address);
}

// Executes the current instruction, if it is a jump, put a breakpoint
// after the instruction and executes the jump
DLLEXPORT void CALLBACK EmuStepOver(EMU_U32 tAddress)
{
	ps2Core.StepOver(tAddress);
}

DLLEXPORT void CALLBACK EmuStepInto(EMU_U32 Address)
{
	ps2Core.StepOver(Address);
}

// Runs the program, generate running stats and stops at breakpoints
DLLEXPORT void CALLBACK EmuRun(unsigned int Address)
{
	ps2Core.Run(Address);
}

// Executes the program, does NOT stops at breakpoints or generates running stats
DLLEXPORT void CALLBACK EmuExecute(unsigned int Address)
{
	ps2Core.Execute(Address);
}

DLLEXPORT void CALLBACK EmuLog(const char* Format, ...)
{
	va_list vaParams;

	va_start(vaParams, Format);
	ps2Core.Log(Format, vaParams);
	va_end(vaParams);
}

DLLEXPORT void CALLBACK EmuAddInstructionBreakpoint(EMU_U32 InstIndex)
{
	ps2Core.AddInstructionBreakpoint(InstIndex);
}

DLLEXPORT void CALLBACK EmuRemoveInstructionBreakpoint(EMU_U32 InstIndex)
{
	ps2Core.RemoveInstructionBreakpoint(InstIndex);
}

DLLEXPORT BOOL CALLBACK EmuIsInstructionBreakpoint(EMU_U32 InstIndex)
{
	return ps2Core.IsInstructionBreakpoint(InstIndex);
}

DLLEXPORT void CALLBACK EmuGetRunningStats(stEmuInstructionCount** stats)
{
	ps2Core.GetRunningStats(stats);
}

DLLEXPORT void CALLBACK EmuGetLoadedStats(stEmuInstructionCount** stats)
{
	ps2Core.GetLoadedStats(stats);
}

DLLEXPORT void CALLBACK EmuGetInstructionInfo(EMU_U32 InstIndex, char** Name, BOOL* IsDisassembled, BOOL* IsImplemented)
{
	ps2Core.GetInstructionInfo(InstIndex, Name, IsDisassembled, IsImplemented);
}

DLLEXPORT void CALLBACK EmuSetConsoleCallback(EMUCONSOLECALLBACK function)
{
	ps2Core.SetConsoleCallback(function);
}

DLLEXPORT void CALLBACK EmuGetRegs(Emu_R5900_Regs** r5900regs,
	Emu_COP0_Regs** cop0regs, Emu_COP1_Regs** cop1regs,
	EMU_VU_Regs** vu0regs, EMU_VU_Regs** vu1regs)
{
	ps2Core.GetRegs(r5900regs, cop0regs,  cop1regs, vu0regs,  vu1regs);
}

DLLEXPORT void CALLBACK EmuConsole(const char* Format, ...)
{
	va_list vaParams;

	va_start(vaParams, Format);
	ps2Core.Console(Format, vaParams);
	va_end(vaParams);
}

DLLEXPORT EMU_U32 CALLBACK EmuGetTotalInstructions()
{
	return ps2Core.GetTotalInstructions();
}

DLLEXPORT void CALLBACK EmuSetByte(EMU_U32 Address, EMU_U08 Data)
{
	ps2Core.SetByte(Address, Data);
}

DLLEXPORT void CALLBACK EmuSetShort(EMU_U32 Address, EMU_U16 Data)
{
	ps2Core.SetShort(Address, Data);
}

DLLEXPORT void CALLBACK EmuSetWord(EMU_U32 Address, EMU_U32 Data)
{
	ps2Core.SetWord(Address, Data);
}

DLLEXPORT void CALLBACK EmuSetDWord(EMU_U32 Address, EMU_U64 Data)
{
	ps2Core.SetDWord(Address, Data);
}

DLLEXPORT EMU_U08 CALLBACK EmuGetByte(EMU_U32 Address)
{
	return ps2Core.GetByte(Address);
}

DLLEXPORT EMU_U16 CALLBACK EmuGetShort(EMU_U32 Address)
{
	return ps2Core.GetShort(Address);
}

DLLEXPORT EMU_U32 CALLBACK EmuGetWord(EMU_U32 Address)
{
	return ps2Core.GetWord(Address);
}

DLLEXPORT EMU_U64 CALLBACK EmuGetDWord(EMU_U32 Address)
{
	return ps2Core.GetDWord(Address);
}

// Get the Index related to the EmuInstructions structure 
DLLEXPORT EMU_U32 CALLBACK EmuInstructionIndex(EMU_U32 tInst)
{
	return ps2Core.InstructionIndex(tInst);
}

DLLEXPORT void CALLBACK EmuGetInstructionsStats(EMU_U32* TotalSupportedInstructions, EMU_U32* TotalDisassembledInstructions, EMU_U32* TotalImplementedInstructions)
{
	ps2Core.GetInstructionsStats(TotalSupportedInstructions, TotalDisassembledInstructions, TotalImplementedInstructions);
}

DLLEXPORT void CALLBACK EmuGSInitWindow()
{
	Common::Gs::GetInstance().InitWindow();
}

DLLEXPORT void CALLBACK EmuGSConfigure()
{
	Common::Gs::GetInstance().Configure();
}

DLLEXPORT void CALLBACK EmuGSCloseWindow()
{
	Common::Gs::GetInstance().CloseWindow();
}

DLLEXPORT void CALLBACK EmuPADConfig()
{
	Common::Pad::GetInstance().Config();
}

DLLEXPORT void CALLBACK EmuGetCOP0RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
{
	strncpy(Buffer, EmuDis_GetCOP0RegName(Reg), BufferSize);
}

DLLEXPORT void CALLBACK EmuGetCOP1RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
{
	strncpy(Buffer, EmuDis_GetCOP1RegName(Reg), BufferSize);
}

DLLEXPORT void CALLBACK EmuGetCOP2FPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
{
	strncpy(Buffer, EmuDis_GetCOP2FPRegName(Reg), BufferSize);
}

DLLEXPORT void CALLBACK EmuGetCOP2IPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
{
	strncpy(Buffer, EmuDis_GetCOP2IPRegName(Reg), BufferSize);
}

DLLEXPORT void CALLBACK EmuGetR5900RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
{
	strncpy(Buffer, EmuDis_GetRegName(Reg), BufferSize);
}

void EmuBiosSyscall()
{
	Common::Bios::GetInstance().Syscall();
}