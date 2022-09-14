#pragma once

#include "EmuPS2Core.h"

namespace Interpreter
{
	class Interpreter
	{
	public:
		Interpreter();
		~Interpreter();

		void Init(EMU_PS2_Regs* PS2Regs);

		void Run();

	private:
		EMU_PS2_Regs* m_PS2Regs;
	};
}