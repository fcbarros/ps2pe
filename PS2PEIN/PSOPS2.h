// PSOPS2.h : main header file for the PSOPS2 application
//

#if !defined(AFX_PSOPS2_H__6748D2C5_81E0_4249_BB87_15A28BC9E50E__INCLUDED_)
#define AFX_PSOPS2_H__6748D2C5_81E0_4249_BB87_15A28BC9E50E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPSOPS2App:
// See PSOPS2.cpp for the implementation of this class
//

class CPSOPS2App : public CWinApp
{
public:
    CPSOPS2App();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPSOPS2App)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CPSOPS2App)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSOPS2_H__6748D2C5_81E0_4249_BB87_15A28BC9E50E__INCLUDED_)
