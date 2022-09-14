#pragma once

#include "EmuPS2Core.h"

#include "Intc.h"

namespace Common
{
	struct stEmu_Dma_Channel_Regs
	{
		EMU_U32 CHCR;
		EMU_U08 null0[12];
		EMU_U32 MADR;
		EMU_U08 null1[12];
		EMU_U32 QWC;
		EMU_U08 null2[12];
		EMU_U32 TADR;
		EMU_U08 null3[12];
		EMU_U32 ASR0;
		EMU_U08 null4[12];
		EMU_U32 ASR1;
		EMU_U08 null5[44];
		EMU_U32 SADR;
	};

	struct stEmu_Dma_Control1_Regs
	{
		EMU_U32 CTRL;
		EMU_U08 null0[12];
		EMU_U32 STAT;
		EMU_U08 null1[12];
		EMU_U32 PCR;
		EMU_U08 null2[12];
		EMU_U32 SQWC;
		EMU_U08 null3[12];
		EMU_U32 RBSR;
		EMU_U08 null4[12];
		EMU_U32 RBOR;
		EMU_U08 null5[12];
		EMU_U32 STADR;
		EMU_U08 null6[12];
	};

	struct stEmu_Dma_Control2_Regs
	{
		EMU_U32 ENABLER;
		EMU_U08 null0[108];
		EMU_U32 ENABLEW;
	};

	struct stEmuDMAChannel
	{
		bool Running;
	};

	struct t_sif_dma_transfer
	{
		void* src,
			* dest;
		int size;
		int attr;
	};

	void Emu_Dma_BiosAddHandler(void);
	void Emu_Dma_BiosRemoveHandler(void);
	void Emu_Dma_BiosEnableHandler(void);

	class Dma
	{
	public:
		// Base Channel Address
		static const EMU_U32 EMU_DMA_START_ADDR = 0x10008000;
		static const EMU_U32 EMU_DMA_END_ADDR = 0x1000EFFF;
		static const EMU_U32 EMU_DMA_ENAB_START_ADDR = 0x1000F520;
		static const EMU_U32 EMU_DMA_ENAB_END_ADDR = 0x1000F5FF;

		static const EMU_U32 EMU_DMA_0 = 0x10008000;
		static const EMU_U32 EMU_DMA_1 = 0x10009000;
		static const EMU_U32 EMU_DMA_2 = 0x1000A000;
		static const EMU_U32 EMU_DMA_3 = 0x1000B000;
		static const EMU_U32 EMU_DMA_4 = 0x1000B400;
		static const EMU_U32 EMU_DMA_5 = 0x1000C000;
		static const EMU_U32 EMU_DMA_6 = 0x1000C400;
		static const EMU_U32 EMU_DMA_7 = 0x1000C800;
		static const EMU_U32 EMU_DMA_8 = 0x1000D000;
		static const EMU_U32 EMU_DMA_9 = 0x1000D400;
		static const EMU_U32 EMU_DMA_CONTROL1 = 0x1000E000;
		static const EMU_U32 EMU_DMA_CONTROL2 = 0x1000F520;

		// Channel Register Address
		static const EMU_U32 EMU_DMA_CHANNEL_CHCR = 0x00000000;
		static const EMU_U32 EMU_DMA_CHANNEL_MADR = 0x00000010;
		static const EMU_U32 EMU_DMA_CHANNEL_QWC = 0x00000020;
		static const EMU_U32 EMU_DMA_CHANNEL_TADR = 0x00000030;
		static const EMU_U32 EMU_DMA_CHANNEL_ASR0 = 0x00000040;
		static const EMU_U32 EMU_DMA_CHANNEL_ASR1 = 0x00000050;
		static const EMU_U32 EMU_DMA_CHANNEL_SADR = 0x00000080;

		// Control 1 Register Address
		static const EMU_U32 EMU_DMA_REG_CTRL = 0x00000000;
		static const EMU_U32 EMU_DMA_REG_STAT = 0x00000010;
		static const EMU_U32 EMU_DMA_REG_PCR = 0x00000020;
		static const EMU_U32 EMU_DMA_REG_SQWC = 0x00000030;
		static const EMU_U32 EMU_DMA_REG_RBSR = 0x00000040;
		static const EMU_U32 EMU_DMA_REG_RBOR = 0x00000050;
		static const EMU_U32 EMU_DMA_REG_STADR = 0x00000060;

		// Control 2 Register Address
		static const EMU_U32 EMU_DMA_REG_ENABLER = 0x00000000;
		static const EMU_U32 EMU_DMA_REG_ENABLEW = 0x00000070;

		static const EMU_U32 EM_DMA_MODE_NORMAL = 0x00000000;
		static const EMU_U32 EM_DMA_MODE_CHAIN = 0x00000004;
		static const EMU_U32 EM_DMA_MODE_INTERLEAVE = 0x00000008;

		// Initializes the DMA memory pointers and registers
		void Init(void);
		// Resets DMA registers
		void Reset(void);
		// Callback for DMA access
		static void Callback(EMU_U32 Address);

		void Start(unsigned int ChannelIndex);
		// General set DMA access
		void Set(EMU_U32 Address, EMU_U64 ChannelIndex);

		void DestChain(EMU_U08 ChannelIndex, stEmu_Dma_Channel_Regs* Channel);

		void AddHandler(EMU_U32 HandlerIndex, EMU_U32 FuncAddress);
		void RemoveHandler(EMU_U32 HandlerIndex);
		EMU_U64 Enable(EMU_U32 HandlerIndex);

		void ExitHandler(void);
		void CallHandler(EMU_U32 HandlerIndex);

		EMU_U08* GetPointer(EMU_U32 Address);

		static Dma& GetInstance();

		Dma(const Dma&) = delete;
		Dma(Dma&&) = delete;
		Dma& operator=(const Dma&) = delete;
		Dma& operator=(Dma&&) = delete;

	private:
		Dma();

		EMU_U08 EmuDmaMemory[EMU_DMA_END_ADDR - EMU_DMA_START_ADDR];
		EMU_U08 EmuDmaEnablerMemory[EMU_DMA_ENAB_END_ADDR - EMU_DMA_ENAB_START_ADDR];

		// Pointer to channel registers in memory
		stEmu_Dma_Channel_Regs* Emu_Dma_Channel_Reg[10];
		// Pointer to control1 registers
		stEmu_Dma_Control1_Regs* Emu_Dma_Control1_Reg;
		// Pointer to control2 registers
		stEmu_Dma_Control2_Regs* Emu_Dma_Control2_Reg;

		stEmuDMAChannel EmuDmaChannel[10];

		stEMU_INTC_HANDLER Emu_Dma_Handler[32];

		void Stop(unsigned int ChannelIndex);
		void EmuDmaTransfer(EMU_U08 ChannelIndex, EMU_U32 Address, EMU_U32 Qwc);
	};
}