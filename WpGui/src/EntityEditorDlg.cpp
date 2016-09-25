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
#include "EntityEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEntityEditorDlg dialog


CEntityEditorDlg::CEntityEditorDlg(CBasicEntity* pEntity,CWnd* pParent /*=NULL*/)
	: CDialog(CEntityEditorDlg::IDD, pParent)
{
  m_pEntity = pEntity;

  for (int i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    m_pParamSlider[i] = NULL;
  }
}


CEntityEditorDlg::~CEntityEditorDlg()
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

void CEntityEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityEditorDlg)
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CEntityEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityEditorDlg message handlers

void CEntityEditorDlg::OnButtonLoad() 
{
  // Open module
  CString fileName;
  CString pathName;
  CString strSection,strData;

  CFileDialog fileDlg(true, LPCTSTR("xml"), LPCTSTR(""),0, LPCTSTR("Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
  fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select entity");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
  long nResp = fileDlg.DoModal(); 
  if (nResp == IDOK)
  {
    fileName = fileDlg.GetFileName();	
    pathName = fileDlg.GetPathName();	 
    int endPath = pathName.ReverseFind ('\\');
    if (endPath>0)
      pathName = pathName.Left(endPath+1); 

    bool resu = m_pEntity->loadDataFromXmlFile((char*)fileDlg.GetPathName().GetBuffer(0), (char*)pathName.GetBuffer(0));

    if (!resu)
    {
      AfxMessageBox(LPCTSTR("Current entity has a different label from file entity"));
    }

    DisplayParamSliders();
  }
}

void CEntityEditorDlg::OnButtonSave() 
{
  if (m_pEntity == NULL)
    return;

  // Open module
  CString fileName;
  CString pathName;
  CString entityName;
  CString strSection,strData;
  CString initFileName = LPCTSTR(m_pEntity->getLabel().c_str());

  CFileDialog fileDlg(false, LPCTSTR("xml"), initFileName, 0, LPCTSTR("Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
  fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select entity");
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
  long nResp = fileDlg.DoModal(); 
  if (nResp == IDOK)
  {
    /*fileName = fileDlg.GetFileName();	
    pathName = fileDlg.GetPathName();	 
    int endPath = pathName.ReverseFind ('\\');
    if (endPath>0)
      pathName = pathName.Left(endPath+1); 

    // If name has changed, rename entity
    entityName = fileName;
    int endName = entityName.ReverseFind ('.');
    if (endName>0)
      entityName = entityName.Left(endName); 

    m_pEntity->setLabel(entityName.GetBuffer(0));
    bool resu = m_pEntity->saveInXmlFile(fileName.GetBuffer(0), pathName.GetBuffer(0));*/
 
    // If name has changed, rename entity
    entityName = fileDlg.GetFileName();
    int endName = entityName.ReverseFind ('.');
    if (endName>0)
      entityName = entityName.Left(endName); 
    
    bool resu = m_pEntity->saveInXmlFile((char*)fileDlg.GetPathName().GetBuffer(0), (char*)entityName.GetBuffer(0));

    if (!resu)
    {
      AfxMessageBox(LPCTSTR("Impossible to save file ") + fileName);
    }
  }
}

BOOL CEntityEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  HICON saveIco = theApp.LoadIcon(IDI_ICON_SAVE); 
  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN); 
  m_ButtonSave.SetIcon(saveIco);
  m_ButtonLoad.SetIcon(loadIco);
  
  DisplayParamSliders();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEntityEditorDlg::DisplayParamSliders()
{
  int i;

  // Clean previous paramCtrl
  for (i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    if (m_pParamSlider[i] != NULL)
    {
      delete m_pParamSlider[i];
      m_pParamSlider[i] = NULL;
    }
  }

  if (m_pEntity!=NULL)
  {
    // Set new params
    int startX = 40;
    int startY = 50;
    for (i=0;(i<MAX_NUMBER_PARAMETER_DISPLAY)&&(i<m_pEntity->getNumParameter());i++)
    {
      m_pParamSlider[i] = new CParameterSlider();
      CRect rect(startX,startY,startX+160,startY+8);
      m_pParamSlider[i]->CreateNewParam(m_pEntity->getParameter(i),rect,this,1003);
      startY +=32;
      if (i==MAX_NUMBER_PARAMETER_DISPLAY/2)
      {
        startX +=180;
        startY = 50;
      }
    }
  }
}