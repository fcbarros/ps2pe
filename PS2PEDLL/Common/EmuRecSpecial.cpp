
#include "EmuMain.h"
#include "EmuBios.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

extern BOOL EmuStopCompile;
extern EMU_U32 RegisterJumpAddress;
extern BOOL EmuBranchInstruction;



void EmuRec_sll( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        if ( R_RD != R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
        }
        
        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            if ( R_RT != R_RD )
            {
                EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
                if ( RegRT0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }                
            if ( Shift )
            {
                SHL32ItoR( RegRD0, Shift );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_srl( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        if ( R_RD != R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
        }
        
        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            if ( R_RT != R_RD )
            {
                EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
                if ( RegRT0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }                
            if ( Shift )
            {
                SHR32ItoR( RegRD0, Shift );
            }
            XOR32RtoR( RegRD1, RegRD1 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_sra( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        if ( R_RD != R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
        }
        
        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            if ( R_RT != R_RD )
            {
                EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
                if ( RegRT0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }                
            if ( Shift )
            {
                SAR32ItoR( RegRD0, Shift );
            }
            XOR32RtoR( RegRD1, RegRD1 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_sllv( EMU_U32 Code )
{
/*
    TempU32 = R5900Regs.Reg[ R_RT ].u32_00_31 << ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F );
    R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)TempU32;
*/
}

void EmuRec_srlv( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].u32_00_31 >> ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F );
*/
}

void EmuRec_srav( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ R_RT ].i32_00_31 >> 
                                               ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F ) );
*/
}

void EmuRec_jr( EMU_U32 Code )
{
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code

    EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
    if ( RegRS0 >= 0 )
    {
        PUSH32R( RegRS0 );
    }
    else
    {
        PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    }
    // Increments the Count register
    ADD8ItoR( EBP, 1 );

    EmuRec_FreeAll( );

//    InsertBlockUpdate( );

    CALLFunc( (EMU_U32)EmuRec_PrepareRegisterJump, (EMU_U32)EmuRec_CurrentAddress( ) );
    ADD32ItoR( ESP, 4 );
    
    JMP32M( (EMU_U32)&RegisterJumpAddress );

    EmuStopCompile = TRUE;

    EmuBranchInstruction = TRUE;
}

void EmuRec_jalr( EMU_U32 Code )
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

void EmuRec_movz( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1,
                RegRT0,
                RegRT1,
                RegRS0,
                RegRS1;
        if ( ( R_RD == R_RT ) || ( R_RD == R_RS ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }
        
        RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
        RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

        if ( R_RT )
        {
            if ( RegRT1 >= 0 )
            {
                CMP32ItoR( RegRT1, 0 );
            }
            else
            {
                CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0 );
            }
            EMU_U08 * LinkNE1 = JNE8( 0 );
            if ( RegRT0 >= 0 )
            {
                CMP32ItoR( RegRT0, 0 );
            }
            else
            {
                CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0 );
            }
            EMU_U08 * LinkNE2 = JNE8( 0 );

            if ( R_RS )
            {
                if ( RegRS0 >= 0 )
                {
                    if ( RegRS0 != RegRD0 )
                    {
                        MOV32RtoR( RegRD0, RegRS0 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    if ( RegRS1 != RegRD1 )
                    {
                        MOV32RtoR( RegRD1, RegRS1 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                XOR32RtoR( RegRD0, RegRD0 );
                XOR32RtoR( RegRD1, RegRD1 );
            }
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );

            SETLINK8( LinkNE1 );
            SETLINK8( LinkNE2 );
        }
        else
        {
            if ( R_RS )
            {
                if ( RegRS0 >= 0 )
                {
                    if ( RegRS0 != RegRD0 )
                    {
                        MOV32RtoR( RegRD0, RegRS0 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    if ( RegRS1 != RegRD1 )
                    {
                        MOV32RtoR( RegRD1, RegRS1 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                XOR32RtoR( RegRD0, RegRD0 );
                XOR32RtoR( RegRD1, RegRD1 );
            }
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );
        }
    }
}

void EmuRec_movn( EMU_U32 Code )
{
    if ( R_RD )
    {
        if ( R_RT )
        {
            EMU_I32 RegRD0,
                    RegRD1,
                    RegRT0,
                    RegRT1,
                    RegRS0,
                    RegRS1;
            if ( ( R_RD == R_RT ) || ( R_RD == R_RS ) )
            {
                RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
                RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
            }
            else
            {
                RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
                RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
            }
        
            RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
            RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

            if ( RegRT1 >= 0 )
            {
                CMP32ItoR( RegRT1, 0 );
            }
            else
            {
                CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0 );
            }
            EMU_U08 * LinkNE = JNE8( 0 );
            if ( RegRT0 >= 0 )
            {
                CMP32ItoR( RegRT0, 0 );
            }
            else
            {
                CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0 );
            }
            EMU_U08 * LinkE = JE8( 0 );

            SETLINK8( LinkNE );
            if ( R_RS )
            {
                if ( RegRS0 >= 0 )
                {
                    if ( RegRS0 != RegRD0 )
                    {
                        MOV32RtoR( RegRD0, RegRS0 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    if ( RegRS1 != RegRD1 )
                    {
                        MOV32RtoR( RegRD1, RegRS1 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                XOR32RtoR( RegRD0, RegRD0 );
                XOR32RtoR( RegRD1, RegRD1 );
            }
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );

            SETLINK8( LinkNE );
        }
    }
}

void EmuRec_syscall( EMU_U32 Code )
{
    EmuRec_FreeAll( );

    CALLFunc( (EMU_U32)Emu_Bios_Syscall, (EMU_U32)EmuRec_CurrentAddress( ) );
}

void EmuRec_break( EMU_U32 Code )
{
}

void EmuRec_sync( EMU_U32 Code )
{
}

void EmuRec_mfhi( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
        RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.HI.u32_00_31 );
        MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.HI.u32_32_63 );
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_mthi( EMU_U32 Code )
{
    EMU_I32 RegRD0,
            RegRD1;
    RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
    RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
    MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_00_31, RegRD0 );
    MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_32_63, RegRD1 );
}

void EmuRec_mflo( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
        RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.LO.u32_00_31 );
        MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.LO.u32_32_63 );
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_mtlo( EMU_U32 Code )
{
    EMU_I32 RegRD0,
            RegRD1;
    RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
    RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
    MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_00_31, RegRD0 );
    MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_32_63, RegRD1 );
}

void EmuRec_dsllv( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].u64_00_63 << 
                                    ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x3F );
*/
}

void EmuRec_dsrlv( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].u64_00_63 >>
                                    ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x3F );
*/
}

void EmuRec_dsrav( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].i64_00_63 >>
                                    ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x3F );
*/
}

void EmuRec_mult( EMU_U32 Code )
{
    EMU_I32 RegRD0,
            RegRD1;

    if ( R_RS && R_RT )
    {
        EMU_I32 RegRS0,
                RegRT0;
        EmuRec_FreeCPUReg( EAX );
        EmuRec_FreeCPUReg( EDX );
        RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        if ( RegRS0 >= 0 )
        {
            MOV32RtoR( EAX, RegRS0 );
        }
        else
        {
            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        }
        if ( RegRT0 >= 0 )
        {
            IMUL32R( RegRT0 );
        }
        else
        {
            IMUL32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
        }
    
        MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_00_31, EAX );
        SAR32ItoR( EAX, 31 );
        MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_32_63, EAX );

        MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_00_31, EDX );
        SAR32ItoR( EDX, 31 );
        MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_32_63, EDX );

        if ( R_RD )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
            MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.LO.u32_00_31 );
            MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.LO.u32_32_63 );
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );
        }
    }
    else
    {
        MOV32ItoM( (EMU_U32)&R5900Regs.LO.u32_00_31, 0 );
        MOV32ItoM( (EMU_U32)&R5900Regs.HI.u32_00_31, 0 );
        MOV32ItoM( (EMU_U32)&R5900Regs.LO.u32_32_63, 0 );
        MOV32ItoM( (EMU_U32)&R5900Regs.HI.u32_32_63, 0 );
        if ( R_RD )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );
        }
    }
}

void EmuRec_multu( EMU_U32 Code )
{
    EMU_I32 RegRD0,
            RegRD1;

    if ( R_RS && R_RT )
    {
        EMU_I32 RegRS0,
                RegRT0;
        EmuRec_FreeCPUReg( EAX );
        EmuRec_FreeCPUReg( EDX );
        RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        if ( RegRS0 >= 0 )
        {
            MOV32RtoR( EAX, RegRS0 );
        }
        else
        {
            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        }
        if ( RegRT0 >= 0 )
        {
            MUL32R( RegRT0 );
        }
        else
        {
            MUL32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
        }
        MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_00_31, EAX );
        SAR32ItoR( EAX, 31 );
        MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_32_63, EAX );

        MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_00_31, EDX );
        SAR32ItoR( EDX, 31 );
        MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_32_63, EDX );

        if ( R_RD )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
            MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.LO.u32_00_31 );
            MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.LO.u32_32_63 );
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );
        }
    }
    else
    {
        MOV32ItoM( (EMU_U32)&R5900Regs.LO.u32_00_31, 0 );
        MOV32ItoM( (EMU_U32)&R5900Regs.HI.u32_00_31, 0 );
        MOV32ItoM( (EMU_U32)&R5900Regs.LO.u32_32_63, 0 );
        MOV32ItoM( (EMU_U32)&R5900Regs.HI.u32_32_63, 0 );
        if ( R_RD )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
            EmuRec_SetIntRegModified( RegRD0 );
            EmuRec_SetIntRegModified( RegRD1 );
        }
    }
}

void EmuRec_div( EMU_U32 Code )
{
    EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
    if ( RegRT0 >= 0 )
    {
        CMP32ItoR( RegRT0, 0 );
    }
    else
    {
        CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0 );
    }
    EMU_U08 * LinkE0 = JE8( 0 );

    EmuRec_FreeCPUReg( EAX );
    EmuRec_FreeCPUReg( EDX );
    XOR32RtoR( EDX, EDX );

    EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
    if ( RegRS0 >= 0 )
    {
        MOV32RtoR( EAX, RegRS0 );
    }
    else
    {
        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    }
    RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
    if ( RegRT0 >= 0 )
    {
        IDIV32R( RegRT0 );
    }
    else
    {
        IDIV32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }

    MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_00_31, EAX );
    SAR32ItoR( EAX, 31 );
    MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_32_63, EAX );

    MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_00_31, EDX );
    SAR32ItoR( EDX, 31 );
    MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_32_63, EDX );

    SETLINK8( LinkE0 );
}

void EmuRec_divu( EMU_U32 Code )
{
    EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
    if ( RegRT0 >= 0 )
    {
        CMP32ItoR( RegRT0, 0 );
    }
    else
    {
        CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0 );
    }
    EMU_U08 * LinkE0 = JE8( 0 );

    EmuRec_FreeCPUReg( EAX );
    EmuRec_FreeCPUReg( EDX );
    XOR32RtoR( EDX, EDX );

    EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
    if ( RegRS0 >= 0 )
    {
        MOV32RtoR( EAX, RegRS0 );
    }
    else
    {
        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    }
    RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
    if ( RegRT0 >= 0 )
    {
        DIV32R( RegRT0 );
    }
    else
    {
        DIV32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }

    MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_00_31, EAX );
    SAR32ItoR( EAX, 31 );
    MOV32RtoM( (EMU_U32)&R5900Regs.LO.u32_32_63, EAX );

    MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_00_31, EDX );
    SAR32ItoR( EDX, 31 );
    MOV32RtoM( (EMU_U32)&R5900Regs.HI.u32_32_63, EDX );

    SETLINK8( LinkE0 );
}

void EmuRec_add( EMU_U32 Code )
{
    // ******************************************
    // Not doing the OVERFLOW TRAP
    // ******************************************
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_addu( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_sub( EMU_U32 Code )
{
    // ******************************************
    // Not doing the OVERFLOW TRAP
    // ******************************************
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    SUB32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    SUB32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    NEG32R( RegRD0 );
                    ADD32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    NEG32R( RegRD0 );
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRT0 >= 0 )
                {
                    SUB32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    SUB32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    XOR32RtoR( RegRD0, RegRD0 );
                    SUB32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    NEG32R( RegRD0 );
                }
            }
            else
            {
                XOR32RtoR( RegRD0, RegRD0 );
                SUB32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_subu( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    SUB32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    SUB32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    NEG32R( RegRD0 );
                    ADD32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    NEG32R( RegRD0 );
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRT0 >= 0 )
                {
                    SUB32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    SUB32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    XOR32RtoR( RegRD0, RegRD0 );
                    SUB32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    NEG32R( RegRD0 );
                }
            }
            else
            {
                XOR32RtoR( RegRD0, RegRD0 );
                SUB32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            EMUREC_SIGN_EXTEND( RegRD1, RegRD0 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_and( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    AND32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    AND32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    AND32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    AND32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    AND32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    AND32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    AND32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    AND32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
                if ( RegRT0 >= 0 )
                {
                    AND32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    AND32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    AND32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    AND32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_or( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    OR32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    OR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    OR32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    OR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    OR32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    OR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    OR32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    OR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
                if ( RegRT0 >= 0 )
                {
                    OR32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    OR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    OR32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    OR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            if ( RegRS1 >= 0 )
            {
                if ( RegRS1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            }
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_xor( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    XOR32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    XOR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    XOR32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    XOR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    XOR32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    XOR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    XOR32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    XOR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
                if ( RegRT0 >= 0 )
                {
                    XOR32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    XOR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    XOR32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    XOR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            if ( RegRS1 >= 0 )
            {
                if ( RegRS1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            }
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_nor( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        if ( R_RS && R_RT )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( R_RD == R_RS )
            {
                if ( RegRT0 >= 0 )
                {
                    OR32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    OR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    OR32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    OR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
            else
            if ( R_RD == R_RT )
            {
                if ( RegRS0 >= 0 )
                {
                    OR32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    OR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    OR32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    OR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
                if ( RegRT0 >= 0 )
                {
                    OR32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    OR32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    OR32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    OR32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
        }
        else
        if ( R_RS )
        {
            EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
            EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            if ( RegRS1 >= 0 )
            {
                if ( RegRS1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            }
        }
        else
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        NOT32R( RegRD0 );
        NOT32R( RegRD1 );
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_mfsa( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.SA;
*/
}

void EmuRec_mtsa( EMU_U32 Code )
{
/*
    R5900Regs.SA = R5900Regs.Reg[ R_RS ].u64_00_63;
*/
}

void EmuRec_slt( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        XOR32RtoR( RegRD1, RegRD1 );        
        
        EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
        EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

        COMPAREINTREGS( RegRS1, RegRT1, R_RS, R_RT, 1 );
        EMU_U08 * LinkA = JG8( 0 );
        EMU_U08 * LinkB = JL8( 0 );
        //  Equal
        COMPAREINTREGS( RegRS0, RegRT0, R_RS, R_RT, 0 );
        // Less than
        EMU_U08 * LinkAE = JGE8( 0 );
        EMUREC_ALIGN_BRANCH;
        SETLINK8( LinkB );
        MOV32ItoR( RegRD0, 0x1 );
        EMU_U08 * LinkEnd = JMP8( 0 );
        // Above or Equal
        EMUREC_ALIGN_BRANCH;
        SETLINK8( LinkA );
        SETLINK8( LinkAE );
        XOR32RtoR( RegRD0, RegRD0 );
        EMUREC_ALIGN_BRANCH;
        SETLINK8( LinkEnd );

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_sltu( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        XOR32RtoR( RegRD1, RegRD1 );        
        
        EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
        EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

        COMPAREINTREGS( RegRS1, RegRT1, R_RS, R_RT, 1 );
        EMU_U08 * LinkA = JA8( 0 );
        EMU_U08 * LinkB = JB8( 0 );
        //  Equal
        COMPAREINTREGS( RegRS0, RegRT0, R_RS, R_RT, 0 );
        EMU_U08 * LinkAE = JAE8( 0 );
        // Less than
        EMUREC_ALIGN_BRANCH;
        SETLINK8( LinkB );
        MOV32ItoR( RegRD0, 0x1 );
        EMU_U08 * LinkEnd = JMP8( 0 );
        // Above or Equal
        EMUREC_ALIGN_BRANCH;
        SETLINK8( LinkA );
        SETLINK8( LinkAE );
        XOR32RtoR( RegRD0, RegRD0 );
        EMUREC_ALIGN_BRANCH;
        SETLINK8( LinkEnd );

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_dadd( EMU_U32 Code )
{
    if ( R_RD )
    {
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }
        EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
        EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

        if ( R_RS && R_RT )
        {
            if ( R_RS == R_RD )
            {
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    ADC32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    ADC32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
            else
            if ( R_RT == R_RD )
            {
                if ( RegRS0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    ADC32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    ADC32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    if ( RegRS0 != RegRD0 )
                    {
                        MOV32RtoR( RegRD0, RegRS0 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    if ( RegRS1 != RegRD1 )
                    {
                        MOV32RtoR( RegRD1, RegRS1 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    ADC32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    ADC32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
        }
        else
        if ( R_RS )
        {
            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            if ( RegRS1 >= 0 )
            {
                if ( RegRS1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            }
        }
        else
        if ( R_RT )
        {
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_daddu( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( ( R_RD == R_RS ) || ( R_RD == R_RT ) )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }
        EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
        EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
        EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
        EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

        if ( R_RS && R_RT )
        {
            if ( R_RS == R_RD )
            {
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    ADC32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    ADC32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
            else
            if ( R_RT == R_RD )
            {
                if ( RegRS0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRS0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    ADC32RtoR( RegRD1, RegRS1 );
                }
                else
                {
                    ADC32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
            }
            else
            {
                if ( RegRS0 >= 0 )
                {
                    if ( RegRS0 != RegRD0 )
                    {
                        MOV32RtoR( RegRD0, RegRS0 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
                }
                if ( RegRS1 >= 0 )
                {
                    if ( RegRS1 != RegRD1 )
                    {
                        MOV32RtoR( RegRD1, RegRS1 );
                    }
                }
                else
                {
                    MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
                }
                if ( RegRT0 >= 0 )
                {
                    ADD32RtoR( RegRD0, RegRT0 );
                }
                else
                {
                    ADD32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
                }
                if ( RegRT1 >= 0 )
                {
                    ADC32RtoR( RegRD1, RegRT1 );
                }
                else
                {
                    ADC32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
                }
            }
        }
        else
        if ( R_RS )
        {
            if ( RegRS0 >= 0 )
            {
                if ( RegRS0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRS0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            }
            if ( RegRS1 >= 0 )
            {
                if ( RegRS1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRS1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            }
        }
        else
        if ( R_RT )
        {
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_dsub( EMU_U32 Code )
{
/*
    // ******************************************
    // Not doing the OVERFLOW TRAP
    // ******************************************
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 - 
                                      R5900Regs.Reg[ R_RT ].u64_00_63;
*/
}

void EmuRec_dsubu( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 - 
                                      R5900Regs.Reg[ R_RT ].u64_00_63;
*/
}

void EmuRec_tge( EMU_U32 Code )
{
}

void EmuRec_tgeu( EMU_U32 Code )
{
}

void EmuRec_tlt( EMU_U32 Code )
{
}

void EmuRec_tltu( EMU_U32 Code )
{
}

void EmuRec_teq( EMU_U32 Code )
{
}

void EmuRec_tne( EMU_U32 Code )
{
}

void EmuRec_dsll( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( R_RD == R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
            SHLD32ItoR( RegRD0, RegRD1, Shift );
            SHL32ItoR( RegRD0, Shift );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_dsrl( EMU_U32 Code )
{
    if ( R_RD )
    {
        EMU_I32 RegRD0,
                RegRD1;
        if ( R_RD == R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
            SHRD32ItoR( RegRD1, RegRD0, Shift );
            SHR32ItoR( RegRD1, Shift );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
/*        MOVQMtoR( MM0, (EMU_U32)&R5900Regs.Reg[ R_RT ] );
        PSRLQItoR( MM0, R_SA );
        MOVQRtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ], MM0 );
        EMMS( );*/
}

void EmuRec_dsra( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.Reg[ R_RT ].i64_00_63 >> R_SA;
*/
}

void EmuRec_dsll32( EMU_U32 Code )
{
    if ( R_RD )
    {
/*        MOVQMtoR( MM0, (EMU_U32)&R5900Regs.Reg[ R_RT ] );
        PSLLQItoR( MM0, R_SA + 32 );
        MOVQRtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ], MM0 );
        EMMS( );*/

        EMU_I32 RegRD0,
                RegRD1;
        if ( R_RD == R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
            MOV32RtoR( RegRD1, RegRD0 );
            SHL32ItoR( RegRD1, Shift );
            XOR32RtoR( RegRD0, RegRD0 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_dsrl32( EMU_U32 Code )
{
    if ( R_RD )
    {
/*        MOVQMtoR( MM0, (EMU_U32)&R5900Regs.Reg[ R_RT ] );
        PSLLQItoR( MM0, R_SA + 32 );
        MOVQRtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ], MM0 );
        EMMS( );*/

        EMU_I32 RegRD0,
                RegRD1;
        if ( R_RD == R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
            MOV32RtoR( RegRD0, RegRD1 );
            SHR32ItoR( RegRD0, Shift );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

void EmuRec_dsra32( EMU_U32 Code )
{
    if ( R_RD )
    {
/*        MOVQMtoR( MM0, (EMU_U32)&R5900Regs.Reg[ R_RT ] );
        PSLLQItoR( MM0, R_SA + 32 );
        MOVQRtoM( (EMU_U32)&R5900Regs.Reg[ R_RD ], MM0 );
        EMMS( );*/

        EMU_I32 RegRD0,
                RegRD1;
        if ( R_RD == R_RT )
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, TRUE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, TRUE );
        }
        else
        {
            RegRD0 = EmuRec_GetIntReg( R_RD, 0, FALSE );
            RegRD1 = EmuRec_GetIntReg( R_RD, 1, FALSE );
        }

        EMU_U32 Shift = R_SA;
        if ( R_RT )
        {
            EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
            EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );
            if ( RegRT0 >= 0 )
            {
                if ( RegRT0 != RegRD0 )
                {
                    MOV32RtoR( RegRD0, RegRT0 );
                }
            }
            else
            {
                MOV32MtoR( RegRD0, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
            }
            if ( RegRT1 >= 0 )
            {
                if ( RegRT1 != RegRD1 )
                {
                    MOV32RtoR( RegRD1, RegRT1 );
                }
            }
            else
            {
                MOV32MtoR( RegRD1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
            }
            MOV32RtoR( RegRD0, RegRD1 );
            SAR32ItoR( RegRD0, Shift );
            XOR32RtoR( RegRD1, RegRD1 );
        }
        else
        {
            XOR32RtoR( RegRD0, RegRD0 );
            XOR32RtoR( RegRD1, RegRD1 );
        }

        EmuRec_SetIntRegModified( RegRD0 );
        EmuRec_SetIntRegModified( RegRD1 );
    }
}

/*

EI

*/
