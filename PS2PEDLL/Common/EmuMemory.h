/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuMemory.h
// Version: 1.002
// Desc: Main Header File for Emulator memory handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 23/04/2002
// Modifications:
// * Version 1.000 ( 23/04/2002 ): Created
// * Version 1.001 ( 04/05/2002 ): Modified
//		- Changed the mapping method doing a simple memory access
//		  in place of using std::map, 100% performance increase
// * Version 1.002 ( 14/05/2002 ): Modified
//		- Comments included
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_MEMORY_H__
#define __EMU_MEMORY_H__

#include "EmuPS2Core.h"
#include <vector>

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Reads processor current ticks
#define RDTSC(v)    __asm _emit 0x0f \
                    __asm _emit 0x31 \
                    __asm mov dword ptr v, eax \
                    __asm mov dword ptr v+4, edx
                        
// Types for memory area - Not used
#define EMMT_CODE       0
#define EMMT_DATA       1
#define EMMT_STACK      2
#define EMMT_RESERVED   3

// Permisions for memory area - Not used
#define EMMP_READ       0x1
#define EMMP_WRITE      0x2
#define EMMP_EXEC       0x4

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Structs and types
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
typedef void (*EMM_WRITECALLBACK)( EMU_U32 );
typedef void (*EMM_READCALLBACK)( EMU_U32, EMU_U08 * );

// Struct for mult-type handling
typedef union
{
    EMU_U08 *  Byte;
    EMU_U16 *  Short;
    EMU_U32 *  Word;
    EMU_U64 *  DWord;
} EMM_Data_Type;

struct stTLB_Entry
{
    EMU_U32    VirtualPage;
    EMU_U32    PhysicalPage;
    EMU_U64    LastAcessed;
};

struct stTLB_WriteCallBack
{
    EMU_U32             StartAddress;
    EMU_U32		         EndAddress;
    EMM_WRITECALLBACK	CallBack;
};

struct stTLB_ReadCallBack
{
    EMU_U32			   StartAddress;
    EMU_U32			   EndAddress;
    EMM_READCALLBACK	CallBack;
};

typedef struct
{
    EMU_U08   InMemory;
    EMU_U08   Saved;
    EMU_U08   Modified;
    EMU_U08   MustExist;
    EMU_U32   Page;
    EMU_U64   LastAcessed;
} stTLB_Map;

typedef stTLB_Map							EMM_MapType;
typedef std::vector<stTLB_WriteCallBack>	EMM_WriteCallBackType;
typedef std::vector<stTLB_ReadCallBack>	    EMM_ReadCallBackType;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Main Class definition
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

 // Main Constructor
void EmuMemInit( EMU_U32 MemorySize, EMU_U32 PageSize );

 // Main Destructor
void EmuMemShutdown( void );

 // Clear memory
void EmuMemClear( void );

 // Gets a real memory pointer to the Address specified
EMU_U08 * EmuMemGetRealPointer( EMU_U32 Address );

// Get a Byte(8 bits) at Address
EMU_U08 EmuMemGetByte( EMU_U32 Address );
 // Get a Short(16 bits) at Address
EMU_U16 EmuMemGetShort( EMU_U32 Address );
 // Get a Word(32 bits) at Address
EMU_U32 EmuMemGetWord( EMU_U32 Address );
 // Get a DWord(64 bits) at Address
EMU_U64 EmuMemGetDWord( EMU_U32 Address );

 // Sets a Byte(8 bits) at Address
void EmuMemSetByte( EMU_U32 Address, EMU_U08 Data );
 // Sets a Short(16 bits) at Address
void EmuMemSetShort( EMU_U32 Address, EMU_U16 Data );
 // Sets a Word(32 bits) at Address
void EmuMemSetWord( EMU_U32 Address, EMU_U32 Data );
 // Sets a DWord(64 bits) at Address
void EmuMemSetDWord( EMU_U32 Address, EMU_U64 Data );

 // Gets a time value (can be anything, not related to real time)
EMU_U64 EmuMemGetTime( void );

 // Writes a file part to memory
bool EmuMemAddFromFile( const char * FileName,
                  EMU_U32 Offset,
                  EMU_U32 FileSize,
                  EMU_U32 EmuAddress,
                  EMU_U08 Permissions );

 // Adds a callback function to call when the Address is written
void EmuMemAddWriteCallBack( EMU_U32 StartAddress,
                       EMU_U32 EndAddress,
                       EMM_WRITECALLBACK CallBack );

 // Adds a callback function to call when the Address is read
void EmuMemAddReadCallBack( EMU_U32 StartAddress,
                      EMU_U32 EndAddress,
                      EMM_READCALLBACK CallBack );

EMU_U08 * EmuMemReadContinuosArea( EMU_U32 Address, EMU_U32 Size );

void EmuMemReplace( EMU_U32 Address );

/*
class CEmuData
{
public:
    CEmuData( void );
    CEmuData( EMU_U08 * pData );
    CEmuData( EMU_U16 * pData );
    CEmuData( EMU_U32 * pData );
    void operator=( EMU_U08 * pData );
    void operator=( EMU_U16 * pData );
    void operator=( EMU_U32 * pData );
    operator unsigned char( );
    operator unsigned short( );
    operator unsigned int( );

    EMM_Data_Type Data;
};
*/

#endif
