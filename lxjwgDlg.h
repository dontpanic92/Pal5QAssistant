// lxjwgDlg.h : header file
//

#if !defined(AFX_LXJWGDLG_H__B7C5D280_D108_4B9C_A02B_3D1F6AE5BEF4__INCLUDED_)
#define AFX_LXJWGDLG_H__B7C5D280_D108_4B9C_A02B_3D1F6AE5BEF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLxjwgDlg dialog
#include "wtf.h"

class CLxjwgDlg : public CDialog
{
// Construction
public:
	CLxjwgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLxjwgDlg)
	enum { IDD = IDD_LXJWG_DIALOG };
	CButton	m_Hook;
	CButton	m_OK;
	CString	m_editlxj;
	BOOL	m_checklxj;
	BOOL	m_checkmoney;
	CString	m_editmoney;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLxjwgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	AutoClick autoClick;
	FreeCamera freeCam;
	PropertyModify pm;
	// Generated message map functions
	//{{AFX_MSG(CLxjwgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnClose();
	afx_msg void OnCheck1();
	afx_msg void OnButton3();
	afx_msg void OnCheck2();
	afx_msg void OnButton2();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LXJWGDLG_H__B7C5D280_D108_4B9C_A02B_3D1F6AE5BEF4__INCLUDED_)
