
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

// macros helpers

#define SIB     4
#define DISP32  5

#define ModRM( mod, rm, reg ) \
	EmuRec_Write8( (mod << 6) | (rm << 3) | (reg) );

#define SibSB( ss, rm, index ) \
	EmuRec_Write8( (ss << 6) | (rm << 3) | (index) );



// stack instructions

// push r32
void PUSH32R( EMU_U32 from )
{
    EmuRec_Write8( 0x50 | from ); 
    EmuRec_SetIntRegTime( from );
}

// push m32
void PUSH32M( EMU_U32 from )
{
    EmuRec_Write8( 0xFF );
    ModRM( 0, 6, DISP32 );
    EmuRec_Write32( from );
}

// push imm32
void PUSH32I( EMU_U32 from )
{
    EmuRec_Write8( 0x68 );
    EmuRec_Write32( from );
}

// pop r32
void POP32R( EMU_U32 from )
{
    EmuRec_Write8( 0x58 | from );
    EmuRec_SetIntRegTime( from );
}

// pushad
void PUSHA32( void )
{
    EmuRec_Write8( 0x60 );
}

// popad
void POPA32( void )
{
    EmuRec_Write8( 0x61 );
}

// ret
void RET( void )
{
    EmuRec_Write8( 0xC3 );
}

// CWDE - Convert AX to EAX (sign Extend)
void CWDE( void )
{
    EmuRec_Write8( 0x98 );
}

// CBW - Convert AL to AX (sign Extend)
void CBW( void )
{
    EmuRec_Write8( 0x66 );
    EmuRec_Write8( 0x98 );
}

// Call

// call func
void CALLFunc( EMU_U32 func, EMU_U32 CurrentAddress )
{
    CALLNearRel32( func - ( CurrentAddress + 5 ) );
}

// call rel32
EMU_U32 * CALLNearRel32( EMU_U32 to )
{
    EmuRec_Write8( 0xE8 );
    EmuRec_Write32( to );
    return (EMU_U32*)( EmuRec_CurrentAddress() - 4 );
}

// Jumps

// jmp rel8
EMU_U08 * JMP8( EMU_U08 to )
{
    EmuRec_Write8( 0xEB );
    EmuRec_Write8( to );
    return (EMU_U08*)( EmuRec_CurrentAddress() - 1 );
}

// jmp rel32
EMU_U32 * JMP32( EMU_U32 to )
{
    EmuRec_Write8( 0xE9 );
    EmuRec_Write32( to );
    return (EMU_U32*)( EmuRec_CurrentAddress() - 4 );
}

// jmp r32
void JMP32R( EMU_U32 to )
{
    EmuRec_Write8( 0xFF );
    ModRM( 3, 4, to );
    EmuRec_SetIntRegTime( to );
}

//extern EMU_U32 RegisterJumpAddress;

// jmp [to]
void JMP32M( EMU_U32 to )
{
    EmuRec_Write8( 0xFF );
    EmuRec_Write8( 0x25 );
    EmuRec_Write32( to );
}

// Jcc
#define J8Rel( cc, to ) { \
    EmuRec_Write8( cc ); EmuRec_Write8( to ); return (EMU_U08*)( EmuRec_CurrentAddress() - 1 ); }

#define J32Rel( cc, to ) { \
    EmuRec_Write8( 0x0F ); EmuRec_Write8( cc ); EmuRec_Write32( to ); return (EMU_U32*)( EmuRec_CurrentAddress() - 4 ); }

EMU_U08 * JA8( EMU_U08 to )
{
    J8Rel( 0x77, to );
}

EMU_U32 * JA32( EMU_U32 to )
{
    J32Rel( 0x87, to );
}

EMU_U08 * JAE8( EMU_U08 to )
{
    J8Rel( 0x73, to );
}

EMU_U08 * JB8( EMU_U08 to )
{
    J8Rel( 0x72, to );
}

EMU_U32 * JB32( EMU_U32 to )
{
    J32Rel( 0x82, to );
}

EMU_U08 * JBE8( EMU_U08 to )
{
    J8Rel( 0x76, to );
}

EMU_U08 * JC8( EMU_U08 to )
{
    J8Rel( 0x72, to );
}

EMU_U08 * JNC8( EMU_U08 to )
{
    J8Rel( 0x73, to );
}

// je rel8
EMU_U08 * JE8( EMU_U08 to )
{
    J8Rel( 0x74, to );
}

// je rel8
EMU_U32 * JE32( EMU_U32 to )
{
    J32Rel( 0x84, to );
}

// jz rel8
EMU_U08 * JZ8( EMU_U08 to )
{
    J8Rel( 0x74, to );
}

// jg rel8
EMU_U08 * JG8( EMU_U08 to )
{
    J8Rel( 0x7F, to );
}

// jg rel32
EMU_U32 * JG32( EMU_U32 to )
{
    J32Rel( 0x8F, to );
}

// jge rel8
EMU_U08 * JGE8( EMU_U08 to )
{
    J8Rel( 0x7D, to );
}

// jge rel32
EMU_U32 * JGE32( EMU_U32 to )
{
    J32Rel( 0x8D, to );
}

// jl rel8
EMU_U08 * JL8( EMU_U08 to )
{
    J8Rel( 0x7C, to );
}

// jl rel32
EMU_U32 * JL32( EMU_U32 to )
{
    J32Rel( 0x8C, to );
}

// jle rel8
EMU_U08 * JLE8( EMU_U08 to )
{
    J8Rel( 0x7E, to );
}

// jle rel32
EMU_U32 * JLE32( EMU_U32 to )
{
    J32Rel( 0x8E, to );
}

// jne rel8
EMU_U08 * JNE8( EMU_U08 to )
{
    J8Rel( 0x75, to );
}

// jne rel32
EMU_U32 * JNE32( EMU_U32 to )
{
    J32Rel( 0x85, to );
}

// jnz rel8
EMU_U08 * JNZ8( EMU_U08 to )
{
    J8Rel( 0x75, to );
}

// jng rel8
EMU_U08 * JNG8( EMU_U08 to )
{
    J8Rel( 0x7E, to );
}

// jnge rel8
EMU_U08 * JNGE8( EMU_U08 to )
{
    J8Rel( 0x7C, to );
}

// jnl rel8
EMU_U08 * JNL8( EMU_U08 to )
{
    J8Rel( 0x7D, to );
}

// jnle rel8
EMU_U08 * JNLE8( EMU_U08 to )
{
    J8Rel( 0x7F, to );
}


// mov instructions

// mov r32 to r32
void MOV32RtoR( EMU_U32 to, EMU_U32 from ) 
{
    EmuRec_Write8( 0x89 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// mov r32 to m32
void MOV32RtoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x89 );
    ModRM( 0, from, DISP32 );
    EmuRec_Write32( to );
    EmuRec_SetIntRegTime( from );
}

// mov m32 to r32
void MOV32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x8B );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from ); 
    EmuRec_SetIntRegTime( to );
}

// mov [r32] to r32
void MOV32RmtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x8B );
    ModRM( 0, to, from );
    EmuRec_SetIntRegTime( to );
}

// mov [r32][r32*scale] to r32
void MOV32RmStoR( EMU_U32 to, EMU_U32 from, EMU_U32 from2, EMU_U32 scale ) 
{
    EmuRec_Write8( 0x8B );
    ModRM(0, to, 0x4 );
    SibSB( scale, from2, from );
    EmuRec_SetIntRegTime( to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( from2 );
}

// mov r32 to [r32]
void MOV32RtoRm( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x89 );
    ModRM( 0, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// mov imm32 to r32
void MOV32ItoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0xB8 | to ); 
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// mov imm32 to m32
void MOV32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0xC7 );
    ModRM( 0, 0, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write32( from ); 
}

// CMov

#define CMOV32RtoR( cc, to, from )      \
    {                                   \
        EmuRec_Write8( 0x0F );          \
        EmuRec_Write8( cc );            \
        ModRM( 3, to, from );           \
        EmuRec_SetIntRegTime( from );   \
        EmuRec_SetIntRegTime( to );     \
    }

#define CMOV32MtoR( cc, to, from )  \
    {                               \
        EmuRec_Write8( 0x0F );      \
        EmuRec_Write8( cc );        \
        ModRM( 0, to, DISP32 );     \
        EmuRec_Write32( from );     \
        EmuRec_SetIntRegTime( to ); \
    }

// cmovne r32 to r32
void CMOVNE32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x45, to, from );
}

// cmovne m32 to r32
void CMOVNE32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x45, to, from );
}

// cmove r32 to r32
void CMOVE32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x44, to, from );
}

// cmove m32 to r32
void CMOVE32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x44, to, from );
}

// cmovg r32 to r32
void CMOVG32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x4F, to, from );
}

// cmovg m32 to r32
void CMOVG32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x4F, to, from );
}

// cmovge r32 to r32
void CMOVGE32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x4D, to, from );
}

// cmovge m32 to r32
void CMOVGE32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x4D, to, from );
}

// cmovl r32 to r32
void CMOVL32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x4C, to, from );
}

// cmovl m32 to r32
void CMOVL32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x4C, to, from );
}

// cmovle r32 to r32
void CMOVLE32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x4E, to, from );
}

// cmovle m32 to r32
void CMOVLE32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x4E, to, from );
}

// cmovle r32 to r32
void CMOVC32RtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x42, to, from );
}

// cmovle m32 to r32
void CMOVC32MtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x42, to, from );
}


// SetCC

#define SET8R( cc, to ) { \
    EmuRec_Write8( 0x0F ); EmuRec_Write8( cc ); \
    EmuRec_Write8( 0xC0 | (to) ); }

// setnge (setl ) r8
void SETNGER( EMU_U32 to )
{
    SET8R( 0x9C, to );
}

void SETSR( EMU_U32 to )
{
    SET8R( 0x98, to );
}

void SETNSR( EMU_U32 to )
{
    SET8R( 0x99, to );
}

void SETOR( EMU_U32 to )
{
    SET8R( 0x90, to );
}

void SETNOR( EMU_U32 to )
{
    SET8R( 0x91, to );
}

void SETCR( EMU_U32 to )
{
    SET8R( 0x92, to );
}

void SETNCR( EMU_U32 to )
{
    SET8R( 0x93, to );
}

// Compare

// cmp imm32 to r32
void CMP32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x3D );
    }
    else
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 7, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// cmp imm32 to m32
void CMP32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x81 );
    ModRM( 0, 7, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write32( from );
}

// cmp r32 to r32
void CMP32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x39 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// cmp m32 to r32
void CMP32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x3B );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// test imm32 to r32
void TEST32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0xA9 );
    }
    else
    {
        EmuRec_Write8( 0xF7 );
        ModRM( 3, 0, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// test imm32 to m32
void TEST32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 0, 0, to );
    EmuRec_Write32( from );
}

// test r32 to r32
void TEST32RtoR( EMU_U32 to, EMU_U32 from ) 
{
    EmuRec_Write8( 0x85 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}


// arithmic instructions

// add imm8 to r32
void ADD8ItoR( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write8( 0x83 ); 
    ModRM( 3, 0, to );
    EmuRec_Write8( from );
    EmuRec_SetIntRegTime( to );
}

// add imm32 to r32
void ADD32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX ) 
    {
        EmuRec_Write8( 0x05 ); 
    }
    else
    {
        EmuRec_Write8( 0x81 ); 
        ModRM( 3, 0, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// add imm8 to m32
void ADD32ItoM( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write8( 0x83 );
    ModRM( 0, 0, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write8( from );
}

// add imm32 to m32
void ADD32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x81 );
    ModRM( 0, 0, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write32( from );
}

// add r32 to r32
void ADD32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x01 ); 
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// add m32 to r32
void ADD32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x03); 
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// adc imm32 to r32
void ADC32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x15 );
    }
    else
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 2, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// adc r32 to r32
void ADC32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x11 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// adc m32 to r32
void ADC32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x13 ); 
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from ); 
    EmuRec_SetIntRegTime( to );
}

// sub imm32 to r32
void SUB32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x2D );
    } 
    else 
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 5, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// sub r32 to r32
void SUB32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x29 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// sub m32 to r32
void SUB32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x2B ); 
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// sbb imm32 to r32
void SBB32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x1D );
    } 
    else 
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 3, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// sbb r32 to r32
void SBB32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x19 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// sbb m32 to r32
void SBB32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x1B ); 
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from ); 
    EmuRec_SetIntRegTime( to );
}

// mul eax by r32 to edx:eax
void MUL32R( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM(3, 4, from );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// imul eax by r32 to edx:eax
void IMUL32R( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM(3, 5, from );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// mul eax by m32 to edx:eax
void MUL32M( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 0, 4, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// imul eax by m32 to edx:eax
void IMUL32M( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 0, 5, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// div eax by r32 to edx:eax
void DIV32R( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM(3, 6, from );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// idiv eax by r32 to edx:eax
void IDIV32R( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM(3, 7, from );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// div eax by m32 to edx:eax
void DIV32M( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 0, 6, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}

// idiv eax by m32 to edx:eax
void IDIV32M( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 0, 7, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( EAX );
    EmuRec_SetIntRegTime( EDX );
}


// Conditional moves

#define CMOV32RtoR( cc, to, from )      \
    {                                   \
        EmuRec_Write8( 0x0F );          \
        EmuRec_Write8( cc );            \
        ModRM( 3, to, from );           \
        EmuRec_SetIntRegTime( from );   \
        EmuRec_SetIntRegTime( to );     \
    }

#define CMOV32MtoR( cc, to, from )      \
    {                                   \
        EmuRec_Write8( 0x0F );          \
        EmuRec_Write8( cc );            \
        ModRM( 0, to, DISP32 );         \
        EmuRec_Write32( from );         \
        EmuRec_SetIntRegTime( to );     \
    }

void CMOVSRtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x48, to, from );
}

void CMOVSMtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x48, to, from );
}

void CMOVNSRtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32RtoR( 0x49, to, from );
}

void CMOVNSMtoR( EMU_U32 to, EMU_U32 from )
{
    CMOV32MtoR( 0x49, to, from );
}

// shifting instructions

// shld imm8 to r32
void SHLD32ItoR( EMU_U32 to, EMU_U32 from, EMU_U08 shift )
{
    EmuRec_Write8( 0x0F );
    EmuRec_Write8( 0xA4 );
    ModRM( 3, to, from );
    EmuRec_Write8( shift );
    EmuRec_SetIntRegTime( to );
}

// shrd imm8 to r32
void SHRD32ItoR( EMU_U32 to, EMU_U32 from, EMU_U08 shift )
{
    EmuRec_Write8( 0x0F );
    EmuRec_Write8( 0xAC );
    ModRM( 3, to, from );
    EmuRec_Write8( shift );
    EmuRec_SetIntRegTime( to );
}

// shl imm8 to r32
void SHL32ItoR( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write8( 0xC1 );
    ModRM( 3, 4, to );
    EmuRec_Write8( from ); 
    EmuRec_SetIntRegTime( to );
}

// shl cl to r32
void SHL32CLtoR( EMU_U32 to )
{
    EmuRec_Write8( 0xD3 ); 
    ModRM( 3, 4, to );
    EmuRec_SetIntRegTime( to );
}

// shr imm8 to r32
void SHR32ItoR( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write8( 0xC1 ); 
    ModRM( 3, 5, to );
    EmuRec_Write8( from ); 
    EmuRec_SetIntRegTime( to );
}

// shr cl to r32
void SHR32CLtoR( EMU_U32 to )
{
    EmuRec_Write8( 0xD3 ); 
    ModRM( 3, 5, to );
    EmuRec_SetIntRegTime( to );
}

// sar imm8 to r32
void SAR32ItoR( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write8( 0xC1 ); 
    ModRM( 3, 7, to );
    EmuRec_Write8( from ); 
    EmuRec_SetIntRegTime( to );
}

// logical instructions

// or imm32 to r32
void OR32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x0D );
    }
    else
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 1, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// or imm32 to m32
void OR32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x81 );
    ModRM( 0, 1, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write32( from );
}

// or r32 to r32
void OR32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x09 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// or m32 to r32
void OR32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x0B );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// xor imm32 to r32
void XOR32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x35 );
    }
    else
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 6, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// xor imm32 to m32
void XOR32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x81 );
    ModRM( 0, 6, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write32( from );
}

// xor r32 to r32
void XOR32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x31 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// xor m32 to r32
void XOR32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x33 );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// and imm32 to r32
void AND32ItoR( EMU_U32 to, EMU_U32 from )
{
    if ( to == EAX )
    {
        EmuRec_Write8( 0x25 );
    }
    else
    {
        EmuRec_Write8( 0x81 );
        ModRM( 3, 0x4, to );
    }
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// and imm32 to m32
void AND32ItoM( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x81 );
    ModRM( 0, 0x4, DISP32 );
    EmuRec_Write32( to );
    EmuRec_Write32( from );
}

// and r32 to r32
void AND32RtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x21 );
    ModRM( 3, from, to );
    EmuRec_SetIntRegTime( from );
    EmuRec_SetIntRegTime( to );
}

// and m32 to r32
void AND32MtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write8( 0x23 );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
    EmuRec_SetIntRegTime( to );
}

// not r32
void NOT32R( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 3, 2, from );
    EmuRec_SetIntRegTime( from );
}

// neg r32
void NEG32R( EMU_U32 from )
{
    EmuRec_Write8( 0xF7 );
    ModRM( 3, 3, from );
    EmuRec_SetIntRegTime( from );
}

//*******************
//* FPU instructions 
//*******************

// fild m32 to fpu reg stack
void FILD32( EMU_U32 from )
{
    EmuRec_Write8( 0xDB );
    ModRM( 0, 0x0, DISP32 );
    EmuRec_Write32( from );
}

// fistp m32 from fpu reg stack
void FISTP32( EMU_U32 from )
{
    EmuRec_Write8( 0xDB );
    ModRM( 0, 0x3, DISP32 );
    EmuRec_Write32( from );
}

// fld m32 to fpu reg stack
void FLD32( EMU_U32 from )
{
    EmuRec_Write8( 0xD9 );
    ModRM( 0, 0x0, DISP32 );
    EmuRec_Write32( from );
}

// fstp m32 from fpu reg stack
void FSTP32( EMU_U32 to )
{
    EmuRec_Write8( 0xD9 );
    ModRM( 0, 0x3, DISP32 );
    EmuRec_Write32( to) ;
}

// fcomp m32 to fpu reg stack
void FCOMP32( EMU_U32 from )
{
    EmuRec_Write8( 0xD8 );
    ModRM( 0, 0x3, DISP32 );
    EmuRec_Write32( from );
}

// fldcw fpu control word from m16
void FLDCW( EMU_U32 from )
{
    EmuRec_Write8( 0xD9 );
    ModRM( 0, 0x5, DISP32 );
    EmuRec_Write32( from );
}

// fnstcw fpu control word to m16
void FNSTCW( EMU_U32 to )
{
    EmuRec_Write8( 0xD9 );
    ModRM( 0, 0x7, DISP32 );
    EmuRec_Write32( to );
}

// fadd m32 to fpu reg stack
void FADD32( EMU_U32 from )
{
    EmuRec_Write8( 0xD8 );
    ModRM( 0, 0x0, DISP32 );
    EmuRec_Write32( from );
}

// fsub m32 to fpu reg stack
void FSUB32( EMU_U32 from )
{
    EmuRec_Write8( 0xD8 );
    ModRM( 0, 0x4, DISP32 );
    EmuRec_Write32( from );
}

// fmul m32 to fpu reg stack
void FMUL32( EMU_U32 from )
{
    EmuRec_Write8( 0xD8 );
    ModRM( 0, 0x1, DISP32 );
    EmuRec_Write32( from );
}

// fdiv m32 to fpu reg stack
void FDIV32( EMU_U32 from )
{
    EmuRec_Write8( 0xD8 );
    ModRM( 0, 0x6, DISP32 );
    EmuRec_Write32( from );
}

// fabs fpu reg stack
void FABS( void )
{
    EmuRec_Write16( 0xE1D9 );
}

// fsqrt fpu reg stack
void FSQRT( void )
{
    EmuRec_Write16( 0xFAD9 );
}

// fchs fpu reg stack
void FCHS( void )
{
    EmuRec_Write16( 0xE0D9 );
}

//*******************
// MMX instructions
//*******************

// r64 = mm

// movq m64 to r64
void MOVQMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0x6F0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// movq r64 to m64
void MOVQRtoM(EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0x7F0F );
    ModRM( 0, from, DISP32 );
    EmuRec_Write32( to);
}

// pand r64 to r64
void PANDRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xDB0F );
    ModRM( 3, to, from );
}

// por r64 to r64
void PORRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xEB0F );
    ModRM( 3, to, from );
}

// por m64 to r64
void PORMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xEB0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// pxor r64 to r64
void PXORRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xEF0F );
    ModRM( 3, to, from );
}

// psllq r64 to r64
void PSLLQRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xF30F );
    ModRM( 3, to, from );
}

// psllq m64 to r64
void PSLLQMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xF30F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// psllq imm8 to r64
void PSLLQItoR( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write16( 0x730F );
    ModRM( 3, 6, to);
    EmuRec_Write8( from );
}

// psrlq r64 to r64
void PSRLQRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xD30F );
    ModRM( 3, to, from );
}

// psrlq m64 to r64
void PSRLQMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xD30F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// psrlq imm8 to r64
void PSRLQItoR( EMU_U32 to, EMU_U08 from )
{
    EmuRec_Write16( 0x730F );
    ModRM( 3, 2, to);
    EmuRec_Write8( from );
}

// paddusb r64 to r64
void PADDUSBRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xDC0F );
    ModRM( 3, to, from );
}

// paddusb m64 to r64
void PADDUSBMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xDC0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// paddusw r64 to r64
void PADDUSWRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xDD0F );
    ModRM( 3, to, from );
}

// paddusw m64 to r64
void PADDUSWMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xDD0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// paddb r64 to r64
void PADDBRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xFC0F );
    ModRM( 3, to, from );
}

// paddb m64 to r64
void PADDBMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xFC0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// paddw r64 to r64
void PADDWRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xFD0F );
    ModRM( 3, to, from );
}

// paddw m64 to r64
void PADDWMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xFD0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// paddd r64 to r64
void PADDDRtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xFE0F );
    ModRM( 3, to, from );
}

// paddd m64 to r64
void PADDDMtoR( EMU_U32 to, EMU_U32 from )
{
    EmuRec_Write16( 0xFE0F );
    ModRM( 0, to, DISP32 );
    EmuRec_Write32( from );
}

// emms
void EMMS( void )
{
    EmuRec_Write16( 0x770F );
}
