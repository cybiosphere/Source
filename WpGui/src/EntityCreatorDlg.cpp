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

// EntityEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "EntityCreatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEntityCreatorDlg dialog


CEntityCreatorDlg::CEntityCreatorDlg(CWnd* pParent, int coordX, int coordY, int layer)
	: CDialog(CEntityCreatorDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CEntityCreatorDlg)
  m_EditBoxName = _T("");
  m_EditBoxSpecieName = _T("");
  //}}AFX_DATA_INIT

  m_StartCoordX = coordX;
  m_StartCoordY = coordY;
  m_StartLayer  = layer;

  m_pTempNewEntity = NULL;

  m_GenomeEditRequired = true;
  m_OpenedFileName = "";
  m_OpenedDirectoryName = "";
  m_pTempGenome = NULL;
}

CEntityCreatorDlg::~CEntityCreatorDlg()
{
  if (m_pTempGenome!=NULL)
    delete m_pTempGenome;
}


void CEntityCreatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityCreatorDlg)
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDC_COMBO_CLASS, m_ComboClass);
	DDX_Text(pDX, IDC_ENTITY_NAME, m_EditBoxName);
	DDX_Text(pDX, IDC_SPECIE_NAME, m_EditBoxSpecieName);
	DDX_Text(pDX, IDC_EDIT_CX, m_StartCoordX);
	DDX_Text(pDX, IDC_EDIT_CY, m_StartCoordY);
    DDX_Text(pDX, IDC_EDIT_LAYER, m_StartLayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityCreatorDlg, CDialog)
	//{{AFX_MSG_MAP(CEntityCreatorDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioMineral)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioVegetal)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioAnimal)
	ON_EN_CHANGE(IDC_SPECIE_NAME, OnChangeSpecieName)
	ON_CBN_DROPDOWN(IDC_COMBO_CLASS, OnDropdownComboClass)
	ON_EN_CHANGE(IDC_ENTITY_NAME, OnChangeEntityName)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityCreatorDlg message handlers

void CEntityCreatorDlg::OnRadioMineral() 
{
  int index = 0;
	m_ComboClass.ResetContent();
  for (ClassType_e i=CLASS_MINERAL_FIRST;i<=CLASS_MINERAL_LAST;i=(ClassType_e)(i+1))
  {
    index = m_ComboClass.AddString(LPCTSTR(CGenome::getClassStrName(i).c_str()));
    m_ComboClass.SetItemData(index,(DWORD_PTR)i);
  }
  m_ComboClass.SetCurSel(0);
  m_ComboClass.EnableWindow(true);
}

void CEntityCreatorDlg::OnRadioVegetal() 
{
  int index = 0;
	m_ComboClass.ResetContent();
  for (ClassType_e i=CLASS_VEGETAL_FIRST;i<=CLASS_VEGETAL_LAST;i=(ClassType_e)(i+1))
  {
    index = m_ComboClass.AddString(LPCTSTR(CGenome::getClassStrName(i).c_str()));
    m_ComboClass.SetItemData(index,(DWORD_PTR)i);
  }
  m_ComboClass.SetCurSel(0);
  m_ComboClass.EnableWindow(true);
}

void CEntityCreatorDlg::OnRadioAnimal() 
{
  int index = 0;
	m_ComboClass.ResetContent();
  for (ClassType_e i=CLASS_ANIMAL_FIRST;i<=CLASS_ANIMAL_LAST;i=(ClassType_e)(i+1))
  {
    index = m_ComboClass.AddString(LPCTSTR(CGenome::getClassStrName(i).c_str()));
    m_ComboClass.SetItemData(index,(DWORD_PTR)i);
  }
  m_ComboClass.SetCurSel(0);
  m_ComboClass.EnableWindow(true);
}


BOOL CEntityCreatorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboClass.EnableWindow(false);
  m_ButtonOK.EnableWindow(false);

  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN); 
  m_ButtonLoad.SetIcon(loadIco);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CEntityCreatorDlg::CheckSpecieDataRssource()
{
  // FRED TBD
  return(true);
}

bool CEntityCreatorDlg::IsGenomeEditRequired()
{
  return(m_GenomeEditRequired);
}

void CEntityCreatorDlg::OnChangeSpecieName() 
{
  UpdateData(TRUE);
}


void CEntityCreatorDlg::OnDropdownComboClass() 
{
	m_ButtonOK.EnableWindow(true);
}

void CEntityCreatorDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CEntityCreatorDlg::OnChangeEntityName() 
{
	UpdateData(TRUE);
}

void CEntityCreatorDlg::OnOK() 
{
  UpdateData(TRUE);

  ClassType_e selectedClass = (ClassType_e)m_ComboClass.GetItemData(m_ComboClass.GetCurSel());

  if (CheckSpecieDataRssource())
  {
    Point_t coord = {m_StartCoordX,m_StartCoordY};

    if (m_pTempGenome == NULL)
    {
      m_pTempGenome = new CGenome(selectedClass,(char*)m_EditBoxSpecieName.GetBuffer(0));
    }

    m_pTempNewEntity = theApp.GetBiotop()->createAndAddEntity((char*)m_EditBoxName.GetBuffer(0),coord,m_StartLayer,m_pTempGenome);
    m_pTempGenome = NULL;

    if ( (m_GenomeEditRequired==false) && (m_pTempNewEntity != NULL) )
    {
      CString filenameWithPath = m_OpenedDirectoryName + m_OpenedFileName;
      m_pTempNewEntity->loadDataFromXmlFile((char*)filenameWithPath.GetBuffer(0));
      m_pTempNewEntity->loadBrainFromXmlFile((char*)filenameWithPath.GetBuffer(0));
    } 
  }

	CDialog::OnOK();
}

CBasicEntity* CEntityCreatorDlg::GetTempNewEntity(void)
{
  return (m_pTempNewEntity);
}

void CEntityCreatorDlg::OnButtonLoad() 
{
  // Open module
  CString fileName;
  CString strSection,strData;

  CFileDialog fileDlg(true, LPCTSTR("xml"), LPCTSTR(""),0, LPCTSTR("Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
  fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select entity");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
  long nResp = fileDlg.DoModal(); 
  if (nResp == IDOK)
  {
    m_OpenedFileName = fileDlg.GetFileName();
    m_OpenedDirectoryName = fileDlg.GetPathName();	 
    int endPath = m_OpenedDirectoryName.ReverseFind ('\\');
    if (endPath>0)
      m_OpenedDirectoryName = m_OpenedDirectoryName.Left(endPath+1); 
   
    m_GenomeEditRequired = false;
    // delete prev genome
    if (m_pTempGenome!=NULL)
      delete m_pTempGenome;

    m_pTempGenome = new CGenome(CLASS_NONE,"");

    string name;
    CBasicEntity::getGenomeFromXmlFile((char*)fileDlg.GetPathName().GetBuffer(0), *m_pTempGenome);
    CBasicEntity::getDefaultLayerFromXmlFile((char*)fileDlg.GetPathName().GetBuffer(0), m_StartLayer);
    CBasicEntity::getEntityNameFromXmlFile((char*)fileDlg.GetPathName().GetBuffer(0), name);

    //m_ComboClass;
    m_EditBoxName = name.c_str();
    m_EditBoxSpecieName = m_pTempGenome->getOwnClassStrName().c_str();

    UpdateData(FALSE);
    m_ButtonOK.EnableWindow(true);
  }
	
}
