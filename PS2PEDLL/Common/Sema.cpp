#include "Sema.h"
#include "Memory.h"

#include "EmuMain.h"

namespace Common
{
	Sema::Sema()
	{
	}

	Sema& Sema::GetInstance()
	{
		static Sema sema;
		return sema;
	}

	void Sema::Init(void)
	{
		Reset();
	}

	void Sema::Reset(void)
	{
	}

	EMU_U64 Sema::Create(EMU_U32 SemaAddress)
	{ // 0x40
		t_sema* sema;
		static EMU_U64 semaid;

		semaid++;
		sema = (t_sema*)Memory::GetInstance().GetRealPointer(SemaAddress);
		ksema[semaid - 1] = SemaAddress;

		return semaid;
	}

	EMU_U64 Sema::Delete(EMU_U32 SemaIndex)
	{ // 0x41
		return SemaIndex;
	}

	EMU_U64 Sema::Signal(EMU_U32 SemaIndex)
	{ // 0x42
		return SemaIndex;
	}

	EMU_U64 Sema::Wait(EMU_U32 SemaIndex)
	{ // 0x44
		return SemaIndex;
	}

	EMU_U64 Sema::Poll(EMU_U32 SemaIndex)
	{ // 0x45
		return SemaIndex;
	}

	void Emu_Sema_Bios_Create(void)
	{ // 0x40
		PS2Regs.R5900Regs.V0.u64_00_63 = Sema::GetInstance().Create(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Emu_Sema_Bios_Delete(void)
	{ // 0x41
		PS2Regs.R5900Regs.V0.u64_00_63 = Sema::GetInstance().Delete(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Emu_Sema_Bios_Signal(void)
	{ // 0x42
		PS2Regs.R5900Regs.V0.u64_00_63 = Sema::GetInstance().Signal(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Emu_Sema_Bios_Wait(void)
	{ // 0x44
		PS2Regs.R5900Regs.V0.u64_00_63 = Sema::GetInstance().Wait(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Emu_Sema_Bios_Poll(void)
	{ // 0x44
		PS2Regs.R5900Regs.V0.u64_00_63 = Sema::GetInstance().Poll(PS2Regs.R5900Regs.A0.u32_00_31);
	}
}