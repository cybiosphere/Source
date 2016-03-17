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

#if !defined(AFX_MEASUREEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_)
#define AFX_MEASUREEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BiotopEditorDlg.h : header file
//
#include "CBiotop.h"


/////////////////////////////////////////////////////////////////////////////
// CMeasureEditorDlg dialog

class CMeasureEditorDlg : public CDialog
{
// Construction
public:
	CMeasureEditorDlg(CBiotop* pBiotop, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasureEditorDlg)
	enum { IDD = IDD_DLG_MEASURE_EDITOR };
	CComboBox	m_ComboEvent;
	CEdit	m_PeriodEditBox;
	CComboBox	m_ComboSubtype;
	CComboBox	m_ComboSpecie;
	CComboBox	m_ComboReaction;
	CComboBox	m_ComboEntity;
	CEdit	m_Title5;
	CEdit	m_Title4;
	CEdit	m_Title3;
	CEdit	m_Title2;
	CEdit	m_Title1;
	CComboBox	m_ComboType;
	CListBox	m_MeasureList;
	CButton	m_ButtonSave;
	CButton	m_ButtonLoad;
	CString	m_MeasureName;
	long	m_EditPeriod;
	int		m_EditThreshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  void InitMeasureList();
  void RefreshEditSection();
  void InitSelComboEntity (CMeasure* pMeas);
  void UpdateComboSubType();
  void UpdateAndInitComboReaction(CMeasure* pMeas);
  void InitSelComboSpecie (CMeasure* pMeas);
  void InitEventBoxes (CMeasure* pMeas);
  void RebuildNewMeasure();

	// Generated message map functions
	//{{AFX_MSG(CMeasureEditorDlg)
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListMeasures();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboEntity();
	afx_msg void OnSelchangeComboReaction();
	afx_msg void OnSelchangeComboSpecie();
	afx_msg void OnSelchangeComboSubtype();
	afx_msg void OnChangeEditPeriod();
	afx_msg void OnButtonAccept();
	afx_msg void OnChangeEditThreshold();
	afx_msg void OnSelchangeComboEvent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  CBiotop* m_pBiotop;
  CMeasure* m_pCurMeasure;
  CMeasure* m_pNewMeasure;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREEDITORDLG_H__C421D7AC_F854_4C03_9437_AE1DE271BD68__INCLUDED_)
