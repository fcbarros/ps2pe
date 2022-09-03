
#include <math.h>
#include "EmuMain.h"
#include "EmuGS.h"
#include "EmuVU.h"
#include "EmuDma.h"
#include "EmuIntc.h"
#include "EmuBios.h"


#define R_RD            ( ( Code >> 11 ) & 0x1F )
#define R_RT            ( ( Code >> 16 ) & 0x1F )
#define R_RS            ( ( Code >> 21 ) & 0x1F )
#define R_SA            ( ( Code >> 06 ) & 0x1F )

#define R_FD            ( ( Code >> 06 ) & 0x1F )
#define R_FS            ( ( Code >> 11 ) & 0x1F )
#define R_FT            ( ( Code >> 16 ) & 0x1F )

//#define R_JUMP          ( ( Code & 0x03FFFFFF ) << 2 )
#define R_BRANCH        (   Code & 0xFFFF )
#define R_IMMEDIATE     (   Code & 0xFFFF )

#define VU_DEST_X       ( ( Code >> 24 ) & 0x01 )
#define VU_DEST_Y       ( ( Code >> 23 ) & 0x01 )
#define VU_DEST_Z       ( ( Code >> 22 ) & 0x01 )
#define VU_DEST_W       ( ( Code >> 21 ) & 0x01 )
#define VU_FT           ( ( Code >> 16 ) & 0x1F )
#define VU_FS           ( ( Code >> 11 ) & 0x1F )
#define VU_FD           ( ( Code >> 06 ) & 0x1F )
#define VU_BC           (   Code & 0x03 )
#define VU_FTF          ( ( Code >> 23 ) & 0x03 )
#define VU_FSF          ( ( Code >> 21 ) & 0x03 )
#define VU_IT           ( ( Code >> 16 ) & 0x1F )
#define VU_IS           ( ( Code >> 11 ) & 0x1F )
#define VU_IMM5         ( ( Code >> 06 ) & 0x1F )
#define VU_IMM15        ( ( Code >> 06 ) & 0x7FFF )
#define VU_RT           ( ( Code >> 16 ) & 0x1F )
#define VU_ID           ( ( Code >> 11 ) & 0x1F )
#define VU_BASE         ( ( Code >> 21 ) & 0x1F )
#define VU_OFFSET       (   Code & 0xFFFF )

#define JUMP_ADDRESS    ( ( Code & 0x03FFFFFF ) << 2 ) | ( R5900Regs.PC & 0xF0000000 )

#define EXECUTE_BRANCH_DELAY( addr )    \
        EmuInBranchDelay = true; \
        EMUFUNCTION( addr, false ); \
        EmuInBranchDelay = false; \


EMU_U32 EmuFloatStatus;
EMU_U32 EmuIntegerStatus;

EMU_U32 RS;
EMU_U32 RD;
EMU_U32 RT;
EMU_U32 FS;
EMU_U32 FT;
EMU_U32 SA;
EMU_U32 Address;
EMU_I32 TempI32;
EMU_U32 TempU32;
EMU_I64 TempI64;
EMU_U64 TempU64;
EMU_128 Temp128;
EMU_U32 RegRTU32;
EMU_U64 RegRTU64;
EMU_F32 TempF32;
const EMU_U16 MaxU16 =  0xFFFF;
const EMU_U32 MaxU32 =  0xFFFFFFFF;
const EMU_U64 MaxU64 =  0xFFFFFFFFFFFFFFFF;

EMU_U64 CpuCycles = 0;

inline EMU_U32 clamp( EMU_U32 X )
{
    if ( X > 0x00007FFF ) 
    {
        return 0x7FFF; 
    } 
    else 
    if ( X < 0xFFFF8000 ) 
    {
        return 0x8000;
    }
    else 
    {
        return X;
    }
}

EMU_U32 EmuInstructionIndex2( EMU_U32 tInst );

void EmuExecuteFast( EMU_U32 tAddress, bool InLoop )
{
   #define EMUFUNCTION  EmuExecuteFast
   #undef EMU_STATS
   #include "EmuExecute.inc"
}

void EmuRunDebug( EMU_U32 tAddress, bool InLoop )
{
   #undef EMUFUNCTION
   #define EMUFUNCTION  EmuRunDebug
   #define EMU_STATS
   #include "EmuExecute.inc"
}
