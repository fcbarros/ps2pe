#include "Interpreter.h"

#include <stdio.h>

namespace Interpreter
{
	Interpreter::Interpreter()
	{
	}

	Interpreter::~Interpreter()
	{
	}

	void Interpreter::EmuConsole2(const char* Format, ...)
	{
		//if (EmuConsoleCallback)
		{
			char Text[4096];
			va_list vaParams;

			va_start(vaParams, Format);
			vsprintf(Text, Format, vaParams);
			va_end(vaParams);
			printf(Text);
			//EmuConsoleCallback(Text);
		}
	}

	bool Interpreter::EmuIsBreakPoint2(EMU_U32 Address)
	{
		for (EMU_U32 i = 0; i < BreakPoints.size(); i++)
		{
			if (BreakPoints[i] == Address)
			{
				return true;
			}
		}

		return false;
	}

	EMU_U32 Interpreter::EmuInstructionIndex3(EMU_U32 tInst)
	{
		for (EMU_U32 i = 0; i < TotalInstructions; i++)
		{
			if ((tInst & EmuInstructions[i].Mask) == EmuInstructions[i].Code)
			{
				return i;
			}
		}

		return TotalInstructions;
	}
}