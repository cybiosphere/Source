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

#if !defined(AFX_CHECKBOXWITHPARAMS_H__INCLUDED_)
#define AFX_CHECKBOXWITHPARAMS_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckBoxWithParams.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CCheckBoxWithParams window

class CCheckBoxWithParams : public CButton
{
// Construction
public:
    CCheckBoxWithParams(CView* pView, bool defaultChecked,
                CString paramName1 = "", int defValParam1=0, 
                CString paramName2 = "", int defValParam2=0);

// Attributes
public:

// Operations
public:
  bool CreateNewButton(const RECT &rect, CWnd *pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBoxWithParams)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCheckBoxWithParams();

  CEdit* m_pParam1Edit;
  CEdit* m_pParam2Edit;

	// Generated message map functions
protected:

  CView*  m_pView;
  CString m_Label;
  CFont*  m_pFont;
    
  CString m_ParamName1;
  int     m_ParamDefVal1; 
  CString m_ParamName2;
  int     m_ParamDefVal2; 

  CStatic* m_pName1Static;
  CStatic* m_pName2Static;

  bool  m_bDefaultChecked;   

	//{{AFX_MSG(CCheckBoxWithParams)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_)
