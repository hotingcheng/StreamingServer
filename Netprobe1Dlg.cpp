// Netprobe1Dlg.cpp : implementation file
//
////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "Netprobe1Dlg.h"
#include "host.h"
#include "ES_Network.h"
#include "ES_UDP.h"
#include "ES_TCP.h"
#include "ES_TCPClient.h"
#include "ES_TCPServer.h"
#include "IOCP.h"
#include <ws2tcpip.h>

#define numofTh 2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool Bfile=false;
bool Ufile=false;
bool recvfile=false;
LPCTSTR recvfname = "";
bool check = false;
bool run =false;
int num=1;

bool Ustart = false;
CString default_path = "C:\\";

bool MCast= false;

#define BUFSIZE     1024
#define MAXADDRSTR  16

#define TIMECAST_ADDR   "234.5.6.7"
#define TIMECAST_PORT   8123
#define TIMECAST_TTL    2
#define TIMECAST_INTRVL 30

char achMCAddr[MAXADDRSTR] = TIMECAST_ADDR;

u_long lMCAddr;
u_short nPort              = TIMECAST_PORT;
u_long  lTTL               = TIMECAST_TTL;
u_short nInterval          = TIMECAST_INTRVL;
SYSTEMTIME stSysTime;

int nRet, x;
BOOL  fFlag;
SOCKADDR_IN stLclAddr, stDstAddr;
SOCKET hS;
struct ip_mreq stMreq;        /* Multicast interface structure */
WSADATA stWSAData;


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
// CNetprobe1Dlg dialog

CNetprobe1Dlg::CNetprobe1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetprobe1Dlg::IDD, pParent),UDM_WINSOCK_SELECT(WM_USER+999)
{
	//{{AFX_DATA_INIT(CNetprobe1Dlg)
	m_Size = 10240;
	m_Refresh = 100;
	m_PeerPort = 8000;
	m_Peer = _T("localhost");
	m_PacketNo = 0;
	m_LocalPort = 8000;
	m_Local = _T("ieugp15");
	m_Rate = 750000;
	m_BufferNo = 5;
	m_MCF = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CNetprobe1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetprobe1Dlg)
	DDX_Control(pDX, IDC_MC, m_MC);
	DDX_Control(pDX, IDC_Conn5, m_Conn5);
	DDX_Control(pDX, IDC_PROGRESS5, m_Pg5);
	DDX_Control(pDX, IDC_PROGRESS4, m_Pg4);
	DDX_Control(pDX, IDC_PROGRESS3, m_Pg3);
	DDX_Control(pDX, IDC_PROGRESS2, m_Pg2);
	DDX_Control(pDX, IDC_PROGRESS1, m_Pg1);
	DDX_Control(pDX, IDC_Conn4, m_Conn4);
	DDX_Control(pDX, IDC_Conn3, m_Conn3);
	DDX_Control(pDX, IDC_Conn2, m_Conn2);
	DDX_Control(pDX, IDC_Conn1, m_Conn1);
	DDX_Control(pDX, IDC_CHECK1, m_CheckC);
	DDX_Control(pDX, IDC_BufferNo, m_BufferNoC);
	DDX_Control(pDX, IDC_Sync2, m_RecvBlock);
	DDX_Control(pDX, IDC_Sync, m_SendBlock);
	DDX_Control(pDX, IDC_Peer, m_PeerC);
	DDX_Control(pDX, IDC_Size, m_SizeC);
	DDX_Control(pDX, IDC_Refresh, m_RefreshC);
	DDX_Control(pDX, IDC_Peerport, m_PeerPortC);
	DDX_Control(pDX, IDC_PacketNo, m_PacketNoC);
	DDX_Control(pDX, IDC_Localport, m_LocalPortC);
	DDX_Control(pDX, IDC_InputRate, m_RateC);
	DDX_Control(pDX, IDC_Local, m_LocalC);
	DDX_Control(pDX, IDCANCEL, m_ButtonQuit);
	DDX_Control(pDX, IDC_Blocking, m_IOMode);
	DDX_Control(pDX, IDC_Async, m_Async);
	DDX_Control(pDX, IDC_UDP, m_UDP);
	DDX_Control(pDX, IDC_Receive, m_ButtonRecv);
	DDX_Control(pDX, IDC_Send, m_ButtonSend);
	DDX_Control(pDX, IDC_TCP, m_Protocol);
	DDX_Text(pDX, IDC_Size, m_Size);
	DDX_Text(pDX, IDC_Refresh, m_Refresh);
	DDX_Text(pDX, IDC_Peerport, m_PeerPort);
	DDX_Text(pDX, IDC_Peer, m_Peer);
	DDX_Text(pDX, IDC_PacketNo, m_PacketNo);
	DDX_Text(pDX, IDC_Localport, m_LocalPort);
	DDX_Text(pDX, IDC_Local, m_Local);
	DDX_Text(pDX, IDC_InputRate, m_Rate);	
	DDX_Control(pDX, IDC_Alert, m_Alert);
	DDX_Text(pDX, IDC_BufferNo, m_BufferNo);
	DDX_Text(pDX, IDC_MCF, m_MCF);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetprobe1Dlg, CDialog)
	//{{AFX_MSG_MAP(CNetprobe1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_HostInfo, OnHostInfo)
	ON_BN_CLICKED(IDC_Send, OnSend)
	ON_BN_CLICKED(IDC_Receive, OnReceive)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_SendFile, OnSendFile)
	ON_BN_CLICKED(IDC_RecvFile, OnRecvFile)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_DOUBLECLICKED(IDC_CHECK1, OnDoubleclickedCheck1)
	ON_BN_CLICKED(IDC_MC, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetprobe1Dlg message handlers

BOOL CNetprobe1Dlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here

	check = true;
	m_CheckC.SetCheck(1);
	m_CheckC.EnableWindow(false);
	m_Alert.SetCheck(1);	
	m_Protocol.SetCheck(1);
	m_IOMode.SetCheck(1);
	m_IOMode.EnableWindow(false);
	UpdateData(false);
	m_ButtonRecv.EnableWindow(false);
	m_Protocol.EnableWindow(false);
	m_UDP.EnableWindow(false);
	m_Async.EnableWindow(false);
	m_SendBlock.EnableWindow(false);
	m_RecvBlock.EnableWindow(false);
	m_Alert.EnableWindow(false);
	m_SizeC.EnableWindow(true);
	m_RefreshC.EnableWindow(false);
	m_PacketNoC.EnableWindow(false);
	m_PeerC.EnableWindow(false);
	m_PeerPortC.EnableWindow(false);

	NumOfClient=0;
	QuitFlag = TRUE;

	m_Pg1.SetRange32(0,100);
	m_Pg2.SetRange32(0,100);
	m_Pg3.SetRange32(0,100);
	m_Pg4.SetRange32(0,100);
	m_Pg5.SetRange32(0,100);


	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetprobe1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNetprobe1Dlg::OnPaint() 
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
HCURSOR CNetprobe1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


////////////////////////////////
//	Send Thread Procedure
///////////////////////////////
DWORD WINAPI CNetprobe1Dlg::ThreadSendProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;

	pclass->RunSendMode(pclass);
	pclass->m_ButtonSend.SetWindowText("Send");
		
	CloseHandle(pclass->hThread1);
	return 0;
}

int CNetprobe1Dlg::StartDelayThread()
{
	DWORD ThreadID;
	hThread4 = CreateThread(0, 
						   0, 
						   CNetprobe1Dlg::ThreadDelayProc,
						   (LPVOID)this, 
						   0, 
						   &ThreadID);

	if (hThread4 == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

DWORD WINAPI CNetprobe1Dlg::ThreadDelayProc(LPVOID IpInstance)
{
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;

	pclass->OKrecv = false;

	Sleep(1000);

//	pclass->OKrecv = true;

	pclass->OnClose();
	AfxMessageBox("Close Connection by Sender");

	DWORD dwExitCode=0;

	TerminateThread(pclass->hThread4,dwExitCode);
	CloseHandle(pclass->hThread4);

	return 0;
}

/////////////////////////////////////////////////////////////////////////
//	Send Thread: for sending packet and updating the sender statistic	 
/////////////////////////////////////////////////////////////////////////
int CNetprobe1Dlg::StartThreadedSend()
{
	DWORD ThreadID;
	hThread1 = CreateThread(0, 
						   0, 
						   CNetprobe1Dlg::ThreadSendProc,
						   (LPVOID)this, 
						   0, 
						   &ThreadID);

	if (hThread1 == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

////////////////////////////////////////////////////////
//		STOP Send Thread
////////////////////////////////////////////////////////
int CNetprobe1Dlg::StopThreadedSend()
{
	DWORD dwExitCode=0;

	TerminateThread(hThread1,dwExitCode);
	CloseHandle(hThread1);

//	if (file)
//	{	
//		m_filepath="";
//		UpdateData(false);
//	}
	
	return 0;
}

///////////////////////////////////////////////////////////////
//	Thread Accept Procedure TCP
///////////////////////////////////////////////////////////////
DWORD WINAPI CNetprobe1Dlg::ThreadAcceptProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;
	int i;
//	int received;
//	struct sockaddr_in _addr;
//	char *buf = new char[100];	
	int dlen = sizeof(struct sockaddr_in);

	while(1)
	{
			if (pclass->pTCPServer->IOCPAccept(&pclass->ConnClient[pclass->NumOfClient],pclass->NumOfClient ))
			{			
				pclass->Tstart = true;
				///////////////////////////////////////////////////////////////
				//		Associate the client with IOCP
				///////////////////////////////////////////////////////////////

				HANDLE a = ::CreateIoCompletionPort((HANDLE)pclass->ConnClient[pclass->NumOfClient]->getClientSocket(),
											 pclass->m_iocp->getTCPHandle(),
											 (DWORD) pclass->ConnClient[pclass->NumOfClient],
											 pclass->m_iocp->getNumOfThread());

				if (!pclass->CreateO )
				{

					pclass->iocpOverlap = new WSAOVERLAPPED*[pclass->lBufferNum];
					pclass->iocpBuf = new WSABUF*[pclass->lBufferNum];

					for (i = 0; i < pclass->lBufferNum; i++)
					{
						//iocpOverlap[i] = new WSAOVERLAPPED;
						pclass->iocpOverlap[i] = (WSAOVERLAPPED *)malloc(sizeof(WSAOVERLAPPED));
						//	iocpOverlap[i]->hEvent = this;	

						pclass->iocpBuf[i] = new WSABUF;
						pclass->iocpBuf[i]->buf = new char[pclass->lPktSize];
						pclass->iocpBuf[i]->len = pclass->lPktSize;

						memset(pclass->iocpBuf[i]->buf, 0, pclass->iocpBuf[i]->len);
						memset(pclass->iocpOverlap[i], 0, sizeof(WSAOVERLAPPED));
						
					}
				}

				if (!pclass->CreateO )
					pclass->CreateO = true;

				
				pclass->ConnClient[pclass->NumOfClient]->ChangeState(pclass->m_iocp->gethConn());
				pclass->ConnClient[pclass->NumOfClient]->setProtocol(1);

				///////////////////////////////////////////////////////////////
				//	set Progress Bar
				///////////////////////////////////////////////////////////////
				pclass->NoOfCon++;
				pclass->ConnClient[pclass->NumOfClient]->setID(pclass->NoOfCon);

				///////////////////////////////////////////////////////////////
				//		Post a Message to the IO Completion Queue
				////////////////`///////////////////////////////////////////////
				::PostQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
											  (DWORD) 0, 
											  (DWORD) pclass->ConnClient[pclass->NumOfClient],
 											  *pclass->iocpOverlap);					
			}
	

	}



}

//////////////////////////////////////////////////////////////////////
//	UDP Accept Thread 
//////////////////////////////////////////////////////////////////////
DWORD WINAPI CNetprobe1Dlg::ThreadUDPAcceptProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;
//	int i;
	int received;
	struct sockaddr_in _addr;
	int dlen = sizeof(struct sockaddr_in);
	int enter =0;
int recvCount = 0;
	int ClientID=-1;
//	DWORD lpNumberOfBytesSent;
	pclass->FC_buf = new char[100];

	pclass->iSendDelay = pclass->lPktSize*1000/pclass->lRate; 
	pclass->iSendDelay1 = pclass->lPktSize*1000/(pclass->lRate*1.2); 
	pclass->iSendDelay1w = pclass->lPktSize*1000/(750000*4.0); 
	pclass->iSendDelay2 = pclass->lPktSize*1000/(pclass->lRate*0.2); 
	pclass->iSendDelay2w = pclass->lPktSize*1000/(750000*0.01); 

	while(1)
	{
			SOCKET s = pclass->pUDP->GetHandle();

			char *buf = new char[100];
			memset(buf, 0, 100);			
			received = recvfrom(s,buf,100, 0, (struct sockaddr *)&_addr, &dlen);	
			
			if (received > 0)
			{
				ClientID = *(int *)buf;
				char *p = buf + 4;

				if (*(buf+4) == -1 || *(buf+4) == -2 || *(buf+4) == -20 || *(buf+4) == -10)
				{					
					ClientID = *(int *)buf;
					char *p = buf+4;
					
					pclass->ConnClient[ClientID]->setSRate(*(int *)p);

					delete [] buf;
				}
				else
				{					
					pclass->NumOfUDPClient++;

					pclass->ConnClient[pclass->NumOfUDPClient] = new ES_Connection(_addr,pclass->NumOfUDPClient);
					pclass->ConnClient[pclass->NumOfUDPClient]->ChangeState(pclass->m_iocp->gethConn());			
					pclass->ConnClient[pclass->NumOfUDPClient]->setProtocol(0);

					pclass->ConnClient[pclass->NumOfUDPClient]->setrecvfile(buf);

					DWORD ThreadID;
					pclass->bIsRun = true;

	//				if (!Ustart)
					{
						pclass->h= CreateThread(0, 
					 						   0, 
											   CNetprobe1Dlg::UDPThreadProc,
											   (LPVOID)pclass, 
											   0, 
											   &ThreadID);

						if (pclass->h == NULL)		
							AfxMessageBox("Error starting new thread!");
					}		
					
					char *pData = new char[100];
					memset(pData, 0, 100); // Clear data to all zero.

					*((unsigned long *) pData) = pclass->NumOfUDPClient; // Fill in the seq no.
					char* p = pData;
					
					//		send ID to the Client
					int datasend = sendto(s, pData, 100, 0, (struct sockaddr *)&_addr, sizeof(struct sockaddr_in));

					
					delete [] pData;
					delete [] buf;


					////////////////////////////////////////////////////////////
					//		Progress Bar
					////////////////////////////////////////////////////////////

					pclass->NoOfCon++;
					pclass->ConnClient[pclass->NumOfUDPClient]->setID(pclass->NoOfCon);
				}
			}
	}

}

///////////////////////////////////////////////////////////////
//	UDP : Flow Control Thread
///////////////////////////////////////////////////////////////
DWORD WINAPI CNetprobe1Dlg::FC_ThreadProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;

	return 0;
}


	
///////////////////////////////////////////////////////////////
//	TCP Thread Pool Procedure
///////////////////////////////////////////////////////////////
DWORD WINAPI CNetprobe1Dlg::TCPThreadPoolProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;

	DWORD lNumBytesTransferred;
	ES_Connection *lCompKey;
	DWORD dwMilliseconds = INFINITE;
	unsigned int ulSeqNum=0;
	DWORD flags = 0, dwFlags=0;
	DWORD lpNumberOfBytesSent;	
	LPOVERLAPPED *lpOverlapped = new LPOVERLAPPED;	
	int header=1;
	int i=0;
	int nBytesRead=0;
	Bfile = 1;
	
	ES_FlashTimer Timer;

	while(pclass->bIsRun)
	{
		::GetQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
									&lNumBytesTransferred, 
									(LPDWORD)&lCompKey, 
									lpOverlapped, 			 						
									dwMilliseconds);

		Timer.Start();

//		pclass->iSendDelay = pclass->lPktSize*1000/pclass->lRate; // iSendDelay is in milliseconds.
		
//		while (pclass->iSendDelay >= Timer.Elapsed())
//		{}

		lCompKey = (ES_Connection *)lCompKey;
	
		////////////////////////////////////////////////////////////
		//	Get File Name (both TCP and UDP)
		////////////////////////////////////////////////////////////
		// one connection is accepted, then do IO request
		if (lCompKey->getState() == pclass->m_iocp->gethFile())
		{
			char *buffer = new char[pclass->lPktSize];
			int datalen = pclass->lPktSize;
			int addrlen = sizeof(struct sockaddr_in);	
			char *buf = new char[100];
			int FS;

			memset(buf,0,100);

			int re = recv(lCompKey->getClientSocket(), buf, 100, 0);

			lCompKey->setrecvfile(buf);

			char ip[100];

			sprintf(ip, "TCP - IP: %s  %s", 
						lCompKey->getClientIP(), default_path+lCompKey->getfile());

			if (lCompKey->getID()==1)
			{
				pclass->m_Conn1.SetWindowText(ip);
			}
			else if (lCompKey->getID()==2)
			{
				pclass->m_Conn2.SetWindowText(ip);
			}
			else if (lCompKey->getID()==3)
			{
				pclass->m_Conn3.SetWindowText(ip);
			}
			else if (lCompKey->getID()==4)
			{
				pclass->m_Conn4.SetWindowText(ip);
			}
			else if (lCompKey->getID()==5)
			{
				pclass->m_Conn5.SetWindowText(ip);
			}


			delete [] buf;

			CString RequestFile = default_path+lCompKey->getfile();
			CFile checkfp; 
			CFileException e;
			
			if (checkfp.Open( (LPCTSTR)RequestFile , CFile::modeRead |CFile::shareDenyNone, &e ))
			{
					char *pData = new char[100];
					memset(pData, 0, 100); // Clear data to all zero.

					*((unsigned long *) pData) = -300; // Fill in the signal no. for the "File Exists"
					char* p = pData;
					  
					int datasend = send(lCompKey->getClientSocket(), pData, 100, 0);
					
					delete [] pData;

					lCompKey->Initfp(RequestFile);		
					lCompKey->ChangeState(pclass->m_iocp->gethTCP());
				
			}
			else
			{
					char *pData = new char[100];
					memset(pData, 0, 100); // Clear data to all zero.

					*((unsigned long *) pData) = -200; // Fill in the signal no. for the "File not found"
					char* p = pData;
					  
					int datasend = send(lCompKey->getClientSocket(), pData, 100, 0);
					
					delete [] pData;


					lCompKey->ChangeState(pclass->m_iocp->gethWait());		
				
			}
	
			::PostQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
										(DWORD) 0, 
										(DWORD) lCompKey,
										 NULL);	
		}

		////////////////////////////////////////////////////////////
		//	Send File to the Client (TCP)
		////////////////////////////////////////////////////////////
		else if (lCompKey->getState() == pclass->m_iocp->gethTCP()) 
		{

			char *pData = new char[pclass->lPktSize];
			memset(pData, 0, pclass->lPktSize); // Clear data to all zero.

//			*((unsigned long *) pData) = lCompKey->getSeqNo(); // Fill in the seq no.
			char* p = pData;
//			p += sizeof(unsigned long);
//			*((int *) p) = pclass->lPktSize; 

			if (Bfile)
			{
//				nBytesRead = lCompKey->getfp()->Read( p+sizeof(int), pclass->lPktSize-sizeof(unsigned long)-sizeof(int));
				nBytesRead = lCompKey->getfp()->Read( p, pclass->lPktSize);
		
				if (nBytesRead<=0)
				{					
					lCompKey->getfp()->Close();	
					lCompKey->ChangeState(pclass->m_iocp->gethFin());						
				}		
			}
			if (nBytesRead>0)
			{
				lCompKey->UpdateBsent(nBytesRead);	
				
				if (lCompKey->getID()==1)
				{
					pclass->m_Pg1.SetPos(lCompKey->bytessent()*100/lCompKey->getfp()->GetLength());
				}
				else if (lCompKey->getID()==2)
				{
					pclass->m_Pg2.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
				}
				else if (lCompKey->getID()==3)
				{
					pclass->m_Pg3.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
				}
				else if (lCompKey->getID()==4)
				{
					pclass->m_Pg4.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
				}
				else if (lCompKey->getID()==5)
				{
					pclass->m_Pg5.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
				}
			}
			
//			ulSeqNum = lCompKey->getSeqNo()+1;
//			lCompKey->UpdateSeqNo(ulSeqNum);			

			memcpy(pclass->iocpBuf[pclass->aBufIndex]->buf, pData, pclass->lPktSize);			
		
			if (nBytesRead>0)
			{
				int result = WSASend (lCompKey->getClientSocket(),
									  pclass->iocpBuf[pclass->aBufIndex],                                     
									  1,                                    
									  &lpNumberOfBytesSent,                            
									  0,                                          
									  pclass->iocpOverlap[pclass->aBufIndex],                           
									  NULL);
			  

				if ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING))
				{
					int b = WSAGetLastError();

					lCompKey->ChangeState(pclass->m_iocp->gethWait());		
			
					::PostQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
												(DWORD) 0, 
												(DWORD) lCompKey,
												 NULL);	
					char temp[100];
					sprintf(temp,"Connection closed from %s", lCompKey->getClientIP());

					if (lCompKey->getID()==1)
					{						
						pclass->m_Conn1.SetWindowText(temp);
						pclass->m_Pg1.SetPos(0);
					}
					else if (lCompKey->getID()==2)
					{
						pclass->m_Conn2.SetWindowText(temp);
						pclass->m_Pg2.SetPos(0);
					}
					else if (lCompKey->getID()==3)
					{
						pclass->m_Conn3.SetWindowText(temp);
						pclass->m_Pg3.SetPos(0);
					}
					else if (lCompKey->getID()==4)
					{
						pclass->m_Conn4.SetWindowText(temp);
						pclass->m_Pg4.SetPos(0);
					}
					else if (lCompKey->getID()==5)
					{
						pclass->m_Conn5.SetWindowText(temp);
						pclass->m_Pg5.SetPos(0);
					}
				}

				pclass->aBufIndex++;
				pclass->aBufIndex %= pclass->lBufferNum;

				

			}
			else
					::PostQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
										(DWORD) 0, 
										(DWORD) lCompKey,
										 NULL);	

			delete [] pData;			
		}

	
		////////////////////////////////////////////////////////////////////////////
		//	In TCP, after "Accept", "hConn" Event 
		////////////////////////////////////////////////////////////////////////////
		else if (lCompKey->getState() == pclass->m_iocp->gethConn() )
		{
			ulSeqNum =0;
			lCompKey->UpdateSeqNo(ulSeqNum);

			if (Bfile)
				lCompKey->ChangeState(pclass->m_iocp->gethFile());
			else if (lCompKey->getProtocol())
				lCompKey->ChangeState(pclass->m_iocp->gethTCP());

			::PostQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
										(DWORD) 0, 
										(DWORD) lCompKey,
										 NULL);	
	
		}

		////////////////////////////////////////////////////////////////////////////
		//		Finish Sending File
		////////////////////////////////////////////////////////////////////////////		
		else if ( lCompKey->getState() == pclass->m_iocp->gethFin() )
		{
//			int result;
/*
			char *pData = new char[pclass->lPktSize];
			memset(pData, 0, pclass->lPktSize); // Clear data to all zero.

			*((unsigned long *) pData) = -100; // Fill in the seq no. 
			char* p = pData;
			p += sizeof(unsigned long);
			*((int *) p) = pclass->lPktSize; 
			  
			memcpy(pclass->iocpBuf[pclass->aBufIndex]->buf, pData, pclass->lPktSize);			
						
			int d = sizeof(const struct sockaddr);	

			result = WSASend (lCompKey->getClientSocket(),
							  pclass->iocpBuf[pclass->aBufIndex],                                     
							  1,                                    
							  &lpNumberOfBytesSent,                            
							  0,                                          
							  pclass->iocpOverlap[pclass->aBufIndex],                           
							  NULL);
*/		  
			lCompKey->ChangeState(pclass->m_iocp->gethWait());		
			
			::PostQueuedCompletionStatus(pclass->m_iocp->getTCPHandle(),
										(DWORD) 0, 
										(DWORD) lCompKey,
										 NULL);	

/*
			if ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING))
			{
				int b = WSAGetLastError();
//				AfxMessageBox("Error in WSASendTo, terminated. File Closes!");
			}

			delete [] pData;
*/	
		}
	
		////////////////////////////////////////////////////////////////////////////
		//		Finish Sending File
		////////////////////////////////////////////////////////////////////////////		
		else if ( lCompKey->getState() == pclass->m_iocp->gethWait() )
		{	
		//	delete file;
			lCompKey->CloseConnection();
			lCompKey->~ES_Connection();
			
		}	

	}

	delete lpOverlapped;

	return 0;
}


///////////////////////////////////////////////////////////////
//	When the user click the "send" button, run this function
///////////////////////////////////////////////////////////////
void CNetprobe1Dlg::OnSend() //Concurrent
{	
	int i;
	// Concurrent Server -- IO Completion Port

	DWORD ThreadID1;
	DWORD ThreadID2;
	DWORD ThreadID3;
	UDPID = 1;
	header = 1;
	
	UpdateData(true);

//	iSendDelay0 = lPktSize*1000.0/12000000;
//	iSendDelay1 = lPktSize*1000/2400000;
//	iSendDelay2 = lPktSize*1000/3200000;

	InitVariable(1);

	NoOfCon = 0;
	Tstart = false;
	
	ConnClient = new ES_Connection*[100];
	m_ButtonQuit.EnableWindow(false);
	m_ButtonRecv.EnableWindow(true);
	CreateO = false;
	aBufIndex = 0;

	NumOfClient=0;
	NumOfUDPClient=0;	

	///////////////////////////////////////////////////////////////
	//	Create IO Completion Port
	///////////////////////////////////////////////////////////////
	m_iocp = new IOCP(numofTh);
	m_iocp->StartUp(m_iocp->getNumOfThread()/2);	// call CreateIOCP


	///////////////////////////////////////////////////////////////
	//	Create Thread Pool TCP
	///////////////////////////////////////////////////////////////
	for ( i=0; i< numofTh; i++)
	{		
		IOCP_hThreadTCP = CreateThread(0, 
					 				   0, 
									   CNetprobe1Dlg::TCPThreadPoolProc,
									   (LPVOID)this, 
									   0, 
									   &ThreadID1);

		if (IOCP_hThreadTCP == NULL)		
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	}

	pUDP = new ES_UDP();
	pTCPServer = new ES_TCPServer();

	if (!pTCPServer->IOCPStartUp(usPort, hostname))
	{
		AfxMessageBox("Error in Starting up the IOCP in TCP");
		return;
	}
	if (!pUDP->IOCPStartUp(usPort, hostname))
	{
		AfxMessageBox("Error in Starting up the IOCP in UDP");
		return;
	}

	///////////////////////////////////////////////////////////////
	//	Create Accept Thread
	///////////////////////////////////////////////////////////////

	Accept_hThread =  CreateThread(0, 
					 			   0, 
								   CNetprobe1Dlg::ThreadAcceptProc,
								   (LPVOID)this, 
								   0, 
								   &ThreadID2);

	if (Accept_hThread == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);

	///////////////////////////////////////////////////////////////
	//	Create UDP Accept Thread
	///////////////////////////////////////////////////////////////

	UDPAccept_hThread =  CreateThread(0, 
					 			   0, 
								   CNetprobe1Dlg::ThreadUDPAcceptProc,
								   (LPVOID)this, 
								   0, 
								   &ThreadID3);

	if (UDPAccept_hThread == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);

		
	m_ButtonSend.EnableWindow(false);
	m_MC.EnableWindow(false);
}


/////////////////////////////////////////////////////////////////
//	Receiver Stat Thread: for updating receiver statistic
///////////////////////////////////////////////////////////////
int CNetprobe1Dlg::RecvStatThread()
{
	DWORD ThreadID;
	hThread2 = CreateThread(0, 
					 	    0, 
						    CNetprobe1Dlg::RecvStatProc,
						    (LPVOID)this, 
						    0, 
						    &ThreadID);

	if (hThread2 == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

////////////////////////////////////////////////////////
//		STOP Receiver Stat Thread
////////////////////////////////////////////////////////
int CNetprobe1Dlg::StopThreadRecvStat()
{
	DWORD dwExitCode=0;

	TerminateThread(hThread2,dwExitCode);
	CloseHandle(hThread2);

	m_ButtonSend.EnableWindow(true);
//	m_ButtonHostInfo.EnableWindow(true);
	m_ButtonQuit.EnableWindow(true);

	return 0;
}

////////////////////////////////////////////////////////
//		Receiver Stat Thread Procedure
////////////////////////////////////////////////////////
DWORD WINAPI CNetprobe1Dlg::RecvStatProc(LPVOID IpInstance)
{
	CNetprobe1Dlg *recvstat = (CNetprobe1Dlg *) IpInstance;
	DWORD dwExitCode=0;

	if (recvstat->aStart)
	{
		char outrate[20];
		double lossrate = 0;
		char temp[20];				

		while (recvstat->OKrecv)
		{

			recvstat->ConvertRate(outrate, (double)(recvstat->apacketrecv)*recvstat->lPktSize*1000/recvstat->RecvTimer.Elapsed());
			
			if (recvstat->atotalpacket != 0)
				lossrate = (double) (100 - (100*recvstat->apacketrecv)/(recvstat->atotalpacket+1));
			if (lossrate <0)
				lossrate =0;

			sprintf(temp, "%d", recvstat->atotalpacket+1);
//			recvstat->m_PacketTransferred.SetWindowText(temp);
//			recvstat->m_DataRate.SetWindowText(outrate);

			sprintf(temp, "%.1f s", recvstat->RecvTimer.Elapsed()/1000);
//			recvstat->m_TimeElapsed.SetWindowText(temp);

			sprintf(temp, "%.0f %%", lossrate);
//			recvstat->m_Loss.SetWindowText(temp);

			unsigned long Loss = recvstat->atotalpacket+1-recvstat->apacketrecv;
			
			sprintf(temp, "%d", Loss);				
//			recvstat->m_PacketLoss.SetWindowText(temp);

			if (recvfile)
			{
				double size;		
				
				size = (recvstat->recvfilesize-(recvstat->atotalpacket+1)*8)/1048576;
				if (size > 1)
					sprintf(temp, "%.2f MBytes", size);	// mega Bytes
				else
				{
					size = (recvstat->recvfilesize-(recvstat->atotalpacket+1)*8)/1024;
					if (size > 1)
						sprintf(temp, "%.2f kBytes", size);	// kilo Bytes
					else
						sprintf(temp, "%.2f Bytes", (recvstat->recvfilesize-(recvstat->atotalpacket+1)*8));	// Bytes
				}
			
//				recvstat->m_RecvFileS.SetWindowText(temp);
			}

			recvstat->DisplayTimer.Start();
			Sleep(recvstat->iRefreshInterval);
		}
	
	}
	else
	{
		char outrate[20];
		char recvtemp[20];
		unsigned int packetno=0, losspacket=0;
		double lossrate;

		while (recvstat->OKrecv)
		{	
			packetno = ((strcmp(recvstat->iProtocol, "UDP") == 0) ? recvstat->pUDP->GetTotalPacket() : recvstat->pTCPServer->GetTotalPacket());
			lossrate = ((strcmp(recvstat->iProtocol, "UDP") == 0) ? recvstat->pUDP->GetLossRate() : recvstat->pTCPServer->GetLossRate());
			losspacket = ((strcmp(recvstat->iProtocol, "UDP") == 0) ? recvstat->pUDP->GetLossPacket() : recvstat->pTCPServer->GetLossPacket());
			
			if (losspacket <0)
			{
				losspacket =0;
				lossrate =0;
			}

			recvstat->ConvertRate(outrate, (double)(packetno-losspacket)*recvstat->lPktSize*1000/recvstat->RecvTimer.Elapsed());
					
			sprintf(recvtemp, "%d", packetno);
////			recvstat->m_PacketTransferred.SetWindowText(recvtemp);		
//			recvstat->m_DataRate.SetWindowText(outrate);

			sprintf(recvtemp, "%.1f s", recvstat->RecvTimer.Elapsed()/1000.0);
//			recvstat->m_TimeElapsed.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%.0f %%", lossrate);
//			recvstat->m_Loss.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%d", losspacket);
//			recvstat->m_PacketLoss.SetWindowText(recvtemp);

			if (recvfile)
			{
				double size;		
				
				if (strcmp(recvstat->iProtocol, "UDP") == 0)
					size = (recvstat->recvfilesize-(packetno)*8)/1048576;
				else
					size = (recvstat->recvfilesize-(packetno+1)*8)/1048576;

				if (size > 1)
					sprintf(recvtemp, "%.2f MBytes", size);	// mega Bytes
				else
				{
					if (strcmp(recvstat->iProtocol, "UDP") == 0)
						size = (recvstat->recvfilesize-(packetno)*8)/1024;
					else
						size = (recvstat->recvfilesize-(packetno+1)*8)/1024;

					if (size > 1)
						sprintf(recvtemp, "%.2f kBytes", size);	// kilo Bytes
					else
					{
						if (strcmp(recvstat->iProtocol, "UDP") == 0)
							sprintf(recvtemp, "%.2f Bytes", (recvstat->recvfilesize-(packetno)*8));	// Bytes
						else
							sprintf(recvtemp, "%.2f Bytes", (recvstat->recvfilesize-(packetno+1)*8));	// Bytes
					}
				}
			
//				recvstat->m_RecvFileS.SetWindowText(recvtemp);
			}
				
			recvstat->DisplayTimer.Start();
			Sleep(recvstat->iRefreshInterval);
		}
	}

	TerminateThread(recvstat->hThread2,dwExitCode);
	CloseHandle(recvstat->hThread2);

	return 0;
}

////////////////////////////////////////////////////////
//		Send packet and Update stat in Send Thread
////////////////////////////////////////////////////////
int CNetprobe1Dlg::RunSendMode(CNetprobe1Dlg* pclass)
{	
	double iSendDelay = 0;
	unsigned int nBytesRead=0; 

	if (pclass->lRate > 0)
		iSendDelay = pclass->lPktSize*1000/pclass->lRate; // iSendDelay is in milliseconds.

	ES_UDP *pUDP = 0;
	ES_TCPClient *pTCP = 0;
	
	if (strcmp(pclass->iProtocol, "UDP") == 0)
	{
		pUDP = new ES_UDP();

		if (!pUDP->Startup(0, pclass->usPort, pclass->hostname, recvfile, recvfname))
		{
			m_ButtonRecv.EnableWindow(true);
//			m_ButtonHostInfo.EnableWindow(true);
			m_ButtonQuit.EnableWindow(true);
			NotHold();
		
			return -1;
		}
	}
	else
	{
		pTCP = new ES_TCPClient;	

		if (pTCP->Connect(pclass->usPort, pclass->hostname) < 0)
		{			
			m_ButtonRecv.EnableWindow(true);
//			m_ButtonHostInfo.EnableWindow(true);
			m_ButtonQuit.EnableWindow(true);
			m_ButtonSend.SetWindowText("Send");
			NotHold();

			return -1;
		}
	} 
	m_ButtonRecv.EnableWindow(false);
//	m_ButtonHostInfo.EnableWindow(false);
	m_ButtonQuit.EnableWindow(false);
	Hold();

	send_complete = false;

	ES_FlashTimer SendTimer, DisplayTimer;

	unsigned long ulSeqNum = 0;
	char sendtemp[20];
	char outrate[20];

	if (Bfile)
	{
//		CString pFileName = m_filepath;
//		f = new CFile((LPCTSTR)pFileName, CFile::modeRead);
	}

	while ((!sendbreak)&&((pclass->lNumToSend > ulSeqNum) || (pclass->bSendForever)))
	{
		char *pData = new char[pclass->lPktSize];
		memset(pData, 0, pclass->lPktSize); // Clear data to all zero.

		*((unsigned long *) pData) = ulSeqNum; // Fill in the seq no.
		char* p = pData;
		p += sizeof(unsigned long);
		*((int *) p) = pclass->lPktSize; 

		if (Bfile)
		{
			nBytesRead = f->Read( p+sizeof(int), lPktSize-sizeof(unsigned long)-sizeof(int));
		
			if (nBytesRead<=0)
			{
//				m_ButtonHostInfo.EnableWindow(true);
				m_ButtonRecv.EnableWindow(true);
				m_ButtonQuit.EnableWindow(true);
				m_ButtonSend.EnableWindow(true);
				NotHold();
				f->Close();
			
				break;
			}
		}

		if (strcmp(pclass->iProtocol, "UDP") == 0)
		{
			if (Bfile)
			{
				if (pUDP->Send(pData, nBytesRead+sizeof(unsigned long)+sizeof(int)) < 0)
				{
					MessageBox("Send Mode: ES_TCP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);					
					m_ButtonSend.SetWindowText("Send");
					m_ButtonRecv.EnableWindow(true);
//					m_ButtonHostInfo.EnableWindow(true);
					m_ButtonQuit.EnableWindow(true);
					NotHold();
					f->Close();

					return -1;
				}
			}
			else if (pUDP->Send(pData, pclass->lPktSize) < 0)
			{
				MessageBox("Send Mode: ES_UDP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);				
				m_ButtonSend.SetWindowText("Send");
				m_ButtonRecv.EnableWindow(true);
//				m_ButtonHostInfo.EnableWindow(true);
				m_ButtonQuit.EnableWindow(true);

				NotHold();

				return -1;
			}
		}
		else
		{ // TCP
			if (Bfile)
			{
				if (pTCP->Send(pData, nBytesRead+sizeof(unsigned long)+sizeof(int)) < 0)
				{
					MessageBox("Send Mode: ES_TCP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);					
					m_ButtonRecv.EnableWindow(true);
//					m_ButtonHostInfo.EnableWindow(true);
					m_ButtonQuit.EnableWindow(true);
					m_ButtonSend.SetWindowText("Send");
					NotHold();

					f->Close();

					return -1;
				}
			}
			else if (pTCP->Send(pData, pclass->lPktSize) < 0)
			{
				MessageBox("Send Mode: ES_TCP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);					
				m_ButtonRecv.EnableWindow(true);
//				m_ButtonHostInfo.EnableWindow(true);
				m_ButtonQuit.EnableWindow(true);
				m_ButtonSend.SetWindowText("Send");
				NotHold();

				return -1;
			}
		}
		
		++ulSeqNum;

		if (DisplayTimer.Elapsed() >= pclass->iRefreshInterval)
		{		
			if ((ulSeqNum)*pclass->lPktSize*1000/SendTimer.Elapsed()<0)
				ConvertRate(outrate,0);
			else
				ConvertRate(outrate, (double)(ulSeqNum)*pclass->lPktSize*1000/SendTimer.Elapsed());

			sprintf(sendtemp, "%d", ulSeqNum);
////			pclass->m_PacketTransferred.SetWindowText(sendtemp);			
////			pclass->m_DataRate.SetWindowText(outrate);

			sprintf(sendtemp, "%.1f s", SendTimer.Elapsed()/1000.0);
//			pclass->m_TimeElapsed.SetWindowText(sendtemp);
			
			DisplayTimer.Start();
		}

		// if send time is longer than refresh interval, we need to refresh
		while ((!sendbreak) && (iSendDelay*ulSeqNum > (unsigned long)SendTimer.Elapsed()))
		{
			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				AfxGetApp()-> PumpMessage( );
			}
			LONG lIdle = 0;
			while (AfxGetApp()-> OnIdle(lIdle++)) 
			{					
				ConvertRate(outrate, (double)(ulSeqNum)*pclass->lPktSize*1000/SendTimer.Elapsed());

				sprintf(sendtemp, "%d", ulSeqNum);
//				pclass->m_PacketTransferred.SetWindowText(sendtemp);			
//				pclass->m_DataRate.SetWindowText(outrate);

				sprintf(sendtemp, "%.1f s", SendTimer.Elapsed()/1000.0);
//				pclass->m_TimeElapsed.SetWindowText(sendtemp);
				
				DisplayTimer.Start();
				Sleep(pclass->iRefreshInterval);
				
				if (send_complete)
					sendbreak =true;
			};
		}

	//	++ulSeqNum;
		delete [] pData;
	}

	if (strcmp(iProtocol, "TCP") == 0)
	{
		pTCP->Disconnect();
		delete pTCP;
	}
	else // UDP
		delete pUDP;

//	m_ButtonHostInfo.EnableWindow(true);
	m_ButtonRecv.EnableWindow(true);
	m_ButtonQuit.EnableWindow(true);
	m_ButtonSend.SetWindowText("Send");
	NotHold();	
	
	Bfile = false;		

	return 0;	
}

////////////////////////////////////////////////////////
//		 
////////////////////////////////////////////////////////
void CNetprobe1Dlg::OnReceive() 
{	
//	int i;
	bIsRun = false;
	Ustart = false;
	FC_Run = false;
	

	delete m_iocp;
//	delete [] FC_buf;

//	delete [] iocpOverlap;
//	delete []  iocpBuf->buf;
//	delete [] iocpBuf;

//	for (i=0; i < m_iocp->getNumOfThread(); i++)	
//	TerminateThread(IOCP_hThreadUDP,0);
//	TerminateThread(FC_Thread,0);
	TerminateThread(h,0);	
	TerminateThread(IOCP_hThreadTCP,0);
	TerminateThread(Accept_hThread,0);
	TerminateThread(UDPAccept_hThread,0);


	if (CreateO)
	{	
		for (int i = 0; i < lBufferNum; i++)
		{
				free(iocpBuf[i]);
				free(iocpOverlap[i]);
		}
		 
		free(iocpBuf);
		free(iocpOverlap);
		CreateO = false;

		for (i = 1; i <= NumOfUDPClient; i++)
		{
			free(ConnClient[i]);			
		}

		if (Tstart)
			free(ConnClient[0]);			

		free(ConnClient);
	}

	NoOfCon = 0;

	char temp[20];

	sprintf(temp, "Connection 1");
	m_Conn1.SetWindowText(temp);
	sprintf(temp, "Connection 2");
	m_Conn2.SetWindowText(temp);
	sprintf(temp, "Connection 3");
	m_Conn3.SetWindowText(temp);
	sprintf(temp, "Connection 4");
	m_Conn4.SetWindowText(temp);
	sprintf(temp, "Connection 5");
	m_Conn5.SetWindowText(temp);

	m_Pg1.SetPos(0);
	m_Pg2.SetPos(0);
	m_Pg3.SetPos(0);
	m_Pg4.SetPos(0);
	m_Pg5.SetPos(0);

//	if (strcmp(iProtocol, "TCP") == 0)
	{
		delete pTCPServer;
	}
//	else // UDP
	{
		delete pUDP;
	}

//	delete m_iocp;
//	IOCP_hThread;
	
	m_ButtonSend.EnableWindow(true);
	m_ButtonRecv.EnableWindow(false);
	m_ButtonQuit.EnableWindow(true);
	m_MC.EnableWindow(true);

}

/////////////////////////////////////////////////////////////
//	Recevie procedure
/////////////////////////////////////////////////////////////
int CNetprobe1Dlg::RunRecvMode()
{
	char outrate[20];
	char recvtemp[20];
	int packetno, losspacket;
	double lossrate;

	if (strcmp(iProtocol, "UDP") == 0)
	{
		pUDP = new ES_UDP();
		recvfilesize = 0;
		if (!pUDP->Startup(1, usPort, hostname,recvfile,recvfname))
		{			
			recvfile = false;	
			m_ButtonSend.EnableWindow(true);
			return -1;
		}
	}
	else
	{
		pTCPServer = new ES_TCPServer();
		if (!pTCPServer->Startup(usPort, hostname,recvfile,recvfname))
		{
			recvfile = false;	
			m_ButtonSend.EnableWindow(true);			
			return -1;
		}
	}

	m_ButtonSend.EnableWindow(false);
//	m_ButtonHostInfo.EnableWindow(false);
	m_ButtonQuit.EnableWindow(false);
	Hold();
/*
	m_DataRate.SetWindowText("0 Bps");
	m_Loss.SetWindowText("0 %");
	m_PacketTransferred.SetWindowText("0");		
	m_PacketLoss.SetWindowText("0");	
	m_TimeElapsed.SetWindowText("0 s");
	m_SendFileS.SetWindowText("0 Bytes");
	m_RecvFileS.SetWindowText("0 Bytes");
*/
	
//	RecvTimer.Start();
	DisplayTimer.Start();

	bIsRunning = true;	// indicate that the receiving process is running
	
	m_ButtonRecv.SetWindowText("Stop Recv");	

	if (SimpleBlockingIO)	// Blocking
	{
		if (!OnAccept())
		{
			return -1;
		}

		while (bIsRunning && OnRead() > 0)
		{
			packetno = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetTotalPacket() : pTCPServer->GetTotalPacket());
			lossrate = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetLossRate() : pTCPServer->GetLossRate());
			losspacket = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetLossPacket() : pTCPServer->GetLossPacket());
			
			if (losspacket <0)
			{
				losspacket =0;
				lossrate =0;
				recvfilesize = 0;
			}	

			if (packetno <0)
				recvfilesize = 0;

			ConvertRate(outrate, (double)(packetno-losspacket)*lPktSize*1000/RecvTimer.Elapsed());
					
			sprintf(recvtemp, "%d", packetno);
//			m_PacketTransferred.SetWindowText(recvtemp);		
//			m_DataRate.SetWindowText(outrate);

			sprintf(recvtemp, "%.1f s", RecvTimer.Elapsed()/1000.0);
//			m_TimeElapsed.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%.0f %%", lossrate);
//			m_Loss.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%d", losspacket);
//			m_PacketLoss.SetWindowText(recvtemp);

			if (recvfile)
			{
				double size;		
				
				size = (recvfilesize-(packetno+1)*8)/1048576;
			
				if (size > 1)
					sprintf(recvtemp, "%.2f MBytes", size);	// mega Bytes
				else
				{
					size = (recvfilesize-(packetno+1)*8)/1024;
					if (size > 1)
						sprintf(recvtemp, "%.2f kBytes", size);	// kilo Bytes
					else
						sprintf(recvtemp, "%.2f Bytes", (recvfilesize-(packetno+1)*8));	// Bytes
				}
			
//				m_RecvFileS.SetWindowText(recvtemp);
			}

			DisplayTimer.Start();
			Sleep(iRefreshInterval);

			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				AfxGetApp()-> PumpMessage( );
			}
			LONG lIdle = 0;
			while ( AfxGetApp()-> OnIdle( lIdle++ ) ) ;
		}		

		OnClose();
	}	
	else	// Async
	{
		// select the receiver socket handle
		socket = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetHandle() : pTCPServer->GetHandle());
		
		// when message comes, dipatch to Window Procedure, identify the message
		int err = WSAAsyncSelect(socket, 
								 m_hWnd, 
								 UDM_WINSOCK_SELECT, 
								 FD_READ | FD_ACCEPT | FD_CLOSE);
		
		if (err != 0)
		{
			recvfile = false;	
			MessageBox("WSAAsyncSelect() failed!", "Netprobe Error", MB_OK);
			OnClose();
		}
		else		
		{
			QuitFlag = false;
			OKrecv = true;	// ready for receiving
		}
	}
	
//	AfxMessageBox(" UDP Finish");

	return 0;
}

// Open the Host Information detail
void CNetprobe1Dlg::OnHostInfo() 
{
	// TODO: Add your control notification handler code here
	//host mydia(IDD_HostInfo);

	Chost mydia(this);
	mydia.DoModal();	
}

void CNetprobe1Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



LRESULT CNetprobe1Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if ((message == UDM_WINSOCK_SELECT) && (bIsRunning) && (!QuitFlag))
	{
		int event = WSAGETSELECTEVENT(lParam);
		int wsaerr = WSAGETSELECTERROR(lParam);
		switch (event) {
			case FD_READ : OnRead(); /* AfxMessageBox(" check point 4"); */ return 0;
			case FD_ACCEPT : OnAccept(); /*AfxMessageBox("Connection Accepted"); */ return 0;
			case FD_CLOSE : //StartDelayThread(); return 0;
				OnClose(); AfxMessageBox("Close Connection by Sender"); return 0;
		}
	}	
	 
	return CDialog::WindowProc(message, wParam, lParam);
}

// convert the data transmission rate
void CNetprobe1Dlg::ConvertRate(char *outrate, double inrate)
{
	double rate = inrate;
 
	rate = inrate/1000000;
	if (rate > 10)
		sprintf(outrate, "%.2f MBps", rate);	// mega Bytes
	else
	{
		rate = inrate/1000;
		if (rate > 10)
			sprintf(outrate, "%.2f kBps", rate);	// kilo Bytes
		else
			sprintf(outrate, "%.2f Bps", inrate);	// Bytes
	}
}

//////////////////////////////////////////////////
//		Variable Initializaton
//////////////////////////////////////////////////
bool CNetprobe1Dlg::InitVariable(int sendrecv)
{
	int choice = sendrecv;

	bIsRun = true;
	iSendDelay =0 ;
	recvfilesize = 0;
	OKrecv = false;
	send_complete = false;
	sendbreak = false;
	stoprecv = false;
	RTStart = false;	
	Accept_hThread = NULL;

	iRefreshInterval = m_Refresh;
	if (iRefreshInterval <= 0)
	{
		char temp[10];
		sprintf(temp, "%d", iRefreshInterval);
		MessageBox("Refresh Interval [" + (CString) temp + "] must be positive and non-zero.", "Netprobe Error", MB_ICONEXCLAMATION);
		return false;
	}
	
	if (choice==0) {
	
		if (m_Peer.GetLength() >= ES_Network::MAX_NAME)
		{
			MessageBox("Error hostname too long, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
		else if (m_Peer.IsEmpty())
		{
			MessageBox("Hostname must be specified, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
	
		strcpy(hostname, m_Peer);
		usPort = m_PeerPort;
		}
		
	else
	{
		if (m_Local.GetLength() >= ES_Network::MAX_NAME)
		{
			MessageBox("Error hostname too long, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
		else if (m_Local.IsEmpty())
		{
			MessageBox("Hostname must be specified, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
		strcpy(hostname, m_Local);
		usPort = m_LocalPort;	
	}

	ES_Network network;
	if (network.ValidateHost(hostname) == 0)
	{
		MessageBox("Remote Hostname [" +(CString) hostname +"] is invalid.", "Netprobe Error", MB_ICONEXCLAMATION);
		return false;
	}	

	if (m_Protocol.GetCheck())
		strcpy(iProtocol, "TCP");
	else
		strcpy(iProtocol, "UDP");
	
	lPktSize = m_Size;

	if (lPktSize < 8)
	{
		char temp[10];
		sprintf(temp, "%d", lPktSize);
		MessageBox("Invalid Packet Size Specified [" +(CString) temp + "].\r\nPacket Size should be no less than 8.", "Netprobe Error", MB_ICONEXCLAMATION);
		return false;
	}
	lRate = m_Rate;
	lNumToSend = m_PacketNo;
	bSendForever = false;
	if (lNumToSend == 0)
		bSendForever = true;

	lBufferNum = m_BufferNo;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//	when FD_READ messages handles by the WindowProc, then call OnRead()
//////////////////////////////////////////////////////////////////////////////
int CNetprobe1Dlg::OnRead()
{
	int err;

	aStart = false;

	if (strcmp(iProtocol, "UDP") == 0)
	{
		err = pUDP->Recv(lPktSize,recvfile);
	
		if (err < 0)
		{
	
			OnClose();

			if (WSAGetLastError() == 10040)
				AfxMessageBox("Receive Mode: Message sent is too long for receive.");
			else if (err == -2)
				MessageBox("Receive Mode: Packet size mismatch, terminated.", "NetProbe Error", MB_ICONEXCLAMATION);
			else if (err != -2) 
				AfxMessageBox("Receive Mode: ES_UDP::Recv() failed.");
						
			NotHold();

			return -err;
		}

		if (pUDP->GetPacketRecv() == 1)
		{			
			// prevent calling 2 RecvStatThread simultaneoudly
			if (!RTStart)
			{	
				RecvTimer.Start();			
				RecvStatThread();
			}
	
			
			RTStart = true;		
			pUDP->setSeqNo();
			pUDP->setPacketNo();
			
		}
	}
	else	// for TCP
	{
		err = pTCPServer->Recv(lPktSize,recvfile);

		if (err <= 0)
		{
	
			OnClose();
			if (err == -2)
				MessageBox("Receive Mode: Packet size mismatch, terminated.", "NetProbe Error", MB_ICONEXCLAMATION);
			
			NotHold();

			return err;
		}

		if (pTCPServer->GetPacketRecv() == 1)
		{
			if (!RTStart)
			{
				RecvTimer.Start();			
				RecvStatThread(); 				
			}

			RTStart = true;
		}		
	}

	if (err >=0)
		recvfilesize += err;

	return 1;
}


bool CNetprobe1Dlg::OnAccept()
{
	if (strcmp(iProtocol, "TCP") == 0)
		return pTCPServer->Accept();
	return true;
}

int CNetprobe1Dlg::OnClose()
{
	if (bIsRunning)
	{
		int er = 0;
	
		do
		{			
			if (strcmp(iProtocol, "UDP") == 0)
				er = pUDP->Recv(lPktSize,recvfile);
			else
				er = pTCPServer->Recv(lPktSize,recvfile);
		} while (er != -1);

		bIsRunning = false;

		if (!SimpleBlockingIO)	// Async
		{
			WSAAsyncSelect(socket, m_hWnd, 0, 0);
			
			StopThreadRecvStat();
	
			OKrecv = false;
			QuitFlag = true;
		}
		else	// blocking or Alertable
			QuitFlag = true;
	
		if (strcmp(iProtocol, "TCP") == 0)
		{
			if (recvfile)
			{
				recvfile = false;
				pTCPServer->CloseFile();
			}

			delete pTCPServer;
		}
		else 
		{
			if (recvfile)
			{
				recvfile = false;
				pUDP->CloseFile();
			}

			delete pUDP;
		}

		m_ButtonRecv.EnableWindow(true);
		m_ButtonSend.EnableWindow(true);
//		m_ButtonHostInfo.EnableWindow(true);
		m_ButtonQuit.EnableWindow(true);
		m_ButtonRecv.SetWindowText("Receive");
		m_ButtonSend.SetWindowText("Send");
		NotHold();

		recvfile = false;
	
		return 1;
	}


	return 0;
}

void CNetprobe1Dlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CNetprobe1Dlg::Hold()
{
	m_BufferNoC.EnableWindow(false);
	m_LocalC.EnableWindow(false);
	m_LocalPortC.EnableWindow(false);
	m_PeerC.EnableWindow(false);
	m_PeerPortC.EnableWindow(false);
	m_RateC.EnableWindow(false);
	m_SizeC.EnableWindow(false);
	m_RefreshC.EnableWindow(false);
	m_PacketNoC.EnableWindow(false);
	m_Protocol.EnableWindow(false);
	m_IOMode.EnableWindow(false);
	m_Async.EnableWindow(false);
	m_UDP.EnableWindow(false);
	m_SendBlock.EnableWindow(false);
	m_RecvBlock.EnableWindow(false);
	m_Alert.EnableWindow(false);
//	m_SendFileC.EnableWindow(false);
//	m_RecvFileC.EnableWindow(false);
//	m_FileC.EnableWindow(false);
//	m_FileTextC.EnableWindow(false);
}

void CNetprobe1Dlg::NotHold()
{
	m_BufferNoC.EnableWindow(true);
	m_LocalC.EnableWindow(true);
	m_LocalPortC.EnableWindow(true);
	m_PeerC.EnableWindow(true);
	m_PeerPortC.EnableWindow(true);
	m_RateC.EnableWindow(true);
	m_SizeC.EnableWindow(true);
	m_RefreshC.EnableWindow(true);
	m_PacketNoC.EnableWindow(true);
	m_Protocol.EnableWindow(true);
	m_IOMode.EnableWindow(true);
	m_Async.EnableWindow(true);
	m_UDP.EnableWindow(true);
	m_SendBlock.EnableWindow(true);
	m_RecvBlock.EnableWindow(true);
	m_Alert.EnableWindow(true);
//	m_SendFileC.EnableWindow(true);
//	m_RecvFileC.EnableWindow(true);
//	m_FileC.EnableWindow(true);
//	m_FileTextC.EnableWindow(true);
//	m_FileC.SetCheck(0);
//	SetDlgItemText(IDC_filepath,"");
	
	check = true;
	Bfile = false;
	recvfile = false;
}

int CNetprobe1Dlg::OnSendFile() 
{
	Bfile = true;

	// TODO: Add your control notification handler code here
	LPCTSTR  lpszFilter ="All Files(*.*)|*.*|C++ Files(*.cpp)|*.cpp|";
	CString str;

	CFileDialog SFile(true, NULL, NULL, NULL, lpszFilter, NULL);
	
	SFile.DoModal();

//	m_filepath = SFile.GetPathName();
	UpdateData(false);

/*	if (m_filepath == "")
	{
		m_PacketNoC.EnableWindow(true);		
		file = false;
	}
*/
	if (Bfile)
	{
		double size;
		char sendtemp[20];

//		m_RecvFileC.EnableWindow(false);
		m_Alert.EnableWindow(false);
		m_RecvBlock.EnableWindow(false);
		m_Async.EnableWindow(false);
		m_ButtonRecv.EnableWindow(false);
		m_PacketNoC.EnableWindow(false);
		m_PacketNo = 0;
//		m_recvfpath = "";
		UpdateData(false);		
		sprintf(sendtemp, "0 Bytes", 0);	// mega Bytes
//		m_RecvFileS.SetWindowText(sendtemp);			

//		CString pFileName = m_filepath;
//		f = new CFile((LPCTSTR)pFileName, CFile::modeRead);

		size = (double)f->GetLength()/1048576;
		if (size > 1)
			sprintf(sendtemp, "%.2f MBytes", size);	// mega Bytes
		else
		{
			size = (double)f->GetLength()/1024;
			if (size > 1)
				sprintf(sendtemp, "%.2f kBytes", size);	// kilo Bytes
			else
				sprintf(sendtemp, "%.2f Bytes", (double)f->GetLength());	// Bytes
		}
	
//		m_SendFileS.SetWindowText(sendtemp);			

		f->Close();
		delete f;
	}
	else
	{
//		m_RecvFileC.EnableWindow(true);
		m_Alert.EnableWindow(true);
		m_RecvBlock.EnableWindow(true);
		m_Async.EnableWindow(true);
		m_ButtonRecv.EnableWindow(true);
		m_PacketNoC.EnableWindow(true);	
//		m_SendFileS.SetWindowText("0 Bytes");			
	}

	return 0;

}

void CNetprobe1Dlg::OnRecvFile() 
{
	// TODO: Add your control notification handler code here
	
	LPCTSTR  lpszFilter ="All Files(*.*)|*.*|";

	CFileDialog RFile(false, NULL, NULL, NULL, lpszFilter, NULL);
	
	RFile.DoModal();

//	m_recvfpath = RFile.GetPathName();
	UpdateData(false);

	recvfile = true;

//	if (m_recvfpath == "")
		recvfile = false;
	
	if (recvfile)
	{
		char temp[20];

//		m_SendFileC.EnableWindow(false);
		m_SendBlock.EnableWindow(false);
		m_IOMode.EnableWindow(false);
		m_ButtonSend.EnableWindow(false);
//		m_RecvFileS.SetWindowText("0 Bytes");
//		m_filepath = "";
		UpdateData(false);
		sprintf(temp, "0 Bytes", 0);	// mega Bytes
//		m_SendFileS.SetWindowText(temp);			

	}
	else
	{
//		m_SendFileC.EnableWindow(true);
		m_SendBlock.EnableWindow(true);
		m_IOMode.EnableWindow(true);
		m_ButtonSend.EnableWindow(true);
//		m_recvfpath="";
		UpdateData(false);
//		m_RecvFileS.SetWindowText("0 Bytes");			
	}

//	recvfname = m_recvfpath; 
	
}

int CNetprobe1Dlg::StartAlertableRecv()
{
	DWORD ThreadID;
	hThread3 = CreateThread(0, 
							0, 
							CNetprobe1Dlg::AlertableRecvProc,
							(LPVOID)this, 
							0, 
							&ThreadID);

	if (hThread3 == NULL)
		MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

int CNetprobe1Dlg::StopAlertableRecv()
{	
	if (bIsRunning)
	{
		aStart = false;
		bIsRunning = false;
		if (closesocket(hSocket) == SOCKET_ERROR)
		{
			AfxMessageBox("Error in closing the socket, terminated.");
			return -1;
		}
		if (strcmp(iProtocol, "TCP") == 0 && closesocket(cSocket) == SOCKET_ERROR)
		{
			AfxMessageBox("Error in closing the socket, terminated.");
			return -1;
		}
		m_ButtonRecv.SetWindowText("Receive");
		CloseHandle(hThread3);

		if (recvfile)
		{
			f1->Close();
			delete f1;
			recvfile = false;
		}

	}

	return 0;
}

DWORD WINAPI CNetprobe1Dlg::AlertableRecvProc(LPVOID IpInstance)
{
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;
	pclass->m_ButtonRecv.SetWindowText("Stop Receive");
	pclass->AlertableRecv(pclass);
	CloseHandle(pclass->hThread3);
	return 0;	
}


void CALLBACK CNetprobe1Dlg::IOCPRoutine(DWORD dwError, 
										 DWORD cbTransferred, 
										 LPWSAOVERLAPPED lpOverlapped, 
										 DWORD dwFlags)
{
	AfxMessageBox("return from IO Completion Port Routine");
}

void CALLBACK CNetprobe1Dlg::SocketIOCompletion(DWORD dwError, 
												DWORD cbTransferred, 
												LPWSAOVERLAPPED lpOverlapped, 
												DWORD dwFlags)
{
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *)lpOverlapped->hEvent;
	DWORD bytes_received, flags = 0;
	bool toread;
	
	if (!pclass->bIsRunning)
		return;

	if (!pclass->aStart)
	{
		pclass->aStart = true;		
		pclass->OKrecv =true;	
		pclass->RecvTimer.Start();
		pclass->RecvStatThread();		
	}

	if (pclass->aReceived < 8)
		toread = true;
	else
		toread = false;

	if (strcmp(pclass->iProtocol, "TCP") == 0 && cbTransferred == 0)
	{
		pclass->StopAlertableRecv();
		return;
	}

	if (dwError == 0)
	{
		if ((pclass->lPktSize-pclass->aReceived) >= (int) cbTransferred)
		{
			char *ptr;
			memcpy(pclass->pData+pclass->aReceived, pclass->pWsaBuf[pclass->aBufIndex]->buf, cbTransferred);
			pclass->pDatalen += cbTransferred;

			ptr = pclass->pData+pclass->aReceived;

			if (recvfile)
			{
				if (pclass->aReceived <= 8)
					pclass->f1->Write( ptr+sizeof(unsigned long)+sizeof(int)-pclass->aReceived,cbTransferred-sizeof(unsigned long)-sizeof(int)+pclass->aReceived);			
				else
					pclass->f1->Write( pclass->pWsaBuf[pclass->aBufIndex]->buf,cbTransferred);			
			}
		}
		else
		{
			char *datapt = pclass->pWsaBuf[pclass->aBufIndex]->buf;

			if (recvfile)
			{
				if (pclass->aReceived <=8)
					pclass->f1->Write( datapt+sizeof(unsigned long)+sizeof(int)-pclass->aReceived,cbTransferred-sizeof(unsigned long)-sizeof(int)+pclass->aReceived);			
				else
				{
					pclass->f1->Write( datapt,pclass->lPktSize - pclass->aReceived);	

					if ((cbTransferred+pclass->aReceived-pclass->lPktSize) >8)
						pclass->f1->Write( datapt+pclass->lPktSize-pclass->aReceived+sizeof(unsigned long)+sizeof(int), cbTransferred-pclass->lPktSize+pclass->aReceived-sizeof(unsigned long)-sizeof(int));	
				}
			}
			datapt += (pclass->lPktSize - pclass->aReceived);
			memcpy(pclass->pData, datapt, (cbTransferred - pclass->lPktSize + pclass->aReceived));
			pclass->pDatalen = cbTransferred - pclass->lPktSize + pclass->aReceived;
			toread = true;
		}

		if (toread && pclass->pDatalen >= 8)
		{
			char *temp = pclass->pData;
			temp += sizeof(unsigned long);
			
			if (pclass->lPktSize != *((unsigned long *)temp))
			{	
				pclass->StopAlertableRecv();
				AfxMessageBox("Packet size mismatch, terminated.");
				return;
			}

			if (strcmp(pclass->iProtocol, "UDP") == 0)
			{
				if (*((unsigned long *)pclass->pData) < pclass->atotalpacket)
				{
					pclass->RecvTimer.Start();
					pclass->apacketrecv = 0;
				}
			}
			pclass->atotalpacket = *((unsigned long *)pclass->pData);
			pclass->apacketrecv++;
		}

		pclass->aReceived += cbTransferred;
		pclass->aReceived %= pclass->lPktSize;	
		pclass->pDatalen %= pclass->lPktSize;
		pclass->recvfilesize += cbTransferred;
			
	}
	else
	{
		pclass->StopAlertableRecv();
		if (dwError == WSAEMSGSIZE)
			AfxMessageBox("Packet size mismatch, terminated.");
		return;
	}

	memset(pclass->pWsaBuf[pclass->aBufIndex]->buf, 0, pclass->pWsaBuf[pclass->aBufIndex]->len);

	int result = WSARecv(pclass->hSocket, 
					     pclass->pWsaBuf[pclass->aBufIndex], 
						 1, 
						 &bytes_received, 
						 &flags, 
						 lpOverlapped, 
						 SocketIOCompletion);

	if ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING))
	{
			pclass->StopAlertableRecv();
			pclass->NotHold();
			AfxMessageBox("Error in WSARecv, terminated.");
			return;
	}

	if (pclass->aBufIndex < (pclass->lBufferNum-1))
		pclass->aBufIndex++;
	else
		pclass->aBufIndex = 0;

}

int CNetprobe1Dlg::AlertableRecv(CNetprobe1Dlg* pclass)
{
	QuitFlag = false;
	bIsRunning = true;
	aStart = false;
	atotalpacket = 0;
	apacketrecv = 0;
	aBufIndex = 0;
	aReceived = 0;
	pData = new char[lPktSize];
	pDatalen = 0;

	if (strcmp(iProtocol, "TCP") == 0)
		cSocket = WSASocket(PF_INET, 
							SOCK_STREAM, 
							IPPROTO_TCP, 
							(LPWSAPROTOCOL_INFO) 0, 
							(GROUP) 0, 
							WSA_FLAG_OVERLAPPED);
	else
		hSocket = WSASocket(PF_INET, 
							SOCK_DGRAM, 
							IPPROTO_UDP, 
							(LPWSAPROTOCOL_INFO) 0, 
							(GROUP) 0, 
							WSA_FLAG_OVERLAPPED);

	if ((strcmp(iProtocol, "TCP") == 0 && cSocket == INVALID_SOCKET))
	{
		AfxMessageBox("Error in creating socket for TCP,terminated.");
		cSocket = NULL;
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(usPort);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	int err;
	if (strcmp(iProtocol, "TCP") == 0)
		err = bind(cSocket, (struct sockaddr *) &addr, sizeof(addr));
	else
		err = bind(hSocket, (struct sockaddr *) &addr, sizeof(addr));
	if (err == SOCKET_ERROR)
	{
		AfxMessageBox("Fail in binding, terminated");
		StopAlertableRecv();
		return 0;
	}

	if (bIsRunning && strcmp(iProtocol, "TCP") == 0)
	{
		if (listen(cSocket, 1) == SOCKET_ERROR)
		{
			AfxMessageBox("Error in listen for TCP,terminated.");
			StopAlertableRecv();
			return 0;
		}
	
		struct sockaddr_in newaddr;
		int newaddrlen = sizeof(struct sockaddr_in);

		hSocket = WSASocket(PF_INET, 
							SOCK_STREAM, 
							IPPROTO_TCP, 
							(LPWSAPROTOCOL_INFO) 0, 
							(GROUP) 0, 
							WSA_FLAG_OVERLAPPED);	

		hSocket = accept(cSocket, (struct sockaddr *) &newaddr, &newaddrlen);
		
		if (hSocket == INVALID_SOCKET && bIsRunning)
		{
			AfxMessageBox("Error in accept for TCP,terminated.");
			hSocket = NULL;
			return 0;
		} 
	}

	DWORD bytes_received, flags = 0;
	WSAOVERLAPPED **pOverlap = new WSAOVERLAPPED*[lBufferNum];
	pWsaBuf = new WSABUF*[lBufferNum];
	unsigned int i;

	if (bIsRunning)
	{
		for (i = 0; i < lBufferNum; i++)
		{
			pOverlap[i] = new WSAOVERLAPPED;
			pOverlap[i]->hEvent = this;	

			pWsaBuf[i] = new WSABUF;
			pWsaBuf[i]->buf = new char[lPktSize];
			pWsaBuf[i]->len = lPktSize;

			memset(pWsaBuf[i]->buf, 0, pWsaBuf[i]->len);

			int result = WSARecv(hSocket, 
								 pWsaBuf[i], 
								 1, 
								 &bytes_received, 
								 &flags, 
								 pOverlap[i], 
								 SocketIOCompletion);

			if (bIsRunning && ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING)))
			{
				StopAlertableRecv();
				NotHold();
				if (err == WSAEMSGSIZE)
					AfxMessageBox("Packet size mismatch, terminated.");
				else
					AfxMessageBox("Error in WSARecv, terminated.");
			}
		}
		
		while (bIsRunning)
		{
			if (aStart)
			{
				// test
			}
			
			long tosleep;
			while((tosleep = (long) (iRefreshInterval-DisplayTimer.Elapsed())) > 0)
			{
				if (SleepEx(tosleep, true) == 0)
					break;
			}
			DisplayTimer.Start();
		}

		for (i = 0; i < lBufferNum; i++)
		{
			free(pWsaBuf[i]);
			free(pOverlap[i]);
		}
	}
	 
	free(pWsaBuf);
	free(pOverlap);
	free(pData);

	OKrecv = false;

	m_ButtonSend.EnableWindow(true);
//	m_ButtonHostInfo.EnableWindow(true);
	m_ButtonQuit.EnableWindow(true);
		
	NotHold();

	return 0;
}


void CNetprobe1Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here

	if (check)
	{
//		m_SendFileC.EnableWindow(true);
//		m_RecvFileC.EnableWindow(true);
		m_PacketNoC.EnableWindow(true);
		
//		m_filepath="";
//		m_recvfpath="";
//		m_RecvFileS.SetWindowText("0 Bytes");
//		m_SendFileS.SetWindowText("0 Bytes");
//		UpdateData(false);	
		check = false;
	}
	else
	{
//		m_SendFileC.EnableWindow(false);
//		m_RecvFileC.EnableWindow(false);
		m_PacketNoC.EnableWindow(true);

//		m_filepath="";
//		m_recvfpath="";
//		m_RecvFileS.SetWindowText("0 Bytes");
//		m_SendFileS.SetWindowText("0 Bytes");
		m_ButtonSend.EnableWindow(true);
		m_ButtonRecv.EnableWindow(true);

		m_Alert.EnableWindow(true);
		m_RecvBlock.EnableWindow(true);
		m_Async.EnableWindow(true);

		m_SendBlock.EnableWindow(true);
		m_IOMode.EnableWindow(true);
	
		UpdateData(false);
		check = true;
	}
}

void CNetprobe1Dlg::OnDoubleclickedCheck1() 
{
	// TODO: Add your control notification handler code here

}



////////////////////////////////////////////////////////////////
//	Thread 
///////////////////////////////////////////////////////////////
int CNetprobe1Dlg::MultiThread()
{
	DWORD ThreadID;
	hThreadm = CreateThread(0, 
					 	    0, 
						    CNetprobe1Dlg::UDPThreadProc,
						    (LPVOID)this, 
						    0, 
						    &ThreadID);

	if (hThreadm == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}


////////////////////////////////
//	UDP Threading Server Procedure
///////////////////////////////
DWORD WINAPI CNetprobe1Dlg::UDPThreadProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;
	ES_Connection *lCompKey;
	unsigned long ulSeqNum=0;
	ES_FlashTimer Timer;
	int nBytesRead;
	Ufile = true;
	bool Urun = true;
	int control=20;
	int i = pclass->NumOfUDPClient;
	bool UDPTrun = pclass->bIsRun;

	while(UDPTrun && Urun)
	{
		Ustart = true;
		{
			lCompKey = pclass->ConnClient[i];
			if (lCompKey->getState() == pclass->m_iocp->gethConn() )
			{
				int addrlen = sizeof(struct sockaddr_in);
	
				ulSeqNum =0;
				lCompKey->UpdateSeqNo(ulSeqNum);
				
				if (Ufile)
					lCompKey->ChangeState(pclass->m_iocp->gethFile());
				else
					lCompKey->ChangeState(pclass->m_iocp->gethUDP());
			}
			else if (lCompKey->getState() == pclass->m_iocp->gethFile())
			{
				CString RequestFile = default_path+lCompKey->getfile();
				CFile checkfp; 
				CFileException e;

				if (checkfp.Open( (LPCTSTR)RequestFile , CFile::modeRead |CFile::shareDenyNone, &e ))
				{
					lCompKey->Initfp(RequestFile);		
					lCompKey->ChangeState(pclass->m_iocp->gethUDP());

					char ip[100];				
					sprintf(ip, "UDP - IP: %s  %s", lCompKey->getClientIP(), default_path+lCompKey->getfile());


					if (pclass->NoOfCon==1)
					{					
						pclass->m_Conn1.SetWindowText(ip);
					}
					else if (pclass->NoOfCon==2)
					{
						pclass->m_Conn2.SetWindowText(ip);
					}
					else if (pclass->NoOfCon==3)
					{
						pclass->m_Conn3.SetWindowText(ip);
					}
					else if (pclass->NoOfCon==4)
					{
						pclass->m_Conn4.SetWindowText(ip);
					}
					else if (pclass->NoOfCon==5)
					{
						pclass->m_Conn5.SetWindowText(ip);
					}

					char *pData = new char[100];
					memset(pData, 0, 100); // Clear data to all zero.

					*((unsigned long *) pData) = -300; // Fill in the signal no. for "File Exists"
					char* p = pData;
			//		p += sizeof(unsigned long);
			//		*((int *) p) = pclass->lPktSize; 
					  
					int datasend = sendto(pclass->pUDP->GetHandle(), pData, 100, 0, (struct sockaddr *)&lCompKey->getClientaddr(), sizeof(struct sockaddr_in));
					
					delete [] pData;

					Sleep(5000);
				}
				else
				{
					char *pData = new char[100];
					memset(pData, 0, 100); // Clear data to all zero.

					*((unsigned long *) pData) = -200; // Fill in the signal no. for the "File not found"
					char* p = pData;
					  
					int datasend = sendto(pclass->pUDP->GetHandle(), pData, 100, 0, (struct sockaddr *)&lCompKey->getClientaddr(), sizeof(struct sockaddr_in));
					
					delete [] pData;

					lCompKey->ChangeState(pclass->m_iocp->gethWait());		
				}
	
			
			}
			else if ( lCompKey->getState() == pclass->m_iocp->gethUDP() ) 
			{
				run = true;
				Timer.Start();
				if (lCompKey->bytessent() < 100000)
				{
					while (10240*1000.0/102400 >= Timer.Elapsed())
					{}
				}
				else if (lCompKey->bytessent() < 300000)
				{
					while (10240*1000.0/250000 >= Timer.Elapsed())
					{}
				}
				else if (lCompKey->bytessent() < 2000000)
				{
					while (10240*1000.0/500000 >= Timer.Elapsed())
					{}
				}
				else
				{	
					if (lCompKey->getSRate() == 0)
					{
						while (pclass->iSendDelay >= Timer.Elapsed())
						{}
					}
					else if (lCompKey->getSRate() == -1)
					{
						while (pclass->iSendDelay1 >= Timer.Elapsed())
						{}
					}
					else if (lCompKey->getSRate() == -2)
					{
						while (pclass->iSendDelay2 >= Timer.Elapsed())
						{}
					}
					else if (lCompKey->getSRate() == -20)
					{
						while (pclass->iSendDelay2w >= Timer.Elapsed())
						{}
					}					
					else if (lCompKey->getSRate() == -10)
					{
						while (pclass->iSendDelay1w >= Timer.Elapsed())
						{}
					}

				}				

//				Sleep(control);
//				Sleep(200);

				int closefile =0;

				char *pData = new char[pclass->lPktSize];
				memset(pData, 0, pclass->lPktSize); // Clear data to all zero.

				*((unsigned long *) pData) = lCompKey->getSeqNo(); // Fill in the seq no.
				char* p = pData;
				p += sizeof(unsigned long);
				*((int *) p) = pclass->lPktSize; 

				if (Ufile)
				{			
					nBytesRead = lCompKey->getfp()->Read( p+sizeof(int), pclass->lPktSize-sizeof(unsigned long)-sizeof(int));
//					nBytesRead = lCompKey->getfp()->Read( p, pclass->lPktSize);

					if (nBytesRead<=0)
					{
						lCompKey->getfp()->Close();	
						lCompKey->ChangeState(pclass->m_iocp->gethFin());					
						closefile = 1;									
					}
				}

				if (nBytesRead>0)
				{
					lCompKey->UpdateBsent(nBytesRead);	
					
					if (lCompKey->getID()==1)
					{
						pclass->m_Pg1.SetPos(lCompKey->bytessent()*100/lCompKey->getfp()->GetLength());
					}
					else if (lCompKey->getID()==2)
					{
						pclass->m_Pg2.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
					}
					else if (lCompKey->getID()==3)
					{
						pclass->m_Pg3.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
					}
					else if (lCompKey->getID()==4)
					{
						pclass->m_Pg4.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
					}
					else if (lCompKey->getID()==5)
					{
						pclass->m_Pg5.SetPos(lCompKey->bytessent()*100/ lCompKey->getfp()->GetLength());
					}
				}
				
				if (!closefile)
				{
					ulSeqNum = lCompKey->getSeqNo()+1;
					lCompKey->UpdateSeqNo(ulSeqNum);			
					int d = sizeof(const struct sockaddr);

					// send a message to notify the client 

					int datasend = sendto(pclass->pUDP->GetHandle(), pData, pclass->lPktSize, 0, (struct sockaddr *)&lCompKey->getClientaddr(), sizeof(struct sockaddr_in));
				}

				delete [] pData;
			}
			else if ( lCompKey->getState() == pclass->m_iocp->gethFin() )
			{
				int result;
		
				char *pData = new char[pclass->lPktSize];
				memset(pData, 0, pclass->lPktSize); // Clear data to all zero.

				*((unsigned long *) pData) = -100; // Fill in the seq no.
				char* p = pData;
				p += sizeof(unsigned long);
				*((int *) p) = pclass->lPktSize; 
				  
				int datasend = sendto(pclass->pUDP->GetHandle(), pData, pclass->lPktSize, 0, (struct sockaddr *)&lCompKey->getClientaddr(), sizeof(struct sockaddr_in));
				
				delete [] pData;

				int d = sizeof(const struct sockaddr);	

				lCompKey->ChangeState(pclass->m_iocp->gethWait());				

				if ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING))
				{
					int b = WSAGetLastError();
	//				AfxMessageBox("Error in WSASendTo, terminated. File Closes!");
				}				
			}		
			else if ( lCompKey->getState() == pclass->m_iocp->gethWait() )
			{					
				lCompKey->~ES_Connection();
		
				if (pclass->header == pclass->NumOfUDPClient)
				{
		//			Urun = false;
					Ustart = false;
				}
				pclass->header++;

				break;
				//printf("header = %d ,NumOfUDPClient = &d ", pclass->header, pclass->NumOfUDPClient);
			}	
		}
	}

	return 0;
}

void CNetprobe1Dlg::WaitForConnection()
{
	pTCPServer = new ES_TCPServer();
	pTCPServer->Startup(usPort, hostname,1,recvfname);

	while (1)
	{
		 if (pTCPServer->Accept())
		 {			 
			 MultiThread();
			 break;
		 }
	}


}


void CNetprobe1Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here

	DWORD ThreadIDMC;

	UpdateData(true);
	MCfilename = m_MCF;
	
	if (MCfilename != "")
	{
		if (MCast == false)
			MCast = true;
		else
			MCast = false;

		if (MCast)
		{		
			SetDlgItemText(IDC_MC,"STOP");
			m_ButtonQuit.EnableWindow(false);
			m_ButtonSend.EnableWindow(false);

			 Mh = CreateThread(0, 
							   0, 
							   CNetprobe1Dlg::MultiCastThreadProc,
							   (LPVOID)this, 
							   0, 
							   &ThreadIDMC);

			if (Mh == NULL)		
					MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);	
		}		
		else
		{
			m_ButtonSend.EnableWindow(true);
			SetDlgItemText(IDC_MC,"Multi-cast");
			m_ButtonQuit.EnableWindow(true);
			TerminateThread(Mh,0);
		}
	}
}

DWORD WINAPI CNetprobe1Dlg::MultiCastThreadProc(LPVOID IpInstance)
{
	 CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;
	
	 ES_FlashTimer Timer;
//	 Sleep(3000);

	 hS = WSASocket(PF_INET, 
						SOCK_DGRAM, 
						IPPROTO_UDP, 
						(LPWSAPROTOCOL_INFO) 0, 
						(GROUP) 0, 
						WSA_FLAG_OVERLAPPED);

	  if (hS == INVALID_SOCKET) {
		printf ("socket() failed, Err: %d\n", WSAGetLastError());
		exit(1);
	  }
	
	  stLclAddr.sin_family      = AF_INET; 
	  stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */
	  stLclAddr.sin_port        = 0;                 /* any port */

	  nRet = bind(hS, 
		  (struct sockaddr*) &stLclAddr, 
		  sizeof(stLclAddr));

	  if (nRet == SOCKET_ERROR) {
		  printf ("bind() port: %d failed, Err: %d\n", nPort, 
		  WSAGetLastError());
	  }

		stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
		stMreq.imr_interface.s_addr = INADDR_ANY;

		nRet = setsockopt(hS, 
						 IPPROTO_IP, 
						 IP_ADD_MEMBERSHIP, 
						 (char *)&stMreq, 
						 sizeof(stMreq));

		if (nRet == SOCKET_ERROR) 
		{
			printf ("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n",
				  achMCAddr, WSAGetLastError());
		}
		
		
		  /* Set IP TTL to traverse up to multiple routers */
		  nRet = setsockopt(hS, 
			 IPPROTO_IP, 
			 IP_MULTICAST_TTL, 
			 (char *)&lTTL, 
			 sizeof(lTTL));
		  if (nRet == SOCKET_ERROR) {
			printf ("setsockopt() IP_MULTICAST_TTL failed, Err: %d\n",
			  WSAGetLastError());
		  }

		  /* Disable loopback */
		  fFlag = FALSE;
		  
		  nRet = setsockopt(hS, 
			 IPPROTO_IP, 
			 IP_MULTICAST_LOOP, 
			 (char *)&fFlag, 
			 sizeof(fFlag));
		  if (nRet == SOCKET_ERROR) {
			printf ("setsockopt() IP_MULTICAST_LOOP failed, Err: %d\n",
			  WSAGetLastError());
		  }

		   /* Assign our destination address */
		  stDstAddr.sin_family =      AF_INET;
		  stDstAddr.sin_addr.s_addr = inet_addr(achMCAddr);
		  stDstAddr.sin_port =        htons(nPort);

		CString MCfile = default_path+pclass->MCfilename;
		CFile checkfp;
		CFileException e;

		if (checkfp.Open( (LPCTSTR)MCfile , CFile::modeRead |CFile::shareDenyNone, &e ))
		{

			CFile *Mfp = new CFile( (LPCTSTR)MCfile, CFile::modeRead | CFile::shareDenyNone );
			
			int seq = 0;
			int total=0;

			 while(1) {
				/* Get System (UTC) Time */
			//    GetSystemTime (&stSysTime);

				char *pData = new char[10240];
				memset(pData, 0, 10240); // Clear data to all zero.

				*((unsigned long *) pData) = seq; // Fill in the seq no.
				char* p = pData+8;
				
				seq++;

				int nBytes = Mfp->Read(p,10232);
				total += nBytes;
				
				if (nBytes <=0)
				{
					char *pData = new char[100];
					memset(pData, 0, 100); // Clear data to all zero.
					*((unsigned long *) pData) = -100; 				

					nRet = sendto(hS, pData, 100 ,0,(struct sockaddr*)&stDstAddr, sizeof(stDstAddr));

					break;
				}

				char temp[200];
				sprintf(temp,"Multicast File Streaming from %s", TIMECAST_ADDR);
													
				pclass->m_Conn1.SetWindowText(temp);

				pclass->m_Pg1.SetPos(total*100.0/Mfp->GetLength());

				Timer.Start();

				if (total < 100000)
				{
					while (10240*1000.0/102400 >= Timer.Elapsed())
					{}
				}
				else if (total < 300000)
				{
					while (10240*1000.0/250000 >= Timer.Elapsed())
					{}
				}
				else if (total < 2000000)
				{
					while (10240*1000.0/500000 >= Timer.Elapsed())
					{}
				}

				/* Send the time to our multicast group! */
				nRet = sendto(hS, pData, 10240 ,0,(struct sockaddr*)&stDstAddr, sizeof(stDstAddr));

				if (nRet < 0) {
				  printf ("sendto() failed, Error: %d\n", WSAGetLastError());
				  exit(1);
				} else {
				}
		
				/* Wait for the specified interval */
				Sleep(50);

			} /* end while */
	
	 		delete Mfp;

			
		}

	 
	  /* Close the socket */
	  closesocket(hS);

	  /* Tell WinSock we're leaving */
//	  WSACleanup();

	return 0;
}
