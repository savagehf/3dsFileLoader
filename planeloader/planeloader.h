
// planeloader.h : main header file for the planeloader application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CplaneloaderApp:
// See planeloader.cpp for the implementation of this class
//

class CplaneloaderApp : public CWinAppEx
{
public:
	CplaneloaderApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CplaneloaderApp theApp;
