#include "EmuMain.h"
#include "EmuGS.h"
#include "EmuPad.h"
#include "EmuIntc.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// GS
_GSinit         GSinit;
_GSopen         GSopen;
_GSclose        GSclose;
_GSshutdown     GSshutdown;
_GSvsync        GSvsync;

_GSwrite32      GSwrite32;
_GSwrite64      GSwrite64;
_GSread32       GSread32;
_GSread64       GSread64;

_GSgifTransfer   GSgifTransfer;
_GSkeyEvent     GSkeyEvent;
_GSmakeSnapshot GSmakeSnapshot;
_GSconfigure    GSconfigure;
_GStest         GStest;
_GSabout        GSabout;
_GSwritePReg    GSwritePReg;
_GSpreg         GSpreg;

stEmu_GS_Privileg_Regs * Emu_GS_Privileg_Reg;

#ifdef __WIN32__
   HWND hWnd_GS = 0;
#else
   int hWnd_GS = 0;
#endif

EMU_U64 TicksMinV;
EMU_U64 TicksMinH;
EMU_U64 TicksMinVCK;

EMU_U64 GSTargetVSync;
EMU_U64 GSTargetHSync;

keyEvent GSKey;

EMU_U08 EmuGsMemory[ EMU_GS_PRIV_END_ADDR - EMU_GS_PRIV_START_ADDR ];

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_GS_Init( void )
{
    Emu_GS_Privileg_Reg = (stEmu_GS_Privileg_Regs *) EmuGsMemory;

    GSTargetVSync = 60;
    GSTargetHSync = 15734;

    TicksMinV = (EMU_U64)CPUClock / GSTargetVSync;
    TicksMinH = (EMU_U64)CPUClock / GSTargetHSync;
    TicksMinVCK = TicksMinH / 10;

    if ( GSpreg )
    {
//        GSpreg( EmuGsMemory );
    }
    GSinit( );

    Emu_GS_Reset( );
}

void Emu_GS_Reset( void )
{
    memset( Emu_GS_Privileg_Reg, 0, sizeof( stEmu_GS_Privileg_Regs ) );

    Emu_GS_Privileg_Reg->PMODE =        0x00000040;
    Emu_GS_Privileg_Reg->SMODE1 =       0x00000000;
    Emu_GS_Privileg_Reg->SMODE2 =       0x00000000;
    Emu_GS_Privileg_Reg->SRFSH =        0x00000000;
    Emu_GS_Privileg_Reg->SYNCH1 =       0x00000000;
    Emu_GS_Privileg_Reg->SYNCH2 =       0x00000000;
    Emu_GS_Privileg_Reg->SYNCV =        0x00000000;
    Emu_GS_Privileg_Reg->DISPFB1 =      0x00000000;
    Emu_GS_Privileg_Reg->DISPLAY1 =     0x00000000;
    Emu_GS_Privileg_Reg->DISPFB2 =      0x00000000;
    Emu_GS_Privileg_Reg->DISPLAY2 =     0x00000000;
    Emu_GS_Privileg_Reg->EXTBUF =       0x00000000;
    Emu_GS_Privileg_Reg->EXTDATA =      0x00000000;
    Emu_GS_Privileg_Reg->EXTWRITE =     0x00000000;
    Emu_GS_Privileg_Reg->BGCOLOR =      0x00000000;
    Emu_GS_Privileg_Reg->CSR =          0x00000000; // 0x0000000000000200
    Emu_GS_Privileg_Reg->IMR =          0x00000000; // 0x000000000000ff00
    Emu_GS_Privileg_Reg->BUSDIR =       0x00000000;
    Emu_GS_Privileg_Reg->SIGBLID =      0x00000000;

    Emu_GS_CloseWindow( );
}

void Emu_GS_InitWindow( void )
{
    if ( hWnd_GS == 0 )
    {
        GSopen( &hWnd_GS, "PS2 Personal Emulator" );
        PAD1init( 3 );
        PAD1open( &hWnd_GS );
    }
}

void Emu_GS_Configure( void )
{
    GSconfigure( );
}

void Emu_GS_CloseWindow( void )
{
    if ( hWnd_GS )
    {
        GSclose( );
        PAD1close( );
        PAD1shutdown( );
        hWnd_GS = 0;
    }
}

void Emu_GS_Shutdown( void )
{
    Emu_GS_CloseWindow( );
    GSshutdown( );
}

void Emu_GS_Flush( void )
{
}

void Emu_GS_ProcessMessages( void )
{
    if ( hWnd_GS )
    {
        static unsigned __int64  StartV = 0,
                        StartH = 0,
                        End = 0;
        static keyEvent * PADKeyPressed;

        RDTSC( End );

        if ( ( End - StartV ) > TicksMinV )
        {
            Emu_GS_Privileg_Reg->CSR |= 0x0000000C;
            Emu_Intc_Control_Reg->STAT_GS = 1;
            Emu_Intc_CallHandler( 2 );
            GSvsync();
//            Emu_Intc_CallHandler( 3 );

            RDTSC( StartV );
            StartH = StartV;
            Emu_PAD_Bios_ReadStatus( 1 );
            Emu_PAD_Bios_ReadStatus( 2 );
            if ( ( PADKeyPressed = PAD1keyEvent( ) ) )
            {
                if ( PADKeyPressed->key == VK_ESCAPE )
                {
                    Emu_GS_CloseWindow( );
                    EmuStopRun = true;
                }
                else
                {
                    GSkeyEvent( PADKeyPressed );
                }
            }
        }
        else
        if ( ( End - StartH ) > TicksMinH )
        {
//            Emu_GS_Privileg_Reg->CSR &= 0xFFFFFFF3;
            Emu_GS_Privileg_Reg->CSR |= 0x00000004;
            Emu_Intc_Control_Reg->STAT_GS = 1;

            RDTSC( StartH );
        }
        else
        {
            Emu_GS_Privileg_Reg->CSR &= 0xFFFFFFF3;
            Emu_Intc_Control_Reg->STAT_GS = 0;
        }
    }
}

void Emu_GS_WriteCallback( EMU_U32 Address )
{
#ifdef EMU_LOG
   EmuLog( "GS: %.8X = %.16X\n", Address, EmuMemGetDWord( Address ) );
#endif
//    if ( GSwritePReg )
    {
//        GSwritePReg( Address );
    }
//    else
    {
        GSwrite64( Address, EmuMemGetDWord( Address ) );
    }
}

void Emu_GS_ReadCallback( EMU_U32 Address, EMU_U08 * RealAddress )
{
}

//
//  iGsGetIMR();
//
void Emu_GS_Bios_GetIMR( void ) 
{ // 0x70
    R5900Regs.V0.u64_00_63 = EmuMemGetDWord( 0x12001010 );
}

//
//  void iGsPetIMR(u64 IMR);
//
void Emu_GS_Bios_PutIMR( void ) 
{ // 0x71
    EmuMemSetDWord( 0x12001010, R5900Regs.A0.u64_00_63 );
}

void Emu_GS_Bios_SetGsCrt( void ) 
{ // 0x02
}

EMU_U08 * Emu_GS_GetPointer( EMU_U32 Address )
{
    return &EmuGsMemory[ Address - EMU_GS_PRIV_START_ADDR ];
}
