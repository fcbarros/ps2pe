#include "gs.h"

#include "EmuMain.h"
#include "Ps2Core.h"

#include <string>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// External plugins defines
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

typedef EMU_U32(CALLBACK* LIBTYPE)();
typedef EMU_U32(CALLBACK* LIBVERSION)();
typedef char* (CALLBACK* LIBNAME)();

// PS2EgetLibType returns 
#define PS2E_LT_GS   0x1
#define PS2E_LT_PAD  0x2
#define PS2E_LT_SPU2 0x4

void EmuSaveConfig();
void EmuLoadConfig();

char EmuDLLDir[256];

void EmuSetDir(const char* Dir, int size)
{
	memset(EmuDLLDir, 0, sizeof(EmuDLLDir));

	strncpy(EmuDLLDir, Dir, sizeof(EmuDLLDir) - 1);
}

#ifdef __WIN32__
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Windows
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


#include <io.h>

#include <windows.h>
#include <windowsx.h>
#pragma warning(disable:4786)

HINSTANCE hInst = NULL;

HMODULE GSdll = 0;
HMODULE PADdll = 0;

#pragma optimize( "", off )
double getClockFreq()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	double clock_freq = 1.0 / (double)frequency.QuadPart;
	return clock_freq;
}

// Calculates the frequency of the processor clock
DLLEXPORT double CALLBACK EmuGetClock()
{
	__int64 i64_perf_start, i64_perf_freq, i64_perf_end;
	__int64 i64_clock_start, i64_clock_end;
	double d_loop_period, d_clock_freq;

	QueryPerformanceFrequency((LARGE_INTEGER*)&i64_perf_freq);

	QueryPerformanceCounter((LARGE_INTEGER*)&i64_perf_start);
	i64_perf_end = 0;

	RDTSC(i64_clock_start);
	while (i64_perf_end < i64_perf_start + 250000)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&i64_perf_end);
	}
	RDTSC(i64_clock_end);

	i64_clock_end -= i64_clock_start;

	d_loop_period = ((double)(i64_perf_freq)) / 250000.0;
	d_clock_freq = ((double)(i64_clock_end & 0xffffffff)) * d_loop_period;

	double freq2 = getClockFreq();

	return d_clock_freq;
}
#pragma optimize( "", on )

BOOL CALLBACK EmuConfigureDlgProc(HWND hW, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HMODULE Hdll = 0;
	HWND hWCGS,
		hWCPAD,
		hWCSPU;
	long DllSearch;
	struct _finddata_t fileinfo;
	LIBTYPE LibType;
	LIBVERSION LibVersion;
	LIBNAME LibName;
	EMU_I32 GSidx = 0,
		PADidx = 0,
		SPUidx = 0;
	EMU_U32 Version, Revision, Build;
	char Buffer[256];
	static std::vector<std::string> gsPlugins;
	static std::vector<std::string> padPlugins;
	static std::vector<std::string> spuPlugins;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetCurrentDirectory(EmuDLLDir);
		gsPlugins.clear();
		padPlugins.clear();
		spuPlugins.clear();
		EmuLoadConfig();

		hWCGS = GetDlgItem(hW, IDC_COMBO_GS);
		hWCPAD = GetDlgItem(hW, IDC_COMBO_PAD);
		hWCSPU = GetDlgItem(hW, IDC_COMBO_SPU);

		SetCurrentDirectory("plugins");
		DllSearch = _findfirst("*.dll", &fileinfo);
		while (DllSearch > 0)
		{
			Hdll = LoadLibrary(fileinfo.name);
			if (Hdll)
			{
				LibType = (LIBTYPE)GetProcAddress(Hdll, "PS2EgetLibType");
				LibName = (LIBNAME)GetProcAddress(Hdll, "PS2EgetLibName");
				LibVersion = (LIBVERSION)GetProcAddress(Hdll, "PS2EgetLibVersion");
				if (LibType)
				{
					switch (LibType())
					{
					case PS2E_LT_GS:
						Version = (LibVersion() >> 16);
						Revision = (LibVersion() >> 8) & 0xFF;
						Build = (LibVersion()) & 0xFF;
						sprintf(Buffer, "%s v%u.%u.%u", LibName(), Version, Revision, Build);
						ComboBox_AddString(hWCGS, Buffer);
						if (!strcmp(fileinfo.name, Common::Ps2Core::GetInstance().GetGSFileName()))
						{
							GSidx = gsPlugins.size();
						}
						gsPlugins.push_back(fileinfo.name);
						break;

					case PS2E_LT_PAD:
						Version = (LibVersion() >> 16);
						Revision = (LibVersion() >> 8) & 0xFF;
						Build = (LibVersion()) & 0xFF;
						sprintf(Buffer, "%s v%u.%u.%u", LibName(), Version, Revision, Build);
						ComboBox_AddString(hWCPAD, Buffer);
						if (!strcmp(fileinfo.name, Common::Ps2Core::GetInstance().GetPADFileName()))
						{
							PADidx = padPlugins.size();
						}
						padPlugins.push_back(fileinfo.name);
						break;

					case PS2E_LT_SPU2:
						Version = (LibVersion() >> 16);
						Revision = (LibVersion() >> 8) & 0xFF;
						Build = (LibVersion()) & 0xFF;
						sprintf(Buffer, "%s v%u.%u.%u", LibName(), Version, Revision, Build);
						ComboBox_AddString(hWCSPU, Buffer);
						if (!strcmp(fileinfo.name, Common::Ps2Core::GetInstance().GetSPUFileName()))
						{
							SPUidx = spuPlugins.size();
						}
						spuPlugins.push_back(fileinfo.name);
						break;
					}
				}
				FreeLibrary(Hdll);
			}
			if (_findnext(DllSearch, &fileinfo))
			{
				break;
			}
		}
		ComboBox_SetCurSel(hWCGS, GSidx);
		ComboBox_SetCurSel(hWCPAD, PADidx);
		ComboBox_SetCurSel(hWCSPU, SPUidx);
		SetCurrentDirectory(EmuDLLDir);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hW, TRUE);
			return TRUE;

		case IDOK:
			hWCGS = GetDlgItem(hW, IDC_COMBO_GS);
			hWCPAD = GetDlgItem(hW, IDC_COMBO_PAD);
			hWCSPU = GetDlgItem(hW, IDC_COMBO_SPU);
			Common::Ps2Core::GetInstance().SetGSFileName(gsPlugins[ComboBox_GetCurSel(hWCGS)].c_str());
			Common::Ps2Core::GetInstance().SetPADFileName(padPlugins[ComboBox_GetCurSel(hWCPAD)].c_str());
			Common::Ps2Core::GetInstance().SetSPUFileName(spuPlugins[ComboBox_GetCurSel(hWCSPU)].c_str());

			EmuSaveConfig();
			EmuLoadConfig();
			EmuReleasePlugins();
			EmuLoadPlugins();
			EndDialog(hW, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

DLLEXPORT void CALLBACK EmuConfig()
{
	DialogBox(hInst,
		MAKEINTRESOURCE(IDD_CONFIG),
		GetActiveWindow(),
		(DLGPROC)EmuConfigureDlgProc);
}

#define GetKeyV( name, var, s, t ) \
            size = s; type = t; \
            RegQueryValueEx(myKey, name, 0, &type, (LPBYTE) var, &size);

#define GetKeyVdw( name, var ) \
            GetKeyV( name, var, 4, REG_DWORD );

#define SetKeyV( name, var, s, t ) \
            RegSetValueEx( myKey, name, 0, t, (LPBYTE) var, s );

#define SetKeyVdw( name, var ) \
	        SetKeyV( name, var, 4, REG_DWORD );

void EmuSaveConfig()
{
	HKEY myKey;
	DWORD myDisp;

	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\PS2PE", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &myKey, &myDisp);

	const char* GSFileName = Common::Ps2Core::GetInstance().GetGSFileName();
	const char* PADFileName = Common::Ps2Core::GetInstance().GetPADFileName();
	const char* SPUFileName = Common::Ps2Core::GetInstance().GetSPUFileName();
	const char* BiosFileName = Common::Ps2Core::GetInstance().GetBiosFileName();

	SetKeyV("GSplugin", GSFileName, strlen(GSFileName) + 1, REG_SZ);
	SetKeyV("PADplugin", PADFileName, strlen(PADFileName) + 1, REG_SZ);
	SetKeyV("SPUplugin", SPUFileName, strlen(SPUFileName) + 1, REG_SZ);
	SetKeyV("BIOSfile", BiosFileName, strlen(BiosFileName) + 1, REG_SZ);

	RegCloseKey(myKey);
}

void EmuLoadConfig()
{
	HKEY myKey;
	DWORD type, size;
	char Buffer[256];

	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\PS2PE", 0, KEY_ALL_ACCESS, &myKey) != ERROR_SUCCESS)
	{
		MessageBox(0, "PS2PE needs to be configured", "First Time", MB_OK);
		EmuSaveConfig();
		EmuConfig();
		return;
	}

	GetKeyV("GSplugin", Buffer, sizeof(Buffer), REG_SZ);
	Common::Ps2Core::GetInstance().SetGSFileName(Buffer);
	GetKeyV("PADplugin", Buffer, sizeof(Buffer), REG_SZ);
	Common::Ps2Core::GetInstance().SetPADFileName(Buffer);
	GetKeyV("SPUplugin", Buffer, sizeof(Buffer), REG_SZ);
	Common::Ps2Core::GetInstance().SetSPUFileName(Buffer);
	GetKeyV("BIOSfile", Buffer, sizeof(Buffer), REG_SZ);
	Common::Ps2Core::GetInstance().SetBiosFileName(Buffer);

	RegCloseKey(myKey);
}

void EmuReleasePlugins()
{
	if (GSdll)
	{
		FreeLibrary(GSdll);
		GSdll = 0;
	}
	if (PADdll)
	{
		FreeLibrary(PADdll);
		PADdll = 0;
	}
}

void EmuLoadPlugins()
{
	char LibraryPath[1024];

	SetCurrentDirectory(EmuDLLDir);
	if (!GSdll)
	{
		strcpy(LibraryPath, "plugins\\");
		strcat(LibraryPath, Common::Ps2Core::GetInstance().GetGSFileName());
		GSdll = LoadLibrary(LibraryPath);

		//GSinit = (_GSinit)GetProcAddress(GSdll, "GSinit");
		//GSopen = (_GSopen)GetProcAddress(GSdll, "GSopen");
		//GSclose = (_GSclose)GetProcAddress(GSdll, "GSclose");
		//GSshutdown = (_GSshutdown)GetProcAddress(GSdll, "GSshutdown");
		//GSvsync = (_GSvsync)GetProcAddress(GSdll, "GSvsync");
		//GSwrite32 = (_GSwrite32)GetProcAddress(GSdll, "GSwrite32");
		//GSwrite64 = (_GSwrite64)GetProcAddress(GSdll, "GSwrite64");
		//GSread32 = (_GSread32)GetProcAddress(GSdll, "GSread32");
		//GSread64 = (_GSread64)GetProcAddress(GSdll, "GSread64");
		//GSgifTransfer = (_GSgifTransfer)GetProcAddress(GSdll, "GSgifTransfer");
		//GSkeyEvent = (_GSkeyEvent)GetProcAddress(GSdll, "GSkeyEvent");
		//GSmakeSnapshot = (_GSmakeSnapshot)GetProcAddress(GSdll, "GSmakeSnapshot");
		//GSconfigure = (_GSconfigure)GetProcAddress(GSdll, "GSconfigure");
		//GStest = (_GStest)GetProcAddress(GSdll, "GStest");
		//GSabout = (_GSabout)GetProcAddress(GSdll, "GSabout");
		//GSwritePReg = (_GSwritePReg)GetProcAddress(GSdll, "GSwritePReg");
		//GSpreg = (_GSpreg)GetProcAddress(GSdll, "GSpreg");

		Common::Gs::GSinit = (Common::GS_GSinit)GetProcAddress(GSdll, "GSinit");
		Common::Gs::GSopen = (Common::GS_GSopen)GetProcAddress(GSdll, "GSopen");
		Common::Gs::GSclose = (Common::GS_GSclose)GetProcAddress(GSdll, "GSclose");
		Common::Gs::GSshutdown = (Common::GS_GSshutdown)GetProcAddress(GSdll, "GSshutdown");
		Common::Gs::GSvsync = (Common::GS_GSvsync)GetProcAddress(GSdll, "GSvsync");
		Common::Gs::GSwrite32 = (Common::GS_GSwrite32)GetProcAddress(GSdll, "GSwrite32");
		Common::Gs::GSwrite64 = (Common::GS_GSwrite64)GetProcAddress(GSdll, "GSwrite64");
		Common::Gs::GSread32 = (Common::GS_GSread32)GetProcAddress(GSdll, "GSread32");
		Common::Gs::GSread64 = (Common::GS_GSread64)GetProcAddress(GSdll, "GSread64");
		Common::Gs::GSgifTransfer = (Common::GS_GSgifTransfer)GetProcAddress(GSdll, "GSgifTransfer");
		Common::Gs::GSkeyEvent = (Common::GS_GSkeyEvent)GetProcAddress(GSdll, "GSkeyEvent");
		Common::Gs::GSmakeSnapshot = (Common::GS_GSmakeSnapshot)GetProcAddress(GSdll, "GSmakeSnapshot");
		Common::Gs::GSconfigure = (Common::GS_GSconfigure)GetProcAddress(GSdll, "GSconfigure");
		Common::Gs::GStest = (Common::GS_GStest)GetProcAddress(GSdll, "GStest");
		Common::Gs::GSabout = (Common::GS_GSabout)GetProcAddress(GSdll, "GSabout");
		Common::Gs::GSwritePReg = (Common::GS_GSwritePReg)GetProcAddress(GSdll, "GSwritePReg");
		Common::Gs::GSpreg = (Common::GS_GSpreg)GetProcAddress(GSdll, "GSpreg");
	}

	if (!PADdll)
	{
		strcpy(LibraryPath, "plugins\\");
		strcat(LibraryPath, Common::Ps2Core::GetInstance().GetPADFileName());
		PADdll = LoadLibrary(LibraryPath);

		//PAD1init = (_PADinit)GetProcAddress(PADdll, "PADinit");
		//PAD1open = (_PADopen)GetProcAddress(PADdll, "PADopen");
		//PAD1close = (_PADclose)GetProcAddress(PADdll, "PADclose");
		//PAD1shutdown = (_PADshutdown)GetProcAddress(PADdll, "PADshutdown");
		//PAD1keyEvent = (_PADkeyEvent)GetProcAddress(PADdll, "PADkeyEvent");
		//PAD1readStatus = (_PADreadStatus)GetProcAddress(PADdll, "PADreadStatus");
		//PAD1query = (_PADquery)GetProcAddress(PADdll, "PADquery");
		//PAD1configure = (_PADconfigure)GetProcAddress(PADdll, "PADconfigure");
		//PAD1test = (_PADtest)GetProcAddress(PADdll, "PADtest");
		//PAD1about = (_PADabout)GetProcAddress(PADdll, "PADabout");

		Common::Pad::PAD1init = (Common::_PADinit)GetProcAddress(PADdll, "PADinit");
		Common::Pad::PAD1open = (Common::_PADopen)GetProcAddress(PADdll, "PADopen");
		Common::Pad::PAD1close = (Common::_PADclose)GetProcAddress(PADdll, "PADclose");
		Common::Pad::PAD1shutdown = (Common::_PADshutdown)GetProcAddress(PADdll, "PADshutdown");
		Common::Pad::PAD1keyEvent = (Common::_PADkeyEvent)GetProcAddress(PADdll, "PADkeyEvent");
		Common::Pad::PAD1readStatus = (Common::_PADreadStatus)GetProcAddress(PADdll, "PADreadStatus");
		Common::Pad::PAD1query = (Common::_PADquery)GetProcAddress(PADdll, "PADquery");
		Common::Pad::PAD1configure = (Common::_PADconfigure)GetProcAddress(PADdll, "PADconfigure");
		Common::Pad::PAD1test = (Common::_PADtest)GetProcAddress(PADdll, "PADtest");
		Common::Pad::PAD1about = (Common::_PADabout)GetProcAddress(PADdll, "PADabout");
	}
}

#else
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Linux
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


// Calculates the frequency of the processor clock
DLLEXPORT double CALLBACK EmuGetClock()
{
	return 0.0;
}

// Loads the Config File
void EmuLoadConfig()
{
}

// Saves the Config File
void EmuSaveConfig()
{
}

// Opens a dialog for the Emu configuration
DLLEXPORT void CALLBACK EmuConfig()
{
}

void EmuLoadPlugins()
{
}

#endif
