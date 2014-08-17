// lxjwg.h : main header file for the LXJWG application
//

#if !defined(AFX_LXJWG_H__C771C431_6BDB_487E_9C0E_6000B65D949D__INCLUDED_)
#define AFX_LXJWG_H__C771C431_6BDB_487E_9C0E_6000B65D949D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLxjwgApp:
// See lxjwg.cpp for the implementation of this class
//

class CLxjwgApp : public CWinApp
{
public:
	CLxjwgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLxjwgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLxjwgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LXJWG_H__C771C431_6BDB_487E_9C0E_6000B65D949D__INCLUDED_)
