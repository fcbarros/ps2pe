#pragma once

#include "Ps2Core.h"

#include "Memory.h"
#include "Bios.h"
#include "Dma.h"
#include "Gif.h"
#include "Gs.h"
#include "Pad.h"
#include "Vif.h"
#include "Timer.h"
#include "Ipu.h"
#include "Fifo.h"
#include "Sif.h"
#include "Intc.h"
#include "Vu.h"
#include "Thread.h"

#include <time.h>

namespace Common
{
	Ps2Core::Ps2Core()
	{
	}

	Ps2Core& Ps2Core::GetInstance()
	{
		static Ps2Core ps2Core;
		return ps2Core;
	}

	void Ps2Core::Initialize()
	{
		Memory::GetInstance().Init(32 * 1024 * 1024, 64 * 1024);

		CPUClock = EmuGetClock();

		EmuLoadConfig();
		EmuLoadPlugins();

		EmuConsoleCallback = NULL;

		ElfFile = CreateElf();

#ifdef EMU_LOG
		time_t CurTime = time(NULL);

		EmuLog("PS2PE - PS2 Personal Emulator v0.01\n\n");
		EmuLog("Log: %s\n", ctime(&CurTime));
		EmuLog("CPU Clock: %.2f Mhz\n\n", CPUClock / 1000000.0f);
		EmuLog("========================================================\n");
		EmuLog("Initialing emulator\n");
#endif

		int i = 0;
		while (strcmp(EmuInstructions[i].Name, "UNKNOW"))
		{
			i++;
		}
		TotalInstructions = i;

		ClearStats();

		Memory::GetInstance().Clear();

		theInterpreter.ClearBreakPoints();
		theInterpreter.ClearInstructionBreakpoints();

		Bios::GetInstance().Init();
		Dma::GetInstance().Init();
		Gif::GetInstance().Init();
		Gs::GetInstance().Init();
		Pad::GetInstance().Init();
		Vif::GetInstance().Init();
		Timer::GetInstance().Init();
		Ipu::GetInstance().Init();
		Fifo::GetInstance().Init();
		Sif::GetInstance().Init();
		Intc::GetInstance().Init();
		Vu::GetInstance().Init();
		Thread::GetInstance().Init();

		//EmuRec_Init(16 * 1024 * 1024);

		EmuReset();
	}

	void Ps2Core::Reset()
	{
#ifdef EMU_LOG
		EmuLog("Reseting emulator\n");
#endif

		ClearStats();

		memset(&PS2Regs.R5900Regs, 0, sizeof(PS2Regs.R5900Regs));
		memset(&PS2Regs.COP0Regs, 0, sizeof(PS2Regs.COP0Regs));
		memset(&PS2Regs.COP1Regs, 0, sizeof(PS2Regs.COP1Regs));

		PS2Regs.R5900Regs.SP.u64_00_63 = (EMU_I32)0x81F00000;
		PS2Regs.R5900Regs.GP.u64_00_63 = (EMU_I32)0x81F80000;

		PS2Regs.COP0Regs.Status = 0x10900000;
		PS2Regs.COP0Regs.PRId_REV = 0x20;
		PS2Regs.COP0Regs.PRId_IMP = 0x2E;

		PS2Regs.COP1Regs.FCR0_REV = 0x00;
		PS2Regs.COP1Regs.FCR0_IMP = 0x2E;
		PS2Regs.COP1Regs.FCR31_00 = 1;
		PS2Regs.COP1Regs.FCR31_24 = 1;

		EmuRunning = false;

		Intc::GetInstance().ClearInterruptIndex();

		PS2Regs.R5900Regs.PC = ElfFile->Header.e_entry;

		//EmuRec_Reset();
	}

	void Ps2Core::SetBiosFile(const char* FileName)
	{
#ifdef EMU_LOG
		EmuLog("Setting Bios File: %s\n", FileName);
#endif
		strcpy(BiosFileName, FileName);
	}

	// Releases memory assossiated with file
	void Ps2Core::Release()
	{
#ifdef EMU_LOG
		EmuLog("Exiting emulator\n");
#endif
		DestroyElf(ElfFile);

		if (fPointer)
		{
			fclose(fPointer);
		}

		Gs::GetInstance().Shutdown();

		//EmuRec_Shutdown();

		Memory::GetInstance().Shutdown();
	}

	EMU_U32 Ps2Core::InstructionIndex(EMU_U32 tInst)
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

	EMU_I32 Ps2Core::Load(const char* FileName)
	{
		DestroyElf(ElfFile);
		ElfFile = CreateElf();

		if (!ReadElf(FileName, ElfFile))
		{
			return 0;
		}
		if ((ElfFile->Header.e_type != ET_EXEC) || (ElfFile->Header.e_machine != EM_MIPS))
		{
			return 0;
		}

#ifdef EMU_LOG
		EmuLog("Reading ELF %s\n", FileName);
#endif

		Memory::GetInstance().Clear();

		Bios::GetInstance().Init();
		Dma::GetInstance().Init();
		Gif::GetInstance().Init();
		Gs::GetInstance().Init();
		Pad::GetInstance().Init();
		Vif::GetInstance().Init();
		Timer::GetInstance().Init();
		Ipu::GetInstance().Init();
		Fifo::GetInstance().Init();
		Sif::GetInstance().Init();
		Intc::GetInstance().Init();
		Vu::GetInstance().Init();
		Thread::GetInstance().Init();

		EmuReset();

		theInterpreter.ClearBreakPoints();

		if (strlen(BiosFileName))
		{
			Bios::GetInstance().Load(BiosFileName);
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
					Memory::GetInstance().AddFromFile(FileName, ElfFile->Program[i].p_offset, ElfFile->Program[i].p_filesz, ElfFile->Program[i].p_vaddr, Memory::EMMP_READ | Memory::EMMP_WRITE | Memory::EMMP_EXEC);
					GenStats(ElfFile->Program[i].p_vaddr, ElfFile->Program[i].p_vaddr + ElfFile->Program[i].p_filesz);
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
		PS2Regs.R5900Regs.PC = ElfFile->Header.e_entry;
		//EmuRec_Recompile(PS2Regs.R5900Regs.PC, EndAddress);
		//PS2Regs.R5900Regs.PC = ElfFile->Header.e_entry;

#ifdef EMU_LOG
		EmuLog("Entry point at %.8X\n", PS2Regs.R5900Regs.PC);
#endif

		return 32768;
	}

	void Ps2Core::ClearStats()
	{
		EmuInstructionsStats.clear();
		EmuInstructionsStats.resize(TotalInstructions + 1);
		for (EMU_U32 i = 0; i <= TotalInstructions; i++)
		{
			EmuInstructionsStats[i].Index = i;
			EmuInstructionsStats[i].Total = 0;
		}
		theInterpreter.ClearStats();
	}

	void Ps2Core::GenStats(EMU_U32 Start, EMU_U32 End)
	{
		ClearStats();

		EMU_U32 Index;
		for (EMU_U32 i = Start; i < End; i += 4)
		{
			Index = InstructionIndex(Memory::GetInstance().GetWord(i));

			EmuInstructionsStats[Index].Total++;
		}
	}

	void Ps2Core::AddBreakPoint(EMU_U32 Address)
	{
		theInterpreter.AddBreakPoint(Address);
	}

	void Ps2Core::RemoveBreakPoint(EMU_U32 Address)
	{
		theInterpreter.RemoveBreakPoint(Address);
	}

	bool Ps2Core::IsBreakPoint(EMU_U32 Address)
	{
		return theInterpreter.IsBreakPoint(Address);
	}

	void Ps2Core::StepOver(EMU_U32 tAddress)
	{
		theInterpreter.StepOver(tAddress);
	}

	void Ps2Core::StepInto(EMU_U32 Address)
	{
		theInterpreter.StepInto(Address);
	}

	void Ps2Core::Run(unsigned int Address)
	{
		theInterpreter.StopRun(false);
		//EmuRunDebug(Address, true);
		//EmuExecuteFast(Address, true);

		theInterpreter.EmuExecuteFast(Address, true);

		Gs::GetInstance().CloseWindow();
	}

	void Ps2Core::Execute(unsigned int Address)
	{
		theInterpreter.StopRun(false);
		//EmuRec_RecompileExecute(Address, EndAddress, TRUE);

		Gs::GetInstance().CloseWindow();
	}

	void Ps2Core::Log(const char* Format, ...)
	{
		if (fPointer)
		{
			va_list vaParams;

			va_start(vaParams, Format);
			vfprintf(fPointer, Format, vaParams);
			va_end(vaParams);
		}
	}

	void Ps2Core::AddInstructionBreakpoint(EMU_U32 InstIndex)
	{
		theInterpreter.AddInstructionBreakpoint(InstIndex);
	}

	void Ps2Core::RemoveInstructionBreakpoint(EMU_U32 InstIndex)
	{
		theInterpreter.RemoveInstructionBreakpoint(InstIndex);
	}

	bool Ps2Core::IsInstructionBreakpoint(EMU_U32 InstIndex)
	{
		return theInterpreter.IsInstructionBreakpoint(InstIndex);
	}

	void Ps2Core::GetRunningStats(stEmuInstructionCount** stats)
	{
		theInterpreter.GetRunningStats(stats);
	}

	void Ps2Core::GetLoadedStats(stEmuInstructionCount** stats)
	{
		std::copy(EmuInstructionsStats.begin(), EmuInstructionsStats.end(), *stats);
	}

	void Ps2Core::GetInstructionInfo(EMU_U32 InstIndex, char** Name, BOOL* IsDisassembled, BOOL* IsImplemented)
	{
		*Name = EmuInstructions[InstIndex].Name;
		*IsDisassembled = (EmuInstructions[InstIndex].Disassembly && EmuInstructions[InstIndex].Disassembly != EmuDis_REGISTERED);
		*IsImplemented = EmuInstructions[InstIndex].Implemented;
	}

	void Ps2Core::SetConsoleCallback(EMUCONSOLECALLBACK function)
	{
		EmuConsoleCallback = function;
	}

	void Ps2Core::GetRegs(Emu_R5900_Regs** r5900regs, Emu_COP0_Regs** cop0regs, Emu_COP1_Regs** cop1regs, EMU_VU_Regs** vu0regs, EMU_VU_Regs** vu1regs)
	{
		*r5900regs = &PS2Regs.R5900Regs;
		*cop0regs = &PS2Regs.COP0Regs;
		*cop1regs = &PS2Regs.COP1Regs;
		*vu0regs = &PS2Regs.VU0Regs;
		*vu1regs = &PS2Regs.VU1Regs;
	}

	void Ps2Core::Console(const char* Format, ...)
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

	EMU_U32 Ps2Core::GetTotalInstructions()
	{
		return TotalInstructions;
	}

	void Ps2Core::SetByte(EMU_U32 Address, EMU_U08 Data)
	{
		Memory::GetInstance().SetByte(Address, Data);
	}

	void Ps2Core::SetShort(EMU_U32 Address, EMU_U16 Data)
	{
		Memory::GetInstance().SetShort(Address, Data);
	}

	void Ps2Core::SetWord(EMU_U32 Address, EMU_U32 Data)
	{
		Memory::GetInstance().SetWord(Address, Data);
	}

	void Ps2Core::SetDWord(EMU_U32 Address, EMU_U64 Data)
	{
		Memory::GetInstance().SetDWord(Address, Data);
	}

	EMU_U08 Ps2Core::GetByte(EMU_U32 Address)
	{
		return Memory::GetInstance().GetByte(Address);
	}

	EMU_U16 Ps2Core::GetShort(EMU_U32 Address)
	{
		return Memory::GetInstance().GetShort(Address);
	}

	EMU_U32 Ps2Core::GetWord(EMU_U32 Address)
	{
		return Memory::GetInstance().GetWord(Address);
	}

	EMU_U64 Ps2Core::GetDWord(EMU_U32 Address)
	{
		return Memory::GetInstance().GetDWord(Address);
	}

	void Ps2Core::GetInstructionsStats(EMU_U32* TotalSupportedInstructions, EMU_U32* TotalDisassembledInstructions, EMU_U32* TotalImplementedInstructions)
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

	bool Ps2Core::FindLabel(const char* Label, EMU_U32* LabelIndex)
	{
		if (ElfFile->Symbol_num > 0)
		{
			for (EMU_U32 i = 0; i < ElfFile->Symbol_num; i++)
			{
				if (!strncmp(&ElfFile->SymbolSTable[ElfFile->Symbol[i].st_name], Label, strlen(Label)))
				{
					*LabelIndex = ElfFile->Symbol[i].st_value;
					return TRUE;
				}
			}
		}
		else if (ElfFile->Header.e_shnum > 0)
		{
			for (int i = 0; i < ElfFile->Header.e_shnum; i++)
			{
				if (!strncmp(&ElfFile->SessionSTable[ElfFile->Session[i].sh_name], Label, strlen(Label)))
				{
					*LabelIndex = ElfFile->Session[i].sh_addr;
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	const char* Ps2Core::DisassemblySymbol(EMU_U32 Address)
	{
		static char Symbol_str[256];
		memset(Symbol_str, 0, sizeof(Symbol_str));
		if ((ElfFile->Symbol_num == 0) &&
			(ElfFile->Header.e_shnum == 0) &&
			(Address == ElfFile->Header.e_entry))
		{
			strcpy_s(Symbol_str, "ENTRY_POINT");
		}
		else if (ElfFile->Symbol_num > 0)
		{
			for (EMU_U32 i = 0; i < ElfFile->Symbol_num; i++)
			{
				if (ElfFile->Symbol[i].st_value == Address)
				{
					if (ElfFile->Symbol[i].st_name > 0)
					{
						strcpy_s(Symbol_str, &ElfFile->SymbolSTable[ElfFile->Symbol[i].st_name]);
						strcat_s(Symbol_str, ":");
					}
					else
					{
						strcpy_s(Symbol_str, &ElfFile->SessionSTable[ElfFile->Session[ElfFile->Symbol[i].st_shndx].sh_name]);
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < ElfFile->Header.e_shnum; i++)
			{
				if (ElfFile->Session[i].sh_addr == Address)
				{
					if (ElfFile->Session[i].sh_name > 0)
					{
						strcpy_s(Symbol_str, &ElfFile->SessionSTable[ElfFile->Session[i].sh_name]);
					}
					break;
				}
			}
		}

		return Symbol_str;
	}

	void Ps2Core::GSInitWindow()
	{
		Gs::GetInstance().InitWindow();
	}

	void Ps2Core::GSConfigure()
	{
		Gs::GetInstance().Configure();
	}

	void Ps2Core::GSCloseWindow()
	{
		Gs::GetInstance().CloseWindow();
	}

	void Ps2Core::PADConfig()
	{
		Pad::GetInstance().Config();
	}

	void Ps2Core::GetCOP0RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
	{
		strncpy(Buffer, EmuDis_GetCOP0RegName(Reg), BufferSize);
	}

	void Ps2Core::GetCOP1RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
	{
		strncpy(Buffer, EmuDis_GetCOP1RegName(Reg), BufferSize);
	}

	void Ps2Core::GetCOP2FPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
	{
		strncpy(Buffer, EmuDis_GetCOP2FPRegName(Reg), BufferSize);
	}

	void Ps2Core::GetCOP2IPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
	{
		strncpy(Buffer, EmuDis_GetCOP2IPRegName(Reg), BufferSize);
	}

	void Ps2Core::GetR5900RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize)
	{
		strncpy(Buffer, EmuDis_GetRegName(Reg), BufferSize);
	}

	const char* Ps2Core::GetBiosFileName()
	{
		return BiosFileName;
	}

	const char* Ps2Core::GetGSFileName()
	{
		return GSFileName;
	}

	const char* Ps2Core::GetPADFileName()
	{
		return PADFileName;
	}

	const char* Ps2Core::GetSPUFileName()
	{
		return SPUFileName;
	}

	void Ps2Core::SetBiosFileName(const char* fileName)
	{
		strcpy(BiosFileName, fileName);
	}

	void Ps2Core::SetGSFileName(const char* fileName)
	{
		strcpy(GSFileName, fileName);
	}

	void Ps2Core::SetPADFileName(const char* fileName)
	{
		strcpy(PADFileName, fileName);
	}

	void Ps2Core::SetSPUFileName(const char* fileName)
	{
		strcpy(SPUFileName, fileName);
	}
}