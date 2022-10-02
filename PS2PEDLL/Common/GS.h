#pragma once

#include "EmuPS2Core.h"
#include "Pad.h"

namespace Common
{
	struct stEmu_GS_Privileg_Regs
	{
		EMU_U64 PMODE;
		EMU_U08 null0[8];
		EMU_U64 SMODE1;
		EMU_U08 null1[8];
		EMU_U64 SMODE2;
		EMU_U08 null2[8];
		EMU_U64 SRFSH;
		EMU_U08 null3[8];
		EMU_U64 SYNCH1;
		EMU_U08 null4[8];
		EMU_U64 SYNCH2;
		EMU_U08 null5[8];
		EMU_U64 SYNCV;
		EMU_U08 null6[8];
		EMU_U64 DISPFB1;
		EMU_U08 null7[8];
		EMU_U64 DISPLAY1;
		EMU_U08 null8[8];
		EMU_U64 DISPFB2;
		EMU_U08 null9[8];
		EMU_U64 DISPLAY2;
		EMU_U08 nullA[8];
		EMU_U64 EXTBUF;
		EMU_U08 nullB[8];
		EMU_U64 EXTDATA;
		EMU_U08 nullC[8];
		EMU_U64 EXTWRITE;
		EMU_U08 nullD[8];
		EMU_U64 BGCOLOR;
		EMU_U08 nullE[3864];
		EMU_U64 CSR;
		EMU_U08 nullF[8];
		EMU_U64 IMR;
		EMU_U08 nullG[40];
		EMU_U64 BUSDIR;
		EMU_U08 nullH[56];
		EMU_U64 SIGBLID;
	};

#define DLLCALL	__stdcall

	typedef EMU_U32(DLLCALL* GS_PS2EgetLibType)();
	typedef EMU_U32(DLLCALL* GS_PS2EgetLibVersion)();
	typedef char* (DLLCALL* GS_PS2EgetLibName)();

	// GS
	typedef EMU_I32(DLLCALL* GS_GSinit)();
	typedef EMU_I32(DLLCALL* GS_GSopen)(void* pDsp, const char* Title);
	typedef void (DLLCALL* GS_GSclose)();
	typedef void (DLLCALL* GS_GSshutdown)();
	typedef void (DLLCALL* GS_GSvsync)();

	typedef void (DLLCALL* GS_GSwrite32)(EMU_U32 mem, EMU_U32 value);
	typedef void (DLLCALL* GS_GSwrite64)(EMU_U32 mem, EMU_U64 value);
	typedef EMU_U32(DLLCALL* GS_GSread32)(EMU_U32 mem);
	typedef EMU_U64(DLLCALL* GS_GSread64)(EMU_U32 mem);

	typedef void (CALLBACK* GS_GSwritePReg)(EMU_U32 mem);
	typedef void (CALLBACK* GS_GSpreg)(void* pPReg);

	typedef void (DLLCALL* GS_GSgifTransfer)(EMU_U32* pMem, EMU_U32 size);
	typedef void (DLLCALL* GS_GSkeyEvent)(keyEvent* ev);

	typedef void (DLLCALL* GS_GSmakeSnapshot)();
	typedef void (DLLCALL* GS_GSconfigure)();
	typedef EMU_I32(DLLCALL* GS_GStest)();
	typedef void (DLLCALL* GS_GSabout)();

	// For Bios Call
	void Emu_GS_Bios_GetIMR();
	void Emu_GS_Bios_PutIMR();
	void Emu_GS_Bios_SetGsCrt();

	class Gs
	{
	public:
		// PS2 GS clock 147.456 Mhz
		static const EMU_U32 GS_CORE_CLOCK = 147456000;	// hz
		// Core clock / 16
		static const EMU_U32 GS_CORE_CLOCK_16 = 9216000;	// hz
		// Core clock / 256
		static const EMU_U32 GS_CORE_CLOCK_256 = 576000;	// hz

		// Base Address por privileged Registers
		static const EMU_U32 EMU_GS_PRIV_START_ADDR = 0x12000000;
		static const EMU_U32 EMU_GS_PRIV_END_ADDR = 0x13FFFFFF;

		static const EMU_U32 EMU_GS_PRIV_ADDR_PMODE = 0x00000000;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SMODE1 = 0x00000010;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SMODE2 = 0x00000020;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SRFSH = 0x00000030;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SYNCH1 = 0x00000040;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SYNCH2 = 0x00000050;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SYNCV = 0x00000060;
		static const EMU_U32 EMU_GS_PRIV_ADDR_DISPFB1 = 0x00000070;
		static const EMU_U32 EMU_GS_PRIV_ADDR_DISPLAY1 = 0x00000080;
		static const EMU_U32 EMU_GS_PRIV_ADDR_DISPFB2 = 0x00000090;
		static const EMU_U32 EMU_GS_PRIV_ADDR_DISPLAY2 = 0x000000A0;
		static const EMU_U32 EMU_GS_PRIV_ADDR_EXTBUF = 0x000000B0;
		static const EMU_U32 EMU_GS_PRIV_ADDR_EXTDATA = 0x000000C0;
		static const EMU_U32 EMU_GS_PRIV_ADDR_EXTWRITE = 0x000000D0;
		static const EMU_U32 EMU_GS_PRIV_ADDR_BGCOLOR = 0x000000E0;
		static const EMU_U32 EMU_GS_PRIV_ADDR_CSR = 0x00001000;
		static const EMU_U32 EMU_GS_PRIV_ADDR_IMR = 0x00001010;
		static const EMU_U32 EMU_GS_PRIV_ADDR_BUSDIR = 0x00001040;
		static const EMU_U32 EMU_GS_PRIV_ADDR_SIGLBLID = 0x00001080;

		// Initializes GS memory pointers and registers
		void Init();
		// Resets registers and GS plugin
		void Reset();
		// callback for write access
		static void WriteCallback(EMU_U32 Address);
		// callback for read access
		void ReadCallback(EMU_U32 Address, EMU_U08* RealAddress);
		// Inits and creates the GS window
		void InitWindow();
		// Calls the GS plugin configure method
		void Configure();
		// Closes the GS window
		void CloseWindow();
		// Shutsdown GS, deallocating memory
		void Shutdown();
		// Flushes GS fifo
		void Flush();
		// Processes Window and GS messages
		void ProcessMessages();

		EMU_U08* GetPointer(EMU_U32 Address);

		static GS_GSgifTransfer  GSgifTransfer;
		static GS_GSinit         GSinit;
		static GS_GSopen         GSopen;
		static GS_GSclose        GSclose;
		static GS_GSshutdown     GSshutdown;
		static GS_GSvsync        GSvsync;

		static GS_GSwrite32      GSwrite32;
		static GS_GSwrite64      GSwrite64;
		static GS_GSread32       GSread32;
		static GS_GSread64       GSread64;

		static GS_GSkeyEvent     GSkeyEvent;
		static GS_GSmakeSnapshot GSmakeSnapshot;
		static GS_GSconfigure    GSconfigure;
		static GS_GStest         GStest;
		static GS_GSabout        GSabout;
		static GS_GSwritePReg    GSwritePReg;
		static GS_GSpreg         GSpreg;

		static Gs& GetInstance();

		Gs(const Gs&) = delete;
		Gs(Gs&&) = delete;
		Gs& operator=(const Gs&) = delete;
		Gs& operator=(Gs&&) = delete;

	private:
		Gs();

		stEmu_GS_Privileg_Regs* Emu_GS_Privileg_Reg;

#ifdef __WIN32__
		HWND hWnd_GS = 0;
#else
		int hWnd_GS = 0;
#endif

		EMU_U64 TicksMinV;
		EMU_U64 TicksMinH;
		EMU_U64 TicksMinVCK;

		EMU_U64 GSTargetVSync;
		EMU_U64 GSTargetHSync;

		keyEvent GSKey;

		EMU_U08 EmuGsMemory[EMU_GS_PRIV_END_ADDR - EMU_GS_PRIV_START_ADDR];
	};
}