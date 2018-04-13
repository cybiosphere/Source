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

#if !defined(AFX_ENTITYEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_)
#define AFX_ENTITYEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityEditorDlg.h : header file
//
#include "CBasicEntity.h"
#include "ParameterSlider.h"

/////////////////////////////////////////////////////////////////////////////
// CEntityEditorDlg dialog

class CEntityEditorDlg : public CDialog
{
// Construction
public:
	CEntityEditorDlg(CBasicEntity* pEntity, CWnd* pParent = NULL);   // standard constructor
    ~CEntityEditorDlg();

// Dialog Data
	//{{AFX_DATA(CEntityEditorDlg)
	enum { IDD = IDD_DLG_ENTITY_EDITOR };
	CButton	m_ButtonSave;
	CButton	m_ButtonLoad;
  CString	m_PhysWelfareText;
  CString	m_FeelingWelfareText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityEditorDlg)
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    CBasicEntity* m_pEntity;
    CParameterSlider* m_pParamSlider[MAX_NUMBER_PARAMETER_DISPLAY];

    void DisplayParamSliders();
public:
  afx_msg void OnLbnSelchangeList2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_)
