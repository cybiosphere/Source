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

// BiotopView.h : interface of the CBiotopView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIOTOPVIEW_H__CF650204_E1D6_41EE_ADF3_26409BA166A7__INCLUDED_)
#define AFX_BIOTOPVIEW_H__CF650204_E1D6_41EE_ADF3_26409BA166A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h"

#include "BiotopDisplay.h"

class CBiotopView : public CScrollView
{
protected: // create from serialization only
	CBiotopView();
	DECLARE_DYNCREATE(CBiotopView)

// Attributes
public:
	CBiotopDoc* GetDocument();
  void SetZoomRate (int zoomRate);
  void SetSpeedRate (int speedRate);
  void StartTimers();
  void SwitchToDisplay2d();
  void SwitchToDisplay3d();
  void ScrollToGridCoord (Point_t centerPos);

//Attributes
protected:
  CBiotopDisplay* m_pBioDisplay;
  CBiotopDisplay* m_pBioDisplay2D;
  CBiotopDisplay* m_pBioDisplay3D;
  UINT SECOND_TIMER_ID;
  UINT REFRESH_TIMER_ID;
  CBiotop* m_pBiotop;
  Point_t m_MenuSelCoord;
  double m_zoomFactor;

  LARGE_INTEGER m_SysTicksPrevSecEnd;
  bool m_IdleDisplayMode;

  CBasicEntity* m_pFocusedEntity; // focused entity change often

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBiotopView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

public:
  bool SetBiotop(CBiotop* pBiotop);
  bool SetSelectedEntity(CBasicEntity* pEntity);
  void ForceRefreshDisplay(bool clearAll = true);
  CBiotopDisplay* GetpBiotopDisplay();
// Implementation
public:
	virtual ~CBiotopView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBiotopView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAppAddNewEntity();
	afx_msg void OnAppAddGroup();
	afx_msg void OnAppRemoveEntity();
	afx_msg void OnAppEditBrain();
	afx_msg void OnAppEditGenome();
	afx_msg void OnAppEditTrainAndGrade();
  afx_msg void OnAppEditIdentifyBySelection();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnAppMonitorShort();
	afx_msg void OnAppMonitorLong();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in BiotopView.cpp
inline CBiotopDoc* CBiotopView::GetDocument()
   { return (CBiotopDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIOTOPVIEW_H__CF650204_E1D6_41EE_ADF3_26409BA166A7__INCLUDED_)
