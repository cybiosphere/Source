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

// Cybiosphere.h : main header file for the CYBIOSPHERE application
//

#if !defined(AFX_CYBIOSPHERE_H__914C6185_93F4_4535_97CB_3888153592DC__INCLUDED_)
#define AFX_CYBIOSPHERE_H__914C6185_93F4_4535_97CB_3888153592DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "BiotopDoc.h"
#include "BiotopView.h"
#include "EntityDoc.h"
#include "EntityView.h"
#include "BioCtrlDoc.h"
#include "BioCtrlView.h"
#include "BrainDoc.h"
#include "BrainView.h"
#include "GeneticDoc.h"
#include "GeneticView.h"
#include "StatisticDoc.h"
#include "StatisticView.h"
#include "LogServerDoc.h"
#include "LogServerView.h"
#include "MapConfigDoc.h"
#include "MapConfigView.h"
#include "CScenarioPlayer.h"

#ifdef USE_CLAN_SERVER
#include "clan_server.h"
#include "API/Core/Text/logger.h"
#endif

#ifdef USE_CLAN_CLIENT
#include "clan_client.h"
#include "API/Core/Text/logger.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereApp:
// See Cybiosphere.cpp for the implementation of this class
//

class CCybiosphereApp : public CWinApp
{
public:
  CCybiosphereApp();
  ~CCybiosphereApp();

  bool RefreshAllWithNewEntity(CBasicEntity* pEntity);

  CBiotopView*    GetBiotopViewPtr(void);
  CEntityView*    GetEntityViewPtr(void);
  CBioCtrlView*   GetBioCtrlViewPtr(void);
  CBrainView*     GetBrainViewPtr(void);
  CGeneticView*   GetGeneticViewPtr(void);
  CStatisticView* GetStatisticViewPtr(void);
  CLogServerView* GetLogServerViewPtr(void);
  CMapConfigView* GetMapConfigViewPtr(void);
 
  BOOL IsModeManual();
  void SetModeManual(BOOL isManual);
  BOOL IsModeStopOnEvent();
  void SetModeStopOnEvent(BOOL isStop);

  CBiotop* GetBiotop();
  CString GetAppliPath();
  CBasicEntity* GetpSelectedEntity();

  void NextSecondStart();
  void NextSecondRefreshAllViews();
  void NextSecondRefreshAllViewsLowCPU();
  void CheckSelectedEntity();

  CScenarioPlayer* GetScenarioPlayer();

  void SetDefaultBiotop();

  void SetOpenedBiotopFile (CString path, CString fileName);
  CString GetOpenedBiotopFileName();
  CString GetOpenedBiotopPath();

  void removeEntityFromBiotop(CBasicEntity* pEntity);
  void addEntityFromFileInBiotop(string fileName, string pathName, Point_t coord);
  void modifyBiotopSpeed(const float newBiotopSpeed);
  feedbackValType forceEntityAction(const entityIdType entityId, const int actionIndex);
  void updateAllBiotopNewMeasures();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCybiosphereApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCybiosphereApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppViewLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateNewBiotopView(CBiotop* pBiotop);
	void CreateEntityView();
	void CreateBioCtrlView(CBiotop* pBiotop);
	void CreateBrainView();
	void CreateGeneticView();
	void CreateStatisticView(CBiotop* pBiotop);
	void CreateMapConfigView(CBiotop* pBiotop);
  void CreateLogServerView();

  CBiotop*        m_pBiotop;
  CBiotopDoc*     m_pBiotopDoc;
  CBiotopView*    m_pBiotopView;
  CEntityDoc*     m_pEntityDoc;
  CEntityView*    m_pEntityView;
  CBioCtrlDoc*    m_pBioCtrlDoc;
  CBioCtrlView*   m_pBioCtrlView;
  CBrainDoc*      m_pBrainDoc;
  CBrainView*     m_pBrainView;
  CGeneticDoc*    m_pGeneticDoc;
  CGeneticView*   m_pGeneticView;
  CStatisticDoc*  m_pStatisticDoc;
  CStatisticView* m_pStatisticView;
  CLogServerDoc*  m_pLogServerDoc;
  CLogServerView* m_pLogServerView;
  CMapConfigDoc*  m_pMapConfigDoc;
  CMapConfigView* m_pMapConfigView;
#ifdef USE_CLAN_SERVER
  Server*         m_pServer;
#endif
#ifdef USE_CLAN_CLIENT
  Client* m_pClient;
#endif
  BOOL m_bModeManual;
  BOOL m_bModeStopOnEvent;
  CString m_AppliPath;
  CBasicEntity* m_pSelectedEntity;
  CScenarioPlayer* m_pScenarioPlayer;
  CString m_OpenedBiotopPath;
  CString m_OpenedBiotopFile;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYBIOSPHERE_H__914C6185_93F4_4535_97CB_3888153592DC__INCLUDED_)
