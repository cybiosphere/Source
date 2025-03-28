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

// StatisticDoc.h : interface of the CMapEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITORDOC_H__F018EFEA_0C64_4A89_99C6_751A1BF82088__INCLUDED_)
#define AFX_MAPEDITORDOC_H__F018EFEA_0C64_4A89_99C6_751A1BF82088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMapEditorDoc: public CDocument
{
protected: // create from serialization only
  CMapEditorDoc();
  DECLARE_DYNCREATE(CMapEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMapEditorDoc)
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual ~CMapEditorDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
  //{{AFX_MSG(CMapEditorDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORDOC_H__F018EFEA_0C64_4A89_99C6_751A1BF82088__INCLUDED_)
