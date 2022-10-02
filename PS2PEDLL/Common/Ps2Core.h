#pragma once

#include "Interpreter.h"

#include "EmuPS2Core.h"
#include "Elf.h"

#include <vector>

namespace Common
{
	typedef void (*EMUCONSOLECALLBACK)(const char*);

	class Ps2Core final
	{
	public:
		void Initialize();
		void Reset();
		void Release();
		void SetBiosFile(const char* FileName);
		EMU_U32 InstructionIndex(EMU_U32 tInst);
		EMU_I32 Load(const char* FileName);
		void ClearStats();
		void GenStats(EMU_U32 Start, EMU_U32 End);
		void AddBreakPoint(EMU_U32 Address);
		void RemoveBreakPoint(EMU_U32 Address);
		bool IsBreakPoint(EMU_U32 Address);
		void StepOver(EMU_U32 tAddress);
		void StepInto(EMU_U32 Address);
		void Run(unsigned int Address);
		void Execute(unsigned int Address);
		void Log(const char* Format, ...);
		void AddInstructionBreakpoint(EMU_U32 InstIndex);
		void RemoveInstructionBreakpoint(EMU_U32 InstIndex);
		bool IsInstructionBreakpoint(EMU_U32 InstIndex);
		void GetRunningStats(stEmuInstructionCount** stats);
		void GetLoadedStats(stEmuInstructionCount** stats);
		void GetInstructionInfo(EMU_U32 InstIndex, char** Name, BOOL* IsDisassembled, BOOL* IsImplemented);
		void SetConsoleCallback(EMUCONSOLECALLBACK function);
		void GetRegs(Emu_R5900_Regs** r5900regs, Emu_COP0_Regs** cop0regs, Emu_COP1_Regs** cop1regs, EMU_VU_Regs** vu0regs, EMU_VU_Regs** vu1regs);
		void Console(const char* Format, ...);
		EMU_U32 GetTotalInstructions();
		void SetByte(EMU_U32 Address, EMU_U08 Data);
		void SetShort(EMU_U32 Address, EMU_U16 Data);
		void SetWord(EMU_U32 Address, EMU_U32 Data);
		void SetDWord(EMU_U32 Address, EMU_U64 Data);
		EMU_U08 GetByte(EMU_U32 Address);
		EMU_U16 GetShort(EMU_U32 Address);
		EMU_U32 GetWord(EMU_U32 Address);
		EMU_U64 GetDWord(EMU_U32 Address);
		void GetInstructionsStats(EMU_U32* TotalSupportedInstructions, EMU_U32* TotalDisassembledInstructions, EMU_U32* TotalImplementedInstructions);
		bool FindLabel(const char* Label, EMU_U32* LabelIndex);
		const char* DisassemblySymbol(EMU_U32 Address);                           
		void GSInitWindow();
		void GSConfigure();
		void GSCloseWindow();
		void PADConfig();
		void GetCOP0RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
		void GetCOP1RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
		void GetCOP2FPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
		void GetCOP2IPRegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);
		void GetR5900RegName(EMU_U32 Reg, char* Buffer, EMU_U32 BufferSize);

		const char* GetBiosFileName();
		const char* GetGSFileName();
		const char* GetPADFileName();
		const char* GetSPUFileName();

		void SetBiosFileName(const char* fileName);
		void SetGSFileName(const char* fileName);
		void SetPADFileName(const char* fileName);
		void SetSPUFileName(const char* fileName);

		inline double GetCPUClock() { return CPUClock; }

		inline void StopRun(bool stopRun) { theInterpreter.StopRun(stopRun); }

		static Ps2Core& GetInstance();

		Ps2Core(const Ps2Core&) = delete;
		Ps2Core(Ps2Core&&) = delete;
		Ps2Core& operator=(const Ps2Core&) = delete;
		Ps2Core& operator=(Ps2Core&&) = delete;

	private:
		Ps2Core();

		EMU_U32 TotalInstructions = 0;

		double CPUClock;

		Elf32_File* ElfFile;

		bool EmuRunning = false;
		bool EmuInBranchDelay = false;

		std::vector<stEmuInstructionCount> EmuInstructionsStats;

		FILE* fPointer = NULL;

		char BiosFileName[1024];
		char GSFileName[1024];
		char PADFileName[1024];
		char SPUFileName[1024];

		EMUCONSOLECALLBACK EmuConsoleCallback;

		EMU_U32 StartAddress;
		EMU_U32 EndAddress;

		Interpreter::Interpreter theInterpreter;

		EMU_PS2_Regs PS2Regs;
	};
}