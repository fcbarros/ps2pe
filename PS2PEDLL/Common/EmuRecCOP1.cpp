
#include <math.h>

#include "EmuMain.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"


void EmuRec_mfc1( EMU_U32 Code )
{
    if ( R_RT )
    {
        MOV32MtoR( EAX, (EMU_U32)&COP1Regs.Reg[ R_FS ].u );
        EMUREC_SIGN_EXTEND( EDX, EAX );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31, EAX );
        MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_32_63, EDX );
    }
}

void EmuRec_cfc1( EMU_U32 Code )
{
/*
    R5900Regs.Reg[ R_RT ].u64_00_63 = (EMU_I32)( R_RD ? COP1Regs.FCR31 : COP1Regs.FCR0 );
*/
}

void EmuRec_mtc1( EMU_U32 Code )
{
    if ( R_RT )
    {
        MOV32MtoR( EAX, (EMU_U32)&R5900Regs.Reg[ R_RT ].u32_00_31 );
        MOV32RtoM( (EMU_U32)&COP1Regs.Reg[ R_FS ].u, EAX );
    }
    else
    {
        MOV32ItoM( (EMU_U32)&COP1Regs.Reg[ R_FS ].u, 0 );
    }
}

void EmuRec_ctc1( EMU_U32 Code )
{
/*
    if ( R_RD )
    {
        COP1Regs.FCR31 = R5900Regs.Reg[ R_RT ].u32_00_31;
    }
    else
    {
        COP1Regs.FCR0 = R5900Regs.Reg[ R_RT ].u32_00_31;
    }
*/
}

void EmuRec_bc1f( EMU_U32 Code )
{
/*
    if ( COP1Regs.FCR31_C == 0 )
    {
        EXECUTE_BRANCH( R5900Regs.PC );
    }
*/
}

void EmuRec_bc1t( EMU_U32 Code )
{
/*
    if ( COP1Regs.FCR31_C == 1 )
    {
        EXECUTE_BRANCH( R5900Regs.PC );
    }
*/
}

void EmuRec_bc1fl( EMU_U32 Code )
{
/*
    if ( COP1Regs.FCR31_C == 0 )
    {
        EXECUTE_BRANCH( R5900Regs.PC );
    }
    else
    {
        R5900Regs.PC += 4;
    }
*/
}

void EmuRec_bc1tl( EMU_U32 Code )
{
/*
    if ( COP1Regs.FCR31_C == 1 )
    {
        EXECUTE_BRANCH( R5900Regs.PC );
    }
    else
    {
        R5900Regs.PC += 4;
    }
*/
}

void EmuRec_add_s( EMU_U32 Code )
{
/*
    COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f + COP1Regs.Reg[ R_FT ].f;
    SetFloatFlags( EmuFloatStatus );
*/
}

void EmuRec_sub_s( EMU_U32 Code )
{
/*
    COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f - COP1Regs.Reg[ R_FT ].f;
    SetFloatFlags( EmuFloatStatus );
*/
}

void EmuRec_mul_s( EMU_U32 Code )
{
	FLD32( (EMU_U32)&COP1Regs.Reg[ R_FS ].f );
	FMUL32( (EMU_U32)&COP1Regs.Reg[ R_FT ].f );
	FSTP32( (EMU_U32)&COP1Regs.Reg[ R_FD ].f );
//    SetFloatFlags( EmuFloatStatus );
}

void EmuRec_div_s( EMU_U32 Code )
{
    CMP32ItoM( (EMU_U32)&COP1Regs.Reg[ R_FT ].f, 0 );
    EMU_U08 * LinkE0 = JE8( 0 );

    FLD32( (EMU_U32)&COP1Regs.Reg[ R_FS ].f );
	FDIV32( (EMU_U32)&COP1Regs.Reg[ R_FT ].f );
	FSTP32( (EMU_U32)&COP1Regs.Reg[ R_FD ].f );

    *LinkE0 = EmuRec_CurrentAddress( ) - LinkE0 - 1;
//        SetFloatFlags( EmuFloatStatus );
}

void EmuRec_sqrt_s( EMU_U32 Code )
{
/*
    if ( COP1Regs.Reg[ R_FT ].f < 0.0f ) 
    {
        COP1Regs.FCR31_I = 1;
        COP1Regs.FCR31_SI = 1;
    }
    else
    {
        COP1Regs.FCR31_I = 0;
        COP1Regs.Reg[ R_FD ].f = (float)sqrt( (double)COP1Regs.Reg[ R_FT ].f );
        SetFloatFlags( EmuFloatStatus );
    }
    COP1Regs.FCR31_D = 0;
*/
}

void EmuRec_abs_s( EMU_U32 Code )
{
/*
    COP1Regs.Reg[ R_FD ].f = (float)fabs( COP1Regs.Reg[ R_FS ].f );
    COP1Regs.FCR31_O = 0;
    COP1Regs.FCR31_U = 0;
*/
}

void EmuRec_mov_s( EMU_U32 Code )
{
//    PUSH32R( EAX );
    MOV32MtoR( EAX, (EMU_U32)&COP1Regs.Reg[ R_FS ].u );
    MOV32RtoM( (EMU_U32)&COP1Regs.Reg[ R_FD ].u, EAX );
//    POP32R( EAX );
}

void EmuRec_neg_s( EMU_U32 Code )
{
/*
    COP1Regs.Reg[ R_FD ].f = -COP1Regs.Reg[ R_FS ].f;
    COP1Regs.FCR31_O = 0;
    COP1Regs.FCR31_U = 0;
*/
}

void EmuRec_rsqrt_s( EMU_U32 Code )
{
/*
    if ( COP1Regs.Reg[ R_FT ].f < 0.0f ) 
    {
        COP1Regs.FCR31_I = 1;
        COP1Regs.FCR31_D = 0;
        COP1Regs.FCR31_SI = 1;
    }
    else
    if ( COP1Regs.Reg[ R_FT ].f == 0.0f ) 
    {
        COP1Regs.FCR31_I = 0;
        COP1Regs.FCR31_D = 1;
        COP1Regs.FCR31_SD = 1;
    }
    else
    {
        COP1Regs.FCR31_I = 0;
        COP1Regs.FCR31_D = 0;
        COP1Regs.Reg[ R_FD ].f = COP1Regs.Reg[ R_FS ].f / (float)(sqrt( COP1Regs.Reg[ R_FT ].f ));
        SetFloatFlags( EmuFloatStatus );
    }
*/
}

void EmuRec_adda_s( EMU_U32 Code )
{
/*
    COP1Regs.Accumulator.f = COP1Regs.Reg[ R_FS ].f + COP1Regs.Reg[ R_FT ].f;
    SetFloatFlags( EmuFloatStatus );
*/
}

void EmuRec_suba_s( EMU_U32 Code )
{
/*
    COP1Regs.Accumulator.f = COP1Regs.Reg[ R_FS ].f - COP1Regs.Reg[ R_FT ].f;
    SetFloatFlags( EmuFloatStatus );
*/
}

void EmuRec_mula_s( EMU_U32 Code )
{
/*
    COP1Regs.Accumulator.f = COP1Regs.Reg[ R_FS ].f * COP1Regs.Reg[ R_FT ].f;
    SetFloatFlags( EmuFloatStatus );
*/
}

void EmuRec_madd_s( EMU_U32 Code )
{
/*
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
        SetFloatFlags( EmuFloatStatus );
    }
*/
}

void EmuRec_msub_s( EMU_U32 Code )
{
/*
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
        SetFloatFlags( EmuFloatStatus );
    }
*/
}

void EmuRec_madda_s( EMU_U32 Code )
{
/*
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
        SetFloatFlags( EmuFloatStatus );
    }
*/
}

void EmuRec_msuba_s( EMU_U32 Code )
{
/*
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
        SetFloatFlags( EmuFloatStatus );
    }
*/
}

void EmuRec_cvt_w_s( EMU_U32 Code )
{
	FLD32( (EMU_U32)&COP1Regs.Reg[ R_FS ].f );
	FISTP32( (EMU_U32)&COP1Regs.Reg[ R_FD ].u );
}

void EmuRec_max_s( EMU_U32 Code )
{
/*
    COP1Regs.Reg[ R_FD ].f = ( COP1Regs.Reg[ R_FS ].f >= COP1Regs.Reg[ R_FT ].f ) ?
                               COP1Regs.Reg[ R_FS ].f : COP1Regs.Reg[ R_FT ].f;
    COP1Regs.FCR31_O = 0;
    COP1Regs.FCR31_U = 0;
*/
}

void EmuRec_min_s( EMU_U32 Code )
{
/*
    COP1Regs.Reg[ R_FD ].f = ( COP1Regs.Reg[ R_FS ].f <= COP1Regs.Reg[ R_FT ].f ) ?
                               COP1Regs.Reg[ R_FS ].f  : COP1Regs.Reg[ R_FT ].f;
    COP1Regs.FCR31_O = 0;
    COP1Regs.FCR31_U = 0;
*/
}

void EmuRec_c_f_s( EMU_U32 Code )
{
/*
    COP1Regs.FCR31_C = 0;
*/
}

void EmuRec_c_eq_s( EMU_U32 Code )
{
/*
    COP1Regs.FCR31_C = ( COP1Regs.Reg[ R_FS ].f == COP1Regs.Reg[ R_FT ].f );
*/
}

void EmuRec_c_lt_s( EMU_U32 Code )
{
/*
    COP1Regs.FCR31_C = ( COP1Regs.Reg[ R_FS ].f < COP1Regs.Reg[ R_FT ].f );
*/
}

void EmuRec_c_le_s( EMU_U32 Code )
{
/*
    COP1Regs.FCR31_C = ( COP1Regs.Reg[ R_FS ].f <= COP1Regs.Reg[ R_FT ].f );
*/
}

void EmuRec_cvt_s_w( EMU_U32 Code )
{
	FILD32( (EMU_U32)&COP1Regs.Reg[ R_FS ].u );
	FSTP32( (EMU_U32)&COP1Regs.Reg[ R_FD ].f );
}
