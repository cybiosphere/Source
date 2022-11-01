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

// GenomeTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CBiotop.h"
#include "GenomeTreeCtrl.h"
#include "resource.h"       // main symbols

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenomeTreeCtrl

CGenomeTreeCtrl::CGenomeTreeCtrl()
{
  m_pImageList = NULL;
}

CGenomeTreeCtrl::~CGenomeTreeCtrl()
{
  if (m_pImageList!=NULL)
    delete m_pImageList;
}

void CGenomeTreeCtrl::SetGenome(CGenome* pGenome, bool showChromos, bool showGenes)
{
  if (m_pGenome != NULL)
    DeleteAllItems( );

  m_pGenome = pGenome;

  if (pGenome == NULL)
    return;

  if (m_pImageList == NULL)
  {
    // create, initialize, and hook up image list
    m_pImageList = new CImageList();
    ASSERT(m_pImageList != NULL);  
    m_pImageList->Create(IDB_GENOME_ICONS,16,16,0x0FFF);
    SetImageList(m_pImageList, TVSIL_NORMAL);
  }

  HTREEITEM hGenomeNode, hPairNode, hChromoNode, hGeneNode;
  CString tmpLabel;
  int nPair,nGene;
  CChromosome* pChromo = NULL;
  hGenomeNode = InsertItem(LPCTSTR(m_pGenome->getSpecieName().c_str()), 0, 1, TVI_ROOT);
  DWORD_PTR addr = (DWORD_PTR)m_pGenome;
  SetItemData(hGenomeNode, addr);

  for (nPair=0; nPair<m_pGenome->getNumPair(); nPair++)
  {
    tmpLabel.Format(LPCTSTR("Pair %d"), m_pGenome->getPair(nPair)->getIdNumber());
    hPairNode = InsertItem(tmpLabel, 2, 3, hGenomeNode);
    SetItemData(hPairNode,(DWORD_PTR)m_pGenome->getPair(nPair));

    hChromoNode = InsertItem(LPCTSTR("mother"), 4, 5, hPairNode);
    if (showChromos)
      EnsureVisible(hChromoNode);
    pChromo = m_pGenome->getPair(nPair)->getMaterChromosome();
    SetItemData(hChromoNode,(DWORD_PTR)pChromo);
    for (nGene=0; nGene<pChromo->getNumGene(); nGene++)
    {
      tmpLabel = CGeneList::getGeneTypeStrName(pChromo->getGene(nGene)->getGeneType()).c_str();
      tmpLabel +=  " ";
      tmpLabel += pChromo->getGene(nGene)->getLabel().c_str();
      hGeneNode = InsertItem(tmpLabel, 6, 7, hChromoNode);
      if (showGenes)
        EnsureVisible(hGeneNode);
      SetItemData(hGeneNode,(DWORD_PTR)pChromo->getGene(nGene));
    }

    hChromoNode = InsertItem(LPCTSTR("father"), 4, 5, hPairNode);
    pChromo = m_pGenome->getPair(nPair)->getPaterChromosome();
    SetItemData(hChromoNode,(DWORD_PTR)pChromo);
    for (nGene=0; nGene<pChromo->getNumGene(); nGene++)
    {
      tmpLabel = CGeneList::getGeneTypeStrName(pChromo->getGene(nGene)->getGeneType()).c_str();
      tmpLabel += " ";
      tmpLabel += pChromo->getGene(nGene)->getLabel().c_str();
      hGeneNode = InsertItem(tmpLabel, 6, 7, hChromoNode);
      if (showGenes)
        EnsureVisible(hGeneNode);
      SetItemData(hGeneNode,(DWORD_PTR)pChromo->getGene(nGene));
    }
  }
}

BEGIN_MESSAGE_MAP(CGenomeTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CGenomeTreeCtrl)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenomeTreeCtrl message handlers

void CGenomeTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
  /*CMenu menu;
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_STRING, 1, "Premiere option");
  menu.AppendMenu(MF_SEPARATOR);
  menu.AppendMenu(MF_STRING | MF_GRAYED, 2, "Deuxieme option");
  ClientToScreen(&point); 
  int Result = menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);*/

 /* CMenu menu; 
  ClientToScreen(&point); 
  menu.LoadMenu(IDR_GENETIC_TYPE); 
  ASSERT (menu.m_hMenu != NULL);
  CMenu* p_popup=menu.GetSubMenu(0); 
  ASSERT (p_popup != NULL);
  int nResult = p_popup->TrackPopupMenu(TPM_LEFTALIGN |  TPM_RIGHTBUTTON, point.x, point.y, this);
  //Agir en fonction du résultat retourné.
  //0 indique que le menu a été annulé
  //Sinon, comparer le resultat avec les identifiants des différentes options
  menu.DestroyMenu();*/

	CTreeCtrl::OnRButtonDown(nFlags, point);
}


bool CGenomeTreeCtrl::SelectAnyItemFromItsPointer(void* pItem)
{

  HTREEITEM curItem = GetRootItem();

  for (UINT i=0; i<GetCount( ); i++)
  {
    if ( (curItem!=NULL) && (GetItemData(curItem)==(DWORD_PTR)pItem) )
    { 
      SelectItem(curItem);
      return (true);
    }
    curItem = GetNextVisibleItem(curItem);
  }

  // Not found
  return (false);
}

bool CGenomeTreeCtrl::UpdateAnyGeneFromItsPointer(CGene* pItem)
{
  CString tmpLabel;
  HTREEITEM curItem = GetRootItem();

  for (UINT i=0; i<GetCount( ); i++)
  {
    if ( (curItem!=NULL) && (GetItemData(curItem)==(DWORD_PTR)pItem) )
    { 
      tmpLabel = CGeneList::getGeneTypeStrName(pItem->getGeneType()).c_str();
      tmpLabel +=  " ";
      tmpLabel += pItem->getLabel().c_str();
      SetItemText(curItem,tmpLabel);
      UpdateWindow();
      return (true);
    }
    curItem = GetNextVisibleItem(curItem);
  }

  // Not found
  return (false);
}

GenomeTreeLevelType_e CGenomeTreeCtrl::GetSelectionTreeLevel()
{
  HTREEITEM pSelectedItem = GetSelectedItem( );
  HTREEITEM pTmpItem = pSelectedItem;
  DWORD_PTR itemPtr = GetItemData(pSelectedItem);

  int level = -1;
  while (pTmpItem != NULL)
  {
    pTmpItem = GetParentItem(pTmpItem);
    level++;
  }
  return ((GenomeTreeLevelType_e)level);
}

GenomeTreeLevelType_e CGenomeTreeCtrl::GetItemTreeLevel(HTREEITEM pItem)
{
  HTREEITEM pTmpItem = pItem;
  int level = -1;
  while (pTmpItem != NULL)
  {
    pTmpItem = GetParentItem(pTmpItem);
    level++;
  }
  return ((GenomeTreeLevelType_e)level);
}

CGene* CGenomeTreeCtrl::GetCurrentMotherAllele()
{
  CGene* foundGene = NULL;

  if (GetSelectionTreeLevel() != TREE_LEVEL_GENE)
    return (foundGene);

  // We are sure selection is a Gene
  HTREEITEM pSelectedItem = GetSelectedItem( );
  HTREEITEM pTmpItem = pSelectedItem;

  CGene* pCurGene = (CGene*)GetItemData(pSelectedItem);

  pTmpItem = GetParentItem(pTmpItem);
  pTmpItem = GetParentItem(pTmpItem);
  CPairOfChromosome* pCurPair = (CPairOfChromosome*)GetItemData(pTmpItem);

  CChromosome* pMotherChromo = pCurPair->getMaterChromosome();
  CChromosome* pFatherChromo = pCurPair->getPaterChromosome();
  CGene* pMotherGene = NULL;
  CGene* pFatherGene = NULL;

  for (int i=0; i<pCurPair->getNumAllele(); i++)
  {
    pMotherGene = pMotherChromo->getGene(i);
    pFatherGene = pFatherChromo->getGene(i);
    
    if ((pMotherGene == pCurGene) || (pFatherGene == pCurGene))
    {
      foundGene = pMotherGene;
    }
  }

  return (foundGene);
}

CGene* CGenomeTreeCtrl::GetCurrentFatherAllele()
{
  CGene* foundGene = NULL;

  if (GetSelectionTreeLevel() != TREE_LEVEL_GENE)
    return (foundGene);

  // We are sure selection is a Gene
  HTREEITEM pSelectedItem = GetSelectedItem( );
  HTREEITEM pTmpItem = pSelectedItem;

  CGene* pCurGene = (CGene*)GetItemData(pSelectedItem);

  pTmpItem = GetParentItem(pTmpItem);
  pTmpItem = GetParentItem(pTmpItem);
  CPairOfChromosome* pCurPair = (CPairOfChromosome*)GetItemData(pTmpItem);

  CChromosome* pMotherChromo = pCurPair->getMaterChromosome();
  CChromosome* pFatherChromo = pCurPair->getPaterChromosome();
  CGene* pMotherGene = NULL;
  CGene* pFatherGene = NULL;

  for (int i=0; i<pCurPair->getNumAllele(); i++)
  {
    pMotherGene = pMotherChromo->getGene(i);
    pFatherGene = pFatherChromo->getGene(i);
    
    if ((pMotherGene == pCurGene) || (pFatherGene == pCurGene))
    {
      foundGene = pFatherGene;
    }
  }

  return (foundGene);
}

CPairOfChromosome* CGenomeTreeCtrl::GetCurrentPairOfChromosome()
{
  CPairOfChromosome* pFoundPair = NULL;

  if (GetSelectionTreeLevel() == TREE_LEVEL_GENOME)
    return (pFoundPair);

  HTREEITEM pSelectedItem = GetSelectedItem();
  HTREEITEM pTmpItem = pSelectedItem;
  if (GetSelectionTreeLevel() == TREE_LEVEL_GENE)
  {
    pTmpItem = GetParentItem(pTmpItem);
    pTmpItem = GetParentItem(pTmpItem);
  }
  else if (GetSelectionTreeLevel() == TREE_LEVEL_CHROMOSOME)
  {
    pTmpItem = GetParentItem(pTmpItem);
  }
  pFoundPair = (CPairOfChromosome*)GetItemData(pTmpItem);
  return (pFoundPair);
}

void CGenomeTreeCtrl::SelectItemFromString(CString findStr)
{
  HTREEITEM curItem = GetSelectedItem();
  (curItem == NULL) ? curItem = GetRootItem() : curItem = GetNextItem(curItem, TVGN_NEXT);

  for (UINT i = 0; i < GetCount(); i++)
  {
    if (GetItemTreeLevel(curItem) == TREE_LEVEL_GENE)
    {
      CGene* pCurGene = (CGene*)GetItemData(curItem);
      if (pCurGene->getLabel().find(findStr.GetBuffer(0)) != std::string::npos)
      {
        SelectItem(curItem);
        break;
      }
      if (GetNextItem(curItem, TVGN_NEXT) != NULL)
      {
        curItem = GetNextItem(curItem, TVGN_NEXT);
      }
      else
      {
        // Go back to Paire level and move next paire
        curItem = GetNextItem(curItem, TVGN_PARENT);
        curItem = GetNextItem(curItem, TVGN_PARENT);
        curItem = GetNextItem(curItem, TVGN_NEXT);
      }
    }
    else
    {
      curItem = GetNextItem(curItem, TVGN_CHILD);
    }
  }
}