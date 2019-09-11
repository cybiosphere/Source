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
#include "BiotopEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBiotopEditorDlg dialog


CBiotopEditorDlg::CBiotopEditorDlg(CBiotop* pBiotop,CWnd* pParent /*=NULL*/)
	: CDialog(CBiotopEditorDlg::IDD, pParent)
{
  m_pBiotop = pBiotop;

  for (int i=0;i<MAX_NUMBER_BIOCTRL_DISPLAY;i++)
  {
    m_pParamSlider[i] = NULL;
  }

  for (int i = 0; i < MAX_NUMBER_RANDOM_ENTITY_GENERATOR; i++)
  {
    m_pRandomGeneratorButtonTable[i] = NULL;
    m_pRandomGeneratorParamsTable[i] = NULL;
  }
}

CBiotopEditorDlg::~CBiotopEditorDlg()
{
  for (int i=0;i<MAX_NUMBER_BIOCTRL_DISPLAY;i++)
  {
    if (m_pParamSlider[i] != NULL)
    {
      delete m_pParamSlider[i];
      m_pParamSlider[i] = NULL;
    }
  }

  for (int i = 0; i < MAX_NUMBER_RANDOM_ENTITY_GENERATOR; i++)
  {
    if (m_pRandomGeneratorButtonTable[i] != NULL)
    {
      delete m_pRandomGeneratorButtonTable[i];
      m_pRandomGeneratorButtonTable[i] = NULL;
    }
    if (m_pRandomGeneratorParamsTable[i] != NULL)
    {
      delete m_pRandomGeneratorParamsTable[i];
      m_pRandomGeneratorParamsTable[i] = NULL;
    }
  }
}

void CBiotopEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBiotopEditorDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBiotopEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CBiotopEditorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBiotopEditorDlg message handlers


BOOL CBiotopEditorDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  // TODO: Add extra initialization here
  HICON saveIco = theApp.LoadIcon(IDI_ICON_SAVE); 
  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN); 

  DisplayParamSliders();

  // Custom entity list
  int iDpi = GetDpiForWindow(this->m_hWnd);
  int startX = MulDiv(20, iDpi, 96);
  int startParamX = MulDiv(300, iDpi, 96);
  int startY = MulDiv(360, iDpi, 96);
  int offsetY = MulDiv(40, iDpi, 96);
  int sizeX = MulDiv(220, iDpi, 96);
  int sizeParamX = MulDiv(60, iDpi, 96);
  int sizeY = MulDiv(20, iDpi, 96);

  for (int i = 0; i < MAX_NUMBER_RANDOM_ENTITY_GENERATOR; i++)
  {
    CString label;
    label.Format("%d", i + 1);
    m_pRandomGeneratorButtonTable[i] = new CFileSelectButton((CView*)this, label, "Select");
    
    if (m_pRandomGeneratorButtonTable[i] != NULL)
    {
      CRect rect(startX, startY, startX + sizeX, startY + sizeY);
      m_pRandomGeneratorButtonTable[i]->CreateNewButton(rect, this, 2000 + i);

      BiotopRandomEntitiyGeneration_t& entityGeneration = m_pBiotop->getRandomEntitiyGeneration(i);
      m_pRandomGeneratorParamsTable[i] = new CCheckBoxWithParams((CView*)this, entityGeneration.IsProportionalToFertility, 
                                                        "period", entityGeneration.avaragePeriodicity, "intensity", entityGeneration.intensity);
      CRect rectParams(startParamX, startY, startParamX + sizeParamX, startY + sizeY);
      m_pRandomGeneratorParamsTable[i]->CreateNewButton(rectParams, this, 2010 + i);
      m_pRandomGeneratorParamsTable[i]->SetWindowTextA("fertility");
      startY += offsetY;
      m_pRandomGeneratorButtonTable[i]->initWithDefaultFile(entityGeneration.entityPathName.c_str(), entityGeneration.entityFileName.c_str());
    }
  }

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBiotopEditorDlg::DisplayParamSliders()
{
  int i;

  // Clean previous paramCtrl
  for (i=0;i<MAX_NUMBER_BIOCTRL_DISPLAY;i++)
  {
    if (m_pParamSlider[i] != NULL)
    {
      delete m_pParamSlider[i];
      m_pParamSlider[i] = NULL;
    }
  }

  if (m_pBiotop!=NULL)
  {
    m_pParamSlider[0] = new CParameterSlider();
    CRect rect(40,50,200,58);
    m_pParamSlider[0]->CreateNewParam(m_pBiotop->getParamFertility(),rect,this,1004);

    m_pParamSlider[1] = new CParameterSlider();
    rect.top += 32;
    rect.bottom += 32;
    m_pParamSlider[1]->CreateNewParam(m_pBiotop->getParamSunlight(),rect,this,1004);

    m_pParamSlider[2] = new CParameterSlider();
    rect.top += 32;
    rect.bottom += 32;
    m_pParamSlider[2]->CreateNewParam(m_pBiotop->getParamRadioactivity(),rect,this,1004);

    m_pParamSlider[3] = new CParameterSlider(true,true);
    rect.top += 32;
    rect.bottom += 32;
    m_pParamSlider[3]->CreateNewParam(m_pBiotop->getParamTemperature(),rect,this,1004);
  }
}

void CBiotopEditorDlg::OnOK()
{
  for (int i = 0; i < MAX_NUMBER_RANDOM_ENTITY_GENERATOR; i++)
  {
    if (m_pRandomGeneratorButtonTable[i]->GetFileName() != "")
    {
      BiotopRandomEntitiyGeneration_t& entityGeneration = m_pBiotop->getRandomEntitiyGeneration(i);
      entityGeneration.entityFileName = m_pRandomGeneratorButtonTable[i]->GetFileName();
      entityGeneration.entityPathName = m_pRandomGeneratorButtonTable[i]->GetPathName();
      entityGeneration.IsProportionalToFertility = m_pRandomGeneratorParamsTable[i]->GetCheck();
      CString param1Str, param2Str;
      m_pRandomGeneratorParamsTable[i]->m_pParam1Edit->GetWindowText(param1Str);
      m_pRandomGeneratorParamsTable[i]->m_pParam2Edit->GetWindowText(param2Str);
      entityGeneration.avaragePeriodicity = atoi(param1Str);
      entityGeneration.intensity = atoi(param2Str);
    }
  }
  CDialog::OnOK();
}