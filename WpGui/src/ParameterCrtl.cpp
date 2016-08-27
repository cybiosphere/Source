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

// ParameterCrtl.cpp : implementation file
//

#include "stdafx.h"
#include "Cybiosphere.h"
#include "ParameterCrtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParameterCrtl

CParameterCrtl::CParameterCrtl()
{
	 m_pMinStatic  = NULL;
   m_pValStatic  = NULL;
	 m_pMaxStatic  = NULL;
	 m_pTextStatic = NULL;
   m_pParam = NULL;
   m_pFont = new CFont; 
   m_pFont->CreatePointFont(70, LPCTSTR("Arial"));
}

CParameterCrtl::~CParameterCrtl()
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


BEGIN_MESSAGE_MAP(CParameterCrtl, CProgressCtrl)
	//{{AFX_MSG_MAP(CParameterCrtl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameterCrtl message handlers

bool CParameterCrtl::CreateNewParam(CGenericParam* pParam,const RECT &rect, CWnd *pParentWnd, UINT nID)
{
  CString tmpStr;
  m_pParam = pParam;

  // Create Progress bar
  bool resu = Create(PBS_SMOOTH,rect,pParentWnd,nID);
  
  // Add Title
  m_pTextStatic = new CStatic();
  CRect rectText = rect;
  rectText.top -= 11;
  rectText.bottom = rect.top;
  m_pTextStatic->Create(LPCTSTR(m_pParam->getLabel().c_str()),SS_CENTER,rectText,pParentWnd);
	m_pTextStatic->SetFont(m_pFont);
  m_pTextStatic->ShowWindow(SW_SHOW);

  // Add Min
  m_pMinStatic = new CStatic();
  CRect rectMin = rect;
  rectMin.top += 9;
  rectMin.bottom += 14;
  rectMin.right = rect.left + 30;
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getMin());
  m_pMinStatic->Create(tmpStr,SS_LEFT,rectMin,pParentWnd);
	m_pMinStatic->SetFont(m_pFont);

  // Add Val
  m_pValStatic = new CEdit();
  CRect rectVal = rect;
  rectVal.top += 9;
  rectVal.bottom += 14;
  rectVal.left = rect.left + 26;
  rectVal.right = rect.right - 26;
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getVal());
  m_pValStatic->Create(/*tmpStr,*/SS_CENTER|WS_DISABLED,rectVal,pParentWnd, nID+100);
	m_pValStatic->SetFont(m_pFont);

  // Add Max
  m_pMaxStatic = new CStatic();
  CRect rectMax = rect;
  rectMax.top += 9;
  rectMax.bottom += 14;
  rectMax.left = rect.right - 30;
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getMax());
  m_pMaxStatic->Create(tmpStr,SS_RIGHT,rectMax,pParentWnd);
	m_pMaxStatic->SetFont(m_pFont);
  
  SetRange((short)m_pParam->getMin(),(short)m_pParam->getMax());
  SetStep(1);
  SetPos((short)m_pParam->getVal());
  
  ShowWindow(SW_SHOW);
  m_pMinStatic->ShowWindow(SW_SHOW);
  m_pValStatic->ShowWindow(SW_SHOW);
  m_pMaxStatic->ShowWindow(SW_SHOW);

  return (resu);
}

bool CParameterCrtl::RefreshDisplayParam(bool resetMinMax)
{
  CString tmpStr;

  // refresh Val
  tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getVal());
  m_pValStatic->SetWindowText(tmpStr);
  m_pValStatic->UpdateWindow();
  
  if (resetMinMax)
  {
    tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getMin());
    m_pMinStatic->SetWindowText(tmpStr);
    m_pMinStatic->UpdateWindow();

    tmpStr.Format(LPCTSTR("%.1f"),m_pParam->getMax());
    m_pMaxStatic->SetWindowText(tmpStr);
    m_pMaxStatic->UpdateWindow();
  }

  // refresh Bar
  SetPos((short)m_pParam->getVal());
  UpdateWindow();

  return (true);
}
