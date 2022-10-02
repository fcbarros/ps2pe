#include "Interpreter.h"

#include "Intc.h"
#include "Dma.h"
#include "Gs.h"
#include "Memory.h"

using namespace Common;

namespace Interpreter
{
	void Interpreter::EmuCore()
	{
		/////////////////////////////////////////////////////////////////////////
		// Core Instructions
		/////////////////////////////////////////////////////////////////////////

		switch (OpCode & 0xFC000000)
		{
		case 0x00000000: // SPECIAL
			EmuSpecial();
			break;

		case 0x04000000: // REGIMM
			EmuRegimm();
			break;

		case 0x08000000: // "j"
			JumpTo = JUMP_ADDRESS;
			ExecuteBranchDelaySlot(PS2Regs.R5900Regs.PC);
			PS2Regs.R5900Regs.PC = JumpTo;
			break;

		case 0x0C000000: // "jal"
			// It does not zero extend the address if it
			// crosses the 32bit bondary
			PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;
			JumpTo = JUMP_ADDRESS;
			ExecuteBranchDelaySlot(PS2Regs.R5900Regs.PC);
			PS2Regs.R5900Regs.PC = JumpTo;
			break;

		case 0x10000000: // "beq"
			BRANCH_CONDITION_RS_RT(== );
			break;

		case 0x14000000: // "bne"
			BRANCH_CONDITION_RS_RT(!= );
			break;

		case 0x18000000: // "blez"
			BRANCH_CONDITION_RS_ZERO(<= );
			break;

		case 0x1C000000: // "bgtz"
			BRANCH_CONDITION_RS_ZERO(> );
			break;

		case 0x20000000: // "addi"
			// ******************************************
			// Not doing the OVERFLOW TRAP
			// ******************************************
			Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)(EMU_I16)R_IMMEDIATE;
			PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 = Temp128.i32_00_31;
			break;

		case 0x24000000: // "addiu"
			Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)(EMU_I16)R_IMMEDIATE;
			PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 = Temp128.i32_00_31;
			break;

		case 0x28000000: // "slti"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 < (EMU_I64)(EMU_I16)R_IMMEDIATE);
			break;

		case 0x2C000000: // "sltiu"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 < (EMU_U64)(EMU_I16)R_IMMEDIATE);
			break;

		case 0x30000000: // "andi"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 & R_IMMEDIATE;
			break;

		case 0x34000000: // "ori"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 | R_IMMEDIATE;
			break;

		case 0x38000000: // "xori"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 ^ R_IMMEDIATE;
			break;

		case 0x3C000000: // "lui"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I32)(R_IMMEDIATE << 16);
			break;

		case 0x40000000: // "COP0"
			EmuCop0();
			break;

		case 0x44000000: // "COP1"
			EmuCop1();
			break;

		case 0x48000000: // "COP2"
			EmuCop2();
			break;

		case 0x50000000: // "beql"
			BRANCH_CONDITION_RS_RT_LIKELY(== );
			break;

		case 0x54000000: // "bnel"
			BRANCH_CONDITION_RS_RT_LIKELY(!= );
			break;

		case 0x58000000: // "blezl"
			BRANCH_CONDITION_RS_ZERO_LIKELY(<= );
			break;

		case 0x5C000000: // "bgtzl"
			BRANCH_CONDITION_RS_ZERO_LIKELY(> );
			break;

		case 0x60000000: // "daddi"
			// ******************************************
			// Not doing the OVERFLOW TRAP
			// ******************************************
			PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 + (EMU_I16)R_IMMEDIATE;
			break;

		case 0x64000000: // "daddiu"
			PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 + (EMU_I16)R_IMMEDIATE;
			break;

		case 0x68000000: // "ldl"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x07;
			TempU64 = Memory::GetInstance().GetDWord(Address - 7) & (0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3));
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF >> (SA << 3))) | TempU64;
			break;

		case 0x6C000000: // "ldr"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x07;
			TempU64 = Memory::GetInstance().GetDWord(Address) & (0xFFFFFFFFFFFFFFFF >> (SA << 3));
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3))) | TempU64;
			break;

		case 0x70000000: // MMI
			EmuMmi();
			break;

		case 0x78000000: // "lq"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = Memory::GetInstance().GetDWord(Address);
			PS2Regs.R5900Regs.Reg[R_RT].u64_64_127 = Memory::GetInstance().GetDWord(Address + 8);
			break;

		case 0x7C000000: // "sq"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			Memory::GetInstance().SetDWord(Address, PS2Regs.R5900Regs.Reg[R_RT].u64_00_63);
			Memory::GetInstance().SetDWord(Address + 8, PS2Regs.R5900Regs.Reg[R_RT].u64_64_127);
			break;

		case 0x80000000: // "lb"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I08)Memory::GetInstance().GetByte(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0x84000000: // "lh"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I16)Memory::GetInstance().GetShort(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0x88000000: // "lwl"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x03;
			TempU32 = Memory::GetInstance().GetWord(Address - 3) & (0xFFFFFFFF << ((3 - SA) << 3));
			PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 &= 0xFFFFFFFF >> ((SA + 1) << 3);
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 | TempU32);
			break;

		case 0x8C000000: // "lw"
			PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 = (EMU_I32)Memory::GetInstance().GetWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0x90000000: // "lbu"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = Memory::GetInstance().GetByte(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0x94000000: // "lhu"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = Memory::GetInstance().GetShort(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0x98000000: // "lwr"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			TempU32 = Memory::GetInstance().GetWord(Address);
			SA = Address & 0x03;
			if (SA)
			{
				TempU32 &= 0xFFFFFFFF >> (SA << 3);
				PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 &= 0xFFFFFFFFFFFFFFFF << ((4 - SA) << 3);
				PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 |= TempU32;
			}
			else
			{
				PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I32)TempU32;
			}
			break;

		case 0x9C000000: // "lwu"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = Memory::GetInstance().GetWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0xA0000000: // "sb"
			Memory::GetInstance().SetByte(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE, PS2Regs.R5900Regs.Reg[R_RT].u08_00_07);
			break;

		case 0xA4000000: // "sh"
			Memory::GetInstance().SetShort(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE, PS2Regs.R5900Regs.Reg[R_RT].u16_00_15);
			break;

		case 0xA8000000: // "swl"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x3;
			TempU32 = (Memory::GetInstance().GetWord(Address - 3) & (0xFFFFFFFF >> ((SA + 1) << 3))) | (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 & (0xFFFFFFFF << ((3 - SA) << 3)));
			Memory::GetInstance().SetWord(Address - 3, TempU32);
			break;

		case 0xAC000000: // "sw"
			Memory::GetInstance().SetWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE, PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
			break;

		case 0xB0000000: // "sdl"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x07;
			TempU64 = (Memory::GetInstance().GetDWord(Address - 7) & (0xFFFFFFFFFFFFFFFF >> (SA << 3))) |
				(PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3)));
			Memory::GetInstance().SetDWord(Address - 7, TempU64);
			break;

		case 0xB4000000: // "sdr"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x07;
			TempU64 = (Memory::GetInstance().GetDWord(Address) &
				(0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3))) |
				(PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF >> (SA << 3)));
			Memory::GetInstance().SetDWord(Address, TempU64);
			break;

		case 0xB8000000: // "swr"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			SA = Address & 0x3;
			TempU32 = (Memory::GetInstance().GetWord(Address) & (0xFFFFFFFF << ((4 - SA) << 3))) |
				(PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 & (0xFFFFFFFF >> (SA << 3)));
			Memory::GetInstance().SetWord(Address, TempU32);
			break;

		case 0xBC000000: // "cache"
			break;

		case 0xC4000000: // "lwc1"      
			TempU32 = Memory::GetInstance().GetWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			PS2Regs.COP1Regs.Reg[R_RT].f = *(float*)&TempU32;
			break;

		case 0xCC000000: // "pref"      
			break;

		case 0xD8000000: // "lqc2"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			PS2Regs.VU0Regs.CPR[R_RT].X.u = Memory::GetInstance().GetWord(Address);
			PS2Regs.VU0Regs.CPR[R_RT].Y.u = Memory::GetInstance().GetWord(Address + 4);
			PS2Regs.VU0Regs.CPR[R_RT].Z.u = Memory::GetInstance().GetWord(Address + 8);
			PS2Regs.VU0Regs.CPR[R_RT].W.u = Memory::GetInstance().GetWord(Address + 12);
			break;

		case 0xDC000000: // "ld"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 =
				Memory::GetInstance().GetDWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
			break;

		case 0xE4000000: // "swc1"
			Memory::GetInstance().SetWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE, PS2Regs.COP1Regs.Reg[R_FT].u);
			break;

		case 0xF8000000: // "sqc2"
			Address = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
			Memory::GetInstance().SetWord(Address, PS2Regs.VU0Regs.CPR[R_RT].X.u);
			Memory::GetInstance().SetWord(Address + 4, PS2Regs.VU0Regs.CPR[R_RT].Y.u);
			Memory::GetInstance().SetWord(Address + 8, PS2Regs.VU0Regs.CPR[R_RT].Z.u);
			Memory::GetInstance().SetWord(Address + 12, PS2Regs.VU0Regs.CPR[R_RT].W.u);
			break;

		case 0xFC000000: // "sd"
			Memory::GetInstance().SetDWord(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE,
				PS2Regs.R5900Regs.Reg[R_RT].u64_00_63);
			break;
		}
		/////////////////////////////////////////////////////////////////////////
		// End of Core Instructions
		/////////////////////////////////////////////////////////////////////////
	}
}