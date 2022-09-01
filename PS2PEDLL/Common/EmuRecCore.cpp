
#include "EmuMain.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

#include "EmuMemory.h"
#include "EmuGS.h"
#include "EmuVif.h"
#include "EmuGif.h"
#include "EmuVu.h"
#include "EmuFifo.h"
#include "EmuIntc.h"
#include "EmuDma.h"
#include "EmuIpu.h"
#include "EmuSif.h"
#include "EmuTimer.h"
#include "EmuBios.h"

extern EMM_MapType* Mapping;
extern EMU_U32          MemoryMask;
extern EMM_Data_Type    MainMemory;

extern BOOL EmuStopCompile;
extern BOOL EmuBranchInstruction;

void InsertLoadMemory(void);

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_j(EMU_U32 Code)
{
	EMU_U32 JumpTo = JUMP_ADDRESS;

	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	EMU_U32 LinkAddress = (EMU_U32)EmuRec_CheckAddress(JumpTo);
	if (LinkAddress)
	{
		EmuRec_SetRegDependency(LinkAddress);
	}
	else
	{
		EmuRec_FreeAll();
	}

	InsertBlockUpdate(JumpTo);

	EMU_U32* Link = JMP32(0);
	if (LinkAddress)
	{
		*Link = LinkAddress - (EMU_U32)Link - 4;
	}
	else
	{
		EmuRec_PushPendingJump(Link, JumpTo);
	}

	EmuRec_ResetRegs();

	EmuStopCompile = TRUE;
	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_jal(EMU_U32 Code)
{
	EMU_U32 JumpTo = JUMP_ADDRESS;

	EMU_I32 RegRA = EmuRec_CheckIntRegCache(31, 0);
	if (RegRA >= 0)
	{
		MOV32ItoR(RegRA, R5900Regs.PC + 4);
		EmuRec_SetIntRegModified(RegRA);
	}
	else
	{
		MOV32ItoM((EMU_U32)&R5900Regs.RA.u64_00_63, R5900Regs.PC + 4);
	}

	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(JumpTo);

	EMU_U32 LinkAddress = (EMU_U32)EmuRec_CheckAddress(JumpTo);
	if (LinkAddress)
	{
		EmuRec_SetRegDependency(LinkAddress);
	}
	else
	{
		EmuRec_FreeAll();
	}

	EMU_U32* Link = JMP32(0);
	if (LinkAddress)
	{
		*Link = LinkAddress - (EMU_U32)Link - 4;
	}
	else
	{
		EmuRec_PushPendingJump(Link, JumpTo);
	}

	EmuRec_ResetRegs();

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_beq(EMU_U32 Code)
{
	EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
	EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);
	EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
	EMU_I32 RegRT1 = EmuRec_CheckIntRegCache(R_RT, 1);

	COMPAREINTREGS(RegRS1, RegRT1, R_RS, R_RT, 1);
	EMU_U32* LinkNE1 = JNE32(0);
	//  Equal
	COMPAREINTREGS(RegRS0, RegRT0, R_RS, R_RT, 0);
	EMU_U32* LinkNE2 = JNE32(0);
	//  Equal Do BRANCH
	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(BranchTo);

	EMU_U08* BranchPointer = EmuRec_CheckAddress(BranchTo);
	if (BranchPointer)
	{
		EmuRec_SetRegDependency(BranchTo);
		EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress() - 1;
		if ((OperandSize > 127) || (OperandSize < -123))
		{
			JMP32(OperandSize - 4);
		}
		else
		{
			JMP8(OperandSize - 1);
		}
	}
	else
	{
		EmuRec_FlushModified(FALSE);
		EMU_U32* Link = JMP32(0);
		EmuRec_PushPendingJump(Link, BranchTo);
	}

	//    EMUREC_ALIGN_BRANCH;
	SETLINK32(LinkNE1);
	SETLINK32(LinkNE2);

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bne(EMU_U32 Code)
{
	EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
	EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);
	EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
	EMU_I32 RegRT1 = EmuRec_CheckIntRegCache(R_RT, 1);

	COMPAREINTREGS(RegRS1, RegRT1, R_RS, R_RT, 1);
	EMU_U08* LinkNE = JNE8(0);
	//  Equal
	COMPAREINTREGS(RegRS0, RegRT0, R_RS, R_RT, 0);
	EMU_U32* LinkE = JE32(0);
	//  Not Equal
//    EMUREC_ALIGN_BRANCH;
	SETLINK8(LinkNE);

	// Do BRANCH
	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(BranchTo);

	EMU_U08* BranchPointer = EmuRec_CheckAddress(BranchTo);
	if (BranchPointer)
	{
		EmuRec_SetRegDependency(BranchTo);
		EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress() - 1;
		if ((OperandSize > 127) || (OperandSize < -123))
		{
			JMP32(OperandSize - 4);
		}
		else
		{
			JMP8(OperandSize - 1);
		}
	}
	else
	{
		EmuRec_FlushModified(FALSE);
		EMU_U32* Link = JMP32(0);
		EmuRec_PushPendingJump(Link, BranchTo);
	}

	//    EMUREC_ALIGN_BRANCH;
	SETLINK32(LinkE);

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_blez(EMU_U32 Code)
{
	EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
	EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);

	COMPAREREGZERO(RegRS1, R_RS, 1);
	EMU_U32* LinkG1 = JG32(0);
	EMU_U08* LinkL = JL8(0);
	//  Equal
	COMPAREREGZERO(RegRS0, R_RS, 0);
	EMU_U32* LinkG2 = JG32(0);
	//  Less or Equal
	EMUREC_ALIGN_BRANCH;
	SETLINK8(LinkL);

	// Do BRANCH
	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(BranchTo);

	EMU_U08* BranchPointer = EmuRec_CheckAddress(BranchTo);
	if (BranchPointer)
	{
		EmuRec_SetRegDependency(BranchTo);
		EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress() - 1;
		if ((OperandSize > 127) || (OperandSize < -123))
		{
			JMP32(OperandSize - 4);
		}
		else
		{
			JMP8(OperandSize - 1);
		}
	}
	else
	{
		EmuRec_FlushModified(FALSE);
		EMU_U32* Link = JMP32(0);
		EmuRec_PushPendingJump(Link, BranchTo);
	}

	//    EMUREC_ALIGN_BRANCH;
	SETLINK32(LinkG1);
	SETLINK32(LinkG2);

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bgtz(EMU_U32 Code)
{
	EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
	EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);

	COMPAREREGZERO(RegRS1, R_RS, 1);
	EMU_U32* LinkLE1 = JLE32(0);
	EMU_U08* LinkG = JG8(0);
	//  Equal
	COMPAREREGZERO(RegRS0, R_RS, 0);
	EMU_U32* LinkLE2 = JLE32(0);
	//  Less or Equal
//    EMUREC_ALIGN_BRANCH;
	SETLINK8(LinkG);

	// Do BRANCH
	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(BranchTo);

	EMU_U08* BranchPointer = EmuRec_CheckAddress(BranchTo);
	if (BranchPointer)
	{
		EmuRec_SetRegDependency(BranchTo);
		EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress() - 1;
		if ((OperandSize > 127) || (OperandSize < -123))
		{
			JMP32(OperandSize - 4);
		}
		else
		{
			JMP8(OperandSize - 1);
		}
	}
	else
	{
		EmuRec_FlushModified(FALSE);
		EMU_U32* Link = JMP32(0);
		EmuRec_PushPendingJump(Link, BranchTo);
	}

	//    EMUREC_ALIGN_BRANCH;
	SETLINK32(LinkLE1);
	SETLINK32(LinkLE2);

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_addi(EMU_U32 Code)
{
	// ******************************************
	// Not doing the OVERFLOW TRAP
	// ******************************************
	if (R_RT)
	{
		EMU_I32 RegRT0,
			RegRT1;
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		if (R_RS)
		{
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
			if (R_RT != R_RS)
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
				EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
			}
			else
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			}
			if (Immediate)
			{
				ADD32ItoR(RegRT0, Immediate);
			}
			EMUREC_SIGN_EXTEND(RegRT1, RegRT0);
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
			if (Immediate)
			{
				MOV32ItoR(RegRT0, Immediate);
				if (Immediate & 0x80000000)
				{
					MOV32ItoR(RegRT1, 0xFFFFFFFF);
				}
				else
				{
					XOR32RtoR(RegRT1, RegRT1);
				}
			}
			else
			{
				XOR32RtoR(RegRT0, RegRT0);
				XOR32RtoR(RegRT1, RegRT1);
			}
		}
		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_addiu(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_I32 RegRT0,
			RegRT1;
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		if (R_RS)
		{
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
			if (R_RT != R_RS)
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
				EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
			}
			else
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			}
			if (Immediate)
			{
				ADD32ItoR(RegRT0, Immediate);
			}
			EMUREC_SIGN_EXTEND(RegRT1, RegRT0);
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
			if (Immediate)
			{
				MOV32ItoR(RegRT0, Immediate);
				if (Immediate & 0x80000000)
				{
					MOV32ItoR(RegRT1, 0xFFFFFFFF);
				}
				else
				{
					XOR32RtoR(RegRT1, RegRT1);
				}
			}
			else
			{
				XOR32RtoR(RegRT0, RegRT0);
				XOR32RtoR(RegRT1, RegRT1);
			}
		}
		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_slti(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;
		EMU_I32 RegRT0,
			RegRT1;

		RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
		if (R_RS != R_RT)
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
		}
		XOR32RtoR(RegRT1, RegRT1);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
		EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);

		if (RegRS1 >= 0)
		{
			CMP32ItoR(RegRS1, (Immediate & 0x80000000) ? 0xFFFFFFFF : 0x0);
		}
		else
		{
			CMP32ItoM((EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63, (Immediate & 0x80000000) ? 0xFFFFFFFF : 0x0);
		}
		EMU_U08* LinkA = JG8(0);
		EMU_U08* LinkB = JL8(0);
		//  Equal
		if (RegRS0 >= 0)
		{
			CMP32ItoR(RegRS0, Immediate);
		}
		else
		{
			CMP32ItoM((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31, Immediate);
		}
		// Less than
		EMU_U08* LinkAE = JGE8(0);

		EMUREC_ALIGN_BRANCH;
		SETLINK8(LinkB);
		MOV32ItoR(RegRT0, 0x1);
		EMU_U08* LinkEnd = JMP8(0);

		// Above or Equal
		EMUREC_ALIGN_BRANCH;
		SETLINK8(LinkAE);
		SETLINK8(LinkAE);
		XOR32RtoR(RegRT0, RegRT0);

		EMUREC_ALIGN_BRANCH;
		SETLINK8(LinkEnd);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sltiu(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;
		EMU_I32 RegRT0,
			RegRT1;

		RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
		if (R_RS != R_RT)
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
		}
		XOR32RtoR(RegRT1, RegRT1);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
		EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);

		if (RegRS1 >= 0)
		{
			CMP32ItoR(RegRS1, (Immediate & 0x80000000) ? 0xFFFFFFFF : 0x0);
		}
		else
		{
			CMP32ItoM((EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63, (Immediate & 0x80000000) ? 0xFFFFFFFF : 0x0);
		}
		EMU_U08* LinkA = JA8(0);
		EMU_U08* LinkB = JB8(0);
		//  Equal
		if (RegRS0 >= 0)
		{
			CMP32ItoR(RegRS0, Immediate);
		}
		else
		{
			CMP32ItoM((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31, Immediate);
		}
		// Less than
		EMU_U08* LinkAE = JAE8(0);

		EMUREC_ALIGN_BRANCH;
		SETLINK8(LinkB);
		MOV32ItoR(RegRT0, 0x1);
		EMU_U08* LinkEnd = JMP8(0);

		// Above or Equal
		EMUREC_ALIGN_BRANCH;
		SETLINK8(LinkA);
		SETLINK8(LinkAE);
		XOR32RtoR(RegRT0, RegRT0);

		EMUREC_ALIGN_BRANCH;
		SETLINK8(LinkEnd);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_andi(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = R_IMMEDIATE;
		EMU_I32 RegRT0,
			RegRT1;

		RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
		XOR32RtoR(RegRT1, RegRT1);

		if (R_RS)
		{
			if (R_RT != R_RS)
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
				EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
			}
			else
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			}
			if (Immediate)
			{
				AND32ItoR(RegRT0, Immediate);
			}
			else
			{
				XOR32RtoR(RegRT0, RegRT0);
			}
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			XOR32RtoR(RegRT0, RegRT0);
		}

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ori(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = R_IMMEDIATE;
		EMU_I32 RegRT0,
			RegRT1;

		if (R_RS)
		{
			if (R_RT != R_RS)
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
				EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
				RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
				EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);
				if (RegRS1 >= 0)
				{
					MOV32RtoR(RegRT1, RegRS1);
				}
				else
				{
					MOV32MtoR(RegRT1, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
				}
				EmuRec_SetIntRegModified(RegRT1);
			}
			else
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			}
			if (Immediate)
			{
				OR32ItoR(RegRT0, Immediate);
			}
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
			if (Immediate)
			{
				MOV32ItoR(RegRT0, Immediate);
			}
			else
			{
				XOR32RtoR(RegRT0, RegRT0);
			}
			XOR32RtoR(RegRT1, RegRT1);
			EmuRec_SetIntRegModified(RegRT1);
		}

		EmuRec_SetIntRegModified(RegRT0);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_xori(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = R_IMMEDIATE;
		EMU_I32 RegRT0,
			RegRT1;

		if (R_RS)
		{
			if (R_RT != R_RS)
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
				EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
				RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
				EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);
				if (RegRS1 >= 0)
				{
					MOV32RtoR(RegRT1, RegRS1);
				}
				else
				{
					MOV32MtoR(RegRT1, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
				}
				EmuRec_SetIntRegModified(RegRT1);
			}
			else
			{
				RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			}
			if (Immediate)
			{
				XOR32ItoR(RegRT0, Immediate);
			}
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
			if (Immediate)
			{
				MOV32ItoR(RegRT0, Immediate);
			}
			else
			{
				XOR32RtoR(RegRT0, RegRT0);
			}
			XOR32RtoR(RegRT1, RegRT1);
			EmuRec_SetIntRegModified(RegRT1);
		}

		EmuRec_SetIntRegModified(RegRT0);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lui(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = R_IMMEDIATE << 16;

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
		if (Immediate)
		{
			MOV32ItoR(RegRT0, Immediate);
			if (Immediate & 0x80000000)
			{
				MOV32ItoR(RegRT1, 0xFFFFFFFF);
			}
			else
			{
				XOR32RtoR(RegRT1, RegRT1);
			}
		}
		else
		{
			XOR32RtoR(RegRT0, RegRT0);
			XOR32RtoR(RegRT1, RegRT1);
		}

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_beql(EMU_U32 Code)
{
	EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
	EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);
	EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
	EMU_I32 RegRT1 = EmuRec_CheckIntRegCache(R_RT, 1);

	COMPAREINTREGS(RegRS1, RegRT1, R_RS, R_RT, 1);
	EMU_U32* LinkNE1 = JNE32(0);
	//  Equal
	COMPAREINTREGS(RegRS0, RegRT0, R_RS, R_RT, 0);
	EMU_U32* LinkNE2 = JNE32(0);
	//  Equal Do BRANCH
	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(BranchTo);

	EMU_U08* BranchPointer = EmuRec_CheckAddress(BranchTo);
	if (BranchPointer)
	{
		EmuRec_SetRegDependency(BranchTo);
		EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress() - 1;
		if ((OperandSize > 127) || (OperandSize < -123))
		{
			JMP32(OperandSize - 4);
		}
		else
		{
			JMP8(OperandSize - 1);
		}
	}
	else
	{
		EmuRec_FlushModified(FALSE);
		EMU_U32* Link = JMP32(0);
		EmuRec_PushPendingJump(Link, BranchTo);
	}

	//    EMUREC_ALIGN_BRANCH;
	SETLINK32(LinkNE1);
	SETLINK32(LinkNE2);

	R5900Regs.PC += 4;

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bnel(EMU_U32 Code)
{
	EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
	EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);
	EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
	EMU_I32 RegRT1 = EmuRec_CheckIntRegCache(R_RT, 1);

	COMPAREINTREGS(RegRS1, RegRT1, R_RS, R_RT, 1);
	EMU_U08* LinkNE = JNE8(0);
	//  Equal
	COMPAREINTREGS(RegRS0, RegRT0, R_RS, R_RT, 0);
	EMU_U32* LinkE = JE32(0);
	//  Not Equal
//    EMUREC_ALIGN_BRANCH;
	SETLINK8(LinkNE);

	// Do BRANCH
	EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
	EmuRec_RecompileInstruction(R5900Regs.PC, TRUE);          // Insert Branch Delay Slot code
	R5900Regs.PC = TempPC;                                      // Restore PC

	// Increments the Count register
	ADD8ItoR(EBP, 1);

	InsertBlockUpdate(BranchTo);

	EMU_U08* BranchPointer = EmuRec_CheckAddress(BranchTo);
	if (BranchPointer)
	{
		EmuRec_SetRegDependency(BranchTo);
		EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress() - 1;
		if ((OperandSize > 127) || (OperandSize < -123))
		{
			JMP32(OperandSize - 4);
		}
		else
		{
			JMP8(OperandSize - 1);
		}
	}
	else
	{
		EmuRec_FlushModified(FALSE);
		EMU_U32* Link = JMP32(0);
		EmuRec_PushPendingJump(Link, BranchTo);
	}

	//    EMUREC_ALIGN_BRANCH;
	SETLINK32(LinkE);

	R5900Regs.PC += 4;

	EmuBranchInstruction = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_blezl(EMU_U32 Code)
{
	/*
		if ( R5900Regs.Reg[ R_RS ].i64_00_63 <= 0 )
		{
			EXECUTE_BRANCH( R5900Regs.PC );
		}
		else
		{
			R5900Regs.PC += 4;
		}
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bgtzl(EMU_U32 Code)
{
	/*
		if ( R5900Regs.Reg[ R_RS ].i64_00_63 > 0 )
		{
			EXECUTE_BRANCH( R5900Regs.PC );
		}
		else
		{
			R5900Regs.PC += 4;
		}
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_daddi(EMU_U32 Code)
{
	if (R_RT)
	{
		// ******************************************
		// Not doing the OVERFLOW TRAP
		// ******************************************
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EMU_I32 RegRT0,
			RegRT1;
		if (R_RT == R_RS)
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, TRUE);
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
		}
		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
		EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);

		if (R_RS)
		{
			if (R_RS == R_RT)
			{
				ADD32ItoR(RegRT0, Immediate);
				if (Immediate & 0x80000000)
				{
					ADC32ItoR(RegRT1, 0xFFFFFFFF);
				}
				else
				{
					ADC32ItoR(RegRT1, 0);
				}
			}
			else
			{
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
				if (RegRS1 >= 0)
				{
					MOV32RtoR(RegRT1, RegRS1);
				}
				else
				{
					MOV32MtoR(RegRT1, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
				}
				ADD32ItoR(RegRT0, Immediate);
				if (Immediate & 0x80000000)
				{
					ADC32ItoR(RegRT1, 0xFFFFFFFF);
				}
				else
				{
					ADC32ItoR(RegRT1, 0);
				}
			}
		}
		else
		{
			MOV32ItoR(RegRT0, Immediate);
			if (Immediate & 0x80000000)
			{
				MOV32ItoR(RegRT1, 0xFFFFFFFF);
			}
			else
			{
				XOR32RtoR(RegRT1, RegRT1);
			}
		}
		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_daddiu(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EMU_I32 RegRT0,
			RegRT1;
		if (R_RT == R_RS)
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, TRUE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, TRUE);
		}
		else
		{
			RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
			RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);
		}
		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);
		EMU_I32 RegRS1 = EmuRec_CheckIntRegCache(R_RS, 1);

		if (R_RS)
		{
			if (R_RS == R_RT)
			{
				ADD32ItoR(RegRT0, Immediate);
				if (Immediate & 0x80000000)
				{
					ADC32ItoR(RegRT1, 0xFFFFFFFF);
				}
				else
				{
					ADC32ItoR(RegRT1, 0);
				}
			}
			else
			{
				if (RegRS0 >= 0)
				{
					MOV32RtoR(RegRT0, RegRS0);
				}
				else
				{
					MOV32MtoR(RegRT0, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
				}
				if (RegRS1 >= 0)
				{
					MOV32RtoR(RegRT1, RegRS1);
				}
				else
				{
					MOV32MtoR(RegRT1, (EMU_U32)&R5900Regs.Reg[R_RS].u32_32_63);
				}
				ADD32ItoR(RegRT0, Immediate);
				if (Immediate & 0x80000000)
				{
					ADC32ItoR(RegRT1, 0xFFFFFFFF);
				}
				else
				{
					ADC32ItoR(RegRT1, 0);
				}
			}
		}
		else
		{
			MOV32ItoR(RegRT0, Immediate);
			if (Immediate & 0x80000000)
			{
				MOV32ItoR(RegRT1, 0xFFFFFFFF);
			}
			else
			{
				XOR32RtoR(RegRT1, RegRT1);
			}
		}
		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ldl_inter(EMU_U32 Code)
{
	EMU_U32 Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
	EMU_U32 SA = Address & 0x07;
	EMU_U64 TempU64 = EmuMemGetDWord(Address - 7) & (0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3));
	R5900Regs.Reg[R_RT].u64_00_63 = (R5900Regs.Reg[R_RT].u64_00_63 &
		(0xFFFFFFFFFFFFFFFF >> ((SA + 1) << 3))) |
		TempU64;
}

void EmuRec_ldl(EMU_U32 Code)
{
	EmuRec_FlushModified(TRUE);

	PUSH32M((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	CALLFunc((EMU_U32)EmuRec_ldl_inter, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ldr_inter(EMU_U32 Code)
{
	EMU_U32 Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
	EMU_U32 SA = Address & 0x07;
	EMU_U64 TempU64 = EmuMemGetDWord(Address) & (0xFFFFFFFFFFFFFFFF >> (SA << 3));
	R5900Regs.Reg[R_RT].u64_00_63 = (R5900Regs.Reg[R_RT].u64_00_63 &
		(0xFFFFFFFFFFFFFFFF << ((8 - SA) << 3))) |
		TempU64;
}

void EmuRec_ldr(EMU_U32 Code)
{
	EmuRec_FlushModified(TRUE);

	PUSH32M((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	CALLFunc((EMU_U32)EmuRec_ldr_inter, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lq(EMU_U32 Code)
{
	/*
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		R5900Regs.Reg[ R_RT ].u64_00_63 = EmuMemGetDWord( Address );
		R5900Regs.Reg[ R_RT ].u64_64_127 = EmuMemGetDWord( Address + 8 );
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sq(EMU_U32 Code)
{
	EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

	EmuRec_FlushModified(TRUE);

	if (RegRS0 >= 0)
	{
		MOV32RtoR(EBP, RegRS0);
	}
	else
	{
		MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	}
	if (Immediate)
	{
		ADD32ItoR(EBP, Immediate);
	}

	if (R_RT)
	{
		EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
		EMU_I32 RegRT1 = EmuRec_CheckIntRegCache(R_RT, 1);
		if (RegRT1 >= 0)
		{
			PUSH32R(RegRT1);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
		}
		if (RegRT0 >= 0)
		{
			PUSH32R(RegRT0);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		}
	}
	else
	{
		PUSH32I(0);
		PUSH32I(0);
	}
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetDWord, (EMU_U32)EmuRec_CurrentAddress());

	ADD32ItoR(EBP, 8);
	if (R_RT)
	{
		PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_96_127);
		PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_64_95);
	}
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetDWord, (EMU_U32)EmuRec_CurrentAddress());

	if (R_RT)
	{
		ADD32ItoR(ESP, 24);
	}
	else
	{
		ADD32ItoR(ESP, 16);
	}
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lb(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		/*        PUSH32R( EBP );
		//	    CALLFunc( (EMU_U32)EmuMemGetByte, (EMU_U32)EmuRec_CurrentAddress( ) );
				CALLFunc( (EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress( ) );
				MOV32RmtoR( EAX, EAX );
				ADD32ItoR( ESP, 4 );
				PUSH32R( EAX );
		*/
		InsertLoadMemory();
		MOV32RmtoR(EAX, EAX);
		PUSH32R(EAX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT0);
		AND32ItoR(RegRT0, 0xFF);
		SHL32ItoR(RegRT0, 24);
		SAR32ItoR(RegRT0, 24);

		EMUREC_SIGN_EXTEND(RegRT1, RegRT0);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lh(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		/*        PUSH32R( EBP );
		//	    CALLFunc( (EMU_U32)EmuMemGetShort, (EMU_U32)EmuRec_CurrentAddress( ) );
				CALLFunc( (EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress( ) );
				MOV32RmtoR( EAX, EAX );
				ADD32ItoR( ESP, 4 );
				PUSH32R( EAX );
		*/
		InsertLoadMemory();
		MOV32RmtoR(EAX, EAX);
		PUSH32R(EAX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT0);
		AND32ItoR(RegRT0, 0xFFFF);
		SHL32ItoR(RegRT0, 16);
		SAR32ItoR(RegRT0, 16);

		EMUREC_SIGN_EXTEND(RegRT1, RegRT0);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwl(EMU_U32 Code)
{
	/*
		RT = R_RT;
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = Address & 0x03;
		TempU32 = EmuMemGetWord( Address - 3 ) & ( 0xFFFFFFFF << ( ( 3 - SA ) << 3 ) );
		R5900Regs.Reg[ RT ].u32_00_31 &= 0xFFFFFFFF >> ( ( SA + 1 ) << 3 );
		R5900Regs.Reg[ RT ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ RT ].u32_00_31 | TempU32 );
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lw(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		//        PUSH32R( EBP );
		//	    CALLFunc( (EMU_U32)EmuMemGetWord, (EMU_U32)EmuRec_CurrentAddress( ) );

		InsertLoadMemory();
		//        CALLFunc( (EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress( ) );
		MOV32RmtoR(EAX, EAX);
		//        ADD32ItoR( ESP, 4 );
		PUSH32R(EAX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT0);

		EMUREC_SIGN_EXTEND(RegRT1, RegRT0);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lbu(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		/*        PUSH32R( EBP );
		//	    CALLFunc( (EMU_U32)EmuMemGetByte, (EMU_U32)EmuRec_CurrentAddress( ) );
				CALLFunc( (EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress( ) );
				MOV32RmtoR( EAX, EAX );
				ADD32ItoR( ESP, 4 );
				PUSH32R( EAX );
		*/
		InsertLoadMemory();
		MOV32RmtoR(EAX, EAX);
		PUSH32R(EAX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT0);
		AND32ItoR(RegRT0, 0xFF);

		EMUREC_SIGN_EXTEND(RegRT1, RegRT0);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lhu(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		/*        PUSH32R( EBP );
		//	    CALLFunc( (EMU_U32)EmuMemGetShort, (EMU_U32)EmuRec_CurrentAddress( ) );
				CALLFunc( (EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress( ) );
				MOV32RmtoR( EAX, EAX );
				ADD32ItoR( ESP, 4 );
				PUSH32R( EAX );
		*/
		InsertLoadMemory();
		MOV32RmtoR(EAX, EAX);
		PUSH32R(EAX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT0);
		AND32ItoR(RegRT0, 0xFFFF);

		EMUREC_SIGN_EXTEND(RegRT1, RegRT0);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwr(EMU_U32 Code)
{
	/*
		RT = R_RT;
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		TempU32 = EmuMemGetWord( Address );
		SA = Address & 0x03;
		if ( SA )
		{
			TempU32 &= 0xFFFFFFFF >> ( SA << 3 );
			R5900Regs.Reg[ RT ].u32_00_31 &= 0xFFFFFFFFFFFFFFFF << ( ( 4 - SA ) << 3 );
			R5900Regs.Reg[ RT ].u32_00_31 |= TempU32;
		}
		else
		{
			R5900Regs.Reg[ RT ].u64_00_63 = (EMU_I32)TempU32;
		}
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwu(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		//        PUSH32R( EBP );
		//	    CALLFunc( (EMU_U32)EmuMemGetWord, (EMU_U32)EmuRec_CurrentAddress( ) );

		InsertLoadMemory();
		//        CALLFunc( (EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress( ) );
		MOV32RmtoR(EAX, EAX);
		//        ADD32ItoR( ESP, 4 );
		PUSH32R(EAX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT0);

		XOR32RtoR(RegRT1, RegRT1);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sb(EMU_U32 Code)
{
	EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

	EmuRec_FlushModified(TRUE);

	if (RegRS0 >= 0)
	{
		MOV32RtoR(EBP, RegRS0);
	}
	else
	{
		MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	}
	if (Immediate)
	{
		ADD32ItoR(EBP, Immediate);
	}

	if (R_RT)
	{
		EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
		if (RegRT0 >= 0)
		{
			PUSH32R(RegRT0);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		}
	}
	else
	{
		PUSH32I(0);
	}
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetByte, (EMU_U32)EmuRec_CurrentAddress());

	ADD32ItoR(ESP, 8);

	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sh(EMU_U32 Code)
{
	EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

	EmuRec_FlushModified(TRUE);
	if (RegRS0 >= 0)
	{
		MOV32RtoR(EBP, RegRS0);
	}
	else
	{
		MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	}
	if (Immediate)
	{
		ADD32ItoR(EBP, Immediate);
	}

	if (R_RT)
	{
		EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
		if (RegRT0 >= 0)
		{
			PUSH32R(RegRT0);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		}
	}
	else
	{
		PUSH32I(0);
	}
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetShort, (EMU_U32)EmuRec_CurrentAddress());

	ADD32ItoR(ESP, 8);

	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_swl(EMU_U32 Code)
{
	/*
		RT = R_RT;
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = Address & 0x3;
		TempU32 = ( EmuMemGetWord( Address - 3 ) &
				  ( 0xFFFFFFFF >> ( ( SA + 1 ) << 3 ) ) ) |
				  ( R5900Regs.Reg[ RT ].u32_00_31 & ( 0xFFFFFFFF << ( ( 3 - SA ) << 3 ) ) );
		EmuMemSetWord( Address - 3, TempU32 );
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sw(EMU_U32 Code)
{
	EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

	EmuRec_FlushModified(TRUE);
	if (RegRS0 >= 0)
	{
		MOV32RtoR(EBP, RegRS0);
	}
	else
	{
		MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	}
	if (Immediate)
	{
		ADD32ItoR(EBP, Immediate);
	}

	if (R_RT)
	{
		EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
		if (RegRT0 >= 0)
		{
			PUSH32R(RegRT0);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		}
	}
	else
	{
		PUSH32I(0);
	}
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetWord, (EMU_U32)EmuRec_CurrentAddress());

	ADD32ItoR(ESP, 8);
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sdl_inter(EMU_U32 Code)
{
	EMU_U32 Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
	EMU_U32 SA = Address & 0x07;
	EMU_U64 TempU64 = (EmuMemGetDWord(Address - 7) & (0xFFFFFFFFFFFFFFFF >> ((SA + 1) << 3))) |
		(R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF << ((7 - SA) << 3)));
	EmuMemSetDWord(Address - 7, TempU64);
}

void EmuRec_sdl(EMU_U32 Code)
{
	EmuRec_FlushModified(TRUE);

	PUSH32M((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	CALLFunc((EMU_U32)EmuRec_sdl_inter, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sdr_inter(EMU_U32 Code)
{
	EMU_U32 Address = R5900Regs.Reg[R_RS].u32_00_31 + (EMU_I16)R_IMMEDIATE;
	EMU_U32 SA = Address & 0x07;
	EMU_U64 TempU64 = (EmuMemGetDWord(Address) &
		(0xFFFFFFFFFFFFFFFF << ((8 - SA) << 3))) |
		(R5900Regs.Reg[R_RT].u64_00_63 & (0xFFFFFFFFFFFFFFFF >> (SA << 3)));
	EmuMemSetDWord(Address, TempU64);
}

void EmuRec_sdr(EMU_U32 Code)
{
	EmuRec_FlushModified(TRUE);

	PUSH32M((EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	CALLFunc((EMU_U32)EmuRec_sdr_inter, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_swr(EMU_U32 Code)
{
	/*
		RT = R_RT;
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		SA = Address & 0x3;
		TempU32 = ( EmuMemGetWord( Address ) & ( 0xFFFFFFFF << ( ( 4 - SA ) << 3 ) ) ) |
					( R5900Regs.Reg[ RT ].u32_00_31 & ( 0xFFFFFFFF >> ( SA << 3 ) ) );
		EmuMemSetWord( Address, TempU32 );
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_cache(EMU_U32 Code)
{
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwc1(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		PUSH32R(EBP);
		//	    CALLFunc( (EMU_U32)EmuMemGetWord, (EMU_U32)EmuRec_CurrentAddress( ) );
		CALLFunc((EMU_U32)EmuMemGetRealPointer, (EMU_U32)EmuRec_CurrentAddress());
		MOV32RmtoR(EAX, EAX);
		ADD32ItoR(ESP, 4);
		MOV32RtoM((EMU_U32)&COP1Regs.Reg[R_RT].u, EAX);

		EmuRec_FetchAll();
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_pref(EMU_U32 Code)
{
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lqc2(EMU_U32 Code)
{
	/*
		RT = R_RT;
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		VU0Regs.CPR[ RT ].X.u = EmuMemGetWord( Address );
		VU0Regs.CPR[ RT ].Y.u = EmuMemGetWord( Address + 4 );
		VU0Regs.CPR[ RT ].Z.u = EmuMemGetWord( Address + 8 );
		VU0Regs.CPR[ RT ].W.u = EmuMemGetWord( Address + 12 );
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ld(EMU_U32 Code)
{
	if (R_RT)
	{
		EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

		EmuRec_FlushModified(TRUE);

		EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

		MOV32RtoM((EMU_U32)&COP0Regs.Count, EBP);
		if (RegRS0 >= 0)
		{
			MOV32RtoR(EBP, RegRS0);
		}
		else
		{
			MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
		}
		if (Immediate)
		{
			ADD32ItoR(EBP, Immediate);
		}

		PUSH32R(EBP);
		CALLFunc((EMU_U32)EmuMemGetDWord, (EMU_U32)EmuRec_CurrentAddress());
		ADD32ItoR(ESP, 4);
		PUSH32R(EAX);
		PUSH32R(EDX);

		EmuRec_FetchAll();

		EMU_I32 RegRT0 = EmuRec_GetIntReg(R_RT, 0, FALSE);
		EMU_I32 RegRT1 = EmuRec_GetIntReg(R_RT, 1, FALSE);

		POP32R(RegRT1);
		POP32R(RegRT0);

		EmuRec_SetIntRegModified(RegRT0);
		EmuRec_SetIntRegModified(RegRT1);
	}
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_swc1(EMU_U32 Code)
{
	EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

	EmuRec_FlushModified(TRUE);
	if (RegRS0 >= 0)
	{
		MOV32RtoR(EBP, RegRS0);
	}
	else
	{
		MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	}
	if (Immediate)
	{
		ADD32ItoR(EBP, Immediate);
	}

	PUSH32M((EMU_U32)&COP1Regs.Reg[R_FT].u);
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetWord, (EMU_U32)EmuRec_CurrentAddress());

	ADD32ItoR(ESP, 8);
	EmuRec_FetchAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sqc2(EMU_U32 Code)
{
	/*
		Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
		RT = R_RT;
		EmuMemSetWord( Address,       VU0Regs.CPR[ RT ].X.u );
		EmuMemSetWord( Address + 4,   VU0Regs.CPR[ RT ].Y.u );
		EmuMemSetWord( Address + 8,   VU0Regs.CPR[ RT ].Z.u );
		EmuMemSetWord( Address + 12,  VU0Regs.CPR[ RT ].W.u );
	*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sd(EMU_U32 Code)
{
	EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

	EMU_I32 RegRS0 = EmuRec_CheckIntRegCache(R_RS, 0);

	EmuRec_FlushModified(TRUE);
	if (RegRS0 >= 0)
	{
		MOV32RtoR(EBP, RegRS0);
	}
	else
	{
		MOV32MtoR(EBP, (EMU_U32)&R5900Regs.Reg[R_RS].u32_00_31);
	}
	if (Immediate)
	{
		ADD32ItoR(EBP, Immediate);
	}

	if (R_RT)
	{
		EMU_I32 RegRT0 = EmuRec_CheckIntRegCache(R_RT, 0);
		EMU_I32 RegRT1 = EmuRec_CheckIntRegCache(R_RT, 1);
		if (RegRT1 >= 0)
		{
			PUSH32R(RegRT1);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_32_63);
		}
		if (RegRT0 >= 0)
		{
			PUSH32R(RegRT0);
		}
		else
		{
			PUSH32M((EMU_U32)&R5900Regs.Reg[R_RT].u32_00_31);
		}
	}
	else
	{
		PUSH32I(0);
		PUSH32I(0);
	}
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemSetDWord, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 12);

	EmuRec_FetchAll();
}

void InsertLoadMemory(void)
{
	CMP32ItoR(EBP, 0x80000000);
	EMU_U08* B1 = JB8(0);
	CMP32ItoR(EBP, 0xA0000000);
	EMU_U08* AE1 = JAE8(0);

	SUB32ItoR(EBP, 0x80000000);

	EMU_U08* J1 = JMP8(0);

	SETLINK8(B1);
	SETLINK8(AE1);

	CMP32ItoR(EBP, 0xA0000000);
	EMU_U08* B2 = JB8(0);
	CMP32ItoR(EBP, 0xC0000000);
	EMU_U08* AE2 = JAE8(0);

	ADD32ItoR(EBP, 0x60000000);

	SETLINK8(J1);
	SETLINK8(B2);
	SETLINK8(AE2);

	CMP32ItoR(EBP, EMU_TIMER_START_ADDR);
	EMU_U32* B3 = JB32(0);
	CMP32ItoR(EBP, EMU_GS_PRIV_END_ADDR);
	EMU_U32* A1 = JA32(0);

	CMP32ItoR(EBP, EMU_IPU_START_ADDR);

	PUSH32R(EBP);
	EMU_U08* AE3 = JAE8(0);
	CALLFunc((EMU_U32)Emu_Timer_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J2 = JMP32(0);

	SETLINK8(AE3);

	CMP32ItoR(EBP, EMU_GIF_START_ADDR);
	EMU_U08* AE4 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Ipu_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J3 = JMP32(0);

	SETLINK8(AE4);

	CMP32ItoR(EBP, EMU_VIF_START_ADDR);
	EMU_U08* AE5 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Gif_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J4 = JMP32(0);

	SETLINK8(AE5);

	CMP32ItoR(EBP, EMU_FIFO_START_ADDR);
	EMU_U08* AE6 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Vif_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J5 = JMP32(0);

	SETLINK8(AE6);

	CMP32ItoR(EBP, EMU_DMA_START_ADDR);
	EMU_U08* AE7 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Fifo_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J6 = JMP32(0);

	SETLINK8(AE7);

	CMP32ItoR(EBP, EMU_INTC_START_ADDR);
	EMU_U08* AE8 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Dma_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J7 = JMP32(0);

	SETLINK8(AE8);

	CMP32ItoR(EBP, EMU_SIF_START_ADDR);
	EMU_U08* AE9 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Intc_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J8 = JMP32(0);

	SETLINK8(AE9);

	CMP32ItoR(EBP, EMU_DMA_ENAB_START_ADDR);
	EMU_U08* AE10 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Sif_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J9 = JMP32(0);

	SETLINK8(AE10);

	CMP32ItoR(EBP, EMU_VU_START_ADDR);
	EMU_U08* AE11 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Dma_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J10 = JMP32(0);

	SETLINK8(AE11);

	CMP32ItoR(EBP, EMU_GS_PRIV_START_ADDR);
	EMU_U08* AE12 = JAE8(0);

	CALLFunc((EMU_U32)Emu_Vu_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J11 = JMP32(0);

	SETLINK8(AE12);

	CALLFunc((EMU_U32)Emu_GS_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J12 = JMP32(0);

	SETLINK32(A1);
	SETLINK32(B3);

	CMP32ItoR(EBP, EMU_BIOS_START_ADDR);
	EMU_U08* B4 = JB8(0);
	CMP32ItoR(EBP, EMU_BIOS_END_ADDR);
	EMU_U08* A2 = JA8(0);

	PUSH32R(EBP);
	CALLFunc((EMU_U32)Emu_Bios_GetPointer, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);
	EMU_U32* J13 = JMP32(0);

	SETLINK8(A2);
	SETLINK8(B4);

	MOV32ItoR(ECX, (EMU_U32)Mapping);
	MOV32RtoR(EDI, EBP);
	SHR32ItoR(EDI, 16);
	SHL32ItoR(EDI, 4);

	ADD32RtoR(EDI, ECX);
	MOV32RmtoR(EAX, EDI);
	TEST32ItoR(EAX, 0x000000FF);
	EMU_U08* NE1 = JNE8(0);
	PUSH32R(EBP);
	CALLFunc((EMU_U32)EmuMemReplace, (EMU_U32)EmuRec_CurrentAddress());
	ADD32ItoR(ESP, 4);

	SETLINK8(NE1);
	ADD32ItoR(EDI, 4);

	MOV32MtoR(EAX, (EMU_U32)&MemoryMask);
	MOV32RmtoR(EDX, EDI);

	MOV32ItoR(ECX, (EMU_U32)MainMemory.Byte);
	AND32RtoR(EAX, EBP);
	OR32RtoR(EAX, EDX);
	ADD32RtoR(EAX, ECX);

	SETLINK32(J2);
	SETLINK32(J3);
	SETLINK32(J4);
	SETLINK32(J5);
	SETLINK32(J6);
	SETLINK32(J7);
	SETLINK32(J8);
	SETLINK32(J9);
	SETLINK32(J10);
	SETLINK32(J11);
	SETLINK32(J12);
	SETLINK32(J13);
}
