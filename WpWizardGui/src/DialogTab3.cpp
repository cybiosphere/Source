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

// DialogTab3.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectWizard.h"
#include "DialogTab3.h"
#include "CBrainAnimal.h"
#include "CAnimMammal.h"
#include "CBiotop.h"
#include "CWater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTab3 dialog


CDialogTab3::CDialogTab3(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTab3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTab3)
		// NOTE: the ClassWizard will add member initialization here
    m_bBabiesStayHome = FALSE;
	//}}AFX_DATA_INIT
    int i;

    for (i=0;i<MAX_NUMBER_BUTTONS_ACTION;i++)
    {
        m_pActionButtonTable[i] = NULL;
    }

    for (i=0;i<MAX_NUMBER_BUTTONS_PREY+1;i++)
    {
        m_pPreyButtonTable[i] = NULL;
        m_pPreyEntities[i] = NULL;
    }

    for (i=0;i<MAX_NUMBER_BUTTONS_PREDATOR;i++)
    {
        m_pPredatorButtonTable[i] = NULL;
        m_pPredatorEntities[i] = NULL;
    }
}

CDialogTab3::~CDialogTab3()
{
  int i;

  for (i=0;i<MAX_NUMBER_BUTTONS_ACTION;i++)
  {
    if (m_pActionButtonTable[i] != NULL)
    {
      delete m_pActionButtonTable[i];
      m_pActionButtonTable[i] = NULL;
    }
  }

  for (i=0;i<MAX_NUMBER_BUTTONS_PREY+1;i++)
  {
    if (m_pPreyButtonTable[i] != NULL)
    {
      delete m_pPreyButtonTable[i];
      m_pPreyButtonTable[i] = NULL;
    }
    if (m_pPreyEntities[i] != NULL)
    {
      delete m_pPreyEntities[i];
      m_pPreyEntities[i] = NULL;
    }
  }

  for (i=0;i<MAX_NUMBER_BUTTONS_PREDATOR;i++)
  {
    if (m_pPredatorButtonTable[i] != NULL)
    {
      delete m_pPredatorButtonTable[i];
      m_pPredatorButtonTable[i] = NULL;
    }
    if (m_pPredatorButtonTable[i] != NULL)
    {
      delete m_pPredatorEntities[i];
      m_pPredatorEntities[i] = NULL;
    }
  }
}


void CDialogTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTab3)
	DDX_Control(pDX, IDC_COMBO4, m_ComboBehavior);
	DDX_Control(pDX, IDC_COMBO3, m_ComboHierarchy);
	DDX_Control(pDX, IDC_COMBO2, m_ComboSocial);
	DDX_Control(pDX, IDC_COMBO1, m_ComboLifeMode);
  DDX_Check(pDX, IDC_CHECK_BABY_HOME, m_bBabiesStayHome);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTab3, CDialog)
	//{{AFX_MSG_MAP(CDialogTab3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTab3 message handlers

BOOL CDialogTab3::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ComboBehavior.SetCurSel(1);
	m_ComboHierarchy.SetCurSel(0);
	m_ComboSocial.SetCurSel(1);
	m_ComboLifeMode.SetCurSel(0);

    // Action list
    m_pActionButtonTable[0]  = new CGeneButton(GENE_REACT_TURNLEFT,(CView*)this,true,0);
    m_pActionButtonTable[1]  = new CGeneButton(GENE_REACT_TURNRIGHT,(CView*)this,true,0);
    m_pActionButtonTable[2]  = new CGeneButton(GENE_REACT_TURNHEADLEFT,(CView*)this,true,0);
    m_pActionButtonTable[3]  = new CGeneButton(GENE_REACT_TURNHEADRIGHT,(CView*)this,true,0);
    m_pActionButtonTable[4]  = new CGeneButton(GENE_REACT_NOTHING,(CView*)this,true,0); //Def param used as successRate
    m_pActionButtonTable[5]  = new CGeneButton(GENE_REACT_DRINK,(CView*)this,true,0,"success",10,"failure",80);
    m_pActionButtonTable[6]  = new CGeneButton(GENE_REACT_EAT,(CView*)this,true,0,"success",10,"failure",60);
    m_pActionButtonTable[7]  = new CGeneButton(GENE_REACT_ATTACK,(CView*)this,false,0,"success",40,"failure",50);
    m_pActionButtonTable[8]  = new CGeneButton(GENE_REACT_RUMINATE,(CView*)this,false,0);
    m_pActionButtonTable[9]  = new CGeneButton(GENE_REACT_COPULATE,(CView*)this,true,0,"success",60,"failure",50);
    m_pActionButtonTable[10] = new CGeneButton(GENE_REACT_HIDE,(CView*)this,false,0);
    m_pActionButtonTable[11] = new CGeneButton(GENE_REACT_STEPBACKWARD,(CView*)this,false,0);
    m_pActionButtonTable[12] = new CGeneButton(GENE_REACT_SLEEP,(CView*)this,true,0);

    int i;
    int startX = 20;
    int startY = 30;
    for (i=0;i<MAX_NUMBER_BUTTONS_ACTION;i++)
    {
      if (m_pActionButtonTable[i] != NULL)
      {
        startX = 20 + (i&0x01) * 380;
        CRect rect(startX, startY, startX+130, startY+15);
        m_pActionButtonTable[i]->CreateNewButton(rect,this,2000+i);
        startY += (i&0x01) * 30;
      }
    }

    // Prey list
    startX = 20;
    startY = 390;
    for (i=0;i<MAX_NUMBER_BUTTONS_PREY;i++)
    {
      CString label;
      label.Format("%d",i+1);
      m_pPreyButtonTable[i]  = new CFileSelectButton((CView*)this, label, "Select");

      if (m_pPreyButtonTable[i] != NULL)
      {
        CRect rect(startX, startY, startX+140, startY+20);
        m_pPreyButtonTable[i]->CreateNewButton(rect,this,2000+i);
        startY += 40;
      }
    }

    // Predator list
    startX = 390;
    startY = 390;
    for (i=0;i<MAX_NUMBER_BUTTONS_PREDATOR;i++)
    {
      CString label;
      label.Format("%d",i+1);
      m_pPredatorButtonTable[i]  = new CFileSelectButton((CView*)this, label, "Select");

      if (m_pPredatorButtonTable[i] != NULL)
      {
        CRect rect(startX, startY, startX+140, startY+20);
        m_pPredatorButtonTable[i]->CreateNewButton(rect,this,2100+i);
        startY += 40;
      }
    }


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


bool CDialogTab3::LoadSensorsAndPredatorsEntities()
{
  int i;
  CGenome* pTempGenome;
  int count=0;

  for (i=0;i<MAX_NUMBER_BUTTONS_PREY+1;i++)
  {
    if (m_pPreyEntities[i] != NULL)
    {
      delete m_pPreyEntities[i];
      m_pPreyEntities[i] = NULL;
    }
  }

  for (i=0;i<MAX_NUMBER_BUTTONS_PREDATOR;i++)
  {
    if (m_pPredatorButtonTable[i] != NULL)
    {
      delete m_pPredatorEntities[i];
      m_pPredatorEntities[i] = NULL;
    }
  }

  for (i=0;i<MAX_NUMBER_BUTTONS_PREY;i++)
  {
    if ( (m_pPreyButtonTable[i] != NULL) && (m_pPreyButtonTable[i]->GetFileName() != "") )
    {
        CString fileName = m_pPreyButtonTable[i]->GetFileName();
        CString pathName = m_pPreyButtonTable[i]->GetPathName();
        CString fileNameWithPath = pathName + fileName;
        string name;
        int startLayer;
        pTempGenome = new CGenome(CLASS_NONE,"");
        CBasicEntity::getGenomeFromXmlFile(fileNameWithPath.GetBuffer(0), *pTempGenome);
        CBasicEntity::getEntityNameFromXmlFile(fileNameWithPath.GetBuffer(0), name);
        CBasicEntity::getDefaultLayerFromXmlFile(fileNameWithPath.GetBuffer(0), startLayer);

        Point_t coord = {1,1};
        m_pPreyEntities[i] = CBiotop::createEntity(name, pTempGenome);   
        if (m_pPreyEntities[i] != NULL)
        {
          m_pPreyEntities[i]->jumpToGridCoord(coord, startLayer);
          m_pPreyEntities[i]->loadDataFromXmlFile(fileNameWithPath.GetBuffer(0), pathName.GetBuffer(0));
          m_pPreyEntities[i]->loadBrainFromXmlFile(fileNameWithPath.GetBuffer(0));
          count++;
        }
    }
    else
    {   
      break; // don't accept gap for the momment
    }
  }

  // Add water to table
  CBasicEntity* pWater = new CWater();
  m_pPreyEntities[count] = pWater;

  for (i=0;i<MAX_NUMBER_BUTTONS_PREDATOR;i++)
  {
    if ( (m_pPredatorButtonTable[i] != NULL) && (m_pPredatorButtonTable[i]->GetFileName() != "") )
    {
        CString fileName = m_pPredatorButtonTable[i]->GetFileName();
        CString pathName = m_pPredatorButtonTable[i]->GetPathName();
        CString fileNameWithPath = pathName + fileName;
        string name;
        int startLayer;
        pTempGenome = new CGenome(CLASS_NONE,"");
        CBasicEntity::getGenomeFromXmlFile(fileNameWithPath.GetBuffer(0), *pTempGenome);
        CBasicEntity::getEntityNameFromXmlFile(fileNameWithPath.GetBuffer(0), name);
        CBasicEntity::getDefaultLayerFromXmlFile(fileNameWithPath.GetBuffer(0), startLayer);

        Point_t coord = {1,1};
        m_pPredatorEntities[i] = CBiotop::createEntity(name, pTempGenome);   
        if (m_pPredatorEntities[i] != NULL)
        {
          m_pPredatorEntities[i]->jumpToGridCoord(coord, startLayer);       
          m_pPredatorEntities[i]->loadDataFromXmlFile(fileNameWithPath.GetBuffer(0), pathName.GetBuffer(0));
          m_pPredatorEntities[i]->loadBrainFromXmlFile(fileNameWithPath.GetBuffer(0));
        }
    }
  }

  return true;
}

int CDialogTab3::GetPreyEntityList(CBasicEntity*** pEntityTab, bool includeWater)
{
  int i,count = 0;
  *pEntityTab = m_pPreyEntities;
  for (i=0;i<MAX_NUMBER_BUTTONS_PREY+1;i++)
  {
    if (m_pPreyEntities[i] != NULL)
      count++;
  }
  if (!includeWater)
    count--;

  return count;
}

int CDialogTab3::GetPredatorEntityList(CBasicEntity*** pEntityTab)
{
  int i,count = 0;
  *pEntityTab = m_pPredatorEntities;
  for (i=0;i<MAX_NUMBER_BUTTONS_PREDATOR;i++)
  {
    if (m_pPredatorEntities[i] != NULL)
      count++;
  }
  return count;
}


