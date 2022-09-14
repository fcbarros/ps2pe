#include "Thread.h"
#include "Memory.h"

#include "EmuMain.h"

namespace Common
{
	struct EmuThread
	{
		EMU_I32 status;
		void* entry;
		void* stack;
		EMU_I32 stackSize;
		void* gpReg;
		EMU_I32 initPriority;
		EMU_I32 currentPriority;
		EMU_U32 attr;
		EMU_U32 option;
		EMU_I32 waitType;
		EMU_I32 waitId;
		EMU_I32 wakeupCount;
	};

	Thread::Thread()
	{
	}

	Thread& Thread::GetInstance()
	{
		static Thread thread;
		return thread;
	}

	void Thread::Init()
	{
		Reset();
	}

	void Thread::Reset()
	{
		memset(EmuThreads, 0, sizeof(EmuThreads));

		Emu_Thread_CurrentId = 0;
		Emu_Thread_NumThreads = 0;
	}

	//
	//  int CreateThread(kThread *);
	//   returns an id
	//
	EMU_U64 Thread::Create(EMU_U32 Address)
	{ // 0x20
		EmuThread* thread;

		EmuThreads[Emu_Thread_NumThreads].Id = Emu_Thread_NumThreads;
		EmuThreads[Emu_Thread_NumThreads].Address = Address;
		EmuThreads[Emu_Thread_NumThreads].RealAddress = Memory::GetInstance().GetRealPointer(Address);
		EmuThreads[Emu_Thread_NumThreads].Running = 0;
		thread = (EmuThread*)EmuThreads[Emu_Thread_NumThreads].RealAddress;

		Emu_Thread_NumThreads++;

		return Emu_Thread_NumThreads - 1;
	}

	//
	//  int StartThread(int thread_id, void *arg);
	//
	void Thread::Start(EMU_U32 Address)
	{ // 0x22
		EmuThreads[Address].Running = 1;

		EmuThread* thread = (EmuThread*)Memory::GetInstance().GetRealPointer(Address);
	}

	//
	//  int GetThreadId();
	//
	EMU_U64 Thread::GetId()
	{ // 0x2f
		return Emu_Thread_CurrentId;
	}

	//
	//  int ReferThreadStatus(int, kThread *);
	//
	EMU_U64 Thread::ReferStatus(EMU_U32 Address)
	{ // 0x30
		EmuThread* thread;

		thread = (EmuThread*)Memory::GetInstance().GetRealPointer(Address);

		return 1;
	}

	void Emu_Thread_Bios_Create()
	{ // 0x20
		PS2Regs.R5900Regs.V0.u64_00_63 = Thread::GetInstance().Create(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Emu_Thread_Bios_Start()
	{ // 0x22
		Thread::GetInstance().Start(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Emu_Thread_Bios_GetId()
	{ // 0x2f
		PS2Regs.R5900Regs.V0.u64_00_63 = Thread::GetInstance().GetId();
	}

	void Emu_Thread_Bios_ReferStatus()
	{ // 0x30
		PS2Regs.R5900Regs.V0.u64_00_63 = Thread::GetInstance().ReferStatus(PS2Regs.R5900Regs.A1.u32_00_31);
	}
}