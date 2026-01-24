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

// DialogTab4.cpp : implementation file
//

#include <direct.h>
#include "stdafx.h"
#include "ProjectWizard.h"
#include "DialogTab4.h"
#include "CBrainAnimal.h"
#include "CAnimMammal.h"
#include "CBiotop.h"
#include "CScenarioPlayer.h"
#include "CMineral.h"
#include "CEntityFactory.h"

#include "CWizardSensor.h"
#include "CWizardBrain.h"
#include "CWizardPurpose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDialogTab4 dialog

CDialogTab4::CDialogTab4(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTab4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTab4)
	m_nbChromosomes = 1;
	m_SexualChrom_Idx = 0;
	m_Age = 1;
	m_Seed = 0;
	m_strData1 = _T("");
	m_strData2 = _T("");
	m_strData3 = _T("");
	m_strData4 = _T("");
  m_strData5 = _T("");
	m_strLabel1 = _T("");
	m_strLabel2 = _T("");
	m_strLabel3 = _T("");
	m_strLabel4 = _T("");
	m_SliderVar = 0;
	m_Variation = 0;
	m_IsAutoLearning = FALSE;
	m_ScoreDisplay = _T("");
	m_NbIterations = 1;
	//}}AFX_DATA_INIT

    m_pGenome = NULL;
    m_pEntity = NULL;
    m_CurChrom_Idx = 0;
    m_GeneSpreadFactor = 0;
    m_EntitySpeedMax = 0;

}

CDialogTab4::~CDialogTab4()
{
  //if (m_pGenome!=NULL)
  //  delete m_pGenome;

  if (m_pEntity!=NULL)
    delete m_pEntity;

}


void CDialogTab4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTab4)
	DDX_Control(pDX, IDC_EDIT_SEXAL_CHROMO, m_EditSexChromIdx);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonMale);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonFemale);
	DDX_Control(pDX, IDC_TREE_GENOME, m_GenomeTree);
	DDX_Text(pDX, IDC_EDIT_NB_CHROMO, m_nbChromosomes);
	DDV_MinMaxUInt(pDX, m_nbChromosomes, 1, 40);
	DDX_Text(pDX, IDC_EDIT_SEXAL_CHROMO, m_SexualChrom_Idx);
	DDV_MinMaxUInt(pDX, m_SexualChrom_Idx, 0, 39);
	DDX_Text(pDX, IDC_EDIT_AGE, m_Age);
	DDV_MinMaxUInt(pDX, m_Age, 1, 99999);
	DDX_Text(pDX, IDC_EDIT_SEED, m_Seed);
	DDV_MinMaxUInt(pDX, m_Seed, 0, 1000);
	DDX_Text(pDX, IDC_DATA1, m_strData1);
	DDX_Text(pDX, IDC_DATA2, m_strData2);
	DDX_Text(pDX, IDC_DATA3, m_strData3);
	DDX_Text(pDX, IDC_DATA4, m_strData4);
  DDX_Text(pDX, IDC_DATA5, m_strData5);
	DDX_Text(pDX, IDC_LABEL1, m_strLabel1);
	DDX_Text(pDX, IDC_LABEL2, m_strLabel2);
	DDX_Text(pDX, IDC_LABEL3, m_strLabel3);
	DDX_Text(pDX, IDC_LABEL4, m_strLabel4);
	DDX_Slider(pDX, IDC_SLIDER1, m_SliderVar);
	DDX_Text(pDX, IDC_EDIT_VARIATION, m_Variation);
	DDX_Check(pDX, IDC_CHECK1, m_IsAutoLearning);
	DDX_Text(pDX, IDC_EDIT_SCORE, m_ScoreDisplay);
	DDX_Text(pDX, IDC_EDIT_ITERARTIONS, m_NbIterations);
	DDV_MinMaxUInt(pDX, m_NbIterations, 0, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTab4, CDialog)
	//{{AFX_MSG_MAP(CDialogTab4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonCreateFemale)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_GENOME, OnSelchangedTree1)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonCreateMale)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonSaveEntity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTab4 message handlers

BOOL CDialogTab4::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogTab4::OnButtonCreateFemale() 
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  if ((pView->GetClassType() < CLASS_VEGETAL_FIRST) || (pView->GetClassType() > CLASS_ANIMAL_LAST))
    CreateGenome(SEX_NONE);
  else
    CreateGenome(SEX_FEMALE);
}

void CDialogTab4::OnButtonCreateMale() 
{
  CreateGenome(SEX_MALE);
}

void CDialogTab4::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDialogTab4::DisplayGenome(CGenome* pGenome)
{
  CString tmpStr;

  m_strLabel1 = "Genome name";
  m_strLabel2 = "class";
  m_strLabel3 = "signature";
  m_strLabel4 = "";

  if (pGenome!=NULL)
  {
    m_strData1 = pGenome->getSpecieName().c_str();
    m_strData2 = pGenome->getOwnClassStrName().c_str();
    CString tmpStr;
    tmpStr.Format("%d",pGenome->getSpecieSignature());
    m_strData3 = tmpStr;
    m_strData4 = "";
    m_strData5 = "";
  }

  // Update windows
  UpdateData(false);
}

void CDialogTab4::DisplayPair(CPairOfChromosome* pPair)
{
  CString tmpStr;

  m_strLabel1 = "Chromosome pair";
  m_strLabel2 = "type";
  m_strLabel3 = "";
  m_strLabel4 = "";

  tmpStr.Format("id %d",pPair->getIdNumber());
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
  m_strData5 = "";

  // Update windows
  UpdateData(false);
}

void CDialogTab4::DisplayChromosome(CChromosome* pChromosome)
{
  CString tmpStr;

  m_strLabel1 = "Chromosome name";
  m_strLabel2 = "number";
  m_strLabel3 = "";
  m_strLabel4 = "Raw data";

  m_strData1 = pChromosome->getLabel().c_str();
  tmpStr.Format("%d",pChromosome->getIdNumber());
  m_strData2 = tmpStr;
  m_strData3 = "";
  m_strData4 = "Chromozome ";
  m_strData4 += pChromosome->getLabel().c_str();
  m_strData5 = pChromosome->buildStringDataFromGenes().c_str();

  // Update windows
  UpdateData(false);
}

void CDialogTab4::DisplayGene(CGene* pGene)
{
  CString tmpStr;
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();

  m_strLabel1 = "Gene name";
  m_strLabel2 = "mutation";
  m_strLabel3 = "dominance";
  m_strLabel4 = "Raw data";

  m_strData1 = pGene->getLabel().c_str();
  tmpStr.Format("type:%d rate:%d",pGene->getMuteType(),pGene->getMuteRate());
  m_strData2 = tmpStr;
  tmpStr.Format("%d",pGene->getRecessiveFactor());
  m_strData3 = tmpStr;
  m_strData4 = m_pEntity->getGeneDescriptionString(pGene).c_str(); 
  m_strData5 = pGene->buildStringDataFromGene().c_str();
  
  // Update windows
  UpdateData(false);
}

void CDialogTab4::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  if (pView->GetClassType() < CLASS_VEGETAL_FIRST)
  {
    m_ButtonMale.ShowWindow(SW_HIDE);
    m_ButtonFemale.SetWindowText("Create entity");
    m_EditSexChromIdx.EnableWindow(false);
  }
  else if (pView->GetClassType() > CLASS_ANIMAL_LAST)
  {
      m_ButtonMale.ShowWindow(SW_HIDE);
      m_ButtonFemale.SetWindowText("Create genome");
      m_EditSexChromIdx.EnableWindow(false);
  }
  else
  {
      m_ButtonMale.ShowWindow(SW_SHOW);
      m_ButtonFemale.SetWindowText("Create female");
      m_EditSexChromIdx.EnableWindow(true);
  }
}


bool CDialogTab4::CreateGenome(SexType_e sex)
{
  UpdateData(true);

  // Clear previous genome
  if (m_pGenome!=NULL)
    delete m_pGenome;

  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();

  m_pGenome = new CGenome( pView->GetClassType(), 
                           pView->GetTabCaractFemale()->m_EditBoxSpecieName.GetBuffer(0));

  for (int i=0; i<m_nbChromosomes; i++)
  {
    m_pGenome->addPair();
  }

  if (m_pGenome->isAnimalGenome())
  {
    // Add additional chromosome for brain
    m_pGenome->addPair();
  }

  if (m_SexualChrom_Idx >= m_nbChromosomes)
  {
    m_SexualChrom_Idx = m_nbChromosomes - 1;
    UpdateData(false);
  }

  if (sex == SEX_FEMALE)
    m_pGenome->setSexFemale(m_SexualChrom_Idx, pView->IsSexualDimorphism());
  else if (sex == SEX_MALE)
    m_pGenome->setSexMale(m_SexualChrom_Idx, pView->IsSexualDimorphism());

  /* Update entity prey and predator in Tab behavior */
  pView->GetTabBehavior()->LoadSensorsAndPredatorsEntities();

  /* Use seed to set generation parameters */
  m_CurChrom_Idx = m_Seed % m_nbChromosomes;
  m_GeneSpreadFactor = m_Seed & 0x0F;

  AddGenesForCaracters(sex);
  m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
  AddGenesForParameters(sex);
  m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
  AddGenesForLifeStages(sex);
  m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
  AddGenesForPhysic();
  m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;

  if (pView->GetpEntity()->isAnimal())
  {
    AddGenesForSensors();
    m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
    AddGenesForFeelings();
    m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
    AddGenesForReactions();
    m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
    AddGenesForPurposes();
    m_CurChrom_Idx = (m_CurChrom_Idx + 1) % m_nbChromosomes;
    AddGenesForBrain();
  }

  // Delete previous entity
  if (m_pEntity == NULL)
    delete m_pEntity;

  // create new entity
  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  m_pEntity = CEntityFactory::createEntity(pTabFemale->m_EditBoxSpecieName.GetBuffer(0), m_pGenome);
  m_pEntity->setDefaultLayer(pTabFemale->m_Layer);

  // Set instinct in brain but not yet in genome (TBC)
  if (pView->GetpEntity()->isAnimal())
  {  
    PrepareBrainInstinct((CAnimal*)m_pEntity);
  }

  m_GenomeTree.SetGenome(m_pGenome);

  return true;
}


void CDialogTab4::AddGenesForCaracters(SexType_e sex)
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  bool sexualDimorph = pView->IsSexualDimorphism();

  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  CDialogTab1* pTabMale   = pView->GetTabCaractMale();
  CDialogTab1* pTabCurCar = pTabFemale;

  // Ensure to refresh data
  pTabFemale->UpdateData(true);
  pTabMale->UpdateData(true);

  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;

  if (sexualDimorph && (sex == SEX_MALE))
    pTabCurCar = pTabMale;

  // Color
  if (sexualDimorph && (pTabFemale->m_color != pTabMale->m_color))
    pCurPaire = m_pGenome->getPair(m_SexualChrom_Idx);
  else
    pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsCaracter(GENE_CARACT_COLOR, 1, GENE_MUTE_RANDOM_BIT, 4, (BYTE*)&(pTabFemale->m_color));
  if (m_Variation > 50)
    pCurGene->tryMutation();

  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsCaracter(GENE_CARACT_COLOR, 1, GENE_MUTE_RANDOM_BIT, 4, (BYTE*)&(pTabCurCar->m_color));
  if (m_Variation > 50)
    pCurGene->tryMutation();

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Form
  AddGenesCarFromCombo(&pTabFemale->m_ComboForm, &pTabMale->m_ComboForm, sex, GENE_CARACT_FORM);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Odor
  AddGenesCarFromCombo(&pTabFemale->m_ComboOdor, &pTabMale->m_ComboOdor, sex, GENE_CARACT_ODOR);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Texture
  AddGenesCarFromCombo(&pTabFemale->m_ComboTexture, &pTabMale->m_ComboTexture, sex, GENE_CARACT_TEXTURE);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Taste
  AddGenesCarFromCombo(&pTabFemale->m_ComboTaste, &pTabMale->m_ComboTaste, sex, GENE_CARACT_TASTE);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Reproduct
  AddGenesCarFromCombo(&pTabFemale->m_ComboReproduct, &pTabMale->m_ComboReproduct, sex, GENE_CARACT_REPRO_TYPE);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Movt
  AddGenesCarFromCombo(&pTabFemale->m_ComboMvt, &pTabMale->m_ComboMvt, sex, GENE_CARACT_MOVE_TYPE);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Habitat
  AddGenesCarFromCombo(&pTabFemale->m_ComboHabitat, &pTabMale->m_ComboHabitat, sex, GENE_CARACT_HABITAT);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Consumption
  AddGenesCarFromCombo(&pTabFemale->m_ComboConsum, &pTabMale->m_ComboConsum, sex, GENE_CARACT_CONSUME_TYPE);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Attribut
  AddGenesCarFromCombo(&pTabFemale->m_ComboAttribut, &pTabMale->m_ComboAttribut, sex, GENE_CARACT_PHY_ATTRIBUTE);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

}


void CDialogTab4::AddGenesCarFromCombo(CComboBox* pComboFemale, CComboBox* pComboMale, SexType_e sex, GeneSubTypeCaracter_e subtype)
{
  int geneIndex;
  CGene* pCurGene = NULL;
  BYTE dataFemale, dataMale;
  CComboBox* pComboCur = pComboFemale;
  CPairOfChromosome* pCurPaire;

  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  bool sexualDimorph = pView->IsSexualDimorphism();

  if (sexualDimorph && (sex == SEX_MALE))
    pComboCur = pComboMale;

  dataFemale = pComboFemale->GetCurSel();
  dataMale   = pComboCur->GetCurSel();
  if (sexualDimorph && (dataFemale != dataMale))
    pCurPaire = m_pGenome->getPair(m_SexualChrom_Idx);
  else
    pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsCaracterUsingDefinition(subtype, 1, dataFemale);
  if (m_Variation > 50)
    pCurGene->tryMutation();

  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsCaracterUsingDefinition(subtype, 1, dataMale);
  if (m_Variation > 50)
    pCurGene->tryMutation();
}


void CDialogTab4::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UpdateData(true);
  m_Variation = m_SliderVar;
  UpdateData(false);

  *pResult = 0;
}


void CDialogTab4::AddGenesForParameters(SexType_e sex)
{
  int i;
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  bool sexualDimorph = pView->IsSexualDimorphism();

  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  CDialogTab1* pTabMale   = pView->GetTabCaractMale();
  CDialogTab1* pTabCurCar = pTabFemale;

  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;
  CGenericParam *pParamFemale,*pParamMale,*pParamCurrent;
  double paramMin, paramNominal, paramMax;
  GeneSubTypeParam_e curGeneSubType;

  if (sexualDimorph && (sex == SEX_MALE))
    pTabCurCar = pTabMale;

  // Generic parameters
  for (i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    if (pTabFemale->m_pParamSlider[i] != NULL)
    {
      pParamFemale = pTabFemale->m_pParamSlider[i]->GetpParam();

      if (pTabMale->m_pParamSlider[i] != NULL)
        pParamMale = pTabMale->m_pParamSlider[i]->GetpParam();
      else
        pParamMale = pParamFemale;

      if (sexualDimorph && (sex == SEX_MALE))
        pParamCurrent = pParamMale;
      else
        pParamCurrent = pParamFemale;

      if (sexualDimorph && (pParamFemale->getVal() != pParamMale->getVal()))
        pCurPaire = m_pGenome->getPair(m_SexualChrom_Idx);
      else
        pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);

      curGeneSubType = pParamFemale->getCodingGeneSubtype();
   
      if ( (curGeneSubType == GENE_PARAM_SPEED) 
        || (curGeneSubType == GENE_PARAM_GESTA_TIME)
        || (curGeneSubType == GENE_PARAM_GESTA_NB) )
      {
        // User choice is max
        paramMin     = pParamFemale->getMin();
        paramNominal = pParamFemale->getVal() / 2.0;
        paramMax     = pParamFemale->getVal();
        if (curGeneSubType == GENE_PARAM_SPEED)
        {
          m_EntitySpeedMax = pParamFemale->getVal();
        }
      }
      else
      {
        paramMin     = pParamFemale->getMin();
        paramNominal = pParamFemale->getVal();
        paramMax     = pParamFemale->getMax();
      }
      geneIndex = pCurPaire->getMaterChromosome().addGene();
      pCurGene = pCurPaire->getMaterChromosome().getGene(geneIndex);
      pCurGene->setAsParameterUsingDefinition(curGeneSubType, 10, paramMin, paramNominal, paramMax);

      curGeneSubType = pParamCurrent->getCodingGeneSubtype();
      if ( (curGeneSubType == GENE_PARAM_SPEED) 
        || (curGeneSubType == GENE_PARAM_GESTA_TIME)
        || (curGeneSubType == GENE_PARAM_GESTA_NB) )
      {
        // User choice is max
        paramMin     = pParamFemale->getMin();
        paramNominal = pParamFemale->getVal() / 2.0;
        paramMax     = pParamFemale->getVal();
        if (curGeneSubType == GENE_PARAM_SPEED)
        {
          m_EntitySpeedMax = pParamFemale->getVal();
        }
      }
      else
      {
        paramMin     = pParamFemale->getMin();
        paramNominal = pParamFemale->getVal();
        paramMax     = pParamFemale->getMax();
      }
      geneIndex = pCurPaire->getPaterChromosome().addGene();
      pCurGene = pCurPaire->getPaterChromosome().getGene(geneIndex);
      pCurGene->setAsParameterUsingDefinition(curGeneSubType, 10, paramMin, paramNominal, paramMax);

      m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;
    }
  }

  // Skip other setting for parasites
  if (m_pGenome->isParasiteGenome())
    return;

  // Weight
  if (sexualDimorph && (pTabFemale->m_Weight != pTabMale->m_Weight))
    pCurPaire = m_pGenome->getPair(m_SexualChrom_Idx);
  else
    pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);

  paramMin     = (double)(pTabFemale->m_WeightMin);
  paramMax     = (double)(pTabFemale->m_Weight);
  paramNominal = paramMin + (paramMax - paramMin) * 0.1;

  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsParameterUsingDefinition(GENE_PARAM_WEIGHT, 10, paramMin, paramNominal, paramMax);

  if (m_pGenome->isAnimalGenome())
  {
    geneIndex = pCurPaire->getMaterChromosome().addGene();
    pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
    pCurGene->setAsParameterUsingDefinition(GENE_PARAM_FAT_WEIGHT, 10, paramMin / 2.0 + 0.1, paramNominal / 2.0 + 0.1, paramMax / 2.0);
  }

  if (sexualDimorph && (sex == SEX_MALE))
  {
    paramMin     = (double)(pTabMale->m_WeightMin);
    paramNominal = (double)(pTabMale->m_WeightMin) * 1.1;
    paramMax     = (double)(pTabMale->m_Weight);
  }

  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsParameterUsingDefinition(GENE_PARAM_WEIGHT, 10, paramMin, paramNominal, paramMax);

  if (m_pGenome->isAnimalGenome())
  {
    geneIndex = pCurPaire->getPaterChromosome().addGene();
    pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
    pCurGene->setAsParameterUsingDefinition(GENE_PARAM_FAT_WEIGHT, 10, paramMin / 2.0 + 0.1, paramNominal / 2.0 + 0.1, paramMax / 2.0);
  }

  if (pView->GetpEntity()->getClass() > CLASS_MINERAL_LAST)
  {
      // Age
      if (sexualDimorph && (pTabFemale->m_LifeExpectency != pTabMale->m_LifeExpectency))
        pCurPaire = m_pGenome->getPair(m_SexualChrom_Idx);
      else
        pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);

      paramMin     = 0;
      paramNominal = 0;
      paramMax     = (double)(pTabFemale->m_LifeExpectency);

      geneIndex = pCurPaire->getMaterChromosome().addGene();
      pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
      pCurGene->setAsParameterUsingDefinition(GENE_PARAM_AGE, 10, paramMin, paramNominal, paramMax);

      if (sexualDimorph && (sex == SEX_MALE))
      {
        paramMin     = 0;
        paramNominal = 0;
        paramMax     = (double)(pTabMale->m_LifeExpectency);
      }

      geneIndex = pCurPaire->getPaterChromosome().addGene();
      pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
      pCurGene->setAsParameterUsingDefinition(GENE_PARAM_AGE, 10, paramMin, paramNominal, paramMax);

      // Decomposition
      if (sexualDimorph && (pTabFemale->m_DecompositionTime != pTabMale->m_DecompositionTime))
        pCurPaire = m_pGenome->getPair(m_SexualChrom_Idx);
      else
        pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);

      paramMin     = 0;
      paramNominal = 0;
      paramMax     = (double)(pTabFemale->m_DecompositionTime);

      geneIndex = pCurPaire->getMaterChromosome().addGene();
      pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
      pCurGene->setAsParameterUsingDefinition(GENE_PARAM_DECOMPOSITION, 10, paramMin, paramNominal, paramMax);

      if (sexualDimorph && (sex == SEX_MALE))
      {
        paramMin     = 0;
        paramNominal = 0;
        paramMax     = (double)(pTabMale->m_DecompositionTime);
      }

      geneIndex = pCurPaire->getPaterChromosome().addGene();
      pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
      pCurGene->setAsParameterUsingDefinition(GENE_PARAM_DECOMPOSITION, 10, paramMin, paramNominal, paramMax);
  }
}


void CDialogTab4::AddGenesForLifeStages(SexType_e sex)
{
  // TBC
}

void CDialogTab4::AddGenesForPhysic()
{
  // TBC
}


void CDialogTab4::AddGenesForSensors()
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  CDialogTab2* pTabSensors  = pView->GetTabSensors();
  CDialogTab3* pTabBehavior = pView->GetTabBehavior();

  CBasicEntity **pPreyList;
  CBasicEntity **pPredList;

  int numPrey = pTabBehavior->GetPreyEntityList(&pPreyList,false);
  int numPred = pTabBehavior->GetPredatorEntityList(&pPredList);

  CAnimal* pAnimal = (CAnimal*)CEntityFactory::createEntity("toto", m_pGenome);   

  if (pAnimal == NULL)
    return;

  if ((numPrey == 0) && (numPred == 0))
    return;

  Point_t coor = {0,0};
  pAnimal->jumpToGridCoord(coor, true, pTabFemale->m_Layer);

  CWizardSensor wizard(pAnimal, numPrey, pPreyList, numPred, pPredList);

  CString param1Str, param2Str;
  int i;

  // Add view sens
  pTabSensors->m_pSensorButtonTable[0]->m_pParam1Edit->GetWindowText(param1Str);
  pTabSensors->m_pSensorButtonTable[0]->m_pParam2Edit->GetWindowText(param2Str);

  if (pTabSensors->m_bUseVisualIdentification == TRUE)
  {
    wizard.AddGenesVisualIdentifySensors(m_CurChrom_Idx, 1.0, atoi(param1Str), 
                              (ViewAngleType_e)pTabSensors->m_comboViewAngle.GetCurSel(), atoi(param2Str), m_Variation);
  }
  else
  {
    wizard.AddGenesVisualSensors(m_CurChrom_Idx, 1.0, atoi(param1Str), 
                              (ViewAngleType_e)pTabSensors->m_comboViewAngle.GetCurSel(), atoi(param2Str), m_Variation);
  }

  // Add smell sens
  pTabSensors->m_pSensorButtonTable[1]->m_pParam1Edit->GetWindowText(param1Str);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;
  wizard.AddGenesSmellSensor(m_CurChrom_Idx, 1.0, atoi(param1Str), m_Variation);

  // Add other sens
  for (i=2;i<MAX_NUMBER_BUTTONS_SENSOR;i++)
  {
    if ( pTabSensors->m_pSensorButtonTable[i] && pTabSensors->m_pSensorButtonTable[i]->GetState() )
    {
      if (pTabSensors->m_pSensorButtonTable[i]->m_pParam1Edit)
        pTabSensors->m_pSensorButtonTable[i]->m_pParam1Edit->GetWindowText(param1Str);
      else
        param1Str = "0";

      if (pTabSensors->m_pSensorButtonTable[i]->m_pParam2Edit)
        pTabSensors->m_pSensorButtonTable[i]->m_pParam2Edit->GetWindowText(param2Str);
      else
        param2Str = "0";

      m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;
      wizard.AddGenesGenericSensor((GeneSubTypeSensor_e)(pTabSensors->m_pSensorButtonTable[i]->m_GeneSubTtype), m_CurChrom_Idx, m_SexualChrom_Idx,
                                    1.0, pTabSensors->m_pSensorButtonTable[i]->m_defParam, m_Variation, atoi(param1Str), atoi(param2Str));
    }
  }
}


void CDialogTab4::AddGenesForFeelings()
{
  // Add genes for welfare
  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex,i;
  CGene* pCurGene = NULL;
  short sensi;
  
  sensi = -500 + getRandInt(m_Variation);
  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_WELFARE, 10.0, UID_BASE_SENS_SUFFER, 1, &sensi);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_WELFARE, 10.0, UID_BASE_SENS_SUFFER, 1, &sensi);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  sensi = 100 + getRandInt(m_Variation);
  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_WELFARE, 10.0, UID_BASE_SENS_PLEASURE, 1, &sensi);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_WELFARE, 10.0, UID_BASE_SENS_PLEASURE, 1, &sensi);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;


  // Add gene for fear according to behavior
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab3* pTabBehavior = pView->GetTabBehavior();
  CDialogTab1* pTabFemale   = pView->GetTabCaractFemale();

  short fearLevel;
  switch (pTabBehavior->m_ComboBehavior.GetCurSel())
  { 
  case 0:
   fearLevel = 100;
   break;
  case 1:
   fearLevel = 80;
   break;
  case 2:
   fearLevel = 60;
   break;
  default:
   fearLevel = 0;
   break;
  }

  short sensiNoise[8];

  if (pTabFemale->m_ComboConsum.GetCurSel() == CONSUM_MEAT)
  {
    // reduce fear level for predators. Do not be afraid by noise in front
    fearLevel = fearLevel/2;
    sensiNoise[0]=0;
    sensiNoise[1]=0;
    sensiNoise[2]=0;
  }
  else
  {
    sensiNoise[0]=1*fearLevel + getRandInt(m_Variation);
    sensiNoise[1]=2*fearLevel + getRandInt(m_Variation);
    sensiNoise[2]=sensiNoise[1]; // keep symetri
  }

  sensiNoise[3]=6*fearLevel + getRandInt(m_Variation);
  sensiNoise[4]=sensiNoise[3]; // keep symetri
  sensiNoise[5]=8*fearLevel + getRandInt(m_Variation);
  sensiNoise[6]=sensiNoise[5]; // keep symetri
  sensiNoise[7]=8*fearLevel + getRandInt(m_Variation);
  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_FEAR, 10.0, UID_BASE_SENS_EAR, 8, sensiNoise);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_FEAR, 10.0, UID_BASE_SENS_EAR, 8, sensiNoise);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  short sensiOdor[NUMBER_ODORS];
  for (i=0; i<NUMBER_ODORS; i++)
    sensiOdor[i] = 0;

  CBasicEntity **pPredList;
  int numPred = pTabBehavior->GetPredatorEntityList(&pPredList);

  for (i=0; i<numPred; i++)
  {
    if ( (pPredList[i] != NULL) && (pPredList[i]->getOdor() >= ODOR_FIRST_TYPE) )
      sensiOdor[(pPredList[i]->getOdor() - ODOR_FIRST_TYPE)] = (10 - numPred)*fearLevel + getRandInt(m_Variation);
  }

  short confidenceLevel;
  switch (pTabBehavior->m_ComboSocial.GetCurSel())
  { 
  case 0:
   confidenceLevel = 20;
   break;
  case 1:
   confidenceLevel = 40 + getRandInt(m_Variation);
   break;
  case 2:
   confidenceLevel = 60 + getRandInt(m_Variation);
   break;
  default:
   confidenceLevel = 0;
   break;
  }

  if (pTabFemale->m_ComboOdor.GetCurSel() >= ODOR_FIRST_TYPE)
    sensiOdor[(pTabFemale->m_ComboOdor.GetCurSel() - ODOR_FIRST_TYPE)] -= confidenceLevel;

  for (i=0; i<NUMBER_ODORS; i++)
    if (sensiOdor[i] > 1000) 
      sensiOdor[i] = 1000;

  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_FEAR, 10.0, UID_BASE_SENS_SMELL, NUMBER_ODORS, sensiOdor);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsFeeling(GENE_FEEL_FEAR, 10.0, UID_BASE_SENS_SMELL, NUMBER_ODORS, sensiOdor);

}

void CDialogTab4::AddGenesForReactions()
{
  // Add genes for reactions
  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex,i,j;
  CGene* pCurGene = NULL;
  double succes, failure, param1;
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab3* pTabBehavior = pView->GetTabBehavior();

  // Movt mgt
  succes  = 0;
  failure = 0;
  param1  = 8 + m_EntitySpeedMax/50;
  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsReactionUsingDefinition(GENE_REACT_MOVEFORWARD, 1.0, succes, failure, param1);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsReactionUsingDefinition(GENE_REACT_MOVEFORWARD, 1.0, succes, failure, param1);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  param1  = m_EntitySpeedMax;
  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsReactionUsingDefinition(GENE_REACT_MOVEFORWARD, 1.0, succes, failure, param1);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsReactionUsingDefinition(GENE_REACT_MOVEFORWARD, 1.0, succes, failure, param1);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  CString param1Str, param2Str;
  GeneSubTypeReaction_e type ;

  bool isPreyOnLayer[MAX_NB_LAYER_VIEW];
  for (i=0;i<MAX_NB_LAYER_VIEW;i++)
  {
    isPreyOnLayer[i] = false;
  }
  CBasicEntity** ptEntity;  
  int numPrey = pTabBehavior->GetPreyEntityList(&ptEntity,false); //remove water
  for (i=0;i<numPrey;i++)
  {
    isPreyOnLayer[ptEntity[i]->getLayer()] = true;
  }

  // Add current layer (babies have to eat on cur layer to get their mother's milk)
  isPreyOnLayer[pView->GetTabCaractFemale()->m_Layer] = true;

  for (i=0; i<MAX_NUMBER_BUTTONS_ACTION; i++)
  {
    if ( pTabBehavior->m_pActionButtonTable[i] && pTabBehavior->m_pActionButtonTable[i]->GetState() )
    {
        type = (GeneSubTypeReaction_e)pTabBehavior->m_pActionButtonTable[i]->m_GeneSubTtype;

        if (pTabBehavior->m_pActionButtonTable[i]->m_pParam1Edit)
          pTabBehavior->m_pActionButtonTable[i]->m_pParam1Edit->GetWindowText(param1Str);
        else
          param1Str = "0";

        if (pTabBehavior->m_pActionButtonTable[i]->m_pParam2Edit)
          pTabBehavior->m_pActionButtonTable[i]->m_pParam2Edit->GetWindowText(param2Str);
        else
          param2Str = "0";

        if ( (type == GENE_REACT_ATTACK) || (type == GENE_REACT_EAT) )
        {
          for (j=0; j<MAX_NB_LAYER_VIEW; j++)
          {
            if (isPreyOnLayer[j])
            {
              succes  = atoi(param1Str);
              failure = atoi(param2Str);
              param1  = j - (int)pView->GetTabCaractFemale()->m_Layer;
              pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
              geneIndex = pCurPaire->getMaterChromosome().addGene();
              pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
              pCurGene->setAsReactionUsingDefinition(type, 1.0, succes, failure, param1);
              geneIndex = pCurPaire->getPaterChromosome().addGene();
              pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
              pCurGene->setAsReactionUsingDefinition(type, 1.0, succes, failure, param1);
            }
          }
        }
        else
        {
          succes  = atoi(param1Str);
          failure = atoi(param2Str);
          param1  = 0;
          pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
          geneIndex = pCurPaire->getMaterChromosome().addGene();
          pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
          pCurGene->setAsReactionUsingDefinition(type, 1.0, succes, failure, param1);
          geneIndex = pCurPaire->getPaterChromosome().addGene();
          pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
          pCurGene->setAsReactionUsingDefinition(type, 1.0, succes, failure, param1);
        }
    }
  }
}

void CDialogTab4::AddGenesForPurposes()
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab3* pTabBehavior = pView->GetTabBehavior();
  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  int bonusSensor, bonusReaction, trigger;

  CGenome* pTmpGenome = new CGenome(*m_pGenome);
  CBasicEntity* pEntity = CEntityFactory::createEntity("toto", pTmpGenome);
  Point_t coor = {0,0};
  pEntity->jumpToGridCoord(coor, true, pTabFemale->m_Layer);

  CWizardPurpose purpWiz((CAnimal*)pEntity);

  // Follow mummy
  switch (pTabBehavior->m_ComboSocial.GetCurSel())
  { 
  case 0:
   bonusSensor = 80 + getRandInt(m_Variation)/4;
   break;
  case 1:
   bonusSensor = 70 + getRandInt(m_Variation)/4;
   break;
  case 2:
   bonusSensor = 60 + getRandInt(m_Variation)/4;
   break;
  default:
   bonusSensor = 0;
   break;
  }
  
  // If baby stay at home, don't follow mother too much
  if (pTabBehavior->m_bBabiesStayHome)
  {
    bonusSensor = bonusSensor / 4;
  }

  trigger = 500 + getRandInt(m_Variation);
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "mummy", 6000, PURPOSE_TRIG_UP, UID_BASE_SENS_HORMONE, 1000, PHEROMONE_BABY-PHEROMONE_FIRST_TYPE, trigger, 200);
  purpWiz.AddPurposeVisualBonus("mummy", bonusSensor, pEntity->getLayer(), pEntity->getForm(), pEntity->getColorType(),
                                pEntity->getTexture(), PHY_ATTRIBUTE_UDDER, true);
  purpWiz.AddPurposeVisualIdentifyBonus("mummy", bonusSensor, pEntity->getLayer(), IDENTIFICATION_MOTHER, true, false, false); 

  purpWiz.AddPurposePheromoneBonus("mummy", bonusSensor, PHEROMONE_MATERNAL);

  // If baby stay at home, hide and sleep. Add also a "home" purpose for mother
  if (pTabBehavior->m_bBabiesStayHome)
  {
    bonusReaction = 1008 + getRandInt(m_Variation)/10;
    purpWiz.AddPurposeReactionBonus ("mummy", UID_BASE_REACT_NOTHING, 10, bonusReaction); // idle
    purpWiz.AddPurposeReactionBonus ("mummy", UID_BASE_REACT_HIDE,    10, bonusReaction); // hide
    purpWiz.AddPurposeInList (m_CurChrom_Idx, "home", 6000, PURPOSE_TRIG_UP, UID_BASE_SENS_HORMONE, 1000, PHEROMONE_MATERNAL-PHEROMONE_FIRST_TYPE, trigger, 200);
    purpWiz.AddPurposePheromoneBonus("home", bonusSensor, PHEROMONE_BABY);
  }

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Runnaway when afraid
  bonusReaction = 1018 + getRandInt(m_Variation)/10;
  bonusSensor = 70 + getRandInt(m_Variation)/10;
  trigger = 200 + 100 * pTabBehavior->m_ComboBehavior.GetCurSel() + getRandInt(m_Variation);
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "runaway", 10, PURPOSE_TRIG_UP, UID_BASE_SENS_FEAR, 1, 0, trigger, 200);
  purpWiz.AddPurposeReactionBonus ("runaway", UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusReaction); // accelerate
  purpWiz.AddPurposeVisualIdentifyBonus("runaway", bonusSensor, pEntity->getLayer(), IDENTIFICATION_PREDATOR, true, true, true);
  purpWiz.AddPurposeVisualIdentifyBonus("runaway", 30, pEntity->getLayer(), IDENTIFICATION_DANGER, false, false, false); // anticipate danger
  purpWiz.AddPurposeVisualIdentifyBonus("runaway", 20, pEntity->getLayer(), IDENTIFICATION_NEUTRAL, false, false, false); // anticipate obstacle
  // For gregarious, stay in group
  if (pTabBehavior->m_ComboSocial.GetCurSel() > 0)
  {
    purpWiz.AddPurposeVisualBonus("runaway", bonusSensor, pEntity->getLayer(), pEntity->getForm(), pEntity->getColorType(),
                                pEntity->getTexture(), PHY_ATTRIBUTE_UNSET, true);
    purpWiz.AddPurposeVisualIdentifyBonus("runaway", bonusSensor, pEntity->getLayer(), IDENTIFICATION_FRIEND, true, true, true);
  }
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // realease when tired
  bonusReaction = 990 - getRandInt(m_Variation)/10;
  trigger = 780 + getRandInt(m_Variation);
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "release", 10, PURPOSE_TRIG_UP, UID_BASE_SENS_TIREDNESS, 1, 0, trigger, 400);
  purpWiz.AddPurposeReactionBonus ("release", UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusReaction); //don't run
  purpWiz.AddPurposeReactionBonus ("release", UID_BASE_REACT_ATTACK, 100, bonusReaction); //don't attack
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Loop on preys to eat
  CBasicEntity **pPreyList;
  int numPrey = pTabBehavior->GetPreyEntityList(&pPreyList,false); // Remove water
  trigger = 280 + getRandInt(m_Variation);
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "feed", 20, PURPOSE_TRIG_UP, UID_BASE_SENS_HUNGER, 1, 0, trigger, 100);
  for (int i=0; i<numPrey; i++)
  {
    bonusSensor = 30 -2*numPrey - 2*i + getRandInt(m_Variation)/4;
    purpWiz.AddPurposeVisualBonus("feed", bonusSensor, pPreyList[i]->getLayer(), pPreyList[i]->getForm(), pPreyList[i]->getColorType(),
                                  pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), false);
  }

  if (numPrey > 0)
  {
    bonusSensor = 30 + getRandInt(m_Variation) / 2;
    purpWiz.AddPurposeVisualIdentifyBonus("feed", bonusSensor, pPreyList[0]->getLayer(), IDENTIFICATION_FOOD, false, false, false);
    if (pTabFemale->m_ComboConsum.GetCurSel() >= CONSUM_OMNI)
      purpWiz.AddPurposeVisualIdentifyBonus("feed", bonusSensor, pPreyList[0]->getLayer(), IDENTIFICATION_PREY, true, true, true);
    // decrease interest for adult food when baby
    purpWiz.AddPurposeVisualIdentifyBonus("mummy", 2, pPreyList[0]->getLayer(), IDENTIFICATION_FOOD, false, false, false);
    if ((numPrey > 1) && (pPreyList[1]->getLayer() != pPreyList[0]->getLayer()))
    {
      purpWiz.AddPurposeVisualIdentifyBonus("feed", bonusSensor, pPreyList[1]->getLayer(), IDENTIFICATION_FOOD, false, false, false);
      if (pTabFemale->m_ComboConsum.GetCurSel() >= CONSUM_OMNI)
        purpWiz.AddPurposeVisualIdentifyBonus("feed", bonusSensor, pPreyList[1]->getLayer(), IDENTIFICATION_PREY, true, true, true);
      // decrease interest for adult food when baby
      purpWiz.AddPurposeVisualIdentifyBonus("mummy", 2, pPreyList[1]->getLayer(), IDENTIFICATION_FOOD, false, false, false);
    }
  }

  bonusReaction = 1008 + getRandInt(m_Variation)/10;
  purpWiz.AddPurposeReactionBonus ("feed", UID_BASE_REACT_EAT, 10, bonusReaction);
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Drink
  bonusSensor = 40 + getRandInt(m_Variation)/4;
  trigger = 280 + getRandInt(m_Variation);
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "drink", 15, PURPOSE_TRIG_UP, UID_BASE_SENS_THIRST, 1, 0, trigger, 100);
  purpWiz.AddPurposeVisualBonus("drink", bonusSensor, 1, FORM_PLAN_SMOOTH, COLOR_CARACTER_BLUE, TEXTURE_REFLECT, PHY_ATTRIBUTE_NONE, false);
  purpWiz.AddPurposeVisualIdentifyBonus("drink", bonusSensor, 1, IDENTIFICATION_DRINK, false, false, false); 
  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  // Regroup when isolated
  if ((pTabBehavior->m_ComboSocial.GetCurSel() > 0) && (pEntity->getOdor() >= ODOR_FIRST_TYPE))
  {
    if (pTabBehavior->m_ComboSocial.GetCurSel() == 2)
      bonusSensor = 44 + getRandInt(m_Variation)/4;
    else
      bonusSensor = 42 + getRandInt(m_Variation)/4;

    trigger = 350 + getRandInt(m_Variation);
    purpWiz.AddPurposeInList (m_CurChrom_Idx, "regroup", 10, PURPOSE_TRIG_DOWN, 
                              UID_BASE_SENS_SMELL, 1000, pEntity->getOdor() - ODOR_FIRST_TYPE, trigger, 440);
    purpWiz.AddPurposeVisualBonus("regroup", bonusSensor, pEntity->getLayer(), pEntity->getForm(), pEntity->getColorType(),
                                  pEntity->getTexture(), pEntity->getMainPhyAttribute(), true);
    purpWiz.AddPurposeVisualIdentifyBonus("regroup", bonusSensor, pEntity->getLayer(), IDENTIFICATION_FRIEND, true, true, true);
    purpWiz.AddPurposeVisualIdentifyBonus("regroup", bonusSensor, pEntity->getLayer(), IDENTIFICATION_SEX_PARTNER, true, true, true); 
    m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;
  }
  else // For solitary animals, regroup for reproduction only
  {
    bonusSensor = 60 + getRandInt(m_Variation)/4;
    purpWiz.AddPurposeInList (m_CurChrom_Idx, "regroup", 10, PURPOSE_TRIG_UP, UID_BASE_SENS_LIBIDO, 1, 0, 600, 440);
    purpWiz.AddPurposeVisualBonus("regroup", bonusSensor, pEntity->getLayer(), pEntity->getForm(), pEntity->getColorType(),
                                  pEntity->getTexture(), pEntity->getMainPhyAttribute(), true);
    purpWiz.AddPurposeVisualIdentifyBonus("regroup", bonusSensor, pEntity->getLayer(), IDENTIFICATION_SEX_PARTNER, true, true, true);
    m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;
  }

  // Sleep at night (should be done according to diurn..)
  trigger = 780 + getRandInt(m_Variation);
  bonusReaction = 1250 + getRandInt(m_Variation)/10;
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "sleep", 20, PURPOSE_TRIG_UP, UID_BASE_SENS_OBSCURITY, 1, 0, trigger, trigger-100);
  purpWiz.AddPurposeReactionBonus ("sleep", UID_BASE_REACT_SLEEP, 10, bonusReaction); // sleep

  // Ruminate when stomach full
  trigger = 800 + getRandInt(m_Variation);
  bonusReaction = 1040 + getRandInt(m_Variation)/10;
  purpWiz.AddPurposeInList (m_CurChrom_Idx, "digest", 20, PURPOSE_TRIG_UP, UID_BASE_SENS_STOMACH_FULL, 1, 0, trigger, trigger-790);
  purpWiz.AddPurposeReactionBonus ("digest", UID_BASE_REACT_RUMINATE, 1, bonusReaction + 150); // ruminate
  purpWiz.AddPurposeReactionBonus ("digest", UID_BASE_REACT_NOTHING,  1, bonusReaction); // idle
  purpWiz.AddPurposeReactionBonus ("digest", UID_BASE_REACT_EAT,      1, 900); // stop eating
  purpWiz.AddPurposeReactionBonus ("digest", UID_BASE_REACT_EAT + 10, 1, 900); // istop eating
  purpWiz.AddPurposeReactionBonus ("digest", UID_BASE_REACT_DRINK,    1, 900); // stop drinking

  purpWiz.AddAllPurposeGenesToAnimal(1.0);

  // Delete previous genome
  delete m_pGenome;
  // Copy entity genome
  m_pGenome = new CGenome(*pEntity->getGenome());
  // Delete entity
  delete pEntity;
}

void CDialogTab4::AddGenesForBrain()
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab3* pTabBehavior = pView->GetTabBehavior();

  // Add genes for reactions
  CPairOfChromosome* pCurPaire = NULL;
  CGene* pCurGene = NULL;
  int geneIndex;

  pCurPaire = m_pGenome->getPair(m_CurChrom_Idx);
  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsBrainSize(GENE_BRAIN_SIZE_HIST_IN, 1.0, 2.0);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsBrainSize(GENE_BRAIN_SIZE_HIST_IN, 1.0, 2.0);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  geneIndex = pCurPaire->getMaterChromosome().addGene();
  pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
  pCurGene->setAsBrainSize(GENE_BRAIN_SIZE_HIST_EXP, 1.0, 3.0);
  geneIndex = pCurPaire->getPaterChromosome().addGene();
  pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
  pCurGene->setAsBrainSize(GENE_BRAIN_SIZE_HIST_EXP, 1.0, 3.0);

  m_CurChrom_Idx = (m_CurChrom_Idx + m_GeneSpreadFactor) % m_nbChromosomes;

  if (pTabBehavior->m_bBabiesStayHome)
  {
    geneIndex = pCurPaire->getMaterChromosome().addGene();
    pCurGene  = pCurPaire->getMaterChromosome().getGene(geneIndex);
    pCurGene->setAsBrainConfigUsingDefinition(GENE_BRAIN_BEHAVIOR, 1, BRAIN_BEHAVIOR_BABY_STAY_HOME);
    geneIndex = pCurPaire->getPaterChromosome().addGene();
    pCurGene  = pCurPaire->getPaterChromosome().getGene(geneIndex);
    pCurGene->setAsBrainConfigUsingDefinition(GENE_BRAIN_BEHAVIOR, 1, BRAIN_BEHAVIOR_BABY_STAY_HOME);
  }

}

void CDialogTab4::PrepareBrainInstinct(CAnimal* pAnimal)
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab3* pTabBehavior = pView->GetTabBehavior();
  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  int level,i;

  CWizardBrain brainWiz(pAnimal);

  // Avoid all obstacles on cur layer
  level = 80 + getRandInt(m_Variation)/10;
  brainWiz.AddVisualObstacle (level, pAnimal->getLayer(), FORM_UNSET, COLOR_CARACTER_UNSET, TEXTURE_NONE, PHY_ATTRIBUTE_NONE);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_PLAN_SMOOTH,     COLOR_CARACTER_WHITE, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_PLAN_ROUGH,      COLOR_CARACTER_BLACK, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_ROUNDISH,        COLOR_CARACTER_GREY,  TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_GEOMETRIC,       COLOR_CARACTER_RED,   TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_VEGET_LONG_LEEF, COLOR_CARACTER_BLUE,  TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_VEGET_BUSH,      COLOR_CARACTER_GREEN, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_VEGET_TREE,      COLOR_CARACTER_YELLOW,TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_QUADRIPED_THIN,  COLOR_CARACTER_ORANGE,TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_QUADRIPED_FAT,   COLOR_CARACTER_PINK,  TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_BIPED,           COLOR_CARACTER_BEIGE, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_CARRION,         COLOR_CARACTER_BROWN, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);
  brainWiz.AddVisualObstacle (20, pAnimal->getLayer(), FORM_QUADRIPED_LONG_NECK, COLOR_CARACTER_UNSET, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, true);

  // Add additional weight to avoid collision with obstacles
  brainWiz.AddVisualObstacle (30, pAnimal->getLayer(), FORM_ROUNDISH, COLOR_CARACTER_UNSET, TEXTURE_NONE, PHY_ATTRIBUTE_NONE);
  brainWiz.AddVisualObstacle (30, pAnimal->getLayer(), FORM_GEOMETRIC, COLOR_CARACTER_UNSET, TEXTURE_NONE, PHY_ATTRIBUTE_NONE);

  // Water is an obstacle
  level = 32 + getRandInt(m_Variation)/10;
  brainWiz.AddVisualObstacle ( level, 1, FORM_PLAN_SMOOTH, COLOR_CARACTER_BLUE, TEXTURE_REFLECT, PHY_ATTRIBUTE_NONE);
  // Only for non-swimmer animal
  brainWiz.AddVisualObstacle ( level+2, 1, FORM_PLAN_SMOOTH, COLOR_CARACTER_BLUE, TEXTURE_REFLECT, PHY_ATTRIBUTE_NONE, true);

  // View identify obstacle
  for (i=0; i<IDENTIFICATION_NUMBER_TYPE; i++)
  {
    brainWiz.AddVisualIdentifyObstacle (20, pAnimal->getLayer(), (IdentificationType_e)i, false, false, false, true);
  }
  // Add additional weight to avoid collision with obstacles
  brainWiz.AddVisualIdentifyObstacle (20, pAnimal->getLayer(), IDENTIFICATION_NEUTRAL, false, false, false, false);

  // Water is an obstacle
  brainWiz.AddVisualIdentifyObstacle (level,   1, IDENTIFICATION_DANGER, false, false, false, false);
  // Only for non-swimmer animal
  brainWiz.AddVisualIdentifyObstacle (level+12, 1, IDENTIFICATION_DANGER, false, false, false, true);

  // Attraction for own specie
  short attractLevel;
  switch (pTabBehavior->m_ComboSocial.GetCurSel())
  { 
  case 0:
   attractLevel = 8 + getRandInt(m_Variation)/10;  // Minimum attraction needed for reproduction
   break;
  case 1:
   attractLevel = 12 + getRandInt(m_Variation)/10;
   break;
  case 2:
   attractLevel = 14 + getRandInt(m_Variation)/10;
   break;
  default:
   attractLevel = 0; 
   break;
  }

  // Decrease bonus when attribute or texture exist as friends are easier to identify
  if (pAnimal->getMainPhyAttribute() > PHY_ATTRIBUTE_NONE)
    attractLevel = attractLevel * 8 / 10;
  if (pAnimal->getTexture() > TEXTURE_NONE)
    attractLevel = attractLevel * 8 / 10;
  
  brainWiz.AddVisualBonus ( attractLevel, pAnimal->getLayer(), pAnimal->getForm(),  pAnimal->getColorType(), pAnimal->getTexture(), pAnimal->getMainPhyAttribute(), false);
  brainWiz.AddVisualIdentifyBonus (attractLevel, pAnimal->getLayer(), IDENTIFICATION_FRIEND, true, true, true, false);
  brainWiz.AddVisualIdentificationBonus ( 30, pAnimal->getForm(),  pAnimal->getColorType(), pAnimal->getTexture(), pAnimal->getMainPhyAttribute(), false, false, false, IDENTIFICATION_FRIEND);
  if (pAnimal->getOdor() >= ODOR_FIRST_TYPE)
  {
    brainWiz.AddOdorBonus (-attractLevel, pAnimal->getOdor(),   UID_BASE_REACT_MOVEFORWARD, 200);
    brainWiz.AddOdorIdentificationBonus(attractLevel,   pAnimal->getOdor(), IDENTIFICATION_FRIEND);
	brainWiz.AddOdorIdentificationBonus(attractLevel/2, pAnimal->getOdor(), IDENTIFICATION_MOTHER);
	brainWiz.AddOdorIdentificationBonus(attractLevel/2, pAnimal->getOdor(), IDENTIFICATION_BABY);
	brainWiz.AddOdorIdentificationBonus(attractLevel/2, pAnimal->getOdor(), IDENTIFICATION_SEX_PARTNER);
  }

  // drink in front of water
  brainWiz.AddVisualBonus ( 25, 1, FORM_PLAN_SMOOTH, COLOR_CARACTER_BLUE, TEXTURE_REFLECT, PHY_ATTRIBUTE_NONE, false, false, UID_BASE_REACT_DRINK, 1);
  brainWiz.AddVisualIdentifyBonus (32, 1, IDENTIFICATION_DRINK, false, false, false, false, UID_BASE_REACT_DRINK, 1);
  brainWiz.AddVisualIdentificationBonus ( 30, FORM_PLAN_SMOOTH, COLOR_CARACTER_BLUE, TEXTURE_REFLECT, PHY_ATTRIBUTE_NONE, false, false, false, IDENTIFICATION_DRINK);
  // water is also a danger
  brainWiz.AddVisualIdentificationBonus ( 30, FORM_PLAN_SMOOTH, COLOR_CARACTER_BLUE, TEXTURE_REFLECT, PHY_ATTRIBUTE_NONE, false, false, false, IDENTIFICATION_DANGER);

  // Loop on preys to eat
  CBasicEntity **pPreyList;
  int numPrey = pTabBehavior->GetPreyEntityList(&pPreyList,false); // Remove water

  for (i=0;i<numPrey;i++)
  {
    int deltaLayer = pPreyList[i]->getLayer() - pAnimal->getLayer();

    if (pPreyList[i]->isVegetal())
    {
      attractLevel = 20 + getRandInt(m_Variation)/10 - numPrey - i;
      if (pPreyList[i]->getOdor() >= ODOR_FIRST_TYPE)
      {
        brainWiz.AddOdorBonus (-attractLevel, pPreyList[i]->getOdor(), UID_BASE_REACT_MOVEFORWARD, 11);
        brainWiz.AddOdorIdentificationBonus(attractLevel, pPreyList[i]->getOdor(), IDENTIFICATION_FOOD);
      }

      attractLevel = 40 + getRandInt(m_Variation)/10 - numPrey - i;
      brainWiz.AddVisualBonus ( attractLevel, pPreyList[i]->getLayer(), pPreyList[i]->getForm(), pPreyList[i]->getColorType(), 
                                pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), false, false,UID_BASE_REACT_EAT+10+deltaLayer, 1);
      brainWiz.AddVisualIdentifyBonus (attractLevel, pPreyList[i]->getLayer(), IDENTIFICATION_FOOD, false, false, false, false, UID_BASE_REACT_EAT+10+deltaLayer, 1);
      brainWiz.AddVisualIdentificationBonus ( attractLevel, pPreyList[i]->getForm(), pPreyList[i]->getColorType(), pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), false, false, false, IDENTIFICATION_FOOD);
    }
    else
    {
      attractLevel = 20 + getRandInt(m_Variation)/10 - numPrey - i;
      if (pPreyList[i]->getOdor() >= ODOR_FIRST_TYPE)
      {
        brainWiz.AddOdorBonus (attractLevel, pPreyList[i]->getOdor(), UID_BASE_REACT_ATTACK, 100);
        brainWiz.AddOdorIdentificationBonus(attractLevel, pPreyList[i]->getOdor(), IDENTIFICATION_PREY);
        brainWiz.AddOdorIdentificationBonus(attractLevel, ODOR_REPULSIVE, IDENTIFICATION_FOOD);
      }

      attractLevel = 20 + getRandInt(m_Variation)/10 - numPrey - i;
      brainWiz.AddVisualBonus ( attractLevel, pPreyList[i]->getLayer(), pPreyList[i]->getForm(), pPreyList[i]->getColorType(), 
                                pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), false, false, UID_BASE_REACT_EAT+10+deltaLayer, 1);
      brainWiz.AddVisualIdentifyBonus (attractLevel, pPreyList[i]->getLayer(), IDENTIFICATION_FOOD, false, false, false, false, UID_BASE_REACT_EAT+10+deltaLayer, 1);

      attractLevel = 30 + getRandInt(m_Variation)/10 - numPrey - i;

      brainWiz.AddVisualIdentificationBonus ( attractLevel, pPreyList[i]->getForm(), pPreyList[i]->getColorType(), pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), false, false, false, IDENTIFICATION_FOOD);
      brainWiz.AddVisualIdentificationBonus ( attractLevel, pPreyList[i]->getForm(), pPreyList[i]->getColorType(), pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), true, false, false, IDENTIFICATION_PREY);

      brainWiz.AddVisualBonus ( attractLevel, pPreyList[i]->getLayer(), pPreyList[i]->getForm(), pPreyList[i]->getColorType(), 
                                pPreyList[i]->getTexture(), pPreyList[i]->getMainPhyAttribute(), true, true, UID_BASE_REACT_ATTACK, 100);
      brainWiz.AddVisualIdentifyBonus (attractLevel, pPreyList[i]->getLayer(), IDENTIFICATION_PREY, true, true, true, true, UID_BASE_REACT_ATTACK, 1);
      
    }
  }

  // avoid to take big rocks as prey
  brainWiz.AddVisualIdentificationBonus ( -10, FORM_ROUNDISH, COLOR_CARACTER_GREY, TEXTURE_NONE, PHY_ATTRIBUTE_NONE, false, true, false, IDENTIFICATION_PREY);

  // Loop on predators
  CBasicEntity **pPredList;
  int numPred = pTabBehavior->GetPredatorEntityList(&pPredList);

  for (i=0; i<numPred; i++)
  {
    attractLevel = 64 + getRandInt(m_Variation)/10 - numPred - i;
    if (pPredList[i]->getOdor() >= ODOR_FIRST_TYPE)
    {
      brainWiz.AddOdorBonus (attractLevel, pPredList[i]->getOdor(), UID_BASE_REACT_MOVEFORWARD + 12, 200);
      brainWiz.AddOdorIdentificationBonus(attractLevel, pPredList[i]->getOdor(), IDENTIFICATION_PREDATOR);
    }

    brainWiz.AddVisualIdentificationBonus ( attractLevel, pPredList[i]->getForm(), pPredList[i]->getColorType(), pPredList[i]->getTexture(), pPredList[i]->getMainPhyAttribute(), true, false, false, IDENTIFICATION_PREDATOR);
    attractLevel = -90 - getRandInt(m_Variation)/10 + numPrey + i;
    brainWiz.AddVisualBonus ( attractLevel, pPredList[i]->getLayer(), pPredList[i]->getForm(), pPredList[i]->getColorType(), 
                              pPredList[i]->getTexture(), pPredList[i]->getMainPhyAttribute(), true, true);
    brainWiz.AddVisualIdentifyBonus (attractLevel, pPredList[i]->getLayer(), IDENTIFICATION_PREDATOR, true, true, true, true);
  }

  if (pTabFemale->m_ComboConsum.GetCurSel() >= CONSUM_OMNI)
  {
    brainWiz.AddOdorBonus ( 30, ODOR_REPULSIVE, UID_BASE_REACT_EAT, 100);
    brainWiz.AddOdorBonus (-60, ODOR_REPULSIVE, UID_BASE_REACT_ATTACK, 100);
  }

  // Default behavior for ODOR_NAUSEOUS (should change for carrion eaters !)
  brainWiz.AddOdorBonus ( 40, ODOR_NAUSEOUS, UID_BASE_REACT_MOVEFORWARD, 11);
  brainWiz.AddOdorBonus ( 10, ODOR_NAUSEOUS, UID_BASE_REACT_TURNLEFT, 1);
  brainWiz.AddOdorBonus ( 10, ODOR_NAUSEOUS, UID_BASE_REACT_TURNRIGHT, 1);
  brainWiz.AddOdorIdentificationBonus(10, ODOR_NAUSEOUS, IDENTIFICATION_DANGER);

  if (pTabBehavior->m_ComboSocial.GetCurSel() == 0)
  {
    // Give more sexual and mother attraction for solitaries
    brainWiz.AddDefaultSexualBonus (52);
    brainWiz.AddDefaultMammalBonus (52);
  }
  else
  {
    brainWiz.AddDefaultSexualBonus (40);
    brainWiz.AddDefaultMammalBonus (40);
  }

  brainWiz.ApplyDefaultBonusOnBrain();
  brainWiz.ApplyBonusListOnBrain();
}

bool CDialogTab4::copy_file(char* src_filename, char* new_filename)
{
  FILE* ptr_old, * ptr_new;
  int  a;

  ptr_old = fopen(src_filename, "rb");
  if (ptr_old == NULL)
    return  false;

  ptr_new = fopen(new_filename, "wb");
  if (ptr_new == NULL)
  {
    fclose(ptr_old);
    return  false;
  }

  while (1)
  {
    a = fgetc(ptr_old);

    if (!feof(ptr_old))
      fputc(a, ptr_new);
    else
      break;
  }

  fclose(ptr_new);
  fclose(ptr_old);
  return  true;
}

bool CDialogTab4::PrepareLearningFilesMammal(CString newFolder)
{
  bool resu;
  CString tmpFileName;

  // Copy xml
  tmpFileName = newFolder + "\\rock.xml";
  resu = copy_file("..\\DataScriptMammal\\rock.xml", tmpFileName.GetBuffer(0));
  tmpFileName = newFolder + "\\water_puddle.xml";
  resu = copy_file("..\\DataScriptMammal\\water_puddle.xml", tmpFileName.GetBuffer(0));
  tmpFileName = newFolder + "\\wooden_fence.xml";
  resu = copy_file("..\\DataScriptMammal\\wooden_fence.xml", tmpFileName.GetBuffer(0));

  // Copy bio
  tmpFileName = newFolder + "\\biotop_empty.bio";
  resu = copy_file("..\\DataScriptMammal\\biotop_empty.bio", tmpFileName.GetBuffer(0));
  tmpFileName = newFolder + "\\biotop_river.bio";
  resu = copy_file("..\\DataScriptMammal\\biotop_river.bio", tmpFileName.GetBuffer(0));
  tmpFileName = newFolder + "\\biotop_prairie.bio";
  resu = copy_file("..\\DataScriptMammal\\biotop_prairie.bio", tmpFileName.GetBuffer(0));

  // Copy scp
  tmpFileName = newFolder + "\\babyLearning.scp";
  resu = copy_file("..\\DataScriptMammal\\babyLearning.scp", tmpFileName.GetBuffer(0));
  tmpFileName = newFolder + "\\childLearning.scp";
  resu = copy_file("..\\DataScriptMammal\\childLearning.scp", tmpFileName.GetBuffer(0));
  tmpFileName = newFolder + "\\adultLearning.scp";
  resu = copy_file("..\\DataScriptMammal\\adultLearning.scp", tmpFileName.GetBuffer(0));


  return resu;
}

void CDialogTab4::OnButtonSaveEntity() 
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CDialogTab1* pTabFemale = pView->GetTabCaractFemale();
  bool resu;

  UpdateData(true);

  if (m_pEntity==NULL)
  {
    AfxMessageBox("Generate a genome before please");
    return;
  }

  // Open module
  CString fileName;
  CString fileNameWithPath;
  CString entityName;
  CString specieName;
  CString strSection,strData;
  CString initFileName = m_pEntity->getLabel().c_str();
  CString learningFolderName;
  CString tempFileName;

  CFileDialog fileDlg(false,"xml",initFileName,0,"Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||");
  fileDlg.m_ofn.lpstrTitle = "Select entity";
  long nResp = fileDlg.DoModal(); 
  if (nResp == IDOK)
  {
    fileName = fileDlg.GetFileName();	
    fileNameWithPath = fileDlg.GetPathName();

    // If name has changed, rename entity
    entityName = fileName;
    int endName = entityName.ReverseFind ('.');
    if (endName>0)
      entityName = entityName.Left(endName); 

    specieName = m_pEntity->getGenome()->getSpecieName().c_str();

    learningFolderName = "..\\..\\DataTraining_" + specieName;

    if (m_pEntity->isAnimal())
    {
      _mkdir(learningFolderName.GetBuffer(0));
    }

    // Prepare training directory for animals
    if (m_pEntity->getClass() == CLASS_MAMMAL)
    {
      PrepareLearningFilesMammal(learningFolderName);
      ((CAnimal*)m_pEntity)->setBrainInstinctInGenes();
    }

    if (!m_pEntity->isAnimal() || !m_IsAutoLearning)
    {
      m_pEntity->setLabel(entityName.GetBuffer(0));
      if (m_pEntity->isMineral())
      {
        // For minerals, delete genome to have smaller file
        m_pEntity->deleteGenome();
      }
      else
      {
        m_pEntity->quickAgeing(m_Age);
      }

      bool resu = m_pEntity->saveInXmlFile(fileNameWithPath.GetBuffer(0));
      if (!resu)
      {
        AfxMessageBox("Impossible to save file " + fileName);
      }
    }
    else
    {
      Point_t coord;
      coord.x = 10;
      coord.y = 10;
      // Prepare temporary files to be used by script
      m_pEntity->setLabel("newEntity");
      m_pEntity->setDefaultLayer(pTabFemale->m_Layer);
      m_pEntity->jumpToGridCoord(coord, true, pTabFemale->m_Layer);

      tempFileName = learningFolderName + "\\newEntity.xml";
      remove(tempFileName.GetBuffer(0));
      resu = m_pEntity->saveInXmlFile(tempFileName.GetBuffer(0));

      CGenome* pMotherGenome = new CGenome(*(m_pEntity->getGenome()));
      pMotherGenome->setSexFemale(m_SexualChrom_Idx, pView->IsSexualDimorphism());
      CBasicEntity* pMother = CEntityFactory::createEntity("mother", pMotherGenome);
      PrepareBrainInstinct((CAnimal*)pMother);
      ((CAnimal*)pMother)->setBrainInstinctInGenes();
      pMother->quickAgeing(50); // FRED TBD: force age to adult according to tab1 ages
      if (pMother->getClass() == CLASS_MAMMAL)
        ((CAnimMammal*)pMother)->deliverAllBabies();
      coord.x = 20;
      coord.y = 20;
      pMother->jumpToGridCoord(coord, true, pTabFemale->m_Layer);
      pMother->setDefaultLayer(pTabFemale->m_Layer);

      tempFileName = learningFolderName + "\\mother.xml";   
      remove(tempFileName.GetBuffer(0));
      pMother->saveInXmlFile(tempFileName.GetBuffer(0));

      // Get preys and predators
      CBasicEntity **pPreyList;
      CBasicEntity **pPredList;
      CDialogTab3* pTabBehavior = pView->GetTabBehavior();
      int numPrey = pTabBehavior->GetPreyEntityList(&pPreyList,false);
      int numPred = pTabBehavior->GetPredatorEntityList(&pPredList);
      int i;
      CMineral emptyEntity("obstacle", coord, 2, NULL);

      for (i=0;i<3;i++)
      {
        CString tmpName;
        tmpName.Format("prey%d", i);
        tempFileName = learningFolderName + "\\" + tmpName + ".xml";
        remove(tempFileName);
        if (pPreyList[i] != NULL)
        {
          pPreyList[i]->setLabel(tmpName.GetBuffer(0));
          pPreyList[i]->saveInXmlFile(tempFileName.GetBuffer(0));
        }
        else
        {
          emptyEntity.setLabel(tmpName.GetBuffer(0));
          emptyEntity.saveInXmlFile(tempFileName.GetBuffer(0));
        }
      }
      for (i=0;i<3;i++)
      {
        CString tmpName;
        CString tmpFileName;
        tmpName.Format("predator%d", i);
        tempFileName = learningFolderName + "\\" + tmpName + ".xml";
        remove(tmpFileName);
        if (pPredList[i] != NULL)
        {
          pPredList[i]->setLabel(tmpName.GetBuffer(0));
          pPredList[i]->saveInXmlFile(tempFileName.GetBuffer(0));
        }
        else
        {
          emptyEntity.setLabel(tmpName.GetBuffer(0));
          emptyEntity.saveInXmlFile(tempFileName.GetBuffer(0));
        }
      }      

      CBiotop tmpBiotop(1,1,1);


      if (m_NbIterations > 0)
      {
        m_ScoreDisplay = "Starting training for entity... \t\t";
        UpdateData(false);
        AfxMessageBox("Press OK to start training. Be patient it can be long...");
      }

      for (i=0; i<m_NbIterations; i++)
      {
        CString tmpStr;
        CScenarioPlayer player(&tmpBiotop);
       
        tempFileName = learningFolderName + "\\";
        player.ReadScenarioFile("babyLearning.scp", tempFileName.GetBuffer(0));
        player.PlayScenario();
        if (player.GetTotalScore()>0)
        {
          tmpStr.Format("%d", player.GetSuccessScore() * 100 / player.GetTotalScore());
          m_ScoreDisplay += "\tScen1:" + tmpStr + "\% ";
          UpdateData(false);
        }

        tempFileName = learningFolderName + "\\";
        player.ReadScenarioFile("childLearning.scp", tempFileName.GetBuffer(0));
        player.PlayScenario();
        if (player.GetTotalScore()>0)
        {
          tmpStr.Format("%d", player.GetSuccessScore() * 100 / player.GetTotalScore());
          m_ScoreDisplay += "\tScen2: " + tmpStr + "\% ";
          UpdateData(false);
        }

        tempFileName = learningFolderName + "\\";
        player.ReadScenarioFile("adultLearning.scp", tempFileName.GetBuffer(0));
        player.PlayScenario();
        if (player.GetTotalScore()>0)
        {
          tmpStr.Format("%d", player.GetSuccessScore() * 100 / player.GetTotalScore());
          m_ScoreDisplay += "\tScen3: " + tmpStr + "\% ";
          UpdateData(false);
        }
      }

      if (m_NbIterations > 0)
      {
        m_ScoreDisplay += "\tTraining for entity complete";
        UpdateData(false);
      }

      // Now, load the trained brain and save entity
      tempFileName = learningFolderName + "\\newEntity.xml";
      m_pEntity->getBrain()->loadFromXmlFile(tempFileName.GetBuffer(0));
      m_pEntity->setLabel(entityName.GetBuffer(0));
      m_pEntity->quickAgeing(m_Age);
      ((CAnimal*)m_pEntity)->setBrainInstinctInGenes();
      resu = m_pEntity->saveInXmlFile(fileNameWithPath.GetBuffer(0));
      if (!resu)
      {
        AfxMessageBox("Impossible to save file " + fileName);
      }
    }
  }
}
