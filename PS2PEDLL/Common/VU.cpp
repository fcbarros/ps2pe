#include "VU.h"

namespace Common
{
	Vu::Vu()
	{
	}

	Vu& Vu::GetInstance()
	{
		static Vu vu;
		return vu;
	}

	void Vu::Init()
	{
		Reset();
	}

	void Vu::Reset()
	{
		memset(&VU0Regs, 0, sizeof(EMU_VU_Regs));
		memset(&VU1Regs, 0, sizeof(EMU_VU_Regs));

		VU0Regs.CPR[0].W.f = 1.0f;
		VU1Regs.CPR[0].W.f = 1.0f;
	}

	EMU_U08* Vu::GetPointer(EMU_U32 Address)
	{
		return &EmuVuMemory[Address - EMU_VU_START_ADDR];
	}
}