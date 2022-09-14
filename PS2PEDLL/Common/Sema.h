#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct t_sema
	{
		int count,
			max_count,
			init_count,
			wait_threads;
		EMU_U32 attr,
			option;
	};

	void Emu_Sema_Bios_Create(void);
	void Emu_Sema_Bios_Delete(void);
	void Emu_Sema_Bios_Signal(void);
	void Emu_Sema_Bios_Wait(void);
	void Emu_Sema_Bios_Poll(void);

	class Sema
	{
	public:
		// Initializes Semaphores
		void Init(void);
		// Resets Semaphores
		void Reset(void);

		EMU_U64 Create(EMU_U32 SemaAddress);
		EMU_U64 Delete(EMU_U32 SemaIndex);
		EMU_U64 Signal(EMU_U32 SemaIndex);
		EMU_U64 Wait(EMU_U32 SemaIndex);
		EMU_U64 Poll(EMU_U32 SemaIndex);

		static Sema& GetInstance();

		Sema(const Sema&) = delete;
		Sema(Sema&&) = delete;
		Sema& operator=(const Sema&) = delete;
		Sema& operator=(Sema&&) = delete;

	private:
		Sema();

		unsigned long ksema[32];
	};
}