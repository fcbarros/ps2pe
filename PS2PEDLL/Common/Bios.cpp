#include "Memory.h"
#include "Bios.h"
#include "Intc.h"
#include "Dma.h"
#include "Sif.h"
#include "Gs.h"
#include "Thread.h"
#include "Sema.h"

#include "EmuMain.h"
#include "Ps2Core.h"

namespace Common
{
	// Names of the 256 Bios calls
	const char* Emu_Bios_Calls_Names[256] =
	{
		//0x00
		"RFU000_FullReset",         "ResetEE",              "SetGsCrt",                 "RFU003",
		"Exit",                     "RFU005",               "LoadExecPS2",              "ExecPS2",
		"RFU008",                   "RFU009",               "AddSbusIntcHandler",       "RemoveSbusIntcHandler",
		"Interrupt2Iop",            "SetVTLBRefillHandler", "SetVCommonHandler",        "SetVInterruptHandler",
		//0x10
		"AddIntcHandler",           "RemoveIntcHandler",    "AddDmacHandler",           "RemoveDmacHandler",
		"_EnableIntc",              "_DisableIntc",         "_EnableDmac",              "_DisableDmac",
		"_SetAlarm",                "_ReleaseAlarm",        "_iEnableIntc",             "_iDisableIntc",
		"_iEnableDmac",             "_iDisableDmac",        "_iSetAlarm",               "_iReleaseAlarm",
		//0x20
		"CreateThread",             "DeleteThread",         "StartThread",              "ExitThread",
		"ExitDeleteThread",         "TerminateThread",      "iTerminateThread",         "DisableDispatchThread",
		"EnableDispatchThread",     "ChangeThreadPriority", "iChangeThreadPriority",    "RotateThreadReadyQueue",
		"iRotateThreadReadyQueue",  "ReleaseWaitThread",    "iReleaseWaitThread",       "GetThreadId",
		//0x30
		"ReferThreadStatus",        "iReferThreadStatus",   "SleepThread",              "WakeupThread",
		"_iWakeupThread",           "CancelWakeupThread",   "iCancelWakeupThread",      "SuspendThread",
		"iSuspendThread",           "ResumeThread",         "iResumeThread",            "JoinThread",
		"RFU060",                   "RFU061",               "EndOfHeap",                "RFU063",
		//0x40
		"CreateSema",               "DeleteSema",           "SignalSema",               "iSignalSema",
		"WaitSema",                 "PollSema",             "iPollSema",                "ReferSemaStatus",
		"iReferSemaStatus",         "RFU073",               "SetOsdConfigParam",        "GetOsdConfigParam",
		"GetGsHParam",              "GetGsVParam",          "SetGsHParam",              "SetGsVParam",
		//0x50
		"RFU080_CreateEventFlag",                           "RFU081_DeleteEventFlag",
		"RFU082_SetEventFlag",                              "RFU083_iSetEventFlag",
		"RFU084_ClearEventFlag",                            "RFU085_iClearEventFlag",
		"RFU086_WaitEvnetFlag",                             "RFU087_PollEvnetFlag",
		"RFU088_iPollEvnetFlag",                            "RFU089_ReferEventFlagStatus",
		"RFU090_iReferEventFlagStatus",                     "RFU091",
		"EnableIntcHandler_iEnableIntcHandler",             "DisableIntcHandler_iDisableIntcHandler",
		"EnableDmacHandler_iEnableDmacHandler",             "DisableDmacHandler_iDisableDmacHandler",
		//0x60
		"KSeg0",                    "EnableCache",          "DisableCache",             "GetCop0",
		"FlushCache",               "RFU101",               "CpuConfig",                "iGetCop0",
		"iFlushCache",              "RFU105",               "iCpuConfig",               "sceSifStopDma",
		"SetCPUTimerHandler",       "SetCPUTimer",          "SetOsdConfigParam2",       "SetOsdConfigParam2",
		//0x70
		"GsGetIMR_iGsGetIMR",                               "GsPutIMR_iGsPutIMR",
		"SetPgifHandler",                                   "SetVSyncFlag",
		"RFU116",                                           "print",
		"sceSifDmaStat_isceSifDmaStat",                     "sceSifSetDma_isceSifSetDma",
		"sceSifSetDChain_isceSifSetDChain",                 "sceSifSetReg",
		"sceSifGetReg",                                     "ExecOSD",
		"Deci2Call",                                        "PSMode",
		"MachineType",                                      "GetMemorySize"
	};

	void Emu_Bios_Dummy();
	void Emu_Bios_Exit();
	void Emu_Bios_RFU060();
	void Emu_Bios_RFU061();
	void Emu_Bios_EndOfHeap();
	void Emu_Bios_FlushCache();
	void Emu_Bios_Deci2Call();
	void Emu_Bios_ExecPS2();

	Bios::Bios()
	{
	}

	Bios& Bios::GetInstance()
	{
		static Bios bios;
		return bios;
	}

	// Load a file into Bios memory area
	void Bios::Load(const char* FileName)
	{
		FILE* pFile = fopen(FileName, "rb");
		if (!pFile)
		{
			return;
		}

		fread(EmuBiosMemory, 4 * 1024 * 1024, 1, pFile);

		fclose(pFile);
	}

	EMU_U08* Bios::GetPointer(EMU_U32 Address)
	{
		return &EmuBiosMemory[Address - EMU_BIOS_START_ADDR];
	}

	EMU_U08* Bios::GetPsR()
	{
		return psR;
	}

	void Bios::Init()
	{
		int i;

		for (i = 0; i < 256; i++)
		{
			Emu_Bios_Function[i] = Emu_Bios_Dummy;
		}

		Emu_Bios_Function[0x02] = Emu_GS_Bios_SetGsCrt;
		Emu_Bios_Function[0x07] = Emu_Bios_ExecPS2;
		Emu_Bios_Function[0x04] = Emu_Bios_Exit;
		Emu_Bios_Function[0x10] = Emu_Intc_BiosAddHandler;
		Emu_Bios_Function[0x11] = Emu_Intc_BiosRemoveHandler;
		Emu_Bios_Function[0x12] = Emu_Dma_BiosAddHandler;
		Emu_Bios_Function[0x13] = Emu_Dma_BiosRemoveHandler;
		Emu_Bios_Function[0x14] = Emu_Intc_BiosEnableHandler;
		Emu_Bios_Function[0x15] = Emu_Intc_BiosDisableHandler;
		Emu_Bios_Function[0x16] = Emu_Dma_BiosEnableHandler;
		Emu_Bios_Function[0x20] = Emu_Thread_Bios_Create;
		Emu_Bios_Function[0x22] = Emu_Thread_Bios_Start;
		Emu_Bios_Function[0x2F] = Emu_Thread_Bios_GetId;
		Emu_Bios_Function[0x30] = Emu_Thread_Bios_ReferStatus;
		Emu_Bios_Function[0x3C] = Emu_Bios_RFU060;
		Emu_Bios_Function[0x3D] = Emu_Bios_RFU061;
		Emu_Bios_Function[0x3E] = Emu_Bios_EndOfHeap;
		Emu_Bios_Function[0x40] = Emu_Sema_Bios_Create;
		Emu_Bios_Function[0x41] = Emu_Sema_Bios_Delete;
		Emu_Bios_Function[0x42] = Emu_Sema_Bios_Signal;
		Emu_Bios_Function[0x44] = Emu_Sema_Bios_Wait;
		Emu_Bios_Function[0x45] = Emu_Sema_Bios_Poll;
		Emu_Bios_Function[0x64] = Emu_Bios_FlushCache;
		Emu_Bios_Function[0x70] = Emu_GS_Bios_GetIMR;
		Emu_Bios_Function[0x71] = Emu_GS_Bios_PutIMR;
		Emu_Bios_Function[0x76] = Emu_Sif_BiosIsceDmaStat;
		Emu_Bios_Function[0x77] = Emu_Sif_BiosIsceSetDma;
		Emu_Bios_Function[0x78] = Emu_Sif_BiosIsceSetDChain;
		Emu_Bios_Function[0x79] = Emu_Sif_BiosSceSetReg;
		Emu_Bios_Function[0x7A] = Emu_Sif_BiosSceGetReg;
		Emu_Bios_Function[0x7c] = Emu_Bios_Deci2Call;
	}

	void Bios::Syscall()
	{
#ifdef EMU_LOG
		EmuLog("BIOS_%s\n", Emu_Bios_Calls_Names[PS2Regs.R5900Regs.V1.u08_00_07]);
#endif

		//    COP0Regs.Reg[14] = R5900Regs.PC;

		if (PS2Regs.R5900Regs.V1.i32_00_31 >= 0)
		{
			Emu_Bios_Function[PS2Regs.R5900Regs.V1.u08_00_07]();
		}
		else
		{
			Emu_Bios_Function[-(PS2Regs.R5900Regs.V1.i32_00_31) & 0xFF]();
		}
	}


	//
	//  EMU_U32 RFU060();
	//
	void Emu_Bios_RFU060()
	{ // 0x3c
		PS2Regs.R5900Regs.V0.u64_00_63 = 0xFFFFFFFF81F00000;
	}

	//
	//  EMU_U32 RFU060();
	//
	void Emu_Bios_RFU061()
	{ // 0x3d
		PS2Regs.R5900Regs.V0.u64_00_63 = PS2Regs.R5900Regs.A0.u32_00_31 + PS2Regs.R5900Regs.A1.u32_00_31;
	}

	//
	//  EMU_U32 k_EndOfHeap();
	//
	void Emu_Bios_EndOfHeap()
	{ // 0x3e
		PS2Regs.R5900Regs.V0.u64_00_63 = 0xFFFFFFFF80000000;
	}

	//
	//  void k_FlushCache(int);
	//
	void Emu_Bios_FlushCache()
	{ // 0x64
	}

	void Emu_Bios_Exit()
	{ // 0x64
		Common::Ps2Core::GetInstance().StopRun(true);
	}

	//
	//  int ExecPS2(void *, void *, int, char **);
	//
	void Emu_Bios_ExecPS2()
	{ // 0x07
	//  cpuRegs.pc = a0.UL[0];
	}

	void Emu_Bios_Dummy()
	{
	}

	//
	//  int Deci2Call(int, u_int *);
	//
	void Emu_Bios_Deci2Call()
	{ // 0x7c
#ifdef EMU_LOG
		EmuLog("  %d %x\n", PS2Regs.R5900Regs.A0.u32_00_31, PS2Regs.R5900Regs.A1.u32_00_31);
#endif

		EMU_U32* addr = (EMU_U32*)Memory::GetInstance().GetRealPointer(PS2Regs.R5900Regs.A1.u32_00_31);

		EMU_U32* deci2addr{ nullptr };
		EMU_U32 deci2handler{ 0 };

		switch (PS2Regs.R5900Regs.A0.i32_00_31)
		{
		case 1: // open
			deci2addr = (EMU_U32*)Memory::GetInstance().GetRealPointer(addr[1]);
#ifdef EMU_LOG
			EmuLog("  deci2open: %x,%x,%x,%x\n",
				addr[3], addr[2], addr[1], addr[0]);
#endif
			deci2handler = addr[2];

			PS2Regs.R5900Regs.V0.u64_00_63 = 1;
			break;

		case 2: // close
			PS2Regs.R5900Regs.V0.u64_00_63 = 1;
			break;

		case 3: // reqsend
#ifdef EMU_LOG
			EmuLog("  deci2reqsend: %x,%x,%x,%x: deci2addr: %x,%x,%x,%x %x,%x,%x,%x\n",
				addr[3], addr[2], addr[1], addr[0],
				deci2addr[7], deci2addr[6], deci2addr[5], deci2addr[4],
				deci2addr[3], deci2addr[2], deci2addr[1], deci2addr[0]);
#endif
			//          cpuRegs.pc = deci2handler;
			EmuConsole("deci2msg: %s", (char*)Memory::GetInstance().GetRealPointer(deci2addr[4] + 0xc));
			deci2addr[3] = 0;
			PS2Regs.R5900Regs.V0.u64_00_63 = 1;
			break;

		case 4: // poll
#ifdef EMU_LOG
			EmuLog("  deci2poll: %x,%x,%x,%x\n",
				addr[3], addr[2], addr[1], addr[0]);
#endif
			PS2Regs.R5900Regs.V0.u64_00_63 = 1;
			break;

		case -5: // exrecv
			break;

		case -6: // exsend
			PS2Regs.R5900Regs.V0.u64_00_63 = 1;
			break;
		}
	}

	/*
	void biosException( void )
	{
		if ( softCall )
		{
			softCall--;

			memcpy( &PS2Regs.R5900Regs, saver, sizeof( R5900Regs ) );
			return;
		}

	//  if ( cpuInterrupt & 0x1 )
	//    if ( Emu_Dma_Control1_Reg->STAT & 0x1 )
		{
			bios_PadReadStatus(1);
			bios_PadReadStatus(2);

	//      cpuInterrupt &= ~0x1;
	//        Emu_Dma_Control1_Reg->STAT &= ~0x1;

			if ( ( Emu_Intc_Handler[ 2 ].active ) )//&& ( Emu_GS_Privileg_Reg->CSR & 0x00000008 ) )
			{ // vblank start
				memcpy( saver, &PS2Regs.R5900Regs, sizeof( R5900Regs ) );

				R5900Regs.Reg[31].u64_00_63 = 0x80000180;
				R5900Regs.PC = Emu_Intc_Handler[2].func;

				softCall = 1;
				return;
			}
			if ( Emu_Intc_Handler[3].active )
			{ // vblank end
				memcpy( saver, &PS2Regs.R5900Regs, sizeof( R5900Regs ) );

				R5900Regs.Reg[31].u64_00_63 = 0x80000180;
				R5900Regs.PC = Emu_Intc_Handler[3].func;

				softCall = 1;
				return;
			}
		}
	//  if ( cpuInterrupt & 0x2 )
		if ( Emu_Dma_Control1_Reg->STAT & 0x2 )
		{
	//      cpuInterrupt&=~0x2;
			Emu_Dma_Control1_Reg->STAT &= ~0x2;
			if ( intch[10].active )
			{ // tim1
				memcpy( saver, &PS2Regs.R5900Regs, sizeof( R5900Regs ) );

				R5900Regs.Reg[31].u64_00_63 = 0x80000180;
				R5900Regs.PC = intch[10].func;

				softCall = 1;
				return;
			}
		}
	//  if ( cpuInterrupt & 0x200000 )
		if ( Emu_Dma_Control1_Reg->STAT & 0x200000 )
		{
	//      cpuInterrupt&=~0x200000;
			Emu_Dma_Control1_Reg->STAT &= ~0x200000;

	//      SysPrintf( "intr 0x200000: %x, %x\n", dmach[5].active, dmach[5].func );

			if ( dmach[5].active )
			{ // sif1?
				memcpy( saver, &PS2Regs.R5900Regs, sizeof( R5900Regs ) );

				R5900Regs.Reg[31].u64_00_63 = 0x80000180;
				R5900Regs.PC = dmach[5].func;

				softCall = 1;
				return;
			}
		}

	//BIOS_LOG("biosRET %x\n", cpuRegs.COP0[14] + 4);
		R5900Regs.PC = COP0Regs.Reg[14] + 4;
	}
	*/


}