
///////////////////////////////////
// COP1 Functions
///////////////////////////////////
switch ( Code & 0x03E00000 )
{
case 0x00000000: // "mfc1"
    R5900Regs.Reg[ R_RT ].u64_00_63 = *(EMU_I32*)&COP1Regs.Reg[ R_FS ];
    break;

case 0x00400000: // "cfc1"
    R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I32)( R_RD ? COP1Regs.FCR31 : COP1Regs.FCR0 );
    break;

case 0x00800000: // "mtc1"
    *(EMU_U32*)&COP1Regs.Reg[ R_RD ] = R5900Regs.Reg[ R_RT ].u32_00_31;
    break;

case 0x00C00000: // "ctc1"
    if ( R_RD )
    {
        COP1Regs.FCR31 = R5900Regs.Reg[ R_RT ].u32_00_31;
    }
    else
    {
        COP1Regs.FCR0 = R5900Regs.Reg[ R_RT ].u32_00_31;
    }
    break;

case 0x01000000: // "BC1"
    ///////////////////////////////////
    // BC1 Functions
    ///////////////////////////////////
    switch ( Code & 0x001F0000 )
    {
    case 0x00000000: // "bc1f",
        if ( COP1Regs.FCR31_C == 0 )
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x00010000: // "bc1t",
        if ( COP1Regs.FCR31_C == 1 )
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x00020000: // "bc1fl"
        if ( COP1Regs.FCR31_C == 0 )
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

    case 0x00030000: // "bc1tl"
        if ( COP1Regs.FCR31_C == 1 )
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
    }
    ///////////////////////////////////
    // End of BC1 Functions
    ///////////////////////////////////
    break;

case 0x02000000: // "S"
    ///////////////////////////////////
    // S Functions
    ///////////////////////////////////
    switch ( Code & 0x0000003F )
    {
    case 0x00000000: // "add.s"
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f + COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 0;
        }
        break;

    case 0x00000001: // "sub.s"
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f - COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 0;
        }
        break;

    case 0x00000002: // "mul.s"
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 0;
        }
        break;

    case 0x00000003: // "div.s"
        FT = R_FT;
        FS = R_FS;
        if ( ( COP1Regs.Reg[ FS ].u == 0 ) && ( COP1Regs.Reg[ FT ].u == 0 ) )
        {
            COP1Regs.FCR31_I = 1;
            COP1Regs.FCR31_D = 0;
            COP1Regs.FCR31_SI = 1;
            COP1Regs.Reg[ R_FD ].u = 0;
        }
        else
        if ( ( COP1Regs.Reg[ FS ].u != 0 ) && ( COP1Regs.Reg[ FT ].u == 0 ) ) 
        {
            COP1Regs.FCR31_I = 0;
            COP1Regs.FCR31_D = 1;
            COP1Regs.FCR31_SD = 1;
        }
        else
        {
            COP1Regs.FCR31_I = 0;
            COP1Regs.FCR31_D = 0;
            COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ FS ].f / COP1Regs.Reg[ FT ].f;
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        break;

    case 0x00000004: // "sqrt.s"
        FT = R_RT;
        if ( COP1Regs.Reg[ FT ].f < 0.0f ) 
        {
            COP1Regs.FCR31_I = 1;
            COP1Regs.FCR31_SI = 1;
        }
        else
        {
            COP1Regs.FCR31_I = 0;
            COP1Regs.Reg[ R_FD ].f = (float)sqrt( (double)COP1Regs.Reg[ FT ].f );
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        COP1Regs.FCR31_D = 0;
        break;

    case 0x00000005: // "abs.s"
        COP1Regs.Reg[ R_FD ].f = (float)fabs( COP1Regs.Reg[ R_FS ].f );
        COP1Regs.FCR31_O = 0;
        COP1Regs.FCR31_U = 0;
        break;

    case 0x00000006: // "mov.s"
        COP1Regs.Reg[ R_FD ].u = COP1Regs.Reg[ R_FS ].u;
        break;

    case 0x00000007: // "neg.s"
        COP1Regs.Reg[ R_FD ].f = -COP1Regs.Reg[ R_FS ].f;
        COP1Regs.FCR31_O = 0;
        COP1Regs.FCR31_U = 0;
        break;

    case 0x00000016: // "rsqrt.s"
        FT = R_FT; 
        if ( COP1Regs.Reg[ FT ].f < 0.0f ) 
        {
            COP1Regs.FCR31_I = 1;
            COP1Regs.FCR31_D = 0;
            COP1Regs.FCR31_SI = 1;
        }
        else
        if ( COP1Regs.Reg[ FT ].f == 0.0f ) 
        {
            COP1Regs.FCR31_I = 0;
            COP1Regs.FCR31_D = 1;
            COP1Regs.FCR31_SD = 1;
        }
        else
        {
            COP1Regs.FCR31_I = 0;
            COP1Regs.FCR31_D = 0;
            COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f / (float)(sqrt( COP1Regs.Reg[ FT ].f ));
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        break;

    case 0x00000018: // "adda.s"
        COP1Regs.Accumulator.f = COP1Regs.Reg[ R_FS ].f + COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 0;
        }
        break;

    case 0x00000019: // "suba.s"
        COP1Regs.Accumulator.f = COP1Regs.Reg[ R_FS ].f - COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 0;
        }
        break;

    case 0x0000001A: // "mula.s"
        COP1Regs.Accumulator.f = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 0;
        }
        break;

    case 0x0000001C: // "madd.s"
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.Reg[ R_FD ].f += COP1Regs.Accumulator.f;
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        break;

    case 0x0000001D: // "msub.s",
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.Reg[ R_FD ].f = COP1Regs.Accumulator.f - COP1Regs.Reg[ R_FD ].f;
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        break;

    case 0x0000001E: // "madda.s"
        TempF32 = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.Accumulator.f += TempF32;
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        break;

    case 0x0000001F: // "msuba.s"
        TempF32 = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
        EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
        if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 1;
            COP1Regs.FCR31_SO = 1;
            COP1Regs.FCR31_U = 0;
        }
        else
        if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
        {
            COP1Regs.FCR31_O = 0;
            COP1Regs.FCR31_U = 1;
            COP1Regs.FCR31_SU = 1;
        }
        else
        {
            COP1Regs.Accumulator.f -= TempF32;
            EMU_FLOAT_LOAD_CONDITION_REGISTER( EmuFloatStatus );
            if ( EMU_FLOAT_CHECK_OVERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 1;
                COP1Regs.FCR31_SO = 1;
                COP1Regs.FCR31_U = 0;
            }
            else
            if ( EMU_FLOAT_CHECK_UNDERFLOW( EmuFloatStatus ) ) 
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 1;
                COP1Regs.FCR31_SU = 1;
            }
            else
            {
                COP1Regs.FCR31_O = 0;
                COP1Regs.FCR31_U = 0;
            }
        }
        break;

    case 0x00000024: // "cvt.w.s"
        COP1Regs.Reg[ R_FD ].i = COP1Regs.Reg[ R_RD ].f;
        break;

    case 0x00000028: // "max.s"
        FT = R_FT;
        FS = R_FS;
        COP1Regs.Reg[ R_FD ].f = ( COP1Regs.Reg[ FS ].f >= COP1Regs.Reg[ FT ].f ) ?
                                   COP1Regs.Reg[ FS ].f : COP1Regs.Reg[ FT ].f;
        COP1Regs.FCR31_O = 0;
        COP1Regs.FCR31_U = 0;
        break;

    case 0x00000029: // "min.s"
        FT = R_FT;
        FS = R_FS;
        COP1Regs.Reg[ R_FD ].f = ( COP1Regs.Reg[ FS ].f <= COP1Regs.Reg[ FT ].f ) ?
                                   COP1Regs.Reg[ FS ].f : COP1Regs.Reg[ FT ].f;
        COP1Regs.FCR31_O = 0;
        COP1Regs.FCR31_U = 0;
        break;

    case 0x00000030: // "c.f.s"
        COP1Regs.FCR31_C = 0;
        break;

    case 0x00000032: // "c.eq.s"
        COP1Regs.FCR31_C = ( COP1Regs.Reg[ R_FS ].f == COP1Regs.Reg[ R_FT ].f );
        break;

    case 0x00000034: // "c.lt.s"
        COP1Regs.FCR31_C = ( COP1Regs.Reg[ R_FS ].f < COP1Regs.Reg[ R_FT ].f );
        break;

    case 0x00000036: // "c.le.s"
        COP1Regs.FCR31_C = ( COP1Regs.Reg[ R_FS ].f <= COP1Regs.Reg[ R_FT ].f );
        break;
    }
    ///////////////////////////////////
    // S Funcions
    ///////////////////////////////////
    break;

case 0x02800000: // "W"
    ///////////////////////////////////
    // W Functions
    ///////////////////////////////////
    if ( ( Code & 0x0000003F ) == 0x00000020 ) // "cvt.s.w"
    {
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].i;
    }
    ///////////////////////////////////
    // End of W Functions
    ///////////////////////////////////
    break;
}
///////////////////////////////////
// End of COP1 Functions
///////////////////////////////////
