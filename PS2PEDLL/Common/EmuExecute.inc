

    EMU_U32 JumpTo;

    R5900Regs.PC = tAddress;

    register EMU_U32 Code;

    while ( ! EmuStopRun )
    {
        if ( ( R5900Regs.R0.u64_00_63 != 0 ) || ( R5900Regs.R0.u64_64_127 != 0 ) )
        {
            EmuConsole( "Register R0 is not zero." );
        }
        // If the PC is FFFFFFFF then we are exiting from an interrupt handler
        if ( R5900Regs.PC == 0xFFFFFFFF )
        {
            Emu_Intc_ExitHandler( );
        }
        else
        if ( R5900Regs.PC == 0xFFFFFFFE )
        {
            Emu_Dma_ExitHandler( );
        }

        // Checking for nop, skipping if no operation

		Code = EMemory.GetWord( R5900Regs.PC );
        R5900Regs.PC += 4;
        if ( Code ) 
        {
#ifdef EMU_STATS
           EmuRunningStats[ EmuInstructionIndex2( Code ) ].Total++;
#endif

/////////////////////////////////////////////////////////////////////////
// Main Loop
/////////////////////////////////////////////////////////////////////////

            #include "EmuCore.inc"

/////////////////////////////////////////////////////////////////////////
// End of Main Loop
/////////////////////////////////////////////////////////////////////////
        }
        COP0Regs.Count++;
		CpuCycles++;

        // No reason for FF, just not to call the function always, this way
        // we call it every 256 instructions excutions
        if ( ! ( CpuCycles & 0xFF ) )
        {
            Emu_GS_ProcessMessages( );
        }

        if ( ! InLoop )
        {
            break;
        }

#ifdef EMU_STATS
        if ( ( InstrBreakPoints[ EmuInstructionIndex2( EMemory.GetWord( R5900Regs.PC ) ) ] ) ||
             ( EmuIsBreakPoint( R5900Regs.PC ) ) )
        {
            break;
        }
#endif
    }
