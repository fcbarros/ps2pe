/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuRecompiler.h
// Version: 1.000
// Desc: Main Header File for the Recompiler
//
// Author: GoldFinger
// Created: 09/07/2002
// Modifications:
// * Version 1.000 ( 09/07/2002 ): Created
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <vector>

#include "EmuPs2Core.h"

extern BOOL EmuStopCompile;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#define EMUFUNCTION     EmuRunDebug
//#define EMUFUNCTION     EmuExecuteFast

#define R_RD            ( ( Code >> 11 ) & 0x1F )
#define R_RT            ( ( Code >> 16 ) & 0x1F )
#define R_RS            ( ( Code >> 21 ) & 0x1F )
#define R_SA            ( ( Code >> 06 ) & 0x1F )

#define R_FD            ( ( Code >> 06 ) & 0x1F )
#define R_FS            ( ( Code >> 11 ) & 0x1F )
#define R_FT            ( ( Code >> 16 ) & 0x1F )

#define R_BRANCH        (   Code & 0xFFFF )
#define R_IMMEDIATE     (   Code & 0xFFFF )

#define VU_DEST_X       ( ( Code >> 24 ) & 0x01 )
#define VU_DEST_Y       ( ( Code >> 23 ) & 0x01 )
#define VU_DEST_Z       ( ( Code >> 22 ) & 0x01 )
#define VU_DEST_W       ( ( Code >> 21 ) & 0x01 )
#define VU_FT           ( ( Code >> 16 ) & 0x1F )
#define VU_FS           ( ( Code >> 11 ) & 0x1F )
#define VU_FD           ( ( Code >> 06 ) & 0x1F )
#define VU_BC           (   Code & 0x03 )
#define VU_FTF          ( ( Code >> 23 ) & 0x03 )
#define VU_FSF          ( ( Code >> 21 ) & 0x03 )
#define VU_IT           ( ( Code >> 16 ) & 0x1F )
#define VU_IS           ( ( Code >> 11 ) & 0x1F )
#define VU_IMM5         ( ( Code >> 06 ) & 0x1F )
#define VU_IMM15        ( ( Code >> 06 ) & 0x7FFF )
#define VU_RT           ( ( Code >> 16 ) & 0x1F )
#define VU_ID           ( ( Code >> 11 ) & 0x1F )
#define VU_BASE         ( ( Code >> 21 ) & 0x1F )
#define VU_OFFSET       (   Code & 0xFFFF )

#define JUMP_ADDRESS    ( ( Code & 0x03FFFFFF ) << 2 ) | ( PS2Regs.R5900Regs.PC & 0xF0000000 )


#define EMU_FLOAT_LOAD_CONDITION_REGISTER( a )      __asm FSTSW a
#define EMU_FLOAT_CHECK_OVERFLOW( a )               (( a ) & 0x0008)
#define EMU_FLOAT_CHECK_UNDERFLOW( a )              (( a ) & 0x0010)

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

#define SetFloatFlags2( var )                        \
        EMU_FLOAT_LOAD_CONDITION_REGISTER( var );   \
        if ( EMU_FLOAT_CHECK_OVERFLOW( var ) )      \
        {                                           \
            PS2Regs.COP1Regs.FCR31_O = 1;                   \
            PS2Regs.COP1Regs.FCR31_SO = 1;                  \
            PS2Regs.COP1Regs.FCR31_U = 0;                   \
        }                                           \
        else                                        \
        if ( EMU_FLOAT_CHECK_UNDERFLOW( var ) )     \
        {                                           \
            PS2Regs.COP1Regs.FCR31_O = 0;                   \
            PS2Regs.COP1Regs.FCR31_U = 1;                   \
            PS2Regs.COP1Regs.FCR31_SU = 1;                  \
        }                                           \
        else                                        \
        {                                           \
            PS2Regs.COP1Regs.FCR31_O = 0;                   \
            PS2Regs.COP1Regs.FCR31_U = 0;                   \
        }


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Structs
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


typedef struct
{
	EMU_U32 Size;
	EMU_U32 Position;
	EMU_U08* Memory;
} stEmuRecMemory;

typedef struct
{
	EMU_U64 LastAccessed;       // Last time accessed
	EMU_U32 Used;               // Is Register Used?
	EMU_U32 Modified;           // Is Register Modified?
	EMU_U32 Code;               // Internal Register code
	EMU_I32 MainIndex;          // Main Index of emulator register, -1 = not used
	EMU_I32 SecIndex;           // Secundary Index of emulator register, -1 = not used
} stEmuRecReg;

typedef struct
{
	EMU_I32 Register[4];      // Index of the 32bits Machine register, -1 = not in cache
} stEmuRecEmuReg;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// External Variables
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

extern EMU_U32 EmuFloatStatus;
extern EMU_U32 EmuIntegerStatus;

extern EMU_U32 RS;
extern EMU_U32 RD;
extern EMU_U32 RT;
extern EMU_U32 FS;
extern EMU_U32 FT;
extern EMU_U32 SA;
extern EMU_U32 Address;
extern EMU_I32 TempI32;
extern EMU_U32 TempU32;
extern EMU_I64 TempI64;
extern EMU_U64 TempU64;
extern EMU_128 Temp128;
extern EMU_U32 RegRTU32;
extern EMU_U64 RegRTU64;
extern EMU_F32 TempF32;
extern EMU_U32 JumpTo;

extern EMU_U64 CpuCycles;


typedef struct
{
	EMU_U32 EmuTargetAddress;
	EMU_U32* LinkAddress;
} stEmuRecJumpRecord;

typedef struct
{
	EMU_U32 Main;
	EMU_U32 Secondary;
	EMU_I32 CPUIndex;
} stEmuRecDepend;

struct stEmuRecInstruction
{
	stEmuRecDepend RegDependency[6];
	union
	{
		EMU_U32 Attributes;
		struct
		{
			EMU_U32 UseFPU : 1;
			EMU_U32 UseMMX : 1;
			EMU_U32 UseSSE : 1;
			EMU_U32 Use3DNOW : 1;
		};
	};
	EMU_U08* CPUAddress;

	stEmuRecInstruction()
	{
		CPUAddress = NULL;
		Attributes = 0;
		for (EMU_U32 i = 0; i < 6; i++)
		{
			RegDependency[i].Main = 0;
			RegDependency[i].Secondary = 0;
			RegDependency[i].CPUIndex = -1;
		}
	}
};

typedef std::map<EMU_U32, stEmuRecInstruction>   stEmuRecAddressTranslationTable;
typedef std::vector<stEmuRecJumpRecord>         stEmuRecPendingJump;

EMU_U08* EmuRec_CurrentAddress();

void EmuRec_PushPendingJump(EMU_U32* LinkAddress, EMU_U32 JumpTo);
void EmuRec_Init(EMU_U32 Size);
void EmuRec_Shutdown();
void EmuRec_Reset();
void EmuRec_Recompile(EMU_U32 StartAddress, EMU_U32 EndAddress);
void EmuRec_RecompileExecute(EMU_U32 EntryPoint, EMU_U32 EndAddress, BOOL Loop);
void EmuRec_RecompileInstruction(EMU_U32 Address, BOOL InBrachDelay);
BOOL EmuRec_CheckRegUse(EMU_U32 Code);
void EmuRec_FlushCPUReg(EMU_U32 Code);
void EmuRec_FreeCPUReg(EMU_U32 Code);
EMU_I32 EmuRec_CheckIntRegCache(EMU_U32 R5900Register, EMU_U32 U32Index);
EMU_U08* EmuRec_CheckAddress(EMU_U32 Address);
void EmuRec_SetIntRegTime(EMU_U32 Register);
void EmuRec_FreeAll();
void EmuRec_FlushAll();
void EmuRec_InsertAddress(EMU_U32 Address, EMU_U08* pMemory);
void EmuRec_InsertRegDependency(EMU_U32 Address, EMU_U32 Index);
void EmuRec_ResolveRegDependency(EMU_U32 Address);
stEmuRecJumpRecord* EmuRec_CheckPendingJumps();
void EmuRec_InsertDependency(EMU_U32 Address);
void EmuRec_PopPendingJump();
void EmuRec_PushPendingJump(EMU_U32* LinkAddress, EMU_U32 JumpTo);

EMU_U32 EmuRec_GetIntReg(EMU_U32 R5900Register, EMU_U32 U32Index, BOOL ExecuteMove);
void EmuRec_SetIntRegModified(EMU_U32 Register);
void EmuRec_PushAll();
void EmuRec_PopAll();


typedef void (*EMURECFUNCTION)(EMU_U32);

extern EMURECFUNCTION EmuRecCoreFunction[64];
extern EMURECFUNCTION EmuRecSpecialFunction[64];
extern EMURECFUNCTION EmuRecRegimmFunction[32];
extern EMURECFUNCTION EmuRecMMIFunction[64];
extern EMURECFUNCTION EmuRecMMI_0Function[32];
extern EMURECFUNCTION EmuRecMMI_1Function[32];
extern EMURECFUNCTION EmuRecMMI_2Function[32];
extern EMURECFUNCTION EmuRecMMI_3Function[32];
extern EMURECFUNCTION EmuRecCOP0Function[32];
extern EMURECFUNCTION EmuRecCOP0_BC0Function[32];
extern EMURECFUNCTION EmuRecCOP0_C0Function[64];
extern EMURECFUNCTION EmuRecCOP1Function[32];
extern EMURECFUNCTION EmuRecCOP1_BC1Function[32];
extern EMURECFUNCTION EmuRecCOP1_SFunction[64];
extern EMURECFUNCTION EmuRecCOP1_WFunction[64];


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions Prototypes
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


#define EMUREC_SIGN_EXTEND( dest, src )    \
                MOV32RtoR( dest, src );    \
                SAR32ItoR( dest, 31 );

#define EMUREC_ALIGN_BRANCH                                         \
                while ( (EMU_U32)EmuRec_CurrentAddress() & 0x03 )   \
                {                                                   \
                    EmuRec_Write8( 0x90 );                          \
                }

// EmuRecCore.cpp
void EmuRec_j(EMU_U32 Code);
void EmuRec_jal(EMU_U32 Code);
void EmuRec_beq(EMU_U32 Code);
void EmuRec_bne(EMU_U32 Code);
void EmuRec_blez(EMU_U32 Code);
void EmuRec_bgtz(EMU_U32 Code);
void EmuRec_addi(EMU_U32 Code);
void EmuRec_addiu(EMU_U32 Code);
void EmuRec_slti(EMU_U32 Code);
void EmuRec_sltiu(EMU_U32 Code);
void EmuRec_andi(EMU_U32 Code);
void EmuRec_ori(EMU_U32 Code);
void EmuRec_xori(EMU_U32 Code);
void EmuRec_lui(EMU_U32 Code);
void EmuRec_beql(EMU_U32 Code);
void EmuRec_bnel(EMU_U32 Code);
void EmuRec_blezl(EMU_U32 Code);
void EmuRec_bgtzl(EMU_U32 Code);
void EmuRec_daddi(EMU_U32 Code);
void EmuRec_daddiu(EMU_U32 Code);
void EmuRec_ldl(EMU_U32 Code);
void EmuRec_ldr(EMU_U32 Code);
void EmuRec_lq(EMU_U32 Code);
void EmuRec_sq(EMU_U32 Code);
void EmuRec_lb(EMU_U32 Code);
void EmuRec_lh(EMU_U32 Code);
void EmuRec_lwl(EMU_U32 Code);
void EmuRec_lw(EMU_U32 Code);
void EmuRec_lbu(EMU_U32 Code);
void EmuRec_lhu(EMU_U32 Code);
void EmuRec_lwr(EMU_U32 Code);
void EmuRec_lwu(EMU_U32 Code);
void EmuRec_sb(EMU_U32 Code);
void EmuRec_sh(EMU_U32 Code);
void EmuRec_swl(EMU_U32 Code);
void EmuRec_sw(EMU_U32 Code);
void EmuRec_sdl(EMU_U32 Code);
void EmuRec_sdr(EMU_U32 Code);
void EmuRec_swr(EMU_U32 Code);
void EmuRec_cache(EMU_U32 Code);
void EmuRec_lwc1(EMU_U32 Code);
void EmuRec_pref(EMU_U32 Code);
void EmuRec_lqc2(EMU_U32 Code);
void EmuRec_ld(EMU_U32 Code);
void EmuRec_swc1(EMU_U32 Code);
void EmuRec_sqc2(EMU_U32 Code);
void EmuRec_sd(EMU_U32 Code);

// EmuRecSpecial.cpp
void EmuRec_sll(EMU_U32 Code);
void EmuRec_srl(EMU_U32 Code);
void EmuRec_sra(EMU_U32 Code);
void EmuRec_sllv(EMU_U32 Code);
void EmuRec_srlv(EMU_U32 Code);
void EmuRec_srav(EMU_U32 Code);
void EmuRec_jr(EMU_U32 Code);
void EmuRec_jalr(EMU_U32 Code);
void EmuRec_movz(EMU_U32 Code);
void EmuRec_movn(EMU_U32 Code);
void EmuRec_syscall(EMU_U32 Code);
void EmuRec_break(EMU_U32 Code);
void EmuRec_sync(EMU_U32 Code);
void EmuRec_mfhi(EMU_U32 Code);
void EmuRec_mthi(EMU_U32 Code);
void EmuRec_mflo(EMU_U32 Code);
void EmuRec_mtlo(EMU_U32 Code);
void EmuRec_dsllv(EMU_U32 Code);
void EmuRec_dsrlv(EMU_U32 Code);
void EmuRec_dsrav(EMU_U32 Code);
void EmuRec_mult(EMU_U32 Code);
void EmuRec_multu(EMU_U32 Code);
void EmuRec_div(EMU_U32 Code);
void EmuRec_divu(EMU_U32 Code);
void EmuRec_add(EMU_U32 Code);
void EmuRec_addu(EMU_U32 Code);
void EmuRec_sub(EMU_U32 Code);
void EmuRec_subu(EMU_U32 Code);
void EmuRec_and(EMU_U32 Code);
void EmuRec_or(EMU_U32 Code);
void EmuRec_xor(EMU_U32 Code);
void EmuRec_nor(EMU_U32 Code);
void EmuRec_mfsa(EMU_U32 Code);
void EmuRec_mtsa(EMU_U32 Code);
void EmuRec_slt(EMU_U32 Code);
void EmuRec_sltu(EMU_U32 Code);
void EmuRec_dadd(EMU_U32 Code);
void EmuRec_daddu(EMU_U32 Code);
void EmuRec_dsub(EMU_U32 Code);
void EmuRec_dsubu(EMU_U32 Code);
void EmuRec_tge(EMU_U32 Code);
void EmuRec_tgeu(EMU_U32 Code);
void EmuRec_tlt(EMU_U32 Code);
void EmuRec_tltu(EMU_U32 Code);
void EmuRec_teq(EMU_U32 Code);
void EmuRec_tne(EMU_U32 Code);
void EmuRec_dsll(EMU_U32 Code);
void EmuRec_dsrl(EMU_U32 Code);
void EmuRec_dsra(EMU_U32 Code);
void EmuRec_dsll32(EMU_U32 Code);
void EmuRec_dsrl32(EMU_U32 Code);
void EmuRec_dsra32(EMU_U32 Code);

// EmuRecRegimm.cpp
void EmuRec_bltz(EMU_U32 Code);
void EmuRec_bgez(EMU_U32 Code);
void EmuRec_bltzl(EMU_U32 Code);
void EmuRec_bgezl(EMU_U32 Code);
void EmuRec_tgei(EMU_U32 Code);
void EmuRec_tgeiu(EMU_U32 Code);
void EmuRec_tlti(EMU_U32 Code);
void EmuRec_tltiu(EMU_U32 Code);
void EmuRec_teqi(EMU_U32 Code);
void EmuRec_tnei(EMU_U32 Code);
void EmuRec_bltzal(EMU_U32 Code);
void EmuRec_bgezal(EMU_U32 Code);
void EmuRec_bltzall(EMU_U32 Code);
void EmuRec_bgezall(EMU_U32 Code);
void EmuRec_mtsab(EMU_U32 Code);
void EmuRec_mtsah(EMU_U32 Code);

// EmuRecCOP0.cpp
void EmuRec_mfc0(EMU_U32 Code);
void EmuRec_mtc0(EMU_U32 Code);
void EmuRec_bc0f(EMU_U32 Code);
void EmuRec_bc0t(EMU_U32 Code);
void EmuRec_bc0fl(EMU_U32 Code);
void EmuRec_bc0tl(EMU_U32 Code);
void EmuRec_tlbr(EMU_U32 Code);
void EmuRec_tlbwi(EMU_U32 Code);
void EmuRec_tlbwr(EMU_U32 Code);
void EmuRec_tlbp(EMU_U32 Code);
void EmuRec_eret(EMU_U32 Code);
void EmuRec_ei(EMU_U32 Code);
void EmuRec_di(EMU_U32 Code);

// EmuRecCOP1.cpp
void EmuRec_mfc1(EMU_U32 Code);
void EmuRec_cfc1(EMU_U32 Code);
void EmuRec_mtc1(EMU_U32 Code);
void EmuRec_ctc1(EMU_U32 Code);
void EmuRec_bc1f(EMU_U32 Code);
void EmuRec_bc1t(EMU_U32 Code);
void EmuRec_bc1fl(EMU_U32 Code);
void EmuRec_bc1tl(EMU_U32 Code);
void EmuRec_add_s(EMU_U32 Code);
void EmuRec_sub_s(EMU_U32 Code);
void EmuRec_mul_s(EMU_U32 Code);
void EmuRec_div_s(EMU_U32 Code);
void EmuRec_sqrt_s(EMU_U32 Code);
void EmuRec_abs_s(EMU_U32 Code);
void EmuRec_mov_s(EMU_U32 Code);
void EmuRec_neg_s(EMU_U32 Code);
void EmuRec_rsqrt_s(EMU_U32 Code);
void EmuRec_adda_s(EMU_U32 Code);
void EmuRec_suba_s(EMU_U32 Code);
void EmuRec_mula_s(EMU_U32 Code);
void EmuRec_madd_s(EMU_U32 Code);
void EmuRec_msub_s(EMU_U32 Code);
void EmuRec_madda_s(EMU_U32 Code);
void EmuRec_msuba_s(EMU_U32 Code);
void EmuRec_cvt_w_s(EMU_U32 Code);
void EmuRec_max_s(EMU_U32 Code);
void EmuRec_min_s(EMU_U32 Code);
void EmuRec_c_f_s(EMU_U32 Code);
void EmuRec_c_eq_s(EMU_U32 Code);
void EmuRec_c_lt_s(EMU_U32 Code);
void EmuRec_c_le_s(EMU_U32 Code);
void EmuRec_cvt_s_w(EMU_U32 Code);

// EmuRecMMI.cpp
void EmuRec_madd(EMU_U32 Code);
void EmuRec_maddu(EMU_U32 Code);
void EmuRec_plzcw(EMU_U32 Code);
void EmuRec_paddw(EMU_U32 Code);
void EmuRec_psubw(EMU_U32 Code);
void EmuRec_pcgtw(EMU_U32 Code);
void EmuRec_pmaxw(EMU_U32 Code);
void EmuRec_paddh(EMU_U32 Code);
void EmuRec_psubh(EMU_U32 Code);
void EmuRec_pcgth(EMU_U32 Code);
void EmuRec_pmaxh(EMU_U32 Code);
void EmuRec_paddb(EMU_U32 Code);
void EmuRec_psubb(EMU_U32 Code);
void EmuRec_pcgtb(EMU_U32 Code);
void EmuRec_paddsw(EMU_U32 Code);
void EmuRec_psubsw(EMU_U32 Code);
void EmuRec_pextlw(EMU_U32 Code);
void EmuRec_ppacw(EMU_U32 Code);
void EmuRec_paddsh(EMU_U32 Code);
void EmuRec_psubsh(EMU_U32 Code);
void EmuRec_pextlh(EMU_U32 Code);
void EmuRec_ppach(EMU_U32 Code);
void EmuRec_paddsb(EMU_U32 Code);
void EmuRec_psubsb(EMU_U32 Code);
void EmuRec_pextlb(EMU_U32 Code);
void EmuRec_ppacb(EMU_U32 Code);
void EmuRec_pext5(EMU_U32 Code);
void EmuRec_ppac5(EMU_U32 Code);
void EmuRec_pmaddw(EMU_U32 Code);
void EmuRec_psllvw(EMU_U32 Code);
void EmuRec_psrlvw(EMU_U32 Code);
void EmuRec_pmsubw(EMU_U32 Code);
void EmuRec_pmfhi(EMU_U32 Code);
void EmuRec_pmflo(EMU_U32 Code);
void EmuRec_pinth(EMU_U32 Code);
void EmuRec_pmultw(EMU_U32 Code);
void EmuRec_pdivw(EMU_U32 Code);
void EmuRec_pcpyld(EMU_U32 Code);
void EmuRec_pmaddh(EMU_U32 Code);
void EmuRec_phmadh(EMU_U32 Code);
void EmuRec_pand(EMU_U32 Code);
void EmuRec_pxor(EMU_U32 Code);
void EmuRec_pmsubh(EMU_U32 Code);
void EmuRec_phmsbh(EMU_U32 Code);
void EmuRec_pexeh(EMU_U32 Code);
void EmuRec_prevh(EMU_U32 Code);
void EmuRec_pmulth(EMU_U32 Code);
void EmuRec_pdivbw(EMU_U32 Code);
void EmuRec_pexew(EMU_U32 Code);
void EmuRec_prot3w(EMU_U32 Code);
void EmuRec_mfhi1(EMU_U32 Code);
void EmuRec_mthi1(EMU_U32 Code);
void EmuRec_mflo1(EMU_U32 Code);
void EmuRec_mtlo1(EMU_U32 Code);
void EmuRec_mult1(EMU_U32 Code);
void EmuRec_multu1(EMU_U32 Code);
void EmuRec_div1(EMU_U32 Code);
void EmuRec_divu1(EMU_U32 Code);
void EmuRec_madd1(EMU_U32 Code);
void EmuRec_maddu1(EMU_U32 Code);
void EmuRec_pabsw(EMU_U32 Code);
void EmuRec_pceqw(EMU_U32 Code);
void EmuRec_pminw(EMU_U32 Code);
void EmuRec_padsbh(EMU_U32 Code);
void EmuRec_pabsh(EMU_U32 Code);
void EmuRec_pceqh(EMU_U32 Code);
void EmuRec_pminh(EMU_U32 Code);
void EmuRec_pceqb(EMU_U32 Code);
void EmuRec_padduw(EMU_U32 Code);
void EmuRec_psubuw(EMU_U32 Code);
void EmuRec_pextuw(EMU_U32 Code);
void EmuRec_padduh(EMU_U32 Code);
void EmuRec_psubuh(EMU_U32 Code);
void EmuRec_pextuh(EMU_U32 Code);
void EmuRec_paddub(EMU_U32 Code);
void EmuRec_psubub(EMU_U32 Code);
void EmuRec_pextub(EMU_U32 Code);
void EmuRec_qfsrv(EMU_U32 Code);
void EmuRec_pmadduw(EMU_U32 Code);
void EmuRec_psravw(EMU_U32 Code);
void EmuRec_pmthi(EMU_U32 Code);
void EmuRec_pmtlo(EMU_U32 Code);
void EmuRec_pinteh(EMU_U32 Code);
void EmuRec_pmultuw(EMU_U32 Code);
void EmuRec_pdivuw(EMU_U32 Code);
void EmuRec_pcpyud(EMU_U32 Code);
void EmuRec_por(EMU_U32 Code);
void EmuRec_pnor(EMU_U32 Code);
void EmuRec_pexch(EMU_U32 Code);
void EmuRec_pcpyh(EMU_U32 Code);
void EmuRec_pexcw(EMU_U32 Code);
void EmuRec_pmfhl(EMU_U32 Code);
void EmuRec_pmthl(EMU_U32 Code);
void EmuRec_psllh(EMU_U32 Code);
void EmuRec_psrlh(EMU_U32 Code);
void EmuRec_psrah(EMU_U32 Code);
void EmuRec_psllw(EMU_U32 Code);
void EmuRec_psrlw(EMU_U32 Code);
void EmuRec_psraw(EMU_U32 Code);


// Writing functions

void EmuRec_Write8(EMU_U08 Data);
void EmuRec_Write16(EMU_U16 Data);
void EmuRec_Write32(EMU_U32 Data);
