// WakeOnLanDlg.cpp : implementation file
//
#define   _WIN32_WINNT     0x0510   

#include "stdafx.h"
#include "WakeOnLan.h"
#include "WakeOnLanDlg.h"
#include "AddMac.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include   <process.h> 

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include   <time.h> 
#include   <sys/types.h> 
#include   <sys/stat.h> 

#include "TimerDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWakeOnLanDlg dialog

CWakeOnLanDlg::CWakeOnLanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWakeOnLanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWakeOnLanDlg)

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&cs);
	m_ThreadCount=0;
	m_bSearchFlag=false;
}

void CWakeOnLanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWakeOnLanDlg)
	DDX_Control(pDX, IDC_STATIC_NextTime, m_NextTime);
	DDX_Control(pDX, IDC_STATICTime, m_StaticTime);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWakeOnLanDlg, CDialog)
	//{{AFX_MSG_MAP(CWakeOnLanDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WakeOnLan, OnWakeSubmit)
	ON_BN_CLICKED(IDC_SEARCHMAC, OnSearchMacSubmit)
	ON_BN_CLICKED(IDC_SelectAll, OnSelectAll)
	ON_BN_CLICKED(IDC_Timer, OnTimerSubmit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AddMac, OnAddMac)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SORT, OnSortSubmit)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteSubmit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_BN_CLICKED(ID_POPMENU_DELETE, OnDeleteSubmit)
	ON_BN_CLICKED(ID_POPMENU_WOL, OnWakeSubmit)
	ON_BN_CLICKED(ID_POPMENU_SETTIME, OnTimerSubmit)
	ON_BN_CLICKED(ID_POPMENU_SAVE, SaveMacToFile)
	ON_BN_CLICKED(ID_POPMENU_MOVEUP, OnMOVEUPSubmit)
 	ON_BN_CLICKED(ID_POPMENU_MOVEDOWN, OnMOVEDOWNSubmit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWakeOnLanDlg message handlers

BOOL CWakeOnLanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//LVS_EX_FULLROWSELECT ѡ��ĳһ����  ; LVS_EX_GRIDLINES  ��ʾ���� ; LVS_SHOWSELALWAYS  һֱΪѡ��״̬
	m_nlastsend = 0;
	m_bSearchFlag=false;
	m_bThreadRunFlag=false;
	m_StaticTime.SetWindowText("");
	m_NextTime.SetWindowText("");
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_List.InsertColumn(0,"",LVCFMT_LEFT,30,-1); 

	m_List.InsertColumn(1,"IP ",LVCFMT_LEFT,110,-1); 
	m_List.InsertColumn(2,"MAC",LVCFMT_LEFT,130,-1);
	m_List.InsertColumn(3,"Host Name",LVCFMT_LEFT,140,-1);
	m_List.InsertColumn(4,"Wake Time",LVCFMT_LEFT,70,-1);
	m_List.InsertColumn(5,"Wake Log",LVCFMT_LEFT,310,-1);


	// TODO: Add extra initialization here
	//����mac.txt��ļ�¼
	LoadMacFormFile();
	m_List.EnsureVisible(0, FALSE);
	//�ָ���ʱ��Ϣ
 	SetTimer(1,1000,NULL);
	//��ʼ�� from ip to ip
	DWORD	dwAddIP,dwStartIP,dwEndIP,dwMask,dwLocalIP;
	CString LocalIp;
	sockaddr_in   inIP; 
	m_progress.SetRange(0,255);
	dwMask=inet_addr("255.255.255.0");
	dwAddIP=inet_addr("0.0.0.1");
	GetLocalIP(LocalIp);
	dwLocalIP	= inet_addr(LocalIp);
	dwStartIP	=(dwLocalIP & dwMask) + dwAddIP;
	dwEndIP		=(dwLocalIP | inet_addr("0.0.0.255")) - dwAddIP ;  
	inIP.sin_addr.s_addr=dwStartIP;
	if(GetDlgItem(IDC_EDIT_IPFrom))
	{
		GetDlgItem(IDC_EDIT_IPFrom)->SetWindowText(inet_ntoa(inIP.sin_addr));
	}
	inIP.sin_addr.s_addr=dwEndIP;
	if(GetDlgItem(IDC_EDIT_IPTo))
	{
		GetDlgItem(IDC_EDIT_IPTo)->SetWindowText(inet_ntoa(inIP.sin_addr));
	}
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWakeOnLanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWakeOnLanDlg::OnPaint() 
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
HCURSOR CWakeOnLanDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWakeOnLanDlg::OnWakeSubmit() 
{
	// TODO: Add your control notification handler code here
//	char * Mac=NULL;
	CString csLog;
	CTime tNow=CTime::GetCurrentTime();
	CString csTime =tNow.Format("%Y-%m-%d %H:%M:%S");
	// 	m_Mac.GetWindowText(csMac);
//  Mac=(LPSTR)(LPCTSTR)csMac;
	POSITION pss=m_List.GetFirstSelectedItemPosition();//pos�����������ţ� 
	if(pss==NULL)
		AfxMessageBox("no item were selected!");
	else
	{
		while(pss)//�ؼ��������ѡ�����
		{  
			int nIdx=-1;
			nIdx	= m_List.GetNextSelectedItem(pss);//������--
			if(nIdx >=0&&nIdx<m_List.GetItemCount())
			{
				CString csMac = m_List.GetItemText(nIdx,2);
				CString Ip	 =	m_List.GetItemText(nIdx,1);
				WakeOnLanByMac((LPSTR)(LPCTSTR)csMac);
				csLog.Format("Last Sent in %s.",csTime);
				m_List.SetItemText(nIdx,5,csLog );
			}
		}
	}
	m_List.SetFocus(); // ���������û�ListCtrl
	AfxMessageBox("Finish send Magic Packet.");

}


//IPAddress:	��ȥ����ź��IP��ȫ����255     ,��   210.45.234.255 
//MacAddress:   6���ֽڵ��ַ������飬ÿ����������һ�������ַ����Window98�п����� 
//                               winipcfg.exe   ��ѯ������ 
int   CWakeOnLanDlg::WakeOnLanByMac(char * Mac) 
{ 
		if( !Mac) return 0;
	//���MACλ��
		if(strlen(Mac)!=17)
		{
			OutputDebugString("Example: %s 00-D0-4C-BF-52-BA");
			return 0;
		}
		
		//���MAC��ַ�Ƿ���ȷ  isxdigit()  �Ƿ���ʮ������
		for (char * a=Mac; *a; a++)
		{
			if (*a!='-' && !isxdigit(*a))
			{
				fprintf(stderr, "MAC Adresse must be like this: 00-D0-4C-BF-52-BA");
				return 1;
			}
		}	
			int dstaddr[6];
			int i=sscanf(Mac, "%2x-%2x-%2x-%2x-%2x-%2x",
                &dstaddr[0], &dstaddr[1], &dstaddr[2], &dstaddr[3], 
				
				&dstaddr[4], &dstaddr[5]);
			if (i!=6)
			{
				fprintf(stderr, "Invalid MAC Adresse!");
				return 1;
			}
			
			unsigned char ether_addr[6];
			for (i=0; i<6; i++)
				ether_addr[i]=(unsigned char)dstaddr[i];
			
			//����Magic Packet
			u_char magicpacket[200];
			memset(magicpacket, 0xff, 6);
			int packetsize=6;
			for (i=0; i<16; i++)
			{
				memcpy(magicpacket+packetsize, ether_addr, 6);
				packetsize+=6;
			}
			
			//����WSA
			WSADATA WSAData;
			if (WSAStartup( MAKEWORD(2, 0), &WSAData)!=0)
			{
				fprintf(stderr, "WSAStartup failed: %d\n", GetLastError());
				return 1;
			}
			
			//����socket
			SOCKET sock=socket(AF_INET, SOCK_DGRAM, 0);
			if (sock==INVALID_SOCKET)
			{
				fprintf(stderr, "Socket create error: %d\n", GetLastError());
				return 1;
			}
			
			//����Ϊ�㲥����
			BOOL bOptVal=TRUE;
			int iOptLen=sizeof(BOOL);
			if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, 
				
				iOptLen)==SOCKET_ERROR)
			{
				fprintf(stderr, "setsockopt error: %d\n", WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return 1;
			}
			
			sockaddr_in to;
			to.sin_family=AF_INET;
			to.sin_port=htons(0);
			to.sin_addr.s_addr=htonl(INADDR_BROADCAST);
			
			//����Magic Packet
			if (sendto(sock, (const char *)magicpacket, packetsize, 0, (const 
				
				struct sockaddr *)&to, sizeof(to))==SOCKET_ERROR) 
				fprintf(stderr, "Magic packet send error: %d", WSAGetLastError());
			else
				printf("Magic packet send!");
			
			closesocket(sock);
			WSACleanup();
/*			*/
	return 0;
		
	
}

int CWakeOnLanDlg::GetHostNameByIp(CString & HostName,	DWORD dwIP )//��ȡĳIP��ʵhostname��ַ �����
{
	WSADATA wsadata;
	WORD dwVersionRequested;
	int err;
	dwVersionRequested=MAKEWORD(1,1);
	err=WSAStartup(dwVersionRequested,&wsadata);
	if(err)
	{
		return FALSE;
	}
	struct hostent *pHost = gethostbyaddr((char *)&dwIP,4,PF_INET);
	if(pHost)
	{
		HostName.Format("%s",pHost->h_name);
	}
	WSACleanup();
	return TRUE;
	
}
int CWakeOnLanDlg::GetHostNameByIp(CString &sHostName,CString sRemoteIP)
{
	DWORD dwIP ;
 	dwIP  = inet_addr(sRemoteIP);
	return GetHostNameByIp(sHostName,dwIP);
}
int CWakeOnLanDlg::GetRemoteMacByIp(CString & sMacAddress,CString sRemoteIP )//��ȡĳIP��ʵmac��ַ �����
{
	DWORD dwIP ;
 	dwIP  = inet_addr(sRemoteIP);
	return GetRemoteMacByIp(sMacAddress,dwIP);
}
int CWakeOnLanDlg::GetRemoteMacByIp(CString & sMacAddress,DWORD remoteIP)//��ȡĳIP��ʵmac��ַ �����
{
	WSADATA wsdata;
	ULONG macAddrlen=6;
	unsigned char remoteMacTemp[6]={0};
	unsigned char remoteMac[6]={0};
	if(WSAStartup(MAKEWORD(2,1),&wsdata)!=0)
	{
		printf("WSAStartup Error!\r\n");
		return FALSE;
	}
	//remoteAddr=inet_addr(remoteIP);
    if(SendARP(remoteIP,(unsigned long)NULL,(PULONG)&remoteMacTemp,&macAddrlen)!=NO_ERROR)
	{
		printf("Get Remote MAC failed!\r\n");
		return FALSE;
	}
	memcpy(remoteMac,remoteMacTemp,6); 
	char * s="-";
	sMacAddress.Format(_T("%02x%s%02x%s%02x%s%02x%s%02x%s%02x"),
			 remoteMac[0],s,
			 remoteMac[1],s,
			 remoteMac[2],s,
			 remoteMac[3],s,
			 remoteMac[4],s,
			 remoteMac[5]);
   	OutputDebugString(sMacAddress);
	return TRUE; 
}
void CWakeOnLanDlg::OnSearchMacSubmit() 
{
	if(m_bSearchFlag==true) //stop search
	{
		if(GetDlgItem(IDC_SEARCHMAC))
		{
			GetDlgItem(IDC_SEARCHMAC)->SetWindowText("Search Mac");
		}
		//stop thread
		m_bSearchFlag=false;
		return;
	}
	else  //start search
	{
		if(GetDlgItem(IDC_SEARCHMAC))
		{
			GetDlgItem(IDC_SEARCHMAC)->SetWindowText("Stop");
		}
		m_bSearchFlag=true;

		CString LocalIp,StartIP,EndIP; 
		sockaddr_in   inIP; 
		DWORD	dwAddIP,dwStartIP,dwEndIP,dwMask,dwLocalIP;
		dwAddIP=inet_addr("0.0.0.1");
		
		//�����߳����� SearchMac����
		GetDlgItem(IDC_EDIT_IPFrom)->GetWindowText(StartIP);
		GetDlgItem(IDC_EDIT_IPTo)->GetWindowText(EndIP);
		
		if(EndIP == "" || StartIP == "")
		{
			dwMask=inet_addr("255.255.255.0");
			GetLocalIP(LocalIp);
			dwLocalIP	= inet_addr(LocalIp);
			dwStartIP	=dwLocalIP & dwMask;
			dwEndIP		=dwLocalIP | inet_addr("0.0.0.254");  
			inIP.sin_addr.s_addr=dwStartIP;
			GetDlgItem(IDC_EDIT_IPFrom)->SetWindowText(inet_ntoa(inIP.sin_addr));
			inIP.sin_addr.s_addr=dwEndIP;
			GetDlgItem(IDC_EDIT_IPTo)->SetWindowText(inet_ntoa(inIP.sin_addr));
			
		}
		//�ж�ip��ַ��ʽ�Ƿ���ȷ
		
		if(-1 == inet_addr((LPSTR)(LPCTSTR)StartIP) || -1 == inet_addr((LPSTR)(LPCTSTR)EndIP)  )
		{
			AfxMessageBox(" Ip format wrong.");
			return ;
		}
		else
		{
			_beginthread(ThreadFun,0,this);
		}

	}
	SaveMacToFile();
	return;
}
	/*
	 *   ѭ��ÿ��ip �����mac �ͼ������
	 *   �ڵ��search mac ʱ�������ڽ����б�����
	 *		
	 *
	 */
void CWakeOnLanDlg::SearchMac() 
{
	// TODO: Add your control notification handler code here
	CString sLocalIp,sStartIP,sEndIP, sMessage;; 
//	sockaddr_in   inIP; 
	DWORD	dwAddIP,dwStartIP,dwEndIP,dwMask,dwloopIP;
	dwAddIP=inet_addr("0.0.0.1");
	dwMask=inet_addr("255.255.255.0");

	//�ӽ����ÿ�ʼIP����ֹIP ��ת����DWORD��
	GetDlgItem(IDC_EDIT_IPFrom)->GetWindowText(sStartIP);
	GetDlgItem(IDC_EDIT_IPTo)->GetWindowText(sEndIP);
	dwStartIP=inet_addr(sStartIP);
	dwEndIP  =inet_addr(sEndIP);

	//����������ĳ���
	m_nLower=0;
	m_nUpper=255;
	m_progress.SetRange((short)m_nLower,(short)m_nUpper);

	//��¼List��ʼ�ͽ���λ����������
	m_ListStart=m_List.GetItemCount();
	int nCount=0;
	m_ThreadCount=0;
	//s����ip����λ
	sMessage.Format("StartIP is : %s , EndIP is %s",sStartIP,sEndIP);
	OutputDebugString(sMessage);	


	for(dwloopIP  = dwStartIP; 
		dwloopIP < dwEndIP  ;  
		dwloopIP += dwAddIP  )
	{
		//������stop ���˳�
		if(m_bSearchFlag==false)
		{
			return;
		}
		nCount++;
		Sleep(5);
		AfxBeginThread((AFX_THREADPROC)AddMacToListByIp,(LPVOID) dwloopIP);
	}

	while(m_ThreadCount!=nCount)//����ɨ�贿�̽��� �� ��������
	{
		Sleep(20);	
	}
	ListSort(m_ListStart,m_List.GetItemCount()-1);
	AfxMessageBox("Finish search. ",MB_ICONINFORMATION);
}


//�õ�����ip
//Ŀǰֻ�ܵõ�Ĭ��������ip,�����ܶԶ��������д���
BOOL     CWakeOnLanDlg::GetLocalIP(CString & LocalIp) 
{ 
	 WORD  wVersionRequested;  
	 WSADATA  wsaData;  
	 char  name[255];  
	 CString  ip; 
	 int err;	 
	 PHOSTENT  hostinfo;  
	 wVersionRequested  =  MAKEWORD( 1, 1 );//�汾��1.1 
	 
	 //1.�����׽��ֿ� 
	 err = WSAStartup( wVersionRequested, &wsaData );
	 if ( err != 0 ) {		 
		 return FALSE;
	 }
	 //�ж��Ƿ����������winsocket�汾���������
	 //�����WSACleanup��ֹwinsocket��ʹ�ò�����             
	 if ( LOBYTE( wsaData.wVersion ) != 1 ||
		 HIBYTE( wsaData.wVersion ) != 1 )
	 {		 
		 WSACleanup( );
		 return FALSE; 
	 }///...if
	 if  ( err  ==  0  )  
	 {  
		 if(  gethostname  (  name,  sizeof(name))  ==  0)  
		 {  
			 if((hostinfo  =  gethostbyname(name))  !=  NULL)  
			 {  
				 ip  =  inet_ntoa  (*(struct  in_addr  *)*hostinfo->h_addr_list);
			 }  
		 }  
		 WSACleanup(  );  
	 }   
//	ina=*(struct  in_addr  *)*hostinfo->h_addr_list;
	LocalIp=ip;
	return TRUE;
} 
//���̺߳��� ���÷����� 	_beginthread(ThreadFun,0,this);
//
void   CWakeOnLanDlg::ThreadFun(LPVOID Parent)
{
	LPVOID pParent;
	pParent=Parent;
	CWnd *pWnd=AfxGetMainWnd();
	if(pWnd)
	{
		((CWakeOnLanDlg *)pWnd)->m_bThreadRunFlag=true;
		OutputDebugString("find the main wnd.");
		((CWakeOnLanDlg *)pWnd)->SearchMac();
		((CWakeOnLanDlg *)pWnd)->SaveMacToFile();
		((CWakeOnLanDlg *)pWnd)->m_bThreadRunFlag=false;
	}
}

void CWakeOnLanDlg::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_List.GetItemCount(); i++)
	{
		m_List.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}
	m_List.SetFocus(); // ���������û�ListCtrl
}

void CWakeOnLanDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bSearchFlag=false;
// 	while(bThreadRunFlag)
// 	{
// 		Sleep(100);
// 	}

// 	SaveMacToFile();
 	CDialog::OnCancel();
}

//���б��浽�ļ�mac.txt
void CWakeOnLanDlg::SaveMacToFile()
{
	struct   _stat   fstat; 
	int   fsize; 
	
	FILE *pFile;
	//char msg[] = "mac list file for wol"; 
	//char buf[200]={'\0'} ; 
	if ((pFile = fopen("mac.txt", "w+")) == NULL) 
	{
		return ;
	}
	fsize   =   _stat(  "mac.txt",   &fstat   ); 
		
//	fseek(file, 0, SEEK_SET); 
//	fread(buf, fstat.st_size, 1,file); 
	CString strHead;
	CString strLocalip;
	GetLocalIP(strLocalip);
	CTime tNow=CTime::GetCurrentTime(); 
	CString strTime=tNow.Format("%Y-%m-%d %H:%M:%S");
	strHead.Format("WakeOnLan.exe ����ip�� %s  ʱ��: %s \n",strLocalip,strTime);
	fwrite(strHead, strlen(strHead), 1, pFile);

	for (int nIdx = 0; nIdx < m_List.GetItemCount(); nIdx++)
	{
		CString sMac,sIP,sHost,sLine,sTime,sLog;
		sIP	=	m_List.GetItemText(nIdx,1);
		sMac=	m_List.GetItemText(nIdx,2);
		sHost=	m_List.GetItemText(nIdx,3);
		sTime=	m_List.GetItemText(nIdx,4);
		sLog=	m_List.GetItemText(nIdx,5);
		if( sIP=="") sIP="null";
		if( sMac=="") sMac="null";
		if( sHost=="") sHost="null";
		if( sTime=="") sTime="null";
		if( sLog=="") sLog="null";
		sLine.Format("%-15s	%-17s	%-20s	%-8s	%s\n",sIP,sMac,sHost,sTime,sLog);
		fwrite(sLine, strlen(sLine), 1, pFile);
	}
	m_List.SetFocus(); // ���������û�ListCtrl
	fclose(pFile); 
	return ;
}
//���ļ�����mac��Ϣ�� list��
bool CWakeOnLanDlg::LoadMacFormFile()
{
	struct   _stat   fstat; 
	int   fsize; 
	FILE *pFile;
	//char buf[200]={'\0'}; 
	if ((pFile = fopen("mac.txt", "r+")) == NULL) 
	{
		return 0;
	}
	fsize   =   _stat(  "mac.txt",   &fstat   ); 
	
	fseek(pFile, 0, SEEK_SET); 
	char sLine[256] = "";

	while (NULL != fgets(sLine, sizeof(sLine), pFile))
	{
		char sIP[32] = "";
		char sMac[32] = "";
		char sHost[32] = "";
		char sTime[32]="";
		char sLog[100]="";
		sscanf(sLine,"%s ",&sIP);		
		if(INADDR_NONE!=inet_addr(sIP) &&strlen(sIP)>=7)
		{
			sscanf(sLine,"%s %s %s %s %s",&sIP,&sMac,&sHost,&sTime,&sLog);
			if( strcmp(sIP	,"null")==0)  strcpy(sIP	,"");
			if( strcmp(sMac	,"null")==0)  strcpy(sMac	,"");
			if( strcmp(sHost,"null")==0)  strcpy(sHost	,"");
			if( strcmp(sTime,"null")==0)  strcpy(sTime	,"");
			if( strcmp(sLog,"null")==0)   strcpy(sLog	,"");
			if(strcmp(sLog,"Last")==0)
			{
				char* pStart=strstr(sLine,"Last");
				strncpy(sLog,pStart,strlen(pStart)-1);
			}
			InsertToList(sIP,sMac,sHost,sTime,sLog);
		}
	}
	m_List.SetFocus(); // ���������û�ListCtrl
	fclose(pFile); 
	return true;	
}

void CWakeOnLanDlg::OnTimerSubmit() 
{
	// TODO: Add your control notification handler code here
	CTimerDlg timerdlg;
 	
	if(timerdlg.DoModal() ==IDOK )
	{
		POSITION pss=m_List.GetFirstSelectedItemPosition();//pos�����������ţ� 
		if(pss==NULL)
		{
			AfxMessageBox("No rows selected!");
			return;
		}
		for (int nIdx =  m_List.GetNextSelectedItem(pss); nIdx>=0 && nIdx<m_List.GetItemCount() ; nIdx = m_List.GetNextSelectedItem(pss))
		{
			
			m_List.SetItemText(nIdx,4,timerdlg.m_stime);
		}
		SetTimer(1,1000,NULL); //һ��1�봥��һ�εĶ�ʱ��
		m_List.SetFocus(); // ���������û�ListCtrl

	}
	SaveMacToFile();
}

void CWakeOnLanDlg::OnTimer(UINT nIDEvent) 
{
	//������Ϳ����źź�10���ڶ���ʾ���ѷ���.����
	//m_nlastsend��ʼΪ0 ���з���ʱ ����Ϊ10���Լ�	
	CTime tNow=CTime::GetCurrentTime();
	CString csTime =tNow.Format("%H:%M:%S");
	CString csLog  =tNow.Format("Last auto sent in %Y-%m-%d %H:%M:%S .");

	if( m_nlastsend <= 0 )
	{
		m_StaticTime.SetWindowText(csTime);
	}
	else
	{
		m_nlastsend--;
	}
	int nCount=m_List.GetItemCount();
	for (int nIdx = 0; nIdx< nCount ;	nIdx ++)
	{		
		char sTime[20]={'\0'};//=m_List.GetItemText(nIdx,3);
		m_List.GetItemText(nIdx,4,sTime,20);
		if(	m_List.GetItemText(nIdx,4) == csTime  )
		{
			CString  csMac=m_List.GetItemText(nIdx,2);
			WakeOnLanByMac((LPSTR)(LPCTSTR)	csMac );
			Sleep(100);
			WakeOnLanByMac((LPSTR)(LPCTSTR)	csMac );
			OutputDebugString("�����ź��ѷ���.");
			m_StaticTime.SetWindowText("Signal has been sent.");
			m_List.SetItemText(nIdx,5,csLog);
			m_nlastsend = 10;
		}
	}

	CDialog::OnTimer(nIDEvent);
}


//��� Add Mac
void CWakeOnLanDlg::OnAddMac() 
{
	// TODO: Add your control notification handler code here
	CAddMac addmacDlg(this);
	if(addmacDlg.DoModal()== IDOK)
	{
		InsertToList(addmacDlg.m_edit_ip,addmacDlg.m_edit_mac,addmacDlg.m_edit_host,"");
	}
	SaveMacToFile();
}

void CWakeOnLanDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	
	CListCtrl* pList   =   (CListCtrl*)   GetDlgItem(IDC_LIST1); 
	if(pList)
	{
		CRect rectList;
		pList->GetWindowRect(&rectList);
		//pList->MoveWindow(30,30,rectList.right -rectList.left   , rectList.bottom -rectList.top  );
		CString DebugString;
		DebugString.Format("cx:%d,cy:%d,top:%d,left:%d,right:%d,bottom:%d\n top:%d,left:%d,right:%d,bottom:%d",cx,cy,rect.top,rect.left,rect.right,rect.bottom,rectList.top,rectList.left,rectList.right,rectList.bottom);
		//OutputDebugString(DebugString);
	}

}

bool CWakeOnLanDlg::InsertToList(CString sIP, CString sMac, CString sHostName ,CString sTime,CString sLog ,int nItem  )
{
		EnterCriticalSection(&cs);
		if(nItem==-1)
		{
			nItem = m_List.GetItemCount();
		}
		char  sItem[10];
		itoa(nItem,sItem,10);
		m_List.InsertItem(nItem,sItem);
		m_List.SetItemText(nItem,1,sIP);
		m_List.SetItemText(nItem,2,sMac);
		m_List.SetItemText(nItem,3,sHostName);
		m_List.SetItemText(nItem,4,sTime);
		m_List.SetItemText(nItem,5,sLog);
		m_List.EnsureVisible(nItem, FALSE);
		
	CBitmap bitmap;
	VERIFY(bitmap.LoadBitmap(IDB_SHAPES)); //װ��λͼ��Դ

	m_imagelist.Create(IDB_SHAPES,16,1,0);    //����ͼ���б�����ÿ��ͼ��Ŀ��Ϊ16����
            //����������Ϊ��������4������������ɫ����ʹͼ����͸��
	m_imagelist.Add(&bitmap,(COLORREF)0xFFFFF);   //ͼ���б��ʼ���󣬵���Add������ͼ����ӽ�ͼ���б��У�
             
	m_list.SetImageList(&m_imagelist,LVSIL_SMALL);  //����SetImageListʹ��ͼ���б���2����������ͼ������ΪСͼ��

		LeaveCriticalSection(&cs);
		return true;
}


UINT CWakeOnLanDlg::AddMacToListByIp(DWORD dwIP)
{
	CString sIp,sMac,sHostName="";
	sockaddr_in inIP;
	inIP.sin_addr.s_addr=dwIP;
	sIp=inet_ntoa (inIP.sin_addr);
	CWakeOnLanDlg *WakeOnLanDlg=(CWakeOnLanDlg*)(AfxGetApp()->m_pMainWnd);

	if(FALSE!=WakeOnLanDlg->GetRemoteMacByIp(sMac,dwIP))
	{
		WakeOnLanDlg->GetHostNameByIp(sHostName,sIp);
		WakeOnLanDlg->InsertToList(sIp,sMac,sHostName,"");
		CString str;
	}
	WakeOnLanDlg->m_progress.SetPos(WakeOnLanDlg->m_nLower++);
	
	CString str;
	str.Format("%i %i",WakeOnLanDlg->m_nLower,WakeOnLanDlg->m_nUpper);
	OutputDebugString(str);
	//OutputDebugString(itoa(WakeOnLanDlg->m_nUpper,nLower,8));

	if(WakeOnLanDlg->m_nLower>=WakeOnLanDlg->m_nUpper-2)
	{
		WakeOnLanDlg->m_bSearchFlag=false;
		WakeOnLanDlg->GetDlgItem(IDC_SEARCHMAC)->SetWindowText("Search Mac");
	}
	WakeOnLanDlg->m_ThreadCount++;
	return true;
}

bool CWakeOnLanDlg::ListSort(int nStart,int nEnd)
{
	for(int i=nEnd;i>=nStart;i--)
	{
		char sArray[5][20];
		char sLog[100];
		DWORD dwMaxIP=inet_addr("0.0.0.0");
		DWORD dwIP;
		char sIP[16];
		int  nMaxid=-1;
		for(int j=nStart;j<=i;j++) //��nIndex �� count-j
		{
			m_List.GetItemText(j,1,sIP,16);
			dwIP=inet_addr(sIP);
			if(htonl(dwMaxIP)<htonl(dwIP))
			{
				dwMaxIP=dwIP;
				nMaxid=j;
			}
		}
		//���� 	��  ɾ��
		m_List.GetItemText(nMaxid,1,sArray[0],20);
		m_List.GetItemText(nMaxid,2,sArray[1],20);
		m_List.GetItemText(nMaxid,3,sArray[2],20);
		m_List.GetItemText(nMaxid,4,sArray[3],20);
		m_List.GetItemText(nMaxid,5,sLog,100);
		m_List.DeleteItem(nMaxid);
		//д�뵽���
		InsertToList(sArray[0],sArray[1],sArray[2],sArray[3],sLog,i);
	}
	for(i=nStart;i<=nEnd;i++)
	{
		char a[5];
		itoa(i,a,10);
		m_List.SetItemText(i,0,a);
	}
return true;
}

void CWakeOnLanDlg::OnSortSubmit() 
{
	// TODO: Add your control notification handler code here
	ListSort(0,m_List.GetItemCount()-1);
	SaveMacToFile();
}
//ɾ��ѡ�е���
void CWakeOnLanDlg::OnDeleteSubmit() 
{
	// TODO: Add your control notification handler code here
		POSITION pos=m_List.GetFirstSelectedItemPosition();//pos�����������ţ� 
		if(pos==NULL)
		{
			AfxMessageBox("No rows selected!");
			return;
		}
		int nIndex;
		while(1)
		{
			pos=m_List.GetFirstSelectedItemPosition();
			if(pos!=NULL)
			{
				nIndex =  m_List.GetNextSelectedItem(pos);
				m_List.DeleteItem(nIndex);
			}else
			{  
				break;
			}
		}
		SaveMacToFile();
}



void CWakeOnLanDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu; 
	pNMHDR=pNMHDR;
	POINT pt = {0}; 
	GetCursorPos(&pt);//�õ������λ�� 
	menu.LoadMenu(IDR_LIST_POPUPMENU);//�˵���ԴID 
	CMenu *pSubMenu=menu.GetSubMenu(0);
	//SetMenu(&menu);//������������
	//menu.Detach();
	pSubMenu->TrackPopupMenu(0,pt.x+5,pt.y,this);     //m_newListCtrl��CListCtrl����
	*pResult = 0;
}
void CWakeOnLanDlg::OnMOVEUPSubmit( ) 
{
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nIndex =  m_List.GetNextSelectedItem(pos);
		
	}
}
void CWakeOnLanDlg::OnMOVEDOWNSubmit( ) 
{
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nIndex =  m_List.GetNextSelectedItem(pos);
		
	}
}
