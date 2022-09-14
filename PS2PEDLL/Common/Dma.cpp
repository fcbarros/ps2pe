#include "Dma.h"

#include "EmuMain.h"

#include "Gif.h"
#include "Memory.h"

namespace Common
{
	Dma::Dma()
	{
	}

	Dma& Dma::GetInstance()
	{
		static Dma dma;
		return dma;
	}

	void Dma::Init()
	{
		Emu_Dma_Channel_Reg[0] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_0 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[1] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_1 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[2] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_2 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[3] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_3 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[4] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_4 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[5] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_5 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[6] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_6 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[7] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_7 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[8] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_8 - EMU_DMA_START_ADDR];
		Emu_Dma_Channel_Reg[9] = (stEmu_Dma_Channel_Regs*)&EmuDmaMemory[EMU_DMA_9 - EMU_DMA_START_ADDR];

		Emu_Dma_Control1_Reg = (stEmu_Dma_Control1_Regs*)&EmuDmaMemory[EMU_DMA_CONTROL1 - EMU_DMA_START_ADDR];
		Emu_Dma_Control2_Reg = (stEmu_Dma_Control2_Regs*)&EmuDmaEnablerMemory[EMU_DMA_CONTROL2 - EMU_DMA_ENAB_START_ADDR];

		Reset();
	}

	void Dma::Reset()
	{
		for (int i = 0; i < 10; i++)
		{
			memset(Emu_Dma_Channel_Reg[i], 0, sizeof(stEmu_Dma_Channel_Regs));

			Emu_Dma_Channel_Reg[i]->CHCR = 0x00000000;
			Emu_Dma_Channel_Reg[i]->MADR = 0x00000000;
			Emu_Dma_Channel_Reg[i]->TADR = 0x00000000;
			Emu_Dma_Channel_Reg[i]->ASR0 = 0x00000000;
			Emu_Dma_Channel_Reg[i]->ASR1 = 0x00000000;
			Emu_Dma_Channel_Reg[i]->SADR = 0x00000000;
			Emu_Dma_Channel_Reg[i]->QWC = 0x00000000;

			EmuDmaChannel[i].Running = false;
		}
		memset(Emu_Dma_Control1_Reg, 0, sizeof(stEmu_Dma_Control1_Regs));
		memset(Emu_Dma_Control2_Reg, 0, sizeof(stEmu_Dma_Control2_Regs));

		Emu_Dma_Control1_Reg->CTRL = 0x00000000;
		Emu_Dma_Control1_Reg->STAT = 0x00000000;
		Emu_Dma_Control1_Reg->PCR = 0x00000000;
		Emu_Dma_Control1_Reg->SQWC = 0x00000000;
		Emu_Dma_Control1_Reg->RBOR = 0x00000000;
		Emu_Dma_Control1_Reg->RBSR = 0x00000000;
		Emu_Dma_Control1_Reg->STADR = 0x00000000;

		Emu_Dma_Control2_Reg->ENABLEW = 0x00000000;
		Emu_Dma_Control2_Reg->ENABLER = 0x00000000;

		memset(Emu_Dma_Handler, 0, sizeof(Emu_Dma_Handler));
	}

	void Dma::Callback(EMU_U32 Address)
	{
		EMU_U32 Register;
		int ChannelIndex = -1;

		if (Address < EMU_DMA_1)
		{
			// DMA Channel 0
			Register = Address - EMU_DMA_0;
			ChannelIndex = 0;
		}
		else if (Address < EMU_DMA_2)
		{
			// DMA Channel 1
			Register = Address - EMU_DMA_1;
			ChannelIndex = 1;
		}
		else if (Address < EMU_DMA_3)
		{
			// DMA Channel 2
			Register = Address - EMU_DMA_2;
			ChannelIndex = 2;
		}
		else if (Address < EMU_DMA_4)
		{
			// DMA Channel 3
			Register = Address - EMU_DMA_3;
			ChannelIndex = 3;
		}
		else if (Address < EMU_DMA_5)
		{
			// DMA Channel 4
			Register = Address - EMU_DMA_4;
			ChannelIndex = 4;
		}
		else if (Address < EMU_DMA_6)
		{
			// DMA Channel 5
			Register = Address - EMU_DMA_5;
			ChannelIndex = 5;
		}
		else if (Address < EMU_DMA_7)
		{
			// DMA Channel 6
			Register = Address - EMU_DMA_6;
			ChannelIndex = 6;
		}
		else if (Address < EMU_DMA_8)
		{
			// DMA Channel 7
			Register = Address - EMU_DMA_7;
			ChannelIndex = 7;
		}
		else if (Address < EMU_DMA_9)
		{
			// DMA Channel 8
			Register = Address - EMU_DMA_8;
			ChannelIndex = 8;
		}
		else if (Address < EMU_DMA_CONTROL1)
		{
			// DMA Channel 9
			Register = Address - EMU_DMA_9;
			ChannelIndex = 9;
		}
		else if (Address < EMU_DMA_CONTROL2)
		{
			// DMA Control1
			Register = Address - EMU_DMA_CONTROL1;

			switch (Register)
			{
			case EMU_DMA_REG_CTRL:   break;
			case EMU_DMA_REG_STAT:   break;
			case EMU_DMA_REG_PCR:    break;
			case EMU_DMA_REG_SQWC:   break;
			case EMU_DMA_REG_RBSR:   break;
			case EMU_DMA_REG_RBOR:   break;
			case EMU_DMA_REG_STADR:  break;
			default:                 break;
			}
		}
		else
		{
			// DMA Control2
			Register = Address - EMU_DMA_CONTROL2;

			switch (Register)
			{
			case EMU_DMA_REG_ENABLER:   break;
			case EMU_DMA_REG_ENABLEW:   break;
			default:                    break;
			}
		}

		if (ChannelIndex >= 0)
		{
			switch (Register)
			{
			case EMU_DMA_CHANNEL_CHCR:
				if (Emu_Dma_Channel_Reg[ChannelIndex]->CHCR & 0x00000100)
				{
					if (!EmuDmaChannel[ChannelIndex].Running)
					{
						Start(ChannelIndex);
					}
				}
				else
				{
					if (EmuDmaChannel[ChannelIndex].Running)
					{
						Stop(ChannelIndex);
					}
				}
				break;
			case EMU_DMA_CHANNEL_MADR:
			case EMU_DMA_CHANNEL_QWC:
			case EMU_DMA_CHANNEL_TADR:
			case EMU_DMA_CHANNEL_ASR0:
			case EMU_DMA_CHANNEL_ASR1:
				break;
			}
		}
	}

	void Dma::Stop(unsigned int ChannelIndex)
	{
		stEmu_Dma_Channel_Regs* Channel = Emu_Dma_Channel_Reg[ChannelIndex];

		EmuDmaChannel[2].Running = false;

		Channel->CHCR &= 0xFFFFFEFF;
	}

	void Dma::Start(unsigned int ChannelIndex)
	{
		stEmu_Dma_Channel_Regs* Channel = Emu_Dma_Channel_Reg[ChannelIndex];

#ifdef EMU_LOG
		EmuLog("Dma start: Channel: %u, Mode: %u\n", ChannelIndex, Channel->CHCR & 0x0000000C);
#endif

		EmuDmaChannel[ChannelIndex].Running = true;

		Channel->CHCR |= 0x00000100;

		switch (Channel->CHCR & 0x0000000C)
		{
		case EM_DMA_MODE_NORMAL:
			switch (ChannelIndex)
			{
			case 2:
				Gif::GetInstance().Transfer(Channel);
				break;

			default:
				break;
			}
			Stop(ChannelIndex);
			break;

		case EM_DMA_MODE_CHAIN:
			switch (ChannelIndex)
			{
			case 2:
				Gif::GetInstance().Transfer(Channel);
				break;

			case 5:
				DestChain(ChannelIndex, Channel);
				break;

			default:
				break;
			}
			Stop(ChannelIndex);
			break;

		case EM_DMA_MODE_INTERLEAVE:
			Stop(ChannelIndex);
			break;
		}
	}

	void Dma::EmuDmaTransfer(EMU_U08 ChannelIndex, EMU_U32 Address, EMU_U32 Qwc)
	{
		switch (ChannelIndex)
		{
		case 5:
			break;

		default:
			break;
		}
	}

	void Dma::DestChain(EMU_U08 ChannelIndex, stEmu_Dma_Channel_Regs* Channel)
	{
		if (Channel->CHCR == 0)
		{
			return;
		}

		EMU_U32* pTag;
		EMU_U32 RecLevel = 0;

		EmuDmaTransfer(ChannelIndex, Channel->MADR, Channel->QWC);

		while (Channel->CHCR & 0x00000100)
		{
			pTag = (EMU_U32*)Memory::GetInstance().GetRealPointer(Channel->TADR);
			Channel->QWC = pTag[0] & 0x0000FFFF;
			Channel->CHCR = (Channel->CHCR & 0x0000FFFF) | (pTag[0] & 0xFFFF0000);

			switch ((pTag[0] >> 28) & 0x07)
			{
			case 0: // cnts
				Channel->MADR = Channel->TADR + 16;
				Channel->TADR += 16 + Channel->QWC * 16;
				Emu_Dma_Control1_Reg->STADR = Channel->TADR;
				break;

			case 1: // cnt
				Channel->MADR = Channel->TADR + 16;
				Channel->TADR += 16 + Channel->QWC * 16;
				break;

			case 7: // end
				Channel->MADR = Channel->TADR + 16;
				Channel->CHCR &= 0xFFFFFEFF;
				break;

			default:
				Channel->CHCR &= 0xFFFFFEFF;
				return;
			}

			if (Channel->QWC)
			{
				EmuDmaTransfer(ChannelIndex, Channel->MADR, Channel->QWC);
			}

			if (pTag[0] & 0x80000000) // IRQ
			{
				CallHandler(2);
			}
		}
	}

	void Dma::Set(EMU_U32 Address, EMU_U64 ChannelIndex)
	{
		stEmu_Dma_Channel_Regs* Channel = Emu_Dma_Channel_Reg[ChannelIndex];

		t_sif_dma_transfer* RealAddress = (t_sif_dma_transfer*)Memory::GetInstance().GetRealPointer(Address);
	}

	void Emu_Dma_BiosAddHandler()
	{ // 0x12
#ifdef EMU_LOG
		EmuLog("  Handler: %u, Address: %.8X\n", PS2Regs.R5900Regs.A0.u32_00_31, PS2Regs.R5900Regs.A1.u32_00_31);
#endif
		Dma::GetInstance().AddHandler(PS2Regs.R5900Regs.A0.u32_00_31, PS2Regs.R5900Regs.A1.u32_00_31);
		PS2Regs.R5900Regs.V0.u64_00_63 = 1;
	}

	void Emu_Dma_BiosRemoveHandler()
	{ // 0x13
#ifdef EMU_LOG
		EmuLog("  Handler: %u\n", PS2Regs.R5900Regs.A0.u32_00_31);
#endif
		Dma::GetInstance().RemoveHandler(PS2Regs.R5900Regs.A0.u32_00_31);
		PS2Regs.R5900Regs.V0.u64_00_63 = 1;
	}

	void Emu_Dma_BiosEnableHandler()
	{ // 0x16
#ifdef EMU_LOG
		EmuLog("  Handler: %u\n", PS2Regs.R5900Regs.A0.u32_00_31);
#endif
		PS2Regs.R5900Regs.V0.u64_00_63 = Dma::GetInstance().Enable(PS2Regs.R5900Regs.A0.u32_00_31);
	}

	void Dma::AddHandler(EMU_U32 HandlerIndex, EMU_U32 FuncAddress)
	{ // 0x12
		Emu_Dma_Handler[HandlerIndex].active = 0;
		Emu_Dma_Handler[HandlerIndex].id = HandlerIndex;
		Emu_Dma_Handler[HandlerIndex].func = FuncAddress;
	}

	void Dma::RemoveHandler(EMU_U32 HandlerIndex)
	{ // 0x13
		Emu_Dma_Handler[HandlerIndex].active = 0;
		Emu_Dma_Handler[HandlerIndex].id = 0;
		Emu_Dma_Handler[HandlerIndex].func = NULL;
	}

	EMU_U64 Dma::Enable(EMU_U32 HandlerIndex)
	{ // 0x16
		EMU_U32 tmp = 0x10000 << HandlerIndex;
		EMU_U32 ret = Memory::GetInstance().GetWord(0xb000e010 - 0xa0000000) & tmp;

		if (ret)
		{
			return 0;
		}

		Emu_Dma_Handler[HandlerIndex].active = 1;
		Memory::GetInstance().SetWord(0xb000e010 - 0xa0000000, tmp);

		return 1;
	}

	void Dma::CallHandler(EMU_U32 HandlerIndex)
	{
		if (EmuInterruptIndex > 0)
		{
			ExitHandler();
			//        return;
		}
		if ((Emu_Dma_Handler[HandlerIndex].active) && (PS2Regs.COP0Regs.Status_EIE))
		{
			switch (HandlerIndex)
			{
			case 2:
				if (Intc::GetInstance().GetControlReg()->STAT_GS & ~Intc::GetInstance().GetControlReg()->MASK)
				{
					memcpy(&PS2Regs.R5900RegsBackup[EmuInterruptIndex], &PS2Regs.R5900Regs, sizeof(PS2Regs.R5900Regs));
					//                memcpy( &COP0RegsBackup, &COP0Regs, sizeof( COP0Regs ) );
					//                memcpy( &COP1RegsBackup, &COP1Regs, sizeof( COP1Regs ) );

					PS2Regs.R5900Regs.Reg[31].u64_00_63 = (EMU_I32)0xFFFFFFFE;
					PS2Regs.R5900Regs.PC = Emu_Dma_Handler[HandlerIndex].func;
					EmuInterruptIndex++;
				}
			}
		}
	}

	void Dma::ExitHandler()
	{
		if (EmuInterruptIndex > 0)
		{
			EmuInterruptIndex--;
			memcpy(&PS2Regs.R5900Regs, &PS2Regs.R5900RegsBackup[EmuInterruptIndex], sizeof(PS2Regs.R5900Regs));
			//        memcpy( &COP0Regs, &COP0RegsBackup, sizeof( COP0Regs ) );
			//        memcpy( &COP1Regs, &COP1RegsBackup, sizeof( COP1Regs ) );
		}
	}

	EMU_U08* Dma::GetPointer(EMU_U32 Address)
	{
		if (Address < EMU_DMA_ENAB_START_ADDR)
		{
			return &EmuDmaMemory[Address - EMU_DMA_START_ADDR];
		}
		else
		{
			return &EmuDmaEnablerMemory[Address - EMU_DMA_ENAB_START_ADDR];
		}
	}
}