/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuGS.h
// Version: 1.001
// Desc: Main Header File for PS2 GS handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Included comments
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_GS_H__
#define __EMU_GS_H__

#include "EmuPAD.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Base Address por privileged Registers
#define EMU_GS_PRIV_START_ADDR          0x12000000
#define EMU_GS_PRIV_END_ADDR            0x13FFFFFF

#define EMU_GS_PRIV_ADDR_PMODE          0x00000000
#define EMU_GS_PRIV_ADDR_SMODE1         0x00000010
#define EMU_GS_PRIV_ADDR_SMODE2         0x00000020
#define EMU_GS_PRIV_ADDR_SRFSH          0x00000030
#define EMU_GS_PRIV_ADDR_SYNCH1         0x00000040
#define EMU_GS_PRIV_ADDR_SYNCH2         0x00000050
#define EMU_GS_PRIV_ADDR_SYNCV          0x00000060
#define EMU_GS_PRIV_ADDR_DISPFB1        0x00000070
#define EMU_GS_PRIV_ADDR_DISPLAY1       0x00000080
#define EMU_GS_PRIV_ADDR_DISPFB2        0x00000090
#define EMU_GS_PRIV_ADDR_DISPLAY2       0x000000A0
#define EMU_GS_PRIV_ADDR_EXTBUF         0x000000B0
#define EMU_GS_PRIV_ADDR_EXTDATA        0x000000C0
#define EMU_GS_PRIV_ADDR_EXTWRITE       0x000000D0
#define EMU_GS_PRIV_ADDR_BGCOLOR        0x000000E0
#define EMU_GS_PRIV_ADDR_CSR            0x00001000
#define EMU_GS_PRIV_ADDR_IMR            0x00001010
#define EMU_GS_PRIV_ADDR_BUSDIR         0x00001040
#define EMU_GS_PRIV_ADDR_SIGLBLID       0x00001080

// General Purpose Registers Codes
#define EMU_GS_REG_PRIM                 0x00
#define EMU_GS_REG_RGBAQ                0x01
#define EMU_GS_REG_ST                   0x02
#define EMU_GS_REG_UV                   0x03
#define EMU_GS_REG_XYZF2                0x04
#define EMU_GS_REG_XYZ2                 0x05
#define EMU_GS_REG_TEX0_1               0x06
#define EMU_GS_REG_TEX0_2               0x07
#define EMU_GS_REG_CLAMP_1              0x08
#define EMU_GS_REG_CLAMP_2              0x09
#define EMU_GS_REG_FOG                  0x0A
#define EMU_GS_REG_XYZF3                0x0C
#define EMU_GS_REG_XYZ3                 0x0D
#define EMU_GS_REG_TEX1_1               0x14
#define EMU_GS_REG_TEX1_2               0x15
#define EMU_GS_REG_TEX2_1               0x16
#define EMU_GS_REG_TEX2_2               0x17
#define EMU_GS_REG_XYOFFSET_1           0x18
#define EMU_GS_REG_XYOFFSET_2           0x19
#define EMU_GS_REG_PRMODECONT           0x1A
#define EMU_GS_REG_PRMODE               0x1B
#define EMU_GS_REG_TEXCLUT              0x1C
#define EMU_GS_REG_SCANMSK              0x22
#define EMU_GS_REG_MIPTBP1_1            0x34
#define EMU_GS_REG_MIPTBP1_2            0x35
#define EMU_GS_REG_MIPTBP2_1            0x36
#define EMU_GS_REG_MIPTBP2_2            0x37
#define EMU_GS_REG_TEXA                 0x3B
#define EMU_GS_REG_FOGCOL               0x3D
#define EMU_GS_REG_TEXFLUSH             0x3F
#define EMU_GS_REG_SCISSOR_1            0x40
#define EMU_GS_REG_SCISSOR_2            0x41
#define EMU_GS_REG_ALPHA_1              0x42
#define EMU_GS_REG_ALPHA_2              0x43
#define EMU_GS_REG_DIMX                 0x44
#define EMU_GS_REG_DTHE                 0x45
#define EMU_GS_REG_COLCLAMP             0x46
#define EMU_GS_REG_TEST_1               0x47
#define EMU_GS_REG_TEST_2               0x48
#define EMU_GS_REG_PABE                 0x49
#define EMU_GS_REG_FBA_1                0x4A
#define EMU_GS_REG_FBA_2                0x4B
#define EMU_GS_REG_FRAME_1              0x4C
#define EMU_GS_REG_FRAME_2              0x4D
#define EMU_GS_REG_ZBUF_1               0x4E
#define EMU_GS_REG_ZBUF_2               0x4F
#define EMU_GS_REG_BITBLTBUF            0x50
#define EMU_GS_REG_TRXPOS               0x51
#define EMU_GS_REG_TRXREG               0x52
#define EMU_GS_REG_TRXDIR               0x53
#define EMU_GS_REG_HWREG                0x54
#define EMU_GS_REG_SIGNAL               0x60
#define EMU_GS_REG_FINISH               0x61
#define EMU_GS_REG_LABEL                0x62

// Privileged Registers Codes
#define EMU_GS_REG_PMODE                0x00
#define EMU_GS_REG_SMODE2               0x02
#define EMU_GS_REG_DISPFB1              0x07
#define EMU_GS_REG_DISPLAY1             0x08
#define EMU_GS_REG_DISPFB2              0x09
#define EMU_GS_REG_DISPLAY2             0x0A
#define EMU_GS_REG_EXTBUF               0x0B
#define EMU_GS_REG_EXTDATA              0x0C
#define EMU_GS_REG_EXTWRITE             0x0D
#define EMU_GS_REG_BGCOLOR              0x0E
#define EMU_GS_REG_CSR                  0x40
#define EMU_GS_REG_IMR                  0x41
#define EMU_GS_REG_BUSDIR               0x44
#define EMU_GS_REG_SIGLBLID             0x48

// Primitives
#define EMU_GS_PRIM_POINT               0x00
#define EMU_GS_PRIM_LINE                0x01
#define EMU_GS_PRIM_LINE_STRIP          0x02
#define EMU_GS_PRIM_TRIANGLE            0x03
#define EMU_GS_PRIM_TRIANGLE_STRIP      0x04
#define EMU_GS_PRIM_TRIANGLE_FAN        0x05
#define EMU_GS_PRIM_SPRITE              0x06

// Pixel Storage Formats
#define EMU_GS_PSF_PSMCT32              0x00
#define EMU_GS_PSF_PSMCT24              0x01
#define EMU_GS_PSF_PSMCT16              0x02
#define EMU_GS_PSF_PSMCT16S             0x0A
#define EMU_GS_PSF_PSMT8                0x13
#define EMU_GS_PSF_PSMT4                0x14
#define EMU_GS_PSF_PSMT8H               0x1B
#define EMU_GS_PSF_PSMT4HL              0x24
#define EMU_GS_PSF_PSMT4HH              0x2C
#define EMU_GS_PSF_PSMZ32               0x30
#define EMU_GS_PSF_PSMZ24               0x31
#define EMU_GS_PSF_PSMZ16               0x32
#define EMU_GS_PSF_PSMZ16S              0x3A


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct
{
	EMU_U64 PMODE;
	EMU_U08 null0[8];
	EMU_U64 SMODE1;
	EMU_U08 null1[8];
	EMU_U64 SMODE2;
	EMU_U08 null2[8];
	EMU_U64 SRFSH;
	EMU_U08 null3[8];
	EMU_U64 SYNCH1;
	EMU_U08 null4[8];
	EMU_U64 SYNCH2;
	EMU_U08 null5[8];
	EMU_U64 SYNCV;
	EMU_U08 null6[8];
	EMU_U64 DISPFB1;
	EMU_U08 null7[8];
	EMU_U64 DISPLAY1;
	EMU_U08 null8[8];
	EMU_U64 DISPFB2;
	EMU_U08 null9[8];
	EMU_U64 DISPLAY2;
	EMU_U08 nullA[8];
	EMU_U64 EXTBUF;
	EMU_U08 nullB[8];
	EMU_U64 EXTDATA;
	EMU_U08 nullC[8];
	EMU_U64 EXTWRITE;
	EMU_U08 nullD[8];
	EMU_U64 BGCOLOR;
	EMU_U08 nullE[3864];
	EMU_U64 CSR;
	EMU_U08 nullF[8];
	EMU_U64 IMR;
	EMU_U08 nullG[40];
	EMU_U64 BUSDIR;
	EMU_U08 nullH[56];
	EMU_U64 SIGBLID;
} stEmu_GS_Privileg_Regs;

extern stEmu_GS_Privileg_Regs* Emu_GS_Privileg_Reg;

extern EMU_U64 TicksMinV;
extern EMU_U64 TicksMinH;
extern EMU_U64 TicksMinVCK;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initializes GS memory pointers and registers
void Emu_GS_Init();
// Resets registers and GS plugin
void Emu_GS_Reset();
// callback for write access
void Emu_GS_WriteCallback(EMU_U32 Address);
// callback for read access
void Emu_GS_ReadCallback(EMU_U32 Address, EMU_U08* RealAddress);
// Inits and creates the GS window
void Emu_GS_InitWindow();
// Calls the GS plugin configure method
void Emu_GS_Configure();
// Closes the GS window
void Emu_GS_CloseWindow();
// Shutsdown GS, deallocating memory
void Emu_GS_Shutdown();
// Flushes GS fifo
void Emu_GS_Flush();
// Processes Window and GS messages
void Emu_GS_ProcessMessages();
// For Bios Call
void Emu_GS_Bios_GetIMR();
void Emu_GS_Bios_PutIMR();
void Emu_GS_Bios_SetGsCrt();

EMU_U08* Emu_GS_GetPointer(EMU_U32 Address);

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// Typedefs for GS plugin
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

#define DLLCALL	__stdcall

typedef EMU_U32(DLLCALL* _PS2EgetLibType)();
typedef EMU_U32(DLLCALL* _PS2EgetLibVersion)();
typedef char* (DLLCALL* _PS2EgetLibName)();

// GS
typedef EMU_I32(DLLCALL* _GSinit)();
typedef EMU_I32(DLLCALL* _GSopen)(void* pDsp, const char* Title);
typedef void (DLLCALL* _GSclose)();
typedef void (DLLCALL* _GSshutdown)();
typedef void (DLLCALL* _GSvsync)();

typedef void (DLLCALL* _GSwrite32)(EMU_U32 mem, EMU_U32 value);
typedef void (DLLCALL* _GSwrite64)(EMU_U32 mem, EMU_U64 value);
typedef EMU_U32(DLLCALL* _GSread32)(EMU_U32 mem);
typedef EMU_U64(DLLCALL* _GSread64)(EMU_U32 mem);

typedef void (CALLBACK* _GSwritePReg)(EMU_U32 mem);
typedef void (CALLBACK* _GSpreg)(void* pPReg);

typedef void (DLLCALL* _GSgifTransfer)(EMU_U32* pMem, EMU_U32 size);
typedef void (DLLCALL* _GSkeyEvent)(keyEvent* ev);

typedef void (DLLCALL* _GSmakeSnapshot)();
typedef void (DLLCALL* _GSconfigure)();
typedef EMU_I32(DLLCALL* _GStest)();
typedef void (DLLCALL* _GSabout)();

// GS
extern _GSinit          GSinit;
extern _GSopen          GSopen;
extern _GSclose         GSclose;
extern _GSshutdown      GSshutdown;
extern _GSvsync         GSvsync;

extern _GSwrite32       GSwrite32;
extern _GSwrite64       GSwrite64;
extern _GSread32        GSread32;
extern _GSread64        GSread64;

extern _GSgifTransfer   GSgifTransfer;
extern _GSkeyEvent      GSkeyEvent;
extern _GSmakeSnapshot  GSmakeSnapshot;
extern _GSconfigure     GSconfigure;
extern _GStest          GStest;
extern _GSabout         GSabout;
extern _GSwritePReg     GSwritePReg;
extern _GSpreg          GSpreg;

#endif // __EMU_GS_H__