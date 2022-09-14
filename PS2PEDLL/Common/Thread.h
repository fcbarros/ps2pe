#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct Emu_Thread_Struct
	{
		EMU_I32 Id;
		EMU_U08 Running;
		EMU_U32 Address;
		EMU_U08* RealAddress;
	};

	void Emu_Thread_Bios_Create();
	void Emu_Thread_Bios_Start();
	void Emu_Thread_Bios_GetId();
	void Emu_Thread_Bios_ReferStatus();

	class Thread
	{
	public:
		// Base Address
		static const EMU_U32 EMU_Thread_START_ADDR = 0x10003000;
		static const EMU_U32 EMU_Thread_END_ADDR = 0x100037FF;

		// Initializes Thread memory pointers and registers
		void Init(void);
		// Resets Thread registers
		void Reset(void);

		EMU_U64 Create(EMU_U32 Address);
		void Start(EMU_U32 Address);
		EMU_U64 GetId(void);
		EMU_U64 ReferStatus(EMU_U32 Address);

		static Thread& GetInstance();

		Thread(const Thread&) = delete;
		Thread(Thread&&) = delete;
		Thread& operator=(const Thread&) = delete;
		Thread& operator=(Thread&&) = delete;

	private:
		Thread();

		EMU_I32 Emu_Thread_CurrentId = 1;
		EMU_I32 Emu_Thread_NumThreads = 0;
		Emu_Thread_Struct EmuThreads[100];
	};
}