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

// CBrainEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "BrainEditorDlg.h"
#include "CEntityFactory.h"
#include "FindStrDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBrainEditorDlg dialog

CBrainEditorDlg::CBrainEditorDlg(CBrain* pBrain, bool identifyBrain, CString userTitle, CWnd* pParent /*=NULL*/)
	: CDialog(CBrainEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrainEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
  m_bColorizeDeltaFromBirth = FALSE;
  m_bColorizeDeltaWithOther = FALSE;
	//}}AFX_DATA_INIT

  m_bIdentifyBrain = identifyBrain;
  m_pBrain = pBrain;
  m_strWindowsTitle = userTitle;

  if (m_bIdentifyBrain == true)
    m_pNeuronMatrix = pBrain->GetIdentifyNeuronTable();
  else
    m_pNeuronMatrix = pBrain->GetDecisionNeuronTable();

  if (m_pNeuronMatrix != NULL)
    m_strBrainBackup = m_pNeuronMatrix->buildStringDataFromNeuronTable();

}


void CBrainEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrainEditorDlg)
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_MORE, m_ButtonMore);
	DDX_Control(pDX, IDC_BUTTON_LESS, m_ButtonLess);
  DDX_Control(pDX, IDC_BUTTON_FINDB, m_ButtonFind);
	DDX_GridControl(pDX, IDC_BRAIN_EDIT_GRID, m_BrainGrid);
  DDX_Check(pDX, IDC_DELTA_CHECK1, m_bColorizeDeltaFromBirth);
  DDX_Check(pDX, IDC_DELTA_CHECK2, m_bColorizeDeltaWithOther);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrainEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CBrainEditorDlg)
  ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_MORE, OnButtonMore)
	ON_BN_CLICKED(IDC_BUTTON_LESS, OnButtonLess)
  ON_BN_CLICKED(IDC_BUTTON_FINDB, OnButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
  ON_BN_CLICKED(IDC_DELTA_CHECK1, OnCheckColorizeDeltaFromBirth)
  ON_BN_CLICKED(IDC_DELTA_CHECK2, OnCheckColorizeDeltaWithOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrainEditorDlg message handlers

BOOL CBrainEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  HICON saveIco = theApp.LoadIcon(IDI_ICON_SAVE); 
  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN); 
  HICON moreIco = theApp.LoadIcon(IDI_ICON_MORE); 
  HICON lessIco = theApp.LoadIcon(IDI_ICON_LESS);
  HICON findIco = theApp.LoadIcon(IDI_ICON_FIND);
  m_ButtonSave.SetIcon(saveIco);
  m_ButtonLoad.SetIcon(loadIco);
  m_ButtonMore.SetIcon(moreIco);
  m_ButtonLess.SetIcon(lessIco);
  m_ButtonFind.SetIcon(findIco);

  if (m_bIdentifyBrain == true)
  {
    m_BrainGrid.SetIdentifyBrain(m_pBrain);
  }
  else
  {
    m_BrainGrid.SetBrain(m_pBrain);
  }

  // Set user title if set
  if (m_strWindowsTitle!="")
    this->SetWindowText(m_strWindowsTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrainEditorDlg::ColorizeDeltaFromReference()
{
  m_BrainGrid.ClearColorizedCells();
  if ((m_pReferenceAnimal != NULL) && (m_pReferenceAnimal->getBrain() != NULL))
  {
    if (m_bIdentifyBrain == true)
    {
      m_BrainGrid.SetReferenceNeuronMatrixToCompare(m_pReferenceAnimal->getBrain()->GetIdentifyNeuronTable());
    }
    else
    {
      m_BrainGrid.SetReferenceNeuronMatrixToCompare(m_pReferenceAnimal->getBrain()->GetDecisionNeuronTable());
    }
    m_BrainGrid.ColorizeDeltaFromReference();
  }
}

void CBrainEditorDlg::OnSize(UINT nType, int cx, int cy)
{
  CDialog::OnSize(nType, cx, cy);

  if (m_BrainGrid.m_hWnd != NULL)
  {
    m_BrainGrid.MoveWindow(10, 50, cx - 20, cy - 60, true);
    UpdateWindow();
    m_BrainGrid.Refresh();
  }
}

void CBrainEditorDlg::OnButtonLoad() 
{
  if (m_pNeuronMatrix==NULL)
    return;

	CString loadedBrain;

    // Open module
    CString fileName;

	CFileDialog fileDlg(true, LPCTSTR("xml"), LPCTSTR(""),0, LPCTSTR("Brain Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
	fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select brain");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    fileName = fileDlg.GetPathName();
    
    bool resu = m_pNeuronMatrix->loadFromXmlFile((char*)(fileName.GetBuffer(0)));
    if (!resu)
    {
      AfxMessageBox(LPCTSTR("Impossible to load file ") + fileName);
    }

    m_BrainGrid.SetNeuronMatrix(m_pNeuronMatrix); // Refresh grid
  }
}

void CBrainEditorDlg::OnButtonSave() 
{
  if (m_pNeuronMatrix==NULL)
    return;

	CString savedBrain;
	savedBrain = m_pNeuronMatrix->buildStringDataFromNeuronTable().c_str();

  // Open module
  CString fileName;

	CFileDialog fileDlg(false, LPCTSTR("xml"), LPCTSTR(""),0, LPCTSTR("Brain Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
	fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select brain");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    fileName = fileDlg.GetPathName();

    bool resu = m_pNeuronMatrix->saveInXmlFile((char*)(fileName.GetBuffer(0)));
    if (!resu)
    {
      AfxMessageBox(LPCTSTR("Impossible to save file ") + fileName);
    }
  }
}

void CBrainEditorDlg::OnButtonMore() 
{
	CCellRange selRange = m_BrainGrid.GetSelectedCellRange();

  for (int i=selRange.GetMinRow(); i<=selRange.GetMaxRow(); i++)
  {
    for (int j=selRange.GetMinCol(); j<=selRange.GetMaxCol(); j++)
    {
      m_pNeuronMatrix->ChangeNeuronTableVal(i-2,j-2,0.01,false);
    }
  }
  m_pNeuronMatrix->NormalizeNeuronMatrix();

  if (m_bIdentifyBrain == true)
    m_BrainGrid.RefreshIdentifyBrainData();
  else
    m_BrainGrid.RefreshBrainData();

  ColorizeDeltaFromReference();
}

void CBrainEditorDlg::OnButtonLess() 
{
	CCellRange selRange = m_BrainGrid.GetSelectedCellRange();

  for (int i=selRange.GetMinRow(); i<=selRange.GetMaxRow(); i++)
  {
    for (int j=selRange.GetMinCol(); j<=selRange.GetMaxCol(); j++)
    {
      m_pNeuronMatrix->ChangeNeuronTableVal(i-2,j-2,-0.01,false);
    }

  }
  m_pNeuronMatrix->NormalizeNeuronMatrix();

  if (m_bIdentifyBrain == true)
    m_BrainGrid.RefreshIdentifyBrainData();
  else
    m_BrainGrid.RefreshBrainData();

  ColorizeDeltaFromReference();
}

void CBrainEditorDlg::OnButtonFind()
{
  CFindStrDlg strDlg;
  long nResp = strDlg.DoModal();
  if (nResp == IDOK)
  {
    m_BrainGrid.SelectRowFromString(strDlg.GetString());
  }
}

void CBrainEditorDlg::OnCancel() 
{
  if (m_pNeuronMatrix!=NULL)
	  m_pNeuronMatrix->buildNeuronTableFromStringData(m_strBrainBackup);
  if (m_pReferenceAnimal!=NULL)
    delete m_pReferenceAnimal;
	CDialog::OnCancel();
}

void CBrainEditorDlg::OnOK()
{
  theApp.updateSelectedEntity(m_pBrain->GetEntity());
  if (m_pReferenceAnimal != NULL)
    delete m_pReferenceAnimal;
  CDialog::OnOK();
}

void CBrainEditorDlg::OnButtonExport() 
{
  CString fileName;
	CFileDialog fileDlg(false, LPCTSTR("csv"), LPCTSTR(""),0, LPCTSTR("Brain Files (*.csv)|*.csv; *.csv|All Files (*.*)|*.*||"));
	fileDlg.m_ofn.lpstrTitle = LPCTSTR("Export in csv file");
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
  {
    fileName = fileDlg.GetPathName();
    if (m_bIdentifyBrain == true)
    {
      m_pBrain->exportIdentificationInCsvFile((char*)fileName.GetBuffer(0));
    }
    else
    {
      m_pBrain->exportDecisionInCsvFile((char*)fileName.GetBuffer(0));
    }
  }
}

void CBrainEditorDlg::OnButtonImport() 
{
  if (m_bIdentifyBrain == true)
  {
    return; // Not supported
  }
  CString fileName;
	CFileDialog fileDlg(true, LPCTSTR("csv"), LPCTSTR(""),0, LPCTSTR("Brain Files (*.csv)|*.csv; *.csv|All Files (*.*)|*.*||"));
	fileDlg.m_ofn.lpstrTitle = LPCTSTR("Export in csv file");
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
  {
    fileName = fileDlg.GetPathName();
    bool resu = m_pBrain->importDecisionFromCsvFile((char*)fileName.GetBuffer(0));
    m_BrainGrid.SetNeuronMatrix(m_pNeuronMatrix); // Refresh grid
  }
}

void CBrainEditorDlg::OnCheckColorizeDeltaFromBirth()
{
  if (m_pReferenceAnimal != NULL)
  {
    delete m_pReferenceAnimal;
    m_pReferenceAnimal = NULL;
  }

  if (m_bColorizeDeltaFromBirth)
  {
    m_bColorizeDeltaFromBirth = FALSE;
  }
  else
  {
    m_bColorizeDeltaFromBirth = TRUE;
    m_pReferenceAnimal = CEntityFactory::createCloneEntity(m_pBrain->GetEntity());
  }
  m_bColorizeDeltaWithOther = FALSE;
  this->UpdateData(false);
  ColorizeDeltaFromReference();
}

void CBrainEditorDlg::OnCheckColorizeDeltaWithOther()
{
  if (m_pReferenceAnimal != NULL)
  {
    delete m_pReferenceAnimal;
    m_pReferenceAnimal = NULL;
  }

  m_bColorizeDeltaFromBirth = FALSE;
  if (m_bColorizeDeltaWithOther)
  {
    m_bColorizeDeltaWithOther = FALSE;
  }
  else
  {
    m_bColorizeDeltaWithOther = TRUE;
    CFileDialog fileDlg(true, LPCTSTR("xml"), LPCTSTR(""), 0, LPCTSTR("Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
    fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select entity");
    long nResp = fileDlg.DoModal();
    if (nResp == IDOK)
    {
     CString fileName = fileDlg.GetFileName();
      m_pReferenceAnimal = CEntityFactory::createEntity(fileName.GetBuffer(0));
    }
  }
  this->UpdateData(false);
  ColorizeDeltaFromReference();
}