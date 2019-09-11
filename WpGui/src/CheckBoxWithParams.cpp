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

// CheckBoxWithParams.cpp : implementation file
//

#include "stdafx.h"
#include "CheckBoxWithParams.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckBoxWithParams

CCheckBoxWithParams::CCheckBoxWithParams(CView* pView, bool defaultChecked,
                         CString paramName1, int defValParam1, CString paramName2, int defValParam2)
{
  m_pView = pView;
  m_Label = " ";
  m_ParamName1   = paramName1;
  m_ParamDefVal1 = defValParam1; 
  m_ParamName2   = paramName2;
  m_ParamDefVal2 = defValParam2;
  m_bDefaultChecked = defaultChecked;
  m_pFont = new CFont; 
  m_pFont->CreatePointFont(80, "Arial");

  m_pName1Static = NULL;
  m_pParam1Edit  = NULL;
  m_pName2Static = NULL;
  m_pParam2Edit  = NULL;
}

CCheckBoxWithParams::~CCheckBoxWithParams()
{
  if (m_pFont != NULL)
  {
    m_pFont->DeleteObject();
    delete m_pFont;
  }

  if (m_pName1Static != NULL)
    delete m_pName1Static;
  if (m_pParam1Edit != NULL)
    delete m_pParam1Edit;
  if (m_pName2Static != NULL)
    delete m_pName2Static;
  if (m_pParam2Edit != NULL)
    delete m_pParam2Edit;
}


BEGIN_MESSAGE_MAP(CCheckBoxWithParams, CButton)
	//{{AFX_MSG_MAP(CCheckBoxWithParams)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckBoxWithParams message handlers

void CCheckBoxWithParams::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CButton::OnLButtonUp(nFlags, point);
}

bool CCheckBoxWithParams::CreateNewButton( const RECT &rect, CWnd *pParentWnd, UINT nID)
{
  bool resu = false;
  CString tmpStr;

  // Create button
  resu = Create(m_Label, BS_AUTOCHECKBOX|BS_LEFTTEXT|WS_VISIBLE|WS_TABSTOP|WS_CHILD, rect, pParentWnd, nID);
  SetFont(m_pFont);

  int iDpi = GetDpiForWindow(this->m_hWnd);
  CRect rectText = rect;

  if (m_bDefaultChecked)
  {
    SetCheck(1);
  }

  int sizeX = MulDiv(40, iDpi, 96);
  int offsetX = MulDiv(50, iDpi, 96);

  if (m_ParamName1 != "")
  {
    // Add Title 
    m_pName1Static = new CStatic();
    rectText.left += MulDiv(70, iDpi, 96);
    rectText.right = rectText.left + sizeX;
    m_pName1Static->Create(m_ParamName1, SS_RIGHT, rectText, pParentWnd);
    m_pName1Static->SetFont(m_pFont);
    m_pName1Static->ShowWindow(SW_SHOW);

    // Add Value
    m_pParam1Edit = new CEdit();
    rectText.left += offsetX;
    rectText.right = rectText.left + sizeX;
    m_pParam1Edit->Create(SS_LEFT|WS_VISIBLE|WS_TABSTOP|WS_CHILD|WS_BORDER, rectText, pParentWnd, nID+151);
    m_pParam1Edit->SetFont(m_pFont);
    tmpStr.Format("%d",m_ParamDefVal1);
    m_pParam1Edit->SetWindowText(tmpStr);
    m_pParam1Edit->ShowWindow(SW_SHOW);
  }

  if (m_ParamName2 != "")
  {
    // Add Title
    m_pName2Static = new CStatic();
    rectText.left += offsetX;
    rectText.right = rectText.left + sizeX;
    m_pName2Static->Create(m_ParamName2, SS_RIGHT, rectText, pParentWnd);
    m_pName2Static->SetFont(m_pFont);
    m_pName2Static->ShowWindow(SW_SHOW);

    // Add Value
    m_pParam2Edit = new CEdit();
    rectText.left += offsetX;
    rectText.right = rectText.left + sizeX;
    m_pParam2Edit->Create(SS_LEFT|WS_VISIBLE|WS_TABSTOP|WS_CHILD|WS_BORDER, rectText, pParentWnd, nID+152);
    m_pParam2Edit->SetFont(m_pFont);
    tmpStr.Format("%d",m_ParamDefVal2);
    m_pParam2Edit->SetWindowText(tmpStr);
    m_pParam2Edit->ShowWindow(SW_SHOW);
  }

  return (resu);
}
