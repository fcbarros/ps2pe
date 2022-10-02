#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	typedef struct
	{
		union
		{
			EMU_U32 STAT;
			struct
			{
				EMU_U32 STAT_GS : 1;     // 0
				EMU_U32 STAT_SBUS : 1;   // 1
				EMU_U32 STAT_VBON : 1;   // 2
				EMU_U32 STAT_VBOF : 1;   // 3
				EMU_U32 STAT_VIF0 : 1;   // 4
				EMU_U32 STAT_VIF1 : 1;   // 5
				EMU_U32 STAT_VU0 : 1;    // 6
				EMU_U32 STAT_VU1 : 1;    // 7
				EMU_U32 STAT_IPU : 1;    // 8
				EMU_U32 STAT_TIM0 : 1;   // 9
				EMU_U32 STAT_TIM1 : 1;   // 10
				EMU_U32 STAT_TIM2 : 1;   // 11
				EMU_U32 STAT_TIM3 : 1;   // 12
				EMU_U32 STAT_SFIFO : 1;  // 13
				EMU_U32 STAT_VU0WD : 1;  // 14
			};
		};
		EMU_U08 null0[12];
		union
		{
			EMU_U32 MASK;
			struct
			{
				EMU_U32 MASK_GS : 1;     // 0
				EMU_U32 MASK_SBUS : 1;   // 1
				EMU_U32 MASK_VBON : 1;   // 2
				EMU_U32 MASK_VBOF : 1;   // 3
				EMU_U32 MASK_VIF0 : 1;   // 4
				EMU_U32 MASK_VIF1 : 1;   // 5
				EMU_U32 MASK_VU0 : 1;    // 6
				EMU_U32 MASK_VU1 : 1;    // 7
				EMU_U32 MASK_IPU : 1;    // 8
				EMU_U32 MASK_TIM0 : 1;   // 9
				EMU_U32 MASK_TIM1 : 1;   // 10
				EMU_U32 MASK_TIM2 : 1;   // 11
				EMU_U32 MASK_TIM3 : 1;   // 12
				EMU_U32 MASK_SFIFO : 1;  // 13
				EMU_U32 MASK_VU0WD : 1;  // 14
			};
		};
		EMU_U08 null1[12];
	} stEmu_Intc_Control_Regs;

	typedef struct
	{
		int active;
		int id;
		unsigned long func;
	} stEMU_INTC_HANDLER;

	void Emu_Intc_BiosAddHandler();
	void Emu_Intc_BiosRemoveHandler();
	void Emu_Intc_BiosEnableHandler();
	void Emu_Intc_BiosDisableHandler();

	class Intc final
	{
	public:
		// Base Address
		static const EMU_U32 EMU_INTC_START_ADDR = 0x1000F000;
		static const EMU_U32 EMU_INTC_END_ADDR = 0x1000F22F;

		// Control Register Address
		static const EMU_U32 EMU_INTC_STAT = 0x00000000;
		static const EMU_U32 EMU_INTC_MASK = 0x00000010;

		// Initializes Interrupt memory pointers and registers
		void Init(void);
		// Resets Interrupt registers
		void Reset(void);
		// Callback for Interrupt access
		static void Callback(EMU_U32 Address);

		void AddHandler(EMU_U32 HandlerInder, EMU_U32 FuncAddress);
		void RemoveHandler(EMU_U32 HandlerIndex);
		void EnableHandler(EMU_U32 HandlerIndex);
		void DisableHandler(EMU_U32 HandlerIndex);

		void CallHandler(EMU_U32 HandlerIndex);
		void ExitHandler(void);

		EMU_U08* GetPointer(EMU_U32 Address);

		stEmu_Intc_Control_Regs* GetControlReg();

		inline void ClearInterruptIndex() { EmuInterruptIndex = 0; }
		inline EMU_U08 GetInterruptIndex() { return EmuInterruptIndex; }
		inline void IncInterruptIndex() { EmuInterruptIndex++; }
		inline void DecInterruptIndex() { EmuInterruptIndex--; }

		static Intc& GetInstance();

		Intc(const Intc&) = delete;
		Intc& operator=(const Intc&) = delete;
		Intc(Intc&&) = delete;
		Intc& operator=(Intc&&) = delete;

	private:
		Intc();

		EMU_U08 EmuIntcMemory[EMU_INTC_END_ADDR - EMU_INTC_START_ADDR];

		EMU_U08 EmuInterruptIndex = 0;

		// Pointer to control1 registers
		stEmu_Intc_Control_Regs* Emu_Intc_Control_Reg;

		stEMU_INTC_HANDLER Emu_Intc_Handler[32];
	};
}