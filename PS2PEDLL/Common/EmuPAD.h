/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuPad.h
// Version: 1.002
// Desc: Main Header File for PS2 Pad handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 30/04/2002
// Modifications:
// * Version 1.000 ( 30/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Comments included
// * Version 1.002 ( 21/05/2002 ): Modified
//      - Included defines
//      - More struct types
//      - More variables
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMUPAD_H__
#define __EMUPAD_H__

#include "EmuPS2Core.h"


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//
// Pad states
//
#define PAD_STATE_DISCONN       0x00
#define PAD_STATE_FINDPAD       0x01
#define PAD_STATE_FINDCTP1      0x02
#define PAD_STATE_EXECCMD       0x05
#define PAD_STATE_STABLE        0x06
#define PAD_STATE_ERROR         0x07

//
// Pad request states
//
#define PAD_RSTAT_COMPLETE      0x00
#define PAD_RSTAT_FAILED        0x01
#define PAD_RSTAT_BUSY          0x02

// rom0:padman
#define PAD_BIND_RPC_ID1 0x8000010f
#define PAD_BIND_RPC_ID2 0x8000011f

#define PAD_RPCCMD_OPEN         0x80000100
#define PAD_RPCCMD_SET_MMODE    0x80000105
#define PAD_RPCCMD_SET_ACTDIR   0x80000106
#define PAD_RPCCMD_SET_ACTALIGN 0x80000107
#define PAD_RPCCMD_GET_BTNMASK  0x80000108
#define PAD_RPCCMD_SET_BTNINFO  0x80000109
#define PAD_RPCCMD_SET_VREF     0x8000010a
#define PAD_RPCCMD_GET_PORTMAX  0x8000010b
#define PAD_RPCCMD_GET_SLOTMAX  0x8000010c
#define PAD_RPCCMD_CLOSE        0x8000010d
#define PAD_RPCCMD_END          0x8000010e

#define PAD_RPCCMD_INIT         0x00000000  /* not supported */
#define PAD_RPCCMD_GET_CONNECT  0x00000000  /* not supported */
#define PAD_RPCCMD_GET_MODVER   0x00000000  /* not supported */

// rom0:xpadman
#define XPAD_BIND_RPC_ID1 0x80000100
#define XPAD_BIND_RPC_ID2 0x80000101

#define XPAD_RPCCMD_OPEN         0x01
#define XPAD_RPCCMD_SET_MMODE    0x06
#define XPAD_RPCCMD_SET_ACTDIR   0x07
#define XPAD_RPCCMD_SET_ACTALIGN 0x08
#define XPAD_RPCCMD_GET_BTNMASK  0x09
#define XPAD_RPCCMD_SET_BTNINFO  0x0A
#define XPAD_RPCCMD_SET_VREF     0x0B
#define XPAD_RPCCMD_GET_PORTMAX  0x0C
#define XPAD_RPCCMD_GET_SLOTMAX  0x0D
#define XPAD_RPCCMD_CLOSE        0x0E
#define XPAD_RPCCMD_END          0x0F
#define XPAD_RPCCMD_INIT         0x10
#define XPAD_RPCCMD_GET_CONNECT  0x11
#define XPAD_RPCCMD_GET_MODVER   0x12


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Structs and types
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef struct 
{
	EMU_U32 key;
	EMU_U32 event;
} keyEvent;

typedef struct 
{
    unsigned char ok;
    unsigned char mode;
    unsigned char btns[2];
    // joysticks
    unsigned char rjoy_h;
    unsigned char rjoy_v;
    unsigned char ljoy_h;
    unsigned char ljoy_v;
    // pressure mode
    unsigned char left_p;
    unsigned char rigth_p;
    unsigned char up_p;
    unsigned char down_p;
    unsigned char triangle_p;
    unsigned char circle_p;
    unsigned char square_p;
    unsigned char cross_p;
    unsigned char unkn16[16];
}  padButtonStatus;

typedef struct 
{
    unsigned int frame;
    unsigned char state;
    unsigned char reqState;
    unsigned char ok;
    unsigned char unkn7;
    unsigned char data[32];
    unsigned int length;
    unsigned int unkn44;
    unsigned int unkn48;
    unsigned int unkn52;
    unsigned int unkn54;
    unsigned int unkn60;
} pad_data;

typedef struct
{
    int open;
    unsigned int port;
    unsigned int slot;
    pad_data * padData;
    unsigned char * padBuf;
} pad_state;

typedef struct 
{
    unsigned char data[32];         // 0, length = 32 bytes
    unsigned int unkn32;            // not used??
    unsigned int unkn36;            // not used??
    unsigned int unkn40;            // byte 40  not used??
    unsigned int unkn44;            // not used?? 44
    unsigned char actData[32];      // actuator (6x4?) 48
    unsigned short modeTable[4];    // padInfo   80
    unsigned int frame;             // byte 88, EMU_U32 22
    unsigned int unkn92;            // not used ??
    unsigned int length;            // 96
    unsigned char modeOk;           // padInfo  100 Dunno about the name though...
    unsigned char modeCurId;        // padInfo    101
    unsigned char unkn102;          // not used??
    unsigned char unknown;          // unknown
    unsigned char nrOfModes;        // padInfo   104
    unsigned char modeCurOffs;      // padInfo   105
    unsigned char nrOfActuators;    // act  106
    unsigned char unkn107[5];       // not used??
    unsigned char state;            // byte 112
    unsigned char reqState;         // byte 113
    unsigned char ok;               // padInfo  114
    unsigned char unkn115[13];      // not used??
} xpad_data;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Exported Variables
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

extern pad_data * PadArea[2][2];
extern xpad_data * XPadArea[2][2];
extern padButtonStatus * PadStat[2];


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initialized PAD plugin
void Emu_PAD_Init( void );
// Resets PAD plugin
void Emu_PAD_Reset( void );
// Plugin PAD config
void Emu_PAD_Config( void );

void Emu_PAD_Bios_ReadStatus( EMU_I32 pad );

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Plugin related stuff
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#define DLLCALL	__stdcall

// PAD
typedef EMU_I32 (DLLCALL* _PADinit)(EMU_U32 flags);
typedef EMU_I32 (DLLCALL* _PADopen)(void *pDsp);
typedef void (DLLCALL* _PADclose)();
typedef void (DLLCALL* _PADshutdown)();
typedef keyEvent* (DLLCALL* _PADkeyEvent)();
typedef void (DLLCALL* _PADreadStatus)(padButtonStatus *data, int pad);
typedef EMU_U32 (DLLCALL* _PADquery)();

typedef void (DLLCALL* _PADconfigure)();
typedef EMU_I32 (DLLCALL* _PADtest)();
typedef void (DLLCALL* _PADabout)();

// PAD1
extern _PADinit        PAD1init;
extern _PADopen        PAD1open;
extern _PADclose       PAD1close;
extern _PADshutdown    PAD1shutdown;
extern _PADkeyEvent    PAD1keyEvent;
extern _PADreadStatus  PAD1readStatus;
extern _PADquery       PAD1query;

extern _PADconfigure   PAD1configure;
extern _PADtest        PAD1test;
extern _PADabout       PAD1about;

// PAD2
extern _PADinit        PAD2init;
extern _PADopen        PAD2open;
extern _PADclose       PAD2close;
extern _PADshutdown    PAD2shutdown;
extern _PADkeyEvent    PAD2keyEvent;
extern _PADreadStatus  PAD2readStatus;
extern _PADquery       PAD2query;

extern _PADconfigure   PAD2configure;
extern _PADtest        PAD2test;
extern _PADabout       PAD2about;

#endif