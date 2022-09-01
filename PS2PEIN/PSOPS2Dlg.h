// PSOPS2Dlg.h : header file
//

#if !defined(AFX_PSOPS2DLG_H__A67B826C_B773_4B95_A448_44724A0A14C6__INCLUDED_)
#define AFX_PSOPS2DLG_H__A67B826C_B773_4B95_A448_44724A0A14C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPSOPS2Dlg dialog

class CPSOPS2Dlg : public CDialog
{
	// Construction
public:
	CPSOPS2Dlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CPSOPS2Dlg)
	enum { IDD = IDD_PSOPS2_DIALOG };
	CEdit	m_Edit_Console;
	CListCtrl	m_List_COP2_4;
	CListCtrl	m_List_COP2_3;
	CListCtrl	m_List_COP2_2;
	CListCtrl	m_List_COP2_1;
	CEdit   m_Edit_Find;
	CButton m_Group_Find;
	CButton m_Button_Find;
	CButton m_Button_Goto;
	CEdit   m_Edit_Goto;
	CButton m_Group_Goto;
	CListBox    m_List_Log;
	CListCtrl   m_List_Running;
	CListCtrl   m_List_Loaded;
	CListCtrl   m_List_Instructions;
	CButton m_Group_Instructions;
	CButton m_Group_Running;
	CButton m_Group_Loaded;
	CListCtrl   m_List_Memory;
	CListCtrl   m_List_Disassembly;
	CListCtrl   m_List_COP1_2;
	CListCtrl   m_List_COP1_1;
	CListCtrl   m_List_GPRS_2;
	CListCtrl   m_List_COP0_2;
	CListCtrl   m_List_COP0_1;
	CListCtrl   m_List_GPRS_1;
	CTabCtrl    m_TabDebug;
	CTabCtrl    m_TabRegs;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSOPS2Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void MemoryGo(unsigned int Address);
	void GenerateLoadedStats(void);
	void GenerateRunningStats(void);
	void GenerateInstructionsStats(void);
	void ShowStats(void);
	void DisassemblyGo(unsigned int Pos);
	void LogInstructions(void);
	unsigned int MemoryPos{ 0 };
	unsigned int DisassemblyPos{ 0 };
	unsigned int DisassemblyNum{ 0 };
	unsigned int MemoryNum{ 0 };
	void FillMemoryInfo(void);
	void ShowMemory();
	void Disassembly();
	void ShowDisassembly();
	void ChangeTabDebug();
	void ShowCOP1(void);
	void ChangeTabRegs();
	void ShowCOP0(void);
	void ShowCOP2_FPR(void);
	void ShowCOP2_IR(void);
	void ShowGPR(void);
	CToolBar TBFile;
	CToolBar TBDebug;
	HICON m_hIcon;
	CFont font;

	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CPSOPS2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeTabRegs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabDebug(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListDisassembly(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListMemory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonStepInto();
	afx_msg void OnClickListDisassembly(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRun();
	afx_msg void OnButtonStepOver();
	afx_msg void OnClickListInstructions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRestart();
	afx_msg void OnButtonGoto();
	afx_msg void OnButtonFind();
	afx_msg void OnButtonExec();
	afx_msg void OnButtonBios();
	afx_msg void OnButtonConfig();
	afx_msg void OnButtonOptions();
	afx_msg void OnDestroy();
	afx_msg void OnButtonPadConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSOPS2DLG_H__A67B826C_B773_4B95_A448_44724A0A14C6__INCLUDED_)
