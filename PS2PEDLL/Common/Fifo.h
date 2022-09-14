#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct stEmu_Fifo_Control_Regs
	{
		EMU_U32 Data;
		EMU_U08 null0[12];
	};

	class Fifo
	{
	public:
		// Base Address
		static const EMU_U32 EMU_FIFO_START_ADDR = 0x10004000;
		static const EMU_U32 EMU_FIFO_END_ADDR = 0x10007FFF;

		static const EMU_U32 EMU_FIFO_VIF0 = 0x10004000;
		static const EMU_U32 EMU_FIFO_VIF1 = 0x10005000;
		static const EMU_U32 EMU_FIFO_GIF = 0x10006000;
		static const EMU_U32 EMU_FIFO_OUT = 0x10007000;
		static const EMU_U32 EMU_FIFO_IN = 0x10007010;

		// Initializes the Fifo memory pointers and registers
		void Init(void);
		// Resets registers
		void Reset(void);
		// Callback for Fifo access
		void Callback(EMU_U32 Address);

		EMU_U08* GetPointer(EMU_U32 Address);

		static Fifo& GetInstance();

		Fifo(const Fifo&) = delete;
		Fifo(Fifo&&) = delete;
		Fifo& operator=(const Fifo&) = delete;
		Fifo& operator=(Fifo&&) = delete;

	private:
		Fifo();

		EMU_U08 EmuFifoMemory[EMU_FIFO_END_ADDR - EMU_FIFO_START_ADDR];

		// Pointer to control1 registers
		stEmu_Fifo_Control_Regs* Emu_Fifo_Control_Reg_VIF0;
		stEmu_Fifo_Control_Regs* Emu_Fifo_Control_Reg_VIF1;
		stEmu_Fifo_Control_Regs* Emu_Fifo_Control_Reg_GIF;
		stEmu_Fifo_Control_Regs* Emu_Fifo_Control_Reg_OUT;
		stEmu_Fifo_Control_Regs* Emu_Fifo_Control_Reg_IN;
	};
}