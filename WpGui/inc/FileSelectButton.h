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

#if !defined(AFX_FILESELECTBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_)
#define AFX_FILESELECTBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSelectButton.h : header file
//

#include "CGene.h"

class CBrainView;

/////////////////////////////////////////////////////////////////////////////
// CFileSelectButton window

class CFileSelectButton : public CButton
{
// Construction
public:
    CFileSelectButton(CView* pView, CString labelHeader, CString labelButton);

// Attributes
public:

// Operations
public:
  bool CreateNewButton(const RECT &rect, CWnd *pParentWnd, UINT nID);

  CString GetFileName();
  CString GetPathName();

  void initWithDefaultFile(CString pathName, CString fileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSelectButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileSelectButton();

	// Generated message map functions
protected:

    CView*  m_pView;
    CString m_Label;
    CFont *m_pFont;
    int m_GeneSubTtype;

    CString m_ButtonText;

	CStatic* m_pName1Static;
	CEdit*   m_pName1Edit;

    CString m_PathName;
    CString m_FileName;

	//{{AFX_MSG(CFileSelectButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESELECTBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_)
