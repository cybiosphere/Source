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

// BrainView.h : interface of the CBrainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRAINVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_BRAINVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBrain.h" 
#include "ParameterCrtl.h"
#include "BrainGridCtrl.h"
#include "ActionButton.h"

#define MAX_NUMBER_BUTTONS_ACTION 16

typedef enum
{
  BRAIN_LEARNING_AUTO = 0,
  BRAIN_LEARNING_NONE,
  BRAIN_LEARNING_STICK,    
  BRAIN_LEARNING_CARROT 
} BrainViewLearningMode_e;


class CBrainView : public CFormView
{
protected: // create from serialization only
	CBrainView();
	DECLARE_DYNCREATE(CBrainView)

public:
	//{{AFX_DATA(CBrainView)
	enum { IDD = IDD_BRAIN_FORMVIEW };
	CButton	m_ButtonAutoChoice;
	CString	m_Feedback;
	CString	m_LabelAction;
	CBrainGridCtrl	m_BrainGrid;
	double	m_LearningRate;
	CString	m_FeelingCurrent;
	CString	m_FeelingPrevious;
	CString	m_FeelingDif;
	int		m_radioMode;
	//}}AFX_DATA

// Attributes
public:
	CBrainDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrainView)
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
  bool SetBrain(CBrain* pBrain);
  //void SetGridCtrlTitles(CBrainGridCtrl *pBrainGridCtrl);
  void ExecuteActionAndGetAutoFeedback(int actionIndex);
  //void RefreshGridCtrlData(CBrainGridCtrl *pBrainGridCtrl);

// Implementation
public:
	virtual ~CBrainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:


protected:

  CBrain* m_pBrain;
  //CReaction* m_pCurReaction;
  //CSensor* m_pCurSensor;

  BrainViewLearningMode_e m_LearningMode;
  CActionButton* m_pActionButtonTable[MAX_NUMBER_BUTTONS_ACTION];

// Generated message map functions
protected:
	//{{AFX_MSG(CBrainView)
	afx_msg void OnButtonExecAuto();
	afx_msg void OnChangeLearning();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonEditIdentify();
	afx_msg void OnRadioAuto();
	afx_msg void OnRadioNone();
	afx_msg void OnRadioStick();
	afx_msg void OnRadioCarrot();
	afx_msg void OnButtonPrediction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CBrainView.cpp
inline CBrainDoc* CBrainView::GetDocument()
   { return (CBrainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRAINVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
