#include <fcntl.h>

#ifdef __WIN32__
#include <io.h>
#else
#include <unistd.h>
#endif

#include "EmuMain.h"
#include "EmuSif.h"
#include "EmuPad.h"
#include "EmuDma.h"
#include "EmuBios.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

EMU_U08 EmuSifMemory[EMU_SIF_END_ADDR - EMU_SIF_START_ADDR];

// PoEMU_I32er to control1 registers
stEmu_Sif_Control_Regs* Emu_Sif_Control_Reg;

struct t_sif_rpc_call* call;

EMU_U32* bin,
* bout;

_fileio fileio[FILEIO_MAX];

static EMU_I32 reg4;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Sif_Init()
{
	Emu_Sif_Control_Reg = (stEmu_Sif_Control_Regs*)EmuSifMemory;

	Emu_Sif_Reset();
}

void Emu_Sif_Reset()
{
	memset(Emu_Sif_Control_Reg, 0, sizeof(stEmu_Sif_Control_Regs));

	memset(fileio, 0, sizeof(fileio));
}

void Emu_Sif_callback(EMU_U32 Address)
{
	// SB_SMFLG
}

EMU_U08* Emu_Sif_GetPointer(EMU_U32 Address)
{
	return &EmuSifMemory[Address - EMU_SIF_START_ADDR];
}

void Emu_Sif_Bios_isceDmaStat()
{ // 0x76
	PS2Regs.R5900Regs.V0.u64_00_63 = 1;
}

void Emu_Sif_Bios_Call_Module()
{
	char* fdesc;
	char    fserv[256],
		fname[256];
	char    file[256];
	EMU_I32 i;

	switch (call->rpc_number)
	{
	case 0: // loadModule (.irx)
		fdesc = (char*)&bin[2];

		i = 0;
		while ((*fdesc != ':') && (*fdesc != 0))
		{
			fserv[i++] = *fdesc++;
		}
		fserv[i] = 0;
		i = 0;
		fdesc++;
		while (*fdesc != 0)
		{
			fname[i++] = *fdesc++;
		}
		fname[i] = 0;

		bout[0] = 0;

		EmuConsole("loadModule %s\n", fname);

		if (strcmp(fserv, "host") == 0)
		{
#ifdef __WIN32__
			sprintf(file, "host\\%s", fname);
#else
			sprintf(file, "host/%s", fname);
#endif
			EmuConsole("opening %s\n", file);

			//          loadIrxFile( file );
		}
		break;

	case 0xFF: // init? returns version
		strncpy((char*)bout, "2000", 4);
		break;

	default:
		EmuConsole("module unk: %x\n", call->rpc_number);
		break;
	}
}

void Emu_Sif_Bios_Call_Pad1()
{
	EMU_I32 port;

	if (call->rpc_number != 1)
	{
#ifdef EMU_LOG
		EmuLog("pad1server unk call %x\n", call->rpc_number);
#endif
		return;
	}
#ifdef EMU_LOG
	EmuLog("pad1server cmd %x\n", bin[0]);
#endif
	switch (bin[0])
	{
	case PAD_RPCCMD_OPEN:
		port = bin[1];

		bout[12 / 4] = 1;

		PadArea[port][0] = (pad_data*)EmuMemGetRealPointer(*(EMU_U32*)&bin[16 / 4]);
		PadArea[port][1] = (pad_data*)EmuMemGetRealPointer(*(EMU_U32*)&bin[16 / 4] + sizeof(pad_data));
		PadArea[port][1]->frame = 0;
		PadArea[port][0]->frame = 0;
		PadArea[port][0]->state = PAD_STATE_STABLE;
		PadArea[port][0]->reqState = PAD_RSTAT_COMPLETE;
		PadArea[port][0]->length = sizeof(padButtonStatus);
		PadStat[port] = (padButtonStatus*)PadArea[port][0]->data;
		PadStat[port]->btns[0] = 0xff;
		PadStat[port]->btns[1] = 0xff;
		break;

	case PAD_RPCCMD_SET_MMODE:
		bout[20 / 4] = 4;
		break;

	case PAD_RPCCMD_GET_PORTMAX:
		bout[12 / 4] = 2;
		break;

	case PAD_RPCCMD_GET_SLOTMAX:
		bout[12 / 4] = 1;
		break;
	}
}

void Emu_Sif_Bios_Call_Pad2()
{
	if (call->rpc_number != 1)
	{
#ifdef EMU_LOG
		EmuLog("pad2server unk call %x\n", call->rpc_number);
#endif
		return;
	}
}

void Emu_Sif_Bios_Call_XPad1()
{
	EMU_I32 port;

	if (call->rpc_number != 1)
	{
#ifdef EMU_LOG
		EmuLog("pad1server unk call %x\n", call->rpc_number);
#endif
		return;
	}
#ifdef EMU_LOG
	EmuLog("xpad1server cmd %x\n", bin[0]);
#endif
	switch (bin[0])
	{
	case XPAD_RPCCMD_OPEN:
		port = bin[1];

		bout[12 / 4] = 1;

		XPadArea[port][0] = (xpad_data*)EmuMemGetRealPointer(*(EMU_U32*)&bin[16 / 4]);
		XPadArea[port][1] = (xpad_data*)EmuMemGetRealPointer(*(EMU_U32*)&bin[16 / 4] + sizeof(pad_data));
		XPadArea[port][1]->frame = 0;
		XPadArea[port][0]->frame = 0;
		XPadArea[port][0]->state = PAD_STATE_STABLE;
		XPadArea[port][0]->reqState = PAD_RSTAT_COMPLETE;
		XPadArea[port][0]->length = sizeof(padButtonStatus);
		PadStat[port] = (padButtonStatus*)XPadArea[port][0]->data;
		PadStat[port]->btns[0] = 0xff;
		PadStat[port]->btns[1] = 0xff;
		break;

	case XPAD_RPCCMD_SET_MMODE:
		bout[20 / 4] = 4;
		break;

	case XPAD_RPCCMD_GET_PORTMAX:
		bout[12 / 4] = 2;
		break;

	case XPAD_RPCCMD_GET_SLOTMAX:
		bout[12 / 4] = 1;
		break;

	case XPAD_RPCCMD_GET_MODVER:
		bout[12 / 4] = 0x300; // version??
		break;
	}
}

void Emu_Sif_Bios_Call_XPad2()
{
	if (call->rpc_number != 1)
	{
#ifdef BIOS_LOG
		BIOS_LOG("xpad2server unk call %x\n", call->rpc_number);
#endif
		return;
	}
}

void Emu_Sif_Bios_Call_Npm()
{
	switch (call->rpc_number)
	{
	case NPM_PUTS:
		EmuConsole("%s", (char*)bin);
		break;
	}
}

void Emu_Sif_Bios_Call_FileIO()
{
	char* fdesc, * ptr;
	char fserv[256], fname[256];
	EMU_I32 i;

	switch (call->rpc_number)
	{
	case 0: // open
		fdesc = (char*)&bin[1];

		i = 0;
		while (*fdesc != ':')
		{
			fserv[i++] = *fdesc++;
		}
		fserv[i] = 0;
		i = 0;
		fdesc++;
		while (*fdesc != 0)
		{
			fname[i++] = *fdesc++;
		}
		fname[i] = 0;
		EmuConsole("open %s, server=%s, mode=%x\n", fname, fserv, bin[0]);

		for (i = 0; i < FILEIO_MAX; i++)
		{
			if (fileio[i].fd == 0)
			{
#ifdef __WIN32__
				fileio[i].mode = O_BINARY;
#endif
				if (bin[0] & PS_RDONLY) fileio[i].mode |= O_RDONLY;
				strcpy(fileio[i].serv, fserv);
				break;
			}
		}
		bout[0] = i + 1;
		if (i == FILEIO_MAX)
		{
			break;
		}

		if (strcmp(fileio[i].serv, "host") == 0)
		{
#ifdef __WIN32__
			sprintf(fileio[i].name, "host\\%s", fname);
#else
			sprintf(fileio[i].name, "host/%s", fname);
#endif
			EmuConsole("opening %s\n", fileio[i].name);

			fileio[i].fd = open(fileio[i].name, fileio[i].mode);
			if (fileio[i].fd == -1)
			{
				bout[0] = -1;
			}
		}
		break;

	case 1: // close
		i = bin[0] - 1;
		bout[0] = close(fileio[i].fd);
		fileio[i].fd = 0;
		break;

	case 2: // read
		i = bin[0] - 1;
		ptr = (char*)EmuMemGetRealPointer(bin[1]);
		bout[0] = read(fileio[i].fd, ptr, bin[2]);
		EmuConsole("read(%d) %x, args=%x,%x\n", fileio[i].fd, *bout, bin[1], bin[2]);
		break;

	case 3: // write
		break;

	case 4: // lseek
		i = bin[0] - 1;
		bout[0] = lseek(fileio[i].fd, bin[1], bin[2]);
		EmuConsole("seek(%d) %x, args=%x,%x\n", fileio[i].fd, *bout, bin[1], bin[2]);
		break;
	}
}

//
//  EMU_I32 k_isceSifSetDma( struct t_sif_dma_transfer *, EMU_I32);
//
void Emu_Sif_Bios_isceSetDma()
{ // 0x77
	struct t_sif_dma_transfer* dmat;
	struct t_sif_cmd_header* hdr;
	struct t_sif_rpc_bind* bind;
	struct t_rpc_server_data* server;
	struct t_rpc_client_data* client;
	EMU_I32 n_transfer;
	EMU_U32 addr;

	n_transfer = PS2Regs.R5900Regs.A1.u32_00_31 - 1;
	if (n_transfer < 0)
	{
		return;
	}

	addr = PS2Regs.R5900Regs.A0.u32_00_31 + n_transfer * sizeof(struct t_sif_dma_transfer);
	dmat = (struct t_sif_dma_transfer*)EmuMemGetRealPointer(addr);

#ifdef EMU_LOG
	EmuLog("  n_transfer=%d, size=%x, attr=%x, dest=%x, src=%x\n",
		dmat->size, dmat->attr,
		dmat->dest, dmat->src);
#endif

	dmat->dest = EmuMemGetRealPointer((EMU_U32)dmat->dest);
	dmat->src = EmuMemGetRealPointer((EMU_U32)dmat->src);

	//  BIOS_LOG("src %p, dst %p\n", dmat->src, dmat->dest);

	if ((dmat->dest == NULL) ||
		(dmat->src == NULL))
	{
		PS2Regs.R5900Regs.A0.u64_00_63 = 1;
		return;
	}

	hdr = (struct t_sif_cmd_header*)dmat->src;

#ifdef EMU_LOG
	EmuLog("  sif_cmd %x %x\n", hdr->command, hdr->size);
#endif
	switch (hdr->command)
	{
	case 0x80000000:
		break;

	case 0x80000009: // sif_bind
		bind = (struct t_sif_rpc_bind*)dmat->src;
		client = (struct t_rpc_client_data*)EmuMemGetRealPointer((EMU_U32)bind->client);
#ifdef EMU_LOG
		EmuLog("  sif_bind %x\n", bind->rpc_number);
#endif
		switch (bind->rpc_number)
		{
		case 0x80000006:
			server = (struct t_rpc_server_data*)&psR[0x1800];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc01800;
			break;

		case PAD_BIND_RPC_ID1:
			server = (struct t_rpc_server_data*)&psR[0x2000];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc02000;
			break;

		case PAD_BIND_RPC_ID2:
			server = (struct t_rpc_server_data*)&psR[0x2800];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc02800;
			break;

		case RPC_NPM_USER:
			server = (struct t_rpc_server_data*)&psR[0x3000];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc03000;
			break;

		case 0x80000001: // fileio
			server = (struct t_rpc_server_data*)&psR[0x3800];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc03800;
			break;

		case XPAD_BIND_RPC_ID1:
			server = (struct t_rpc_server_data*)&psR[0x4000];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc04000;
			break;

		case XPAD_BIND_RPC_ID2:
			server = (struct t_rpc_server_data*)&psR[0x4800];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc04800;
			break;

		default: // unknown
			EmuConsole("unknown server=%x\n", bind->rpc_number);
			server = (struct t_rpc_server_data*)&psR[0x1000];
			*(EMU_U32*)&server->client = (EMU_U32)bind->client;
			*(EMU_U32*)&client->server = 0xbfc01000;
			break;
		}
		break;

	case 0x8000000a: // sif_call
		call = (struct t_sif_rpc_call*)dmat->src;
		bout = (EMU_U32*)EmuMemGetRealPointer((EMU_U32)call->receive);
		n_transfer--;
		addr = PS2Regs.R5900Regs.A0.u32_00_31 + n_transfer * sizeof(struct t_sif_dma_transfer);
		dmat = (struct t_sif_dma_transfer*)EmuMemGetRealPointer(addr);
		bin = (EMU_U32*)EmuMemGetRealPointer((EMU_U32)dmat->src);

#ifdef EMU_LOG
		EmuLog("  sif_call %x, server=%x, send_size %x (bin=%x) recv_size=%x (bout=%x)\n",
			call->rpc_number, call->server, call->send_size, (EMU_U32)dmat->src, call->rec_size, (EMU_U32)call->receive);
#endif
		switch ((EMU_U32)call->server)
		{
		case 0xbfc01800: // module
			Emu_Sif_Bios_Call_Module();
			break;

		case 0xbfc02000: // pad1
			Emu_Sif_Bios_Call_Pad1();
			break;

		case 0xbfc02800: // pad2
			Emu_Sif_Bios_Call_Pad2();
			break;

		case 0xbfc03000: // npm
			Emu_Sif_Bios_Call_Npm();
			break;

		case 0xbfc03800: // fileio
			Emu_Sif_Bios_Call_FileIO();
			break;

		case 0xbfc04000: // xpad1
			Emu_Sif_Bios_Call_XPad1();
			break;

		case 0xbfc04800: // xpad2
			Emu_Sif_Bios_Call_XPad2();
			break;

		default:
			/*            for ( int i = 0; i < 32; i++ )
						{
							if ( ( sifServer[ i ].active == 1 ) &&
									   ( sifServer[ i ].server == (EMU_U32)call->server ) )
							{
								break;
							}
							sifServerCall( sifServer[ i ].server );
						}*/
			break;
		}
		break;
	}

	PS2Regs.R5900Regs.V0.u64_00_63 = 1;
}

//
//  void isceSifSetDChain();
//
void Emu_Sif_Bios_isceSetDChain()
{ // 0x78
	EmuMemSetWord(0xb000c000, 0);
	EmuMemSetWord(0xb000c020, 0);

	PS2Regs.R5900Regs.SP.u64_00_63 -= 0x10;

	EmuMemSetWord(0xb000c000, 0x184);
	EmuMemSetWord(PS2Regs.R5900Regs.SP.u32_00_31, EmuMemGetWord(0xb000c000));

	PS2Regs.R5900Regs.SP.u64_00_63 += 0x10;
}

//
//  EMU_I32 k_sceSifSetReg( EMU_I32, EMU_U32);
//
void Emu_Sif_Bios_sceSetReg()
{ // 0x79
	switch (PS2Regs.R5900Regs.A0.u32_00_31)
	{
	case 0x4:
		reg4 = PS2Regs.R5900Regs.A1.u32_00_31;
		break;

	default:
		break;
	}
}

//
//  EMU_I32 k_sceSifGetReg( EMU_I32);
//
void Emu_Sif_Bios_sceGetReg()
{ // 0x7a
#ifdef EMU_LOG
	EmuLog("   a0=%x\n", PS2Regs.R5900Regs.A0.u32_00_31);
#endif

	switch (PS2Regs.R5900Regs.A0.u32_00_31)
	{
	case 0x4:
		PS2Regs.R5900Regs.V0.u64_00_63 = reg4 | 0x40000;
		break;

	default:
		PS2Regs.R5900Regs.V0.u64_00_63 = 0x60000;
		break;
	}
}
