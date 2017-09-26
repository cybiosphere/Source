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

// EntityView.h : interface of the CEntityView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTITYVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_ENTITYVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBasicEntity.h" 
#include "ParameterCrtl.h"

#define MAX_NUMBER_PARAMETER_DISPLAY  30

class CEntityView : public CFormView
{
protected: // create from serialization only
	CEntityView();
	DECLARE_DYNCREATE(CEntityView)

public:
	//{{AFX_DATA(CEntityView)
	enum { IDD = IDD_ENTITY_FORMVIEW };
	long		m_nEntityId;
	CString	m_Label;
	CString	m_StageString;
	BOOL	m_checkImmortal;
	CString	m_PurposeString;
	//}}AFX_DATA

// Attributes
public:
	CEntityDoc* GetDocument();

  CImageList* m_pIconSex;
  CImageList* m_pIconForm;
  CImageList* m_pIconOdor;
  CImageList* m_pIconPheromon;
  CImageList* m_pIconTaste;
  CImageList* m_pIconTexture;
  CImageList* m_pIconAttribute;
  CImageList* m_pIconHealth;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityView)
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

// Implementation
public:
	virtual ~CEntityView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
  void SelectAndDisplayEntity(CBasicEntity* pEntity);
  void RefreshParameters();

protected:

  CBasicEntity* m_pEntity;
  CParameterCrtl* m_pParamCtrl[MAX_NUMBER_PARAMETER_DISPLAY];

// Generated message map functions
protected:
	//{{AFX_MSG(CEntityView)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckImmortal();
	afx_msg void OnEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EntityView.cpp
inline CEntityDoc* CEntityView::GetDocument()
   { return (CEntityDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
