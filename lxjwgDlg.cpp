// lxjwgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lxjwg.h"
#include "lxjwgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLxjwgDlg dialog

CLxjwgDlg::CLxjwgDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLxjwgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLxjwgDlg)
	m_editlxj = _T("");
	m_checklxj = FALSE;
	m_checkmoney = FALSE;
	m_editmoney = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if(!pal5q.privilegeUp())
		MessageBox("获取访问令牌失败!");
	pal5q.startScan();
	pm.start();
}

void CLxjwgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLxjwgDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_Hook);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Text(pDX, IDC_EDIT1, m_editlxj);
	DDX_Check(pDX, IDC_CHECK1, m_checklxj);
	DDX_Check(pDX, IDC_CHECK2, m_checkmoney);
	DDX_Text(pDX, IDC_EDIT2, m_editmoney);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLxjwgDlg, CDialog)
//{{AFX_MSG_MAP(CLxjwgDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLxjwgDlg message handlers

BOOL CLxjwgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLxjwgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
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
HCURSOR CLxjwgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLxjwgDlg::OnOK() 
{
	static bool started = false;
	// TODO: Add extra validation here
	if(started == false){
		if(!autoClick.start()){
			return;
		}
		m_OK.SetWindowText("关闭自动连携技");
		started = true;
	}else{
		autoClick.stop();
		m_OK.SetWindowText("自动连携技");
		started = false;
	}
	
}

void CLxjwgDlg::OnButton1() 
{
	static bool started = false;
	// TODO: Add extra validation here
	if(started == false){
		if(!freeCam.start()){
			return;
		}
		m_Hook.SetWindowText("关闭摄像机漫游");
		started = true;
	}else{
		freeCam.stop();
		m_Hook.SetWindowText("摄像机漫游");
		started = false;
	}
}

void CLxjwgDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	autoClick.stop();
	freeCam.stop();
	pm.stop();
	CDialog::OnClose();
}

void CLxjwgDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	pm.setLocked(m_checklxj, atoi(m_editlxj), PropertyModify::LXJYS);
}

void CLxjwgDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	m_editlxj.Format("%u", pm.getValue(PropertyModify::LXJYS));
	m_editmoney.Format("%u", pm.getValue(PropertyModify::MONEY));
	this->UpdateData(FALSE);
}

void CLxjwgDlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	pm.setLocked(m_checkmoney, atoi(m_editlxj), PropertyModify::MONEY);

}

void CLxjwgDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	pm.setValue(atoi(m_editlxj), PropertyModify::LXJYS);
}

void CLxjwgDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	pm.setValue(atoi(m_editmoney), PropertyModify::MONEY);	
}
