// host.cpp : implementation file
//
//////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "host.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// Chost dialog


Chost::Chost(CWnd* pParent /*=NULL*/)
	: CDialog(Chost::IDD, pParent)
{
	//{{AFX_DATA_INIT(Chost)
	m_IPreturn = _T("");
	m_IPaddr = _T("");
	m_Offname = _T("");	
	m_hostIP = _T("");
	//}}AFX_DATA_INIT
	
}


void Chost::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Chost)
	DDX_Control(pDX, IDC_COMBO1, m_com);
	DDX_Control(pDX, ID_Lookup, m_ctlLU);
	DDX_Text(pDX, IDC_IPReturn, m_IPreturn);
	DDX_Text(pDX, IDC_IPaddr, m_IPaddr);
	DDX_Text(pDX, IDC_Offname, m_Offname);
	DDX_CBString(pDX, IDC_COMBO1, m_hostIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Chost, CDialog)
	//{{AFX_MSG_MAP(Chost)
	ON_BN_CLICKED(ID_Lookup, OnLookup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Chost message handlers

void Chost::OnLookup() 
{
	struct hostent *hptr;
	const char * addr;
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if((inet_addr((LPCSTR)m_hostIP)) == INADDR_NONE)	// by name
	{
		if ( hptr = gethostbyname((LPCSTR)m_hostIP)	)
			addr = inet_ntoa(*((struct in_addr*)hptr->h_addr_list[0]));
		else
			addr = NULL;
	}
	else
		addr = (LPCSTR)m_hostIP;	// by IP

	if (inet_addr(addr) != INADDR_NONE)
	{
		unsigned long input = inet_addr(addr);
		hptr=gethostbyaddr((char *)(&input), sizeof(input), AF_INET);
	}

	if (hptr != NULL)
	{
		m_com.AddString(m_hostIP);
		m_Offname = hptr->h_name;
		UpdateData( FALSE );
		
		int i=0;

		m_IPaddr = m_IPreturn = inet_ntoa (*(in_addr *)hptr->h_addr_list[i++]);
		
		while ( hptr->h_addr_list[i] != NULL)
		{
			m_IPaddr += "\r\n";
			m_IPaddr += inet_ntoa (*(in_addr *)hptr->h_addr_list[i++]);
		}
		
		UpdateData ( FALSE );
	}
	else
	{
		m_Offname = "";
		m_IPaddr = "";
		m_IPreturn = "";
		UpdateData ( FALSE );
		MessageBox("No such hostname");
	}

	

}
