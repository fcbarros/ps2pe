
#include "EmuMain.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

extern BOOL EmuStopCompile;

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_j( EMU_U32 Code )
{
    EMU_U32 JumpTo = JUMP_ADDRESS;

    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U32 LinkAddress = (EMU_U32)EmuRec_CheckAddress( JumpTo );
    if ( LinkAddress )
    {
        EmuRec_ResolveRegDependency( LinkAddress );
    }
    else
    {
        EmuRec_FreeAll( );
    }
    EMU_U32 * Link = JMP32( 0 );
    if ( LinkAddress )
    {
        *Link = LinkAddress - (EMU_U32)Link - 4;
    }
    else
    {
        EmuRec_PushPendingJump( Link, JumpTo );
    }

    EmuStopCompile = TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_jal( EMU_U32 Code )
{
    EMU_U32 JumpTo = JUMP_ADDRESS;

    EMU_I32 RegRA = EmuRec_CheckIntRegCache( 31, 0 );
    if ( RegRA >= 0 )
    {
        MOV32ItoR( RegRA, R5900Regs.PC + 4 );
    }
    else
    {
        MOV32ItoM( (EMU_U32)&R5900Regs.RA.u64_00_63, R5900Regs.PC + 4 );
    }

    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U32 LinkAddress = (EMU_U32)EmuRec_CheckAddress( JumpTo );
    if ( LinkAddress )
    {
        EmuRec_ResolveRegDependency( LinkAddress );
    }
    else
    {
        EmuRec_FreeAll( );
    }
    EMU_U32 * Link = JMP32( 0 );
    if ( LinkAddress )
    {
        *Link = LinkAddress - (EMU_U32)Link - 4;
    }
    else
    {
        EmuRec_PushPendingJump( Link, JumpTo );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_beq( EMU_U32 Code )
{
    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
    EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );
    EMU_I32 RegRT0 = EmuRec_CheckIntRegCache( R_RT, 0 );
    EMU_I32 RegRT1 = EmuRec_CheckIntRegCache( R_RT, 1 );

    if ( ( RegRS1 >= 0 ) && ( RegRT1 >= 0 ) )
    {
        CMP32RtoR( RegRS1, RegRT1 );
    }
    else
    if ( RegRS1 >= 0 )
    {
        CMP32MtoR( RegRS1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
    }
    else
    if ( RegRT1 >= 0 )
    {
        RegRS1 = EmuRec_GetIntReg( R_RS, 1, TRUE );
        CMP32RtoR( RegRS1, RegRT1 );
    }
    else
    {
        RegRS1 = EmuRec_GetIntReg( R_RS, 1, TRUE );
        CMP32MtoR( RegRS1, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
    }
    EMU_U08 * LinkNE1 = JNE8( 0 );
    //  Equal

    MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    CMP32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    EMU_U08 * LinkNE2 = JNE8( 0 );
    //  Equal Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EmuRec_ResolveRegDependency( BranchTo );
        EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress( ) - 1;
        if ( ( OperandSize > 127 ) || ( OperandSize < -123 ) )
        {
            JMP32( OperandSize - 4 );
        }
        else
        {
            JMP8( OperandSize - 1 );
        }
    }
    else
    {
        EmuRec_FreeAll( );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

    EMUREC_ALIGN_BRANCH;
    *LinkNE1 = EmuRec_CurrentAddress( ) - LinkNE1 - 1;
    *LinkNE2 = EmuRec_CurrentAddress( ) - LinkNE2 - 1;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bne( EMU_U32 Code )
{
    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
    CMP32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
    EMU_U08 * LinkNE = JNE8( 0 );
    //  Equal
    MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    CMP32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    EMU_U08 * LinkE = JE8( 0 );
    //  Not Equal
    EMUREC_ALIGN_BRANCH;
    *LinkNE = EmuRec_CurrentAddress( ) - LinkNE - 1;

    // Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EmuRec_ResolveRegDependency( BranchTo );
        EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress( ) - 1;
        if ( ( OperandSize > 127 ) || ( OperandSize < -123 ) )
        {
            JMP32( OperandSize - 4 );
        }
        else
        {
            JMP8( OperandSize - 1 );
        }
    }
    else
    {
        EmuRec_FreeAll( );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

    EMUREC_ALIGN_BRANCH;
    *LinkE = EmuRec_CurrentAddress( ) - LinkE - 1;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_blez( EMU_U32 Code )
{
    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63, 0 );
    EMU_U08 * LinkG1 = JG8( 0 );
    EMU_U08 * LinkL = JL8( 0 );
    //  Equal
    CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31, 0 );
    EMU_U08 * LinkG2 = JG8( 0 );
    //  Less or Equal
    EMUREC_ALIGN_BRANCH;
    *LinkL = EmuRec_CurrentAddress( ) - LinkL - 1;

    // Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EmuRec_ResolveRegDependency( BranchTo );
        EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress( ) - 1;
        if ( ( OperandSize > 127 ) || ( OperandSize < -123 ) )
        {
            JMP32( OperandSize - 4 );
        }
        else
        {
            JMP8( OperandSize - 1 );
        }
    }
    else
    {
        EmuRec_FreeAll( );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

    EMUREC_ALIGN_BRANCH;
    *LinkG1 = EmuRec_CurrentAddress( ) - LinkG1 - 1;
    *LinkG2 = EmuRec_CurrentAddress( ) - LinkG2 - 1;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bgtz( EMU_U32 Code )
{
    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63, 0 );
    EMU_U08 * LinkLE1 = JLE8( 0 );
    EMU_U08 * LinkG = JG8( 0 );
    //  Equal
    CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31, 0 );
    EMU_U08 * LinkLE2 = JLE8( 0 );
    //  Less or Equal
    EMUREC_ALIGN_BRANCH;
    *LinkG = EmuRec_CurrentAddress( ) - LinkG - 1;

    // Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EmuRec_ResolveRegDependency( BranchTo );
        EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress( ) - 1;
        if ( ( OperandSize > 127 ) || ( OperandSize < -123 ) )
        {
            JMP32( OperandSize - 4 );
        }
        else
        {
            JMP8( OperandSize - 1 );
        }
    }
    else
    {
        EmuRec_FreeAll( );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

    EMUREC_ALIGN_BRANCH;
    *LinkLE1 = EmuRec_CurrentAddress( ) - LinkLE1 - 1;
    *LinkLE2 = EmuRec_CurrentAddress( ) - LinkLE2 - 1;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_addi( EMU_U32 Code )
{
    // ******************************************
    // Not doing the OVERFLOW TRAP
    // ******************************************
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;
        if ( R_RS )
        {
            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            if ( Immediate )
            {
                ADD32ItoR( EAX, Immediate );
            }
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );
            SAR32ItoR( EAX, 31 );
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EAX );
        }
        else
        {
            MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, ( Immediate ) ? Immediate : 0x0 );
            MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, ( Immediate & 0x80000000 ) ? 0xFFFFFFFF : 0x0 );
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_addiu( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;
        if ( R_RS )
        {
            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            if ( Immediate )
            {
                ADD32ItoR( EAX, Immediate );
            }
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );
            SAR32ItoR( EAX, 31 );
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EAX );
        }
        else
        {
            MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, ( Immediate ) ? Immediate : 0x0 );
            MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, ( Immediate & 0x80000000 ) ? 0xFFFFFFFF : 0x0 );
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_slti( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0x0 );

        CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63, ( Immediate & 0x80000000 ) ? 0xFFFFFFFF : 0x0 );
        EMU_U08 * LinkA = JG8( 0 );
        EMU_U08 * LinkB = JL8( 0 );
        //  Equal
        CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31, Immediate );
        // Less than
        EMU_U08 * LinkAE = JGE8( 0 );
        
        EMUREC_ALIGN_BRANCH;
        *LinkB = EmuRec_CurrentAddress( ) - LinkB - 1;
        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0x1 );
        EMU_U08 * LinkEnd = JMP8( 0 );

        // Above or Equal
        EMUREC_ALIGN_BRANCH;
        *LinkA = EmuRec_CurrentAddress( ) - LinkA - 1;
        *LinkAE = EmuRec_CurrentAddress( ) - LinkAE - 1;
        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0x0 );

        EMUREC_ALIGN_BRANCH;
        *LinkEnd = EmuRec_CurrentAddress( ) - LinkEnd - 1;
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sltiu( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0x0 );

        CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63, ( Immediate & 0x80000000 ) ? 0xFFFFFFFF : 0x0 );
        EMU_U08 * LinkA = JA8( 0 );
        EMU_U08 * LinkB = JB8( 0 );
        //  Equal
        CMP32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31, Immediate );
        // Less than
        EMU_U08 * LinkAE = JAE8( 0 );
        EMUREC_ALIGN_BRANCH;
        *LinkB = EmuRec_CurrentAddress( ) - LinkB - 1;
        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0x1 );
        EMU_U08 * LinkEnd = JMP8( 0 );
        // Above or Equal
        EMUREC_ALIGN_BRANCH;
        *LinkA = EmuRec_CurrentAddress( ) - LinkA - 1;
        *LinkAE = EmuRec_CurrentAddress( ) - LinkAE - 1;
        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, 0x0 );
        EMUREC_ALIGN_BRANCH;
        *LinkEnd = EmuRec_CurrentAddress( ) - LinkEnd - 1;
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_andi( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = R_IMMEDIATE;
        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0x0 );

        if ( R_RS != R_RT )
        {
            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            AND32ItoR( EAX, Immediate );
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );
        }
        else
        {
            AND32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, Immediate );
            AND32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0 );
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ori( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = R_IMMEDIATE;
        if ( R_RS != R_RT )
        {
            MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EDX );

            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            if ( Immediate )
            {
                OR32ItoR( EAX, Immediate );
            }
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );
        }
        else
        {
            OR32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, Immediate );
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_xori( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = R_IMMEDIATE;
        if ( R_RS != R_RT )
        {
            MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EDX );

            MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
            if ( Immediate )
            {
                XOR32ItoR( EAX, Immediate );
            }
            MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );
        }
        else
        {
            XOR32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, Immediate );
        }
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lui( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = R_IMMEDIATE << 16;

        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, Immediate );
        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, ( Immediate & 0x80000000 ) ? 0xFFFFFFFF : 0x0 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_beql( EMU_U32 Code )
{
    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_32_63 );
    CMP32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
    EMU_U08 * LinkNE1 = JNE8( 0 );
    //  Equal
    MOV32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    CMP32MtoR( EDX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    EMU_U08 * LinkNE2 = JNE8( 0 );
    //  Equal Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress( ) - 1;
        if ( ( OperandSize > 127 ) || ( OperandSize < -123 ) )
        {
            JMP32( OperandSize - 4 );
        }
        else
        {
            JMP8( OperandSize - 1 );
        }
    }
    else
    {
        EmuRec_FreeAll( );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

    EMUREC_ALIGN_BRANCH;
    *LinkNE1 = EmuRec_CurrentAddress( ) - LinkNE1 - 1;
    *LinkNE2 = EmuRec_CurrentAddress( ) - LinkNE2 - 1;

    R5900Regs.PC += 4;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_bnel( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].u64_00_63 != R5900Regs.Reg[ R_RT ].u64_00_63 )
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
void EmuRec_blezl( EMU_U32 Code )
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
void EmuRec_bgtzl( EMU_U32 Code )
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
void EmuRec_daddi( EMU_U32 Code )
{
/*
    // ******************************************
    // Not doing the OVERFLOW TRAP
    // ******************************************
    R5900Regs.Reg[ R_RT ].i64_00_63 = R5900Regs.Reg[ R_RS ].i64_00_63 + (EMU_I16)R_IMMEDIATE;
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_daddiu( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RT ].i64_00_63 = R5900Regs.Reg[ R_RS ].i64_00_63 + (EMU_I16)R_IMMEDIATE;
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ldl( EMU_U32 Code )
{
/*
    RT = R_RT;
    Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
    SA = Address & 0x07;
    TempU64 = EmuMemGetDWord( Address - 7 ) & ( 0xFFFFFFFFFFFFFFFF << ( ( 7 - SA ) << 3 ) );
    R5900Regs.Reg[ RT ].u64_00_63 = ( R5900Regs.Reg[ RT ].u64_00_63 &
                                    ( 0xFFFFFFFFFFFFFFFF >> ( ( SA + 1 ) << 3 ) ) ) |
                                    TempU64;
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_ldr( EMU_U32 Code )
{
/*
    RT = R_RT;
    Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
    SA = Address & 0x07;
    TempU64 = EmuMemGetDWord( Address ) & ( 0xFFFFFFFFFFFFFFFF >> ( SA  << 3 ) );
    R5900Regs.Reg[ RT ].u64_00_63 = ( R5900Regs.Reg[ RT ].u64_00_63 &
                                    ( 0xFFFFFFFFFFFFFFFF << ( ( 8 - SA ) << 3 ) ) ) |
                                    TempU64;
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lq( EMU_U32 Code )
{
/*
    Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
    R5900Regs.Reg[ R_RT ].u64_00_63 = EmuMemGetDWord( Address );
    R5900Regs.Reg[ R_RT ].u64_64_127 = EmuMemGetDWord( Address + 8 );
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sq( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

    if ( R_RT )
    {
    	PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
    	PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }
    else
    {
        PUSH32I( 0 );
        PUSH32I( 0 );
    }
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetDWord, (EMU_U32)EmuRec_CurrentAddress( ) );
	POP32R( EAX );

	ADD32ItoR( EAX, 8 );
    if ( R_RT )
    {
	    PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_96_127 );
	    PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_64_95 );
    }
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetDWord, (EMU_U32)EmuRec_CurrentAddress( ) );

    if ( R_RT )
    {
    	ADD32ItoR( ESP, 20 );
    }
    else
    {
    	ADD32ItoR( ESP, 12 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lb( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        if ( Immediate )
        {
            ADD32ItoR( EAX, Immediate );
        }

        PUSH32R( EAX );
	    CALLFunc( (EMU_U32)EmuMemGetByte, (EMU_U32)EmuRec_CurrentAddress( ) );

        AND32ItoR( EAX, 0xFF );
        SHL32ItoR( EAX, 24 );
        SAR32ItoR( EAX, 24 );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );

        SAR32ItoR( EAX, 8 );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EAX );
    
        ADD32ItoR( ESP, 4 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lh( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        if ( Immediate )
        {
            ADD32ItoR( EAX, Immediate );
        }

        PUSH32R( EAX );
	    CALLFunc( (EMU_U32)EmuMemGetShort, (EMU_U32)EmuRec_CurrentAddress( ) );

        AND32ItoR( EAX, 0xFFFF );
        CWDE( );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );

        SHL32ItoR( EAX, 16 );
        SAR32ItoR( EAX, 31 );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EAX );
    
        ADD32ItoR( ESP, 4 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwl( EMU_U32 Code )
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
void EmuRec_lw( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        if ( Immediate )
        {
            ADD32ItoR( EAX, Immediate );
        }

        PUSH32R( EAX );
	    CALLFunc( (EMU_U32)EmuMemGetWord, (EMU_U32)EmuRec_CurrentAddress( ) );

        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );

        SAR32ItoR( EAX, 31 );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EAX );
    
        ADD32ItoR( ESP, 4 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lbu( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        if ( Immediate )
        {
            ADD32ItoR( EAX, Immediate );
        }

        PUSH32R( EAX );
	    CALLFunc( (EMU_U32)EmuMemGetByte, (EMU_U32)EmuRec_CurrentAddress( ) );

        AND32ItoR( EAX, 0xFF );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );

        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0 );
    
        ADD32ItoR( ESP, 4 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lhu( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        if ( Immediate )
        {
            ADD32ItoR( EAX, Immediate );
        }

        PUSH32R( EAX );
	    CALLFunc( (EMU_U32)EmuMemGetShort, (EMU_U32)EmuRec_CurrentAddress( ) );

        AND32ItoR( EAX, 0xFFFF );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );

        MOV32ItoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, 0 );
    
        ADD32ItoR( ESP, 4 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwr( EMU_U32 Code )
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
void EmuRec_lwu( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RT ].u64_00_63 = 
        EmuMemGetWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sb( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

    if ( R_RT )
    {
    	PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }
    else
    {
        PUSH32I( 0 );
    }
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetByte, (EMU_U32)EmuRec_CurrentAddress( ) );

    ADD32ItoR( ESP, 8 );
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sh( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

    if ( R_RT )
    {
    	PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }
    else
    {
        PUSH32I( 0 );
    }
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetShort, (EMU_U32)EmuRec_CurrentAddress( ) );

    ADD32ItoR( ESP, 8 );
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_swl( EMU_U32 Code )
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
void EmuRec_sw( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

    if ( R_RT )
    {
    	PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }
    else
    {
        PUSH32I( 0 );
    }
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetWord, (EMU_U32)EmuRec_CurrentAddress( ) );

    ADD32ItoR( ESP, 8 );
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sdl( EMU_U32 Code )
{
/*
    RT = R_RT;
    Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
    SA = Address & 0x07;
    TempU64 = ( EmuMemGetDWord( Address - 7 ) & ( 0xFFFFFFFFFFFFFFFF >> ( ( SA + 1 ) << 3 ) ) ) |
              ( R5900Regs.Reg[ RT ].u64_00_63 & ( 0xFFFFFFFFFFFFFFFF << ( ( 7 - SA ) << 3 ) ) );
    EmuMemSetDWord( Address - 7, TempU64 );
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sdr( EMU_U32 Code )
{
/*
    RT = R_RT;
    Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
    SA = Address & 0x07;
    TempU64 = ( EmuMemGetDWord( Address ) & 
              ( 0xFFFFFFFFFFFFFFFF << ( ( 8 - SA ) << 3 ) ) ) |
              ( R5900Regs.Reg[ RT ].u64_00_63 & ( 0xFFFFFFFFFFFFFFFF >> ( SA << 3 ) ) );
    EmuMemSetDWord( Address, TempU64 );
*/
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_swr( EMU_U32 Code )
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
void EmuRec_cache( EMU_U32 Code )
{
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lwc1( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

    PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemGetWord, (EMU_U32)EmuRec_CurrentAddress( ) );

    MOV32RtoM( (EMU_U32)&COP1Regs.Reg[ R_RT ].u, EAX );
    ADD32ItoR( ESP, 4 );
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_pref( EMU_U32 Code )
{
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_lqc2( EMU_U32 Code )
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
void EmuRec_ld( EMU_U32 Code )
{
    if ( R_RT )
    {
        EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
        if ( Immediate )
        {
            ADD32ItoR( EAX, Immediate );
        }

        PUSH32R( EAX );
	    CALLFunc( (EMU_U32)EmuMemGetDWord, (EMU_U32)EmuRec_CurrentAddress( ) );

        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );

        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EDX );
    
        ADD32ItoR( ESP, 4 );
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_swc1( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

	PUSH32M( (EMU_U32)&COP1Regs.Reg[ R_FT ].u );
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetWord, (EMU_U32)EmuRec_CurrentAddress( ) );

	ADD32ItoR( ESP, 8 );
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void EmuRec_sqc2( EMU_U32 Code )
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
void EmuRec_sd( EMU_U32 Code )
{
    EMU_U32 Immediate = (EMU_I16)R_IMMEDIATE;

    MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RS ].u32_00_31 );
    if ( Immediate )
    {
        ADD32ItoR( EAX, Immediate );
    }

    if ( R_RT )
    {
	    PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63 );
    	PUSH32M( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
    }
    else
    {
        PUSH32I( 0 );
        PUSH32I( 0 );
    }
	PUSH32R( EAX );
	CALLFunc( (EMU_U32)EmuMemSetDWord, (EMU_U32)EmuRec_CurrentAddress( ) );

    ADD32ItoR( ESP, 12 );
}
