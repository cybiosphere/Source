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

// Cybiosphere.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Cybiosphere.h"
#include "CEntityFactory.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "BiotopDoc.h"
#include "BiotopView.h"
#include "StartupHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereApp

BEGIN_MESSAGE_MAP(CCybiosphereApp, CWinApp)
	//{{AFX_MSG_MAP(CCybiosphereApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_VIEW_LOG, OnAppViewLog)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereApp construction

CCybiosphereApp::CCybiosphereApp()
{
  m_pBiotop = NULL;
  m_pEntityDoc   = NULL;
  m_pEntityView  = NULL;
  m_pBioCtrlDoc  = NULL;
  m_pBioCtrlView = NULL;
  m_bModeManual = false;
  m_bModeMaxSpeed = false;
  m_bModeStopOnEvent = false;
  m_MaxSpeedStepfactor = 1;
  m_pSelectedEntity = NULL;
  m_pScenarioPlayer = NULL;
  m_OpenedBiotopPath = "";
  m_OpenedBiotopFile = "";
#ifdef USE_CLAN_SERVER
  m_pServer = NULL;
#endif
}

CCybiosphereApp::~CCybiosphereApp()
{
  if (m_pBiotop!=NULL)
    delete m_pBiotop;

  if (m_pScenarioPlayer!=NULL)
    delete m_pScenarioPlayer;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCybiosphereApp object

CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereApp initialization

BOOL CCybiosphereApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplateBiotop;
	pDocTemplateBiotop = new CMultiDocTemplate(
		IDR_BIOTOP_TYPE,
		RUNTIME_CLASS(CBiotopDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CBiotopView));
	AddDocTemplate(pDocTemplateBiotop);

	CMultiDocTemplate* pDocTemplateEntity;
	pDocTemplateEntity = new CMultiDocTemplate(
		IDR_ENTITY_TYPE,
		RUNTIME_CLASS(CEntityDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CEntityView));
	AddDocTemplate(pDocTemplateEntity);

	CMultiDocTemplate* pDocTemplateBioCtrl;
	pDocTemplateBioCtrl = new CMultiDocTemplate(
		IDR_BIOCTRL_TYPE,
		RUNTIME_CLASS(CBioCtrlDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CBioCtrlView));
	AddDocTemplate(pDocTemplateBioCtrl);


	CMultiDocTemplate* pDocTemplateBrain;
	pDocTemplateBrain = new CMultiDocTemplate(
		IDR_BRAIN_TYPE,
		RUNTIME_CLASS(CBrainDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CBrainView));
	AddDocTemplate(pDocTemplateBrain);

	CMultiDocTemplate* pDocTemplateGenetic;
	pDocTemplateGenetic = new CMultiDocTemplate(
		IDR_GENETIC_TYPE,
		RUNTIME_CLASS(CGeneticDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGeneticView));
	AddDocTemplate(pDocTemplateGenetic);

	CMultiDocTemplate* pDocTemplateStatistic;
	pDocTemplateStatistic = new CMultiDocTemplate(
		IDR_STATISTIC_TYPE,
		RUNTIME_CLASS(CStatisticDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CStatisticView));
	AddDocTemplate(pDocTemplateStatistic);

	CMultiDocTemplate* pDocTemplateMapConfig;
	pDocTemplateMapConfig = new CMultiDocTemplate(
		IDR_MAPCONFIG_TYPE,
		RUNTIME_CLASS(CMapConfigDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMapConfigView));
	AddDocTemplate(pDocTemplateMapConfig);

  CMultiDocTemplate* pDocTemplateLogServer;
  pDocTemplateLogServer = new CMultiDocTemplate(
    IDR_LOGSERVER_TYPE,
    RUNTIME_CLASS(CLogServerDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CLogServerView));
  AddDocTemplate(pDocTemplateLogServer);

  CMultiDocTemplate* pDocTemplateMapEditor;
  pDocTemplateMapEditor = new CMultiDocTemplate(
    IDR_MAPEDITOR_TYPE,
    RUNTIME_CLASS(CMapEditorDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CMapEditorView));
  AddDocTemplate(pDocTemplateMapEditor);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

  cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  // Save the application path
  m_AppliPath = m_pszHelpFilePath; 
  int endPath = m_AppliPath.ReverseFind ('\\');
  if (endPath>0)
    m_AppliPath = m_AppliPath.Left(endPath+1); 

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

  string fileIni = m_AppliPath + "\\Cybiosphere.ini";
  string ServerPortStr{ getIpPortStrFromIniFile(fileIni) };

#ifndef USE_CLAN_CLIENT
  // Init biotop and startup scenario if set
  createBiotopAndScenarioFromIniFile(fileIni, &m_pBiotop, &m_pScenarioPlayer);
#else
  // Get Server info in ini file
  string ServerAddrStr{ getIpServerAddrFromIniFile(fileIni) };
  string LogInStr{ getIpServerLoginFromIniFile(fileIni) };

  m_pClient = new Client(ServerAddrStr, ServerPortStr, LogInStr);

  // Connect to server and wait for biotop init from server
  m_pClient->connect_to_server();
  BeginWaitCursor();

  for (int i = 0; i < 2000; i++)
  {
    System::sleep(10);
    m_pClient->process_new_events();
    if (m_pClient->is_logged_in())
      break;
  }
  if (m_pClient->is_logged_in() != true)
  {
    AfxMessageBox("Loggin FAILED. Cannot reach server");
    return false;
  }

  while (!(m_pClient->is_biotop_config_complete()))
  {
    System::sleep(10);
    m_pClient->process_new_events();
  }
  EndWaitCursor();
  m_pBiotop = m_pClient->get_pBiotop();
  m_pScenarioPlayer = new CScenarioPlayer(m_pBiotop);
#endif

#ifdef USE_CLAN_SERVER
  m_pServer = new Server(ServerPortStr, m_pBiotop, m_pScenarioPlayer);
  m_pServer->startServer();
#endif

  CreateEntityView();
  CreateNewBiotopView(m_pBiotop);
  CreateBioCtrlView(m_pBiotop);
  CreateBrainView();
  CreateGeneticView();
  CreateStatisticView(m_pBiotop);
  CreateMapConfigView(m_pBiotop);
  CreateMapEditorView(m_pBiotop);
  CreateLogServerView();

  // Start timers
  GetBiotopViewPtr()->StartTimers();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCybiosphereApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CCybiosphereApp::OnAppViewLog()
{
  CString execString = LPCTSTR("notepad ") + GetAppliPath() + LPCTSTR("CybioCore.log");
  WinExec((char*)(execString.GetBuffer(0)),SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereApp message handlers

void CCybiosphereApp::CreateNewBiotopView(CBiotop* pBiotop)
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<1;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pBiotopDoc = (CBiotopDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pBiotopDoc->GetFirstViewPosition();
  m_pBiotopView = (CBiotopView*)(m_pBiotopDoc->GetNextView(pos));
  m_pBiotopView->SetBiotop(pBiotop);
}

CBiotopView* CCybiosphereApp::GetBiotopViewPtr()
{
  return (m_pBiotopView);
}

void CCybiosphereApp::CreateEntityView()
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<2;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pEntityDoc = (CEntityDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pEntityDoc->GetFirstViewPosition();
  m_pEntityView = (CEntityView*)(m_pEntityDoc->GetNextView(pos));
}

CEntityView* CCybiosphereApp::GetEntityViewPtr()
{
  return (m_pEntityView);
}

void CCybiosphereApp::CreateBioCtrlView(CBiotop* pBiotop)
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<3;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pBioCtrlDoc = (CBioCtrlDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pBioCtrlDoc->GetFirstViewPosition();
  m_pBioCtrlView = (CBioCtrlView*)(m_pBioCtrlDoc->GetNextView(pos));
  m_pBioCtrlView->SetBiotop(pBiotop);
}

CBioCtrlView* CCybiosphereApp::GetBioCtrlViewPtr()
{
  return (m_pBioCtrlView);
}

BOOL CCybiosphereApp::IsModeManual()
{
  return (m_bModeManual);
}

void CCybiosphereApp::SetModeManual(BOOL isManual)
{
  if (m_bModeManual != isManual)
  {
    m_bModeManual = isManual;
    m_pBioCtrlView->ForceModeManual(m_bModeManual);
    modifyBiotopSpeed(GetBiotopViewPtr()->GetSpeedRate());
#ifdef USE_CLAN_SERVER
    m_pServer->set_manual_mode(isManual);
#elif USE_CLAN_CLIENT
    m_pClient->set_manual_mode(isManual);
#endif
  }
}

BOOL CCybiosphereApp::IsModeStopOnEvent()
{
  return (m_bModeStopOnEvent);
}

void CCybiosphereApp::SetModeStopOnEvent(BOOL isStop)
{
  m_bModeStopOnEvent = isStop;
}

void CCybiosphereApp::SetModeMaxSpeed(BOOL isMaxSpeed)
{
  if (m_bModeMaxSpeed != isMaxSpeed)
  {
    m_bModeMaxSpeed = isMaxSpeed;
#ifdef USE_CLAN_CLIENT
    m_pClient->set_maxSpeed_mode(isMaxSpeed);
#else
    m_MaxSpeedStepfactor = computeMaxSpeedStepfactor(m_pBiotop, m_bModeMaxSpeed);
#if USE_CLAN_SERVER
    m_pServer->set_maxSpeed_mode(isMaxSpeed);
#endif // USE_CLAN_SERVER
#endif // USE_CLAN_CLIENT
  }
}

void CCybiosphereApp::SetMapEditorMode(MapEditorModeType_e editMode)
{
  m_editMode = editMode;
}

MapEditorModeType_e CCybiosphereApp::GetMapEditorMode(void)
{
  return m_editMode;
}

void CCybiosphereApp::CreateBrainView()
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<4;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pBrainDoc = (CBrainDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pBrainDoc->GetFirstViewPosition();
  m_pBrainView = (CBrainView*)(m_pBrainDoc->GetNextView(pos));
}

CBrainView* CCybiosphereApp::GetBrainViewPtr()
{
  return (m_pBrainView);
}

CBiotop* CCybiosphereApp::GetBiotop()
{
  return (m_pBiotop);
}

void CCybiosphereApp::CreateGeneticView()
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<5;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pGeneticDoc = (CGeneticDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pGeneticDoc->GetFirstViewPosition();
  m_pGeneticView = (CGeneticView*)(m_pGeneticDoc->GetNextView(pos));
}

CGeneticView* CCybiosphereApp::GetGeneticViewPtr()
{
  return (m_pGeneticView);
}

void CCybiosphereApp::CreateStatisticView(CBiotop* pBiotop)
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<6;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pStatisticDoc = (CStatisticDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pStatisticDoc->GetFirstViewPosition();
  m_pStatisticView = (CStatisticView*)(m_pStatisticDoc->GetNextView(pos));
  m_pStatisticView->SetBiotop(pBiotop);
}

CStatisticView* CCybiosphereApp::GetStatisticViewPtr()
{
  return (m_pStatisticView);
}

void CCybiosphereApp::CreateMapConfigView(CBiotop* pBiotop)
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition( );
  for (int i=0; i<7;i++)
    pDocTempl = GetNextDocTemplate (pos);
  m_pMapConfigDoc = (CMapConfigDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pMapConfigDoc->GetFirstViewPosition();
  m_pMapConfigView = (CMapConfigView*)(m_pMapConfigDoc->GetNextView(pos));
  m_pMapConfigView->SetBiotop(pBiotop);
}

CMapConfigView* CCybiosphereApp::GetMapConfigViewPtr()
{
  return (m_pMapConfigView);
}

void CCybiosphereApp::CreateLogServerView()
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition();
  for (int i = 0; i<8; i++)
    pDocTempl = GetNextDocTemplate(pos);
  m_pLogServerDoc = (CLogServerDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pLogServerDoc->GetFirstViewPosition();
  m_pLogServerView = (CLogServerView*)(m_pLogServerDoc->GetNextView(pos));
}

CLogServerView* CCybiosphereApp::GetLogServerViewPtr()
{
  return (m_pLogServerView);
}

void CCybiosphereApp::CreateMapEditorView(CBiotop* pBiotop)
{
  CDocTemplate* pDocTempl;
  POSITION pos = GetFirstDocTemplatePosition();
  for (int i = 0; i < 9; i++)
    pDocTempl = GetNextDocTemplate(pos);
  m_pMapEditorDoc = (CMapEditorDoc*)pDocTempl->OpenDocumentFile(NULL);
  pos = m_pMapEditorDoc->GetFirstViewPosition();
  m_pMapEditorView = (CMapEditorView*)(m_pMapEditorDoc->GetNextView(pos));
  m_pMapEditorView->SetBiotop(pBiotop);
}

CMapEditorView* CCybiosphereApp::GetMapEditorViewPtr()
{
  return (m_pMapEditorView);
}

#ifdef USE_CLAN_CLIENT
void CCybiosphereApp::WaitForEventRefreshEntity()
{
  for (int retry = 0; retry < 1000; retry++)
  {
    Sleep(10);
    m_pClient->process_new_events();
    BiotopEventPair eventPair = m_pBiotop->getNextUnreadGuiBiotopEvent();
    if (eventPair.first != ENTITY_ID_INVALID)
    {
      if (eventPair.second.eventList.test(BIOTOP_EVENT_ENTITY_MODIFIED) || eventPair.second.eventList.test(BIOTOP_EVENT_ENTITY_ADDED))
      {
        return;
      }
    }
  }
}
#endif

bool CCybiosphereApp::setSelectedEntity(CBasicEntity* pEntity)
{
  m_pSelectedEntity = pEntity;
#ifdef USE_CLAN_CLIENT
  if ((pEntity != NULL) && (pEntity->isRemoteControlled()))
  {
    entityIdType entityId = pEntity->getId();
    m_pClient->send_event_request_entity_refresh(pEntity->getId(), pEntity->getLabel());
    // Wait for entity update from server (TODO: confirm reception)
    WaitForEventRefreshEntity();
    m_pSelectedEntity = m_pBiotop->getEntityById(entityId);
  }
#endif
  RefreshAllWithNewEntity(m_pSelectedEntity);
  return (true);
}

bool CCybiosphereApp::updateSelectedEntity(CBasicEntity* pEntity)
{
  m_pSelectedEntity = pEntity;
#ifdef USE_CLAN_CLIENT
  if (pEntity != NULL)
  {
    entityIdType entityId = pEntity->getId();
    m_pClient->send_event_update_entity_data(pEntity);
    // Wait for entity update from server (TODO: confirm reception)
    WaitForEventRefreshEntity();
    m_pSelectedEntity = m_pBiotop->getEntityById(entityId);
  }
#endif
  RefreshAllWithNewEntity(m_pSelectedEntity);
  return (true);
}

bool CCybiosphereApp::refreshSelectedEntity(CBasicEntity* pEntity)
{
  m_pSelectedEntity = pEntity;
#ifdef USE_CLAN_CLIENT
  if (pEntity != NULL)
  {
    m_pClient->send_event_update_entity_position(pEntity);
  }
#endif
  RefreshAllWithNewEntity(m_pSelectedEntity);
  return (true);
}

bool CCybiosphereApp::RefreshAllWithNewEntity(CBasicEntity* pEntity)
{
  GetBiotopViewPtr()->SetSelectedEntity(pEntity);
  GetEntityViewPtr()->SelectAndDisplayEntity(pEntity);
  GetGeneticViewPtr()->SetEntity(pEntity);
  GetMapConfigViewPtr()->UpdateSelectedEntity(pEntity);
  return (true);
}

CString CCybiosphereApp::GetAppliPath()
{
  return (m_AppliPath);
}

CBasicEntity* CCybiosphereApp::GetpSelectedEntity()
{
  return (m_pSelectedEntity);
}


void CCybiosphereApp::NextSecondStart()
{
#ifdef USE_CLAN_CLIENT
  if (fabs(m_pClient->get_biotop_speed() - GetBiotopViewPtr()->GetSpeedRate()) > 0.01)
  {
    GetBioCtrlViewPtr()->ForceSetSpeed(m_pClient->get_biotop_speed());
    GetBiotopViewPtr()->SetSpeedRate(m_pClient->get_biotop_speed());
  }
  SetModeManual(m_pClient->get_manual_mode());
  SetModeMaxSpeed(m_pClient->get_maxSpeed_mode());
  GetLogServerViewPtr()->AddLog(Logger::getOnGoingString().c_str());
#endif
}


void CCybiosphereApp::NextSecondRefreshAllViews()
{
  NextSecondRefreshAllViewsLowCPU();
  // additional Gui refresh
  GetEntityViewPtr()->RefreshParameters();
  GetStatisticViewPtr()->NextSecond();
  GetMapConfigViewPtr()->RefreshDisplay();
}

void CCybiosphereApp::NextSecondRefreshAllViewsLowCPU()
{
  CheckSelectedEntity();
  GetBioCtrlViewPtr()->UpdateTimerDisplay(m_pBiotop->getBiotopTime());

#ifdef USE_CLAN_SERVER
  SetModeManual(m_pServer->get_manual_mode());
  SetModeMaxSpeed(m_pServer->get_maxSpeed_mode());
  if (fabs(m_pServer->get_biotop_speed() - GetBiotopViewPtr()->GetSpeedRate()) > 0.01)
  {
    GetBioCtrlViewPtr()->ForceSetSpeed(m_pServer->get_biotop_speed());
  }
  GetLogServerViewPtr()->AddLog(Logger::getOnGoingString().c_str());
#endif
}

void CCybiosphereApp::proceedBiotopEvents()
{
  GetBiotopViewPtr()->SoftRefreshDisplay();
#ifdef USE_CLAN_CLIENT
  m_pClient->process_new_events();
  m_pClient->processBiotopEvents();
#endif
#ifdef USE_CLAN_SERVER
  m_pServer->process_new_events();
  m_pServer->processBiotopEvents();
#endif
}

void CCybiosphereApp::CheckSelectedEntity()
{
  if ( (m_pSelectedEntity!=NULL) && (m_pSelectedEntity->isToBeRemoved()) )
  {
    GetBiotopViewPtr()->SetSelectedEntity(NULL);
    GetEntityViewPtr()->SelectAndDisplayEntity(NULL);
    GetGeneticViewPtr()->SetEntity(NULL);
    m_pSelectedEntity = NULL;
  }
}

CScenarioPlayer* CCybiosphereApp::GetScenarioPlayer()
{
  return m_pScenarioPlayer;
}

void CCybiosphereApp::CreateNewDefaultBiotop(size_t sizeX, size_t sizeY)
{
  if (m_pBiotop!=NULL)
  {
    m_pBiotop->resetResizeAndAddDefaultEntities(sizeX, sizeY);
  }
}

void CCybiosphereApp::SetOpenedBiotopFile(CString path, CString fileName)
{
  m_OpenedBiotopPath = path;
  m_OpenedBiotopFile = fileName;
}

CString CCybiosphereApp::GetOpenedBiotopFileName()
{
  return m_OpenedBiotopFile;
}

CString CCybiosphereApp::GetOpenedBiotopPath()
{
  return m_OpenedBiotopPath;
}

void CCybiosphereApp::removeEntityFromBiotop(CBasicEntity* pEntity)
{
#ifdef USE_CLAN_CLIENT
  if ((pEntity != NULL) && (pEntity->getId() > 0))
  {
    m_pClient->send_event_remove_entity(pEntity, pEntity->getId());
  }
#else
  if ((pEntity != NULL) && (pEntity->getId() > 0))
  {
    pEntity->autoRemove();
  }
#endif
}

void CCybiosphereApp::addEntityFromFileInBiotop(string fileName, string pathName, Point_t globalCoord)
{
#ifdef USE_CLAN_CLIENT
  CBasicEntity* pEntity = CEntityFactory::createEntity(pathName + fileName);
  if (pEntity != NULL)
  {
    pEntity->jumpToGridCoord(globalCoord, false);
    m_pClient->send_event_add_entity(pEntity);
    // Wait for creation by server
    WaitForEventRefreshEntity();
  }
#else
  m_pBiotop->createAndAddEntity(fileName, pathName, globalCoord);
#endif
}

void CCybiosphereApp::modifyBiotopSpeed(const float newBiotopSpeed)
{
#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_change_biotop_speed(newBiotopSpeed, m_bModeManual, m_bModeMaxSpeed);
#else
#ifdef USE_CLAN_SERVER
  m_pServer->send_event_change_biotop_speed(newBiotopSpeed, m_bModeManual, m_bModeMaxSpeed);
#endif
  GetBiotopViewPtr()->SetSpeedRate(newBiotopSpeed);
#endif
}

feedbackValType CCybiosphereApp::forceEntityAction(const entityIdType entityId, const int actionIndex)
{
#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_force_entity_action(entityId, actionIndex);
#endif
  return GetBiotop()->forceEntityAction(entityId, actionIndex);
}

void CCybiosphereApp::updateAllBiotopNewMeasures()
{
  CMeasure* getMeasureByIndex(int index);
  for (int idx = 0; idx < m_pBiotop->getNbOfMeasures(); idx++)
  {
    CMeasure* pMeasure = m_pBiotop->getMeasureByIndex(idx);
    if ((pMeasure != NULL) && (pMeasure->GetTotalMeasureNumberFromStart() < 2))
    {
#ifdef USE_CLAN_CLIENT
      m_pClient->send_event_create_measure(pMeasure);
#endif // USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
      m_pServer->send_event_create_measure(pMeasure);
#endif // USE_CLAN_SERVER
    }
  }
  GetStatisticViewPtr()->RebuildMeasChkBox();
}

void CCybiosphereApp::addEntitySpawnerInBiotop(int index, string entityFileName, string pathName, int intensityRate, int avaragePeriod, bool isProportionalToFertility)
{
  m_pBiotop->addEntitySpawner(index, entityFileName, pathName, intensityRate, avaragePeriod, isProportionalToFertility);
  BiotopRandomEntitiyGeneration_t& generator = m_pBiotop->getRandomEntitiyGeneration(index);
#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_add_entity_spawner(index, generator);
#endif // USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
  m_pServer->send_event_add_entity_spawner(index, generator);
#endif // USE_CLAN_SERVER
}

void CCybiosphereApp::proceedBiotopNextStep()
{
#ifndef USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
  if (!m_pServer->checkAllCoprocessorCompleteSecond())
    return;
  proceedBiotopEvents();
  m_pServer->send_event_new_second_start();
#endif
  for (int i = 0; i < m_MaxSpeedStepfactor; i++)
  {
    m_pBiotop->nextSecond(true);
#ifdef USE_CLAN_SERVER
    m_pServer->send_event_new_second_end();
#endif
  }
#endif // !USE_CLAN_CLIENT
}

void CCybiosphereApp::addGeomapSpecieInBiotop(std::string specieName)
{
  m_pBiotop->addGeoMapSpeciePopulation(specieName);
  CGeoMapPopulation* pGeoMapPopu = m_pBiotop->getGeoMapSpecieByIndex(m_pBiotop->getNbOfGeoMapSpecie() - 1);
#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_create_specie_map(pGeoMapPopu);
#endif // USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
  m_pServer->send_event_create_specie_map(pGeoMapPopu);
#endif // USE_CLAN_SERVER
}

void CCybiosphereApp::addGeneticMarker(CGene& modelGene, bool markDominantAlleleOnly)
{
  m_pBiotop->markAllEntitiesWithGene(modelGene, markDominantAlleleOnly);
  GetBiotopViewPtr()->ForceRefreshDisplay(true);

#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_mark_entities_with_gene(modelGene, markDominantAlleleOnly);
#endif // USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
  m_pServer->send_event_mark_entities_with_gene(modelGene, markDominantAlleleOnly);
#endif // USE_CLAN_SERVER
}

void CCybiosphereApp::clearGeneticMarker()
{
  m_pBiotop->clearMarksOnAllEntities();
  GetBiotopViewPtr()->ForceRefreshDisplay(true);

#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_mark_entities_with_gene(CGene{}, false);
#endif // USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
  m_pServer->send_event_mark_entities_with_gene(CGene{}, false);
#endif // USE_CLAN_SERVER
}

void CCybiosphereApp::updateBiotopClimate(const ClimateType_e newClimateType, const int newPeriod)
{
  m_pBiotop->setClimateModel(newClimateType, newPeriod);

#ifdef USE_CLAN_CLIENT
  m_pClient->send_event_update_biotop_climate(*m_pBiotop->getParamFertility(), *m_pBiotop->getParamTemperature());
#endif // USE_CLAN_CLIENT
#ifdef USE_CLAN_SERVER
  m_pServer->send_event_update_biotop_climate(*m_pBiotop->getParamFertility(), *m_pBiotop->getParamTemperature());
#endif // USE_CLAN_SERVER
}