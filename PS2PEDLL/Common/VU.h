#pragma once

#include "EmuPS2Core.h"

namespace Common
{
	struct EMU_VF
	{
		EMU_UF32 X;
		EMU_UF32 Y;
		EMU_UF32 Z;
		EMU_UF32 W;
	};

	union EMU_VI
	{
		EMU_U32 u32;
		EMU_I32 i32;
		struct
		{
			EMU_U16 u16_00_15;
			EMU_U16 u16_16_31;
		};
		struct
		{
			EMU_I16 i16_00_15;
			EMU_I16 i16_16_31;
		};
	};

	struct EMU_VU_Regs
	{
		EMU_VF CPR[32];

		EMU_VF Acc;

		union
		{
			EMU_VI CCR[32];
			struct
			{
				EMU_VI RegI[16];                  // 00 - 15
				union
				{
					EMU_U32 Status;                 // 16
					struct
					{
						EMU_U32     Status_Z : 1;
						EMU_U32     Status_S : 1;
						EMU_U32     Status_U : 1;
						EMU_U32     Status_O : 1;
						EMU_U32     Status_I : 1;
						EMU_U32     Status_D : 1;
						EMU_U32     Status_ZS : 1;
						EMU_U32     Status_SS : 1;
						EMU_U32     Status_US : 1;
						EMU_U32     Status_OS : 1;
						EMU_U32     Status_IS : 1;
						EMU_U32     Status_DS : 1;
						EMU_U32     Status_12_31 : 20;
					};
				};
				union
				{
					EMU_U32 MAC;                    // 17
					struct
					{
						EMU_U32     MAC_ZW : 1;
						EMU_U32     MAC_ZZ : 1;
						EMU_U32     MAC_ZY : 1;
						EMU_U32     MAC_ZX : 1;
						EMU_U32     MAC_SW : 1;
						EMU_U32     MAC_SZ : 1;
						EMU_U32     MAC_SY : 1;
						EMU_U32     MAC_SX : 1;
						EMU_U32     MAC_UW : 1;
						EMU_U32     MAC_UZ : 1;
						EMU_U32     MAC_UY : 1;
						EMU_U32     MAC_UX : 1;
						EMU_U32     MAC_OW : 1;
						EMU_U32     MAC_OZ : 1;
						EMU_U32     MAC_OY : 1;
						EMU_U32     MAC_OX : 1;
						EMU_U32     MAC_16_31 : 16;
					};
				};
				union
				{
					EMU_U32 Clipping;               // 18
					struct
					{
						EMU_U32     Clipping_1HX : 1;
						EMU_U32     Clipping_1LX : 1;
						EMU_U32     Clipping_1HY : 1;
						EMU_U32     Clipping_1LY : 1;
						EMU_U32     Clipping_1HZ : 1;
						EMU_U32     Clipping_1LZ : 1;
						EMU_U32     Clipping_2HX : 1;
						EMU_U32     Clipping_2LX : 1;
						EMU_U32     Clipping_2HY : 1;
						EMU_U32     Clipping_2LY : 1;
						EMU_U32     Clipping_2HZ : 1;
						EMU_U32     Clipping_2LZ : 1;
						EMU_U32     Clipping_3HX : 1;
						EMU_U32     Clipping_3LX : 1;
						EMU_U32     Clipping_3HY : 1;
						EMU_U32     Clipping_3LY : 1;
						EMU_U32     Clipping_3HZ : 1;
						EMU_U32     Clipping_3LZ : 1;
						EMU_U32     Clipping_4HX : 1;
						EMU_U32     Clipping_4LX : 1;
						EMU_U32     Clipping_4HY : 1;
						EMU_U32     Clipping_4LY : 1;
						EMU_U32     Clipping_4HZ : 1;
						EMU_U32     Clipping_4LZ : 1;
						EMU_U32     MAC_24_31 : 8;
					};
				};
				EMU_U32 Reserved1;                  // 19

				EMU_UF32 R;                         // 20
				EMU_UF32 I;                         // 21
				EMU_UF32 Q;                         // 22

				EMU_U32 Reserved2;                  // 23
				EMU_U32 Reserved3;                  // 24
				EMU_U32 Reserved4;                  // 25

				EMU_F32 TPC;                        // 26
				EMU_F32 CMSAR0;                     // 27

				union
				{
					EMU_U32 FBRST;                  // 28
					struct
					{
						EMU_U32     FBRST_FB0 : 1;
						EMU_U32     FBRST_RS0 : 1;
						EMU_U32     FBRST_DE0 : 1;
						EMU_U32     FBRST_TE0 : 1;
						EMU_U32     FBRST_04_07 : 4;
						EMU_U32     FBRST_FB1 : 1;
						EMU_U32     FBRST_RS1 : 1;
						EMU_U32     FBRST_DE1 : 1;
						EMU_U32     FBRST_TE1 : 1;
						EMU_U32     FBRST_12_31 : 20;
					};
				};
				union
				{
					EMU_U32 VPU_STAT;               // 29
					struct
					{
						EMU_U32     VPU_STAT_VBS0 : 1;
						EMU_U32     VPU_STAT_VDS0 : 1;
						EMU_U32     VPU_STAT_VTS0 : 1;
						EMU_U32     VPU_STAT_VFS0 : 1;
						EMU_U32     VPU_STAT_04 : 1;
						EMU_U32     VPU_STAT_DIV0 : 1;
						EMU_U32     VPU_STAT_06 : 1;
						EMU_U32     VPU_STAT_IBS0 : 1;
						EMU_U32     VPU_STAT_VBS1 : 1;
						EMU_U32     VPU_STAT_VDS1 : 1;
						EMU_U32     VPU_STAT_VTS1 : 1;
						EMU_U32     VPU_STAT_VFS1 : 1;
						EMU_U32     VPU_STAT_VGW1 : 1;
						EMU_U32     VPU_STAT_DIV1 : 1;
						EMU_U32     VPU_STAT_EFU1 : 1;
						EMU_U32     FBRST_15_31 : 17;
					};
				};
				EMU_U32 Reserved5;                  // 30

				EMU_F32 CMSAR1;                     // 31
			};
		};
	};

	class Vu
	{
	public:
		static const EMU_U32 EMU_VU_START_ADDR = 0x11000000;
		static const EMU_U32 EMU_VU_END_ADDR = 0x11FFFFFF;

		static const EMU_U32 EMU_VU_MICROMEM0 = 0x11000000;
		static const EMU_U32 EMU_VU_MICROMEM1 = 0x11008000;

		static const EMU_U32 EMU_VU_MEM0 = 0x11004000;
		static const EMU_U32 EMU_VU_MEM1 = 0x1100C000;

		// Initialized VU memory pointers and registers
		void Init(void);
		// Resets VU registers
		void Reset(void);

		EMU_U08* GetPointer(EMU_U32 Address);

		static Vu& GetInstance();

		Vu(const Vu&) = delete;
		Vu(Vu&&) = delete;
		Vu& operator=(const Vu&) = delete;
		Vu& operator=(Vu&&) = delete;

	private:
		Vu();

		EMU_U08 EmuVuMemory[EMU_VU_END_ADDR - EMU_VU_START_ADDR];

		// Pointer to control1 registers
		EMU_VU_Regs VU0Regs;
		EMU_VU_Regs VU1Regs;
	};
}