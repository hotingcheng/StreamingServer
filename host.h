#if !defined(AFX_HOST_H__DE7A2CDA_7691_4CC2_824A_E90BD0139258__INCLUDED_)
#define AFX_HOST_H__DE7A2CDA_7691_4CC2_824A_E90BD0139258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// host.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Chost dialog

class Chost : public CDialog
{
// Construction
public:
	Chost(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Chost)
	enum { IDD = IDD_HostInfo };
	CComboBox	m_com;
	CButton	m_ctlLU;
	CString	m_IPreturn;
	CString	m_IPaddr;
	CString	m_Offname;
	CString	m_hostIP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Chost)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Chost)
	afx_msg void OnLookup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOST_H__DE7A2CDA_7691_4CC2_824A_E90BD0139258__INCLUDED_)
