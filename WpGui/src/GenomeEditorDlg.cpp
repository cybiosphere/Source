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

// GenomeEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "GenomeEditorDlg.h"
#include "CGeneDefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGenomeEditorDlg dialog


CGenomeEditorDlg::CGenomeEditorDlg(CBasicEntity* pEntity,CWnd* pParent /*=NULL*/)
	: CDialog(CGenomeEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenomeEditorDlg)
	m_SliderM1Title = _T("");
	m_SliderM2Title = _T("");
	m_SliderM3Title = _T("");
	m_SliderM4Title = _T("");
	m_SliderP1Title = _T("");
	m_SliderP2Title = _T("");
	m_SliderP3Title = _T("");
	m_SliderP4Title = _T("");
	m_strDescriptionM = _T("");
	m_strDescriptionP = _T("");
	m_MuteRate = 0;
	m_InitAge = 0;
	//}}AFX_DATA_INIT

  m_pOldEntity = pEntity;
  m_pNewGenome = new CGenome(*pEntity->getGenome());

  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = NULL;

  m_pCurrentEditGeneM  = NULL;
  m_pCurrentEditGeneP  = NULL;

  m_EditMode = EDIT_MODE_INIT;

  m_bIsWeightChanging = false;
}


void CGenomeEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenomeEditorDlg)
	DDX_Control(pDX, IDC_EDIT_LABEL, m_EditLabel);
	DDX_Control(pDX, IDC_LIST_WEIGHT_P, m_WeightListCtrl_P);
	DDX_Control(pDX, IDC_LIST_WEIGHT_M, m_WeightListCtrl_M);
	DDX_Control(pDX, IDC_EDIT_MUTE_RATE, m_MuteRateBox);
	DDX_Control(pDX, IDC_COMBO5, m_ComboMuteType);
	DDX_Control(pDX, IDC_COMBO4, m_ComboRight);
	DDX_Control(pDX, IDC_COMBO3, m_ComboLeft);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
	DDX_Control(pDX, IDC_SLIDER_P4, m_SliderP4);
	DDX_Control(pDX, IDC_SLIDER_P3, m_SliderP3);
	DDX_Control(pDX, IDC_SLIDER_P2, m_SliderP2);
	DDX_Control(pDX, IDC_SLIDER_P1, m_SliderP1);
	DDX_Control(pDX, IDC_SLIDER_M4, m_SliderM4);
	DDX_Control(pDX, IDC_SLIDER_M3, m_SliderM3);
	DDX_Control(pDX, IDC_SLIDER_M2, m_SliderM2);
	DDX_Control(pDX, IDC_SLIDER_M1, m_SliderM1);
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_ButtonAdd);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDC_TREE_GENOME, m_GenomeTreeEdit);
	DDX_Text(pDX, IDC_EDIT_SLIDEM1, m_SliderM1Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEM2, m_SliderM2Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEM3, m_SliderM3Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEM4, m_SliderM4Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEP1, m_SliderP1Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEP2, m_SliderP2Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEP3, m_SliderP3Title);
	DDX_Text(pDX, IDC_EDIT_SLIDEP4, m_SliderP4Title);
	DDX_Text(pDX, IDC_EDIT_DESCM, m_strDescriptionM);
	DDX_Text(pDX, IDC_EDIT_DESCP, m_strDescriptionP);
	DDX_Text(pDX, IDC_EDIT_MUTE_RATE, m_MuteRate);
	DDV_MinMaxInt(pDX, m_MuteRate, 0, 100);
	DDX_Text(pDX, IDC_EDIT_AGE, m_InitAge);
	DDV_MinMaxUInt(pDX, m_InitAge, 0, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenomeEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CGenomeEditorDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_GENOME, OnSelchangedTreeGenome)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_M1, OnReleasedcaptureSliderM1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_M2, OnReleasedcaptureSliderM2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_M3, OnReleasedcaptureSliderM3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_M4, OnReleasedcaptureSliderM4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_P1, OnReleasedcaptureSliderP1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_P2, OnReleasedcaptureSliderP2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_P3, OnReleasedcaptureSliderP3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_P4, OnReleasedcaptureSliderP4)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_SET_BRAIN, OnSetBrainInGenes)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeComboLeft)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeComboRight)
	ON_EN_CHANGE(IDC_EDIT_MUTE_RATE, OnChangeEditMuteRate)
	ON_CBN_SELCHANGE(IDC_COMBO5, OnSelchangeComboMuteType)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_WEIGHT_M, OnEndlabeleditListWeightM)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_WEIGHT_P, OnEndlabeleditListWeightP)
	ON_EN_CHANGE(IDC_EDIT_LABEL, OnChangeEditLabel)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_WEIGHT_M, OnItemchangingListWeightM)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_WEIGHT_P, OnItemchangingListWeightP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenomeEditorDlg message handlers

BOOL CGenomeEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  m_ButtonAdd.ShowWindow(SW_HIDE);
  m_ComboLeft.ShowWindow(SW_HIDE);
  m_ComboRight.ShowWindow(SW_HIDE);
  m_Combo1.ShowWindow(SW_HIDE);
  m_Combo2.ShowWindow(SW_HIDE);
  m_ComboMuteType.ShowWindow(SW_HIDE);
  m_MuteRateBox.ShowWindow(SW_HIDE);
  m_EditLabel.ShowWindow(SW_HIDE);

  m_GenomeTreeEdit.SetGenome(m_pNewGenome,true);
  m_GenomeTreeEdit.SelectAnyItemFromItsPointer((void*)m_pNewGenome);
  
  HICON saveIco = theApp.LoadIcon(IDI_ICON_SAVE); 
  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN); 
  m_ButtonSave.SetIcon(saveIco);
  m_ButtonLoad.SetIcon(loadIco);

  m_WeightListCtrl_M.SetSensor(NULL);
  m_WeightListCtrl_P.SetSensor(NULL);

  m_InitAge = m_pOldEntity->getAge();
  UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGenomeEditorDlg::OnCancel() 
{

	if (m_pNewGenome!=NULL)
    delete m_pNewGenome;
 
	CDialog::OnCancel();
}

void CGenomeEditorDlg::OnOK() 
{
  UpdateData(true);

  if (m_pNewGenome!=NULL)
  {
    entityIdType oldId = m_pOldEntity->getId();
    theApp.GetBiotop()->resetEntityGenome(oldId, m_pNewGenome);
    CBasicEntity* pNewEntity = theApp.GetBiotop()->getEntityById(oldId);

    pNewEntity->quickAgeing(m_InitAge);

    theApp.setSelectedEntity(NULL); // Clear all
    theApp.updateSelectedEntity(pNewEntity);
  } 

	CDialog::OnOK();
}

void CGenomeEditorDlg::OnSelchangedTreeGenome(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

  HTREEITEM pSelectedItem = m_GenomeTreeEdit.GetSelectedItem( );
  DWORD_PTR itemPtr = m_GenomeTreeEdit.GetItemData(pSelectedItem);

  m_bIsWeightChanging = false;	

  switch(m_GenomeTreeEdit.GetSelectionTreeLevel())
  {
  case TREE_LEVEL_GENOME:
    {
      CGenome* pGenome = (CGenome*)itemPtr;
      DisplayGenome(pGenome);
      m_EditMode = EDIT_MODE_GENOME;
      break;
    }
  case TREE_LEVEL_PAIRE:
    {
      CPairOfChromosome* pPair = (CPairOfChromosome*)itemPtr;
      DisplayPair(pPair);
      m_EditMode = EDIT_MODE_PAIRE;
      break;
    }
  case TREE_LEVEL_CHROMOSOME:
    {
      CChromosome* pChromosome = (CChromosome*)itemPtr;
      DisplayChromosome(pChromosome);
      m_EditMode = EDIT_MODE_CHROMO;
      break;
    }
  case TREE_LEVEL_GENE:
    {
      Sleep(100);
      CGene* pGene = (CGene*)itemPtr;
      DisplayGene(pGene);
      m_EditMode = EDIT_MODE_GENE;

      m_pCurrentEditGeneM = m_GenomeTreeEdit.GetCurrentMotherAllele();
      m_pCurrentEditGeneP = m_GenomeTreeEdit.GetCurrentFatherAllele();
      EditGenePair();

      break;
    }
  default:
    {
      // Bad value. Clear all data
      m_EditMode = EDIT_MODE_INIT;
      break;
    }
  }

	*pResult = 0;
}

void CGenomeEditorDlg::DisplayGenome(CGenome* pGenome)
{
  CString tmpStr;

  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = NULL;

  m_ButtonAdd.SetWindowText(LPCTSTR("Add paire"));
  m_ButtonAdd.ShowWindow(SW_SHOW);  
  m_Combo1.ShowWindow(SW_HIDE);
  m_Combo2.ShowWindow(SW_HIDE);
  m_ComboLeft.ShowWindow(SW_HIDE);
  m_ComboRight.ShowWindow(SW_HIDE);
  m_ComboMuteType.ShowWindow(SW_HIDE);
  m_MuteRateBox.ShowWindow(SW_HIDE);
  m_WeightListCtrl_M.ShowWindow(SW_HIDE);
  m_WeightListCtrl_P.ShowWindow(SW_HIDE);
  m_EditLabel.ShowWindow(SW_HIDE);

  ClearAndHideAllSliders();
	m_strDescriptionM = "";
	m_strDescriptionP = "";

  // Update windows
  UpdateData(false);

}

void CGenomeEditorDlg::DisplayPair(CPairOfChromosome* pPair)
{
  CString tmpStr;

  m_pCurrentPair       = pPair;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = NULL;

  m_ButtonAdd.SetWindowText(LPCTSTR("Add Gene"));
  m_ButtonAdd.ShowWindow(SW_SHOW);
  m_Combo1.ShowWindow(SW_SHOW);
  m_Combo2.ShowWindow(SW_HIDE);
  m_ComboLeft.ShowWindow(SW_HIDE);
  m_ComboRight.ShowWindow(SW_HIDE);
  m_ComboMuteType.ShowWindow(SW_HIDE);
  m_MuteRateBox.ShowWindow(SW_HIDE);
  m_WeightListCtrl_M.ShowWindow(SW_HIDE);
  m_WeightListCtrl_P.ShowWindow(SW_HIDE);
  m_EditLabel.ShowWindow(SW_HIDE);

  ClearAndHideAllSliders();
	m_strDescriptionM = "";
	m_strDescriptionP = "";

  int index;
  int indexInitSel = 0;
	m_Combo1.ResetContent();

  index = m_Combo1.AddString(LPCTSTR("neutral"));
  m_Combo1.SetItemData(index,(DWORD_PTR)CHROMOSOME_NEUTRAL);
  index = m_Combo1.AddString(LPCTSTR("sexual male"));
  m_Combo1.SetItemData(index,(DWORD_PTR)CHROMOSOME_SEX_MALE);
  index = m_Combo1.AddString(LPCTSTR("sexual female"));
  m_Combo1.SetItemData(index,(DWORD_PTR)CHROMOSOME_SEX_FEMALE);

  m_Combo1.SetCurSel(m_pCurrentPair->getSex());

  // Update windows
  UpdateData(false);
}

void CGenomeEditorDlg::DisplayChromosome(CChromosome* pChromosome)
{
  CString tmpStr;

  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = pChromosome;
  m_pCurrentGene       = NULL;

  //m_ButtonAdd.ShowWindow(SW_HIDE);
  m_ButtonAdd.SetWindowText(LPCTSTR("Add Gene"));
  m_ButtonAdd.ShowWindow(SW_SHOW);
  m_Combo1.ShowWindow(SW_HIDE);
  m_Combo2.ShowWindow(SW_HIDE);
  m_ComboLeft.ShowWindow(SW_HIDE);
  m_ComboRight.ShowWindow(SW_HIDE);
  m_ComboMuteType.ShowWindow(SW_HIDE);
  m_MuteRateBox.ShowWindow(SW_HIDE);
  m_WeightListCtrl_M.ShowWindow(SW_HIDE);
  m_WeightListCtrl_P.ShowWindow(SW_HIDE);
  m_EditLabel.ShowWindow(SW_HIDE);

  ClearAndHideAllSliders();
	m_strDescriptionM = "";
	m_strDescriptionP = "";

  // Update windows
  UpdateData(false);
}

void CGenomeEditorDlg::DisplayGene(CGene* pGene)
{
  CString tmpStr;

  m_pCurrentPair       = NULL;
  m_pCurrentChromosome = NULL;
  m_pCurrentGene       = pGene;

  m_ButtonAdd.ShowWindow(SW_HIDE);
  m_Combo1.ShowWindow(SW_SHOW);
  m_Combo2.ShowWindow(SW_SHOW);
  m_ComboLeft.ShowWindow(SW_HIDE);
  m_ComboRight.ShowWindow(SW_HIDE);
  m_ComboMuteType.ShowWindow(SW_SHOW);
  m_MuteRateBox.ShowWindow(SW_SHOW);
  m_EditLabel.ShowWindow(SW_HIDE);
  // Update windows
  UpdateData(false);
}

void CGenomeEditorDlg::ClearAndHideAllSliders()
{
  m_SliderM1.ShowWindow(SW_HIDE); 
  m_SliderP1.ShowWindow(SW_HIDE); 
  m_SliderM2.ShowWindow(SW_HIDE); 
  m_SliderP2.ShowWindow(SW_HIDE); 
  m_SliderM3.ShowWindow(SW_HIDE); 
  m_SliderP3.ShowWindow(SW_HIDE); 
  m_SliderM4.ShowWindow(SW_HIDE); 
  m_SliderP4.ShowWindow(SW_HIDE);
	m_SliderM1Title = "";
	m_SliderM2Title = "";
	m_SliderM3Title = "";
	m_SliderM4Title = "";
	m_SliderP1Title = "";
	m_SliderP2Title = "";
	m_SliderP3Title = "";
	m_SliderP4Title = "";
  // Update windows
  UpdateData(false);
}


void CGenomeEditorDlg::OnButtonAdd() 
{
	if (m_EditMode == EDIT_MODE_GENOME)
  {
    int index = m_pNewGenome->addPair();
    CPairOfChromosome* pPaire = m_pNewGenome->getPair(index);

    if (pPaire!=NULL)
    {
      m_GenomeTreeEdit.SetGenome(m_pNewGenome,true);
      m_GenomeTreeEdit.SelectAnyItemFromItsPointer((void*)pPaire);
    }
  }
	else if (m_EditMode == EDIT_MODE_PAIRE)
  {
    int indexM = m_pCurrentPair->getMaterChromosome()->addGene();
    int indexP = m_pCurrentPair->getPaterChromosome()->addGene();
    
    CGene* pGeneM = m_pCurrentPair->getMaterChromosome()->getGene(indexM);
    CGene* pGeneP = m_pCurrentPair->getPaterChromosome()->getGene(indexP);
    
    if ((pGeneP!=NULL)&&(pGeneM!=NULL))
    {    
      m_GenomeTreeEdit.SetGenome(m_pNewGenome,true,true);
      m_GenomeTreeEdit.SelectAnyItemFromItsPointer((void*)pGeneM);

      m_pCurrentEditGeneM = pGeneM;
      m_pCurrentEditGeneP = pGeneP;

      EditGenePair();
    }
  }
	else if (m_EditMode == EDIT_MODE_CHROMO)
  {
    CPairOfChromosome* pCurrentPair = m_pNewGenome->getPair(m_pCurrentChromosome->getIdNumber()); 

    int indexM = pCurrentPair->getMaterChromosome()->addGene();
    int indexP = pCurrentPair->getPaterChromosome()->addGene();
    
    CGene* pGeneM = pCurrentPair->getMaterChromosome()->getGene(indexM);
    CGene* pGeneP = pCurrentPair->getPaterChromosome()->getGene(indexP);
    
    if ((pGeneP!=NULL)&&(pGeneM!=NULL))
    {    
      m_GenomeTreeEdit.SetGenome(m_pNewGenome,true,true);
      m_GenomeTreeEdit.SelectAnyItemFromItsPointer((void*)pGeneM);

      m_pCurrentEditGeneM = pGeneM;
      m_pCurrentEditGeneP = pGeneP;

      EditGenePair();
    }
  }
}


bool CGenomeEditorDlg::EditGenePair()
{
  int index;
  int indexInitSel = 0;
	m_Combo1.ResetContent();
  for (GeneType_e i=GENE_GENERIC; i<GENE_NUMBER_TYPE; i=(GeneType_e)(i+1))
  {
    index = m_Combo1.AddString(LPCTSTR(CGeneList::getGeneTypeStrName(i).c_str()));
    m_Combo1.SetItemData(index,(DWORD_PTR)i);
    if (m_pCurrentEditGeneM->getGeneType() == i)
      indexInitSel = index;
  }
  m_Combo1.SetCurSel(indexInitSel);

  indexInitSel = 0;
	m_ComboMuteType.ResetContent();
  for (GeneMuteType_e j=GENE_MUTE_RANDOM_BIT; j<GENE_MUTE_NUMBER_TYPE; j=(GeneMuteType_e)(j+1))
  {
    index = m_ComboMuteType.AddString(LPCTSTR(CGeneList::getGeneMuteTypeStrName(j).c_str()));
    m_ComboMuteType.SetItemData(index,(DWORD_PTR)j);
    if (m_pCurrentEditGeneM->getMuteType() == j)
      indexInitSel = index;
  }
  m_ComboMuteType.SetCurSel(indexInitSel);

  m_MuteRate = m_pCurrentEditGeneM->getMuteRate();
  UpdateData(false);

  UpdateCombo1(false);  // Force other combo refresh

  m_EditMode = EDIT_MODE_GENE;

  return (true);
}

void CGenomeEditorDlg::OnSelchangeCombo1() 
{
  UpdateCombo1(true);
}

void CGenomeEditorDlg::UpdateCombo1(bool rebuildGene) 
{
  int index;
  int indexInitSel = 0;
	m_Combo2.ResetContent();

	if (m_EditMode == EDIT_MODE_GENE)
  {
    GeneType_e geneType = (GeneType_e)m_Combo1.GetItemData(m_Combo1.GetCurSel());
    
    for (size_t idx = 0; idx < CGeneList::getNumberOfGeneSubtype(geneType); idx++)
    { 
      const CGeneDefinitions* pDef = CGeneList::getDefinitionsByIndex(geneType, idx);
      index = m_Combo2.AddString(pDef->label.c_str());
      m_Combo2.SetItemData(index, (DWORD_PTR)pDef->geneSubType);
      if (m_pCurrentEditGeneM->getGeneSubType() == pDef->geneSubType)
        indexInitSel = index;
    }

    if ((geneType==GENE_SENSOR)||(geneType==GENE_FEELING))
    {
       m_WeightListCtrl_M.ShowWindow(SW_SHOW);
       m_WeightListCtrl_P.ShowWindow(SW_SHOW);
    }
    else
    {
      m_WeightListCtrl_M.SetSensor(NULL);
      m_WeightListCtrl_M.ShowWindow(SW_HIDE);
      m_WeightListCtrl_P.SetSensor(NULL);
      m_WeightListCtrl_P.ShowWindow(SW_HIDE);
    }

    m_Combo2.SetCurSel(indexInitSel);
    UpdateCombo2(rebuildGene);  // Force other combo refresh
  }
	else if (m_EditMode == EDIT_MODE_PAIRE)
  {
    ChromosomeType_e sexType = (ChromosomeType_e)m_Combo1.GetItemData(m_Combo1.GetCurSel());
    switch(sexType)
    {
    case CHROMOSOME_SEX_MALE: 
      m_pNewGenome->setSexMale(m_pCurrentPair->getIdNumber());
      break;
    case CHROMOSOME_SEX_FEMALE: 
      m_pNewGenome->setSexFemale(m_pCurrentPair->getIdNumber());
      break;
    default:
      m_pNewGenome->setSexNeutral(m_pCurrentPair->getIdNumber());
      break;
    }
  }
}

void CGenomeEditorDlg::OnSelchangeCombo2() 
{
  UpdateCombo2(true);
}

void CGenomeEditorDlg::UpdateCombo2(bool rebuildGene) 
{
  int i;
  m_ComboLeft.ShowWindow(SW_HIDE);
  m_ComboRight.ShowWindow(SW_HIDE);
  m_EditLabel.ShowWindow(SW_HIDE);
  CString strEditLabel = LPCTSTR("");

	if (m_EditMode == EDIT_MODE_GENE)
  {
    GeneType_e geneType = (GeneType_e)m_Combo1.GetItemData(m_Combo1.GetCurSel());
    GeneSubType_e geneSubType = (GeneSubType_e)m_Combo2.GetItemData(m_Combo2.GetCurSel());
    
    double scaleData1{ 0 }, scaleData2{ 0 }, scaleData3{ 0 }, scaleData4{ 0 };
    size_t numParam = m_pCurrentEditGeneM->getNumParameter();
    if (numParam > 0) scaleData1 = m_pCurrentEditGeneM->getParameterScale(0);
    if (numParam > 1) scaleData2 = m_pCurrentEditGeneM->getParameterScale(1);
    if (numParam > 2) scaleData3 = m_pCurrentEditGeneM->getParameterScale(2);
    if (numParam > 3) scaleData4 = m_pCurrentEditGeneM->getParameterScale(3);

    long range    = 1;
    long pageSize = 1;
    double ticDiv = 1.0;
    long initSlideM[4] = {0,0,0,0};
    long initSlideP[4] = {0,0,0,0};

    auto rawDataM = m_pCurrentEditGeneM->getData();
    BYTE* pDataM = rawDataM.data();
    int dataLenM = rawDataM.size();
    auto rawDataP = m_pCurrentEditGeneP->getData();
    BYTE* pDataP = rawDataP.data();
    int dataLenP = rawDataP.size();

    switch (geneType)
    {
    case GENE_CARACTER:
    case GENE_BRAIN_CONFIG:
      {
        range    = 255;
        pageSize = 1;
        if (geneSubType == GENE_CARACT_COLOR)
          ticDiv   = 10.0;
        else
          ticDiv   = 1.0;
        // Init slide
        for (i=0; ((i<dataLenM) && (i<4)); i++)
          initSlideM[i] = pDataM[i];
        for (i=0; ((i<dataLenP) && (i<4)); i++)
          initSlideP[i] = pDataP[i];

        break;
      }
    case GENE_PARAMETER:
    case GENE_LIFESTAGE:
    case GENE_PHY_WELFARE:
    case GENE_SENSOR:
    case GENE_REACTION:
      {
        range    = 1000;
        pageSize = 1;
        ticDiv   = 100.0;
        // Init slide
        for (i=0; ((i<dataLenM/(int)sizeof(WORD)) && (i<4)); i++)
          initSlideM[i] = ((WORD*)pDataM)[i] * 1000 / 0xFFFF;
        for (i=0; ((i<dataLenP/(int)sizeof(WORD)) && (i<4)); i++)
          initSlideP[i] = ((WORD*)pDataP)[i] * 1000 / 0xFFFF;

        if (geneType == GENE_SENSOR)
        {
          CSensor* pSens = m_pOldEntity->getTemporarySensorFromGene(m_pCurrentEditGeneM);
          m_WeightListCtrl_M.SetSensor(pSens, ((WORD*)pDataM + 4));
          m_WeightListCtrl_P.SetSensor(pSens, ((WORD*)pDataP + 4));
        }

        break;
      }
    case GENE_BRAIN_SIZE:
      {
        range    = 100;
        pageSize = 1;
        ticDiv   = 10.0;
        // Init slide
        for (i=0; ((i<dataLenM/(int)sizeof(WORD)) && (i<4)); i++)
          initSlideM[i] = ((WORD*)pDataM)[i] * 100 / 0xFFFF;
        for (i=0; ((i<dataLenP/(int)sizeof(WORD)) && (i<4)); i++)
          initSlideP[i] = ((WORD*)pDataP)[i] * 100 / 0xFFFF;

        break;
      }
    case GENE_FEELING:
      {
	      m_ComboLeft.ResetContent();
        CBrain* pBrain = m_pOldEntity->getBrain();
        int indexInitSel = 0;

        if (pBrain != NULL)
        {
          // Get associated Sensor
          DWORD* pSensUid = (DWORD*)pDataM;
          CSensor* pSens = NULL;
          if (pSensUid != NULL)
            pSens = pBrain->GetSensorByUniqueId(*pSensUid);

          for (int i=0; i<pBrain->GetNumberSensor(); i++)
          {
            int index = m_ComboLeft.AddString(LPCTSTR(pBrain->GetSensorByIndex(i)->GetLabel().c_str()));
            m_ComboLeft.SetItemData(index,(DWORD_PTR)pBrain->GetSensorByIndex(i));

            if (pBrain->GetSensorByIndex(i) == pSens)
              indexInitSel = i;
          }
          m_ComboLeft.SetCurSel(indexInitSel);
          m_ComboLeft.ShowWindow(SW_SHOW);

          m_WeightListCtrl_M.SetSensor(pSens, ((WORD*)pDataM + 2));
          m_WeightListCtrl_P.SetSensor(pSens, ((WORD*)pDataP + 2));
        }
        break;
      }
    case GENE_PURPOSE:
      {
        range    = 1000;
        pageSize = 1;
        ticDiv   = 100.0;

	      m_ComboLeft.ResetContent();
        m_ComboRight.ResetContent();
        CBrain* pBrain = m_pOldEntity->getBrain();
        int indexInitSel = 0;

        if ( (geneSubType==GENE_PURPOSE_TRIGGER_UP || geneSubType==GENE_PURPOSE_TRIGGER_DOWN)
          && (pBrain != NULL) )
        {
          // Init slide
		      if (dataLenM>5)
          {
			      initSlideM[0] = ((WORD*)pDataM)[0] * 1000 / 0xFFFF;
			      initSlideM[1] = ((WORD*)pDataM)[1] * 1000 / 0xFFFF;
			      initSlideM[2] = ((WORD*)pDataM)[2] * 1000 / 0xFFFF;
			      initSlideM[3] = ((WORD*)pDataM)[3] * 1000 / 0xFFFF;
		      }
		      else
          {
			      initSlideM[0] = 0;
			      initSlideM[1] = 0;
			      initSlideM[2] = 0;
			      initSlideM[3] = 0;
		      }

		      if (dataLenP>5)
          {
            initSlideP[0] = ((WORD*)pDataP)[0] * 1000 / 0xFFFF;
            initSlideP[1] = ((WORD*)pDataP)[1] * 1000 / 0xFFFF;
            initSlideP[2] = ((WORD*)pDataP)[2] * 1000 / 0xFFFF;
            initSlideP[3] = ((WORD*)pDataP)[3] * 1000 / 0xFFFF;
		      }
		      else
          {
			      initSlideP[0] = 0;
			      initSlideP[1] = 0;
			      initSlideP[2] = 0;
			      initSlideP[3] = 0;
		      }

          // Get associated Sensor
          DWORD* pSensUid = NULL;
		      if (dataLenM>9)
            pSensUid = ((DWORD*)pDataM)+2;

          CSensor* pSens = NULL;
          if (pSensUid != NULL)
            pSens = pBrain->GetSensorByUniqueId(*pSensUid);

          for (int i=0; i<pBrain->GetNumberSensor(); i++)
          {
            int index = m_ComboLeft.AddString(LPCTSTR(pBrain->GetSensorByIndex(i)->GetLabel().c_str()));
            m_ComboLeft.SetItemData(index,(DWORD_PTR)pBrain->GetSensorByIndex(i));

            if (pBrain->GetSensorByIndex(i) == pSens)
              indexInitSel = i;
          }
          m_ComboLeft.SetCurSel(indexInitSel);
          m_ComboLeft.ShowWindow(SW_SHOW);
          m_EditLabel.ShowWindow(SW_SHOW);
          if (dataLenM>12)
          {
             //m_EditLabel.SetWindowText((char*)pDataM+8);
            strEditLabel = ((char*)pDataM+12);
          }
        }
        else if ( (geneSubType==GENE_PURPOSE_SENSOR) && (pBrain != NULL) )       
        {
          /*initSlideM[0] = ((WORD*)pDataM)[4] * scaleData1;
          initSlideP[0] = ((WORD*)pDataP)[4] * scaleData1;
          initSlideM[1] = ((WORD*)pDataM)[5] * scaleData2;
          initSlideP[1] = ((WORD*)pDataP)[5] * scaleData2;*/
          // Get associated Sensor
          DWORD* pPurposeUid = (DWORD*)pDataM;
          CPurpose* pPurpose = NULL;
          if (pPurposeUid != NULL)
            pPurpose = pBrain->GetPurposeByUniqueId(*pPurposeUid);

          for (int i=0; i<pBrain->GetNumberPurpose(); i++)
          {
            int index = m_ComboLeft.AddString(LPCTSTR(pBrain->GetPurposeByIndex(i)->GetLabel().c_str()));
            m_ComboLeft.SetItemData(index,(DWORD_PTR)pBrain->GetPurposeByIndex(i));

            if (pBrain->GetPurposeByIndex(i) == pPurpose)
              indexInitSel = i;
          }
          m_ComboLeft.SetCurSel(indexInitSel);
          m_ComboLeft.ShowWindow(SW_SHOW);

          CSensor* pSensor = NULL;
          if (pPurposeUid != NULL)
            pSensor = pBrain->GetSensorByUniqueId(*(pPurposeUid+1));

          for (i=0; i<pBrain->GetNumberSensor(); i++)
          {
            int index = m_ComboRight.AddString(LPCTSTR(pBrain->GetSensorByIndex(i)->GetLabel().c_str()));
            m_ComboRight.SetItemData(index,(DWORD_PTR)pBrain->GetSensorByIndex(i));

            if (pBrain->GetSensorByIndex(i) == pSensor)
              indexInitSel = i;
          }
          m_ComboRight.SetCurSel(indexInitSel);
          m_ComboRight.ShowWindow(SW_SHOW);

          m_WeightListCtrl_M.ShowWindow(SW_SHOW);
          m_WeightListCtrl_P.ShowWindow(SW_SHOW);

          CSensor* pSens = (CSensor*)m_ComboRight.GetItemData(indexInitSel);
          m_WeightListCtrl_M.SetSensor(pSens, ((WORD*)pDataM + 4), false);
          m_WeightListCtrl_P.SetSensor(pSens, ((WORD*)pDataP + 4), false);
        }
        else if ( (geneSubType==GENE_PURPOSE_REACTION) && (pBrain != NULL) )       
        {
          range         = 2000;
          initSlideM[0] = ((WORD*)pDataM)[4] * 2000 / 0xFFFF;
          initSlideP[0] = ((WORD*)pDataP)[4] * 2000 / 0xFFFF;

          // Get associated Sensor
          DWORD* pPurposeUid = (DWORD*)pDataM;
          CPurpose* pPurpose = NULL;
          if (pPurposeUid != NULL)
            pPurpose = pBrain->GetPurposeByUniqueId(*pPurposeUid);

          for (int i=0; i<pBrain->GetNumberPurpose(); i++)
          {
            int index = m_ComboLeft.AddString(LPCTSTR(pBrain->GetPurposeByIndex(i)->GetLabel().c_str()));
            m_ComboLeft.SetItemData(index,(DWORD_PTR)pBrain->GetPurposeByIndex(i));

            if (pBrain->GetPurposeByIndex(i) == pPurpose)
              indexInitSel = i;
          }
          m_ComboLeft.SetCurSel(indexInitSel);
          m_ComboLeft.ShowWindow(SW_SHOW);

          CReaction* pReaction = NULL;
          if (pPurposeUid != NULL)
            pReaction = pBrain->GetReactionByUniqueId(*(pPurposeUid+1));

          for (i=0; i<pBrain->GetNumberReaction(); i++)
          {
            int index = m_ComboRight.AddString(LPCTSTR(pBrain->GetReactionByIndex(i)->GetLabel().c_str()));
            m_ComboRight.SetItemData(index,(DWORD_PTR)pBrain->GetReactionByIndex(i));

            if (pBrain->GetReactionByIndex(i) == pReaction)
              indexInitSel = i;
          }
          m_ComboRight.SetCurSel(indexInitSel);
          m_ComboRight.ShowWindow(SW_SHOW);

        }
        break;
      }

    // FRED: TBC
    default:

      break;
    }

    m_SliderM1.SetRange(0,range);
    m_SliderM1.SetPos(range - initSlideM[0]);
    m_SliderM1.SetPageSize(pageSize);
    m_SliderP1.SetRange(0,range);
    m_SliderP1.SetPos(range - initSlideP[0]);
    m_SliderP1.SetPageSize(pageSize);
    if ((numParam > 0) && m_pCurrentEditGeneM->getParameterIsConfigurable(0))
    { 
      m_SliderM1.SetTicFreq((long)floor(ticDiv/scaleData1));
      m_SliderM1.ShowWindow(true);     
      m_SliderP1.SetTicFreq((long)floor(ticDiv/scaleData1));
      m_SliderP1.ShowWindow(true);
      m_SliderM1Title = m_pCurrentEditGeneM->getParameterStrName(0).c_str();
      m_SliderP1Title = m_pCurrentEditGeneP->getParameterStrName(0).c_str();
    }
    else
    {
      m_SliderM1.ShowWindow(false);
      m_SliderP1.ShowWindow(false);
      m_SliderM1Title = "";
      m_SliderP1Title = "";
    }

    m_SliderM2.SetRange(0,range);
    m_SliderM2.SetPos(range - initSlideM[1]);
    m_SliderM2.SetPageSize(pageSize);
    m_SliderP2.SetRange(0,range);
    m_SliderP2.SetPos(range - initSlideP[1]);
    m_SliderP2.SetPageSize(pageSize);
    if ((numParam > 1) && m_pCurrentEditGeneM->getParameterIsConfigurable(1))
    {
      m_SliderM2.SetTicFreq((long)floor(ticDiv/scaleData2));
      m_SliderM2.ShowWindow(true);
      m_SliderP2.SetTicFreq((long)floor(ticDiv/scaleData2));
      m_SliderP2.ShowWindow(true);
      m_SliderM2Title = m_pCurrentEditGeneM->getParameterStrName(1).c_str();
      m_SliderP2Title = m_pCurrentEditGeneP->getParameterStrName(1).c_str();
    }
    else
    {
      m_SliderM2.ShowWindow(false);
      m_SliderP2.ShowWindow(false);
      m_SliderM2Title = "";
      m_SliderP2Title = "";
    }

    m_SliderM3.SetRange(0,range);
    m_SliderM3.SetPos(range - initSlideM[2]);
    m_SliderM3.SetPageSize(pageSize);
    m_SliderP3.SetRange(0,range);
    m_SliderP3.SetPos(range - initSlideP[2]);
    m_SliderP3.SetPageSize(pageSize);
    if ((numParam > 2) && m_pCurrentEditGeneM->getParameterIsConfigurable(2))
    {
      m_SliderM3.SetTicFreq((long)floor(ticDiv/scaleData3));
      m_SliderM3.ShowWindow(true);
      m_SliderP3.SetTicFreq((long)floor(ticDiv/scaleData3));
      m_SliderP3.ShowWindow(true);
      m_SliderM3Title = m_pCurrentEditGeneM->getParameterStrName(2).c_str();
      m_SliderP3Title = m_pCurrentEditGeneP->getParameterStrName(2).c_str();
    }
    else
    {
      m_SliderM3.ShowWindow(false);
      m_SliderP3.ShowWindow(false);
      m_SliderM3Title = "";
      m_SliderP3Title = "";
    }

    m_SliderM4.SetRange(0,range);
    m_SliderM4.SetPos(range - initSlideM[3]);
    m_SliderM4.SetPageSize(pageSize);
    m_SliderP4.SetRange(0,range);
    m_SliderP4.SetPos(range - initSlideP[3]);
    m_SliderP4.SetPageSize(pageSize);
    if ((numParam > 3) && m_pCurrentEditGeneM->getParameterIsConfigurable(3))
    {
      m_SliderM4.SetTicFreq((long)floor(ticDiv/scaleData4));
      m_SliderM4.ShowWindow(true);
      m_SliderP4.SetTicFreq((long)floor(ticDiv/scaleData4));
      m_SliderP4.ShowWindow(true);
      m_SliderM4Title = m_pCurrentEditGeneM->getParameterStrName(3).c_str();
      m_SliderP4Title = m_pCurrentEditGeneM->getParameterStrName(3).c_str();
    }
    else
    {
      m_SliderM4.ShowWindow(false);
      m_SliderP4.ShowWindow(false);
      m_SliderM4Title = "";
      m_SliderP4Title = "";
    }

    if (geneSubType==GENE_PURPOSE_TRIGGER_UP || geneSubType==GENE_PURPOSE_TRIGGER_DOWN)
    {
      UpdateData(false); // update slider names
      m_EditLabel.SetWindowText(strEditLabel);
    }

    // Refresh 
    RefreshCurrentGeneM(rebuildGene);
    RefreshCurrentGeneP(rebuildGene);
  }
}

bool CGenomeEditorDlg::RefreshCurrentGeneM(bool rebuildGene, bool resetWeightTable)
{
  if (m_pCurrentEditGeneM==NULL)
    return(false);

	if ((m_EditMode == EDIT_MODE_GENE)&&(rebuildGene))
  {
    GeneType_e geneType = (GeneType_e)m_Combo1.GetItemData(m_Combo1.GetCurSel());
    GeneSubType_e geneSubType = (GeneSubType_e)m_Combo2.GetItemData(m_Combo2.GetCurSel());
    GeneMuteType_e muteType = (GeneMuteType_e)m_ComboMuteType.GetItemData(m_ComboMuteType.GetCurSel());
    int i;

    switch(geneType)
    {
    case GENE_CARACTER:
      unsigned char data[4];
      data[0] = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
      data[1] = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); 
      data[2] = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos(); 
      data[3] = m_SliderM4.GetRangeMax() - m_SliderM4.GetPos(); 
      m_pCurrentEditGeneM->setAsCaracter(geneSubType,m_MuteRate,muteType,4,data);
      break;
    case GENE_PARAMETER:
      long min,nominal,max;
      min     = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
      nominal = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); 
      max     = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos(); 
      m_pCurrentEditGeneM->setAsParameter(geneSubType,m_MuteRate,min,nominal,max);
      break;
    case GENE_LIFESTAGE:
      long ratio;
      ratio   = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
      m_pCurrentEditGeneM->setAsLifeStage(geneSubType,m_MuteRate,ratio);
      break;
    case GENE_PHY_WELFARE:
      long sensi;
      sensi   = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
      min     = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); // Reverse to have min at bottom!
      nominal = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos(); 
      max     = m_SliderM4.GetRangeMax() - m_SliderM4.GetPos(); 
      m_pCurrentEditGeneM->setAsPhysicWelfare(geneSubType,m_MuteRate,sensi,min,nominal,max);
      break;
    case GENE_SENSOR:
      {
        long data1,data2,data3,data4;
        data1 = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
        data2 = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); 
        data3 = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos();
        data4 = m_SliderM4.GetRangeMax() - m_SliderM4.GetPos(); 

        if (resetWeightTable == true)
        {
          m_pCurrentEditGeneM->setAsSensor(geneSubType,0,0,NULL,data1,data2,data3,data4);
          // Weight table must be built dynamically according to Sensor type
          int weightSize = m_pOldEntity->getExpectedBrainSensorWeightSize(m_pCurrentEditGeneM);
          short* weightTab = new short[weightSize];
          for (i=0; i<weightSize; i++)
            weightTab[i] = 100; // Def value
          m_pCurrentEditGeneM->setAsSensor(geneSubType,m_MuteRate,weightSize,weightTab,data1,data2,data3,data4);
          CSensor* pSens = m_pOldEntity->getTemporarySensorFromGene(m_pCurrentEditGeneM);
          auto rawDataM = m_pCurrentEditGeneM->getData();
          BYTE* pDataM = rawDataM.data();
          int dataLenM = rawDataM.size();
          m_WeightListCtrl_M.SetSensor(pSens,((WORD*)pDataM + 4));
          delete [] weightTab; //Table is duplicated in Gene data
        }
        else
        {
          int weightSize = m_WeightListCtrl_M.GetWeightBufSize();
          short* weightTab = m_WeightListCtrl_M.GetWeightBuf();
          m_pCurrentEditGeneM->setAsSensor(geneSubType,m_MuteRate,weightSize,weightTab,data1,data2,data3,data4);
        }
        break;
      }
    case GENE_REACTION:
      {
        long data1,data2,data3,data4;
        data1 = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
        data2 = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); 
        data3 = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos();
        data4 = m_SliderM4.GetRangeMax() - m_SliderM4.GetPos(); 
        m_pCurrentEditGeneM->setAsReaction(geneSubType,m_MuteRate,data1,data2,data3,data4);
        break;
      }
    case GENE_BRAIN_SIZE:
      {
        long data1;
        data1 = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
        m_pCurrentEditGeneM->setAsBrainSize(geneSubType,m_MuteRate,data1);
        break;
      }
    case GENE_FEELING:
      {
        int curSensSel = m_ComboLeft.GetCurSel();
        if (curSensSel<0)
          break;
        CSensor* pSens = (CSensor*)m_ComboLeft.GetItemData(curSensSel);

        if (resetWeightTable == true)
        {
          int sensiSize = pSens->GetSubCaptorNumber();
          short* sensiTab = new short[sensiSize];
          for (i=0; i<sensiSize; i++)
            sensiTab[i] = 100; // Def value
          m_pCurrentEditGeneM->setAsFeeling(geneSubType,m_MuteRate,pSens->GetUniqueId(),sensiSize,sensiTab);
          auto rawDataM = m_pCurrentEditGeneM->getData();
          BYTE* pDataM = rawDataM.data();
          int dataLenM = rawDataM.size();
          m_WeightListCtrl_M.SetSensor(pSens,((WORD*)pDataM + 2));
          delete [] sensiTab; //Table is duplicated in Gene data
        }
        else
        {
          int sensiSize = m_WeightListCtrl_M.GetWeightBufSize();
          short* sensiTab = m_WeightListCtrl_M.GetWeightBuf();
          m_pCurrentEditGeneM->setAsFeeling(geneSubType,m_MuteRate,pSens->GetUniqueId(),sensiSize,sensiTab);
        }
        break;
      }  
    case GENE_PURPOSE:
      {
        if (geneSubType==GENE_PURPOSE_TRIGGER_UP || geneSubType==GENE_PURPOSE_TRIGGER_DOWN)
        {
          long data1,data2,data3,data4;
          data1 = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos();
          data2 = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); 
          data3 = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos();
          data4 = m_SliderM4.GetRangeMax() - m_SliderM4.GetPos(); 

          int curSensSel = m_ComboLeft.GetCurSel();
          if (curSensSel<0)
            break;
          CSensor* pSens = (CSensor*)m_ComboLeft.GetItemData(curSensSel);
          CString label; 
          m_EditLabel.GetWindowText(label);
          m_pCurrentEditGeneM->setAsPurposeTrigger(geneSubType,m_MuteRate,pSens->GetUniqueId(),data1,data2,data3,data4,label.GetLength()+1,(char*)label.GetBuffer(0));
        }
        else if (geneSubType==GENE_PURPOSE_SENSOR)
        {
          int curPurposeSel = m_ComboLeft.GetCurSel();
          if (curPurposeSel<0)
            break;
          CPurpose* pPurpose = (CPurpose*)m_ComboLeft.GetItemData(curPurposeSel);

          int curSensorSel = m_ComboRight.GetCurSel();
          if (curSensorSel<0)
            break;
          CSensor* pSensor = (CSensor*)m_ComboRight.GetItemData(curSensorSel);

          if (resetWeightTable == true)
          {
            int bonusTabSize = pSensor->GetSubCaptorNumber();
            short* bonusTab = new short[bonusTabSize];
            for (i=0; i<bonusTabSize; i++)
              bonusTab[i] = 10; // Def value = 1.0
            m_pCurrentEditGeneM->setAsPurposeSensorBonus(m_MuteRate,pPurpose->GetUniqueId(),pSensor->GetUniqueId(),bonusTabSize,bonusTab);
            auto rawDataM = m_pCurrentEditGeneM->getData();
            BYTE* pDataM = rawDataM.data();
            int dataLenM = rawDataM.size();
            m_WeightListCtrl_M.SetSensor(pSensor,((WORD*)pDataM + 4), false);
            delete [] bonusTab; //Table is duplicated in Gene data
          }
          else
          {
            int bonusTabSize = m_WeightListCtrl_M.GetWeightBufSize();
            short* bonusTab = m_WeightListCtrl_M.GetWeightBuf();
            m_pCurrentEditGeneM->setAsPurposeSensorBonus(m_MuteRate,pPurpose->GetUniqueId(),pSensor->GetUniqueId(),bonusTabSize,bonusTab);
          }
        }
        else if (geneSubType==GENE_PURPOSE_REACTION)
        {
          long data1;
          data1 = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); 

          int curPurposeSel = m_ComboLeft.GetCurSel();
          if (curPurposeSel<0)
            break;
          CPurpose* pPurpose = (CPurpose*)m_ComboLeft.GetItemData(curPurposeSel);

          int curReactionSel = m_ComboRight.GetCurSel();
          if (curReactionSel<0)
            break;
          CReaction* pReaction = (CReaction*)m_ComboRight.GetItemData(curReactionSel);

          m_pCurrentEditGeneM->setAsPurposeReactionBonus(m_MuteRate,pPurpose->GetUniqueId(),pReaction->GetUniqueId(),data1);
        }
        break;
      }
    case GENE_BRAIN_CONFIG:
      data[0] = m_SliderM1.GetRangeMax() - m_SliderM1.GetPos(); // Reverse to have min at bottom!
      data[1] = m_SliderM2.GetRangeMax() - m_SliderM2.GetPos(); // FRU 
      data[2] = m_SliderM3.GetRangeMax() - m_SliderM3.GetPos(); // FRU 
      data[3] = m_SliderM4.GetRangeMax() - m_SliderM4.GetPos(); // FRU 
      m_pCurrentEditGeneM->setAsBrainConfig(geneSubType,m_MuteRate,muteType,1,data);
      break;
    default:

      break;
    }
  }

  // Refresh desc string
  m_strDescriptionM = m_pOldEntity->getGeneDescriptionString(m_pCurrentEditGeneM).c_str();
  m_GenomeTreeEdit.UpdateAnyGeneFromItsPointer(m_pCurrentEditGeneM);
  // Update windows
  UpdateData(false);

  return(true);
}

bool CGenomeEditorDlg::RefreshCurrentGeneP(bool rebuildGene, bool resetWeightTable)
{
  if (m_pCurrentEditGeneP==NULL)
    return(false);

	if ((m_EditMode == EDIT_MODE_GENE)&&(rebuildGene))
  {
    GeneType_e geneType = (GeneType_e)m_Combo1.GetItemData(m_Combo1.GetCurSel());
    GeneSubType_e geneSubType = (GeneSubType_e)m_Combo2.GetItemData(m_Combo2.GetCurSel());
    GeneMuteType_e muteType = (GeneMuteType_e)m_ComboMuteType.GetItemData(m_ComboMuteType.GetCurSel());
    int i;

    switch(geneType)
    {
    case GENE_CARACTER:
      unsigned char data[4];
      data[0] = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
      data[1] = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); 
      data[2] = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos(); 
      data[3] = m_SliderP4.GetRangeMax() - m_SliderP4.GetPos(); 
      m_pCurrentEditGeneP->setAsCaracter(geneSubType,m_MuteRate,muteType,4,data);
      break;
    case GENE_PARAMETER:
      long min,nominal,max;
      min     = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
      nominal = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); 
      max     = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos(); 
      m_pCurrentEditGeneP->setAsParameter(geneSubType,m_MuteRate,min,nominal,max);
      break;
    case GENE_LIFESTAGE:
      long ratio;
      ratio   = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
      m_pCurrentEditGeneP->setAsLifeStage(geneSubType,m_MuteRate,ratio);
      break;
    case GENE_PHY_WELFARE:
      long sensi;
      sensi   = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
      min     = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); // Reverse to have min at bottom!
      nominal = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos(); 
      max     = m_SliderP4.GetRangeMax() - m_SliderP4.GetPos(); 
      m_pCurrentEditGeneP->setAsPhysicWelfare(geneSubType,m_MuteRate,sensi,min,nominal,max);
      break;
    case GENE_SENSOR:
      {
        long data1,data2,data3,data4;
        data1 = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
        data2 = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); 
        data3 = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos();
        data4 = m_SliderP4.GetRangeMax() - m_SliderP4.GetPos(); 

        if (resetWeightTable == true)
        {
          m_pCurrentEditGeneP->setAsSensor(geneSubType,0,0,NULL,data1,data2,data3,data4);
          // Weight table must be built dynamically according to Sensor type
          int weightSize = m_pOldEntity->getExpectedBrainSensorWeightSize(m_pCurrentEditGeneP);
          short* weightTab = new short[weightSize];
          for (i=0; i<weightSize; i++)
            weightTab[i] = 100; // Def value
          m_pCurrentEditGeneP->setAsSensor(geneSubType,m_MuteRate,weightSize,weightTab,data1,data2,data3,data4);
          CSensor* pSens = m_pOldEntity->getTemporarySensorFromGene(m_pCurrentEditGeneP);
          auto rawDataP = m_pCurrentEditGeneP->getData();
          BYTE* pDataP = rawDataP.data();
          int dataLenP = rawDataP.size();
          m_WeightListCtrl_P.SetSensor(pSens,((WORD*)pDataP + 4));
          delete [] weightTab; //Table is duplicated in Gene data
        }
        else
        {
          int weightSize = m_WeightListCtrl_P.GetWeightBufSize();
          short* weightTab = m_WeightListCtrl_P.GetWeightBuf();
          m_pCurrentEditGeneP->setAsSensor(geneSubType,m_MuteRate,weightSize,weightTab,data1,data2,data3,data4);
        }
        break;
      }
    case GENE_REACTION:
      {
        long data1,data2,data3,data4;
        data1 = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
        data2 = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); 
        data3 = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos();
        data4 = m_SliderP4.GetRangeMax() - m_SliderP4.GetPos(); 
        m_pCurrentEditGeneP->setAsReaction(geneSubType,m_MuteRate,data1,data2,data3,data4);
        break;
      }
    case GENE_BRAIN_SIZE:
      {
        long data1;
        data1 = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
        m_pCurrentEditGeneP->setAsBrainSize(geneSubType,m_MuteRate,data1);
        break;
      }
    case GENE_FEELING:
      {
        int curSensSel = m_ComboLeft.GetCurSel();
        if (curSensSel<0)
          break;
        CSensor* pSens = (CSensor*)m_ComboLeft.GetItemData(curSensSel);

        if (resetWeightTable == true)
        {
          int sensiSize = pSens->GetSubCaptorNumber();
          short* sensiTab = new short[sensiSize];
          for (i=0; i<sensiSize; i++)
            sensiTab[i] = 100; // Def value
          m_pCurrentEditGeneP->setAsFeeling(geneSubType,m_MuteRate,pSens->GetUniqueId(),sensiSize,sensiTab);
          auto rawDataP = m_pCurrentEditGeneP->getData();
          BYTE* pDataP = rawDataP.data();
          int dataLenP = rawDataP.size();
          m_WeightListCtrl_P.SetSensor(pSens,((WORD*)pDataP + 2));
          delete [] sensiTab; //Table is duplicated in Gene data
        }
        else
        {
          int sensiSize = m_WeightListCtrl_P.GetWeightBufSize();
          short* sensiTab = m_WeightListCtrl_P.GetWeightBuf();
          m_pCurrentEditGeneP->setAsFeeling(geneSubType,m_MuteRate,pSens->GetUniqueId(),sensiSize,sensiTab);
        }
        break;
      } 
    case GENE_PURPOSE:
      {
        if (geneSubType==GENE_PURPOSE_TRIGGER_UP || geneSubType==GENE_PURPOSE_TRIGGER_DOWN)
        {
          long data1,data2,data3,data4;
          data1 = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos();
          data2 = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); 
          data3 = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos();
          data4 = m_SliderP4.GetRangeMax() - m_SliderP4.GetPos(); 

          int curSensSel = m_ComboLeft.GetCurSel();
          if (curSensSel<0)
            break;
          CSensor* pSens = (CSensor*)m_ComboLeft.GetItemData(curSensSel);
          CString label; 
          m_EditLabel.GetWindowText(label);
          m_pCurrentEditGeneP->setAsPurposeTrigger(geneSubType,m_MuteRate,pSens->GetUniqueId(),data1,data2,data3,data4,label.GetLength()+1,(char*)label.GetBuffer(0));
        }
        else if (geneSubType==GENE_PURPOSE_SENSOR)
        {
          int curPurposeSel = m_ComboLeft.GetCurSel();
          if (curPurposeSel<0)
            break;
          CPurpose* pPurpose = (CPurpose*)m_ComboLeft.GetItemData(curPurposeSel);

          int curSensorSel = m_ComboRight.GetCurSel();
          if (curSensorSel<0)
            break;
          CSensor* pSensor = (CSensor*)m_ComboRight.GetItemData(curSensorSel);

          if (resetWeightTable == true)
          {
            int bonusTabSize = pSensor->GetSubCaptorNumber();
            short* bonusTab = new short[bonusTabSize];
            for (i=0; i<bonusTabSize; i++)
              bonusTab[i] = 10; // Def value = 1.0
            m_pCurrentEditGeneP->setAsPurposeSensorBonus(m_MuteRate,pPurpose->GetUniqueId(),pSensor->GetUniqueId(),bonusTabSize,bonusTab);
            auto rawDataP = m_pCurrentEditGeneP->getData();
            BYTE* pDataP = rawDataP.data();
            int dataLenP = rawDataP.size();
            m_WeightListCtrl_P.SetSensor(pSensor,((WORD*)pDataP + 4), false);
            delete [] bonusTab; //Table is duplicated in Gene data
          }
          else
          {
            int bonusTabSize = m_WeightListCtrl_P.GetWeightBufSize();
            short* bonusTab = m_WeightListCtrl_P.GetWeightBuf();
            m_pCurrentEditGeneP->setAsPurposeSensorBonus(m_MuteRate,pPurpose->GetUniqueId(),pSensor->GetUniqueId(),bonusTabSize,bonusTab);
          }
        }
        else if (geneSubType==GENE_PURPOSE_REACTION)
        {
          long data1;
          data1 = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); 

          int curPurposeSel = m_ComboLeft.GetCurSel();
          if (curPurposeSel<0)
            break;
          CPurpose* pPurpose = (CPurpose*)m_ComboLeft.GetItemData(curPurposeSel);

          int curReactionSel = m_ComboRight.GetCurSel();
          if (curReactionSel<0)
            break;
          CReaction* pReaction = (CReaction*)m_ComboRight.GetItemData(curReactionSel);

          m_pCurrentEditGeneP->setAsPurposeReactionBonus(m_MuteRate,pPurpose->GetUniqueId(),pReaction->GetUniqueId(),data1);
        }
        break;
      }
    case GENE_BRAIN_CONFIG:
      data[0] = m_SliderP1.GetRangeMax() - m_SliderP1.GetPos(); // Reverse to have min at bottom!
      data[1] = m_SliderP2.GetRangeMax() - m_SliderP2.GetPos(); // FRU
      data[2] = m_SliderP3.GetRangeMax() - m_SliderP3.GetPos(); // FRU 
      data[3] = m_SliderP4.GetRangeMax() - m_SliderP4.GetPos(); // FRU 
      m_pCurrentEditGeneP->setAsBrainConfig(geneSubType,m_MuteRate,muteType,1,data);
      break;
    default:

      break;
    }
  }

  // Refresh desc string
  m_strDescriptionP = m_pOldEntity->getGeneDescriptionString(m_pCurrentEditGeneP).c_str();
  m_GenomeTreeEdit.UpdateAnyGeneFromItsPointer(m_pCurrentEditGeneP);
  // Update windows
  UpdateData(false);

  return(true);
}


void CGenomeEditorDlg::OnReleasedcaptureSliderM1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneM(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderM2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneM(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderM3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneM(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderM4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneM(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderP1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneP(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderP2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneP(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderP3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneP(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnReleasedcaptureSliderP4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RefreshCurrentGeneP(true);
	*pResult = 0;
}

void CGenomeEditorDlg::OnButtonLoad() 
{
  // Open module
  CString fileName;
  CString strSection,strData;

	CFileDialog fileDlg(true, LPCTSTR("xml"), LPCTSTR(""),0, LPCTSTR("Genome Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
	fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select genome");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    fileName = fileDlg.GetPathName();

    bool resu = m_pNewGenome->loadFromXmlFile((char*)fileName.GetBuffer(0));

    if (!resu)
    {
      AfxMessageBox(LPCTSTR("Impossible to save file ") + fileName);
    }

    m_GenomeTreeEdit.SetGenome(m_pNewGenome,true);
  }
}

void CGenomeEditorDlg::OnButtonSave() 
{
  // Open module
  CString fileName;
  CString strSection,strData;

	CFileDialog fileDlg(false, LPCTSTR("xml"), LPCTSTR(""),0, LPCTSTR("Genome Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
	fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select genome");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    fileName = fileDlg.GetPathName();	

    bool resu = m_pNewGenome->saveInXmlFile((char*)fileName.GetBuffer(0));

    if (!resu)
    {
      AfxMessageBox(LPCTSTR("Impossible to save file ") + fileName);
    }
  }
}

void CGenomeEditorDlg::OnSetBrainInGenes() 
{
  int resu = AfxMessageBox (LPCTSTR("Do you want to include identification?"),  MB_YESNOCANCEL | MB_ICONINFORMATION );
  if (resu == IDYES)
  {
    m_pNewGenome->setBrainIdentifyInGenes(m_pOldEntity->getBrain());
    m_pNewGenome->setBrainInstinctInGenes(m_pOldEntity->getBrain());
    m_GenomeTreeEdit.SetGenome(m_pNewGenome,true);
  }
  else if (resu == IDNO)
  {
    m_pNewGenome->setBrainInstinctInGenes(m_pOldEntity->getBrain());
    m_GenomeTreeEdit.SetGenome(m_pNewGenome,true);
  }
  return;
}

void CGenomeEditorDlg::OnSelchangeComboLeft() 
{
    // Refresh 
    RefreshCurrentGeneM(true);
    RefreshCurrentGeneP(true);	
}

void CGenomeEditorDlg::OnSelchangeComboRight() 
{
    // Refresh 
    RefreshCurrentGeneM(true);
    RefreshCurrentGeneP(true);	
}

void CGenomeEditorDlg::OnChangeEditMuteRate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
  // Update data
  UpdateData(true);
  RefreshCurrentGeneM(true);
  RefreshCurrentGeneP(true);
}

void CGenomeEditorDlg::OnSelchangeComboMuteType() 
{
	// TODO: Add your control notification handler code here
  RefreshCurrentGeneM(true);
  RefreshCurrentGeneP(true);	
}

void CGenomeEditorDlg::OnEndlabeleditListWeightM(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

  if (m_bIsWeightChanging)
  {
    m_WeightListCtrl_M.UdpateSelectedData();
	  RefreshCurrentGeneM(true,false);
  }

	*pResult = 0;
}

void CGenomeEditorDlg::OnEndlabeleditListWeightP(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

  if (m_bIsWeightChanging)
  {
    m_WeightListCtrl_P.UdpateSelectedData();
	  RefreshCurrentGeneP(true,false);
  }

	*pResult = 0;
}

void CGenomeEditorDlg::OnChangeEditLabel() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	  // Update data
  UpdateData(true);
  RefreshCurrentGeneM(true);
  RefreshCurrentGeneP(true);
}

void CGenomeEditorDlg::OnItemchangingListWeightM(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bIsWeightChanging = true;
	*pResult = 0;
}

void CGenomeEditorDlg::OnItemchangingListWeightP(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bIsWeightChanging = true;	
	*pResult = 0;
}
