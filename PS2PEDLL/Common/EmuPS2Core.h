/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuPS2Core.h
// Version: 1.001
// Desc: Main Header File for PS2 registers and type handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Comments included
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __WIN32__
    #include <windows.h>
    #include <stdarg.h>

    #include "../resource.h"
#else
    #include <varargs.h>
#endif

#ifdef __WIN32__
    #define DLLEXPORT   __declspec( dllexport )
#else
    #define DLLEXPORT
#endif

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Main PS2 types
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef char                EMU_I08;
typedef short               EMU_I16;
typedef int                 EMU_I32;
typedef __int64             EMU_I64;
typedef unsigned char       EMU_U08;
typedef unsigned short      EMU_U16;
typedef unsigned int        EMU_U32;
typedef unsigned __int64    EMU_U64;

typedef float               EMU_F32;
typedef double              EMU_F64;

typedef union
{
    EMU_F32 f;
    EMU_U32 u;
    EMU_I32 i;
} EMU_UF32;

// Type EMU_128, has both signed and unsigned members
typedef union
{
    // Unsigned types
    EMU_U08 u08[ 16 ];
    struct
    {
        EMU_U08 u08_00_07;
        EMU_U08 u08_08_15;
        EMU_U08 u08_16_23;
        EMU_U08 u08_24_31;
        EMU_U08 u08_32_39;
        EMU_U08 u08_40_47;
        EMU_U08 u08_48_55;
        EMU_U08 u08_56_63;
        EMU_U08 u08_64_71;
        EMU_U08 u08_72_79;
        EMU_U08 u08_80_87;
        EMU_U08 u08_88_95;
        EMU_U08 u08_96_103;
        EMU_U08 u08_104_111;
        EMU_U08 u08_112_119;
        EMU_U08 u08_120_127;
    };
    EMU_U16 u16[ 8 ];
    struct
    {
        EMU_U16 u16_00_15;
        EMU_U16 u16_16_31;
        EMU_U16 u16_32_47;
        EMU_U16 u16_48_63;
        EMU_U16 u16_64_79;
        EMU_U16 u16_80_95;
        EMU_U16 u16_96_111;
        EMU_U16 u16_112_127;
    };
    EMU_U32 u32[ 4 ]; 
    struct
    {
        EMU_U32 u32_00_31;
        EMU_U32 u32_32_63;
        EMU_U32 u32_64_95;
        EMU_U32 u32_96_127;
    };
    EMU_U64 u64[ 2 ];
    struct
    {
        EMU_U64 u64_00_63;
        EMU_U64 u64_64_127;
    };

    // signed types
    EMU_I08 i08[ 16 ];
    struct
    {
        EMU_I08 i08_00_07;
        EMU_I08 i08_08_15;
        EMU_I08 i08_16_23;
        EMU_I08 i08_24_31;
        EMU_I08 i08_32_39;
        EMU_I08 i08_40_47;
        EMU_I08 i08_48_55;
        EMU_I08 i08_56_63;
        EMU_I08 i08_64_71;
        EMU_I08 i08_72_79;
        EMU_I08 i08_80_87;
        EMU_I08 i08_88_95;
        EMU_I08 i08_96_103;
        EMU_I08 i08_104_111;
        EMU_I08 i08_112_119;
        EMU_I08 i08_120_127;
    };
    EMU_I16 i16[ 8 ];
    struct
    {
        EMU_I16 i16_00_15;
        EMU_I16 i16_16_31;
        EMU_I16 i16_32_47;
        EMU_I16 i16_48_63;
        EMU_I16 i16_64_79;
        EMU_I16 i16_80_95;
        EMU_I16 i16_96_111;
        EMU_I16 i16_112_127;
    };
    EMU_I32 i32[ 4 ];
    struct
    {
        EMU_I32 i32_00_31;
        EMU_I32 i32_32_63;
        EMU_I32 i32_64_95;
        EMU_I32 i32_96_127;
    };
    EMU_I64 i64[ 2 ];
    struct
    {
        EMU_I64 i64_00_63;
        EMU_I64 i64_64_127;
    };

    // Float part
    EMU_F32 f32[ 4 ];
    struct
    {
       EMU_F32 x;
       EMU_F32 y;
       EMU_F32 z;
       EMU_F32 w;
    };
} EMU_128;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Registers definitions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// R5900 registers
typedef struct
{
    union
    {
        EMU_128 Reg[ 32 ];
        struct
        {
            EMU_128 R0; // 0
            EMU_128 AT; // 1
            EMU_128 V0; // 2
            EMU_128 V1; // 3
            EMU_128 A0; // 4
            EMU_128 A1; // 5
            EMU_128 A2; // 6
            EMU_128 A3; // 7
            EMU_128 T0; // 8
            EMU_128 T1; // 9
            EMU_128 T2; // 10
            EMU_128 T3; // 11
            EMU_128 T4; // 12
            EMU_128 T5; // 13
            EMU_128 T6; // 14
            EMU_128 T7; // 15
            EMU_128 S0; // 16
            EMU_128 S1; // 17
            EMU_128 S2; // 18
            EMU_128 S3; // 19
            EMU_128 S4; // 20
            EMU_128 S5; // 21
            EMU_128 S6; // 22
            EMU_128 S7; // 23
            EMU_128 T8; // 24
            EMU_128 T9; // 25
            EMU_128 K0; // 26
            EMU_128 K1; // 27
            EMU_128 GP; // 28
            EMU_128 SP; // 29
            EMU_128 FP; // 30
            EMU_128 RA; // 31
        };
    };

    EMU_128 HI;
    EMU_128 LO;

    EMU_U64 SA;
    EMU_U32 PC;
} Emu_R5900_Regs;


// COP0 registers
typedef struct
{
    union 
    {
        EMU_U32     Reg[ 32 ];
        struct
        {
            EMU_U32 Index;      // 0
            EMU_U32 Random;     // 1
            EMU_U32 EntryLo0;   // 2
            EMU_U32 EntryLo1;   // 3
            EMU_U32 Context;    // 4
            EMU_U32 PageMask;   // 5
            EMU_U32 Wired;      // 6
            EMU_U32 Reserved1;  // 7
            EMU_U32 BadVAddr;   // 8
            EMU_U32 Count;      // 9
            EMU_U32 EntryHi;    // 10
            EMU_U32 Compare;    // 11
				union
				{
               EMU_U32 Status;     // 12
					struct
					{
						EMU_U32 Status_IE:1;
						EMU_U32 Status_EXL:1;
						EMU_U32 Status_ERL:1;
						EMU_U32 Status_KSU:2;
						EMU_U32 Status_05_09:5;
						EMU_U32 Status_IM:2;
						EMU_U32 Status_BEM:1;
						EMU_U32 Status_13_14:2;
						EMU_U32 Status_IM7:1;
						EMU_U32 Status_EIE:1;
						EMU_U32 Status_EDI:1;
						EMU_U32 Status_CH:1;
						EMU_U32 Status_19_21:3;
						EMU_U32 Status_BEV:1;
						EMU_U32 Status_DEV:1;
						EMU_U32 Status_24_27:4;
						EMU_U32 Status_CU:4;
               };
				};
            union
            {
               EMU_U32 Cause;      // 13
					struct
					{
						EMU_U32 Cause_00_01:2;     // 0
						EMU_U32 Cause_ExcCode:5;   // 2
						EMU_U32 Cause_07_09:3;     // 7
						EMU_U32 Cause_IP2:1;       // 10
						EMU_U32 Cause_IP3:1;       // 11
						EMU_U32 Cause_12_14:3;     // 12
						EMU_U32 Cause_IP7:1;       // 15
						EMU_U32 Cause_EXC2:3;      // 16
						EMU_U32 Cause_19_27:9;     // 19
						EMU_U32 Cause_CE:2;        // 28
						EMU_U32 Cause_BD2:1;       // 30
						EMU_U32 Cause_BD:1;        // 31
               };
            };
            EMU_U32 EPC;        // 14
            union
				{
                EMU_U32 PRId;       // 15
					 struct
					 {
						  EMU_U32 PRId_REV:8;
						  EMU_U32 PRId_IMP:8;
						  EMU_U32 PRId_RESERVED:16;
					 };
				};
            EMU_U32 Config;     // 16
            EMU_U32 Reserved2;  // 17
            EMU_U32 Reserved3;  // 18
            EMU_U32 Reserved4;  // 19
            EMU_U32 Reserved5;  // 20
            EMU_U32 Reserved6;  // 21
            EMU_U32 Reserved7;  // 22
            EMU_U32 Reserved8;  // 23
            EMU_U32 Debug;      // 24
            EMU_U32 Perf;       // 25
            EMU_U32 Reserved9;  // 26
            EMU_U32 Reserved10; // 27
            EMU_U32 TagLo;      // 28
            EMU_U32 TagHi;      // 29
            EMU_U32 Error_EPC;  // 30
            EMU_U32 Reserved11; // 31
        };
    };
} Emu_COP0_Regs;


// COP1 registers
typedef struct
{
    EMU_UF32        Reg[ 32 ];
	 union
	 {
		 EMU_U32      FCR0;
		 struct
		 {
		     EMU_U32  FCR0_REV:8;
			  EMU_U32  FCR0_IMP:8;
			  EMU_U32  FCR0_RESERVED:16;
		 };
	 };
	 union
	 {
		EMU_U32		  FCR31;
		struct
		{
			  EMU_U32  FCR31_00:1;
			  EMU_U32  FCR31_01:1;
			  EMU_U32  FCR31_02:1;
			  EMU_U32  FCR31_SU:1;
			  EMU_U32  FCR31_SO:1;
			  EMU_U32  FCR31_SD:1;
			  EMU_U32  FCR31_SI:1;
			  EMU_U32  FCR31_07_13:7;
			  EMU_U32  FCR31_U:1;
			  EMU_U32  FCR31_O:1;
			  EMU_U32  FCR31_D:1;
			  EMU_U32  FCR31_I:1;
			  EMU_U32  FCR31_18_22:5;
			  EMU_U32  FCR31_C:1;
			  EMU_U32  FCR31_24:1;
			  EMU_U32  FCR31_25_31:7;
		};
	 };
    EMU_UF32        Accumulator;
} Emu_COP1_Regs;

typedef struct 
{
    Emu_R5900_Regs R5900Regs;
    Emu_COP0_Regs COP0Regs;
    Emu_COP1_Regs COP1Regs;

    Emu_R5900_Regs R5900RegsBackup[10];
    Emu_COP0_Regs COP0RegsBackup[10];
    Emu_COP1_Regs COP1RegsBackup[10];
} EMU_PS2_Regs;
