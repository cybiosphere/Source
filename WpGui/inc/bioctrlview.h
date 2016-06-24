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

// BioCtrlView.h : interface of the CBioCtrlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIOCTRLVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_BIOCTRLVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h" 
#include "ParameterCrtl.h"

#define MAX_NUMBER_BIOCTRL_DISPLAY  10

class CBioCtrlView : public CFormView
{
protected: // create from serialization only
	CBioCtrlView();
	DECLARE_DYNCREATE(CBioCtrlView)

public:
	//{{AFX_DATA(CBioCtrlView)
	enum { IDD = IDD_BIOCTRL_FORMVIEW };
	CComboBox	m_ComboWind;
	CSliderCtrl	m_SliderSpeed;
	CSliderCtrl	m_SliderZoom;
	BOOL	m_bManualMode;
	CString	m_StrTime;
	BOOL	m_bStopOnEvent;
	CString	m_StrScenarioScore;
	//}}AFX_DATA

  int m_UserSpeed;

// Attributes
public:
	CBioCtrlDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBioCtrlView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

public:
  bool SetBiotop(CBiotop* pBiotop);
  void UpdateTimerDisplay(BiotopTime_t* pNewTime);
  void ForceModeManual(BOOL newMode);
  bool ForceChangeSpeed(int rate);
  bool RestoreUserSpeed();
  void SetStrScenarioScore(CString text);

// Implementation
public:
	virtual ~CBioCtrlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:


protected:

  CBiotop* m_pBiotop;
  CParameterCrtl* m_pParamCtrl[MAX_NUMBER_BIOCTRL_DISPLAY];

// Generated message map functions
protected:
	//{{AFX_MSG(CBioCtrlView)
	afx_msg void OnCheckModeManual();
	afx_msg void OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderZoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEdit();
	afx_msg void OnCheckStopOnEv();
	afx_msg void OnSelchangeComboWind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CBioCtrlView.cpp
inline CBioCtrlDoc* CBioCtrlView::GetDocument()
   { return (CBioCtrlDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIOCTRLVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
