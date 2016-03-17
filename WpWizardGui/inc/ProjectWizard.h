/*
https://github.com/cybiosphere
copyright (c) 2005-2014 Frederic RIBERT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

// ProjectWizard.h : main header file for the PROJECTWIZARD application
//

#if !defined(AFX_PROJECTWIZARD_H__CB7A4C01_1172_4163_A0D5_2CFE7E918969__INCLUDED_)
#define AFX_PROJECTWIZARD_H__CB7A4C01_1172_4163_A0D5_2CFE7E918969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ProjectWizardDoc.h"
#include "ProjectWizardView.h"

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardApp:
// See ProjectWizard.cpp for the implementation of this class
//

class CProjectWizardApp : public CWinApp
{
public:
	CProjectWizardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectWizardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CProjectWizardApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTWIZARD_H__CB7A4C01_1172_4163_A0D5_2CFE7E918969__INCLUDED_)
