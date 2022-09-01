#include "EmuExecute.h"

void emuMmi(EMU_U32 Code, stEmuTemp& emuTemp)
{
	EMU_U32 SA = 0;

	///////////////////////////////////
	// MMI Funcions
	///////////////////////////////////
	switch (Code & 0x0000003F)
	{
	case 0x00000000: // "madd"
		emuTemp.Temp128.u64_00_63 = (EMU_I64)(((EMU_I64)R5900Regs.HI.u32_00_31 << 32) | (EMU_I64)R5900Regs.LO.u32_00_31) +
			((EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31);
		R5900Regs.LO.u64_00_63 = emuTemp.Temp128.i32_00_31;
		R5900Regs.HI.u64_00_63 = emuTemp.Temp128.i32_32_63;
		if (R_RD)
		{
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
		}
		break;

	case 0x00000001: // "maddu"
		emuTemp.Temp128.u64_00_63 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31) +
			((EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31);
		R5900Regs.LO.u64_00_63 = emuTemp.Temp128.i32_00_31;
		R5900Regs.HI.u64_00_63 = emuTemp.Temp128.i32_32_63;
		if (R_RD)
		{
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
		}
		break;

	case 0x00000004: // "plzcw"
		R5900Regs.Reg[R_RD].u32_00_31 = 0;
		R5900Regs.Reg[R_RD].u32_32_63 = 0;
		emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_00_31;
		while ((emuTemp.TempU32 & 0x80000000) ==
			(R5900Regs.Reg[R_RS].u32_00_31 & 0x80000000))
		{
			R5900Regs.Reg[R_RD].u32_00_31++;
			emuTemp.TempU32 <<= 1;
		}
		emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_32_63;
		while ((emuTemp.TempU32 & 0x80000000) ==
			(R5900Regs.Reg[R_RS].u32_32_63 & 0x80000000))
		{
			R5900Regs.Reg[R_RD].u32_32_63++;
			emuTemp.TempU32 <<= 1;
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
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RS].u32_00_31 + R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RS].u32_32_63 + R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RS].u32_64_95 + R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RS].u32_96_127 + R5900Regs.Reg[R_RT].u32_96_127;
			break;

		case 0x00000040: // "psubw"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RS].u32_00_31 - R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RS].u32_32_63 - R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RS].u32_64_95 - R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RS].u32_96_127 - R5900Regs.Reg[R_RT].u32_96_127;
			break;

		case 0x00000080: // "pcgtw"
			R5900Regs.Reg[R_RD].u32_00_31 = (R5900Regs.Reg[R_RS].u32_00_31 > R5900Regs.Reg[R_RT].u32_00_31) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[R_RD].u32_32_63 = (R5900Regs.Reg[R_RS].u32_32_63 > R5900Regs.Reg[R_RT].u32_32_63) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[R_RD].u32_64_95 = (R5900Regs.Reg[R_RS].u32_64_95 > R5900Regs.Reg[R_RT].u32_64_95) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[R_RD].u32_96_127 = (R5900Regs.Reg[R_RS].u32_96_127 > R5900Regs.Reg[R_RT].u32_96_127) ? 0xFFFFFFFF : 0;
			break;

		case 0x000000C0: // "pmaxw"
			R5900Regs.Reg[R_RD].u32_00_31 = (R5900Regs.Reg[R_RS].i32_00_31 > R5900Regs.Reg[R_RT].i32_00_31) ?
				R5900Regs.Reg[R_RS].u32_00_31 : R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = (R5900Regs.Reg[R_RS].i32_32_63 > R5900Regs.Reg[R_RT].i32_32_63) ?
				R5900Regs.Reg[R_RS].u32_32_63 : R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_64_95 = (R5900Regs.Reg[R_RS].i32_64_95 > R5900Regs.Reg[R_RT].i32_64_95) ?
				R5900Regs.Reg[R_RS].u32_64_95 : R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_96_127 = (R5900Regs.Reg[R_RS].i32_96_127 > R5900Regs.Reg[R_RT].i32_96_127) ?
				R5900Regs.Reg[R_RS].u32_96_127 : R5900Regs.Reg[R_RT].u32_96_127;
			break;

		case 0x00000100: // "paddh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RS].u16_00_15 + R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_16_31 + R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RS].u16_32_47 + R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_48_63 + R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RS].u16_64_79 + R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_80_95 + R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RS].u16_96_111 + R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_112_127 + R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x00000140: // "psubh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RS].u16_00_15 - R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_16_31 - R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RS].u16_32_47 - R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_48_63 - R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RS].u16_64_79 - R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_80_95 - R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RS].u16_96_111 - R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_112_127 - R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x00000180: // "pcgth"
			R5900Regs.Reg[R_RD].u16_00_15 = (R5900Regs.Reg[R_RS].u16_00_15 > R5900Regs.Reg[R_RT].u16_00_15) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_16_31 = (R5900Regs.Reg[R_RS].u16_16_31 > R5900Regs.Reg[R_RT].u16_16_31) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_32_47 = (R5900Regs.Reg[R_RS].u16_32_47 > R5900Regs.Reg[R_RT].u16_32_47) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_48_63 = (R5900Regs.Reg[R_RS].u16_48_63 > R5900Regs.Reg[R_RT].u16_48_63) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_64_79 = (R5900Regs.Reg[R_RS].u16_64_79 > R5900Regs.Reg[R_RT].u16_64_79) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_80_95 = (R5900Regs.Reg[R_RS].u16_80_95 > R5900Regs.Reg[R_RT].u16_80_95) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_96_111 = (R5900Regs.Reg[R_RS].u16_96_111 > R5900Regs.Reg[R_RT].u16_96_111) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_112_127 = (R5900Regs.Reg[R_RS].u16_112_127 > R5900Regs.Reg[R_RT].u16_112_127) ? 0xFFFF : 0;
			break;

		case 0x000001C0: // "pmaxh"
			R5900Regs.Reg[R_RD].u16_00_15 = (R5900Regs.Reg[R_RS].i16_00_15 > R5900Regs.Reg[R_RT].i16_00_15) ?
				R5900Regs.Reg[R_RS].u16_00_15 : R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = (R5900Regs.Reg[R_RS].i16_16_31 > R5900Regs.Reg[R_RT].i16_16_31) ?
				R5900Regs.Reg[R_RS].u16_16_31 : R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_32_47 = (R5900Regs.Reg[R_RS].i16_32_47 > R5900Regs.Reg[R_RT].i16_32_47) ?
				R5900Regs.Reg[R_RS].u16_32_47 : R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_48_63 = (R5900Regs.Reg[R_RS].i16_48_63 > R5900Regs.Reg[R_RT].i16_48_63) ?
				R5900Regs.Reg[R_RS].u16_48_63 : R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = (R5900Regs.Reg[R_RS].i16_64_79 > R5900Regs.Reg[R_RT].i16_64_79) ?
				R5900Regs.Reg[R_RS].u16_64_79 : R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = (R5900Regs.Reg[R_RS].i16_80_95 > R5900Regs.Reg[R_RT].i16_80_95) ?
				R5900Regs.Reg[R_RS].u16_80_95 : R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_96_111 = (R5900Regs.Reg[R_RS].i16_96_111 > R5900Regs.Reg[R_RT].i16_96_111) ?
				R5900Regs.Reg[R_RS].u16_96_111 : R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_112_127 = (R5900Regs.Reg[R_RS].i16_112_127 > R5900Regs.Reg[R_RT].i16_112_127) ?
				R5900Regs.Reg[R_RS].u16_112_127 : R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x00000200: // "paddb"
			R5900Regs.Reg[R_RD].u08_00_07 = R5900Regs.Reg[R_RS].u08_00_07 + R5900Regs.Reg[R_RT].u08_00_07;
			R5900Regs.Reg[R_RD].u08_08_15 = R5900Regs.Reg[R_RS].u08_08_15 + R5900Regs.Reg[R_RT].u08_08_15;
			R5900Regs.Reg[R_RD].u08_16_23 = R5900Regs.Reg[R_RS].u08_16_23 + R5900Regs.Reg[R_RT].u08_16_23;
			R5900Regs.Reg[R_RD].u08_24_31 = R5900Regs.Reg[R_RS].u08_24_31 + R5900Regs.Reg[R_RT].u08_24_31;
			R5900Regs.Reg[R_RD].u08_32_39 = R5900Regs.Reg[R_RS].u08_32_39 + R5900Regs.Reg[R_RT].u08_32_39;
			R5900Regs.Reg[R_RD].u08_40_47 = R5900Regs.Reg[R_RS].u08_40_47 + R5900Regs.Reg[R_RT].u08_40_47;
			R5900Regs.Reg[R_RD].u08_48_55 = R5900Regs.Reg[R_RS].u08_48_55 + R5900Regs.Reg[R_RT].u08_48_55;
			R5900Regs.Reg[R_RD].u08_56_63 = R5900Regs.Reg[R_RS].u08_56_63 + R5900Regs.Reg[R_RT].u08_56_63;
			R5900Regs.Reg[R_RD].u08_64_71 = R5900Regs.Reg[R_RS].u08_64_71 + R5900Regs.Reg[R_RT].u08_64_71;
			R5900Regs.Reg[R_RD].u08_72_79 = R5900Regs.Reg[R_RS].u08_72_79 + R5900Regs.Reg[R_RT].u08_72_79;
			R5900Regs.Reg[R_RD].u08_80_87 = R5900Regs.Reg[R_RS].u08_80_87 + R5900Regs.Reg[R_RT].u08_80_87;
			R5900Regs.Reg[R_RD].u08_88_95 = R5900Regs.Reg[R_RS].u08_88_95 + R5900Regs.Reg[R_RT].u08_88_95;
			R5900Regs.Reg[R_RD].u08_96_103 = R5900Regs.Reg[R_RS].u08_96_103 + R5900Regs.Reg[R_RT].u08_96_103;
			R5900Regs.Reg[R_RD].u08_104_111 = R5900Regs.Reg[R_RS].u08_104_111 + R5900Regs.Reg[R_RT].u08_104_111;
			R5900Regs.Reg[R_RD].u08_112_119 = R5900Regs.Reg[R_RS].u08_112_119 + R5900Regs.Reg[R_RT].u08_112_119;
			R5900Regs.Reg[R_RD].u08_120_127 = R5900Regs.Reg[R_RS].u08_120_127 + R5900Regs.Reg[R_RT].u08_120_127;
			break;

		case 0x00000240: // "psubb"
			R5900Regs.Reg[R_RD].u08_00_07 = R5900Regs.Reg[R_RS].u08_00_07 - R5900Regs.Reg[R_RT].u08_00_07;
			R5900Regs.Reg[R_RD].u08_08_15 = R5900Regs.Reg[R_RS].u08_08_15 - R5900Regs.Reg[R_RT].u08_08_15;
			R5900Regs.Reg[R_RD].u08_16_23 = R5900Regs.Reg[R_RS].u08_16_23 - R5900Regs.Reg[R_RT].u08_16_23;
			R5900Regs.Reg[R_RD].u08_24_31 = R5900Regs.Reg[R_RS].u08_24_31 - R5900Regs.Reg[R_RT].u08_24_31;
			R5900Regs.Reg[R_RD].u08_32_39 = R5900Regs.Reg[R_RS].u08_32_39 - R5900Regs.Reg[R_RT].u08_32_39;
			R5900Regs.Reg[R_RD].u08_40_47 = R5900Regs.Reg[R_RS].u08_40_47 - R5900Regs.Reg[R_RT].u08_40_47;
			R5900Regs.Reg[R_RD].u08_48_55 = R5900Regs.Reg[R_RS].u08_48_55 - R5900Regs.Reg[R_RT].u08_48_55;
			R5900Regs.Reg[R_RD].u08_56_63 = R5900Regs.Reg[R_RS].u08_56_63 - R5900Regs.Reg[R_RT].u08_56_63;
			R5900Regs.Reg[R_RD].u08_64_71 = R5900Regs.Reg[R_RS].u08_64_71 - R5900Regs.Reg[R_RT].u08_64_71;
			R5900Regs.Reg[R_RD].u08_72_79 = R5900Regs.Reg[R_RS].u08_72_79 - R5900Regs.Reg[R_RT].u08_72_79;
			R5900Regs.Reg[R_RD].u08_80_87 = R5900Regs.Reg[R_RS].u08_80_87 - R5900Regs.Reg[R_RT].u08_80_87;
			R5900Regs.Reg[R_RD].u08_88_95 = R5900Regs.Reg[R_RS].u08_88_95 - R5900Regs.Reg[R_RT].u08_88_95;
			R5900Regs.Reg[R_RD].u08_96_103 = R5900Regs.Reg[R_RS].u08_96_103 - R5900Regs.Reg[R_RT].u08_96_103;
			R5900Regs.Reg[R_RD].u08_104_111 = R5900Regs.Reg[R_RS].u08_104_111 - R5900Regs.Reg[R_RT].u08_104_111;
			R5900Regs.Reg[R_RD].u08_112_119 = R5900Regs.Reg[R_RS].u08_112_119 - R5900Regs.Reg[R_RT].u08_112_119;
			R5900Regs.Reg[R_RD].u08_120_127 = R5900Regs.Reg[R_RS].u08_120_127 - R5900Regs.Reg[R_RT].u08_120_127;
			break;

		case 0x00000280: // "pcgtb"
			R5900Regs.Reg[R_RD].u08_00_07 = (R5900Regs.Reg[R_RS].u08_00_07 > R5900Regs.Reg[R_RT].u08_00_07) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_08_15 = (R5900Regs.Reg[R_RS].u08_08_15 > R5900Regs.Reg[R_RT].u08_08_15) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_16_23 = (R5900Regs.Reg[R_RS].u08_16_23 > R5900Regs.Reg[R_RT].u08_16_23) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_24_31 = (R5900Regs.Reg[R_RS].u08_24_31 > R5900Regs.Reg[R_RT].u08_24_31) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_32_39 = (R5900Regs.Reg[R_RS].u08_32_39 > R5900Regs.Reg[R_RT].u08_32_39) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_40_47 = (R5900Regs.Reg[R_RS].u08_40_47 > R5900Regs.Reg[R_RT].u08_40_47) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_48_55 = (R5900Regs.Reg[R_RS].u08_48_55 > R5900Regs.Reg[R_RT].u08_48_55) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_56_63 = (R5900Regs.Reg[R_RS].u08_56_63 > R5900Regs.Reg[R_RT].u08_56_63) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_64_71 = (R5900Regs.Reg[R_RS].u08_64_71 > R5900Regs.Reg[R_RT].u08_64_71) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_72_79 = (R5900Regs.Reg[R_RS].u08_72_79 > R5900Regs.Reg[R_RT].u08_72_79) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_80_87 = (R5900Regs.Reg[R_RS].u08_80_87 > R5900Regs.Reg[R_RT].u08_80_87) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_88_95 = (R5900Regs.Reg[R_RS].u08_88_95 > R5900Regs.Reg[R_RT].u08_88_95) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_96_103 = (R5900Regs.Reg[R_RS].u08_96_103 > R5900Regs.Reg[R_RT].u08_96_103) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_104_111 = (R5900Regs.Reg[R_RS].u08_104_111 > R5900Regs.Reg[R_RT].u08_104_111) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_112_119 = (R5900Regs.Reg[R_RS].u08_112_119 > R5900Regs.Reg[R_RT].u08_112_119) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_120_127 = (R5900Regs.Reg[R_RS].u08_120_127 > R5900Regs.Reg[R_RT].u08_120_127) ? 0xFF : 0;
			break;

		case 0x00000400: // "paddsw"
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_00_31 + R5900Regs.Reg[R_RT].i32_00_31;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_00_31 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_00_31 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_00_31 = emuTemp.Temp128.u32_00_31;
				}
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_32_63 + R5900Regs.Reg[R_RT].i32_32_63;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_32_63 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_32_63 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_32_63 = emuTemp.Temp128.u32_00_31;
				}
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_64_95 + R5900Regs.Reg[R_RT].i32_64_95;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_64_95 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_64_95 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_64_95 = emuTemp.Temp128.u32_00_31;
				}
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_96_127 + R5900Regs.Reg[R_RT].i32_96_127;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_96_127 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_96_127 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_96_127 = emuTemp.Temp128.u32_00_31;
				}
			break;

		case 0x00000440: // "psubsw"
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_00_31 - R5900Regs.Reg[R_RT].i32_00_31;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_00_31 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_00_31 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_00_31 = emuTemp.Temp128.u32_00_31;
				}
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_32_63 - R5900Regs.Reg[R_RT].i32_32_63;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_32_63 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_32_63 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_32_63 = emuTemp.Temp128.u32_00_31;
				}
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_64_95 - R5900Regs.Reg[R_RT].i32_64_95;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_64_95 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_64_95 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_64_95 = emuTemp.Temp128.u32_00_31;
				}
			emuTemp.Temp128.i64_00_63 = R5900Regs.Reg[R_RS].i32_96_127 - R5900Regs.Reg[R_RT].i32_96_127;
			if (emuTemp.Temp128.i64_00_63 > 0x7FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u32_96_127 = 0x7FFFFFFF;
			}
			else
				if ((0x10000000 <= emuTemp.Temp128.i64_00_63) && (emuTemp.Temp128.i64_00_63 < 0x80000000))
				{
					R5900Regs.Reg[R_RD].u32_96_127 = 0x80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u32_96_127 = emuTemp.Temp128.u32_00_31;
				}
			break;

		case 0x00000480: // "pextlw"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RS].u32_00_31;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RS].u32_32_63;
			break;

		case 0x000004C0: // "ppacw"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RS].u32_00_31;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RS].u32_64_95;
			break;

		case 0x00000500: // "paddsh"
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_00_15 + R5900Regs.Reg[R_RT].i16_00_15;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_00_15 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_00_15 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_00_15 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_16_31 + R5900Regs.Reg[R_RT].i16_16_31;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_16_31 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_16_31 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_16_31 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_32_47 + R5900Regs.Reg[R_RT].i16_32_47;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].i16_32_47 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_32_47 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_32_47 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_48_63 + R5900Regs.Reg[R_RT].i16_48_63;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_48_63 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_48_63 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_48_63 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_64_79 + R5900Regs.Reg[R_RT].i16_64_79;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_64_79 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_64_79 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_64_79 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_80_95 + R5900Regs.Reg[R_RT].i16_80_95;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_80_95 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_80_95 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_80_95 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_96_111 + R5900Regs.Reg[R_RT].i16_96_111;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_96_111 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_96_111 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_96_111 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_112_127 + R5900Regs.Reg[R_RT].i16_112_127;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_112_127 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_112_127 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_112_127 = emuTemp.Temp128.u16_00_15;
				}
			break;

		case 0x00000540: // "psubsh"
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_00_15 - R5900Regs.Reg[R_RT].i16_00_15;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_00_15 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_00_15 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_00_15 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_16_31 - R5900Regs.Reg[R_RT].i16_16_31;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_16_31 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_16_31 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_16_31 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_32_47 - R5900Regs.Reg[R_RT].i16_32_47;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_32_47 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_32_47 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_32_47 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_48_63 - R5900Regs.Reg[R_RT].i16_48_63;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_48_63 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_48_63 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_48_63 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_64_79 - R5900Regs.Reg[R_RT].i16_64_79;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_64_79 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_64_79 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_64_79 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_80_95 - R5900Regs.Reg[R_RT].i16_80_95;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_80_95 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_80_95 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_80_95 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_96_111 - R5900Regs.Reg[R_RT].i16_96_111;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_96_111 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_96_111 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_96_111 = emuTemp.Temp128.u16_00_15;
				}
			emuTemp.Temp128.i32_00_31 = R5900Regs.Reg[R_RS].i16_112_127 - R5900Regs.Reg[R_RT].i16_112_127;
			if (emuTemp.Temp128.i32_00_31 > 0x7FFF)
			{
				R5900Regs.Reg[R_RD].u16_112_127 = 0x7FFF;
			}
			else
				if ((0x10000 <= emuTemp.Temp128.i32_00_31) && (emuTemp.Temp128.i32_00_31 < 0x18000))
				{
					R5900Regs.Reg[R_RD].u16_112_127 = 0x8000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u16_112_127 = emuTemp.Temp128.u16_00_15;
				}
			break;

		case 0x00000580: // "pextlh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_00_15;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_16_31;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_32_47;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_48_63;
			break;

		case 0x000005C0: // "ppach"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RS].u16_00_15;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_32_47;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RS].u16_64_79;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_96_111;
			break;

			/////////////////////////////////////////////////////////////////////////
		case 0x00000600: // "paddsb"
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_00_07 + R5900Regs.Reg[R_RT].i08_00_07;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_00_07 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_08_15 + R5900Regs.Reg[R_RT].i08_08_15;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_08_15 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_16_23 + R5900Regs.Reg[R_RT].i08_16_23;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_16_23 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_24_31 + R5900Regs.Reg[R_RT].i08_24_31;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_24_31 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_32_39 + R5900Regs.Reg[R_RT].i08_32_39;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_32_39 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_40_47 + R5900Regs.Reg[R_RT].i08_40_47;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_40_47 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_48_55 + R5900Regs.Reg[R_RT].i08_48_55;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_48_55 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_56_63 + R5900Regs.Reg[R_RT].i08_56_63;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_56_63 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_64_71 + R5900Regs.Reg[R_RT].i08_64_71;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_64_71 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_72_79 + R5900Regs.Reg[R_RT].i08_72_79;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_72_79 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_80_87 + R5900Regs.Reg[R_RT].i08_80_87;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_80_87 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_88_95 + R5900Regs.Reg[R_RT].i08_88_95;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_88_95 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_96_103 + R5900Regs.Reg[R_RT].i08_96_103;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_96_103 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_104_111 + R5900Regs.Reg[R_RT].i08_104_111;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_104_111 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_112_119 + R5900Regs.Reg[R_RT].i08_112_119;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_112_119 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_120_127 + R5900Regs.Reg[R_RT].i08_120_127;
			if (emuTemp.TempI32 > 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_120_127 = emuTemp.TempI32;
			}
			break;

		case 0x00000640: // "psubsb"
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_00_07 - R5900Regs.Reg[R_RT].i08_00_07;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_00_07 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_08_15 - R5900Regs.Reg[R_RT].i08_08_15;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_08_15 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_16_23 - R5900Regs.Reg[R_RT].i08_16_23;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_16_23 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_24_31 - R5900Regs.Reg[R_RT].i08_24_31;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_24_31 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_32_39 - R5900Regs.Reg[R_RT].i08_32_39;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_32_39 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_40_47 - R5900Regs.Reg[R_RT].i08_40_47;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_40_47 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_48_55 - R5900Regs.Reg[R_RT].i08_48_55;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_48_55 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_56_63 - R5900Regs.Reg[R_RT].i08_56_63;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_56_63 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_64_71 - R5900Regs.Reg[R_RT].i08_64_71;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_64_71 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_72_79 - R5900Regs.Reg[R_RT].i08_72_79;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_72_79 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_80_87 - R5900Regs.Reg[R_RT].i08_80_87;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_80_87 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_88_95 - R5900Regs.Reg[R_RT].i08_88_95;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_88_95 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_96_103 - R5900Regs.Reg[R_RT].i08_96_103;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_96_103 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_104_111 - R5900Regs.Reg[R_RT].i08_104_111;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_104_111 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_112_119 - R5900Regs.Reg[R_RT].i08_112_119;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_112_119 = emuTemp.TempI32;
			}
			emuTemp.TempI32 = R5900Regs.Reg[R_RS].i08_120_127 - R5900Regs.Reg[R_RT].i08_120_127;
			if (emuTemp.TempI32 >= 0x7f)
			{
				R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x7f;
			}
			else if (0x100 <= emuTemp.TempI32 && emuTemp.TempI32 < 0x180)
			{
				R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x80;
			}
			else
			{
				R5900Regs.Reg[R_RD].i08_120_127 = emuTemp.TempI32;
			}
			break;

		case 0x00000680: // "pextlb"
			R5900Regs.Reg[R_RD].u08_00_07 = R5900Regs.Reg[R_RT].u08_00_07;
			R5900Regs.Reg[R_RD].u08_08_15 = R5900Regs.Reg[R_RS].u08_00_07;
			R5900Regs.Reg[R_RD].u08_16_23 = R5900Regs.Reg[R_RT].u08_08_15;
			R5900Regs.Reg[R_RD].u08_24_31 = R5900Regs.Reg[R_RS].u08_08_15;
			R5900Regs.Reg[R_RD].u08_32_39 = R5900Regs.Reg[R_RT].u08_16_23;
			R5900Regs.Reg[R_RD].u08_40_47 = R5900Regs.Reg[R_RS].u08_16_23;
			R5900Regs.Reg[R_RD].u08_48_55 = R5900Regs.Reg[R_RT].u08_24_31;
			R5900Regs.Reg[R_RD].u08_56_63 = R5900Regs.Reg[R_RS].u08_24_31;
			R5900Regs.Reg[R_RD].u08_64_71 = R5900Regs.Reg[R_RT].u08_32_39;
			R5900Regs.Reg[R_RD].u08_72_79 = R5900Regs.Reg[R_RS].u08_32_39;
			R5900Regs.Reg[R_RD].u08_80_87 = R5900Regs.Reg[R_RT].u08_40_47;
			R5900Regs.Reg[R_RD].u08_88_95 = R5900Regs.Reg[R_RS].u08_40_47;
			R5900Regs.Reg[R_RD].u08_96_103 = R5900Regs.Reg[R_RT].u08_48_55;
			R5900Regs.Reg[R_RD].u08_104_111 = R5900Regs.Reg[R_RS].u08_48_55;
			R5900Regs.Reg[R_RD].u08_112_119 = R5900Regs.Reg[R_RT].u08_56_63;
			R5900Regs.Reg[R_RD].u08_120_127 = R5900Regs.Reg[R_RS].u08_56_63;
			break;

		case 0x000006C0: // "ppacb"
			R5900Regs.Reg[R_RD].u08_00_07 = R5900Regs.Reg[R_RT].u08_00_07;
			R5900Regs.Reg[R_RD].u08_08_15 = R5900Regs.Reg[R_RT].u08_16_23;
			R5900Regs.Reg[R_RD].u08_16_23 = R5900Regs.Reg[R_RT].u08_32_39;
			R5900Regs.Reg[R_RD].u08_24_31 = R5900Regs.Reg[R_RT].u08_48_55;
			R5900Regs.Reg[R_RD].u08_32_39 = R5900Regs.Reg[R_RT].u08_64_71;
			R5900Regs.Reg[R_RD].u08_40_47 = R5900Regs.Reg[R_RT].u08_80_87;
			R5900Regs.Reg[R_RD].u08_48_55 = R5900Regs.Reg[R_RT].u08_96_103;
			R5900Regs.Reg[R_RD].u08_56_63 = R5900Regs.Reg[R_RT].u08_112_119;
			R5900Regs.Reg[R_RD].u08_64_71 = R5900Regs.Reg[R_RS].u08_00_07;
			R5900Regs.Reg[R_RD].u08_72_79 = R5900Regs.Reg[R_RS].u08_16_23;
			R5900Regs.Reg[R_RD].u08_80_87 = R5900Regs.Reg[R_RS].u08_32_39;
			R5900Regs.Reg[R_RD].u08_88_95 = R5900Regs.Reg[R_RS].u08_48_55;
			R5900Regs.Reg[R_RD].u08_96_103 = R5900Regs.Reg[R_RS].u08_64_71;
			R5900Regs.Reg[R_RD].u08_104_111 = R5900Regs.Reg[R_RS].u08_80_87;
			R5900Regs.Reg[R_RD].u08_112_119 = R5900Regs.Reg[R_RS].u08_96_103;
			R5900Regs.Reg[R_RD].u08_120_127 = R5900Regs.Reg[R_RS].u08_112_119;
			break;

		case 0x00000780: // "pext5"
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u32_00_31 = ((emuTemp.TempU32 & 0x8000) << 16) | ((emuTemp.TempU32 & 0x7c00) << 9) |
				((emuTemp.TempU32 & 0x03e0) << 6) | ((emuTemp.TempU32 & 0x001f) << 3);
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u32_32_63 = ((emuTemp.TempU32 & 0x8000) << 16) | ((emuTemp.TempU32 & 0x7c00) << 9) |
				((emuTemp.TempU32 & 0x03e0) << 6) | ((emuTemp.TempU32 & 0x001f) << 3);
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u32_64_95 = ((emuTemp.TempU32 & 0x8000) << 16) | ((emuTemp.TempU32 & 0x7c00) << 9) |
				((emuTemp.TempU32 & 0x03e0) << 6) | ((emuTemp.TempU32 & 0x001f) << 3);
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u32_96_127 = ((emuTemp.TempU32 & 0x8000) << 16) | ((emuTemp.TempU32 & 0x7c00) << 9) |
				((emuTemp.TempU32 & 0x03e0) << 6) | ((emuTemp.TempU32 & 0x001f) << 3);
			break;

		case 0x000007C0: // "ppac5"
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_00_31 = ((emuTemp.TempU32 & 0x80000000) >> 16) | ((emuTemp.TempU32 & 0x00f80000) >> 9) |
				((emuTemp.TempU32 & 0x0000f800) >> 6) | ((emuTemp.TempU32 & 0x000000f8) >> 3);
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_32_63 = ((emuTemp.TempU32 & 0x80000000) >> 16) | ((emuTemp.TempU32 & 0x00f80000) >> 9) |
				((emuTemp.TempU32 & 0x0000f800) >> 6) | ((emuTemp.TempU32 & 0x000000f8) >> 3);
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_64_95 = ((emuTemp.TempU32 & 0x80000000) >> 16) | ((emuTemp.TempU32 & 0x00f80000) >> 9) |
				((emuTemp.TempU32 & 0x0000f800) >> 6) | ((emuTemp.TempU32 & 0x000000f8) >> 3);
			emuTemp.TempU32 = R5900Regs.Reg[R_RT].u32_96_127;
			R5900Regs.Reg[R_RD].u32_96_127 = ((emuTemp.TempU32 & 0x80000000) >> 16) | ((emuTemp.TempU32 & 0x00f80000) >> 9) |
				((emuTemp.TempU32 & 0x0000f800) >> 6) | ((emuTemp.TempU32 & 0x000000f8) >> 3);
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
			emuTemp.TempI64 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31) +
				((EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31);
			R5900Regs.LO.u64_00_63 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)(emuTemp.TempI64 >> 32);
			R5900Regs.Reg[R_RD].u64_00_63 = emuTemp.TempI64;

			emuTemp.TempI64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95) +
				((EMU_I64)R5900Regs.Reg[R_RS].i32_64_95 * (EMU_I64)R5900Regs.Reg[R_RT].i32_64_95);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)(emuTemp.TempI64 >> 32);
			R5900Regs.Reg[R_RD].u64_64_127 = emuTemp.TempI64;
			break;

		case 0x00000080: // "psllvw"
			R5900Regs.Reg[R_RD].i64_00_63 = (EMU_I64)(R5900Regs.Reg[R_RT].i32_00_31 <<
				(R5900Regs.Reg[R_RS].u32_00_31 & 0x1F));
			R5900Regs.Reg[R_RD].i64_64_127 = (EMU_I64)(R5900Regs.Reg[R_RT].i32_64_95 <<
				(R5900Regs.Reg[R_RS].u32_64_95 & 0x1F));
			break;

		case 0x000000C0: // "psrlvw"
			R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I64)(R5900Regs.Reg[R_RT].i32_00_31 >>
				(R5900Regs.Reg[R_RS].u32_00_31 & 0x1F));
			R5900Regs.Reg[R_RD].u64_64_127 = (EMU_I64)(R5900Regs.Reg[R_RT].i32_64_95 >>
				(R5900Regs.Reg[R_RS].u32_64_95 & 0x1F));
			break;

		case 0x00000100: // "pmsubw"
			emuTemp.TempI64 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31) -
				((EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31);
			R5900Regs.LO.u64_00_63 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((emuTemp.TempI64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[R_RD].u64_00_63 = emuTemp.TempI64;
			emuTemp.TempI64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95) -
				((EMU_I64)R5900Regs.Reg[R_RS].i32_64_95 * (EMU_I64)R5900Regs.Reg[R_RT].i32_64_95);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((emuTemp.TempI64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[R_RD].u64_64_127 = emuTemp.TempI64;
			break;

		case 0x00000200: // "pmfhi",
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.HI.u64_00_63;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.HI.u64_64_127;
			break;

		case 0x00000240: // "pmflo"
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.LO.u64_64_127;
			break;

		case 0x00000280: // "pinth"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_64_79;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_80_95;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_96_111;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_112_127;
			break;

		case 0x00000300: // "pmultw"
			emuTemp.TempI64 = R5900Regs.Reg[R_RS].i32_00_31 * R5900Regs.Reg[R_RT].i32_00_31;
			R5900Regs.LO.u64_00_63 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((emuTemp.TempI64 >> 32) & 0x00000000FFFFFFFF);
			if (R_RD)
			{
				R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
			}
			emuTemp.TempI64 = R5900Regs.Reg[R_RS].i32_64_95 * R5900Regs.Reg[R_RT].i32_64_95;
			R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((emuTemp.TempI64 >> 32) & 0x00000000FFFFFFFF);
			if (R_RD)
			{
				R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.LO.u64_64_127;
			}
			break;

		case 0x00000340: // "pdivw"
			R5900Regs.LO.u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].i32_00_31 /
				R5900Regs.Reg[R_RT].i32_00_31);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(R5900Regs.Reg[R_RS].i32_64_95 /
				R5900Regs.Reg[R_RT].i32_64_95);
			R5900Regs.HI.u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].i32_00_31 %
				R5900Regs.Reg[R_RT].i32_00_31);
			R5900Regs.HI.u64_64_127 = (EMU_I32)(R5900Regs.Reg[R_RS].i32_64_95 %
				R5900Regs.Reg[R_RT].i32_64_95);
			break;

		case 0x00000380: // "pcpyld"
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.Reg[R_RS].u64_00_63;
			break;

		case 0x00000400: // "pmaddh"
			R5900Regs.LO.u32_00_31 = (EMU_I32)(R5900Regs.LO.u32_00_31 +
				(R5900Regs.Reg[R_RS].i16_00_15 * R5900Regs.Reg[R_RT].i16_00_15));
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.LO.u32_32_63 = (EMU_I32)(R5900Regs.LO.u32_32_63 +
				(R5900Regs.Reg[R_RS].i16_16_31 * R5900Regs.Reg[R_RT].i16_16_31));
			R5900Regs.HI.u32_00_31 = (EMU_I32)(R5900Regs.HI.u32_00_31 +
				(R5900Regs.Reg[R_RS].i16_32_47 * R5900Regs.Reg[R_RT].i16_32_47));
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.HI.u32_32_63 = (EMU_I32)(R5900Regs.HI.u32_32_63 +
				(R5900Regs.Reg[R_RS].i16_48_63 * R5900Regs.Reg[R_RT].i16_48_63));
			R5900Regs.LO.u32_64_95 = (EMU_I32)(R5900Regs.LO.u32_64_95 +
				(R5900Regs.Reg[R_RS].i16_64_79 * R5900Regs.Reg[R_RT].i16_64_79));
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.LO.u32_96_127 = (EMU_I32)(R5900Regs.LO.u32_96_127 +
				(R5900Regs.Reg[R_RS].i16_80_95 * R5900Regs.Reg[R_RT].i16_80_95));
			R5900Regs.HI.u32_64_95 = (EMU_I32)(R5900Regs.HI.u32_64_95 +
				(R5900Regs.Reg[R_RS].i16_96_111 * R5900Regs.Reg[R_RT].i16_96_111));
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			R5900Regs.HI.u32_96_127 = (EMU_I32)(R5900Regs.HI.u32_96_127 +
				(R5900Regs.Reg[R_RS].i16_112_127 * R5900Regs.Reg[R_RT].i16_112_127));
			break;

		case 0x00000440: // "phmadh"
			R5900Regs.LO.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_16_31 * R5900Regs.Reg[R_RT].i16_16_31) +
				(R5900Regs.Reg[R_RS].i16_00_15 * R5900Regs.Reg[R_RT].i16_00_15));
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.HI.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_48_63 * R5900Regs.Reg[R_RT].i16_48_63) +
				(R5900Regs.Reg[R_RS].i16_32_47 * R5900Regs.Reg[R_RT].i16_32_47));
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.LO.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_80_95 * R5900Regs.Reg[R_RT].i16_80_95) +
				(R5900Regs.Reg[R_RS].i16_64_79 * R5900Regs.Reg[R_RT].i16_64_79));
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.HI.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_112_127 * R5900Regs.Reg[R_RT].i16_112_127) +
				(R5900Regs.Reg[R_RS].i16_96_111 * R5900Regs.Reg[R_RT].i16_96_111));
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			break;

		case 0x00000480: // "pand" 
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 & R5900Regs.Reg[R_RT].u64_00_63;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.Reg[R_RS].u64_64_127 & R5900Regs.Reg[R_RT].u64_64_127;
			break;

		case 0x000004C0: // "pxor" 
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 ^ R5900Regs.Reg[R_RT].u64_00_63;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.Reg[R_RS].u64_64_127 ^ R5900Regs.Reg[R_RT].u64_64_127;
			break;

		case 0x00000500: // "pmsubh"
			R5900Regs.LO.u32_00_31 = (EMU_I32)(R5900Regs.LO.u32_00_31 -
				(R5900Regs.Reg[R_RS].i16_00_15 * R5900Regs.Reg[R_RT].i16_00_15));
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.LO.u32_32_63 = (EMU_I32)(R5900Regs.LO.u32_32_63 -
				(R5900Regs.Reg[R_RS].i16_16_31 * R5900Regs.Reg[R_RT].i16_16_31));
			R5900Regs.HI.u32_00_31 = (EMU_I32)(R5900Regs.HI.u32_00_31 -
				(R5900Regs.Reg[R_RS].i16_32_47 * R5900Regs.Reg[R_RT].i16_32_47));
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.HI.u32_32_63 = (EMU_I32)(R5900Regs.HI.u32_32_63 -
				(R5900Regs.Reg[R_RS].i16_48_63 * R5900Regs.Reg[R_RT].i16_48_63));
			R5900Regs.LO.u32_64_95 = (EMU_I32)(R5900Regs.LO.u32_64_95 -
				(R5900Regs.Reg[R_RS].i16_64_79 * R5900Regs.Reg[R_RT].i16_64_79));
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.LO.u32_96_127 = (EMU_I32)(R5900Regs.LO.u32_96_127 -
				(R5900Regs.Reg[R_RS].i16_80_95 * R5900Regs.Reg[R_RT].i16_80_95));
			R5900Regs.HI.u32_64_95 = (EMU_I32)(R5900Regs.HI.u32_64_95 -
				(R5900Regs.Reg[R_RS].i16_96_111 * R5900Regs.Reg[R_RT].i16_96_111));
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			R5900Regs.HI.u32_96_127 = (EMU_I32)(R5900Regs.HI.u32_96_127 -
				(R5900Regs.Reg[R_RS].i16_112_127 * R5900Regs.Reg[R_RT].i16_112_127));
			break;

		case 0x00000540: // "phmsbh"
			R5900Regs.LO.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_16_31 * R5900Regs.Reg[R_RT].i16_16_31) -
				(R5900Regs.Reg[R_RS].i16_00_15 * R5900Regs.Reg[R_RT].i16_00_15));
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.HI.u32_00_31 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_48_63 * R5900Regs.Reg[R_RT].i16_48_63) -
				(R5900Regs.Reg[R_RS].i16_32_47 * R5900Regs.Reg[R_RT].i16_32_47));
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.LO.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_80_95 * R5900Regs.Reg[R_RT].i16_80_95) -
				(R5900Regs.Reg[R_RS].i16_64_79 * R5900Regs.Reg[R_RT].i16_64_79));
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.HI.u32_64_95 = (EMU_I32)(
				(R5900Regs.Reg[R_RS].i16_112_127 * R5900Regs.Reg[R_RT].i16_112_127) -
				(R5900Regs.Reg[R_RS].i16_96_111 * R5900Regs.Reg[R_RT].i16_96_111));
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			break;

		case 0x00000680: // "pexeh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x000006C0: // "prevh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_112_127;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RT].u16_64_79;
			break;

		case 0x00000700: // "pmulth"
			R5900Regs.LO.u32_00_31 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_00_15 *
				R5900Regs.Reg[R_RT].i16_00_15);
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.LO.u32_32_63 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_16_31 *
				R5900Regs.Reg[R_RT].i16_16_31);
			R5900Regs.HI.u32_00_31 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_32_47 *
				R5900Regs.Reg[R_RT].i16_32_47);
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.HI.u32_32_63 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_48_63 *
				R5900Regs.Reg[R_RT].i16_48_63);
			R5900Regs.LO.u32_64_95 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_64_79 *
				R5900Regs.Reg[R_RT].i16_64_79);
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.LO.u32_96_127 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_80_95 *
				R5900Regs.Reg[R_RT].i16_80_95);
			R5900Regs.HI.u32_64_95 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_96_111 *
				R5900Regs.Reg[R_RT].i16_96_111);
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			R5900Regs.HI.u32_96_127 = (EMU_I16)(R5900Regs.Reg[R_RS].i16_112_127 *
				R5900Regs.Reg[R_RT].u16_112_127);
			break;

		case 0x00000740: // "pdivbw"
			R5900Regs.LO.i32_00_31 = R5900Regs.Reg[R_RS].i32_00_31 / R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.LO.i32_32_63 = R5900Regs.Reg[R_RS].i32_32_63 / R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.LO.i32_64_95 = R5900Regs.Reg[R_RS].i32_64_95 / R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.LO.i32_96_127 = R5900Regs.Reg[R_RS].i32_96_127 / R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.HI.i32_00_31 = R5900Regs.Reg[R_RS].i32_00_31 % R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.HI.i32_32_63 = R5900Regs.Reg[R_RS].i32_32_63 % R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.HI.i32_64_95 = R5900Regs.Reg[R_RS].i32_64_95 % R5900Regs.Reg[R_RT].i16_00_15;
			R5900Regs.HI.i32_96_127 = R5900Regs.Reg[R_RS].i32_96_127 % R5900Regs.Reg[R_RT].i16_00_15;
			break;

		case 0x00000780: // "pexew"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RT].u32_96_127;
			break;

		case 0x000007C0: // "prot3w"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RT].u32_96_127;
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
		emuTemp.TempI64 = (EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31;
		R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(emuTemp.TempI64 >> 32);
		if (R_RD)
		{
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		}
		break;

	case 0x00000019: // "multu1"
		emuTemp.TempU64 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
		R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempU64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(emuTemp.TempU64 >> 32);
		if (R_RD)
		{
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		}
		break;

	case 0x0000001A: // "div1"
		R5900Regs.LO.u64_64_127 = R5900Regs.Reg[R_RS].i32_00_31 / R5900Regs.Reg[R_RT].i32_00_31;
		R5900Regs.HI.u64_64_127 = R5900Regs.Reg[R_RS].i32_00_31 % R5900Regs.Reg[R_RT].i32_00_31;
		break;

	case 0x0000001B: // "divu1"
		R5900Regs.LO.u64_64_127 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_00_31 / R5900Regs.Reg[R_RT].u32_00_31);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_00_31 % R5900Regs.Reg[R_RT].u32_00_31);
		break;

	case 0x00000020: // "madd1"
		emuTemp.TempI64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95) +
			((EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31);
		R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempI64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)(emuTemp.TempI64 >> 32);
		R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		break;

	case 0x00000021: // "maddu1"
		emuTemp.TempU64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95) +
			((EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31);
		R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempU64 & 0x00000000FFFFFFFF);
		R5900Regs.HI.u64_64_127 = (EMU_I32)((emuTemp.TempU64 >> 32) & 0x00000000FFFFFFFF);
		R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_64_127;
		break;

	case 0x00000028: // "MMI1"
		///////////////////////////////////
		// MMI1 Funcions
		///////////////////////////////////
		switch (Code & 0x000007C0)
		{
		case 0x00000040: // "pabsw"
			R5900Regs.Reg[R_RD].u32_00_31 = abs(R5900Regs.Reg[R_RT].i32_00_31);
			R5900Regs.Reg[R_RD].u32_32_63 = abs(R5900Regs.Reg[R_RT].i32_32_63);
			R5900Regs.Reg[R_RD].u32_64_95 = abs(R5900Regs.Reg[R_RT].i32_64_95);
			R5900Regs.Reg[R_RD].u32_96_127 = abs(R5900Regs.Reg[R_RT].i32_96_127);
			break;

		case 0x00000080: // "pceqw"
			R5900Regs.Reg[R_RD].u32_00_31 = (R5900Regs.Reg[R_RS].u32_00_31 == R5900Regs.Reg[R_RT].u32_00_31) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[R_RD].u32_32_63 = (R5900Regs.Reg[R_RS].u32_32_63 == R5900Regs.Reg[R_RT].u32_32_63) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[R_RD].u32_64_95 = (R5900Regs.Reg[R_RS].u32_64_95 == R5900Regs.Reg[R_RT].u32_64_95) ? 0xFFFFFFFF : 0;
			R5900Regs.Reg[R_RD].u32_96_127 = (R5900Regs.Reg[R_RS].u32_96_127 == R5900Regs.Reg[R_RT].u32_96_127) ? 0xFFFFFFFF : 0;
			break;

		case 0x000000C0: // "pminw"
			R5900Regs.Reg[R_RD].u32_00_31 = (R5900Regs.Reg[R_RS].i32_00_31 < R5900Regs.Reg[R_RT].i32_00_31) ?
				R5900Regs.Reg[R_RS].u32_00_31 : R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = (R5900Regs.Reg[R_RS].i32_32_63 < R5900Regs.Reg[R_RT].i32_32_63) ?
				R5900Regs.Reg[R_RS].u32_32_63 : R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_64_95 = (R5900Regs.Reg[R_RS].i32_64_95 < R5900Regs.Reg[R_RT].i32_64_95) ?
				R5900Regs.Reg[R_RS].u32_64_95 : R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_96_127 = (R5900Regs.Reg[R_RS].i32_96_127 < R5900Regs.Reg[R_RT].i32_96_127) ?
				R5900Regs.Reg[R_RS].u32_96_127 : R5900Regs.Reg[R_RT].u32_96_127;
			break;

		case 0x00000100: // "padsbh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RS].u16_00_15 - R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_16_31 - R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RS].u16_32_47 - R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_48_63 - R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RS].u16_64_79 + R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_80_95 + R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RS].u16_96_111 + R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_112_127 + R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x00000140: // "pabsh"
			R5900Regs.Reg[R_RD].u16_00_15 = abs(R5900Regs.Reg[R_RT].u16_00_15);
			R5900Regs.Reg[R_RD].u16_16_31 = abs(R5900Regs.Reg[R_RT].u16_16_31);
			R5900Regs.Reg[R_RD].u16_32_47 = abs(R5900Regs.Reg[R_RT].u16_32_47);
			R5900Regs.Reg[R_RD].u16_48_63 = abs(R5900Regs.Reg[R_RT].u16_48_63);
			R5900Regs.Reg[R_RD].u16_64_79 = abs(R5900Regs.Reg[R_RT].u16_64_79);
			R5900Regs.Reg[R_RD].u16_80_95 = abs(R5900Regs.Reg[R_RT].u16_80_95);
			R5900Regs.Reg[R_RD].u16_96_111 = abs(R5900Regs.Reg[R_RT].u16_96_111);
			R5900Regs.Reg[R_RD].u16_112_127 = abs(R5900Regs.Reg[R_RT].u16_112_127);
			break;

		case 0x00000180: // "pceqh"
			R5900Regs.Reg[R_RD].u16_00_15 = (R5900Regs.Reg[R_RS].u16_00_15 == R5900Regs.Reg[R_RT].u16_00_15) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_16_31 = (R5900Regs.Reg[R_RS].u16_16_31 == R5900Regs.Reg[R_RT].u16_16_31) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_32_47 = (R5900Regs.Reg[R_RS].u16_32_47 == R5900Regs.Reg[R_RT].u16_32_47) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_48_63 = (R5900Regs.Reg[R_RS].u16_48_63 == R5900Regs.Reg[R_RT].u16_48_63) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_64_79 = (R5900Regs.Reg[R_RS].u16_64_79 == R5900Regs.Reg[R_RT].u16_64_79) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_80_95 = (R5900Regs.Reg[R_RS].u16_80_95 == R5900Regs.Reg[R_RT].u16_80_95) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_96_111 = (R5900Regs.Reg[R_RS].u16_96_111 == R5900Regs.Reg[R_RT].u16_96_111) ? 0xFFFF : 0;
			R5900Regs.Reg[R_RD].u16_112_127 = (R5900Regs.Reg[R_RS].u16_112_127 == R5900Regs.Reg[R_RT].u16_112_127) ? 0xFFFF : 0;
			break;

		case 0x000001C0: // "pminh"
			R5900Regs.Reg[R_RD].u16_00_15 = (R5900Regs.Reg[R_RS].i16_00_15 < R5900Regs.Reg[R_RT].i16_00_15) ?
				R5900Regs.Reg[R_RS].u16_00_15 : R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = (R5900Regs.Reg[R_RS].i16_16_31 < R5900Regs.Reg[R_RT].i16_16_31) ?
				R5900Regs.Reg[R_RS].u16_16_31 : R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_32_47 = (R5900Regs.Reg[R_RS].i16_32_47 < R5900Regs.Reg[R_RT].i16_32_47) ?
				R5900Regs.Reg[R_RS].u16_32_47 : R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_48_63 = (R5900Regs.Reg[R_RS].i16_48_63 < R5900Regs.Reg[R_RT].i16_48_63) ?
				R5900Regs.Reg[R_RS].u16_48_63 : R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = (R5900Regs.Reg[R_RS].i16_64_79 < R5900Regs.Reg[R_RT].i16_64_79) ?
				R5900Regs.Reg[R_RS].u16_64_79 : R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = (R5900Regs.Reg[R_RS].i16_80_95 < R5900Regs.Reg[R_RT].i16_80_95) ?
				R5900Regs.Reg[R_RS].u16_80_95 : R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_96_111 = (R5900Regs.Reg[R_RS].i16_96_111 < R5900Regs.Reg[R_RT].i16_96_111) ?
				R5900Regs.Reg[R_RS].u16_96_111 : R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_112_127 = (R5900Regs.Reg[R_RS].i16_112_127 < R5900Regs.Reg[R_RT].i16_112_127) ?
				R5900Regs.Reg[R_RS].u16_112_127 : R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x00000280: // "pceqb"
			R5900Regs.Reg[R_RD].u08_00_07 = (R5900Regs.Reg[R_RS].u08_00_07 == R5900Regs.Reg[R_RT].u08_00_07) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_08_15 = (R5900Regs.Reg[R_RS].u08_08_15 == R5900Regs.Reg[R_RT].u08_08_15) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_16_23 = (R5900Regs.Reg[R_RS].u08_16_23 == R5900Regs.Reg[R_RT].u08_16_23) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_24_31 = (R5900Regs.Reg[R_RS].u08_24_31 == R5900Regs.Reg[R_RT].u08_24_31) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_32_39 = (R5900Regs.Reg[R_RS].u08_32_39 == R5900Regs.Reg[R_RT].u08_32_39) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_40_47 = (R5900Regs.Reg[R_RS].u08_40_47 == R5900Regs.Reg[R_RT].u08_40_47) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_48_55 = (R5900Regs.Reg[R_RS].u08_48_55 == R5900Regs.Reg[R_RT].u08_48_55) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_56_63 = (R5900Regs.Reg[R_RS].u08_56_63 == R5900Regs.Reg[R_RT].u08_56_63) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_64_71 = (R5900Regs.Reg[R_RS].u08_64_71 == R5900Regs.Reg[R_RT].u08_64_71) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_72_79 = (R5900Regs.Reg[R_RS].u08_72_79 == R5900Regs.Reg[R_RT].u08_72_79) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_80_87 = (R5900Regs.Reg[R_RS].u08_80_87 == R5900Regs.Reg[R_RT].u08_80_87) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_88_95 = (R5900Regs.Reg[R_RS].u08_88_95 == R5900Regs.Reg[R_RT].u08_88_95) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_96_103 = (R5900Regs.Reg[R_RS].u08_96_103 == R5900Regs.Reg[R_RT].u08_96_103) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_104_111 = (R5900Regs.Reg[R_RS].u08_104_111 == R5900Regs.Reg[R_RT].u08_104_111) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_112_119 = (R5900Regs.Reg[R_RS].u08_112_119 == R5900Regs.Reg[R_RT].u08_112_119) ? 0xFF : 0;
			R5900Regs.Reg[R_RD].u08_120_127 = (R5900Regs.Reg[R_RS].u08_120_127 == R5900Regs.Reg[R_RT].u08_120_127) ? 0xFF : 0;
			break;

		case 0x00000400: // "padduw"
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_00_31 + R5900Regs.Reg[R_RT].u32_00_31;
			if (emuTemp.TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[R_RD].u32_00_31 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_00_31 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_32_63 + R5900Regs.Reg[R_RT].u32_32_63;
			if (emuTemp.TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[R_RD].u32_32_63 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_32_63 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_64_95 + R5900Regs.Reg[R_RT].u32_64_95;
			if (emuTemp.TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[R_RD].u32_64_95 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_64_95 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_96_127 + R5900Regs.Reg[R_RT].u32_96_127;
			if (emuTemp.TempU32 > 0xffffffff)
			{
				R5900Regs.Reg[R_RD].u32_96_127 = 0xffffffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_96_127 = emuTemp.TempU32;
			}
			break;

		case 0x00000440: // "psubuw"
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_00_31 - R5900Regs.Reg[R_RT].u32_00_31;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u32_00_31 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_00_31 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_32_63 - R5900Regs.Reg[R_RT].u32_32_63;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u32_32_63 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_32_63 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_64_95 - R5900Regs.Reg[R_RT].u32_64_95;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u32_64_95 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_64_95 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u32_96_127 - R5900Regs.Reg[R_RT].u32_96_127;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u32_96_127 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u32_96_127 = emuTemp.TempU32;
			}
			break;

		case 0x00000480: // "pextuw"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RS].u32_64_95;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_96_127;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RS].u32_96_127;
			break;

		case 0x00000500: // "padduh"
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_00_15 + R5900Regs.Reg[R_RT].u16_00_15;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_00_15 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_00_15 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_16_31 + R5900Regs.Reg[R_RT].u16_16_31;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_16_31 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_16_31 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_32_47 + R5900Regs.Reg[R_RT].u16_32_47;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_32_47 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_32_47 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_48_63 + R5900Regs.Reg[R_RT].u16_48_63;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_48_63 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_48_63 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_64_79 + R5900Regs.Reg[R_RT].u16_64_79;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_64_79 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_64_79 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_80_95 + R5900Regs.Reg[R_RT].u16_80_95;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_80_95 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_80_95 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_96_111 + R5900Regs.Reg[R_RT].u16_96_111;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_96_111 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_96_111 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_112_127 + R5900Regs.Reg[R_RT].u16_112_127;
			if (emuTemp.TempU32 > 0xffff)
			{
				R5900Regs.Reg[R_RD].u16_112_127 = 0xffff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_112_127 = emuTemp.TempU32;
			}
			break;

		case 0x00000540: // "psubuh"
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_00_15 - R5900Regs.Reg[R_RT].u16_00_15;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_00_15 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_00_15 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_16_31 - R5900Regs.Reg[R_RT].u16_16_31;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_16_31 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_16_31 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_32_47 - R5900Regs.Reg[R_RT].u16_32_47;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_32_47 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_32_47 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_48_63 - R5900Regs.Reg[R_RT].u16_48_63;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_48_63 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_48_63 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_64_79 - R5900Regs.Reg[R_RT].u16_64_79;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_64_79 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_64_79 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_80_95 - R5900Regs.Reg[R_RT].u16_80_95;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_80_95 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_80_95 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_96_111 - R5900Regs.Reg[R_RT].u16_96_111;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_96_111 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_96_111 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u16_112_127 - R5900Regs.Reg[R_RT].u16_112_127;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u16_112_127 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u16_112_127 = emuTemp.TempU32;
			}
			break;

		case 0x00000580: // "pextuh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_64_79;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_80_95;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_96_111;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_112_127;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_112_127;
			break;

		case 0x00000600: // "paddub"
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_00_07 + R5900Regs.Reg[R_RT].u08_00_07;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_00_07 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_00_07 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_08_15 + R5900Regs.Reg[R_RT].u08_08_15;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_08_15 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_08_15 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_16_23 + R5900Regs.Reg[R_RT].u08_16_23;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_16_23 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_16_23 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_24_31 + R5900Regs.Reg[R_RT].u08_24_31;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_24_31 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_24_31 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_32_39 + R5900Regs.Reg[R_RT].u08_32_39;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_32_39 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_32_39 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_40_47 + R5900Regs.Reg[R_RT].u08_40_47;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_40_47 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_40_47 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_48_55 + R5900Regs.Reg[R_RT].u08_48_55;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_48_55 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_48_55 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_56_63 + R5900Regs.Reg[R_RT].u08_56_63;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_56_63 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_56_63 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_64_71 + R5900Regs.Reg[R_RT].u08_64_71;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_64_71 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_64_71 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_72_79 + R5900Regs.Reg[R_RT].u08_72_79;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_72_79 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_72_79 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_80_87 + R5900Regs.Reg[R_RT].u08_80_87;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_80_87 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_80_87 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_88_95 + R5900Regs.Reg[R_RT].u08_88_95;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_88_95 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_88_95 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_96_103 + R5900Regs.Reg[R_RT].u08_96_103;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_96_103 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_96_103 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_104_111 + R5900Regs.Reg[R_RT].u08_104_111;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_104_111 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_104_111 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_112_119 + R5900Regs.Reg[R_RT].u08_112_119;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_112_119 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_112_119 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_120_127 + R5900Regs.Reg[R_RT].u08_120_127;
			if (emuTemp.TempU32 > 0xff)
			{
				R5900Regs.Reg[R_RD].u08_120_127 = 0xff;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_120_127 = emuTemp.TempU32;
			}
			break;

		case 0x00000640: // "psubub"
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_00_07 - R5900Regs.Reg[R_RT].u08_00_07;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_00_07 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_00_07 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_08_15 - R5900Regs.Reg[R_RT].u08_08_15;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_08_15 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_08_15 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_16_23 - R5900Regs.Reg[R_RT].u08_16_23;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_16_23 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_16_23 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_24_31 - R5900Regs.Reg[R_RT].u08_24_31;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_24_31 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_24_31 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_32_39 - R5900Regs.Reg[R_RT].u08_32_39;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_32_39 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_32_39 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_40_47 - R5900Regs.Reg[R_RT].u08_40_47;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_40_47 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_40_47 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_48_55 - R5900Regs.Reg[R_RT].u08_48_55;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_48_55 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_48_55 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_56_63 - R5900Regs.Reg[R_RT].u08_56_63;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_56_63 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_56_63 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_64_71 - R5900Regs.Reg[R_RT].u08_64_71;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_64_71 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_64_71 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_72_79 - R5900Regs.Reg[R_RT].u08_72_79;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_72_79 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_72_79 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_80_87 - R5900Regs.Reg[R_RT].u08_80_87;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_80_87 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_80_87 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_88_95 - R5900Regs.Reg[R_RT].u08_88_95;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_88_95 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_88_95 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_96_103 - R5900Regs.Reg[R_RT].u08_96_103;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_96_103 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_96_103 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_104_111 - R5900Regs.Reg[R_RT].u08_104_111;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_104_111 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_104_111 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_112_119 - R5900Regs.Reg[R_RT].u08_112_119;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_112_119 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_112_119 = emuTemp.TempU32;
			}
			emuTemp.TempU32 = R5900Regs.Reg[R_RS].u08_120_127 - R5900Regs.Reg[R_RT].u08_120_127;
			if (emuTemp.TempU32 <= 0)
			{
				R5900Regs.Reg[R_RD].u08_120_127 = 0;
			}
			else
			{
				R5900Regs.Reg[R_RD].u08_120_127 = emuTemp.TempU32;
			}
			break;

		case 0x00000680: // "pextub"
			R5900Regs.Reg[R_RD].u08_00_07 = R5900Regs.Reg[R_RT].u08_64_71;
			R5900Regs.Reg[R_RD].u08_08_15 = R5900Regs.Reg[R_RS].u08_64_71;
			R5900Regs.Reg[R_RD].u08_16_23 = R5900Regs.Reg[R_RT].u08_72_79;
			R5900Regs.Reg[R_RD].u08_24_31 = R5900Regs.Reg[R_RS].u08_72_79;
			R5900Regs.Reg[R_RD].u08_32_39 = R5900Regs.Reg[R_RT].u08_80_87;
			R5900Regs.Reg[R_RD].u08_40_47 = R5900Regs.Reg[R_RS].u08_80_87;
			R5900Regs.Reg[R_RD].u08_48_55 = R5900Regs.Reg[R_RT].u08_88_95;
			R5900Regs.Reg[R_RD].u08_56_63 = R5900Regs.Reg[R_RS].u08_88_95;
			R5900Regs.Reg[R_RD].u08_64_71 = R5900Regs.Reg[R_RT].u08_96_103;
			R5900Regs.Reg[R_RD].u08_72_79 = R5900Regs.Reg[R_RS].u08_96_103;
			R5900Regs.Reg[R_RD].u08_80_87 = R5900Regs.Reg[R_RT].u08_104_111;
			R5900Regs.Reg[R_RD].u08_88_95 = R5900Regs.Reg[R_RS].u08_104_111;
			R5900Regs.Reg[R_RD].u08_96_103 = R5900Regs.Reg[R_RT].u08_112_119;
			R5900Regs.Reg[R_RD].u08_104_111 = R5900Regs.Reg[R_RS].u08_112_119;
			R5900Regs.Reg[R_RD].u08_112_119 = R5900Regs.Reg[R_RT].u08_120_127;
			R5900Regs.Reg[R_RD].u08_120_127 = R5900Regs.Reg[R_RS].u08_120_127;
			break;

		case 0x000006C0: // "qfsrv"
			if (R5900Regs.SA)
			{
				R5900Regs.Reg[R_RD].u64_00_63 = (R5900Regs.Reg[R_RS].u64_00_63 << (64 - R5900Regs.SA)) |
					(R5900Regs.Reg[R_RT].u64_00_63 >> R5900Regs.SA);

				R5900Regs.Reg[R_RD].u64_64_127 = (R5900Regs.Reg[R_RS].u64_64_127 << (64 - R5900Regs.SA)) |
					(R5900Regs.Reg[R_RT].u64_64_127 >> R5900Regs.SA);
			}
			else
			{
				R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63;
				R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.Reg[R_RT].u64_64_127;
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
			emuTemp.TempU64 = (((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31) +
				((EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31);
			R5900Regs.LO.u64_00_63 = (EMU_I32)(emuTemp.TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((emuTemp.TempU64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[R_RD].u64_00_63 = emuTemp.TempU64;
			emuTemp.TempU64 = (((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95) +
				((EMU_U64)R5900Regs.Reg[R_RS].u32_64_95 * (EMU_U64)R5900Regs.Reg[R_RT].u32_64_95);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((emuTemp.TempU64 >> 32) & 0x00000000FFFFFFFF);
			R5900Regs.Reg[R_RD].u64_00_63 = emuTemp.TempU64;
			break;

		case 0x000000C0: // "psravw"
			SA = R5900Regs.Reg[R_RS].u32_00_31 & 0x1F;
			R5900Regs.Reg[R_RD].u64_00_63 = (R5900Regs.Reg[R_RT].u32_00_31 & 0x80000000) ?
				(EMU_I32)((MaxU32 << (32 - SA)) | (R5900Regs.Reg[R_RT].u32_00_31 >> SA)) :
				(EMU_I32)(R5900Regs.Reg[R_RT].u32_00_31 >> SA);
			SA = R5900Regs.Reg[R_RS].u32_64_95 & 0x1F;
			R5900Regs.Reg[R_RD].u64_64_127 = (R5900Regs.Reg[R_RT].u32_64_95 & 0x80000000) ?
				(EMU_I32)((MaxU32 << (32 - SA)) | (R5900Regs.Reg[R_RT].u32_64_95 >> SA)) :
				(EMU_I32)(R5900Regs.Reg[R_RT].u32_64_95 >> SA);
			break;

		case 0x00000200: // "pmthi"
			R5900Regs.HI.u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63;
			R5900Regs.HI.u64_64_127 = R5900Regs.Reg[R_RS].u64_64_127;
			break;

		case 0x00000240: // "pmtlo"
			R5900Regs.LO.u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63;
			R5900Regs.LO.u64_64_127 = R5900Regs.Reg[R_RS].u64_64_127;
			break;

		case 0x00000280: // "pinteh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RS].u16_00_15;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RS].u16_32_47;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RS].u16_64_79;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RS].u16_96_111;
			break;

		case 0x00000300: // "pmultuw"
			emuTemp.TempU64 = R5900Regs.Reg[R_RS].u32_00_31 * R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.LO.u64_00_63 = (EMU_I32)(emuTemp.TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_00_63 = (EMU_I32)((emuTemp.TempU64 >> 32) & 0x00000000FFFFFFFF);
			if (R_RD)
			{
				R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
			}
			emuTemp.TempU64 = R5900Regs.Reg[R_RS].u32_64_95 * R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.LO.u64_64_127 = (EMU_I32)(emuTemp.TempU64 & 0x00000000FFFFFFFF);
			R5900Regs.HI.u64_64_127 = (EMU_I32)((emuTemp.TempU64 >> 32) & 0x00000000FFFFFFFF);
			if (R_RD)
			{
				R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.LO.u64_64_127;
			}
			break;

		case 0x00000340: // "pdivuw"
			R5900Regs.LO.u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_00_31 /
				R5900Regs.Reg[R_RT].u32_00_31);
			R5900Regs.LO.u64_64_127 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_64_95 /
				R5900Regs.Reg[R_RT].u32_64_95);
			R5900Regs.HI.u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_00_31 %
				R5900Regs.Reg[R_RT].u32_00_31);
			R5900Regs.HI.u64_64_127 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_64_95 %
				R5900Regs.Reg[R_RT].u32_64_95);
			break;

		case 0x00000380: // "pcpyud"
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_64_127;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.Reg[R_RT].u64_64_127;
			break;

		case 0x00000480: // "por"
			R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 | R5900Regs.Reg[R_RT].u64_00_63;
			R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.Reg[R_RS].u64_64_127 | R5900Regs.Reg[R_RT].u64_64_127;
			break;

		case 0x000004C0: // "pnor"
			R5900Regs.Reg[R_RD].u64_00_63 = ~(R5900Regs.Reg[R_RS].u64_00_63 | R5900Regs.Reg[R_RT].u64_00_63);
			R5900Regs.Reg[R_RD].u64_64_127 = ~(R5900Regs.Reg[R_RS].u64_64_127 | R5900Regs.Reg[R_RT].u64_64_127);
			break;

		case 0x00000680: // "pexch"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_32_47;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_16_31;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_48_63;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RT].u16_96_111;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_80_95;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RT].u16_112_127;
			break;

		case 0x000006C0: // "pcpyh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_00_15;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_64_79;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RT].u16_64_79;
			break;

		case 0x00000780: // "pexcw"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RT].u32_64_95;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_32_63;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RT].u32_96_127;
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
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_00_31;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_00_31;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_64_95;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_64_95;
			break;

		case 0x00000070: // "pmfhl.uw"
			R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.LO.u32_32_63;
			R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.HI.u32_32_63;
			R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.LO.u32_96_127;
			R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.HI.u32_96_127;
			break;

		case 0x000000B0: // "pmfhl.slw"
			emuTemp.TempU64 = ((EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31;
			if (emuTemp.TempU64 >= 0x000000007FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u64_00_63 = 0x000000007FFFFFFF;
			}
			else
				if (emuTemp.TempU64 <= 0xFFFFFFFF80000000)
				{
					R5900Regs.Reg[R_RD].u64_00_63 = 0xFFFFFFFF80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.i32_00_31;
				}
			emuTemp.TempU64 = ((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95;
			if (emuTemp.TempU64 >= 0x000000007FFFFFFF)
			{
				R5900Regs.Reg[R_RD].u64_64_127 = 0x000000007FFFFFFF;
			}
			else
				if (emuTemp.TempU64 <= 0xFFFFFFFF80000000)
				{
					R5900Regs.Reg[R_RD].u64_64_127 = 0xFFFFFFFF80000000;
				}
				else
				{
					R5900Regs.Reg[R_RD].u64_64_127 = R5900Regs.LO.i32_64_95;
				}
			break;

		case 0x000000F0: // "pmfhl.lh"
			R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.LO.u16_00_15;
			R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.LO.u16_32_47;
			R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.HI.u16_00_15;
			R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.HI.u16_32_47;
			R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.LO.u16_64_79;
			R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.LO.u16_96_111;
			R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.HI.u16_64_79;
			R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.HI.u16_96_111;
			break;

		case 0x00000130: // "pmfhl.sh"
			R5900Regs.Reg[R_RD].u16_00_15 = clamp(R5900Regs.LO.u32_00_31);
			R5900Regs.Reg[R_RD].u16_16_31 = clamp(R5900Regs.LO.u32_32_63);
			R5900Regs.Reg[R_RD].u16_32_47 = clamp(R5900Regs.HI.u32_00_31);
			R5900Regs.Reg[R_RD].u16_48_63 = clamp(R5900Regs.HI.u32_32_63);
			R5900Regs.Reg[R_RD].u16_64_79 = clamp(R5900Regs.LO.u32_64_95);
			R5900Regs.Reg[R_RD].u16_80_95 = clamp(R5900Regs.LO.u32_96_127);
			R5900Regs.Reg[R_RD].u16_96_111 = clamp(R5900Regs.HI.u32_64_95);
			R5900Regs.Reg[R_RD].u16_112_127 = clamp(R5900Regs.HI.u32_96_127);
			break;
		}

	case 0x00000031: // "pmthl.lw"
		R5900Regs.LO.u32_00_31 = R5900Regs.Reg[R_RS].u32_00_31;
		R5900Regs.HI.u32_00_31 = R5900Regs.Reg[R_RS].u32_32_63;
		R5900Regs.LO.u32_64_95 = R5900Regs.Reg[R_RS].u32_64_95;
		R5900Regs.HI.u32_64_95 = R5900Regs.Reg[R_RS].u32_96_127;
		break;

	case 0x00000034: // "psllh"
		R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15 << R_SA;
		R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_16_31 << R_SA;
		R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_32_47 << R_SA;
		R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_48_63 << R_SA;
		R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_64_79 << R_SA;
		R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RT].u16_80_95 << R_SA;
		R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_96_111 << R_SA;
		R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RT].u16_112_127 << R_SA;
		break;

	case 0x00000036: // "psrlh"
		R5900Regs.Reg[R_RD].u16_00_15 = R5900Regs.Reg[R_RT].u16_00_15 >> R_SA;
		R5900Regs.Reg[R_RD].u16_16_31 = R5900Regs.Reg[R_RT].u16_16_31 >> R_SA;
		R5900Regs.Reg[R_RD].u16_32_47 = R5900Regs.Reg[R_RT].u16_32_47 >> R_SA;
		R5900Regs.Reg[R_RD].u16_48_63 = R5900Regs.Reg[R_RT].u16_48_63 >> R_SA;
		R5900Regs.Reg[R_RD].u16_64_79 = R5900Regs.Reg[R_RT].u16_64_79 >> R_SA;
		R5900Regs.Reg[R_RD].u16_80_95 = R5900Regs.Reg[R_RT].u16_80_95 >> R_SA;
		R5900Regs.Reg[R_RD].u16_96_111 = R5900Regs.Reg[R_RT].u16_96_111 >> R_SA;
		R5900Regs.Reg[R_RD].u16_112_127 = R5900Regs.Reg[R_RT].u16_112_127 >> R_SA;
		break;

	case 0x00000037: // "psrah"
		R5900Regs.Reg[R_RD].u16_00_15 = (R5900Regs.Reg[R_RT].u16_00_15 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_00_15 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_00_15 >> R_SA);
		R5900Regs.Reg[R_RD].u16_16_31 = (R5900Regs.Reg[R_RT].u16_16_31 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_16_31 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_16_31 >> R_SA);
		R5900Regs.Reg[R_RD].u16_32_47 = (R5900Regs.Reg[R_RT].u16_32_47 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_32_47 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_32_47 >> R_SA);
		R5900Regs.Reg[R_RD].u16_48_63 = (R5900Regs.Reg[R_RT].u16_48_63 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_48_63 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_48_63 >> R_SA);
		R5900Regs.Reg[R_RD].u16_64_79 = (R5900Regs.Reg[R_RT].u16_64_79 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_64_79 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_64_79 >> R_SA);
		R5900Regs.Reg[R_RD].u16_80_95 = (R5900Regs.Reg[R_RT].u16_80_95 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_80_95 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_80_95 >> R_SA);
		R5900Regs.Reg[R_RD].u16_96_111 = (R5900Regs.Reg[R_RT].u16_96_111 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_96_111 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_96_111 >> R_SA);
		R5900Regs.Reg[R_RD].u16_112_127 = (R5900Regs.Reg[R_RT].u16_112_127 & 0x8000) ?
			((MaxU16 << (16 - R_SA)) | (R5900Regs.Reg[R_RT].u16_112_127 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u16_112_127 >> R_SA);
		break;

	case 0x0000003C: // "psllw"
		R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_00_31 << R_SA;
		R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RT].u32_32_63 << R_SA;
		R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_64_95 << R_SA;
		R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RT].u32_96_127 << R_SA;
		break;

	case 0x0000003E: // "psrlw"
		R5900Regs.Reg[R_RD].u32_00_31 = R5900Regs.Reg[R_RT].u32_00_31 >> R_SA;
		R5900Regs.Reg[R_RD].u32_32_63 = R5900Regs.Reg[R_RT].u32_32_63 >> R_SA;
		R5900Regs.Reg[R_RD].u32_64_95 = R5900Regs.Reg[R_RT].u32_64_95 >> R_SA;
		R5900Regs.Reg[R_RD].u32_96_127 = R5900Regs.Reg[R_RT].u32_96_127 >> R_SA;
		break;

	case 0x0000003F: // "psraw"
		R5900Regs.Reg[R_RD].u32_00_31 = (R5900Regs.Reg[R_RT].u32_00_31 & 0x80000000) ?
			((MaxU32 << (32 - R_SA)) | (R5900Regs.Reg[R_RT].u32_00_31 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u32_00_31 >> R_SA);
		R5900Regs.Reg[R_RD].u32_32_63 = (R5900Regs.Reg[R_RT].u32_32_63 & 0x80000000) ?
			((MaxU32 << (32 - R_SA)) | (R5900Regs.Reg[R_RT].u32_32_63 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u32_32_63 >> R_SA);
		R5900Regs.Reg[R_RD].u32_64_95 = (R5900Regs.Reg[R_RT].u32_64_95 & 0x80000000) ?
			((MaxU32 << (32 - R_SA)) | (R5900Regs.Reg[R_RT].u32_64_95 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u32_64_95 >> R_SA);
		R5900Regs.Reg[R_RD].u32_96_127 = (R5900Regs.Reg[R_RT].u32_96_127 & 0x80000000) ?
			((MaxU32 << (32 - R_SA)) | (R5900Regs.Reg[R_RT].u32_96_127 >> R_SA)) :
			(R5900Regs.Reg[R_RT].u32_96_127 >> R_SA);
		break;
	}
	///////////////////////////////////
	// End of MMI Funcions
	///////////////////////////////////
}