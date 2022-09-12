#include "EmuExecute.h"

void EmuSpecial(EMU_U32 Code)
{
	///////////////////////////////////
	// Special Functions
	///////////////////////////////////
	switch (Code & 0x0000003F)
	{
	case 0x00000000: // "sll"
		TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 << R_SA;
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)TempU32;
		break;

	case 0x00000002: // "srl"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> R_SA;
		break;

	case 0x00000003: // "sra"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].i32_00_31 >> R_SA);
		break;

	case 0x00000004: // "sllv"
		TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 << (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x1F);
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)TempU32;
		break;

	case 0x00000006: // "srlv"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x1F);
		break;

	case 0x00000007: // "srav"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].i32_00_31 >>
			(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x1F));
		break;

	case 0x00000008: // "jr"
		// It does not zero extend the address if it
		// crosses the 32bit bondary
		JumpTo = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31;
		EXECUTE_BRANCH_DELAY_SLOT(PS2Regs.R5900Regs.PC);
		PS2Regs.R5900Regs.PC = JumpTo;
		break;

	case 0x00000009: // "jalr"
		// It does not zero extend the address if it
		// crosses the 32bit bondary
		if (R_RD)
		{
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.PC + 4;
		}
		JumpTo = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31;
		EXECUTE_BRANCH_DELAY_SLOT(PS2Regs.R5900Regs.PC);
		PS2Regs.R5900Regs.PC = JumpTo;
		break;

	case 0x0000000A: // "movz"
		if (PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 == 0)
		{
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
		}
		break;

	case 0x0000000B: // "movn"
		if (PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 != 0)
		{
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
		}
		break;

	case 0x0000000C: // "syscall"
		Emu_Bios_Syscall();
		break;

	case 0x0000000D: // "break"
	case 0x0000000F: // "sync"
		break;

	case 0x00000010: // "mfhi"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.HI.u64_00_63;
		break;

	case 0x00000011: // "mthi"
		PS2Regs.R5900Regs.HI.u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
		break;

	case 0x00000012: // "mflo"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
		break;

	case 0x00000013: // "mtlo"
		PS2Regs.R5900Regs.LO.u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
		break;

	case 0x00000014: // "dsllv"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 <<
			(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x3F);
		break;

	case 0x00000016: // "dsrlv"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 >>
			(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x3F);
		break;

	case 0x00000017: // "dsrav"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 >>
			(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x3F);
		break;

	case 0x00000018: // "mult"
		Temp128.i64_00_63 = (EMU_I64)PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
		PS2Regs.R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
		PS2Regs.R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
		if (R_RD)
		{
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
		}
		break;

	case 0x00000019: // "multu"
		Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
		PS2Regs.R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
		PS2Regs.R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
		if (R_RD)
		{
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
		}
		break;

	case 0x0000001A: // "div"
		if (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31)
		{
			PS2Regs.R5900Regs.LO.i64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 / PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
			PS2Regs.R5900Regs.HI.i64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 % PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
		}
		break;

	case 0x0000001B: // "divu"
		if (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31)
		{
			PS2Regs.R5900Regs.LO.i64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 / PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
			PS2Regs.R5900Regs.HI.i64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 % PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
		}
		break;

	case 0x00000020: // "add"
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
		PS2Regs.R5900Regs.Reg[R_RD].i64_00_63 = Temp128.i32_00_31;
		break;

	case 0x00000021: // "addu"
		Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
		PS2Regs.R5900Regs.Reg[R_RD].i64_00_63 = Temp128.i32_00_31;
		break;

	case 0x00000022: // "sub"
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 - (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
		PS2Regs.R5900Regs.Reg[R_RD].i64_00_63 = Temp128.i32_00_31;
		break;

	case 0x00000023: // "subu"
		Temp128.u64_00_63 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 - (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
		PS2Regs.R5900Regs.Reg[R_RD].i64_00_63 = Temp128.i32_00_31;
		break;

	case 0x00000024: // "and"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 &
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x00000025: // "or"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 |
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x00000026: // "xor"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 ^
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x00000027: // "nor"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = ~(PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 |
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63);
		break;

	case 0x00000028: // "mfsa"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.SA;
		break;

	case 0x00000029: // "mtsa"
		PS2Regs.R5900Regs.SA = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
		break;

	case 0x0000002A: // "slt"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 < PS2Regs.R5900Regs.Reg[R_RT].i64_00_63;
		break;

	case 0x0000002B: // "sltu"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 < PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x0000002C: // "dadd"
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 +
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x0000002D: // "daddu"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 +
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x0000002E: // "dsub"
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 -
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x0000002F: // "dsubu"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 -
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
		break;

	case 0x00000030: // "tge"
	case 0x00000031: // "tgeu"
	case 0x00000032: // "tlt"
	case 0x00000033: // "tltu"
	case 0x00000034: // "teq"
	case 0x00000036: // "tne"
		break;

	case 0x00000038: // "dsll"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 << R_SA;
		break;

	case 0x0000003A: // "dsrl"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 >> R_SA;
		break;

	case 0x0000003B: // "dsra"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 >> R_SA;
		break;

	case 0x0000003C: // "dsll32"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 << (R_SA + 32);
		break;

	case 0x0000003E: // "dsrl32"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 >> (R_SA + 32);
		break;

	case 0x0000003F: // "dsra32"
		PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].i64_00_63 >> (R_SA + 32);
		break;
	}
	///////////////////////////////////
	// End of Special Functions
	///////////////////////////////////
}