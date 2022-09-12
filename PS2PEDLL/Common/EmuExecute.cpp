
#include "EmuExecute.h"

EMU_U32 EmuFloatStatus;
EMU_U32 EmuIntegerStatus;

EMU_U32 RS;
EMU_U32 RD;
EMU_U32 RT;
EMU_U32 FS;
EMU_U32 FT;
EMU_U32 SA;
EMU_U32 Address;
EMU_I32 TempI32;
EMU_U32 TempU32;
EMU_I64 TempI64;
EMU_U64 TempU64;
EMU_128 Temp128;
EMU_U32 RegRTU32;
EMU_U64 RegRTU64;
EMU_F32 TempF32;
const EMU_U16 MaxU16 = 0xFFFF;
const EMU_U32 MaxU32 = 0xFFFFFFFF;
const EMU_U64 MaxU64 = 0xFFFFFFFFFFFFFFFF;
EMU_U32 JumpTo;
bool enableStat = false;
bool enableDebug = false;

EMU_U64 CpuCycles = 0;

EMU_U32 clamp(EMU_U32 X)
{
	if (X > 0x00007FFF)
	{
		return 0x7FFF;
	}
	else if (X < 0xFFFF8000)
	{
		return 0x8000;
	}
	else
	{
		return X;
	}
}

EMU_U32 EmuInstructionIndex2(EMU_U32 tInst);

void EmuRunLoop(EMU_U32 tAddress, bool InLoop);

void EmuExecuteFast(EMU_U32 tAddress, bool InLoop)
{
	enableStat = false;
	enableDebug = false;
	EmuRunLoop(tAddress, InLoop);
}

void EmuRunDebug(EMU_U32 tAddress, bool InLoop)
{
	enableStat = true;
	enableDebug = true;
	EmuRunLoop(tAddress, InLoop);
}

void EmuRunLoop(EMU_U32 tAddress, bool InLoop)
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
			if (enableStat)
			{
				EmuRunningStats[EmuInstructionIndex2(Code)].Total++;
			}

			/////////////////////////////////////////////////////////////////////////
			// Main Loop
			/////////////////////////////////////////////////////////////////////////

			EmuCore(Code);

			/////////////////////////////////////////////////////////////////////////
			// End of Main Loop
			/////////////////////////////////////////////////////////////////////////
		}
		COP0Regs.Count++;
		CpuCycles++;

		// No reason for FF, just not to call the function always, this way
		// we call it every 256 instructions excutions
		if (!(CpuCycles & 0xFF))
		{
			Emu_GS_ProcessMessages();
		}

		if (!InLoop)
		{
			break;
		}

		if (enableDebug && (InstrBreakPoints[EmuInstructionIndex2(EmuMemGetWord(R5900Regs.PC))]) || (EmuIsBreakPoint(R5900Regs.PC)))
		{
			break;
		}
	}
}