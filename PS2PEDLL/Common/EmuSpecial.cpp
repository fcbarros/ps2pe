#include "EmuExecute.h"

void emuSpecial(EMU_U32 Code, stEmuTemp& emuTemp)
{
    EMU_U32 JumpTo{ 0 };

    ///////////////////////////////////
    // Special Functions
    ///////////////////////////////////
    switch (Code & 0x0000003F)
    {
    case 0x00000000: // "sll"
        emuTemp.TempU32 = R5900Regs.Reg[R_RT].u32_00_31 << R_SA;
        R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)emuTemp.TempU32;
        break;

    case 0x00000002: // "srl"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u32_00_31 >> R_SA;
        break;

    case 0x00000003: // "sra"
        R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RT].i32_00_31 >> R_SA);
        break;

    case 0x00000004: // "sllv"
        emuTemp.TempU32 = R5900Regs.Reg[R_RT].u32_00_31 << (R5900Regs.Reg[R_RS].u32_00_31 & 0x1F);
        R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)emuTemp.TempU32;
        break;

    case 0x00000006: // "srlv"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u32_00_31 >> (R5900Regs.Reg[R_RS].u32_00_31 & 0x1F);
        break;

    case 0x00000007: // "srav"
        R5900Regs.Reg[R_RD].u64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RT].i32_00_31 >>
            (R5900Regs.Reg[R_RS].u32_00_31 & 0x1F));
        break;

    case 0x00000008: // "jr"
        // It does not zero extend the address if it
        // crosses the 32bit bondary
        JumpTo = R5900Regs.Reg[R_RS].u32_00_31;
        EXECUTE_BRANCH_DELAY_SLOT(R5900Regs.PC);
        R5900Regs.PC = JumpTo;
        break;

    case 0x00000009: // "jalr"
        // It does not zero extend the address if it
        // crosses the 32bit bondary
        if (R_RD)
        {
            R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.PC + 4;
        }
        JumpTo = R5900Regs.Reg[R_RS].u32_00_31;
        EXECUTE_BRANCH_DELAY_SLOT(R5900Regs.PC);
        R5900Regs.PC = JumpTo;
        break;

    case 0x0000000A: // "movz"
        if (R5900Regs.Reg[R_RT].u64_00_63 == 0)
        {
            R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63;
        }
        break;

    case 0x0000000B: // "movn"
        if (R5900Regs.Reg[R_RT].u64_00_63 != 0)
        {
            R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63;
        }
        break;

    case 0x0000000C: // "syscall"
        Emu_Bios_Syscall();
        break;

    case 0x0000000D: // "break"
    case 0x0000000F: // "sync"
        break;

    case 0x00000010: // "mfhi"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.HI.u64_00_63;
        break;

    case 0x00000011: // "mthi"
        R5900Regs.HI.u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63;
        break;

    case 0x00000012: // "mflo"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
        break;

    case 0x00000013: // "mtlo"
        R5900Regs.LO.u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63;
        break;

    case 0x00000014: // "dsllv"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63 << (R5900Regs.Reg[R_RS].u32_00_31 & 0x3F);
        break;

    case 0x00000016: // "dsrlv"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63 >> (R5900Regs.Reg[R_RS].u32_00_31 & 0x3F);
        break;

    case 0x00000017: // "dsrav"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].i64_00_63 >> (R5900Regs.Reg[R_RS].u32_00_31 & 0x3F);
        break;

    case 0x00000018: // "mult"
        emuTemp.Temp128.i64_00_63 = (EMU_I64)R5900Regs.Reg[R_RS].i32_00_31 * (EMU_I64)R5900Regs.Reg[R_RT].i32_00_31;
        R5900Regs.LO.u64_00_63 = emuTemp.Temp128.i32_00_31;
        R5900Regs.HI.u64_00_63 = emuTemp.Temp128.i32_32_63;
        if (R_RD)
        {
            R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
        }
        break;

    case 0x00000019: // "multu"
        emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 * (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
        R5900Regs.LO.u64_00_63 = emuTemp.Temp128.i32_00_31;
        R5900Regs.HI.u64_00_63 = emuTemp.Temp128.i32_32_63;
        if (R_RD)
        {
            R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.LO.u64_00_63;
        }
        break;

    case 0x0000001A: // "div"
        if (R5900Regs.Reg[R_RT].u32_00_31)
        {
            R5900Regs.LO.i64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].i32_00_31 / R5900Regs.Reg[R_RT].i32_00_31);
            R5900Regs.HI.i64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].i32_00_31 % R5900Regs.Reg[R_RT].i32_00_31);
        }
        break;

    case 0x0000001B: // "divu"
        if (R5900Regs.Reg[R_RT].u32_00_31)
        {
            R5900Regs.LO.i64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_00_31 / R5900Regs.Reg[R_RT].u32_00_31);
            R5900Regs.HI.i64_00_63 = (EMU_I32)(R5900Regs.Reg[R_RS].u32_00_31 % R5900Regs.Reg[R_RT].u32_00_31);
        }
        break;

    case 0x00000020: // "add"
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
        R5900Regs.Reg[R_RD].i64_00_63 = emuTemp.Temp128.i32_00_31;
        break;

    case 0x00000021: // "addu"
        emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 + (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
        R5900Regs.Reg[R_RD].i64_00_63 = emuTemp.Temp128.i32_00_31;
        break;

    case 0x00000022: // "sub"
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 - (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
        R5900Regs.Reg[R_RD].i64_00_63 = emuTemp.Temp128.i32_00_31;
        break;

    case 0x00000023: // "subu"
        emuTemp.Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[R_RS].u32_00_31 - (EMU_U64)R5900Regs.Reg[R_RT].u32_00_31;
        R5900Regs.Reg[R_RD].i64_00_63 = emuTemp.Temp128.i32_00_31;
        break;

    case 0x00000024: // "and"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 &
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x00000025: // "or"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 |
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x00000026: // "xor"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 ^
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x00000027: // "nor"
        R5900Regs.Reg[R_RD].u64_00_63 = ~(R5900Regs.Reg[R_RS].u64_00_63 |
            R5900Regs.Reg[R_RT].u64_00_63);
        break;

    case 0x00000028: // "mfsa"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.SA;
        break;

    case 0x00000029: // "mtsa"
        R5900Regs.SA = R5900Regs.Reg[R_RS].u64_00_63;
        break;

    case 0x0000002A: // "slt"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].i64_00_63 < R5900Regs.Reg[R_RT].i64_00_63;
        break;

    case 0x0000002B: // "sltu"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 < R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x0000002C: // "dadd"
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 +
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x0000002D: // "daddu"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 +
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x0000002E: // "dsub"
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 -
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x0000002F: // "dsubu"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RS].u64_00_63 -
            R5900Regs.Reg[R_RT].u64_00_63;
        break;

    case 0x00000030: // "tge"
    case 0x00000031: // "tgeu"
    case 0x00000032: // "tlt"
    case 0x00000033: // "tltu"
    case 0x00000034: // "teq"
    case 0x00000036: // "tne"
        break;

    case 0x00000038: // "dsll"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63 << R_SA;
        break;

    case 0x0000003A: // "dsrl"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63 >> R_SA;
        break;

    case 0x0000003B: // "dsra"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].i64_00_63 >> R_SA;
        break;

    case 0x0000003C: // "dsll32"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63 << (R_SA + 32);
        break;

    case 0x0000003E: // "dsrl32"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].u64_00_63 >> (R_SA + 32);
        break;

    case 0x0000003F: // "dsra32"
        R5900Regs.Reg[R_RD].u64_00_63 = R5900Regs.Reg[R_RT].i64_00_63 >> (R_SA + 32);
        break;
    }
}