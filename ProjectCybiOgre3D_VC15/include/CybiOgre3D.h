/*
-----------------------------------------------------------------------------
Filename:    CybiOgre3D.h
-----------------------------------------------------------------------------

This source file is generated by the Ogre AppWizard.

Check out: http://conglomerate.berlios.de/wiki/doku.php?id=ogrewizards

Based on the Example Framework for OGRE
(Object-oriented Graphics Rendering Engine)

Copyright (c) 2000-2007 The OGRE Team
For the latest info, see http://www.ogre3d.org/

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the OGRE engine.
-----------------------------------------------------------------------------
*/
#ifndef __CybiOgre3D_h_
#define __CybiOgre3D_h_


#include "CybiOgreApplication.h"
/*
#include "ItemSelectorViewManager.h"
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include "OgreCEGUIRenderer.h"
*/
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include "CBiotop.h"

Real mAnimChop = 7.96666f;
Real mAnimChopBlend = 0.3f;

bool initComplete = false;

typedef struct 
{
  CBasicEntity*        pBasicEntity;
  Entity*              pMeshEnt;
  SceneNode*           pMeshNode;
  AnimationState*      pAnimState;
  Point_t              targetBiotopCoord;
  Vector3              translateVect3;
  int                  curDirection;
  int                  curRotation;
  bool                 isMoving;
  string               strCurACtion;
  double               scale;
  double               yPos;
} MeshEntity_t;

typedef struct 
{
  CBasicEntity*        pBasicEntity;
  choiceIndType        actionId_idle;
  choiceIndType        actionId_walk;
  choiceIndType        actionId_run;
  choiceIndType        actionId_turnLeft;
  choiceIndType        actionId_turnRight;
  choiceIndType        actionId_attack;
} PlayerEntity_t;

std::vector<MeshEntity_t*>   m_tMesh;
CBiotop*                     m_pBiotop;
PlayerEntity_t               m_Player;

class CybiOgre3DFrameListener : public ExampleFrameListener//, ItemSelectorInterface
{
private:
  SceneManager* mSceneMgr;
  Real          m_secCnt;
  Real          m_TimeSpeed;
  bool          m_bPlayerActionWalkOnGoing;
  bool          m_bPlayerActionRunOnGoing;
  bool          m_bPlayerActionTurnLeftOnGoing;
  bool          m_bPlayerActionTurnRightOnGoing;
  bool          m_bPlayerActionAttacktOnGoing;

  /*
  CEGUI::OgreCEGUIRenderer*    mGUIRenderer;
  CEGUI::System*        mGUISystem;
  ItemSelectorViewManager* mCompositorSelectorViewManager;
  CEGUI::Renderer* mGuiRenderer;
  CEGUI::Window* mGuiAvg;
  CEGUI::Window* mGuiCurr;
  CEGUI::Window* mGuiBest;
  CEGUI::Window* mGuiWorst;
  CEGUI::Window* mGuiTris;
  CEGUI::Window* mGuiDbg;
  CEGUI::Window* mRoot;*/

public:
  CybiOgre3DFrameListener(SceneManager *sceneMgr, RenderWindow* win, Camera* cam);
  bool frameStarted(const FrameEvent& evt);
  void updateMeshEntityNewSecond(int index);
  void updateMeshEntityPosition(int index, Real rate);
  void setMeshEntityPosition(int index);
  void updateInfoParamTime();
  void updateInfoPopulation();
  void speedVariation(int variation);
  void setPlayerActionIdle();
  void setPlayerActionWalk();
  void setPlayerActionRun();
  void setPlayerActionTurnLeft();
  void setPlayerActionTurnRight();
  void setPlayerActionTurnAttack();
  void forcePlayerAction();
};


class CybiOgre3DApp : public ExampleApplication
{
private:
   //Overlay* m_Overlay;
   Light* mSunLight;

public:
  CybiOgre3DApp();
  ~CybiOgre3DApp();

protected:

  virtual void createCamera(void);
  virtual bool configure(void);

  // Just override the mandatory create scene method
  virtual void createScene(void);

  // Create new frame listener
  void createFrameListener(void);

  bool createMeshEntity (CBasicEntity* pBasicEntity, int insertIndex);
  bool SetEntityPlayer(CBasicEntity* pEntity);
};

#endif // #ifndef __CybiOgre3D_h_