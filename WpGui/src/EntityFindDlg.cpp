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

// EntityFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "EntityFindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityFindDlg dialog


CEntityFindDlg::CEntityFindDlg(CBiotop* pBiotop, CWnd* pParent /*=NULL*/)
	: CDialog(CEntityFindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityFindDlg)
	m_bSelect = FALSE;
	//}}AFX_DATA_INIT

  m_pBiotop = pBiotop;
  m_CurSelInd = -1;
}


void CEntityFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityFindDlg)
	DDX_Control(pDX, IDC_LIST_ENTITY, m_EntityList);
	DDX_Check(pDX, IDC_CHECK1, m_bSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityFindDlg, CDialog)
	//{{AFX_MSG_MAP(CEntityFindDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENTITY, OnClickListEntity)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ENTITY, OnDblclkListEntity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityFindDlg message handlers

BOOL CEntityFindDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CBasicEntity* pCurEnt= NULL;
  CString tempStr;

  m_EntityList.InsertColumn(0, "Name", LVCFMT_LEFT, 200);
  m_EntityList.InsertColumn(1, "Id",   LVCFMT_LEFT, 54);
  
  for (int i=0; i<m_pBiotop->getNbOfEntities(); i++)
  {
    pCurEnt = m_pBiotop->getEntityByIndex(i);
    m_EntityList.InsertItem(i,pCurEnt->getLabel().c_str());
    tempStr.Format("%d",pCurEnt->getId());
    m_EntityList.SetItemText(i, 1, tempStr);
  }
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CEntityFindDlg::GetSelectionIndex(void)
{
  return m_CurSelInd;
}

bool CEntityFindDlg::IsForceSelection(void)
{
  return m_bSelect;
}

void CEntityFindDlg::OnClickListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_EntityList.GetFirstSelectedItemPosition();
	m_CurSelInd = m_EntityList.GetNextSelectedItem(pos);	
	*pResult = 0;
}

void CEntityFindDlg::OnDblclkListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_EntityList.GetFirstSelectedItemPosition();
	m_CurSelInd = m_EntityList.GetNextSelectedItem(pos);	
	*pResult = 0;
  OnOK();
}
