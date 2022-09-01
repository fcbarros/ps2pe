
#include "EmuMain.h"
#include "EmuThread.h"

typedef struct 
{
    EMU_I32 status;
    void    * entry;
    void    * stack;
    EMU_I32 stackSize;
    void    * gpReg;
    EMU_I32 initPriority;
    EMU_I32 currentPriority;
    EMU_U32 attr;
    EMU_U32 option;
    EMU_I32 waitType;
    EMU_I32 waitId;
    EMU_I32 wakeupCount;
} EmuThread;

EMU_I32 Emu_Thread_CurrentId = 1;
EMU_I32 Emu_Thread_NumThreads = 0;

typedef struct
{
    EMU_I32 Id;
    EMU_U08 Running;
    EMU_U32 Address;
    EMU_U08 * RealAddress;
} Emu_Thread_Struct;

Emu_Thread_Struct EmuThreads[ 100 ];


void Emu_Thread_Init( void )
{
    Emu_Thread_Reset( );
}

void Emu_Thread_Reset( void )
{
    memset( EmuThreads, 0, sizeof( EmuThreads ) );

    Emu_Thread_CurrentId = 0;
    Emu_Thread_NumThreads = 0;
}

void Emu_Thread_Bios_Create( void ) 
{ // 0x20
	R5900Regs.V0.u64_00_63 = Emu_Thread_Create( R5900Regs.A0.u32_00_31 );
}

void Emu_Thread_Bios_Start( void ) 
{ // 0x22
	Emu_Thread_Start( R5900Regs.A0.u32_00_31 );
}

void Emu_Thread_Bios_GetId( void ) 
{ // 0x2f
	R5900Regs.V0.u64_00_63 = Emu_Thread_GetId( );
}

void Emu_Thread_Bios_ReferStatus( void ) 
{ // 0x30
	R5900Regs.V0.u64_00_63 = Emu_Thread_ReferStatus( R5900Regs.A1.u32_00_31 );
}

//
//  int CreateThread(kThread *);
//   returns an id
//
EMU_U64 Emu_Thread_Create( EMU_U32 Address ) 
{ // 0x20
    EmuThread * thread;

    EmuThreads[ Emu_Thread_NumThreads ].Id = Emu_Thread_NumThreads;
    EmuThreads[ Emu_Thread_NumThreads ].Address = Address;
    EmuThreads[ Emu_Thread_NumThreads ].RealAddress = EmuMemGetRealPointer( Address );
    EmuThreads[ Emu_Thread_NumThreads ].Running = 0;
    thread = (EmuThread*)EmuThreads[ Emu_Thread_NumThreads ].RealAddress;

    Emu_Thread_NumThreads++;

    return Emu_Thread_NumThreads - 1;
}

//
//  int StartThread(int thread_id, void *arg);
//
void Emu_Thread_Start( EMU_U32 Address ) 
{ // 0x22
    EmuThreads[ Address ].Running = 1;

    EmuThread * thread = (EmuThread*)EmuMemGetRealPointer( Address );
}

//
//  int GetThreadId(void);
//
EMU_U64 Emu_Thread_GetId( void ) 
{ // 0x2f
    return Emu_Thread_CurrentId;
}

//
//  int ReferThreadStatus(int, kThread *);
//
EMU_U64 Emu_Thread_ReferStatus( EMU_U32 Address ) 
{ // 0x30
    EmuThread * thread;

    thread = (EmuThread*)EmuMemGetRealPointer( Address );

    return 1;
}

/*
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EMU_U08 EmuThreadMemory[ EMU_Thread_END_ADDR - EMU_Thread_START_ADDR ];

// Pointer to control1 registers
stEmu_Thread_Control_Regs * Emu_Thread_Control_Reg;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Thread_callback( EMM_Word Address )
{
    EMM_Word Register = Address - EMU_Thread_START_ADDR;

    switch ( Register )
    {
    case EMU_Thread_CTRL:
    case EMU_Thread_MODE:
    case EMU_Thread_STAT:
    case EMU_Thread_TAG0:
    case EMU_Thread_TAG1:
    case EMU_Thread_TAG2:
    case EMU_Thread_TAG3:
    case EMU_Thread_CNT:
    case EMU_Thread_P3CNT:
    case EMU_Thread_P3TAG:
        break;
    }
}

extern _GSThreadTransfer       GSThreadTransfer;
//extern _GSdmaThread       GSdmaThread;

#define EmuDmaThreadTransfer2( _addr, _qwc ) { \
    if ( _addr & 0x80000000 ) \
        GSdmaThread( (EMU_U32*)Emu_Dma_Channel_Reg[ 2 ], (EMU_I08*)EMemory.GetRealPointer( _addr ) - _addr ); \
    else \
        GSdmaThread( (EMU_U32*)Emu_Dma_Channel_Reg[ 2 ], (EMU_I08*)EMemory.GetRealPointer( _addr ) - _addr ); \
}

#define EmuDmaThreadTransfer( _addr, _qwc ) { \
    if ( _addr & 0x80000000 ) \
        GSThreadTransfer( (EMU_U32*)&EMemScratchPad[ _addr & 0x3FFF ], _qwc ); \
    else \
        GSThreadTransfer( (EMU_U32*)EMemory.ReadContinuosArea( _addr, _qwc * 16 ), _qwc ); \
}

#define EmuDmaGetPointer( ptr, _addr ) {\
    if ( _addr & 0x80000000 ) \
        ptr = (EMU_U32*)&EMemScratchPad[ _addr & 0x3FFF ]; \
    else \
        ptr = (EMU_U32*)EMemory.GetRealPointer( _addr & 0x7FFFFFFF ); \
}

void Emu_Thread_Transfer( stEmu_Dma_Channel_Regs * Channel )
{
    if ( Channel->CHCR == 0 )
    {
        return;
    }

    if ( Channel->CHCR & 0x00000004 ) // Chain
    {
        EMU_U32 * pTag;
        EMU_U32 RecLevel = 0;

        EmuDmaThreadTransfer( Channel->MADR, Channel->QWC );

        while ( Channel->CHCR & 0x00000100 )
        {
            EmuDmaGetPointer( pTag, Channel->TADR );
            Channel->QWC  = pTag[0] & 0x0000FFFF;
            Channel->CHCR = ( Channel->CHCR & 0x0000FFFF ) | ( pTag[0] & 0xFFFF0000 );

            switch ( ( pTag[0] >> 28 ) & 0x07 )
            {
            case 0: // refe
                Channel->MADR = pTag[1];
                Channel->CHCR &= 0xFFFFFEFF;
                break;

            case 1: // cnt
                Channel->MADR = Channel->TADR + 16;
                Channel->TADR += 16 + Channel->QWC * 16;
                break;

            case 2: // next
                Channel->MADR = Channel->TADR + 16;
                Channel->TADR = pTag[1];
                break;

            case 3: // ref
                Channel->MADR = pTag[1];
                Channel->TADR += 16;
                break;

            case 4: // refs
                Channel->MADR = pTag[1];
                Channel->TADR += 16;
                break;

            case 5: // call
                Channel->MADR = Channel->TADR + 16;
                RecLevel++;
                if ( RecLevel == 1 )
                {
                    Channel->ASR0 = Channel->MADR + ( Channel->QWC * 16 );
                }
                else
                {
                    Channel->ASR1 = Channel->MADR + ( Channel->QWC * 16 );
                }
                Channel->TADR = pTag[1];
                break;

            case 6: // ret
                Channel->MADR = Channel->TADR + 16;
                if ( RecLevel == 1 )
                {
                    Channel->TADR = Channel->ASR0;
                }
                else
                {
                    Channel->TADR = Channel->ASR1;
                }
                RecLevel--;
                break;

            case 7: // end
                Channel->MADR = Channel->TADR + 16;
                Channel->CHCR &= 0xFFFFFEFF;
                break;

            default:
                Channel->CHCR &= 0xFFFFFEFF;
                return;
            }

            if ( Channel->QWC )
            {
                EmuDmaThreadTransfer( Channel->MADR, Channel->QWC );
            }

            if ( pTag[0] & 0x80000000 ) // IRQ
            {
//                Emu_Dma_CallHandler( 2 );
            }
        }
    }
    else
    {
        if ( Channel->QWC )
        {
            EmuDmaThreadTransfer( Channel->MADR, Channel->QWC );
        }
    }
}

EMU_U08 * Emu_Thread_GetPointer( EMM_Word Address )
{
    return &EmuThreadMemory[ Address - EMU_Thread_START_ADDR ];
}
*/