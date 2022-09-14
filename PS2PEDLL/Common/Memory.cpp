#include "Memory.h"

#include "Timer.h"
#include "Gif.h"
#include "Dma.h"
#include "Intc.h"
#include "Ipu.h"
#include "Vif.h"
#include "Fifo.h"
#include "Sif.h"
#include "Bios.h"
#include "Vu.h"
#include "Gs.h"

namespace Common
{
	Memory::Memory()
	{
	}

	Memory& Memory::GetInstance()
	{
		static Memory memory;
		return memory;
	}

	void Memory::Init(EMU_U32 gMemorySize, EMU_U32 gPageSize)
	{
		MemorySize = gMemorySize;
		PageSize = 65536; //gPageSize;
		MemoryMask = PageSize - 1;
		PageMask = ~(PageSize - 1);
		NumberOfFrames = MemorySize / PageSize;
		NumberOfPages = (EMU_U32)((EMU_U64)0x100000000 / (EMU_U64)PageSize);

		CurrentNumberOfPages = 0;

		MainMemory.Byte = new EMU_U08[MemorySize];
		memset(MainMemory.Byte, 0, MemorySize);

		Mapping = new EMM_MapType[NumberOfPages];

		RaftMemory = new EMU_U08[1024 * 1024]; // 1 Megabyte

		Clear();
	}

	void Memory::Shutdown()
	{
		delete[] MainMemory.Byte;

		delete[] RaftMemory;
	}

	void Memory::Clear()
	{
		memset(Mapping, 0, NumberOfPages * sizeof(EMM_MapType));
		CurrentNumberOfPages = 0;
	}

	EMU_U08* Memory::GetRealPointer(EMU_U32 Address)
	{
		if ((Address >= 0x80000000) && (Address < 0xA0000000))
		{
			Address -= 0x80000000;
		}
		else if ((Address >= 0xA0000000) && (Address < 0xC0000000))
		{
			Address -= 0xA0000000;
		}
		if ((Address >= Timer::EMU_TIMER_START_ADDR) && (Address <= Gs::EMU_GS_PRIV_END_ADDR))
		{
			if (Address < Ipu::EMU_IPU_START_ADDR)
			{
				return Timer::GetInstance().GetPointer(Address);
			}
			else if (Address < Gif::EMU_GIF_START_ADDR)
			{
				return Ipu::GetInstance().GetPointer(Address);
			}
			else if (Address < Vif::EMU_VIF_START_ADDR)
			{
				return Gif::GetInstance().GetPointer(Address);
			}
			else if (Address < Fifo::EMU_FIFO_START_ADDR)
			{
				return Vif::GetInstance().GetPointer(Address);
			}
			else if (Address < Dma::EMU_DMA_START_ADDR)
			{
				return Fifo::GetInstance().GetPointer(Address);
			}
			else if (Address < Intc::EMU_INTC_START_ADDR)
			{
				return Dma::GetInstance().GetPointer(Address);
			}
			else if (Address < Sif::EMU_SIF_START_ADDR)
			{
				return Intc::GetInstance().GetPointer(Address);
			}
			else if (Address < Dma::EMU_DMA_ENAB_START_ADDR)
			{
				return Sif::GetInstance().GetPointer(Address);
			}
			else if (Address < Vu::EMU_VU_START_ADDR)
			{
				return Dma::GetInstance().GetPointer(Address);
			}
			else if (Address < Gs::EMU_GS_PRIV_START_ADDR)
			{
				return Vu::GetInstance().GetPointer(Address);
			}
			else
			{
				return Gs::GetInstance().GetPointer(Address);
			}
		}
		else if ((Address >= Bios::EMU_BIOS_START_ADDR) && (Address <= Bios::EMU_BIOS_END_ADDR))
		{
			return Bios::GetInstance().GetPointer(Address);
		}
		else
		{
			EMM_MapType& Entry = Mapping[Address >> 16];
			if (!Entry.InMemory)
			{
				Replace(Address);
			}

			return &MainMemory.Byte[Entry.Page | (Address & MemoryMask)];
		}
	}

	void Memory::SetByte(EMU_U32 Address, EMU_U08 Data)
	{
		*(EMU_U08*)GetRealPointer(Address) = Data;
		CallCallBackWrite(Address);
	}

	void Memory::SetShort(EMU_U32 Address, EMU_U16 Data)
	{
		*(EMU_U16*)GetRealPointer(Address) = Data;
		CallCallBackWrite(Address);
	}

	void Memory::SetWord(EMU_U32 Address, EMU_U32 Data)
	{
		*(EMU_U32*)GetRealPointer(Address) = Data;
		CallCallBackWrite(Address);
	}

	void Memory::SetDWord(EMU_U32 Address, EMU_U64 Data)
	{
		*(EMU_U64*)GetRealPointer(Address) = Data;
		CallCallBackWrite(Address);
	}

	EMU_U08 Memory::GetByte(EMU_U32 Address)
	{
		return *(EMU_U08*)GetRealPointer(Address);
	}

	EMU_U16 Memory::GetShort(EMU_U32 Address)
	{
		return *(EMU_U16*)GetRealPointer(Address);
	}

	EMU_U32 Memory::GetWord(EMU_U32 Address)
	{
		return *(EMU_U32*)GetRealPointer(Address);
	}

	EMU_U64 Memory::GetDWord(EMU_U32 Address)
	{
		return *(EMU_U64*)GetRealPointer(Address);
	}

	void Memory::Replace(EMU_U32 Address)
	{
		EMM_MapType& Entry = Mapping[Address >> 16];

		if (CurrentNumberOfPages < NumberOfFrames)
		{
			Entry.Page = CurrentNumberOfPages * PageSize;
			CurrentNumberOfPages++;
		}

		Entry.InMemory = 1;
		Entry.Modified = 1;
		Entry.LastAcessed = GetTime();
	}

	void Memory::Load(EMU_U32 Address)
	{
		FILE* pPageFile = NULL;
		char FileName[256];

		sprintf(FileName, "fPage%u.tmp", Address & PageMask);

		pPageFile = fopen(FileName, "rb");

		EMM_MapType& Entry = Mapping[Address >> 16];

		fread(&MainMemory.Byte[Entry.Page | (Address & MemoryMask)], PageSize, 1, pPageFile);

		fclose(pPageFile);

		Entry.Modified = 0;
	}

	void Memory::Save(EMU_U32 Address)
	{
		FILE* pPageFile = NULL;
		char FileName[256];

		sprintf(FileName, "fPage%u.tmp", Address & PageMask);

		pPageFile = fopen(FileName, "wb");

		EMM_MapType& Entry = Mapping[Address >> 16];

		fwrite(&MainMemory.Byte[Entry.Page | (Address & MemoryMask)], PageSize, 1, pPageFile);

		fclose(pPageFile);

		Entry.Modified = 0;
	}

	void Memory::AddWriteCallBack(EMU_U32 StartAddress, EMU_U32 EndAddress, EMM_WRITECALLBACK CallBack)
	{
		stTLB_WriteCallBack CallBack_temp;

		CallBack_temp.StartAddress = StartAddress;
		CallBack_temp.EndAddress = EndAddress;
		CallBack_temp.CallBack = CallBack;

		WriteCallBacks.push_back(CallBack_temp);
	}

	void Memory::AddReadCallBack(EMU_U32 StartAddress, EMU_U32 EndAddress, EMM_READCALLBACK CallBack)
	{
		stTLB_ReadCallBack CallBack_temp;

		CallBack_temp.StartAddress = StartAddress;
		CallBack_temp.EndAddress = EndAddress;
		CallBack_temp.CallBack = CallBack;

		ReadCallBacks.push_back(CallBack_temp);
	}

	void Memory::CallCallBackWrite(EMU_U32 Address)
	{
		for (unsigned int i = 0; i < WriteCallBacks.size(); i++)
		{
			if ((Address >= WriteCallBacks[i].StartAddress) &&
				(Address < WriteCallBacks[i].EndAddress))
			{
				(*WriteCallBacks[i].CallBack)(Address);
				return;
			}
		}
	}

	void Memory::CallCallBackRead(EMU_U32 Address, EMU_U08* RealAddress)
	{
		for (unsigned int i = 0; i < ReadCallBacks.size(); i++)
		{
			if ((Address >= ReadCallBacks[i].StartAddress) &&
				(Address < ReadCallBacks[i].EndAddress))
			{
				(*ReadCallBacks[i].CallBack)(Address, RealAddress);
				return;
			}
		}
	}

	EMU_U64 Memory::GetTime()
	{
		EMU_U64 Ticks = 0;
		//RDTSC(Ticks);
		return Ticks;
	}

	bool Memory::AddFromFile(const char* FileName, EMU_U32 Offset, EMU_U32 FileSize, EMU_U32 EmuAddress, EMU_U08 Permissions)
	{
		FILE* pFile = fopen(FileName, "rb");
		if (!pFile)
		{
			return false;
		}
		//    EMU_U08 Bytetmp;

		fseek(pFile, Offset, SEEK_SET);

		EMU_U32 StartPage = EmuAddress >> 16;
		EMU_U32 EndPage = (EmuAddress + FileSize) >> 16;

		EMM_MapType Entry = Mapping[StartPage];
		if (!Entry.InMemory)
		{
			Replace(EmuAddress);
			Entry = Mapping[EmuAddress >> 16];
		}

		if (EndPage == StartPage)
		{
			fread(&MainMemory.Byte[Entry.Page | (EmuAddress & MemoryMask)], 1, FileSize, pFile);
		}
		else
		{
			EMU_U32 RealEndAddr = EmuAddress + FileSize;
			EMU_U32 BufferPos = 0;
			EMU_U32 EndAddr = (EmuAddress & 0xFFFF0000) + PageSize;
			while (BufferPos < FileSize)
			{
				fread(&MainMemory.Byte[Entry.Page | (EmuAddress & MemoryMask)], 1, EndAddr - EmuAddress, pFile);
				BufferPos += EndAddr - EmuAddress;
				EmuAddress = EndAddr;
				Entry = Mapping[EmuAddress >> 16];
				if (!Entry.InMemory)
				{
					Replace(EmuAddress);
					Entry = Mapping[EmuAddress >> 16];
				}
				EndAddr = (EmuAddress & 0xFFFF0000) + PageSize;
				if (EndAddr > RealEndAddr)
				{
					EndAddr = RealEndAddr;
				}
			}
		}

		fclose(pFile);

		return true;
	}

	EMU_U08* Memory::ReadContinuosArea(EMU_U32 Address, EMU_U32 Size)
	{
		if ((Address >= 0x80000000) && (Address < 0xA0000000))
		{
			Address -= 0x80000000;
		}
		else if ((Address >= 0xA0000000) && (Address < 0xC0000000))
		{
			Address -= 0xA0000000;
		}
		if ((Address >= Timer::EMU_TIMER_START_ADDR) && (Address <= Gs::EMU_GS_PRIV_END_ADDR))
		{
			if (Address < Ipu::EMU_IPU_START_ADDR)
			{
				return Timer::GetInstance().GetPointer(Address);
			}
			else  if (Address < Gif::EMU_GIF_START_ADDR)
			{
				return Ipu::GetInstance().GetPointer(Address);
			}
			else if (Address < Vif::EMU_VIF_START_ADDR)
			{
				return Gif::GetInstance().GetPointer(Address);
			}
			else if (Address < Fifo::EMU_FIFO_START_ADDR)
			{
				return Vif::GetInstance().GetPointer(Address);
			}
			else if (Address < Dma::EMU_DMA_START_ADDR)
			{
				return Fifo::GetInstance().GetPointer(Address);
			}
			else if (Address < Intc::EMU_INTC_START_ADDR)
			{
				return Dma::GetInstance().GetPointer(Address);
			}
			else if (Address < Sif::EMU_SIF_START_ADDR)
			{
				return Intc::GetInstance().GetPointer(Address);
			}
			else if (Address < Dma::EMU_DMA_ENAB_START_ADDR)
			{
				return Sif::GetInstance().GetPointer(Address);
			}
			else if (Address < Vu::EMU_VU_START_ADDR)
			{
				return Dma::GetInstance().GetPointer(Address);
			}
			else if (Address < Gs::EMU_GS_PRIV_START_ADDR)
			{
				return Vu::GetInstance().GetPointer(Address);
			}
			else
			{
				return Gs::GetInstance().GetPointer(Address);
			}
		}
		else if ((Address >= Bios::EMU_BIOS_START_ADDR) && (Address <= Bios::EMU_BIOS_END_ADDR))
		{
			return Bios::GetInstance().GetPointer(Address);
		}
		else
		{
			EMU_U32 StartPage = Address >> 16;
			EMU_U32 EndPage = (Address + Size) >> 16;

			EMM_MapType Entry = Mapping[StartPage];
			if (!Entry.InMemory)
			{
				Replace(Address);
				Entry = Mapping[Address >> 16];
			}

			if (EndPage == StartPage)
			{
				return &MainMemory.Byte[Entry.Page | (Address & MemoryMask)];
			}
			else
			{
				EMU_U32 RealEndAddr = Address + Size;
				EMU_U32 BufferPos = 0;
				EMU_U32 EndAddr = (Address & 0xFFFF0000) + PageSize;
				while (BufferPos < Size)
				{
					memcpy(&RaftMemory[BufferPos], &MainMemory.Byte[Entry.Page | (Address & MemoryMask)], EndAddr - Address);
					BufferPos += EndAddr - Address;
					Address = EndAddr;
					Entry = Mapping[Address >> 16];
					if (!Entry.InMemory)
					{
						Replace(Address);
						Entry = Mapping[Address >> 16];
					}
					EndAddr = (Address & 0xFFFF0000) + PageSize;
					if (EndAddr > RealEndAddr)
					{
						EndAddr = RealEndAddr;
					}
				}

				return RaftMemory;
			}
		}
	}

	EMU_U08* Memory::GetScratchPad()
	{
		return EMemScratchPad;
	}

/*
// Types for memory area - Not used
#define EMMT_CODE       0
#define EMMT_DATA       1
#define EMMT_STACK      2
#define EMMT_RESERVED   3

// Permisions for memory area - Not used
#define EMMP_READ       0x1
#define EMMP_WRITE      0x2
#define EMMP_EXEC       0x4
*/
}