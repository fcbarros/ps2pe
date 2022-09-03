// PS2PEDLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "common/EmuMain.h"

extern HINSTANCE hInst;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    static char CurrentDir[ 256 ] = { "\0" };

    hInst = (HINSTANCE)hModule;

    if ( strlen( CurrentDir ) <= 0 )
    {
       GetCurrentDirectory( sizeof( CurrentDir ), CurrentDir );
       //EmuSetDir( CurrentDir );
    }

    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }


    return TRUE;
}

