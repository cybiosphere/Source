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

// GeneticView.h : interface of the CGeneticView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENETICVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
#define AFX_GENETICVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h" 
#include "ParameterCrtl.h"
#include "GenomeTreeCtrl.h"

class CGeneticView : public CFormView
{
protected: // create from serialization only
	CGeneticView();
	DECLARE_DYNCREATE(CGeneticView)

public:
	//{{AFX_DATA(CGeneticView)
	enum { IDD = IDD_GENETIC_FORMVIEW };
	CGenomeTreeCtrl	m_GenomeTree;
	CString	m_strData1;
	CString	m_strData2;
	CString	m_strData3;
	CString	m_strData4;
	CString	m_strLabel1;
	CString	m_strLabel2;
	CString	m_strLabel3;
	CString	m_strLabel4;
	//}}AFX_DATA

// Attributes
public:
	CGeneticDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneticView)
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
  bool SetEntity(CBasicEntity* pEntity);

  void DisplayGenome(CGenome* pGenome);
  void DisplayPair(CPairOfChromosome* pPair);
  void DisplayChromosome(CChromosome* pChromosome);
  void DisplayGene(CGene* pGene);

// Implementation
public:
	virtual ~CGeneticView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:


protected:

  CBasicEntity* m_pEntity;

  // Ref on current selected object.
  // 1 only must be != NULL at a time
  CGenome*           m_pCurrentGenome;
  CPairOfChromosome* m_pCurrentPair;
  CChromosome*       m_pCurrentChromosome;
  CGene*             m_pCurrentGene;

// Generated message map functions
protected:
	//{{AFX_MSG(CGeneticView)
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonEdit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CGeneticView.cpp
inline CGeneticDoc* CGeneticView::GetDocument()
   { return (CGeneticDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENETICVIEW_H__58782761_4F5D_4E50_829A_0F3B31591B0D__INCLUDED_)
