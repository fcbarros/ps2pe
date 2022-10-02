#pragma once

#include "Vu.h"

#include "EmuPS2Core.h"
#include "EmuDisassembly.h"

#include <intrin.h>
#include <cfenv>
#include <vector>
#include <map>

namespace Interpreter
{
	//#define EMUFUNCTION     EmuRunDebug
#define EMUFUNCTION     EmuExecuteFast

#define R_RD            ( ( OpCode >> 11 ) & 0x1F )
#define R_RT            ( ( OpCode >> 16 ) & 0x1F )
#define R_RS            ( ( OpCode >> 21 ) & 0x1F )
#define R_SA            ( ( OpCode >> 06 ) & 0x1F )

#define R_FD            ( ( OpCode >> 06 ) & 0x1F )
#define R_FS            ( ( OpCode >> 11 ) & 0x1F )
#define R_FT            ( ( OpCode >> 16 ) & 0x1F )

#define R_BRANCH        (   OpCode & 0xFFFF )
#define R_IMMEDIATE     (   OpCode & 0xFFFF )

#define VU_DEST_X       ( ( OpCode >> 24 ) & 0x01 )
#define VU_DEST_Y       ( ( OpCode >> 23 ) & 0x01 )
#define VU_DEST_Z       ( ( OpCode >> 22 ) & 0x01 )
#define VU_DEST_W       ( ( OpCode >> 21 ) & 0x01 )
#define VU_FT           ( ( OpCode >> 16 ) & 0x1F )
#define VU_FS           ( ( OpCode >> 11 ) & 0x1F )
#define VU_FD           ( ( OpCode >> 06 ) & 0x1F )
#define VU_BC           (   OpCode & 0x03 )
#define VU_FTF          ( ( OpCode >> 23 ) & 0x03 )
#define VU_FSF          ( ( OpCode >> 21 ) & 0x03 )
#define VU_IT           ( ( OpCode >> 16 ) & 0x1F )
#define VU_IS           ( ( OpCode >> 11 ) & 0x1F )
#define VU_IMM5         ( ( OpCode >> 06 ) & 0x1F )
#define VU_IMM15        ( ( OpCode >> 06 ) & 0x7FFF )
#define VU_RT           ( ( OpCode >> 16 ) & 0x1F )
#define VU_ID           ( ( OpCode >> 11 ) & 0x1F )
#define VU_BASE         ( ( OpCode >> 21 ) & 0x1F )
#define VU_OFFSET       (   OpCode & 0xFFFF )

#define JUMP_ADDRESS    ( ( OpCode & 0x03FFFFFF ) << 2 ) | ( PS2Regs.R5900Regs.PC & 0xF0000000 )

	class Interpreter final
	{
	public:
		Interpreter();
		~Interpreter();

		void EmuExecuteFast(EMU_U32 tAddress, bool InLoop);
		void EmuRunDebug(EMU_U32 tAddress, bool InLoop);

		void ClearBreakPoints();
		void AddBreakPoint(EMU_U32 Address);
		void RemoveBreakPoint(EMU_U32 Address);
		bool IsBreakPoint(EMU_U32 Address);

		void ClearInstructionBreakpoints();
		void AddInstructionBreakpoint(EMU_U32 InstIndex);
		void RemoveInstructionBreakpoint(EMU_U32 InstIndex);
		bool IsInstructionBreakpoint(EMU_U32 InstIndex);

		inline void StopRun(bool stopRun) { EmuStopRun = stopRun; }
		void StepOver(EMU_U32 tAddress);
		void StepInto(EMU_U32 Address);

		void ClearStats();
		void GetRunningStats(stEmuInstructionCount** stats);

	private:
		static inline EMU_U32 clamp(EMU_U32 X)
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

		inline void SetFloatFlags()
		{
			if (std::fetestexcept(FE_OVERFLOW))
			{
				PS2Regs.COP1Regs.FCR31_O = 1;
				PS2Regs.COP1Regs.FCR31_SO = 1;
				PS2Regs.COP1Regs.FCR31_U = 0;
			}
			else if (std::fetestexcept(FE_UNDERFLOW))
			{
				PS2Regs.COP1Regs.FCR31_O = 0;
				PS2Regs.COP1Regs.FCR31_U = 1;
				PS2Regs.COP1Regs.FCR31_SU = 1;
			}
			else
			{
				PS2Regs.COP1Regs.FCR31_O = 0;
				PS2Regs.COP1Regs.FCR31_U = 0;
			}
		}

		inline void SetIntegerFlags()
		{
			unsigned int flags = __readeflags();
			PS2Regs.R5900Regs.Overflow = flags & 0x800;
			PS2Regs.R5900Regs.Carry = flags & 0x1;
		}

		inline void ExecuteBranch(EMU_U32 addr)
		{
			JumpTo = PS2Regs.R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EmuInBranchDelay = true;
			EMUFUNCTION(addr, false);
			EmuInBranchDelay = false;
			PS2Regs.R5900Regs.PC = JumpTo;
		}

		inline void ExecuteBranchDelaySlot(EMU_U32 addr)
		{
			EmuInBranchDelay = true;
			EMUFUNCTION(addr, false);
			EmuInBranchDelay = false;
		}

		void EmuRunLoop(EMU_U32 tAddress, bool InLoop);

		void EmuCore();
		void EmuCop0();
		void EmuCop1();
		void EmuCop2();
		void EmuMmi();
		void EmuRegimm();
		void EmuSpecial();

		std::map<EMU_U32, bool> InstrBreakPoints;
		std::map<EMU_U32, bool> BreakPoints;
		std::vector<stEmuInstructionCount> EmuRunningStats;
		bool EmuRunning = false;
		bool EmuStopRun = false;
		bool EmuInBranchDelay = false;

		static const EMU_U16 MaxU16 = 0xFFFF;
		static const EMU_U32 MaxU32 = 0xFFFFFFFF;
		static const EMU_U64 MaxU64 = 0xFFFFFFFFFFFFFFFF;

		EMU_U32 OpCode;

		EMU_U32 EmuIntegerStatus;

		EMU_U32 SA;
		EMU_U32 Address;
		EMU_I32 TempI32;
		EMU_U32 TempU32;
		EMU_I64 TempI64;
		EMU_U64 TempU64;
		EMU_128 Temp128;
		EMU_F32 TempF32;
		EMU_U32 JumpTo;
		bool enableStat = false;
		bool enableDebug = false;
		EMU_U64 CpuCycles = 0;
	};
}