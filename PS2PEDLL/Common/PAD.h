#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct keyEvent
	{
		EMU_U32 key;
		EMU_U32 event;
	};

	struct padButtonStatus
	{
		unsigned char ok;
		unsigned char mode;
		unsigned char btns[2];
		// joysticks
		unsigned char rjoy_h;
		unsigned char rjoy_v;
		unsigned char ljoy_h;
		unsigned char ljoy_v;
		// pressure mode
		unsigned char left_p;
		unsigned char rigth_p;
		unsigned char up_p;
		unsigned char down_p;
		unsigned char triangle_p;
		unsigned char circle_p;
		unsigned char square_p;
		unsigned char cross_p;
		unsigned char unkn16[16];
	};

	struct pad_data
	{
		unsigned int frame;
		unsigned char state;
		unsigned char reqState;
		unsigned char ok;
		unsigned char unkn7;
		unsigned char data[32];
		unsigned int length;
		unsigned int unkn44;
		unsigned int unkn48;
		unsigned int unkn52;
		unsigned int unkn54;
		unsigned int unkn60;
	};

	struct pad_state
	{
		int open;
		unsigned int port;
		unsigned int slot;
		pad_data* padData;
		unsigned char* padBuf;
	};

	struct xpad_data
	{
		unsigned char data[32];         // 0, length = 32 bytes
		unsigned int unkn32;            // not used??
		unsigned int unkn36;            // not used??
		unsigned int unkn40;            // byte 40  not used??
		unsigned int unkn44;            // not used?? 44
		unsigned char actData[32];      // actuator (6x4?) 48
		unsigned short modeTable[4];    // padInfo   80
		unsigned int frame;             // byte 88, EMU_U32 22
		unsigned int unkn92;            // not used ??
		unsigned int length;            // 96
		unsigned char modeOk;           // padInfo  100 Dunno about the name though...
		unsigned char modeCurId;        // padInfo    101
		unsigned char unkn102;          // not used??
		unsigned char unknown;          // unknown
		unsigned char nrOfModes;        // padInfo   104
		unsigned char modeCurOffs;      // padInfo   105
		unsigned char nrOfActuators;    // act  106
		unsigned char unkn107[5];       // not used??
		unsigned char state;            // byte 112
		unsigned char reqState;         // byte 113
		unsigned char ok;               // padInfo  114
		unsigned char unkn115[13];      // not used??
	};

#define DLLCALL	__stdcall

	typedef EMU_I32(DLLCALL* _PADinit)(EMU_U32 flags);
	typedef EMU_I32(DLLCALL* _PADopen)(void* pDsp);
	typedef void (DLLCALL* _PADclose)();
	typedef void (DLLCALL* _PADshutdown)();
	typedef keyEvent* (DLLCALL* _PADkeyEvent)();
	typedef void (DLLCALL* _PADreadStatus)(padButtonStatus* data, int pad);
	typedef EMU_U32(DLLCALL* _PADquery)();

	typedef void (DLLCALL* _PADconfigure)();
	typedef EMU_I32(DLLCALL* _PADtest)();
	typedef void (DLLCALL* _PADabout)();

	void Emu_PAD_Bios_ReadStatus(EMU_I32 pad);

	class Pad final
	{
	public:
		//
		// Pad states
		//
		static const EMU_U08 PAD_STATE_DISCONN = 0x00;
		static const EMU_U08 PAD_STATE_FINDPAD = 0x01;
		static const EMU_U08 PAD_STATE_FINDCTP1 = 0x02;
		static const EMU_U08 PAD_STATE_EXECCMD = 0x05;
		static const EMU_U08 PAD_STATE_STABLE = 0x06;
		static const EMU_U08 PAD_STATE_ERROR = 0x07;

		//
		// Pad request states
		//
		static const EMU_U08 PAD_RSTAT_COMPLETE = 0x00;
		static const EMU_U08 PAD_RSTAT_FAILED = 0x01;
		static const EMU_U08 PAD_RSTAT_BUSY = 0x02;

		// rom0:padman
		static const EMU_U32 PAD_BIND_RPC_ID1 = 0x8000010f;
		static const EMU_U32 PAD_BIND_RPC_ID2 = 0x8000011f;

		static const EMU_U32 PAD_RPCCMD_OPEN = 0x80000100;
		static const EMU_U32 PAD_RPCCMD_SET_MMODE = 0x80000105;
		static const EMU_U32 PAD_RPCCMD_SET_ACTDIR = 0x80000106;
		static const EMU_U32 PAD_RPCCMD_SET_ACTALIGN = 0x80000107;
		static const EMU_U32 PAD_RPCCMD_GET_BTNMASK = 0x80000108;
		static const EMU_U32 PAD_RPCCMD_SET_BTNINFO = 0x80000109;
		static const EMU_U32 PAD_RPCCMD_SET_VREF = 0x8000010a;
		static const EMU_U32 PAD_RPCCMD_GET_PORTMAX = 0x8000010b;
		static const EMU_U32 PAD_RPCCMD_GET_SLOTMAX = 0x8000010c;
		static const EMU_U32 PAD_RPCCMD_CLOSE = 0x8000010d;
		static const EMU_U32 PAD_RPCCMD_END = 0x8000010e;

		//#define PAD_RPCCMD_INIT         0x00000000  /* not supported */
		//#define PAD_RPCCMD_GET_CONNECT  0x00000000  /* not supported */
		//#define PAD_RPCCMD_GET_MODVER   0x00000000  /* not supported */

		// rom0:xpadman
		static const EMU_U32 XPAD_BIND_RPC_ID1 = 0x80000100;
		static const EMU_U32 XPAD_BIND_RPC_ID2 = 0x80000101;

		static const EMU_U08 XPAD_RPCCMD_OPEN = 0x01;
		static const EMU_U08 XPAD_RPCCMD_SET_MMODE = 0x06;
		static const EMU_U08 XPAD_RPCCMD_SET_ACTDIR = 0x07;
		static const EMU_U08 XPAD_RPCCMD_SET_ACTALIGN = 0x08;
		static const EMU_U08 XPAD_RPCCMD_GET_BTNMASK = 0x09;
		static const EMU_U08 XPAD_RPCCMD_SET_BTNINFO = 0x0A;
		static const EMU_U08 XPAD_RPCCMD_SET_VREF = 0x0B;
		static const EMU_U08 XPAD_RPCCMD_GET_PORTMAX = 0x0C;
		static const EMU_U08 XPAD_RPCCMD_GET_SLOTMAX = 0x0D;
		static const EMU_U08 XPAD_RPCCMD_CLOSE = 0x0E;
		static const EMU_U08 XPAD_RPCCMD_END = 0x0F;
		static const EMU_U08 XPAD_RPCCMD_INIT = 0x10;
		static const EMU_U08 XPAD_RPCCMD_GET_CONNECT = 0x11;
		static const EMU_U08 XPAD_RPCCMD_GET_MODVER = 0x12;

		// Initialized PAD plugin
		void Init(void);
		// Resets PAD plugin
		void Reset(void);
		// Plugin PAD config
		void Config(void);

		padButtonStatus*& GetPadStat(int i);
		pad_data*& GetPadArea(int i1, int i2);
		xpad_data*& GetXPadArea(int i1, int i2);

		// PAD1
		static _PADreadStatus  PAD1readStatus;
		static _PADquery       PAD1query;
		static _PADconfigure   PAD1configure;
		static _PADtest        PAD1test;
		static _PADabout       PAD1about;
		static _PADinit        PAD1init;
		static _PADopen        PAD1open;
		static _PADclose       PAD1close;
		static _PADshutdown    PAD1shutdown;
		static _PADkeyEvent    PAD1keyEvent;

		static Pad& GetInstance();

		Pad(const Pad&) = delete;
		Pad(Pad&&) = delete;
		Pad& operator=(const Pad&) = delete;
		Pad& operator=(Pad&&) = delete;

	private:
		Pad();

		padButtonStatus* PadStat[2] = { NULL, NULL };
		pad_data* PadArea[2][2];
		xpad_data* XPadArea[2][2];

		// PAD2
		_PADinit        PAD2init;
		_PADopen        PAD2open;
		_PADclose       PAD2close;
		_PADshutdown    PAD2shutdown;
		_PADkeyEvent    PAD2keyEvent;
		_PADreadStatus  PAD2readStatus;
		_PADquery       PAD2query;

		_PADconfigure   PAD2configure;
		_PADtest        PAD2test;
		_PADabout       PAD2about;
	};
}