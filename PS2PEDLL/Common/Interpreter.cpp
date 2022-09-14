#include "Interpreter.h"

namespace Interpreter
{
	Interpreter::Interpreter()
		: m_PS2Regs{ nullptr }
	{
	}

	Interpreter::~Interpreter()
	{
	}

	void Interpreter::Init(EMU_PS2_Regs* PS2Regs)
	{
		m_PS2Regs = PS2Regs;
	}

	void Interpreter::Run()
	{

	}
}