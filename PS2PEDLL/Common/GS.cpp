#ifdef __WIN32__
#include <windows.h>
#else
#endif

#include "EmuMain.h"

#include "Ps2Core.h"
#include "Gs.h"
#include "Memory.h"
#include "Intc.h"
#include "Pad.h"

namespace Common
{
	EMU_I32 DLLCALL DummyGSInit() { return 0; }
	EMU_I32 DLLCALL DummyGSopen(void* pDsp, const char* Title) { return 0; }
	void DLLCALL DummyGSclose() {}
	void DLLCALL DummyGSshutdown() {}
	void DLLCALL DummyGSvsync() {}
	void DLLCALL DummyGSwrite32(EMU_U32 mem, EMU_U32 value) {}
	void DLLCALL DummyGSwrite64(EMU_U32 mem, EMU_U64 value) {}
	EMU_U32 DLLCALL DummyGSread32(EMU_U32 mem) { return 0; }
	EMU_U64 DLLCALL DummyGSread64(EMU_U32 mem) { return 0; }
	void CALLBACK DummyGSwritePReg(EMU_U32 mem) {}
	void CALLBACK DummyGSpreg(void* pPReg) {}
	void DLLCALL DummyGSgifTransfer(EMU_U32* pMem, EMU_U32 size) {}
	void DLLCALL DummyGSkeyEvent(keyEvent* ev) {}
	void DLLCALL DummyGSmakeSnapshot() {}
	void DLLCALL DummyGSconfigure() {}
	EMU_I32 DLLCALL DummyGStest() { return 0; }
	void DLLCALL DummyGSabout() {}

	GS_GSinit Gs::GSinit = &DummyGSInit;
	GS_GSopen Gs::GSopen = &DummyGSopen;
	GS_GSclose Gs::GSclose = &DummyGSclose;
	GS_GSshutdown Gs::GSshutdown = &DummyGSshutdown;
	GS_GSvsync Gs::GSvsync = &DummyGSvsync;
	GS_GSwrite32 Gs::GSwrite32 = &DummyGSwrite32;
	GS_GSwrite64 Gs::GSwrite64 = &DummyGSwrite64;
	GS_GSread32 Gs::GSread32 = &DummyGSread32;
	GS_GSread64 Gs::GSread64 = &DummyGSread64;
	GS_GSwritePReg Gs::GSwritePReg = &DummyGSwritePReg;
	GS_GSpreg Gs::GSpreg = &DummyGSpreg;
	GS_GSgifTransfer Gs::GSgifTransfer = &DummyGSgifTransfer;
	GS_GSkeyEvent Gs::GSkeyEvent = &DummyGSkeyEvent;
	GS_GSmakeSnapshot Gs::GSmakeSnapshot = &DummyGSmakeSnapshot;
	GS_GSconfigure Gs::GSconfigure = &DummyGSconfigure;
	GS_GStest Gs::GStest = &DummyGStest;
	GS_GSabout Gs::GSabout = &DummyGSabout;

	Gs::Gs()
	{
	}

	Gs& Gs::GetInstance()
	{
		static Gs gs;
		return gs;
	}

	// Current CPUClock 2894559600.0000000

	void Gs::Init()
	{
		Emu_GS_Privileg_Reg = (stEmu_GS_Privileg_Regs*)EmuGsMemory;

		GSTargetVSync = 60;
		GSTargetHSync = 15734;

		TicksMinV = (EMU_U64)Ps2Core::GetInstance().GetCPUClock() / GSTargetVSync;
		TicksMinH = (EMU_U64)Ps2Core::GetInstance().GetCPUClock() / GSTargetHSync;
		TicksMinVCK = TicksMinH / 10;

		if (GSpreg)
		{
			//        GSpreg( EmuGsMemory );
		}
		GSinit();

		Reset();
	}

	void Gs::Reset()
	{
		memset(Emu_GS_Privileg_Reg, 0, sizeof(stEmu_GS_Privileg_Regs));

		Emu_GS_Privileg_Reg->PMODE = 0x00000040;
		Emu_GS_Privileg_Reg->SMODE1 = 0x00000000;
		Emu_GS_Privileg_Reg->SMODE2 = 0x00000000;
		Emu_GS_Privileg_Reg->SRFSH = 0x00000000;
		Emu_GS_Privileg_Reg->SYNCH1 = 0x00000000;
		Emu_GS_Privileg_Reg->SYNCH2 = 0x00000000;
		Emu_GS_Privileg_Reg->SYNCV = 0x00000000;
		Emu_GS_Privileg_Reg->DISPFB1 = 0x00000000;
		Emu_GS_Privileg_Reg->DISPLAY1 = 0x00000000;
		Emu_GS_Privileg_Reg->DISPFB2 = 0x00000000;
		Emu_GS_Privileg_Reg->DISPLAY2 = 0x00000000;
		Emu_GS_Privileg_Reg->EXTBUF = 0x00000000;
		Emu_GS_Privileg_Reg->EXTDATA = 0x00000000;
		Emu_GS_Privileg_Reg->EXTWRITE = 0x00000000;
		Emu_GS_Privileg_Reg->BGCOLOR = 0x00000000;
		Emu_GS_Privileg_Reg->CSR = 0x00000000; // 0x0000000000000200
		Emu_GS_Privileg_Reg->IMR = 0x00000000; // 0x000000000000ff00
		Emu_GS_Privileg_Reg->BUSDIR = 0x00000000;
		Emu_GS_Privileg_Reg->SIGBLID = 0x00000000;

		CloseWindow();
	}

	void Gs::InitWindow()
	{
		if (hWnd_GS == 0)
		{
			GSopen(&hWnd_GS, "PS2 Personal Emulator");
			Pad::GetInstance().PAD1init(3);
			Pad::GetInstance().PAD1open(&hWnd_GS);
		}
	}

	void Gs::Configure()
	{
		GSconfigure();
	}

	void Gs::CloseWindow()
	{
		if (hWnd_GS)
		{
			GSclose();
			Pad::GetInstance().PAD1close();
			Pad::GetInstance().PAD1shutdown();
			hWnd_GS = 0;
		}
	}

	void Gs::Shutdown()
	{
		CloseWindow();
		GSshutdown();
	}

	void Gs::Flush()
	{
	}

	void Gs::ProcessMessages()
	{
		if (hWnd_GS)
		{
			static EMU_U64 StartV = 0,
				StartH = 0,
				End = 0;
			static keyEvent* PADKeyPressed;

			RDTSC(End);

			if ((End - StartV) > TicksMinV)
			{
				Emu_GS_Privileg_Reg->CSR |= 0x0000000C;
				Intc::GetInstance().GetControlReg()->STAT_GS = 1;
				Intc::GetInstance().CallHandler(2);
				GSvsync();
				//            Emu_Intc_CallHandler( 3 );

				RDTSC(StartV);
				StartH = StartV;
				Emu_PAD_Bios_ReadStatus(1);
				Emu_PAD_Bios_ReadStatus(2);
				if ((PADKeyPressed = Pad::GetInstance().PAD1keyEvent()))
				{
					if (PADKeyPressed->key == VK_ESCAPE)
					{
						CloseWindow();
						Ps2Core::GetInstance().StopRun(true);
					}
					else
					{
						GSkeyEvent(PADKeyPressed);
					}
				}
			}
			else if ((End - StartH) > TicksMinH)
			{
				//            Emu_GS_Privileg_Reg->CSR &= 0xFFFFFFF3;
				Emu_GS_Privileg_Reg->CSR |= 0x00000004;
				Intc::GetInstance().GetControlReg()->STAT_GS = 1;

				RDTSC(StartH);
			}
			else
			{
				Emu_GS_Privileg_Reg->CSR &= 0xFFFFFFF3;
				Intc::GetInstance().GetControlReg()->STAT_GS = 0;
			}
		}
	}

	void Gs::WriteCallback(EMU_U32 Address)
	{
#ifdef EMU_LOG
		EmuLog("GS: %.8X = %.16X\n", Address, Memory::GetInstance().GetDWord(Address));
#endif
		//    if ( GSwritePReg )
		{
			//        GSwritePReg( Address );
		}
		//    else
		{
			Gs::GetInstance().GSwrite64(Address, Memory::GetInstance().GetDWord(Address));
		}
	}

	void Gs::ReadCallback(EMU_U32 Address, EMU_U08* RealAddress)
	{
	}

	//
	//  iGsGetIMR();
	//
	void Emu_GS_Bios_GetIMR()
	{ // 0x70
		PS2Regs.R5900Regs.V0.u64_00_63 = Memory::GetInstance().GetDWord(0x12001010);
	}

	//
	//  void iGsPetIMR(u64 IMR);
	//
	void Emu_GS_Bios_PutIMR()
	{ // 0x71
		Memory::GetInstance().SetDWord(0x12001010, PS2Regs.R5900Regs.A0.u64_00_63);
	}

	void Emu_GS_Bios_SetGsCrt()
	{ // 0x02
	}

	EMU_U08* Gs::GetPointer(EMU_U32 Address)
	{
		return &EmuGsMemory[Address - EMU_GS_PRIV_START_ADDR];
	}
	/*
		// General Purpose Registers Codes
	#define EMU_GS_REG_PRIM                 0x00
	#define EMU_GS_REG_RGBAQ                0x01
	#define EMU_GS_REG_ST                   0x02
	#define EMU_GS_REG_UV                   0x03
	#define EMU_GS_REG_XYZF2                0x04
	#define EMU_GS_REG_XYZ2                 0x05
	#define EMU_GS_REG_TEX0_1               0x06
	#define EMU_GS_REG_TEX0_2               0x07
	#define EMU_GS_REG_CLAMP_1              0x08
	#define EMU_GS_REG_CLAMP_2              0x09
	#define EMU_GS_REG_FOG                  0x0A
	#define EMU_GS_REG_XYZF3                0x0C
	#define EMU_GS_REG_XYZ3                 0x0D
	#define EMU_GS_REG_TEX1_1               0x14
	#define EMU_GS_REG_TEX1_2               0x15
	#define EMU_GS_REG_TEX2_1               0x16
	#define EMU_GS_REG_TEX2_2               0x17
	#define EMU_GS_REG_XYOFFSET_1           0x18
	#define EMU_GS_REG_XYOFFSET_2           0x19
	#define EMU_GS_REG_PRMODECONT           0x1A
	#define EMU_GS_REG_PRMODE               0x1B
	#define EMU_GS_REG_TEXCLUT              0x1C
	#define EMU_GS_REG_SCANMSK              0x22
	#define EMU_GS_REG_MIPTBP1_1            0x34
	#define EMU_GS_REG_MIPTBP1_2            0x35
	#define EMU_GS_REG_MIPTBP2_1            0x36
	#define EMU_GS_REG_MIPTBP2_2            0x37
	#define EMU_GS_REG_TEXA                 0x3B
	#define EMU_GS_REG_FOGCOL               0x3D
	#define EMU_GS_REG_TEXFLUSH             0x3F
	#define EMU_GS_REG_SCISSOR_1            0x40
	#define EMU_GS_REG_SCISSOR_2            0x41
	#define EMU_GS_REG_ALPHA_1              0x42
	#define EMU_GS_REG_ALPHA_2              0x43
	#define EMU_GS_REG_DIMX                 0x44
	#define EMU_GS_REG_DTHE                 0x45
	#define EMU_GS_REG_COLCLAMP             0x46
	#define EMU_GS_REG_TEST_1               0x47
	#define EMU_GS_REG_TEST_2               0x48
	#define EMU_GS_REG_PABE                 0x49
	#define EMU_GS_REG_FBA_1                0x4A
	#define EMU_GS_REG_FBA_2                0x4B
	#define EMU_GS_REG_FRAME_1              0x4C
	#define EMU_GS_REG_FRAME_2              0x4D
	#define EMU_GS_REG_ZBUF_1               0x4E
	#define EMU_GS_REG_ZBUF_2               0x4F
	#define EMU_GS_REG_BITBLTBUF            0x50
	#define EMU_GS_REG_TRXPOS               0x51
	#define EMU_GS_REG_TRXREG               0x52
	#define EMU_GS_REG_TRXDIR               0x53
	#define EMU_GS_REG_HWREG                0x54
	#define EMU_GS_REG_SIGNAL               0x60
	#define EMU_GS_REG_FINISH               0x61
	#define EMU_GS_REG_LABEL                0x62

	// Privileged Registers Codes
	#define EMU_GS_REG_PMODE                0x00
	#define EMU_GS_REG_SMODE2               0x02
	#define EMU_GS_REG_DISPFB1              0x07
	#define EMU_GS_REG_DISPLAY1             0x08
	#define EMU_GS_REG_DISPFB2              0x09
	#define EMU_GS_REG_DISPLAY2             0x0A
	#define EMU_GS_REG_EXTBUF               0x0B
	#define EMU_GS_REG_EXTDATA              0x0C
	#define EMU_GS_REG_EXTWRITE             0x0D
	#define EMU_GS_REG_BGCOLOR              0x0E
	#define EMU_GS_REG_CSR                  0x40
	#define EMU_GS_REG_IMR                  0x41
	#define EMU_GS_REG_BUSDIR               0x44
	#define EMU_GS_REG_SIGLBLID             0x48

	// Primitives
	#define EMU_GS_PRIM_POINT               0x00
	#define EMU_GS_PRIM_LINE                0x01
	#define EMU_GS_PRIM_LINE_STRIP          0x02
	#define EMU_GS_PRIM_TRIANGLE            0x03
	#define EMU_GS_PRIM_TRIANGLE_STRIP      0x04
	#define EMU_GS_PRIM_TRIANGLE_FAN        0x05
	#define EMU_GS_PRIM_SPRITE              0x06

	// Pixel Storage Formats
	#define EMU_GS_PSF_PSMCT32              0x00
	#define EMU_GS_PSF_PSMCT24              0x01
	#define EMU_GS_PSF_PSMCT16              0x02
	#define EMU_GS_PSF_PSMCT16S             0x0A
	#define EMU_GS_PSF_PSMT8                0x13
	#define EMU_GS_PSF_PSMT4                0x14
	#define EMU_GS_PSF_PSMT8H               0x1B
	#define EMU_GS_PSF_PSMT4HL              0x24
	#define EMU_GS_PSF_PSMT4HH              0x2C
	#define EMU_GS_PSF_PSMZ32               0x30
	#define EMU_GS_PSF_PSMZ24               0x31
	#define EMU_GS_PSF_PSMZ16               0x32
	#define EMU_GS_PSF_PSMZ16S              0x3A
		*/
}