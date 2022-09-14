#pragma once

#include "EmuPS2Core.h"
#include "Dma.h"

namespace Common
{
	struct stEmu_Gif_Control_Regs
	{
		EMU_U32 CTRL;
		EMU_U08 null0[12];
		EMU_U32 MODE;
		EMU_U08 null1[12];
		EMU_U32 STAT;
		EMU_U08 null2[28];
		EMU_U32 TAG0;
		EMU_U08 null3[12];
		EMU_U32 TAG1;
		EMU_U08 null4[12];
		EMU_U32 TAG2;
		EMU_U08 null5[12];
		EMU_U32 TAG3;
		EMU_U08 null6[12];
		EMU_U32 CNT;
		EMU_U08 null7[12];
		EMU_U32 P3CNT;
		EMU_U08 null8[12];
		EMU_U32 P3TAG;
		EMU_U08 null9[12];
	};

	class Gif
	{
	public:
		// Base Address
		static const EMU_U32 EMU_GIF_START_ADDR = 0x10003000;
		static const EMU_U32 EMU_GIF_END_ADDR = 0x100037FF;

		// Control Register Address
		static const EMU_U32 EMU_GIF_CTRL = 0x00000000;
		static const EMU_U32 EMU_GIF_MODE = 0x00000010;
		static const EMU_U32 EMU_GIF_STAT = 0x00000020;
		static const EMU_U32 EMU_GIF_TAG0 = 0x00000040;
		static const EMU_U32 EMU_GIF_TAG1 = 0x00000050;
		static const EMU_U32 EMU_GIF_TAG2 = 0x00000060;
		static const EMU_U32 EMU_GIF_TAG3 = 0x00000070;
		static const EMU_U32 EMU_GIF_CNT = 0x00000080;
		static const EMU_U32 EMU_GIF_P3CNT = 0x00000090;
		static const EMU_U32 EMU_GIF_P3TAG = 0x000000A0;

		// Initializes Gif memory pointers and registers
		void Init(void);
		// Resets Gif registers
		void Reset(void);
		// Callback for Gif access
		void Callback(EMU_U32 Address);
		// Gif transfer to GS
		void Transfer(stEmu_Dma_Channel_Regs* Channel);

		EMU_U08* GetPointer(EMU_U32 Address);

		static Gif& GetInstance();

		Gif(const Gif&) = delete;
		const Gif& operator=(const Gif&) = delete;
		Gif(Gif&&) = delete;
		Gif& operator=(Gif&&) = delete;

	private:
		Gif();

		EMU_U08 EmuGifMemory[EMU_GIF_END_ADDR - EMU_GIF_START_ADDR];

		// Pointer to control1 registers
		stEmu_Gif_Control_Regs* Emu_Gif_Control_Reg;
	};
}