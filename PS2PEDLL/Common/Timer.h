#pragma once

#include "EmuPS2Core.h"

namespace Common
{
    struct stEmu_Timer_Control_Regs
    {
        EMU_U32 COUNT;
        EMU_U08 null0[12];
        EMU_U32 MODE;
        EMU_U08 null1[12];
        EMU_U32 COMP;
        EMU_U08 null2[12];
        EMU_U32 HOLD;
        EMU_U08 null3[12];
    };

    class Timer
    {
    public:
        static const EMU_U32 EMU_TIMER_START_ADDR = 0x10000000;
        static const EMU_U32 EMU_TIMER_END_ADDR = 0x10001FFF;

        // Base Address
        static const EMU_U32 EMU_TIMER0 = 0x10000000;
        static const EMU_U32 EMU_TIMER1 = 0x10000800;
        static const EMU_U32 EMU_TIMER2 = 0x10001000;
        static const EMU_U32 EMU_TIMER3 = 0x10001800;

        // Control Register Address
        static const EMU_U32 EMU_TIMER_COUNT = 0x00000000;
        static const EMU_U32 EMU_TIMER_MODE = 0x00000010;
        static const EMU_U32 EMU_TIMER_COMP = 0x00000020;
        static const EMU_U32 EMU_TIMER_HOLD = 0x00000030;

        // Initializes Timer memory pointers and registers
        void Init(void);
        // Resets Timer registers
        void Reset(void);
        // Callback for Timer access
        static void Callback(EMU_U32 Address);

        EMU_U08* GetPointer(EMU_U32 Address);

        static Timer& GetInstance();

        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        Timer(Timer&&) = delete;
        Timer& operator=(Timer&&) = delete;

    private:
        Timer();

        EMU_U08 EmuTimerMemory[EMU_TIMER_END_ADDR - EMU_TIMER_START_ADDR];

        // Pointer to control registers
        stEmu_Timer_Control_Regs* Emu_Timer_Control_Reg[4];
    };
}