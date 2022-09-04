
#ifdef __WIN32__
    #pragma warning(disable:4786)
#endif

#include "EmuMain.h"
#include "EmuGS.h"
#include "EmuRecompiler.h"
#include "EmuRecX86.h"

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

stEmuRecMemory RecMemory;

stEmuRecReg RecRegularRegisters[ 6 ];

stEmuRecEmuReg RecR5900Regs[ 32 ];

EMU_U32 CurrentPCAddress;

stEmuRecAddressTranslationTable EmuRecAddressTranslationTable;

stEmuRecPendingJump JumpTable;

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Writing functions
void EmuRec_Write8( EMU_U08 Data )
{
    RecMemory.Memory[ RecMemory.Position ] = Data;
    RecMemory.Position++;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_Write16( EMU_U16 Data )
{
    *((EMU_U16*)&RecMemory.Memory[ RecMemory.Position ]) = Data;
    RecMemory.Position += 2;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_Write32( EMU_U32 Data )
{
    *((EMU_U32*)&RecMemory.Memory[ RecMemory.Position ]) = Data;
    RecMemory.Position += 4;
}

///////////////////////////////////////////////////////////////////////
EMU_U64 EmuRec_GetTime( void )
{
    EMU_U64 Ticks;
    RDTSC( Ticks );
    return Ticks;
}

///////////////////////////////////////////////////////////////////////
EMU_I32 EmuRec_GetCPURegIndex( EMU_U32 Code )
{
    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        if ( RecRegularRegisters[ Index ].Code == Code )
        {
            return Index;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_FlushReg( EMU_U32 Index )
{
    EMU_U32 RegIndex = RecRegularRegisters[ Index ].MainIndex;
    EMU_U32 U32Index = RecRegularRegisters[ Index ].SecIndex;
    EMU_U32 Register = RecRegularRegisters[ Index ].Code;
    MOV32RtoM( (EMU_U32)&R5900Regs.Reg[ RegIndex ].u32[ U32Index ], Register );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_FlushCPUReg( EMU_U32 Code )
{
    EMU_I32 Index = EmuRec_GetCPURegIndex( Code );
    if ( Index >= 0 )
    {
        if ( ( RecRegularRegisters[ Index ].Used ) &&
             ( RecRegularRegisters[ Index ].Modified ) )
        {
            EmuRec_FlushReg( Index );
            RecRegularRegisters[ Index ].Modified = FALSE;
        }
    }
}

///////////////////////////////////////////////////////////////////////
void EmuRec_SetReg( EMU_U32 Index )
{
    EMU_U32 RegIndex = RecRegularRegisters[ Index ].MainIndex;
    EMU_U32 U32Index = RecRegularRegisters[ Index ].SecIndex;
    EMU_U32 Register = RecRegularRegisters[ Index ].Code;
    MOV32MtoR( Register, (EMU_U32)&R5900Regs.Reg[ RegIndex ].u32[ U32Index ] );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_Reset( void )
{
    EMU_U32 Code[ 6 ] = { EAX, EBX, ECX, EDX, ESI, EDI };
    EMU_U08 Index;
    for ( Index = 0; Index < 6; Index++ )
    {
        RecRegularRegisters[ Index ].Used = FALSE;
        RecRegularRegisters[ Index ].Modified = FALSE;
        RecRegularRegisters[ Index ].LastAccessed = 0;
        RecRegularRegisters[ Index ].Code = Code[ Index ];
        RecRegularRegisters[ Index ].MainIndex = -1;
        RecRegularRegisters[ Index ].SecIndex = -1;
    }
    for ( Index = 0; Index < 32; Index++ )
    {
        RecR5900Regs[ Index ].Register[ 0 ] = -1;
        RecR5900Regs[ Index ].Register[ 1 ] = -1;
        RecR5900Regs[ Index ].Register[ 2 ] = -1;
        RecR5900Regs[ Index ].Register[ 3 ] = -1;
    }
    memset( RecMemory.Memory, 0, RecMemory.Size );
    RecMemory.Position = 0;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_Init( EMU_U32 Size )
{
    RecMemory.Size = Size;
    RecMemory.Memory = (EMU_U08*)malloc( RecMemory.Size );
    EmuRec_Reset( );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_Shutdown( void )
{
    free( RecMemory.Memory );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_FreeIntReg( EMU_U32 Register )
{
    if ( Register < 6 )
    {
        if ( RecRegularRegisters[ Register ].Modified )
        {
            EmuRec_FlushReg( Register );
            RecRegularRegisters[ Register ].Modified = FALSE;
        }
        RecR5900Regs[ RecRegularRegisters[ Register ].MainIndex ].Register[ RecRegularRegisters[ Register ].SecIndex ] = -1;
    
        RecRegularRegisters[ Register ].LastAccessed = 0;
        RecRegularRegisters[ Register ].MainIndex = -1;
        RecRegularRegisters[ Register ].SecIndex = -1;
        RecRegularRegisters[ Register ].Used = FALSE;
    }
}

///////////////////////////////////////////////////////////////////////
void EmuRec_FreeCPUReg( EMU_U32 Code )
{
    EMU_I32 Index = EmuRec_GetCPURegIndex( Code );
    if ( Index >= 0 )
    {
        EmuRec_FreeIntReg( Index );
    }
}

///////////////////////////////////////////////////////////////////////
BOOL EmuRec_CheckRegUse( EMU_U32 Code )
{
    EMU_I32 Index = EmuRec_GetCPURegIndex( Code );
    if ( Index >= 0 )
    {
        return RecRegularRegisters[ Index ].Used;
    }
    else
    {
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////////
EMU_I32 EmuRec_CheckIntRegCache( EMU_U32 R5900Register, EMU_U32 U32Index )
{
    if ( RecR5900Regs[ R5900Register ].Register[ U32Index ] >= 0 )
    {
        return RecRegularRegisters[ RecR5900Regs[ R5900Register ].Register[ U32Index ] ].Code;
    }
    else
    {
        return -1;
    }
}


///////////////////////////////////////////////////////////////////////
EMU_U32 EmuRec_FreeAnyIntReg( void )
{
    EMU_U64 OlderTime = 0xFFFFFFFFFFFFFFFF;
    EMU_U32 MainIndex = 6;
    EMU_U32 Index;

    for ( Index = 0; Index < 6; Index++ )
    {
        if ( RecRegularRegisters[ Index ].Used )
        {
            if ( RecRegularRegisters[ Index ].LastAccessed < OlderTime )
            {
                OlderTime = RecRegularRegisters[ Index ].LastAccessed;
                MainIndex = Index;
            }
        }
    }
    EmuRec_FreeIntReg( MainIndex );

    return MainIndex;
}

///////////////////////////////////////////////////////////////////////
EMU_U32 EmuRec_CacheReg( EMU_U32 R5900Register, EMU_U32 U32Index, BOOL ExecuteMove )
{
    EMU_U32 Index;
    for ( Index = 0; Index < 6; Index++ )
    {
        if ( ! RecRegularRegisters[ Index ].Used )
        {
            break;
        }
    }
    if ( Index == 6 )
    {
        Index = EmuRec_FreeAnyIntReg( );
    }

    RecR5900Regs[ R5900Register ].Register[ U32Index ] = Index;
    RecRegularRegisters[ Index ].Used = TRUE;
    RecRegularRegisters[ Index ].MainIndex = R5900Register;
    RecRegularRegisters[ Index ].SecIndex = U32Index;

    if ( ExecuteMove )
    {
        EmuRec_SetReg( Index );
    }
    RecRegularRegisters[ Index ].LastAccessed = EmuRec_GetTime( );

    return Index;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_FreeAll( void )
{
    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        EmuRec_FreeIntReg( Index );
    }
    MOV32RtoM( (EMU_U32)&COP0Regs.Count, EBP );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_FlushAll( void )
{
    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        if ( RecRegularRegisters[ Index ].Used &&
             RecRegularRegisters[ Index ].Modified )
        {
            EmuRec_FlushReg( Index );
        }
    }
    MOV32RtoM( (EMU_U32)&COP0Regs.Count, EBP );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_PushAll( void )
{
    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        if ( RecRegularRegisters[ Index ].Used )
        {
            PUSH32R( RecRegularRegisters[ Index ].Code );
        }
    }
    PUSH32R( EBP );
}

///////////////////////////////////////////////////////////////////////
void EmuRec_PopAll( void )
{
    POP32R( EBP );
    for ( EMU_I32 Index = 5; Index >= 0; Index-- )
    {
        if ( RecRegularRegisters[ Index ].Used )
        {
            POP32R( RecRegularRegisters[ Index ].Code );
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Gets an existing struct if the register is cached or an avaiable one or returns NULL
EMU_U32 EmuRec_GetIntReg( EMU_U32 R5900Register, EMU_U32 U32Index, BOOL ExecuteMove )
{
    if ( RecR5900Regs[ R5900Register ].Register[ U32Index ] >= 0 )
    {
        EMU_U08 Code = RecRegularRegisters[ RecR5900Regs[ R5900Register ].Register[ U32Index ] ].Code;
        return Code;
    }
    else
    {
        EMU_U08 Code = RecRegularRegisters[ EmuRec_CacheReg( R5900Register, U32Index, ExecuteMove ) ].Code;
        return Code;
    }
}

void EmuRec_SetIntRegModified( EMU_U32 Register )
{
    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        if ( RecRegularRegisters[ Index ].Code == Register )
        {
            RecRegularRegisters[ Index ].Modified = TRUE;
            return;
        }
    }
}

void EmuRec_SetIntRegTime( EMU_U32 Register )
{
    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        if ( RecRegularRegisters[ Index ].Code == Register )
        {
            RecRegularRegisters[ Index ].LastAccessed = EmuRec_GetTime( );
            return;
        }
    }
}
// 0x030fa17d
// Inserts Address in the Address Translation Table pointing to the memory
void EmuRec_InsertAddress( EMU_U32 Address, EMU_U08 * pMemory )
{
    stEmuRecInstruction &Temp = EmuRecAddressTranslationTable[ Address ];
    Temp.CPUAddress = pMemory;
    EmuRec_InsertDependency( Address );
}

// Checks if Address if currently in the Address Translation Table
EMU_U08 * EmuRec_CheckAddress( EMU_U32 Address )
{
    stEmuRecInstruction &Temp = EmuRecAddressTranslationTable[ Address ];
    return Temp.CPUAddress;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_ResolveRegDependency( EMU_U32 Address )
{
    stEmuRecInstruction &Temp = EmuRecAddressTranslationTable[ Address ];

    for ( EMU_U32 Index = 0; Index < 6; Index++ )
    {
        EMU_I32 CPUReg = Temp.RegDependency[ Index ].CPUIndex;
        if ( CPUReg >= 0 )
        {
           EMU_U32 Main = Temp.RegDependency[ Index ].Main;
           EMU_U32 Sec = Temp.RegDependency[ Index ].Secondary;
           EMU_U32 EmuIndex = EmuRec_GetCPURegIndex( CPUReg );
           if ( RecR5900Regs[ Main ].Register[ Sec ] != EmuIndex )
           {
               if ( RecR5900Regs[ Main ].Register[ Sec ] >= 0 )
               {
                   EmuRec_FreeIntReg( RecR5900Regs[ Main ].Register[ Sec ] );
               }
               if ( EmuIndex >= 0 )
               {
                   EmuRec_FreeIntReg( EmuIndex );
               }
               RecR5900Regs[ Main ].Register[ Sec ] = EmuIndex;
               RecRegularRegisters[ EmuIndex ].Used = TRUE;
               RecRegularRegisters[ EmuIndex ].MainIndex = Main;
               RecRegularRegisters[ EmuIndex ].SecIndex = Sec;
               EmuRec_SetReg( EmuIndex );
               RecRegularRegisters[ EmuIndex ].LastAccessed = EmuRec_GetTime( );
           }
        }
        else
        {
            EmuRec_FreeIntReg( Index );
        }
    }
}

///////////////////////////////////////////////////////////////////////
void EmuRec_InsertRegDependency( EMU_U32 Address, EMU_U32 Index )
{
    stEmuRecInstruction &Temp = EmuRecAddressTranslationTable[ Address ];

    Temp.RegDependency[ Index ].Main = RecRegularRegisters[ Index ].MainIndex; 
    Temp.RegDependency[ Index ].Secondary = RecRegularRegisters[ Index ].SecIndex;
    Temp.RegDependency[ Index ].CPUIndex = RecRegularRegisters[ Index ].Code;
}

///////////////////////////////////////////////////////////////////////
void EmuRec_InsertDependency( EMU_U32 Address )
{
   for ( EMU_U32 Index = 0; Index < 6; Index++ )
   {
       if ( RecRegularRegisters[ Index ].Used )
       {
           EmuRec_InsertRegDependency( Address, Index );
       }
   }
}

// Returns the last item in the Pendign Jump Tables
stEmuRecJumpRecord * EmuRec_CheckPendingJumps( void )
{
    if ( JumpTable.size() > 0 )
    {
        return &JumpTable[ JumpTable.size( ) - 1 ];
    }
    else
    {
        return NULL;
    }
}

// Removes the last item in the Pending Jump Table
void EmuRec_PopPendingJump( void )
{
    JumpTable.pop_back( );
}

// Removes the last item in the Pending Jump Table
void EmuRec_PushPendingJump( EMU_U32 * LinkAddress, EMU_U32 JumpTo )
{
    stEmuRecJumpRecord Temp;
    Temp.EmuTargetAddress = JumpTo;
    Temp.LinkAddress = LinkAddress;

    JumpTable.push_back( Temp );
}

EMU_U08 * EmuRec_CurrentAddress( void )
{
    return &RecMemory.Memory[ RecMemory.Position ];
}

