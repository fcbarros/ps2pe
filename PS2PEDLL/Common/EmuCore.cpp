#include "EmuExecute.h"

void emuSpecial(EMU_U32 Code, stEmuTemp& emuTemp);
void emuRegimm(EMU_U32 Code, stEmuTemp& emuTemp);
void emuCop0(EMU_U32 Code, stEmuTemp& emuTemp);
void emuCop1(EMU_U32 Code, stEmuTemp& emuTemp);
void emuCop2(EMU_U32 Code, stEmuTemp& emuTemp);
void emuMmi(EMU_U32 Code, stEmuTemp& emuTemp);

void emuCore(EMU_U32 Code, stEmuTemp& emuTemp)
{
	/////////////////////////////////////////////////////////////////////////
	// Core Instructions
	/////////////////////////////////////////////////////////////////////////
	EMU_U32 SA{ 0 };
	EMU_U32 JumpTo{ 0 };

	switch (Code & 0xFC000000)
	{
	case 0x00000000: // SPECIAL
		emuSpecial(Code, emuTemp);
		break;

	case 0x04000000: // REGIMM
		emuRegimm(Code, emuTemp);
		break;

	case 0x08000000: // "j"
		JumpTo = JUMP_ADDRESS;
		EXECUTE_BRANCH_DELAY_SLOT(R5900Regs.PC);
		R5900Regs.PC = JumpTo;
		break;

	case 0x0C000000: // "jal"
		// It does not zero extend the address if it
		// crosses the 32bit bondary
		R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
		JumpTo = JUMP_ADDRESS;
		EXECUTE_BRANCH_DELAY_SLOT(R5900Regs.PC);
		R5900Regs.PC = JumpTo;
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
		emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)(EMU_I16)R_IMMEDIATE;
		R5900Regs.Reg[R_RT].i64_00_63 = emuTemp.Temp128.i32_00_31;
		break;

	case 0x24000000: // "addiu"
		emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)(EMU_I16)R_IMMEDIATE;
		R5900Regs.Reg[R_RT].i64_00_63 = emuTemp.Temp128.i32_00_31;
		break;

	case 0x28000000: // "slti"
		R5900Regs.Reg[R_RT].u64_00_63 = (R5900Regs.Reg[R_RS].i64_00_63 < (EMU_I64)(EMU_I16)R_IMMEDIATE);
		break;

	case 0x2C000000: // "sltiu"
		R5900Regs.Reg[R_RT].u64_00_63 = (R5900Regs.Reg[R_RS].u64_00_63 < (EMU_U64)(EMU_I16)R_IMMEDIATE);
		break;

	case 0x30000000: // "andi"
		R5900Regs.Reg[R_RT].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 & R_IMMEDIATE;
		break;

	case 0x34000000: // "ori"
		R5900Regs.Reg[R_RT].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 | R_IMMEDIATE;
		break;

	case 0x38000000: // "xori"
		R5900Regs.Reg[R_RT].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 ^ R_IMMEDIATE;
		break;

	case 0x3C000000: // "lui"
		R5900Regs.Reg[R_RT].i64_00_63 = (EMU_I64)(R_IMMEDIATE << 16);
		break;

	case 0x40000000: // "COP0"
		emuCop0(Code, emuTemp);
		break;

	case 0x44000000: // "COP1"
		emuCop1(Code, emuTemp);
		break;

	case 0x48000000: // "COP2"
		emuCop2(Code, emuTemp);
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
		R5900Regs.Reg[R_RT].i64_00_63 = R5900Regs.Reg[R_RS].i64_00_63 + (EMU_I16)R_IMMEDIATE;
		break;

	case 0x64000000: // "daddiu"
		R5900Regs.Reg[R_RT].i64_00_63 = R5900Regs.Reg[R_RS].i64_00_63 + (EMU_I16)R_IMMEDIATE;
		break;

	case 0x68000000: // "ldl"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x07;
		emuTemp.TempU64 = EmuMemGetDWord(emuTemp.Address & 0xFFFFFFF8);
		R5900Regs.Reg[R_RT].u64_00_63 = (R5900Regs.Reg[R_RT].u64_00_63 & (0x00FFFFFFFFFFFFFF >> (SA << 3))) |
			(emuTemp.TempU64 << ((7 - SA) << 3));
		break;

	case 0x6C000000: // "ldr"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x07;
		emuTemp.TempU64 = EmuMemGetDWord(emuTemp.Address & 0xFFFFFFF8);
		R5900Regs.Reg[R_RT].u64_00_63 = (R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFF00 << ((7 - SA) << 3))) |
			(emuTemp.TempU64 >> (SA << 3));
		break;

	case 0x70000000: // MMI
		emuMmi(Code, emuTemp);
		break;

	case 0x78000000: // "lq"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		R5900Regs.Reg[R_RT].u64_00_63 = EmuMemGetDWord(emuTemp.Address);
		R5900Regs.Reg[R_RT].u64_64_127 = EmuMemGetDWord(emuTemp.Address + 8);
		break;

	case 0x7C000000: // "sq"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		EmuMemSetDWord(emuTemp.Address, R5900Regs.Reg[R_RT].u64_00_63);
		EmuMemSetDWord(emuTemp.Address + 8, R5900Regs.Reg[R_RT].u64_64_127);
		break;

	case 0x80000000: // "lb"
		R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I08)
			EmuMemGetByte(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0x84000000: // "lh"
		R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I16)
			EmuMemGetShort(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0x88000000: // "lwl"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x03;
		emuTemp.TempU32 = EmuMemGetWord(emuTemp.Address - 3) & (0xFFFFFFFF << ((3 - SA) << 3));
		R5900Regs.Reg[R_RT].u32_00_31 &= 0xFFFFFFFF >> ((SA + 1) << 3);
		R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RT].u32_00_31 | emuTemp.TempU32);
		break;

	case 0x8C000000: // "lw"
		R5900Regs.Reg[R_RT].i64_00_63 = (EMU_I32)
			EmuMemGetWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0x90000000: // "lbu"
		R5900Regs.Reg[R_RT].u64_00_63 =
			EmuMemGetByte(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0x94000000: // "lhu"
		R5900Regs.Reg[R_RT].u64_00_63 =
			EmuMemGetShort(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0x98000000: // "lwr"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		emuTemp.TempU32 = EmuMemGetWord(emuTemp.Address);
		SA = emuTemp.Address & 0x03;
		if (SA)
		{
			emuTemp.TempU32 &= 0xFFFFFFFF >> (SA << 3);
			R5900Regs.Reg[R_RT].u32_00_31 &= 0xFFFFFFFFFFFFFFFF << ((4 - SA) << 3);
			R5900Regs.Reg[R_RT].u32_00_31 |= emuTemp.TempU32;
		}
		else
		{
			R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I32)emuTemp.TempU32;
		}
		break;

	case 0x9C000000: // "lwu"
		R5900Regs.Reg[R_RT].u64_00_63 = EmuMemGetWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0xA0000000: // "sb"
		EmuMemSetByte(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE,
			R5900Regs.Reg[R_RT].u08_00_07);
		break;

	case 0xA4000000: // "sh"
		EmuMemSetShort(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE,
			R5900Regs.Reg[R_RT].u16_00_15);
		break;

	case 0xA8000000: // "swl"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x3;
		emuTemp.TempU32 = (EmuMemGetWord(emuTemp.Address - 3) &
			(0xFFFFFFFF >> ((SA + 1) << 3))) |
			(R5900Regs.Reg[R_RT].u32_00_31 & (0xFFFFFFFF << ((3 - SA) << 3)));
		EmuMemSetWord(emuTemp.Address - 3, emuTemp.TempU32);
		break;

	case 0xAC000000: // "sw"
		EmuMemSetWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE,
			R5900Regs.Reg[R_RT].u32_00_31);
		break;

	case 0xB0000000: // "sdl"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x07;
		emuTemp.TempU64 = (EmuMemGetDWord(emuTemp.Address - 7) & (0xFFFFFFFFFFFFFFFF >> (SA << 3))) |
			(R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3)));
		EmuMemSetDWord(emuTemp.Address - 7, emuTemp.TempU64);
		break;

	case 0xB4000000: // "sdr"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x07;
		emuTemp.TempU64 = (EmuMemGetDWord(emuTemp.Address) &
			(0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3))) |
			(R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF >> (SA << 3)));
		EmuMemSetDWord(emuTemp.Address, emuTemp.TempU64);
		break;

	case 0xB8000000: // "swr"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = emuTemp.Address & 0x3;
		emuTemp.TempU32 = (EmuMemGetWord(emuTemp.Address) & (0xFFFFFFFF << ((4 - SA) << 3))) |
			(R5900Regs.Reg[R_RT].u32_00_31 & (0xFFFFFFFF >> (SA << 3)));
		EmuMemSetWord(emuTemp.Address, emuTemp.TempU32);
		break;

	case 0xBC000000: // "cache"
		break;

	case 0xC4000000: // "lwc1"      
		emuTemp.TempU32 = EmuMemGetWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		COP1Regs.Reg[R_RT].f = *(float*)&emuTemp.TempU32;
		break;

	case 0xCC000000: // "pref"      
		break;

	case 0xD8000000: // "lqc2"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		VU0Regs.CPR[R_RT].X.u = EmuMemGetWord(emuTemp.Address);
		VU0Regs.CPR[R_RT].Y.u = EmuMemGetWord(emuTemp.Address + 4);
		VU0Regs.CPR[R_RT].Z.u = EmuMemGetWord(emuTemp.Address + 8);
		VU0Regs.CPR[R_RT].W.u = EmuMemGetWord(emuTemp.Address + 12);
		break;

	case 0xDC000000: // "ld"
		R5900Regs.Reg[R_RT].u64_00_63 =
			EmuMemGetDWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE);
		break;

	case 0xE4000000: // "swc1"
		EmuMemSetWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE,
			COP1Regs.Reg[R_FT].u);
		break;

	case 0xF8000000: // "sqc2"
		emuTemp.Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		EmuMemSetWord(emuTemp.Address, VU0Regs.CPR[R_RT].X.u);
		EmuMemSetWord(emuTemp.Address + 4, VU0Regs.CPR[R_RT].Y.u);
		EmuMemSetWord(emuTemp.Address + 8, VU0Regs.CPR[R_RT].Z.u);
		EmuMemSetWord(emuTemp.Address + 12, VU0Regs.CPR[R_RT].W.u);
		break;

	case 0xFC000000: // "sd"
		EmuMemSetDWord(R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE,
			R5900Regs.Reg[R_RT].u64_00_63);
		break;
	}

	/////////////////////////////////////////////////////////////////////////
	// End of Core Instructions
	/////////////////////////////////////////////////////////////////////////
}