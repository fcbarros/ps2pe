
#include "EmuMain.h"
#include "EmuIntc.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EMU_U08 EmuIntcMemory[ EMU_INTC_END_ADDR - EMU_INTC_START_ADDR ];

// Pointer to control1 registers
stEmu_Intc_Control_Regs * Emu_Intc_Control_Reg;

stEMU_INTC_HANDLER Emu_Intc_Handler[ 32 ];

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Intc_Init( void )
{
//    Emu_Intc_Control_Reg = (stEmu_Intc_Control_Regs *) EMemory.GetRealPointer( EMU_INTC_START_ADDR );
    Emu_Intc_Control_Reg = (stEmu_Intc_Control_Regs *) EmuIntcMemory;

    Emu_Intc_Reset( );
}

void Emu_Intc_Reset( void )
{
    memset( EmuIntcMemory, 0, sizeof( EmuIntcMemory ) );

    Emu_Intc_Control_Reg->STAT =     0x00000000;
    Emu_Intc_Control_Reg->MASK =     0x00000000;

    memset( Emu_Intc_Handler, 0, sizeof( Emu_Intc_Handler ) );

    EmuInterruptIndex = 0;
}

void Emu_Intc_callback( EMU_U32 Address )
{
    EMU_U32 Register = Address - EMU_INTC_START_ADDR;

    switch ( Register )
    {
    case EMU_INTC_STAT:
    case EMU_INTC_MASK:
        break;
    }
    if ( Address == 0x1000F180 )
    {
        EmuConsole( "%c", EMemory.GetByte( Address ) );
    }
}

void Emu_Intc_Bios_AddHandler( void ) 
{ // BiosCall 0x10
#ifdef EMU_LOG
    EmuLog( "  Handler: %u, Address: %.8X\n", R5900Regs.A0.u32_00_31, R5900Regs.A1.u32_00_31 );
#endif
	Emu_Intc_AddHandler( R5900Regs.A0.u32_00_31, R5900Regs.A1.u32_00_31 );

	R5900Regs.V0.u64_00_63 = R5900Regs.A0.u32_00_31;
}

void Emu_Intc_Bios_RemoveHandler( void )
{ // 0x11
#ifdef EMU_LOG
    EmuLog( "  Handler: %u\n", R5900Regs.A0.u32_00_31 );
#endif
	Emu_Intc_RemoveHandler( R5900Regs.A0.u32_00_31 );
	R5900Regs.V0.u64_00_63 = 1;
}

void Emu_Intc_Bios_EnableHandler( void )
{ // 0x14
#ifdef EMU_LOG
    EmuLog( "  Handler: %u\n", R5900Regs.A0.u32_00_31 );
#endif
	Emu_Intc_EnableHandler( R5900Regs.A0.u32_00_31 );
	R5900Regs.V0.u64_00_63 = 0;
}

void Emu_Intc_Bios_DisableHandler( void )
{ // 0x15
#ifdef EMU_LOG
    EmuLog( "  Handler: %u\n", R5900Regs.A0.u32_00_31 );
#endif
	Emu_Intc_DisableHandler( R5900Regs.A0.u32_00_31 );
	R5900Regs.V0.u64_00_63 = 1;
}

void Emu_Intc_AddHandler( EMU_U32 HandlerIndex, EMU_U32 FuncAddress ) 
{ // BiosCall 0x10
    Emu_Intc_Handler[ HandlerIndex ].active = 0;
    Emu_Intc_Handler[ HandlerIndex ].id = HandlerIndex;
    Emu_Intc_Handler[ HandlerIndex ].func = FuncAddress;
}

void Emu_Intc_RemoveHandler( EMU_U32 HandlerIndex )
{ // 0x11
	Emu_Intc_Handler[ HandlerIndex ].active = 0;
	Emu_Intc_Handler[ HandlerIndex ].id = 0;
	Emu_Intc_Handler[ HandlerIndex ].func = NULL;
}

void Emu_Intc_EnableHandler( EMU_U32 HandlerIndex )
{ // 0x14
    Emu_Intc_Handler[ HandlerIndex ].active = 1;
}

void Emu_Intc_DisableHandler( EMU_U32 HandlerIndex )
{ // 0x15
#ifdef EMU_LOG
    EmuLog( "  Handler: %u\n", HandlerIndex );
#endif
    Emu_Intc_Handler[ HandlerIndex ].active = 0;
}

void Emu_Intc_Level1( EMU_U32 ExecCode )
{
   COP0Regs.Cause_ExcCode = ExecCode;

   if ( EmuInBranchDelay )
   {
      COP0Regs.EPC = R5900Regs.PC - 4;
      COP0Regs.Cause_BD = 1;
   }
   else
   {
      COP0Regs.EPC = R5900Regs.PC;
      COP0Regs.Cause_BD = 0;
   }
   COP0Regs.Status_EXL = 1;
}

void Emu_Intc_CallHandler( EMU_U32 HandlerIndex )
{
    if ( EmuInterruptIndex > 0 )
    {
        Emu_Intc_ExitHandler( );
//        return;
    }
    if ( ( Emu_Intc_Handler[ HandlerIndex ].active ) &&
         ( COP0Regs.Status_EIE ) )
    {
        switch ( HandlerIndex )
        {
        case 2:
        case 3:
            if ( Emu_Intc_Control_Reg->STAT_GS & ~Emu_Intc_Control_Reg->MASK )
            {
                memcpy( &R5900RegsBackup[ EmuInterruptIndex ], &R5900Regs, sizeof( R5900Regs ) );
//                memcpy( &COP0RegsBackup, &COP0Regs, sizeof( COP0Regs ) );
//                memcpy( &COP1RegsBackup, &COP1Regs, sizeof( COP1Regs ) );

                R5900Regs.Reg[31].u64_00_63 = (EMU_I32)0xFFFFFFFF;
                R5900Regs.PC = Emu_Intc_Handler[ HandlerIndex ].func;
                EmuInterruptIndex++; 
            }
        }
    }
}

void Emu_Intc_ExitHandler( void )
{
    if ( EmuInterruptIndex > 0 )
    {
        EmuInterruptIndex--;
        memcpy( &R5900Regs, &R5900RegsBackup[ EmuInterruptIndex ], sizeof( R5900Regs ) );
//        memcpy( &COP0Regs, &COP0RegsBackup, sizeof( COP0Regs ) );
//        memcpy( &COP1Regs, &COP1RegsBackup, sizeof( COP1Regs ) );
    }
}

EMU_U08 * Emu_Intc_GetPointer( EMU_U32 Address )
{
    return &EmuIntcMemory[ Address - EMU_INTC_START_ADDR ];
}
