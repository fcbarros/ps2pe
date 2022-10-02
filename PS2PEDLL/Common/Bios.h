#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	typedef void (*EMUBIOSFUNCTIONS)();

	extern const char* Emu_Bios_Calls_Names[256];

	class Bios final
	{
	public:
		static const EMU_U32 EMU_BIOS_START_ADDR = 0x1FC00000;
		static const EMU_U32 EMU_BIOS_END_ADDR = 0x1FFFFFFF;

		void Load(const char* FileName);
		void Init();

		EMU_U08* GetPointer(EMU_U32 Address);

		EMU_U08* GetPsR();

		void Syscall();

		static Bios& GetInstance();

		Bios(const Bios&) = delete;
		Bios(Bios&&) = delete;
		Bios& operator=(const Bios&) = delete;
		Bios& operator=(Bios&&) = delete;

	private:
		Bios();

		EMU_U08 EmuBiosMemory[4 * 1024 * 1024];

		EMU_U08* psR = EmuBiosMemory;

		EMUBIOSFUNCTIONS Emu_Bios_Function[256];
	};
}