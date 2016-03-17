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

// ActionButton.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "ActionButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionButton

CActionButton::CActionButton(CReaction* pReact, int reactIndex, CBrainView* pBrainView)
{
  m_pReaction     = pReact;
  m_pBrainView    = pBrainView;
  m_ReactionIndex = reactIndex;

  m_pFont = new CFont; 
  m_pFont->CreatePointFont(85, "Arial");
}

CActionButton::~CActionButton()
{
  if (m_pFont != NULL)
  {
    m_pFont->DeleteObject();
    delete m_pFont;
  }
}


BEGIN_MESSAGE_MAP(CActionButton, CButton)
	//{{AFX_MSG_MAP(CActionButton)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionButton message handlers

void CActionButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pReaction != NULL)
  {
    m_pBrainView->ExecuteActionAndGetAutoFeedback(m_ReactionIndex);
  }

	CButton::OnLButtonUp(nFlags, point);
}

bool CActionButton::CreateNewButton( const RECT &rect, CWnd *pParentWnd, UINT nID)
{
  bool resu = false;

  // Create button
  if (m_pReaction != NULL)
  {
    resu = Create(m_pReaction->GetLabel().c_str(), WS_VISIBLE|WS_TABSTOP|WS_CHILD, rect, pParentWnd, nID);
    SetFont(m_pFont);
  }

  return (resu);
}
