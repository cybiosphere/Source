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

#if !defined(AFX_ACTIONBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_)
#define AFX_ACTIONBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionButton.h : header file
//

#include "CReaction.h"

class CBrainView;

/////////////////////////////////////////////////////////////////////////////
// CActionButton window

class CActionButton : public CButton
{
// Construction
public:
	CActionButton(CReaction* pReact, int reactIndex, CBrainView* pBrainView);

// Attributes
public:

// Operations
public:
  bool CreateNewButton(const RECT &rect, CWnd *pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CActionButton();

	// Generated message map functions
protected:

  CReaction*  m_pReaction;
  CBrainView* m_pBrainView;
  int m_ReactionIndex;
  CFont *m_pFont;

	//{{AFX_MSG(CActionButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONBUTTON_H__9BDFE171_2D84_47C5_9F23_56EC8F1C7F81__INCLUDED_)
