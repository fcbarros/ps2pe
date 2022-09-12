// PSOPS2Dlg.cpp : implementation file
//
#include "pch.h"
#include "stdafx.h"
#include "PSOPS2.h"
#include "PSOPS2Dlg.h"

#include "ps2pe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool EmuRunning = false;
HMODULE EmuDll;
double CPUClock;
EMU_U32 TotalInstructions;

Emu_R5900_Regs* R5900Regs;
Emu_COP0_Regs* COP0Regs;
Emu_COP1_Regs* COP1Regs;
EMU_VU_Regs* VU0Regs;
EMU_VU_Regs* VU1Regs;

EMUCONFIG EmuConfig;
EMUSETCONSOLECALLBACK EmuSetConsoleCallback;
EMUGETCLOCK EmuGetClock;
EMUINITIALIZE EmuInitialize;
EMURELEASE EmuRelease;
EMUGETTOTALINSTRUCTIONS EmuGetTotalInstructions;
EMUSETBIOSFILE EmuSetBiosFile;
EMUGETINSTRUCTIONSSTATS EmuGetInstructionsStats;
EMUGETR5900REGNAME EmuGetR5900RegName;
EMUGETCOP0REGNAME EmuGetCOP0RegName;
EMUGETCOP1REGNAME EmuGetCOP1RegName;
EMUGETCOP2FPREGNAME EmuGetCOP2FPRegName;
EMUGETCOP2IPREGNAME EmuGetCOP2IPRegName;
EMUGETREGS EmuGetRegs;
EMUISBREAKPOINT EmuIsBreakPoint;
EMUGETBYTE EmuGetByte;
EMUGETWORD EmuGetWord;
EMUGETDISASSEMBLYSYMBOL EmuGetDisassemblySymbol;
EMUDISASSEMBLY EmuDisassembly;
EMULOAD EmuLoad;
EMURUN EmuRun;
EMUEXECUTE EmuExecute;
EMUSTEPOVER EmuStepOver;
EMUSTEPINTO EmuStepInto;
EMUADDBREAKPOINT EmuAddBreakPoint;
EMUREMOVEBREAKPOINT EmuRemoveBreakPoint;
EMUGSINITWINDOW EmuGSInitWindow;
EMUGSCONFIGURE EmuGSConfigure;
EMUGSCLOSEWINDOW EmuGSCloseWindow;
EMUADDINSTRUCTIONBREAKPOINT EmuAddInstructionBreakpoint;
EMUREMOVEINSTRUCTIONBREAKPOINT EmuRemoveInstructionBreakpoint;
EMUISINSTRUCTIONBREAKPOINT EmuIsInstructionBreakpoint;
EMUGETINSTRUCTIONINFO EmuGetInstructionInfo;
EMUGETRUNNINGSTATS EmuGetRunningStats;
EMUGETLOADEDSTATS EmuGetLoadedStats;
EMURESET EmuReset;
EMUFINDLABEL EmuFindLabel;
EMUPADCONFIG EmuPADConfig;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
		//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSOPS2Dlg dialog

CPSOPS2Dlg::CPSOPS2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSOPS2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPSOPS2Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_PS2PE);
}

void CPSOPS2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPSOPS2Dlg)
	DDX_Control(pDX, IDC_LIST_CONSOLE, m_Edit_Console);
	DDX_Control(pDX, IDC_LIST_COP2_4, m_List_COP2_4);
	DDX_Control(pDX, IDC_LIST_COP2_3, m_List_COP2_3);
	DDX_Control(pDX, IDC_LIST_COP2_2, m_List_COP2_2);
	DDX_Control(pDX, IDC_LIST_COP2_1, m_List_COP2_1);
	DDX_Control(pDX, IDC_EDIT_FIND, m_Edit_Find);
	DDX_Control(pDX, IDC_GROUP_FIND, m_Group_Find);
	DDX_Control(pDX, IDC_BUTTON_FIND, m_Button_Find);
	DDX_Control(pDX, IDC_BUTTON_GOTO, m_Button_Goto);
	DDX_Control(pDX, IDC_EDIT_GOTO, m_Edit_Goto);
	DDX_Control(pDX, IDC_GROUP_GOTO, m_Group_Goto);
	DDX_Control(pDX, IDC_LIST_LOG, m_List_Log);
	DDX_Control(pDX, IDC_LIST_RUNNING, m_List_Running);
	DDX_Control(pDX, IDC_LIST_LOADED, m_List_Loaded);
	DDX_Control(pDX, IDC_LIST_INSTRUCTIONS, m_List_Instructions);
	DDX_Control(pDX, IDC_STATIC_GROUP_INSTRUCTIONS_STATS, m_Group_Instructions);
	DDX_Control(pDX, IDC_STATIC_GROUP_RUNNING_STATS, m_Group_Running);
	DDX_Control(pDX, IDC_STATIC_GROUP_LOADED_STATS, m_Group_Loaded);
	DDX_Control(pDX, IDC_LIST_MEMORY, m_List_Memory);
	DDX_Control(pDX, IDC_LIST_DISASSEMBLY, m_List_Disassembly);
	DDX_Control(pDX, IDC_LIST_COP1_2, m_List_COP1_2);
	DDX_Control(pDX, IDC_LIST_COP1_1, m_List_COP1_1);
	DDX_Control(pDX, IDC_LIST_GPRS_2, m_List_GPRS_2);
	DDX_Control(pDX, IDC_LIST_COP0_2, m_List_COP0_2);
	DDX_Control(pDX, IDC_LIST_COP0_1, m_List_COP0_1);
	DDX_Control(pDX, IDC_LIST_GPRS_1, m_List_GPRS_1);
	DDX_Control(pDX, IDC_TAB_DEBUG, m_TabDebug);
	DDX_Control(pDX, IDC_TAB_REGS, m_TabRegs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPSOPS2Dlg, CDialog)
	//{{AFX_MSG_MAP(CPSOPS2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_REGS, OnSelchangeTabRegs)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DEBUG, OnSelchangeTabDebug)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_DISASSEMBLY, OnKeydownListDisassembly)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_MEMORY, OnKeydownListMemory)
	ON_COMMAND(ID_BUTTON_OPEN, OnButtonOpen)
	ON_COMMAND(ID_BUTTON_STEP_INTO, OnButtonStepInto)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DISASSEMBLY, OnClickListDisassembly)
	ON_COMMAND(ID_BUTTON_RUN, OnButtonRun)
	ON_COMMAND(ID_BUTTON_STEP_OVER, OnButtonStepOver)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INSTRUCTIONS, OnClickListInstructions)
	ON_COMMAND(ID_BUTTON_RESTART, OnButtonRestart)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnButtonGoto)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_COMMAND(ID_BUTTON_EXEC, OnButtonExec)
	ON_COMMAND(ID_BUTTON_OPTIONS, OnButtonOptions)
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_PAD_CONFIG, OnButtonPadConfig)
	ON_COMMAND(ID_BUTTON_BIOS, OnButtonBios)
	ON_COMMAND(ID_BUTTON_CONFIG, OnButtonConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSOPS2Dlg message handlers

static int CALLBACK AlphaCompareInstructions(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*)lParamSort;
	CString    strItem1 = pListCtrl->GetItemText(lParam1, 1);
	CString    strItem2 = pListCtrl->GetItemText(lParam2, 1);

	return strItem1.Compare(strItem2);
}

static unsigned __int64 getAbsoluteTime()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

CWnd* Window;
CEdit* ConsoleBox;

void ConsoleOutput(char* Text)
{
	CString TextTemp;
	ConsoleBox->GetWindowText(TextTemp);
	TextTemp += Text;

	// Changing \n alones to \r\n to make the window control understands
	int Pos = 0;
	while ((Pos = TextTemp.Find("\n", Pos)) >= 0)
	{
		if ((Pos == 0) || (TextTemp[Pos - 1] != '\r'))
		{
			TextTemp.Insert(Pos, '\r');
			Pos += 1;
		}
		Pos += 1;
	}
	ConsoleBox->SetWindowText(TextTemp);

	static MSG msg;

	if (PeekMessage(&msg, *Window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

BOOL CPSOPS2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//  SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	EmuDll = LoadLibrary("ps2pelib.dll");
	EmuGetRegs = (EMUGETREGS)GetProcAddress(EmuDll, "EmuGetRegs");
	EmuSetConsoleCallback = (EMUSETCONSOLECALLBACK)GetProcAddress(EmuDll, "EmuSetConsoleCallback");
	EmuGetClock = (EMUGETCLOCK)GetProcAddress(EmuDll, "EmuGetClock");
	EmuInitialize = (EMUINITIALIZE)GetProcAddress(EmuDll, "EmuInitialize");
	EmuRelease = (EMURELEASE)GetProcAddress(EmuDll, "EmuRelease");
	EmuGetTotalInstructions = (EMUGETTOTALINSTRUCTIONS)GetProcAddress(EmuDll, "EmuGetTotalInstructions");
	EmuSetBiosFile = (EMUSETBIOSFILE)GetProcAddress(EmuDll, "EmuSetBiosFile");
	EmuGetInstructionsStats = (EMUGETINSTRUCTIONSSTATS)GetProcAddress(EmuDll, "EmuGetInstructionsStats");
	EmuGetR5900RegName = (EMUGETR5900REGNAME)GetProcAddress(EmuDll, "EmuGetR5900RegName");
	EmuGetCOP0RegName = (EMUGETCOP0REGNAME)GetProcAddress(EmuDll, "EmuGetCOP0RegName");
	EmuGetCOP1RegName = (EMUGETCOP1REGNAME)GetProcAddress(EmuDll, "EmuGetCOP1RegName");
	EmuGetCOP2FPRegName = (EMUGETCOP2FPREGNAME)GetProcAddress(EmuDll, "EmuGetCOP2FPRegName");
	EmuGetCOP2IPRegName = (EMUGETCOP2IPREGNAME)GetProcAddress(EmuDll, "EmuGetCOP2IPRegName");
	EmuIsBreakPoint = (EMUISBREAKPOINT)GetProcAddress(EmuDll, "EmuIsBreakPoint");
	EmuGetByte = (EMUGETBYTE)GetProcAddress(EmuDll, "EmuGetByte");
	EmuGetWord = (EMUGETWORD)GetProcAddress(EmuDll, "EmuGetWord");
	EmuGetDisassemblySymbol = (EMUGETDISASSEMBLYSYMBOL)GetProcAddress(EmuDll, "EmuGetDisassemblySymbol");
	EmuDisassembly = (EMUDISASSEMBLY)GetProcAddress(EmuDll, "EmuDisassembly");
	EmuLoad = (EMULOAD)GetProcAddress(EmuDll, "EmuLoad");
	EmuRun = (EMURUN)GetProcAddress(EmuDll, "EmuRun");
	EmuExecute = (EMUEXECUTE)GetProcAddress(EmuDll, "EmuExecute");
	EmuStepOver = (EMUSTEPOVER)GetProcAddress(EmuDll, "EmuStepOver");
	EmuStepInto = (EMUSTEPINTO)GetProcAddress(EmuDll, "EmuStepInto");
	EmuAddBreakPoint = (EMUADDBREAKPOINT)GetProcAddress(EmuDll, "EmuAddBreakPoint");
	EmuRemoveBreakPoint = (EMUREMOVEBREAKPOINT)GetProcAddress(EmuDll, "EmuRemoveBreakPoint");
	EmuGSInitWindow = (EMUGSINITWINDOW)GetProcAddress(EmuDll, "EmuGSInitWindow");
	EmuGSConfigure = (EMUGSCONFIGURE)GetProcAddress(EmuDll, "EmuGSConfigure");
	EmuGSCloseWindow = (EMUGSCLOSEWINDOW)GetProcAddress(EmuDll, "EmuGSCloseWindow");
	EmuAddInstructionBreakpoint = (EMUADDINSTRUCTIONBREAKPOINT)GetProcAddress(EmuDll, "EmuAddInstructionBreakpoint");
	EmuRemoveInstructionBreakpoint = (EMUREMOVEINSTRUCTIONBREAKPOINT)GetProcAddress(EmuDll, "EmuRemoveInstructionBreakpoint");
	EmuIsInstructionBreakpoint = (EMUISINSTRUCTIONBREAKPOINT)GetProcAddress(EmuDll, "EmuIsInstructionBreakpoint");
	EmuGetInstructionInfo = (EMUGETINSTRUCTIONINFO)GetProcAddress(EmuDll, "EmuGetInstructionInfo");
	EmuGetRunningStats = (EMUGETRUNNINGSTATS)GetProcAddress(EmuDll, "EmuGetRunningStats");
	EmuGetLoadedStats = (EMUGETLOADEDSTATS)GetProcAddress(EmuDll, "EmuGetLoadedStats");
	EmuReset = (EMURESET)GetProcAddress(EmuDll, "EmuReset");
	EmuFindLabel = (EMUFINDLABEL)GetProcAddress(EmuDll, "EmuFindLabel");
	EmuPADConfig = (EMUPADCONFIG)GetProcAddress(EmuDll, "EmuPADConfig");
	EmuConfig = (EMUCONFIG)GetProcAddress(EmuDll, "EmuConfig");

	CPUClock = EmuGetClock();

	EmuInitialize();
	EmuGetRegs(&R5900Regs, &COP0Regs, &COP1Regs, &VU0Regs, &VU1Regs);
	TotalInstructions = EmuGetTotalInstructions();

	DisassemblyNum = 13;
	MemoryNum = 14;

	DisassemblyPos = 0;
	MemoryPos = 0;

	SetWindowPos(NULL, 0, 0, 720, 600, SWP_NOMOVE | SWP_NOZORDER);

	m_TabRegs.SetWindowPos(NULL, 0, 28, 710, 306, SWP_NOZORDER);
	m_TabDebug.SetWindowPos(NULL, 0, 335, 710, 238, SWP_NOZORDER);

	if (TBFile.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP, 6666))
	{
		TBFile.LoadToolBar(IDR_TOOLBAR_FILE);
		TBFile.SetBarStyle(CBRS_ALIGN_TOP);//| TBSTYLE_TOOLTIPS );
		TBFile.SetWindowPos(NULL, 0, 0, 120, 28, SWP_NOZORDER);
		//		  TBFile.EnableToolTips( TRUE );
	}
	if (TBDebug.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP, 6667))
	{
		TBDebug.LoadToolBar(IDR_TOOLBAR_DEBUG);
		TBDebug.SetBarStyle(CBRS_ALIGN_TOP);
		TBDebug.SetWindowPos(NULL, 100, 0, 174, 28, SWP_NOZORDER);
	}
	m_Group_Goto.SetWindowPos(NULL, 280, -4, 154, 30, SWP_NOZORDER);
	m_Edit_Goto.SetWindowPos(NULL, 284, 4, 80, 19, SWP_NOZORDER);
	m_Button_Goto.SetWindowPos(NULL, 368, 4, 60, 19, SWP_NOZORDER);

	m_Group_Find.SetWindowPos(NULL, 438, -4, 184, 30, SWP_NOZORDER);
	m_Edit_Find.SetWindowPos(NULL, 442, 4, 110, 19, SWP_NOZORDER);
	m_Button_Find.SetWindowPos(NULL, 556, 4, 60, 19, SWP_NOZORDER);

	m_List_GPRS_1.SetWindowPos(NULL, 5, 32, 327, 297, SWP_NOZORDER);
	m_List_GPRS_2.SetWindowPos(NULL, 335, 32, 327, 297, SWP_NOZORDER);

	m_List_COP0_1.SetWindowPos(NULL, 5, 32, 210, 297, SWP_NOZORDER);
	m_List_COP0_2.SetWindowPos(NULL, 220, 32, 210, 297, SWP_NOZORDER);

	m_List_COP1_1.SetWindowPos(NULL, 5, 32, 327, 297, SWP_NOZORDER);
	m_List_COP1_2.SetWindowPos(NULL, 335, 32, 327, 297, SWP_NOZORDER);

	m_List_COP2_1.SetWindowPos(NULL, 5, 32, 327, 297, SWP_NOZORDER);
	m_List_COP2_2.SetWindowPos(NULL, 335, 32, 327, 297, SWP_NOZORDER);
	m_List_COP2_3.SetWindowPos(NULL, 5, 32, 327, 297, SWP_NOZORDER);
	m_List_COP2_4.SetWindowPos(NULL, 335, 32, 327, 297, SWP_NOZORDER);

	m_List_Log.SetWindowPos(NULL, 5, 32, 656, 297, SWP_NOZORDER);
	m_Edit_Console.SetWindowPos(NULL, 5, 32, 656, 297, SWP_NOZORDER);

	m_List_Disassembly.SetWindowPos(NULL, 5, 339, 658, 231, SWP_NOZORDER);
	m_List_Memory.SetWindowPos(NULL, 5, 339, 658, 231, SWP_NOZORDER);

	m_Group_Instructions.SetWindowPos(NULL, 5, 339, 297, 231, SWP_NOZORDER);
	m_Group_Loaded.SetWindowPos(NULL, 305, 339, 177, 231, SWP_NOZORDER);
	m_Group_Running.SetWindowPos(NULL, 485, 339, 177, 231, SWP_NOZORDER);

	m_List_Instructions.SetWindowPos(NULL, 11, 354, 285, 212, SWP_NOZORDER);
	m_List_Loaded.SetWindowPos(NULL, 309, 354, 169, 212, SWP_NOZORDER);
	m_List_Running.SetWindowPos(NULL, 489, 354, 169, 212, SWP_NOZORDER);

	VERIFY(font.CreateFont(8, 16, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH, "Fixedsys"));

	m_TabRegs.InsertItem(0, "GPRs");
	m_TabRegs.InsertItem(1, "COP0");
	m_TabRegs.InsertItem(2, "COP1");
	m_TabRegs.InsertItem(3, "COP2 FPR");
	m_TabRegs.InsertItem(4, "COP2 IR");
	m_TabRegs.InsertItem(5, "Log");
	m_TabRegs.InsertItem(6, "Console");

	m_TabDebug.InsertItem(0, "Disassembly");
	m_TabDebug.InsertItem(1, "Memory");
	m_TabDebug.InsertItem(2, "Stats");

	m_List_GPRS_1.SetFont(&font);
	m_List_GPRS_1.InsertColumn(0, "Name", LVCFMT_LEFT, 25, -1);
	m_List_GPRS_1.InsertColumn(1, "Value", LVCFMT_LEFT, 300, -1);
	m_List_GPRS_2.SetFont(&font);
	m_List_GPRS_2.InsertColumn(0, "Name", LVCFMT_LEFT, 25, -1);
	m_List_GPRS_2.InsertColumn(1, "Value", LVCFMT_LEFT, 300, -1);

	m_List_COP0_1.SetFont(&font);
	m_List_COP0_1.InsertColumn(0, "Name", LVCFMT_LEFT, 100, -1);
	m_List_COP0_1.InsertColumn(1, "Value", LVCFMT_LEFT, 100, -1);
	m_List_COP0_2.SetFont(&font);
	m_List_COP0_2.InsertColumn(0, "Name", LVCFMT_LEFT, 100, -1);
	m_List_COP0_2.InsertColumn(1, "Value", LVCFMT_LEFT, 100, -1);

	m_List_COP1_1.SetFont(&font);
	m_List_COP1_1.InsertColumn(0, "Name", LVCFMT_LEFT, 100, -1);
	m_List_COP1_1.InsertColumn(1, "Value", LVCFMT_LEFT, 127, -1);
	m_List_COP1_1.InsertColumn(2, "Value", LVCFMT_LEFT, 100, -1);
	m_List_COP1_2.SetFont(&font);
	m_List_COP1_2.InsertColumn(0, "Name", LVCFMT_LEFT, 100, -1);
	m_List_COP1_2.InsertColumn(1, "Value", LVCFMT_LEFT, 127, -1);
	m_List_COP1_2.InsertColumn(2, "Value", LVCFMT_LEFT, 100, -1);

	m_List_COP2_1.SetFont(&font);
	m_List_COP2_1.InsertColumn(0, "Name", LVCFMT_LEFT, 40, -1);
	m_List_COP2_1.InsertColumn(1, "X", LVCFMT_LEFT, 70, -1);
	m_List_COP2_1.InsertColumn(2, "Y", LVCFMT_LEFT, 70, -1);
	m_List_COP2_1.InsertColumn(3, "Z", LVCFMT_LEFT, 70, -1);
	m_List_COP2_1.InsertColumn(4, "W", LVCFMT_LEFT, 70, -1);

	m_List_COP2_2.SetFont(&font);
	m_List_COP2_2.InsertColumn(0, "Name", LVCFMT_LEFT, 40, -1);
	m_List_COP2_2.InsertColumn(1, "X", LVCFMT_LEFT, 70, -1);
	m_List_COP2_2.InsertColumn(2, "Y", LVCFMT_LEFT, 70, -1);
	m_List_COP2_2.InsertColumn(3, "Z", LVCFMT_LEFT, 70, -1);
	m_List_COP2_2.InsertColumn(4, "W", LVCFMT_LEFT, 70, -1);

	m_List_COP2_3.SetFont(&font);
	m_List_COP2_3.InsertColumn(0, "Name", LVCFMT_LEFT, 80, -1);
	m_List_COP2_3.InsertColumn(1, "Value", LVCFMT_LEFT, 110, -1);
	m_List_COP2_3.InsertColumn(2, "Float", LVCFMT_LEFT, 110, -1);
	m_List_COP2_4.SetFont(&font);
	m_List_COP2_4.InsertColumn(0, "Name", LVCFMT_LEFT, 80, -1);
	m_List_COP2_4.InsertColumn(1, "Hex", LVCFMT_LEFT, 110, -1);
	m_List_COP2_4.InsertColumn(2, "Float", LVCFMT_LEFT, 110, -1);

	m_List_Disassembly.SetFont(&font);
	m_List_Disassembly.InsertColumn(0, "B", LVCFMT_LEFT, 24, -1);
	m_List_Disassembly.InsertColumn(1, "Address", LVCFMT_LEFT, 80, -1);
	m_List_Disassembly.InsertColumn(2, "Code", LVCFMT_LEFT, 80, -1);
	m_List_Disassembly.InsertColumn(3, "Label", LVCFMT_LEFT, 130, -1);
	m_List_Disassembly.InsertColumn(4, "Assembly", LVCFMT_LEFT, 320, -1);

	m_List_Memory.SetFont(&font);
	m_List_Memory.InsertColumn(0, "Address", LVCFMT_LEFT, 70, -1);
	for (unsigned int i = 0; i < 16; i++)
	{
		m_List_Memory.InsertColumn(i + 1, "Data", LVCFMT_LEFT, 28, -1);
	}
	m_List_Memory.InsertColumn(17, "Text", LVCFMT_LEFT, 200, -1);

	m_List_Instructions.InsertColumn(0, "B", LVCFMT_LEFT, 20, -1);
	m_List_Instructions.InsertColumn(1, "Instruction", LVCFMT_LEFT, 85, -1);
	m_List_Instructions.InsertColumn(2, "Disassembled", LVCFMT_LEFT, 78, -1);
	m_List_Instructions.InsertColumn(3, "Implemented", LVCFMT_LEFT, 78, -1);

	m_List_Loaded.InsertColumn(0, "Instruction", LVCFMT_LEFT, 80, -1);
	m_List_Loaded.InsertColumn(1, "Total", LVCFMT_LEFT, 60, -1);

	m_List_Running.InsertColumn(0, "Instruction", LVCFMT_LEFT, 80, -1);
	m_List_Running.InsertColumn(1, "Total", LVCFMT_LEFT, 60, -1);

	// General registers
	for (unsigned int i = 0; i < 16; i++)
	{
		m_List_GPRS_1.InsertItem(i, "item");
		m_List_GPRS_2.InsertItem(i, "item");
		m_List_COP0_1.InsertItem(i, "item");
		m_List_COP0_2.InsertItem(i, "item");
		m_List_COP1_1.InsertItem(i, "item");
		m_List_COP1_2.InsertItem(i, "item");
		m_List_COP2_1.InsertItem(i, "item");
		m_List_COP2_2.InsertItem(i, "item");
		m_List_COP2_3.InsertItem(i, "item");
		m_List_COP2_4.InsertItem(i, "item");
	}
	// Hi, Lo and PC registers
	m_List_GPRS_1.InsertItem(16, "item");
	m_List_GPRS_2.InsertItem(16, "item");
	m_List_GPRS_1.InsertItem(17, "item");
	m_List_GPRS_2.InsertItem(17, "item");

	// FCR0, FCR1, Accumulator
	m_List_COP1_1.InsertItem(16, "item");
	m_List_COP1_1.InsertItem(17, "item");
	m_List_COP1_2.InsertItem(16, "item");
	m_List_COP1_2.InsertItem(17, "item");

	m_List_COP2_2.InsertItem(16, "item");

	for (unsigned int i = 0; i < DisassemblyNum; i++)
	{
		m_List_Disassembly.InsertItem(i, "item");
	}

	for (unsigned int i = 0; i < MemoryNum; i++)
	{
		m_List_Memory.InsertItem(i, "item");
	}

	for (unsigned int i = 0; i < TotalInstructions; i++)
	{
		m_List_Instructions.InsertItem(i, "item");
		m_List_Loaded.InsertItem(i, "item");
		m_List_Running.InsertItem(i, "item");
	}

	// Selecting the GPR Tab
	m_TabRegs.SetCurSel(0);
	ChangeTabRegs();

	// Selecting the Disassembly Tab
	m_TabDebug.SetCurSel(0);
	ChangeTabDebug();

	m_List_Disassembly.EnableWindow(FALSE);
	m_List_Memory.EnableWindow(FALSE);
	m_Button_Goto.EnableWindow(FALSE);
	m_Button_Find.EnableWindow(FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_EXEC, FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_PAUSE, FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_RUN, FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_STEP_INTO, FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_STEP_OVER, FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_RESTART, FALSE);
	TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_BIOS, FALSE);

	GenerateInstructionsStats();
	GenerateLoadedStats();
	GenerateRunningStats();

	m_List_Instructions.SortItems(AlphaCompareInstructions, (LPARAM)&m_List_Instructions);

	CString strTemp;
	strTemp.Format("Current CPU Clock: %.2f Mhz", CPUClock / 1000000.0f);
	m_List_Log.AddString(strTemp);

	LogInstructions();

	EmuSetBiosFile("bios.bin");

	Window = this;
	ConsoleBox = &m_Edit_Console;
	EmuSetConsoleCallback(ConsoleOutput);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPSOPS2Dlg::LogInstructions()
{
	unsigned int TotalSupportedInstructions = 0;
	unsigned int TotalDisassembledInstructions = 0;
	unsigned int TotalImplementedInstructions = 0;
	EmuGetInstructionsStats(&TotalSupportedInstructions,
		&TotalDisassembledInstructions,
		&TotalImplementedInstructions);
	CString strTemp;
	m_List_Log.AddString("");
	m_List_Log.AddString("Instructions Statistics:");

	strTemp.Format("Total Instructions: %u", TotalSupportedInstructions);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Total Disassembled: %u ( %.2f%% )",
		TotalDisassembledInstructions,
		((float)TotalDisassembledInstructions / (float)TotalSupportedInstructions) * 100.0f);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Total Implemented: %u ( %.2f%% )",
		TotalImplementedInstructions,
		((float)TotalImplementedInstructions / (float)TotalSupportedInstructions) * 100.0f);
	m_List_Log.AddString(strTemp);
}

void CPSOPS2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPSOPS2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPSOPS2Dlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

int CPSOPS2Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CPSOPS2Dlg::OnSelchangeTabRegs(NMHDR* pNMHDR, LRESULT* pResult)
{
	ChangeTabRegs();

	*pResult = 0;
}

void CPSOPS2Dlg::ShowGPR()
{
	m_List_GPRS_1.BringWindowToTop();
	m_List_GPRS_2.BringWindowToTop();
	m_List_GPRS_1.ShowWindow(SW_SHOW);
	m_List_GPRS_2.ShowWindow(SW_SHOW);

	char Buffer[256];
	Buffer[255] = '\0';
	CString strText;
	for (int i = 0; i < 32; i++)
	{
		EmuGetR5900RegName(i, Buffer, sizeof(Buffer) - 1);
		strText = Buffer;
		strText.MakeUpper();
		if ((i / 16) == 0)
		{
			m_List_GPRS_1.SetItemText(i, 0, strText);
			strText.Format(TEXT("%.8X_%.8X_%.8X_%.8X"),
				R5900Regs->Reg[i].u32_96_127,
				R5900Regs->Reg[i].u32_64_95,
				R5900Regs->Reg[i].u32_32_63,
				R5900Regs->Reg[i].u32_00_31);
			m_List_GPRS_1.SetItemText(i, 1, strText);
		}
		else
		{
			m_List_GPRS_2.SetItemText(i - 16, 0, strText);
			strText.Format(TEXT("%.8X_%.8X_%.8X_%.8X"),
				R5900Regs->Reg[i].u32_96_127,
				R5900Regs->Reg[i].u32_64_95,
				R5900Regs->Reg[i].u32_32_63,
				R5900Regs->Reg[i].u32_00_31);
			m_List_GPRS_2.SetItemText(i - 16, 1, strText);
		}
	}

	EmuGetR5900RegName(32, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_GPRS_1.SetItemText(16, 0, strText);
	strText.Format(TEXT("%.8X_%.8X_%.8X_%.8X"),
		R5900Regs->HI.u32_96_127,
		R5900Regs->HI.u32_64_95,
		R5900Regs->HI.u32_32_63,
		R5900Regs->HI.u32_00_31);
	m_List_GPRS_1.SetItemText(16, 1, strText);

	EmuGetR5900RegName(33, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_GPRS_1.SetItemText(17, 0, strText);
	strText.Format(TEXT("%.8X_%.8X_%.8X_%.8X"),
		R5900Regs->LO.u32_96_127,
		R5900Regs->LO.u32_64_95,
		R5900Regs->LO.u32_32_63,
		R5900Regs->LO.u32_00_31);
	m_List_GPRS_1.SetItemText(17, 1, strText);

	EmuGetR5900RegName(34, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_GPRS_2.SetItemText(16, 0, strText);
	strText.Format(TEXT("%.8X"),
		R5900Regs->SA);
	m_List_GPRS_2.SetItemText(16, 1, strText);

	EmuGetR5900RegName(35, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_GPRS_2.SetItemText(17, 0, strText);
	strText.Format(TEXT("%.8X"),
		R5900Regs->PC);
	m_List_GPRS_2.SetItemText(17, 1, strText);
}

void CPSOPS2Dlg::ShowCOP0()
{
	m_List_COP0_1.BringWindowToTop();
	m_List_COP0_2.BringWindowToTop();
	m_List_COP0_1.ShowWindow(SW_SHOW);
	m_List_COP0_2.ShowWindow(SW_SHOW);

	char Buffer[256];
	Buffer[255] = '\0';
	CString strText;
	for (int i = 0; i < 32; i++)
	{
		EmuGetCOP0RegName(i, Buffer, sizeof(Buffer) - 1);
		strText = Buffer;
		strText.MakeUpper();
		if ((i / 16) == 0)
		{
			m_List_COP0_1.SetItemText(i, 0, strText);
			strText.Format(TEXT("%.8X"),
				COP0Regs->Reg[i]);
			m_List_COP0_1.SetItemText(i, 1, strText);
		}
		else
		{
			m_List_COP0_2.SetItemText(i - 16, 0, strText);
			strText.Format(TEXT("%.8X"),
				COP0Regs->Reg[i]);
			m_List_COP0_2.SetItemText(i - 16, 1, strText);
		}
	}
}

void CPSOPS2Dlg::ShowCOP2_FPR()
{
	m_List_COP2_1.BringWindowToTop();
	m_List_COP2_2.BringWindowToTop();
	m_List_COP2_1.ShowWindow(SW_SHOW);
	m_List_COP2_2.ShowWindow(SW_SHOW);

	char Buffer[256];
	Buffer[255] = '\0';
	CString strText;
	for (int i = 0; i < 32; i++)
	{
		EmuGetCOP2FPRegName(i, Buffer, sizeof(Buffer) - 1);
		strText = Buffer;
		strText.MakeUpper();
		if ((i / 16) == 0)
		{
			m_List_COP2_1.SetItemText(i, 0, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].X.f);
			m_List_COP2_1.SetItemText(i, 1, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].Y.f);
			m_List_COP2_1.SetItemText(i, 2, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].Z.f);
			m_List_COP2_1.SetItemText(i, 3, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].W.f);
			m_List_COP2_1.SetItemText(i, 4, strText);
		}
		else
		{
			m_List_COP2_2.SetItemText(i - 16, 0, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].X.f);
			m_List_COP2_2.SetItemText(i - 16, 1, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].Y.f);
			m_List_COP2_2.SetItemText(i - 16, 2, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].Z.f);
			m_List_COP2_2.SetItemText(i - 16, 3, strText);
			strText.Format(TEXT("%.5f"), VU0Regs->CPR[i].W.f);
			m_List_COP2_2.SetItemText(i - 16, 4, strText);
		}
	}
	EmuGetCOP2FPRegName(32, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_COP2_2.SetItemText(16, 0, strText);
	strText.Format(TEXT("%.5f"), VU0Regs->Acc.X.f);
	m_List_COP2_2.SetItemText(16, 1, strText);
	strText.Format(TEXT("%.5f"), VU0Regs->Acc.Y.f);
	m_List_COP2_2.SetItemText(16, 2, strText);
	strText.Format(TEXT("%.5f"), VU0Regs->Acc.Z.f);
	m_List_COP2_2.SetItemText(16, 3, strText);
	strText.Format(TEXT("%.5f"), VU0Regs->Acc.W.f);
	m_List_COP2_2.SetItemText(16, 4, strText);
}

void CPSOPS2Dlg::ShowCOP2_IR()
{
	m_List_COP2_3.BringWindowToTop();
	m_List_COP2_4.BringWindowToTop();
	m_List_COP2_3.ShowWindow(SW_SHOW);
	m_List_COP2_4.ShowWindow(SW_SHOW);

	char Buffer[256];
	Buffer[255] = '\0';
	CString strText;
	for (int i = 0; i < 32; i++)
	{
		EmuGetCOP2IPRegName(i, Buffer, sizeof(Buffer) - 1);
		strText = Buffer;
		strText.MakeUpper();
		if ((i / 16) == 0)
		{
			m_List_COP2_3.SetItemText(i, 0, strText);
			strText.Format(TEXT("%.4X_%.4X"), VU0Regs->CCR[i].u16_16_31, VU0Regs->CCR[i].u16_00_15);
			m_List_COP2_3.SetItemText(i, 1, strText);
			strText.Format(TEXT("%.10f"), *(EMU_F32*)&VU0Regs->CCR[i].u32);
			m_List_COP2_3.SetItemText(i, 2, strText);
		}
		else
		{
			m_List_COP2_4.SetItemText(i - 16, 0, strText);
			strText.Format(TEXT("%.4X_%.4X"), VU0Regs->CCR[i].u16_16_31, VU0Regs->CCR[i].u16_00_15);
			m_List_COP2_4.SetItemText(i - 16, 1, strText);
			strText.Format(TEXT("%.10f"), *(EMU_F32*)&VU0Regs->CCR[i].u32);
			m_List_COP2_4.SetItemText(i - 16, 2, strText);
		}
	}
}

void CPSOPS2Dlg::ChangeTabRegs()
{
	int TabNumber = m_TabRegs.GetCurSel();

	switch (TabNumber)
	{
	case 0:  ShowGPR();       break;
	case 1:  ShowCOP0();      break;
	case 2:  ShowCOP1();      break;
	case 3:  ShowCOP2_FPR();  break;
	case 4:  ShowCOP2_IR();   break;
	case 5:  m_List_Log.ShowWindow(SW_SHOW);    break;
	case 6:  m_Edit_Console.ShowWindow(SW_SHOW);    break;
	}

	if (TabNumber != 0)
	{
		m_List_GPRS_1.ShowWindow(SW_HIDE);
		m_List_GPRS_2.ShowWindow(SW_HIDE);
	}
	if (TabNumber != 1)
	{
		m_List_COP0_1.ShowWindow(SW_HIDE);
		m_List_COP0_2.ShowWindow(SW_HIDE);
	}
	if (TabNumber != 2)
	{
		m_List_COP1_1.ShowWindow(SW_HIDE);
		m_List_COP1_2.ShowWindow(SW_HIDE);
	}
	if (TabNumber != 3)
	{
		m_List_COP2_1.ShowWindow(SW_HIDE);
		m_List_COP2_2.ShowWindow(SW_HIDE);
	}
	if (TabNumber != 4)
	{
		m_List_COP2_3.ShowWindow(SW_HIDE);
		m_List_COP2_4.ShowWindow(SW_HIDE);
	}
	if (TabNumber != 5)
	{
		m_List_Log.ShowWindow(SW_HIDE);
	}
	if (TabNumber != 6)
	{
		m_Edit_Console.ShowWindow(SW_HIDE);
	}
}

void CPSOPS2Dlg::ShowCOP1()
{
	m_List_COP1_1.BringWindowToTop();
	m_List_COP1_2.BringWindowToTop();
	m_List_COP1_1.ShowWindow(SW_SHOW);
	m_List_COP1_2.ShowWindow(SW_SHOW);

	char Buffer[256];
	Buffer[255] = '\0';
	CString strText;
	for (int i = 0; i < 32; i++)
	{
		EmuGetCOP1RegName(i, Buffer, sizeof(Buffer) - 1);
		strText = Buffer;
		strText.MakeUpper();
		if ((i / 18) == 0)
		{
			m_List_COP1_1.SetItemText(i, 0, strText);
			strText.Format(TEXT("%f"), COP1Regs->Reg[i].f);
			m_List_COP1_1.SetItemText(i, 1, strText);
			strText.Format(TEXT("%.8X"), COP1Regs->Reg[i].u);
			m_List_COP1_1.SetItemText(i, 2, strText);
		}
		else
		{
			m_List_COP1_2.SetItemText(i - 18, 0, strText);
			strText.Format(TEXT("%f"), COP1Regs->Reg[i].f);
			m_List_COP1_2.SetItemText(i - 18, 1, strText);
			strText.Format(TEXT("%.8X"), COP1Regs->Reg[i].u);
			m_List_COP1_2.SetItemText(i - 18, 2, strText);
		}
	}

	m_List_COP1_2.SetItemText(14, 0, "");
	m_List_COP1_2.SetItemText(14, 1, "");
	m_List_COP1_2.SetItemText(14, 2, "");

	EmuGetCOP1RegName(34, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_COP1_2.SetItemText(15, 0, strText);
	strText.Format(TEXT("%f"), COP1Regs->Accumulator.f);
	m_List_COP1_2.SetItemText(15, 1, strText);
	strText.Format(TEXT("%.8X"), COP1Regs->Accumulator.u);
	m_List_COP1_2.SetItemText(15, 2, strText);

	EmuGetCOP1RegName(32, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_COP1_2.SetItemText(16, 0, strText);
	strText.Format(TEXT("%.8X"), COP1Regs->FCR0);
	m_List_COP1_2.SetItemText(16, 1, strText);
	m_List_COP1_2.SetItemText(16, 2, "");

	EmuGetCOP1RegName(33, Buffer, sizeof(Buffer) - 1);
	strText = Buffer;
	strText.MakeUpper();
	m_List_COP1_2.SetItemText(17, 0, strText);
	strText.Format(TEXT("%.8X"), COP1Regs->FCR31);
	m_List_COP1_2.SetItemText(17, 1, strText);
	m_List_COP1_2.SetItemText(17, 2, "");
}

void CPSOPS2Dlg::OnSelchangeTabDebug(NMHDR* pNMHDR, LRESULT* pResult)
{
	ChangeTabDebug();

	*pResult = 0;
}

void CPSOPS2Dlg::ChangeTabDebug()
{
	int TabNumber = m_TabDebug.GetCurSel();

	m_List_Disassembly.ShowWindow(SW_HIDE);
	m_List_Memory.ShowWindow(SW_HIDE);
	m_Group_Instructions.ShowWindow(SW_HIDE);
	m_Group_Loaded.ShowWindow(SW_HIDE);
	m_Group_Running.ShowWindow(SW_HIDE);
	m_List_Instructions.ShowWindow(SW_HIDE);
	m_List_Loaded.ShowWindow(SW_HIDE);
	m_List_Running.ShowWindow(SW_HIDE);

	switch (TabNumber)
	{
	case 0:  ShowDisassembly();  break;
	case 1:  ShowMemory();       break;
	case 2:  ShowStats();        break;
	}
}

void CPSOPS2Dlg::ShowDisassembly()
{
	m_List_Disassembly.BringWindowToTop();
	m_List_Disassembly.ShowWindow(SW_SHOW);

	Disassembly();
}

void CPSOPS2Dlg::Disassembly()
{
	char Buffer[256];
	Buffer[255] = '\0';
	unsigned int tAddress = DisassemblyPos << 2;
	for (unsigned int i = tAddress; i < (unsigned int)(tAddress + (DisassemblyNum << 2)); i += 4)
	{
		CString strPointer = "";
		if (EmuIsBreakPoint(i))
		{
			strPointer = "B";
		}
		else
		{
			strPointer = " ";
		}
		if (i == R5900Regs->PC)
		{
			strPointer += ">";
		}
		m_List_Disassembly.SetItemText((i - tAddress) / 4, 0, strPointer);
		strPointer.Format(TEXT("%.8X"), i);
		m_List_Disassembly.SetItemText((i - tAddress) / 4, 1, strPointer);
		strPointer.Format(TEXT("%.8X"), EmuGetWord(i));
		m_List_Disassembly.SetItemText((i - tAddress) / 4, 2, strPointer);
		EmuGetDisassemblySymbol(i, Buffer, sizeof(Buffer) - 1);
		m_List_Disassembly.SetItemText((i - tAddress) / 4, 3, Buffer);
		EmuDisassembly(i, Buffer, sizeof(Buffer) - 1);
		m_List_Disassembly.SetItemText((i - tAddress) / 4, 4, Buffer);
	}
}

void CPSOPS2Dlg::ShowMemory()
{
	m_List_Memory.BringWindowToTop();
	m_List_Memory.ShowWindow(SW_SHOW);

	FillMemoryInfo();
}

void CPSOPS2Dlg::FillMemoryInfo()
{
	unsigned int tAddress = MemoryPos * 16;
	CString strPointer = "";
	for (unsigned int i = tAddress;
		i < (tAddress + (16 * MemoryNum));
		i += 16)
	{
		strPointer.Format(TEXT("%.8X"), i);
		m_List_Memory.SetItemText((i - tAddress) / 16, 0, strPointer);
		CString Texto = "";
		for (unsigned int j = i; j < i + 16; j++)
		{
			strPointer.Format(TEXT("%.2X"), EmuGetByte(j));
			m_List_Memory.SetItemText((i - tAddress) / 16, 1 + j - i, strPointer);
			if ((EmuGetByte(j) < 32) || (EmuGetByte(j) > 127))
			{
				Texto += ".";
			}
			else
			{
				Texto += (char)EmuGetByte(j);
			}
		}
		m_List_Memory.SetItemText((i - tAddress) / 16, 17, Texto);
	}
}

void CPSOPS2Dlg::OnKeydownListDisassembly(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	switch (pLVKeyDow->wVKey)
	{
	case VK_DOWN:   DisassemblyPos += 1;               break;
	case VK_UP:     DisassemblyPos -= 1;               break;
	case VK_NEXT:   DisassemblyPos += DisassemblyNum;  break;
	case VK_PRIOR:  DisassemblyPos -= DisassemblyNum;  break;
	case VK_F9:     OnButtonRun();                     break;
	case VK_F10:    OnButtonStepOver();                break;
	case VK_F11:    OnButtonStepInto();                break;
	}
	if (DisassemblyPos > 0x7FFFFFFF)
	{
		DisassemblyPos = 0;
	}
	else
		if (DisassemblyPos > (0x40000000 - DisassemblyNum + 1))
		{
			DisassemblyPos = 0x40000000 - DisassemblyNum + 1;
		}

	Disassembly();

	*pResult = 1;
}

void CPSOPS2Dlg::OnKeydownListMemory(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	switch (pLVKeyDow->wVKey)
	{
	case VK_DOWN:   MemoryPos += 1;          break;
	case VK_UP:     MemoryPos -= 1;          break;
	case VK_NEXT:   MemoryPos += MemoryNum;  break;
	case VK_PRIOR:  MemoryPos -= MemoryNum;  break;
	case VK_F9:     OnButtonRun();           break;
	case VK_F10:    OnButtonStepOver();      break;
	case VK_F11:    OnButtonStepInto();      break;
	}
	if (MemoryPos > 0x7FFFFFFF)
	{
		MemoryPos = 0;
	}
	else
		if (MemoryPos > (0x40000000 - MemoryNum + 1))
		{
			MemoryPos = 0x40000000 - MemoryNum + 1;
		}

	FillMemoryInfo();

	*pResult = 1;
}

void CPSOPS2Dlg::OnButtonOpen()
{
	static char BASED_CODE szFilter[] = "ELF Files (*.elf)|*.elf|All Files (*.*)|*.*||";

	CFileDialog* OpenDialog = new CFileDialog(
		true,
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		szFilter,
		NULL);

	OpenDialog->m_ofn.lpstrTitle = "Open PS2 File";

	if (OpenDialog->DoModal() == IDOK)
	{
		CString FileName = OpenDialog->GetPathName();

		if (EmuLoad(FileName) <= 0)
		{
			MessageBoxA("Could not load file.", "Error Loading", MB_OK);
		}

		m_List_Disassembly.EnableWindow(TRUE);
		m_List_Memory.EnableWindow(TRUE);
		m_Button_Goto.EnableWindow(TRUE);
		m_Button_Find.EnableWindow(TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_EXEC, TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_PAUSE, TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_RUN, TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_STEP_INTO, TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_STEP_OVER, TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_RESTART, TRUE);
		TBDebug.GetToolBarCtrl().EnableButton(ID_BUTTON_BIOS, TRUE);

		DisassemblyGo(R5900Regs->PC);

		MemoryGo(R5900Regs->PC);

		ChangeTabRegs();

		GenerateLoadedStats();

		Disassembly();
		FillMemoryInfo();

		m_List_Disassembly.SetFocus();
	}

	delete OpenDialog;
}

void CPSOPS2Dlg::OnButtonStepInto()
{
	EmuStepInto(R5900Regs->PC);
	DisassemblyGo(R5900Regs->PC);
	ChangeTabRegs();
	GenerateRunningStats();
}

void CPSOPS2Dlg::DisassemblyGo(unsigned int Pos)
{
	int PageInit = Pos - ((Pos / 4) % DisassemblyNum) * 4;

	DisassemblyPos = PageInit / 4;
	Disassembly();
}


void CPSOPS2Dlg::OnClickListDisassembly(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pListItem = (NM_LISTVIEW*)pNMHDR;

	if (pListItem->iItem != -1)
	{
		unsigned int tAddress = (DisassemblyPos + pListItem->iItem) * 4;
		if (EmuIsBreakPoint(tAddress))
		{
			EmuRemoveBreakPoint(tAddress);
		}
		else
		{
			EmuAddBreakPoint(tAddress);
		}
		Disassembly();
	}

	*pResult = 1;
}

UINT __cdecl MyControllingFunction(LPVOID pParam)
{
	EmuRun(*(unsigned int*)pParam);

	return 0;
}

void CPSOPS2Dlg::OnButtonRun()
{
	unsigned __int64 StartTicks = 0,
		EndTicks = 0,
		TotalTicks = 0;
	unsigned int StartInstructions = COP0Regs->Count,
		EndInstructions = 0,
		TotalInstructions = 0;
	m_List_Log.AddString("");
	m_List_Log.AddString("Running...");
	StartTicks = getAbsoluteTime();

	EmuGSInitWindow();
	EmuRunning = true;

	ShowCursor(false);
	EmuRun(R5900Regs->PC);
	EmuGSCloseWindow();
	ShowCursor(true);

	EndTicks = getAbsoluteTime();
	TotalTicks = EndTicks - StartTicks;
	EndInstructions = COP0Regs->Count;
	TotalInstructions = EndInstructions - StartInstructions;

	CString strTemp;
	strTemp.Format("Total ticks count: %u", TotalTicks);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Total time: %.4f s", (double)(__int64)TotalTicks / (double)CPUClock);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Total Instructions Executed: %u", TotalInstructions);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Instructions per second: %.2f",
		(double)TotalInstructions / ((double)(__int64)TotalTicks / (double)CPUClock));
	m_List_Log.AddString(strTemp);

	DisassemblyGo(R5900Regs->PC);
	ChangeTabRegs();
	GenerateRunningStats();
}

void CPSOPS2Dlg::OnButtonStepOver()
{
	EmuStepOver(R5900Regs->PC);
	DisassemblyGo(R5900Regs->PC);
	ChangeTabRegs();
	GenerateRunningStats();
}

void CPSOPS2Dlg::ShowStats()
{
	m_Group_Instructions.BringWindowToTop();
	m_Group_Loaded.BringWindowToTop();
	m_Group_Running.BringWindowToTop();
	m_Group_Instructions.ShowWindow(SW_SHOW);
	m_Group_Loaded.ShowWindow(SW_SHOW);
	m_Group_Running.ShowWindow(SW_SHOW);

	m_List_Instructions.BringWindowToTop();
	m_List_Loaded.BringWindowToTop();
	m_List_Running.BringWindowToTop();
	m_List_Instructions.ShowWindow(SW_SHOW);
	m_List_Loaded.ShowWindow(SW_SHOW);
	m_List_Running.ShowWindow(SW_SHOW);
}

void CPSOPS2Dlg::GenerateInstructionsStats()
{
	char* Name;
	BOOL IsDisassembled,
		IsImplemented;
	for (unsigned int i = 0; i < TotalInstructions; i++)
	{
		EmuGetInstructionInfo(i, &Name, &IsDisassembled, &IsImplemented);
		m_List_Instructions.SetItemData(i, i);
		m_List_Instructions.SetItemText(i, 0, EmuIsInstructionBreakpoint(i) ? "B" : "");
		m_List_Instructions.SetItemText(i, 1, Name);
		m_List_Instructions.SetItemText(i, 2, IsDisassembled ? "Yes" : "No");
		m_List_Instructions.SetItemText(i, 3, IsImplemented ? "Yes" : "No");
	}
}

void CPSOPS2Dlg::GenerateRunningStats()
{
	char* Name;
	BOOL IsDisassembled,
		IsImplemented;
	CString strTemp;

	EMU_U32 size = TotalInstructions + 1;
	stEmuInstructionCount* stats = new stEmuInstructionCount[size];
	EmuGetRunningStats(&stats);

	std::vector<stEmuInstructionCount> TempStats(size);
	memcpy(&TempStats[0], stats, size * sizeof(stEmuInstructionCount));

	unsigned int i{0};
	for (; i < TotalInstructions; i++)
	{
		unsigned int MaxValue = 0;
		unsigned int MaxIndex = 0;
		for (unsigned int j = 0; j < TempStats.size(); j++)
		{
			if (TempStats[j].Total >= MaxValue)
			{
				MaxValue = TempStats[j].Total;
				MaxIndex = j;
			}
		}
		EmuGetInstructionInfo(TempStats[MaxIndex].Index, &Name, &IsDisassembled, &IsImplemented);
		m_List_Running.SetItemText(i, 0, Name);
		strTemp.Format("%u", MaxValue);
		m_List_Running.SetItemText(i, 1, strTemp);
		TempStats.erase(TempStats.begin() + MaxIndex);
	}
	EmuGetInstructionInfo(stats[TotalInstructions].Index, &Name, &IsDisassembled, &IsImplemented);
	m_List_Running.SetItemText(i, 0, Name);
	strTemp.Format("%u", stats[TotalInstructions].Total);
	m_List_Running.SetItemText(i, 1, strTemp);
}

void CPSOPS2Dlg::GenerateLoadedStats()
{
	char* Name;
	BOOL IsDisassembled,
		IsImplemented;
	CString strTemp;

	EMU_U32 size = TotalInstructions + 1;
	stEmuInstructionCount * stats = new stEmuInstructionCount[size];
	EmuGetLoadedStats(&stats);

	std::vector<stEmuInstructionCount> TempStats(size);
	memcpy(&TempStats[0], stats, size * sizeof(stEmuInstructionCount));

	unsigned int i{0};
	for (; i < TotalInstructions; i++)
	{
		unsigned int MaxValue = 0;
		unsigned int MaxIndex = 0;
		for (unsigned int j = 0; j < TempStats.size(); j++)
		{
			if (TempStats[j].Total >= MaxValue)
			{
				MaxValue = TempStats[j].Total;
				MaxIndex = j;
			}
		}
		EmuGetInstructionInfo(TempStats[MaxIndex].Index, &Name, &IsDisassembled, &IsImplemented);
		m_List_Loaded.SetItemText(i, 0, Name);
		strTemp.Format("%u", MaxValue);
		m_List_Loaded.SetItemText(i, 1, strTemp);
		TempStats.erase(TempStats.begin() + MaxIndex);
	}
	EmuGetInstructionInfo(stats[TotalInstructions].Index, &Name, &IsDisassembled, &IsImplemented);
	m_List_Loaded.SetItemText(i, 0, Name);
	strTemp.Format("%u", stats[TotalInstructions].Total);
	m_List_Loaded.SetItemText(i, 1, strTemp);

	delete[] stats;
}

void CPSOPS2Dlg::OnClickListInstructions(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pListItem = (NM_LISTVIEW*)pNMHDR;

	if (pListItem->iItem != -1)
	{
		unsigned int Index = pListItem->iItem;
		if (EmuIsInstructionBreakpoint(m_List_Instructions.GetItemData(pListItem->iItem)))
		{
			EmuRemoveInstructionBreakpoint(m_List_Instructions.GetItemData(pListItem->iItem));
		}
		else
		{
			EmuAddInstructionBreakpoint(m_List_Instructions.GetItemData(pListItem->iItem));
		}
		m_List_Instructions.SetItemText(pListItem->iItem, 0,
			EmuIsInstructionBreakpoint(m_List_Instructions.GetItemData(pListItem->iItem)) ? "B" : "");
		Disassembly();
	}

	*pResult = 1;
}

void CPSOPS2Dlg::OnButtonRestart()
{
	EmuReset();

	DisassemblyGo(R5900Regs->PC);

	MemoryGo(R5900Regs->PC);

	ChangeTabRegs();
	Disassembly();
	FillMemoryInfo();
}

void CPSOPS2Dlg::OnButtonGoto()
{
	char Text[256];
	unsigned int HexValue;

	if (m_Edit_Goto.GetLine(0, Text))
	{
		sscanf_s(Text, "%x", &HexValue);

		switch (m_TabDebug.GetCurSel())
		{
		case 0:
			DisassemblyGo(HexValue);
			break;
		case 1:
			MemoryGo(HexValue);
			break;
		default:
			break;
		}
	}
}

void CPSOPS2Dlg::MemoryGo(unsigned int Address)
{
	int PageInit = Address - ((Address / 16) % MemoryNum) * 16;

	MemoryPos = PageInit / 16;
	FillMemoryInfo();
}

void CPSOPS2Dlg::OnButtonFind()
{
	char Text[256];

	if (m_Edit_Find.GetLine(0, Text))
	{
		EMU_U32 Address;
		if (EmuFindLabel(Text, &Address))
		{
			switch (m_TabDebug.GetCurSel())
			{
			case 0:
				DisassemblyGo(Address);
				break;
			case 1:
				MemoryGo(Address);
				break;
			default:
				break;
			}
		}
		else
		{
			MessageBox("Nothing found", "Find Results");
		}
	}
}

void CPSOPS2Dlg::OnOK()
{
	// Do nothing
}

void CPSOPS2Dlg::OnButtonExec()
{
	unsigned __int64 StartTicks = 0,
		EndTicks = 0,
		TotalTicks = 0;
	unsigned int StartInstructions = COP0Regs->Count,
		EndInstructions = 0,
		TotalInstructions = 0;
	m_List_Log.AddString("");
	m_List_Log.AddString("Running...");
	StartTicks = getAbsoluteTime();

	EmuGSInitWindow();
	EmuRunning = true;

	ShowCursor(false);
	EmuExecute(R5900Regs->PC);
	ShowCursor(true);

	GenerateRunningStats();

	EndTicks = getAbsoluteTime();
	TotalTicks = EndTicks - StartTicks;
	EndInstructions = COP0Regs->Count;
	TotalInstructions = EndInstructions - StartInstructions;

	CString strTemp;
	strTemp.Format("Total ticks count: %u", TotalTicks);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Total time: %.4f s", (double)(__int64)TotalTicks / (double)CPUClock);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Total Instructions Executed: %u", TotalInstructions);
	m_List_Log.AddString(strTemp);
	strTemp.Format("Instructions per second: %.2f",
		(double)TotalInstructions / ((double)(__int64)TotalTicks / (double)CPUClock));
	m_List_Log.AddString(strTemp);

	DisassemblyGo(R5900Regs->PC);
	ChangeTabRegs();
}

void CPSOPS2Dlg::OnButtonOptions()
{
	EmuGSConfigure();
}

void CPSOPS2Dlg::OnDestroy()
{
	EmuRelease();

	CDialog::OnDestroy();
}

void CPSOPS2Dlg::OnButtonPadConfig()
{
	EmuPADConfig();
}

void CPSOPS2Dlg::OnButtonBios()
{
	R5900Regs->PC = 0xBFC00000;
	DisassemblyGo(R5900Regs->PC);
}

void CPSOPS2Dlg::OnButtonConfig()
{
	EmuConfig();
}
