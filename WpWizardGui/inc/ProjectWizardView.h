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

// ProjectWizardView.h : interface of the CProjectWizardView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTWIZARDVIEW_H__4EDA7D78_08D6_4FCF_98CB_49FE5818EF20__INCLUDED_)
#define AFX_PROJECTWIZARDVIEW_H__4EDA7D78_08D6_4FCF_98CB_49FE5818EF20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WizardTabCtrl.h"
#include "DialogTab1.h"
#include "DialogTab2.h"
#include "DialogTab3.h"
#include "DialogTab4.h"

#include "CBasicEntity.h"

class CProjectWizardView : public CFormView
{

private:
    CBasicEntity* m_pEntity;
    ClassType_e   m_ClassType;
    bool          m_bSexualDimorphism;

protected: // create from serialization only
	CProjectWizardView();
	DECLARE_DYNCREATE(CProjectWizardView)

public:
	//{{AFX_DATA(CProjectWizardView)
	enum { IDD = IDD_PROJECTWIZARD_FORM };
	CWizardTabCtrl	m_WizardTab;
	//}}AFX_DATA

// Attributes
public:
	CProjectWizardDoc* GetDocument();

    CBasicEntity* GetpEntity();
    ClassType_e   GetClassType();
    bool          IsSexualDimorphism();

    void          SetpEntity(CBasicEntity* pEntity, bool bSexualDimorphism);

    CDialogTab1* GetTabCaractFemale();
    CDialogTab1* GetTabCaractMale();
    CDialogTab2* GetTabSensors();
    CDialogTab3* GetTabBehavior();
    CDialogTab4* GetTabGenerate();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectWizardView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProjectWizardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CProjectWizardView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ProjectWizardView.cpp
inline CProjectWizardDoc* CProjectWizardView::GetDocument()
   { return (CProjectWizardDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTWIZARDVIEW_H__4EDA7D78_08D6_4FCF_98CB_49FE5818EF20__INCLUDED_)
