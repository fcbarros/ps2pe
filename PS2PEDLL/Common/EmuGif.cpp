
#include "EmuMain.h"
#include "EmuGS.h"
#include "EmuGif.h"
#include "EmuDma.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EMU_U08 EmuGifMemory[ EMU_GIF_END_ADDR - EMU_GIF_START_ADDR ];

// Pointer to control1 registers
stEmu_Gif_Control_Regs * Emu_Gif_Control_Reg;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Gif_Init( void )
{
//    Emu_Gif_Control_Reg = (stEmu_Gif_Control_Regs *) EMemory.GetRealPointer( EMU_GIF_START_ADDR );
    Emu_Gif_Control_Reg = (stEmu_Gif_Control_Regs *) EmuGifMemory;

    Emu_Gif_Reset( );
}

void Emu_Gif_Reset( void )
{
    memset( Emu_Gif_Control_Reg, 0, sizeof( stEmu_Gif_Control_Regs ) );

    Emu_Gif_Control_Reg->CTRL =     0x00000000;
    Emu_Gif_Control_Reg->MODE =     0x00000000;
    Emu_Gif_Control_Reg->STAT =     0x00000000;
    Emu_Gif_Control_Reg->TAG0 =     0x00000000;
    Emu_Gif_Control_Reg->TAG1 =     0x00000000;
    Emu_Gif_Control_Reg->TAG2 =     0x00000000;
    Emu_Gif_Control_Reg->TAG3 =     0x00000000;
    Emu_Gif_Control_Reg->CNT =      0x00000000;
    Emu_Gif_Control_Reg->P3CNT =    0x00000000;
    Emu_Gif_Control_Reg->P3TAG =    0x00000000;
}

void Emu_Gif_callback( EMU_U32 Address )
{
    EMU_U32 Register = Address - EMU_GIF_START_ADDR;

    switch ( Register )
    {
    case EMU_GIF_CTRL:  // w
		if ( Emu_Gif_Control_Reg->CTRL & 0x1 )
		{
			Emu_Gif_Reset( );
		}

		// Here: PSE field: transfer processing stop & restart
		// "There is no influence in the operation even if 0 is 
		//  written when the register is not stopped temporarily"

		break;

    case EMU_GIF_MODE:  // w
		
		// PATH3 mask - how can i know that it is PATH3 ?

		// continuous/intermittent mode - may be useful for the plugins i think

		break;

    case EMU_GIF_STAT:  // r
    case EMU_GIF_TAG0:  // r
    case EMU_GIF_TAG1:  // r
    case EMU_GIF_TAG2:  // r
    case EMU_GIF_TAG3:  // r
    case EMU_GIF_CNT:   // r
    case EMU_GIF_P3CNT: // r
    case EMU_GIF_P3TAG: // r
        break;
    }
}

extern _GSgifTransfer       GSgifTransfer;
//extern _GSdmaGIF       GSdmaGIF;

#define EmuDmaGifTransfer2( _addr, _qwc ) { \
    if ( _addr & 0x80000000 ) \
        GSdmaGIF( (EMU_U32*)Emu_Dma_Channel_Reg[ 2 ], (EMU_I08*)EMemory.GetRealPointer( _addr ) - _addr ); \
    else \
        GSdmaGIF( (EMU_U32*)Emu_Dma_Channel_Reg[ 2 ], (EMU_I08*)EMemory.GetRealPointer( _addr ) - _addr ); \
}

#define EmuDmaGifTransfer( _addr, _qwc ) { \
    if ( _addr & 0x80000000 ) \
        GSgifTransfer( (EMU_U32*)&EMemScratchPad[ _addr & 0x3FFF ], _qwc ); \
    else \
        GSgifTransfer( (EMU_U32*)EmuMemReadContinuosArea( _addr, _qwc * 16 ), _qwc ); \
}

#define EmuDmaGetPointer( ptr, _addr ) {\
    if ( _addr & 0x80000000 ) \
        ptr = (EMU_U32*)&EMemScratchPad[ _addr & 0x3FFF ]; \
    else \
        ptr = (EMU_U32*)EmuMemGetRealPointer( _addr & 0x7FFFFFFF ); \
}

void Emu_Gif_Transfer( stEmu_Dma_Channel_Regs * Channel )
{
    if ( Channel->CHCR == 0 )
    {
        return;
    }

    if ( Channel->CHCR & 0x00000004 ) // Chain
    {
        EMU_U32 * pTag;
        EMU_U32 RecLevel = 0;

        EmuDmaGifTransfer( Channel->MADR, Channel->QWC );

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
                EmuDmaGifTransfer( Channel->MADR, Channel->QWC );
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
            EmuDmaGifTransfer( Channel->MADR, Channel->QWC );
        }
    }
}

EMU_U08 * Emu_Gif_GetPointer( EMU_U32 Address )
{
    return &EmuGifMemory[ Address - EMU_GIF_START_ADDR ];
}

