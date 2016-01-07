// Netprobe1.h : main header file for the NETPROBE1 application
//

#if !defined(AFX_NETPROBE1_H__D545487B_2B92_42F5_A17D_807A3D02E79B__INCLUDED_)
#define AFX_NETPROBE1_H__D545487B_2B92_42F5_A17D_807A3D02E79B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetprobe1App:
// See Netprobe1.cpp for the implementation of this class
//

class CNetprobe1App : public CWinApp
{
public:
	CNetprobe1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetprobe1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNetprobe1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETPROBE1_H__D545487B_2B92_42F5_A17D_807A3D02E79B__INCLUDED_)
