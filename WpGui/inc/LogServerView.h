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

// LogServerView.h : interface of the CLogServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSERVER_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_LOGSERVER_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "LogServerDoc.h"

class CLogServerView : public CFormView
{
protected: // create from serialization only
	CLogServerView();
	DECLARE_DYNCREATE(CLogServerView)

public:
	//{{AFX_DATA(CLogServerView)
	enum { IDD = IDD_LOGSERVER_FORMVIEW };
  CEdit m_ServerLogEditBox;
  CString	m_strLog;
	//}}AFX_DATA

// Attributes
public:
  CLogServerDoc* GetDocument();

// Operations
public:
  void AddLog(std::string newLog);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogServerView)
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
	virtual ~CLogServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:



// Generated message map functions
protected:
	//{{AFX_MSG(CLogServerView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGSERVER_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
