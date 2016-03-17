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

// DialogTab2.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectWizard.h"
#include "DialogTab2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDialogTab2 dialog


CDialogTab2::CDialogTab2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTab2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTab2)
		// NOTE: the ClassWizard will add member initialization here
  m_bUseVisualIdentification = TRUE;
	//}}AFX_DATA_INIT

    for (int i=0;i<MAX_NUMBER_BUTTONS_SENSOR;i++)
    {
        m_pSensorButtonTable[i] = NULL;
    }
}

CDialogTab2::~CDialogTab2()
{
  for (int i=0;i<MAX_NUMBER_BUTTONS_SENSOR;i++)
  {
    if (m_pSensorButtonTable[i] != NULL)
    {
      delete m_pSensorButtonTable[i];
      m_pSensorButtonTable[i] = NULL;
    }
  }
}

void CDialogTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTab2)
	DDX_Control(pDX, IDC_COMBO1, m_comboViewAngle);
  DDX_Check(pDX, IDC_CHECK_IDENTIFY, m_bUseVisualIdentification);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTab2, CDialog)
	//{{AFX_MSG_MAP(CDialogTab2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTab2 message handlers

BOOL CDialogTab2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_comboViewAngle.SetCurSel(2);

	// TODO: Add extra initialization here
    m_pSensorButtonTable[0]  = new CGeneButton(GENE_SENS_VIEW, (CView*)this, true, 100, "range", 30, "N focus", 8);
    m_pSensorButtonTable[1]  = new CGeneButton(GENE_SENS_SMELL, (CView*)this, true, 100, "range", 30);
    m_pSensorButtonTable[2]  = new CGeneButton(GENE_SENS_PHEROMONE,(CView*)this, true, 200, "range", 10);
    m_pSensorButtonTable[3]  = new CGeneButton(GENE_SENS_EAR,(CView*)this, true, 200, "range", 30);
    m_pSensorButtonTable[4]  = new CGeneButton(GENE_SENS_HUNGER,(CView*)this, true, 100);
    m_pSensorButtonTable[5]  = new CGeneButton(GENE_SENS_THIRST,(CView*)this, true, 200);
    m_pSensorButtonTable[6]  = new CGeneButton(GENE_SENS_STOMACH_LOAD,(CView*)this, true, 500);
    m_pSensorButtonTable[7]  = new CGeneButton(GENE_SENS_PLEASURE,(CView*)this, true, 100);
    m_pSensorButtonTable[8]  = new CGeneButton(GENE_SENS_SUFFER,(CView*)this, true, 200);
    m_pSensorButtonTable[9]  = new CGeneButton(GENE_SENS_TIREDNESS,(CView*)this, true, 500);
    m_pSensorButtonTable[10] = new CGeneButton(GENE_SENS_OBSCURITY,(CView*)this, true, 100);
    m_pSensorButtonTable[11] = new CGeneButton(GENE_SENS_LIBIDO,(CView*)this, true, 100);
    m_pSensorButtonTable[12] = new CGeneButton(GENE_SENS_SPEED,(CView*)this, true, 300);
    m_pSensorButtonTable[13] = new CGeneButton(GENE_SENS_COMPASS,(CView*)this, false, 100);
    m_pSensorButtonTable[14] = new CGeneButton(GENE_SENS_FEAR,(CView*)this, true, 400);
    m_pSensorButtonTable[15] = new CGeneButton(GENE_SENS_TEMPERATURE,(CView*)this, true, 200);
    m_pSensorButtonTable[16] = new CGeneButton(GENE_SENS_HORMONE,(CView*)this, true, 300);
    m_pSensorButtonTable[17] = new CGeneButton(GENE_SENS_ORIENTATION,(CView*)this, true, 300);

    int startX = 20;
    int startY = 40;
    startX = 20;
    CRect rect(startX, startY, startX+130, startY+15);
    m_pSensorButtonTable[0]->CreateNewButton(rect,this,2000);
    startY += 30;

    for (int i=0;i<MAX_NUMBER_BUTTONS_SENSOR-1;i++)
    {
      if (m_pSensorButtonTable[i+1] != NULL)
      {
        startX = 20 + (i&0x01) * 380;
        CRect rect(startX, startY, startX+130, startY+15);
        m_pSensorButtonTable[i+1]->CreateNewButton(rect,this,2000+i);
        startY += (i&0x01) * 30;
      }
    }
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
