// BrainView.h : interface of the CBrainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRAINVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_BRAINVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
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
	enum { IDD = IDD_STATISTIC_FORMVIEW };
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
  void ExecuteActionAndGetAutoFeedback(int actionIndex);

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


// Generated message map functions
protected:
	//{{AFX_MSG(CBrainView)
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
