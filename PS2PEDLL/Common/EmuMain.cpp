#include <stdio.h>
#include <time.h>
#include <float.h>

#include "EmuDma.h"
#include "EmuDisassembly.h"
#include "EmuGS.h"
#include "EmuGif.h"
#include "EmuVif.h"
#include "EmuTimer.h"
#include "EmuIpu.h"
#include "EmuFifo.h"
#include "EmuSif.h"
#include "EmuIntc.h"
#include "EmuVU.h"
#include "EmuThread.h"
#include "EmuBios.h"
#include "EmuRecompiler.h"

//////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////

EMU_U32 TotalInstructions = 0;

double CPUClock;

Elf32_File* ElfFile;

EMU_U08 EMemScratchPad[16 * 1024];

Emu_R5900_Regs R5900Regs;
Emu_COP0_Regs COP0Regs;
Emu_COP1_Regs COP1Regs;

Emu_R5900_Regs R5900RegsBackup[10];
Emu_COP0_Regs COP0RegsBackup[10];
Emu_COP1_Regs COP1RegsBackup[10];

bool EmuRunning = false;
bool EmuStopRun = false;
bool EmuInBranchDelay = false;

EMU_U08 EmuInterruptIndex = 0;

std::vector<EMU_U32>   BreakPoints;
std::vector<EMU_U32>   InstrBreakPoints;

std::vector<stEmuInstructionCount> EmuInstructionsStats;

std::vector<stEmuInstructionCount> EmuRunningStats;

FILE* fPointer = NULL;

char BiosFileName[1024];
char GSFileName[1024];
char PADFileName[1024];
char SPUFileName[1024];

EMUCONSOLECALLBACK EmuConsoleCallback;

EMU_U32 StartAddress;
EMU_U32 EndAddress;

// Initializes Registers and anythings else needed
DLLEXPORT void CALLBACK EmuInitialize()
{
	EmuMemInit(32 * 1024 * 1024, 64 * 1024);

	CPUClock = EmuGetClock();

	EmuLoadConfig();
	EmuLoadPlugins();

	EmuConsoleCallback = NULL;

	ElfFile = CreateElf();

#ifdef EMU_LOG
	//    fPointer = fopen( "ps2pe.log", "w" );

	time_t CurTime = time(NULL);

	EmuLog("PS2PE - PS2 Personal Emulator v0.01\n\n");
	EmuLog("Log: %s\n", ctime(&CurTime));
	EmuLog("CPU Clock: %.2f Mhz\n\n", CPUClock / 1000000.0f);
	EmuLog("========================================================\n");
	EmuLog("Initialing emulator\n");
#endif

	EmuInstructionsStats.clear();
	EmuRunningStats.clear();

	int i = 0;

	while (strcmp(EmuInstructions[i].Name, "UNKNOW"))
	{
		i++;
	}
	TotalInstructions = i;
	EmuInstructionsStats.resize(TotalInstructions + 1);
	EmuRunningStats.resize(TotalInstructions + 1);
	InstrBreakPoints.resize(TotalInstructions + 1);
	EmuClearStats();

	EmuMemClear();

	BreakPoints.clear();

	Emu_Bios_Init();
	Emu_Dma_Init();
	Emu_Gif_Init();
	Emu_GS_Init();
	Emu_PAD_Init();
	Emu_Vif_Init();
	Emu_Timer_Init();
	Emu_Ipu_Init();
	Emu_Fifo_Init();
	Emu_Sif_Init();
	Emu_Intc_Init();
	Emu_Vu_Init();
	Emu_Thread_Init();

	EmuRec_Init(16 * 1024 * 1024);

	EmuReset();

	EmuMemAddWriteCallBack(0x12000000, 0x13FFFFFF, Emu_GS_WriteCallback);
	EmuMemAddWriteCallBack(0x1000F520, 0x1000F5FF, Emu_Dma_callback);
	EmuMemAddWriteCallBack(0x10003000, 0x100037FF, Emu_Gif_callback);
	EmuMemAddWriteCallBack(0x10000000, 0x10001FFF, Emu_Timer_callback);
	EmuMemAddWriteCallBack(0x10002000, 0x10002FFF, Emu_Ipu_callback);
	EmuMemAddWriteCallBack(0x10003800, 0x10003FFF, Emu_Vif_callback);
	EmuMemAddWriteCallBack(0x10004000, 0x10007FFF, Emu_Fifo_callback);
	EmuMemAddWriteCallBack(0x1000F000, 0x1000F1FF, Emu_Intc_callback);
	EmuMemAddWriteCallBack(0x1000F230, 0x1000F2FF, Emu_Sif_callback);
	EmuMemAddWriteCallBack(0x10008000, 0x1000EFFF, Emu_Dma_callback);

	//    EmuMemAddWriteCallBack( 0x11000000, 0x11007FFF, Emu_Vu0_callback );
	//    EmuMemAddWriteCallBack( 0x11008000, 0x11FFFFFF, Emu_Vu1_callback );

	//    EmuMemAddReadCallBack( 0x12000000, 0x13FFFFFF, Emu_GS_ReadCallback );
}

DLLEXPORT void CALLBACK EmuReset()
{
#ifdef EMU_LOG
	EmuLog("Reseting emulator\n");
#endif

	EmuClearStats();

	memset(&R5900Regs, 0, sizeof(R5900Regs));
	memset(&COP0Regs, 0, sizeof(COP0Regs));
	memset(&COP1Regs, 0, sizeof(COP1Regs));

	R5900Regs.SP.u64_00_63 = (EMU_I32)0x81F00000;
	R5900Regs.GP.u64_00_63 = (EMU_I32)0x81F80000;

	COP0Regs.Status = 0x10900000;
	COP0Regs.PRId_REV = 0x20;
	COP0Regs.PRId_IMP = 0x2E;

	COP1Regs.FCR0_REV = 0x00;
	COP1Regs.FCR0_IMP = 0x2E;
	COP1Regs.FCR31_00 = 1;
	COP1Regs.FCR31_24 = 1;

	Emu_Dma_Reset();
	Emu_Gif_Reset();
	Emu_GS_Reset();
	Emu_PAD_Reset();
	Emu_Vif_Reset();
	Emu_Timer_Reset();
	Emu_Ipu_Reset();
	Emu_Fifo_Reset();
	Emu_Sif_Reset();
	Emu_Intc_Reset();
	Emu_Vu_Reset();
	Emu_Thread_Reset();

	EmuRunning = false;

	EmuInterruptIndex = 0;

	R5900Regs.PC = ElfFile->Header.e_entry;

	EmuRec_Reset();
}

DLLEXPORT void CALLBACK EmuSetBiosFile(const char* FileName)
{
#ifdef EMU_LOG
	EmuLog("Setting Bios File: %s\n", FileName);
#endif
	strcpy(BiosFileName, FileName);
}

// Releases memory assossiated with file
DLLEXPORT void CALLBACK EmuRelease()
{
#ifdef EMU_LOG
	EmuLog("Exiting emulator\n");
#endif
	DestroyElf(ElfFile);

	if (fPointer)
	{
		fclose(fPointer);
	}

	GSshutdown();

	EmuRec_Shutdown();

	EmuMemShutdown();
}

EMU_U32 EmuInstructionIndex2(EMU_U32 tInst)
{
	for (EMU_U32 i = 0; i < TotalInstructions; i++)
	{
		if ((tInst & EmuInstructions[i].Mask) == EmuInstructions[i].Code)
		{
			return i;
		}
	}

	return TotalInstructions;
}

// Returns the instruction Index of an Instruction
DLLEXPORT EMU_U32 CALLBACK EmuInstructionIndex(EMU_U32 tInst)
{
	return EmuInstructionIndex2(tInst);
}

// Loads a File into mem
DLLEXPORT EMU_I32 CALLBACK EmuLoad(const char* FileName)
{
	DestroyElf(ElfFile);
	ElfFile = CreateElf();

	if (!ReadElf(FileName, ElfFile))
	{
		return 0;
	}
	if ((ElfFile->Header.e_type != ET_EXEC) ||
		(ElfFile->Header.e_machine != EM_MIPS))
	{
		return 0;
	}

#ifdef EMU_LOG
	EmuLog("Reading ELF %s\n", FileName);
#endif

	EmuMemClear();

	Emu_Dma_Init();
	Emu_Gif_Init();
	Emu_GS_Init();
	Emu_PAD_Init();
	Emu_Vif_Init();
	Emu_Timer_Init();
	Emu_Ipu_Init();
	Emu_Fifo_Init();
	Emu_Sif_Init();
	Emu_Intc_Init();
	Emu_Vu_Init();
	Emu_Thread_Init();

	EmuReset();

	BreakPoints.clear();

	if (strlen(BiosFileName))
	{
		Emu_Bios_Load(BiosFileName);
	}
	StartAddress = 0xFFFFFFFF;
	EndAddress = 0x0;

	// Mapped memory
	for (int i = 0; i < ElfFile->Header.e_phnum; i++)
	{
		switch (ElfFile->Program[i].p_type)
		{
		case PT_LOAD:
			if (ElfFile->Program[i].p_memsz > 0)
			{
				EmuMemAddFromFile(FileName,
					ElfFile->Program[i].p_offset,
					ElfFile->Program[i].p_filesz,
					ElfFile->Program[i].p_vaddr,
					EMMP_READ | EMMP_WRITE | EMMP_EXEC);
				EmuGenStats(ElfFile->Program[i].p_vaddr,
					ElfFile->Program[i].p_vaddr + ElfFile->Program[i].p_filesz);
				if (StartAddress > ElfFile->Program[i].p_vaddr)
				{
					StartAddress = ElfFile->Program[i].p_vaddr;
				}
				if (EndAddress < (ElfFile->Program[i].p_vaddr + ElfFile->Program[i].p_filesz))
				{
					EndAddress = ElfFile->Program[i].p_vaddr + ElfFile->Program[i].p_filesz;
				}
			}
			break;
		}
	}
	R5900Regs.PC = ElfFile->Header.e_entry;
	EmuRec_Recompile(R5900Regs.PC, EndAddress);
	R5900Regs.PC = ElfFile->Header.e_entry;

#ifdef EMU_LOG
	EmuLog("Entry point at %.8X\n", R5900Regs.PC);
#endif

	return 32768;
}

void EmuClearStats()
{
	for (EMU_U32 i = 0; i <= TotalInstructions; i++)
	{
		EmuInstructionsStats[i].Index = i;
		EmuInstructionsStats[i].Total = 0;
		EmuRunningStats[i].Index = i;
		EmuRunningStats[i].Total = 0;

		InstrBreakPoints[i] = 0;
	}
}

void EmuGenStats(EMU_U32 Start, EMU_U32 End)
{
	EmuClearStats();

	EMU_U32 Index;
	for (EMU_U32 i = Start; i < End; i += 4)
	{
		Index = EmuInstructionIndex(EmuMemGetWord(i));

		EmuInstructionsStats[Index].Total++;
	}
}

DLLEXPORT void CALLBACK EmuAddBreakPoint(EMU_U32 Address)
{
	BreakPoints.push_back(Address);
}

DLLEXPORT void CALLBACK EmuRemoveBreakPoint(EMU_U32 Address)
{
	for (EMU_U32 i = 0; i < BreakPoints.size(); i++)
	{
		if (BreakPoints[i] == Address)
		{
			BreakPoints.erase(BreakPoints.begin() + i);
		}
	}
}

DLLEXPORT BOOL CALLBACK EmuIsBreakPoint(EMU_U32 Address)
{
	for (EMU_U32 i = 0; i < BreakPoints.size(); i++)
	{
		if (BreakPoints[i] == Address)
		{
			return TRUE;
		}
	}

	return FALSE;
}

DLLEXPORT void CALLBACK EmuStepOver(EMU_U32 tAddress)
{
	EmuStopRun = false;
	EmuInBranchDelay = false;
	EmuExecuteFast(tAddress, false);
	if (R5900Regs.PC != (tAddress + 4))
	{
		EmuAddBreakPoint(tAddress + 8);
		EmuRun(R5900Regs.PC);
		EmuRemoveBreakPoint(tAddress + 8);
	}
}

DLLEXPORT void CALLBACK EmuStepInto(EMU_U32 Address)
{
	EmuStopRun = false;
	EmuInBranchDelay = false;
	EmuRunDebug(Address, false);
}

DLLEXPORT void CALLBACK EmuRun(unsigned int Address)
{
	EmuStopRun = false;
	EmuRunDebug(Address, true);
	//EmuExecuteFast(Address, true);
	Emu_GS_CloseWindow();
}

DLLEXPORT void CALLBACK EmuExecute(unsigned int Address)
{
	EmuStopRun = false;
	//    EmuExecuteFast( Address, true );
	EmuRec_RecompileExecute(Address, EndAddress, TRUE);
	Emu_GS_CloseWindow();
}

DLLEXPORT void CALLBACK EmuLog(const char* Format, ...)
{
	if (fPointer)
	{
		va_list vaParams;

		va_start(vaParams, Format);
		vfprintf(fPointer, Format, vaParams);
		va_end(vaParams);
	}
}

DLLEXPORT void CALLBACK EmuAddInstructionBreakpoint(EMU_U32 InstIndex)
{
	InstrBreakPoints[InstIndex] = TRUE;
}

DLLEXPORT void CALLBACK EmuRemoveInstructionBreakpoint(EMU_U32 InstIndex)
{
	InstrBreakPoints[InstIndex] = FALSE;
}

DLLEXPORT BOOL CALLBACK EmuIsInstructionBreakpoint(EMU_U32 InstIndex)
{
	return InstrBreakPoints[InstIndex];
}

DLLEXPORT void CALLBACK EmuGetRunningStats(stEmuInstructionCount** stats)
{
	std::copy(EmuRunningStats.begin(), EmuRunningStats.end(), *stats);

	for (EMU_U32 i = 0; i < EmuRunningStats.size(); i++)
	{
		if (EmuRunningStats[i].Total)
		{
			EmuConsole("%-20s - %u\n",
				EmuInstructions[EmuRunningStats[i].Index].Name,
				EmuRunningStats[i].Total);
		}
	}
}

DLLEXPORT void CALLBACK EmuGetLoadedStats(stEmuInstructionCount** stats)
{
	std::copy(EmuInstructionsStats.begin(), EmuInstructionsStats.end(), *stats);
}

DLLEXPORT void CALLBACK EmuGetInstructionInfo(EMU_U32 InstIndex, char** Name, BOOL* IsDisassembled, BOOL* IsImplemented)
{
	*Name = EmuInstructions[InstIndex].Name;
	*IsDisassembled = (EmuInstructions[InstIndex].Disassembly && EmuInstructions[InstIndex].Disassembly != EmuDis_REGISTERED);
	*IsImplemented = EmuInstructions[InstIndex].Implemented;
}

DLLEXPORT void CALLBACK EmuSetConsoleCallback(EMUCONSOLECALLBACK function)
{
	EmuConsoleCallback = function;
}

DLLEXPORT void CALLBACK EmuGetRegs(Emu_R5900_Regs** r5900regs,
	Emu_COP0_Regs** cop0regs,
	Emu_COP1_Regs** cop1regs,
	EMU_VU_Regs** vu0regs,
	EMU_VU_Regs** vu1regs)
{
	*r5900regs = &R5900Regs;
	*cop0regs = &COP0Regs;
	*cop1regs = &COP1Regs;
	*vu0regs = &VU0Regs;
	*vu1regs = &VU1Regs;
}

DLLEXPORT void CALLBACK EmuConsole(const char* Format, ...)
{
	if (EmuConsoleCallback)
	{
		char Text[4096];
		va_list vaParams;

		va_start(vaParams, Format);
		vsprintf(Text, Format, vaParams);
		va_end(vaParams);
		EmuConsoleCallback(Text);
	}
}

DLLEXPORT EMU_U32 CALLBACK EmuGetTotalInstructions()
{
	return TotalInstructions;
}

DLLEXPORT void CALLBACK EmuSetByte(EMU_U32 Address, EMU_U08 Data)
{
	EmuMemSetByte(Address, Data);
}

DLLEXPORT void CALLBACK EmuSetShort(EMU_U32 Address, EMU_U16 Data)
{
	EmuMemSetShort(Address, Data);
}

DLLEXPORT void CALLBACK EmuSetWord(EMU_U32 Address, EMU_U32 Data)
{
	EmuMemSetWord(Address, Data);
}

DLLEXPORT void CALLBACK EmuSetDWord(EMU_U32 Address, EMU_U64 Data)
{
	EmuMemSetDWord(Address, Data);
}

DLLEXPORT EMU_U08 CALLBACK EmuGetByte(EMU_U32 Address)
{
	return EmuMemGetByte(Address);
}

DLLEXPORT EMU_U16 CALLBACK EmuGetShort(EMU_U32 Address)
{
	return EmuMemGetShort(Address);
}

DLLEXPORT EMU_U32 CALLBACK EmuGetWord(EMU_U32 Address)
{
	return EmuMemGetWord(Address);
}

DLLEXPORT EMU_U64 CALLBACK EmuGetDWord(EMU_U32 Address)
{
	return EmuMemGetDWord(Address);
}

DLLEXPORT void CALLBACK EmuGetInstructionsStats(EMU_U32* TotalSupportedInstructions, EMU_U32* TotalDisassembledInstructions, EMU_U32* TotalImplementedInstructions)
{
	*TotalSupportedInstructions = 0;
	*TotalDisassembledInstructions = 0;
	*TotalImplementedInstructions = 0;
	for (unsigned int i = 0; i < TotalInstructions; i++)
	{
		if (EmuInstructions[i].Disassembly != EmuDis_REGISTERED)
		{
			(*TotalSupportedInstructions)++;
		}
		if (EmuInstructions[i].Disassembly &&
			EmuInstructions[i].Disassembly != EmuDis_REGISTERED)
		{
			(*TotalDisassembledInstructions)++;
		}
		if (EmuInstructions[i].Implemented)
		{
			(*TotalImplementedInstructions)++;
		}
	}
}

DLLEXPORT void CALLBACK EmuGSInitWindow()
{
	Emu_GS_InitWindow();
}

DLLEXPORT void CALLBACK EmuGSConfigure()
{
	Emu_GS_Configure();
}

DLLEXPORT void CALLBACK EmuGSCloseWindow()
{
	Emu_GS_CloseWindow();
}

DLLEXPORT void CALLBACK EmuPADConfig()
{
	Emu_PAD_Config();
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

