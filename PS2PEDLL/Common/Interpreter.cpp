#include "Interpreter.h"
#include "Ps2Core.h"

#include <stdio.h>

namespace Interpreter
{
	Interpreter::Interpreter()
	{
	}

	Interpreter::~Interpreter()
	{
	}

	void Interpreter::ClearBreakPoints()
	{
		BreakPoints.clear();
	}

	void Interpreter::AddBreakPoint(EMU_U32 Address)
	{
		BreakPoints[Address] = true;
	}

	void Interpreter::RemoveBreakPoint(EMU_U32 Address)
	{
		BreakPoints.erase(Address);
	}

	bool Interpreter::IsBreakPoint(EMU_U32 Address)
	{
		return BreakPoints.contains(Address);
	}

	void Interpreter::ClearInstructionBreakpoints()
	{
		InstrBreakPoints.clear();
	}

	void Interpreter::AddInstructionBreakpoint(EMU_U32 InstIndex)
	{
		InstrBreakPoints[InstIndex] = true;
	}

	void Interpreter::RemoveInstructionBreakpoint(EMU_U32 InstIndex)
	{
		InstrBreakPoints.erase(InstIndex);
	}

	bool Interpreter::IsInstructionBreakpoint(EMU_U32 InstIndex)
	{
		return InstrBreakPoints.contains(InstIndex);
	}

	void Interpreter::StepOver(EMU_U32 tAddress)
	{
		EmuStopRun = false;
		EmuInBranchDelay = false;
		EmuExecuteFast(tAddress, false);
		if (PS2Regs.R5900Regs.PC != (tAddress + 4))
		{
			AddBreakPoint(tAddress + 8);
			EmuExecuteFast(PS2Regs.R5900Regs.PC, true);
			RemoveBreakPoint(tAddress + 8);
		}
	}

	void Interpreter::StepInto(EMU_U32 Address)
	{
		EmuStopRun = false;
		EmuInBranchDelay = false;
		EmuRunDebug(Address, false);
	}

	void Interpreter::ClearStats()
	{
		EMU_U32 totalInstructions = Common::Ps2Core::GetInstance().GetTotalInstructions();
		EmuRunningStats.clear();
		EmuRunningStats.resize(totalInstructions + 1);
		for (EMU_U32 i = 0; i <= totalInstructions; i++)
		{
			EmuRunningStats[i].Index = i;
			EmuRunningStats[i].Total = 0;
		}
	}

	void Interpreter::GetRunningStats(stEmuInstructionCount** stats)
	{
		std::copy(EmuRunningStats.begin(), EmuRunningStats.end(), *stats);

		for (EMU_U32 i = 0; i < EmuRunningStats.size(); i++)
		{
			if (EmuRunningStats[i].Total)
			{
				Common::Ps2Core::GetInstance().Console("%-20s - %u\n",
					EmuInstructions[EmuRunningStats[i].Index].Name,
					EmuRunningStats[i].Total);
			}
		}
	}
}