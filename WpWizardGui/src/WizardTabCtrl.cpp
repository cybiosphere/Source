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

// WizardTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectWizard.h"
#include "WizardTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardTabCtrl

CWizardTabCtrl::CWizardTabCtrl()
{
  m_tabPages[0] = new CDialogTab1();
  m_tabPages[1] = new CDialogTab1(NULL,TRUE);
  m_tabPages[2] = new CDialogTab2();
  m_tabPages[3] = new CDialogTab3();
  m_tabPages[4] = new CDialogTab4();

  m_tabCurrent = 0;
  m_bInitComplete = false;

  for(int i=0; i<NUM_TABS; i++)
    m_tabIndex[i] = -1;

  m_tab_CurMask = TAB_MASK_CAR_FEMALE | TAB_MASK_GENERATE;
}

CWizardTabCtrl::~CWizardTabCtrl()
{
    delete m_tabPages[0];
    delete m_tabPages[1];
    delete m_tabPages[2];
    delete m_tabPages[3];
    delete m_tabPages[4];
    delete m_pIconTabs;
}


BEGIN_MESSAGE_MAP(CWizardTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CWizardTabCtrl)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTabSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardTabCtrl message handlers

void CWizardTabCtrl::Init(void)
{
  m_pIconTabs = new CImageList();
  ASSERT(m_pIconTabs != NULL);  
  m_pIconTabs->Create(IDB_TABS_ICONS,16,16,0x0FFF);

  SetImageList(m_pIconTabs);

  m_tabPages[0]->Create(IDD_DIALOG_TAB1,this);
  m_tabPages[1]->Create(IDD_DIALOG_TAB1,this);
  m_tabPages[2]->Create(IDD_DIALOG_TAB2,this);
  m_tabPages[3]->Create(IDD_DIALOG_TAB3,this);
  m_tabPages[4]->Create(IDD_DIALOG_TAB4,this);

  CRect tabRect, itemRect;
  int nx, ny, nxc, nyc;

  GetClientRect(&tabRect);
  GetItemRect(0, &itemRect);
  nx = itemRect.left;
  ny = itemRect.bottom+1; 
  nxc = tabRect.right - itemRect.left - 1; 
  nyc = tabRect.bottom - ny - 1; 

  m_tabPages[0]->SetWindowPos(&wndTop,nx,ny,nxc,nyc,SWP_SHOWWINDOW);
  for(int i=1; i<NUM_TABS; i++)
    m_tabPages[i]->SetWindowPos(&wndTop,nx,ny,nxc,nyc,SWP_HIDEWINDOW);

  SetTabDisplay();

  ((CDialogTab1*)m_tabPages[0])->InitView();

  m_bInitComplete = true;

}

void CWizardTabCtrl::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
  m_tabPages[m_tabCurrent]->UpdateData(true);
  m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
  
  int tabViewIndex = GetCurSel();
  for(int i=0; i<NUM_TABS; i++)
  {
    if (m_tabIndex[i] == tabViewIndex)
    {
      m_tabCurrent = i;
      break;
    }
  }

  m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
  m_tabPages[m_tabCurrent]->SetFocus();  

  *pResult = 0;
}

void CWizardTabCtrl::Resize(int cx, int cy)
{
  MoveWindow(5, 5, cx-10, cy-10, true);
}

void CWizardTabCtrl::AddTabMask(unsigned char tabsMask)
{
  m_tab_CurMask |= tabsMask;
}

void CWizardTabCtrl::RemoveTabMask(unsigned char tabsMask)
{
  m_tab_CurMask &= ~tabsMask;
}

void CWizardTabCtrl::SetTabDisplay()
{
  int cnt = GetItemCount();
  int i;

  for(i=0; i<NUM_TABS; i++)
    m_tabIndex[i] = -1;

  for (i=(cnt-1); i>=0; i--)
    DeleteItem(i);

  if (m_tab_CurMask & TAB_MASK_CAR_FEMALE)
  {
    if (m_tab_CurMask & TAB_MASK_CAR_MALE)
    {
      m_tabIndex[0] = InsertItem(0,"Caracters female",0);
    }
    else
    {
      m_tabIndex[0] = InsertItem(0,"Caracters",0);
    }
  }
  if (m_tab_CurMask & TAB_MASK_CAR_MALE)
  {
    m_tabIndex[1] = InsertItem(1,"Caracters male",0);
  }
  if (m_tab_CurMask & TAB_MASK_SENSORS)
  {
    m_tabIndex[2] = InsertItem(2,"Sensors",1);
  }
  if (m_tab_CurMask & TAB_MASK_BEHAVIOR)
  {
    m_tabIndex[3] = InsertItem(3,"Behavior",2);
  }

  if (m_tab_CurMask & TAB_MASK_GENERATE)
  {
    m_tabIndex[4] = InsertItem(4,"Generate",3);
  }

  this->RedrawWindow();
}

void CWizardTabCtrl::UpdateTabsWithNewEntity()
{
    if (m_tab_CurMask & TAB_MASK_CAR_FEMALE)
      ((CDialogTab1*)m_tabPages[0])->DisplayParamSliders();

    if (m_tab_CurMask & TAB_MASK_CAR_MALE)
      ((CDialogTab1*)m_tabPages[1])->DisplayParamSliders();
}

