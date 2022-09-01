/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuIntc.h
// Version: 1.001
// Desc: Main Header File for PS2 Interrupt handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 26/04/2002
// Modifications:
// * Version 1.000 ( 26/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Included comments
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_INTC_H__
#define __EMU_INTC_H__

#include "EmuMemory.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Base Address
#define EMU_INTC_START_ADDR     0x1000F000
#define EMU_INTC_END_ADDR       0x1000F22F

// Control Register Address
#define EMU_INTC_STAT           0x00000000
#define EMU_INTC_MASK           0x00000010


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct
{
    union
    {
        EMU_U32 STAT;
        struct
        {
           EMU_U32 STAT_GS:1;     // 0
           EMU_U32 STAT_SBUS:1;   // 1
           EMU_U32 STAT_VBON:1;   // 2
           EMU_U32 STAT_VBOF:1;   // 3
           EMU_U32 STAT_VIF0:1;   // 4
           EMU_U32 STAT_VIF1:1;   // 5
           EMU_U32 STAT_VU0:1;    // 6
           EMU_U32 STAT_VU1:1;    // 7
           EMU_U32 STAT_IPU:1;    // 8
           EMU_U32 STAT_TIM0:1;   // 9
           EMU_U32 STAT_TIM1:1;   // 10
           EMU_U32 STAT_TIM2:1;   // 11
           EMU_U32 STAT_TIM3:1;   // 12
           EMU_U32 STAT_SFIFO:1;  // 13
           EMU_U32 STAT_VU0WD:1;  // 14
        };
    };    
    EMU_U08 null0[12];
    union
    {
        EMU_U32 MASK;
        struct
        {
           EMU_U32 MASK_GS:1;     // 0
           EMU_U32 MASK_SBUS:1;   // 1
           EMU_U32 MASK_VBON:1;   // 2
           EMU_U32 MASK_VBOF:1;   // 3
           EMU_U32 MASK_VIF0:1;   // 4
           EMU_U32 MASK_VIF1:1;   // 5
           EMU_U32 MASK_VU0:1;    // 6
           EMU_U32 MASK_VU1:1;    // 7
           EMU_U32 MASK_IPU:1;    // 8
           EMU_U32 MASK_TIM0:1;   // 9
           EMU_U32 MASK_TIM1:1;   // 10
           EMU_U32 MASK_TIM2:1;   // 11
           EMU_U32 MASK_TIM3:1;   // 12
           EMU_U32 MASK_SFIFO:1;  // 13
           EMU_U32 MASK_VU0WD:1;  // 14
        };
    };    
    EMU_U08 null1[12];
} stEmu_Intc_Control_Regs;

typedef struct 
{
	int active;
	int id;
	unsigned long func;
} stEMU_INTC_HANDLER;

extern stEMU_INTC_HANDLER Emu_Intc_Handler[32];

// Pointer to control1 registers
extern stEmu_Intc_Control_Regs * Emu_Intc_Control_Reg;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initializes Interrupt memory pointers and registers
void Emu_Intc_Init( void );
// Resets Interrupt registers
void Emu_Intc_Reset( void );
// Callback for Interrupt access
void Emu_Intc_callback( EMU_U32 Address );

void Emu_Intc_Bios_AddHandler( void );
void Emu_Intc_Bios_RemoveHandler( void );
void Emu_Intc_Bios_EnableHandler( void );
void Emu_Intc_Bios_DisableHandler( void );

void Emu_Intc_AddHandler( EMU_U32 HandlerInder, EMU_U32 FuncAddress );
void Emu_Intc_RemoveHandler( EMU_U32 HandlerIndex );
void Emu_Intc_EnableHandler( EMU_U32 HandlerIndex );
void Emu_Intc_DisableHandler( EMU_U32 HandlerIndex );

void Emu_Intc_CallHandler( EMU_U32 HandlerIndex );
void Emu_Intc_ExitHandler( void );

EMU_U08 * Emu_Intc_GetPointer( EMU_U32 Address );

#endif // __EMU_INTC_H__