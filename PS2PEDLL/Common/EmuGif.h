/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuGif.h
// Version: 1.001
// Desc: Main Header File for PS2 Gif handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Included comments
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_GIF_H__
#define __EMU_GIF_H__

#include "EmuMemory.h"
#include "EmuDma.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Base Address
#define EMU_GIF_START_ADDR      0x10003000
#define EMU_GIF_END_ADDR        0x100037FF

// Control Register Address
#define EMU_GIF_CTRL            0x00000000
#define EMU_GIF_MODE            0x00000010
#define EMU_GIF_STAT            0x00000020
#define EMU_GIF_TAG0            0x00000040
#define EMU_GIF_TAG1            0x00000050
#define EMU_GIF_TAG2            0x00000060
#define EMU_GIF_TAG3            0x00000070
#define EMU_GIF_CNT             0x00000080
#define EMU_GIF_P3CNT           0x00000090
#define EMU_GIF_P3TAG           0x000000A0


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct
{
    EMU_U32 CTRL;
    EMU_U08 null0[12];
    EMU_U32 MODE;
    EMU_U08 null1[12];
    EMU_U32 STAT;
    EMU_U08 null2[28];
    EMU_U32 TAG0;
    EMU_U08 null3[12];
    EMU_U32 TAG1;
    EMU_U08 null4[12];
    EMU_U32 TAG2;
    EMU_U08 null5[12];
    EMU_U32 TAG3;
    EMU_U08 null6[12];
    EMU_U32 CNT;
    EMU_U08 null7[12];
    EMU_U32 P3CNT;
    EMU_U08 null8[12];
    EMU_U32 P3TAG;
    EMU_U08 null9[12];
} stEmu_Gif_Control_Regs;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Fucntions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initializes Gif memory pointers and registers
void Emu_Gif_Init( void );
// Resets Gif registers
void Emu_Gif_Reset( void );
// Callback for Gif access
void Emu_Gif_callback( EMU_U32 Address );
// Gif transfer to GS
void Emu_Gif_Transfer( stEmu_Dma_Channel_Regs * Channel );

EMU_U08 * Emu_Gif_GetPointer( EMU_U32 Address );

#endif