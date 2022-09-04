#include "EmuExecute.h"

void EmuMmi(EMU_U32 Code)
{
	///////////////////////////////////
	// MMI Funcions
	///////////////////////////////////
	switch (Code & 0x0000003F)
	{
	case 0x00000000: // "madd"
		Temp128.u64_00_63 = (EMU_I64)(((EMU_I64)R5900Regs.HI.u32_00_31 << 32) | (EMU_I64)R5900Regs.LO.u32_00_31) +
			((EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31);
		R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
		R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
		RD = R_RD;
		if (RD)
		{
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.u64_00_63;
		}
		break;

	case 0x00000001: // "maddu"
		Temp128.u64_00_63 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31) +
			((EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31);
		R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
		R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
		if (R_RD)
		{
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
		}
		break;

	case 0x00000004: // "plzcw"
		R5900Regs.Reg[R_RD].u32_00_31 = 0;
		R5900Regs.Reg[R_RD].u32_32_63 = 0;
		TempU32 = R5900Regs.Reg[R_RS].u32_00_31;
		while ((TempU32 & 0x80000000) ==
			(R5900Regs.Reg[R_RS].u32_00_31 & 0x80000000))
		{
			R5900Regs.Reg[R_RD].u32_00_31++;
			TempU32 <<= 1;
		}
		TempU32 = R5900Regs.Reg[R_RS].u32_32_63;
		while ((TempU32 & 0x80000000) ==
			(R5900Regs.Reg[R_RS].u32_32_63 & 0x80000000))
		{
			R5900Regs.Reg[R_RD].u32_32_63++;
			TempU32 <<= 1;
		}
		R5900Regs.Reg[R_RD].u32_00_31--;
		R5900Regs.Reg[R_RD].u32_32_63--;
		break;

	case 0x00000008: // MMI0
		///////////////////////////////////
		// MMI0 Funcions
		///////////////////////////////////
		switch (Code & 0x000007C0)
		{
		case 0x00000000: // "paddw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RS].u32_00_31 + R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RS].u32_32_63 + R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RS].u32_64_95 + R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RS].u32_96_127 + R5900Regs.Reg[RT].u32_96_127;
			break;

		case 0x00000040: // "psubw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RS].u32_00_31 - R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RS].u32_32_63 - R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RS].u32_64_95 - R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RS].u32_96_127 - R5900Regs.Reg[RT].u32_96_127;
			break;

		case 0x00000080: // "pcgtw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = (R5900Regs.Reg[RS].u32_00_31 > R5900Regs.Reg[RT].u32_00_31) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[RD].u32_32_63 = (R5900Regs.Reg[RS].u32_32_63 > R5900Regs.Reg[RT].u32_32_63) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[RD].u32_64_95 = (R5900Regs.Reg[RS].u32_64_95 > R5900Regs.Reg[RT].u32_64_95) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[RD].u32_96_127 = (R5900Regs.Reg[RS].u32_96_127 > R5900Regs.Reg[RT].u32_96_127) ? 0xFFFFFFFF : 0;
			break;

		case 0x000000C0: // "pmaxw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = (R5900Regs.Reg[RS].i32_00_31 > R5900Regs.Reg[RT].i32_00_31) ?
				R5900Regs.Reg[RS].u32_00_31 : R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = (R5900Regs.Reg[RS].i32_32_63 > R5900Regs.Reg[RT].i32_32_63) ?
				R5900Regs.Reg[RS].u32_32_63 : R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_64_95 = (R5900Regs.Reg[RS].i32_64_95 > R5900Regs.Reg[RT].i32_64_95) ?
				R5900Regs.Reg[RS].u32_64_95 : R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_96_127 = (R5900Regs.Reg[RS].i32_96_127 > R5900Regs.Reg[RT].i32_96_127) ?
				R5900Regs.Reg[RS].u32_96_127 : R5900Regs.Reg[RT].u32_96_127;
			break;

		case 0x00000100: // "paddh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RS].u16_00_15 + R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_16_31 + R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RS].u16_32_47 + R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_48_63 + R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RS].u16_64_79 + R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_80_95 + R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RS].u16_96_111 + R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_112_127 + R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x00000140: // "psubh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RS].u16_00_15 - R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_16_31 - R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RS].u16_32_47 - R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_48_63 - R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RS].u16_64_79 - R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_80_95 - R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RS].u16_96_111 - R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_112_127 - R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x00000180: // "pcgth"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = (R5900Regs.Reg[RS].u16_00_15 > R5900Regs.Reg[RT].u16_00_15) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_16_31 = (R5900Regs.Reg[RS].u16_16_31 > R5900Regs.Reg[RT].u16_16_31) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_32_47 = (R5900Regs.Reg[RS].u16_32_47 > R5900Regs.Reg[RT].u16_32_47) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_48_63 = (R5900Regs.Reg[RS].u16_48_63 > R5900Regs.Reg[RT].u16_48_63) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_64_79 = (R5900Regs.Reg[RS].u16_64_79 > R5900Regs.Reg[RT].u16_64_79) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_80_95 = (R5900Regs.Reg[RS].u16_80_95 > R5900Regs.Reg[RT].u16_80_95) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_96_111 = (R5900Regs.Reg[RS].u16_96_111 > R5900Regs.Reg[RT].u16_96_111) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_112_127 = (R5900Regs.Reg[RS].u16_112_127 > R5900Regs.Reg[RT].u16_112_127) ? 0xFFFF : 0;
			break;

		case 0x000001C0: // "pmaxh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = (R5900Regs.Reg[RS].i16_00_15 > R5900Regs.Reg[RT].i16_00_15) ?
				R5900Regs.Reg[RS].u16_00_15 : R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = (R5900Regs.Reg[RS].i16_16_31 > R5900Regs.Reg[RT].i16_16_31) ?
				R5900Regs.Reg[RS].u16_16_31 : R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_32_47 = (R5900Regs.Reg[RS].i16_32_47 > R5900Regs.Reg[RT].i16_32_47) ?
				R5900Regs.Reg[RS].u16_32_47 : R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_48_63 = (R5900Regs.Reg[RS].i16_48_63 > R5900Regs.Reg[RT].i16_48_63) ?
				R5900Regs.Reg[RS].u16_48_63 : R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = (R5900Regs.Reg[RS].i16_64_79 > R5900Regs.Reg[RT].i16_64_79) ?
				R5900Regs.Reg[RS].u16_64_79 : R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = (R5900Regs.Reg[RS].i16_80_95 > R5900Regs.Reg[RT].i16_80_95) ?
				R5900Regs.Reg[RS].u16_80_95 : R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_96_111 = (R5900Regs.Reg[RS].i16_96_111 > R5900Regs.Reg[RT].i16_96_111) ?
				R5900Regs.Reg[RS].u16_96_111 : R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_112_127 = (R5900Regs.Reg[RS].i16_112_127 > R5900Regs.Reg[RT].i16_112_127) ?
				R5900Regs.Reg[RS].u16_112_127 : R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x00000200: // "paddb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = R5900Regs.Reg[RS].u08_00_07 + R5900Regs.Reg[RT].u08_00_07;
			R5900Regs.Reg[RD].u08_08_15 = R5900Regs.Reg[RS].u08_08_15 + R5900Regs.Reg[RT].u08_08_15;
			R5900Regs.Reg[RD].u08_16_23 = R5900Regs.Reg[RS].u08_16_23 + R5900Regs.Reg[RT].u08_16_23;
			R5900Regs.Reg[RD].u08_24_31 = R5900Regs.Reg[RS].u08_24_31 + R5900Regs.Reg[RT].u08_24_31;
			R5900Regs.Reg[RD].u08_32_39 = R5900Regs.Reg[RS].u08_32_39 + R5900Regs.Reg[RT].u08_32_39;
			R5900Regs.Reg[RD].u08_40_47 = R5900Regs.Reg[RS].u08_40_47 + R5900Regs.Reg[RT].u08_40_47;
			R5900Regs.Reg[RD].u08_48_55 = R5900Regs.Reg[RS].u08_48_55 + R5900Regs.Reg[RT].u08_48_55;
			R5900Regs.Reg[RD].u08_56_63 = R5900Regs.Reg[RS].u08_56_63 + R5900Regs.Reg[RT].u08_56_63;
			R5900Regs.Reg[RD].u08_64_71 = R5900Regs.Reg[RS].u08_64_71 + R5900Regs.Reg[RT].u08_64_71;
			R5900Regs.Reg[RD].u08_72_79 = R5900Regs.Reg[RS].u08_72_79 + R5900Regs.Reg[RT].u08_72_79;
			R5900Regs.Reg[RD].u08_80_87 = R5900Regs.Reg[RS].u08_80_87 + R5900Regs.Reg[RT].u08_80_87;
			R5900Regs.Reg[RD].u08_88_95 = R5900Regs.Reg[RS].u08_88_95 + R5900Regs.Reg[RT].u08_88_95;
			R5900Regs.Reg[RD].u08_96_103 = R5900Regs.Reg[RS].u08_96_103 + R5900Regs.Reg[RT].u08_96_103;
			R5900Regs.Reg[RD].u08_104_111 = R5900Regs.Reg[RS].u08_104_111 + R5900Regs.Reg[RT].u08_104_111;
			R5900Regs.Reg[RD].u08_112_119 = R5900Regs.Reg[RS].u08_112_119 + R5900Regs.Reg[RT].u08_112_119;
			R5900Regs.Reg[RD].u08_120_127 = R5900Regs.Reg[RS].u08_120_127 + R5900Regs.Reg[RT].u08_120_127;
			break;

		case 0x00000240: // "psubb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = R5900Regs.Reg[RS].u08_00_07 - R5900Regs.Reg[RT].u08_00_07;
			R5900Regs.Reg[RD].u08_08_15 = R5900Regs.Reg[RS].u08_08_15 - R5900Regs.Reg[RT].u08_08_15;
			R5900Regs.Reg[RD].u08_16_23 = R5900Regs.Reg[RS].u08_16_23 - R5900Regs.Reg[RT].u08_16_23;
			R5900Regs.Reg[RD].u08_24_31 = R5900Regs.Reg[RS].u08_24_31 - R5900Regs.Reg[RT].u08_24_31;
			R5900Regs.Reg[RD].u08_32_39 = R5900Regs.Reg[RS].u08_32_39 - R5900Regs.Reg[RT].u08_32_39;
			R5900Regs.Reg[RD].u08_40_47 = R5900Regs.Reg[RS].u08_40_47 - R5900Regs.Reg[RT].u08_40_47;
			R5900Regs.Reg[RD].u08_48_55 = R5900Regs.Reg[RS].u08_48_55 - R5900Regs.Reg[RT].u08_48_55;
			R5900Regs.Reg[RD].u08_56_63 = R5900Regs.Reg[RS].u08_56_63 - R5900Regs.Reg[RT].u08_56_63;
			R5900Regs.Reg[RD].u08_64_71 = R5900Regs.Reg[RS].u08_64_71 - R5900Regs.Reg[RT].u08_64_71;
			R5900Regs.Reg[RD].u08_72_79 = R5900Regs.Reg[RS].u08_72_79 - R5900Regs.Reg[RT].u08_72_79;
			R5900Regs.Reg[RD].u08_80_87 = R5900Regs.Reg[RS].u08_80_87 - R5900Regs.Reg[RT].u08_80_87;
			R5900Regs.Reg[RD].u08_88_95 = R5900Regs.Reg[RS].u08_88_95 - R5900Regs.Reg[RT].u08_88_95;
			R5900Regs.Reg[RD].u08_96_103 = R5900Regs.Reg[RS].u08_96_103 - R5900Regs.Reg[RT].u08_96_103;
			R5900Regs.Reg[RD].u08_104_111 = R5900Regs.Reg[RS].u08_104_111 - R5900Regs.Reg[RT].u08_104_111;
			R5900Regs.Reg[RD].u08_112_119 = R5900Regs.Reg[RS].u08_112_119 - R5900Regs.Reg[RT].u08_112_119;
			R5900Regs.Reg[RD].u08_120_127 = R5900Regs.Reg[RS].u08_120_127 - R5900Regs.Reg[RT].u08_120_127;
			break;

		case 0x00000280: // "pcgtb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = (R5900Regs.Reg[RS].u08_00_07 > R5900Regs.Reg[RT].u08_00_07) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_08_15 = (R5900Regs.Reg[RS].u08_08_15 > R5900Regs.Reg[RT].u08_08_15) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_16_23 = (R5900Regs.Reg[RS].u08_16_23 > R5900Regs.Reg[RT].u08_16_23) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_24_31 = (R5900Regs.Reg[RS].u08_24_31 > R5900Regs.Reg[RT].u08_24_31) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_32_39 = (R5900Regs.Reg[RS].u08_32_39 > R5900Regs.Reg[RT].u08_32_39) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_40_47 = (R5900Regs.Reg[RS].u08_40_47 > R5900Regs.Reg[RT].u08_40_47) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_48_55 = (R5900Regs.Reg[RS].u08_48_55 > R5900Regs.Reg[RT].u08_48_55) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_56_63 = (R5900Regs.Reg[RS].u08_56_63 > R5900Regs.Reg[RT].u08_56_63) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_64_71 = (R5900Regs.Reg[RS].u08_64_71 > R5900Regs.Reg[RT].u08_64_71) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_72_79 = (R5900Regs.Reg[RS].u08_72_79 > R5900Regs.Reg[RT].u08_72_79) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_80_87 = (R5900Regs.Reg[RS].u08_80_87 > R5900Regs.Reg[RT].u08_80_87) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_88_95 = (R5900Regs.Reg[RS].u08_88_95 > R5900Regs.Reg[RT].u08_88_95) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_96_103 = (R5900Regs.Reg[RS].u08_96_103 > R5900Regs.Reg[RT].u08_96_103) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_104_111 = (R5900Regs.Reg[RS].u08_104_111 > R5900Regs.Reg[RT].u08_104_111) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_112_119 = (R5900Regs.Reg[RS].u08_112_119 > R5900Regs.Reg[RT].u08_112_119) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_120_127 = (R5900Regs.Reg[RS].u08_120_127 > R5900Regs.Reg[RT].u08_120_127) ? 0xFF : 0;
			break;

		case 0x00000400: // "paddsw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_00_31 + R5900Regs.Reg[RT].i32_00_31;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_00_31 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_00_31 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_00_31 = Temp128.u32_00_31;
				}
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_32_63 + R5900Regs.Reg[RT].i32_32_63;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_32_63 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_32_63 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_32_63 = Temp128.u32_00_31;
				}
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_64_95 + R5900Regs.Reg[RT].i32_64_95;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_64_95 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_64_95 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_64_95 = Temp128.u32_00_31;
				}
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_96_127 + R5900Regs.Reg[RT].i32_96_127;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_96_127 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_96_127 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_96_127 = Temp128.u32_00_31;
				}
			break;

		case 0x00000440: // "psubsw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_00_31 - R5900Regs.Reg[RT].i32_00_31;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_00_31 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_00_31 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_00_31 = Temp128.u32_00_31;
				}
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_32_63 - R5900Regs.Reg[RT].i32_32_63;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_32_63 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_32_63 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_32_63 = Temp128.u32_00_31;
				}
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_64_95 - R5900Regs.Reg[RT].i32_64_95;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_64_95 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_64_95 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_64_95 = Temp128.u32_00_31;
				}
			Temp128.i64_00_63 = R5900Regs.Reg[RS].i32_96_127 - R5900Regs.Reg[RT].i32_96_127;
			if (Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[RD].u32_96_127 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[RD].u32_96_127 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u32_96_127 = Temp128.u32_00_31;
				}
			break;

		case 0x00000480: // "pextlw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RS].u32_00_31;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RS].u32_32_63;
			break;

		case 0x000004C0: // "ppacw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RS].u32_00_31;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RS].u32_64_95;
			break;

		case 0x00000500: // "paddsh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_00_15 + R5900Regs.Reg[RT].i16_00_15;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_00_15 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_00_15 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_00_15 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_16_31 + R5900Regs.Reg[RT].i16_16_31;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_16_31 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_16_31 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_16_31 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_32_47 + R5900Regs.Reg[RT].i16_32_47;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].i16_32_47 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_32_47 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_32_47 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_48_63 + R5900Regs.Reg[RT].i16_48_63;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_48_63 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_48_63 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_48_63 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_64_79 + R5900Regs.Reg[RT].i16_64_79;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_64_79 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_64_79 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_64_79 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_80_95 + R5900Regs.Reg[RT].i16_80_95;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_80_95 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_80_95 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_80_95 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_96_111 + R5900Regs.Reg[RT].i16_96_111;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_96_111 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_96_111 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_96_111 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_112_127 + R5900Regs.Reg[RT].i16_112_127;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_112_127 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_112_127 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_112_127 = Temp128.u16_00_15;
				}
			break;

		case 0x00000540: // "psubsh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_00_15 - R5900Regs.Reg[RT].i16_00_15;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_00_15 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_00_15 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_00_15 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_16_31 - R5900Regs.Reg[RT].i16_16_31;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_16_31 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_16_31 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_16_31 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_32_47 - R5900Regs.Reg[RT].i16_32_47;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_32_47 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_32_47 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_32_47 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_48_63 - R5900Regs.Reg[RT].i16_48_63;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_48_63 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_48_63 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_48_63 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_64_79 - R5900Regs.Reg[RT].i16_64_79;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_64_79 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_64_79 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_64_79 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_80_95 - R5900Regs.Reg[RT].i16_80_95;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_80_95 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_80_95 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_80_95 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_96_111 - R5900Regs.Reg[RT].i16_96_111;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_96_111 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_96_111 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_96_111 = Temp128.u16_00_15;
				}
			Temp128.i32_00_31 = R5900Regs.Reg[RS].i16_112_127 - R5900Regs.Reg[RT].i16_112_127;
			if (Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[RD].u16_112_127 = 0x7FFF;
			}
			else
				if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[RD].u16_112_127 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[RD].u16_112_127 = Temp128.u16_00_15;
				}
			break;

		case 0x00000580: // "pextlh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_00_15;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_16_31;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_32_47;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_48_63;
			break;

		case 0x000005C0: // "ppach"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RS].u16_00_15;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_32_47;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RS].u16_64_79;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_96_111;
			break;

			/////////////////////////////////////////////////////////////////////////
		case 0x00000600: // "paddsb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempI32 = R5900Regs.Reg[RS].i08_00_07 + R5900Regs.Reg[RT].i08_00_07;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_00_07 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_00_07 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_00_07 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_08_15 + R5900Regs.Reg[RT].i08_08_15;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_08_15 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_08_15 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_08_15 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_16_23 + R5900Regs.Reg[RT].i08_16_23;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_16_23 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_16_23 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_16_23 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_24_31 + R5900Regs.Reg[RT].i08_24_31;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_24_31 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_24_31 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_24_31 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_32_39 + R5900Regs.Reg[RT].i08_32_39;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_32_39 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_32_39 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_32_39 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_40_47 + R5900Regs.Reg[RT].i08_40_47;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_40_47 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_40_47 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_40_47 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_48_55 + R5900Regs.Reg[RT].i08_48_55;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_48_55 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_48_55 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_48_55 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_56_63 + R5900Regs.Reg[RT].i08_56_63;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_56_63 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_56_63 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_56_63 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_64_71 + R5900Regs.Reg[RT].i08_64_71;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_64_71 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_64_71 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_64_71 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_72_79 + R5900Regs.Reg[RT].i08_72_79;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_72_79 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_72_79 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_72_79 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_80_87 + R5900Regs.Reg[RT].i08_80_87;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_80_87 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_80_87 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_80_87 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_88_95 + R5900Regs.Reg[RT].i08_88_95;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_88_95 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_88_95 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_88_95 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_96_103 + R5900Regs.Reg[RT].i08_96_103;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_96_103 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_96_103 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_96_103 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_104_111 + R5900Regs.Reg[RT].i08_104_111;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_104_111 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_104_111 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_104_111 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_112_119 + R5900Regs.Reg[RT].i08_112_119;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_112_119 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_112_119 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_112_119 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_120_127 + R5900Regs.Reg[RT].i08_120_127;
			if (TempI32 > 0x7f)
			{
				R5900Regs.Reg[RD].i08_120_127 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_120_127 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_120_127 = TempI32;
			}
			break;

		case 0x00000640: // "psubsb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempI32 = R5900Regs.Reg[RS].i08_00_07 - R5900Regs.Reg[RT].i08_00_07;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_00_07 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_00_07 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_00_07 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_08_15 - R5900Regs.Reg[RT].i08_08_15;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_08_15 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_08_15 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_08_15 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_16_23 - R5900Regs.Reg[RT].i08_16_23;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_16_23 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_16_23 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_16_23 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_24_31 - R5900Regs.Reg[RT].i08_24_31;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_24_31 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_24_31 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_24_31 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_32_39 - R5900Regs.Reg[RT].i08_32_39;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_32_39 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_32_39 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_32_39 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_40_47 - R5900Regs.Reg[RT].i08_40_47;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_40_47 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_40_47 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_40_47 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_48_55 - R5900Regs.Reg[RT].i08_48_55;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_48_55 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_48_55 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_48_55 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_56_63 - R5900Regs.Reg[RT].i08_56_63;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_56_63 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_56_63 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_56_63 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_64_71 - R5900Regs.Reg[RT].i08_64_71;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_64_71 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_64_71 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_64_71 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_72_79 - R5900Regs.Reg[RT].i08_72_79;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_72_79 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_72_79 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_72_79 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_80_87 - R5900Regs.Reg[RT].i08_80_87;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_80_87 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_80_87 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_80_87 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_88_95 - R5900Regs.Reg[RT].i08_88_95;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_88_95 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_88_95 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_88_95 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_96_103 - R5900Regs.Reg[RT].i08_96_103;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_96_103 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_96_103 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_96_103 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_104_111 - R5900Regs.Reg[RT].i08_104_111;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_104_111 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_104_111 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_104_111 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_112_119 - R5900Regs.Reg[RT].i08_112_119;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_112_119 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_112_119 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_112_119 = TempI32;
			}
			TempI32 = R5900Regs.Reg[RS].i08_120_127 - R5900Regs.Reg[RT].i08_120_127;
			if (TempI32 >= 0x7f)
			{
				R5900Regs.Reg[RD].i08_120_127 = (EMU_I08)0x7f;
			}
			else if (0x100 <= TempI32 && TempI32 < 0x180)
			{
				R5900Regs.Reg[RD].i08_120_127 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[RD].i08_120_127 = TempI32;
			}
			break;

		case 0x00000680: // "pextlb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = R5900Regs.Reg[RT].u08_00_07;
			R5900Regs.Reg[RD].u08_08_15 = R5900Regs.Reg[RS].u08_00_07;
			R5900Regs.Reg[RD].u08_16_23 = R5900Regs.Reg[RT].u08_08_15;
			R5900Regs.Reg[RD].u08_24_31 = R5900Regs.Reg[RS].u08_08_15;
			R5900Regs.Reg[RD].u08_32_39 = R5900Regs.Reg[RT].u08_16_23;
			R5900Regs.Reg[RD].u08_40_47 = R5900Regs.Reg[RS].u08_16_23;
			R5900Regs.Reg[RD].u08_48_55 = R5900Regs.Reg[RT].u08_24_31;
			R5900Regs.Reg[RD].u08_56_63 = R5900Regs.Reg[RS].u08_24_31;
			R5900Regs.Reg[RD].u08_64_71 = R5900Regs.Reg[RT].u08_32_39;
			R5900Regs.Reg[RD].u08_72_79 = R5900Regs.Reg[RS].u08_32_39;
			R5900Regs.Reg[RD].u08_80_87 = R5900Regs.Reg[RT].u08_40_47;
			R5900Regs.Reg[RD].u08_88_95 = R5900Regs.Reg[RS].u08_40_47;
			R5900Regs.Reg[RD].u08_96_103 = R5900Regs.Reg[RT].u08_48_55;
			R5900Regs.Reg[RD].u08_104_111 = R5900Regs.Reg[RS].u08_48_55;
			R5900Regs.Reg[RD].u08_112_119 = R5900Regs.Reg[RT].u08_56_63;
			R5900Regs.Reg[RD].u08_120_127 = R5900Regs.Reg[RS].u08_56_63;
			break;

		case 0x000006C0: // "ppacb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = R5900Regs.Reg[RT].u08_00_07;
			R5900Regs.Reg[RD].u08_08_15 = R5900Regs.Reg[RT].u08_16_23;
			R5900Regs.Reg[RD].u08_16_23 = R5900Regs.Reg[RT].u08_32_39;
			R5900Regs.Reg[RD].u08_24_31 = R5900Regs.Reg[RT].u08_48_55;
			R5900Regs.Reg[RD].u08_32_39 = R5900Regs.Reg[RT].u08_64_71;
			R5900Regs.Reg[RD].u08_40_47 = R5900Regs.Reg[RT].u08_80_87;
			R5900Regs.Reg[RD].u08_48_55 = R5900Regs.Reg[RT].u08_96_103;
			R5900Regs.Reg[RD].u08_56_63 = R5900Regs.Reg[RT].u08_112_119;
			R5900Regs.Reg[RD].u08_64_71 = R5900Regs.Reg[RS].u08_00_07;
			R5900Regs.Reg[RD].u08_72_79 = R5900Regs.Reg[RS].u08_16_23;
			R5900Regs.Reg[RD].u08_80_87 = R5900Regs.Reg[RS].u08_32_39;
			R5900Regs.Reg[RD].u08_88_95 = R5900Regs.Reg[RS].u08_48_55;
			R5900Regs.Reg[RD].u08_96_103 = R5900Regs.Reg[RS].u08_64_71;
			R5900Regs.Reg[RD].u08_104_111 = R5900Regs.Reg[RS].u08_80_87;
			R5900Regs.Reg[RD].u08_112_119 = R5900Regs.Reg[RS].u08_96_103;
			R5900Regs.Reg[RD].u08_120_127 = R5900Regs.Reg[RS].u08_112_119;
			break;

		case 0x00000780: // "pext5"
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u32_00_31 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
				((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
			TempU32 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u32_32_63 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
				((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
			TempU32 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u32_64_95 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
				((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
			TempU32 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u32_96_127 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
				((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
			break;

		case 0x000007C0: // "ppac5"
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_00_31 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
				((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
			TempU32 = R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_32_63 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
				((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
			TempU32 = R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_64_95 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
				((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
			TempU32 = R5900Regs.Reg[RT].u32_96_127;
			R5900Regs.Reg[RD].u32_96_127 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
				((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
			break;
		}
		///////////////////////////////////
		// End of MMI0 Funcions
		///////////////////////////////////
		break;

	case 0x00000009: // MMI2
		///////////////////////////////////
		// MMI2 Funcions
		///////////////////////////////////
		switch (Code & 0x000007C0)
		{
		case 0x00000000: // "pmaddw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempI64 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31) + ((EMU_U64)R5900Regs.Reg[RS].i32_00_31 * (EMU_U64)R5900Regs.Reg[RT].i32_00_31);
			R5900Regs.LO.u64_00_63 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[RD].u64_00_63 = TempI64;
			TempI64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95) + ((EMU_U64)R5900Regs.Reg[RS].i32_64_95 * (EMU_U64)R5900Regs.Reg[RT].i32_64_95);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[RD].u64_64_127 = TempI64;
			break;

		case 0x00000080: // "psllvw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = (EMU_I32)((EMU_U64)R5900Regs.Reg[RT].u32_00_31 << (R5900Regs.Reg[RS].u32_00_31 & 0x1F));
			R5900Regs.Reg[RD].u64_64_127 = (EMU_I32)((EMU_U64)R5900Regs.Reg[RT].u32_64_95 << (R5900Regs.Reg[RS].u32_64_95 & 0x1F));
			break;

		case 0x000000C0: // "psrlvw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = (EMU_I32)(R5900Regs.Reg[RT].u32_00_31 >> (R5900Regs.Reg[RS].u32_00_31 & 0x1F));
			R5900Regs.Reg[RD].u64_64_127 = (EMU_I32)(R5900Regs.Reg[RT].u32_64_95 >> (R5900Regs.Reg[RS].u32_64_95 & 0x1F));
			break;

		case 0x00000100: // "pmsubw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempI64 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | R5900Regs.LO.u32_00_31) - (R5900Regs.Reg[RS].i32_00_31 * R5900Regs.Reg[RT].i32_00_31);
			R5900Regs.LO.u64_00_63 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[RD].u64_00_63 = TempI64;
			TempI64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95) - (R5900Regs.Reg[RS].i32_64_95 * R5900Regs.Reg[RT].i32_64_95);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[RD].u64_64_127 = TempI64;
			break;

		case 0x00000200: // "pmfhi",
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.HI.u64_00_63;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.HI.u64_64_127;
			break;

		case 0x00000240: // "pmflo"
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.u64_00_63;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.LO.u64_64_127;
			break;

		case 0x00000280: // "pinth"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_64_79;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_80_95;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_96_111;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_112_127;
			break;

		case 0x00000300: // "pmultw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempI64 = R5900Regs.Reg[RS].i32_00_31 * R5900Regs.Reg[RT].i32_00_31;
			R5900Regs.LO.u64_00_63 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			if (RD)
			{
				R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.u64_00_63;
			}
			TempI64 = R5900Regs.Reg[RS].i32_64_95 * R5900Regs.Reg[RT].i32_64_95;
			R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			if (RD)
			{
				R5900Regs.Reg[RD].u64_64_127 = R5900Regs.LO.u64_64_127;
			}
			break;

		case 0x00000340: // "pdivw"
			RS = R_RS;
			RT = R_RT;
			R5900Regs.LO.u64_00_63 = (EMU_I32)(R5900Regs.Reg[RS].i32_00_31 /
				R5900Regs.Reg[RT].i32_00_31);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(R5900Regs.Reg[RS].i32_64_95 /
				R5900Regs.Reg[RT].i32_64_95);
			R5900Regs.HI.u64_00_63 = (EMU_I32)(R5900Regs.Reg[RS].i32_00_31 %
				R5900Regs.Reg[RT].i32_00_31);
			R5900Regs.HI.u64_64_127 = (EMU_I32)(R5900Regs.Reg[RS].i32_64_95 %
				R5900Regs.Reg[RT].i32_64_95);
			break;

		case 0x00000380: // "pcpyld"
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.Reg[R_RS].u64_00_63;
			break;

		case 0x00000400: // "pmaddh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.LO.u32_00_31 = (EMU_I32)(R5900Regs.LO.u32_00_31 +
				(R5900Regs.Reg[RS].i16_00_15 * R5900Regs.Reg[RT].i16_00_15));
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.LO.u32_32_63 = (EMU_I32)(R5900Regs.LO.u32_32_63 +
				(R5900Regs.Reg[RS].i16_16_31 * R5900Regs.Reg[RT].i16_16_31));
			R5900Regs.HI.u32_00_31 = (EMU_I32)(R5900Regs.HI.u32_00_31 +
				(R5900Regs.Reg[RS].i16_32_47 * R5900Regs.Reg[RT].i16_32_47));
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.HI.u32_32_63 = (EMU_I32)(R5900Regs.HI.u32_32_63 +
				(R5900Regs.Reg[RS].i16_48_63 * R5900Regs.Reg[RT].i16_48_63));
			R5900Regs.LO.u32_64_95 = (EMU_I32)(R5900Regs.LO.u32_64_95 +
				(R5900Regs.Reg[RS].i16_64_79 * R5900Regs.Reg[RT].i16_64_79));
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.LO.u32_96_127 = (EMU_I32)(R5900Regs.LO.u32_96_127 +
				(R5900Regs.Reg[RS].i16_80_95 * R5900Regs.Reg[RT].i16_80_95));
			R5900Regs.HI.u32_64_95 = (EMU_I32)(R5900Regs.HI.u32_64_95 +
				(R5900Regs.Reg[RS].i16_96_111 * R5900Regs.Reg[RT].i16_96_111));
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			R5900Regs.HI.u32_96_127 = (EMU_I32)(R5900Regs.HI.u32_96_127 +
				(R5900Regs.Reg[RS].i16_112_127 * R5900Regs.Reg[RT].i16_112_127));
			break;

		case 0x00000440: // "phmadh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.LO.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_16_31 * R5900Regs.Reg[RT].i16_16_31) +
				(R5900Regs.Reg[RS].i16_00_15 * R5900Regs.Reg[RT].i16_00_15));
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.HI.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_48_63 * R5900Regs.Reg[RT].i16_48_63) +
				(R5900Regs.Reg[RS].i16_32_47 * R5900Regs.Reg[RT].i16_32_47));
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.LO.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_80_95 * R5900Regs.Reg[RT].i16_80_95) +
				(R5900Regs.Reg[RS].i16_64_79 * R5900Regs.Reg[RT].i16_64_79));
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.HI.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_112_127 * R5900Regs.Reg[RT].i16_112_127) +
				(R5900Regs.Reg[RS].i16_96_111 * R5900Regs.Reg[RT].i16_96_111));
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			break;

		case 0x00000480: // "pand" 
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.Reg[RS].u64_00_63 &
				R5900Regs.Reg[RT].u64_00_63;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.Reg[RS].u64_64_127 &
				R5900Regs.Reg[RT].u64_64_127;
			break;

		case 0x000004C0: // "pxor" 
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.Reg[RS].u64_00_63 ^
				R5900Regs.Reg[RT].u64_00_63;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.Reg[RS].u64_64_127 ^
				R5900Regs.Reg[RT].u64_64_127;
			break;

		case 0x00000500: // "pmsubh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.LO.u32_00_31 = (EMU_I32)(R5900Regs.LO.u32_00_31 -
				(R5900Regs.Reg[RS].i16_00_15 * R5900Regs.Reg[RT].i16_00_15));
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.LO.u32_32_63 = (EMU_I32)(R5900Regs.LO.u32_32_63 -
				(R5900Regs.Reg[RS].i16_16_31 * R5900Regs.Reg[RT].i16_16_31));
			R5900Regs.HI.u32_00_31 = (EMU_I32)(R5900Regs.HI.u32_00_31 -
				(R5900Regs.Reg[RS].i16_32_47 * R5900Regs.Reg[RT].i16_32_47));
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.HI.u32_32_63 = (EMU_I32)(R5900Regs.HI.u32_32_63 -
				(R5900Regs.Reg[RS].i16_48_63 * R5900Regs.Reg[RT].i16_48_63));
			R5900Regs.LO.u32_64_95 = (EMU_I32)(R5900Regs.LO.u32_64_95 -
				(R5900Regs.Reg[RS].i16_64_79 * R5900Regs.Reg[RT].i16_64_79));
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.LO.u32_96_127 = (EMU_I32)(R5900Regs.LO.u32_96_127 -
				(R5900Regs.Reg[RS].i16_80_95 * R5900Regs.Reg[RT].i16_80_95));
			R5900Regs.HI.u32_64_95 = (EMU_I32)(R5900Regs.HI.u32_64_95 -
				(R5900Regs.Reg[RS].i16_96_111 * R5900Regs.Reg[RT].i16_96_111));
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			R5900Regs.HI.u32_96_127 = (EMU_I32)(R5900Regs.HI.u32_96_127 -
				(R5900Regs.Reg[RS].i16_112_127 * R5900Regs.Reg[RT].i16_112_127));
			break;

		case 0x00000540: // "phmsbh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.LO.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_16_31 * R5900Regs.Reg[RT].i16_16_31) -
				(R5900Regs.Reg[RS].i16_00_15 * R5900Regs.Reg[RT].i16_00_15));
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.HI.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_48_63 * R5900Regs.Reg[RT].i16_48_63) -
				(R5900Regs.Reg[RS].i16_32_47 * R5900Regs.Reg[RT].i16_32_47));
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.LO.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_80_95 * R5900Regs.Reg[RT].i16_80_95) -
				(R5900Regs.Reg[RS].i16_64_79 * R5900Regs.Reg[RT].i16_64_79));
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.HI.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[RS].i16_112_127 * R5900Regs.Reg[RT].i16_112_127) -
				(R5900Regs.Reg[RS].i16_96_111 * R5900Regs.Reg[RT].i16_96_111));
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			break;

		case 0x00000680: // "pexeh"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x000006C0: // "prevh"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_112_127;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RT].u16_64_79;
			break;

		case 0x00000700: // "pmulth"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.LO.u32_00_31 = (EMU_I16)(R5900Regs.Reg[RS].i16_00_15 *
				R5900Regs.Reg[RT].i16_00_15);
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.LO.u32_32_63 = (EMU_I16)(R5900Regs.Reg[RS].i16_16_31 *
				R5900Regs.Reg[RT].i16_16_31);
			R5900Regs.HI.u32_00_31 = (EMU_I16)(R5900Regs.Reg[RS].i16_32_47 *
				R5900Regs.Reg[RT].i16_32_47);
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.HI.u32_32_63 = (EMU_I16)(R5900Regs.Reg[RS].i16_48_63 *
				R5900Regs.Reg[RT].i16_48_63);
			R5900Regs.LO.u32_64_95 = (EMU_I16)(R5900Regs.Reg[RS].i16_64_79 *
				R5900Regs.Reg[RT].i16_64_79);
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.LO.u32_96_127 = (EMU_I16)(R5900Regs.Reg[RS].i16_80_95 *
				R5900Regs.Reg[RT].i16_80_95);
			R5900Regs.HI.u32_64_95 = (EMU_I16)(R5900Regs.Reg[RS].i16_96_111 *
				R5900Regs.Reg[RT].i16_96_111);
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			R5900Regs.HI.u32_96_127 = (EMU_I16)(R5900Regs.Reg[RS].i16_112_127 *
				R5900Regs.Reg[RT].u16_112_127);
			break;

		case 0x00000740: // "pdivbw"
			RS = R_RS;
			RT = R_RT;
			R5900Regs.LO.i32_00_31 = R5900Regs.Reg[RS].i32_00_31 / R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.LO.i32_32_63 = R5900Regs.Reg[RS].i32_32_63 / R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.LO.i32_64_95 = R5900Regs.Reg[RS].i32_64_95 / R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.LO.i32_96_127 = R5900Regs.Reg[RS].i32_96_127 / R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.HI.i32_00_31 = R5900Regs.Reg[RS].i32_00_31 % R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.HI.i32_32_63 = R5900Regs.Reg[RS].i32_32_63 % R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.HI.i32_64_95 = R5900Regs.Reg[RS].i32_64_95 % R5900Regs.Reg[RT].i16_00_15;
			R5900Regs.HI.i32_96_127 = R5900Regs.Reg[RS].i32_96_127 % R5900Regs.Reg[RT].i16_00_15;
			break;

		case 0x00000780: // "pexew"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RT].u32_96_127;
			break;

		case 0x000007C0: // "prot3w"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RT].u32_96_127;
			break;
		}
		///////////////////////////////////
		// End of MMI2 Funcions
		///////////////////////////////////
		break;

	case 0x00000010: // "mfhi1"
		R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.HI.u64_64_127;
		break;

	case 0x00000011: // "mthi1"
		R5900Regs.HI.u64_64_127 = R5900Regs.Reg[R_RS].u64_00_63;
		break;

	case 0x00000012: // "mflo1"
		R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		break;

	case 0x00000013: // "mtlo1"
		R5900Regs.LO.u64_64_127 = R5900Regs.Reg[R_RS].u64_00_63;
		break;

	case 0x00000018: // "mult1"
		RD = R_RD;
		TempI64 = (EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31;
		R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(TempI64 >> 32);
		if (RD)
		{
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		}
		break;

	case 0x00000019: // "multu1"
		RD = R_RD;
		TempU64 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
		R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(TempU64 >> 32);
		if (RD)
		{
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		}
		break;

	case 0x0000001A: // "div1"
		RS = R_RS;
		RT = R_RT;
		R5900Regs.LO.u64_64_127 = R5900Regs.Reg[RS].i32_00_31 / R5900Regs.Reg[RT].i32_00_31;
		R5900Regs.HI.u64_64_127 = R5900Regs.Reg[RS].i32_00_31 % R5900Regs.Reg[RT].i32_00_31;
		break;

	case 0x0000001B: // "divu1"
		RS = R_RS;
		RT = R_RT;
		R5900Regs.LO.u64_64_127 = (EMU_I32)(R5900Regs.Reg[RS].u32_00_31 / R5900Regs.Reg[RT].u32_00_31);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(R5900Regs.Reg[RS].u32_00_31 % R5900Regs.Reg[RT].u32_00_31);
		break;

	case 0x00000020: // "madd1"
		TempI64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95) + (R5900Regs.Reg[R_RS].i32_00_31 * R5900Regs.Reg[R_RT].i32_00_31);
		R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
		R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		break;

	case 0x00000021: // "maddu1"
		TempU64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95) + (R5900Regs.Reg[R_RS].u32_00_31 * R5900Regs.Reg[R_RT].u32_00_31);
		R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
		R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		break;

	case 0x00000028: // "MMI1"
		///////////////////////////////////
		// MMI1 Funcions
		///////////////////////////////////
		switch (Code & 0x000007C0)
		{
		case 0x00000040: // "pabsw"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = abs(R5900Regs.Reg[RT].i32_00_31);
			R5900Regs.Reg[RD].u32_32_63 = abs(R5900Regs.Reg[RT].i32_32_63);
			R5900Regs.Reg[RD].u32_64_95 = abs(R5900Regs.Reg[RT].i32_64_95);
			R5900Regs.Reg[RD].u32_96_127 = abs(R5900Regs.Reg[RT].i32_96_127);
			break;

		case 0x00000080: // "pceqw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = (R5900Regs.Reg[RS].u32_00_31 == R5900Regs.Reg[RT].u32_00_31) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[RD].u32_32_63 = (R5900Regs.Reg[RS].u32_32_63 == R5900Regs.Reg[RT].u32_32_63) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[RD].u32_64_95 = (R5900Regs.Reg[RS].u32_64_95 == R5900Regs.Reg[RT].u32_64_95) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[RD].u32_96_127 = (R5900Regs.Reg[RS].u32_96_127 == R5900Regs.Reg[RT].u32_96_127) ? 0xFFFFFFFF : 0;
			break;

		case 0x000000C0: // "pminw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = (R5900Regs.Reg[RS].i32_00_31 < R5900Regs.Reg[RT].i32_00_31) ?
				R5900Regs.Reg[RS].u32_00_31 : R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = (R5900Regs.Reg[RS].i32_32_63 < R5900Regs.Reg[RT].i32_32_63) ?
				R5900Regs.Reg[RS].u32_32_63 : R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_64_95 = (R5900Regs.Reg[RS].i32_64_95 < R5900Regs.Reg[RT].i32_64_95) ?
				R5900Regs.Reg[RS].u32_64_95 : R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_96_127 = (R5900Regs.Reg[RS].i32_96_127 < R5900Regs.Reg[RT].i32_96_127) ?
				R5900Regs.Reg[RS].u32_96_127 : R5900Regs.Reg[RT].u32_96_127;
			break;

		case 0x00000100: // "padsbh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RS].u16_00_15 - R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_16_31 - R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RS].u16_32_47 - R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_48_63 - R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RS].u16_64_79 + R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_80_95 + R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RS].u16_96_111 + R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_112_127 + R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x00000140: // "pabsh"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = abs(R5900Regs.Reg[RT].u16_00_15);
			R5900Regs.Reg[RD].u16_16_31 = abs(R5900Regs.Reg[RT].u16_16_31);
			R5900Regs.Reg[RD].u16_32_47 = abs(R5900Regs.Reg[RT].u16_32_47);
			R5900Regs.Reg[RD].u16_48_63 = abs(R5900Regs.Reg[RT].u16_48_63);
			R5900Regs.Reg[RD].u16_64_79 = abs(R5900Regs.Reg[RT].u16_64_79);
			R5900Regs.Reg[RD].u16_80_95 = abs(R5900Regs.Reg[RT].u16_80_95);
			R5900Regs.Reg[RD].u16_96_111 = abs(R5900Regs.Reg[RT].u16_96_111);
			R5900Regs.Reg[RD].u16_112_127 = abs(R5900Regs.Reg[RT].u16_112_127);
			break;

		case 0x00000180: // "pceqh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = (R5900Regs.Reg[RS].u16_00_15 == R5900Regs.Reg[RT].u16_00_15) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_16_31 = (R5900Regs.Reg[RS].u16_16_31 == R5900Regs.Reg[RT].u16_16_31) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_32_47 = (R5900Regs.Reg[RS].u16_32_47 == R5900Regs.Reg[RT].u16_32_47) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_48_63 = (R5900Regs.Reg[RS].u16_48_63 == R5900Regs.Reg[RT].u16_48_63) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_64_79 = (R5900Regs.Reg[RS].u16_64_79 == R5900Regs.Reg[RT].u16_64_79) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_80_95 = (R5900Regs.Reg[RS].u16_80_95 == R5900Regs.Reg[RT].u16_80_95) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_96_111 = (R5900Regs.Reg[RS].u16_96_111 == R5900Regs.Reg[RT].u16_96_111) ? 0xFFFF : 0;
			R5900Regs.Reg[RD].u16_112_127 = (R5900Regs.Reg[RS].u16_112_127 == R5900Regs.Reg[RT].u16_112_127) ? 0xFFFF : 0;
			break;

		case 0x000001C0: // "pminh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = (R5900Regs.Reg[RS].i16_00_15 < R5900Regs.Reg[RT].i16_00_15) ?
				R5900Regs.Reg[RS].u16_00_15 : R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = (R5900Regs.Reg[RS].i16_16_31 < R5900Regs.Reg[RT].i16_16_31) ?
				R5900Regs.Reg[RS].u16_16_31 : R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_32_47 = (R5900Regs.Reg[RS].i16_32_47 < R5900Regs.Reg[RT].i16_32_47) ?
				R5900Regs.Reg[RS].u16_32_47 : R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_48_63 = (R5900Regs.Reg[RS].i16_48_63 < R5900Regs.Reg[RT].i16_48_63) ?
				R5900Regs.Reg[RS].u16_48_63 : R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = (R5900Regs.Reg[RS].i16_64_79 < R5900Regs.Reg[RT].i16_64_79) ?
				R5900Regs.Reg[RS].u16_64_79 : R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = (R5900Regs.Reg[RS].i16_80_95 < R5900Regs.Reg[RT].i16_80_95) ?
				R5900Regs.Reg[RS].u16_80_95 : R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_96_111 = (R5900Regs.Reg[RS].i16_96_111 < R5900Regs.Reg[RT].i16_96_111) ?
				R5900Regs.Reg[RS].u16_96_111 : R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_112_127 = (R5900Regs.Reg[RS].i16_112_127 < R5900Regs.Reg[RT].i16_112_127) ?
				R5900Regs.Reg[RS].u16_112_127 : R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x00000280: // "pceqb"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = (R5900Regs.Reg[RS].u08_00_07 == R5900Regs.Reg[RT].u08_00_07) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_08_15 = (R5900Regs.Reg[RS].u08_08_15 == R5900Regs.Reg[RT].u08_08_15) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_16_23 = (R5900Regs.Reg[RS].u08_16_23 == R5900Regs.Reg[RT].u08_16_23) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_24_31 = (R5900Regs.Reg[RS].u08_24_31 == R5900Regs.Reg[RT].u08_24_31) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_32_39 = (R5900Regs.Reg[RS].u08_32_39 == R5900Regs.Reg[RT].u08_32_39) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_40_47 = (R5900Regs.Reg[RS].u08_40_47 == R5900Regs.Reg[RT].u08_40_47) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_48_55 = (R5900Regs.Reg[RS].u08_48_55 == R5900Regs.Reg[RT].u08_48_55) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_56_63 = (R5900Regs.Reg[RS].u08_56_63 == R5900Regs.Reg[RT].u08_56_63) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_64_71 = (R5900Regs.Reg[RS].u08_64_71 == R5900Regs.Reg[RT].u08_64_71) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_72_79 = (R5900Regs.Reg[RS].u08_72_79 == R5900Regs.Reg[RT].u08_72_79) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_80_87 = (R5900Regs.Reg[RS].u08_80_87 == R5900Regs.Reg[RT].u08_80_87) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_88_95 = (R5900Regs.Reg[RS].u08_88_95 == R5900Regs.Reg[RT].u08_88_95) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_96_103 = (R5900Regs.Reg[RS].u08_96_103 == R5900Regs.Reg[RT].u08_96_103) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_104_111 = (R5900Regs.Reg[RS].u08_104_111 == R5900Regs.Reg[RT].u08_104_111) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_112_119 = (R5900Regs.Reg[RS].u08_112_119 == R5900Regs.Reg[RT].u08_112_119) ? 0xFF : 0;
			R5900Regs.Reg[RD].u08_120_127 = (R5900Regs.Reg[RS].u08_120_127 == R5900Regs.Reg[RT].u08_120_127) ? 0xFF : 0;
			break;

		case 0x00000400: // "padduw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RS].u32_00_31 + R5900Regs.Reg[RT].u32_00_31;
			if (TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[RD].u32_00_31 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[RD].u32_00_31 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u32_32_63 + R5900Regs.Reg[RT].u32_32_63;
			if (TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[RD].u32_32_63 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[RD].u32_32_63 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u32_64_95 + R5900Regs.Reg[RT].u32_64_95;
			if (TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[RD].u32_64_95 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[RD].u32_64_95 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u32_96_127 + R5900Regs.Reg[RT].u32_96_127;
			if (TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[RD].u32_96_127 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[RD].u32_96_127 = TempU32;
			}
			break;

		case 0x00000440: // "psubuw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RS].u32_00_31 - R5900Regs.Reg[RT].u32_00_31;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u32_00_31 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u32_00_31 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u32_32_63 - R5900Regs.Reg[RT].u32_32_63;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u32_32_63 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u32_32_63 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u32_64_95 - R5900Regs.Reg[RT].u32_64_95;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u32_64_95 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u32_64_95 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u32_96_127 - R5900Regs.Reg[RT].u32_96_127;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u32_96_127 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u32_96_127 = TempU32;
			}
			break;

		case 0x00000480: // "pextuw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RS].u32_64_95;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_96_127;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RS].u32_96_127;
			break;

		case 0x00000500: // "padduh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RS].u16_00_15 + R5900Regs.Reg[RT].u16_00_15;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_00_15 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_00_15 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_16_31 + R5900Regs.Reg[RT].u16_16_31;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_16_31 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_16_31 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_32_47 + R5900Regs.Reg[RT].u16_32_47;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_32_47 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_32_47 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_48_63 + R5900Regs.Reg[RT].u16_48_63;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_48_63 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_48_63 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_64_79 + R5900Regs.Reg[RT].u16_64_79;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_64_79 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_64_79 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_80_95 + R5900Regs.Reg[RT].u16_80_95;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_80_95 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_80_95 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_96_111 + R5900Regs.Reg[RT].u16_96_111;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_96_111 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_96_111 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_112_127 + R5900Regs.Reg[RT].u16_112_127;
			if (TempU32 > 0xffff)
			{
				R5900Regs.Reg[RD].u16_112_127 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[RD].u16_112_127 = TempU32;
			}
			break;

		case 0x00000540: // "psubuh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RS].u16_00_15 - R5900Regs.Reg[RT].u16_00_15;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_00_15 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_00_15 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_16_31 - R5900Regs.Reg[RT].u16_16_31;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_16_31 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_16_31 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_32_47 - R5900Regs.Reg[RT].u16_32_47;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_32_47 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_32_47 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_48_63 - R5900Regs.Reg[RT].u16_48_63;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_48_63 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_48_63 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_64_79 - R5900Regs.Reg[RT].u16_64_79;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_64_79 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_64_79 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_80_95 - R5900Regs.Reg[RT].u16_80_95;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_80_95 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_80_95 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_96_111 - R5900Regs.Reg[RT].u16_96_111;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_96_111 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_96_111 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u16_112_127 - R5900Regs.Reg[RT].u16_112_127;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u16_112_127 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u16_112_127 = TempU32;
			}
			break;

		case 0x00000580: // "pextuh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_64_79;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_80_95;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_96_111;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_112_127;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_112_127;
			break;

		case 0x00000600: // "paddub"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RS].u08_00_07 + R5900Regs.Reg[RT].u08_00_07;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_00_07 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_00_07 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_08_15 + R5900Regs.Reg[RT].u08_08_15;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_08_15 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_08_15 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_16_23 + R5900Regs.Reg[RT].u08_16_23;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_16_23 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_16_23 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_24_31 + R5900Regs.Reg[RT].u08_24_31;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_24_31 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_24_31 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_32_39 + R5900Regs.Reg[RT].u08_32_39;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_32_39 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_32_39 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_40_47 + R5900Regs.Reg[RT].u08_40_47;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_40_47 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_40_47 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_48_55 + R5900Regs.Reg[RT].u08_48_55;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_48_55 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_48_55 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_56_63 + R5900Regs.Reg[RT].u08_56_63;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_56_63 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_56_63 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_64_71 + R5900Regs.Reg[RT].u08_64_71;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_64_71 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_64_71 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_72_79 + R5900Regs.Reg[RT].u08_72_79;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_72_79 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_72_79 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_80_87 + R5900Regs.Reg[RT].u08_80_87;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_80_87 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_80_87 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_88_95 + R5900Regs.Reg[RT].u08_88_95;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_88_95 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_88_95 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_96_103 + R5900Regs.Reg[RT].u08_96_103;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_96_103 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_96_103 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_104_111 + R5900Regs.Reg[RT].u08_104_111;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_104_111 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_104_111 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_112_119 + R5900Regs.Reg[RT].u08_112_119;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_112_119 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_112_119 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_120_127 + R5900Regs.Reg[RT].u08_120_127;
			if (TempU32 > 0xff)
			{
				R5900Regs.Reg[RD].u08_120_127 = 0xff;
			}
			else
			{
				R5900Regs.Reg[RD].u08_120_127 = TempU32;
			}
			break;

		case 0x00000640: // "psubub"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU32 = R5900Regs.Reg[RS].u08_00_07 - R5900Regs.Reg[RT].u08_00_07;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_00_07 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_00_07 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_08_15 - R5900Regs.Reg[RT].u08_08_15;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_08_15 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_08_15 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_16_23 - R5900Regs.Reg[RT].u08_16_23;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_16_23 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_16_23 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_24_31 - R5900Regs.Reg[RT].u08_24_31;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_24_31 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_24_31 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_32_39 - R5900Regs.Reg[RT].u08_32_39;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_32_39 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_32_39 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_40_47 - R5900Regs.Reg[RT].u08_40_47;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_40_47 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_40_47 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_48_55 - R5900Regs.Reg[RT].u08_48_55;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_48_55 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_48_55 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_56_63 - R5900Regs.Reg[RT].u08_56_63;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_56_63 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_56_63 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_64_71 - R5900Regs.Reg[RT].u08_64_71;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_64_71 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_64_71 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_72_79 - R5900Regs.Reg[RT].u08_72_79;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_72_79 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_72_79 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_80_87 - R5900Regs.Reg[RT].u08_80_87;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_80_87 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_80_87 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_88_95 - R5900Regs.Reg[RT].u08_88_95;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_88_95 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_88_95 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_96_103 - R5900Regs.Reg[RT].u08_96_103;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_96_103 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_96_103 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_104_111 - R5900Regs.Reg[RT].u08_104_111;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_104_111 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_104_111 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_112_119 - R5900Regs.Reg[RT].u08_112_119;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_112_119 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_112_119 = TempU32;
			}
			TempU32 = R5900Regs.Reg[RS].u08_120_127 - R5900Regs.Reg[RT].u08_120_127;
			if (TempU32 <= 0)
			{
				R5900Regs.Reg[RD].u08_120_127 = 0;
			}
			else
			{
				R5900Regs.Reg[RD].u08_120_127 = TempU32;
			}
			break;

		case 0x00000680: // "pextub"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u08_00_07 = R5900Regs.Reg[RT].u08_64_71;
			R5900Regs.Reg[RD].u08_08_15 = R5900Regs.Reg[RS].u08_64_71;
			R5900Regs.Reg[RD].u08_16_23 = R5900Regs.Reg[RT].u08_72_79;
			R5900Regs.Reg[RD].u08_24_31 = R5900Regs.Reg[RS].u08_72_79;
			R5900Regs.Reg[RD].u08_32_39 = R5900Regs.Reg[RT].u08_80_87;
			R5900Regs.Reg[RD].u08_40_47 = R5900Regs.Reg[RS].u08_80_87;
			R5900Regs.Reg[RD].u08_48_55 = R5900Regs.Reg[RT].u08_88_95;
			R5900Regs.Reg[RD].u08_56_63 = R5900Regs.Reg[RS].u08_88_95;
			R5900Regs.Reg[RD].u08_64_71 = R5900Regs.Reg[RT].u08_96_103;
			R5900Regs.Reg[RD].u08_72_79 = R5900Regs.Reg[RS].u08_96_103;
			R5900Regs.Reg[RD].u08_80_87 = R5900Regs.Reg[RT].u08_104_111;
			R5900Regs.Reg[RD].u08_88_95 = R5900Regs.Reg[RS].u08_104_111;
			R5900Regs.Reg[RD].u08_96_103 = R5900Regs.Reg[RT].u08_112_119;
			R5900Regs.Reg[RD].u08_104_111 = R5900Regs.Reg[RS].u08_112_119;
			R5900Regs.Reg[RD].u08_112_119 = R5900Regs.Reg[RT].u08_120_127;
			R5900Regs.Reg[RD].u08_120_127 = R5900Regs.Reg[RS].u08_120_127;
			break;

		case 0x000006C0: // "qfsrv"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			if (R5900Regs.SA)
			{
				R5900Regs.Reg[RD].u64_00_63 = (R5900Regs.Reg[RS].u64_00_63 << (64 - R5900Regs.SA)) |
					(R5900Regs.Reg[RT].u64_00_63 >> R5900Regs.SA);

				R5900Regs.Reg[RD].u64_64_127 = (R5900Regs.Reg[RS].u64_64_127 << (64 - R5900Regs.SA)) |
					(R5900Regs.Reg[RT].u64_64_127 >> R5900Regs.SA);
			}
			else
			{
				R5900Regs.Reg[RD].u64_00_63 = R5900Regs.Reg[RT].u64_00_63;
				R5900Regs.Reg[RD].u64_64_127 = R5900Regs.Reg[RT].u64_64_127;
			}
			break;
		}
		///////////////////////////////////
		// End of MMI1 Funcions
		///////////////////////////////////
		break;

	case 0x00000029: // "MMI3"
		///////////////////////////////////
		// MMI3 Funcions
		///////////////////////////////////
		switch (Code & 0x000007C0)
		{
		case 0x00000000: // "pmadduw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU64 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | R5900Regs.LO.u32_00_31) + (R5900Regs.Reg[RS].u32_00_31 * R5900Regs.Reg[RT].u32_00_31);
			R5900Regs.LO.u64_00_63 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[RD].u64_00_63 = TempU64;
			TempU64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95) + (R5900Regs.Reg[RS].u32_64_95 * R5900Regs.Reg[RT].u32_64_95);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[RD].u64_00_63 = TempU64;
			break;

		case 0x000000C0: // "psravw"
			RT = R_RT;
			RD = R_RD;
			SA = R5900Regs.Reg[R_RS].u32_00_31 & 0x1F;
			R5900Regs.Reg[RD].u64_00_63 = (R5900Regs.Reg[RT].u32_00_31 & 0x80000000) ?
				(EMU_I32)((MaxU32 << (32 - SA)) | (R5900Regs.Reg[RT].u32_00_31 >> SA)) :
				(EMU_I32)(R5900Regs.Reg[RT].u32_00_31 >> SA);
			SA = R5900Regs.Reg[RS].u32_64_95 & 0x1F;
			R5900Regs.Reg[RD].u64_64_127 = (R5900Regs.Reg[RT].u32_64_95 & 0x80000000) ?
				(EMU_I32)((MaxU32 << (32 - SA)) | (R5900Regs.Reg[RT].u32_64_95 >> SA)) :
				(EMU_I32)(R5900Regs.Reg[RT].u32_64_95 >> SA);
			break;

		case 0x00000200: // "pmthi"
			RS = R_RS;
			R5900Regs.HI.u64_00_63 = R5900Regs.Reg[RS].u64_00_63;
			R5900Regs.HI.u64_64_127 = R5900Regs.Reg[RS].u64_64_127;
			break;

		case 0x00000240: // "pmtlo"
			RS = R_RS;
			R5900Regs.LO.u64_00_63 = R5900Regs.Reg[RS].u64_00_63;
			R5900Regs.LO.u64_64_127 = R5900Regs.Reg[RS].u64_64_127;
			break;

		case 0x00000280: // "pinteh"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RS].u16_00_15;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RS].u16_32_47;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RS].u16_64_79;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RS].u16_96_111;
			break;

		case 0x00000300: // "pmultuw"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			TempU64 = R5900Regs.Reg[RS].u32_00_31 * R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.LO.u64_00_63 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
			if (RD)
			{
				R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.u64_00_63;
			}
			TempU64 = R5900Regs.Reg[RS].u32_64_95 * R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
			if (RD)
			{
				R5900Regs.Reg[RD].u64_64_127 = R5900Regs.LO.u64_64_127;
			}
			break;

		case 0x00000340: // "pdivuw"
			RS = R_RS;
			RT = R_RT;
			R5900Regs.LO.u64_00_63 = (EMU_I32)(R5900Regs.Reg[RS].u32_00_31 /
				R5900Regs.Reg[RT].u32_00_31);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(R5900Regs.Reg[RS].u32_64_95 /
				R5900Regs.Reg[RT].u32_64_95);
			R5900Regs.HI.u64_00_63 = (EMU_I32)(R5900Regs.Reg[RS].u32_00_31 %
				R5900Regs.Reg[RT].u32_00_31);
			R5900Regs.HI.u64_64_127 = (EMU_I32)(R5900Regs.Reg[RS].u32_64_95 %
				R5900Regs.Reg[RT].u32_64_95);
			break;

		case 0x00000380: // "pcpyud"
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_64_127;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.Reg[R_RT].u64_64_127;
			break;

		case 0x00000480: // "por"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = R5900Regs.Reg[RS].u64_00_63 | R5900Regs.Reg[RT].u64_00_63;
			R5900Regs.Reg[RD].u64_64_127 = R5900Regs.Reg[RS].u64_64_127 | R5900Regs.Reg[RT].u64_64_127;
			break;

		case 0x000004C0: // "pnor"
			RS = R_RS;
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u64_00_63 = ~(R5900Regs.Reg[RS].u64_00_63 | R5900Regs.Reg[RT].u64_00_63);
			R5900Regs.Reg[RD].u64_64_127 = ~(R5900Regs.Reg[RS].u64_64_127 | R5900Regs.Reg[RT].u64_64_127);
			break;

		case 0x00000680: // "pexch"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_32_47;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_16_31;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_48_63;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RT].u16_96_111;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_80_95;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RT].u16_112_127;
			break;

		case 0x000006C0: // "pcpyh"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_00_15;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_64_79;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RT].u16_64_79;
			break;

		case 0x00000780: // "pexcw"
			RT = R_RT;
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RT].u32_64_95;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_32_63;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RT].u32_96_127;
			break;
		}
		///////////////////////////////////
		// End of MMI3 Funcions
		///////////////////////////////////
		break;

	case 0x00000030: // "pmfhl"
		switch (Code & 0x000001FF)
		{
		case 0x00000030: // "pmfhl.lw"
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			break;

		case 0x00000070: // "pmfhl.uw"
			RD = R_RD;
			R5900Regs.Reg[RD].u32_00_31 = R5900Regs.LO.u32_32_63;
			R5900Regs.Reg[RD].u32_32_63 = R5900Regs.HI.u32_32_63;
			R5900Regs.Reg[RD].u32_64_95 = R5900Regs.LO.u32_96_127;
			R5900Regs.Reg[RD].u32_96_127 = R5900Regs.HI.u32_96_127;
			break;

		case 0x000000B0: // "pmfhl.slw"
			RD = R_RD;
			TempU64 = ((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31;
			if (TempU64 >= 0x000000007FFFFFFF)
			{
				R5900Regs.Reg[RD].u64_00_63 = 0x000000007FFFFFFF;
			}
			else
				if (TempU64 <= 0xFFFFFFFF80000000)
				{
					R5900Regs.Reg[RD].u64_00_63 = 0xFFFFFFFF80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u64_00_63 = R5900Regs.LO.i32_00_31;
				}
			TempU64 = ((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95;
			if (TempU64 >= 0x000000007FFFFFFF)
			{
				R5900Regs.Reg[RD].u64_64_127 = 0x000000007FFFFFFF;
			}
			else
				if (TempU64 <= 0xFFFFFFFF80000000)
				{
					R5900Regs.Reg[RD].u64_64_127 = 0xFFFFFFFF80000000;
				}
				else
				{
					R5900Regs.Reg[RD].u64_64_127 = R5900Regs.LO.i32_64_95;
				}
			break;

		case 0x000000F0: // "pmfhl.lh"
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = R5900Regs.LO.u16_00_15;
			R5900Regs.Reg[RD].u16_16_31 = R5900Regs.LO.u16_32_47;
			R5900Regs.Reg[RD].u16_32_47 = R5900Regs.HI.u16_00_15;
			R5900Regs.Reg[RD].u16_48_63 = R5900Regs.HI.u16_32_47;
			R5900Regs.Reg[RD].u16_64_79 = R5900Regs.LO.u16_64_79;
			R5900Regs.Reg[RD].u16_80_95 = R5900Regs.LO.u16_96_111;
			R5900Regs.Reg[RD].u16_96_111 = R5900Regs.HI.u16_64_79;
			R5900Regs.Reg[RD].u16_112_127 = R5900Regs.HI.u16_96_111;
			break;

		case 0x00000130: // "pmfhl.sh"
			RD = R_RD;
			R5900Regs.Reg[RD].u16_00_15 = clamp(R5900Regs.LO.u32_00_31);
			R5900Regs.Reg[RD].u16_16_31 = clamp(R5900Regs.LO.u32_32_63);
			R5900Regs.Reg[RD].u16_32_47 = clamp(R5900Regs.HI.u32_00_31);
			R5900Regs.Reg[RD].u16_48_63 = clamp(R5900Regs.HI.u32_32_63);
			R5900Regs.Reg[RD].u16_64_79 = clamp(R5900Regs.LO.u32_64_95);
			R5900Regs.Reg[RD].u16_80_95 = clamp(R5900Regs.LO.u32_96_127);
			R5900Regs.Reg[RD].u16_96_111 = clamp(R5900Regs.HI.u32_64_95);
			R5900Regs.Reg[RD].u16_112_127 = clamp(R5900Regs.HI.u32_96_127);
			break;
		}

	case 0x00000031: // "pmthl.lw"
		RS = R_RS;
		R5900Regs.LO.u32_00_31 = R5900Regs.Reg[RS].u32_00_31;
		R5900Regs.HI.u32_00_31 = R5900Regs.Reg[RS].u32_32_63;
		R5900Regs.LO.u32_64_95 = R5900Regs.Reg[RS].u32_64_95;
		R5900Regs.HI.u32_64_95 = R5900Regs.Reg[RS].u32_96_127;
		break;

	case 0x00000034: // "psllh"
		RT = R_RT;
		RD = R_RD;
		SA = R_SA;
		R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15 << SA;
		R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_16_31 << SA;
		R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_32_47 << SA;
		R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_48_63 << SA;
		R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_64_79 << SA;
		R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RT].u16_80_95 << SA;
		R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_96_111 << SA;
		R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RT].u16_112_127 << SA;
		break;

	case 0x00000036: // "psrlh"
		RT = R_RT;
		RD = R_RD;
		SA = R_SA;
		R5900Regs.Reg[RD].u16_00_15 = R5900Regs.Reg[RT].u16_00_15 >> SA;
		R5900Regs.Reg[RD].u16_16_31 = R5900Regs.Reg[RT].u16_16_31 >> SA;
		R5900Regs.Reg[RD].u16_32_47 = R5900Regs.Reg[RT].u16_32_47 >> SA;
		R5900Regs.Reg[RD].u16_48_63 = R5900Regs.Reg[RT].u16_48_63 >> SA;
		R5900Regs.Reg[RD].u16_64_79 = R5900Regs.Reg[RT].u16_64_79 >> SA;
		R5900Regs.Reg[RD].u16_80_95 = R5900Regs.Reg[RT].u16_80_95 >> SA;
		R5900Regs.Reg[RD].u16_96_111 = R5900Regs.Reg[RT].u16_96_111 >> SA;
		R5900Regs.Reg[RD].u16_112_127 = R5900Regs.Reg[RT].u16_112_127 >> SA;
		break;

	case 0x00000037: // "psrah"
		RT = R_RT;
		RD = R_RD;
		SA = R_SA;
		R5900Regs.Reg[RD].u16_00_15 = (R5900Regs.Reg[RT].u16_00_15 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_00_15 >> SA)) :
			(R5900Regs.Reg[RT].u16_00_15 >> SA);
		R5900Regs.Reg[RD].u16_16_31 = (R5900Regs.Reg[RT].u16_16_31 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_16_31 >> SA)) :
			(R5900Regs.Reg[RT].u16_16_31 >> SA);
		R5900Regs.Reg[RD].u16_32_47 = (R5900Regs.Reg[RT].u16_32_47 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_32_47 >> SA)) :
			(R5900Regs.Reg[RT].u16_32_47 >> SA);
		R5900Regs.Reg[RD].u16_48_63 = (R5900Regs.Reg[RT].u16_48_63 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_48_63 >> SA)) :
			(R5900Regs.Reg[RT].u16_48_63 >> SA);
		R5900Regs.Reg[RD].u16_64_79 = (R5900Regs.Reg[RT].u16_64_79 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_64_79 >> SA)) :
			(R5900Regs.Reg[RT].u16_64_79 >> SA);
		R5900Regs.Reg[RD].u16_80_95 = (R5900Regs.Reg[RT].u16_80_95 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_80_95 >> SA)) :
			(R5900Regs.Reg[RT].u16_80_95 >> SA);
		R5900Regs.Reg[RD].u16_96_111 = (R5900Regs.Reg[RT].u16_96_111 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_96_111 >> SA)) :
			(R5900Regs.Reg[RT].u16_96_111 >> SA);
		R5900Regs.Reg[RD].u16_112_127 = (R5900Regs.Reg[RT].u16_112_127 & 0x8000) ?
			((MaxU16 << (16 - SA)) | (R5900Regs.Reg[RT].u16_112_127 >> SA)) :
			(R5900Regs.Reg[RT].u16_112_127 >> SA);
		break;

	case 0x0000003C: // "psllw"
		RT = R_RT;
		RD = R_RD;
		SA = R_SA;
		R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_00_31 << SA;
		R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RT].u32_32_63 << SA;
		R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_64_95 << SA;
		R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RT].u32_96_127 << SA;
		break;

	case 0x0000003E: // "psrlw"
		RT = R_RT;
		RD = R_RD;
		SA = R_SA;
		R5900Regs.Reg[RD].u32_00_31 = R5900Regs.Reg[RT].u32_00_31 >> SA;
		R5900Regs.Reg[RD].u32_32_63 = R5900Regs.Reg[RT].u32_32_63 >> SA;
		R5900Regs.Reg[RD].u32_64_95 = R5900Regs.Reg[RT].u32_64_95 >> SA;
		R5900Regs.Reg[RD].u32_96_127 = R5900Regs.Reg[RT].u32_96_127 >> SA;
		break;

	case 0x0000003F: // "psraw"
		RT = R_RT;
		RD = R_RD;
		SA = R_SA;
		R5900Regs.Reg[RD].u32_00_31 = (R5900Regs.Reg[RT].u32_00_31 & 0x80000000) ?
			((MaxU32 << (32 - SA)) | (R5900Regs.Reg[RT].u32_00_31 >> SA)) :
			(R5900Regs.Reg[RT].u32_00_31 >> SA);
		R5900Regs.Reg[RD].u32_32_63 = (R5900Regs.Reg[RT].u32_32_63 & 0x80000000) ?
			((MaxU32 << (32 - SA)) | (R5900Regs.Reg[RT].u32_32_63 >> SA)) :
			(R5900Regs.Reg[RT].u32_32_63 >> SA);
		R5900Regs.Reg[RD].u32_64_95 = (R5900Regs.Reg[RT].u32_64_95 & 0x80000000) ?
			((MaxU32 << (32 - SA)) | (R5900Regs.Reg[RT].u32_64_95 >> SA)) :
			(R5900Regs.Reg[RT].u32_64_95 >> SA);
		R5900Regs.Reg[RD].u32_96_127 = (R5900Regs.Reg[RT].u32_96_127 & 0x80000000) ?
			((MaxU32 << (32 - SA)) | (R5900Regs.Reg[RT].u32_96_127 >> SA)) :
			(R5900Regs.Reg[RT].u32_96_127 >> SA);
		break;
	}
	///////////////////////////////////
	// End of MMI Funcions
	///////////////////////////////////
}