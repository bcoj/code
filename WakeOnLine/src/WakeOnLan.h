// WakeOnLan.h : main header file for the WAKEONLAN application
//

#if !defined(AFX_WAKEONLAN_H__9D8ABB59_4142_4CEE_A0F7_37BF6624AFFC__INCLUDED_)
#define AFX_WAKEONLAN_H__9D8ABB59_4142_4CEE_A0F7_37BF6624AFFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWakeOnLanApp:
// See WakeOnLan.cpp for the implementation of this class
//

class CWakeOnLanApp : public CWinApp
{
public:
	CWakeOnLanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWakeOnLanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWakeOnLanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAKEONLAN_H__9D8ABB59_4142_4CEE_A0F7_37BF6624AFFC__INCLUDED_)
