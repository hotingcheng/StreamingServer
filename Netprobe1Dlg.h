// Netprobe1Dlg.h : header file
//

#if !defined(AFX_NETPROBE1DLG_H__48A53967_0409_4E30_B004_90DF1FADFA27__INCLUDED_)
#define AFX_NETPROBE1DLG_H__48A53967_0409_4E30_B004_90DF1FADFA27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ES_UDP.h"
#include "ES_TCPServer.h"
#include "ES_TIMER.h"
#include "IOCP.h"
#include "ES_Connection.h"

/////////////////////////////////////////////////////////////////////////////
// CNetprobe1Dlg dialog

class CNetprobe1Dlg : public CDialog
{
private:		
	CString MCfilename;
	char *FC_buf;
	bool FC_Run;
	int header;
	int NoOfCon;
	bool Tstart;
	int UDPID;
	int NumOfUDPClient;
	double iSendDelay;
	double iSendDelay0;
	double iSendDelay1;
	double iSendDelay1w;
	double iSendDelay2;
	double iSendDelay2w;
	bool bIsRun;
	bool CreateO;
	//ES_Connection *ConnClient[65535];
	ES_Connection **ConnClient;
	int NumOfClient;
	double recvfilesize;
	CFile *f;
	CFile *f1;
	int pDatalen;
	SOCKET cSocket;
	char * pData;
	bool aStart;		// alertable starts
	int aReceived;
	SOCKET hSocket;
	WSABUF **pWsaBuf;
	WSABUF **iocpBuf;

	WSAOVERLAPPED **iocpOverlap;
	unsigned int aBufIndex;
	unsigned long atotalpacket;
	int apacketrecv;
	unsigned int lBufferNum;

	unsigned long ulSeqNum;
	unsigned int nBytesRead; 

	bool OKrecv;
	bool send_complete;
	bool sendbreak;
	bool stoprecv;
	bool RTStart;
	SOCKET socket;
	bool QuitFlag;	
	bool bIsRunning;
	const unsigned int UDM_WINSOCK_SELECT;
	ES_FlashTimer DisplayTimer;
	ES_FlashTimer RecvTimer;
	ES_UDP* pUDP;
	ES_TCPServer* pTCPServer;
	bool SimpleBlockingIO;
	unsigned int lNumToSend;
	bool bSendForever;
	double lRate;
	unsigned long lPktSize;
	char iProtocol[20];
	int usPort;
	char hostname[1024];
	int iRefreshInterval;
//	unsigned long ulSeqNum;

// Construction
public:
	void WaitForConnection();
	void NotHold();
	void Hold();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CNetprobe1Dlg(CWnd* pParent = NULL);	// standard constructor	
	int RunSendMode(CNetprobe1Dlg* pclass);
	
	int AlertableRecv(CNetprobe1Dlg* pclass);
	static void CALLBACK SocketIOCompletion(IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags);
	static void CALLBACK IOCPRoutine(IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags);
	static DWORD WINAPI AlertableRecvProc(LPVOID IpInstance);
	int StopAlertableRecv();
	int StartAlertableRecv();

	int RecvStatThread();
	static DWORD WINAPI RecvStatProc(LPVOID );
	int StopThreadRecvStat();

	int RunRecvMode();
	static DWORD WINAPI ThreadSendProc(LPVOID );

	int StartDelayThread();
	static DWORD WINAPI ThreadDelayProc(LPVOID );

	int MultiThread();
	static DWORD WINAPI UDPThreadProc(LPVOID );

	static DWORD WINAPI TCPThreadPoolProc(LPVOID );
	static DWORD WINAPI FC_ThreadProc(LPVOID );

	static DWORD WINAPI ThreadAcceptProc(LPVOID );
	static DWORD WINAPI ThreadUDPAcceptProc(LPVOID );

	static DWORD WINAPI MultiCastThreadProc(LPVOID );
	

	int OnClose();
	bool OnAccept();
	int OnRead();
	int StopThreadedSend();
	int StartThreadedSend();

	IOCP *m_iocp;
		
	HANDLE Mh;
	HANDLE FC_Thread;
	HANDLE h;
	HANDLE hThread1;	
	HANDLE hThread2;
	HANDLE hThread3;
	HANDLE hThread4;
	HANDLE hThreadm;
	HANDLE Accept_hThread;
	HANDLE UDPAccept_hThread;
	HANDLE IOCP_hThreadTCP;
	HANDLE IOCP_hThreadUDP;


	HANDLE Conn;
	HANDLE TCP;
	HANDLE UDP;


	void ConvertRate(char* outrate, double inrate);
	bool InitVariable(int sendrecv);

// Dialog Data
	//{{AFX_DATA(CNetprobe1Dlg)
	enum { IDD = IDD_NETPROBE1_DIALOG };
	CButton	m_MC;
	CStatic	m_Conn5;
	CProgressCtrl	m_Pg5;
	CProgressCtrl	m_Pg4;
	CProgressCtrl	m_Pg3;
	CProgressCtrl	m_Pg2;
	CProgressCtrl	m_Pg1;
	CStatic	m_Conn4;
	CStatic	m_Conn3;
	CStatic	m_Conn2;
	CStatic	m_Conn1;
	CButton	m_CheckC;
	CEdit	m_BufferNoC;
	CButton	m_RecvBlock;
	CButton	m_SendBlock;
	CEdit	m_PeerC;
	CEdit	m_SizeC;
	CEdit	m_RefreshC;
	CEdit	m_PeerPortC;
	CEdit	m_PacketNoC;
	CEdit	m_LocalPortC;
	CEdit	m_RateC;
	CEdit	m_LocalC;
	CButton	m_ButtonQuit;
	CButton	m_IOMode;
	CButton m_Async;
	CButton m_UDP;
	CButton	m_ButtonRecv;
	CButton	m_ButtonSend;
	CButton	m_Protocol;
	UINT	m_Size;
	UINT	m_Refresh;
	UINT	m_PeerPort;
	CString	m_Peer;
	UINT	m_PacketNo;
	UINT	m_LocalPort;
	CString	m_Local;
	double	m_Rate;
	CButton	m_Alert;
	UINT	m_BufferNo;
	CString	m_MCF;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetprobe1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNetprobe1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHostInfo();
	afx_msg void OnSend();
	afx_msg void OnReceive();
	virtual void OnCancel();
	afx_msg void OnCancelMode();
	afx_msg int OnSendFile();
	afx_msg void OnRecvFile();
	afx_msg void OnCheck1();
	afx_msg void OnDoubleclickedCheck1();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETPROBE1DLG_H__48A53967_0409_4E30_B004_90DF1FADFA27__INCLUDED_)
