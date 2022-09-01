
#include <windows.h>

#include "EmuMain.h"
#include "EmuPAD.h"

// PAD1
_PADinit        PAD1init;
_PADopen        PAD1open;
_PADclose       PAD1close;
_PADshutdown    PAD1shutdown;
_PADkeyEvent    PAD1keyEvent;
_PADreadStatus  PAD1readStatus;
_PADquery       PAD1query;

_PADconfigure   PAD1configure;
_PADtest        PAD1test;
_PADabout       PAD1about;

// PAD2
_PADinit        PAD2init;
_PADopen        PAD2open;
_PADclose       PAD2close;
_PADshutdown    PAD2shutdown;
_PADkeyEvent    PAD2keyEvent;
_PADreadStatus  PAD2readStatus;
_PADquery       PAD2query;

_PADconfigure   PAD2configure;
_PADtest        PAD2test;
_PADabout       PAD2about;

pad_data * PadArea[2][2];
xpad_data * XPadArea[2][2];

padButtonStatus * PadStat[2] = { NULL, NULL };

void Emu_PAD_Init( void )
{
}

void Emu_PAD_Reset( void )
{
}

void Emu_PAD_Config( void )
{
	PAD1configure( );
}

void Emu_PAD_Bios_ReadStatus( EMU_I32 pad )
{
    int i = pad-1;
    if ( PadStat[i] != NULL ) 
    {
        if ( PadArea[i][0] != NULL ) 
        {
            PadArea[i][0]->state = PAD_STATE_STABLE;
            PadArea[i][0]->reqState = PAD_RSTAT_COMPLETE;
        }
        if ( XPadArea[i][0] != NULL )
        {
            XPadArea[i][0]->state = PAD_STATE_STABLE;
            XPadArea[i][0]->reqState = PAD_RSTAT_COMPLETE;
        }
        PAD1readStatus( (padButtonStatus*)PadStat[i], pad );
    }
}

