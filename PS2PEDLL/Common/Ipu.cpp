#include "Ipu.h"

namespace Common
{
	Ipu::Ipu()
	{
	}

	Ipu& Ipu::GetInstance()
	{
		static Ipu ipu;
		return ipu;
	}

	void Ipu::Init()
	{
		//    Emu_Ipu_Control_Reg = (stEmu_Ipu_Control_Regs *) EMemory.GetRealPointer( EMU_IPU );
		Emu_Ipu_Control_Reg = (stEmu_Ipu_Control_Regs*)EmuIpuMemory;

		Reset();
	}

	void Ipu::Reset()
	{
		memset(Emu_Ipu_Control_Reg, 0, sizeof(stEmu_Ipu_Control_Regs));
	}

	void Ipu::Callback(EMU_U32 Address)
	{
		EMU_U32 Register = Address - EMU_IPU_START_ADDR;

		switch (Register)
		{
		case EMU_IPU_CMD:
		case EMU_IPU_CTRL:
		case EMU_IPU_BP:
		case EMU_IPU_TOP:
			break;
		}
	}

	EMU_U08* Ipu::GetPointer(EMU_U32 Address)
	{
		return &EmuIpuMemory[Address - EMU_IPU_START_ADDR];
	}
}