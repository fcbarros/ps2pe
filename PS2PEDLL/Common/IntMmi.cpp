#include "Interpreter.h"

namespace Interpreter
{
	void Interpreter::EmuMmi()
	{
		///////////////////////////////////
		// MMI Funcions
		///////////////////////////////////
		switch (OpCode & 0x0000003F)
		{
		case 0x00000000: // "madd"
			Temp128.u64_00_63 = (EMU_I64)(((EMU_I64)PS2Regs.R5900Regs.HI.u32_00_31 << 32) | (EMU_I64)PS2Regs.R5900Regs.LO.u32_00_31) +
				((EMU_I64)PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
			PS2Regs.R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
			PS2Regs.R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
			if (R_RD)
			{
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
			}
			break;

		case 0x00000001: // "maddu"
			Temp128.u64_00_63 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)PS2Regs.R5900Regs.LO.u32_00_31) +
				((EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
			PS2Regs.R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
			PS2Regs.R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
			if (R_RD)
			{
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
			}
			break;

		case 0x00000004: // "plzcw"
			PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0;
			PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0;
			TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31;
			while ((TempU32 & 0x80000000) ==
				(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x80000000))
			{
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31++;
				TempU32 <<= 1;
			}
			TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63;
			while ((TempU32 & 0x80000000) ==
				(PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 & 0x80000000))
			{
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63++;
				TempU32 <<= 1;
			}
			PS2Regs.R5900Regs.Reg[R_RD].u32_00_31--;
			PS2Regs.R5900Regs.Reg[R_RD].u32_32_63--;
			break;

		case 0x00000008: // MMI0
			///////////////////////////////////
			// MMI0 Funcions
			///////////////////////////////////
			switch (OpCode & 0x000007C0)
			{
			case 0x00000000: // "paddw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 + PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 + PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 + PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;

			case 0x00000040: // "psubw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 - PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 - PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 - PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 - PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;

			case 0x00000080: // "pcgtw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 > PS2Regs.R5900Regs.Reg[R_RT].u32_00_31) ? 0xFFFFFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = (PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 > PS2Regs.R5900Regs.Reg[R_RT].u32_32_63) ? 0xFFFFFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = (PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 > PS2Regs.R5900Regs.Reg[R_RT].u32_64_95) ? 0xFFFFFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = (PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 > PS2Regs.R5900Regs.Reg[R_RT].u32_96_127) ? 0xFFFFFFFF : 0;
				break;

			case 0x000000C0: // "pmaxw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = (PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 > PS2Regs.R5900Regs.Reg[R_RT].i32_00_31) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 : PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = (PS2Regs.R5900Regs.Reg[R_RS].i32_32_63 > PS2Regs.R5900Regs.Reg[R_RT].i32_32_63) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 : PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = (PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 > PS2Regs.R5900Regs.Reg[R_RT].i32_64_95) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 : PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = (PS2Regs.R5900Regs.Reg[R_RS].i32_96_127 > PS2Regs.R5900Regs.Reg[R_RT].i32_96_127) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 : PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;

			case 0x00000100: // "paddh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 + PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 + PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 + PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 + PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 + PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 + PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 + PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 + PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x00000140: // "psubh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 - PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 - PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 - PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 - PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 - PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 - PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 - PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 - PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x00000180: // "pcgth"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = (PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 > PS2Regs.R5900Regs.Reg[R_RT].u16_00_15) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = (PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 > PS2Regs.R5900Regs.Reg[R_RT].u16_16_31) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = (PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 > PS2Regs.R5900Regs.Reg[R_RT].u16_32_47) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = (PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 > PS2Regs.R5900Regs.Reg[R_RT].u16_48_63) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = (PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 > PS2Regs.R5900Regs.Reg[R_RT].u16_64_79) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = (PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 > PS2Regs.R5900Regs.Reg[R_RT].u16_80_95) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = (PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 > PS2Regs.R5900Regs.Reg[R_RT].u16_96_111) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = (PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 > PS2Regs.R5900Regs.Reg[R_RT].u16_112_127) ? 0xFFFF : 0;
				break;

			case 0x000001C0: // "pmaxh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = (PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 > PS2Regs.R5900Regs.Reg[R_RT].i16_00_15) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 : PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = (PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 > PS2Regs.R5900Regs.Reg[R_RT].i16_16_31) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 : PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = (PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 > PS2Regs.R5900Regs.Reg[R_RT].i16_32_47) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 : PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = (PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 > PS2Regs.R5900Regs.Reg[R_RT].i16_48_63) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 : PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = (PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 > PS2Regs.R5900Regs.Reg[R_RT].i16_64_79) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 : PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = (PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 > PS2Regs.R5900Regs.Reg[R_RT].i16_80_95) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 : PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = (PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 > PS2Regs.R5900Regs.Reg[R_RT].i16_96_111) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 : PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = (PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 > PS2Regs.R5900Regs.Reg[R_RT].i16_112_127) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 : PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x00000200: // "paddb"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 + PS2Regs.R5900Regs.Reg[R_RT].u08_00_07;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = PS2Regs.R5900Regs.Reg[R_RS].u08_08_15 + PS2Regs.R5900Regs.Reg[R_RT].u08_08_15;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = PS2Regs.R5900Regs.Reg[R_RS].u08_16_23 + PS2Regs.R5900Regs.Reg[R_RT].u08_16_23;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = PS2Regs.R5900Regs.Reg[R_RS].u08_24_31 + PS2Regs.R5900Regs.Reg[R_RT].u08_24_31;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = PS2Regs.R5900Regs.Reg[R_RS].u08_32_39 + PS2Regs.R5900Regs.Reg[R_RT].u08_32_39;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = PS2Regs.R5900Regs.Reg[R_RS].u08_40_47 + PS2Regs.R5900Regs.Reg[R_RT].u08_40_47;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = PS2Regs.R5900Regs.Reg[R_RS].u08_48_55 + PS2Regs.R5900Regs.Reg[R_RT].u08_48_55;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = PS2Regs.R5900Regs.Reg[R_RS].u08_56_63 + PS2Regs.R5900Regs.Reg[R_RT].u08_56_63;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = PS2Regs.R5900Regs.Reg[R_RS].u08_64_71 + PS2Regs.R5900Regs.Reg[R_RT].u08_64_71;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = PS2Regs.R5900Regs.Reg[R_RS].u08_72_79 + PS2Regs.R5900Regs.Reg[R_RT].u08_72_79;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = PS2Regs.R5900Regs.Reg[R_RS].u08_80_87 + PS2Regs.R5900Regs.Reg[R_RT].u08_80_87;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = PS2Regs.R5900Regs.Reg[R_RS].u08_88_95 + PS2Regs.R5900Regs.Reg[R_RT].u08_88_95;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = PS2Regs.R5900Regs.Reg[R_RS].u08_96_103 + PS2Regs.R5900Regs.Reg[R_RT].u08_96_103;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = PS2Regs.R5900Regs.Reg[R_RS].u08_104_111 + PS2Regs.R5900Regs.Reg[R_RT].u08_104_111;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = PS2Regs.R5900Regs.Reg[R_RS].u08_112_119 + PS2Regs.R5900Regs.Reg[R_RT].u08_112_119;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = PS2Regs.R5900Regs.Reg[R_RS].u08_120_127 + PS2Regs.R5900Regs.Reg[R_RT].u08_120_127;
				break;

			case 0x00000240: // "psubb"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 - PS2Regs.R5900Regs.Reg[R_RT].u08_00_07;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = PS2Regs.R5900Regs.Reg[R_RS].u08_08_15 - PS2Regs.R5900Regs.Reg[R_RT].u08_08_15;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = PS2Regs.R5900Regs.Reg[R_RS].u08_16_23 - PS2Regs.R5900Regs.Reg[R_RT].u08_16_23;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = PS2Regs.R5900Regs.Reg[R_RS].u08_24_31 - PS2Regs.R5900Regs.Reg[R_RT].u08_24_31;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = PS2Regs.R5900Regs.Reg[R_RS].u08_32_39 - PS2Regs.R5900Regs.Reg[R_RT].u08_32_39;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = PS2Regs.R5900Regs.Reg[R_RS].u08_40_47 - PS2Regs.R5900Regs.Reg[R_RT].u08_40_47;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = PS2Regs.R5900Regs.Reg[R_RS].u08_48_55 - PS2Regs.R5900Regs.Reg[R_RT].u08_48_55;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = PS2Regs.R5900Regs.Reg[R_RS].u08_56_63 - PS2Regs.R5900Regs.Reg[R_RT].u08_56_63;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = PS2Regs.R5900Regs.Reg[R_RS].u08_64_71 - PS2Regs.R5900Regs.Reg[R_RT].u08_64_71;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = PS2Regs.R5900Regs.Reg[R_RS].u08_72_79 - PS2Regs.R5900Regs.Reg[R_RT].u08_72_79;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = PS2Regs.R5900Regs.Reg[R_RS].u08_80_87 - PS2Regs.R5900Regs.Reg[R_RT].u08_80_87;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = PS2Regs.R5900Regs.Reg[R_RS].u08_88_95 - PS2Regs.R5900Regs.Reg[R_RT].u08_88_95;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = PS2Regs.R5900Regs.Reg[R_RS].u08_96_103 - PS2Regs.R5900Regs.Reg[R_RT].u08_96_103;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = PS2Regs.R5900Regs.Reg[R_RS].u08_104_111 - PS2Regs.R5900Regs.Reg[R_RT].u08_104_111;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = PS2Regs.R5900Regs.Reg[R_RS].u08_112_119 - PS2Regs.R5900Regs.Reg[R_RT].u08_112_119;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = PS2Regs.R5900Regs.Reg[R_RS].u08_120_127 - PS2Regs.R5900Regs.Reg[R_RT].u08_120_127;
				break;

			case 0x00000280: // "pcgtb"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = (PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 > PS2Regs.R5900Regs.Reg[R_RT].u08_00_07) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = (PS2Regs.R5900Regs.Reg[R_RS].u08_08_15 > PS2Regs.R5900Regs.Reg[R_RT].u08_08_15) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = (PS2Regs.R5900Regs.Reg[R_RS].u08_16_23 > PS2Regs.R5900Regs.Reg[R_RT].u08_16_23) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = (PS2Regs.R5900Regs.Reg[R_RS].u08_24_31 > PS2Regs.R5900Regs.Reg[R_RT].u08_24_31) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = (PS2Regs.R5900Regs.Reg[R_RS].u08_32_39 > PS2Regs.R5900Regs.Reg[R_RT].u08_32_39) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = (PS2Regs.R5900Regs.Reg[R_RS].u08_40_47 > PS2Regs.R5900Regs.Reg[R_RT].u08_40_47) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = (PS2Regs.R5900Regs.Reg[R_RS].u08_48_55 > PS2Regs.R5900Regs.Reg[R_RT].u08_48_55) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = (PS2Regs.R5900Regs.Reg[R_RS].u08_56_63 > PS2Regs.R5900Regs.Reg[R_RT].u08_56_63) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = (PS2Regs.R5900Regs.Reg[R_RS].u08_64_71 > PS2Regs.R5900Regs.Reg[R_RT].u08_64_71) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = (PS2Regs.R5900Regs.Reg[R_RS].u08_72_79 > PS2Regs.R5900Regs.Reg[R_RT].u08_72_79) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = (PS2Regs.R5900Regs.Reg[R_RS].u08_80_87 > PS2Regs.R5900Regs.Reg[R_RT].u08_80_87) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = (PS2Regs.R5900Regs.Reg[R_RS].u08_88_95 > PS2Regs.R5900Regs.Reg[R_RT].u08_88_95) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = (PS2Regs.R5900Regs.Reg[R_RS].u08_96_103 > PS2Regs.R5900Regs.Reg[R_RT].u08_96_103) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = (PS2Regs.R5900Regs.Reg[R_RS].u08_104_111 > PS2Regs.R5900Regs.Reg[R_RT].u08_104_111) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = (PS2Regs.R5900Regs.Reg[R_RS].u08_112_119 > PS2Regs.R5900Regs.Reg[R_RT].u08_112_119) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = (PS2Regs.R5900Regs.Reg[R_RS].u08_120_127 > PS2Regs.R5900Regs.Reg[R_RT].u08_120_127) ? 0xFF : 0;
				break;

			case 0x00000400: // "paddsw"
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 + PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = Temp128.u32_00_31;
				}
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_32_63 + PS2Regs.R5900Regs.Reg[R_RT].i32_32_63;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = Temp128.u32_00_31;
				}
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 + PS2Regs.R5900Regs.Reg[R_RT].i32_64_95;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = Temp128.u32_00_31;
				}
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_96_127 + PS2Regs.R5900Regs.Reg[R_RT].i32_96_127;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = Temp128.u32_00_31;
				}
				break;

			case 0x00000440: // "psubsw"
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 - PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = Temp128.u32_00_31;
				}
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_32_63 - PS2Regs.R5900Regs.Reg[R_RT].i32_32_63;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = Temp128.u32_00_31;
				}
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 - PS2Regs.R5900Regs.Reg[R_RT].i32_64_95;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = Temp128.u32_00_31;
				}
				Temp128.i64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_96_127 - PS2Regs.R5900Regs.Reg[R_RT].i32_96_127;
				if (Temp128.i64_00_63 > 0x7FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = 0x7FFFFFFF;
				}
				else if ((0x10000000 <= Temp128.i64_00_63) && (Temp128.i64_00_63 < 0x80000000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = 0x80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = Temp128.u32_00_31;
				}
				break;

			case 0x00000480: // "pextlw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63;
				break;

			case 0x000004C0: // "ppacw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95;
				break;

			case 0x00000500: // "paddsh"
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 + PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 + PS2Regs.R5900Regs.Reg[R_RT].i16_16_31;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 + PS2Regs.R5900Regs.Reg[R_RT].i16_32_47;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i16_32_47 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 + PS2Regs.R5900Regs.Reg[R_RT].i16_48_63;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 + PS2Regs.R5900Regs.Reg[R_RT].i16_64_79;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 + PS2Regs.R5900Regs.Reg[R_RT].i16_80_95;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 + PS2Regs.R5900Regs.Reg[R_RT].i16_96_111;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 + PS2Regs.R5900Regs.Reg[R_RT].i16_112_127;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = Temp128.u16_00_15;
				}
				break;

			case 0x00000540: // "psubsh"
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 - PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 - PS2Regs.R5900Regs.Reg[R_RT].i16_16_31;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 - PS2Regs.R5900Regs.Reg[R_RT].i16_32_47;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 - PS2Regs.R5900Regs.Reg[R_RT].i16_48_63;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 - PS2Regs.R5900Regs.Reg[R_RT].i16_64_79;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 - PS2Regs.R5900Regs.Reg[R_RT].i16_80_95;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 - PS2Regs.R5900Regs.Reg[R_RT].i16_96_111;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = Temp128.u16_00_15;
				}
				Temp128.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 - PS2Regs.R5900Regs.Reg[R_RT].i16_112_127;
				if (Temp128.i32_00_31 > 0x7FFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = 0x7FFF;
				}
				else if ((0x10000 <= Temp128.i32_00_31) && (Temp128.i32_00_31 < 0x18000))
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = 0x8000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = Temp128.u16_00_15;
				}
				break;

			case 0x00000580: // "pextlh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_48_63;
				break;

			case 0x000005C0: // "ppach"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111;
				break;

				/////////////////////////////////////////////////////////////////////////
			case 0x00000600: // "paddsb"
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_00_07 + PS2Regs.R5900Regs.Reg[R_RT].i08_00_07;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_00_07 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_08_15 + PS2Regs.R5900Regs.Reg[R_RT].i08_08_15;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_08_15 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_16_23 + PS2Regs.R5900Regs.Reg[R_RT].i08_16_23;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_16_23 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_24_31 + PS2Regs.R5900Regs.Reg[R_RT].i08_24_31;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_24_31 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_32_39 + PS2Regs.R5900Regs.Reg[R_RT].i08_32_39;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_32_39 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_40_47 + PS2Regs.R5900Regs.Reg[R_RT].i08_40_47;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_40_47 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_48_55 + PS2Regs.R5900Regs.Reg[R_RT].i08_48_55;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_48_55 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_56_63 + PS2Regs.R5900Regs.Reg[R_RT].i08_56_63;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_56_63 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_64_71 + PS2Regs.R5900Regs.Reg[R_RT].i08_64_71;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_64_71 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_72_79 + PS2Regs.R5900Regs.Reg[R_RT].i08_72_79;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_72_79 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_80_87 + PS2Regs.R5900Regs.Reg[R_RT].i08_80_87;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_80_87 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_88_95 + PS2Regs.R5900Regs.Reg[R_RT].i08_88_95;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_88_95 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_96_103 + PS2Regs.R5900Regs.Reg[R_RT].i08_96_103;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_96_103 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_104_111 + PS2Regs.R5900Regs.Reg[R_RT].i08_104_111;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_104_111 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_112_119 + PS2Regs.R5900Regs.Reg[R_RT].i08_112_119;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_112_119 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_120_127 + PS2Regs.R5900Regs.Reg[R_RT].i08_120_127;
				if (TempI32 > 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_120_127 = TempI32;
				}
				break;

			case 0x00000640: // "psubsb"
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_00_07 - PS2Regs.R5900Regs.Reg[R_RT].i08_00_07;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_00_07 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_00_07 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_08_15 - PS2Regs.R5900Regs.Reg[R_RT].i08_08_15;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_08_15 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_08_15 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_16_23 - PS2Regs.R5900Regs.Reg[R_RT].i08_16_23;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_16_23 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_16_23 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_24_31 - PS2Regs.R5900Regs.Reg[R_RT].i08_24_31;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_24_31 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_24_31 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_32_39 - PS2Regs.R5900Regs.Reg[R_RT].i08_32_39;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_32_39 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_32_39 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_40_47 - PS2Regs.R5900Regs.Reg[R_RT].i08_40_47;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_40_47 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_40_47 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_48_55 - PS2Regs.R5900Regs.Reg[R_RT].i08_48_55;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_48_55 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_48_55 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_56_63 - PS2Regs.R5900Regs.Reg[R_RT].i08_56_63;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_56_63 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_56_63 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_64_71 - PS2Regs.R5900Regs.Reg[R_RT].i08_64_71;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_64_71 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_64_71 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_72_79 - PS2Regs.R5900Regs.Reg[R_RT].i08_72_79;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_72_79 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_72_79 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_80_87 - PS2Regs.R5900Regs.Reg[R_RT].i08_80_87;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_80_87 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_80_87 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_88_95 - PS2Regs.R5900Regs.Reg[R_RT].i08_88_95;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_88_95 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_88_95 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_96_103 - PS2Regs.R5900Regs.Reg[R_RT].i08_96_103;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_96_103 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_96_103 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_104_111 - PS2Regs.R5900Regs.Reg[R_RT].i08_104_111;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_104_111 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_104_111 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_112_119 - PS2Regs.R5900Regs.Reg[R_RT].i08_112_119;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_112_119 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_112_119 = TempI32;
				}
				TempI32 = PS2Regs.R5900Regs.Reg[R_RS].i08_120_127 - PS2Regs.R5900Regs.Reg[R_RT].i08_120_127;
				if (TempI32 >= 0x7f)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x7f;
				}
				else if (0x100 <= TempI32 && TempI32 < 0x180)
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_120_127 = (EMU_I08)0x80;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].i08_120_127 = TempI32;
				}
				break;

			case 0x00000680: // "pextlb"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = PS2Regs.R5900Regs.Reg[R_RT].u08_00_07;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = PS2Regs.R5900Regs.Reg[R_RS].u08_00_07;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = PS2Regs.R5900Regs.Reg[R_RT].u08_08_15;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = PS2Regs.R5900Regs.Reg[R_RS].u08_08_15;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = PS2Regs.R5900Regs.Reg[R_RT].u08_16_23;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = PS2Regs.R5900Regs.Reg[R_RS].u08_16_23;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = PS2Regs.R5900Regs.Reg[R_RT].u08_24_31;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = PS2Regs.R5900Regs.Reg[R_RS].u08_24_31;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = PS2Regs.R5900Regs.Reg[R_RT].u08_32_39;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = PS2Regs.R5900Regs.Reg[R_RS].u08_32_39;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = PS2Regs.R5900Regs.Reg[R_RT].u08_40_47;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = PS2Regs.R5900Regs.Reg[R_RS].u08_40_47;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = PS2Regs.R5900Regs.Reg[R_RT].u08_48_55;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = PS2Regs.R5900Regs.Reg[R_RS].u08_48_55;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = PS2Regs.R5900Regs.Reg[R_RT].u08_56_63;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = PS2Regs.R5900Regs.Reg[R_RS].u08_56_63;
				break;

			case 0x000006C0: // "ppacb"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = PS2Regs.R5900Regs.Reg[R_RT].u08_00_07;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = PS2Regs.R5900Regs.Reg[R_RT].u08_16_23;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = PS2Regs.R5900Regs.Reg[R_RT].u08_32_39;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = PS2Regs.R5900Regs.Reg[R_RT].u08_48_55;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = PS2Regs.R5900Regs.Reg[R_RT].u08_64_71;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = PS2Regs.R5900Regs.Reg[R_RT].u08_80_87;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = PS2Regs.R5900Regs.Reg[R_RT].u08_96_103;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = PS2Regs.R5900Regs.Reg[R_RT].u08_112_119;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = PS2Regs.R5900Regs.Reg[R_RS].u08_00_07;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = PS2Regs.R5900Regs.Reg[R_RS].u08_16_23;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = PS2Regs.R5900Regs.Reg[R_RS].u08_32_39;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = PS2Regs.R5900Regs.Reg[R_RS].u08_48_55;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = PS2Regs.R5900Regs.Reg[R_RS].u08_64_71;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = PS2Regs.R5900Regs.Reg[R_RS].u08_80_87;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = PS2Regs.R5900Regs.Reg[R_RS].u08_96_103;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = PS2Regs.R5900Regs.Reg[R_RS].u08_112_119;
				break;

			case 0x00000780: // "pext5"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
					((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
					((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
					((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) |
					((TempU32 & 0x03e0) << 6) | ((TempU32 & 0x001f) << 3);
				break;

			case 0x000007C0: // "ppac5"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
					((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
					((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
					((TempU32 & 0x0000f800) >> 6) | ((TempU32 & 0x000000f8) >> 3);
				TempU32 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) |
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
			switch (OpCode & 0x000007C0)
			{
			case 0x00000000: // "pmaddw"
				TempI64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)PS2Regs.R5900Regs.LO.u32_00_31) + ((EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = TempI64;
				TempI64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)PS2Regs.R5900Regs.LO.u32_64_95) + ((EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 * (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].i32_64_95);
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = TempI64;
				break;

			case 0x00000080: // "psllvw"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)((EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 << (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x1F));
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = (EMU_I32)((EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 << (PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 & 0x1F));
				break;

			case 0x000000C0: // "psrlvw"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x1F));
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 >> (PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 & 0x1F));
				break;

			case 0x00000100: // "pmsubw"
				TempI64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)PS2Regs.R5900Regs.LO.u32_00_31) - ((EMU_I64)PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = TempI64;
				TempI64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_64_95 << 32) | PS2Regs.R5900Regs.LO.u32_64_95) - (PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 * PS2Regs.R5900Regs.Reg[R_RT].i32_64_95);
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = TempI64;
				break;

			case 0x00000200: // "pmfhi",
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.HI.u64_00_63;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.HI.u64_64_127;
				break;

			case 0x00000240: // "pmflo"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.LO.u64_64_127;
				break;

			case 0x00000280: // "pinth"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127;
				break;

			case 0x00000300: // "pmultw"
				TempI64 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
				if (R_RD)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
				}
				TempI64 = PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 * PS2Regs.R5900Regs.Reg[R_RT].i32_64_95;
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
				if (R_RD)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.LO.u64_64_127;
				}
				break;

			case 0x00000340: // "pdivw"
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 /
					PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 /
					PS2Regs.R5900Regs.Reg[R_RT].i32_64_95);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 %
					PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 %
					PS2Regs.R5900Regs.Reg[R_RT].i32_64_95);
				break;

			case 0x00000380: // "pcpyld"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
				break;

			case 0x00000400: // "pmaddh"
				PS2Regs.R5900Regs.LO.u32_00_31 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_00_31 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 * PS2Regs.R5900Regs.Reg[R_RT].i16_00_15));
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_00_31;
				PS2Regs.R5900Regs.LO.u32_32_63 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_32_63 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 * PS2Regs.R5900Regs.Reg[R_RT].i16_16_31));
				PS2Regs.R5900Regs.HI.u32_00_31 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_00_31 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 * PS2Regs.R5900Regs.Reg[R_RT].i16_32_47));
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_00_31;
				PS2Regs.R5900Regs.HI.u32_32_63 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_32_63 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 * PS2Regs.R5900Regs.Reg[R_RT].i16_48_63));
				PS2Regs.R5900Regs.LO.u32_64_95 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_64_95 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 * PS2Regs.R5900Regs.Reg[R_RT].i16_64_79));
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_64_95;
				PS2Regs.R5900Regs.LO.u32_96_127 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_96_127 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 * PS2Regs.R5900Regs.Reg[R_RT].i16_80_95));
				PS2Regs.R5900Regs.HI.u32_64_95 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_64_95 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 * PS2Regs.R5900Regs.Reg[R_RT].i16_96_111));
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_64_95;
				PS2Regs.R5900Regs.HI.u32_96_127 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_96_127 +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 * PS2Regs.R5900Regs.Reg[R_RT].i16_112_127));
				break;

			case 0x00000440: // "phmadh"
				PS2Regs.R5900Regs.LO.u32_00_31 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 * PS2Regs.R5900Regs.Reg[R_RT].i16_16_31) +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 * PS2Regs.R5900Regs.Reg[R_RT].i16_00_15));
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_00_31;
				PS2Regs.R5900Regs.HI.u32_00_31 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 * PS2Regs.R5900Regs.Reg[R_RT].i16_48_63) +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 * PS2Regs.R5900Regs.Reg[R_RT].i16_32_47));
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_00_31;
				PS2Regs.R5900Regs.LO.u32_64_95 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 * PS2Regs.R5900Regs.Reg[R_RT].i16_80_95) +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 * PS2Regs.R5900Regs.Reg[R_RT].i16_64_79));
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_64_95;
				PS2Regs.R5900Regs.HI.u32_64_95 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 * PS2Regs.R5900Regs.Reg[R_RT].i16_112_127) +
					(PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 * PS2Regs.R5900Regs.Reg[R_RT].i16_96_111));
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_64_95;
				break;

			case 0x00000480: // "pand" 
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 &
					PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_64_127 &
					PS2Regs.R5900Regs.Reg[R_RT].u64_64_127;
				break;

			case 0x000004C0: // "pxor" 
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 ^
					PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_64_127 ^
					PS2Regs.R5900Regs.Reg[R_RT].u64_64_127;
				break;

			case 0x00000500: // "pmsubh"
				PS2Regs.R5900Regs.LO.u32_00_31 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_00_31 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 * PS2Regs.R5900Regs.Reg[R_RT].i16_00_15));
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_00_31;
				PS2Regs.R5900Regs.LO.u32_32_63 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_32_63 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 * PS2Regs.R5900Regs.Reg[R_RT].i16_16_31));
				PS2Regs.R5900Regs.HI.u32_00_31 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_00_31 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 * PS2Regs.R5900Regs.Reg[R_RT].i16_32_47));
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_00_31;
				PS2Regs.R5900Regs.HI.u32_32_63 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_32_63 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 * PS2Regs.R5900Regs.Reg[R_RT].i16_48_63));
				PS2Regs.R5900Regs.LO.u32_64_95 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_64_95 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 * PS2Regs.R5900Regs.Reg[R_RT].i16_64_79));
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_64_95;
				PS2Regs.R5900Regs.LO.u32_96_127 = (EMU_I32)(PS2Regs.R5900Regs.LO.u32_96_127 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 * PS2Regs.R5900Regs.Reg[R_RT].i16_80_95));
				PS2Regs.R5900Regs.HI.u32_64_95 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_64_95 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 * PS2Regs.R5900Regs.Reg[R_RT].i16_96_111));
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_64_95;
				PS2Regs.R5900Regs.HI.u32_96_127 = (EMU_I32)(PS2Regs.R5900Regs.HI.u32_96_127 -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 * PS2Regs.R5900Regs.Reg[R_RT].i16_112_127));
				break;

			case 0x00000540: // "phmsbh"
				PS2Regs.R5900Regs.LO.u32_00_31 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 * PS2Regs.R5900Regs.Reg[R_RT].i16_16_31) -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 * PS2Regs.R5900Regs.Reg[R_RT].i16_00_15));
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_00_31;
				PS2Regs.R5900Regs.HI.u32_00_31 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 * PS2Regs.R5900Regs.Reg[R_RT].i16_48_63) -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 * PS2Regs.R5900Regs.Reg[R_RT].i16_32_47));
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_00_31;
				PS2Regs.R5900Regs.LO.u32_64_95 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 * PS2Regs.R5900Regs.Reg[R_RT].i16_80_95) -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 * PS2Regs.R5900Regs.Reg[R_RT].i16_64_79));
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_64_95;
				PS2Regs.R5900Regs.HI.u32_64_95 = (EMU_I32)(
					(PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 * PS2Regs.R5900Regs.Reg[R_RT].i16_112_127) -
					(PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 * PS2Regs.R5900Regs.Reg[R_RT].i16_96_111));
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_64_95;
				break;

			case 0x00000680: // "pexeh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x000006C0: // "prevh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				break;

			case 0x00000700: // "pmulth"
				PS2Regs.R5900Regs.LO.u32_00_31 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_00_15);
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_00_31;
				PS2Regs.R5900Regs.LO.u32_32_63 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_16_31);
				PS2Regs.R5900Regs.HI.u32_00_31 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_32_47);
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_00_31;
				PS2Regs.R5900Regs.HI.u32_32_63 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_48_63);
				PS2Regs.R5900Regs.LO.u32_64_95 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_64_79);
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_64_95;
				PS2Regs.R5900Regs.LO.u32_96_127 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_80_95);
				PS2Regs.R5900Regs.HI.u32_64_95 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 *
					PS2Regs.R5900Regs.Reg[R_RT].i16_96_111);
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_64_95;
				PS2Regs.R5900Regs.HI.u32_96_127 = (EMU_I16)(PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 *
					PS2Regs.R5900Regs.Reg[R_RT].u16_112_127);
				break;

			case 0x00000740: // "pdivbw"
				PS2Regs.R5900Regs.LO.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 / PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.LO.i32_32_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_32_63 / PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.LO.i32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 / PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.LO.i32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].i32_96_127 / PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.HI.i32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 % PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.HI.i32_32_63 = PS2Regs.R5900Regs.Reg[R_RS].i32_32_63 % PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.HI.i32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 % PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				PS2Regs.R5900Regs.HI.i32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].i32_96_127 % PS2Regs.R5900Regs.Reg[R_RT].i16_00_15;
				break;

			case 0x00000780: // "pexew"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;

			case 0x000007C0: // "prot3w"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;
			}
			///////////////////////////////////
			// End of MMI2 Funcions
			///////////////////////////////////
			break;

		case 0x00000010: // "mfhi1"
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.HI.u64_64_127;
			break;

		case 0x00000011: // "mthi1"
			PS2Regs.R5900Regs.HI.u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
			break;

		case 0x00000012: // "mflo1"
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_64_127;
			break;

		case 0x00000013: // "mtlo1"
			PS2Regs.R5900Regs.LO.u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
			break;

		case 0x00000018: // "mult1"
			TempI64 = (EMU_I64)PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
			PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)(TempI64 >> 32);
			if (R_RD)
			{
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_64_127;
			}
			break;

		case 0x00000019: // "multu1"
			TempU64 = (EMU_U64)PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
			PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
			PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)(TempU64 >> 32);
			if (R_RD)
			{
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_64_127;
			}
			break;

		case 0x0000001A: // "div1"
			PS2Regs.R5900Regs.LO.u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 / PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
			PS2Regs.R5900Regs.HI.u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 % PS2Regs.R5900Regs.Reg[R_RT].i32_00_31;
			break;

		case 0x0000001B: // "divu1"
			PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 / PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
			PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 % PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
			break;

		case 0x00000020: // "madd1"
			TempI64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_64_95 << 32) | PS2Regs.R5900Regs.LO.u32_64_95) + (PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 * PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
			PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempI64 & 0x00000000FFFFFFFF);
			PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempI64 >> 32) & 0x00000000FFFFFFFF);
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_64_127;
			break;

		case 0x00000021: // "maddu1"
			TempU64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_64_95 << 32) | PS2Regs.R5900Regs.LO.u32_64_95) + (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 * PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
			PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
			PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
			PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_64_127;
			break;

		case 0x00000028: // "MMI1"
			///////////////////////////////////
			// MMI1 Funcions
			///////////////////////////////////
			switch (OpCode & 0x000007C0)
			{
			case 0x00000040: // "pabsw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = abs(PS2Regs.R5900Regs.Reg[R_RT].i32_00_31);
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = abs(PS2Regs.R5900Regs.Reg[R_RT].i32_32_63);
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = abs(PS2Regs.R5900Regs.Reg[R_RT].i32_64_95);
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = abs(PS2Regs.R5900Regs.Reg[R_RT].i32_96_127);
				break;

			case 0x00000080: // "pceqw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 == PS2Regs.R5900Regs.Reg[R_RT].u32_00_31) ? 0xFFFFFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = (PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 == PS2Regs.R5900Regs.Reg[R_RT].u32_32_63) ? 0xFFFFFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = (PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 == PS2Regs.R5900Regs.Reg[R_RT].u32_64_95) ? 0xFFFFFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = (PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 == PS2Regs.R5900Regs.Reg[R_RT].u32_96_127) ? 0xFFFFFFFF : 0;
				break;

			case 0x000000C0: // "pminw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = (PS2Regs.R5900Regs.Reg[R_RS].i32_00_31 < PS2Regs.R5900Regs.Reg[R_RT].i32_00_31) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 : PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = (PS2Regs.R5900Regs.Reg[R_RS].i32_32_63 < PS2Regs.R5900Regs.Reg[R_RT].i32_32_63) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 : PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = (PS2Regs.R5900Regs.Reg[R_RS].i32_64_95 < PS2Regs.R5900Regs.Reg[R_RT].i32_64_95) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 : PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = (PS2Regs.R5900Regs.Reg[R_RS].i32_96_127 < PS2Regs.R5900Regs.Reg[R_RT].i32_96_127) ?
					PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 : PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;

			case 0x00000100: // "padsbh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 - PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 - PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 - PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 - PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 + PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 + PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 + PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 + PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x00000140: // "pabsh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_00_15);
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_16_31);
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_32_47);
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_48_63);
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_64_79);
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_80_95);
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_96_111);
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = abs(PS2Regs.R5900Regs.Reg[R_RT].u16_112_127);
				break;

			case 0x00000180: // "pceqh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = (PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 == PS2Regs.R5900Regs.Reg[R_RT].u16_00_15) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = (PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 == PS2Regs.R5900Regs.Reg[R_RT].u16_16_31) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = (PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 == PS2Regs.R5900Regs.Reg[R_RT].u16_32_47) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = (PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 == PS2Regs.R5900Regs.Reg[R_RT].u16_48_63) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = (PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 == PS2Regs.R5900Regs.Reg[R_RT].u16_64_79) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = (PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 == PS2Regs.R5900Regs.Reg[R_RT].u16_80_95) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = (PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 == PS2Regs.R5900Regs.Reg[R_RT].u16_96_111) ? 0xFFFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = (PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 == PS2Regs.R5900Regs.Reg[R_RT].u16_112_127) ? 0xFFFF : 0;
				break;

			case 0x000001C0: // "pminh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = (PS2Regs.R5900Regs.Reg[R_RS].i16_00_15 < PS2Regs.R5900Regs.Reg[R_RT].i16_00_15) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 : PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = (PS2Regs.R5900Regs.Reg[R_RS].i16_16_31 < PS2Regs.R5900Regs.Reg[R_RT].i16_16_31) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 : PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = (PS2Regs.R5900Regs.Reg[R_RS].i16_32_47 < PS2Regs.R5900Regs.Reg[R_RT].i16_32_47) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 : PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = (PS2Regs.R5900Regs.Reg[R_RS].i16_48_63 < PS2Regs.R5900Regs.Reg[R_RT].i16_48_63) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 : PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = (PS2Regs.R5900Regs.Reg[R_RS].i16_64_79 < PS2Regs.R5900Regs.Reg[R_RT].i16_64_79) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 : PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = (PS2Regs.R5900Regs.Reg[R_RS].i16_80_95 < PS2Regs.R5900Regs.Reg[R_RT].i16_80_95) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 : PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = (PS2Regs.R5900Regs.Reg[R_RS].i16_96_111 < PS2Regs.R5900Regs.Reg[R_RT].i16_96_111) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 : PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = (PS2Regs.R5900Regs.Reg[R_RS].i16_112_127 < PS2Regs.R5900Regs.Reg[R_RT].i16_112_127) ?
					PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 : PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x00000280: // "pceqb"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = (PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 == PS2Regs.R5900Regs.Reg[R_RT].u08_00_07) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = (PS2Regs.R5900Regs.Reg[R_RS].u08_08_15 == PS2Regs.R5900Regs.Reg[R_RT].u08_08_15) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = (PS2Regs.R5900Regs.Reg[R_RS].u08_16_23 == PS2Regs.R5900Regs.Reg[R_RT].u08_16_23) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = (PS2Regs.R5900Regs.Reg[R_RS].u08_24_31 == PS2Regs.R5900Regs.Reg[R_RT].u08_24_31) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = (PS2Regs.R5900Regs.Reg[R_RS].u08_32_39 == PS2Regs.R5900Regs.Reg[R_RT].u08_32_39) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = (PS2Regs.R5900Regs.Reg[R_RS].u08_40_47 == PS2Regs.R5900Regs.Reg[R_RT].u08_40_47) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = (PS2Regs.R5900Regs.Reg[R_RS].u08_48_55 == PS2Regs.R5900Regs.Reg[R_RT].u08_48_55) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = (PS2Regs.R5900Regs.Reg[R_RS].u08_56_63 == PS2Regs.R5900Regs.Reg[R_RT].u08_56_63) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = (PS2Regs.R5900Regs.Reg[R_RS].u08_64_71 == PS2Regs.R5900Regs.Reg[R_RT].u08_64_71) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = (PS2Regs.R5900Regs.Reg[R_RS].u08_72_79 == PS2Regs.R5900Regs.Reg[R_RT].u08_72_79) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = (PS2Regs.R5900Regs.Reg[R_RS].u08_80_87 == PS2Regs.R5900Regs.Reg[R_RT].u08_80_87) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = (PS2Regs.R5900Regs.Reg[R_RS].u08_88_95 == PS2Regs.R5900Regs.Reg[R_RT].u08_88_95) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = (PS2Regs.R5900Regs.Reg[R_RS].u08_96_103 == PS2Regs.R5900Regs.Reg[R_RT].u08_96_103) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = (PS2Regs.R5900Regs.Reg[R_RS].u08_104_111 == PS2Regs.R5900Regs.Reg[R_RT].u08_104_111) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = (PS2Regs.R5900Regs.Reg[R_RS].u08_112_119 == PS2Regs.R5900Regs.Reg[R_RT].u08_112_119) ? 0xFF : 0;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = (PS2Regs.R5900Regs.Reg[R_RS].u08_120_127 == PS2Regs.R5900Regs.Reg[R_RT].u08_120_127) ? 0xFF : 0;
				break;

			case 0x00000400: // "padduw"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 + PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				if (TempU32 > 0xffffffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0xffffffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 + PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				if (TempU32 > 0xffffffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0xffffffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 + PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				if (TempU32 > 0xffffffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = 0xffffffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 + PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				if (TempU32 > 0xffffffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = 0xffffffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = TempU32;
				}
				break;

			case 0x00000440: // "psubuw"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 - PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63 - PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 - PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u32_96_127 - PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = TempU32;
				}
				break;

			case 0x00000480: // "pextuw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RS].u32_96_127;
				break;

			case 0x00000500: // "padduh"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 + PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 + PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 + PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 + PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 + PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 + PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 + PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 + PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				if (TempU32 > 0xffff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = 0xffff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = TempU32;
				}
				break;

			case 0x00000540: // "psubuh"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15 - PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_16_31 - PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47 - PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_48_63 - PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79 - PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95 - PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111 - PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127 - PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = TempU32;
				}
				break;

			case 0x00000580: // "pextuh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_112_127;
				break;

			case 0x00000600: // "paddub"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 + PS2Regs.R5900Regs.Reg[R_RT].u08_00_07;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_08_15 + PS2Regs.R5900Regs.Reg[R_RT].u08_08_15;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_16_23 + PS2Regs.R5900Regs.Reg[R_RT].u08_16_23;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_24_31 + PS2Regs.R5900Regs.Reg[R_RT].u08_24_31;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_32_39 + PS2Regs.R5900Regs.Reg[R_RT].u08_32_39;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_40_47 + PS2Regs.R5900Regs.Reg[R_RT].u08_40_47;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_48_55 + PS2Regs.R5900Regs.Reg[R_RT].u08_48_55;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_56_63 + PS2Regs.R5900Regs.Reg[R_RT].u08_56_63;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_64_71 + PS2Regs.R5900Regs.Reg[R_RT].u08_64_71;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_72_79 + PS2Regs.R5900Regs.Reg[R_RT].u08_72_79;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_80_87 + PS2Regs.R5900Regs.Reg[R_RT].u08_80_87;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_88_95 + PS2Regs.R5900Regs.Reg[R_RT].u08_88_95;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_96_103 + PS2Regs.R5900Regs.Reg[R_RT].u08_96_103;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_104_111 + PS2Regs.R5900Regs.Reg[R_RT].u08_104_111;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_112_119 + PS2Regs.R5900Regs.Reg[R_RT].u08_112_119;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_120_127 + PS2Regs.R5900Regs.Reg[R_RT].u08_120_127;
				if (TempU32 > 0xff)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = 0xff;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = TempU32;
				}
				break;

			case 0x00000640: // "psubub"
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 - PS2Regs.R5900Regs.Reg[R_RT].u08_00_07;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_08_15 - PS2Regs.R5900Regs.Reg[R_RT].u08_08_15;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_16_23 - PS2Regs.R5900Regs.Reg[R_RT].u08_16_23;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_24_31 - PS2Regs.R5900Regs.Reg[R_RT].u08_24_31;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_32_39 - PS2Regs.R5900Regs.Reg[R_RT].u08_32_39;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_40_47 - PS2Regs.R5900Regs.Reg[R_RT].u08_40_47;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_48_55 - PS2Regs.R5900Regs.Reg[R_RT].u08_48_55;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_56_63 - PS2Regs.R5900Regs.Reg[R_RT].u08_56_63;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_64_71 - PS2Regs.R5900Regs.Reg[R_RT].u08_64_71;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_72_79 - PS2Regs.R5900Regs.Reg[R_RT].u08_72_79;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_80_87 - PS2Regs.R5900Regs.Reg[R_RT].u08_80_87;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_88_95 - PS2Regs.R5900Regs.Reg[R_RT].u08_88_95;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_96_103 - PS2Regs.R5900Regs.Reg[R_RT].u08_96_103;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_104_111 - PS2Regs.R5900Regs.Reg[R_RT].u08_104_111;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_112_119 - PS2Regs.R5900Regs.Reg[R_RT].u08_112_119;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = TempU32;
				}
				TempU32 = PS2Regs.R5900Regs.Reg[R_RS].u08_120_127 - PS2Regs.R5900Regs.Reg[R_RT].u08_120_127;
				if (TempU32 <= 0)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = 0;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = TempU32;
				}
				break;

			case 0x00000680: // "pextub"
				PS2Regs.R5900Regs.Reg[R_RD].u08_00_07 = PS2Regs.R5900Regs.Reg[R_RT].u08_64_71;
				PS2Regs.R5900Regs.Reg[R_RD].u08_08_15 = PS2Regs.R5900Regs.Reg[R_RS].u08_64_71;
				PS2Regs.R5900Regs.Reg[R_RD].u08_16_23 = PS2Regs.R5900Regs.Reg[R_RT].u08_72_79;
				PS2Regs.R5900Regs.Reg[R_RD].u08_24_31 = PS2Regs.R5900Regs.Reg[R_RS].u08_72_79;
				PS2Regs.R5900Regs.Reg[R_RD].u08_32_39 = PS2Regs.R5900Regs.Reg[R_RT].u08_80_87;
				PS2Regs.R5900Regs.Reg[R_RD].u08_40_47 = PS2Regs.R5900Regs.Reg[R_RS].u08_80_87;
				PS2Regs.R5900Regs.Reg[R_RD].u08_48_55 = PS2Regs.R5900Regs.Reg[R_RT].u08_88_95;
				PS2Regs.R5900Regs.Reg[R_RD].u08_56_63 = PS2Regs.R5900Regs.Reg[R_RS].u08_88_95;
				PS2Regs.R5900Regs.Reg[R_RD].u08_64_71 = PS2Regs.R5900Regs.Reg[R_RT].u08_96_103;
				PS2Regs.R5900Regs.Reg[R_RD].u08_72_79 = PS2Regs.R5900Regs.Reg[R_RS].u08_96_103;
				PS2Regs.R5900Regs.Reg[R_RD].u08_80_87 = PS2Regs.R5900Regs.Reg[R_RT].u08_104_111;
				PS2Regs.R5900Regs.Reg[R_RD].u08_88_95 = PS2Regs.R5900Regs.Reg[R_RS].u08_104_111;
				PS2Regs.R5900Regs.Reg[R_RD].u08_96_103 = PS2Regs.R5900Regs.Reg[R_RT].u08_112_119;
				PS2Regs.R5900Regs.Reg[R_RD].u08_104_111 = PS2Regs.R5900Regs.Reg[R_RS].u08_112_119;
				PS2Regs.R5900Regs.Reg[R_RD].u08_112_119 = PS2Regs.R5900Regs.Reg[R_RT].u08_120_127;
				PS2Regs.R5900Regs.Reg[R_RD].u08_120_127 = PS2Regs.R5900Regs.Reg[R_RS].u08_120_127;
				break;

			case 0x000006C0: // "qfsrv"
				if (PS2Regs.R5900Regs.SA)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 << (64 - PS2Regs.R5900Regs.SA)) |
						(PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 >> PS2Regs.R5900Regs.SA);

					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = (PS2Regs.R5900Regs.Reg[R_RS].u64_64_127 << (64 - PS2Regs.R5900Regs.SA)) |
						(PS2Regs.R5900Regs.Reg[R_RT].u64_64_127 >> PS2Regs.R5900Regs.SA);
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.Reg[R_RT].u64_64_127;
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
			switch (OpCode & 0x000007C0)
			{
			case 0x00000000: // "pmadduw"
				TempU64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_00_31 << 32) | PS2Regs.R5900Regs.LO.u32_00_31) + (PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 * PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = TempU64;
				TempU64 = (((EMU_U64)PS2Regs.R5900Regs.HI.u32_64_95 << 32) | PS2Regs.R5900Regs.LO.u32_64_95) + (PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 * PS2Regs.R5900Regs.Reg[R_RT].u32_64_95);
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = TempU64;
				break;

			case 0x000000C0: // "psravw"
				SA = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 & 0x1F;
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 & 0x80000000) ?
					(EMU_I32)((MaxU32 << (32 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> SA)) :
					(EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> SA);
				SA = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 & 0x1F;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = (PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 & 0x80000000) ?
					(EMU_I32)((MaxU32 << (32 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 >> SA)) :
					(EMU_I32)(PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 >> SA);
				break;

			case 0x00000200: // "pmthi"
				PS2Regs.R5900Regs.HI.u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
				PS2Regs.R5900Regs.HI.u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_64_127;
				break;

			case 0x00000240: // "pmtlo"
				PS2Regs.R5900Regs.LO.u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63;
				PS2Regs.R5900Regs.LO.u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_64_127;
				break;

			case 0x00000280: // "pinteh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RS].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RS].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RS].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RS].u16_96_111;
				break;

			case 0x00000300: // "pmultuw"
				TempU64 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 * PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
				if (R_RD)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.u64_00_63;
				}
				TempU64 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 * PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(TempU64 & 0x00000000FFFFFFFF);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)((TempU64 >> 32) & 0x00000000FFFFFFFF);
				if (R_RD)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.LO.u64_64_127;
				}
				break;

			case 0x00000340: // "pdivuw"
				PS2Regs.R5900Regs.LO.u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 /
					PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
				PS2Regs.R5900Regs.LO.u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 /
					PS2Regs.R5900Regs.Reg[R_RT].u32_64_95);
				PS2Regs.R5900Regs.HI.u64_00_63 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_00_31 %
					PS2Regs.R5900Regs.Reg[R_RT].u32_00_31);
				PS2Regs.R5900Regs.HI.u64_64_127 = (EMU_I32)(PS2Regs.R5900Regs.Reg[R_RS].u32_64_95 %
					PS2Regs.R5900Regs.Reg[R_RT].u32_64_95);
				break;

			case 0x00000380: // "pcpyud"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_64_127;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.Reg[R_RT].u64_64_127;
				break;

			case 0x00000480: // "por"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 | PS2Regs.R5900Regs.Reg[R_RT].u64_00_63;
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.Reg[R_RS].u64_64_127 | PS2Regs.R5900Regs.Reg[R_RT].u64_64_127;
				break;

			case 0x000004C0: // "pnor"
				PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = ~(PS2Regs.R5900Regs.Reg[R_RS].u64_00_63 | PS2Regs.R5900Regs.Reg[R_RT].u64_00_63);
				PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = ~(PS2Regs.R5900Regs.Reg[R_RS].u64_64_127 | PS2Regs.R5900Regs.Reg[R_RT].u64_64_127);
				break;

			case 0x00000680: // "pexch"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_80_95;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RT].u16_112_127;
				break;

			case 0x000006C0: // "pcpyh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79;
				break;

			case 0x00000780: // "pexcw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
				break;
			}
			///////////////////////////////////
			// End of MMI3 Funcions
			///////////////////////////////////
			break;

		case 0x00000030: // "pmfhl"
			switch (OpCode & 0x000001FF)
			{
			case 0x00000030: // "pmfhl.lw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_00_31;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_64_95;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_64_95;
				break;

			case 0x00000070: // "pmfhl.uw"
				PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.LO.u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.HI.u32_32_63;
				PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.LO.u32_96_127;
				PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.HI.u32_96_127;
				break;

			case 0x000000B0: // "pmfhl.slw"
				TempU64 = ((EMU_U64)PS2Regs.R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)PS2Regs.R5900Regs.LO.u32_00_31;
				if (TempU64 >= 0x000000007FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = 0x000000007FFFFFFF;
				}
				else if (TempU64 <= 0xFFFFFFFF80000000)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = 0xFFFFFFFF80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_00_63 = PS2Regs.R5900Regs.LO.i32_00_31;
				}
				TempU64 = ((EMU_U64)PS2Regs.R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)PS2Regs.R5900Regs.LO.u32_64_95;
				if (TempU64 >= 0x000000007FFFFFFF)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = 0x000000007FFFFFFF;
				}
				else if (TempU64 <= 0xFFFFFFFF80000000)
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = 0xFFFFFFFF80000000;
				}
				else
				{
					PS2Regs.R5900Regs.Reg[R_RD].u64_64_127 = PS2Regs.R5900Regs.LO.i32_64_95;
				}
				break;

			case 0x000000F0: // "pmfhl.lh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.LO.u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.LO.u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.HI.u16_00_15;
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.HI.u16_32_47;
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.LO.u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.LO.u16_96_111;
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.HI.u16_64_79;
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.HI.u16_96_111;
				break;

			case 0x00000130: // "pmfhl.sh"
				PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = clamp(PS2Regs.R5900Regs.LO.u32_00_31);
				PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = clamp(PS2Regs.R5900Regs.LO.u32_32_63);
				PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = clamp(PS2Regs.R5900Regs.HI.u32_00_31);
				PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = clamp(PS2Regs.R5900Regs.HI.u32_32_63);
				PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = clamp(PS2Regs.R5900Regs.LO.u32_64_95);
				PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = clamp(PS2Regs.R5900Regs.LO.u32_96_127);
				PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = clamp(PS2Regs.R5900Regs.HI.u32_64_95);
				PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = clamp(PS2Regs.R5900Regs.HI.u32_96_127);
				break;
			}

		case 0x00000031: // "pmthl.lw"
			PS2Regs.R5900Regs.LO.u32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].u32_00_31;
			PS2Regs.R5900Regs.HI.u32_00_31 = PS2Regs.R5900Regs.Reg[R_RS].u32_32_63;
			PS2Regs.R5900Regs.LO.u32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].u32_64_95;
			PS2Regs.R5900Regs.HI.u32_64_95 = PS2Regs.R5900Regs.Reg[R_RS].u32_96_127;
			break;

		case 0x00000034: // "psllh"
			PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RT].u16_80_95 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RT].u16_112_127 << R_SA;
			break;

		case 0x00000036: // "psrlh"
			PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = PS2Regs.R5900Regs.Reg[R_RT].u16_00_15 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = PS2Regs.R5900Regs.Reg[R_RT].u16_16_31 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = PS2Regs.R5900Regs.Reg[R_RT].u16_32_47 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = PS2Regs.R5900Regs.Reg[R_RT].u16_48_63 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = PS2Regs.R5900Regs.Reg[R_RT].u16_64_79 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = PS2Regs.R5900Regs.Reg[R_RT].u16_80_95 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = PS2Regs.R5900Regs.Reg[R_RT].u16_96_111 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = PS2Regs.R5900Regs.Reg[R_RT].u16_112_127 >> R_SA;
			break;

		case 0x00000037: // "psrah"
			SA = R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u16_00_15 = (PS2Regs.R5900Regs.Reg[R_RT].u16_00_15 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_00_15 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_00_15 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_16_31 = (PS2Regs.R5900Regs.Reg[R_RT].u16_16_31 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_16_31 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_16_31 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_32_47 = (PS2Regs.R5900Regs.Reg[R_RT].u16_32_47 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_32_47 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_32_47 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_48_63 = (PS2Regs.R5900Regs.Reg[R_RT].u16_48_63 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_48_63 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_48_63 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_64_79 = (PS2Regs.R5900Regs.Reg[R_RT].u16_64_79 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_64_79 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_64_79 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_80_95 = (PS2Regs.R5900Regs.Reg[R_RT].u16_80_95 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_80_95 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_80_95 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_96_111 = (PS2Regs.R5900Regs.Reg[R_RT].u16_96_111 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_96_111 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_96_111 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u16_112_127 = (PS2Regs.R5900Regs.Reg[R_RT].u16_112_127 & 0x8000) ?
				((MaxU16 << (16 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u16_112_127 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u16_112_127 >> SA);
			break;

		case 0x0000003C: // "psllw"
			PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 << R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127 << R_SA;
			break;

		case 0x0000003E: // "psrlw"
			PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 >> R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127 >> R_SA;
			break;

		case 0x0000003F: // "psraw"
			SA = R_SA;
			PS2Regs.R5900Regs.Reg[R_RD].u32_00_31 = (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 & 0x80000000) ?
				((MaxU32 << (32 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u32_32_63 = (PS2Regs.R5900Regs.Reg[R_RT].u32_32_63 & 0x80000000) ?
				((MaxU32 << (32 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u32_32_63 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u32_32_63 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u32_64_95 = (PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 & 0x80000000) ?
				((MaxU32 << (32 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 >> SA);
			PS2Regs.R5900Regs.Reg[R_RD].u32_96_127 = (PS2Regs.R5900Regs.Reg[R_RT].u32_96_127 & 0x80000000) ?
				((MaxU32 << (32 - SA)) | (PS2Regs.R5900Regs.Reg[R_RT].u32_96_127 >> SA)) :
				(PS2Regs.R5900Regs.Reg[R_RT].u32_96_127 >> SA);
			break;
		}
		///////////////////////////////////
		// End of MMI Funcions
		///////////////////////////////////
	}
}