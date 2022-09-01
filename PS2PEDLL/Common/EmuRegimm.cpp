#include "EmuExecute.h"

void emuRegimm(EMU_U32 Code, stEmuTemp& emuTemp)
{
    ///////////////////////////////////
    // REGIMM Functions
    ///////////////////////////////////
    switch (Code & 0x001F0000)
    {
    case 0x00000000: // "bltz"
        BRANCH_CONDITION_RS_ZERO(< );
        break;

    case 0x00010000: // "bgez"
        BRANCH_CONDITION_RS_ZERO(>= );
        break;

    case 0x00020000: // "bltzl"
        BRANCH_CONDITION_RS_ZERO_LIKELY(< );
        break;

    case 0x00030000: // "bgezl"
        BRANCH_CONDITION_RS_ZERO_LIKELY(>= );
        break;

    case 0x00080000: // "tgei"
    case 0x00090000: // "tgeiu"
    case 0x000A0000: // "tlti"
    case 0x000B0000: // "tltiu"
    case 0x000C0000: // "teqi"
    case 0x000E0000: // "tnei"
        break;

    case 0x00100000: // "bltzal"
        BRANCH_CONDITION_RS_ZERO_LINK(< );
        break;

    case 0x00110000: // "bgezal"
        BRANCH_CONDITION_RS_ZERO_LINK(>= );
        break;

    case 0x00120000: // "bltzall"
        BRANCH_CONDITION_RS_ZERO_LINK_LIKELY(< );
        break;

    case 0x00130000: // "bgezall"
        BRANCH_CONDITION_RS_ZERO_LINK_LIKELY(>= );
        break;

    case 0x00180000: // "mtsab"
        R5900Regs.SA = ((R5900Regs.Reg[R_RS].u08_00_07 & 0xF) ^
            (Code & 0x0000000F)) * 8;
        break;

    case 0x00190000: // "mtsah"
        R5900Regs.SA = ((R5900Regs.Reg[R_RS].u08_00_07 & 0x7) ^
            (Code & 0x00000007)) * 16;
        break;
    }
    ///////////////////////////////////
    // End of REGIMM Functions
    ///////////////////////////////////
}