#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct _fileio
	{
		EMU_I08 name[256];
		EMU_I08 serv[256];
		EMU_I32 mode;
		EMU_I32 fd;
	};

	struct stEmu_Sif_Control_Regs
	{
		EMU_U32 SB_SMFLG;
		EMU_U08 null0[12];
	};

	struct t_sif_cmd_header
	{
		EMU_U32 size;
		void* dest;
		int command;
		EMU_U32 unknown;
	};

	struct t_sif_handler
	{
		void (*handler)(void* a, void* b);
		void* buff;
	};

	// RPC
	struct t_rpc_header
	{
		void* pkt_addr; /* 04   00 */
		EMU_U32 rpc_id;     /* 05   01 */
		int sema_id;    /* 06   02 */
		EMU_U32 mode;       /* 07   03 */
	};

	struct t_rpc_receive_data
	{
		t_rpc_header      hdr;
		void* src,      /* 04 */
			* dest;     /* 05 */
		int              size;       /* 06 */
	};

	struct t_sif_rpc_other_data
	{
		t_sif_cmd_header      sifcmd;
		int rec_id;     // 04
		void* pkt_addr;  // 05
		int rpc_id;     // 06

		t_rpc_receive_data* receive;  // 07
		void* src;       // 08
		void* dest;      // 09
		int size;       // 10
	};

	struct t_sif_rpc_bind
	{
		t_sif_cmd_header  sifcmd;
		int rec_id;     // 04
		void* pkt_addr;  // 05
		int rpc_id;     // 06
		struct t_rpc_client_data* client;   // 07
		int rpc_number; // 08
	};

	struct t_rpc_data_queue;
	struct t_rpc_server_data
	{
		int command;    /* 04   00 */

		void* (*func)(EMU_U32, void*, int);  /* 05   01 */
		void* buff;     /* 06   02 */
		int size;       /* 07   03 */

		void* (*func2)(EMU_U32, void*, int); /* 08   04 */
		void* buff2;        /* 09   05 */
		int              size2;      /* 10   06 */

		struct t_rpc_client_data* client;    /* 11   07 */
		void* pkt_addr; /* 12   08 */
		int              rpc_number; /* 13   09 */

		void* receive;  /* 14   10 */
		int rec_size;   /* 15   11 */
		int has_async_ef;   /* 16   12 */
		int rec_id;     /* 17   13 */

		t_rpc_server_data* link;     /* 18   14 */
		t_rpc_server_data* next;     /* 19   15 */
		t_rpc_data_queue* queued_object;    /* 20   16 */
	};

	struct t_sif_rpc_rend
	{
		t_sif_cmd_header  sifcmd;
		int rec_id;     // 4
		void* pkt_addr;  // 5
		int rpc_id;     // 6

		t_rpc_client_data* client;    // 7
		EMU_U32 command;    // 8
		t_rpc_server_data* server;    // 9
		void* buff,      // 10
			* buff2;     // 11
	};

	struct t_rpc_client_data
	{
		t_rpc_header hdr;
		EMU_U32 command;            /* 04   08 */
		void* buff,             /* 05   09 */
			* buff2;            /* 06   10 */
		void (*end_function)(void*);  /* 07   11 */
		void* end_param;        /* 08   12*/
		t_rpc_server_data* server;   /* 09   13 */
	};

	struct t_rpc_data_queue
	{
		int thread_id,  /* 00 */
			active;     /* 01 */
		t_rpc_server_data* svdata_ref,    /* 02 */
			* start,        /* 03 */
			* end;      /* 04 */
		t_rpc_data_queue* next;      /* 05 */
	};

	struct t_sif_rpc_call
	{
		t_sif_cmd_header  sifcmd;
		int rec_id;     /* 04 */
		void* pkt_addr;  /* 05 */
		int rpc_id;     /* 06 */
		t_rpc_client_data* client;   /* 07 */
		int rpc_number; /* 08 */
		int send_size;  /* 09 */
		void* receive;   /* 10 */
		int rec_size;   /* 11 */
		int has_async_ef;   /* 12 */
		t_rpc_server_data* server;   /* 13 */
	};

	void Emu_Sif_BiosSceGetReg(void);
	void Emu_Sif_BiosSceSetReg(void);
	void Emu_Sif_BiosIsceSetDChain(void);
	void Emu_Sif_BiosIsceSetDma(void);
	void Emu_Sif_BiosIsceDmaStat(void);

	class Sif final
	{
	public:
		// Base Address
		static const EMU_U32 EMU_SIF_START_ADDR = 0x1000F230;
		static const EMU_U32 EMU_SIF_END_ADDR = 0x1000F41F;

		// Control Register Address
		static const EMU_U32 EMU_SIF_SB_SMFLG = 0x00000000;

		static const EMU_U32 NPM_PUTS = 0x01;
		static const EMU_U32 RPC_NPM_USER = 0x014d704e;

		static const EMU_U32 PS_RDONLY = 0x0001;
		static const EMU_U32 PS_WRONLY = 0x0002;
		static const EMU_U32 PS_RDWR = 0x0003;
		static const EMU_U32 PS_NBLOCK = 0x0010;
		static const EMU_U32 PS_APPEND = 0x0100;
		static const EMU_U32 PS_CREAT = 0x0200;
		static const EMU_U32 PS_TRUNC = 0x0400;
		static const EMU_U32 PS_NOWAIT = 0x8000;

		static const EMU_U32 FILEIO_MAX = 32;

		static const EMU_U32 SYSTEM_CMD = 0x80000000;

		void Init(void);
		void Reset(void);
		static void Callback(EMU_U32 Address);

		EMU_U08* GetPointer(EMU_U32 Address);

		void SetBin(EMU_U32* bin);
		void SetBout(EMU_U32* bout);
		void SetCall(t_sif_rpc_call* call);

		void BiosCallModule();
		void BiosCallPad1();
		void BiosCallPad2();
		void BiosCallXPad1();
		void BiosCallXPad2();
		void BiosCallNpm();
		void BiosCallFileIO();

		static Sif& GetInstance();

		Sif(const Sif&) = delete;
		Sif(Sif&&) = delete;
		Sif& operator=(const Sif&) = delete;
		Sif& operator=(Sif&&) = delete;

	private:
		Sif();

		EMU_U08 EmuSifMemory[EMU_SIF_END_ADDR - EMU_SIF_START_ADDR];

		// PoEMU_I32er to control1 registers
		stEmu_Sif_Control_Regs* Emu_Sif_Control_Reg;

		t_sif_rpc_call* call;

		EMU_U32* bin,
			* bout;

		_fileio fileio[FILEIO_MAX];
	};
}