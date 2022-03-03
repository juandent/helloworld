
// Investments.h : main header file for the Investments application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CInvestmentsApp:
// See Investments.cpp for the implementation of this class
//

class CInvestmentsApp : public CWinAppEx
{
public:
	CInvestmentsApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileFondo();
	afx_msg void OnFileInversion();
	afx_msg void OnFileRendimiento();
	afx_msg void OnFileRendimientostabla();
	afx_msg void OnFileGenericgrid();
};

extern CInvestmentsApp theApp;
