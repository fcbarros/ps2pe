
///////////////////////////////////
// REGIMM Functions
///////////////////////////////////
switch ( Code & 0x001F0000 )
{
case 0x00000000: // "bltz"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    break;

case 0x00010000: // "bgez"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    break;

case 0x00020000: // "bltzl"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    else
    {
        R5900Regs.PC += 4;
    }
    break;

case 0x00030000: // "bgezl"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    else
    {
        R5900Regs.PC += 4;
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
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    break;

case 0x00110000: // "bgezal"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    break;

case 0x00120000: // "bltzall"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 < 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    else
    {
        R5900Regs.PC += 4;
    }
    break;

case 0x00130000: // "bgezall"
    if ( R5900Regs.Reg[ R_RS ].i64_00_63 >= 0 )
    {
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
    }
    else
    {
        R5900Regs.PC += 4;
    }
    break;

case 0x00180000: // "mtsab"
    R5900Regs.SA = ( ( R5900Regs.Reg[ R_RS ].u08_00_07 & 0xF ) ^ 
                     ( Code & 0x0000000F ) ) * 8;
    break;

case 0x00190000: // "mtsah"
    R5900Regs.SA = ( ( R5900Regs.Reg[ R_RS ].u08_00_07 & 0x7) ^ 
                    ( Code & 0x00000007 ) ) * 16;
    break;
}
///////////////////////////////////
// End of REGIMM Functions
///////////////////////////////////
