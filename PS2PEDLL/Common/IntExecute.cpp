#include "Interpreter.h"

#include "Intc.h"
#include "Dma.h"
#include "Gs.h"
#include "Memory.h"

using namespace Common;

namespace Interpreter
{
	void Interpreter::EmuExecuteFast(EMU_U32 tAddress, bool InLoop)
	{
		enableStat = false;
		enableDebug = false;
		EmuRunLoop(tAddress, InLoop);
	}

	void Interpreter::EmuRunDebug(EMU_U32 tAddress, bool InLoop)
	{
		enableStat = true;
		enableDebug = true;
		EmuRunLoop(tAddress, InLoop);
	}

	void Interpreter::EmuRunLoop(EMU_U32 tAddress, bool InLoop)
	{
		PS2Regs.R5900Regs.PC = tAddress;

		while (!EmuStopRun)
		{
			if ((PS2Regs.R5900Regs.R0.u64_00_63 != 0) || (PS2Regs.R5900Regs.R0.u64_64_127 != 0))
			{
				EmuConsole2("Register R0 is not zero.");
			}
			// If the PC is FFFFFFFF then we are exiting from an interrupt handler
			if (PS2Regs.R5900Regs.PC == 0xFFFFFFFF)
			{
				Intc::GetInstance().ExitHandler();
			}
			else if (PS2Regs.R5900Regs.PC == 0xFFFFFFFE)
			{
				Dma::GetInstance().ExitHandler();
			}

			// Checking for nop, skipping if no operation

			OpCode = Memory::GetInstance().GetWord(PS2Regs.R5900Regs.PC);
			PS2Regs.R5900Regs.PC += 4;
			if (OpCode)
			{
				if (enableStat)
				{
					EmuRunningStats[EmuInstructionIndex3(OpCode)].Total++;
				}

				EmuCore();
			}
			PS2Regs.COP0Regs.Count++;
			CpuCycles++;

			// No reason for FF, just not to call the function always, this way
			// we call it every 256 instructions excutions
			if (!(CpuCycles & 0xFF))
			{
				Gs::GetInstance().ProcessMessages();
			}

			if (!InLoop)
			{
				break;
			}

			if (enableDebug && (InstrBreakPoints[EmuInstructionIndex3(Memory::GetInstance().GetWord(PS2Regs.R5900Regs.PC))]) || (EmuIsBreakPoint2(PS2Regs.R5900Regs.PC)))
			{
				break;
			}
		}
	}
}