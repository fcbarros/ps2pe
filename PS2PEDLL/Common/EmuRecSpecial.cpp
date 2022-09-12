#include "EmuMain.h"
#include "EmuBios.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

extern BOOL EmuStopCompile;

void EmuRec_sll(EMU_U32 Code)
{
	if (R_RD)
	{
		EMU_U32 Shift = R_SA;
		if (R_RT)
		{
			if (Shift)
			{
				MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				SHL32ItoR(EDX, Shift);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
				SAR32ItoR(EDX, 31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
			}
			else
			{
				MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				if (R_RT != R_RD)
				{
					MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
				}
				SAR32ItoR(EDX, 31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
			}
		}
		else
		{
			MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
			MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
		}
	}
}

void EmuRec_srl(EMU_U32 Code)
{
	if (R_RD)
	{
		EMU_U32 Shift = R_SA;
		if (R_RT)
		{
			if (Shift)
			{
				MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				SHR32ItoR(EDX, Shift);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
			}
			else
			{
				MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				if (R_RT != R_RD)
				{
					MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
				}
				SAR32ItoR(EDX, 31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
			}
		}
		else
		{
			MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
			MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
		}
	}
}

void EmuRec_sra(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ R_RT ].i32_00_31 >> R_SA );
	*/
}

void EmuRec_sllv(EMU_U32 Code)
{
	/*
		TempU32 = R5900Regs.Reg[ R_RT ].u32_00_31 << ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F );
		R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)TempU32;
	*/
}

void EmuRec_srlv(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].u32_00_31 >> ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F );
	*/
}

void EmuRec_srav(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ R_RT ].i32_00_31 >>
												   ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F ) );
	*/
}

void EmuRec_jr(EMU_U32 Code)
{
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code

	ADD32ItoR(EBP, 1);
	PUSH32M((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);

	CALLFunc((EMU_U32)EmuRec_CheckAddress, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);

	JMP32R(EAX);

	EmuStopCompile = TRUE;
}

void EmuRec_jalr(EMU_U32 Code)
{
	/*
		// It does not zero extend the address if it
		// crosses the 32bit bondary
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.PC + 4;
		JumpTo = R5900Regs.Reg[ R_RS ].u32_00_31;
		EXECUTE_BRANCH_DELAY_SLOT( R5900Regs.PC );
		R5900Regs.PC = JumpTo;
	*/
}

void EmuRec_movz(EMU_U32 Code)
{
	/*
		if ( R5900Regs.Reg[ R_RT ].u64_00_63 == 0 )
		{
			R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63;
		}
	*/
}

void EmuRec_movn(EMU_U32 Code)
{
	/*
		if ( R5900Regs.Reg[ R_RT ].u64_00_63 != 0 )
		{
			R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63;
		}
	*/
}

void EmuRec_syscall(EMU_U32 Code)
{
	CALLFunc((EMU_U32)Emu_Bios_Syscall, (EMU_U32)EmuRec_CurrentAddress());
}

void EmuRec_break(EMU_U32 Code)
{
}

void EmuRec_sync(EMU_U32 Code)
{
}

void EmuRec_mfhi(EMU_U32 Code)
{
	if (R_RD)
	{
		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.HI.u32_00_31);
		MOV32MtoR(EAX, (EMU_U32)&R5900Regs.HI.u32_32_63);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EAX);
	}
}

void EmuRec_mthi(EMU_U32 Code)
{
	MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31);
	MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63);
	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_00_31, EDX);
	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_32_63, EAX);
}

void EmuRec_mflo(EMU_U32 Code)
{
	if (R_RD)
	{
		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.LO.u32_00_31);
		MOV32MtoR(EAX, (EMU_U32)&R5900Regs.LO.u32_32_63);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EAX);
	}
}

void EmuRec_mtlo(EMU_U32 Code)
{
	MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31);
	MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63);
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_00_31, EDX);
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_32_63, EAX);
}

void EmuRec_dsllv(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].u64_00_63 <<
										( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x3F );
	*/
}

void EmuRec_dsrlv(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].u64_00_63 >>
										( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x3F );
	*/
}

void EmuRec_dsrav(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].i64_00_63 >>
										( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x3F );
	*/
}

void EmuRec_mult(EMU_U32 Code)
{
	if (R_RS && R_RT)
	{
		MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		IMUL32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
	}
	else
	{
		XOR32RtoR(EAX, EAX);
		XOR32RtoR(EDX, EDX);
	}
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_00_31, EAX);
	if (R_RD)
	{
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
	}
	SAR32ItoR(EAX, 31);
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_32_63, EAX);
	if (R_RD)
	{
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EAX);
	}

	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_00_31, EDX);
	SAR32ItoR(EDX, 31);
	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_32_63, EDX);
}

void EmuRec_multu(EMU_U32 Code)
{
	if (R_RS && R_RT)
	{
		MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		MUL32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
	}
	else
	{
		XOR32RtoR(EAX, EAX);
		XOR32RtoR(EDX, EDX);
	}
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_00_31, EAX);
	if (R_RD)
	{
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
	}
	SAR32ItoR(EAX, 31);
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_32_63, EAX);
	if (R_RD)
	{
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EAX);
	}

	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_00_31, EDX);
	SAR32ItoR(EDX, 31);
	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_32_63, EDX);
}

void EmuRec_div(EMU_U32 Code)
{
	/*
		RS = R_RS;
		RT = R_RT;
		if ( R5900Regs.Reg[ RT ].u32_00_31 )
		{
			R5900Regs.LO.i64_00_63 = (EMU_I32)( R5900Regs.Reg[ RS ].i32_00_31 / R5900Regs.Reg[ RT ].i32_00_31 );
			R5900Regs.HI.i64_00_63 = (EMU_I32)( R5900Regs.Reg[ RS ].i32_00_31 % R5900Regs.Reg[ RT ].i32_00_31 );
		}
	*/
}
// 201580
void EmuRec_divu(EMU_U32 Code)
{
	CMP32ItoM((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31, 0);
	EMU_U08* LinkE0 = JE8(0);

	MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);

	XOR32RtoR(EDX, EDX);
	DIV32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);

	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_00_31, EAX);
	SAR32ItoR(EAX, 31);
	MOV32RtoM((EMU_U32)&R5900Regs.LO.u32_32_63, EAX);

	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_00_31, EDX);
	SAR32ItoR(EDX, 31);
	MOV32RtoM((EMU_U32)&R5900Regs.HI.u32_32_63, EDX);

	*LinkE0 = EmuRec_CurrentAddress() - LinkE0 - 1;
}

void EmuRec_add(EMU_U32 Code)
{
	// ******************************************
	// Not doing the OVERFLOW TRAP
	// ******************************************
	if (R_RD)
	{
		if (R_RS)
		{
			MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
			if (R_RT)
			{
				ADD32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
			}
			MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
			SAR32ItoR(EDX, 31);
			MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
		}
		else
		{
			if (R_RT)
			{
				MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
				SAR32ItoR(EDX, 31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
			}
			else
			{
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
			}
		}
	}
}

void EmuRec_addu(EMU_U32 Code)
{
	if (R_RD)
	{
		if (R_RS)
		{
			MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
			if (R_RT)
			{
				ADD32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
			}
			MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
			SAR32ItoR(EDX, 31);
			MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
		}
		else
		{
			if (R_RT)
			{
				MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EDX);
				SAR32ItoR(EDX, 31);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
			}
			else
			{
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
			}
		}
	}
}

void EmuRec_sub(EMU_U32 Code)
{
	/*
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[ R_RS ].u32_00_31 - (EMU_U64)R5900Regs.Reg[ R_RT ].u32_00_31;
		R5900Regs.Reg[ R_RD ].i64_00_63 = Temp128.i32_00_31;
	*/
}

void EmuRec_subu(EMU_U32 Code)
{
	/*
		Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[ R_RS ].u32_00_31 - (EMU_U64)R5900Regs.Reg[ R_RT ].u32_00_31;
		R5900Regs.Reg[ R_RD ].i64_00_63 = Temp128.i32_00_31;
	*/
}

void EmuRec_and(EMU_U32 Code)
{
	if (R_RD)
	{
		if (R_RT)
		{
			if (R_RS)
			{
				MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);

				AND32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
				AND32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);

				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
				MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EBX);
			}
			else
			{
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
				MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
			}
		}
		else
		{
			MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
			MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);
		}
	}
}

void EmuRec_or(EMU_U32 Code)
{
	if (R_RD)
	{
		if (R_RT)
		{
			MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
			MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
			if (R_RS)
			{
				OR32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				OR32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
			}
		}
		else
		{
			MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
			MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		}
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EBX);
	}
}

void EmuRec_xor(EMU_U32 Code)
{
	if (R_RD)
	{
		if (R_RT)
		{
			MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
			MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
			if (R_RS)
			{
				XOR32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				XOR32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
			}
		}
		else
		{
			MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
			MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		}
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EBX);
	}
}

void EmuRec_nor(EMU_U32 Code)
{
	if (R_RD)
	{
		if (R_RT)
		{
			MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
			MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
			if (R_RS)
			{
				OR32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				OR32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
			}
		}
		else
		{
			MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
			MOV32MtoR(EBX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		}
		NOT32R(EAX);
		NOT32R(EBX);

		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EBX);
	}
}

void EmuRec_mfsa(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.SA;
	*/
}

void EmuRec_mtsa(EMU_U32 Code)
{
	/*
		R5900Regs.SA = R5900Regs.Reg[ R_RS ].u64_00_63;
	*/
}

void EmuRec_slt(EMU_U32 Code)
{
	if (R_RD)
	{
		MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);

		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		CMP32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
		EMU_U08* LinkA = JG8(0);
		EMU_U08* LinkB = JL8(0);
		//  Equal
		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		CMP32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		// Less than
		EMU_U08* LinkAE = JGE8(0);
		EMUREC_ALIGN_BRANCH;
		*LinkB = EmuRec_CurrentAddress() - LinkB - 1;
		MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x1);
		EMU_U08* LinkEnd = JMP8(0);
		// Above or Equal
		EMUREC_ALIGN_BRANCH;
		*LinkA = EmuRec_CurrentAddress() - LinkA - 1;
		*LinkAE = EmuRec_CurrentAddress() - LinkAE - 1;
		MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
		EMUREC_ALIGN_BRANCH;
		*LinkEnd = EmuRec_CurrentAddress() - LinkEnd - 1;
	}
}

void EmuRec_sltu(EMU_U32 Code)
{
	if (R_RD)
	{
		MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, 0x0);

		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		CMP32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
		EMU_U08* LinkA = JA8(0);
		EMU_U08* LinkB = JB8(0);
		//  Equal
		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		CMP32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		// Less than
		EMU_U08* LinkAE = JAE8(0);
		EMUREC_ALIGN_BRANCH;
		*LinkB = EmuRec_CurrentAddress() - LinkB - 1;
		MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x1);
		EMU_U08* LinkEnd = JMP8(0);
		// Above or Equal
		EMUREC_ALIGN_BRANCH;
		*LinkA = EmuRec_CurrentAddress() - LinkA - 1;
		*LinkAE = EmuRec_CurrentAddress() - LinkAE - 1;
		MOV32ItoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, 0x0);
		EMUREC_ALIGN_BRANCH;
		*LinkEnd = EmuRec_CurrentAddress() - LinkEnd - 1;
	}
}

void EmuRec_dadd(EMU_U32 Code)
{
	if (R_RD)
	{
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		ADD32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		ADC32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
	}
}

void EmuRec_daddu(EMU_U32 Code)
{
	if (R_RD)
	{
		MOV32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		MOV32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
		ADD32MtoR(EAX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		ADC32MtoR(EDX, (EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_00_31, EAX);
		MOV32RtoM((EMU_U32)&R5900Regs.Reg[R_RD].u32_32_63, EDX);
	}
}

void EmuRec_dsub(EMU_U32 Code)
{
	/*
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 -
										  R5900Regs.Reg[ R_RT ].u64_00_63;
	*/
}

void EmuRec_dsubu(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 -
										  R5900Regs.Reg[ R_RT ].u64_00_63;
	*/
}

void EmuRec_tge(EMU_U32 Code)
{
}

void EmuRec_tgeu(EMU_U32 Code)
{
}

void EmuRec_tlt(EMU_U32 Code)
{
}

void EmuRec_tltu(EMU_U32 Code)
{
}

void EmuRec_teq(EMU_U32 Code)
{
}

void EmuRec_tne(EMU_U32 Code)
{
}

void EmuRec_dsll(EMU_U32 Code)
{
	if (R_RD)
	{
		MOVQMtoR(MM0, (EMU_U32)&R5900Regs.Reg[R_RT]);
		PSLLQItoR(MM0, R_SA);
		MOVQRtoM((EMU_U32)&R5900Regs.Reg[R_RD], MM0);
		EMMS();

		/*        EMU_U32 Shift = R_SA;
				if ( R_RT )
				{
					MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
					MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
					SHLD32ItoR( EDX, EAX, Shift );
					SHL32ItoR( EAX, Shift );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, EAX );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, EDX );
				}
				else
				{
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, 0x0 );
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, 0x0 );
				}*/
	}
}

void EmuRec_dsrl(EMU_U32 Code)
{
	if (R_RD)
	{
		MOVQMtoR(MM0, (EMU_U32)&R5900Regs.Reg[R_RT]);
		PSRLQItoR(MM0, R_SA);
		MOVQRtoM((EMU_U32)&R5900Regs.Reg[R_RD], MM0);
		EMMS();
		/*
				EMU_U32 Shift = R_SA;
				if ( R_RT )
				{
					MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
					MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
					SHRD32ItoR( EAX, EDX, Shift );
					SHR32ItoR( EDX, Shift );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, EAX );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, EDX );
				}
				else
				{
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, 0x0 );
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, 0x0 );
				}*/
	}
}

void EmuRec_dsra(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].i64_00_63 >> R_SA;
	*/
}

void EmuRec_dsll32(EMU_U32 Code)
{
	if (R_RD)
	{
		MOVQMtoR(MM0, (EMU_U32)&R5900Regs.Reg[R_RT]);
		PSLLQItoR(MM0, R_SA + 32);
		MOVQRtoM((EMU_U32)&R5900Regs.Reg[R_RD], MM0);
		EMMS();

		/*        EMU_U32 Shift = R_SA;
				if ( R_RT )
				{
					MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
					MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
					MOV32RtoR( EDX, EAX );
					SHL32ItoR( EDX, Shift );
					XOR32RtoR( EAX, EAX );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, EAX );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, EDX );
				}
				else
				{
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, 0x0 );
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, 0x0 );
				}*/
	}
}

void EmuRec_dsrl32(EMU_U32 Code)
{
	if (R_RD)
	{
		MOVQMtoR(MM0, (EMU_U32)&R5900Regs.Reg[R_RT]);
		PSRLQItoR(MM0, R_SA + 32);
		MOVQRtoM((EMU_U32)&R5900Regs.Reg[R_RD], MM0);
		EMMS();
		/*
				EMU_U32 Shift = R_SA;
				if ( R_RT )
				{
					MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
					MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
					MOV32RtoR( EAX, EDX );
					SHL32ItoR( EAX, Shift );
					XOR32RtoR( EDX, EDX );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, EAX );
					MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, EDX );
				}
				else
				{
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_00_31, 0x0 );
					MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RD ].u32_32_63, 0x0 );
				} */
	}
}

void EmuRec_dsra32(EMU_U32 Code)
{
	/*
		R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].i64_00_63 >> ( R_SA + 32 );
	*/
}

/*

EI

*/
