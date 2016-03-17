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

#if !defined(AFX_DIALOGTAB2_H__F0B70293_3D4C_446C_9BE4_3116FA525A03__INCLUDED_)
#define AFX_DIALOGTAB2_H__F0B70293_3D4C_446C_9BE4_3116FA525A03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTab2.h : header file
//

#define MAX_NUMBER_BUTTONS_SENSOR 19

#include "GeneButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogTab2 dialog

class CDialogTab2 : public CDialog
{
// Construction
public:
	CDialogTab2(CWnd* pParent = NULL);   // standard constructor
    ~CDialogTab2();

// Dialog Data
	//{{AFX_DATA(CDialogTab2)
	enum { IDD = IDD_DIALOG_TAB2 };
	CComboBox	m_comboViewAngle;
  BOOL	m_bUseVisualIdentification;
	//}}AFX_DATA

    CGeneButton* m_pSensorButtonTable[MAX_NUMBER_BUTTONS_SENSOR];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTab2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL



// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogTab2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTAB2_H__F0B70293_3D4C_446C_9BE4_3116FA525A03__INCLUDED_)
