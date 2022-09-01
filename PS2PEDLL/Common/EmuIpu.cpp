
#include "EmuMain.h"
#include "EmuIpu.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EMU_U08 EmuIpuMemory[ EMU_IPU_END_ADDR - EMU_IPU_START_ADDR ];

// Pointer to control1 registers
stEmu_Ipu_Control_Regs * Emu_Ipu_Control_Reg;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Ipu_Init( void )
{
//    Emu_Ipu_Control_Reg = (stEmu_Ipu_Control_Regs *) EMemory.GetRealPointer( EMU_IPU );
    Emu_Ipu_Control_Reg = (stEmu_Ipu_Control_Regs *) EmuIpuMemory;

    Emu_Ipu_Reset( );
}

void Emu_Ipu_Reset( void )
{
    memset( Emu_Ipu_Control_Reg, 0, sizeof( stEmu_Ipu_Control_Regs ) );
}

void Emu_Ipu_callback( EMU_U32 Address )
{
    EMU_U32 Register = Address - EMU_IPU_START_ADDR;

    switch ( Register )
    {
    case EMU_IPU_CMD:
    case EMU_IPU_CTRL:
    case EMU_IPU_BP:
    case EMU_IPU_TOP:
        break;
    }
}

EMU_U08 * Emu_Ipu_GetPointer( EMU_U32 Address )
{
    return &EmuIpuMemory[ Address - EMU_IPU_START_ADDR ];
}
