#pragma once

#include "Vu.h"

#include "EmuPS2Core.h"
#include "EmuDisassembly.h"

#include <cfenv>
#include <vector>

namespace Interpreter
{
    struct stEmuInstructionCount
    {
        EMU_U32 Index;
        EMU_U32 Total;
    };

	class Interpreter
	{
	public:
		Interpreter();
		~Interpreter();

		void EmuExecuteFast(EMU_U32 tAddress, bool InLoop);
		void EmuRunDebug(EMU_U32 tAddress, bool InLoop);		

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
            if ((bool)std::fetestexcept(FE_OVERFLOW))
            {
                PS2Regs.COP1Regs.FCR31_O = 1;
                PS2Regs.COP1Regs.FCR31_SO = 1;
                PS2Regs.COP1Regs.FCR31_U = 0;
            }
            else if ((bool)std::fetestexcept(FE_UNDERFLOW))
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

		void EmuRunLoop(EMU_U32 tAddress, bool InLoop);

		void EmuCore();
		void EmuCop0();
		void EmuCop1();
		void EmuCop2();
		void EmuMmi();
		void EmuRegimm();
		void EmuSpecial();

        // TODO: Remove
        EMU_PS2_Regs PS2Regs;
        Common::EMU_VU_Regs VU0Regs;
        Common::EMU_VU_Regs VU1Regs;
        bool EmuRunning = false;
        bool EmuStopRun = false;
        bool EmuInBranchDelay = false;
        std::vector<stEmuInstructionCount> EmuRunningStats;
        std::vector<EMU_U32>   InstrBreakPoints;
        std::vector<EMU_U32>   BreakPoints;

        void EmuConsole2(const char* Format, ...);
        bool EmuIsBreakPoint2(EMU_U32 Address);
        EMU_U32 EmuInstructionIndex3(EMU_U32 tInst);
        //

		static const EMU_U16 MaxU16 = 0xFFFF;
		static const EMU_U32 MaxU32 = 0xFFFFFFFF;
		static const EMU_U64 MaxU64 = 0xFFFFFFFFFFFFFFFF;

		EMU_U32 OpCode;

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
		EMU_U32 JumpTo;
		bool enableStat = false;
		bool enableDebug = false;
		EMU_U64 CpuCycles = 0;
	};

#define EMUFUNCTION     EmuRunDebug
    //#define EMUFUNCTION     EmuExecuteFast

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

//#define EMU_FLOAT_LOAD_CONDITION_REGISTER( a )      __asm FSTSW a
//#define EMU_FLOAT_CHECK_OVERFLOW( a )               (( a ) & 0x0008)
//#define EMU_FLOAT_CHECK_UNDERFLOW( a )              (( a ) & 0x0010)

#define EMU_INTEGER_LOAD_CONDITION_REGISTER( a )    __asm LAHF; __asm mov a, eax;
#define EMU_INTEGER_CHECK_OVERFLOW( a )             (( a ) & 0x0800)
#define EMU_INTEGER_CHECK_CARRY( a )                (( a ) & 0x0001)

#define EXECUTE_BRANCH_DELAY_SLOT( addr )                                           \
        EmuInBranchDelay = true;                                                    \
        EMUFUNCTION( addr, false );                                                 \
        EmuInBranchDelay = false;

#define EXECUTE_BRANCH( addr )                                                      \
        JumpTo = PS2Regs.R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);                         \
        EXECUTE_BRANCH_DELAY_SLOT( addr );                                          \
        PS2Regs.R5900Regs.PC = JumpTo;

#define BRANCH_CONDITION_RS_RT( cond )                                              \
        if ( PS2Regs.R5900Regs.Reg[ R_RS ].i64_00_63 cond PS2Regs.R5900Regs.Reg[ R_RT ].i64_00_63 ) \
        {                                                                           \
            EXECUTE_BRANCH( PS2Regs.R5900Regs.PC );                                         \
        }

#define BRANCH_CONDITION_RS_RT_LIKELY( cond )                                       \
        if ( PS2Regs.R5900Regs.Reg[ R_RS ].i64_00_63 cond PS2Regs.R5900Regs.Reg[ R_RT ].i64_00_63 ) \
        {                                                                           \
            EXECUTE_BRANCH( PS2Regs.R5900Regs.PC );                                         \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            PS2Regs.R5900Regs.PC += 4;                                                      \
        }

#define BRANCH_CONDITION_RS_ZERO( cond )                                            \
        if ( PS2Regs.R5900Regs.Reg[ R_RS ].i64_00_63 cond 0 )                               \
        {                                                                           \
            EXECUTE_BRANCH( PS2Regs.R5900Regs.PC );                                         \
        }

#define BRANCH_CONDITION_RS_ZERO_LIKELY( cond )                                     \
        if ( PS2Regs.R5900Regs.Reg[ R_RS ].i64_00_63 cond 0 )                               \
        {                                                                           \
            EXECUTE_BRANCH( PS2Regs.R5900Regs.PC );                                         \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            PS2Regs.R5900Regs.PC += 4;                                                      \
        }

#define BRANCH_CONDITION_RS_ZERO_LINK( cond )                                       \
        if ( PS2Regs.R5900Regs.Reg[ R_RS ].i64_00_63 cond 0 )                               \
        {                                                                           \
            PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;                              \
            EXECUTE_BRANCH( PS2Regs.R5900Regs.PC );                                         \
        }

#define BRANCH_CONDITION_RS_ZERO_LINK_LIKELY( cond )                                \
        if ( PS2Regs.R5900Regs.Reg[ R_RS ].i64_00_63 cond 0 )                               \
        {                                                                           \
            PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;                              \
            EXECUTE_BRANCH( PS2Regs.R5900Regs.PC );                                         \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            PS2Regs.R5900Regs.PC += 4;                                                      \
        }

}