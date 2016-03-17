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

#if !defined(AFX_BIOTOPEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_)
#define AFX_BIOTOPEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BiotopEditorDlg.h : header file
//
#include "CBiotop.h"
#include "ParameterSlider.h"

/////////////////////////////////////////////////////////////////////////////
// CBiotopEditorDlg dialog

class CBiotopEditorDlg : public CDialog
{
// Construction
public:
	CBiotopEditorDlg(CBiotop* pBiotop, CWnd* pParent = NULL);   // standard constructor
    ~CBiotopEditorDlg();

// Dialog Data
	//{{AFX_DATA(CBiotopEditorDlg)
	enum { IDD = IDD_DLG_BIOTOP_EDITOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBiotopEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBiotopEditorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  CBiotop* m_pBiotop;
  CParameterSlider* m_pParamSlider[MAX_NUMBER_BIOCTRL_DISPLAY];

  void DisplayParamSliders();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIOTOPEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_)
