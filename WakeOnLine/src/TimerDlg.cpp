// TimerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WakeOnLan.h"
#include "TimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimerDlg dialog


CTimerDlg::CTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimerDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimerDlg, CDialog)
	//{{AFX_MSG_MAP(CTimerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimerDlg message handlers

void CTimerDlg::OnOK() 
{
	// TODO: Add extra validation here
	CTime ttime;
	m_Time.GetTime(ttime);
	m_stime=ttime.Format("%H:%M:%S");
	CDialog::OnOK();
}

BOOL CTimerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTime   tNow=CTime::GetCurrentTime();
	tNow += CTimeSpan(0,0,2,0); 
	m_Time.SetTime(&tNow);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
