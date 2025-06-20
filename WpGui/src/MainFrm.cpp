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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "MainFrm.h"

#include "EntityCreatorDlg.h"
#include "EntityEditorDlg.h"
#include "EntityFindDlg.h"
#include "BiotopSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_NEW_BIOTOP, OnAppNewBiotop)
	ON_COMMAND(ID_FILE_OPEN_BIOTOP, OnAppOpenBiotop)
	ON_COMMAND(ID_FILE_SAVE_BIOTOP, OnAppSaveBiotop)
	ON_COMMAND(ID_FILE_SAVE_AS_BIOTOP, OnAppSaveAsBiotop)
	ON_COMMAND(ID_FILE_RUN_SCENARIO_BIOTOP, OnAppRunLearningScenario)
  ON_COMMAND(ID_APP_DISPLAY_LOGSERVER, OnAppDisplayLogServer)
	ON_COMMAND(ID_APP_DISPLAY_MAP, OnAppDisplayMap)
	ON_COMMAND(ID_APP_DISPLAY_BRAIN, OnAppDisplayBrain)
	ON_COMMAND(ID_APP_DISPLAY_GENETIC, OnAppDisplayGenetic)
	ON_COMMAND(ID_APP_DISPLAY_STAT, OnAppDisplayStat)
  ON_COMMAND(ID_APP_DISPLAY_MAPEDITOR, OnAppDisplayMapEditor)
	ON_COMMAND(ID_APP_SELECT_VIEW, OnAppSelectView)
	ON_COMMAND(ID_APP_SELECT_CREATE, OnAppSelectCreate)
	ON_COMMAND(ID_APP_SELECT_STICK, OnAppSelectStick)
	ON_COMMAND(ID_APP_SELECT_CARROT, OnAppSelectCarrot)
	ON_COMMAND(ID_APP_DISPLAY_2D, OnAppDisplay2d)
	ON_COMMAND(ID_APP_DISPLAY_3D, OnAppDisplay3d)
  ON_COMMAND(ID_APP_DISPLAY_2DSAT, OnAppDisplay2dSat)
	ON_COMMAND(ID_EDIT_FIND, OnAppFindEntity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  m_DisplayMode   = DISPLAY_MODE_FULL_SIZE;
  m_ViewMode      = VIEW_MODE_2D;
  m_SelectionMode = SELECT_MODE_VIEW;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

  //m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_APP_DISPLAY_GENETIC),TBBS_CHECKBOX);
  //m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_APP_DISPLAY_BRAIN),TBBS_CHECKBOX);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_3D,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2D,true);

  OnAppSelectView();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
  resizeAllChildFrames(cx,cy);

}

void CMainFrame::OnAppNewBiotop() 
{
  if (IDNO == AfxMessageBox("Do you realy want to create new biotop with default entities?", MB_YESNO | MB_ICONEXCLAMATION))
    return; 

  // Unselect entity
  theApp.setSelectedEntity(NULL);

  CBiotopSizeDlg biotopSizeDlg;
  if (biotopSizeDlg.DoModal() != IDOK)
    return;

  theApp.CreateNewDefaultBiotop(biotopSizeDlg.GetBiotopSizeX(), biotopSizeDlg.GetBiotopSizeY());
  theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->RefreshBiotop();
  theApp.GetBiotopViewPtr()->ForceRefreshDisplay(TRUE);
  theApp.GetBioCtrlViewPtr()->UpdateTimerDisplay(theApp.GetBiotop()->getBiotopTime());
  theApp.SetOpenedBiotopFile("", "");
}

void CMainFrame::OnAppOpenBiotop() 
{
  // Open Biotop
  CString fileName,pathName;
  CString strSection,strData;

	CFileDialog fileDlg(true,"bio","",0,"Biotop Files (*.bio)|*.bio; *.bio|All Files (*.*)|*.*||");
	fileDlg.m_ofn.lpstrTitle = "Select biotop";
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    // Unselect entity
    theApp.setSelectedEntity(NULL);
    // Load file
    fileName = fileDlg.GetFileName();	
    pathName = fileDlg.GetPathName();	 
    int endPath = pathName.ReverseFind ('\\');
    if (endPath>0)
      pathName = pathName.Left(endPath+1); 

    bool resu = theApp.GetBiotop()->loadFromXmlFile(fileName.GetBuffer(0),pathName.GetBuffer(0));
    theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->RefreshBiotop();
    theApp.GetBiotopViewPtr()->ForceRefreshDisplay(TRUE);
    theApp.GetBioCtrlViewPtr()->UpdateTimerDisplay(theApp.GetBiotop()->getBiotopTime());

    if (resu == true)
    {
      theApp.SetOpenedBiotopFile(pathName, fileName);
    }
    else 
    {
      AfxMessageBox("Impossible to open file " + fileName);
      theApp.SetOpenedBiotopFile("", "");
    }
  }
}



void CMainFrame::OnAppSaveBiotop()
{
  CString fileName, pathName;

  fileName = theApp.GetOpenedBiotopFileName();
  pathName = theApp.GetOpenedBiotopPath();

  if ( (fileName != "") && (pathName != ""))
  {
    int iAnswer = AfxMessageBox("Do you want to save all entities in files ?", MB_YESNO);
    bool doSaveEntities = (iAnswer == IDYES) ? true : false;
    theApp.GetBiotop()->saveInXmlFile(fileName.GetBuffer(0), pathName.GetBuffer(0), doSaveEntities);
  }
  else
  {
    OnAppSaveAsBiotop();
  }
}

void CMainFrame::OnAppSaveAsBiotop()
{
  // Open module
  CString fileName, pathName;
  CString strSection,strData;

	CFileDialog fileDlg(false,"bio","",0,"Biotop Files (*.bio)|*.bio; *.bio|All Files (*.*)|*.*||");
	fileDlg.m_ofn.lpstrTitle = "Select biotop";
  //fileDlg.m_ofn.lpstrInitialDir = GetAppliPath();
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    fileName = fileDlg.GetFileName();	
    pathName = fileDlg.GetPathName();	 
    int endPath = pathName.ReverseFind ('\\');
    if (endPath>0)
      pathName = pathName.Left(endPath+1); 

    bool resu = theApp.GetBiotop()->saveInXmlFile(fileName.GetBuffer(0), pathName.GetBuffer(0));

    if (resu == true)
    {
      theApp.SetOpenedBiotopFile(pathName, fileName);
    }
    else 
    {
      AfxMessageBox("Impossible to save file " + fileName);
      theApp.SetOpenedBiotopFile("", "");
    }
  }
}

void CMainFrame::OnAppRunLearningScenario()
{
  // Open Scenario
  CString fileName,pathName;
  CString strSection,strData;

	CFileDialog fileDlg(true,"scp","",0,"Scenario Files (*.scp)|*.scp; *.scp|All Files (*.*)|*.*||");
	fileDlg.m_ofn.lpstrTitle = "Select scenario";
	long nResp = fileDlg.DoModal(); 
	if (nResp == IDOK)
	{
    fileName = fileDlg.GetFileName();	
    pathName = fileDlg.GetPathName();	 
    int endPath = pathName.ReverseFind ('\\');
    if (endPath>0)
      pathName = pathName.Left(endPath+1); 

    // Unselect entity
    theApp.setSelectedEntity(NULL);
    // Load file
    bool resu = theApp.GetScenarioPlayer()->ReadScenarioFile(fileName.GetBuffer(0), pathName.GetBuffer(0));
    theApp.SetOpenedBiotopFile("", "");
    // Start reading scenario twice to update biotop and refresh display
    theApp.GetScenarioPlayer()->NextCmdNextSecond();
    theApp.GetScenarioPlayer()->NextCmdNextSecond(); 
    theApp.GetBiotopViewPtr()->ForceRefreshDisplay(true);
    theApp.GetBiotopViewPtr()->ForceIdleDisplayMode();
  }
}

void CMainFrame::unselectAllButtonTools()
{
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_LOGSERVER, false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_BRAIN, false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_GENETIC, false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_STAT, false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_MAP, false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_MAPEDITOR, false);
}

void CMainFrame::OnAppDisplayLogServer()
{
  unselectAllButtonTools();
  if (m_DisplayMode == DISPLAY_MODE_LOGSERVER)
  {
    m_DisplayMode = DISPLAY_MODE_FULL_SIZE;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_LOGSERVER, false);
  }
  else
  {
    m_DisplayMode = DISPLAY_MODE_LOGSERVER;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_LOGSERVER, true);
  }
  CRect wndRect;
  this->GetClientRect(wndRect);
  resizeAllChildFrames(wndRect.Width(), wndRect.Height());
}

void CMainFrame::OnAppDisplayMap() 
{
  unselectAllButtonTools();
  if (m_DisplayMode == DISPLAY_MODE_MAP)
  {
	  m_DisplayMode = DISPLAY_MODE_FULL_SIZE;
      m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_MAP,false);
  }
  else
  {
	  m_DisplayMode = DISPLAY_MODE_MAP;
      m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_MAP,true);
  } 
  CRect wndRect;
  this->GetClientRect(wndRect);
  resizeAllChildFrames(wndRect.Width(),wndRect.Height());
}

void CMainFrame::OnAppDisplayBrain() 
{
  unselectAllButtonTools();
  if (m_DisplayMode == DISPLAY_MODE_LAB_BRAIN)
  {
	  m_DisplayMode = DISPLAY_MODE_FULL_SIZE;
      m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_BRAIN,false);
  }
  else
  {
	  m_DisplayMode = DISPLAY_MODE_LAB_BRAIN;
      m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_BRAIN,true);
  }
  CRect wndRect;
  this->GetClientRect(wndRect);
  resizeAllChildFrames(wndRect.Width(),wndRect.Height());
}

void CMainFrame::OnAppDisplayGenetic() 
{
  unselectAllButtonTools();
  if (m_DisplayMode == DISPLAY_MODE_LAB_GENETIC)
  {
	  m_DisplayMode = DISPLAY_MODE_FULL_SIZE;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_GENETIC,false);
  }
  else
  {
	  m_DisplayMode = DISPLAY_MODE_LAB_GENETIC;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_GENETIC,true);
  }
  CRect wndRect;
  this->GetClientRect(wndRect);
  resizeAllChildFrames(wndRect.Width(),wndRect.Height());
}

void CMainFrame::OnAppDisplayStat() 
{
  unselectAllButtonTools();
  if (m_DisplayMode == DISPLAY_MODE_STATISTIC)
  {
	  m_DisplayMode = DISPLAY_MODE_FULL_SIZE;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_STAT,false);
  }
  else
  {
	  m_DisplayMode = DISPLAY_MODE_STATISTIC;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_STAT,true);
  }
  CRect wndRect;
  this->GetClientRect(wndRect);
  resizeAllChildFrames(wndRect.Width(),wndRect.Height());
}

void CMainFrame::OnAppDisplayMapEditor()
{
  unselectAllButtonTools();
  if (m_DisplayMode == DISPLAY_MODE_MAPEDITOR)
  {
    m_DisplayMode = DISPLAY_MODE_FULL_SIZE;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_MAPEDITOR, false);
  }
  else
  {
    m_DisplayMode = DISPLAY_MODE_MAPEDITOR;
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_MAPEDITOR, true);
  }
  CRect wndRect;
  this->GetClientRect(wndRect);
  resizeAllChildFrames(wndRect.Width(), wndRect.Height());
}

void CMainFrame::hideAllToolsWindows()
{
  theApp.GetBrainViewPtr()->GetParentFrame()->ShowWindow(SW_HIDE);
  theApp.GetGeneticViewPtr()->GetParentFrame()->ShowWindow(SW_HIDE);
  theApp.GetStatisticViewPtr()->GetParentFrame()->ShowWindow(SW_HIDE);
  theApp.GetMapConfigViewPtr()->GetParentFrame()->ShowWindow(SW_HIDE);
  theApp.GetLogServerViewPtr()->GetParentFrame()->ShowWindow(SW_HIDE);
  theApp.GetMapEditorViewPtr()->GetParentFrame()->ShowWindow(SW_HIDE);
}

void CMainFrame::resizeAllChildFrames(int cx, int cy)
{
  int iDpi = GetDpiForWindow(this->m_hWnd);
  int clientW = cx - MulDiv(5, iDpi, 96);
  int clientH = cy - MulDiv(38, iDpi, 96);
  int dpiScaledWidth = MulDiv(232, iDpi, 96);
  int dpiScaledHeight = MulDiv(253, iDpi, 96);

  theApp.GetBioCtrlViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, 0, 0, dpiScaledWidth, dpiScaledHeight, SWP_SHOWWINDOW);
  theApp.GetEntityViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, 0, dpiScaledHeight, dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);

  theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, clientH, SWP_SHOWWINDOW);
  theApp.GetBrainViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, dpiScaledHeight, SWP_HIDEWINDOW);
  theApp.GetGeneticViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, dpiScaledHeight, SWP_HIDEWINDOW);
  theApp.GetStatisticViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, dpiScaledHeight, SWP_HIDEWINDOW);
  theApp.GetMapConfigViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, dpiScaledHeight, SWP_HIDEWINDOW);
  theApp.GetLogServerViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, dpiScaledHeight, SWP_HIDEWINDOW);
  theApp.GetMapEditorViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, dpiScaledHeight, SWP_HIDEWINDOW);

  switch (m_DisplayMode)
  {
  case DISPLAY_MODE_FULL_SIZE:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, 0, clientW - dpiScaledWidth, clientH, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    break;

  case DISPLAY_MODE_LAB_GENETIC:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, dpiScaledHeight, clientW - dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    theApp.GetGeneticViewPtr()->GetParentFrame()->ShowWindow(SW_SHOW);
    break;

  case DISPLAY_MODE_LAB_BRAIN:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, dpiScaledHeight, clientW - dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    theApp.GetBrainViewPtr()->GetParentFrame()->ShowWindow(SW_SHOW);
    break;

  case DISPLAY_MODE_STATISTIC:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, dpiScaledHeight, clientW - dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    theApp.GetStatisticViewPtr()->GetParentFrame()->ShowWindow(SW_SHOW);
    break;

  case DISPLAY_MODE_MAP:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, dpiScaledHeight, clientW - dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    theApp.GetMapConfigViewPtr()->GetParentFrame()->ShowWindow(SW_SHOW);
    break;

  case DISPLAY_MODE_MAPEDITOR:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, dpiScaledHeight, clientW - dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    theApp.GetMapEditorViewPtr()->GetParentFrame()->ShowWindow(SW_SHOW);
    break;

  case DISPLAY_MODE_LOGSERVER:
    theApp.GetBiotopViewPtr()->GetParentFrame()->SetWindowPos(&wndTop, dpiScaledWidth, dpiScaledHeight, clientW - dpiScaledWidth, clientH - dpiScaledHeight, SWP_SHOWWINDOW);
    hideAllToolsWindows();
    theApp.GetLogServerViewPtr()->GetParentFrame()->ShowWindow(SW_SHOW);
    break;
  }
}


void CMainFrame::OnAppSelectView() 
{
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_VIEW,true);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CREATE,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_STICK,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CARROT,false);

  m_SelectionMode = SELECT_MODE_VIEW;
}

void CMainFrame::OnAppSelectCreate() 
{
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_VIEW,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CREATE,true);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_STICK,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CARROT,false);

  m_SelectionMode = SELECT_MODE_CREATE;
}

void CMainFrame::OnAppSelectStick() 
{
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_VIEW,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CREATE,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_STICK,true);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CARROT,false);

  m_SelectionMode = SELECT_MODE_STICK;
}

void CMainFrame::OnAppSelectCarrot() 
{
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_VIEW,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CREATE,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_STICK,false);
  m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_SELECT_CARROT,true);

  m_SelectionMode = SELECT_MODE_CARROT;
}

void CMainFrame::OnAppDisplay2d() 
{
  if (m_ViewMode != VIEW_MODE_2D)
  {
    theApp.GetBiotopViewPtr()->SwitchToDisplay2d();
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2D,true);
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_3D,false);
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2DSAT, false);
    m_ViewMode = VIEW_MODE_2D;

    CMenu* myMenu = GetMenu( );
    myMenu->EnableMenuItem(ID_APP_DISPLAY_2D, MF_GRAYED);
    myMenu->EnableMenuItem(ID_APP_DISPLAY_2D, MF_ENABLED);
  }
}

void CMainFrame::OnAppDisplay3d() 
{
  if (m_ViewMode != VIEW_MODE_3D)
  {
    theApp.GetBiotopViewPtr()->SwitchToDisplay3d();
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_3D,true);
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2D,false);
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2DSAT, false);
    m_ViewMode = VIEW_MODE_3D;
  }
}

void CMainFrame::OnAppDisplay2dSat()
{
  if (m_ViewMode != VIEW_MODE_2DSAT)
  {
    theApp.GetBiotopViewPtr()->SwitchToDisplay2dSat();
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2D, false);
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_3D, false);
    m_wndToolBar.GetToolBarCtrl().PressButton(ID_APP_DISPLAY_2DSAT, true);
    m_ViewMode = VIEW_MODE_2DSAT;
  }
}

void CMainFrame::OnAppFindEntity() 
{
  CEntityFindDlg dlg(theApp.GetBiotop());
  
	long nResp = dlg.DoModal(); 
	if (nResp == IDOK)
	{
    entityIdType entityId = dlg.GetSelectionId();
    if (entityId != ENTITY_ID_INVALID)
    {
      CBasicEntity* pSelEnt = theApp.GetBiotop()->getEntityById(entityId);
      if (dlg.IsForceSelection())
        theApp.setSelectedEntity(pSelEnt);
      theApp.GetBiotopViewPtr()->ScrollToGridCoord(theApp.GetBiotop()->getEntityById(entityId)->getGridCoord());
    }
  }
}