/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// File: EmuRecX86.h
// Version: 1.000
// Desc: Main Header File for the Recompiler
//
// Author: GoldFinger
// Created: 09/07/2002
// Modifications:
// * Version 1.000 ( 09/07/2002 ): Created
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef __EMU_RECX86_H__
#define __EMU_RECX86_H__

#include "EmuPs2Core.h"

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#define EAX 0
#define EBX 3
#define ECX 1
#define EDX 2
#define ESI 6
#define EDI 7
#define EBP 5
#define ESP 4

#define MM0 0
#define MM1 1
#define MM2 2
#define MM3 3
#define MM4 4
#define MM5 5
#define MM6 6
#define MM7 7

#define AL  0
#define CL  1
#define DL  2
#define BL  3
#define AH  4
#define CH  5
#define DH  6
#define BH  7

// Asm functions

// stack instructions
void PUSH32R( EMU_U32 from );
void PUSH32M( EMU_U32 from );
void PUSH32I( EMU_U32 from );
void POP32R( EMU_U32 from );
void PUSHA32( void );
void POPA32( void );
void RET( void );

// Sign Extend
void CWDE( void );
void CBW( void );

// Call
void CALLFunc( EMU_U32 func, EMU_U32 CurrentAddress );
EMU_U32 * CALLNearRel32( EMU_U32 to );

// Jumps
EMU_U08 * JMP8( EMU_U08 to );
EMU_U32 * JMP32( EMU_U32 to );
void JMP32R( EMU_U32 to );
void JMP32M( EMU_U32 to );

// Jcc
EMU_U08 * JA8( EMU_U08 to );
EMU_U08 * JAE8( EMU_U08 to );
EMU_U08 * JB8( EMU_U08 to );
EMU_U08 * JBE8( EMU_U08 to );
EMU_U08 * JC8( EMU_U08 to );
EMU_U08 * JNC8( EMU_U08 to );
EMU_U08 * JE8( EMU_U08 to );
EMU_U08 * JZ8( EMU_U08 to );
EMU_U08 * JG8( EMU_U08 to );
EMU_U08 * JGE8( EMU_U08 to );
EMU_U08 * JL8( EMU_U08 to );
EMU_U08 * JLE8( EMU_U08 to );
EMU_U08 * JNE8( EMU_U08 to );
EMU_U08 * JNZ8( EMU_U08 to );
EMU_U08 * JNG8( EMU_U08 to );
EMU_U08 * JNGE8( EMU_U08 to );
EMU_U08 * JNL8( EMU_U08 to );
EMU_U08 * JNLE8( EMU_U08 to );
EMU_U32 * JA32( EMU_U32 to );
EMU_U32 * JB32( EMU_U32 to );
EMU_U32 * JE32( EMU_U32 to );
EMU_U32 * JNE32( EMU_U32 to );
EMU_U32 * JG32( EMU_U32 to );
EMU_U32 * JLE32( EMU_U32 to );
EMU_U32 * JGE32( EMU_U32 to );
EMU_U32 * JL32( EMU_U32 to );

// Mov
void MOV32RtoR( EMU_U32 to, EMU_U32 from );
void MOV32RtoM( EMU_U32 to, EMU_U32 from );
void MOV32MtoR( EMU_U32 to, EMU_U32 from );
void MOV32RmtoR( EMU_U32 to, EMU_U32 from ); 
void MOV32RmStoR( EMU_U32 to, EMU_U32 from, EMU_U32 from2, EMU_U32 scale );
void MOV32RtoRm( EMU_U32 to, EMU_U32 from );
void MOV32ItoR( EMU_U32 to, EMU_U32 from );
void MOV32ItoM( EMU_U32 to, EMU_U32 from );

// CMov
void CMOVNE32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVNE32MtoR( EMU_U32 to, EMU_U32 from );
void CMOVE32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVE32MtoR( EMU_U32 to, EMU_U32 from );
void CMOVG32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVG32MtoR( EMU_U32 to, EMU_U32 from );
void CMOVGE32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVGE32MtoR( EMU_U32 to, EMU_U32 from );
void CMOVL32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVL32MtoR( EMU_U32 to, EMU_U32 from );
void CMOVLE32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVLE32MtoR( EMU_U32 to, EMU_U32 from );
void CMOVC32RtoR( EMU_U32 to, EMU_U32 from );
void CMOVC32MtoR( EMU_U32 to, EMU_U32 from );

// SetCC
void SETNGER( EMU_U32 to );
void SETSR( EMU_U32 to );
void SETNSR( EMU_U32 to );
void SETOR( EMU_U32 to );
void SETNOR( EMU_U32 to );
void SETCR( EMU_U32 to );
void SETNCR( EMU_U32 to );

// Compare
void CMP32ItoR( EMU_U32 to, EMU_U32 from );
void CMP32ItoM( EMU_U32 to, EMU_U32 from );
void CMP32RtoR( EMU_U32 to, EMU_U32 from );
void CMP32MtoR( EMU_U32 to, EMU_U32 from );
void TEST32ItoR( EMU_U32 to, EMU_U32 from );
void TEST32ItoM( EMU_U32 to, EMU_U32 from );
void TEST32RtoR( EMU_U32 to, EMU_U32 from ); 

// Aritmetic
void ADD8ItoR( EMU_U32 to, EMU_U08 from );
void ADD32ItoR( EMU_U32 to, EMU_U32 from );
void ADD8ItoM( EMU_U32 to, EMU_U08 from );
void ADD32ItoM( EMU_U32 to, EMU_U32 from );
void ADD32RtoR( EMU_U32 to, EMU_U32 from );
void ADD32MtoR( EMU_U32 to, EMU_U32 from );

void ADC32ItoR( EMU_U32 to, EMU_U32 from );
void ADC32RtoR( EMU_U32 to, EMU_U32 from );
void ADC32MtoR( EMU_U32 to, EMU_U32 from );

void SUB32ItoR( EMU_U32 to, EMU_U32 from );
void SUB32RtoR( EMU_U32 to, EMU_U32 from );
void SUB32MtoR( EMU_U32 to, EMU_U32 from );

void SBB32ItoR( EMU_U32 to, EMU_U32 from );
void SBB32RtoR( EMU_U32 to, EMU_U32 from );
void SBB32MtoR( EMU_U32 to, EMU_U32 from );

void MUL32R( EMU_U32 from );
void IMUL32R( EMU_U32 from );
void MUL32M( EMU_U32 from );
void IMUL32M( EMU_U32 from );

void DIV32R( EMU_U32 from );
void IDIV32R( EMU_U32 from );
void DIV32M( EMU_U32 from );
void IDIV32M( EMU_U32 from );

// Conditional moves
void CMOVSRtoR( EMU_U32 to, EMU_U32 from );
void CMOVSMtoR( EMU_U32 to, EMU_U32 from );
void CMOVNSRtoR( EMU_U32 to, EMU_U32 from );
void CMOVNSMtoR( EMU_U32 to, EMU_U32 from );

// shifting instructions
void SHLD32ItoR( EMU_U32 to, EMU_U32 from, EMU_U08 shift );
void SHRD32ItoR( EMU_U32 to, EMU_U32 from, EMU_U08 shift );
void SHL32ItoR( EMU_U32 to, EMU_U08 from );
void SHL32CLtoR( EMU_U32 to );
void SHR32ItoR( EMU_U32 to, EMU_U08 from );
void SHR32CLtoR( EMU_U32 to );
void SAR32ItoR( EMU_U32 to, EMU_U08 from );

// logical instructions
void OR32ItoR( EMU_U32 to, EMU_U32 from );
void OR32ItoM( EMU_U32 to, EMU_U32 from );
void OR32RtoR( EMU_U32 to, EMU_U32 from );
void OR32MtoR( EMU_U32 to, EMU_U32 from );
void XOR32ItoR( EMU_U32 to, EMU_U32 from );
void XOR32ItoM( EMU_U32 to, EMU_U32 from );
void XOR32RtoR( EMU_U32 to, EMU_U32 from );
void XOR32MtoR( EMU_U32 to, EMU_U32 from );
void AND32ItoR( EMU_U32 to, EMU_U32 from );
void AND32ItoM( EMU_U32 to, EMU_U32 from );
void AND32RtoR( EMU_U32 to, EMU_U32 from );
void AND32MtoR( EMU_U32 to, EMU_U32 from );
void NOT32R( EMU_U32 from );
void NEG32R( EMU_U32 from );

// FPU instructions
void FILD32( EMU_U32 from );
void FISTP32( EMU_U32 from );
void FLD32( EMU_U32 from );
void FSTP32( EMU_U32 to);
void FLDCW( EMU_U32 from );
void FNSTCW( EMU_U32 to);
void FADD32( EMU_U32 from );
void FSUB32( EMU_U32 from );
void FMUL32( EMU_U32 from );
void FDIV32( EMU_U32 from );
void FABS( void );
void FSQRT( void );
void FCHS( void );
void FCOMP32( EMU_U32 from );

// MMX instructions
void MOVQMtoR( EMU_U32 to, EMU_U32 from );
void MOVQRtoM( EMU_U32 to, EMU_U32 from );
void PANDRtoR( EMU_U32 to, EMU_U32 from );
void PORRtoR( EMU_U32 to, EMU_U32 from );
void PORMtoR( EMU_U32 to, EMU_U32 from );
void PXORRtoR( EMU_U32 to, EMU_U32 from );
void PSLLQRtoR( EMU_U32 to, EMU_U32 from );
void PSLLQMtoR( EMU_U32 to, EMU_U32 from );
void PSLLQItoR( EMU_U32 to, EMU_U08 from );
void PSRLQRtoR( EMU_U32 to, EMU_U32 from );
void PSRLQMtoR( EMU_U32 to, EMU_U32 from );
void PSRLQItoR( EMU_U32 to, EMU_U08 from );
void PADDUSBRtoR( EMU_U32 to, EMU_U32 from );
void PADDUSBMtoR( EMU_U32 to, EMU_U32 from );
void PADDUSWRtoR( EMU_U32 to, EMU_U32 from );
void PADDUSWMtoR( EMU_U32 to, EMU_U32 from );
void PADDBRtoR( EMU_U32 to, EMU_U32 from );
void PADDBMtoR( EMU_U32 to, EMU_U32 from );
void PADDWRtoR( EMU_U32 to, EMU_U32 from );
void PADDWMtoR( EMU_U32 to, EMU_U32 from );
void PADDDRtoR( EMU_U32 to, EMU_U32 from );
void PADDDMtoR( EMU_U32 to, EMU_U32 from );
void EMMS( void );

#endif
