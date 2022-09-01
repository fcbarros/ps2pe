/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuVif.h
// Version: 1.001
// Desc: Main Header File for PS2 Vif handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 30/04/2002
// Modifications:
// * Version 1.000 ( 30/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Comments included
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_VIF_H__
#define __EMU_VIF_H__

#include "EmuMemory.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Base Address
#define EMU_VIF_START_ADDR      0x10003800
#define EMU_VIF_END_ADDR        0x10003FFF

#define EMU_VIF0                0x10003800
#define EMU_VIF1                0x10003C00

// Control Register Address
#define EMU_VIF_STAT            0x00000000
#define EMU_VIF_FBRST           0x00000010
#define EMU_VIF_ERR             0x00000020
#define EMU_VIF_MARK            0x00000030
#define EMU_VIF_CYCLE           0x00000040
#define EMU_VIF_MODE            0x00000050
#define EMU_VIF_NUM             0x00000060
#define EMU_VIF_MASK            0x00000070
#define EMU_VIF_CODE            0x00000080
#define EMU_VIF_ITOPS           0x00000090
#define EMU_VIF_BASE            0x000000A0
#define EMU_VIF_OFST            0x000000B0
#define EMU_VIF_TOPS            0x000000C0
#define EMU_VIF_ITOP            0x000000D0
#define EMU_VIF_TOP             0x000000E0
#define EMU_VIF_R0              0x00000100
#define EMU_VIF_R1              0x00000110
#define EMU_VIF_R2              0x00000120
#define EMU_VIF_R3              0x00000130
#define EMU_VIF_C0              0x00000140
#define EMU_VIF_C1              0x00000150
#define EMU_VIF_C2              0x00000160
#define EMU_VIF_C3              0x00000170


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct
{
    EMU_U32 STAT;
    EMU_U08 null0[12];
    EMU_U32 FBRST;
    EMU_U08 null1[12];
    EMU_U32 ERR;
    EMU_U08 null2[12];
    EMU_U32 MARK;
    EMU_U08 null3[12];
    EMU_U32 CYCLE;
    EMU_U08 null4[12];
    EMU_U32 MODE;
    EMU_U08 null5[12];
    EMU_U32 NUM;
    EMU_U08 null6[12];
    EMU_U32 MASK;
    EMU_U08 null7[12];
    EMU_U32 CODE;
    EMU_U08 null8[12];
    EMU_U32 ITOPS;
    EMU_U08 null9[12];
    EMU_U32 BASE;
    EMU_U08 null10[12];
    EMU_U32 OFST;
    EMU_U08 null11[12];
    EMU_U32 TOPS;
    EMU_U08 null12[12];
    EMU_U32 ITOP;
    EMU_U08 null13[12];
    EMU_U32 TOP;
    EMU_U08 null14[28];
    EMU_U32 R0;
    EMU_U08 null15[12];
    EMU_U32 R1;
    EMU_U08 null16[12];
    EMU_U32 R2;
    EMU_U08 null17[12];
    EMU_U32 R3;
    EMU_U08 null18[12];
    EMU_U32 C0;
    EMU_U08 null19[12];
    EMU_U32 C1;
    EMU_U08 null20[12];
    EMU_U32 C2;
    EMU_U08 null21[12];
    EMU_U32 C3;
    EMU_U08 null22[12];
} stEmu_Vif_Control_Regs;



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initialized Vif memory pointers and registers
void Emu_Vif_Init( void );
// Resets Vif registers
void Emu_Vif_Reset( void );
// Callback for Vif access
void Emu_Vif_callback( EMU_U32 Address );

EMU_U08 * Emu_Vif_GetPointer( EMU_U32 Address );

#endif // __EMU_VIF_H__
