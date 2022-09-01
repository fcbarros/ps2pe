/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuSif.h
// Version: 1.001
// Desc: Main Header File for PS2 Sif handling
//
// Author: Fabio "GoldFinger" Barros
// Created: 30/04/2002
// Modifications:
// * Version 1.000 ( 30/04/2002 ): Created
// * Version 1.001 ( 14/05/2002 ): Modified
//		- Comments included
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_SIF_H__
#define __EMU_SIF_H__

#include "EmuMemory.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Base Address
#define EMU_SIF_START_ADDR      0x1000F230
#define EMU_SIF_END_ADDR        0x1000F41F

// Control Register Address
#define EMU_SIF_SB_SMFLG        0x00000000

#define NPM_PUTS     0x01
#define RPC_NPM_USER 0x014d704e

#define PS_RDONLY   0x0001
#define PS_WRONLY   0x0002
#define PS_RDWR     0x0003
#define PS_NBLOCK   0x0010
#define PS_APPEND   0x0100
#define PS_CREAT    0x0200
#define PS_TRUNC    0x0400
#define PS_NOWAIT   0x8000

#define FILEIO_MAX  32

typedef struct 
{
    EMU_I08 name[256];
    EMU_I08 serv[256];
    EMU_I32 mode;
    EMU_I32 fd;
} _fileio;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct
{
    EMU_U32 SB_SMFLG;
    EMU_U08 null0[12];
} stEmu_Sif_Control_Regs;


// Others

#define SYSTEM_CMD  0x80000000

struct t_sif_cmd_header
{
   EMU_U32  size;
   void     * dest;
   int      command;
   EMU_U32  unknown;
};

struct t_sif_dma_transfer
{
   void     * src,
            * dest;
   int      size;
   int      attr;
};

struct t_sif_handler
{
   void     (*handler)( void * a, void * b );
   void     * buff;
};

// RPC
struct t_sif_rpc_rend
{
   struct t_sif_cmd_header  sifcmd;
   int              rec_id;     /* 04 */
   void             *pkt_addr;  /* 05 */
   int              rpc_id;     /* 06 */
                                
   struct t_rpc_client_data *client;    /* 7 */
   EMU_U32              command;    /* 8 */
   struct t_rpc_server_data *server;    /* 9 */
   void             *buff,      /* 10 */
                    *buff2;     /* 11 */
};

struct t_sif_rpc_other_data
{
   struct t_sif_cmd_header      sifcmd;
   int              rec_id;     /* 04 */
   void             *pkt_addr;  /* 05 */
   int              rpc_id;     /* 06 */
                                
   struct t_rpc_receive_data    * receive;  /* 07 */
   void             *src;       /* 08 */
   void             *dest;      /* 09 */
   int              size;       /* 10 */
};

struct t_sif_rpc_bind
{
   struct t_sif_cmd_header  sifcmd;
   int              rec_id;     /* 04 */
   void             *pkt_addr;  /* 05 */
   int              rpc_id;     /* 06 */
   struct t_rpc_client_data * client;   /* 07 */
   int              rpc_number; /* 08 */
};

struct t_sif_rpc_call
{
   struct t_sif_cmd_header  sifcmd;
   int              rec_id;     /* 04 */
   void             *pkt_addr;  /* 05 */
   int              rpc_id;     /* 06 */
   struct t_rpc_client_data * client;   /* 07 */
   int              rpc_number; /* 08 */
   int              send_size;  /* 09 */
   void             *receive;   /* 10 */
   int              rec_size;   /* 11 */
   int              has_async_ef;   /* 12 */
   struct t_rpc_server_data * server;   /* 13 */
};

struct t_rpc_server_data
{
   int              command;    /* 04   00 */

   void  *          (*func)(EMU_U32, void *, int);  /* 05   01 */
   void             * buff;     /* 06   02 */
   int              size;       /* 07   03 */

   void  *          (*func2)(EMU_U32, void *, int); /* 08   04 */
   void             * buff2;        /* 09   05 */
   int              size2;      /* 10   06 */

   struct t_rpc_client_data *client;    /* 11   07 */
   void             * pkt_addr; /* 12   08 */
   int              rpc_number; /* 13   09 */

   void             * receive;  /* 14   10 */
   int              rec_size;   /* 15   11 */
   int              has_async_ef;   /* 16   12 */
   int              rec_id;     /* 17   13 */

   struct t_rpc_server_data * link;     /* 18   14 */
   struct r_rpc_server_data * next;     /* 19   15 */
   struct t_rpc_data_queue  * queued_object;    /* 20   16 */
};


struct t_rpc_header
{
   void             * pkt_addr; /* 04   00 */
   EMU_U32              rpc_id;     /* 05   01 */
   int              sema_id;    /* 06   02 */
   EMU_U32              mode;       /* 07   03 */
};


struct t_rpc_client_data
{
   struct t_rpc_header      hdr;
   EMU_U32          command;            /* 04   08 */
   void             * buff,             /* 05   09 */
                    * buff2;            /* 06   10 */
   void             (*end_function)( void *);  /* 07   11 */
   void             * end_param;        /* 08   12*/
   struct t_rpc_server_data * server;   /* 09   13 */
};

struct t_rpc_receive_data
{
   struct t_rpc_header      hdr;
   void             * src,      /* 04 */
                    * dest;     /* 05 */
   int              size;       /* 06 */
};

struct t_rpc_data_queue
{
   int              thread_id,  /* 00 */
                    active;     /* 01 */
   struct t_rpc_server_data *svdata_ref,    /* 02 */
                    * start,        /* 03 */
                    * end;      /* 04 */
   struct t_rpc_data_queue  *next;      /* 05 */
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Initialization of Sif memory pointers and registers
void Emu_Sif_Init( void );
// Resets Sif registers
void Emu_Sif_Reset( void );
// Callback for Sif access
void Emu_Sif_callback( EMU_U32 Address );

EMU_U08 * Emu_Sif_GetPointer( EMU_U32 Address );

void Emu_Sif_Bios_sceGetReg( void );
void Emu_Sif_Bios_sceSetReg( void );
void Emu_Sif_Bios_isceSetDChain( void );
void Emu_Sif_Bios_isceSetDma( void );
void Emu_Sif_Bios_isceDmaStat( void );

#endif