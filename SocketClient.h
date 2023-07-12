// SocketClient.h : main header file for the SOCKETCLIENT application
//

#if !defined(AFX_SOCKETCLIENT_H__E144CE1C_2C42_46EF_BA8F_4E6DCEEF339C__INCLUDED_)
#define AFX_SOCKETCLIENT_H__E144CE1C_2C42_46EF_BA8F_4E6DCEEF339C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "winsock2.h"									//引用头文件
#pragma comment (lib,"ws2_32.lib")							//链接库文件
/////////////////////////////////////////////////////////////////////////////
// CSocketClientApp:
// See SocketClient.cpp for the implementation of this class
//

class CSocketClientApp : public CWinApp
{
public:
	CSocketClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSocketClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCLIENT_H__E144CE1C_2C42_46EF_BA8F_4E6DCEEF339C__INCLUDED_)
