#include "Interpreter.h"

#include <cfenv>

namespace Interpreter
{
	void Interpreter::EmuCop1()
	{
		std::feclearexcept(FE_OVERFLOW);
		std::feclearexcept(FE_UNDERFLOW);

		///////////////////////////////////
		// COP1 Functions
		///////////////////////////////////
		switch (OpCode & 0x03E00000)
		{
		case 0x00000000: // "mfc1"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = PS2Regs.COP1Regs.Reg[R_FS].i;
			break;

		case 0x00400000: // "cfc1"
			PS2Regs.R5900Regs.Reg[R_RT].u64_00_63 = (EMU_I32)(R_RD ? PS2Regs.COP1Regs.FCR31 : PS2Regs.COP1Regs.FCR0);
			break;

		case 0x00800000: // "mtc1"
			PS2Regs.COP1Regs.Reg[R_FS].u = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
			break;

		case 0x00C00000: // "ctc1"
			if (R_RD)
			{
				PS2Regs.COP1Regs.FCR31 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
			}
			else
			{
				PS2Regs.COP1Regs.FCR0 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
			}
			break;

		case 0x01000000: // "BC1"
			///////////////////////////////////
			// BC1 Functions
			///////////////////////////////////
			switch (OpCode & 0x001F0000)
			{
			case 0x00000000: // "bc1f",
				if (PS2Regs.COP1Regs.FCR31_C == 0)
				{
					ExecuteBranch(PS2Regs.R5900Regs.PC);
				}
				break;

			case 0x00010000: // "bc1t",
				if (PS2Regs.COP1Regs.FCR31_C == 1)
				{
					ExecuteBranch(PS2Regs.R5900Regs.PC);
				}
				break;

			case 0x00020000: // "bc1fl"
				if (PS2Regs.COP1Regs.FCR31_C == 0)
				{
					ExecuteBranch(PS2Regs.R5900Regs.PC);
				}
				else
				{
					PS2Regs.R5900Regs.PC += 4;
				}
				break;

			case 0x00030000: // "bc1tl"
				if (PS2Regs.COP1Regs.FCR31_C == 1)
				{
					ExecuteBranch(PS2Regs.R5900Regs.PC);
				}
				else
				{
					PS2Regs.R5900Regs.PC += 4;
				}
				break;
			}
			///////////////////////////////////
			// End of BC1 Functions
			///////////////////////////////////
			break;

		case 0x02000000: // "S"
			///////////////////////////////////
			// S Functions
			///////////////////////////////////
			switch (OpCode & 0x0000003F)
			{
			case 0x00000000: // "add.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f + PS2Regs.COP1Regs.Reg[R_FT].f;
				SetFloatFlags();
				break;

			case 0x00000001: // "sub.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f - PS2Regs.COP1Regs.Reg[R_FT].f;
				SetFloatFlags();
				break;

			case 0x00000002: // "mul.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f * PS2Regs.COP1Regs.Reg[R_FT].f;
				SetFloatFlags();
				break;

			case 0x00000003: // "div.s"
				if ((PS2Regs.COP1Regs.Reg[R_FS].u == 0) && (PS2Regs.COP1Regs.Reg[R_FT].u == 0))
				{
					PS2Regs.COP1Regs.FCR31_I = 1;
					PS2Regs.COP1Regs.FCR31_D = 0;
					PS2Regs.COP1Regs.FCR31_SI = 1;
					PS2Regs.COP1Regs.Reg[R_FD].u = 0;
				}
				else if ((PS2Regs.COP1Regs.Reg[R_FS].u != 0) && (PS2Regs.COP1Regs.Reg[R_FT].u == 0))
				{
					PS2Regs.COP1Regs.FCR31_I = 0;
					PS2Regs.COP1Regs.FCR31_D = 1;
					PS2Regs.COP1Regs.FCR31_SD = 1;
				}
				else
				{
					PS2Regs.COP1Regs.FCR31_I = 0;
					PS2Regs.COP1Regs.FCR31_D = 0;
					PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f / PS2Regs.COP1Regs.Reg[R_FT].f;
					SetFloatFlags();
				}
				break;

			case 0x00000004: // "sqrt.s"
				if (PS2Regs.COP1Regs.Reg[R_FT].f < 0.0f)
				{
					PS2Regs.COP1Regs.FCR31_I = 1;
					PS2Regs.COP1Regs.FCR31_SI = 1;
				}
				else
				{
					PS2Regs.COP1Regs.FCR31_I = 0;
					PS2Regs.COP1Regs.Reg[R_FD].f = (float)sqrt((double)PS2Regs.COP1Regs.Reg[R_FT].f);
					SetFloatFlags();
				}
				PS2Regs.COP1Regs.FCR31_D = 0;
				break;

			case 0x00000005: // "abs.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = (float)fabs(PS2Regs.COP1Regs.Reg[R_FS].f);
				PS2Regs.COP1Regs.FCR31_O = 0;
				PS2Regs.COP1Regs.FCR31_U = 0;
				break;

			case 0x00000006: // "mov.s"
				PS2Regs.COP1Regs.Reg[R_FD].u = PS2Regs.COP1Regs.Reg[R_FS].u;
				break;

			case 0x00000007: // "neg.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = -PS2Regs.COP1Regs.Reg[R_FS].f;
				PS2Regs.COP1Regs.FCR31_O = 0;
				PS2Regs.COP1Regs.FCR31_U = 0;
				break;

			case 0x00000016: // "rsqrt.s"
				if (PS2Regs.COP1Regs.Reg[R_FT].f < 0.0f)
				{
					PS2Regs.COP1Regs.FCR31_I = 1;
					PS2Regs.COP1Regs.FCR31_D = 0;
					PS2Regs.COP1Regs.FCR31_SI = 1;
				}
				else if (PS2Regs.COP1Regs.Reg[R_FT].f == 0.0f)
				{
					PS2Regs.COP1Regs.FCR31_I = 0;
					PS2Regs.COP1Regs.FCR31_D = 1;
					PS2Regs.COP1Regs.FCR31_SD = 1;
				}
				else
				{
					PS2Regs.COP1Regs.FCR31_I = 0;
					PS2Regs.COP1Regs.FCR31_D = 0;
					PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f / (float)(sqrt(PS2Regs.COP1Regs.Reg[R_FT].f));
					SetFloatFlags();
				}
				break;

			case 0x00000018: // "adda.s"
				PS2Regs.COP1Regs.Accumulator.f = PS2Regs.COP1Regs.Reg[R_FS].f + PS2Regs.COP1Regs.Reg[R_FT].f;
				SetFloatFlags();
				break;

			case 0x00000019: // "suba.s"
				PS2Regs.COP1Regs.Accumulator.f = PS2Regs.COP1Regs.Reg[R_FS].f - PS2Regs.COP1Regs.Reg[R_FT].f;
				SetFloatFlags();
				break;

			case 0x0000001A: // "mula.s"
				PS2Regs.COP1Regs.Accumulator.f = PS2Regs.COP1Regs.Reg[R_FS].f * PS2Regs.COP1Regs.Reg[R_FT].f;
				SetFloatFlags();
				break;

			case 0x0000001C: // "madd.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f * PS2Regs.COP1Regs.Reg[R_FT].f;
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
					PS2Regs.COP1Regs.Reg[R_FD].f += PS2Regs.COP1Regs.Accumulator.f;
					SetFloatFlags();
				}
				break;

			case 0x0000001D: // "msub.s",
				PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Reg[R_FS].f * PS2Regs.COP1Regs.Reg[R_FT].f;
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
					PS2Regs.COP1Regs.Reg[R_FD].f = PS2Regs.COP1Regs.Accumulator.f - PS2Regs.COP1Regs.Reg[R_FD].f;
					SetFloatFlags();
				}
				break;

			case 0x0000001E: // "madda.s"
				TempF32 = PS2Regs.COP1Regs.Reg[R_FS].f * PS2Regs.COP1Regs.Reg[R_FT].f;
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
					PS2Regs.COP1Regs.Accumulator.f += TempF32;
					SetFloatFlags();
				}
				break;

			case 0x0000001F: // "msuba.s"
				TempF32 = PS2Regs.COP1Regs.Reg[R_FS].f * PS2Regs.COP1Regs.Reg[R_FT].f;
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
					PS2Regs.COP1Regs.Accumulator.f -= TempF32;
					SetFloatFlags();
				}
				break;

			case 0x00000024: // "cvt.w.s"
				PS2Regs.COP1Regs.Reg[R_FD].i = (EMU_I32)PS2Regs.COP1Regs.Reg[R_RD].f;
				break;

			case 0x00000028: // "max.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = (PS2Regs.COP1Regs.Reg[R_FS].f >= PS2Regs.COP1Regs.Reg[R_FT].f) ?
					PS2Regs.COP1Regs.Reg[R_FS].f : PS2Regs.COP1Regs.Reg[R_FT].f;
				PS2Regs.COP1Regs.FCR31_O = 0;
				PS2Regs.COP1Regs.FCR31_U = 0;
				break;

			case 0x00000029: // "min.s"
				PS2Regs.COP1Regs.Reg[R_FD].f = (PS2Regs.COP1Regs.Reg[R_FS].f <= PS2Regs.COP1Regs.Reg[R_FT].f) ?
					PS2Regs.COP1Regs.Reg[R_FS].f : PS2Regs.COP1Regs.Reg[R_FT].f;
				PS2Regs.COP1Regs.FCR31_O = 0;
				PS2Regs.COP1Regs.FCR31_U = 0;
				break;

			case 0x00000030: // "c.f.s"
				PS2Regs.COP1Regs.FCR31_C = 0;
				break;

			case 0x00000032: // "c.eq.s"
				PS2Regs.COP1Regs.FCR31_C = (PS2Regs.COP1Regs.Reg[R_FS].f == PS2Regs.COP1Regs.Reg[R_FT].f);
				break;

			case 0x00000034: // "c.lt.s"
				PS2Regs.COP1Regs.FCR31_C = (PS2Regs.COP1Regs.Reg[R_FS].f < PS2Regs.COP1Regs.Reg[R_FT].f);
				break;

			case 0x00000036: // "c.le.s"
				PS2Regs.COP1Regs.FCR31_C = (PS2Regs.COP1Regs.Reg[R_FS].f <= PS2Regs.COP1Regs.Reg[R_FT].f);
				break;
			}
			///////////////////////////////////
			// S Funcions
			///////////////////////////////////
			break;

		case 0x02800000: // "W"
			///////////////////////////////////
			// W Functions
			///////////////////////////////////
			if ((OpCode & 0x0000003F) == 0x00000020) // "cvt.s.w"
			{
				PS2Regs.COP1Regs.Reg[R_FD].f = (EMU_F32)PS2Regs.COP1Regs.Reg[R_FS].i;
			}
			///////////////////////////////////
			// End of W Functions
			///////////////////////////////////
			break;
		}
		///////////////////////////////////
		// End of COP1 Functions
		///////////////////////////////////
	}
}