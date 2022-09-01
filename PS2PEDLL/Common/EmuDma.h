/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuDma.h
// Version: 1.001
// Desc: Main Header File for PS2 dma handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Included comments
//		- Organized file
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_DMA_H__
#define __EMU_DMA_H__

#include "EmuMain.h"
#include "EmuIntc.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Base Channel Address
#define EMU_DMA_START_ADDR      0x10008000
#define EMU_DMA_END_ADDR        0x1000EFFF
#define EMU_DMA_ENAB_START_ADDR 0x1000F520
#define EMU_DMA_ENAB_END_ADDR   0x1000F5FF

#define EMU_DMA_0               0x10008000
#define EMU_DMA_1               0x10009000
#define EMU_DMA_2               0x1000A000
#define EMU_DMA_3               0x1000B000
#define EMU_DMA_4               0x1000B400
#define EMU_DMA_5               0x1000C000
#define EMU_DMA_6               0x1000C400
#define EMU_DMA_7               0x1000C800
#define EMU_DMA_8               0x1000D000
#define EMU_DMA_9               0x1000D400
#define EMU_DMA_CONTROL1        0x1000E000
#define EMU_DMA_CONTROL2        0x1000F520

// Channel Register Address
#define EMU_DMA_CHANNEL_CHCR    0x00000000
#define EMU_DMA_CHANNEL_MADR    0x00000010
#define EMU_DMA_CHANNEL_QWC     0x00000020
#define EMU_DMA_CHANNEL_TADR    0x00000030
#define EMU_DMA_CHANNEL_ASR0    0x00000040
#define EMU_DMA_CHANNEL_ASR1    0x00000050
#define EMU_DMA_CHANNEL_SADR    0x00000080

// Control 1 Register Address
#define EMU_DMA_REG_CTRL        0x00000000
#define EMU_DMA_REG_STAT        0x00000010
#define EMU_DMA_REG_PCR         0x00000020
#define EMU_DMA_REG_SQWC        0x00000030
#define EMU_DMA_REG_RBSR        0x00000040
#define EMU_DMA_REG_RBOR        0x00000050
#define EMU_DMA_REG_STADR       0x00000060

// Control 2 Register Address
#define EMU_DMA_REG_ENABLER     0x00000000
#define EMU_DMA_REG_ENABLEW     0x00000070


#define EM_DMA_MODE_NORMAL          0x00000000
#define EM_DMA_MODE_CHAIN           0x00000004
#define EM_DMA_MODE_INTERLEAVE      0x00000008


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef struct
{
    EMU_U32 CHCR;
    EMU_U08 null0[12];
    EMU_U32 MADR;
    EMU_U08 null1[12];
    EMU_U32 QWC;
    EMU_U08 null2[12];
    EMU_U32 TADR;
    EMU_U08 null3[12];
    EMU_U32 ASR0;
    EMU_U08 null4[12];
    EMU_U32 ASR1;
    EMU_U08 null5[44];
    EMU_U32 SADR;
} stEmu_Dma_Channel_Regs;

typedef struct
{
    EMU_U32 CTRL;
    EMU_U08 null0[12];
    EMU_U32 STAT;
    EMU_U08 null1[12];
    EMU_U32 PCR;
    EMU_U08 null2[12];
    EMU_U32 SQWC;
    EMU_U08 null3[12];
    EMU_U32 RBSR;
    EMU_U08 null4[12];
    EMU_U32 RBOR;
    EMU_U08 null5[12];
    EMU_U32 STADR;
    EMU_U08 null6[12];
} stEmu_Dma_Control1_Regs;

typedef struct
{
    EMU_U32 ENABLER;
    EMU_U08 null0[108];
    EMU_U32 ENABLEW;
} stEmu_Dma_Control2_Regs;

typedef struct
{
    bool    Running;
} stEmuDMAChannel;


// Pointer to channel registers in memory
extern stEmu_Dma_Channel_Regs  * Emu_Dma_Channel_Reg[ 10 ];
// Pointer to control1 registers
extern stEmu_Dma_Control1_Regs * Emu_Dma_Control1_Reg;
// Pointer to control2 registers
extern stEmu_Dma_Control2_Regs * Emu_Dma_Control2_Reg;

extern stEmuDMAChannel EmuDmaChannel[ 10 ];

extern stEMU_INTC_HANDLER Emu_Dma_Handler[32];

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initializes the DMA memory pointers and registers
void Emu_Dma_Init( void );
// Resets DMA registers
void Emu_Dma_Reset( void );
// Callback for DMA access
void Emu_Dma_callback( EMU_U32 Address );

void Emu_Dma_Start( unsigned int ChannelIndex );
// General set DMA access
void Emu_Dma_Set( EMU_U32 Address, EMU_U64 ChannelIndex );

void Emu_Dma_Dest_Chain( EMU_U08 ChannelIndex, stEmu_Dma_Channel_Regs  * Channel );

void Emu_Dma_Bios_AddHandler( void );
void Emu_Dma_Bios_RemoveHandler( void );
void Emu_Dma_Bios_EnableHandler( void );

void Emu_Dma_AddHandler( EMU_U32 HandlerIndex, EMU_U32 FuncAddress );
void Emu_Dma_RemoveHandler( EMU_U32 HandlerIndex );
EMU_U64 Emu_Dma_Enable( EMU_U32 HandlerIndex );

void Emu_Dma_ExitHandler( void );
void Emu_Dma_CallHandler( EMU_U32 HandlerIndex );

EMU_U08 * Emu_Dma_GetPointer( EMU_U32 Address );

#endif // __EMU_DMA_H__

