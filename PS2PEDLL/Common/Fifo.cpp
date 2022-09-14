#include "Fifo.h"

namespace Common
{
	Fifo::Fifo()
	{
	}

	Fifo& Fifo::GetInstance()
	{
		static Fifo fifo;
		return fifo;
	}

	void Fifo::Init()
	{
		//    Emu_Fifo_Control_Reg_VIF0 = (stEmu_Fifo_Control_Regs *) EMemory.GetRealPointer( EMU_FIFO_VIF0 );
		//    Emu_Fifo_Control_Reg_VIF1 = (stEmu_Fifo_Control_Regs *) EMemory.GetRealPointer( EMU_FIFO_VIF1 );
		//    Emu_Fifo_Control_Reg_GIF  = (stEmu_Fifo_Control_Regs *) EMemory.GetRealPointer( EMU_FIFO_GIF );
		//    Emu_Fifo_Control_Reg_OUT  = (stEmu_Fifo_Control_Regs *) EMemory.GetRealPointer( EMU_FIFO_OUT );
		//    Emu_Fifo_Control_Reg_IN   = (stEmu_Fifo_Control_Regs *) EMemory.GetRealPointer( EMU_FIFO_IN );
		Emu_Fifo_Control_Reg_VIF0 = (stEmu_Fifo_Control_Regs*)&EmuFifoMemory[EMU_FIFO_VIF0 - EMU_FIFO_START_ADDR];
		Emu_Fifo_Control_Reg_VIF1 = (stEmu_Fifo_Control_Regs*)&EmuFifoMemory[EMU_FIFO_VIF1 - EMU_FIFO_START_ADDR];
		Emu_Fifo_Control_Reg_GIF = (stEmu_Fifo_Control_Regs*)&EmuFifoMemory[EMU_FIFO_GIF - EMU_FIFO_START_ADDR];
		Emu_Fifo_Control_Reg_OUT = (stEmu_Fifo_Control_Regs*)&EmuFifoMemory[EMU_FIFO_OUT - EMU_FIFO_START_ADDR];
		Emu_Fifo_Control_Reg_IN = (stEmu_Fifo_Control_Regs*)&EmuFifoMemory[EMU_FIFO_IN - EMU_FIFO_START_ADDR];

		Reset();
	}

	void Fifo::Reset()
	{
		memset(Emu_Fifo_Control_Reg_VIF0, 0, sizeof(stEmu_Fifo_Control_Regs));
		memset(Emu_Fifo_Control_Reg_VIF1, 0, sizeof(stEmu_Fifo_Control_Regs));
		memset(Emu_Fifo_Control_Reg_GIF, 0, sizeof(stEmu_Fifo_Control_Regs));
		memset(Emu_Fifo_Control_Reg_OUT, 0, sizeof(stEmu_Fifo_Control_Regs));
		memset(Emu_Fifo_Control_Reg_IN, 0, sizeof(stEmu_Fifo_Control_Regs));
	}

	void Fifo::Callback(EMU_U32 Address)
	{
		EMU_U32 Register = Address;

		if (Address < EMU_FIFO_VIF1)
		{
			// VIF0
		}
		else if (Address < EMU_FIFO_GIF)
		{
			// VIF1
		}
		else if (Address < EMU_FIFO_OUT)
		{
			// GIF
		}
		else if (Address < EMU_FIFO_IN)
		{
			// OUT
		}
		else
		{
			// IN
		}
	}

	EMU_U08* Fifo::GetPointer(EMU_U32 Address)
	{
		return &EmuFifoMemory[Address - EMU_FIFO_START_ADDR];
	}
}