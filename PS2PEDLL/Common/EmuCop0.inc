
///////////////////////////////////
// COP0 Functions
///////////////////////////////////
switch ( Code & 0x03E00000 )
{
case 0x00000000: // "mfc0"
    R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I32)COP0Regs.Reg[ R_RD ];
    break;

case 0x00800000: // "mtc0"
    COP0Regs.Reg[ R_RD ] = R5900Regs.Reg[ R_RT ].u32_00_31;
    break;

case 0x01000000: // "BC0"
    ///////////////////////////////////
    // BC0 Functions
    ///////////////////////////////////
    switch ( Code & 0x001F0000 )
    {
    case 0x00000000: // "bc0f"
        // Branch if there is no DMA running, always for now
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x00010000: // "bc0t"
        break;

    case 0x00020000: // "bc0fl"
        // Branch if there is no DMA running, always for now
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x00030000: // "bc0tl"
        R5900Regs.PC += 4;
        break;
    }
    ///////////////////////////////////
    // End of BC0 Functions
    ///////////////////////////////////
    break;

case 0x02000000: // "C0"
    ///////////////////////////////////
    // C0 Functions
    ///////////////////////////////////
    switch ( Code & 0x0000003F )
    {
    case 0x00000001: // "tlbr"
    case 0x00000002: // "tlbwi"
    case 0x00000006: // "tlbwr"
    case 0x00000008: // "tlbp"
        break;

    case 0x00000018: // "eret"
        if ( COP0Regs.Status_ERL  == 1 ) 
        {
            COP0Regs.Status_ERL = 0;
            R5900Regs.PC = COP0Regs.Error_EPC;
        }
        else
        {
            COP0Regs.Status_ERL = 1;
            R5900Regs.PC = COP0Regs.EPC;
        }
        break;

    case 0x00000038: // "ei"
        if ( ( COP0Regs.Status_EDI == 1 ) || 
             ( COP0Regs.Status_EXL == 1 ) ||
             ( COP0Regs.Status_ERL == 1 ) ||
             ( COP0Regs.Status_KSU == 0 ) )
        {
            COP0Regs.Status_EIE = 1;
        }

        break;

    case 0x00000039: // "di"
        if ( ( COP0Regs.Status_EDI == 1 ) || 
             ( COP0Regs.Status_EXL == 1 ) ||
             ( COP0Regs.Status_ERL == 1 ) ||
             ( COP0Regs.Status_KSU == 0 ) )
        {
            COP0Regs.Status_EIE = 0;
        }

        break;
    }
    ///////////////////////////////////
    // End of C0 Functions
    ///////////////////////////////////
    break;
}
///////////////////////////////////
// End of COP0 Functions
///////////////////////////////////
