

#include "EmuMain.h"
#include "EmuIntc.h"
#include "EmuGS.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"


extern stEmuRecMemory RecMemory;

extern stEmuRecReg RecRegularRegisters[ 6 ];

extern stEmuRecEmuReg RecR5900Regs[ 32 ];

extern EMU_U32 CurrentPCAddress;

BOOL EmuStopCompile = FALSE;
BOOL EmuBranchInstruction = FALSE;

typedef void (*RECFUNCTION)( void );

void InsertBlockUpdate( EMU_U32 Address )
{
    // Call the update function if needed or exits
    TEST32ItoR( EBP, 0x1E );
    EMU_U08 * LinkNE1 = JNE8( 0 );
    MOV32ItoM( (EMU_U32)&R5900Regs.PC, Address );
    EmuRec_FlushModified( FALSE );
//    EmuRec_FlushModified( TRUE );
    CALLFunc( (EMU_U32)Emu_GS_ProcessMessages, (EMU_U32)EmuRec_CurrentAddress( ) );
    CMP32ItoM( (EMU_U32)&EmuStopRun, TRUE );
    EMU_U08 * LinkNE2 = JNE8( 0 );
    RET( );
    SETLINK8( LinkNE2 );
    EmuRec_FetchAll( );
    SETLINK8( LinkNE1 );
}

void EmuRec_RecompileExecute( EMU_U32 EntryPoint, EMU_U32 EndAddress, BOOL Loop )
{
    static EMU_U08 TempExec[ 1024 ];
    static stEmuRecMemory TempMemory;
    EMU_U32 CallAddress;

    memcpy( &TempMemory, &RecMemory, sizeof( stEmuRecMemory ) );

    RECFUNCTION RecFunction = (RECFUNCTION)&TempExec;
    RecMemory.Memory = TempExec;
    RecMemory.Position = 0;

    // EBP is being used as the Count register
    // Prepare for being called
    // Save all registers
    PUSHA32( );
    MOV32MtoR( EBP, (EMU_U32)&COP0Regs.Count );

    EMU_U08 * CurrentAddress = EmuRec_CheckAddress( EntryPoint );

    if ( CurrentAddress )
    {
        CallAddress = (EMU_U32)CurrentAddress;
        EmuRec_SetRegDependency( EntryPoint );
    }

    EMU_U32 * Link = CALLNearRel32( 0 );
    MOV32RtoM( (EMU_U32)&COP0Regs.Count, EBP );
    POPA32( );
    RET( );

    // Restore the Address of the rec memory
    memcpy( &RecMemory, &TempMemory, sizeof( stEmuRecMemory ) );

    if ( CurrentAddress == NULL )
    {
        EmuRec_Recompile( EntryPoint, EndAddress );
        CallAddress = (EMU_U32)EmuRec_CheckAddress( EntryPoint );
    }
    *Link = CallAddress - (EMU_U32)Link - 4;

    // Set the stop flag to false
    EmuStopRun = ! Loop;
    
    // Execute the code
    RecFunction( );
}

void EmuRec_RecompileInstruction( EMU_U32 Address, BOOL InBrachDelay )
{
    R5900Regs.PC = Address;
    CurrentPCAddress = Address;

    if ( EmuRec_IsPendingJumps( Address ) )
    {
        EmuRec_FreeAll( );
    }

    // 0 - Insert the current IP address in the Address Translation Table
    EmuRec_InsertAddress( R5900Regs.PC, &RecMemory.Memory[ RecMemory.Position ] );

    // 1 - Get the Instruction
    // Current Instruction Code
    EMU_U32 Code = EmuMemGetWord( R5900Regs.PC );
    // 1.1 - Updates the PC to the next instruction
    R5900Regs.PC += 4;
    // 1.2 - If the instruction is not NOP procede
    if ( Code ) 
    {
        // 2 - Call the Instruction Recompiler Function
        EmuRecCoreFunction[ Code >> 26 ]( Code );
    }

    if ( ! EmuStopCompile )
    {
        // Increments the Count register
        ADD8ItoR( EBP, 1 );
    
//        if ( ! InBrachDelay )
//        if ( ! EmuBranchInstruction )
        {
/*            EmuBranchInstruction = FALSE;
            // This block is just for debug
            CMP32ItoM( (EMU_U32)&EmuStopRun, TRUE ); // Verify Step
            EMU_U08 * LinkNE3 = JNE8( 0 );
            EmuRec_FlushModified( ); // in Step
            MOV32ItoM( (EMU_U32)&R5900Regs.PC, R5900Regs.PC );
            RET( );
            SETLINK8( LinkNE3 );
            EmuRec_FlushModified( );  // not in Step, save all registers
            PUSH32I( R5900Regs.PC ); // checks if address is a breakpoint
            CALLFunc( (EMU_U32)EmuIsBreakPoint, (EMU_U32)EmuRec_CurrentAddress( ) );
        //        ADD32ItoR( ESP, 4 );
            CMP32ItoR( EAX, TRUE );
            EMU_U08 * LinkNE4 = JNE8( 0 );
            CALLFunc( (EMU_U32)EmuRec_ResetRegs, (EMU_U32)EmuRec_CurrentAddress( ) );
            MOV32ItoM( (EMU_U32)&R5900Regs.PC, R5900Regs.PC );
            RET( );
            SETLINK8( LinkNE4 );
            EmuRec_FetchAll( ); // Is not a breakpoint, restores the registers
*/    
            // Call the update function if needed or exits
/*            TEST32ItoR( EBP, 0x7FF );
            EMU_U08 * LinkNE1 = JNE8( 0 );
            MOV32ItoM( (EMU_U32)&R5900Regs.PC, R5900Regs.PC );
            EmuRec_FlushModified( FALSE );
            CALLFunc( (EMU_U32)Emu_GS_ProcessMessages, (EMU_U32)EmuRec_CurrentAddress( ) );
            CMP32ItoM( (EMU_U32)&EmuStopRun, TRUE );
            EMU_U08 * LinkNE2 = JNE8( 0 );
            RET( );
            *LinkNE2 = EmuRec_CurrentAddress( ) - LinkNE2 - 1;
            EmuRec_FetchAll( );
            *LinkNE1 = EmuRec_CurrentAddress( ) - LinkNE1 - 1;*/
        }
    }
}

void EmuRec_Recompile( EMU_U32 StartAddress, EMU_U32 EndAddress )
{
    EmuStopCompile = FALSE;

    // Pending jump table
    stEmuRecPendingJump PendingJump;

    R5900Regs.PC = StartAddress;

    // Main compilation loop
    while ( ( ! EmuStopCompile ) && ( R5900Regs.PC < EndAddress ) )
    {
        EmuRec_RecompileInstruction( R5900Regs.PC, FALSE );

        // 3 - Check next adress
        // 3.1 - Check to see if next address is already computed
        // 3.1.2 - If no continue with the current address recompilation
        if ( EmuRec_CheckAddress( R5900Regs.PC ) != NULL )
        {
            // 3.1.1 - If yes, exits the loop
//            break;
        }
    }

    stEmuRecJumpRecord * JumpRecord;
    EMU_U08 * DestinationAddress;

    // Pending Jumps check
    while ( TRUE )
    {
        // 3.1.1.1 - If there is pending Jumps
        JumpRecord = EmuRec_CheckPendingJumps( );
        if ( JumpRecord != NULL )
        {
            // 3.1.1.1.1 - Update the pending jump table with the current defines addresses
            DestinationAddress = EmuRec_CheckAddress( JumpRecord->EmuTargetAddress );
            if ( DestinationAddress != NULL )
            {
                *JumpRecord->LinkAddress = (EMU_U32)DestinationAddress - (EMU_U32)JumpRecord->LinkAddress - 4;
                // 3.1.1.1.2 - Removes the jump from the table
                EmuRec_PopPendingJump( );
            }
            else
            {
                EmuRec_ResetRegs( );
                // 3.1.1.1.3 - Get the next unresolved jump and calls EmuRec_Recompile with the address
                EmuRec_Recompile( JumpRecord->EmuTargetAddress, EndAddress );
            }
        }
        else
        {
            // 3.1.1.2 - If there is no pending jumps, exits the loop
            break;
        }
    }
}

void EmuRec_Invalid( EMU_U32 Code )
{
    EmuLog( "Opcode %.8X invalid at address %.8X\n", Code, R5900Regs.PC );
    EmuConsole( "Opcode %.8X invalid at address %.8X\n", Code, R5900Regs.PC );
}

void EmuRec_Special( EMU_U32 Code )
{
    EmuRecSpecialFunction[ Code & 0x3F ]( Code );
}

void EmuRec_Regimm( EMU_U32 Code )
{
    EmuRecRegimmFunction[ ( Code >> 16 ) & 0x1F ]( Code );
}

void EmuRec_MMI( EMU_U32 Code )
{
    EmuRecMMIFunction[ Code & 0x3F ]( Code );
}

void EmuRec_MMI_0( EMU_U32 Code )
{
    EmuRecMMI_0Function[ Code & 0x3F ]( Code );
}

void EmuRec_MMI_1( EMU_U32 Code )
{
    EmuRecMMI_1Function[ Code & 0x3F ]( Code );
}

void EmuRec_MMI_2( EMU_U32 Code )
{
    EmuRecMMI_2Function[ Code & 0x3F ]( Code );
}

void EmuRec_MMI_3( EMU_U32 Code )
{
    EmuRecMMI_3Function[ Code & 0x3F ]( Code );
}

void EmuRec_COP0( EMU_U32 Code )
{
    EmuRecCOP0Function[ ( Code >> 21 ) & 0x1F ]( Code );
}

void EmuRec_COP0_BC0( EMU_U32 Code )
{
    EmuRecCOP0_BC0Function[ ( Code >> 16 ) & 0x1F ]( Code );
}

void EmuRec_COP0_C0( EMU_U32 Code )
{
    EmuRecCOP0_C0Function[ Code & 0x3F ]( Code );
}

void EmuRec_COP1( EMU_U32 Code )
{
    EmuRecCOP1Function[ ( Code >> 21 ) & 0x1F ]( Code );
}

void EmuRec_COP1_BC1( EMU_U32 Code )
{
    EmuRecCOP1_BC1Function[ ( Code >> 16 ) & 0x1F ]( Code );
}

void EmuRec_COP1_S( EMU_U32 Code )
{
    EmuRecCOP1_SFunction[ Code & 0x3F ]( Code );
}

void EmuRec_COP1_W( EMU_U32 Code )
{
    EmuRecCOP1_WFunction[ Code & 0x3F ]( Code );
}

EMURECFUNCTION EmuRecCoreFunction[64] =
{
    EmuRec_Special,  // "Special Instructions"
    EmuRec_Regimm,   // "Regimm Instructions"
    EmuRec_j,        // "j"
    EmuRec_jal,      // "jal"
    EmuRec_beq,      // "beq"
    EmuRec_bne,      // "bne"
    EmuRec_blez,     // "blez"
    EmuRec_bgtz,     // "bgtz"
    EmuRec_addi,     // "addi"
    EmuRec_addiu,    // "addiu"
    EmuRec_slti,     // "slti"
    EmuRec_sltiu,    // "sltiu"
    EmuRec_andi,     // "andi"
    EmuRec_ori,      // "ori"
    EmuRec_xori,     // "xori"
    EmuRec_lui,      // "lui"
    EmuRec_COP0,     // "COP0"
    EmuRec_COP1,     // "COP1"
    EmuRec_Invalid,  // "COP2"
    EmuRec_Invalid,  // "REGULAR(1)"
    EmuRec_beql,     // "beql"
    EmuRec_bnel,     // "bnel"
    EmuRec_blezl,    // "blezl"
    EmuRec_bgtzl,    // "bgtzl"
    EmuRec_daddi,    // "daddi"
    EmuRec_daddiu,   // "daddiu"
    EmuRec_ldl,      // "ldl"
    EmuRec_ldr,      // "ldr"
    EmuRec_MMI,      // "MMI"
    EmuRec_Invalid,  // "REGULAR(2)"
    EmuRec_lq,       // "lq"
    EmuRec_sq,       // "sq"
    EmuRec_lb,       // "lb"
    EmuRec_lh,       // "lh"
    EmuRec_lwl,      // "lwl"
    EmuRec_lw,       // "lw"
    EmuRec_lbu,      // "lbu"
    EmuRec_lhu,      // "lhu"
    EmuRec_lwr,      // "lwr"
    EmuRec_lwu,      // "lwu"
    EmuRec_sb,       // "sb"
    EmuRec_sh,       // "sh"
    EmuRec_swl,      // "swl"
    EmuRec_sw,       // "sw"
    EmuRec_sdl,      // "sdl"
    EmuRec_sdr,      // "sdr"
    EmuRec_swr,      // "swr"
    EmuRec_cache,    // "cache"
    EmuRec_Invalid,  // "REGULAR(LL)"
    EmuRec_lwc1,     // "lwc1"
    EmuRec_Invalid,  // "REGULAR(3)"
    EmuRec_pref,     // "pref"
    EmuRec_Invalid,  // "REGULAR(4)"
    EmuRec_Invalid,  // "REGULAR(5)"
    EmuRec_lqc2,     // "lqc2"
    EmuRec_ld,       // "ld"
    EmuRec_Invalid,  // "REGULAR(6)"
    EmuRec_swc1,     // "swc1"
    EmuRec_Invalid,  // "REGULAR(7)"
    EmuRec_Invalid,  // "REGULAR(8)"
    EmuRec_Invalid,  // "REGULAR(SC)"
    EmuRec_Invalid,  // "REGULAR(9)"
    EmuRec_sqc2,     // "sqc2"
    EmuRec_sd,       // "sd"
};

EMURECFUNCTION EmuRecSpecialFunction[64] =
{
    EmuRec_sll,      // "sll"
    EmuRec_Invalid,  // "SPECIAL(1)"
    EmuRec_srl,      // "srl"
    EmuRec_sra,      // "sra"
    EmuRec_sllv,     // "sllv"
    EmuRec_Invalid,  // "SPECIAL(2)"
    EmuRec_srlv,     // "srlv"
    EmuRec_srav,     // "srav"
    EmuRec_jr,       // "jr"
    EmuRec_jalr,     // "jalr"
    EmuRec_movz,     // "movz"
    EmuRec_movn,     // "movn"
    EmuRec_syscall,  // "syscall"
    EmuRec_break,    // "break"
    EmuRec_Invalid,  // "SPECIAL(3)"
    EmuRec_sync,     // "sync"
    EmuRec_mfhi,     // "mfhi"
    EmuRec_mthi,     // "mthi"
    EmuRec_mflo,     // "mflo"
    EmuRec_mtlo,     // "mtlo"
    EmuRec_dsllv,    // "dsllv"
    EmuRec_Invalid,  // "SPECIAL(4)"
    EmuRec_dsrlv,    // "dsrlv"
    EmuRec_dsrav,    // "dsrav"
    EmuRec_mult,     // "mult"
    EmuRec_multu,    // "multu"
    EmuRec_div,      // "div"
    EmuRec_divu,     // "divu"
    EmuRec_Invalid,  // "SPECIAL(DM)"
    EmuRec_Invalid,  // "SPECIAL(5)"
    EmuRec_Invalid,  // "SPECIAL(DV)"
    EmuRec_Invalid,  // "SPECIAL(DU)"
    EmuRec_add,      // "add"
    EmuRec_addu,     // "addu"
    EmuRec_sub,      // "sub"
    EmuRec_subu,     // "subu"
    EmuRec_and,      // "and"
    EmuRec_or,       // "or"
    EmuRec_xor,      // "xor"
    EmuRec_nor,      // "nor"
    EmuRec_mfsa,     // "mfsa"
    EmuRec_mtsa,     // "mtsa"
    EmuRec_slt,      // "slt"
    EmuRec_sltu,     // "sltu"
    EmuRec_dadd,     // "dadd"
    EmuRec_daddu,    // "daddu"
    EmuRec_dsub,     // "dsub"
    EmuRec_dsubu,    // "dsubu"
    EmuRec_tge,      // "tge"
    EmuRec_tgeu,     // "tgeu"
    EmuRec_tlt,      // "tlt"
    EmuRec_tltu,     // "tltu"
    EmuRec_teq,      // "teq"
    EmuRec_Invalid,  // "SPECIAL(8)"
    EmuRec_tne,      // "tne"
    EmuRec_Invalid,  // "SPECIAL(9)"
    EmuRec_dsll,     // "dsll"
    EmuRec_Invalid,  // "SPECIAL(A)"
    EmuRec_dsrl,     // "dsrl"
    EmuRec_dsra,     // "dsra"
    EmuRec_dsll32,   // "dsll32"
    EmuRec_Invalid,  // "SPECIAL(B)"
    EmuRec_dsrl32,   // "dsrl32"
    EmuRec_dsra32    // "dsra32"
};

EMURECFUNCTION EmuRecRegimmFunction[32] =
{
    EmuRec_bltz,     // "bltz"
    EmuRec_bgez,     // "bgez"
    EmuRec_bltzl,    // "bltzl"
    EmuRec_bgezl,    // "bgezl"
    EmuRec_Invalid,  // "REGIMM(1)"
    EmuRec_Invalid,  // "REGIMM(2)"
    EmuRec_Invalid,  // "REGIMM(3)"
    EmuRec_Invalid,  // "REGIMM(4)"
    EmuRec_tgei,     // "tgei"
    EmuRec_tgeiu,    // "tgeiu"
    EmuRec_tlti,     // "tlti"
    EmuRec_tltiu,    // "tltiu"
    EmuRec_teqi,     // "teqi"
    EmuRec_Invalid,  // "REGIMM(5)"
    EmuRec_tnei,     // "tnei"
    EmuRec_Invalid,  // "REGIMM(6)"
    EmuRec_bltzal,   // "bltzal"
    EmuRec_bgezal,   // "bgezal"
    EmuRec_bltzall,  // "bltzall"
    EmuRec_bgezall,  // "bgezall"
    EmuRec_Invalid,  // "REGIMM(7)"
    EmuRec_Invalid,  // "REGIMM(8)"
    EmuRec_Invalid,  // "REGIMM(9)"
    EmuRec_Invalid,  // "REGIMM(A)"
    EmuRec_mtsab,    // "mtsab"
    EmuRec_mtsah,    // "mtsah"
    EmuRec_Invalid,  // "REGIMM(B)"
    EmuRec_Invalid,  // "REGIMM(C)"
    EmuRec_Invalid,  // "REGIMM(D)"
    EmuRec_Invalid,  // "REGIMM(E)"
    EmuRec_Invalid,  // "REGIMM(F)"
    EmuRec_Invalid   // "REGIMM(10)"
};

EMURECFUNCTION EmuRecMMIFunction[64] =
{
    EmuRec_madd,     // "madd"
    EmuRec_maddu,    // "maddu"
    EmuRec_Invalid,  // "MMI(1)"
    EmuRec_Invalid,  // "MMI(2)"
    EmuRec_plzcw,    // "plzcw"
    EmuRec_Invalid,  // "MMI(3)"
    EmuRec_Invalid,  // "MMI(4)"
    EmuRec_Invalid,  // "MMI(5)"
    EmuRec_MMI_0,    // "MMI0"
    EmuRec_MMI_2,    // "MMI2"
    EmuRec_Invalid,  // "MMI(6)"
    EmuRec_Invalid,  // "MMI(7)"
    EmuRec_Invalid,  // "MMI(8)"
    EmuRec_Invalid,  // "MMI(9)"
    EmuRec_Invalid,  // "MMI(A)"
    EmuRec_Invalid,  // "MMI(B)"
    EmuRec_mfhi1,    // "mfhi1"
    EmuRec_mthi1,    // "mthi1"
    EmuRec_mflo1,    // "mflo1"
    EmuRec_mtlo1,    // "mtlo1"
    EmuRec_Invalid,  // "MMI(C)"
    EmuRec_Invalid,  // "MMI(D)"
    EmuRec_Invalid,  // "MMI(E)"
    EmuRec_Invalid,  // "MMI(F)"
    EmuRec_mult1,    // "mult1"
    EmuRec_multu1,   // "multu1"
    EmuRec_div1,     // "div1"
    EmuRec_divu1,    // "divu1"
    EmuRec_Invalid,  // "MMI(10)"
    EmuRec_Invalid,  // "MMI(11)"
    EmuRec_Invalid,  // "MMI(12)"
    EmuRec_Invalid,  // "MMI(13)"
    EmuRec_madd1,    // "madd1"
    EmuRec_maddu1,   // "maddu1"
    EmuRec_Invalid,  // "MMI(14)"
    EmuRec_Invalid,  // "MMI(15)"
    EmuRec_Invalid,  // "MMI(16)"
    EmuRec_Invalid,  // "MMI(17)"
    EmuRec_Invalid,  // "MMI(18)"
    EmuRec_Invalid,  // "MMI(19)"
    EmuRec_MMI_1,    // "MMI1"
    EmuRec_MMI_3,    // "MMI3"
    EmuRec_Invalid,  // "MMI(1A)"
    EmuRec_Invalid,  // "MMI(1B)"
    EmuRec_Invalid,  // "MMI(1C)"
    EmuRec_Invalid,  // "MMI(1D)"
    EmuRec_Invalid,  // "MMI(1E)"
    EmuRec_Invalid,  // "MMI(1F)"
    EmuRec_pmfhl,    // "pmfhl"
    EmuRec_pmthl,    // "pmthl"
    EmuRec_Invalid,  // "MMI(20)"
    EmuRec_Invalid,  // "MMI(21)"
    EmuRec_psllh,    // "psllh"
    EmuRec_Invalid,  // "MMI(22)"
    EmuRec_psrlh,    // "psrlh"
    EmuRec_psrah,    // "psrah"
    EmuRec_Invalid,  // "MMI(23)"
    EmuRec_Invalid,  // "MMI(24)"
    EmuRec_Invalid,  // "MMI(25)"
    EmuRec_Invalid,  // "MMI(26)"
    EmuRec_psllw,    // "psllw"
    EmuRec_Invalid,  // "MMI(27)"
    EmuRec_psrlw,    // "psrlw"
    EmuRec_psraw     // "psraw"
};

EMURECFUNCTION EmuRecMMI_0Function[32] =
{
    EmuRec_paddw,    // "paddw"
    EmuRec_psubw,    // "psubw"
    EmuRec_pcgtw,    // "pcgtw"
    EmuRec_pmaxw,    // "pmaxw"
    EmuRec_paddh,    // "paddh"
    EmuRec_psubh,    // "psubh"
    EmuRec_pcgth,    // "pcgth"
    EmuRec_pmaxh,    // "pmaxh"
    EmuRec_paddb,    // "paddb"
    EmuRec_psubb,    // "psubb"
    EmuRec_pcgtb,    // "pcgtb"
    EmuRec_Invalid,  // "MMI0(1)"
    EmuRec_Invalid,  // "MMI0(2)"
    EmuRec_Invalid,  // "MMI0(3)"
    EmuRec_Invalid,  // "MMI0(4)"
    EmuRec_Invalid,  // "MMI0(5)"
    EmuRec_paddsw,   // "paddsw"
    EmuRec_psubsw,   // "psubsw"
    EmuRec_pextlw,   // "pextlw"
    EmuRec_ppacw,    // "ppacw"
    EmuRec_paddsh,   // "paddsh"
    EmuRec_psubsh,   // "psubsh"
    EmuRec_pextlh,   // "pextlh"
    EmuRec_ppach,    // "ppach"
    EmuRec_paddsb,   // "paddsb"
    EmuRec_psubsb,   // "psubsb"
    EmuRec_pextlb,   // "pextlb"
    EmuRec_ppacb,    // "ppacb"
    EmuRec_Invalid,  // "MMI0(6)"
    EmuRec_Invalid,  // "MMI0(7)"
    EmuRec_pext5,    // "pext5"
    EmuRec_ppac5     // "ppac5"
};

EMURECFUNCTION EmuRecMMI_1Function[32] =
{
    EmuRec_Invalid,  // "MMI1(1)"
    EmuRec_pabsw,    // "pabsw"
    EmuRec_pceqw,    // "pceqw"
    EmuRec_pminw,    // "pminw"
    EmuRec_padsbh,   // "padsbh"
    EmuRec_pabsh,    // "pabsh"
    EmuRec_pceqh,    // "pceqh"
    EmuRec_pminh,    // "pminh"
    EmuRec_Invalid,  // "MMI1(2)"
    EmuRec_Invalid,  // "MMI1(3)"
    EmuRec_pceqb,    // "pceqb"
    EmuRec_Invalid,  // "MMI1(4)"
    EmuRec_Invalid,  // "MMI1(5)"
    EmuRec_Invalid,  // "MMI1(6)"
    EmuRec_Invalid,  // "MMI1(7)"
    EmuRec_Invalid,  // "MMI1(8)"
    EmuRec_padduw,   // "padduw"
    EmuRec_psubuw,   // "psubuw"
    EmuRec_pextuw,   // "pextuw"
    EmuRec_Invalid,  // "MMI1(9)"
    EmuRec_padduh,   // "padduh"
    EmuRec_psubuh,   // "psubuh"
    EmuRec_pextuh,   // "pextuh"
    EmuRec_Invalid,  // "MMI1(A)"
    EmuRec_paddub,   // "paddub"
    EmuRec_psubub,   // "psubub"
    EmuRec_pextub,   // "pextub"
    EmuRec_qfsrv,    // "qfsrv"
    EmuRec_Invalid,  // "MMI1(B)"
    EmuRec_Invalid,  // "MMI1(C)"
    EmuRec_Invalid,  // "MMI1(D)"
    EmuRec_Invalid   // "MMI1(E)"
};

EMURECFUNCTION EmuRecMMI_2Function[32] =
{
    EmuRec_pmaddw,   // "pmaddw"
    EmuRec_Invalid,  // "MMI2(1)"
    EmuRec_psllvw,   // "psllvw"
    EmuRec_psrlvw,   // "psrlvw"
    EmuRec_pmsubw,   // "pmsubw"
    EmuRec_Invalid,  // "MMI2(2)"
    EmuRec_Invalid,  // "MMI2(3)"
    EmuRec_Invalid,  // "MMI2(4)"
    EmuRec_pmfhi,    // "pmfhi"
    EmuRec_pmflo,    // "pmflo"
    EmuRec_pinth,    // "pinth"
    EmuRec_Invalid,  // "MMI2(5)"
    EmuRec_pmultw,   // "pmultw"
    EmuRec_pdivw,    // "pdivw"
    EmuRec_pcpyld,   // "pcpyld"
    EmuRec_Invalid,  // "MMI2(6)"
    EmuRec_pmaddh,   // "pmaddh"
    EmuRec_phmadh,   // "phmadh"
    EmuRec_pand,     // "pand"
    EmuRec_pxor,     // "pxor"
    EmuRec_pmsubh,   // "pmsubh"
    EmuRec_phmsbh,   // "phmsbh"
    EmuRec_Invalid,  // "MMI2(7)"
    EmuRec_Invalid,  // "MMI2(8)"
    EmuRec_Invalid,  // "MMI2(9)"
    EmuRec_Invalid,  // "MMI2(A)"
    EmuRec_pexeh,    // "pexeh"
    EmuRec_prevh,    // "prevh"
    EmuRec_pmulth,   // "pmulth"
    EmuRec_pdivbw,   // "pdivbw"
    EmuRec_pexew,    // "pexew"
    EmuRec_prot3w    // "prot3w"
};

EMURECFUNCTION EmuRecMMI_3Function[32] =
{
    EmuRec_pmadduw,  // "pmadduw"
    EmuRec_Invalid,  // "MMI3(1)"
    EmuRec_Invalid,  // "MMI3(2)"
    EmuRec_psravw,   // "psravw"
    EmuRec_Invalid,  // "MMI3(3)"
    EmuRec_Invalid,  // "MMI3(4)"
    EmuRec_Invalid,  // "MMI3(5)"
    EmuRec_Invalid,  // "MMI3(6)"
    EmuRec_pmthi,    // "pmthi"
    EmuRec_pmtlo,    // "pmtlo"
    EmuRec_pinteh,   // "pinteh",
    EmuRec_Invalid,  // "MMI3(7)"
    EmuRec_pmultuw,  // "pmultuw"
    EmuRec_pdivuw,   // "pdivuw"
    EmuRec_pcpyud,   // "pcpyud"
    EmuRec_Invalid,  // "MMI3(8)"
    EmuRec_Invalid,  // "MMI3(9)"
    EmuRec_Invalid,  // "MMI3(A)"
    EmuRec_por,      // "por"
    EmuRec_pnor,     // "pnor"
    EmuRec_Invalid,  // "MMI3(B)"
    EmuRec_Invalid,  // "MMI3(C)"
    EmuRec_Invalid,  // "MMI3(D)"
    EmuRec_Invalid,  // "MMI3(E)"
    EmuRec_Invalid,  // "MMI3(F)"
    EmuRec_Invalid,  // "MMI3(10)"
    EmuRec_pexch,    // "pexch"
    EmuRec_pcpyh,    // "pcpyh"
    EmuRec_Invalid,  // "MMI3(11)"
    EmuRec_Invalid,  // "MMI3(12)"
    EmuRec_pexcw,    // "pexcw"
    EmuRec_Invalid   // "MMI3(13)"
};

EMURECFUNCTION EmuRecCOP0Function[32] =
{
    EmuRec_mfc0,     // "mfc0"
    EmuRec_Invalid,  // "COP0(1)"
    EmuRec_Invalid,  // "COP0(2)"
    EmuRec_Invalid,  // "COP0(3)"
    EmuRec_mtc0,     // "mtc0"
    EmuRec_Invalid,  // "COP0(4)"
    EmuRec_Invalid,  // "COP0(5)"
    EmuRec_Invalid,  // "COP0(6)"
    EmuRec_COP0_BC0, // "BC0"
    EmuRec_Invalid,  // "COP0(7)"
    EmuRec_Invalid,  // "COP0(8)"
    EmuRec_Invalid,  // "COP0(9)"
    EmuRec_Invalid,  // "COP0(A)"
    EmuRec_Invalid,  // "COP0(B)"
    EmuRec_Invalid,  // "COP0(C)"
    EmuRec_Invalid,  // "COP0(D)"
    EmuRec_COP0_C0,  // "C0"
    EmuRec_Invalid,  // "COP0(E)"
    EmuRec_Invalid,  // "COP0(F)"
    EmuRec_Invalid,  // "COP0(10)"
    EmuRec_Invalid,  // "COP0(11)"
    EmuRec_Invalid,  // "COP0(12)"
    EmuRec_Invalid,  // "COP0(13)"
    EmuRec_Invalid,  // "COP0(14)"
    EmuRec_Invalid,  // "COP0(15)"
    EmuRec_Invalid,  // "COP0(16)"
    EmuRec_Invalid,  // "COP0(17)"
    EmuRec_Invalid,  // "COP0(18)"
    EmuRec_Invalid,  // "COP0(19)"
    EmuRec_Invalid,  // "COP0(1A)"
    EmuRec_Invalid,  // "COP0(1B)"
    EmuRec_Invalid   // "COP0(1C)"
};

EMURECFUNCTION EmuRecCOP0_BC0Function[32] =
{
    EmuRec_bc0f,     // "bc0f"
    EmuRec_bc0t,     // "bc0t"
    EmuRec_bc0fl,    // "bc0fl"
    EmuRec_bc0tl,    // "bc0tl"
    EmuRec_Invalid,  // "BC0(1)"
    EmuRec_Invalid,  // "BC0(2)"
    EmuRec_Invalid,  // "BC0(3)"
    EmuRec_Invalid,  // "BC0(4)"
    EmuRec_Invalid,  // "BC0(5)"
    EmuRec_Invalid,  // "BC0(6)"
    EmuRec_Invalid,  // "BC0(7)"
    EmuRec_Invalid,  // "BC0(8)"
    EmuRec_Invalid,  // "BC0(9)"
    EmuRec_Invalid,  // "BC0(A)"
    EmuRec_Invalid,  // "BC0(B)"
    EmuRec_Invalid,  // "BC0(C)"
    EmuRec_Invalid,  // "BC0(D)"
    EmuRec_Invalid,  // "BC0(E)"
    EmuRec_Invalid,  // "BC0(F)"
    EmuRec_Invalid,  // "BC0(10)"
    EmuRec_Invalid,  // "BC0(11)"
    EmuRec_Invalid,  // "BC0(12)"
    EmuRec_Invalid,  // "BC0(13)"
    EmuRec_Invalid,  // "BC0(14)"
    EmuRec_Invalid,  // "BC0(15)"
    EmuRec_Invalid,  // "BC0(16)"
    EmuRec_Invalid,  // "BC0(17)"
    EmuRec_Invalid,  // "BC0(18)"
    EmuRec_Invalid,  // "BC0(19)"
    EmuRec_Invalid,  // "BC0(1A)"
    EmuRec_Invalid,  // "BC0(1B)"
    EmuRec_Invalid   // "BC0(1C)"
};

EMURECFUNCTION EmuRecCOP0_C0Function[64] =
{
    EmuRec_Invalid,  // "C0(1)"
    EmuRec_tlbr,     // "tlbr"
    EmuRec_tlbwi,    // "tlbwi"
    EmuRec_Invalid,  // "C0(2)"
    EmuRec_Invalid,  // "C0(3)"
    EmuRec_Invalid,  // "C0(4)"
    EmuRec_tlbwr,    // "tlbwr"
    EmuRec_Invalid,  // "C0(5)"
    EmuRec_tlbp,     // "tlbp"
    EmuRec_Invalid,  // "C0(6)"
    EmuRec_Invalid,  // "C0(7)"
    EmuRec_Invalid,  // "C0(8)"
    EmuRec_Invalid,  // "C0(9)"
    EmuRec_Invalid,  // "C0(A)"
    EmuRec_Invalid,  // "C0(B)"
    EmuRec_Invalid,  // "C0(C)"
    EmuRec_Invalid,  // "C0(D)"
    EmuRec_Invalid,  // "C0(E)"
    EmuRec_Invalid,  // "C0(F)"
    EmuRec_Invalid,  // "C0(10)"
    EmuRec_Invalid,  // "C0(11)"
    EmuRec_Invalid,  // "C0(12)"
    EmuRec_Invalid,  // "C0(13)"
    EmuRec_Invalid,  // "C0(14)"
    EmuRec_eret,     // "eret"
    EmuRec_Invalid,  // "C0(15)"
    EmuRec_Invalid,  // "C0(16)"
    EmuRec_Invalid,  // "C0(17)"
    EmuRec_Invalid,  // "C0(18)"
    EmuRec_Invalid,  // "C0(19)"
    EmuRec_Invalid,  // "C0(1A)"
    EmuRec_Invalid,  // "C0(1B)"
    EmuRec_Invalid,  // "C0(1C)"
    EmuRec_Invalid,  // "C0(1D)"
    EmuRec_Invalid,  // "C0(1E)"
    EmuRec_Invalid,  // "C0(1F)"
    EmuRec_Invalid,  // "C0(20)"
    EmuRec_Invalid,  // "C0(21)"
    EmuRec_Invalid,  // "C0(22)"
    EmuRec_Invalid,  // "C0(23)"
    EmuRec_Invalid,  // "C0(24)"
    EmuRec_Invalid,  // "C0(25)"
    EmuRec_Invalid,  // "C0(26)"
    EmuRec_Invalid,  // "C0(27)"
    EmuRec_Invalid,  // "C0(28)"
    EmuRec_Invalid,  // "C0(29)"
    EmuRec_Invalid,  // "C0(2A)"
    EmuRec_Invalid,  // "C0(2B)"
    EmuRec_Invalid,  // "C0(2C)"
    EmuRec_Invalid,  // "C0(2D)"
    EmuRec_Invalid,  // "C0(2E)"
    EmuRec_Invalid,  // "C0(2F)"
    EmuRec_Invalid,  // "C0(30)"
    EmuRec_Invalid,  // "C0(31)"
    EmuRec_Invalid,  // "C0(32)"
    EmuRec_Invalid,  // "C0(33)"
    EmuRec_ei,       // "ei"
    EmuRec_di,       // "di"
    EmuRec_Invalid,  // "C0(34)"
    EmuRec_Invalid,  // "C0(35)"
    EmuRec_Invalid,  // "C0(36)"
    EmuRec_Invalid,  // "C0(37)"
    EmuRec_Invalid,  // "C0(38)"
    EmuRec_Invalid   // "C0(39)"
};

EMURECFUNCTION EmuRecCOP1Function[32] =
{
    EmuRec_mfc1,     // "mfc1"
    EmuRec_Invalid,  // "COP1(1)"
    EmuRec_cfc1,     // "cfc1"
    EmuRec_Invalid,  // "COP1(2)"
    EmuRec_mtc1,     // "mtc1"
    EmuRec_Invalid,  // "COP1(3)"
    EmuRec_ctc1,     // "ctc1"
    EmuRec_Invalid,  // "COP1(4)"
    EmuRec_COP1_BC1, // "BC1"
    EmuRec_Invalid,  // "COP1(5)"
    EmuRec_Invalid,  // "COP1(6)"
    EmuRec_Invalid,  // "COP1(7)"
    EmuRec_Invalid,  // "COP1(8)"
    EmuRec_Invalid,  // "COP1(9)"
    EmuRec_Invalid,  // "COP1(A)"
    EmuRec_Invalid,  // "COP1(B)"
    EmuRec_COP1_S,   // "S"
    EmuRec_Invalid,  // "COP1(C)"
    EmuRec_Invalid,  // "COP1(D)"
    EmuRec_Invalid,  // "COP1(E)"
    EmuRec_COP1_W,   // "W"
    EmuRec_Invalid,  // "COP1(F)"
    EmuRec_Invalid,  // "COP1(10)"
    EmuRec_Invalid,  // "COP1(11)"
    EmuRec_Invalid,  // "COP1(12)"
    EmuRec_Invalid,  // "COP1(13)"
    EmuRec_Invalid,  // "COP1(14)"
    EmuRec_Invalid,  // "COP1(15)"
    EmuRec_Invalid,  // "COP1(16)"
    EmuRec_Invalid,  // "COP1(17)"
    EmuRec_Invalid,  // "COP1(18)"
    EmuRec_Invalid   // "COP1(19)"
};

EMURECFUNCTION EmuRecCOP1_BC1Function[32] =
{
    EmuRec_bc1f,     // "bc1f"
    EmuRec_bc1t,     // "bc1t"
    EmuRec_bc1fl,    // "bc1fl"
    EmuRec_bc1tl,    // "bc1tl"
    EmuRec_Invalid,  // "BC1(1)"
    EmuRec_Invalid,  // "BC1(2)"
    EmuRec_Invalid,  // "BC1(3)"
    EmuRec_Invalid,  // "BC1(4)"
    EmuRec_Invalid,  // "BC1(5)"
    EmuRec_Invalid,  // "BC1(6)"
    EmuRec_Invalid,  // "BC1(7)"
    EmuRec_Invalid,  // "BC1(8)"
    EmuRec_Invalid,  // "BC1(9)"
    EmuRec_Invalid,  // "BC1(A)"
    EmuRec_Invalid,  // "BC1(B)"
    EmuRec_Invalid,  // "BC1(C)"
    EmuRec_Invalid,  // "BC1(D)"
    EmuRec_Invalid,  // "BC1(E)"
    EmuRec_Invalid,  // "BC1(F)"
    EmuRec_Invalid,  // "BC1(10)"
    EmuRec_Invalid,  // "BC1(11)"
    EmuRec_Invalid,  // "BC1(12)"
    EmuRec_Invalid,  // "BC1(13)"
    EmuRec_Invalid,  // "BC1(14)"
    EmuRec_Invalid,  // "BC1(15)"
    EmuRec_Invalid,  // "BC1(16)"
    EmuRec_Invalid,  // "BC1(17)"
    EmuRec_Invalid,  // "BC1(18)"
    EmuRec_Invalid,  // "BC1(19)"
    EmuRec_Invalid,  // "BC1(1A)"
    EmuRec_Invalid,  // "BC1(1B)"
    EmuRec_Invalid   // "BC1(1C)"
};

EMURECFUNCTION EmuRecCOP1_SFunction[64] =
{
    EmuRec_add_s,    // "add.s"
    EmuRec_sub_s,    // "sub.s"
    EmuRec_mul_s,    // "mul.s"
    EmuRec_div_s,    // "div.s"
    EmuRec_sqrt_s,   // "sqrt.s"
    EmuRec_abs_s,    // "abs.s"
    EmuRec_mov_s,    // "mov.s"
    EmuRec_neg_s,    // "neg.s"
    EmuRec_Invalid,  // "S(1)"
    EmuRec_Invalid,  // "S(2)"
    EmuRec_Invalid,  // "S(3)"
    EmuRec_Invalid,  // "S(4)"
    EmuRec_Invalid,  // "S(5)"
    EmuRec_Invalid,  // "S(6)"
    EmuRec_Invalid,  // "S(7)"
    EmuRec_Invalid,  // "S(8)"
    EmuRec_Invalid,  // "S(9)"
    EmuRec_Invalid,  // "S(A)"
    EmuRec_Invalid,  // "S(B)"
    EmuRec_Invalid,  // "S(C)"
    EmuRec_Invalid,  // "S(D)"
    EmuRec_Invalid,  // "S(E)"
    EmuRec_rsqrt_s,  // "rsqrt.s"
    EmuRec_Invalid,  // "S(F)"
    EmuRec_adda_s,   // "adda.s"
    EmuRec_suba_s,   // "suba.s"
    EmuRec_mula_s,   // "mula.s"
    EmuRec_Invalid,  // "S(10)"
    EmuRec_madd_s,   // "madd.s"
    EmuRec_msub_s,   // "msub.s"
    EmuRec_madda_s,  // "madda.s"
    EmuRec_msuba_s,  // "msuba.s"
    EmuRec_Invalid,  // "S(11)"
    EmuRec_Invalid,  // "S(12)"
    EmuRec_Invalid,  // "S(13)"
    EmuRec_Invalid,  // "S(14)"
    EmuRec_cvt_w_s,  // "cvt.w.s"
    EmuRec_Invalid,  // "S(15)"
    EmuRec_Invalid,  // "S(16)"
    EmuRec_Invalid,  // "S(17)"
    EmuRec_max_s,    // "max.s"
    EmuRec_min_s,    // "min.s"
    EmuRec_Invalid,  // "S(18)"
    EmuRec_Invalid,  // "S(19)"
    EmuRec_Invalid,  // "S(1A)"
    EmuRec_Invalid,  // "S(1B)"
    EmuRec_Invalid,  // "S(1C)"
    EmuRec_Invalid,  // "S(1D)"
    EmuRec_c_f_s,    // "c.f.s"
    EmuRec_Invalid,  // "S(1E)"
    EmuRec_c_eq_s,   // "c.eq.s"
    EmuRec_Invalid,  // "S(1F)"
    EmuRec_c_lt_s,   // "c.lt.s"
    EmuRec_Invalid,  // "S(20)"
    EmuRec_c_le_s,   // "c.le.s"
    EmuRec_Invalid,  // "S(21)"
    EmuRec_Invalid,  // "S(22)"
    EmuRec_Invalid,  // "S(23)"
    EmuRec_Invalid,  // "S(24)"
    EmuRec_Invalid,  // "S(25)"
    EmuRec_Invalid,  // "S(26)"
    EmuRec_Invalid,  // "S(27)"
    EmuRec_Invalid,  // "S(28)"
    EmuRec_Invalid   // "S(29)"
};

EMURECFUNCTION EmuRecCOP1_WFunction[64] =
{
    EmuRec_Invalid,  // "W(1)"
    EmuRec_Invalid,  // "W(2)"
    EmuRec_Invalid,  // "W(3)"
    EmuRec_Invalid,  // "W(4)"
    EmuRec_Invalid,  // "W(5)"
    EmuRec_Invalid,  // "W(6)"
    EmuRec_Invalid,  // "W(7)"
    EmuRec_Invalid,  // "W(8)"
    EmuRec_Invalid,  // "W(9)"
    EmuRec_Invalid,  // "W(A)"
    EmuRec_Invalid,  // "W(B)"
    EmuRec_Invalid,  // "W(C)"
    EmuRec_Invalid,  // "W(D)"
    EmuRec_Invalid,  // "W(E)"
    EmuRec_Invalid,  // "W(F)"
    EmuRec_Invalid,  // "W(10)"
    EmuRec_Invalid,  // "W(11)"
    EmuRec_Invalid,  // "W(12)"
    EmuRec_Invalid,  // "W(13)"
    EmuRec_Invalid,  // "W(14)"
    EmuRec_Invalid,  // "W(15)"
    EmuRec_Invalid,  // "W(16)"
    EmuRec_Invalid,  // "W(17)"
    EmuRec_Invalid,  // "W(18)"
    EmuRec_Invalid,  // "W(19)"
    EmuRec_Invalid,  // "W(1A)"
    EmuRec_Invalid,  // "W(1B)"
    EmuRec_Invalid,  // "W(1C)"
    EmuRec_Invalid,  // "W(1D)"
    EmuRec_Invalid,  // "W(1E)"
    EmuRec_Invalid,  // "W(1F)"
    EmuRec_Invalid,  // "W(20)"
    EmuRec_cvt_s_w,  // "cvt.s.w"
    EmuRec_Invalid,  // "W(21)"
    EmuRec_Invalid,  // "W(22)"
    EmuRec_Invalid,  // "W(23)"
    EmuRec_Invalid,  // "W(24)"
    EmuRec_Invalid,  // "W(25)"
    EmuRec_Invalid,  // "W(26)"
    EmuRec_Invalid,  // "W(27)"
    EmuRec_Invalid,  // "W(28)"
    EmuRec_Invalid,  // "W(29)"
    EmuRec_Invalid,  // "W(2A)"
    EmuRec_Invalid,  // "W(2B)"
    EmuRec_Invalid,  // "W(2C)"
    EmuRec_Invalid,  // "W(2D)"
    EmuRec_Invalid,  // "W(2E)"
    EmuRec_Invalid,  // "W(2F)"
    EmuRec_Invalid,  // "W(30)"
    EmuRec_Invalid,  // "W(31)"
    EmuRec_Invalid,  // "W(32)"
    EmuRec_Invalid,  // "W(33)"
    EmuRec_Invalid,  // "W(34)"
    EmuRec_Invalid,  // "W(35)"
    EmuRec_Invalid,  // "W(36)"
    EmuRec_Invalid,  // "W(37)"
    EmuRec_Invalid,  // "W(38)"
    EmuRec_Invalid,  // "W(39)"
    EmuRec_Invalid,  // "W(3A)"
    EmuRec_Invalid,  // "W(3B)"
    EmuRec_Invalid,  // "W(3C)"
    EmuRec_Invalid,  // "W(3D)"
    EmuRec_Invalid,  // "W(3E)"
    EmuRec_Invalid   // "W(3F)"
};

/*
/ COP2 Instruct
/ -> COP2 MACRO
 "bc2f",       
 "bc2t",       
 "bc2fl",      
 "bc2tl",      
 "qmfc2",      
 "cfc2",       
 "qmtc2",      
 "ctc2",       

 "vaddx",      
 "vaddy",      
 "vaddz",      
 "vaddw",      
 "vsubx",      
 "vsuby",      
 "vsubz",      
 "vsubw",      
 "vmaddx",     
 "vmaddy",     
 "vmaddz",     
 "vmaddw",     
 "vmsubx",     
 "vmsuby",     
 "vmsubz",     
 "vmsubw",     
 "vmaxx",      
 "vmaxy",      
 "vmaxz",      
 "vmaxw",      
 "vminix",     
 "vminiy",     
 "vminiz",     
 "vminiw",     
 "vmulx",      
 "vmuly",      
 "vmulz",      
 "vmulw",      
 "vmulq",      
 "vmaxi",      
 "vmuli",      
 "vminii",     
 "vaddq",      
 "vmaddq",     
 "vaddi",      
 "vmaddi",     
 "vsubq",      
 "vmsubq",     
 "vsubi",      
 "vmsubi",     
 "vadd",       
 "vmadd",      
 "vmul",       
 "vmax",       
 "vsub",       
 "vmsub",      
 "vopmsub",    
 "vmini",      
 "viadd",      
 "visub",      
 "viaddi",     
 "viand",      
 "vior",       
 "vcallms",    
 "vcallmsr",   
 "vaddax",     
 "vadday",     
 "vaddaz",     
 "vaddaw",     
 "vsubax",     
 "vsubay",     
 "vsubaz",     
 "vsubaw",     
 "vmaddax",    
 "vmadday",    
 "vmaddaz",    
 "vmaddaw",    
 "vmsubax",    
 "vmsubay",    
 "vmsubaz",    
 "vmsubaw",    
 "vitof0",     
 "vitof4",     
 "vitof12",    
 "vitof15",    
 "vftoi0",     
 "vftoi4",     
 "vftoi12",    
 "vftoi15",    
 "vmulax",     
 "vmulay",     
 "vmulaz",     
 "vmulaw",     
 "vmulaq",     
 "vabs",       
 "vmulai",     
 "vclipw.xyz", 
 "vaddaq",     
 "vmaddaq",    
 "vaddai",     
 "vmaddai",    
 "vsubaq",     
 "vmsubaq",    
 "vsubai",     
 "vmsubai",    
 "vadda",      
 "vmadda",     
 "vmula",		
 "vsuba",      
 "vmsuba",     
 "vopmula",    
 "vnop",       
 "vmove",      
 "vmr32",      
 "vlqi",       
 "vsqi",       
 "vlqd",       
 "vsqd",       
 "vdiv",       
 "vsqrt",      
 "vrsqrt",     
 "vwaitq",     
 "vmtir",      
 "vmfir",      
 "vilwr",      
 "viswr",      
 "vrnext",     
 "vrget",      
 "vrinit",     
*/