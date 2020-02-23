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
	m_bSelect = TRUE;
	//}}AFX_DATA_INIT

  m_pBiotop = pBiotop;
	m_CurSelId = -1;
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

  m_EntityList.InsertColumn(0, LPCTSTR("Name"), LVCFMT_LEFT, 200);
  m_EntityList.InsertColumn(1, LPCTSTR("Id"),   LVCFMT_LEFT, 54);
  // Limit search to 10000 entities
	int index = 0;
  for (int i=0; i<min(m_pBiotop->getNbOfEntities(), 10000); i++)
  {
    pCurEnt = m_pBiotop->getEntityByIndex(i);
		if (!(pCurEnt->isToBeRemoved()))
		{
			m_EntityList.InsertItem(index, LPCTSTR(pCurEnt->getLabel().c_str()));
			tempStr.Format(LPCTSTR("%d"), pCurEnt->getId());
			m_EntityList.SetItemText(index, 1, tempStr);
			index++;
		}
  }
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CEntityFindDlg::GetSelectionId(void)
{
  return m_CurSelId;
}

bool CEntityFindDlg::IsForceSelection(void)
{
  return m_bSelect;
}

void CEntityFindDlg::OnClickListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_EntityList.GetFirstSelectedItemPosition();
	CString idString = m_EntityList.GetItemText(m_EntityList.GetNextSelectedItem(pos), 1);
	m_CurSelId = atoi((char*)idString.GetBuffer(0));
	*pResult = 0;
}

void CEntityFindDlg::OnDblclkListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClickListEntity(pNMHDR, pResult);
  OnOK();
}
