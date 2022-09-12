#include "EmuMemory.h"
#include "EmuGS.h"
#include "EmuVif.h"
#include "EmuGif.h"
#include "EmuVu.h"
#include "EmuFifo.h"
#include "EmuIntc.h"
#include "EmuDma.h"
#include "EmuIpu.h"
#include "EmuSif.h"
#include "EmuTimer.h"
#include "EmuBios.h"


EMU_U32        MemorySize;
EMU_U32        MemoryMask;
EMU_U32        PageSize;
EMU_U32        PageMask;
EMU_U32        NumberOfFrames;
EMU_U32        NumberOfPages;
EMU_U32        CurrentNumberOfPages;

EMM_Data_Type       MainMemory;
stTLB_Entry* Table;

EMM_MapType* Mapping;
EMM_WriteCallBackType	WriteCallBacks;
EMM_ReadCallBackType	ReadCallBacks;

EMU_U08* RaftMemory;


void EmuMemReplace(EMU_U32 Address);
inline void EmuMemCallCallBackWrite(EMU_U32 Address);
inline void EmuMemCallCallBackRead(EMU_U32 Address);

void EmuMemInit(EMU_U32 gMemorySize, EMU_U32 gPageSize)
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

	EmuMemClear();
}

void EmuMemShutdown()
{
	delete[] MainMemory.Byte;

	delete[] RaftMemory;
}

void EmuMemClear()
{
	memset(Mapping, 0, NumberOfPages * sizeof(EMM_MapType));
	CurrentNumberOfPages = 0;
}

EMU_U08* EmuMemGetRealPointer(EMU_U32 Address)
{
	if ((Address >= 0x80000000) && (Address < 0xA0000000))
	{
		Address -= 0x80000000;
	}
	else
		if ((Address >= 0xA0000000) && (Address < 0xC0000000))
		{
			Address -= 0xA0000000;
		}
	if ((Address >= EMU_TIMER_START_ADDR) &&
		(Address <= EMU_GS_PRIV_END_ADDR))
	{
		if (Address < EMU_IPU_START_ADDR)
		{
			return Emu_Timer_GetPointer(Address);
		}
		else
			if (Address < EMU_GIF_START_ADDR)
			{
				return Emu_Ipu_GetPointer(Address);
			}
			else
				if (Address < EMU_VIF_START_ADDR)
				{
					return Emu_Gif_GetPointer(Address);
				}
				else
					if (Address < EMU_FIFO_START_ADDR)
					{
						return Emu_Vif_GetPointer(Address);
					}
					else
						if (Address < EMU_DMA_START_ADDR)
						{
							return Emu_Fifo_GetPointer(Address);
						}
						else
							if (Address < EMU_INTC_START_ADDR)
							{
								return Emu_Dma_GetPointer(Address);
							}
							else
								if (Address < EMU_SIF_START_ADDR)
								{
									return Emu_Intc_GetPointer(Address);
								}
								else
									if (Address < EMU_DMA_ENAB_START_ADDR)
									{
										return Emu_Sif_GetPointer(Address);
									}
									else
										if (Address < EMU_VU_START_ADDR)
										{
											return Emu_Dma_GetPointer(Address);
										}
										else
											if (Address < EMU_GS_PRIV_START_ADDR)
											{
												return Emu_Vu_GetPointer(Address);
											}
											else
											{
												return Emu_GS_GetPointer(Address);
											}
	}
	else
		if ((Address >= EMU_BIOS_START_ADDR) && (Address <= EMU_BIOS_END_ADDR))
		{
			return Emu_Bios_GetPointer(Address);
		}
		else
		{
			EMM_MapType& Entry = Mapping[Address >> 16];
			if (!Entry.InMemory)
			{
				EmuMemReplace(Address);
			}

			return &MainMemory.Byte[Entry.Page | (Address & MemoryMask)];
		}
}

void EmuMemSetByte(EMU_U32 Address, EMU_U08 Data)
{
	*(EMU_U08*)EmuMemGetRealPointer(Address) = Data;
	EmuMemCallCallBackWrite(Address);
}

void EmuMemSetShort(EMU_U32 Address, EMU_U16 Data)
{
	*(EMU_U16*)EmuMemGetRealPointer(Address) = Data;
	EmuMemCallCallBackWrite(Address);
}

void EmuMemSetWord(EMU_U32 Address, EMU_U32 Data)
{
	*(EMU_U32*)EmuMemGetRealPointer(Address) = Data;
	EmuMemCallCallBackWrite(Address);
}

void EmuMemSetDWord(EMU_U32 Address, EMU_U64 Data)
{
	*(EMU_U64*)EmuMemGetRealPointer(Address) = Data;
	EmuMemCallCallBackWrite(Address);
}

EMU_U08 EmuMemGetByte(EMU_U32 Address)
{
	return *(EMU_U08*)EmuMemGetRealPointer(Address);
}

EMU_U16 EmuMemGetShort(EMU_U32 Address)
{
	return *(EMU_U16*)EmuMemGetRealPointer(Address);
}

EMU_U32 EmuMemGetWord(EMU_U32 Address)
{
	return *(EMU_U32*)EmuMemGetRealPointer(Address);
}

EMU_U64 EmuMemGetDWord(EMU_U32 Address)
{
	return *(EMU_U64*)EmuMemGetRealPointer(Address);
}

void EmuMemReplace(EMU_U32 Address)
{
	EMM_MapType& Entry = Mapping[Address >> 16];

	if (CurrentNumberOfPages < NumberOfFrames)
	{
		Entry.Page = CurrentNumberOfPages * PageSize;
		CurrentNumberOfPages++;
	}
	/*    if ( Mapping.size( ) < NumberOfFrames )
		{
			Entry.Page = ( Mapping.size() - 1 ) * PageSize;
		}
		else
		{
			EMU_U64 minLastAcessed = 0xFFFFFFFFFFFFFFFF;
			EMM_MapType::iterator pReplaced;
			EMM_MapType::iterator pMap = Mapping.begin( );
			while ( pMap != Mapping.end( ) )
			{
				if ( ( pMap->second.InMemory ) &&
					 ( ! pMap->second.MustExist ) &&
					 ( pMap->second.LastAcessed < minLastAcessed ) )
				{
					pReplaced = pMap;
					minLastAcessed = pMap->second.LastAcessed;
				}
				pMap++;
			}
			stTLB_Map &Replace_Entry = pMap->second;
			if ( Replace_Entry.Modified )
			{
				Save( pMap->first );
			}
			Replace_Entry.InMemory = 0;

			Entry.Page = Replace_Entry.Page;
		}
		if ( Entry.Saved )
		{
			Load( Address );
		}
	*/
	Entry.InMemory = 1;
	Entry.Modified = 1;
	Entry.LastAcessed = EmuMemGetTime();
}

void EmuMemLoad(EMU_U32 Address)
{
	FILE* pPageFile = NULL;
	char FileName[256];

	sprintf(FileName, "fPage%u.tmp", Address & PageMask);

	pPageFile = fopen(FileName, "rb");

	EMM_MapType& Entry = Mapping[Address >> 16];

	fread(&MainMemory.Byte[Entry.Page | (Address & MemoryMask)],
		PageSize, 1, pPageFile);

	fclose(pPageFile);

	Entry.Modified = 0;
}

void EmuMemSave(EMU_U32 Address)
{
	FILE* pPageFile = NULL;
	char FileName[256];

	sprintf(FileName, "fPage%u.tmp", Address & PageMask);

	pPageFile = fopen(FileName, "wb");

	EMM_MapType& Entry = Mapping[Address >> 16];

	fwrite(&MainMemory.Byte[Entry.Page | (Address & MemoryMask)],
		PageSize, 1, pPageFile);

	fclose(pPageFile);

	Entry.Modified = 0;
}

void EmuMemAddWriteCallBack(EMU_U32 StartAddress,
	EMU_U32 EndAddress,
	EMM_WRITECALLBACK CallBack)
{
	stTLB_WriteCallBack CallBack_temp;

	CallBack_temp.StartAddress = StartAddress;
	CallBack_temp.EndAddress = EndAddress;
	CallBack_temp.CallBack = CallBack;

	WriteCallBacks.push_back(CallBack_temp);
}

void EmuMemAddReadCallBack(EMU_U32 StartAddress,
	EMU_U32 EndAddress,
	EMM_READCALLBACK CallBack)
{
	stTLB_ReadCallBack CallBack_temp;

	CallBack_temp.StartAddress = StartAddress;
	CallBack_temp.EndAddress = EndAddress;
	CallBack_temp.CallBack = CallBack;

	ReadCallBacks.push_back(CallBack_temp);
}

inline void EmuMemCallCallBackWrite(EMU_U32 Address)
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

inline void EmuMemCallCallBackRead(EMU_U32 Address, EMU_U08* RealAddress)
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

EMU_U64 EmuMemGetTime()
{
	register EMU_U64 Ticks;
	RDTSC(Ticks);
	return Ticks;
}

bool EmuMemAddFromFile(const char* FileName,
	EMU_U32 Offset,
	EMU_U32 FileSize,
	EMU_U32 EmuAddress,
	EMU_U08 Permissions)
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
		EmuMemReplace(EmuAddress);
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
				EmuMemReplace(EmuAddress);
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

EMU_U08* EmuMemReadContinuosArea(EMU_U32 Address, EMU_U32 Size)
{
	if ((Address >= 0x80000000) && (Address < 0xA0000000))
	{
		Address -= 0x80000000;
	}
	else
		if ((Address >= 0xA0000000) && (Address < 0xC0000000))
		{
			Address -= 0xA0000000;
		}
	if ((Address >= EMU_TIMER_START_ADDR) &&
		(Address <= EMU_GS_PRIV_END_ADDR))
	{
		if (Address < EMU_IPU_START_ADDR)
		{
			return Emu_Timer_GetPointer(Address);
		}
		else
			if (Address < EMU_GIF_START_ADDR)
			{
				return Emu_Ipu_GetPointer(Address);
			}
			else
				if (Address < EMU_VIF_START_ADDR)
				{
					return Emu_Gif_GetPointer(Address);
				}
				else
					if (Address < EMU_FIFO_START_ADDR)
					{
						return Emu_Vif_GetPointer(Address);
					}
					else
						if (Address < EMU_DMA_START_ADDR)
						{
							return Emu_Fifo_GetPointer(Address);
						}
						else
							if (Address < EMU_INTC_START_ADDR)
							{
								return Emu_Dma_GetPointer(Address);
							}
							else
								if (Address < EMU_SIF_START_ADDR)
								{
									return Emu_Intc_GetPointer(Address);
								}
								else
									if (Address < EMU_DMA_ENAB_START_ADDR)
									{
										return Emu_Sif_GetPointer(Address);
									}
									else
										if (Address < EMU_VU_START_ADDR)
										{
											return Emu_Dma_GetPointer(Address);
										}
										else
											if (Address < EMU_GS_PRIV_START_ADDR)
											{
												return Emu_Vu_GetPointer(Address);
											}
											else
											{
												return Emu_GS_GetPointer(Address);
											}
	}
	else
		if ((Address >= EMU_BIOS_START_ADDR) && (Address <= EMU_BIOS_END_ADDR))
		{
			return Emu_Bios_GetPointer(Address);
		}
		else
		{
			EMU_U32 StartPage = Address >> 16;
			EMU_U32 EndPage = (Address + Size) >> 16;

			EMM_MapType Entry = Mapping[StartPage];
			if (!Entry.InMemory)
			{
				EmuMemReplace(Address);
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
					memcpy(&RaftMemory[BufferPos],
						&MainMemory.Byte[Entry.Page | (Address & MemoryMask)],
						EndAddr - Address);
					BufferPos += EndAddr - Address;
					Address = EndAddr;
					Entry = Mapping[Address >> 16];
					if (!Entry.InMemory)
					{
						EmuMemReplace(Address);
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

/*
** CEmuData
*/
/*
CEmuData::CEmuData( void )
{
	Data.Byte = NULL;
}

CEmuData::CEmuData( EMU_U08 * pData )
{
	Data.Byte = pData;
}

CEmuData::CEmuData( EMU_U16 * pData )
{
	Data.Short = pData;
}

CEmuData::CEmuData( EMU_U32 * pData )
{
	Data.Word = pData;
}

void CEmuData::operator=( EMU_U08 * pData )
{
	Data.Byte = pData;
}

void CEmuData::operator=( EMU_U16 * pData )
{
	Data.Short = pData;
}

void CEmuData::operator=( EMU_U32 * pData )
{
	Data.Word = pData;
}

CEmuData::operator unsigned char( )
{
	return *Data.Byte;
}

CEmuData::operator unsigned short( )
{
	return *Data.Short;
}

CEmuData::operator unsigned int( )
{
	return *Data.Word;
}

*/


