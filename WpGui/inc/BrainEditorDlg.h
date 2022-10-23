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

#if !defined(AFX_BRAINEDITORDLG_H__1DA9CAF1_4353_4411_B31C_0EC157D607A1__INCLUDED_)
#define AFX_BRAINEDITORDLG_H__1DA9CAF1_4353_4411_B31C_0EC157D607A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrainEditorDlg.h : header file
//
#include "BrainGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// BrainEditorDlg dialog

class CBrainEditorDlg : public CDialog
{
// Construction
public:
	CBrainEditorDlg(CBrain* pBrain, bool identifyBrain = false, CString userTitle = LPCTSTR(""), CWnd* pParent = NULL);   // standard constructor 

protected:
// Dialog Data
	//{{AFX_DATA(CBrainEditorDlg)
	enum { IDD = IDD_DLG_BRAIN_EDITOR };
	CButton	m_ButtonLoad;
	CButton	m_ButtonSave;
	CButton	m_ButtonMore;
	CButton	m_ButtonLess;
	CButton	m_ButtonFind;
  CBrainGridCtrl m_BrainGrid;
	BOOL m_bColorizeDeltaFromBirth;
	BOOL m_bColorizeDeltaWithOther;

	//}}AFX_DATA

  CBrain* m_pBrain;
  bool m_bIdentifyBrain;
  CNeuronMatrix* m_pNeuronMatrix;
  string m_strBrainBackup;
  CString m_strWindowsTitle;
	CBasicEntity* m_pReferenceAnimal{ NULL };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrainEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ColorizeDeltaFromReference();

	// Generated message map functions
	//{{AFX_MSG(CBrainEditorDlg)
  afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonMore();
	afx_msg void OnButtonLess();
	afx_msg void OnButtonFind();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonExport();
	afx_msg void OnButtonImport();
	afx_msg void OnCheckColorizeDeltaFromBirth();
	afx_msg void OnCheckColorizeDeltaWithOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRAINEDITORDLG_H__1DA9CAF1_4353_4411_B31C_0EC157D607A1__INCLUDED_)
