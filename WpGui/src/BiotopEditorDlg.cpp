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

