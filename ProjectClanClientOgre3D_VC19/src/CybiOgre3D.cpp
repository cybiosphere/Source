/*
-----------------------------------------------------------------------------
Filename:    CybiOgre3D.cpp
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

#include "CybiOgre3D.h"
#include "clan_client.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

int OFFSET_COORD_X = 2000;
int OFFSET_COORD_Y = 2000;

Real mAnimChop = 7.96666f;
Real mAnimChopBlend = 0.3f;
bool initComplete = false;
std::vector<MeshEntity_t*>   m_tMesh;
CBiotop*                     m_pBiotop;
PlayerEntity_t               m_Player;
Client*                      m_pClient;

void removeMeshEntity(SceneManager* pSceneMgr, CBasicEntity* pEntity )
{
  std::vector<MeshEntity_t*>::iterator it;
  for (it = m_tMesh.begin(); it != m_tMesh.end(); ++it)
  {
    MeshEntity_t* pMesh = *it;
    if (pMesh->pBasicEntity == pEntity)
    {
      pSceneMgr->destroyEntity(pMesh->pMeshEnt);
      m_tMesh.erase(it);
      return;
    }
  }
  return;
}

bool createMeshEntity (SceneManager* pSceneMgr, CBasicEntity* pBasicEntity)
{
  if ( (pBasicEntity == NULL) || (pBasicEntity->isToBeRemoved()) )
    return false;

  Point_t coord = pBasicEntity->getStepCoord();

  Entity *meshEnt;
  string nameEnt =  pBasicEntity->getSpecieName() + StringConverter::toString(pBasicEntity->getId());
  string nameFile = pBasicEntity->getSpecieName() + ".mesh";
  double scale = pBasicEntity->getSizeRate();
  meshEnt = pSceneMgr->createEntity(nameEnt, nameFile);

  MeshEntity_t* pNewMesh = new MeshEntity_t;

  if (pNewMesh!=NULL)
  {
    pNewMesh->pBasicEntity = pBasicEntity;
    pNewMesh->entityId = pBasicEntity->getId();
    pNewMesh->pMeshEnt = meshEnt;
    pNewMesh->targetBiotopCoord = coord;
    pNewMesh->curDirection = pBasicEntity->getStepDirection();
    pNewMesh->isMoving = false;
    pNewMesh->scale = scale;
    pNewMesh->yPos = scale-1.0;
    pNewMesh->pMeshNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
    pNewMesh->pMeshNode->attachObject(meshEnt);
    pNewMesh->pMeshNode->setPosition( Vector3(coord.y-OFFSET_COORD_Y, pNewMesh->yPos, coord.x-OFFSET_COORD_X) );
    pNewMesh->pMeshNode->yaw(Degree(pBasicEntity->getStepDirection()),Ogre::Node::TS_WORLD);
    pNewMesh->pMeshNode->setScale(scale,scale,scale);

    if (pBasicEntity->getBrain() != NULL)
    {
      pNewMesh->pAnimState = meshEnt->getAnimationState("Idle");
      pNewMesh->pAnimState->setEnabled(true);
      pNewMesh->pAnimState->setLoop(true); 
    }
    else
    {
      pNewMesh->pAnimState = NULL;
    }

    //m_tMesh.insert(m_tMesh.begin() + insertIndex, 1, pNewMesh);
    m_tMesh.push_back(pNewMesh);
  }

  return true;
}

void updateMeshEntityPtr(int meshIndex, CBasicEntity* pBasicEntity)
{
  m_tMesh[meshIndex]->pBasicEntity = pBasicEntity;
  m_tMesh[meshIndex]->entityId = pBasicEntity->getId();
}

int getMeshEntityIndex(entityIdType entityId)
{
  int meshIndex = -1;
  for (int i = 0; i < m_tMesh.size(); ++i)
  {
    if (m_tMesh[i]->entityId == entityId)
    {
      meshIndex = i;
      break;
    }
  }
  return meshIndex;
}

CybiOgre3DFrameListener::CybiOgre3DFrameListener(SceneManager *sceneMgr, RenderWindow* win, Camera* cam)
: ExampleFrameListener(win, cam), mSceneMgr(sceneMgr)
{
  m_secCnt = 0;
  m_TimeSpeed = 1;

  m_bPlayerActionWalkOnGoing = false;
  m_bPlayerActionRunOnGoing= false;
  m_bPlayerActionTurnLeftOnGoing = false;
  m_bPlayerActionTurnRightOnGoing = false;
  m_bPlayerActionAttacktOnGoing = false;
}

bool CybiOgre3DFrameListener::frameStarted(const FrameEvent& evt)
{
  if( ( ExampleFrameListener::frameStarted(evt) == false ) || (!initComplete) )
    return false;

  int i;

  for (i = 0; i < m_tMesh.size(); ++i)
  {
    Real inc = evt.timeSinceLastFrame * m_pClient->get_biotop_speed(); //* mAnimationSpeed[i]; 
    updateMeshEntityPosition(i, inc);
    if (m_tMesh[i]->pAnimState == NULL)
      continue;
    m_tMesh[i]->pAnimState->addTime(inc);
  }

  m_secCnt += evt.timeSinceLastFrame *m_pClient->get_biotop_speed();

  m_pClient->process_new_events();
 
  if(m_pClient->check_if_event_next_second_end_and_clean()) //  (m_secCnt >= 1) 
  {
    forcePlayerAction();
    m_pBiotop->nextSecond();
    m_secCnt = 0;

    BiotopEvent_t bioEvent;
    CBasicEntity* pEntity;
    MeshEntity_t* pMesh;
    for (int i = 0; i<m_pBiotop->getNbOfBiotopEvents(); i++)
    {
      bioEvent = m_pBiotop->getBiotopEvent(i); 
      switch (bioEvent.eventType)
      {
      case BIOTOP_EVENT_ENTITY_MOVED:
      case BIOTOP_EVENT_ENTITY_CHANGED:
      {
        /* TODO: Replace from Client::on_event_biotop_updateentityposition
        pEntity = m_pBiotop->getEntityById(bioEvent.entityId);
        int meshIndex = getMeshEntityIndex(bioEvent.entityId);
        setMeshEntityPreviousPosition(meshIndex);
        updateMeshEntityNewSecond(meshIndex);*/
        break;
      }
      case BIOTOP_EVENT_ENTITY_MODIFIED:
      {
        //pEntity = m_pBiotop->getEntityById(bioEvent.entityId);
        int meshIndex = getMeshEntityIndex(bioEvent.entityId);
        if (meshIndex >= 0) 
        {
          //updateMeshEntityPtr(meshIndex, pEntity);
          updateMeshEntityNewSecond(meshIndex);
        }
        break;
      }
      case BIOTOP_EVENT_ENTITY_ADDED:
        pEntity = m_pBiotop->getEntityById(bioEvent.entityId);
        createMeshEntity(mSceneMgr, pEntity);
        break;
      case BIOTOP_EVENT_ENTITY_REMOVED:
        removeMeshEntity(mSceneMgr, bioEvent.pEntity);
        break;
      default:
        break;
      }
    }
    m_pBiotop->resetBiotopEvents();

    updateInfoParamTime();
    updateInfoPopulation();
    // reset player to idle
    setPlayerActionIdle();
  }

  return true;
}

void CybiOgre3DFrameListener::updateMeshEntityNewSecond(int meshIndex)
{
  CBasicEntity* pBasicEntity = m_tMesh[meshIndex]->pBasicEntity;

  if ( pBasicEntity->getStatus() == STATUS_DEAD )
  {
    if ( pBasicEntity->getClass() >= CLASS_ANIMAL_FIRST )
    {
      Point_t coord = pBasicEntity->getStepCoord();
      m_tMesh[meshIndex]->pAnimState->setEnabled(false);
      m_tMesh[meshIndex]->pAnimState = m_tMesh[meshIndex]->pMeshEnt->getAnimationState("Dead");
      m_tMesh[meshIndex]->pMeshNode->setPosition( Vector3(coord.y-OFFSET_COORD_Y, m_tMesh[meshIndex]->yPos, coord.x-OFFSET_COORD_X) );
      m_tMesh[meshIndex]->pAnimState->setLoop(false);
      m_tMesh[meshIndex]->strCurACtion = "Dead";
      m_tMesh[meshIndex]->pAnimState->setEnabled(true);
      m_tMesh[meshIndex]->translateVect3 = Vector3(0,0,0);
      m_tMesh[meshIndex]->isMoving = false;
      return;
    }
    // Fred: else use dead mesh
  }

  if ( pBasicEntity->isAlive() )
  {
    Point_t coord = pBasicEntity->getStepCoord();
    Point_t prevCoord = m_tMesh[meshIndex]->targetBiotopCoord;

    m_tMesh[meshIndex]->curMoveRate = 0;

    if ( (coord.x != prevCoord.x) || (coord.y != prevCoord.y) || (pBasicEntity->getStepDirection() != m_tMesh[meshIndex]->curDirection) )
    {
      m_tMesh[meshIndex]->translateVect3 = Vector3((coord.y - prevCoord.y), 0, (coord.x - prevCoord.x));
      m_tMesh[meshIndex]->curRotation = (pBasicEntity->getStepDirection() - m_tMesh[meshIndex]->curDirection) % 360;
      if (m_tMesh[meshIndex]->curRotation > 180)
        m_tMesh[meshIndex]->curRotation -= 360;
      if (m_tMesh[meshIndex]->curRotation < -180)
        m_tMesh[meshIndex]->curRotation += 360;

      m_tMesh[meshIndex]->isMoving = true;
      m_tMesh[meshIndex]->targetBiotopCoord = coord;
      m_tMesh[meshIndex]->curDirection = pBasicEntity->getStepDirection();
    }
    else
    {
      m_tMesh[meshIndex]->translateVect3 = Vector3(0,0,0);
      m_tMesh[meshIndex]->isMoving = false;
    }
  }

  if (pBasicEntity->getBrain() != NULL)
  {
    double squareLen = m_tMesh[meshIndex]->translateVect3.squaredLength();
    int labelIndex = pBasicEntity->getBrain()->GetCurrentReactionIndex();
    string labelAction = pBasicEntity->getBrain()->GetReactionByIndex(labelIndex)->GetLabel();
    if ( (labelAction == "Turn_Right") || (labelAction == "Turn_Left") || (labelAction == "StepBack") || (labelAction == "Walk") )
    {
      labelAction = "Walk";
    }
    else if ( (labelAction == "Nothing") || (labelAction == "Idle") || (labelAction == "Stop"))
    {
      labelAction = "Idle";
    }

    if ( (squareLen>400) && (labelAction != "Attack") )
    {
      labelAction = "Run";
    }
    else if ( (squareLen>5) && (labelAction != "Attack") )
    {
      labelAction = "Walk";
    }

    if (m_tMesh[meshIndex]->strCurACtion != labelAction)
    {
      m_tMesh[meshIndex]->pAnimState->setEnabled(false);
      m_tMesh[meshIndex]->pAnimState = m_tMesh[meshIndex]->pMeshEnt->getAnimationState(labelAction);
      m_tMesh[meshIndex]->pAnimState->setEnabled(true);
    }

    if ( (labelAction != "Sleep") && (labelAction != "Turn_head_left") && (labelAction != "Turn_head_right") && (labelAction != "Hide") )
      m_tMesh[meshIndex]->pAnimState->setLoop(true);
    else
      m_tMesh[meshIndex]->pAnimState->setLoop(false);

    m_tMesh[meshIndex]->strCurACtion = labelAction;
  }
}


void CybiOgre3DFrameListener::updateMeshEntityPosition(int meshIndex, Real rate)
{
  if ((m_tMesh[meshIndex]->isMoving) && (m_tMesh[meshIndex]->curMoveRate<1))
  {
    m_tMesh[meshIndex]->pMeshNode->translate( rate*m_tMesh[meshIndex]->translateVect3 );
    m_tMesh[meshIndex]->pMeshNode->yaw(Degree(rate*m_tMesh[meshIndex]->curRotation));
    m_tMesh[meshIndex]->curMoveRate += rate;
  }
}

void CybiOgre3DFrameListener::setMeshEntityPreviousPosition(int meshIndex)
{
  CBasicEntity* pBasicEntity = m_tMesh[meshIndex]->pBasicEntity;
  if ( pBasicEntity->isAlive() )
  {  
    Point_t coord = pBasicEntity->getPrevStepCoord();
    if (coord.x != -1)
      m_tMesh[meshIndex]->pMeshNode->setPosition( Vector3(coord.y-OFFSET_COORD_Y, m_tMesh[meshIndex]->yPos, coord.x-OFFSET_COORD_X) );
    Quaternion q;
    q.FromAngleAxis(Degree(pBasicEntity->getPrevStepDirection()), Vector3::UNIT_Y);
    m_tMesh[meshIndex]->pMeshNode->setOrientation(q); 
  }
}

void CybiOgre3DFrameListener::updateInfoParamTime()
{

}

void CybiOgre3DFrameListener::updateInfoPopulation()
{

}

void CybiOgre3DFrameListener::speedVariation(int variation)
{
  if ((m_TimeSpeed>0) || (variation>0))
  {
    if (m_TimeSpeed<2)
    {
      m_TimeSpeed += ((Real)variation)/2.0;
    }
    else
    {
      m_TimeSpeed += variation;
    }
  }
}

void CybiOgre3DFrameListener::setPlayerActionIdle()
{
  if (m_Player.pBasicEntity == NULL)
    return;

  m_pBiotop->forceEntityAction(m_Player.pBasicEntity->getId(), m_Player.actionId_idle);
  m_bPlayerActionWalkOnGoing = false;
  m_bPlayerActionRunOnGoing = false;
  m_bPlayerActionTurnRightOnGoing = false;
  m_bPlayerActionTurnLeftOnGoing = false;
  m_bPlayerActionAttacktOnGoing = false;
}

void CybiOgre3DFrameListener::setPlayerActionWalk()
{
  m_bPlayerActionWalkOnGoing = true;
}

void CybiOgre3DFrameListener::setPlayerActionRun()
{
  m_bPlayerActionRunOnGoing = true;
}

void CybiOgre3DFrameListener::setPlayerActionTurnLeft()
{
  m_bPlayerActionTurnLeftOnGoing = true;
  m_bPlayerActionTurnRightOnGoing = false;

}

void CybiOgre3DFrameListener::setPlayerActionTurnRight()
{
  m_bPlayerActionTurnRightOnGoing = true;
  m_bPlayerActionTurnLeftOnGoing = false;
}

void CybiOgre3DFrameListener::setPlayerActionTurnAttack()
{
  m_bPlayerActionAttacktOnGoing = true;
}

void CybiOgre3DFrameListener::forcePlayerAction()
{
  if (m_Player.pBasicEntity == NULL)
    return;

  if (m_bPlayerActionRunOnGoing)
    m_pBiotop->forceEntityAction(m_Player.pBasicEntity->getId(), m_Player.actionId_run);
  else if (m_bPlayerActionWalkOnGoing)
    m_pBiotop->forceEntityAction(m_Player.pBasicEntity->getId(), m_Player.actionId_walk);

  if (m_bPlayerActionTurnLeftOnGoing)
    m_pBiotop->forceEntityAction(m_Player.pBasicEntity->getId(), m_Player.actionId_turnLeft);
  else if (m_bPlayerActionTurnRightOnGoing)
    m_pBiotop->forceEntityAction(m_Player.pBasicEntity->getId(), m_Player.actionId_turnRight);

  if (m_bPlayerActionAttacktOnGoing)
    m_pBiotop->forceEntityAction(m_Player.pBasicEntity->getId(), m_Player.actionId_attack);
}

int getStringSectionOgre(
                        string lpAppName,
                        string lpKeyName,
                        string lpDefault,
                        string &lpReturnedString,
                        int nSize,
                        string lpSourceString
                        )
{
  string foundStr;
  int indexSection,indexStart, indexEnd, lenght;
  indexSection = lpSourceString.find(lpAppName,0);
  indexStart = lpSourceString.find(lpKeyName,indexSection);
  indexEnd = lpSourceString.find("\n",indexStart);

  if ((indexStart<0) || (indexEnd<0))
    return -1;

  foundStr = lpSourceString.substr(indexStart,indexEnd-indexStart);
  indexStart = foundStr.find("=",0);
  lenght = foundStr.length() - indexStart - 1;
  
  if (lenght>0)
  {
    lpReturnedString = foundStr.substr(foundStr.length()-lenght, lenght);
  }
  else
  {
    lpReturnedString = lpDefault;
    lenght = lpDefault.length();
  }
  return (lenght);
}

int getStringSectionFromFileOgre(
                        string lpAppName,
                        string lpKeyName,
                        string lpDefault,
                        string &lpReturnedString,
                        int nSize,
                        string fileNameWithPath
                        )
{
  string fileString;
  char* pbuf;
  unsigned long int begin, end;
  long int fLength; 
  ifstream f1;
  bool resu = false;

  f1.open( fileNameWithPath.c_str());
  begin = f1.tellg();
  f1.seekg(0, ios::end);
  end = f1.tellg();
  fLength = (end - begin);
  f1.seekg(0, ios::beg);

  if ( (fLength==0) || (fLength>1000000) )
  {
    lpReturnedString = "";
    return (0);
  }

  pbuf = new char[fLength+1];
  memset(pbuf,0,fLength+1);
  f1.read(pbuf,fLength);
  fileString = pbuf;
  f1.close();

  delete[] pbuf;

  return ( getStringSectionOgre(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize,fileString) );
}

CybiOgre3DApp::CybiOgre3DApp()
{
  m_pBiotop = NULL;
}

CybiOgre3DApp::~CybiOgre3DApp()
{

}

void CybiOgre3DApp::createCamera(void)
{
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");

  // Position it at 500 in Z direction
  mCamera->setPosition(Vector3(0,0,80));
  // Look back along -Z
  mCamera->lookAt(Vector3(0,0,-300));
  mCamera->setNearClipDistance(5);
}


bool CybiOgre3DApp::configure(void)
{
  // Show the configuration dialog and initialise the system
  // You can skip this and use root.restoreConfig() to load configuration
  // settings if you were sure there are valid ones saved in ogre.cfg
  if(mRoot->restoreConfig() || mRoot->showConfigDialog())
  {
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by passing 'true'
    mWindow = mRoot->initialise(true);
    // Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    HWND hwnd;
    mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
    LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
    SetClassLong( hwnd, /*GCL_HICON*/0, iconID );
#endif
    return true;
  }
  else
  {
    return false;
  }
}


// Just override the mandatory create scene method
void CybiOgre3DApp::createScene(void)
{
  mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
  mSceneMgr->setShadowTextureSize(512);
  mSceneMgr->setShadowColour(ColourValue(0.4, 0.4, 0.4));

  // Setup animation default
  Animation::setDefaultInterpolationMode(Animation::IM_LINEAR);
  Animation::setDefaultRotationInterpolationMode(Animation::RIM_LINEAR);

  // Set ambient light
  mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));

  // Create a skydome
  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  // Put in a bit of fog for the hell of it
  mSceneMgr->setFog(FOG_EXP, ColourValue::White, 0.0002);

  OFFSET_COORD_X = 5 * m_pBiotop->getDimension()->x;
  OFFSET_COORD_Y = 5 * m_pBiotop->getDimension()->y;

  CWater* waterGlobalEntity = new CWater();
  CGrass* grassGlobalEntity = new CGrass();
  Point_t coord;
  int x,y;
  waterGlobalEntity->setLabel("water");
  for (x=0; x<m_pBiotop->getDimension()->x; x++)
  {
    for (y=0; y<m_pBiotop->getDimension()->y; y++)
    {
      coord.x = x;
      coord.y = y;
      if (m_pBiotop->getLayerType(coord,1) == LAYER_OVER_WET_GROUND)
      {
        waterGlobalEntity->jumpToGridCoord(coord,0);
        createMeshEntity(mSceneMgr, waterGlobalEntity);
        waterGlobalEntity->setId(waterGlobalEntity->getId()+1);
        //i++;
      }
      else if (m_pBiotop->getLayerType(coord,1) == LAYER_GLOBAL_GRASS)
      {
        grassGlobalEntity->jumpToGridCoord(coord,0);
        createMeshEntity(mSceneMgr, grassGlobalEntity);
        grassGlobalEntity->setId(grassGlobalEntity->getId()+1);
        //i++;
      }
    }
  }

  waterGlobalEntity->setLabel("water_deap");
  for (x=0; x<m_pBiotop->getDimension()->x; x++)
  {
    for (y=0; y<m_pBiotop->getDimension()->y; y++)
    {
      coord.x = x;
      coord.y = y;
      if (m_pBiotop->getLayerType(coord,1) == LAYER_OVER_WATER)
      {
        waterGlobalEntity->jumpToGridCoord(coord,0);
        createMeshEntity(mSceneMgr, waterGlobalEntity);
        waterGlobalEntity->setId(waterGlobalEntity->getId()+1);
        //i++;
      }
    }
  }

  Entity *ent = mSceneMgr->createEntity("grass", "grass.mesh");

  // Give it a little ambience with lights
  Light* l;
  l = mSceneMgr->createLight("YellowLight");
  l->setType(Light::LT_DIRECTIONAL);
  l->setPosition(-1800, 2500, -1800);
#ifndef _DEBUG
  Vector3 dir(-l->getPosition());
  dir.normalise();
  l->setDirection(dir);
#endif
  l->setDiffuseColour(1.0, 0.9, 0.8);

  // Position the camera
  mCamera->setPosition(100,20,0);
  mCamera->lookAt(0,10,0);

  // Report whether hardware skinning is enabled or not
  Technique* t = ent->getSubEntity(0)->getMaterial()->getBestTechnique();
  Pass* p = t->getPass(0);

  Plane plane;
  plane.normal = Vector3::UNIT_Y;
  plane.d = 100;
  MeshManager::getSingleton().createPlane("Myplane",
    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    10000,10000,60,60,true,1,60,60,Vector3::UNIT_Z);
  Entity* pPlaneEnt = mSceneMgr->createEntity( "plane", "Myplane" );
  pPlaneEnt->setMaterialName("Cybios/Ground");
  pPlaneEnt->setCastShadows(false);
  mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,99,0))->attachObject(pPlaneEnt);

  initComplete = true;
}

// Create new frame listener
void CybiOgre3DApp::createFrameListener(void)
{
  mFrameListener= new CybiOgre3DFrameListener(mSceneMgr, mWindow, mCamera);
  mRoot->addFrameListener(mFrameListener);
  mFrameListener->speedVariation(0);
}

int CybiOgre3DApp::getMeshEntityIndex(CBasicEntity* pBasicEntity)
{
  int meshIndex = -1;
  for (int i = 0; i < m_tMesh.size(); ++i)
  {
    if (m_tMesh[i]->pBasicEntity == pBasicEntity)
    {
      meshIndex = i;
      break;
    }
  }
  return meshIndex;
}


void CybiOgre3DApp::setMeshEntityPreviousPosition(CBasicEntity* pBasicEntity)
{
  int meshIndex = getMeshEntityIndex(pBasicEntity);
  if (meshIndex>=0)
  {
   ((CybiOgre3DFrameListener*)mFrameListener)->setMeshEntityPreviousPosition(meshIndex);
  }
  return ;
}

void CybiOgre3DApp::updateMeshEntityNewSecond(CBasicEntity* pBasicEntity)
{
  int meshIndex = getMeshEntityIndex(pBasicEntity);
  if (meshIndex>=0)
  {
   ((CybiOgre3DFrameListener*)mFrameListener)->updateMeshEntityNewSecond(meshIndex);
  }
  return ;
}


bool CybiOgre3DApp::SetEntityPlayer(CBasicEntity* pEntity)
{
  if (pEntity!=NULL)
  {
    pEntity->setUserControlled(true);
    m_Player.pBasicEntity = pEntity;
    CBrain* pBrain = pEntity->getBrain();
    if (pBrain != NULL)
    {
      m_Player.actionId_idle      = pBrain->GetReactionIndexByLabel("Idle");
      m_Player.actionId_walk      = pBrain->GetReactionIndexByLabel("Walk");
      m_Player.actionId_run       = pBrain->GetReactionIndexByLabel("Run");
      m_Player.actionId_turnLeft  = pBrain->GetReactionIndexByLabel("Turn_Left");
      m_Player.actionId_turnRight = pBrain->GetReactionIndexByLabel("Turn_Right");
      m_Player.actionId_attack    = pBrain->GetReactionIndexByLabel("Attack");
    }
  }
  else
  {
    m_Player.pBasicEntity       = NULL;
    m_Player.actionId_idle      = -1;
    m_Player.actionId_walk      = -1;
    m_Player.actionId_run       = -1;
    m_Player.actionId_turnLeft  = -1;
    m_Player.actionId_turnRight = -1;
    m_Player.actionId_attack    = -1;
  }
  return true;
}

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[])
#endif
  {
    // Create application object
    CybiOgre3DApp app;

    // Get Server info in ini file
    string ServerAddrStr;
    string ServerPortStr;
    char resuBuffer[512];
    string fileIni = "Cybiosphere.ini";
    int resu = getStringSectionFromFile("CYBIOSPHERE", "ServerAddr", "localhost", resuBuffer, 512, fileIni);
    ServerAddrStr = resuBuffer;
    resu = getStringSectionFromFile("CYBIOSPHERE", "ServerPort", "4556", resuBuffer, 512, fileIni);
    ServerPortStr = resuBuffer;

    //std::string serverAddr = "localhost"; //"192.168.1.67"
    //std::string portId     = "4556";

    m_pClient = new Client(ServerAddrStr, ServerPortStr, &app);
    // Connect to server and wait for biotop init from server
    m_pClient->connect_to_server();
    while (!(m_pClient->is_biotop_config_complete()))
    {
      System::sleep(10);
      m_pClient->process_new_events();
    }

    m_pBiotop = m_pClient->get_pBiotop();
    
    try {
      app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " <<
        e.getFullDescription().c_str() << std::endl;
#endif
    }

    m_pClient->disconnect_from_server();
    delete m_pClient;
    return 0;
  }

#ifdef __cplusplus
}
#endif