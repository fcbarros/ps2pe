#include "Interpreter.h"

using namespace Common;

namespace Interpreter
{
    void Interpreter::EmuRegimm()
    {
        ///////////////////////////////////
        // REGIMM Functions
        ///////////////////////////////////
        switch (OpCode & 0x001F0000)
        {
        case 0x00000000: // "bltz"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 < 0)
            {
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            break;

        case 0x00010000: // "bgez"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 >= 0)
            {
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            break;

        case 0x00020000: // "bltzl"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 < 0)
            {
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            else
            {
                PS2Regs.R5900Regs.PC += 4;
            }
            break;

        case 0x00030000: // "bgezl"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 >= 0)
            {
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            else
            {
                PS2Regs.R5900Regs.PC += 4;
            }
            break;

        case 0x00080000: // "tgei"
        case 0x00090000: // "tgeiu"
        case 0x000A0000: // "tlti"
        case 0x000B0000: // "tltiu"
        case 0x000C0000: // "teqi"
        case 0x000E0000: // "tnei"
            break;

        case 0x00100000: // "bltzal"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 < 0)
            {
                PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            break;

        case 0x00110000: // "bgezal"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 >= 0)
            {
                PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            break;

        case 0x00120000: // "bltzall"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 < 0)
            {
                PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            else
            {
                PS2Regs.R5900Regs.PC += 4;
            }
            break;

        case 0x00130000: // "bgezall"
            if (PS2Regs.R5900Regs.Reg[R_RS].i64_00_63 >= 0)
            {
                PS2Regs.R5900Regs.RA.u64_00_63 = PS2Regs.R5900Regs.PC + 4;
                ExecuteBranch(PS2Regs.R5900Regs.PC);
            }
            else
            {
                PS2Regs.R5900Regs.PC += 4;
            }
            break;

        case 0x00180000: // "mtsab"
            PS2Regs.R5900Regs.SA = ((PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 & 0xF) ^ (OpCode & 0x0000000F)) * 8;
            break;

        case 0x00190000: // "mtsah"
            PS2Regs.R5900Regs.SA = ((PS2Regs.R5900Regs.Reg[R_RS].u08_00_07 & 0x7) ^ (OpCode & 0x00000007)) * 16;
            break;
        }
        ///////////////////////////////////
        // End of REGIMM Functions
        ///////////////////////////////////
    }
}