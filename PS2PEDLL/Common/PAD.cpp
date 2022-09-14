#include "PAD.h"

#include <windows.h>

namespace Common
{
    EMU_I32 DLLCALL DummyPADinit(EMU_U32 flags) { return 0; }
    EMU_I32 DLLCALL DummyPADopen(void* pDsp) { return 0; }
    void DLLCALL DummyPADclose() {}
    void DLLCALL DummyPADshutdown() {}
    keyEvent* DLLCALL DummyPADkeyEvent() { return nullptr; }
    void DLLCALL DummyPADreadStatus(padButtonStatus* data, int pad) {}
    EMU_U32 DLLCALL DummyPADquery() { return 0; }
    void DLLCALL DummyPADconfigure() {}
    EMU_I32 DLLCALL DummyPADtest() { return 0; }
    void DLLCALL DummyPADabout() {}

    _PADinit Pad::PAD1init = &DummyPADinit;
    _PADopen Pad::PAD1open = &DummyPADopen;
    _PADclose Pad::PAD1close = &DummyPADclose;
    _PADshutdown Pad::PAD1shutdown = &DummyPADshutdown;
    _PADkeyEvent Pad::PAD1keyEvent = &DummyPADkeyEvent;
    _PADreadStatus Pad::PAD1readStatus = &DummyPADreadStatus;
    _PADquery Pad::PAD1query = &DummyPADquery;
    _PADconfigure Pad::PAD1configure = &DummyPADconfigure;
    _PADtest Pad::PAD1test = &DummyPADtest;
    _PADabout Pad::PAD1about = &DummyPADabout;

    Pad::Pad()
    {
    }

    Pad& Pad::GetInstance()
    {
        static Pad pad;
        return pad;
    }

    void Pad::Init(void)
    {
    }

    void Pad::Reset(void)
    {
    }

    void Pad::Config(void)
    {
        PAD1configure();
    }

    padButtonStatus*& Pad::GetPadStat(int i)
    {
        return PadStat[i];
    }

    pad_data*& Pad::GetPadArea(int i1, int i2)
    {
        return PadArea[i1][i2];
    }

    xpad_data*& Pad::GetXPadArea(int i1, int i2)
    {
        return XPadArea[i1][i2];
    }

    void Emu_PAD_Bios_ReadStatus(EMU_I32 pad)
    {
        int i = pad - 1;

        padButtonStatus* padStat = Pad::GetInstance().GetPadStat(i);
        pad_data* padArea = Pad::GetInstance().GetPadArea(i, 0);
        xpad_data* xPadArea = Pad::GetInstance().GetXPadArea(i, 0);

        if (padStat != NULL)
        {
            if (padArea != NULL)
            {
                padArea->state = Pad::PAD_STATE_STABLE;
                padArea->reqState = Pad::PAD_RSTAT_COMPLETE;
            }
            if (xPadArea != NULL)
            {
                xPadArea->state = Pad::PAD_STATE_STABLE;
                xPadArea->reqState = Pad::PAD_RSTAT_COMPLETE;
            }
            Pad::GetInstance().PAD1readStatus(padStat, pad);
        }
    }

}