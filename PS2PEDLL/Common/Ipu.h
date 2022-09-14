#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct stEmu_Ipu_Control_Regs
	{
		EMU_U32 CMD;
		EMU_U08 null0[12];
		EMU_U32 CTRL;
		EMU_U08 null1[12];
		EMU_U32 BP;
		EMU_U08 null2[12];
		EMU_U32 TOP;
		EMU_U08 null3[12];
	};

	class Ipu
	{
	public:
		// Base Address
		static const EMU_U32 EMU_IPU_START_ADDR = 0x10002000;
		static const EMU_U32 EMU_IPU_END_ADDR = 0x10002FFF;

		// Control Register Address
		static const EMU_U32 EMU_IPU_CMD = 0x00000000;
		static const EMU_U32 EMU_IPU_CTRL = 0x00000010;
		static const EMU_U32 EMU_IPU_BP = 0x00000020;
		static const EMU_U32 EMU_IPU_TOP = 0x00000030;

		// Initilizes IPU memory pointers and registers
		void Init(void);
		// Resets IPU registers
		void Reset(void);
		// Callback for IPU access
		static void Callback(EMU_U32 Address);

		EMU_U08* GetPointer(EMU_U32 Address);


		static Ipu& GetInstance();

		Ipu(const Ipu&) = delete;
		Ipu& operator=(const Ipu&) = delete;
		Ipu(Ipu&) = delete;
		Ipu& operator=(Ipu&&) = delete;

	private:
		Ipu();

		EMU_U08 EmuIpuMemory[EMU_IPU_END_ADDR - EMU_IPU_START_ADDR];

		// Pointer to control1 registers
		stEmu_Ipu_Control_Regs* Emu_Ipu_Control_Reg;
	};
}