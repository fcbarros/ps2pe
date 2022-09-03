
/////////////////////////////////////////////////////////////////////////
// Core Instructions
/////////////////////////////////////////////////////////////////////////

    switch ( Code & 0xFC000000 )
    {
    case 0x00000000: // SPECIAL
        #include "EmuSpecial.inc"
        break;

    case 0x04000000: // REGIMM
        #include "EmuRegimm.inc"
        break;

    case 0x08000000: // "j"
        JumpTo = JUMP_ADDRESS;
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
        break;

    case 0x0C000000: // "jal"
        // It does not zero extend the address if it
        // crosses the 32bit bondary
        R5900Regs.RA.u64_00_63 = R5900Regs.PC + 4;
        JumpTo = JUMP_ADDRESS;
		EXECUTE_BRANCH_DELAY( R5900Regs.PC );
        R5900Regs.PC = JumpTo;
        break;

    case 0x10000000: // "beq"
        if ( R5900Regs.Reg[ R_RS ].u64_00_63 == R5900Regs.Reg[ R_RT ].u64_00_63 )
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x14000000: // "bne"
        if ( R5900Regs.Reg[ R_RS ].u64_00_63 != R5900Regs.Reg[ R_RT ].u64_00_63 )
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x18000000: // "blez"
        if ( R5900Regs.Reg[ R_RS ].i64_00_63 <= 0 )
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x1C000000: // "bgtz"
        if ( R5900Regs.Reg[ R_RS ].i64_00_63 > 0 )
        {
            JumpTo = R5900Regs.PC + (((EMU_I16)R_BRANCH) << 2);
			EXECUTE_BRANCH_DELAY( R5900Regs.PC );
            R5900Regs.PC = JumpTo;
        }
        break;

    case 0x20000000: // "addi"
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_U64)(EMU_I16)R_IMMEDIATE;
        R5900Regs.Reg[ R_RT ].i64_00_63 = Temp128.i32_00_31;
        break;

    case 0x24000000: // "addiu"
        Temp128.u64_00_63 = (EMU_U64)R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_U64)(EMU_I16)R_IMMEDIATE;
        R5900Regs.Reg[ R_RT ].i64_00_63 = Temp128.i32_00_31;
        break;

    case 0x28000000: // "slti"
        R5900Regs.Reg[ R_RT ].u64_00_63 = ( R5900Regs.Reg[ R_RS ].i64_00_63 < (EMU_I64)(EMU_I16)R_IMMEDIATE );
        break;

    case 0x2C000000: // "sltiu"
        R5900Regs.Reg[ R_RT ].u64_00_63 = ( R5900Regs.Reg[ R_RS ].u64_00_63 < (EMU_U64)(EMU_I16)R_IMMEDIATE );
        break;

    case 0x30000000: // "andi"
        R5900Regs.Reg[ R_RT ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 & R_IMMEDIATE;
        break;

    case 0x34000000: // "ori"
        R5900Regs.Reg[ R_RT ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 | R_IMMEDIATE;
        break;

    case 0x38000000: // "xori"
        R5900Regs.Reg[ R_RT ].u64_00_63 = R5900Regs.Reg[ R_RS ].u64_00_63 ^ R_IMMEDIATE;
        break;

    case 0x3C000000: // "lui"
        R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I32)(R_IMMEDIATE << 16);
        break;

    case 0x40000000: // "COP0"
        #include "EmuCop0.inc"
        break;

    case 0x44000000: // "COP1"
        #include "EmuCop1.inc"
        break;

    case 0x48000000: // "COP2"
        #include "EmuCop2.inc"
        break;

    case 0x50000000: // "beql"
        if ( R5900Regs.Reg[ R_RS ].u64_00_63 == R5900Regs.Reg[ R_RT ].u64_00_63 )
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

    case 0x54000000: // "bnel"
        if ( R5900Regs.Reg[ R_RS ].u64_00_63 != R5900Regs.Reg[ R_RT ].u64_00_63 )
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

    case 0x58000000: // "blezl"
        if ( R5900Regs.Reg[ R_RS ].i64_00_63 <= 0 )
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

    case 0x5C000000: // "bgtzl"
        if ( R5900Regs.Reg[ R_RS ].i64_00_63 > 0 )
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

    case 0x60000000: // "daddi"
        // ******************************************
        // Not doing the OVERFLOW TRAP
        // ******************************************
        R5900Regs.Reg[ R_RT ].i64_00_63 = R5900Regs.Reg[ R_RS ].i64_00_63 + (EMU_I16)R_IMMEDIATE;
        break;

    case 0x64000000: // "daddiu"
        R5900Regs.Reg[ R_RT ].i64_00_63 = R5900Regs.Reg[ R_RS ].i64_00_63 + (EMU_I16)R_IMMEDIATE;
        break;

    case 0x68000000: // "ldl"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x07;
        TempU64 = EMemory.GetDWord( Address - 7 ) & ( 0xFFFFFFFFFFFFFFFF << ( ( 7 - SA ) << 3 ) );
        R5900Regs.Reg[ RT ].u64_00_63 = ( R5900Regs.Reg[ RT ].u64_00_63 &
                                        ( 0xFFFFFFFFFFFFFFFF >> ( ( SA + 1 ) << 3 ) ) ) |
                                        TempU64;
        break;

    case 0x6C000000: // "ldr"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x07;
        TempU64 = EMemory.GetDWord( Address ) & ( 0xFFFFFFFFFFFFFFFF >> ( SA  << 3 ) );
        R5900Regs.Reg[ RT ].u64_00_63 = ( R5900Regs.Reg[ RT ].u64_00_63 &
                                        ( 0xFFFFFFFFFFFFFFFF << ( ( 8 - SA ) << 3 ) ) ) |
                                        TempU64;
        break;

    case 0x70000000: // MMI
        #include "EmuMmi.inc"
        break;

    case 0x78000000: // "lq"
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        R5900Regs.Reg[ R_RT ].u64_00_63 = EMemory.GetDWord( Address );
        R5900Regs.Reg[ R_RT ].u64_64_127 = EMemory.GetDWord( Address + 8 );
        break;

    case 0x7C000000: // "sq"
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        EMemory.SetDWord( Address, R5900Regs.Reg[ R_RT ].u64_00_63 );
        EMemory.SetDWord( Address + 8, R5900Regs.Reg[ R_RT ].u64_64_127 );
        break;

    case 0x80000000: // "lb"
        R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I08)
            EMemory.GetByte( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0x84000000: // "lh"
        R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I16)
            EMemory.GetShort( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0x88000000: // "lwl"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x03;
        TempU32 = EMemory.GetWord( Address - 3 ) & ( 0xFFFFFFFF << ( ( 3 - SA ) << 3 ) );
        R5900Regs.Reg[ RT ].u32_00_31 &= 0xFFFFFFFF >> ( ( SA + 1 ) << 3 );
        R5900Regs.Reg[ RT ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ RT ].u32_00_31 | TempU32 );
        break;

    case 0x8C000000: // "lw"
        R5900Regs.Reg[ R_RT ].i64_00_63 = (EMU_I32)
            EMemory.GetWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0x90000000: // "lbu"
        R5900Regs.Reg[ R_RT ].u64_00_63 = 
            EMemory.GetByte( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0x94000000: // "lhu"
        R5900Regs.Reg[ R_RT ].u64_00_63 = 
            EMemory.GetShort( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0x98000000: // "lwr"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        TempU32 = EMemory.GetWord( Address );
        SA = Address & 0x03;
        if ( SA )
        {
            TempU32 &= 0xFFFFFFFF >> ( SA << 3 );
            R5900Regs.Reg[ RT ].u32_00_31 &= 0xFFFFFFFFFFFFFFFF << ( ( 4 - SA ) << 3 );
            R5900Regs.Reg[ RT ].u32_00_31 |= TempU32;
        }
        else
        {
            R5900Regs.Reg[ RT ].u64_00_63 = (EMU_I32)TempU32;
        }
        break;

    case 0x9C000000: // "lwu"
        R5900Regs.Reg[ R_RT ].u64_00_63 = 
            EMemory.GetWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0xA0000000: // "sb"
        EMemory.SetByte( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE, 
            R5900Regs.Reg[ R_RT ].u08_00_07 );
        break;

    case 0xA4000000: // "sh"
        EMemory.SetShort( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE, 
            R5900Regs.Reg[ R_RT ].u16_00_15 );
        break;

    case 0xA8000000: // "swl"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x3;
        TempU32 = ( EMemory.GetWord( Address - 3 ) & 
                  ( 0xFFFFFFFF >> ( ( SA + 1 ) << 3 ) ) ) |
                  ( R5900Regs.Reg[ RT ].u32_00_31 & ( 0xFFFFFFFF << ( ( 3 - SA ) << 3 ) ) );
        EMemory.SetWord( Address - 3, TempU32 );
        break;

    case 0xAC000000: // "sw"
        EMemory.SetWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE, 
                         R5900Regs.Reg[ R_RT ].u32_00_31 );
        break;

    case 0xB0000000: // "sdl"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x07;
        TempU64 = ( EMemory.GetDWord( Address - 7 ) & ( 0xFFFFFFFFFFFFFFFF >> ( ( SA + 1 ) << 3 ) ) ) |
                  ( R5900Regs.Reg[ RT ].u64_00_63 & ( 0xFFFFFFFFFFFFFFFF << ( ( 7 - SA ) << 3 ) ) );
        EMemory.SetDWord( Address - 7, TempU64 );
        break;

    case 0xB4000000: // "sdr"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x07;
        TempU64 = ( EMemory.GetDWord( Address ) & 
                ( 0xFFFFFFFFFFFFFFFF << ( ( 8 - SA ) << 3 ) ) ) |
                ( R5900Regs.Reg[ RT ].u64_00_63 & ( 0xFFFFFFFFFFFFFFFF >> ( SA << 3 ) ) );
        EMemory.SetDWord( Address, TempU64 );
        break;

/*    case 0xB0000000: // "sdl"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x07;
        TempU64 = ( EMemory.GetDWord( Address  & 0xFFFFFFFC ) & ( 0xFFFFFFFFFFFFFFFF << ( ( SA + 1 ) << 3 ) ) ) |
                  ( R5900Regs.Reg[ RT ].u64_00_63 >> ( ( 7 - SA ) << 3 ) );
        EMemory.SetDWord( Address  & 0xFFFFFFFC, TempU64 );
        break;

    case 0xB4000000: // "sdr"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x07;
        TempU64 = ( EMemory.GetDWord( Address & 0xFFFFFFFC ) & ( 0xFFFFFFFFFFFFFFFF >> ( ( 8 - SA ) << 3 ) ) ) |
                ( R5900Regs.Reg[ RT ].u64_00_63 << ( SA << 3 ) );
        EMemory.SetDWord( Address & 0xFFFFFFFC, TempU64 );
        break;
*/
    case 0xB8000000: // "swr"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        SA = Address & 0x3;
        TempU32 = ( EMemory.GetWord( Address ) & ( 0xFFFFFFFF << ( ( 4 - SA ) << 3 ) ) ) |
			   ( R5900Regs.Reg[ RT ].u32_00_31 & ( 0xFFFFFFFF >> ( SA << 3 ) ) );
        EMemory.SetWord( Address, TempU32 );
        break;

    case 0xBC000000: // "cache"
        break;

    case 0xC4000000: // "lwc1"      
        TempU32 = EMemory.GetWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        COP1Regs.Reg[ R_RT ].f = *(float*)&TempU32;
        break;

    case 0xCC000000: // "pref"      
        break;

    case 0xD8000000: // "lqc2"
        RT = R_RT;
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        VU0Regs.CPR[ RT ].X.u = EMemory.GetWord( Address );
        VU0Regs.CPR[ RT ].Y.u = EMemory.GetWord( Address + 4 );
        VU0Regs.CPR[ RT ].Z.u = EMemory.GetWord( Address + 8 );
        VU0Regs.CPR[ RT ].W.u = EMemory.GetWord( Address + 12 );
        break;

    case 0xDC000000: // "ld"
        R5900Regs.Reg[ R_RT ].u64_00_63 = 
                EMemory.GetDWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE );
        break;

    case 0xE4000000: // "swc1"
        EMemory.SetWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE, 
                COP1Regs.Reg[ R_FT ].u );
        break;

    case 0xF8000000: // "sqc2"
        Address = R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE;
        RT = R_RT;
        EMemory.SetWord( Address,       VU0Regs.CPR[ RT ].X.u );
        EMemory.SetWord( Address + 4,   VU0Regs.CPR[ RT ].Y.u );
        EMemory.SetWord( Address + 8,   VU0Regs.CPR[ RT ].Z.u );
        EMemory.SetWord( Address + 12,  VU0Regs.CPR[ RT ].W.u );
        break;

    case 0xFC000000: // "sd"
        EMemory.SetDWord( R5900Regs.Reg[ R_RS ].u32_00_31 + (EMU_I16)R_IMMEDIATE, 
                        R5900Regs.Reg[ R_RT ].u64_00_63 );
        break;
    }            


/////////////////////////////////////////////////////////////////////////
// End of Core Instructions
/////////////////////////////////////////////////////////////////////////