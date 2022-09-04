
#include "EmuMain.h"
#include "EmuRecompiler.h"

inline EMU_U32 clamp( EMU_U32 X )
{
    if ( X > 0x00007FFF ) 
    {
        return 0x7FFF; 
    } 
    else 
    if ( X < 0xFFFF8000 ) 
    {
        return 0x8000;
    }
    else 
    {
        return X;
    }
}

void EmuRec_madd( EMU_U32 Code )
{
/*
    Temp128.u64_00_63 = (EMU_I64)( ( (EMU_I64)R5900Regs.HI.u32_00_31 << 32 ) | (EMU_I64)R5900Regs.LO.u32_00_31 ) + 
                                   ( (EMU_I64)R5900Regs.Reg[ R_RS ].i32_00_31 * (EMU_I64)R5900Regs.Reg[ R_RT ].i32_00_31 );
    R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
    R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
    RD = R_RD;
    if ( RD )
    {
        R5900Regs.Reg[ RD ].u64_00_63 = R5900Regs.LO.u64_00_63; 
    }
*/
}

void EmuRec_maddu( EMU_U32 Code )
{
/*
    Temp128.u64_00_63 = ( ( (EMU_U64)R5900Regs.HI.u32_00_31 << 32) | (EMU_U64)R5900Regs.LO.u32_00_31 ) + 
                          ( (EMU_U64)R5900Regs.Reg[ R_RS ].u32_00_31 * (EMU_U64)R5900Regs.Reg[ R_RT ].u32_00_31 );
    R5900Regs.LO.u64_00_63 = Temp128.i32_00_31;
    R5900Regs.HI.u64_00_63 = Temp128.i32_32_63;
    RD = R_RD;
    if ( RD )
    {
        R5900Regs.Reg[ RD ].u64_00_63 = R5900Regs.LO.u64_00_63; 
    }
*/
}

void EmuRec_plzcw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31 = 0;
    R5900Regs.Reg[ R_RD ].u32_32_63 = 0;
    TempU32 = R5900Regs.Reg[ R_RS ].u32_00_31;
    while ( ( TempU32 & 0x80000000 ) == 
        ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x80000000 ) )
    {
        R5900Regs.Reg[ R_RD ].u32_00_31++;
        TempU32 <<= 1;
    }
    TempU32 = R5900Regs.Reg[ R_RS ].u32_32_63;
    while ( ( TempU32 & 0x80000000 ) == 
        ( R5900Regs.Reg[ R_RS ].u32_32_63 & 0x80000000 ) )
    {
        R5900Regs.Reg[ R_RD ].u32_32_63++;
        TempU32 <<= 1;
    }
    R5900Regs.Reg[ R_RD ].u32_00_31--;
    R5900Regs.Reg[ R_RD ].u32_32_63--;
*/
}

void EmuRec_paddw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31  = R5900Regs.Reg[ R_RS ].u32_00_31  + R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_32_63  = R5900Regs.Reg[ R_RS ].u32_32_63  + R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_64_95  = R5900Regs.Reg[ R_RS ].u32_64_95  + R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_96_127 = R5900Regs.Reg[ R_RS ].u32_96_127 + R5900Regs.Reg[ R_RT ].u32_96_127;
*/
}

void EmuRec_psubw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31  = R5900Regs.Reg[ R_RS ].u32_00_31  - R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_32_63  = R5900Regs.Reg[ R_RS ].u32_32_63  - R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_64_95  = R5900Regs.Reg[ R_RS ].u32_64_95  - R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_96_127 = R5900Regs.Reg[ R_RS ].u32_96_127 - R5900Regs.Reg[ R_RT ].u32_96_127;
*/
}

void EmuRec_pcgtw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31  = ( R5900Regs.Reg[ R_RS ].u32_00_31  > R5900Regs.Reg[ R_RT ].u32_00_31  ) ? 0xFFFFFFFF : 0;
    R5900Regs.Reg[ R_RD ].u32_32_63  = ( R5900Regs.Reg[ R_RS ].u32_32_63  > R5900Regs.Reg[ R_RT ].u32_32_63  ) ? 0xFFFFFFFF : 0;
    R5900Regs.Reg[ R_RD ].u32_64_95  = ( R5900Regs.Reg[ R_RS ].u32_64_95  > R5900Regs.Reg[ R_RT ].u32_64_95  ) ? 0xFFFFFFFF : 0;
    R5900Regs.Reg[ R_RD ].u32_96_127 = ( R5900Regs.Reg[ R_RS ].u32_96_127 > R5900Regs.Reg[ R_RT ].u32_96_127 ) ? 0xFFFFFFFF : 0;
*/
}

void EmuRec_pmaxw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31  = ( R5900Regs.Reg[ R_RS ].i32_00_31  > R5900Regs.Reg[ R_RT ].i32_00_31  ) ?
                                         R5900Regs.Reg[ R_RS ].u32_00_31  : R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_32_63  = ( R5900Regs.Reg[ R_RS ].i32_32_63  > R5900Regs.Reg[ R_RT ].i32_32_63  ) ?
                                         R5900Regs.Reg[ R_RS ].u32_32_63  : R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_64_95  = ( R5900Regs.Reg[ R_RS ].i32_64_95  > R5900Regs.Reg[ R_RT ].i32_64_95  ) ?
                                         R5900Regs.Reg[ R_RS ].u32_64_95  : R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_96_127 = ( R5900Regs.Reg[ R_RS ].i32_96_127 > R5900Regs.Reg[ R_RT ].i32_96_127 ) ?
                                         R5900Regs.Reg[ R_RS ].u32_96_127 : R5900Regs.Reg[ R_RT ].u32_96_127;
*/
}

void EmuRec_paddh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RS ].u16_00_15   + R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RS ].u16_16_31   + R5900Regs.Reg[ R_RT ].u16_16_31;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RS ].u16_32_47   + R5900Regs.Reg[ R_RT ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RS ].u16_48_63   + R5900Regs.Reg[ R_RT ].u16_48_63;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RS ].u16_64_79   + R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RS ].u16_80_95   + R5900Regs.Reg[ R_RT ].u16_80_95;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RS ].u16_96_111  + R5900Regs.Reg[ R_RT ].u16_96_111;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RS ].u16_112_127 + R5900Regs.Reg[ R_RT ].u16_112_127;
*/
}

void EmuRec_psubh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RS ].u16_00_15   - R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RS ].u16_16_31   - R5900Regs.Reg[ R_RT ].u16_16_31;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RS ].u16_32_47   - R5900Regs.Reg[ R_RT ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RS ].u16_48_63   - R5900Regs.Reg[ R_RT ].u16_48_63;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RS ].u16_64_79   - R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RS ].u16_80_95   - R5900Regs.Reg[ R_RT ].u16_80_95;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RS ].u16_96_111  - R5900Regs.Reg[ R_RT ].u16_96_111;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RS ].u16_112_127 - R5900Regs.Reg[ R_RT ].u16_112_127;
*/
}

void EmuRec_pcgth( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = ( R5900Regs.Reg[ R_RS ].u16_00_15   > R5900Regs.Reg[ R_RT ].u16_00_15   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_16_31   = ( R5900Regs.Reg[ R_RS ].u16_16_31   > R5900Regs.Reg[ R_RT ].u16_16_31   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_32_47   = ( R5900Regs.Reg[ R_RS ].u16_32_47   > R5900Regs.Reg[ R_RT ].u16_32_47   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_48_63   = ( R5900Regs.Reg[ R_RS ].u16_48_63   > R5900Regs.Reg[ R_RT ].u16_48_63   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_64_79   = ( R5900Regs.Reg[ R_RS ].u16_64_79   > R5900Regs.Reg[ R_RT ].u16_64_79   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_80_95   = ( R5900Regs.Reg[ R_RS ].u16_80_95   > R5900Regs.Reg[ R_RT ].u16_80_95   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_96_111  = ( R5900Regs.Reg[ R_RS ].u16_96_111  > R5900Regs.Reg[ R_RT ].u16_96_111  ) ? 0xFFFF : 0;
    R5900Regs.Reg[ R_RD ].u16_112_127 = ( R5900Regs.Reg[ R_RS ].u16_112_127 > R5900Regs.Reg[ R_RT ].u16_112_127 ) ? 0xFFFF : 0;
*/
}

void EmuRec_pmaxh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = ( R5900Regs.Reg[ R_RS ].i16_00_15   > R5900Regs.Reg[ R_RT ].i16_00_15   ) ?
                                          R5900Regs.Reg[ R_RS ].u16_00_15   : R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_16_31   = ( R5900Regs.Reg[ R_RS ].i16_16_31   > R5900Regs.Reg[ R_RT ].i16_16_31   ) ?
                                          R5900Regs.Reg[ R_RS ].u16_16_31   : R5900Regs.Reg[ R_RT ].u16_16_31;
    R5900Regs.Reg[ R_RD ].u16_32_47   = ( R5900Regs.Reg[ R_RS ].i16_32_47   > R5900Regs.Reg[ R_RT ].i16_32_47   ) ?
                                          R5900Regs.Reg[ R_RS ].u16_32_47   : R5900Regs.Reg[ R_RT ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_48_63   = ( R5900Regs.Reg[ R_RS ].i16_48_63   > R5900Regs.Reg[ R_RT ].i16_48_63   ) ?
                                          R5900Regs.Reg[ R_RS ].u16_48_63   : R5900Regs.Reg[ R_RT ].u16_48_63;
    R5900Regs.Reg[ R_RD ].u16_64_79   = ( R5900Regs.Reg[ R_RS ].i16_64_79   > R5900Regs.Reg[ R_RT ].i16_64_79   ) ?
                                          R5900Regs.Reg[ R_RS ].u16_64_79   : R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_80_95   = ( R5900Regs.Reg[ R_RS ].i16_80_95   > R5900Regs.Reg[ R_RT ].i16_80_95   ) ?
                                          R5900Regs.Reg[ R_RS ].u16_80_95   : R5900Regs.Reg[ R_RT ].u16_80_95;
    R5900Regs.Reg[ R_RD ].u16_96_111  = ( R5900Regs.Reg[ R_RS ].i16_96_111  > R5900Regs.Reg[ R_RT ].i16_96_111  ) ?
                                          R5900Regs.Reg[ R_RS ].u16_96_111  : R5900Regs.Reg[ R_RT ].u16_96_111;
    R5900Regs.Reg[ R_RD ].u16_112_127 = ( R5900Regs.Reg[ R_RS ].i16_112_127 > R5900Regs.Reg[ R_RT ].i16_112_127 ) ?
                                          R5900Regs.Reg[ R_RS ].u16_112_127 : R5900Regs.Reg[ R_RT ].u16_112_127;
*/
}

void EmuRec_paddb( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u08_00_07   = R5900Regs.Reg[ R_RS ].u08_00_07   + R5900Regs.Reg[ R_RT ].u08_00_07;
    R5900Regs.Reg[ R_RD ].u08_08_15   = R5900Regs.Reg[ R_RS ].u08_08_15   + R5900Regs.Reg[ R_RT ].u08_08_15;
    R5900Regs.Reg[ R_RD ].u08_16_23   = R5900Regs.Reg[ R_RS ].u08_16_23   + R5900Regs.Reg[ R_RT ].u08_16_23;
    R5900Regs.Reg[ R_RD ].u08_24_31   = R5900Regs.Reg[ R_RS ].u08_24_31   + R5900Regs.Reg[ R_RT ].u08_24_31;
    R5900Regs.Reg[ R_RD ].u08_32_39   = R5900Regs.Reg[ R_RS ].u08_32_39   + R5900Regs.Reg[ R_RT ].u08_32_39;
    R5900Regs.Reg[ R_RD ].u08_40_47   = R5900Regs.Reg[ R_RS ].u08_40_47   + R5900Regs.Reg[ R_RT ].u08_40_47;
    R5900Regs.Reg[ R_RD ].u08_48_55   = R5900Regs.Reg[ R_RS ].u08_48_55   + R5900Regs.Reg[ R_RT ].u08_48_55;
    R5900Regs.Reg[ R_RD ].u08_56_63   = R5900Regs.Reg[ R_RS ].u08_56_63   + R5900Regs.Reg[ R_RT ].u08_56_63;
    R5900Regs.Reg[ R_RD ].u08_64_71   = R5900Regs.Reg[ R_RS ].u08_64_71   + R5900Regs.Reg[ R_RT ].u08_64_71;
    R5900Regs.Reg[ R_RD ].u08_72_79   = R5900Regs.Reg[ R_RS ].u08_72_79   + R5900Regs.Reg[ R_RT ].u08_72_79;
    R5900Regs.Reg[ R_RD ].u08_80_87   = R5900Regs.Reg[ R_RS ].u08_80_87   + R5900Regs.Reg[ R_RT ].u08_80_87;
    R5900Regs.Reg[ R_RD ].u08_88_95   = R5900Regs.Reg[ R_RS ].u08_88_95   + R5900Regs.Reg[ R_RT ].u08_88_95;
    R5900Regs.Reg[ R_RD ].u08_96_103  = R5900Regs.Reg[ R_RS ].u08_96_103  + R5900Regs.Reg[ R_RT ].u08_96_103;
    R5900Regs.Reg[ R_RD ].u08_104_111 = R5900Regs.Reg[ R_RS ].u08_104_111 + R5900Regs.Reg[ R_RT ].u08_104_111;
    R5900Regs.Reg[ R_RD ].u08_112_119 = R5900Regs.Reg[ R_RS ].u08_112_119 + R5900Regs.Reg[ R_RT ].u08_112_119;
    R5900Regs.Reg[ R_RD ].u08_120_127 = R5900Regs.Reg[ R_RS ].u08_120_127 + R5900Regs.Reg[ R_RT ].u08_120_127;
*/
}

void EmuRec_psubb( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u08_00_07   = R5900Regs.Reg[ R_RS ].u08_00_07   - R5900Regs.Reg[ R_RT ].u08_00_07;
    R5900Regs.Reg[ R_RD ].u08_08_15   = R5900Regs.Reg[ R_RS ].u08_08_15   - R5900Regs.Reg[ R_RT ].u08_08_15;
    R5900Regs.Reg[ R_RD ].u08_16_23   = R5900Regs.Reg[ R_RS ].u08_16_23   - R5900Regs.Reg[ R_RT ].u08_16_23;
    R5900Regs.Reg[ R_RD ].u08_24_31   = R5900Regs.Reg[ R_RS ].u08_24_31   - R5900Regs.Reg[ R_RT ].u08_24_31;
    R5900Regs.Reg[ R_RD ].u08_32_39   = R5900Regs.Reg[ R_RS ].u08_32_39   - R5900Regs.Reg[ R_RT ].u08_32_39;
    R5900Regs.Reg[ R_RD ].u08_40_47   = R5900Regs.Reg[ R_RS ].u08_40_47   - R5900Regs.Reg[ R_RT ].u08_40_47;
    R5900Regs.Reg[ R_RD ].u08_48_55   = R5900Regs.Reg[ R_RS ].u08_48_55   - R5900Regs.Reg[ R_RT ].u08_48_55;
    R5900Regs.Reg[ R_RD ].u08_56_63   = R5900Regs.Reg[ R_RS ].u08_56_63   - R5900Regs.Reg[ R_RT ].u08_56_63;
    R5900Regs.Reg[ R_RD ].u08_64_71   = R5900Regs.Reg[ R_RS ].u08_64_71   - R5900Regs.Reg[ R_RT ].u08_64_71;
    R5900Regs.Reg[ R_RD ].u08_72_79   = R5900Regs.Reg[ R_RS ].u08_72_79   - R5900Regs.Reg[ R_RT ].u08_72_79;
    R5900Regs.Reg[ R_RD ].u08_80_87   = R5900Regs.Reg[ R_RS ].u08_80_87   - R5900Regs.Reg[ R_RT ].u08_80_87;
    R5900Regs.Reg[ R_RD ].u08_88_95   = R5900Regs.Reg[ R_RS ].u08_88_95   - R5900Regs.Reg[ R_RT ].u08_88_95;
    R5900Regs.Reg[ R_RD ].u08_96_103  = R5900Regs.Reg[ R_RS ].u08_96_103  - R5900Regs.Reg[ R_RT ].u08_96_103;
    R5900Regs.Reg[ R_RD ].u08_104_111 = R5900Regs.Reg[ R_RS ].u08_104_111 - R5900Regs.Reg[ R_RT ].u08_104_111;
    R5900Regs.Reg[ R_RD ].u08_112_119 = R5900Regs.Reg[ R_RS ].u08_112_119 - R5900Regs.Reg[ R_RT ].u08_112_119;
    R5900Regs.Reg[ R_RD ].u08_120_127 = R5900Regs.Reg[ R_RS ].u08_120_127 - R5900Regs.Reg[ R_RT ].u08_120_127;
*/
}

void EmuRec_pcgtb( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u08_00_07   = ( R5900Regs.Reg[ R_RS ].u08_00_07   > R5900Regs.Reg[ R_RT ].u08_00_07   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_08_15   = ( R5900Regs.Reg[ R_RS ].u08_08_15   > R5900Regs.Reg[ R_RT ].u08_08_15   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_16_23   = ( R5900Regs.Reg[ R_RS ].u08_16_23   > R5900Regs.Reg[ R_RT ].u08_16_23   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_24_31   = ( R5900Regs.Reg[ R_RS ].u08_24_31   > R5900Regs.Reg[ R_RT ].u08_24_31   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_32_39   = ( R5900Regs.Reg[ R_RS ].u08_32_39   > R5900Regs.Reg[ R_RT ].u08_32_39   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_40_47   = ( R5900Regs.Reg[ R_RS ].u08_40_47   > R5900Regs.Reg[ R_RT ].u08_40_47   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_48_55   = ( R5900Regs.Reg[ R_RS ].u08_48_55   > R5900Regs.Reg[ R_RT ].u08_48_55   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_56_63   = ( R5900Regs.Reg[ R_RS ].u08_56_63   > R5900Regs.Reg[ R_RT ].u08_56_63   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_64_71   = ( R5900Regs.Reg[ R_RS ].u08_64_71   > R5900Regs.Reg[ R_RT ].u08_64_71   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_72_79   = ( R5900Regs.Reg[ R_RS ].u08_72_79   > R5900Regs.Reg[ R_RT ].u08_72_79   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_80_87   = ( R5900Regs.Reg[ R_RS ].u08_80_87   > R5900Regs.Reg[ R_RT ].u08_80_87   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_88_95   = ( R5900Regs.Reg[ R_RS ].u08_88_95   > R5900Regs.Reg[ R_RT ].u08_88_95   ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_96_103  = ( R5900Regs.Reg[ R_RS ].u08_96_103  > R5900Regs.Reg[ R_RT ].u08_96_103  ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_104_111 = ( R5900Regs.Reg[ R_RS ].u08_104_111 > R5900Regs.Reg[ R_RT ].u08_104_111 ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_112_119 = ( R5900Regs.Reg[ R_RS ].u08_112_119 > R5900Regs.Reg[ R_RT ].u08_112_119 ) ? 0xFF : 0;
    R5900Regs.Reg[ R_RD ].u08_120_127 = ( R5900Regs.Reg[ R_RS ].u08_120_127 > R5900Regs.Reg[ R_RT ].u08_120_127 ) ? 0xFF : 0;
*/
}

void EmuRec_paddsw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_00_31 + R5900Regs.Reg[ RT ].i32_00_31;       
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_00_31 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_00_31 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_00_31 = Temp128.u32_00_31;
    }

    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_32_63 + R5900Regs.Reg[ RT ].i32_32_63;
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_32_63 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_32_63 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_32_63 = Temp128.u32_00_31;
    }

    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_64_95 + R5900Regs.Reg[ RT ].i32_64_95;
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_64_95 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_64_95 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_64_95 = Temp128.u32_00_31;
    }

    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_96_127 + R5900Regs.Reg[ RT ].i32_96_127;
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_96_127 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_96_127 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_96_127 = Temp128.u32_00_31;
    }
*/
}

void EmuRec_psubsw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_00_31 - R5900Regs.Reg[ RT ].i32_00_31;       
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_00_31 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_00_31 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_00_31 = Temp128.u32_00_31;
    }
    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_32_63 - R5900Regs.Reg[ RT ].i32_32_63;
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_32_63 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_32_63 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_32_63 = Temp128.u32_00_31;
    }
    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_64_95 - R5900Regs.Reg[ RT ].i32_64_95;
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_64_95 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_64_95 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_64_95 = Temp128.u32_00_31;
    }
    Temp128.i64_00_63 = R5900Regs.Reg[ RS ].i32_96_127 - R5900Regs.Reg[ RT ].i32_96_127;
    if ( Temp128.i64_00_63 > 0x7FFFFFFF )
    {
        R5900Regs.Reg[ RD ].u32_96_127 = 0x7FFFFFFF;
    }
    else 
    if ( ( 0x10000000 <= Temp128.i64_00_63 ) && ( Temp128.i64_00_63 < 0x80000000 ) )
    {
        R5900Regs.Reg[ RD ].u32_96_127 = 0x80000000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_96_127 = Temp128.u32_00_31;
    }
*/
}

void EmuRec_pextlw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31  = R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_32_63  = R5900Regs.Reg[ R_RS ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_64_95  = R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_96_127 = R5900Regs.Reg[ R_RS ].u32_32_63;
*/
}

void EmuRec_ppacw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31  = R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_32_63  = R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_64_95  = R5900Regs.Reg[ R_RS ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_96_127 = R5900Regs.Reg[ R_RS ].u32_64_95;
*/
}

void EmuRec_paddsh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_00_15 + R5900Regs.Reg[ RT ].i16_00_15;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_00_15 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_00_15 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_00_15 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_16_31 + R5900Regs.Reg[ RT ].i16_16_31;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_16_31 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_16_31 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_16_31 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_32_47 + R5900Regs.Reg[ RT ].i16_32_47;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].i16_32_47 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_32_47 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_32_47 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_48_63 + R5900Regs.Reg[ RT ].i16_48_63;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_48_63 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_48_63 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_48_63 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_64_79 + R5900Regs.Reg[ RT ].i16_64_79;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_64_79 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_64_79 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_64_79 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_80_95 + R5900Regs.Reg[ RT ].i16_80_95;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_80_95 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_80_95 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_80_95 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_96_111 + R5900Regs.Reg[ RT ].i16_96_111;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_96_111 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_96_111 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_96_111 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_112_127 + R5900Regs.Reg[ RT ].i16_112_127;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_112_127 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_112_127 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_112_127 = Temp128.u16_00_15;
    }
*/
}
        
void EmuRec_psubsh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_00_15 - R5900Regs.Reg[ RT ].i16_00_15;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_00_15 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_00_15 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_00_15 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_16_31 - R5900Regs.Reg[ RT ].i16_16_31;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_16_31 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_16_31 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_16_31 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_32_47 - R5900Regs.Reg[ RT ].i16_32_47;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_32_47 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_32_47 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_32_47 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_48_63 - R5900Regs.Reg[ RT ].i16_48_63;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_48_63 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_48_63 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_48_63 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_64_79 - R5900Regs.Reg[ RT ].i16_64_79;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_64_79 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_64_79 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_64_79 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_80_95 - R5900Regs.Reg[ RT ].i16_80_95;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_80_95 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_80_95 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_80_95 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_96_111 - R5900Regs.Reg[ RT ].i16_96_111;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_96_111 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_96_111 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_96_111 = Temp128.u16_00_15;
    }
    Temp128.i32_00_31 = R5900Regs.Reg[ RS ].i16_112_127 - R5900Regs.Reg[ RT ].i16_112_127;       
    if ( Temp128.i32_00_31 > 0x7FFF )
    {
        R5900Regs.Reg[ RD ].u16_112_127 = 0x7FFF;
    }
    else 
    if ( ( 0x10000 <= Temp128.i32_00_31 ) && ( Temp128.i32_00_31 < 0x18000 ) )
    {
        R5900Regs.Reg[ RD ].u16_112_127 = 0x8000;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_112_127 = Temp128.u16_00_15;
    }
*/
}

void EmuRec_pextlh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_16_31   = R5900Regs.Reg[ RS ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_32_47   = R5900Regs.Reg[ RT ].u16_16_31;
    R5900Regs.Reg[ RD ].u16_48_63   = R5900Regs.Reg[ RS ].u16_16_31;
    R5900Regs.Reg[ RD ].u16_64_79   = R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_80_95   = R5900Regs.Reg[ RS ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_96_111  = R5900Regs.Reg[ RT ].u16_48_63;
    R5900Regs.Reg[ RD ].u16_112_127 = R5900Regs.Reg[ RS ].u16_48_63;
*/
}

void EmuRec_ppach( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_16_31   = R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_32_47   = R5900Regs.Reg[ RT ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_48_63   = R5900Regs.Reg[ RT ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_64_79   = R5900Regs.Reg[ RS ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_80_95   = R5900Regs.Reg[ RS ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_96_111  = R5900Regs.Reg[ RS ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_112_127 = R5900Regs.Reg[ RS ].u16_96_111;
*/
}

/////////////////////////////////////////////////////////////////////////
void EmuRec_paddsb( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempI32 = R5900Regs.Reg[ RS ].i08_00_07 + R5900Regs.Reg[ RT ].i08_00_07;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_00_07 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_00_07 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_00_07 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_08_15 + R5900Regs.Reg[ RT ].i08_08_15;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_08_15 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_08_15 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_08_15 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_16_23 + R5900Regs.Reg[ RT ].i08_16_23;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_16_23 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_16_23 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_16_23 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_24_31 + R5900Regs.Reg[ RT ].i08_24_31;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_24_31 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_24_31 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_24_31 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_32_39 + R5900Regs.Reg[ RT ].i08_32_39;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_32_39 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_32_39 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_32_39 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_40_47 + R5900Regs.Reg[ RT ].i08_40_47;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_40_47 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_40_47 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_40_47 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_48_55 + R5900Regs.Reg[ RT ].i08_48_55;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_48_55 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_48_55 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_48_55 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_56_63 + R5900Regs.Reg[ RT ].i08_56_63;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_56_63 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_56_63 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_56_63 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_64_71 + R5900Regs.Reg[ RT ].i08_64_71;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_64_71 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_64_71 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_64_71 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_72_79 + R5900Regs.Reg[ RT ].i08_72_79;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_72_79 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_72_79 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_72_79 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_80_87 + R5900Regs.Reg[ RT ].i08_80_87;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_80_87 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_80_87 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_80_87 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_88_95 + R5900Regs.Reg[ RT ].i08_88_95;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_88_95 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_88_95 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_88_95 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_96_103 + R5900Regs.Reg[ RT ].i08_96_103;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_96_103 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_96_103 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_96_103 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_104_111 + R5900Regs.Reg[ RT ].i08_104_111;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_104_111 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_104_111 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_104_111 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_112_119 + R5900Regs.Reg[ RT ].i08_112_119;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_112_119 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_112_119 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_112_119 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_120_127 + R5900Regs.Reg[ RT ].i08_120_127;       
    if ( TempI32 > 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_120_127 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_120_127 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_120_127 = TempI32;
    }
*/
}

void EmuRec_psubsb( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempI32 = R5900Regs.Reg[ RS ].i08_00_07 - R5900Regs.Reg[ RT ].i08_00_07;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_00_07 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_00_07 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_00_07 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_08_15 - R5900Regs.Reg[ RT ].i08_08_15;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_08_15 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_08_15 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_08_15 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_16_23 - R5900Regs.Reg[ RT ].i08_16_23;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_16_23 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_16_23 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_16_23 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_24_31 - R5900Regs.Reg[ RT ].i08_24_31;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_24_31 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_24_31 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_24_31 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_32_39 - R5900Regs.Reg[ RT ].i08_32_39;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_32_39 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_32_39 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_32_39 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_40_47 - R5900Regs.Reg[ RT ].i08_40_47;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_40_47 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_40_47 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_40_47 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_48_55 - R5900Regs.Reg[ RT ].i08_48_55;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_48_55 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_48_55 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_48_55 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_56_63 - R5900Regs.Reg[ RT ].i08_56_63;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_56_63 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_56_63 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_56_63 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_64_71 - R5900Regs.Reg[ RT ].i08_64_71;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_64_71 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_64_71 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_64_71 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_72_79 - R5900Regs.Reg[ RT ].i08_72_79;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_72_79 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_72_79 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_72_79 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_80_87 - R5900Regs.Reg[ RT ].i08_80_87;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_80_87 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_80_87 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_80_87 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_88_95 - R5900Regs.Reg[ RT ].i08_88_95;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_88_95 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_88_95 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_88_95 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_96_103 - R5900Regs.Reg[ RT ].i08_96_103;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_96_103 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_96_103 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_96_103 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_104_111 - R5900Regs.Reg[ RT ].i08_104_111;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_104_111 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_104_111 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_104_111 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_112_119 - R5900Regs.Reg[ RT ].i08_112_119;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_112_119 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_112_119 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_112_119 = TempI32;
    }
    TempI32 = R5900Regs.Reg[ RS ].i08_120_127 - R5900Regs.Reg[ RT ].i08_120_127;       
    if ( TempI32 >= 0x7f )
    {
        R5900Regs.Reg[ RD ].i08_120_127 = (EMU_I08)0x7f;
    }
    else if ( 0x100 <= TempI32 && TempI32 < 0x180 )
    {
        R5900Regs.Reg[ RD ].i08_120_127 = (EMU_I08)0x80;
    }
    else
    {
        R5900Regs.Reg[ RD ].i08_120_127 = TempI32;
    }
*/
}

void EmuRec_pextlb( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u08_00_07   = R5900Regs.Reg[ RT ].u08_00_07;
    R5900Regs.Reg[ RD ].u08_08_15   = R5900Regs.Reg[ RS ].u08_00_07;
    R5900Regs.Reg[ RD ].u08_16_23   = R5900Regs.Reg[ RT ].u08_08_15;
    R5900Regs.Reg[ RD ].u08_24_31   = R5900Regs.Reg[ RS ].u08_08_15;
    R5900Regs.Reg[ RD ].u08_32_39   = R5900Regs.Reg[ RT ].u08_16_23;
    R5900Regs.Reg[ RD ].u08_40_47   = R5900Regs.Reg[ RS ].u08_16_23;
    R5900Regs.Reg[ RD ].u08_48_55   = R5900Regs.Reg[ RT ].u08_24_31;
    R5900Regs.Reg[ RD ].u08_56_63   = R5900Regs.Reg[ RS ].u08_24_31;
    R5900Regs.Reg[ RD ].u08_64_71   = R5900Regs.Reg[ RT ].u08_32_39;
    R5900Regs.Reg[ RD ].u08_72_79   = R5900Regs.Reg[ RS ].u08_32_39;
    R5900Regs.Reg[ RD ].u08_80_87   = R5900Regs.Reg[ RT ].u08_40_47;
    R5900Regs.Reg[ RD ].u08_88_95   = R5900Regs.Reg[ RS ].u08_40_47;
    R5900Regs.Reg[ RD ].u08_96_103  = R5900Regs.Reg[ RT ].u08_48_55;
    R5900Regs.Reg[ RD ].u08_104_111 = R5900Regs.Reg[ RS ].u08_48_55;
    R5900Regs.Reg[ RD ].u08_112_119 = R5900Regs.Reg[ RT ].u08_56_63;
    R5900Regs.Reg[ RD ].u08_120_127 = R5900Regs.Reg[ RS ].u08_56_63;
*/
}

void EmuRec_ppacb( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u08_00_07   = R5900Regs.Reg[ RT ].u08_00_07;
    R5900Regs.Reg[ RD ].u08_08_15   = R5900Regs.Reg[ RT ].u08_16_23;
    R5900Regs.Reg[ RD ].u08_16_23   = R5900Regs.Reg[ RT ].u08_32_39;
    R5900Regs.Reg[ RD ].u08_24_31   = R5900Regs.Reg[ RT ].u08_48_55;
    R5900Regs.Reg[ RD ].u08_32_39   = R5900Regs.Reg[ RT ].u08_64_71;
    R5900Regs.Reg[ RD ].u08_40_47   = R5900Regs.Reg[ RT ].u08_80_87;
    R5900Regs.Reg[ RD ].u08_48_55   = R5900Regs.Reg[ RT ].u08_96_103;
    R5900Regs.Reg[ RD ].u08_56_63   = R5900Regs.Reg[ RT ].u08_112_119;
    R5900Regs.Reg[ RD ].u08_64_71   = R5900Regs.Reg[ RS ].u08_00_07;
    R5900Regs.Reg[ RD ].u08_72_79   = R5900Regs.Reg[ RS ].u08_16_23;
    R5900Regs.Reg[ RD ].u08_80_87   = R5900Regs.Reg[ RS ].u08_32_39;
    R5900Regs.Reg[ RD ].u08_88_95   = R5900Regs.Reg[ RS ].u08_48_55;
    R5900Regs.Reg[ RD ].u08_96_103  = R5900Regs.Reg[ RS ].u08_64_71;
    R5900Regs.Reg[ RD ].u08_104_111 = R5900Regs.Reg[ RS ].u08_80_87;
    R5900Regs.Reg[ RD ].u08_112_119 = R5900Regs.Reg[ RS ].u08_96_103;
    R5900Regs.Reg[ RD ].u08_120_127 = R5900Regs.Reg[ RS ].u08_112_119;
*/
}

void EmuRec_pext5( EMU_U32 Code )
{
/*
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u32_00_31 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) | 
        ((TempU32 & 0x03e0) <<  6) | ((TempU32 & 0x001f) << 3);
    TempU32 = R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u32_32_63 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) | 
        ((TempU32 & 0x03e0) <<  6) | ((TempU32 & 0x001f) << 3);
    TempU32 = R5900Regs.Reg[ RT ].u16_64_79;
    R5900Regs.Reg[ RD ].u32_64_95 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) | 
        ((TempU32 & 0x03e0) <<  6) | ((TempU32 & 0x001f) << 3);
    TempU32 = R5900Regs.Reg[ RT ].u16_96_111;
    R5900Regs.Reg[ RD ].u32_96_127 = ((TempU32 & 0x8000) << 16) | ((TempU32 & 0x7c00) << 9) | 
        ((TempU32 & 0x03e0) <<  6) | ((TempU32 & 0x001f) << 3);
*/
}

void EmuRec_ppac5( EMU_U32 Code )
{
/*
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RT ].u32_00_31;
    R5900Regs.Reg[ RD ].u32_00_31 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) | 
        ((TempU32 & 0x0000f800) >>  6) | ((TempU32 & 0x000000f8) >> 3);
    TempU32 = R5900Regs.Reg[ RT ].u32_32_63;
    R5900Regs.Reg[ RD ].u32_32_63 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) | 
        ((TempU32 & 0x0000f800) >>  6) | ((TempU32 & 0x000000f8) >> 3);
    TempU32 = R5900Regs.Reg[ RT ].u32_64_95;
    R5900Regs.Reg[ RD ].u32_64_95 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) | 
        ((TempU32 & 0x0000f800) >>  6) | ((TempU32 & 0x000000f8) >> 3);
    TempU32 = R5900Regs.Reg[ RT ].u32_96_127;
    R5900Regs.Reg[ RD ].u32_96_127 = ((TempU32 & 0x80000000) >> 16) | ((TempU32 & 0x00f80000) >> 9) | 
        ((TempU32 & 0x0000f800) >>  6) | ((TempU32 & 0x000000f8) >> 3);
*/
}

void EmuRec_pmaddw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempI64 = ( ( R5900Regs.HI.u32_00_31 << 32) | R5900Regs.LO.u32_00_31 ) + 
        ( R5900Regs.Reg[ RS ].i32_00_31 * R5900Regs.Reg[ RT ].i32_00_31 );
    R5900Regs.LO.u64_00_63 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_00_63 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    R5900Regs.Reg[ RD ].u64_00_63 = TempI64; 
    TempI64 = ( ( R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95 ) + 
        ( R5900Regs.Reg[ RS ].i32_64_95 * R5900Regs.Reg[ RT ].i32_64_95 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    R5900Regs.Reg[ RD ].u64_64_127 = TempI64; 
*/
}

void EmuRec_psllvw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u64_00_63   = (EMU_I32)( R5900Regs.Reg[ RT ].u32_00_31 << 
        ( R5900Regs.Reg[ RS ].u32_00_31 & 0x1F ) );
    R5900Regs.Reg[ RD ].u64_64_127  = (EMU_I32)( R5900Regs.Reg[ RT ].u32_64_95 << 
        ( R5900Regs.Reg[ RS ].u32_64_95 & 0x1F ) );
*/
}

void EmuRec_psrlvw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ R_RT ].u32_00_31 >> 
                                               ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F ) );
    R5900Regs.Reg[ R_RD ].u64_64_127 = (EMU_I32)( R5900Regs.Reg[ R_RT ].u32_64_95 >> 
                                                ( R5900Regs.Reg[ R_RS ].u32_64_95 & 0x1F ) );
*/
}

void EmuRec_pmsubw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempI64 = ( ( R5900Regs.HI.u32_00_31 << 32 ) | R5900Regs.LO.u32_00_31 ) - 
        ( R5900Regs.Reg[ RS ].i32_00_31 * R5900Regs.Reg[ RT ].i32_00_31 );
    R5900Regs.LO.u64_00_63 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_00_63 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    R5900Regs.Reg[ RD ].u64_00_63 = TempI64; 
    TempI64 = ( ( R5900Regs.HI.u32_64_95 << 32 ) | R5900Regs.LO.u32_64_95 ) - 
        ( R5900Regs.Reg[ RS ].i32_64_95 * R5900Regs.Reg[ RT ].i32_64_95 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    R5900Regs.Reg[ RD ].u64_64_127 = TempI64; 
*/
}

void EmuRec_pmfhi( EMU_U32 Code )
{
/*
    RD = R_RD;
    R5900Regs.Reg[ RD ].u64_00_63   = R5900Regs.HI.u64_00_63;
    R5900Regs.Reg[ RD ].u64_64_127  = R5900Regs.HI.u64_64_127;
*/
}

void EmuRec_pmflo( EMU_U32 Code )
{
/*
    RD = R_RD;
    R5900Regs.Reg[ RD ].u64_00_63   = R5900Regs.LO.u64_00_63;
    R5900Regs.Reg[ RD ].u64_64_127  = R5900Regs.LO.u64_64_127;
*/
}

void EmuRec_pinth( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_16_31   = R5900Regs.Reg[ RS ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_32_47   = R5900Regs.Reg[ RT ].u16_16_31;
    R5900Regs.Reg[ RD ].u16_48_63   = R5900Regs.Reg[ RS ].u16_80_95;
    R5900Regs.Reg[ RD ].u16_64_79   = R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_80_95   = R5900Regs.Reg[ RS ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_96_111  = R5900Regs.Reg[ RT ].u16_48_63;
    R5900Regs.Reg[ RD ].u16_112_127 = R5900Regs.Reg[ RS ].u16_112_127;
*/
}

void EmuRec_pmultw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempI64 = R5900Regs.Reg[ RS ].i32_00_31 * R5900Regs.Reg[ RT ].i32_00_31;
    R5900Regs.LO.u64_00_63 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_00_63 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    if ( RD )
    {
        R5900Regs.Reg[ RD ].u64_00_63 = R5900Regs.LO.u64_00_63; 
    }
    TempI64 = R5900Regs.Reg[ RS ].i32_64_95 * R5900Regs.Reg[ RT ].i32_64_95;
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    if ( RD )
    {
        R5900Regs.Reg[ RD ].u64_64_127 = R5900Regs.LO.u64_64_127; 
    }
*/
}

void EmuRec_pdivw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    R5900Regs.LO.u64_00_63 = (EMU_I32)( R5900Regs.Reg[ RS ].i32_00_31 /
        R5900Regs.Reg[ RT ].i32_00_31 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( R5900Regs.Reg[ RS ].i32_64_95 /
        R5900Regs.Reg[ RT ].i32_64_95 );
    R5900Regs.HI.u64_00_63 = (EMU_I32)( R5900Regs.Reg[ RS ].i32_00_31 %
        R5900Regs.Reg[ RT ].i32_00_31 );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( R5900Regs.Reg[ RS ].i32_64_95 %
        R5900Regs.Reg[ RT ].i32_64_95 );
*/
}

void EmuRec_pcpyld( EMU_U32 Code )
{
/*
    RD = R_RD;
    R5900Regs.Reg[ RD ].u64_00_63   = R5900Regs.Reg[ R_RT ].u64_00_63;
    R5900Regs.Reg[ RD ].u64_64_127  = R5900Regs.Reg[ R_RS ].u64_00_63;
*/
}

void EmuRec_pmaddh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.LO.u32_00_31 = (EMU_I32)( R5900Regs.LO.u32_00_31 + 
        ( R5900Regs.Reg[ RS ].i16_00_15 * R5900Regs.Reg[ RT ].i16_00_15 ) );
    R5900Regs.Reg[ RD ].u32_00_31 = R5900Regs.LO.u32_00_31;
    R5900Regs.LO.u32_32_63 = (EMU_I32)( R5900Regs.LO.u32_32_63 + 
        ( R5900Regs.Reg[ RS ].i16_16_31 * R5900Regs.Reg[ RT ].i16_16_31 ) );
    R5900Regs.HI.u32_00_31 = (EMU_I32)( R5900Regs.HI.u32_00_31 + 
        ( R5900Regs.Reg[ RS ].i16_32_47 * R5900Regs.Reg[ RT ].i16_32_47 ) );
    R5900Regs.Reg[ RD ].u32_32_63 = R5900Regs.HI.u32_00_31;
    R5900Regs.HI.u32_32_63 = (EMU_I32)( R5900Regs.HI.u32_32_63 + 
        ( R5900Regs.Reg[ RS ].i16_48_63 * R5900Regs.Reg[ RT ].i16_48_63 ) );
    R5900Regs.LO.u32_64_95 = (EMU_I32)( R5900Regs.LO.u32_64_95 + 
        ( R5900Regs.Reg[ RS ].i16_64_79 * R5900Regs.Reg[ RT ].i16_64_79 ) );
    R5900Regs.Reg[ RD ].u32_64_95 = R5900Regs.LO.u32_64_95;
    R5900Regs.LO.u32_96_127 = (EMU_I32)( R5900Regs.LO.u32_96_127 + 
        ( R5900Regs.Reg[ RS ].i16_80_95 * R5900Regs.Reg[ RT ].i16_80_95 ) );
    R5900Regs.HI.u32_64_95 = (EMU_I32)( R5900Regs.HI.u32_64_95 + 
        ( R5900Regs.Reg[ RS ].i16_96_111 * R5900Regs.Reg[ RT ].i16_96_111 ) );
    R5900Regs.Reg[ RD ].u32_96_127 = R5900Regs.HI.u32_64_95;
    R5900Regs.HI.u32_96_127 = (EMU_I32)( R5900Regs.HI.u32_96_127 + 
        ( R5900Regs.Reg[ RS ].i16_112_127 * R5900Regs.Reg[ RT ].i16_112_127 ) );
*/
}

void EmuRec_phmadh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.LO.u32_00_31 = (EMU_I32)(
        ( R5900Regs.Reg[ RS ].i16_16_31 * R5900Regs.Reg[ RT ].i16_16_31 ) + 
        ( R5900Regs.Reg[ RS ].i16_00_15 * R5900Regs.Reg[ RT ].i16_00_15 ) );
    R5900Regs.Reg[ RD ].u32_00_31 = R5900Regs.LO.u32_00_31;
    R5900Regs.HI.u32_00_31 = (EMU_I32)( 
        ( R5900Regs.Reg[ RS ].i16_48_63 * R5900Regs.Reg[ RT ].i16_48_63 ) + 
        ( R5900Regs.Reg[ RS ].i16_32_47 * R5900Regs.Reg[ RT ].i16_32_47 ) );
    R5900Regs.Reg[ RD ].u32_32_63 = R5900Regs.HI.u32_00_31;
    R5900Regs.LO.u32_64_95 = (EMU_I32)( 
        ( R5900Regs.Reg[ RS ].i16_80_95 * R5900Regs.Reg[ RT ].i16_80_95 ) + 
        ( R5900Regs.Reg[ RS ].i16_64_79 * R5900Regs.Reg[ RT ].i16_64_79 ) );
    R5900Regs.Reg[ RD ].u32_64_95 = R5900Regs.LO.u32_64_95;
    R5900Regs.HI.u32_64_95 = (EMU_I32)( 
        ( R5900Regs.Reg[ RS ].i16_112_127 * R5900Regs.Reg[ RT ].i16_112_127 ) + 
        ( R5900Regs.Reg[ RS ].i16_96_111 * R5900Regs.Reg[ RT ].i16_96_111 ) );
    R5900Regs.Reg[ RD ].u32_96_127 = R5900Regs.HI.u32_64_95;
*/
}

void EmuRec_pand( EMU_U32 Code ) 
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u64_00_63  = R5900Regs.Reg[ RS ].u64_00_63 & 
        R5900Regs.Reg[ RT ].u64_00_63;
    R5900Regs.Reg[ RD ].u64_64_127 = R5900Regs.Reg[ RS ].u64_64_127 & 
        R5900Regs.Reg[ RT ].u64_64_127;
*/
}

void EmuRec_pxor( EMU_U32 Code ) 
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u64_00_63  = R5900Regs.Reg[ RS ].u64_00_63 ^ 
        R5900Regs.Reg[ RT ].u64_00_63;
    R5900Regs.Reg[ RD ].u64_64_127 = R5900Regs.Reg[ RS ].u64_64_127 ^ 
        R5900Regs.Reg[ RT ].u64_64_127;
*/
}

void EmuRec_pmsubh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.LO.u32_00_31 = (EMU_I32)( R5900Regs.LO.u32_00_31 - 
        ( R5900Regs.Reg[ RS ].i16_00_15 * R5900Regs.Reg[ RT ].i16_00_15 ) );
    R5900Regs.Reg[ RD ].u32_00_31 = R5900Regs.LO.u32_00_31;
    R5900Regs.LO.u32_32_63 = (EMU_I32)( R5900Regs.LO.u32_32_63 - 
        ( R5900Regs.Reg[ RS ].i16_16_31 * R5900Regs.Reg[ RT ].i16_16_31 ) );
    R5900Regs.HI.u32_00_31 = (EMU_I32)( R5900Regs.HI.u32_00_31 - 
        ( R5900Regs.Reg[ RS ].i16_32_47 * R5900Regs.Reg[ RT ].i16_32_47 ) );
    R5900Regs.Reg[ RD ].u32_32_63 = R5900Regs.HI.u32_00_31;
    R5900Regs.HI.u32_32_63 = (EMU_I32)( R5900Regs.HI.u32_32_63 - 
        ( R5900Regs.Reg[ RS ].i16_48_63 * R5900Regs.Reg[ RT ].i16_48_63 ) );
    R5900Regs.LO.u32_64_95 = (EMU_I32)( R5900Regs.LO.u32_64_95 - 
        ( R5900Regs.Reg[ RS ].i16_64_79 * R5900Regs.Reg[ RT ].i16_64_79 ) );
    R5900Regs.Reg[ RD ].u32_64_95 = R5900Regs.LO.u32_64_95;
    R5900Regs.LO.u32_96_127 = (EMU_I32)( R5900Regs.LO.u32_96_127 - 
        ( R5900Regs.Reg[ RS ].i16_80_95 * R5900Regs.Reg[ RT ].i16_80_95 ) );
    R5900Regs.HI.u32_64_95 = (EMU_I32)( R5900Regs.HI.u32_64_95 - 
        ( R5900Regs.Reg[ RS ].i16_96_111 * R5900Regs.Reg[ RT ].i16_96_111 ) );
    R5900Regs.Reg[ RD ].u32_96_127 = R5900Regs.HI.u32_64_95;
    R5900Regs.HI.u32_96_127 = (EMU_I32)( R5900Regs.HI.u32_96_127 - 
        ( R5900Regs.Reg[ RS ].i16_112_127 * R5900Regs.Reg[ RT ].i16_112_127 ) );
*/
}

void EmuRec_phmsbh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.LO.u32_00_31 = (EMU_I32)( 
        ( R5900Regs.Reg[ RS ].i16_16_31 * R5900Regs.Reg[ RT ].i16_16_31 ) - 
        ( R5900Regs.Reg[ RS ].i16_00_15 * R5900Regs.Reg[ RT ].i16_00_15 ) );
    R5900Regs.Reg[ RD ].u32_00_31 = R5900Regs.LO.u32_00_31;
    R5900Regs.HI.u32_00_31 = (EMU_I32)(
        ( R5900Regs.Reg[ RS ].i16_48_63 * R5900Regs.Reg[ RT ].i16_48_63 ) - 
        ( R5900Regs.Reg[ RS ].i16_32_47 * R5900Regs.Reg[ RT ].i16_32_47 ) );
    R5900Regs.Reg[ RD ].u32_32_63 = R5900Regs.HI.u32_00_31;
    R5900Regs.LO.u32_64_95 = (EMU_I32)(
        ( R5900Regs.Reg[ RS ].i16_80_95 * R5900Regs.Reg[ RT ].i16_80_95 ) - 
        ( R5900Regs.Reg[ RS ].i16_64_79 * R5900Regs.Reg[ RT ].i16_64_79 ) );
    R5900Regs.Reg[ RD ].u32_64_95 = R5900Regs.LO.u32_64_95;
    R5900Regs.HI.u32_64_95 = (EMU_I32)( 
        ( R5900Regs.Reg[ RS ].i16_112_127 * R5900Regs.Reg[ RT ].i16_112_127 ) - 
        ( R5900Regs.Reg[ RS ].i16_96_111 * R5900Regs.Reg[ RT ].i16_96_111 ) );
    R5900Regs.Reg[ RD ].u32_96_127 = R5900Regs.HI.u32_64_95;
*/
}

void EmuRec_pexeh( EMU_U32 Code )
{
/*
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_16_31   = R5900Regs.Reg[ RT ].u16_16_31;
    R5900Regs.Reg[ RD ].u16_32_47   = R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_48_63   = R5900Regs.Reg[ RT ].u16_48_63;
    R5900Regs.Reg[ RD ].u16_64_79   = R5900Regs.Reg[ RT ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_80_95   = R5900Regs.Reg[ RT ].u16_80_95;
    R5900Regs.Reg[ RD ].u16_96_111  = R5900Regs.Reg[ RT ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_112_127 = R5900Regs.Reg[ RT ].u16_112_127;
*/
}

void EmuRec_prevh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].u16_48_63;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RT ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].u16_16_31;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].u16_112_127;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RT ].u16_96_111;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].u16_80_95;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RT ].u16_64_79;
*/
}

void EmuRec_pmulth( EMU_U32 Code )
{
/*
    R5900Regs.LO.u32_00_31  = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_00_15 *
                                         R5900Regs.Reg[ R_RT ].i16_00_15 );
    R5900Regs.Reg[ R_RD ].u32_00_31 = R5900Regs.LO.u32_00_31;
    R5900Regs.LO.u32_32_63  = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_16_31 *
                                         R5900Regs.Reg[ R_RT ].i16_16_31 );
    R5900Regs.HI.u32_00_31  = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_32_47 *
                                         R5900Regs.Reg[ R_RT ].i16_32_47 );
    R5900Regs.Reg[ R_RD ].u32_32_63 = R5900Regs.HI.u32_00_31;
    R5900Regs.HI.u32_32_63  = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_48_63 *
                                         R5900Regs.Reg[ R_RT ].i16_48_63 );
    R5900Regs.LO.u32_64_95  = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_64_79 *
                                         R5900Regs.Reg[ R_RT ].i16_64_79 );
    R5900Regs.Reg[ R_RD ].u32_64_95 = R5900Regs.LO.u32_64_95;
    R5900Regs.LO.u32_96_127 = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_80_95 *
                                         R5900Regs.Reg[ R_RT ].i16_80_95 );
    R5900Regs.HI.u32_64_95  = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_96_111 *
                                         R5900Regs.Reg[ R_RT ].i16_96_111 );
    R5900Regs.Reg[ R_RD ].u32_96_127 = R5900Regs.HI.u32_64_95;
    R5900Regs.HI.u32_96_127 = (EMU_I16)( R5900Regs.Reg[ R_RS ].i16_112_127 *
                                         R5900Regs.Reg[ R_RT ].u16_112_127 );
*/
}

void EmuRec_pdivbw( EMU_U32 Code )
{
/*
    R5900Regs.LO.i32_00_31  = R5900Regs.Reg[ R_RS ].i32_00_31  / R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.LO.i32_32_63  = R5900Regs.Reg[ R_RS ].i32_32_63  / R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.LO.i32_64_95  = R5900Regs.Reg[ R_RS ].i32_64_95  / R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.LO.i32_96_127 = R5900Regs.Reg[ R_RS ].i32_96_127 / R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.HI.i32_00_31  = R5900Regs.Reg[ R_RS ].i32_00_31  % R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.HI.i32_32_63  = R5900Regs.Reg[ R_RS ].i32_32_63  % R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.HI.i32_64_95  = R5900Regs.Reg[ R_RS ].i32_64_95  % R5900Regs.Reg[ R_RT ].i16_00_15;
    R5900Regs.HI.i32_96_127 = R5900Regs.Reg[ R_RS ].i32_96_127 % R5900Regs.Reg[ R_RT ].i16_00_15;
*/
}

void EmuRec_pexew( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.Reg[ R_RT ].u32_96_127;
*/
}

void EmuRec_prot3w( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.Reg[ R_RT ].u32_96_127;
*/
}

void EmuRec_mfhi1( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.HI.u64_64_127;
*/
}

void EmuRec_mthi1( EMU_U32 Code )
{
/*
    R5900Regs.HI.u64_64_127 = R5900Regs.Reg[ R_RS ].u64_00_63;
*/
}

void EmuRec_mflo1( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.LO.u64_64_127;
*/
}

void EmuRec_mtlo1( EMU_U32 Code )
{
/*
    R5900Regs.LO.u64_64_127 = R5900Regs.Reg[ R_RS ].u64_00_63;
*/
}

void EmuRec_mult1( EMU_U32 Code )
{
/*
    TempI64 = (EMU_I64)R5900Regs.Reg[ R_RS ].i32_00_31 * (EMU_I64)R5900Regs.Reg[ R_RT ].i32_00_31;
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( TempI64 >> 32 );
    if ( R_RD )
    {
        R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.LO.u64_64_127; 
    }
*/
}

void EmuRec_multu1( EMU_U32 Code )
{
/*
    TempU64 = (EMU_U64)R5900Regs.Reg[ R_RS ].u32_00_31 * (EMU_U64)R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempU64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( TempU64 >> 32 );
    if ( R_RD )
    {
        R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.LO.u64_64_127; 
    }
*/
}

void EmuRec_div1( EMU_U32 Code )
{
/*
    R5900Regs.LO.u64_64_127 =   R5900Regs.Reg[ R_RS ].i32_00_31 / R5900Regs.Reg[ R_RT ].i32_00_31;
    R5900Regs.HI.u64_64_127 =   R5900Regs.Reg[ R_RS ].i32_00_31 % R5900Regs.Reg[ R_RT ].i32_00_31;
*/
}

void EmuRec_divu1( EMU_U32 Code )
{
/*
    R5900Regs.LO.u64_64_127 = (EMU_I32)( R5900Regs.Reg[ R_RS ].u32_00_31 / R5900Regs.Reg[ R_RT ].u32_00_31 );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( R5900Regs.Reg[ R_RS ].u32_00_31 % R5900Regs.Reg[ R_RT ].u32_00_31 );
*/
}

void EmuRec_madd1( EMU_U32 Code )
{
/*
    TempI64 = ( ( R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95 ) +  
        ( R5900Regs.Reg[ R_RS ].i32_00_31 * R5900Regs.Reg[ R_RT ].i32_00_31 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempI64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( ( TempI64 >> 32 ) & 0x00000000FFFFFFFF );
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.LO.u64_64_127; 
*/
}

void EmuRec_maddu1( EMU_U32 Code )
{
/*
    TempU64 = ( ( R5900Regs.HI.u32_64_95 << 32) | R5900Regs.LO.u32_64_95 ) + 
                ( R5900Regs.Reg[ R_RS ].u32_00_31 * R5900Regs.Reg[ R_RT ].u32_00_31 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempU64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( TempU64 >> 32 );
    R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.LO.u64_64_127; 
*/
}

void EmuRec_pabsw( EMU_U32 Code )
{
/*
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u32_00_31   = abs( R5900Regs.Reg[ RT ].u32_00_31 );
    R5900Regs.Reg[ RD ].u32_32_63   = abs( R5900Regs.Reg[ RT ].u32_32_63 );
    R5900Regs.Reg[ RD ].u32_64_95   = abs( R5900Regs.Reg[ RT ].u32_64_95 );
    R5900Regs.Reg[ RD ].u32_96_127  = abs( R5900Regs.Reg[ RT ].u32_96_127 );
*/
}

void EmuRec_pceqw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u32_00_31  = ( R5900Regs.Reg[ RS ].u32_00_31  == R5900Regs.Reg[ RT ].u32_00_31  ) ? 0xFFFFFFFF : 0;
    R5900Regs.Reg[ RD ].u32_32_63  = ( R5900Regs.Reg[ RS ].u32_32_63  == R5900Regs.Reg[ RT ].u32_32_63  ) ? 0xFFFFFFFF : 0;
    R5900Regs.Reg[ RD ].u32_64_95  = ( R5900Regs.Reg[ RS ].u32_64_95  == R5900Regs.Reg[ RT ].u32_64_95  ) ? 0xFFFFFFFF : 0;
    R5900Regs.Reg[ RD ].u32_96_127 = ( R5900Regs.Reg[ RS ].u32_96_127 == R5900Regs.Reg[ RT ].u32_96_127 ) ? 0xFFFFFFFF : 0;
*/
}

void EmuRec_pminw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u32_00_31  = ( R5900Regs.Reg[ RS ].i32_00_31  < R5900Regs.Reg[ RT ].i32_00_31  ) ?
        R5900Regs.Reg[ RS ].u32_00_31  : R5900Regs.Reg[ RT ].u32_00_31;
    R5900Regs.Reg[ RD ].u32_32_63  = ( R5900Regs.Reg[ RS ].i32_32_63  < R5900Regs.Reg[ RT ].i32_32_63  ) ?
        R5900Regs.Reg[ RS ].u32_32_63  : R5900Regs.Reg[ RT ].u32_32_63;
    R5900Regs.Reg[ RD ].u32_64_95  = ( R5900Regs.Reg[ RS ].i32_64_95  < R5900Regs.Reg[ RT ].i32_64_95  ) ?
        R5900Regs.Reg[ RS ].u32_64_95  : R5900Regs.Reg[ RT ].u32_64_95;
    R5900Regs.Reg[ RD ].u32_96_127 = ( R5900Regs.Reg[ RS ].i32_96_127 < R5900Regs.Reg[ RT ].i32_96_127 ) ?
        R5900Regs.Reg[ RS ].u32_96_127 : R5900Regs.Reg[ RT ].u32_96_127;
*/
}

void EmuRec_padsbh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = R5900Regs.Reg[ RS ].u16_00_15   - R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_16_31   = R5900Regs.Reg[ RS ].u16_16_31   - R5900Regs.Reg[ RT ].u16_16_31;
    R5900Regs.Reg[ RD ].u16_32_47   = R5900Regs.Reg[ RS ].u16_32_47   - R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_48_63   = R5900Regs.Reg[ RS ].u16_48_63   - R5900Regs.Reg[ RT ].u16_48_63;
    R5900Regs.Reg[ RD ].u16_64_79   = R5900Regs.Reg[ RS ].u16_64_79   + R5900Regs.Reg[ RT ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_80_95   = R5900Regs.Reg[ RS ].u16_80_95   + R5900Regs.Reg[ RT ].u16_80_95;
    R5900Regs.Reg[ RD ].u16_96_111  = R5900Regs.Reg[ RS ].u16_96_111  + R5900Regs.Reg[ RT ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_112_127 = R5900Regs.Reg[ RS ].u16_112_127 + R5900Regs.Reg[ RT ].u16_112_127;
*/
}

void EmuRec_pabsh( EMU_U32 Code )
{
/*
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = abs( R5900Regs.Reg[ RT ].u16_00_15 );
    R5900Regs.Reg[ RD ].u16_16_31   = abs( R5900Regs.Reg[ RT ].u16_16_31 );
    R5900Regs.Reg[ RD ].u16_32_47   = abs( R5900Regs.Reg[ RT ].u16_32_47 );
    R5900Regs.Reg[ RD ].u16_48_63   = abs( R5900Regs.Reg[ RT ].u16_48_63 );
    R5900Regs.Reg[ RD ].u16_64_79   = abs( R5900Regs.Reg[ RT ].u16_64_79 );
    R5900Regs.Reg[ RD ].u16_80_95   = abs( R5900Regs.Reg[ RT ].u16_80_95 );
    R5900Regs.Reg[ RD ].u16_96_111  = abs( R5900Regs.Reg[ RT ].u16_96_111 );
    R5900Regs.Reg[ RD ].u16_112_127 = abs( R5900Regs.Reg[ RT ].u16_112_127 );
*/
}

void EmuRec_pceqh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = ( R5900Regs.Reg[ RS ].u16_00_15   == R5900Regs.Reg[ RT ].u16_00_15   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_16_31   = ( R5900Regs.Reg[ RS ].u16_16_31   == R5900Regs.Reg[ RT ].u16_16_31   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_32_47   = ( R5900Regs.Reg[ RS ].u16_32_47   == R5900Regs.Reg[ RT ].u16_32_47   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_48_63   = ( R5900Regs.Reg[ RS ].u16_48_63   == R5900Regs.Reg[ RT ].u16_48_63   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_64_79   = ( R5900Regs.Reg[ RS ].u16_64_79   == R5900Regs.Reg[ RT ].u16_64_79   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_80_95   = ( R5900Regs.Reg[ RS ].u16_80_95   == R5900Regs.Reg[ RT ].u16_80_95   ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_96_111  = ( R5900Regs.Reg[ RS ].u16_96_111  == R5900Regs.Reg[ RT ].u16_96_111  ) ? 0xFFFF : 0;
    R5900Regs.Reg[ RD ].u16_112_127 = ( R5900Regs.Reg[ RS ].u16_112_127 == R5900Regs.Reg[ RT ].u16_112_127 ) ? 0xFFFF : 0;
*/
}

void EmuRec_pminh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = ( R5900Regs.Reg[ RS ].i16_00_15   < R5900Regs.Reg[ RT ].i16_00_15   ) ?
        R5900Regs.Reg[ RS ].u16_00_15   : R5900Regs.Reg[ RT ].u16_00_15;
    R5900Regs.Reg[ RD ].u16_16_31   = ( R5900Regs.Reg[ RS ].i16_16_31   < R5900Regs.Reg[ RT ].i16_16_31   ) ?
        R5900Regs.Reg[ RS ].u16_16_31   : R5900Regs.Reg[ RT ].u16_16_31;
    R5900Regs.Reg[ RD ].u16_32_47   = ( R5900Regs.Reg[ RS ].i16_32_47   < R5900Regs.Reg[ RT ].i16_32_47   ) ?
        R5900Regs.Reg[ RS ].u16_32_47   : R5900Regs.Reg[ RT ].u16_32_47;
    R5900Regs.Reg[ RD ].u16_48_63   = ( R5900Regs.Reg[ RS ].i16_48_63   < R5900Regs.Reg[ RT ].i16_48_63   ) ?
        R5900Regs.Reg[ RS ].u16_48_63   : R5900Regs.Reg[ RT ].u16_48_63;
    R5900Regs.Reg[ RD ].u16_64_79   = ( R5900Regs.Reg[ RS ].i16_64_79   < R5900Regs.Reg[ RT ].i16_64_79   ) ?
        R5900Regs.Reg[ RS ].u16_64_79   : R5900Regs.Reg[ RT ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_80_95   = ( R5900Regs.Reg[ RS ].i16_80_95   < R5900Regs.Reg[ RT ].i16_80_95   ) ?
        R5900Regs.Reg[ RS ].u16_80_95   : R5900Regs.Reg[ RT ].u16_80_95;
    R5900Regs.Reg[ RD ].u16_96_111  = ( R5900Regs.Reg[ RS ].i16_96_111  < R5900Regs.Reg[ RT ].i16_96_111  ) ?
        R5900Regs.Reg[ RS ].u16_96_111  : R5900Regs.Reg[ RT ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_112_127 = ( R5900Regs.Reg[ RS ].i16_112_127 < R5900Regs.Reg[ RT ].i16_112_127 ) ?
        R5900Regs.Reg[ RS ].u16_112_127 : R5900Regs.Reg[ RT ].u16_112_127;
*/
}

void EmuRec_pceqb( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u08_00_07   = ( R5900Regs.Reg[ RS ].u08_00_07   == R5900Regs.Reg[ RT ].u08_00_07   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_08_15   = ( R5900Regs.Reg[ RS ].u08_08_15   == R5900Regs.Reg[ RT ].u08_08_15   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_16_23   = ( R5900Regs.Reg[ RS ].u08_16_23   == R5900Regs.Reg[ RT ].u08_16_23   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_24_31   = ( R5900Regs.Reg[ RS ].u08_24_31   == R5900Regs.Reg[ RT ].u08_24_31   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_32_39   = ( R5900Regs.Reg[ RS ].u08_32_39   == R5900Regs.Reg[ RT ].u08_32_39   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_40_47   = ( R5900Regs.Reg[ RS ].u08_40_47   == R5900Regs.Reg[ RT ].u08_40_47   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_48_55   = ( R5900Regs.Reg[ RS ].u08_48_55   == R5900Regs.Reg[ RT ].u08_48_55   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_56_63   = ( R5900Regs.Reg[ RS ].u08_56_63   == R5900Regs.Reg[ RT ].u08_56_63   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_64_71   = ( R5900Regs.Reg[ RS ].u08_64_71   == R5900Regs.Reg[ RT ].u08_64_71   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_72_79   = ( R5900Regs.Reg[ RS ].u08_72_79   == R5900Regs.Reg[ RT ].u08_72_79   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_80_87   = ( R5900Regs.Reg[ RS ].u08_80_87   == R5900Regs.Reg[ RT ].u08_80_87   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_88_95   = ( R5900Regs.Reg[ RS ].u08_88_95   == R5900Regs.Reg[ RT ].u08_88_95   ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_96_103  = ( R5900Regs.Reg[ RS ].u08_96_103  == R5900Regs.Reg[ RT ].u08_96_103  ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_104_111 = ( R5900Regs.Reg[ RS ].u08_104_111 == R5900Regs.Reg[ RT ].u08_104_111 ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_112_119 = ( R5900Regs.Reg[ RS ].u08_112_119 == R5900Regs.Reg[ RT ].u08_112_119 ) ? 0xFF : 0;
    R5900Regs.Reg[ RD ].u08_120_127 = ( R5900Regs.Reg[ RS ].u08_120_127 == R5900Regs.Reg[ RT ].u08_120_127 ) ? 0xFF : 0;
*/
}

void EmuRec_padduw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RS ].u32_00_31 + R5900Regs.Reg[ RT ].u32_00_31;       
    if ( TempU32 > 0xffffffff )
    {
        R5900Regs.Reg[ RD ].u32_00_31 = 0xffffffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_00_31 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u32_32_63 + R5900Regs.Reg[ RT ].u32_32_63;       
    if ( TempU32 > 0xffffffff )
    {
        R5900Regs.Reg[ RD ].u32_32_63 = 0xffffffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_32_63 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u32_64_95 + R5900Regs.Reg[ RT ].u32_64_95;       
    if ( TempU32 > 0xffffffff )
    {
        R5900Regs.Reg[ RD ].u32_64_95 = 0xffffffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_64_95 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u32_96_127 + R5900Regs.Reg[ RT ].u32_96_127;       
    if ( TempU32 > 0xffffffff )
    {
        R5900Regs.Reg[ RD ].u32_96_127 = 0xffffffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_96_127 = TempU32;
    }
*/
}

void EmuRec_psubuw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RS ].u32_00_31 - R5900Regs.Reg[ RT ].u32_00_31;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u32_00_31 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_00_31 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u32_32_63 - R5900Regs.Reg[ RT ].u32_32_63;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u32_32_63 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_32_63 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u32_64_95 - R5900Regs.Reg[ RT ].u32_64_95;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u32_64_95 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_64_95 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u32_96_127 - R5900Regs.Reg[ RT ].u32_96_127;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u32_96_127 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u32_96_127 = TempU32;
    }
*/
}

void EmuRec_pextuw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u32_00_31   = R5900Regs.Reg[ RT ].u32_64_95;
    R5900Regs.Reg[ RD ].u32_32_63   = R5900Regs.Reg[ RS ].u32_64_95;
    R5900Regs.Reg[ RD ].u32_64_95   = R5900Regs.Reg[ RT ].u32_96_127;
    R5900Regs.Reg[ RD ].u32_96_127  = R5900Regs.Reg[ RS ].u32_96_127;
*/
}

void EmuRec_padduh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RS ].u16_00_15 + R5900Regs.Reg[ RT ].u16_00_15;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_00_15 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_00_15 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_16_31 + R5900Regs.Reg[ RT ].u16_16_31;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_16_31 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_16_31 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_32_47 + R5900Regs.Reg[ RT ].u16_32_47;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_32_47 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_32_47 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_48_63 + R5900Regs.Reg[ RT ].u16_48_63;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_48_63 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_48_63 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_64_79 + R5900Regs.Reg[ RT ].u16_64_79;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_64_79 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_64_79 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_80_95 + R5900Regs.Reg[ RT ].u16_80_95;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_80_95 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_80_95 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_96_111 + R5900Regs.Reg[ RT ].u16_96_111;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_96_111 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_96_111 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_112_127 + R5900Regs.Reg[ RT ].u16_112_127;       
    if ( TempU32 > 0xffff )
    {
        R5900Regs.Reg[ RD ].u16_112_127 = 0xffff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_112_127 = TempU32;
    }
*/
}

void EmuRec_psubuh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RS ].u16_00_15 - R5900Regs.Reg[ RT ].u16_00_15;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_00_15 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_00_15 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_16_31 - R5900Regs.Reg[ RT ].u16_16_31;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_16_31 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_16_31 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_32_47 - R5900Regs.Reg[ RT ].u16_32_47;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_32_47 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_32_47 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_48_63 - R5900Regs.Reg[ RT ].u16_48_63;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_48_63 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_48_63 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_64_79 - R5900Regs.Reg[ RT ].u16_64_79;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_64_79 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_64_79 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_80_95 - R5900Regs.Reg[ RT ].u16_80_95;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_80_95 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_80_95 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_96_111 - R5900Regs.Reg[ RT ].u16_96_111;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_96_111 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_96_111 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u16_112_127 - R5900Regs.Reg[ RT ].u16_112_127;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u16_112_127 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u16_112_127 = TempU32;
    }
*/
}

void EmuRec_pextuh( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u16_00_15   = R5900Regs.Reg[ RT ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_16_31   = R5900Regs.Reg[ RS ].u16_64_79;
    R5900Regs.Reg[ RD ].u16_32_47   = R5900Regs.Reg[ RT ].u16_80_95;
    R5900Regs.Reg[ RD ].u16_48_63   = R5900Regs.Reg[ RS ].u16_80_95;
    R5900Regs.Reg[ RD ].u16_64_79   = R5900Regs.Reg[ RT ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_80_95   = R5900Regs.Reg[ RS ].u16_96_111;
    R5900Regs.Reg[ RD ].u16_96_111  = R5900Regs.Reg[ RT ].u16_112_127;
    R5900Regs.Reg[ RD ].u16_112_127 = R5900Regs.Reg[ RS ].u16_112_127;
*/
}

void EmuRec_paddub( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RS ].u08_00_07 + R5900Regs.Reg[ RT ].u08_00_07;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_00_07 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_00_07 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_08_15 + R5900Regs.Reg[ RT ].u08_08_15;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_08_15 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_08_15 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_16_23 + R5900Regs.Reg[ RT ].u08_16_23;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_16_23 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_16_23 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_24_31 + R5900Regs.Reg[ RT ].u08_24_31;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_24_31 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_24_31 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_32_39 + R5900Regs.Reg[ RT ].u08_32_39;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_32_39 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_32_39 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_40_47 + R5900Regs.Reg[ RT ].u08_40_47;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_40_47 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_40_47 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_48_55 + R5900Regs.Reg[ RT ].u08_48_55;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_48_55 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_48_55 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_56_63 + R5900Regs.Reg[ RT ].u08_56_63;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_56_63 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_56_63 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_64_71 + R5900Regs.Reg[ RT ].u08_64_71;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_64_71 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_64_71 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_72_79 + R5900Regs.Reg[ RT ].u08_72_79;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_72_79 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_72_79 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_80_87 + R5900Regs.Reg[ RT ].u08_80_87;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_80_87 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_80_87 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_88_95 + R5900Regs.Reg[ RT ].u08_88_95;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_88_95 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_88_95 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_96_103 + R5900Regs.Reg[ RT ].u08_96_103;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_96_103 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_96_103 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_104_111 + R5900Regs.Reg[ RT ].u08_104_111;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_104_111 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_104_111 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_112_119 + R5900Regs.Reg[ RT ].u08_112_119;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_112_119 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_112_119 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_120_127 + R5900Regs.Reg[ RT ].u08_120_127;       
    if ( TempU32 > 0xff )
    {
        R5900Regs.Reg[ RD ].u08_120_127 = 0xff;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_120_127 = TempU32;
    }
*/
}

void EmuRec_psubub( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU32 = R5900Regs.Reg[ RS ].u08_00_07 - R5900Regs.Reg[ RT ].u08_00_07;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_00_07 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_00_07 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_08_15 - R5900Regs.Reg[ RT ].u08_08_15;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_08_15 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_08_15 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_16_23 - R5900Regs.Reg[ RT ].u08_16_23;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_16_23 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_16_23 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_24_31 - R5900Regs.Reg[ RT ].u08_24_31;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_24_31 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_24_31 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_32_39 - R5900Regs.Reg[ RT ].u08_32_39;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_32_39 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_32_39 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_40_47 - R5900Regs.Reg[ RT ].u08_40_47;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_40_47 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_40_47 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_48_55 - R5900Regs.Reg[ RT ].u08_48_55;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_48_55 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_48_55 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_56_63 - R5900Regs.Reg[ RT ].u08_56_63;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_56_63 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_56_63 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_64_71 - R5900Regs.Reg[ RT ].u08_64_71;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_64_71 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_64_71 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_72_79 - R5900Regs.Reg[ RT ].u08_72_79;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_72_79 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_72_79 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_80_87 - R5900Regs.Reg[ RT ].u08_80_87;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_80_87 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_80_87 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_88_95 - R5900Regs.Reg[ RT ].u08_88_95;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_88_95 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_88_95 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_96_103 - R5900Regs.Reg[ RT ].u08_96_103;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_96_103 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_96_103 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_104_111 - R5900Regs.Reg[ RT ].u08_104_111;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_104_111 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_104_111 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_112_119 - R5900Regs.Reg[ RT ].u08_112_119;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_112_119 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_112_119 = TempU32;
    }
    TempU32 = R5900Regs.Reg[ RS ].u08_120_127 - R5900Regs.Reg[ RT ].u08_120_127;       
    if ( TempU32 <= 0 )
    {
        R5900Regs.Reg[ RD ].u08_120_127 = 0;
    }
    else
    {
        R5900Regs.Reg[ RD ].u08_120_127 = TempU32;
    }
*/
}

void EmuRec_pextub( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    R5900Regs.Reg[ RD ].u08_00_07   = R5900Regs.Reg[ RT ].u08_64_71;
    R5900Regs.Reg[ RD ].u08_08_15   = R5900Regs.Reg[ RS ].u08_64_71;
    R5900Regs.Reg[ RD ].u08_16_23   = R5900Regs.Reg[ RT ].u08_72_79;
    R5900Regs.Reg[ RD ].u08_24_31   = R5900Regs.Reg[ RS ].u08_72_79;
    R5900Regs.Reg[ RD ].u08_32_39   = R5900Regs.Reg[ RT ].u08_80_87;
    R5900Regs.Reg[ RD ].u08_40_47   = R5900Regs.Reg[ RS ].u08_80_87;
    R5900Regs.Reg[ RD ].u08_48_55   = R5900Regs.Reg[ RT ].u08_88_95;
    R5900Regs.Reg[ RD ].u08_56_63   = R5900Regs.Reg[ RS ].u08_88_95;
    R5900Regs.Reg[ RD ].u08_64_71   = R5900Regs.Reg[ RT ].u08_96_103;
    R5900Regs.Reg[ RD ].u08_72_79   = R5900Regs.Reg[ RS ].u08_96_103;
    R5900Regs.Reg[ RD ].u08_80_87   = R5900Regs.Reg[ RT ].u08_104_111;
    R5900Regs.Reg[ RD ].u08_88_95   = R5900Regs.Reg[ RS ].u08_104_111;
    R5900Regs.Reg[ RD ].u08_96_103  = R5900Regs.Reg[ RT ].u08_112_119;
    R5900Regs.Reg[ RD ].u08_104_111 = R5900Regs.Reg[ RS ].u08_112_119;
    R5900Regs.Reg[ RD ].u08_112_119 = R5900Regs.Reg[ RT ].u08_120_127;
    R5900Regs.Reg[ RD ].u08_120_127 = R5900Regs.Reg[ RS ].u08_120_127;
*/
}

void EmuRec_qfsrv( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    if ( R5900Regs.SA )
    {
        R5900Regs.Reg[ RD ].u64_00_63  = ( R5900Regs.Reg[ RS ].u64_00_63 << ( 64 - R5900Regs.SA ) ) |
            ( R5900Regs.Reg[ RT ].u64_00_63 >> R5900Regs.SA );
        
        R5900Regs.Reg[ RD ].u64_64_127 = ( R5900Regs.Reg[ RS ].u64_64_127 << ( 64 - R5900Regs.SA ) ) |
            ( R5900Regs.Reg[ RT ].u64_64_127 >> R5900Regs.SA );
    }
    else
    {
        R5900Regs.Reg[ RD ].u64_00_63  = R5900Regs.Reg[ RT ].u64_00_63;
        R5900Regs.Reg[ RD ].u64_64_127 = R5900Regs.Reg[ RT ].u64_64_127;
    }
*/
}

void EmuRec_pmadduw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU64 = ( ( R5900Regs.HI.u32_00_31 << 32 ) | R5900Regs.LO.u32_00_31 ) + 
                ( R5900Regs.Reg[ RS ].u32_00_31 * R5900Regs.Reg[ RT ].u32_00_31 );
    R5900Regs.LO.u64_00_63 = (EMU_I32)( TempU64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_00_63 = (EMU_I32)( TempU64 >> 32 );
    R5900Regs.Reg[ RD ].u64_00_63 = TempU64; 
    TempU64 = ( ( R5900Regs.HI.u32_64_95 << 32 ) | R5900Regs.LO.u32_64_95 ) + 
                ( R5900Regs.Reg[ RS ].u32_64_95 * R5900Regs.Reg[ RT ].u32_64_95 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempU64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( TempU64 >> 32 );
    R5900Regs.Reg[ RD ].u64_00_63 = TempU64; 
*/
}

void EmuRec_psravw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63 = (EMU_I32)( R5900Regs.Reg[ R_RT ].i32_00_31 >> 
                                               ( R5900Regs.Reg[ R_RS ].u32_00_31 & 0x1F ) );
    R5900Regs.Reg[ R_RD ].u64_64_127 = (EMU_I32)( R5900Regs.Reg[ R_RT ].i32_64_95 >> 
                                                ( R5900Regs.Reg[ R_RS ].u32_64_95 & 0x1F ) );
*/
}

void EmuRec_pmthi( EMU_U32 Code )
{
/*
    R5900Regs.HI.u64_00_63  = R5900Regs.Reg[ R_RS ].u64_00_63;
    R5900Regs.HI.u64_64_127 = R5900Regs.Reg[ R_RS ].u64_64_127;
*/
}

void EmuRec_pmtlo( EMU_U32 Code )
{
/*
    R5900Regs.LO.u64_00_63  = R5900Regs.Reg[ R_RS ].u64_00_63;
    R5900Regs.LO.u64_64_127 = R5900Regs.Reg[ R_RS ].u64_64_127;
*/
}

void EmuRec_pinteh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RS ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RS ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RS ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].u16_96_111;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RS ].u16_96_111;
*/
}

void EmuRec_pmultuw( EMU_U32 Code )
{
/*
    RS = R_RS;
    RT = R_RT;
    RD = R_RD;
    TempU64 = R5900Regs.Reg[ RS ].u32_00_31 * R5900Regs.Reg[ RT ].u32_00_31;
    R5900Regs.LO.u64_00_63 = (EMU_I32)( TempU64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_00_63 = (EMU_I32)( TempU64 >> 32 );
    if ( RD )
    {
        R5900Regs.Reg[ RD ].u64_00_63 = R5900Regs.LO.u64_00_63; 
    }    
    TempU64 = R5900Regs.Reg[ RS ].u32_64_95 * R5900Regs.Reg[ RT ].u32_64_95;
    R5900Regs.LO.u64_64_127 = (EMU_I32)( TempU64 & 0x00000000FFFFFFFF );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( TempU64 >> 32 );
    if ( RD )
    {
        R5900Regs.Reg[ RD ].u64_64_127 = R5900Regs.LO.u64_64_127; 
    }
*/
}

void EmuRec_pdivuw( EMU_U32 Code )
{
/*
    R5900Regs.LO.u64_00_63  = (EMU_I32)( R5900Regs.Reg[ R_RS ].u32_00_31 /
                                         R5900Regs.Reg[ R_RT ].u32_00_31 );
    R5900Regs.LO.u64_64_127 = (EMU_I32)( R5900Regs.Reg[ R_RS ].u32_64_95 /
                                         R5900Regs.Reg[ R_RT ].u32_64_95 );
    R5900Regs.HI.u64_00_63  = (EMU_I32)( R5900Regs.Reg[ R_RS ].u32_00_31 %
                                         R5900Regs.Reg[ R_RT ].u32_00_31 );
    R5900Regs.HI.u64_64_127 = (EMU_I32)( R5900Regs.Reg[ R_RS ].u32_64_95 %
                                         R5900Regs.Reg[ R_RT ].u32_64_95 );
*/
}

void EmuRec_pcpyud( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63   = R5900Regs.Reg[ R_RS ].u64_64_127;
    R5900Regs.Reg[ R_RD ].u64_64_127  = R5900Regs.Reg[ R_RT ].u64_64_127;
*/
}

void EmuRec_por( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63  = R5900Regs.Reg[ R_RS ].u64_00_63  | R5900Regs.Reg[ R_RT ].u64_00_63;
    R5900Regs.Reg[ R_RD ].u64_64_127 = R5900Regs.Reg[ R_RS ].u64_64_127 | R5900Regs.Reg[ R_RT ].u64_64_127;
*/
}

void EmuRec_pnor( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u64_00_63  = ~( R5900Regs.Reg[ R_RS ].u64_00_63  | R5900Regs.Reg[ R_RT ].u64_00_63 );
    R5900Regs.Reg[ R_RD ].u64_64_127 = ~( R5900Regs.Reg[ R_RS ].u64_64_127 | R5900Regs.Reg[ R_RT ].u64_64_127 );
*/
}

void EmuRec_pexch( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RT ].u16_32_47;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].u16_16_31;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RT ].u16_48_63;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RT ].u16_96_111;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].u16_80_95;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RT ].u16_112_127;
*/
}

void EmuRec_pcpyh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RT ].u16_00_15;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].u16_64_79;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RT ].u16_64_79;
*/
}

void EmuRec_pexcw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.Reg[ R_RT ].u32_00_31;
    R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.Reg[ R_RT ].u32_64_95;
    R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.Reg[ R_RT ].u32_32_63;
    R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.Reg[ R_RT ].u32_96_127;
*/
}

void EmuRec_pmfhl( EMU_U32 Code )
{
/*
    switch ( Code & 0x000001FF )
    {
    case 0x00000030: // "pmfhl.lw"
        R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.LO.u32_00_31;
        R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.HI.u32_00_31;
        R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.LO.u32_64_95;
        R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.HI.u32_64_95;
        break;
        
    case 0x00000070: // "pmfhl.uw"
        R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.LO.u32_32_63;
        R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.HI.u32_32_63;
        R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.LO.u32_96_127;
        R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.HI.u32_96_127;
        break;
        
    case 0x000000B0: // "pmfhl.slw"
        TempU64 = ( (EMU_U64)R5900Regs.HI.u32_00_31 << 32 ) | (EMU_U64)R5900Regs.LO.u32_00_31;
        if( TempU64 >= 0x000000007FFFFFFF ) 
        {
            R5900Regs.Reg[ R_RD ].u64_00_63 = 0x000000007FFFFFFF;
        }
        else 
        if( TempU64 <= 0xFFFFFFFF80000000 ) 
        {
            R5900Regs.Reg[ R_RD ].u64_00_63 = 0xFFFFFFFF80000000;
        }
        else
        {
            R5900Regs.Reg[ R_RD ].u64_00_63 = R5900Regs.LO.i32_00_31;
        }
        TempU64 = ((EMU_U64)R5900Regs.HI.u32_64_95 << 32) | (EMU_U64)R5900Regs.LO.u32_64_95;
        if( TempU64 >= 0x000000007FFFFFFF ) 
        {
            R5900Regs.Reg[ R_RD ].u64_64_127 = 0x000000007FFFFFFF;
        }
        else 
        if( TempU64 <= 0xFFFFFFFF80000000 ) 
        {
            R5900Regs.Reg[ R_RD ].u64_64_127 = 0xFFFFFFFF80000000;
        }
        else
        {
            R5900Regs.Reg[ R_RD ].u64_64_127 = R5900Regs.LO.i32_64_95;
        }
        break;
                
    case 0x000000F0: // "pmfhl.lh"
        R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.LO.u16_00_15;
        R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.LO.u16_32_47;
        R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.HI.u16_00_15;
        R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.HI.u16_32_47;
        R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.LO.u16_64_79;
        R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.LO.u16_96_111;
        R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.HI.u16_64_79;
        R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.HI.u16_96_111;
        break;
        
    case 0x00000130: // "pmfhl.sh"
        R5900Regs.Reg[ R_RD ].u16_00_15   = clamp( R5900Regs.LO.u32_00_31 );
        R5900Regs.Reg[ R_RD ].u16_16_31   = clamp( R5900Regs.LO.u32_32_63 );
        R5900Regs.Reg[ R_RD ].u16_32_47   = clamp( R5900Regs.HI.u32_00_31 );
        R5900Regs.Reg[ R_RD ].u16_48_63   = clamp( R5900Regs.HI.u32_32_63 );
        R5900Regs.Reg[ R_RD ].u16_64_79   = clamp( R5900Regs.LO.u32_64_95 );
        R5900Regs.Reg[ R_RD ].u16_80_95   = clamp( R5900Regs.LO.u32_96_127 );
        R5900Regs.Reg[ R_RD ].u16_96_111  = clamp( R5900Regs.HI.u32_64_95 );
        R5900Regs.Reg[ R_RD ].u16_112_127 = clamp( R5900Regs.HI.u32_96_127 );
        break;
    }
*/
}

void EmuRec_pmthl( EMU_U32 Code )
{
/*
    R5900Regs.LO.u32_00_31 = R5900Regs.Reg[ R_RS ].u32_00_31;
    R5900Regs.HI.u32_00_31 = R5900Regs.Reg[ R_RS ].u32_32_63;
    R5900Regs.LO.u32_64_95 = R5900Regs.Reg[ R_RS ].u32_64_95;
    R5900Regs.HI.u32_64_95 = R5900Regs.Reg[ R_RS ].u32_96_127;
*/
}

void EmuRec_psllh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].u16_00_15   << R_SA;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RT ].u16_16_31   << R_SA;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].u16_32_47   << R_SA;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RT ].u16_48_63   << R_SA;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].u16_64_79   << R_SA;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RT ].u16_80_95   << R_SA;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].u16_96_111  << R_SA;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RT ].u16_112_127 << R_SA;
*/
}

void EmuRec_psrlh( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].u16_00_15   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RT ].u16_16_31   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].u16_32_47   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RT ].u16_48_63   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].u16_64_79   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RT ].u16_80_95   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].u16_96_111  >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RT ].u16_112_127 >> R_SA;
*/
}

void EmuRec_psrah( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u16_00_15   = R5900Regs.Reg[ R_RT ].i16_00_15   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_16_31   = R5900Regs.Reg[ R_RT ].i16_16_31   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_32_47   = R5900Regs.Reg[ R_RT ].i16_32_47   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_48_63   = R5900Regs.Reg[ R_RT ].i16_48_63   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_64_79   = R5900Regs.Reg[ R_RT ].i16_64_79   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_80_95   = R5900Regs.Reg[ R_RT ].i16_80_95   >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_96_111  = R5900Regs.Reg[ R_RT ].i16_96_111  >> R_SA;
    R5900Regs.Reg[ R_RD ].u16_112_127 = R5900Regs.Reg[ R_RT ].i16_112_127 >> R_SA;
*/
}

void EmuRec_psllw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.Reg[ R_RT ].u32_00_31  << R_SA;
    R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.Reg[ R_RT ].u32_32_63  << R_SA;
    R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.Reg[ R_RT ].u32_64_95  << R_SA;
    R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.Reg[ R_RT ].u32_96_127 << R_SA;
*/
}

void EmuRec_psrlw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.Reg[ R_RT ].u32_00_31  >> R_SA;
    R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.Reg[ R_RT ].u32_32_63  >> R_SA;
    R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.Reg[ R_RT ].u32_64_95  >> R_SA;
    R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.Reg[ R_RT ].u32_96_127 >> R_SA;
*/
}

void EmuRec_psraw( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RD ].u32_00_31   = R5900Regs.Reg[ R_RT ].i32_00_31  >> R_SA;
    R5900Regs.Reg[ R_RD ].u32_32_63   = R5900Regs.Reg[ R_RT ].i32_32_63  >> R_SA;
    R5900Regs.Reg[ R_RD ].u32_64_95   = R5900Regs.Reg[ R_RT ].i32_64_95  >> R_SA;
    R5900Regs.Reg[ R_RD ].u32_96_127  = R5900Regs.Reg[ R_RT ].i32_96_127 >> R_SA;
*/
}
