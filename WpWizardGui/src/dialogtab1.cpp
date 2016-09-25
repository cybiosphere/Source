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

// DialogTab1.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectWizard.h"
#include "DialogTab1.h"
#include "ProjectWizardDoc.h"
#include "ProjectWizardView.h"
#include "CBiotop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProjectWizardApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogTab1 dialog

CDialogTab1::CDialogTab1(CWnd* pParent, bool bHideHeader)
: CDialog(CDialogTab1::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDialogTab1)
  m_EditBoxSpecieName = _T("");
  m_LifeExpectency = 0;
  m_DurationStage1 = 5;
  m_ColorName = _T("");
  m_DecompositionTime = 0;
  m_DurationStage2 = 15;
  m_DurationStage3 = 60;
  m_DurationStage4 = 20;
  m_DurationStage5 = 5;
  m_DurationStage6 = 10;
  m_Weight = 0;
  m_Layer = 0;
  m_bSexualDimorphism = FALSE;
	m_WeightMin = 0.0;
	//}}AFX_DATA_INIT
  m_color = 0x00FFFFFF;
  m_bInitDone = false;
  m_bHideHeader = bHideHeader;
  m_weightParamId = -1;
  m_growthSpeedParamId = -1;
  m_growthSpeed = 0;
  
  for (int i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    m_pParamSlider[i] = NULL;
  }
}

CDialogTab1::~CDialogTab1()
{
  for (int i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    if (m_pParamSlider[i] != NULL)
    {
      delete m_pParamSlider[i];
      m_pParamSlider[i] = NULL;
    }
  }
}

void CDialogTab1::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDialogTab1)
	DDX_Control(pDX, IDC_EDIT_WEIGHT_MIN, m_EditWeightMin);
  DDX_Control(pDX, IDC_EDIT_WEIGHT, m_EditWeight);
  DDX_Control(pDX, IDC_CHECK_DIMORPH, m_Chk_SexualDimCtrl);
  DDX_Control(pDX, IDC_EDIT_LAYER, m_EditLayerCtrl);
  DDX_Control(pDX, IDC_EDIT_NAME, m_EditBoxSpecieCtrl);
  DDX_Control(pDX, IDC_RADIO1, m_RadioTypeCtrl1);
  DDX_Control(pDX, IDC_RADIO2, m_RadioTypeCtrl2);
  DDX_Control(pDX, IDC_RADIO3, m_RadioTypeCtrl3);
  DDX_Control(pDX, IDC_SPIN6, m_Spin6);
  DDX_Control(pDX, IDC_SPIN5, m_Spin5);
  DDX_Control(pDX, IDC_SPIN4, m_Spin4);
  DDX_Control(pDX, IDC_SPIN3, m_Spin3);
  DDX_Control(pDX, IDC_SPIN2, m_Spin2);
  DDX_Control(pDX, IDC_COMBO_TEXTURE, m_ComboTexture);
  DDX_Control(pDX, IDC_COMBO_TASTE, m_ComboTaste);
  DDX_Control(pDX, IDC_COMBO_REPRO, m_ComboReproduct);
  DDX_Control(pDX, IDC_COMBO_MOVT, m_ComboMvt);
  DDX_Control(pDX, IDC_COMBO_HABITAT, m_ComboHabitat);
  DDX_Control(pDX, IDC_COMBO_CONSUM, m_ComboConsum);
  DDX_Control(pDX, IDC_COMBO_ATTRIBUTE, m_ComboAttribut);
  DDX_Control(pDX, IDC_SPIN1, m_Spin1);
  DDX_Control(pDX, IDC_COMBO_ODOR, m_ComboOdor);
  DDX_Control(pDX, IDC_COMBO_FORM, m_ComboForm);
  DDX_Control(pDX, IDC_BUTTON_COLOR, m_ColorButton);
  DDX_Control(pDX, IDC_COMBO_CLASS, m_ComboClass);
  DDX_Text(pDX, IDC_EDIT_NAME, m_EditBoxSpecieName);
  DDX_Text(pDX, IDC_EDIT_LIFE_EXP, m_LifeExpectency);
  DDX_Text(pDX, IDC_EDIT1, m_DurationStage1);
  DDV_MinMaxUInt(pDX, m_DurationStage1, 0, 3650);
  DDX_Text(pDX, IDC_EDIT_COLOR_NAME, m_ColorName);
  DDX_Text(pDX, IDC_EDIT_DECOMP, m_DecompositionTime);
  DDX_Text(pDX, IDC_EDIT2, m_DurationStage2);
  DDV_MinMaxUInt(pDX, m_DurationStage2, 0, 3650);
  DDX_Text(pDX, IDC_EDIT3, m_DurationStage3);
  DDV_MinMaxUInt(pDX, m_DurationStage3, 0, 3650);
  DDX_Text(pDX, IDC_EDIT4, m_DurationStage4);
  DDV_MinMaxUInt(pDX, m_DurationStage4, 0, 3650);
  DDX_Text(pDX, IDC_EDIT5, m_DurationStage5);
  DDV_MinMaxUInt(pDX, m_DurationStage5, 0, 3650);
  DDX_Text(pDX, IDC_EDIT6, m_DurationStage6);
  DDV_MinMaxUInt(pDX, m_DurationStage6, 0, 3650);
  DDX_Text(pDX, IDC_EDIT_WEIGHT, m_Weight);
  DDX_Text(pDX, IDC_EDIT_LAYER, m_Layer);
  DDV_MinMaxUInt(pDX, m_Layer, 0, 10);
  DDX_Check(pDX, IDC_CHECK_DIMORPH, m_bSexualDimorphism);
	DDX_Text(pDX, IDC_EDIT_WEIGHT_MIN, m_WeightMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTab1, CDialog)
//{{AFX_MSG_MAP(CDialogTab1)
ON_BN_CLICKED(IDC_RADIO1, OnRadioMineral)
ON_BN_CLICKED(IDC_RADIO2, OnRadioVegetal)
ON_BN_CLICKED(IDC_RADIO3, OnRadioAnimal)
ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
ON_WM_PAINT()
ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
ON_CBN_SELCHANGE(IDC_COMBO_CLASS, OnSelchangeComboClass)
ON_BN_CLICKED(IDC_CHECK_DIMORPH, OnCheck1)
ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
ON_WM_MOUSEMOVE()
	ON_EN_CHANGE(IDC_EDIT_WEIGHT, OnChangeEditWeight)
ON_WM_LBUTTONUP()
ON_WM_DRAWITEM()
ON_WM_MEASUREITEM()
ON_WM_CAPTURECHANGED()
ON_WM_CANCELMODE()
	ON_EN_CHANGE(IDC_EDIT_WEIGHT_MIN, OnChangeEditWeightMin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDialogTab1::InitView(void)
{
  if (m_bHideHeader)
  {
    m_ComboClass.EnableWindow(false);
    m_RadioTypeCtrl1.EnableWindow(false);
    m_RadioTypeCtrl2.EnableWindow(false);
    m_RadioTypeCtrl3.EnableWindow(false);
    m_EditBoxSpecieCtrl.EnableWindow(false);
    m_EditLayerCtrl.EnableWindow(false);
    m_Chk_SexualDimCtrl.EnableWindow(false);
  }
  else
  {
    m_RadioTypeCtrl1.SetCheck(true);
    OnRadioMineral();
  }
  
  // ATTRIBUTES 
  
  // init form
  int index = 0;
  m_ComboForm.ResetContent();
  for (FormType_e i=FORM_UNKNOWN; i<FORM_NUMBER_TYPE; i=(FormType_e)(i+1))
  {
    index = m_ComboForm.AddString(CBasicEntity::getFormStrName(i).c_str());
    m_ComboForm.SetItemData(index,(DWORD)i);
  }
  m_ComboForm.SetCurSel(0);
  
  // init odor
  index = 0;
  m_ComboOdor.ResetContent();
  for (OdorType_e j=ODOR_NONE; j<ODOR_NUMBER_TYPE; j=(OdorType_e)(j+1))
  {
    index = m_ComboOdor.AddString(CBasicEntity::getOdorStrName(j).c_str());
    m_ComboOdor.SetItemData(index,(DWORD)j);
  }
  m_ComboOdor.SetCurSel(0);
  
  // init taste
  index = 0;
  m_ComboTaste.ResetContent();
  for (TasteType_e k=TASTE_NONE; k<TASTE_NUMBER_TYPE; k=(TasteType_e)(k+1))
  {
    index = m_ComboTaste.AddString(CBasicEntity::getTasteStrName(k).c_str());
    m_ComboTaste.SetItemData(index,(DWORD)k);
  }
  m_ComboTaste.SetCurSel(0);
  
  // init texture
  index = 0;
  m_ComboTexture.ResetContent();
  for (TextureType_e l=TEXTURE_NONE; l<TEXTURE_NUMBER_TYPE; l=(TextureType_e)(l+1))
  {
    index = m_ComboTexture.AddString(CBasicEntity::getTextureStrName(l).c_str());
    m_ComboTexture.SetItemData(index,(DWORD)l);
  }
  m_ComboTexture.SetCurSel(0);
  
  // init attribut
  index = 0;
  m_ComboAttribut.ResetContent();
  for (PhyAttributeType_e m=PHY_ATTRIBUTE_NONE; m<PHY_ATTRIBUTE_NUMBER_TYPE; m=(PhyAttributeType_e)(m+1))
  {
    index = m_ComboAttribut.AddString(CBasicEntity::getPhyAttributeStrName(m).c_str());
    m_ComboAttribut.SetItemData(index,(DWORD)m);
  }
  m_ComboAttribut.SetCurSel(0);
  
  // init Habitat
  index = 0;
  m_ComboHabitat.ResetContent();
  for (HabitatType_e n=HABITAT_FIRST_TYPE; n<HABITAT_NUMBER_TYPE; n=(HabitatType_e)(n+1))
  {
    index = m_ComboHabitat.AddString(CBasicEntity::getHabitatStrName(n).c_str());
    m_ComboHabitat.SetItemData(index,(DWORD)n);
  }
  m_ComboHabitat.SetCurSel(0);
  
  // init Reproduction
  index = 0;
  m_ComboReproduct.ResetContent();
  for (ReproType_e o=REPRODUCT_NONE; o<REPRODUCT_NUMBER_TYPE; o=(ReproType_e)(o+1))
  {
    index = m_ComboReproduct.AddString(CBasicEntity::getReproStrName(o).c_str());
    m_ComboReproduct.SetItemData(index,(DWORD)o);
  }
  m_ComboReproduct.SetCurSel(0);
  
  // init Movement
  index = 0;
  m_ComboMvt.ResetContent();
  for (MoveType_e p=MOVE_NONE; p<MOVE_NUMBER_TYPE; p=(MoveType_e)(p+1))
  {
    index = m_ComboMvt.AddString(CBasicEntity::getMoveStrName(p).c_str());
    m_ComboMvt.SetItemData(index,(DWORD)p);
  }
  m_ComboMvt.SetCurSel(0);
  
  // init Consume
  index = 0;
  m_ComboConsum.ResetContent();
  for (ConsumeType_e q=CONSUM_NONE; q<CONSUM_NUMBER_TYPE; q=(ConsumeType_e)(q+1))
  {
    index = m_ComboConsum.AddString(CBasicEntity::getConsumeStrName(q).c_str());
    m_ComboConsum.SetItemData(index,(DWORD)q);
  }
  m_ComboConsum.SetCurSel(0);
  
  
  // LIFE DURATION
  
  m_Spin1.SetBase(10);
  m_Spin1.SetRange(0,3650);
  m_Spin1.SetPos(m_DurationStage1);
  
  m_Spin2.SetBase(10);
  m_Spin2.SetRange(0,3650);
  m_Spin2.SetPos(m_DurationStage2);
  
  m_Spin3.SetBase(10);
  m_Spin3.SetRange(0,3650);
  m_Spin3.SetPos(m_DurationStage3);
  
  m_Spin4.SetBase(10);
  m_Spin4.SetRange(0,3650);
  m_Spin4.SetPos(m_DurationStage4);
  
  m_Spin5.SetBase(10);
  m_Spin5.SetRange(0,3650);
  m_Spin5.SetPos(m_DurationStage5);
  
  m_Spin6.SetBase(10);
  m_Spin6.SetRange(0,3650);
  m_Spin6.SetPos(m_DurationStage6);
  
  m_bInitDone = true;
}

/////////////////////////////////////////////////////////////////////////////
// CDialogTab1 message handlers

void CDialogTab1::OnRadioMineral() 
{
  int index = 0;
  m_ComboClass.ResetContent();
  for (ClassType_e i=CLASS_MINERAL_FIRST;i<=CLASS_MINERAL_LAST;i=(ClassType_e)(i+1))
  {
    index = m_ComboClass.AddString(CGenome::getClassStrName(i).c_str());
    m_ComboClass.SetItemData(index,(DWORD)i);
  }
  m_ComboClass.SetCurSel(0);
  m_ComboClass.EnableWindow(true);
  m_Chk_SexualDimCtrl.SetCheck(false);
  OnCheck1();
  m_Chk_SexualDimCtrl.EnableWindow(false);
  m_EditWeight.EnableWindow(true);
  m_EditWeightMin.EnableWindow(false);
  OnSelchangeComboClass();
  OnChangeEditWeight();

// Default values for combo
  m_ComboForm.SetCurSel(FORM_ROUNDISH);
  m_ComboOdor.SetCurSel(ODOR_NONE);
  m_ComboTaste.SetCurSel(TASTE_NONE);
  m_ComboTexture.SetCurSel(TEXTURE_NONE);
  m_ComboAttribut.SetCurSel(PHY_ATTRIBUTE_NONE);
  m_ComboHabitat.SetCurSel(HABITAT_EARTH);
  m_ComboReproduct.SetCurSel(REPRODUCT_NONE);
  m_ComboMvt.SetCurSel(MOVE_NONE);
  m_ComboConsum.SetCurSel(CONSUM_NONE);

  m_Layer = 2;
  UpdateData(false);
}

void CDialogTab1::OnRadioVegetal() 
{
  int index = 0;
  m_ComboClass.ResetContent();
  for (ClassType_e i=CLASS_VEGETAL_FIRST;i<=CLASS_VEGETAL_LAST;i=(ClassType_e)(i+1))
  {
    index = m_ComboClass.AddString(CGenome::getClassStrName(i).c_str());
    m_ComboClass.SetItemData(index,(DWORD)i);
  }

  m_ComboClass.SelectString(0, CGenome::getClassStrName(CLASS_SPERMATOPHYTA).c_str());
  m_ComboClass.EnableWindow(true);
  m_Chk_SexualDimCtrl.EnableWindow(true);
  m_EditWeight.EnableWindow(false);
  m_EditWeightMin.EnableWindow(true);
  OnSelchangeComboClass();
  OnChangeEditWeightMin();

// Default values for combo
  m_ComboForm.SetCurSel(FORM_VEGET_LONG_LEEF);
  m_ComboOdor.SetCurSel(ODOR_AROMATIC);
  m_ComboTaste.SetCurSel(TASTE_BITTER);
  m_ComboTexture.SetCurSel(TEXTURE_NONE);
  m_ComboAttribut.SetCurSel(PHY_ATTRIBUTE_NONE);
  m_ComboHabitat.SetCurSel(HABITAT_EARTH);
  m_ComboReproduct.SetCurSel(REPRODUCT_SEXUAL);
  m_ComboMvt.SetCurSel(MOVE_NONE);
  m_ComboConsum.SetCurSel(CONSUM_NONE);

  m_Layer = 1;
  UpdateData(false);
}

void CDialogTab1::OnRadioAnimal() 
{
  int index = 0;
  m_ComboClass.ResetContent();
  for (ClassType_e i=CLASS_ANIMAL_FIRST;i<=CLASS_ANIMAL_LAST;i=(ClassType_e)(i+1))
  {
    index = m_ComboClass.AddString(CGenome::getClassStrName(i).c_str());
    m_ComboClass.SetItemData(index,(DWORD)i);
  }
  m_ComboClass.SelectString(0, CGenome::getClassStrName(CLASS_MAMMAL).c_str());
  m_ComboClass.EnableWindow(true);
  m_Chk_SexualDimCtrl.EnableWindow(true);
  m_EditWeight.EnableWindow(false);
  m_EditWeightMin.EnableWindow(true);
  OnSelchangeComboClass();

// Default values for combo
  m_ComboForm.SetCurSel(FORM_QUADRIPED_FAT);
  m_ComboOdor.SetCurSel(ODOR_HIRCINE);
  m_ComboTaste.SetCurSel(TASTE_SALTY);
  m_ComboTexture.SetCurSel(TEXTURE_NONE);
  m_ComboAttribut.SetCurSel(PHY_ATTRIBUTE_NONE);
  m_ComboHabitat.SetCurSel(HABITAT_EARTH);
  m_ComboReproduct.SetCurSel(REPRODUCT_SEXUAL);
  m_ComboMvt.SetCurSel(MOVE_WALK);
  m_ComboConsum.SetCurSel(CONSUM_VEGETAL);

  m_Layer = 2;
  UpdateData(false);
}

void CDialogTab1::OnButtonColor() 
{
  CColorDialog colorDlg;
  colorDlg.DoModal();
  m_color = colorDlg.GetColor();
  m_ColorName = CBasicEntity::getColorStrName(CBasicEntity::convertRgbColorInCaracter(m_color)).c_str();
  UpdateData(false);
  RedrawWindow();
}

void CDialogTab1::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
  CRect rect(13,95,73,115);
  dc.FillSolidRect(rect,m_color);
  
  CBrush brush(0x00FFFFFF);
  CBrush brushgrey(0x00888888);
  CRect frameRect(13,380,733,520);
  COLORREF black = 0x00000000;
  
  dc.FillRect(frameRect,&brush);
  dc.Draw3dRect(frameRect, black, 0x00BBBBBB);
  CPen penBlack(PS_SOLID,1,black);
  dc.SelectObject(&penBlack);
  dc.MoveTo(frameRect.left, frameRect.bottom-5);
  dc.LineTo(frameRect.left+5, frameRect.bottom-5);
  
  if (GetpEntity() != NULL) 
  {
    CBasicEntity* pTempEntity = NULL;
    CGenome* pTempGenome = new CGenome(*GetpEntity()->getGenome());
    pTempEntity = CBiotop::createEntity("titi", pTempGenome);
    
    UpdateData(true);
    
    int stepX = 720/m_LifeExpectency;
    double stepY = 150.0/m_Weight;
    
    if (m_growthSpeedParamId > -1)
    {
      double curGrowthSpeed = GetpEntity()->getParameter(m_growthSpeedParamId)->getVal();
      pTempEntity->getParameter(m_growthSpeedParamId)->setValNominal(curGrowthSpeed);
      pTempEntity->getParameter(m_growthSpeedParamId)->setVal(curGrowthSpeed);
    }

    if (m_weightParamId>-1)
    {
      double minweight = GetpEntity()->getParameter(m_weightParamId)->getMin();
	  pTempEntity->getParameter(m_weightParamId)->setValMin(minweight);
      double maxweight = GetpEntity()->getParameter(m_weightParamId)->getMax();
	  pTempEntity->getParameter(m_weightParamId)->setValMax(maxweight);  
      pTempEntity->getParameter(m_weightParamId)->setVal(minweight);
      stepY = 3;
    }

    pTempEntity->quickAgeing(1); 
    dc.MoveTo(frameRect.left, frameRect.bottom - 5 - stepY*pTempEntity->getWeight());
    
    for (int i=0;i<m_LifeExpectency;i++)
    {
      pTempEntity->quickAgeing(1);     
      dc.LineTo(frameRect.left + stepX*i, frameRect.bottom - 5 - stepY*pTempEntity->getWeight());
    }
    
    if (m_growthSpeedParamId > -1)
    {
      m_Weight = pTempEntity->getWeight() * 1.4; // By default, 10% fat. max 50%
      UpdateData(false);
    }
    
    delete pTempEntity;
  }
}

BOOL CDialogTab1::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  // TODO: Add extra initialization here
  InitView();
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


/*void CDialogTab1::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// TODO: Add your control notification handler code here
m_DurationLifeStage1 = m_Spin1.GetPos();
*pResult = 0;
}*/

void CDialogTab1::OnChangeEdit1() 
{
  if (m_bInitDone)
    UpdateData(true);
  
  m_LifeExpectency = m_DurationStage1 + m_DurationStage2 + m_DurationStage3 + m_DurationStage4;
  
  if (m_bInitDone)
    UpdateData(false);
}

void CDialogTab1::OnChangeEdit2() 
{
  if (m_bInitDone)
    UpdateData(true);
  
  m_LifeExpectency = m_DurationStage1 + m_DurationStage2 + m_DurationStage3 + m_DurationStage4;
  
  if (m_bInitDone)
    UpdateData(false);	
}

void CDialogTab1::OnChangeEdit3() 
{
  if (m_bInitDone)
    UpdateData(true);
  
  m_LifeExpectency = m_DurationStage1 + m_DurationStage2 + m_DurationStage3 + m_DurationStage4;
  
  if (m_bInitDone)
    UpdateData(false);
}

void CDialogTab1::OnChangeEdit4() 
{
  if (m_bInitDone)
    UpdateData(true);
  
  m_LifeExpectency = m_DurationStage1 + m_DurationStage2 + m_DurationStage3 + m_DurationStage4;
  
  if (m_bInitDone)
    UpdateData(false);
}

void CDialogTab1::OnChangeEdit5() 
{
  if (m_bInitDone)
    UpdateData(true);
  
  m_DecompositionTime = m_DurationStage5 + m_DurationStage6;
  
  if (m_bInitDone)
    UpdateData(false);
}

void CDialogTab1::OnChangeEdit6() 
{
  if (m_bInitDone)
    UpdateData(true);
  
  m_DecompositionTime = m_DurationStage5 + m_DurationStage6;
  
  if (m_bInitDone)
    UpdateData(false);	
}

void CDialogTab1::OnSelchangeComboClass() 
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  CBasicEntity* pEntity = NULL;
  
  ClassType_e selectedClass = (ClassType_e)m_ComboClass.GetItemData(m_ComboClass.GetCurSel());
  CGenome* pTempGenome = new CGenome(selectedClass,/*m_EditBoxSpecieName.GetBuffer(0)*/ "titi");
  
  pEntity = CBiotop::createEntity("toto", pTempGenome);
  pView->SetpEntity(pEntity, m_bSexualDimorphism);
  
}

CBasicEntity* CDialogTab1::GetpEntity()
{
  CProjectWizardView* pView = (CProjectWizardView*)GetParentFrame()->GetActiveView();
  return pView->GetpEntity();
}


void CDialogTab1::DisplayParamSliders()
{
  int i;
  
  CBasicEntity* pEntity = GetpEntity();
  
  // Clean previous paramCtrl
  m_weightParamId = -1;
  m_growthSpeedParamId = -1;
  for (i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    if (m_pParamSlider[i] != NULL)
    {
      m_pParamSlider[i]->ShowWindow(SW_HIDE);
      delete m_pParamSlider[i];
      m_pParamSlider[i] = NULL;
    }
  }
  
  if (pEntity!=NULL)
  {
    // Set new params
    int startX = 230;
    int startY = 110;
    int count = 0;
    CString label;
    
    // Display PARAM_PHYSIC (except weight)
    for (i=0;(count<MAX_NUMBER_PARAMETER_DISPLAY)&&(i<pEntity->getNumParameter());i++)
    {
      label = pEntity->getParameter(i)->getLabel().c_str();
      if ( (pEntity->getParameter(i)->getType() == PARAM_PHYSIC) && (label.Find("eight") == -1) )
      {
        if (count == MAX_NUMBER_PARAMETER_DISPLAY/3)
        {
          startX +=175;
          startY = 110;
          count = 0;
        }
        m_pParamSlider[count] = new CParameterSlider();
        CRect rect(startX,startY,startX+160,startY+8);
        m_pParamSlider[count]->CreateNewParam(pEntity->getParameter(i),rect,this,1003);
        startY +=32;
        count ++;
      }
      
      if ( (pEntity->getParameter(i)->getType() == PARAM_PHYSIC) && (label == "Weight") )
      {
        m_weightParamId = i;
        m_Weight    = cybio_round(pEntity->getParameter(i)->getMax());
        m_WeightMin = cybio_round(pEntity->getParameter(i)->getMin());
        UpdateData(false);
      }
      else if (label == "Growth speed rate")
      {
        m_growthSpeedParamId = i;
      }
    }
    startX +=175;
    startY = 110;
    
    // Display PARAM_REPRODUCTION
    for (i=0;(count<MAX_NUMBER_PARAMETER_DISPLAY)&&(i<pEntity->getNumParameter());i++)
    {
      if (pEntity->getParameter(i)->getType() == PARAM_REPRODUCTION)
      {
        m_pParamSlider[count] = new CParameterSlider();
        CRect rect(startX,startY,startX+160,startY+8);
        m_pParamSlider[count]->CreateNewParam(pEntity->getParameter(i),rect,this,1003);
        startY +=32;
        count ++;
      }
    }
  }
}

void CDialogTab1::OnCheck1() 
{
  // TODO: Add your control notification handler code here
  UpdateData(true);
  OnSelchangeComboClass();
  
}

void CDialogTab1::OnChangeEditName() 
{
  if (m_bInitDone)
    UpdateData(true);	
}

void CDialogTab1::OnMouseMove(UINT nFlags, CPoint point) 
{
  if ( (m_growthSpeedParamId>-1) && (abs(GetpEntity()->getParameter(m_growthSpeedParamId)->getVal() - m_growthSpeed) > 0.1) )
  {
    m_growthSpeed = GetpEntity()->getParameter(m_growthSpeedParamId)->getVal();
    RedrawWindow();
  }
  
  CDialog::OnMouseMove(nFlags, point);
}



void CDialogTab1::OnChangeEditWeight() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

    UpdateData(true);
    if (m_weightParamId>-1)
    {
	  GetpEntity()->getParameter(m_weightParamId)->setValMax(m_Weight);
      GetpEntity()->getParameter(m_weightParamId)->setValMin(m_Weight);
      m_WeightMin = m_Weight;
      UpdateData(false);
      RedrawWindow();
    }
}

void CDialogTab1::OnChangeEditWeightMin() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
    UpdateData(true);
    if (m_weightParamId>-1)
    {
	  GetpEntity()->getParameter(m_weightParamId)->setValMin(m_WeightMin);
      GetpEntity()->getParameter(m_weightParamId)->setValMax(200*m_WeightMin);
      RedrawWindow();
    }
	
}
