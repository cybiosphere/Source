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

// MeasureEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "MeasureEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMeasureEditorDlg dialog


CMeasureEditorDlg::CMeasureEditorDlg(CBiotop* pBiotop,CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureEditorDlg::IDD, pParent)
{
  m_MeasureName = "";
  m_pBiotop = pBiotop;
  m_pCurMeasure = NULL;
  m_pNewMeasure = NULL;
  m_EditPeriod = 10;
  m_EditThreshold = 0;
}


void CMeasureEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureEditorDlg)
	DDX_Control(pDX, IDC_COMBO_EVENT, m_ComboEvent);
	DDX_Control(pDX, IDC_EDIT_PERIOD, m_PeriodEditBox);
	DDX_Control(pDX, IDC_COMBO_SUBTYPE, m_ComboSubtype);
	DDX_Control(pDX, IDC_COMBO_SPECIE, m_ComboSpecie);
	DDX_Control(pDX, IDC_COMBO_REACTION, m_ComboReaction);
	DDX_Control(pDX, IDC_COMBO_ENTITY, m_ComboEntity);
	DDX_Control(pDX, IDC_EDIT_TITLE5, m_Title5);
	DDX_Control(pDX, IDC_EDIT_TITLE4, m_Title4);
	DDX_Control(pDX, IDC_EDIT_TITLE3, m_Title3);
	DDX_Control(pDX, IDC_EDIT_TITLE2, m_Title2);
	DDX_Control(pDX, IDC_EDIT_TITLE1, m_Title1);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ComboType);
	DDX_Control(pDX, IDC_LIST_MEASURES, m_MeasureList);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
	DDX_Text(pDX, IDC_EDIT_NAME, m_MeasureName);
	DDX_Text(pDX, IDC_EDIT_PERIOD, m_EditPeriod);
	DDV_MinMaxLong(pDX, m_EditPeriod, 1, 99999999);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_EditThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasureEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_LBN_SELCHANGE(IDC_LIST_MEASURES, OnSelchangeListMeasures)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_ENTITY, OnSelchangeComboEntity)
	ON_CBN_SELCHANGE(IDC_COMBO_REACTION, OnSelchangeComboReaction)
	ON_CBN_SELCHANGE(IDC_COMBO_SPECIE, OnSelchangeComboSpecie)
	ON_CBN_SELCHANGE(IDC_COMBO_SUBTYPE, OnSelchangeComboSubtype)
	ON_EN_CHANGE(IDC_EDIT_PERIOD, OnChangeEditPeriod)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnButtonAccept)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, OnChangeEditThreshold)
	ON_CBN_SELCHANGE(IDC_COMBO_EVENT, OnSelchangeComboEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureEditorDlg message handlers

void CMeasureEditorDlg::OnButtonLoad() 
{
  // FRED: TBD

}

void CMeasureEditorDlg::OnButtonSave() 
{
  CString fileName;
  CFileDialog fileDlg(false, LPCTSTR("csv"), LPCTSTR(""), 0, LPCTSTR("Brain Files (*.csv)|*.csv; *.csv|All Files (*.*)|*.*||"));
  fileDlg.m_ofn.lpstrTitle = LPCTSTR("Export in csv file");
  long nResp = fileDlg.DoModal();
  if (nResp == IDOK)
  {
    fileName = fileDlg.GetPathName();
    if (m_pBiotop!=NULL)
      m_pBiotop->saveAllMeasuresInFile((char*)fileName.GetBuffer(0));
  }
}

BOOL CMeasureEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  HICON saveIco = theApp.LoadIcon(IDI_ICON_SAVE); 
  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN); 
  m_ButtonSave.SetIcon(saveIco);
  m_ButtonLoad.SetIcon(loadIco);
  
  m_Title1.SetWindowText(LPCTSTR("Period in second"));
  m_Title2.SetWindowText(LPCTSTR("Entity name"));
  m_Title3.SetWindowText(LPCTSTR("Type"));
  m_Title4.SetWindowText(LPCTSTR("Reaction"));
  m_Title5.SetWindowText(LPCTSTR("Specie name"));

  InitMeasureList();

  int index, i;
	m_ComboType.ResetContent();
  for (MeasureType_e id=MEASURE_TYPE_GENERIC; id<MEASURE_NUMBER_TYPE; id=(MeasureType_e)(id+1))
  {
    index = m_ComboType.AddString(LPCTSTR(CMeasure::getMeasureTypeStrName(id).c_str()));
    m_ComboType.SetItemData(index,(DWORD)id);
  }
  m_ComboType.SetCurSel(0);

  RefreshEditSection();

  m_ComboEntity.ResetContent();
  for (i=0; i<m_pBiotop->getNbOfEntities(); i++)
  {
    index = m_ComboEntity.AddString(LPCTSTR(m_pBiotop->getEntityByIndex(i)->getLabel().c_str()));
    m_ComboType.SetItemData(index,(DWORD)m_pBiotop->getEntityByIndex(i)->getId());
  }

  m_ComboSpecie.ResetContent();
  CString specieName;
  for (i=0; i<m_pBiotop->getNbOfEntities(); i++)
  {
    specieName = LPCTSTR(m_pBiotop->getEntityByIndex(i)->getGenome()->getSpecieName().c_str());
    index = m_ComboSpecie.FindStringExact(0,specieName);
    if (index==CB_ERR)
    {  
      m_ComboSpecie.AddString(specieName);
    }
  }

	m_ComboEvent.ResetContent();
  for (EventType_e idEv=EVENT_TYPE_NONE; idEv<EVENT_NUMBER_TYPE; idEv=(EventType_e)(idEv+1))
  {
    index = m_ComboEvent.AddString(LPCTSTR(CMeasure::getEventTypeStrName(idEv).c_str()));
    m_ComboEvent.SetItemData(index,(DWORD)idEv);
  }
  m_ComboEvent.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasureEditorDlg::InitMeasureList()
{
  m_MeasureList.ResetContent();

  CMeasure* pCurMeasure;
  if (m_pBiotop!=NULL)
  {
    for (int i=0; i<MAX_NB_DISPLAY_MEASURES; i++)
    {
      pCurMeasure = m_pBiotop->getMeasureById(i);
      if (pCurMeasure!=NULL)
      {
        m_MeasureList.AddString(LPCTSTR(pCurMeasure->GetLabel().c_str()));
      }
      else
      {
        m_MeasureList.AddString(LPCTSTR("empty"));
      }
    }
  }
}

void CMeasureEditorDlg::OnSelchangeListMeasures() 
{
  int index = m_MeasureList.GetCurSel();

  m_pCurMeasure = m_pBiotop->getMeasureById(index);
  if ( (m_pCurMeasure!=NULL) && (m_pCurMeasure->IsRecording()) )
  {
    m_ComboType.SetCurSel(m_pCurMeasure->GetType());
  }
  else
  {
    m_pCurMeasure = NULL;
    m_ComboType.SetCurSel(0);
  }

  RefreshEditSection();
}

void CMeasureEditorDlg::RefreshEditSection()
{
  MeasureType_e measType = (MeasureType_e)m_ComboType.GetCurSel();

  // Init values
  InitSelComboEntity(m_pCurMeasure);

  // Update combos
  UpdateComboSubType();
  UpdateAndInitComboReaction(m_pCurMeasure);

  // Init values
  if (m_pCurMeasure!=NULL)
  {
    int curSel;
    m_EditPeriod = m_pCurMeasure->GetPeriod();
    curSel = m_pCurMeasure->GetSubTypeId();
    m_ComboSubtype.SetCurSel(curSel);
  }
  else
  {
    m_EditPeriod = 10;
    m_ComboSubtype.SetCurSel(0);
  }

  InitSelComboSpecie(m_pCurMeasure);
  InitEventBoxes(m_pCurMeasure);

  // Prepare controls
  switch (measType)
  {
  case MEASURE_TYPE_GENERIC:
    m_Title1.ShowWindow(SW_HIDE);
    m_Title2.ShowWindow(SW_HIDE);
    m_Title3.ShowWindow(SW_HIDE);
    m_Title4.ShowWindow(SW_HIDE);
    m_Title5.ShowWindow(SW_HIDE);
    m_PeriodEditBox.ShowWindow(SW_HIDE);
		m_ComboSubtype.ShowWindow(SW_HIDE);
		m_ComboEntity.ShowWindow(SW_HIDE);
		m_ComboReaction.ShowWindow(SW_HIDE);
    m_ComboSpecie.ShowWindow(SW_HIDE);
    break;

  case MEASURE_TYPE_PARAMETER_ENTITY:
    m_Title1.ShowWindow(SW_SHOW);
    m_Title2.ShowWindow(SW_SHOW);
    m_Title3.ShowWindow(SW_SHOW);
    m_Title4.ShowWindow(SW_HIDE);
    m_Title5.ShowWindow(SW_HIDE);
    m_PeriodEditBox.ShowWindow(SW_SHOW);
		m_ComboSubtype.ShowWindow(SW_SHOW);
		m_ComboEntity.ShowWindow(SW_SHOW);
		m_ComboReaction.ShowWindow(SW_HIDE);
    m_ComboSpecie.ShowWindow(SW_HIDE);
    break;

  case MEASURE_TYPE_PARAMETER_BIOTOP:
    m_Title1.ShowWindow(SW_SHOW);
    m_Title2.ShowWindow(SW_HIDE);
    m_Title3.ShowWindow(SW_SHOW);
    m_Title4.ShowWindow(SW_HIDE);
    m_Title5.ShowWindow(SW_HIDE);
    m_PeriodEditBox.ShowWindow(SW_SHOW);
		m_ComboSubtype.ShowWindow(SW_SHOW);
		m_ComboEntity.ShowWindow(SW_HIDE);
		m_ComboReaction.ShowWindow(SW_HIDE);
    m_ComboSpecie.ShowWindow(SW_HIDE);
    break;

  case MEASURE_TYPE_FEELING:
    m_Title1.ShowWindow(SW_SHOW);
    m_Title2.ShowWindow(SW_SHOW);
    m_Title3.ShowWindow(SW_SHOW);
    m_Title4.ShowWindow(SW_HIDE);
    m_Title5.ShowWindow(SW_HIDE);
    m_PeriodEditBox.ShowWindow(SW_SHOW);
		m_ComboSubtype.ShowWindow(SW_SHOW);
		m_ComboEntity.ShowWindow(SW_SHOW);
		m_ComboReaction.ShowWindow(SW_HIDE);
    m_ComboSpecie.ShowWindow(SW_HIDE);
    break;

  case MEASURE_TYPE_REACTION:
    m_Title1.ShowWindow(SW_SHOW);
    m_Title2.ShowWindow(SW_SHOW);
    m_Title3.ShowWindow(SW_SHOW);
    m_Title4.ShowWindow(SW_SHOW);
    m_Title5.ShowWindow(SW_HIDE);
    m_PeriodEditBox.ShowWindow(SW_SHOW);
		m_ComboSubtype.ShowWindow(SW_SHOW);
		m_ComboEntity.ShowWindow(SW_SHOW);
		m_ComboReaction.ShowWindow(SW_SHOW);
    m_ComboSpecie.ShowWindow(SW_HIDE);
    break;

  case MEASURE_TYPE_POPULATION:
    m_Title1.ShowWindow(SW_SHOW);
    m_Title2.ShowWindow(SW_HIDE);
    m_Title3.ShowWindow(SW_SHOW);
    m_Title4.ShowWindow(SW_HIDE);   
    m_PeriodEditBox.ShowWindow(SW_SHOW);
		m_ComboSubtype.ShowWindow(SW_SHOW);
		m_ComboEntity.ShowWindow(SW_HIDE);
		m_ComboReaction.ShowWindow(SW_HIDE);
    if ( (m_pCurMeasure!=NULL) && (m_pCurMeasure->GetSubTypeId()==MEASURE_POPULATION_SPECIFIC) )
    {
      m_Title5.ShowWindow(SW_SHOW);
      m_ComboSpecie.ShowWindow(SW_SHOW);
    }
    else
    {
      m_Title5.ShowWindow(SW_HIDE);
      m_ComboSpecie.ShowWindow(SW_HIDE);
    }
    break;

  default:
    m_Title1.ShowWindow(SW_HIDE);
    m_Title2.ShowWindow(SW_HIDE);
    m_Title3.ShowWindow(SW_HIDE);
    m_Title4.ShowWindow(SW_HIDE);
    m_Title5.ShowWindow(SW_HIDE);
    m_PeriodEditBox.ShowWindow(SW_HIDE);
		m_ComboSubtype.ShowWindow(SW_HIDE);
		m_ComboEntity.ShowWindow(SW_HIDE);
		m_ComboReaction.ShowWindow(SW_HIDE);
    m_ComboSpecie.ShowWindow(SW_HIDE);
    break;
  }

  RebuildNewMeasure();
  
}

void CMeasureEditorDlg::OnSelchangeComboType() 
{
	RefreshEditSection();	
}

void CMeasureEditorDlg::InitSelComboEntity (CMeasure* pMeas)
{
  CBasicEntity* pEntity = NULL;

  // Init values
  if (pMeas==NULL)
  {
    m_ComboEntity.SetCurSel(0);
    return;
  }

  switch (pMeas->GetType())
  {
  case MEASURE_TYPE_PARAMETER_ENTITY:
    pEntity = ((CMeasureEntityParam*)pMeas)->GetpEntity();
    m_ComboEntity.SetCurSel( m_pBiotop->getEntityTableIndex(pEntity) );  
    break;

  default:
    // Nothing
    break;

  }
}

void CMeasureEditorDlg::UpdateComboSubType ()
{
  int i;
  m_ComboSubtype.ResetContent();

  switch (m_ComboType.GetCurSel())
  {
  case MEASURE_TYPE_FEELING:
    for (i=0;i<MEASURE_FEELING_NB_TYPE;i++)
    {
      m_ComboSubtype.AddString(LPCTSTR(CMeasureFeeling::GetMeasureFeelingTypeStrName((MeasureFeelingType_e)i).c_str()));
    }
    break;

  case MEASURE_TYPE_REACTION:
    for (i=0;i<MEASURE_ACTION_NB_TYPE;i++)
    {
      m_ComboSubtype.AddString(LPCTSTR(CMeasureReaction::GetMeasureReactionTypeStrName((MeasureReactionType_e)i).c_str()));
    }
    break;

  case MEASURE_TYPE_POPULATION:
    for (i=0;i<MEASURE_POPULATION_NUMBER_TYPE;i++)
    {
      m_ComboSubtype.AddString(LPCTSTR(CMeasurePopulation::GetMeasurePopulationTypeStrName((MeasurePopulationType_e)i).c_str()));
    }
    break;

  case MEASURE_TYPE_PARAMETER_BIOTOP:
    for (i=0;i<BIO_PARAM_NUMBERT_TYPE;i++)
    {
      m_ComboSubtype.AddString(LPCTSTR(m_pBiotop->getParameter(i)->getLabel().c_str()));
    }
    break;

  case MEASURE_TYPE_PARAMETER_ENTITY:
    {
      CBasicEntity* pEntity = NULL;
      pEntity = m_pBiotop->getEntityByIndex( m_ComboEntity.GetCurSel() );
      if (pEntity!=NULL)
      {
        for (i=0; i<pEntity->getNumParameter(); i++)
        {
          m_ComboSubtype.AddString(LPCTSTR(pEntity->getParameter(i)->getLabel().c_str()));
        }
      }
    }
    break;

  case MEASURE_TYPE_GENERIC:
  default:
    // Nothing
    break;
  }
}

void CMeasureEditorDlg::UpdateAndInitComboReaction(CMeasure* pMeas)
{
  int i;
  m_ComboReaction.ResetContent();

  switch (m_ComboType.GetCurSel())
  {
  case MEASURE_TYPE_REACTION:
    {
      CBasicEntity* pEntity = NULL;
      pEntity = m_pBiotop->getEntityByIndex( m_ComboEntity.GetCurSel() );
      if (pEntity!=NULL)
      {
        for (i=0;i<pEntity->getBrain()->GetNumberReaction();i++)
        {
          m_ComboReaction.AddString(LPCTSTR(pEntity->getBrain()->GetReactionByIndex(i)->GetLabel().c_str()));
        }

        if (pMeas==NULL)
        {
          m_ComboReaction.SetCurSel(0);
        }
        else
        {
          m_ComboReaction.SetCurSel(((CMeasureReaction*)pMeas)->GetReactionIndex());
        }
      }
    }
    break;
  default:
    // Nothing
    break;
  }
}

void CMeasureEditorDlg::InitSelComboSpecie (CMeasure* pMeas)
{
  // Init values anyway
  m_ComboSpecie.SetCurSel(0);
  if (pMeas==NULL)
    return;

  switch (pMeas->GetType())
  {
  case MEASURE_TYPE_POPULATION:
    if (pMeas->GetSubTypeId()==MEASURE_POPULATION_SPECIFIC)
    {
      m_ComboSpecie.SelectString(0, LPCTSTR(((CMeasurePopulation*)pMeas)->GetSpecieName().c_str()));
    }
    break;

  default:
    // Nothing
    break;

  }
}

void CMeasureEditorDlg::InitEventBoxes(CMeasure* pMeas)
{
  // Init values anyway
  m_ComboEvent.SetCurSel(0);
  m_EditThreshold = 0;
  if (pMeas==NULL)
    return;

  m_ComboEvent.SetCurSel(pMeas->GetEventType());
  m_EditThreshold = pMeas->GetEventThreshold();

}

void CMeasureEditorDlg::RebuildNewMeasure()
{
  // Delete previous measure:
  if (m_pNewMeasure != NULL)
  {
    delete (m_pNewMeasure);
    m_pNewMeasure = NULL;
  }

  // build new measure
  int index = m_MeasureList.GetCurSel();

  switch (m_ComboType.GetCurSel())
  {
  case MEASURE_TYPE_PARAMETER_ENTITY:
    {
      CBasicEntity* pEntity = m_pBiotop->getEntityByIndex(m_ComboEntity.GetCurSel());
      m_pNewMeasure = new CMeasureEntityParam( pEntity, m_ComboSubtype.GetCurSel(), m_EditPeriod, index); 
    }
    break;

  case MEASURE_TYPE_PARAMETER_BIOTOP:
    {
      m_pNewMeasure = new CMeasureBiotopParam( m_pBiotop, m_ComboSubtype.GetCurSel(), m_EditPeriod, index); 
    }
    break;

  case MEASURE_TYPE_FEELING:
    {
      CBasicEntity* pEntity = m_pBiotop->getEntityByIndex(m_ComboEntity.GetCurSel());
      MeasureFeelingType_e measureType = (MeasureFeelingType_e)m_ComboSubtype.GetCurSel();
      m_pNewMeasure = new CMeasureFeeling( pEntity, m_EditPeriod, index, measureType);  
    }
    break;

  case MEASURE_TYPE_REACTION:
    {
      CBasicEntity* pEntity = m_pBiotop->getEntityByIndex(m_ComboEntity.GetCurSel());
      MeasureReactionType_e measureType = (MeasureReactionType_e)m_ComboSubtype.GetCurSel();
      m_pNewMeasure = new CMeasureReaction( pEntity, m_ComboReaction.GetCurSel(), m_EditPeriod, index, measureType);  
    }
    break;

  case MEASURE_TYPE_POPULATION:
    {
      CString specieName;
      int combInd = m_ComboSpecie.GetCurSel();
      m_ComboSpecie.GetLBText(combInd,specieName);
      MeasurePopulationType_e measureType = (MeasurePopulationType_e)m_ComboSubtype.GetCurSel();
      m_pNewMeasure = new CMeasurePopulation( m_pBiotop, m_EditPeriod, index, measureType, 100, (char*)specieName.GetBuffer(0)); 
    }
    break;

  default:
    // Nothing
    break;
  }

  m_pCurMeasure = m_pNewMeasure;

  if (m_pCurMeasure!=NULL)
  {
    m_MeasureName = m_pCurMeasure->GetLabel().c_str();
    m_pCurMeasure->SetEvent((EventType_e)m_ComboEvent.GetCurSel(), m_EditThreshold);
  }
  else
  {
    m_MeasureName = "";
  }  

  UpdateData(false);
}

void CMeasureEditorDlg::OnSelchangeComboEntity() 
{
  UpdateComboSubType();
  m_ComboSubtype.SetCurSel(0);
	RebuildNewMeasure();	
}

void CMeasureEditorDlg::OnSelchangeComboReaction() 
{
  RebuildNewMeasure();
}

void CMeasureEditorDlg::OnSelchangeComboSpecie() 
{
  RebuildNewMeasure();	
}

void CMeasureEditorDlg::OnSelchangeComboSubtype() 
{
  RebuildNewMeasure();
  RefreshEditSection();
}

void CMeasureEditorDlg::OnChangeEditPeriod() 
{
  UpdateData(true);
  RebuildNewMeasure();
}

void CMeasureEditorDlg::OnButtonAccept() 
{
	m_pBiotop->replaceMeasure(m_MeasureList.GetCurSel(), m_pNewMeasure);
  // Reset New Measure to avoid deletion
  m_pNewMeasure = NULL;
  InitMeasureList();
}

void CMeasureEditorDlg::OnChangeEditThreshold() 
{
  UpdateData(true);
	RebuildNewMeasure();
}

void CMeasureEditorDlg::OnSelchangeComboEvent() 
{
	RebuildNewMeasure();
}
