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

// FileSelectButton.cpp : implementation file
//

#include "stdafx.h"
#include "FileSelectButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSelectButton

CFileSelectButton::CFileSelectButton(CView* pView, CString labelHeader, CString labelButton)
{
  m_pView = pView;
  m_Label = labelHeader;
  m_ButtonText = labelButton;

  m_pFont = new CFont; 
  m_pFont->CreatePointFont(85, "Arial");

  m_pName1Static = NULL;
  m_pName1Edit   = NULL;

  m_PathName = "";
  m_FileName = "";
}

CFileSelectButton::~CFileSelectButton()
{
  if (m_pFont != NULL)
  {
    m_pFont->DeleteObject();
    delete m_pFont;
  }

  if (m_pName1Static != NULL)
    delete m_pName1Static;
  if (m_pName1Edit != NULL)
    delete m_pName1Edit;
}


BEGIN_MESSAGE_MAP(CFileSelectButton, CButton)
	//{{AFX_MSG_MAP(CFileSelectButton)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSelectButton message handlers

void CFileSelectButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
    CFileDialog dlg(true,"xml","",0,"Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||");

    // Set Dialog properties
    dlg.m_ofn.lpstrTitle = "Select entity";

    // Open Dialog
    long nResponse = dlg.DoModal();

    if (nResponse == IDOK)
    {
        m_FileName = dlg.GetFileName();
        m_PathName = dlg.GetPathName();	 
        int endPath = m_PathName.ReverseFind ('\\');
        if (endPath>0)
          m_PathName = m_PathName.Left(endPath+1); 

        m_pName1Edit->SetWindowText(dlg.GetFileName());
    }

	CButton::OnLButtonUp(nFlags, point);
}

bool CFileSelectButton::CreateNewButton( const RECT &rect, CWnd *pParentWnd, UINT nID)
{
  bool resu = false;

  CRect rectText = rect;
  

  // Add Title
  m_pName1Static = new CStatic();
  rectText.right = rectText.left + 20;
  m_pName1Static->Create(m_Label, SS_RIGHT, rectText, pParentWnd);
  m_pName1Static->SetFont(m_pFont);
  m_pName1Static->ShowWindow(SW_SHOW);

  // Add Selection
  m_pName1Edit = new CEdit();
  rectText.left += 30;
  rectText.right = rectText.left + 240;
  m_pName1Edit->Create(SS_LEFT|WS_VISIBLE|WS_TABSTOP|WS_CHILD|WS_BORDER|WS_DISABLED, rectText, pParentWnd, nID+151);
  m_pName1Edit->SetFont(m_pFont);
  m_pName1Edit->ShowWindow(SW_SHOW);

  // Create button
  rectText.left += 250;
  rectText.right = rectText.left + 40;
  resu = Create(m_ButtonText, WS_VISIBLE|WS_TABSTOP|WS_CHILD, rectText, pParentWnd, nID);
  SetFont(m_pFont);

  return (resu);
}

CString CFileSelectButton::GetFileName()
{
  return m_FileName;
}

CString CFileSelectButton::GetPathName()
{
  return m_PathName;
}

