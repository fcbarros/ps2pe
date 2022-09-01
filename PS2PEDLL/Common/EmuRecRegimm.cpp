
#include "EmuMain.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

extern BOOL EmuBranchInstruction;

void EmuRec_bltz( EMU_U32 Code )
{
    EmuRec_FlushAll( );

    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
    EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

    COMPAREREGZERO( RegRS1, R_RS, 1 );
    EMU_U32 * LinkG = JG32( 0 );
    EMU_U08 * LinkL = JL8( 0 );
    //  Equal
    COMPAREREGZERO( RegRS0, R_RS, 0 );
    EMU_U32 * LinkGE = JGE32( 0 );
    //  Less or Equal
//    EMUREC_ALIGN_BRANCH;
    SETLINK8( LinkL );

    // Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    // Increments the Count register
    ADD8ItoR( EBP, 1 );

    InsertBlockUpdate( BranchTo );

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EmuRec_SetRegDependency( BranchTo );
        EMU_I32 OperandSize = BranchPointer - EmuRec_CurrentAddress( ) - 1;
        if ( ( OperandSize > 127 ) || ( OperandSize < 128 ) )
        {
            JMP32( OperandSize  - 4 );
        }
        else
        {
            JMP8( OperandSize - 1 );
        }
    }
    else
    {
        EmuRec_FlushModified( FALSE );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

//    EMUREC_ALIGN_BRANCH;
    SETLINK32( LinkG );
    SETLINK32( LinkGE );

    EmuBranchInstruction = TRUE;
}

void EmuRec_bgez( EMU_U32 Code )
{
    EMU_U32 BranchTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);

    EMU_I32 RegRS0 = EmuRec_CheckIntRegCache( R_RS, 0 );
    EMU_I32 RegRS1 = EmuRec_CheckIntRegCache( R_RS, 1 );

    COMPAREREGZERO( RegRS1, R_RS, 1 );
    EMU_U32 * LinkL1 = JL32( 0 );
    EMU_U08 * LinkG = JG8( 0 );
    //  Equal
    COMPAREREGZERO( RegRS0, R_RS, 0 );
    EMU_U32 * LinkL2 = JL32( 0 );
    //  Less or Equal
//    EMUREC_ALIGN_BRANCH;
    SETLINK8( LinkG );

    // Do BRANCH
    EMU_U32 TempPC = R5900Regs.PC;                              // Save Current PC
    EmuRec_RecompileInstruction( R5900Regs.PC, TRUE );          // Insert Branch Delay Slot code
    R5900Regs.PC = TempPC;                                      // Restore PC

    // Increments the Count register
    ADD8ItoR( EBP, 1 );

    EMU_U08 * BranchPointer = EmuRec_CheckAddress( BranchTo );
    if ( BranchPointer )
    {
        EmuRec_SetRegDependency( BranchTo );
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
        EmuRec_FlushModified( FALSE );
        EMU_U32 * Link = JMP32( 0 );
        EmuRec_PushPendingJump( Link, BranchTo );
    }

//    EMUREC_ALIGN_BRANCH;
    SETLINK32( LinkL1 );
    SETLINK32( LinkL2 );

    EmuBranchInstruction = TRUE;
}

void EmuRec_bltzl( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        EXECUTE_BRANCH( R5900Regs.PC );
    }
    else
    {
        R5900Regs.PC += 4;
    }
*/
}

void EmuRec_bgezl( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        EXECUTE_BRANCH( R5900Regs.PC );
    }
    else
    {
        R5900Regs.PC += 4;
    }
*/
}

void EmuRec_tgei( EMU_U32 Code )
{
}

void EmuRec_tgeiu( EMU_U32 Code )
{
}

void EmuRec_tlti( EMU_U32 Code )
{
}

void EmuRec_tltiu( EMU_U32 Code )
{
}

void EmuRec_teqi( EMU_U32 Code )
{
}

void EmuRec_tnei( EMU_U32 Code )
{
}

void EmuRec_bltzal( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        EXECUTE_BRANCH( R5900Regs.PC );
    }
*/
}

void EmuRec_bgezal( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        EXECUTE_BRANCH( R5900Regs.PC );
    }
*/
}

void EmuRec_bltzall( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        EXECUTE_BRANCH( R5900Regs.PC );
    }
    else
    {
        R5900Regs.PC += 4;
    }
*/
}

void EmuRec_bgezall( EMU_U32 Code )
{
/*
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        EXECUTE_BRANCH( R5900Regs.PC );
    }
    else
    {
        R5900Regs.PC += 4;
    }
*/
}

void EmuRec_mtsab( EMU_U32 Code )
{
/*
    R5900Regs.SA = ( ( R5900Regs.Reg[ R_RS ].u08_00_07 & 0xF ) ^ 
                     ( Code & 0x0000000F ) ) * 8;
*/
}

void EmuRec_mtsah( EMU_U32 Code )
{
/*
    R5900Regs.SA = ( ( R5900Regs.Reg[ R_RS ].u08_00_07 & 0x7) ^ 
                    ( Code & 0x00000007 ) ) * 16;
*/
}
