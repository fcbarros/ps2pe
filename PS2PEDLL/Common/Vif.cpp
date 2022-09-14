#include "Vif.h"

namespace Common
{
	Vif::Vif()
	{
	}

	Vif& Vif::GetInstance()
	{
		static Vif vif;
		return vif;
	}

	void Vif::Init()
	{
		Emu_Vif_Control_Reg0 = (stEmu_Vif_Control_Regs*)&EmuVifMemory[EMU_VIF0 - EMU_VIF_START_ADDR];
		Emu_Vif_Control_Reg1 = (stEmu_Vif_Control_Regs*)&EmuVifMemory[EMU_VIF1 - EMU_VIF_START_ADDR];

		Reset();
	}

	void Vif::Reset()
	{
		memset(Emu_Vif_Control_Reg0, 0, sizeof(stEmu_Vif_Control_Regs));
		memset(Emu_Vif_Control_Reg1, 0, sizeof(stEmu_Vif_Control_Regs));

		Emu_Vif_Control_Reg0->STAT = 0x00000000;
		Emu_Vif_Control_Reg0->FBRST = 0x00000000;
		Emu_Vif_Control_Reg0->ERR = 0x00000000;
		Emu_Vif_Control_Reg0->MARK = 0x00000000;
		Emu_Vif_Control_Reg0->CYCLE = 0x00000000;
		Emu_Vif_Control_Reg0->MODE = 0x00000000;
		Emu_Vif_Control_Reg0->NUM = 0x00000000;
		Emu_Vif_Control_Reg0->MASK = 0x00000000;
		Emu_Vif_Control_Reg0->CODE = 0x00000000;
		Emu_Vif_Control_Reg0->ITOPS = 0x00000000;
		Emu_Vif_Control_Reg0->BASE = 0x00000000;
		Emu_Vif_Control_Reg0->OFST = 0x00000000;
		Emu_Vif_Control_Reg0->TOPS = 0x00000000;
		Emu_Vif_Control_Reg0->ITOP = 0x00000000;
		Emu_Vif_Control_Reg0->TOP = 0x00000000;
		Emu_Vif_Control_Reg0->R0 = 0x00000000;
		Emu_Vif_Control_Reg0->R1 = 0x00000000;
		Emu_Vif_Control_Reg0->R2 = 0x00000000;
		Emu_Vif_Control_Reg0->R3 = 0x00000000;
		Emu_Vif_Control_Reg0->C0 = 0x00000000;
		Emu_Vif_Control_Reg0->C1 = 0x00000000;
		Emu_Vif_Control_Reg0->C2 = 0x00000000;
		Emu_Vif_Control_Reg0->C3 = 0x00000000;

		Emu_Vif_Control_Reg1->STAT = 0x00000000;
		Emu_Vif_Control_Reg1->FBRST = 0x00000000;
		Emu_Vif_Control_Reg1->ERR = 0x00000000;
		Emu_Vif_Control_Reg1->MARK = 0x00000000;
		Emu_Vif_Control_Reg1->CYCLE = 0x00000000;
		Emu_Vif_Control_Reg1->MODE = 0x00000000;
		Emu_Vif_Control_Reg1->NUM = 0x00000000;
		Emu_Vif_Control_Reg1->MASK = 0x00000000;
		Emu_Vif_Control_Reg1->CODE = 0x00000000;
		Emu_Vif_Control_Reg1->ITOPS = 0x00000000;
		Emu_Vif_Control_Reg1->BASE = 0x00000000;
		Emu_Vif_Control_Reg1->OFST = 0x00000000;
		Emu_Vif_Control_Reg1->TOPS = 0x00000000;
		Emu_Vif_Control_Reg1->ITOP = 0x00000000;
		Emu_Vif_Control_Reg1->TOP = 0x00000000;
		Emu_Vif_Control_Reg1->R0 = 0x00000000;
		Emu_Vif_Control_Reg1->R1 = 0x00000000;
		Emu_Vif_Control_Reg1->R2 = 0x00000000;
		Emu_Vif_Control_Reg1->R3 = 0x00000000;
		Emu_Vif_Control_Reg1->C0 = 0x00000000;
		Emu_Vif_Control_Reg1->C1 = 0x00000000;
		Emu_Vif_Control_Reg1->C2 = 0x00000000;
		Emu_Vif_Control_Reg1->C3 = 0x00000000;
	}

	void Vif::Callback(EMU_U32 Address)
	{
		EMU_U32 Register = Address;

		if (Address < EMU_VIF1)
		{
			Register -= EMU_VIF0;
		}
		else
		{
			Register -= EMU_VIF1;
		}

		switch (Register)
		{
		case EMU_VIF_STAT:
		case EMU_VIF_FBRST:
		case EMU_VIF_ERR:
		case EMU_VIF_MARK:
		case EMU_VIF_CYCLE:
		case EMU_VIF_MODE:
		case EMU_VIF_NUM:
		case EMU_VIF_MASK:
		case EMU_VIF_CODE:
		case EMU_VIF_ITOPS:
		case EMU_VIF_BASE:
		case EMU_VIF_OFST:
		case EMU_VIF_TOPS:
		case EMU_VIF_ITOP:
		case EMU_VIF_TOP:
		case EMU_VIF_R0:
		case EMU_VIF_R1:
		case EMU_VIF_R2:
		case EMU_VIF_R3:
		case EMU_VIF_C0:
		case EMU_VIF_C1:
		case EMU_VIF_C2:
		case EMU_VIF_C3:
			break;
		}
	}

	EMU_U08* Vif::GetPointer(EMU_U32 Address)
	{
		return &EmuVifMemory[Address - EMU_VIF_START_ADDR];
	}
}