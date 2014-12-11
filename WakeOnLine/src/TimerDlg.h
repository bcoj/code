#if !defined(AFX_TIMERDLG_H__D25D8A5B_3EB2_447D_B8EE_06E5A5CA812F__INCLUDED_)
#define AFX_TIMERDLG_H__D25D8A5B_3EB2_447D_B8EE_06E5A5CA812F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTimerDlg dialog

class CTimerDlg : public CDialog
{
// Construction
public:
	CTimerDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_stime ;
// Dialog Data
	//{{AFX_DATA(CTimerDlg)
	enum { IDD = IDD_Timer };
	CDateTimeCtrl	m_Time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimerDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMERDLG_H__D25D8A5B_3EB2_447D_B8EE_06E5A5CA812F__INCLUDED_)
