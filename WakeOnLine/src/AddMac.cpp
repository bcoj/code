// AddMac.cpp : implementation file
//

#include "stdafx.h"
#include "WakeOnLan.h"
#include "AddMac.h"
#include "WakeOnLanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddMac dialog


CAddMac::CAddMac(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddMac)
	m_edit_ip = _T("");
	m_edit_mac = _T("");
	m_pParent=pParent;
	m_edit_host = _T("");
	//}}AFX_DATA_INIT
}


void CAddMac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddMac)
	DDX_Text(pDX, IDC_EDIT_IP, m_edit_ip);
	DDX_Text(pDX, IDC_EDIT_MAC, m_edit_mac);
	DDX_Text(pDX, IDC_ADD_HOST, m_edit_host);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddMac, CDialog)
	//{{AFX_MSG_MAP(CAddMac)
	ON_BN_CLICKED(IDOK2, OnGetMac)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddMac message handlers

void CAddMac::OnGetMac() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CWakeOnLanDlg *WakeOnLanDlg=(CWakeOnLanDlg*)m_pParent;
	WakeOnLanDlg->GetRemoteMacByIp(m_edit_mac, m_edit_ip);
	WakeOnLanDlg->GetHostNameByIp(m_edit_host, m_edit_ip);

	UpdateData(FALSE);
}

void CAddMac::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
