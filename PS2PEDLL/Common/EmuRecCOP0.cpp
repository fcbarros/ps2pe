
#include "EmuMain.h"
#include "EmuRecompiler.h"

void EmuRec_mfc0( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I32)COP0Regs.Reg[ R_RD ];
*/
}

void EmuRec_mtc0( EMU_U32 Code )
{
/*
    COP0Regs.Reg[ R_RD ] = R5900Regs.Reg[ R_RT ].u32_00_31;
*/
}

void EmuRec_bc0f( EMU_U32 Code )
{
/*
   // Branch if there is no DMA running, always for now
   {
       EXECUTE_BRANCH( R5900Regs.PC );
   }
*/
}

void EmuRec_bc0t( EMU_U32 Code )
{
}

void EmuRec_bc0fl( EMU_U32 Code )
{
/*
   // Branch if there is no DMA running, always for now
   {
       EXECUTE_BRANCH( R5900Regs.PC );
   }
*/
}

void EmuRec_bc0tl( EMU_U32 Code )
{
/*
    R5900Regs.PC += 4;
*/
}

void EmuRec_tlbr( EMU_U32 Code )
{
}

void EmuRec_tlbwi( EMU_U32 Code )
{
}

void EmuRec_tlbwr( EMU_U32 Code )
{
}

void EmuRec_tlbp( EMU_U32 Code )
{
}

void EmuRec_eret( EMU_U32 Code )
{
/*
    if ( COP0Regs.Status_ERL  == 1 ) 
    {
        COP0Regs.Status_ERL = 0;
        R5900Regs.PC = COP0Regs.Error_EPC;
    }
    else
    {
        COP0Regs.Status_ERL = 1;
        R5900Regs.PC = COP0Regs.EPC;
    }
*/
}

void EmuRec_ei( EMU_U32 Code )
{
/*
    if ( ( COP0Regs.Status_EDI == 1 ) || 
         ( COP0Regs.Status_EXL == 1 ) ||
         ( COP0Regs.Status_ERL == 1 ) ||
         ( COP0Regs.Status_KSU == 0 ) )
    {
        COP0Regs.Status_EIE = 1;
    }
*/
}

void EmuRec_di( EMU_U32 Code )
{
/*
    if ( ( COP0Regs.Status_EDI == 1 ) || 
         ( COP0Regs.Status_EXL == 1 ) ||
         ( COP0Regs.Status_ERL == 1 ) ||
         ( COP0Regs.Status_KSU == 0 ) )
    {
        COP0Regs.Status_EIE = 0;
    }
*/
}

