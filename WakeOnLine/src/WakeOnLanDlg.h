// WakeOnLanDlg.h : header file
//

#if !defined(AFX_WAKEONLANDLG_H__EB650DDF_03D7_4A12_A518_31BAD24032AB__INCLUDED_)
#define AFX_WAKEONLANDLG_H__EB650DDF_03D7_4A12_A518_31BAD24032AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//  for sendarp()
#include <Iphlpapi.h>

//Library Use Iphlpapi.lib.   
//DLL Requires Iphlpapi.dll
#pragma comment(lib, "Iphlpapi.lib")
// end for

//add for wol
#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"wsock32.lib ")
#include <winsock.h>
#include <iostream.h>
//add end


//add for getmac
#include   <Nb30.h>  
typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER    NameBuff[30];
} ASTAT, * PASTAT;
//add end



/////////////////////////////////////////////////////////////////////////////
// CWakeOnLanDlg dialog
class CWakeOnLanDlg : public CDialog
{
// Construction
public:
	CWakeOnLanDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWakeOnLanDlg)
	enum { IDD = IDD_WAKEONLAN_DIALOG };
	CStatic	m_NextTime;
	CStatic	m_StaticTime;
	CButton	m_button1;
	CProgressCtrl	m_progress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWakeOnLanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	
    int			WakeOnLanByMac(char * Mac); 
	int 		GetLocalIP(CString & LocalIp);  //获取本机ip地址 
	static void ThreadFun(LPVOID Parent);
	bool		LoadMacFormFile();	
	int			m_nlastsend;
public:
	bool		InsertToList(CString sIP,CString sMac,CString sHostName="",CString sTime="" ,CString sLog="" ,int nItem=-1 );
	void		SearchMac();

	int			GetRemoteMacByIp(CString	&sMacAddress,DWORD		dwremoteIP); //获取某IP真实mac地址
	int			GetRemoteMacByIp(CString	&sMacAddress,CString	sremoteIP);
	int			GetHostNameByIp(CString		&sHostName	,DWORD		dwIP	);
	int			GetHostNameByIp(CString		&sHostName	,CString	sIP		);
	static UINT AddMacToListByIp(DWORD dIP);
	bool ListSort(int nStart,int nEnd);
private:

	CEdit m_IP;
	CEdit m_Mac;
	CListCtrl m_List;
	short m_nLower;
	short m_nUpper;
	CTime m_tNextTime;
	char m_sTime[20];
	int m_ListStart ;
	int m_ThreadCount ;

public:

	bool m_bSearchFlag;
	bool m_bThreadRunFlag;
	CRITICAL_SECTION cs;   //多线程写同一内存空间用的边界变量
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWakeOnLanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWakeSubmit();
	afx_msg void OnSearchMacSubmit();
	afx_msg void OnSelectAll();
	virtual void OnCancel();
	afx_msg void OnTimerSubmit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAddMac();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSortSubmit();
	afx_msg void OnDeleteSubmit();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void SaveMacToFile();
	afx_msg void  OnMOVEUPSubmit( ) ;
	afx_msg void  OnMOVEDOWNSubmit( ); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAKEONLANDLG_H__EB650DDF_03D7_4A12_A518_31BAD24032AB__INCLUDED_)
