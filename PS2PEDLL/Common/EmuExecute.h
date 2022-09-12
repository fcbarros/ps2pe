#pragma once

#include <math.h>
#include "EmuMain.h"
#include "EmuGS.h"
#include "EmuVU.h"
#include "EmuDma.h"
#include "EmuIntc.h"
#include "EmuBios.h"
#include "EmuRecompiler.h"

extern EMU_U32 EmuFloatStatus;
extern EMU_U32 EmuIntegerStatus;

extern EMU_U32 RS;
extern EMU_U32 RD;
extern EMU_U32 RT;
extern EMU_U32 FS;
extern EMU_U32 FT;
extern EMU_U32 SA;
extern EMU_U32 Address;
extern EMU_I32 TempI32;
extern EMU_U32 TempU32;
extern EMU_I64 TempI64;
extern EMU_U64 TempU64;
extern EMU_128 Temp128;
extern EMU_U32 RegRTU32;
extern EMU_U64 RegRTU64;
extern EMU_F32 TempF32;
extern const EMU_U16 MaxU16;
extern const EMU_U32 MaxU32;
extern const EMU_U64 MaxU64;
extern EMU_U32 JumpTo;

extern EMU_U64 CpuCycles;

extern bool DebugMode;

EMU_U32 clamp(EMU_U32 X);

void EmuCore(EMU_U32 Code);
void EmuCop0(EMU_U32 Code);
void EmuCop1(EMU_U32 Code);
void EmuCop2(EMU_U32 Code);
void EmuMmi(EMU_U32 Code);
void EmuRegimm(EMU_U32 Code);
void EmuSpecial(EMU_U32 Code);