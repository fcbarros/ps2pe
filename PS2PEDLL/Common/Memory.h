#pragma once

#include "EmuPS2Core.h"

#include <vector>

namespace Common
{
	typedef void (*EMM_WRITECALLBACK)(EMU_U32);
	typedef void (*EMM_READCALLBACK)(EMU_U32, EMU_U08*);

	// Struct for mult-type handling
	union EMM_Data_Type
	{
		EMU_U08* Byte;
		EMU_U16* Short;
		EMU_U32* Word;
		EMU_U64* DWord;
	};

	struct stTLB_Entry
	{
		EMU_U32 VirtualPage;
		EMU_U32 PhysicalPage;
		EMU_U64 LastAcessed;
	};

	struct stTLB_WriteCallBack
	{
		EMU_U32 StartAddress;
		EMU_U32 EndAddress;
		EMM_WRITECALLBACK CallBack;
	};

	struct stTLB_ReadCallBack
	{
		EMU_U32 StartAddress;
		EMU_U32 EndAddress;
		EMM_READCALLBACK CallBack;
	};

	struct EMM_MapType
	{
		EMU_U08 InMemory;
		EMU_U08 Saved;
		EMU_U08 Modified;
		EMU_U08 MustExist;
		EMU_U32 Page;
		EMU_U64 LastAcessed;
	};

	typedef std::vector<stTLB_WriteCallBack> EMM_WriteCallBackType;
	typedef std::vector<stTLB_ReadCallBack>	EMM_ReadCallBackType;

	class Memory
	{
	public:
		void Init(EMU_U32 gMemorySize, EMU_U32 gPageSize);
		void Shutdown();
		void Clear();
		EMU_U08* GetRealPointer(EMU_U32 Address);

		void SetByte(EMU_U32 Address, EMU_U08 Data);
		void SetShort(EMU_U32 Address, EMU_U16 Data);
		void SetWord(EMU_U32 Address, EMU_U32 Data);
		void SetDWord(EMU_U32 Address, EMU_U64 Data);
		EMU_U08 GetByte(EMU_U32 Address);
		EMU_U16 GetShort(EMU_U32 Address);
		EMU_U32 GetWord(EMU_U32 Address);
		EMU_U64 GetDWord(EMU_U32 Address);

		void AddWriteCallBack(EMU_U32 StartAddress, EMU_U32 EndAddress, EMM_WRITECALLBACK CallBack);
		void AddReadCallBack(EMU_U32 StartAddress, EMU_U32 EndAddress, EMM_READCALLBACK CallBack);
		void CallCallBackWrite(EMU_U32 Address);
		void CallCallBackRead(EMU_U32 Address, EMU_U08* RealAddress);
		EMU_U64 GetTime();
		bool AddFromFile(const char* FileName, EMU_U32 Offset, EMU_U32 FileSize, EMU_U32 EmuAddress, EMU_U08 Permissions);
		EMU_U08* ReadContinuosArea(EMU_U32 Address, EMU_U32 Size);

		EMU_U08* GetScratchPad();

		static Memory& GetInstance();

		Memory(const Memory&) = delete;
		Memory& operator=(const Memory&) = delete;
		Memory(Memory&) = delete;
		Memory& operator=(Memory&&) = delete;

	private:
		Memory();

		EMU_U32 MemorySize;
		EMU_U32 MemoryMask;
		EMU_U32 PageSize;
		EMU_U32 PageMask;
		EMU_U32 NumberOfFrames;
		EMU_U32 NumberOfPages;
		EMU_U32 CurrentNumberOfPages;

		EMM_Data_Type MainMemory;
		stTLB_Entry* Table;

		EMM_MapType* Mapping;
		EMM_WriteCallBackType WriteCallBacks;
		EMM_ReadCallBackType ReadCallBacks;

		EMU_U08* RaftMemory;

		EMU_U08 EMemScratchPad[16 * 1024];

		void Replace(EMU_U32 Address);
		void Load(EMU_U32 Address);
		void Save(EMU_U32 Address);
	};
}