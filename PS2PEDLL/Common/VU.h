#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	class Vu final
	{
	public:
		static const EMU_U32 EMU_VU_START_ADDR = 0x11000000;
		static const EMU_U32 EMU_VU_END_ADDR = 0x11FFFFFF;

		static const EMU_U32 EMU_VU_MICROMEM0 = 0x11000000;
		static const EMU_U32 EMU_VU_MICROMEM1 = 0x11008000;

		static const EMU_U32 EMU_VU_MEM0 = 0x11004000;
		static const EMU_U32 EMU_VU_MEM1 = 0x1100C000;

		// Initialized VU memory pointers and registers
		void Init(void);
		// Resets VU registers
		void Reset(void);

		EMU_U08* GetPointer(EMU_U32 Address);

		static Vu& GetInstance();

		Vu(const Vu&) = delete;
		Vu(Vu&&) = delete;
		Vu& operator=(const Vu&) = delete;
		Vu& operator=(Vu&&) = delete;

	private:
		Vu();

		EMU_U08 EmuVuMemory[EMU_VU_END_ADDR - EMU_VU_START_ADDR];

		// Pointer to control1 registers
		EMU_VU_Regs VU0Regs;
		EMU_VU_Regs VU1Regs;
	};
}