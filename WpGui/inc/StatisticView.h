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

// StatisticView.h : interface of the CStatisticView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTIC_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_STATISTIC_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h" 
#include "ParameterCrtl.h"
#include "ColorButton.h"

#define MAX_NB_DISPLAY_MEASURES 10

typedef struct 
{
  CMeasure*     pMeasure;
  CColorButton* pChkBox;
  COLORREF      color;
  CRect         coord;
} MeasureDisplay_t;


class CStatisticView : public CFormView
{
protected: // create from serialization only
	CStatisticView();
	DECLARE_DYNCREATE(CStatisticView)

public:
	//{{AFX_DATA(CStatisticView)
	enum { IDD = IDD_STATISTIC_FORMVIEW };
	int		m_radioDuration;
	//}}AFX_DATA

// Attributes
public:
	CBioCtrlDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticView)
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

// Implementation
public:
	virtual ~CStatisticView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:

  void RebuildMeasChkBox();
  void NextSecond();

protected:
  void ResetMinMaxTime();
  void ResetMinMaxMeasures();
  void DrawMeasure(CPaintDC *pDC, CMeasure* pMeasure, COLORREF color, int index);

protected:

  CBiotop* m_pBiotop;
  CRect    m_GraphRect;

  //CButton*         m_pMeasChkBoxTable[MAX_NB_DISPLAY_MEASURES];
  MeasureDisplay_t m_pMeasDisplayList[MAX_NB_DISPLAY_MEASURES];

  timeCountType m_TimeScaleSecPerPix;
  timeCountType m_RefreshCounter;
  timeCountType m_TimeMin;
  timeCountType m_TimeMax;

  POINT* m_pTmpPointTable;
  CFont* m_pFont;

  bool m_IsScrollMode;

// Generated message map functions
protected:
	//{{AFX_MSG(CStatisticView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnEditMeasure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTIC_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
