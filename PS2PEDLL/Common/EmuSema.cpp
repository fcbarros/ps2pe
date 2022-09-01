
#include "EmuMain.h"
#include "EmuSema.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

unsigned long ksema[32];

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void Emu_Sema_Init( void )
{
    Emu_Sema_Reset( );
}

void Emu_Sema_Reset( void )
{
}

void Emu_Sema_Bios_Create( void )
{ // 0x40
	R5900Regs.V0.u64_00_63 = Emu_Sema_Create( R5900Regs.A0.u32_00_31 );
}

void Emu_Sema_Bios_Delete( void )
{ // 0x41
	R5900Regs.V0.u64_00_63 = Emu_Sema_Delete( R5900Regs.A0.u32_00_31 );
}

void Emu_Sema_Bios_Signal( void )
{ // 0x42
	R5900Regs.V0.u64_00_63 = Emu_Sema_Signal( R5900Regs.A0.u32_00_31 );
}

void Emu_Sema_Bios_Wait( void )
{ // 0x44
	R5900Regs.V0.u64_00_63 = Emu_Sema_Wait( R5900Regs.A0.u32_00_31 );
}

void Emu_Sema_Bios_Poll( void )
{ // 0x44
	R5900Regs.V0.u64_00_63 = Emu_Sema_Poll( R5900Regs.A0.u32_00_31 );
}

EMU_U64 Emu_Sema_Create( EMU_U32 SemaAddress )
{ // 0x40
    t_sema * sema;
    static EMU_U64 semaid;

    semaid++;
    sema = (t_sema*)EmuMemGetRealPointer( SemaAddress );
    ksema[ semaid - 1 ] = SemaAddress;

    return semaid;
}

EMU_U64 Emu_Sema_Delete( EMU_U32 SemaIndex )
{ // 0x41
    return SemaIndex;
}

EMU_U64 Emu_Sema_Signal( EMU_U32 SemaIndex )
{ // 0x42
    return SemaIndex;
}

EMU_U64 Emu_Sema_Wait( EMU_U32 SemaIndex )
{ // 0x44
    return SemaIndex;
}

EMU_U64 Emu_Sema_Poll( EMU_U32 SemaIndex )
{ // 0x45
    return SemaIndex;
}
