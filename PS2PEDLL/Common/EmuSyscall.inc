/*
SYSCALLS

+0000 - RFU000_FullReset
+0001 - ResetEE
* +0002 - SetGsCrt
+0003 - RFU003
+0004 - Exit
+0005 - RFU005
+0006 - LoadPS2Exe
+0007 - ExecPS2
+0008 - RFU008
+0009 - RFU009
+0010 - AddSbusIntcHandler
+0011 - RemoveSbusIntcHandler
+0012 - Interrupt2Iop
+0013 - SetVTLBRefillHandler
+0014 - SetVCommonHandler
+0015 - SetVInterruptHandler
* +0016 - AddIntcHandler
* +0017 - RemoveIntcHandler
* +0018 - AddDmacHandler
* +0019 - RemoveDmacHandler
* +0020 - _EnableIntc
* +0021 - _DisableIntc
* +0022 - _EnableDmac
+0023 - _DisableDmac
+0024 - _SetAlarm
+0025 - _ReleaseAlarm
-0026 - _iEnableIntc
-0027 - _iDisableIntc
-0028 - _iEnableDmac
-0029 - _iDisableDmac
-0030 - _iSetAlarm
-0031 - _iReleaseAlarm
* +0032 - CreateThread
+0033 - DeleteThread
* +0034 - StartThread
+0035 - ExitThread
+0036 - ExitDeleteThread
+0037 - TerminateThread
-0038 - iTerminateThread
+0039 - DisableDispatchThread
+0040 - EnableDispatchThread
+0041 - ChangeThreadPriority
-0042 - iChangeThreadPriority
+0043 - RotateThreadReadyQueue
-0044 - iRotateThreadReadyQueue
+0045 - ReleaseWaitThread
-0046 - iReleaseWaitThread
+0047 - GetThreadId
+0048 - ReferThreadStatus
-0049 - iReferThreadStatus
+0050 - SleepThread
+0051 - WakeupThread
-0052 - iWakeupThread
+0053 - CancelWakeupThread
-0054 - iCancelWakeupThread
+0055 - SuspendThread
-0056 - iSuspendThread
+0057 - ResumeThread
-0058 - iResumeThread
+0059 - JoinThread
* +0060 - RFU060
+0061 - RFU061
* +0062 - EndOfHeap
+0063 - RFU063
* +0064 - CreateSema
* +0065 - DeleteSema
* +0066 - SignalSema
-0067 - iSignalSema
* +0068 - WaitSema
+0069 - PollSema
-0070 - iPollSema
+0071 - ReferSemaStatus
-0072 - iReferSemaStatus
+0073 - RFU073
+0074 - SetOsdConfigParam
+0075 - GetOsdConfigParam
+0076 - GetGsHParam
+0077 - GetGsVParam
+0078 - SetGsHParam
+0079 - SetGsVParam
+0080 - RFU080_CreateEventFlag
+0081 - RFU081_DeleteEventFlag
+0082 - RFU082_SetEventFlag
+0083 - RFU083_iSetEventFlag
+0084 - RFU084_ClearEventFlag
+0085 - RFU085_iClearEventFlag
+0086 - RFU086_WaitEvnetFlag
+0087 - RFU087_PollEvnetFlag
+0088 - RFU088_iPollEvnetFlag
+0089 - RFU089_ReferEventFlagStatus
-0090 - RFU090_iReferEventFlagStatus
+0091 - RFU091
* +0092 - EnableIntcHandler
-0092 - iEnableIntcHandler
* +0093 - DisableIntcHandler
-0093 - iDisableIntcHandler
* +0094 - EnableDmacHandler
-0094 - iEnableDmacHandler
* +0095 - DisableDmacHandler
-0095 - iDisableDmacHandler
+0096 - KSeg0
+0097 - EnableCache
+0098 - DisableCache
+0099 - GetCop0
+0100 - FlushCache
+0102 - CpuConfig
-0103 - iGetCop0
-0104 - iFlushCache
-0106 - iCpuConfig
+0107 - sceSifStopDma
+0108 - SetCPUTimerHandler
+0109 - SetCPUTimer
+0110 - ForceRead
+0111 - ForceWrite
* +0112 - GsGetIMR
-0112 - iGsGetIMR
* +0113 - GsPutIMR
-0113 - iGsPutIMR
+0114 - SetPgifHandler
+0115 - SetVSyncFlag
+0116 - RFU116
+0117 - print
+0118 - sceSifDmaStat
-0118 - isceSifDmaStat
+0119 - sceSifSetDma
-0119 - isceSifSetDma
+0120 - sceSifSetDChain
-0120 - isceSifSetDChain
+0121 - sceSifSetReg
+0122 - sceSifGetReg
+0123 - ExecOSD
+0124 - Deci2Call
+0125 - PSMode
+0126 - MachineType
+0127 - GetMemorySize
*/

#ifdef EMU_LOG
    EmuLog( "BIOS_%s\n", Emu_Bios_Calls_Names[ R5900Regs.V1.u08_00_07 ] );
#endif

COP0Regs.Reg[14] = R5900Regs.PC;

switch ( R5900Regs.V1.u32_00_31 )
{
case 0x0002: // SetGsCrt (2)
	break;

case 0x0004: // Exit (4)
	EmuStopRun = true;
	break;

case 0x0010: // AddIntcHandler (16)
	Emu_Intc_Bios_AddHandler( );
	break;

case 0x0011: // RemoveIntcHandler (17)
	Emu_Intc_Bios_RemoveHandler( );
	break;

case 0x0012: // k_AddDmacHandler (18)
	Emu_Dma_Bios_AddHandler( );
	break;

case 0x0013: // k_RemoveDmacHandler (19)
	Emu_Dma_Bios_RemoveHandler( );
	break;

case 0x0014: // _EnableIntc (20)
	Emu_Intc_Bios_EnableHandler( );
	break;

case 0x0015: // _DisableIntc (21)
	Emu_Intc_Bios_DisableHandler( );
	break;

case 0x0016: // k_EnableDmac (22)
    Emu_Dma_Bios_Enable( );
	break;

case 0x0020: // CreateThread (32)
	Emu_Thread_Bios_Create( );
	break;

case 0x0022: // StartThread (34)
	Emu_Thread_Bios_Start( );
	break;

case 0x002F: // GetThreadId (47)
	Emu_Thread_Bios_GetId( );
	break;

case 0x0030: // ReferThreadStatus (48)
	Emu_Thread_Bios_ReferStatus( );
	break;

case 0x003c: // RFU060 (60)
	Emu_Bios_RFU060( );
	break;

case 0x003e: // EndOfHeap (62)
	Emu_Bios_EndOfHeap( );
	break;

case 0x0040: // k_CreateSema (64)
	Emu_Sema_Bios_Create( );
	break;

case 0x0041: // k_DeleteSema (65)
	Emu_Sema_Bios_Delete( );
	break;

case 0x0042: // k_SignalSema (66)
	Emu_Sema_Bios_Signal( );
	break;

case 0x0044: // k_WaitSema (68)
	Emu_Sema_Bios_Wait( );
	break;
 
case 0x0070: // GsGetIMR_iGsGetIMR (112)
	Emu_GS_Bios_GetIMR( );
	break;

case 0x0071: // GsPutIMR_iGsPutIMR (113)
	Emu_GS_Bios_PutIMR( );
	break;

case 0x0076: // sceSifDmaStat_isceSifDmaStat (118)
	Emu_Sif_Bios_isceDmaStat( );
	break;

case 0x0077: // k_sceSifSetDma (119)
	Emu_Sif_Bios_isceSetDma( );
	break;

case 0x0078: // k_sceSifSetDChain (120)
	Emu_Sif_Bios_isceSetDChain( );
	break;

case 0x0079: // k_sceSifSetReg (121)
	Emu_Sif_Bios_sceSetReg( );
	break;

case 0x007A: // k_sceSifGetReg (122)
	Emu_Sif_Bios_sceGetReg( );
	break;

default:
	break;
}
R5900Regs.PC += 4;
//Emu_Intc_CallHandler( 2 );
