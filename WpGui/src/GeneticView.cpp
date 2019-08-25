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

// GeneticView.cpp : implementation of the CGeneticView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "GeneticDoc.h"
#include "GeneticView.h"
#include "GenomeEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGeneticView

IMPLEMENT_DYNCREATE(CGeneticView, CFormView)

BEGIN_MESSAGE_MAP(CGeneticView, CFormView)
	//{{AFX_MSG_MAP(CGeneticView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_EDIT, OnButtonEdit)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneticView construction/destruction

CGeneticView::CGeneticView()
	: CFormView(CGeneticView::IDD)
{
	//{{AFX_DATA_INIT(CGeneticView)
	m_strData1 = _T("");
	m_strData2 = _T("");
	m_strData3 = _T("");
	m_strData4 = _T("");
	m_strLabel1 = _T("");
	m_strLabel2 = _T("");
	m_strLabel3 = _T("");
	m_strLabel4 = _T("");
	//}}AFX_DATA_INIT

  m_pEntity = NULL;;

  m_pCurrentGenome     = NULL;
  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = NULL;
}

CGeneticView::~CGeneticView()
{

}

void CGeneticView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneticView)
	DDX_Control(pDX, IDC_TREE1, m_GenomeTree);
	DDX_Text(pDX, IDC_DATA1, m_strData1);
	DDX_Text(pDX, IDC_DATA2, m_strData2);
	DDX_Text(pDX, IDC_DATA3, m_strData3);
	DDX_Text(pDX, IDC_DATA4, m_strData4);
	DDX_Text(pDX, IDC_LABEL1, m_strLabel1);
	DDX_Text(pDX, IDC_LABEL2, m_strLabel2);
	DDX_Text(pDX, IDC_LABEL3, m_strLabel3);
	DDX_Text(pDX, IDC_LABEL4, m_strLabel4);
	//}}AFX_DATA_MAP
}

BOOL CGeneticView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CGeneticView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CGeneticView printing

BOOL CGeneticView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGeneticView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGeneticView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGeneticView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CGeneticView diagnostics

#ifdef _DEBUG
void CGeneticView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGeneticView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGeneticDoc* CGeneticView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeneticDoc)));
	return (CGeneticDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGeneticView message handlers

/////////////////////////////////////////////////////////////////////////////
// CGeneticView functionnal

bool CGeneticView::SetEntity(CBasicEntity* pEntity)
{
  m_pEntity = pEntity;

  if (pEntity!=NULL)
  {
    m_GenomeTree.SetGenome(m_pEntity->getGenome());
    DisplayGenome(m_pEntity->getGenome());
  }
  else
  {
    m_strData1 = "";
    m_strData2 = "";
    m_strData3 = "";
    m_strData4 = "";
    m_GenomeTree.DeleteAllItems();
    UpdateData(false);
  }
  
  return (true);
}


void CGeneticView::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
  HTREEITEM pSelectedItem = m_GenomeTree.GetSelectedItem( );
  DWORD_PTR itemPtr = m_GenomeTree.GetItemData(pSelectedItem);

  switch(m_GenomeTree.GetSelectionTreeLevel())
  {
  case TREE_LEVEL_GENOME:
    {
      CGenome* pGenome = (CGenome*)itemPtr;
      DisplayGenome(pGenome);
      break;
    }
  case TREE_LEVEL_PAIRE:
    {
      CPairOfChromosome* pPair = (CPairOfChromosome*)itemPtr;
      DisplayPair(pPair);
      break;
    }
  case TREE_LEVEL_CHROMOSOME:
    {
      CChromosome* pChromosome = (CChromosome*)itemPtr;
      DisplayChromosome(pChromosome);
      break;
    }
  case TREE_LEVEL_GENE:
    {
      CGene* pGene = (CGene*)itemPtr;
      DisplayGene(pGene);
      break;
    }
  default:
    {
      // Bad value. Clear all data
      break;
    }
  }

	*pResult = 0;
}

void CGeneticView::DisplayGenome(CGenome* pGenome)
{
  CString tmpStr;

  m_pCurrentGenome     = pGenome;
  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = NULL;

  m_strLabel1 = "Genome name";
  m_strLabel2 = "class";
  m_strLabel3 = "signature";
  m_strLabel4 = "";

  if (pGenome!=NULL)
  {
    m_strData1 = pGenome->getSpecieName().c_str();
    m_strData2 = pGenome->getOwnClassStrName().c_str();
    CString tmpStr;
    tmpStr.Format(LPCTSTR("%d"),pGenome->getSpecieSignature());
    m_strData3 = tmpStr;
    m_strData4 = LPCTSTR("");
  }

  // Update windows
  UpdateData(false);

}

void CGeneticView::DisplayPair(CPairOfChromosome* pPair)
{
  CString tmpStr;

  m_pCurrentGenome     = NULL;
  m_pCurrentPair       = pPair;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = NULL;

  m_strLabel1 = "Chromosome pair";
  m_strLabel2 = "type";
  m_strLabel3 = "";
  m_strLabel4 = "";

  tmpStr.Format(LPCTSTR("id %d"),pPair->getIdNumber());
  m_strData1 = tmpStr;

  switch (pPair->getSex())
  {
  case CHROMOSOME_NEUTRAL:
    m_strData2 = "neutral";
    break;
  case CHROMOSOME_SEX_MALE:
    m_strData2 = "sexual male";
    break;
  case CHROMOSOME_SEX_FEMALE:
    m_strData2 = "sexual female";
    break;
  default:
    m_strData2 = "error";
    break;
  }

  m_strData3 = "";
  m_strData4 = "";

  // Update windows
  UpdateData(false);
}

void CGeneticView::DisplayChromosome(CChromosome* pChromosome)
{
  CString tmpStr;

  m_pCurrentGenome     = NULL;
  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = pChromosome;
  m_pCurrentGene       = NULL;

  m_strLabel1 = "Chromosome name";
  m_strLabel2 = "number";
  m_strLabel3 = "";
  m_strLabel4 = "Raw data";

  m_strData1 = pChromosome->getLabel().c_str();
  tmpStr.Format(LPCTSTR("%d"),pChromosome->getIdNumber());
  m_strData2 = tmpStr;
  m_strData3 = LPCTSTR("");
  m_strData4 = pChromosome->buildStringDataFromGenes().c_str();

  // Update windows
  UpdateData(false);
}

void CGeneticView::DisplayGene(CGene* pGene)
{
  CString tmpStr;

  m_pCurrentGenome     = NULL;
  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = pGene;

  m_strLabel1 = "Gene name";
  m_strLabel2 = "mutation";
  m_strLabel3 = "dominance";
  m_strLabel4 = "infos";

  m_strData1 = pGene->getLabel().c_str();
  m_strData1 += FormatString(" (size=%d)", pGene->getDataLen()).c_str();
  tmpStr.Format(LPCTSTR("type:%d rate:%d"),pGene->getMuteType(),pGene->getMuteRate());
  m_strData2 = tmpStr;
  tmpStr.Format(LPCTSTR("%d"),pGene->getDominanceFactor());
  m_strData3 = tmpStr;
  m_strData4 = m_pEntity->getGeneDescriptionString(pGene).c_str(); //pGene->buildStringDataFromGene();

  // Update windows
  UpdateData(false);
}

void CGeneticView::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CFormView::OnRButtonDown(nFlags, point);
}

void CGeneticView::OnButtonEdit() 
{
  if ( (m_pEntity != NULL) && (m_pEntity->getGenome() != NULL) )
  {
	  CGenomeEditorDlg dlgEdit(m_pEntity);
	  dlgEdit.DoModal();
  }
}

void CGeneticView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if (m_GenomeTree.m_hWnd != NULL)
	{
    int iDpi = GetDpiForWindow(this->m_hWnd);
    m_GenomeTree.MoveWindow(MulDiv(338, iDpi, 96), MulDiv(8, iDpi, 96), cx - MulDiv(344, iDpi, 96), cy - MulDiv(16, iDpi, 96), true);
		UpdateWindow ();
	}
	
}
