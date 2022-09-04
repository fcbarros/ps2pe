#include <string.h>

#include "EmuDisassembly.h"

DLLEXPORT BOOL CALLBACK EmuFindLabel(char* Label, EMU_U32* LabelIndex)
{
	if (ElfFile->Symbol_num > 0)
	{
		for (EMU_U32 i = 0; i < ElfFile->Symbol_num; i++)
		{
			if (!strncmp(&ElfFile->SymbolSTable[ElfFile->Symbol[i].st_name], Label, strlen(Label)))
			{
				*LabelIndex = ElfFile->Symbol[i].st_value;
				return TRUE;
			}
		}
	}
	else if (ElfFile->Header.e_shnum > 0)
	{
		for (int i = 0; i < ElfFile->Header.e_shnum; i++)
		{
			if (!strncmp(&ElfFile->SessionSTable[ElfFile->Session[i].sh_name], Label, strlen(Label)))
			{
				*LabelIndex = ElfFile->Session[i].sh_addr;
				return TRUE;
			}
		}
	}

	return FALSE;
}

DLLEXPORT void CALLBACK EmuGetDisassemblySymbol(EMU_U32 Address, char* Buffer, EMU_U32 BufferSize)
{
	strcpy_s(Buffer, BufferSize, EmuDisassemblySymbol(Address));
}

const char* EmuDisassemblySymbol(EMU_U32 Address)
{
	char Symbol_str[256];
	memset(Symbol_str, 0, sizeof(Symbol_str));
	if ((ElfFile->Symbol_num == 0) &&
		(ElfFile->Header.e_shnum == 0) &&
		(Address == ElfFile->Header.e_entry))
	{
		strcpy_s(Symbol_str, "ENTRY_POINT");
	}
	else  if (ElfFile->Symbol_num > 0)
	{
		for (EMU_U32 i = 0; i < ElfFile->Symbol_num; i++)
		{
			if (ElfFile->Symbol[i].st_value == Address)
			{
				if (ElfFile->Symbol[i].st_name > 0)
				{
					strcpy_s(Symbol_str, &ElfFile->SymbolSTable[ElfFile->Symbol[i].st_name]);
					strcat_s(Symbol_str, ":");
				}
				else
				{
					strcpy_s(Symbol_str, &ElfFile->SessionSTable[ElfFile->Session[ElfFile->Symbol[i].st_shndx].sh_name]);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ElfFile->Header.e_shnum; i++)
		{
			if (ElfFile->Session[i].sh_addr == Address)
			{
				if (ElfFile->Session[i].sh_name > 0)
				{
					strcpy_s(Symbol_str, &ElfFile->SessionSTable[ElfFile->Session[i].sh_name]);
				}
				break;
			}
		}
	}

	return Symbol_str;
}
//0x4b04043c
// 010010 1 1000 00100 00000 10000 111100
// COP2     W    

// Returns the disassembly instruction
DLLEXPORT void CALLBACK EmuDisassembly(EMU_U32 Address, char* Buffer, EMU_U32 BufferSize)
{
	char stDisassembly[256];

	EMU_U32 tInstruction = EmuMemGetWord(Address);
	EMU_U32 Index = EmuInstructionIndex(tInstruction);

	strcpy_s(stDisassembly, EmuInstructions[Index].Name);
	if (EmuInstructions[Index].Disassembly)
	{
		strcat_s(stDisassembly, (*EmuInstructions[Index].Disassembly)(Address, tInstruction));
	}

	strcpy_s(Buffer, BufferSize, stDisassembly);
}

const char* EmuDis_GetCOP0RegName(EMU_U32 Reg)
{
	static const char* EmuCOP0RegName[32] = {
		"Index", "Random", "EntryLo0", "EntryLo1",
		"Context", "PageMask", "Wired", "Reserved0",
		"BadVAddr", "Count", "EntryHi", "Compare",
		"Status", "Cause", "EPC", "PRId",
		"Config", "Reserved1", "Reserved2", "Reserved3",
		"Reserved4", "Reserved5", "Reserved6", "Reserved7",
		"Debug", "Perf", "Reserved8", "Reserved9",
		"TagLo", "TagHi", "Error_EPC", "Reserved10" };

	return EmuCOP0RegName[Reg];
}

const char* EmuDis_GetRegName(EMU_U32 Reg)
{
	static const char* EmuRegName[36] = {
		"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
		"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
		"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
		"t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
		"hi", "lo", "sa", "pc" };

	return EmuRegName[Reg];
}

const char* EmuDis_GetCOP1RegName(EMU_U32 Reg)
{
	static const char* EmuRegName[35] = {
		"fpr0", "fpr1", "fpr2", "fpr3", "fpr4", "fpr5", "fpr6", "fpr7",
		"fpr8", "fpr9", "fpr10", "fpr11", "fpr12", "fpr13", "fpr14", "fpr15",
		"fpr16", "fpr17", "fpr18", "fpr19", "fpr20", "fpr21", "fpr22", "fpr23",
		"fpr24", "fpr25", "fpr26", "fpr27", "fpr28", "fpr29", "fpr30", "fpr31",
		"fcr0", "fcr31", "accumulator" };

	return EmuRegName[Reg];
}

const char* EmuDis_GetCOP2FPRegName(EMU_U32 Reg)
{
	static const char* EmuRegName[33] = {
		"vf0", "vf1", "vf2", "vf3", "vf4", "vf5", "vf6", "vf7",
		"vf8", "vf9", "vf10", "vf11", "vf12", "vf13", "vf14", "vf15",
		"vf16", "vf17", "vf18", "vf19", "vf20", "vf21", "vf22", "vf23",
		"vf24", "vf25", "vf26", "vf27", "vf28", "vf29", "vf30", "vf31",
		"acc" };

	return EmuRegName[Reg];
}

const char* EmuDis_GetCOP2IPRegName(EMU_U32 Reg)
{
	static const char* EmuRegName[32] = {
		"vi0", "vi1", "vi2", "vi3", "vi4", "vi5", "vi6", "vi7",
		"vi8", "vi9", "vi10", "vi11", "vi12", "vi13", "vi14", "vi15",
		"status", "mac", "clipping", "reserved1", "r", "i", "q", "reserved2",
		"reserved3", "reserved4", "tpc", "cmsar0", "fbrst", "vpu_stat", "reserved5", "cmsar1" };

	return EmuRegName[Reg];
}

const char* EmuDis_Nothing(EMU_U32, EMU_U32 Code)
{
	return "";
}

const char* EmuDis_RS_RT_I16(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 I16 = Code & 0x0000FFFF;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s, $%x",
		EmuDis_GetRegName(RT), EmuDis_GetRegName(RS), I16);

	return Local_dis_str;
}

const char* EmuDis_I26(EMU_U32 Address, EMU_U32 Code)
{
	EMU_U32 I26 = (Code & 0x03FFFFFF) << 2;
	I26 |= (Address + 4) & 0xC0000000;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " $%x", I26);

	const char* Label = EmuDisassemblySymbol(I26);
	if (strlen(Label) > 0)
	{
		strcat_s(Local_dis_str, "     ( ");
		strcat_s(Local_dis_str, Label);
		strcat_s(Local_dis_str, " )");
	}

	return Local_dis_str;
}

const char* EmuDis_0_RT_I16(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 I16 = Code & 0x0000FFFF;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, $%x", EmuDis_GetRegName(RT), I16);

	return Local_dis_str;
}

const char* EmuDis_BASE_RT_OFF16(EMU_U32, EMU_U32 Code)
{
	EMU_U32 BASE = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 OFF16 = Code & 0x0000FFFF;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, $%x(%s)",
		EmuDis_GetRegName(RT), OFF16, EmuDis_GetRegName(BASE));

	return Local_dis_str;
}

const char* EmuDis_SYSCALL(EMU_U32, EMU_U32 Code)
{
	EMU_U32 I20 = (Code >> 6) & 0x000FFFFF;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " ($%x)", I20);

	return Local_dis_str;
}

const char* EmuDis_0_RT_RD_SA(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;
	EMU_U32 SA = (Code >> 6) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s, %d",
		EmuDis_GetRegName(RD), EmuDis_GetRegName(RT), SA);

	return Local_dis_str;
}

const char* EmuDis_REGISTERED(EMU_U32, EMU_U32 Code)
{
	EMU_U32 Index = EmuInstructionIndex(Code);
	EMU_U32 I32 = ~EmuInstructions[Index].Mask & Code;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " ($%.8x:$%.8x)",
		EmuInstructions[Index].Code,
		I32);

	return Local_dis_str;
}

const char* EmuDis_RS_RT_BRANCH16(EMU_U32 Address, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 OFF32 = (Code & 0x0000FFFF) << 2;
	// Signed extended
	OFF32 = (OFF32 & 0x00020000) ? (OFF32 | 0xFFFC0000) : OFF32;
	OFF32 += Address + 4;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s, $%x",
		EmuDis_GetRegName(RS), EmuDis_GetRegName(RT),
		OFF32);

	const char* Label = EmuDisassemblySymbol(OFF32);
	if (strlen(Label) > 0)
	{
		strcat_s(Local_dis_str, "     ( ");
		strcat_s(Local_dis_str, Label);
		strcat_s(Local_dis_str, " )");
	}

	return Local_dis_str;
}

const char* EmuDis_RS_0_BRANCH16(EMU_U32 Address, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 OFF32 = (Code & 0x0000FFFF) << 2;
	// Signed extended
	OFF32 = (OFF32 & 0x00020000) ? (OFF32 | 0xFFFC0000) : OFF32;
	OFF32 += Address + 4;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, $%x",
		EmuDis_GetRegName(RS),
		OFF32);

	const char* Label = EmuDisassemblySymbol(OFF32);
	if (strlen(Label) > 0)
	{
		strcat_s(Local_dis_str, "     ( ");
		strcat_s(Local_dis_str, Label);
		strcat_s(Local_dis_str, " )");
	}

	return Local_dis_str;
}

const char* EmuDis_PREF(EMU_U32, EMU_U32 Code)
{
	EMU_U32 BASE = (Code >> 21) & 0x0000001F;
	EMU_U32 HINT = (Code >> 16) & 0x0000001F;
	EMU_U32 I16 = Code & 0x0000FFFF;

	char Hint_str[32];

	switch (HINT)
	{
	case 0:     strcpy_s(Hint_str, "load");                 break;
	case 1:     strcpy_s(Hint_str, "store");                break;
	case 4:     strcpy_s(Hint_str, "load_streamed");        break;
	case 5:     strcpy_s(Hint_str, "store_streamed");       break;
	case 6:     strcpy_s(Hint_str, "load_retained");        break;
	case 7:     strcpy_s(Hint_str, "store_retained");       break;
	case 25:    strcpy_s(Hint_str, "writeback_invalidate"); break;
	default:    strcpy_s(Hint_str, "nop");                  break;
	}

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, $%x(%s)",
		Hint_str,
		I16,
		EmuDis_GetRegName(BASE));

	return Local_dis_str;
}

const char* EmuDis_JR(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s",
		EmuDis_GetRegName(RS));

	return Local_dis_str;
}

const char* EmuDis_JALR(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s",
		EmuDis_GetRegName(RD), EmuDis_GetRegName(RS));

	return Local_dis_str;
}

const char* EmuDis_RS_RT_RD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s, %s",
		EmuDis_GetRegName(RD),
		EmuDis_GetRegName(RS),
		EmuDis_GetRegName(RT));

	return Local_dis_str;
}

const char* EmuDis_RT_FS(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 FS = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, FPR[ %d ]",
		EmuDis_GetRegName(RT),
		FS);

	return Local_dis_str;
}

const char* EmuDis_FS_FD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 FS = (Code >> 11) & 0x0000001F;
	EMU_U32 FD = (Code >> 06) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " FPR[ %d ], FPR[ %d ]",
		FD,
		FS);

	return Local_dis_str;
}

const char* EmuDis_FT_FS_FD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 FS = (Code >> 11) & 0x0000001F;
	EMU_U32 FD = (Code >> 6) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " FPR[ %d ], FPR[ %d ], FPR[ %d ]",
		FD,
		FS,
		FT);

	return Local_dis_str;
}

const char* EmuDis_BASE_FT_OFF16(EMU_U32, EMU_U32 Code)
{
	EMU_U32 BASE = (Code >> 21) & 0x0000001F;
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 OFF16 = Code & 0x0000FFFF;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " FPR[ %d ], $%x(%s)",
		FT,
		OFF16,
		EmuDis_GetRegName(BASE));

	return Local_dis_str;
}

const char* EmuDis_RS_RT(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s",
		EmuDis_GetRegName(RS),
		EmuDis_GetRegName(RT));

	return Local_dis_str;
}

const char* EmuDis_RD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RD = (Code >> 16) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s",
		EmuDis_GetRegName(RD));

	return Local_dis_str;
}

const char* EmuDis_RS(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s",
		EmuDis_GetRegName(RS));

	return Local_dis_str;
}

const char* EmuDis_SHIFT_V(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s, %s",
		EmuDis_GetRegName(RD),
		EmuDis_GetRegName(RT),
		EmuDis_GetRegName(RS));

	return Local_dis_str;
}

const char* EmuDis_RS_I16(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_I64 I16 = (short)(Code & 0x0000FFFF);

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %I64X",
		EmuDis_GetRegName(RS),
		I16);

	return Local_dis_str;
}

const char* EmuDis_RD11(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s",
		EmuDis_GetRegName(RD));

	return Local_dis_str;
}

const char* EmuDis_0_RT_RD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s",
		EmuDis_GetRegName(RD), EmuDis_GetRegName(RT));

	return Local_dis_str;
}

const char* EmuDis_FT_FS(EMU_U32, EMU_U32 Code)
{
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 FS = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " FPR[ %d ], FPR[ %d ]",
		FS,
		FT);

	return Local_dis_str;
}

const char* EmuDis_FT_0_FD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 FD = (Code >> 06) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " FPR[ %d ], FPR[ %d ]",
		FD,
		FT);

	return Local_dis_str;
}

const char* EmuDis_BRANCH16(EMU_U32 Address, EMU_U32 Code)
{
	EMU_U32 OFF32 = (short)(Code & 0x0000FFFF);
	// Signed extended
	OFF32 <<= 2;
	OFF32 += Address + 4;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " $%x",
		OFF32);

	const char* Label = EmuDisassemblySymbol(OFF32);
	if (strlen(Label) > 0)
	{
		strcat_s(Local_dis_str, "     ( ");
		strcat_s(Local_dis_str, Label);
		strcat_s(Local_dis_str, " )");
	}

	return Local_dis_str;
}

const char* EmuDis_SYNC_STYPE(EMU_U32, EMU_U32 Code)
{
	EMU_U32 SType = (Code >> 06) & 0x00000010;

	char Local_dis_str[256];
	if (SType)
	{
		strcpy_s(Local_dis_str, ".p");
	}
	else
	{
		strcpy_s(Local_dis_str, ".l");
	}

	return Local_dis_str;
}

const char* EmuDis_RS_0_RD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RS = (Code >> 21) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s",
		EmuDis_GetRegName(RD),
		EmuDis_GetRegName(RS));

	return Local_dis_str;
}

const char* EmuDis_MC0(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 RD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " %s, %s",
		EmuDis_GetRegName(RT), EmuDis_GetCOP0RegName(RD));

	return Local_dis_str;
}

const char* EmuDis_VU_RT_FD_INTERLOCK(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 FD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, "%s %s, %s",
		(Code & 0x01 ? ".I" : ".NI"),
		EmuDis_GetRegName(RT), EmuDis_GetCOP2FPRegName(FD));

	return Local_dis_str;
}

const char* EmuDis_VU_RT_ID_INTERLOCK(EMU_U32, EMU_U32 Code)
{
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 FD = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, "%s %s, %s",
		(Code & 0x01 ? ".I" : ".NI"),
		EmuDis_GetRegName(RT), EmuDis_GetCOP2IPRegName(FD));

	return Local_dis_str;
}

const char* EmuDis_VU_DEST_FT_FS_FD_BC(EMU_U32, EMU_U32 Code)
{
	/*    EMU_U32 DEST = ( Code >> 21 ) & 0x0000000F;
		EMU_U32 FT = ( Code >> 16 ) & 0x0000001F;
		EMU_U32 FS = ( Code >> 11 ) & 0x0000001F;
		EMU_U32 FD = ( Code >> 06 ) & 0x0000001F;
		EMU_U32 BC = Code & 0x00000003;

		sprintf_s( Local_dis_str, "%s %s, VI[%u]",
					( Code & 0x01 ? ".I" : ".NI" ),
				EmuDis_GetRegName( RT ), FD );

		return Local_dis_str;
	*/
	return "";
}

const char* EmuDis_VU_DEST_FT_FS(EMU_U32, EMU_U32 Code)
{
	EMU_U32 DEST = (Code >> 21) & 0x0000000F;
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 FS = (Code >> 11) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, ".%s%s%s%s vf[%d], vf[%d]",
		DEST & 0x8 ? "x" : "", DEST & 0x4 ? "y" : "",
		DEST & 0x2 ? "z" : "", DEST & 0x1 ? "w" : "",
		FT, FS);

	return Local_dis_str;
}

const char* EmuDis_BASE_VFT_OFF16(EMU_U32, EMU_U32 Code)
{
	EMU_U32 BASE = (Code >> 21) & 0x0000001F;
	EMU_U32 RT = (Code >> 16) & 0x0000001F;
	EMU_U32 OFF16 = Code & 0x0000FFFF;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, " vf[%d], $%x(%s)",
		RT, OFF16, EmuDis_GetRegName(BASE));

	return Local_dis_str;
}

const char* EmuDis_VU_DEST_FT_FS_FD(EMU_U32, EMU_U32 Code)
{
	EMU_U32 DEST = (Code >> 21) & 0x0000000F;
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 FS = (Code >> 11) & 0x0000001F;
	EMU_U32 FD = (Code >> 6) & 0x0000001F;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, ".%s%s%s%s vf[%d], vf[%d], vf[%d]",
		DEST & 0x8 ? "x" : "", DEST & 0x4 ? "y" : "",
		DEST & 0x2 ? "z" : "", DEST & 0x1 ? "w" : "",
		FD, FS, FT);

	return Local_dis_str;
}

const char* EmuDis_VU_DEST_FT_FS_BC(EMU_U32, EMU_U32 Code)
{
	EMU_U32 DEST = (Code >> 21) & 0x0000000F;
	EMU_U32 FT = (Code >> 16) & 0x0000001F;
	EMU_U32 FS = (Code >> 11) & 0x0000001F;
	EMU_U32 BC = (Code) & 0x00000003;

	char Local_dis_str[256];
	sprintf_s(Local_dis_str, ".%s%s%s%s ACC, vf[%d], vf[%d]",
		//       BC == 0 ? "x" : BC == 1 ? "y" : BC == 2 ? "z" : "w",
		DEST & 0x8 ? "x" : "", DEST & 0x4 ? "y" : "",
		DEST & 0x2 ? "z" : "", DEST & 0x1 ? "w" : "",
		FS, FT);

	return Local_dis_str;
}
