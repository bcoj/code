#if !defined(AFX_ADDMAC_H__DAFA7337_BCEC_4D32_9877_7F4C992D2021__INCLUDED_)
#define AFX_ADDMAC_H__DAFA7337_BCEC_4D32_9877_7F4C992D2021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddMac.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddMac dialog

class CAddMac : public CDialog
{
// Construction
public:
	CAddMac(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddMac)
	enum { IDD = IDD_AddMac };
	CString	m_edit_ip;
	CString	m_edit_mac;
	CString	m_edit_host;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddMac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	// Generated message map functions
	//{{AFX_MSG(CAddMac)
	afx_msg void OnGetMac();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMAC_H__DAFA7337_BCEC_4D32_9877_7F4C992D2021__INCLUDED_)
