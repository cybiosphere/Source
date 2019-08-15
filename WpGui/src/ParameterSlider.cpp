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

// ParameterSlider.cpp : implementation file
//

#include "stdafx.h"
#include "CBiotop.h"
#include "ParameterSlider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParameterSlider

CParameterSlider::CParameterSlider(bool allowRangeEditMin, bool allowRangeEditMax)
{
   m_pMinStatic  = NULL;
   m_pValStatic  = NULL;
   m_pMaxStatic  = NULL;
   m_pTextStatic = NULL;
   m_pParam = NULL;
   m_pFont = new CFont; 
   m_pFont->CreatePointFont(64, LPCTSTR("Arial"));
   m_bIsChanging = false;
   m_bAllowRangeEditMin = allowRangeEditMin;
   m_bAllowRangeEditMax = allowRangeEditMax;
}

CParameterSlider::~CParameterSlider()
{
  if (m_pMinStatic != NULL)
    delete m_pMinStatic;
  if (m_pValStatic != NULL)
    delete m_pValStatic;
  if (m_pMaxStatic != NULL)
    delete m_pMaxStatic;
  if (m_pTextStatic != NULL)
    delete m_pTextStatic;
  if (m_pFont != NULL)
  {
    m_pFont->DeleteObject();
    delete m_pFont;
  }
}


BEGIN_MESSAGE_MAP(CParameterSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CParameterSlider)
	ON_NOTIFY_REFLECT(NM_RELEASEDCAPTURE, OnReleasedcapture)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameterSlider message handlers

bool CParameterSlider::CreateNewParam(CGenericParam* pParam, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
  CString tmpStr;
  m_pParam = pParam;
  RECT tmpRect = rect;
  // Create Progress bar
  tmpRect.top -= 2;
  tmpRect.bottom += 1;
  bool resu = Create(TBS_NOTICKS,tmpRect,pParentWnd,nID);

  // Add Title
  m_pTextStatic = new CStatic();
  CRect rectText = rect;
  rectText.top -= 14;
  rectText.bottom = rect.top;
  m_pTextStatic->Create(LPCTSTR(m_pParam->getLabel().c_str()),SS_CENTER,rectText,pParentWnd);
  m_pTextStatic->SetFont(m_pFont);
  m_pTextStatic->ShowWindow(SW_SHOW);

  // Add Min
  m_pMinStatic = new CEdit();
  CRect rectMin = rect;
  rectMin.top += 9;
  rectMin.bottom += 14;
  rectMin.right = rect.left + 30;
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getMin());
  if (m_bAllowRangeEditMin)
    m_pMinStatic->Create(SS_CENTER,rectMin,pParentWnd, nID+101);
  else
    m_pMinStatic->Create(SS_CENTER|WS_DISABLED,rectMin,pParentWnd, nID+101);
  m_pMinStatic->SetFont(m_pFont);
  m_pMinStatic->SetWindowText(tmpStr);

  // Add Val
  m_pValStatic = new CEdit();
  CRect rectVal = rect;
  rectVal.top += 9;
  rectVal.bottom += 14;
  rectVal.left = rect.left + 26;
  rectVal.right = rect.right - 26;
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getVal());
  m_pValStatic->Create(SS_CENTER|WS_DISABLED,rectVal,pParentWnd, nID+100);
  m_pValStatic->SetFont(m_pFont);
  m_pValStatic->SetWindowText(tmpStr);

  // Add Max
  m_pMaxStatic = new CEdit();
  CRect rectMax = rect;
  rectMax.top += 9;
  rectMax.bottom += 14;
  rectMax.left = rect.right - 30;
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getMax());
  if (m_bAllowRangeEditMax)
    m_pMaxStatic->Create(SS_CENTER,rectMax,pParentWnd, nID+102);
  else
    m_pMaxStatic->Create(SS_CENTER|WS_DISABLED,rectMax,pParentWnd, nID+102);
  m_pMaxStatic->SetFont(m_pFont);
  m_pMaxStatic->SetWindowText(tmpStr);
  
  SetRange((short)m_pParam->getMin(),(short)m_pParam->getMax());
  SetPageSize(1);
  SetPos((short)m_pParam->getVal());
  
  ShowWindow(SW_SHOW);
  m_pMinStatic->ShowWindow(SW_SHOW);
  m_pValStatic->ShowWindow(SW_SHOW);
  m_pMaxStatic->ShowWindow(SW_SHOW);

  return (resu);
}

bool CParameterSlider::RefreshDisplayParam()
{
  CString tmpStr;

  if (!m_bIsChanging)
  {
    // refresh Val
    tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getVal());
    m_pValStatic->SetWindowText(tmpStr);
    m_pValStatic->UpdateWindow();

    // refresh Bar
    SetPos((short)m_pParam->getVal());
    UpdateWindow();
  }
  else
  {
    // refresh Val
    tmpStr.Format(LPCTSTR("%.1f"),(double)GetPos());
    m_pValStatic->SetWindowText(tmpStr);
    m_pValStatic->UpdateWindow();
  }

  return (true);
}

void CParameterSlider::OnReleasedcapture(NMHDR* pNMHDR, LRESULT* pResult) 
{
  m_pParam->setVal(GetPos());

  RefreshDisplayParam();
  m_bIsChanging = false;
  *pResult = 0;
}

void CParameterSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bIsChanging = true;
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


void CParameterSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bIsChanging)
        RefreshDisplayParam();

	CSliderCtrl::OnMouseMove(nFlags, point);
}

CGenericParam* CParameterSlider::GetpParam()
{
  return m_pParam;
}

void CParameterSlider::OnDestroy() 
{
  if (m_bAllowRangeEditMin)
  {
    CString textMin;
    m_pMinStatic->GetWindowText(textMin);
    m_pParam->setValMin(atof((char*)textMin.GetBuffer(0)));
  }

  if (m_bAllowRangeEditMax)
  {
    CString textMax;
    m_pMaxStatic->GetWindowText(textMax);
    m_pParam->setValMax(atof((char*)textMax.GetBuffer(0)));
  }

	CSliderCtrl::OnDestroy();
	
}

