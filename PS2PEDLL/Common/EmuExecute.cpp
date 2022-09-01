
#include "EmuExecute.h"

stEmuTemp emuExecuteStatus;

EMU_U32 EmuInstructionIndex2(EMU_U32 tInst);

void emuExecute(EMU_U32 tAddress, bool InLoop);
void emuCore(EMU_U32 Code, stEmuTemp& emuExecuteStatus);

void EmuExecuteFast(EMU_U32 tAddress, bool InLoop)
{
#undef EMUFUNCTION
#define EMUFUNCTION  EmuExecuteFast
#undef EMU_STATS
	emuExecute(tAddress, InLoop);
}

void EmuRunDebug(EMU_U32 tAddress, bool InLoop)
{
#undef EMUFUNCTION
#define EMUFUNCTION  EmuRunDebug
#define EMU_STATS
	emuExecute(tAddress, InLoop);
}

void emuExecute(EMU_U32 tAddress, bool InLoop)
{
	R5900Regs.PC = tAddress;

	register EMU_U32 Code;

	while (!EmuStopRun)
	{
		if ((R5900Regs.R0.u64_00_63 != 0) || (R5900Regs.R0.u64_64_127 != 0))
		{
			EmuConsole("Register R0 is not zero.");
		}
		// If the PC is FFFFFFFF then we are exiting from an interrupt handler
		if (R5900Regs.PC == 0xFFFFFFFF)
		{
			Emu_Intc_ExitHandler();
		}
		else if (R5900Regs.PC == 0xFFFFFFFE)
		{
			Emu_Dma_ExitHandler();
		}

		// Checking for nop, skipping if no operation

		Code = EmuMemGetWord(R5900Regs.PC);
		R5900Regs.PC += 4;
		if (Code)
		{
#ifdef EMU_STATS
			EmuRunningStats[EmuInstructionIndex2(Code)].Total++;
#endif
			/////////////////////////////////////////////////////////////////////////
			// Main Loop
			/////////////////////////////////////////////////////////////////////////
			emuCore(Code, emuExecuteStatus);

			/////////////////////////////////////////////////////////////////////////
			// End of Main Loop
			/////////////////////////////////////////////////////////////////////////
		}
		COP0Regs.Count++;
		emuExecuteStatus.CpuCycles++;

		// No reason for FF, just not to call the function always, this way
		// we call it every 256 instructions excutions
		if (!(emuExecuteStatus.CpuCycles & 0xFF))
		{
			Emu_GS_ProcessMessages();
		}

		if (!InLoop)
		{
			break;
		}

#ifdef EMU_STATS
		if ((InstrBreakPoints[EmuInstructionIndex2(EmuMemGetWord(R5900Regs.PC))]) ||
			(EmuIsBreakPoint(R5900Regs.PC)))
		{
			break;
		}
#endif
	}
}
