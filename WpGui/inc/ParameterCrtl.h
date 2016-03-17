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

#if !defined(AFX_PARAMETERCRTL_H__F0F028E7_F490_40A4_9840_52A57AB17A60__INCLUDED_)
#define AFX_PARAMETERCRTL_H__F0F028E7_F490_40A4_9840_52A57AB17A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterCrtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParameterCrtl window

class CParameterCrtl : public CProgressCtrl
{
// Construction
public:
	CParameterCrtl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParameterCrtl)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool CreateNewParam(CGenericParam* pParam, const RECT& rect, CWnd* pParentWnd, UINT nID);
	bool RefreshDisplayParam(bool resetMinMax=false);
	virtual ~CParameterCrtl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CParameterCrtl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CStatic* m_pMinStatic;
	CEdit*   m_pValStatic;
	CStatic* m_pMaxStatic;
	CStatic* m_pTextStatic;
  CGenericParam* m_pParam;
  CFont *m_pFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERCRTL_H__F0F028E7_F490_40A4_9840_52A57AB17A60__INCLUDED_)
