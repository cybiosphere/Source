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

// BiotopView.cpp : implementation of the CBiotopView class
//

#include "stdafx.h"
#include "stdafxGL.h"
#include "BiotopDisplayGL.h"
#include "BiotopDisplayMFC.h"

#include "Cybiosphere.h"

#include "BiotopDoc.h"
#include "BiotopView.h"

#include "CBrain.h"
#include "CBiotop.h"
#include "CBasicEntity.h"
#include "CIdentifyChecker.h"

#include "CLifeStage.h"

#include "EntityCreatorDlg.h"
#include "GenomeEditorDlg.h"
#include "BrainEditorDlg.h"
#include "MeasureEditorDlg.h"
#include "TrainingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBiotopView

IMPLEMENT_DYNCREATE(CBiotopView, CView)

BEGIN_MESSAGE_MAP(CBiotopView, CView)
	//{{AFX_MSG_MAP(CBiotopView)
	ON_WM_PAINT()
	ON_WM_SIZE()
  ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_APP_ADD_NEW_ENTITY, OnAppAddNewEntity)
	ON_COMMAND(ID_APP_ADD_GROUP_ENTITY, OnAppAddGroup)
	ON_COMMAND(ID_APP_REMOVE_ENTITY, OnAppRemoveEntity)
	ON_COMMAND(ID_APP_EDIT_BRAIN, OnAppEditBrain)
	ON_COMMAND(ID_APP_EDIT_GENOME, OnAppEditGenome)
	ON_COMMAND(ID_APP_EDIT_TRAIN_AND_GRADE, OnAppEditTrainAndGrade)
  ON_COMMAND(ID_APP_EDIT_IDENTIFY, OnAppEditIdentifyBySelection)
  ON_COMMAND(ID_APP_EDIT_CHECK_IDENTIFY, OnAppEditCheckIdentify)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_CANCELMODE()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_APP_MONITOR_SHORT, OnAppMonitorShort)
	ON_COMMAND(ID_APP_MONITOR_LONG, OnAppMonitorLong)
  ON_COMMAND(ID_APP_MONITOR_SPECIE, OnAppMonitorSpecie)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBiotopView construction/destruction

CBiotopView::CBiotopView()
{
  m_pBioDisplay3D = new CBiotopDisplayGL();
  m_pBioDisplay2D = new CBiotopDisplayMFC();

  m_pBioDisplay = m_pBioDisplay2D;

  m_MenuSelCoord.x = 1;
  m_MenuSelCoord.y = 1;
  m_IdleDisplayMode = false;
  m_zoomFactor = 100;
  m_pFocusedEntity = NULL;
  m_SpeedRate = 1.0;
}

CBiotopView::~CBiotopView()
{
	if ( ::GetDC (m_hWnd) && !::ReleaseDC ( m_hWnd, ::GetDC (m_hWnd) ) ) {						// Are We Able To Release The DC
		AfxMessageBox ( "Release Device Context Failed.", MB_OK | MB_ICONINFORMATION );
	}

	if ( m_hWnd && !::DestroyWindow ( m_hWnd ) )	{							// Are We Able To Destroy The Window?
		AfxMessageBox( "Could Not Release m_hWnd.", MB_OK | MB_ICONINFORMATION );
		m_hWnd = NULL;														// Set m_hWnd To NULL
	}

  if (m_pBioDisplay!=NULL)
  {
	  m_pBioDisplay->KillWindow();	// Shutdown
    delete ((CBiotopDisplayGL*)m_pBioDisplay3D);
    delete ((CBiotopDisplayMFC*)m_pBioDisplay2D);
  }
 
}

BOOL CBiotopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopView drawing

void CBiotopView::OnDraw(CDC* pDC)
{
	CBiotopDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
  
}

void CBiotopView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

  SECOND_TIMER_ID = 1;
  REFRESH_TIMER_ID = 2;
}

int CBiotopView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CView::OnCreate(lpCreateStruct); 

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopView printing

BOOL CBiotopView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBiotopView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBiotopView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopView diagnostics

#ifdef _DEBUG
void CBiotopView::AssertValid() const
{
	CView::AssertValid();
}

void CBiotopView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBiotopDoc* CBiotopView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBiotopDoc)));
	return (CBiotopDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBiotopView message handlers

void CBiotopView::OnPaint() 
{
  if (!m_IdleDisplayMode)
    m_pBioDisplay->RedrawScene();
  else 
    m_pBioDisplay->RedrawSceneIdleNoCPU();
  CScrollView::OnPaint();
}

void CBiotopView::OnSize(UINT nType, int cx, int cy) 
{
  m_pBioDisplay2D->ResizeScene( cx, cy);
  m_pBioDisplay3D->ResizeScene( cx, cy);
  if (m_pBioDisplay == m_pBioDisplay3D)
  {
    m_pBioDisplay3D->RefreshScene();
  }
  m_pBioDisplay->DisplayView();
}

void CBiotopView::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent==SECOND_TIMER_ID)
  {
    if (!theApp.IsModeManual())
    {
      LARGE_INTEGER curSysTicks;
      QueryPerformanceCounter(&curSysTicks);
      DWORD idleTime = curSysTicks.LowPart - m_SysTicksPrevSecEnd.LowPart;
      theApp.NextSecondStart();
      if ((!m_IdleDisplayMode)&&(idleTime<8000))
      {
        m_IdleDisplayMode = true;
        m_pBioDisplay->RefreshSceneIdleNoCPU();
        m_pBioDisplay->RedrawSceneIdleNoCPU();
      }
      if ((m_IdleDisplayMode)&&(idleTime>80000)&&(m_SpeedRate<80))
      {
        m_IdleDisplayMode = false;
        m_pBioDisplay->RefreshSceneIdleNoCPU();
        m_pBioDisplay->RedrawScene();
      }

      if (!theApp.GetScenarioPlayer()->IsScenarioFileOpened())
      {
        m_pBiotop->nextSecond();
        // In idle mode, call twice pBiotop->nextSecond to boost performance 
        if (m_IdleDisplayMode)
        {
          m_pBiotop->nextSecond();
        }
      }
      else
      {
        int prevSucessScore = theApp.GetScenarioPlayer()->m_successScore;
        int prevTotalScore  = theApp.GetScenarioPlayer()->m_totalScore;

        if (theApp.GetScenarioPlayer()->NextCmdNextSecond())
        {
          CString scoreText;
          scoreText.Format("Score:%d / %d", theApp.GetScenarioPlayer()->m_successScore, theApp.GetScenarioPlayer()->m_totalScore);  
          theApp.GetBioCtrlViewPtr()->SetStrScenarioScore(scoreText);
          // In case of failure
          if ((theApp.GetScenarioPlayer()->m_totalScore > prevTotalScore) && (theApp.GetScenarioPlayer()->m_successScore == prevSucessScore))
          {
            // If stop on event, auto freeze
            if (theApp.IsModeStopOnEvent())
            {
              theApp.SetModeManual(true);
              AfxMessageBox(theApp.GetScenarioPlayer()->GetCurrentCmd().c_str(), MB_ICONEXCLAMATION);
            }
          }
        }
        else
        {
          // scenario is finished
          CString scoreText;
          scoreText.Format("Score:%d / %d OK", theApp.GetScenarioPlayer()->m_successScore, theApp.GetScenarioPlayer()->m_totalScore);  
          theApp.GetBioCtrlViewPtr()->SetStrScenarioScore(scoreText);
          m_pBiotop->nextSecond();
        }
      }

      if (!m_IdleDisplayMode)
      {
        m_pBioDisplay->RefreshNextSecond();
        theApp.NextSecondRefreshAllViews();
      }
      else
      {
        theApp.NextSecondRefreshAllViewsLowCPU();
      }

      // Stop on event
      if (theApp.IsModeStopOnEvent())
      {
        if (m_pBiotop->checkMeasureEvents())
        {
          theApp.SetModeManual(true);
        }
      }

      if (idleTime<300) // FRED: TBC 200=>NO
      {
        // In case of low CPU, reduce speed
        if (!theApp.GetBioCtrlViewPtr()->ForceChangeSpeed(-1))
        {
          // In case of very low CPU, Stop
          theApp.SetModeManual(true);
          AfxMessageBox("CPU TOO LOW. AUTOMATIC STOP!!!");
        }
      }
      else if (idleTime>330)
      {
        theApp.GetBioCtrlViewPtr()->RestoreUserSpeed();
      }

      QueryPerformanceCounter(&m_SysTicksPrevSecEnd);
    }
    else if (m_IdleDisplayMode)
    {
      m_pFocusedEntity = NULL;
      m_IdleDisplayMode = false;
      m_pBioDisplay->RefreshSceneIdleNoCPU();
      m_pBioDisplay->RedrawScene();
    }

    if (!m_IdleDisplayMode)
    {
      // Set focused entity if exists
      CBasicEntity* pEntity = theApp.GetpSelectedEntity();
      if ((pEntity) && (pEntity->getBrain() != NULL))
      {
        if (m_pFocusedEntity != pEntity->getBrain()->getpBrainFocusedEntityInfo()->pPreviousEntity)
        {
          if (m_pFocusedEntity != NULL)        
            m_pFocusedEntity->forceHasChanged();

          m_pFocusedEntity = pEntity->getBrain()->getpBrainFocusedEntityInfo()->pPreviousEntity;
          if (m_pFocusedEntity != NULL)        
          {
            m_pFocusedEntity->forceHasChanged();
            m_pBioDisplay->SetFocusedEntityId (m_pFocusedEntity->getId());
            m_pBioDisplay->RefreshNextSecond();
          }
        }
      }
    }
  }

  if (nIDEvent==REFRESH_TIMER_ID)
  {
    if (!m_IdleDisplayMode)
      m_pBioDisplay->RefreshScene();
  }
	CView::OnTimer(nIDEvent);
}

void CBiotopView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  m_pBioDisplay->SetLButtonIsDown(true);

  CView::OnLButtonDown(nFlags, point);
}

void CBiotopView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pBioDisplay->SetLButtonIsDown(false);

	CView::OnLButtonUp(nFlags, point);
}

bool CBiotopView::SetSelectedEntity(CBasicEntity* pEntity)
{
  // Reset focused entity
  m_pBioDisplay3D->SetFocusedEntityId(-1);
  m_pBioDisplay2D->SetFocusedEntityId(-1);

  if (m_pFocusedEntity != NULL)        
    m_pFocusedEntity->forceHasChanged();
  m_pFocusedEntity = NULL;

  if (pEntity != NULL)
  {
    m_pBioDisplay3D->SetSelectedEntityId (pEntity->getId());
    m_pBioDisplay2D->SetSelectedEntityId (pEntity->getId());
    return (true);
  }
  else
  {
    m_pBioDisplay3D->SetSelectedEntityId (-1);
    m_pBioDisplay2D->SetSelectedEntityId (-1);
    return (false);
  }
}

void CBiotopView::ForceRefreshDisplay(bool clearAll)
{
  if (clearAll)
    m_pBioDisplay->RefreshSceneIdleNoCPU();
  m_pBioDisplay->RefreshNextSecond();
  m_pBioDisplay->RefreshScene();
}

void CBiotopView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  Point_t bioCoord = m_pBioDisplay->GetGridCoordFromScreenPos(point);
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(bioCoord);
  theApp.RefreshAllWithNewEntity(pEntity);
	
  // Selection is used in menu
  m_MenuSelCoord = bioCoord;

  CView::OnLButtonDblClk(nFlags, point);
}

void CBiotopView::OnMouseMove(UINT nFlags, CPoint point) 
{
  m_pBioDisplay->OnMouseMove(point);

  if ( (nFlags & MK_LBUTTON) != 0 ) 
    m_pBioDisplay->SetLButtonIsDown(true);
  else
    m_pBioDisplay->SetLButtonIsDown(false);

  CView::OnMouseMove(nFlags, point);
}

void CBiotopView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
  CScrollView::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CBiotopView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
  CScrollView::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CBiotopView::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
  CPoint outPoint(-1,-1);
  m_pBioDisplay->OnMouseMove(outPoint);
  m_pBioDisplay->SetLButtonIsDown(false);

  CView::OnNcMouseMove(nHitTest, point);
}



bool CBiotopView::SetBiotop(CBiotop* pBiotop)
{
  m_pBiotop = pBiotop;
  m_pBioDisplay2D->SetBiotop(pBiotop);
  m_pBioDisplay2D->Initialize(this);
  m_pBioDisplay3D->SetBiotop(pBiotop);
  m_pBioDisplay3D->Initialize(this);

  return (true);
}

void CBiotopView::SetZoomRate (int zoomRate)
{
  m_zoomFactor = zoomRate;
  m_pBioDisplay->SetZoomFactor((double)(zoomRate*2/25 * 25)/200.0);
}

void CBiotopView::StartTimers()
{
  SetTimer(REFRESH_TIMER_ID,10,NULL);
  SetSpeedRate(1.0);
}

void CBiotopView::SetSpeedRate (double speedRate)
{
  m_SpeedRate = speedRate;
  KillTimer(SECOND_TIMER_ID);
  if (speedRate>0)
  {
    m_pBioDisplay->SetNbRefreshPerStep((1000/speedRate-9)/200+1);
    SetTimer(SECOND_TIMER_ID, 1000.0 / speedRate - 9.0, NULL);
    if (speedRate > 70)
    {
      m_IdleDisplayMode = true;
      m_pBioDisplay->RefreshSceneIdleNoCPU();
    }
  }
  else
  {
    m_pBioDisplay->SetNbRefreshPerStep(4);
    SetTimer(SECOND_TIMER_ID,1000,NULL);
  }

  QueryPerformanceCounter(&m_SysTicksPrevSecEnd);
}

double CBiotopView::GetSpeedRate()
{
  return (m_SpeedRate);
}

void CBiotopView::OnRButtonDown(UINT nFlags, CPoint point) 
{

  Point_t bioCoord = m_pBioDisplay->GetGridCoordFromScreenPos(point);
  if (m_pBiotop->isCoordValid(bioCoord,1))
  {
    m_MenuSelCoord = bioCoord;
  }
  else
  {
	  m_MenuSelCoord.x = 1;
	  m_MenuSelCoord.y = 1;
  }
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(bioCoord); 
  // Do not take into account global water and grass
  if ((pEntity!=NULL) && (pEntity->getId()<ENTITY_ID_FIRST_USER_ENTITY))
    pEntity = NULL;

  CMenu menu; 
  ClientToScreen(&point);
  menu.LoadMenu(IDR_MAINFRAME); 
  ASSERT (menu.m_hMenu != NULL);
  CMenu* p_popup= menu.GetSubMenu(1); 
  ASSERT (p_popup != NULL);
  
  p_popup->EnableMenuItem(ID_EDIT_PASTE, MF_ENABLED);
  p_popup->EnableMenuItem(ID_EDIT_FIND, MF_GRAYED);

  if (pEntity==NULL)
  {
    p_popup->EnableMenuItem(ID_EDIT_CUT, MF_GRAYED);
    p_popup->EnableMenuItem(ID_EDIT_COPY, MF_GRAYED);
    p_popup->EnableMenuItem(ID_APP_REMOVE_ENTITY, MF_GRAYED);
    p_popup->EnableMenuItem(ID_APP_EDIT_GENOME, MF_GRAYED);
    p_popup->EnableMenuItem(ID_APP_EDIT_BRAIN, MF_GRAYED);
    p_popup->EnableMenuItem(ID_APP_EDIT_TRAIN_AND_GRADE, MF_GRAYED); 
    p_popup->EnableMenuItem(ID_APP_EDIT_IDENTIFY, MF_GRAYED);
    p_popup->EnableMenuItem(ID_APP_EDIT_CHECK_IDENTIFY, MF_GRAYED);
    p_popup->EnableMenuItem(ID_APP_MONITOR_SPECIE, MF_GRAYED);
  }
  else 
  {
    if (pEntity->getClass() < CLASS_ANIMAL_FIRST)
    {
      p_popup->EnableMenuItem(ID_APP_EDIT_TRAIN_AND_GRADE, MF_GRAYED);
      p_popup->EnableMenuItem(ID_APP_EDIT_CHECK_IDENTIFY, MF_GRAYED);
    }
    if ((theApp.GetpSelectedEntity() != NULL) && (theApp.GetpSelectedEntity()->getClass() < CLASS_ANIMAL_FIRST))
    {
      p_popup->EnableMenuItem(ID_APP_EDIT_IDENTIFY, MF_GRAYED); 
    }
  }
  int nResult = p_popup->TrackPopupMenu(TPM_LEFTALIGN |  TPM_RIGHTBUTTON, point.x, point.y, this);
  //Agir en fonction du résultat retourné.
  //0 indique que le menu a été annulé
  //Sinon, comparer le resultat avec les identifiants des différentes options

  menu.DestroyMenu();	
	
	CView::OnRButtonDown(nFlags, point);
}


void CBiotopView::OnAppAddNewEntity() 
{
  CBasicEntity* pNewEntity = NULL;
  entityIdType newEntityId = -1;

	CEntityCreatorDlg dlgCreate(NULL,m_MenuSelCoord.x,m_MenuSelCoord.y);

  if (dlgCreate.DoModal() == IDOK)
  {
    newEntityId = dlgCreate.GetNewEntityId();
    if ( (newEntityId > 0) && (dlgCreate.IsGenomeEditRequired()) )
    {
      pNewEntity = theApp.GetBiotop()->getEntityById(newEntityId);
	    CGenomeEditorDlg dlgEdit(pNewEntity);
      dlgEdit.DoModal();
    }
  }
  ForceRefreshDisplay();
  theApp.NextSecondRefreshAllViews();
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;
} 

void CBiotopView::OnAppAddGroup() 
{
  // Open module
  CString fileName;
  CString pathName;
  CString fullPathName;
  CString strSection,strData;
  CBasicEntity newEntity;
  entityIdType newEntityId = -1;
  int randoffsetx, randoffsety;

  CFileDialog fileDlg(true,"xml","",OFN_ALLOWMULTISELECT,"Entity Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||");
  fileDlg.m_ofn.lpstrTitle = "Select entities";
  long nResp = fileDlg.DoModal(); 
  if (nResp == IDOK)
  {
    
    POSITION pos = fileDlg.GetStartPosition();
    while (pos!=NULL)
    {
      fullPathName = fileDlg.GetNextPathName(pos);
      int endPath = fullPathName.ReverseFind ('\\');
      if (endPath>0)
      {
        fileName = fullPathName.Mid(endPath+1);;	
        pathName = fullPathName.Left(endPath+1);
      }
      else
      {
        fileName = fullPathName;	
        pathName = "";	 
      }

      m_pBiotop->createAndAddEntity(fileName.GetBuffer(0), pathName.GetBuffer(0), m_MenuSelCoord);

      randoffsetx = getRandInt(3) - 1;
      randoffsety = getRandInt(3) - 1;
      m_MenuSelCoord.x+=randoffsetx;
      m_MenuSelCoord.y+=randoffsety;
    }
  }

   ForceRefreshDisplay();
  theApp.NextSecondRefreshAllViews();
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;
}

void CBiotopView::OnAppRemoveEntity() 
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord); 
  theApp.removeEntityFromBiotop(pEntity);
  // force redraw all
  ForceRefreshDisplay();
  theApp.NextSecondRefreshAllViews();
  m_MenuSelCoord.x = 1;
  m_MenuSelCoord.y = 1;
}

void CBiotopView::OnAppEditBrain() 
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord); 
  if (pEntity!=NULL)
  {
	  CBrainEditorDlg dlg(pEntity->getBrain());
    dlg.DoModal();
  }
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;
}

void CBiotopView::OnAppEditGenome() 
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  if (pEntity!=NULL)
  {
	  CGenomeEditorDlg dlgEdit(pEntity);
	  dlgEdit.DoModal();
  }
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;	
}

void CBiotopView::OnAppEditTrainAndGrade() 
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  if ((pEntity != NULL) && (pEntity->getClass() >= CLASS_ANIMAL_FIRST)) 
  {
	  CFileDialog fileDlg(true,"scp","",0,"Scenario Files (*.scp)|*.scp; *.scp|All Files (*.*)|*.*||");
	  fileDlg.m_ofn.lpstrTitle = "Select a file in training directory";
	  long nResp = fileDlg.DoModal(); 
	  if (nResp == IDOK)
    {
      // Force ModeManual
      theApp.SetModeManual(true);

      CString openedDirectoryName = fileDlg.GetPathName();	 
      int endPath = openedDirectoryName.ReverseFind ('\\');
      if (endPath>0)
        openedDirectoryName = openedDirectoryName.Left(endPath+1);

      // Overwrite "newEntity.xml" with trained entity
      CString newEntityFileName= openedDirectoryName + "newEntity.xml"; 
      pEntity->saveInXmlFile( newEntityFileName.GetBuffer(0), "newEntity");

      CTrainingDlg dlg(openedDirectoryName);
      long nResponse = dlg.DoModal();
      if (nResponse == IDOK)
      {
        // NewEntity Brain has been saved. Reload it in current entity
        pEntity->loadBrainFromXmlFile(newEntityFileName.GetBuffer(0));
      }
    }
  }
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;	
}

void CBiotopView::OnAppEditIdentifyBySelection() 
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  CBasicEntity* pSelectedEntity = theApp.GetpSelectedEntity();
  CBrain* pSelectedBrain = NULL;
  
  if (pSelectedEntity!=NULL)
    pSelectedBrain = pSelectedEntity->getBrain();

  if ((pSelectedBrain!=NULL) && (pEntity!=NULL))
  {
    double entityWeight = pSelectedBrain->GetViewedEntityWeight(pEntity);
    pSelectedBrain->ComputeAndGetIdentification(pEntity);
    CString title;
    title.Format(" weight=%f", entityWeight);
    title = pEntity->getLabel().c_str() + title;
    CBrainEditorDlg dlg(pSelectedBrain, true, title);
    dlg.DoModal();
  }

	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;	
}

void CBiotopView::OnAppEditCheckIdentify()
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  if ((pEntity != NULL) && (pEntity->getClass() >= CLASS_ANIMAL_FIRST))
  {
    CFileDialog fileDlg(true, "chk", "", 0, "Check Files (*.chk)|*.chk; *.chk|All Files (*.*)|*.*||");
    fileDlg.m_ofn.lpstrTitle = "Select a check file in training directory";
    long nResp = fileDlg.DoModal();
    if (nResp == IDOK)
    {
      // Force ModeManual
      theApp.SetModeManual(true);

      CString openedDirectoryName = fileDlg.GetPathName();
      int endPath = openedDirectoryName.ReverseFind('\\');
      if (endPath>0)
        openedDirectoryName = openedDirectoryName.Left(endPath + 1);
      
      CIdentifyChecker checker(pEntity);
      checker.ReadCheckerFile(fileDlg.GetFileName().GetBuffer(0), openedDirectoryName.GetBuffer(0));
      checker.StartCheck();
      CString resuStr;
      resuStr.Format("Check result : OK  %d, total %d\n", checker.GetSuccessScore(), checker.GetTotalScore());
      resuStr += checker.GetResultInfo().c_str();
      AfxMessageBox(resuStr);
    }
  }
}

void CBiotopView::SwitchToDisplay2d()
{
  Point_t centerCoord = m_pBioDisplay3D->GetCurrentGridCenterPos();
  m_pBioDisplay = m_pBioDisplay2D;
  m_pBioDisplay2D->DisplayView();
  ForceRefreshDisplay();
  m_pBioDisplay2D->ScrollToGridCoord(centerCoord);
}

void CBiotopView::SwitchToDisplay3d()
{
  Point_t centerCoord = m_pBioDisplay2D->GetCurrentGridCenterPos();
  m_pBioDisplay = m_pBioDisplay3D;
  m_pBioDisplay3D->DisplayView();
  ForceRefreshDisplay();
  SetZoomRate(m_zoomFactor);
  m_pBioDisplay3D->ScrollToGridCoord(centerCoord);
}

void CBiotopView::ScrollToGridCoord (Point_t centerPos)
{
  m_pBioDisplay->ScrollToGridCoord(centerPos);
  ForceRefreshDisplay();
}

void CBiotopView::OnEditCopy() 
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  if (pEntity!=NULL)
  {
	  if ( !OpenClipboard() )
    {
      AfxMessageBox( "Cannot open the Clipboard" );
      return;
    }
    // Remove the current Clipboard contents
    if( !EmptyClipboard() )
    {
      AfxMessageBox( "Cannot empty the Clipboard" );
      return;
    }
 
    CString strPath = theApp.GetAppliPath()+"temp\\TempEntity1.xml";
    string strName = pEntity->getLabel()+"c1";
    // To support multi select copy/paste, several names can be set here
    pEntity->saveInXmlFile(strPath.GetBuffer(0), strName);

    // Get the currently selected data
    HGLOBAL hData = GlobalAlloc (GMEM_FIXED, 100);
    strcpy((char*)hData,"TempEntity1.xml");

    // For the appropriate data formats...
    if ( SetClipboardData( CF_DSPTEXT, hData ) == NULL )
    {
      AfxMessageBox( "Unable to set Clipboard data" );
      CloseClipboard();
      return;
    }
    // ...
    CloseClipboard();
  }
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;	

}

void CBiotopView::OnEditPaste() 
{
	if ( !OpenClipboard() )
  {
    AfxMessageBox( "Cannot open the Clipboard" );
    return;
  }

  HGLOBAL hData = GetClipboardData(CF_DSPTEXT);

  if (hData != NULL)
  {
    string name;
    CString strPath = theApp.GetAppliPath()+"temp\\";
    CGenome* pTempGenome = new CGenome(CLASS_NONE,"");
    theApp.GetBiotop()->createAndAddEntity((char*)hData, strPath.GetBuffer(0), m_MenuSelCoord);
  }

  ForceRefreshDisplay();
  theApp.NextSecondRefreshAllViews();

  GlobalFree(hData);
  EmptyClipboard();
  CloseClipboard();
}

CBiotopDisplay* CBiotopView::GetpBiotopDisplay()
{
  return (m_pBioDisplay);
}

void CBiotopView::OnAppMonitorShort() 
{
  m_pBiotop->deleteAllMeasures();
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord); 
  // Do not take into account global water and grass
  if ((pEntity!=NULL) && (pEntity->getId()<ENTITY_ID_FIRST_USER_ENTITY))
    pEntity = NULL;

  if (pEntity!=NULL)
  {
    if (pEntity->getClass()>=CLASS_VEGETAL_FIRST)
    {
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Weight"), 1, 0);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Health rate"), 1, 1); 
    }

    if (pEntity->getClass()>=CLASS_ANIMAL_FIRST)
    {
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Hunger rate"), 10, 1); 
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Thirst rate"), 10, 2);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Pleasure rate"), 1, 3);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Suffering rate"), 1, 4);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Speed"), 1, 5);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Tiredness rate"), 1, 6);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Fear rate"), 1, 7);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Stomach filling"), 10, 8);
      m_pBiotop->addMeasureFeeling(pEntity, 1, 9, MEASURE_FEELING_DIF);
    }
  }
  else
  {
    // Monitor biotop
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_SUNLIGHT,   3600, 0);
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_FERTILITY,  3600, 1);
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_TEMPERATURE,3600, 2);

    m_pBiotop->addMeasurePopulation(3600, 3, MEASURE_POPULATION_TOTAL,   10*m_pBiotop->getNbOfEntities());
    m_pBiotop->addMeasurePopulation(3600, 4, MEASURE_POPULATION_ANIMAL,  10*m_pBiotop->getNbOfAnimals());
    m_pBiotop->addMeasurePopulation(3600, 5, MEASURE_POPULATION_VEGETAL, 10*m_pBiotop->getNbOfVegetals());
    m_pBiotop->addMeasurePopulation(3600, 6, MEASURE_POPULATION_MINERAL, 10*m_pBiotop->getNbOfMinerals());
  }
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;

  theApp.GetStatisticViewPtr()->RebuildMeasChkBox();
}

void CBiotopView::OnAppMonitorLong() 
{
  m_pBiotop->deleteAllMeasures();
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  // Do not take into account global water and grass
  if ((pEntity!=NULL) && (pEntity->getId()<ENTITY_ID_FIRST_USER_ENTITY))
    pEntity = NULL;

  if (pEntity!=NULL)
  {
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_SUNLIGHT,   3600, 0);
    m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Protection factor"), 3600, 1);
    m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Weight"), 3600, 2);

    if (pEntity->getClass() >= CLASS_VEGETAL_FIRST)
    {
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Health rate"), 3600, 3);
    }
    if (pEntity->getClass()>=CLASS_ANIMAL_FIRST)
    {
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Fat weight"), 3600, 4);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Hunger rate"), 3600, 5); 
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Thirst rate"), 3600, 6);
    }
    if (pEntity->getClass()==CLASS_MAMMAL)
    {
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Libido rate"), 3600, 7);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Gestation baby number"), 3600, 8);
      m_pBiotop->addMeasureEntityParam(pEntity, pEntity->getParamIdByName("Gestation time"), 3600, 9);
    }
  }
  else
  {
    // Monitor biotop
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_SUNLIGHT,   3600, 0);
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_FERTILITY,  43200, 1);
    m_pBiotop->addMeasureBiotopParam(BIO_PARAM_TEMPERATURE,43200, 2);

    m_pBiotop->addMeasurePopulation(43200, 3, MEASURE_POPULATION_TOTAL,   10*m_pBiotop->getNbOfEntities());
    m_pBiotop->addMeasurePopulation(43200, 4, MEASURE_POPULATION_ANIMAL,  10*m_pBiotop->getNbOfAnimals());
    m_pBiotop->addMeasurePopulation(43200, 5, MEASURE_POPULATION_VEGETAL, 10*m_pBiotop->getNbOfVegetals());
    m_pBiotop->addMeasurePopulation(43200, 6, MEASURE_POPULATION_MINERAL, 10*m_pBiotop->getNbOfMinerals());
  }
	m_MenuSelCoord.x = 1;
	m_MenuSelCoord.y = 1;

  theApp.GetStatisticViewPtr()->RebuildMeasChkBox();
}

void CBiotopView::OnAppMonitorSpecie()
{
  CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(m_MenuSelCoord);
  // Do not take into account global water and grass
  if ((pEntity != NULL) && (pEntity->getId() < ENTITY_ID_FIRST_USER_ENTITY))
    pEntity = NULL;
  if ((pEntity != NULL) && (pEntity->getGenome() != NULL))
  {
    std::string specieName = pEntity->getGenome()->getSpecieName();
    m_pBiotop->addMeasurePopulation(43200, m_pBiotop->getUnusedMeasureId(10), MEASURE_POPULATION_SPECIFIC, 
                                    10 * (m_pBiotop->getNbOfSpecieEntities(specieName) + 1), specieName);
  }

  m_MenuSelCoord.x = 1;
  m_MenuSelCoord.y = 1;
  theApp.GetStatisticViewPtr()->RebuildMeasChkBox();
}