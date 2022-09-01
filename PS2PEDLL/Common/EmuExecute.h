#pragma once

#include <math.h>
#include "EmuMain.h"
#include "EmuGS.h"
#include "EmuVU.h"
#include "EmuDma.h"
#include "EmuIntc.h"
#include "EmuBios.h"
#include "EmuRecompiler.h"

const EMU_U16 MaxU16 = 0xFFFF;
const EMU_U32 MaxU32 = 0xFFFFFFFF;
const EMU_U64 MaxU64 = 0xFFFFFFFFFFFFFFFF;

struct stEmuTemp
{
    EMU_U32 EmuFloatStatus;

    //EMU_U32 SA;
    EMU_U32 Address;
    EMU_I32 TempI32;
    EMU_U32 TempU32;
    EMU_I64 TempI64;
    EMU_U64 TempU64;
    EMU_128 Temp128;
    EMU_U32 RegRTU32;
    EMU_U64 RegRTU64;
    EMU_F32 TempF32;
    EMU_U64 CpuCycles = 0;
};

inline EMU_U32 clamp(EMU_U32 X)
{
    if (X > 0x00007FFF)
    {
        return 0x7FFF;
    }
    else
        if (X < 0xFFFF8000)
        {
            return 0x8000;
        }
        else
        {
            return X;
        }
}
