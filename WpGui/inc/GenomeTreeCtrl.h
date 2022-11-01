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

#if !defined(AFX_GENOMETREECTRL_H__26A2CB8A_581E_4605_9AB6_565D3EBBBCD0__INCLUDED_)
#define AFX_GENOMETREECTRL_H__26A2CB8A_581E_4605_9AB6_565D3EBBBCD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenomeTreeCtrl.h : header file
//


typedef enum
{
  TREE_LEVEL_GENOME     = 0,
  TREE_LEVEL_PAIRE      = 1,
  TREE_LEVEL_CHROMOSOME = 2,
  TREE_LEVEL_GENE       = 3,
  TREE_LEVEL_NUMBER_TYPE
} GenomeTreeLevelType_e;


/////////////////////////////////////////////////////////////////////////////
// CGenomeTreeCtrl window

class CGenomeTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CGenomeTreeCtrl();

// Attributes
public:

protected:
  CGenome* m_pGenome;
  CImageList* m_pImageList;

// Operations
public:
  void SetGenome(CGenome* pGenome, bool showChromos = false, bool showGenes = false);

  bool SelectAnyItemFromItsPointer(void* pItem);
  bool UpdateAnyGeneFromItsPointer(CGene* pItem);
  
  GenomeTreeLevelType_e GetSelectionTreeLevel();
  GenomeTreeLevelType_e GetItemTreeLevel(HTREEITEM pItem);

  CGene* GetCurrentMotherAllele();
  CGene* GetCurrentFatherAllele();
  CPairOfChromosome* GetCurrentPairOfChromosome();

  void SelectItemFromString(CString findStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenomeTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGenomeTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGenomeTreeCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENOMETREECTRL_H__26A2CB8A_581E_4605_9AB6_565D3EBBBCD0__INCLUDED_)
