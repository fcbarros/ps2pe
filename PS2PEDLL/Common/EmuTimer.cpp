#include "EmuMain.h"
#include "EmuTimer.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EMU_U08 EmuTimerMemory[EMU_TIMER_END_ADDR - EMU_TIMER_START_ADDR];

// Pointer to control1 registers
stEmu_Timer_Control_Regs* Emu_Timer_Control_Reg[4];

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Timer_Init()
{
	Emu_Timer_Control_Reg[0] = (stEmu_Timer_Control_Regs*)&EmuTimerMemory[EMU_TIMER0 - EMU_TIMER_START_ADDR];
	Emu_Timer_Control_Reg[1] = (stEmu_Timer_Control_Regs*)&EmuTimerMemory[EMU_TIMER1 - EMU_TIMER_START_ADDR];
	Emu_Timer_Control_Reg[2] = (stEmu_Timer_Control_Regs*)&EmuTimerMemory[EMU_TIMER2 - EMU_TIMER_START_ADDR];
	Emu_Timer_Control_Reg[3] = (stEmu_Timer_Control_Regs*)&EmuTimerMemory[EMU_TIMER3 - EMU_TIMER_START_ADDR];

	Emu_Timer_Reset();
}

void Emu_Timer_Reset()
{
	memset(Emu_Timer_Control_Reg[0], 0, sizeof(stEmu_Timer_Control_Regs));
	memset(Emu_Timer_Control_Reg[1], 0, sizeof(stEmu_Timer_Control_Regs));
	memset(Emu_Timer_Control_Reg[2], 0, sizeof(stEmu_Timer_Control_Regs));
	memset(Emu_Timer_Control_Reg[3], 0, sizeof(stEmu_Timer_Control_Regs));
}

void Emu_Timer_callback(EMU_U32 Address)
{
	EMU_U32 Register = Address;

	if (Address < EMU_TIMER1)
	{
		Register -= EMU_TIMER0;
	}
	else if (Address < EMU_TIMER2)
	{
		Register -= EMU_TIMER1;
	}
	else if (Address < EMU_TIMER3)
	{
		Register -= EMU_TIMER2;
	}
	else
	{
		Register -= EMU_TIMER3;
	}

	switch (Register)
	{
	case EMU_TIMER_COUNT:
	case EMU_TIMER_MODE:
	case EMU_TIMER_COMP:
	case EMU_TIMER_HOLD:
		break;
	}
}

EMU_U08* Emu_Timer_GetPointer(EMU_U32 Address)
{
	return &EmuTimerMemory[Address - EMU_TIMER_START_ADDR];
}
