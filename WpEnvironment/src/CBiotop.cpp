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

//===========================================================================
// FILE: CBiotop.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a biotop environment
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/09/2002 : Creation
//
//===========================================================================

#include "CBiotop.h"

// Mineral entities
#include "CMineral.h"
#include "CWater.h"

// Vegetable entities
#include "CVegetable.h"
#include "CVegetSpermatophyta.h"

// Animal entities
#include "CAnimal.h"
#include "CAnimMammal.h"

Point_t vectorDirection[8] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

//===========================================================================
// Constructors / Destructors
//===========================================================================
CBiotop::CBiotop(int dimX,int dimY, int dimZ) 
{
  // Initialisation
  m_Label = "Default biotop";
  m_Dimension.x  = dimX;
  m_Dimension.y  = dimY;
  m_nbLayer      = dimZ;
  m_IndexLastAnimal = 0;

  m_BioTime.seconds  = 0;
  m_BioTime.hours    = 12;
  m_BioTime.days     = 0;
  m_BioTime.years    = 0;

  m_WindDirection = 1;
  m_WindStrenght  = 1; // 0,1 or 2
  
  m_DefaultFilePath = "";

  m_tParam.resize(0);
  m_pFertilityRate = new CCyclicParam(0,50,864,"Avarage fertility",PARAM_ENVIRONMENT);
  m_tParam.push_back(m_pFertilityRate);
  m_pSunlightRate = new CCyclicParam(0,100,24,"Sunlight",PARAM_ENVIRONMENT);
  m_tParam.push_back(m_pSunlightRate);
  m_pRadioactivity = new CGenericParam(0,1,1,100,"Radioactivity",PARAM_ENVIRONMENT);
  m_tParam.push_back(m_pRadioactivity);
  m_pTemperature = new CCyclicParam(20,26,864,"Avarage Temperature",PARAM_ENVIRONMENT); // TBD use global min/max temperature
  m_tParam.push_back(m_pTemperature);

  // Build 3D dynamic table
  buildGrid(dimX, dimY, dimZ);

  m_pWaterGlobalEntity = new CWater();
  // Set object caract
  m_pWaterGlobalEntity->setEntityFromGenome(0);
  m_pWaterGlobalEntity->setId(ENTITY_ID_WATER);

  m_pGrassGlobalEntity = new CGrass();
  // Set object caract
  m_pGrassGlobalEntity->setEntityFromGenome(0);
  m_pGrassGlobalEntity->setId(ENTITY_ID_GRASS);

  m_IdLastEntity = ENTITY_ID_FIRST_USER_ENTITY; // Grass is last id at startup

  m_tMeasures.resize(0);

  m_tFoundIds = new FoundEntity_t[MAX_FOUND_ENTITIES];

  resetCpuMarker();

  CYBIOCORE_LOG_INIT;
}
   
CBiotop::~CBiotop() 
{
  deleteAllMeasures();
  deleteAllParameters();
  deleteAllEntities();
  deleteGrid();  // Delete 3D dynamic table

  delete m_pWaterGlobalEntity;
  delete m_pGrassGlobalEntity;
  delete m_tFoundIds;

  CYBIOCORE_LOG_CLOSE;
}
//===========================================================================
// Entities management
//===========================================================================

entityIdType CBiotop::addEntity(CBasicEntity* pEntity, Point_t coord, int newLayer) 
{
  int layer;
  if (newLayer>-2)
    layer = newLayer;
  else
    layer = pEntity->getLayer();

  if (getNbOfEntities()>MAXIMUM_NB_ENTITIES)
     return (-1); 
   
  if ( !isCoordValidAndFree(coord,layer) )
     return (-1); 

  // Set new id
  m_IdLastEntity++;
  pEntity->setId(m_IdLastEntity);
  // Go to position
  pEntity->jumpToGridCoord(coord, layer);
  // Attach it to biotop
  pEntity->attachToBiotop(this);

  if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
  {
    // Add it to list at the begining
    m_tEntity.insert(m_tEntity.begin(), pEntity);
    m_IndexLastAnimal++;
  }
  else
  {
    // Add it to list at the end
    m_tEntity.push_back(pEntity);
  }

  if (pEntity->getAge()<1)
  {
    // Enter 1st day and change LifeStage
    pEntity->nextDay();
  }

  // Set home position if needed
  if (pEntity->getBrain() != NULL)
  {
    pEntity->getBrain()->SetHomePurposePositionInGeoMap();
  }

  addBiotopEvent(BIOTOP_EVENT_ENTITY_ADDED, pEntity);

  return (m_IdLastEntity);
}

bool CBiotop::addRemoteCtrlEntity(entityIdType idEntity, CBasicEntity* pEntity, Point_t stepCoord, int newLayer)
{
  int layer;
  if (newLayer>-2)
    layer = newLayer;
  else
    layer = pEntity->getLayer();

  if (getNbOfEntities()>MAXIMUM_NB_ENTITIES)
    return false; 
  
  if ( !isCoordValidAndFree(CBasicEntity::getGridCoordFromStepCoord(stepCoord), layer) )
    return false; 

  //check ID:
  if (getEntityById(idEntity) != NULL)
    return false; 

  pEntity->setId(idEntity);
  // Go to position
  pEntity->jumpToStepCoord(stepCoord, layer);
  // Attach it to biotop
  pEntity->attachToBiotop(this);

  pEntity->setRemoteControlled(true);

  if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
  {
    // Add it to list at the begining
    m_tEntity.insert(m_tEntity.begin(), pEntity);
    m_IndexLastAnimal++;
  }
  else
  {
    // Add it to list at the end
    m_tEntity.push_back(pEntity);
  }

  if (pEntity->getAge()<1)
  {
    // Enter 1st day and change LifeStage
    pEntity->nextDay();
  }

  // Set home position if needed
  if (pEntity->getBrain() != NULL)
  {
    pEntity->getBrain()->SetHomePurposePositionInGeoMap();
  }

  // Update m_IdLastEntity
  if (m_IdLastEntity <= idEntity)
    m_IdLastEntity = idEntity + 1;

  addBiotopEvent(BIOTOP_EVENT_ENTITY_ADDED, pEntity);

  return true;
}


CBasicEntity* CBiotop::createEntity(string name, CGenome* pGenome)
{
  if (pGenome==NULL)
     return (NULL); 

  CBasicEntity* pNewEntity = NULL;

  // Set defaut coord
  Point_t coord = {-1,-1};
  int layer = -1;

  // Create the BasicEntity derived object
  switch(pGenome->getClass())
  {
  // Vegetable classs
  case CLASS_PHYCOPHYTA:
  case CLASS_MYCOPHYTA:
  case CLASS_BRYOPHYTA:
  case CLASS_PTERIDOPHYTA:
    pNewEntity = new CVegetable(name,coord,layer,pGenome);
    break;

  case CLASS_SPERMATOPHYTA:
    pNewEntity = new CVegetSpermatophyta(name,coord,layer,pGenome);
    break;

   // Animal classs
  case CLASS_MAMMAL:
    pNewEntity = new CAnimMammal(name,coord,layer,pGenome);

    break;

  case CLASS_REPTILE:
  case CLASS_ARTHROPOD:
  case CLASS_AVE:
    pNewEntity = new CAnimal(name,coord,layer,pGenome);
    break; 

  case CLASS_WATER:
    pNewEntity = new CWater(name,coord,pGenome);
    break; 

  default:
    pNewEntity = new CMineral(name,coord,layer,pGenome);

  }

  if (pNewEntity==NULL)
     return (NULL); 

  // Initialise the new entity
  pNewEntity->setEntityFromGenome();

  return (pNewEntity);
}


CBasicEntity* CBiotop::createEntity(TiXmlDocument *pXmlDoc, string pathNameForBabies)
{
  string name;
  CGenome* pTempGenome = new CGenome(CLASS_NONE,"");

  CBasicEntity::getGenomeFromXmlFile(pXmlDoc, *pTempGenome);
  CBasicEntity::getEntityNameFromXmlFile(pXmlDoc, name);

  CBasicEntity* pNewEntity = createEntity(name, pTempGenome);

  if(pNewEntity != NULL)
  {
    pNewEntity->loadDataFromXmlFile(pXmlDoc, pathNameForBabies);
    pNewEntity->loadBrainFromXmlFile(pXmlDoc);
  }
  else
  {
    delete pTempGenome;
  }

  return (pNewEntity);
}


CBasicEntity* CBiotop::createCloneEntity(CBasicEntity* pModelEntity)
{
 CBasicEntity* pNewEntity = NULL;

  int layer;
  string name;

  layer = pModelEntity->getLayer();
  name  = pModelEntity->getLabel();

  if (pModelEntity->getGenome() == NULL)
  {
    pNewEntity = new CBasicEntity(); // FRED Should use constructor with copy caracters
    if (pNewEntity==NULL)
      return (NULL);
    pNewEntity->setLabel(name);
  }
  else
  {
    // Create the BasicEntity derived object
    switch(pModelEntity->getGenome()->getClass())
    {
    // Vegetable classs
    case CLASS_PHYCOPHYTA:
    case CLASS_MYCOPHYTA:
    case CLASS_BRYOPHYTA:
    case CLASS_PTERIDOPHYTA:
      pNewEntity = new CVegetable(name,*(CVegetable*)pModelEntity);
      break;

    case CLASS_SPERMATOPHYTA:
      pNewEntity = new CVegetSpermatophyta(name,*(CVegetSpermatophyta*)pModelEntity);
      break;

     // Animal classs
    case CLASS_MAMMAL:
      pNewEntity = new CAnimMammal(name,*(CAnimMammal*)pModelEntity);
      break;

    case CLASS_REPTILE:
    case CLASS_ARTHROPOD:
    case CLASS_AVE:
      pNewEntity = new CAnimal(name,*(CAnimal*)pModelEntity);
      break; 

    case CLASS_WATER:
      pNewEntity = new CWater(name,*(CWater*)pModelEntity);
      break; 

    default:
      pNewEntity = new CMineral(name,*(CMineral*)pModelEntity);

    }

    if (pNewEntity==NULL)
      return (NULL);

    // Set parameters
    pNewEntity->setEntityFromGenome(0);

    // quick aging
    pNewEntity->quickAgeing(pModelEntity->getAge());
  }

  // Copie parameters
  for (int i=0; i<pNewEntity->getNumParameter(); i++)
    pNewEntity->getParameter(i)->forceVal(pModelEntity->getParameter(i)->getVal());

  // Copie Status
  pNewEntity->setStatus(pModelEntity->getStatus());

  return pNewEntity;
}


entityIdType CBiotop::createAndAddEntity(string name, Point_t coord, int layer, CGenome* pGenome)
{
  // Check coords
  if ( !isCoordValidAndFree(coord,layer) )
     return (-1); 

  // Create entity
  CBasicEntity* pNewEntity = createEntity(name,pGenome);
  if (pNewEntity==NULL)
    return (-1);

  if (pGenome->getClass() > CLASS_MINERAL_LAST)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - New entity : specie %s name %s\n", pGenome->getSpecieName().c_str(), name.c_str());
  }

  // Put it in the biotop (with check coord);
  entityIdType resuId = addEntity(pNewEntity, coord, layer);
  if (resuId == -1)
    delete pNewEntity;

  return (resuId);
}


entityIdType CBiotop::createAndAddEntity(string fileName, string pathName, Point_t coord)
{
  entityIdType newEntityId;

  string fileNameWithPath = pathName + fileName;
  TiXmlDocument *pXmlDoc = new TiXmlDocument(fileNameWithPath);
  if (!pXmlDoc->LoadFile())
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Error loading entity file: %s\n", fileNameWithPath.c_str());
    CYBIOCORE_LOG_FLUSH;
    delete pXmlDoc;
    return -1;
  }
  newEntityId = createAndAddEntity(pXmlDoc, pathName, coord);
  CYBIOCORE_LOG_FLUSH;
  delete pXmlDoc;
  return newEntityId;
}


entityIdType CBiotop::createAndAddEntity(TiXmlDocument *pXmlDoc, string pathName, Point_t coord)
{
  int startLayer;
  string name;

  CBasicEntity::getDefaultLayerFromXmlFile(pXmlDoc, startLayer);

  // Check coords
  if ( !isCoordValidAndFree(coord,startLayer) )
     return (-1); 

  CGenome* pTempGenome = new CGenome(CLASS_NONE,"");
  CBasicEntity::getGenomeFromXmlFile(pXmlDoc, *pTempGenome);
  CBasicEntity::getEntityNameFromXmlFile(pXmlDoc, name);

  entityIdType newEntityId = createAndAddEntity(name, coord, startLayer, pTempGenome);

  CBasicEntity* pEntity = getEntityById(newEntityId);
  if (pEntity!=NULL)
  {
    pEntity->loadDataFromXmlFile(pXmlDoc, pathName);
    pEntity->loadBrainFromXmlFile(pXmlDoc);

    // Set home position if needed
    if (pEntity->getBrain() != NULL)
    {
      pEntity->getBrain()->SetHomePurposePositionInGeoMap();
    }
  }
  else
  {
    delete pTempGenome;
    return -1;
  }

  return newEntityId;
}


entityIdType CBiotop::createAndAddCloneEntity(entityIdType idModelEntity, Point_t cloneCoord, int cloneLayer, string cloneName)
{
  CBasicEntity* pModelEntity = getEntityById(idModelEntity);
  CBasicEntity* pNewEntity = NULL;

  int layer = cloneLayer;
  string name = cloneName;

  if (layer == -2)
    layer = pModelEntity->getLayer();

  if (name == "")
    name = pModelEntity->getLabel();

  // Check coords
  if ( !isCoordValidAndFree(cloneCoord,layer) )
     return (-1); 

  pNewEntity = createCloneEntity(pModelEntity);

  if (pNewEntity==NULL)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Error copy clone Entity : %s\n", pModelEntity->getLabel().c_str());
    return (-1);
  }

  // Put it in the biotop (with check coord);
  entityIdType resuId = addEntity(pNewEntity, cloneCoord, layer);
  if (resuId == -1)
    delete pNewEntity;

  return (resuId);
}


bool CBiotop::resetEntityGenome(entityIdType idEntity, CGenome* pNewEntityGenome)
{
  CBasicEntity* pOldEntity = getEntityById(idEntity);

  if (pOldEntity == NULL)
    return (false);

  // Keep old entity data
  string oldLabel    = pOldEntity->getLabel();
  Point_t oldCoord   = pOldEntity->getGridCoord();
  int oldLayer       = pOldEntity->getLayer();
  entityIdType oldId = pOldEntity->getId();

  // Destroy Old entity
  int prevIndex = deleteEntity(oldId);

  // Create new entity
  CBasicEntity* pNewEntity = createEntity(oldLabel,pNewEntityGenome);
  if (pNewEntity==NULL)
    return (false);

  // Set coord
  pNewEntity->jumpToGridCoord(oldCoord, oldLayer); 

  // Set old id
  pNewEntity->setId(oldId);
  // Attach it to biotop
  pNewEntity->attachToBiotop(this);
  // Add it to list
  m_tEntity.insert(m_tEntity.begin() + prevIndex, 1, pNewEntity);
  if (pNewEntity->getClass() >= CLASS_ANIMAL_FIRST)
  {
    m_IndexLastAnimal++;
  }
  // Enter 1st day and change LifeStage
  pNewEntity->nextDay();

  return (true);
}


bool CBiotop::replaceEntityByAnother(entityIdType idEntity, CBasicEntity* pNewEntity)
{
  CBasicEntity* pOldEntity = getEntityById(idEntity);

  if ((pOldEntity==NULL) || (pNewEntity==NULL))
    return (false);

  // Keep old entity data
  Point_t oldCoord   = pOldEntity->getGridCoord();
  int oldLayer       = pOldEntity->getLayer();
  entityIdType oldId = pOldEntity->getId();

  // Destroy Old entity
  int prevIndex = deleteEntity(oldId);

  // Set coord
  pNewEntity->jumpToGridCoord(oldCoord, oldLayer); 

  // Set old id
  pNewEntity->setId(oldId);
  // Attach it to biotop
  pNewEntity->attachToBiotop(this);
  // Add it to list
  m_tEntity.insert(m_tEntity.begin() + prevIndex, 1, pNewEntity);
  if (pNewEntity->getClass() >= CLASS_ANIMAL_FIRST)
  {
    m_IndexLastAnimal++;
  }

  addBiotopEvent(BIOTOP_EVENT_ENTITY_MODIFIED, pNewEntity);

  return true;
}


int CBiotop::deleteEntity(entityIdType idEntity) 
{ 
  CBasicEntity* pEntity = NULL;
  for (int i=0; i<getNbOfEntities(); i++)     
  { 
    pEntity = m_tEntity[i];
    if (pEntity->getId() == idEntity)
    {
      if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
      {
        m_IndexLastAnimal--;
      }
      CYBIOCORE_LOG_TIME(m_BioTime);
      CYBIOCORE_LOG("BIOTOP - Entity removed : specie %s name %s\n", pEntity->getSpecieName().c_str(), pEntity->getLabel().c_str());
      delete (m_tEntity[i]); 
      m_tEntity.erase(m_tEntity.begin()+i);
      return (i);
    }
  }
  // If not found
  return (-1);
}
 

void CBiotop::deleteAllEntities() 
{
  // loop from top to bottom 
  for (int i=getNbOfEntities()-1; i>=0; i--)  
  {
    if (m_tEntity[i] != NULL)
      delete (m_tEntity[i]); 
  }
  m_tEntity.clear();
}          


void CBiotop::deleteAllMeasures() 
{
  // loop from top to bottom 
  for (int i=m_tMeasures.size()-1; i>=0; i--) 
  {
    if (m_tMeasures[i] != NULL)
      delete (m_tMeasures[i]); 
  }
  m_tMeasures.clear();
}   


void CBiotop::deleteAllParameters() 
{
  // loop from top to bottom 
  for (int i=m_tParam.size()-1; i>=0; i--) 
  {
    if (m_tParam[i] != NULL)
      delete (m_tParam[i]); 
  }
  m_tParam.clear();

  m_pFertilityRate = NULL;
  m_pSunlightRate = NULL;
  m_pRadioactivity = NULL;
  m_pTemperature = NULL;
}  


void CBiotop::displayEntities(void)
{
  for (CBasicEntity* pEntity : m_tEntity)
  {
    printf("x=%2d, y=%2d ", pEntity->getGridCoord().x, pEntity->getGridCoord().y);
    printf("%-12s", pEntity->getLabel().c_str());
    printf("ID=%3d Direction=%3d \n", pEntity->getId(), pEntity->getDirection());
  }  
}


void CBiotop::setDefaultEntitiesForTest(void)
{

  // Seed the random generator
  // srand( (unsigned)time(NULL) );


  CGenome* pGenome1 = new CGenome(CLASS_NONE,"");
  CGenome* pGenome2 = new CGenome(CLASS_NONE,"");
  //CGenome* pGenome3 = new CGenome(CLASS_NONE,"");
  //CGenome* pGenome4 = new CGenome(CLASS_NONE,"");
  //CGenome* pGenome5 = new CGenome(CLASS_NONE,"");
  CGenome* pGenome6 = new CGenome(CLASS_NONE,"");
  //CGenome* pGenome7 = new CGenome(CLASS_NONE,"");
  CGenome* pGenome8 = new CGenome(CLASS_NONE,"");

  pGenome6->loadFromXmlFile("..\\DataScriptMammal\\rock.xml");
  pGenome8->loadFromXmlFile("..\\DataScriptMammal\\wooden_fence.xml");

  pGenome1->loadFromXmlFile("..\\DataScriptMammal\\grassDry.xml");
  pGenome2->loadFromXmlFile("..\\DataScriptMammal\\grassDry2.xml");

  Point_t coord = {10,10};
  string name;
  entityIdType pierreId, grassId, fenceId;

  int i;
  for (i=0; i<0; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome6);
    coord.x = 2 + getRandInt(60);
    coord.y = 2 + getRandInt(40);
    pierreId = createAndAddEntity("rock",coord,2,pGenome);
  }
  for ( i=0; i<m_Dimension.y-1; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome8);
    coord.x = 0;
    coord.y = i;
    fenceId = createAndAddEntity("wooden_fence",coord,2,pGenome);
    if (fenceId!=-1)
    {
      getEntityById(fenceId)->setDirection(2);
    }
  }
  for ( i=0; i<m_Dimension.y-1; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome8);
    coord.x = m_Dimension.x-3;
    coord.y = i;
    fenceId = createAndAddEntity("wooden_fence",coord,2,pGenome);
    if (fenceId!=-1)
    {
      getEntityById(fenceId)->setDirection(2);
    }
  }
  for ( i=0; i<m_Dimension.x-2; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome8);
    coord.x = i;
    coord.y = m_Dimension.y-2;
    fenceId = createAndAddEntity("wooden_fence",coord,2,pGenome);
  }
  for ( i=0; i<m_Dimension.x-2; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome8);
    coord.x = i;
    coord.y = 0;
    fenceId = createAndAddEntity("wooden_fence",coord,2,pGenome);
  }
  for (i=1; i<m_Dimension.y/5; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome6);
    coord.x = getRandInt(m_Dimension.x) + 1;
    coord.y = getRandInt(m_Dimension.y) + 1;
    pierreId = createAndAddEntity("rock",coord,2,pGenome);
  }

  for (i=0; i<2*m_Dimension.y; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome1);
    coord.x = getRandInt(m_Dimension.x) + 2;
    coord.y = getRandInt(m_Dimension.y) + 2;
    grassId = createAndAddEntity("grassDry",coord,1,pGenome);
  }
  for (i=0; i<m_Dimension.x; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome2);
	  //name.Format("grass%d",20+i);

    coord.x = getRandInt(m_Dimension.x) + 1;
  	coord.y = getRandInt(m_Dimension.y) + 1;
    grassId = createAndAddEntity("grassDry2",coord,1,pGenome);
  }

  addMeasurePopulation(14400,7,MEASURE_POPULATION_VEGETAL,1000);

  addMeasureBiotopParam(BIO_PARAM_SUNLIGHT,3600,9);

  delete pGenome1;
  delete pGenome2;
  delete pGenome6;
  delete pGenome8;
}


int CBiotop::getNbOfEntities() 
{
  return (m_tEntity.size());
}  

int CBiotop::getNbOfAnimals() 
{
  return (m_IndexLastAnimal);
}  

int CBiotop::getNbOfVegetals() 
{
  int tempCount = 0;
  ClassType_e curClass;

  for (CBasicEntity* pEntity : m_tEntity)
  {
    curClass = pEntity->getClass();
    if ( (curClass >= CLASS_VEGETAL_FIRST) && (curClass <= CLASS_VEGETAL_LAST) && (pEntity->getId()>0) )
      tempCount++;
  } 

  return (tempCount);
}  

int CBiotop::getNbOfMinerals() 
{
  int tempCount = 0;
  ClassType_e curClass;

  for (CBasicEntity* pEntity : m_tEntity)
  {
    curClass = pEntity->getClass();
    if ( (curClass >= CLASS_MINERAL_FIRST) && (curClass <= CLASS_MINERAL_LAST) && (pEntity->getId()>0) )
      tempCount++;
  } 

  return (tempCount);
}  

int CBiotop::getNbOfSpecieEntities(string& SpecieName) 
{
  int tempCount = 0;
  CGenome* pCurGenome;

  for (CBasicEntity* pEntity : m_tEntity)
  {
    pCurGenome = pEntity->getGenome();
    if ( (pCurGenome!=NULL) && (pCurGenome->getSpecieName()== SpecieName) && (pEntity->isAlive()) )
      tempCount++;
  } 

  return (tempCount);
}  


//===========================================================================
// Entities access
//===========================================================================

CBasicEntity* CBiotop::getEntityById(entityIdType idEntity) 
{
  // Check if it is water
  if (idEntity==ENTITY_ID_WATER)
  {
    return (m_pWaterGlobalEntity);
  }

  if (idEntity==ENTITY_ID_GRASS)
  {
    return (m_pGrassGlobalEntity);
  }
  
  for (CBasicEntity* pEntity : m_tEntity)
  {   
    if ((pEntity)->getId() == idEntity)
    {
        return (pEntity);
    }
  }
  // If not found
  return (NULL);
}   

CBasicEntity* CBiotop::getEntityByIndex(int index) 
{
  CBasicEntity* pFoundEntity = NULL;

  if (index<getNbOfEntities())
  {
    pFoundEntity = m_tEntity[index];
  }
  
  return (pFoundEntity);
}

CBasicEntity* CBiotop::getEntityByName(string& entityName)
{
  for (CBasicEntity* pEntity : m_tEntity)
  {   
    if (pEntity->getLabel() == entityName)
    {
        return (pEntity);
    }
  }
  return (NULL);
}

int CBiotop::getEntityTableIndex(CBasicEntity* pEntity)
{
  for (int i=0; i<getNbOfEntities(); i++)   
  {   
    if (m_tEntity[i] == pEntity)
    {
        return (i);
    }
  }
  return (-1);
}
   
CBasicEntity* CBiotop::findEntity(Point_t searchCoord, int layer) 
{ 
  CBasicEntity* pFoundEntity = NULL;

  if ( isCoordValid(searchCoord,layer) )
  {
    pFoundEntity = m_tBioGrid[searchCoord.x][searchCoord.y][layer].pEntity;
  }
  
  return (pFoundEntity);
}

// Rq: pFoundIds must be deleted out of the function!
//     startCoord entity include

/*int CBiotop::findEntities(FoundEntity_t*& pFoundIds,Point_t startCoord, int distance, bool includeWater)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;
  
  for (int i = startCoord.x - distance; i <= startCoord.x + distance;i++)
  {
    for (int j = startCoord.y - distance; j <= startCoord.y + distance;j++)
    {
      curCoord.x = i;
      curCoord.y = j;
      for (int layer = 0; layer < m_nbLayer; layer++)
      {
        if (nbFoundIds >= MAX_FOUND_ENTITIES)
          break;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = max(abs(i-startCoord.x),abs(j-startCoord.y));
          nbFoundIds++;
        }
      }
    }
  }
  pFoundIds = m_tFoundIds;
  return (nbFoundIds);
}*/

int CBiotop::findEntities(FoundEntity_t*& pFoundIds,Point_t startCoord, int distance, bool includeWater)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;
  int k, i, segment, layer;

  // Entities in center pos
  for (layer = 0; layer < m_nbLayer; layer++)
  {
    pCurEntity = findEntity(startCoord,layer);
    if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
    {
      m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
      m_tFoundIds[nbFoundIds].distance = 0;
      nbFoundIds++;
    }
  }

  // Do circles around center
  for (k=0; k<distance; k++)
  {
    for (i=-k; i<k; i++)
    {
      for (segment=0;segment<4;segment++)
      {
        switch(segment)
        {
        case 0:
          curCoord.x = startCoord.x+i;
          curCoord.y = startCoord.y-k;
          break;
        case 1:
          curCoord.x = startCoord.x+i+1;
          curCoord.y = startCoord.y+k;
          break;
        case 2:
          curCoord.x = startCoord.x-k;
          curCoord.y = startCoord.y+i+1;
          break;
        case 3:
          curCoord.x = startCoord.x+k;
          curCoord.y = startCoord.y+i;
          break;
        }

        for (layer = 0; layer < m_nbLayer; layer++)
        {
          if (nbFoundIds >= MAX_FOUND_ENTITIES)
            break;
          pCurEntity = findEntity(curCoord,layer);
          if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
          {
            m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
            m_tFoundIds[nbFoundIds].distance = k;
            nbFoundIds++;
          }
        }
      }
    }
  }

  pFoundIds = m_tFoundIds;
  return (nbFoundIds);
}


//  startCoord entity not include
int CBiotop::findEntities(FoundEntity_t*& pFoundIds,
                          Point_t startCoord, UCHAR sectorBmp, 
                          int distance, int layer,
                          bool includeWater)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;

  if ( (sectorBmp & SECTOR_EE_BIT_MASK) !=0 )
  {
    int starty = startCoord.y;
    int endy = startCoord.y;
    int offset = 0;
    for (int i = startCoord.x + 1; i <= startCoord.x + distance;i++)
    {
      for (int j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = i-startCoord.x;
          nbFoundIds++;
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      starty -= offset;
      endy += offset;
    }
  }
  if ( (sectorBmp & SECTOR_NE_BIT_MASK) !=0 )
  {
    int starty = startCoord.y + 1;
    int endy = startCoord.y + 1;
    int offset = 0;
    for (int i = startCoord.x + 1; i <= startCoord.x + distance;i++)
    {
      for (int j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = max(i-startCoord.x,j-startCoord.y);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy>startCoord.y + distance)
        endy = startCoord.y + distance;
    }
  }
  if ( (sectorBmp & SECTOR_NN_BIT_MASK) !=0 )
  {
    int startx = startCoord.x;
    int endx = startCoord.x;
    int offset = 0;
    for (int j = startCoord.y + 1; j <= startCoord.y + distance;j++)
    {
      for (int i = startx; i <= endx; i++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = j-startCoord.y;
          nbFoundIds++;
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      startx -= offset;
      endx += offset;
    }
  }
  if ( (sectorBmp & SECTOR_NW_BIT_MASK) !=0 )
  {
    int starty = startCoord.y + 1;
    int endy = startCoord.y + 1;
    int offset = 0;
    for (int i = startCoord.x - 1; i >= startCoord.x - distance;i--)
    {
      for (int j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = max(startCoord.x-i,j-startCoord.y);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy>startCoord.y + distance)
        endy = startCoord.y + distance;
    }
  }
  if ( (sectorBmp & SECTOR_WW_BIT_MASK) !=0 )
  {
    int starty = startCoord.y;
    int endy = startCoord.y;
    int offset = 0;
    for (int i = startCoord.x - 1; i >= startCoord.x - distance;i--)
    {
      for (int j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = startCoord.x-i;
          nbFoundIds++;
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      starty -= offset;
      endy += offset;
    }
  }
  if ( (sectorBmp & SECTOR_SW_BIT_MASK) !=0 )
  {
    int starty = startCoord.y - 1;
    int endy = startCoord.y - 1;
    int offset = 0;
    for (int i = startCoord.x - 1; i >= startCoord.x - distance;i--)
    {
      for (int j = starty; j >= endy; j--)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = max(startCoord.x-i,startCoord.y-j);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy<startCoord.y - distance)
        endy = startCoord.y - distance;
    }
  }
  if ( (sectorBmp & SECTOR_SS_BIT_MASK) !=0 )
  {
    int startx = startCoord.x;
    int endx = startCoord.x;
    int offset = 0;
    for (int j = startCoord.y - 1; j >= startCoord.y - distance;j--)
    {
      for (int i = startx; i <= endx; i++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = startCoord.y-j;
          nbFoundIds++;
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      startx -= offset;
      endx += offset;
    }
  }
  if ( (sectorBmp & SECTOR_SE_BIT_MASK) !=0 )
  {
    int starty = startCoord.y - 1;
    int endy = startCoord.y - 1;
    int offset = 0;
    for (int i = startCoord.x + 1; i <= startCoord.x + distance;i++)
    {
      for (int j = starty; j >= endy; j--)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId()!=0)) )
        {
          m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
          m_tFoundIds[nbFoundIds].distance = max(i-startCoord.x,startCoord.y-j);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy<startCoord.y - distance)
        endy = startCoord.y - distance;
    }
  }

  pFoundIds = m_tFoundIds;
  return (nbFoundIds);
}

//  find all entities betwen 2 range, on any layer
int CBiotop::findFarEntities(FoundEntity_t*& pFoundIds,
                             Point_t startCoord, UCHAR sectorBmp, 
                             int rangeMin, int rangeMax)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;
  int entityDist;

  if ( (sectorBmp & SECTOR_EE_BIT_MASK) !=0 )
  {
    int starty = startCoord.y;
    int endy = startCoord.y;
    int offset = 0;
    for (int i = startCoord.x + 1; i <= startCoord.x + rangeMax;i++)
    {
      entityDist = i-startCoord.x;
      if (entityDist>=rangeMin)
      {
        for (int j = starty; j <= endy; j++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            {
              if ( pCurEntity!=NULL ) 
              {
                m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
                m_tFoundIds[nbFoundIds].distance = entityDist;
                nbFoundIds++;
              }
            }
          }
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      starty -= offset;
      endy += offset;
    }
  }
  if ( (sectorBmp & SECTOR_NE_BIT_MASK) !=0 )
  {
    int starty = startCoord.y + 1;
    int endy = startCoord.y + 1;
    int offset = 0;
    for (int i = startCoord.x + 1; i <= startCoord.x + rangeMax;i++)
    {
      for (int j = starty; j <= endy; j++)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = max(i-startCoord.x,j-startCoord.y);
        if (entityDist>=rangeMin)
        {
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy>startCoord.y + rangeMax)
        endy = startCoord.y + rangeMax;
    }
  }
  if ( (sectorBmp & SECTOR_NN_BIT_MASK) !=0 )
  {
    int startx = startCoord.x;
    int endx = startCoord.x;
    int offset = 0;
    for (int j = startCoord.y + 1; j <= startCoord.y + rangeMax;j++)
    {
      entityDist = j-startCoord.y;
      if (entityDist>=rangeMin)
      {
        for (int i = startx; i <= endx; i++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      startx -= offset;
      endx += offset;
    }
  }
  if ( (sectorBmp & SECTOR_NW_BIT_MASK) !=0 )
  {
    int starty = startCoord.y + 1;
    int endy = startCoord.y + 1;
    int offset = 0;
    for (int i = startCoord.x - 1; i >= startCoord.x - rangeMax;i--)
    {
      for (int j = starty; j <= endy; j++)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = max(startCoord.x-i,j-startCoord.y);
        if (entityDist>=rangeMin)
        {
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy>startCoord.y + rangeMax)
        endy = startCoord.y + rangeMax;
    }
  }
  if ( (sectorBmp & SECTOR_WW_BIT_MASK) !=0 )
  {
    int starty = startCoord.y;
    int endy = startCoord.y;
    int offset = 0;
    for (int i = startCoord.x - 1; i >= startCoord.x - rangeMax;i--)
    {
      entityDist = startCoord.x-i;
      if (entityDist>=rangeMin)
      {
        for (int j = starty; j <= endy; j++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      starty -= offset;
      endy += offset;
    }
  }
  if ( (sectorBmp & SECTOR_SW_BIT_MASK) !=0 )
  {
    int starty = startCoord.y - 1;
    int endy = startCoord.y - 1;
    int offset = 0;
    for (int i = startCoord.x - 1; i >= startCoord.x - rangeMax;i--)
    {
      for (int j = starty; j >= endy; j--)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = max(startCoord.x-i,startCoord.y-j);
        if (entityDist>=rangeMin)
        {
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy<startCoord.y - rangeMax)
        endy = startCoord.y - rangeMax;
    }
  }
  if ( (sectorBmp & SECTOR_SS_BIT_MASK) !=0 )
  {
    int startx = startCoord.x;
    int endx = startCoord.x;
    int offset = 0;
    for (int j = startCoord.y - 1; j >= startCoord.y - rangeMax;j--)
    {
      entityDist = startCoord.y-j;
      if (entityDist>=rangeMin)
      {
        for (int i = startx; i <= endx; i++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 1 time over 2
      offset = (offset+1)%2;
      startx -= offset;
      endx += offset;
    }
  }
  if ( (sectorBmp & SECTOR_SE_BIT_MASK) !=0 )
  {
    int starty = startCoord.y - 1;
    int endy = startCoord.y - 1;
    int offset = 0;
    for (int i = startCoord.x + 1; i <= startCoord.x + rangeMax;i++)
    {
      for (int j = starty; j >= endy; j--)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = max(i-startCoord.x,startCoord.y-j);
        if (entityDist>=rangeMin)
        {
          for (int layer = 0; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              m_tFoundIds[nbFoundIds].pEntity = pCurEntity;
              m_tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy<startCoord.y - rangeMax)
        endy = startCoord.y - rangeMax;
    }
  }

  pFoundIds = m_tFoundIds;
  return (nbFoundIds);
}

bool CBiotop::isCoordValidAndFree(Point_t coord, int layer)
{
  if ( (coord.x < 0) || (coord.x >= m_Dimension.x)
     ||(coord.y < 0) || (coord.y >= m_Dimension.y)
     ||(layer < 0)   || (layer >= m_nbLayer)
     ||(findEntity(coord,layer) != NULL) ) // water is consider as not free
  {
    return (false);
  }
  else 
  {
    return (true);
  }
}

bool CBiotop::isCoordValid(Point_t coord, int layer)
{
  if ( (coord.x < 0) || (coord.x >= m_Dimension.x)
     ||(coord.y < 0) || (coord.y >= m_Dimension.y)
     ||(layer < 0)   || (layer >= m_nbLayer))
  {
    return (false);
  }
  else 
  {
    return (true);
  }
}

CBasicEntity* CBiotop::findTopLevelEntity(Point_t searchCoord) 
{
  CBasicEntity* pFoundEntity = NULL;
   
  for (int layer=m_nbLayer-1;layer>=0;layer--) 
  {       
    pFoundEntity = findEntity(searchCoord, layer);
    if (pFoundEntity!=NULL) 
    {
      return (pFoundEntity);
    }
  }
  // No entity found => return NULL
  return (NULL);
}


feedbackValType CBiotop::forceEntityAction(entityIdType idEntity,choiceIndType myChoice)
{
  feedbackValType resu = 0;
  CBasicEntity* pEntity = NULL;
  pEntity = getEntityById(idEntity);
  int i;
  bool isValid;

  // Force entity action
  if (pEntity != NULL)
  {
    resu = pEntity->forceNextAction(myChoice);
  }

  // Loop on all other entities for basic action  
  /*for (i=0; i<getNbOfAnimals(); i++)    
  {
    pEntity = m_tEntity[i];
    if ((pEntity != NULL)&&(pEntity->getId()!=idEntity))
    {
      pEntity->nextSecond();
    }
  }*/

  // Trigger measurement
  for (i=0; i<m_tMeasures.size(); i++) 
  {
    isValid = m_tMeasures[i]->NextSecond();
    if (!isValid)
    {
      m_tMeasures[i]->StopMeasurement();
    }
  }

  return(resu);
}

choiceIndType CBiotop::predictEntityAction(entityIdType idEntity)
{
  choiceIndType resu = -1;
  CBasicEntity* pEntity = NULL;
  pEntity = getEntityById(idEntity);

  // Force entity action
  if (pEntity != NULL)
  {
    resu = pEntity->predictNextAction();
  }

  return (resu);
}

//===========================================================================
// Time management
//===========================================================================

void CBiotop::nextSecond(void)
{
  CBasicEntity* pEntity = NULL;
  int i;
  bool isValid;
  m_BioTime.seconds++;
  logCpuMarkerStart(BIOTOP_CPUMARKER_TOTAL);

  if (m_BioTime.seconds>=3600)
  {
    // Next hour for biotop
    nextHour();

    // loop from top to bottom to clear entities each hour
    for (i=getNbOfEntities()-1; i>=0; i--)   
    {
      pEntity = m_tEntity[i];
      if (pEntity != NULL) 
      {
        if (pEntity->isToBeRemoved())
        {
          deleteEntity(pEntity->getId());
        }
        else
        {
          pEntity->nextHour();
          if (pEntity->checkIfhasChanged())
            addBiotopEvent(BIOTOP_EVENT_ENTITY_CHANGED, pEntity);
        }
      }
    }
  }

  // Loop on all animals for basic action
  logCpuMarkerStart(BIOTOP_CPUMARKER_ANIMALS);
  for (i=0; i<getNbOfAnimals(); i++)    
  {
    pEntity = m_tEntity[i];
    if (pEntity != NULL)
    {
      pEntity->nextSecond();
      if (pEntity->checkIfhasChanged())
        addBiotopEvent(BIOTOP_EVENT_ENTITY_CHANGED, pEntity);
    }
  }
  logCpuMarkerEnd(BIOTOP_CPUMARKER_ANIMALS);

  // Trigger measurement
  for (i=0; i<m_tMeasures.size(); i++) 
  {
    isValid = m_tMeasures[i]->NextSecond();
    if (!isValid)
    {
      m_tMeasures[i]->StopMeasurement();
    }
  }
  logCpuMarkerEnd(BIOTOP_CPUMARKER_TOTAL);
}

void CBiotop::nextHour(void)
{
  for (int i=0;i<m_Dimension.x;i++)
  {
    for (int j=0;j<m_Dimension.y;j++)
    {
      for (int odor=0;odor<ODOR_NUMBER_TYPE;odor++)
      {
        if (m_tBioSquare[i][j].odorTrace[odor]<0.01)
          m_tBioSquare[i][j].odorTrace[odor] = 0;
        else
          m_tBioSquare[i][j].odorTrace[odor] = m_tBioSquare[i][j].odorTrace[odor]/2.0;
      }
    }
  }

  // Rain management
  if (testChance(10.0, getFertility({ 1,1 }) / 24.0))
  {
    spreadWaterPuddlesByRain(getRandInt(100));
  }

  // Update time
  m_BioTime.seconds = 0;
  m_BioTime.hours++;
  // Cyclic parameters updates
  m_pSunlightRate->NextStep();
  //m_pFertilityRate->NextStep();
  m_pTemperature->NextStep();

  if (m_BioTime.hours >= 24)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - nextDay CPU total=%f animals=%f custom1=%f custom2=%f for %d animals\n",
      m_CpuMonitoring[BIOTOP_CPUMARKER_TOTAL].cpuTimeCumulated,
      m_CpuMonitoring[BIOTOP_CPUMARKER_ANIMALS].cpuTimeCumulated,
      m_CpuMonitoring[BIOTOP_CPUMARKER_CUSTOM1].cpuTimeCumulated,
      m_CpuMonitoring[BIOTOP_CPUMARKER_CUSTOM2].cpuTimeCumulated,
      getNbOfAnimals());
    resetCpuMarker();

    m_BioTime.hours = 0;
    m_BioTime.days++;
    if (m_BioTime.days >= 365)
    {
      m_BioTime.days = 0;
      m_BioTime.years++;
    }
  }
}


BiotopTime_t CBiotop::getBiotopTime(void)
{
  return (m_BioTime);
}

//===========================================================================
// Grid management
//===========================================================================
void CBiotop::buildGrid(int dimX, int dimY, int dimZ)
{
  // Build 3D dynamic table
  m_tBioSquare = new BiotopSquare_t*[dimX];
  m_tBioGrid   = new BiotopCube_t**[dimX];
  ASSERT( m_tBioGrid != NULL );
  for (int i=0;i<dimX;i++)
  {
    m_tBioSquare[i] = new BiotopSquare_t[dimY];
    m_tBioGrid[i]   = new BiotopCube_t*[dimY];
    ASSERT( m_tBioGrid[i] != NULL );
    for (int j=0;j<dimY;j++)
    {
      m_tBioGrid[i][j] = new BiotopCube_t[dimZ];
      ASSERT( m_tBioGrid[i][j] != NULL );
    }
  }
}

void CBiotop::deleteGrid(void)
{
  // Delete 3D dynamic table 
  for (int i=0;i<m_Dimension.x;i++)
  {
    for (int j=0;j<m_Dimension.y;j++)
    {
      delete [] m_tBioGrid[i][j];
    }
    delete [] m_tBioGrid[i];
    delete [] m_tBioSquare[i];
  }
  delete [] m_tBioGrid;
  delete [] m_tBioSquare;
}

void CBiotop::initGridDefaultLayerType(void)
{
  int i,j;

  // Put Ground Everywhere:
  for (i=0;i<m_Dimension.x;i++)
  {
    for (j=0;j<m_Dimension.y;j++)
    {
      m_tBioGrid[i][j][0].layerType = LAYER_UNDER_GROUND;
      m_tBioGrid[i][j][1].layerType = LAYER_OVER_GROUND;
      m_tBioGrid[i][j][2].layerType = LAYER_OVER_GROUND;
    }
  }

  // Put Grass to the West:
  /*for (i=0;i<m_Dimension.x/10;i++)
  {
    for (j=0;j<m_Dimension.y;j++)
    {
      m_tBioGrid[i][j][0].layerType = LAYER_UNDER_GROUND;
      m_tBioGrid[i][j][1].layerType = LAYER_GLOBAL_GRASS; // grass 
      m_tBioGrid[i][j][2].layerType = LAYER_OVER_GROUND;
    }
  }*/

  // Add a River on South
  /*for (i=0;i<m_Dimension.x;i++)
  {
    m_tBioGrid[i][0][0].layerType = LAYER_UNDER_WATER;
    m_tBioGrid[i][0][1].layerType = LAYER_OVER_WATER;
    m_tBioGrid[i][0][2].layerType = LAYER_IN_AIR;
    m_tBioGrid[i][1][0].layerType = LAYER_UNDER_GROUND;
    m_tBioGrid[i][1][1].layerType = LAYER_OVER_WET_GROUND;
    m_tBioGrid[i][1][2].layerType = LAYER_OVER_GROUND;
  }
  // Add a River on West
  for (j=1;j<m_Dimension.y;j++)
  {
    m_tBioGrid[0][j][0].layerType = LAYER_UNDER_WATER;
    m_tBioGrid[0][j][1].layerType = LAYER_OVER_WATER;
    m_tBioGrid[0][j][2].layerType = LAYER_IN_AIR;
    m_tBioGrid[1][j][0].layerType = LAYER_UNDER_WATER;
    m_tBioGrid[1][j][1].layerType = LAYER_OVER_WATER;
    m_tBioGrid[1][j][2].layerType = LAYER_IN_AIR;
  }
  m_tBioGrid[2][2][0].layerType = LAYER_UNDER_GROUND;
  m_tBioGrid[2][2][1].layerType = LAYER_OVER_WET_GROUND;
  m_tBioGrid[2][2][2].layerType = LAYER_OVER_GROUND;*/

  // Add lake in the center
  for (i=(m_Dimension.x/2-5); i<(m_Dimension.x/2+5); i++)
  {
    for (j=(m_Dimension.y/2-5); j<(m_Dimension.y/2+5); j++)
    {
      m_tBioGrid[i][j][0].layerType = LAYER_UNDER_GROUND;
      m_tBioGrid[i][j][1].layerType = LAYER_OVER_WET_GROUND;
      m_tBioGrid[i][j][2].layerType = LAYER_OVER_GROUND;
    }
  }

  for (i=(m_Dimension.x/2-4); i<(m_Dimension.x/2+4); i++)
  {
    for (j=(m_Dimension.y/2-4); j<(m_Dimension.y/2+4); j++)
    {
      m_tBioGrid[i][j][0].layerType = LAYER_UNDER_WATER;
      m_tBioGrid[i][j][1].layerType = LAYER_OVER_WATER;
      m_tBioGrid[i][j][2].layerType = LAYER_IN_AIR;
    }
  }

}

void CBiotop::initGridDefaultAltitude(void)
{
  for (int i=0;i<m_Dimension.x;i++)
  {
    for (int j=0;j<m_Dimension.y;j++)
    {
      m_tBioSquare[i][j].altitude = 0;
    }
  }
}

void CBiotop::initGridEntity(void)
{
  // 
  for (int i=0;i<m_Dimension.x;i++)
  {
    for (int j=0;j<m_Dimension.y;j++)
    {
      for (int k=0;k<m_nbLayer;k++)
      {
        m_tBioGrid[i][j][k].pEntity = NULL;
      }

      // If water present: set water global entity
      if ( (m_tBioGrid[i][j][1].layerType == LAYER_OVER_WATER) || (m_tBioGrid[i][j][1].layerType == LAYER_OVER_WET_GROUND) )
      {
        //m_tBioGrid[i][j][0].pEntity = m_pWaterGlobalEntity;
        m_tBioGrid[i][j][1].pEntity = m_pWaterGlobalEntity;
        // set mapId
        if (m_tBioGrid[i][j][1].layerType == LAYER_OVER_WATER)
          m_tBioSquare[i][j].mapId = 3; // deap water
        else
          m_tBioSquare[i][j].mapId = 4; // wet ground
      }
      else if (m_tBioGrid[i][j][1].layerType == LAYER_GLOBAL_GRASS) 
      {
        m_tBioGrid[i][j][1].pEntity = m_pGrassGlobalEntity;
        // set mapId
        m_tBioSquare[i][j].mapId = 5; //grass ground
      }
      else
      {
        // set mapId
        m_tBioSquare[i][j].mapId = 6; //def ground
      }

      // reset odor traces
      for (int odor=0;odor<ODOR_NUMBER_TYPE;odor++)
      {
        m_tBioSquare[i][j].odorTrace[odor] = 0;
      }

      // reset custom color
      m_tBioSquare[i][j].customColor = 0x00FFFFFF; //No color
    }
  }

  updateGridAllEntities();
}

void CBiotop::updateGridEntity(CBasicEntity* pCurEntity)
{
  ASSERT (pCurEntity!=NULL);
  Point_t tmpCoord;
  int tmpLayer;
   
  if ( pCurEntity->checkIfhasMovedAndClear() )
  {
    tmpCoord = pCurEntity->getPrevGridCoord();
    tmpLayer = pCurEntity->getPrevLayer();
    if ( isCoordValid(tmpCoord,tmpLayer) )
    {
      // memorize odor trace
      m_tBioSquare[tmpCoord.x][tmpCoord.y].odorTrace[pCurEntity->getOdor()] = MAX_SENSOR_VAL/10;
      m_tBioGrid[tmpCoord.x][tmpCoord.y][tmpLayer].pEntity = NULL; 
    }
    tmpCoord = pCurEntity->getGridCoord();
    tmpLayer = pCurEntity->getLayer();
    if ( isCoordValid(tmpCoord,tmpLayer) )
    {
      m_tBioGrid[tmpCoord.x][tmpCoord.y][tmpLayer].pEntity  = pCurEntity; 
    }
  }
}

int CBiotop::getGridDistance(Point_t gridCoord1, Point_t gridCoord2)
{
  return max(abs(gridCoord1.x - gridCoord2.x), abs(gridCoord1.y - gridCoord2.y));
}

void CBiotop::updateGridAllEntities(void)
{
  Point_t tmpCoord;
  int i,tmpLayer;

  // Clear previous positions
  for (CBasicEntity* pCurEntity : m_tEntity)
  {  
    if ( (pCurEntity) && (pCurEntity->checkIfhasMoved()) )
    {
      tmpCoord = pCurEntity->getPrevGridCoord();
      tmpLayer = pCurEntity->getPrevLayer();
      if ( isCoordValid(tmpCoord,tmpLayer) )
      {
        m_tBioGrid[tmpCoord.x][tmpCoord.y][tmpLayer].pEntity  = NULL; 
      }
    }
  }
  // Set new positions
  for (CBasicEntity* pCurEntity : m_tEntity)
  {  
    if ( (pCurEntity) && (pCurEntity->checkIfhasMovedAndClear()) )
    {
      tmpCoord = pCurEntity->getGridCoord();
      tmpLayer = pCurEntity->getLayer();
      if ( isCoordValid(tmpCoord,tmpLayer) )
      {
        m_tBioGrid[tmpCoord.x][tmpCoord.y][tmpLayer].pEntity  = pCurEntity; 
      }
    }
  }
}

//===========================================================================
// Measure management
//===========================================================================
bool CBiotop::addMeasureBiotopParam(BiotopParameterType_e type, int period, int id)
{
  CMeasureParam* pNewMeas = new CMeasureBiotopParam(this, type, period, id);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);

  return (true);
}

bool CBiotop::addMeasureEntityParam(CBasicEntity* pEntity, int paramId, int period, int id)
{
  if ( (pEntity==NULL) || (period<0) || (pEntity->getParameter(paramId)==NULL) )
    return (false);

  CMeasureParam* pNewMeas = new CMeasureEntityParam(pEntity, paramId, period, id);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);

  return (true);
}

bool CBiotop::addMeasureFeeling(CBasicEntity* pEntity, int period, int id, MeasureFeelingType_e type)
{
  if ((pEntity==NULL)||(period<0))
    return (false);

  CMeasureFeeling* pNewMeas = new CMeasureFeeling(pEntity, period, id, type);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);

  return (true);
}

bool CBiotop::addMeasureReaction(CBasicEntity* pEntity, int reactionInd, int period, int id, MeasureReactionType_e type)
{
  if ((pEntity==NULL)||(period<0))
    return (false);

  CMeasureReaction* pNewMeas = new CMeasureReaction(pEntity, reactionInd, period, id, type);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);

  return (true);
}

bool CBiotop::addMeasurePopulation(int period, int id, MeasurePopulationType_e type, int maxVal, string SpecieName)
{
  CMeasurePopulation* pNewMeas = new CMeasurePopulation(this, period, id, type, maxVal, SpecieName);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);

  return (true);
}

CMeasure* CBiotop::getMeasureById(int id)
{
  CMeasure* foundMeas = NULL;

  for (int i=0; i<m_tMeasures.size(); i++) 
  {
    if (m_tMeasures[i]->GetId() == id)
      foundMeas = m_tMeasures[i];
  }

  return (foundMeas);
}

CMeasure* CBiotop::getMeasureByIndex(int index)
{
  return (m_tMeasures[index]);
}

int CBiotop::getNbOfMeasures(void)
{
  return (m_tMeasures.size());
}

void CBiotop::replaceMeasure(int id, CMeasure* pMeasure)
{
  if (pMeasure==NULL)
    return;

  CMeasure* foundMeas = NULL;
  int foundIndex = -1;

  for (int i=0; i<m_tMeasures.size(); i++) 
  {
    if (m_tMeasures[i]->GetId() == id)
    {
      foundMeas = m_tMeasures[i];
      foundIndex = i;
      break;
    }
  }

  if (foundMeas!=NULL)
  {
    m_tMeasures.erase(m_tMeasures.begin() + foundIndex);
    m_tMeasures.insert(m_tMeasures.begin() + foundIndex,  1, pMeasure);
    delete(foundMeas);
  }
  else
  {
    m_tMeasures.push_back(pMeasure);
  }
  
  pMeasure->StartMeasurement(m_BioTime);
}


bool CBiotop::checkMeasureEvents()
{
  bool resu = false;
  for (int i=0; i<m_tMeasures.size(); i++) 
  {
    if (m_tMeasures[i]->CheckEvent() == true)
    {
      resu = true;
    }
  }
  return (resu);
}

void CBiotop::saveAllMeasuresInFile(string fileNameWithPath)
{
  for (int i = 0; i<m_tMeasures.size(); i++)
  {
    m_tMeasures[i]->saveInFile(fileNameWithPath);
  }
}


//===========================================================================
// Event management
//===========================================================================
bool CBiotop::addBiotopEvent(BiotopEventType_e biotopEvent, CBasicEntity* pEntity)
{
  BiotopEvent_t newEvent;
  newEvent.eventType = biotopEvent;
  newEvent.pEntity = pEntity;
  newEvent.entityId = pEntity->getId();
  m_tEvents.push_back(std::move(newEvent));

  // Avoid overload by cleaning oldest events
  if (m_tEvents.size() > 100000)
  {
    m_tEvents.erase(m_tEvents.begin(), m_tEvents.begin() + 1000);
  }

  return true;
}

bool CBiotop::resetBiotopEvents()
{
  m_tEvents.resize(0);
  return true;
}

int CBiotop::getNbOfBiotopEvents(void)
{
  return m_tEvents.size();
}

BiotopEvent_t CBiotop::getBiotopEvent(unsigned int index)
{
  return m_tEvents[index];
}


//===========================================================================
// Save/Load in file
//===========================================================================
bool CBiotop::saveInXmlFile(string fileName, string pathName)
{
  bool resu = false;
  TiXmlDocument *pXmlDoc = new TiXmlDocument(pathName + fileName);

  if (pXmlDoc==NULL) 
    return false;

  saveInXmlFile(pXmlDoc, pathName, true);

  resu = pXmlDoc->SaveFile();
  delete pXmlDoc;
  return resu;

}

bool CBiotop::saveInXmlFile(TiXmlDocument *pXmlDoc, string pathNameForEntities, bool saveEntities)
{
  TiXmlElement* pElement;
  TiXmlNode* pNodeBiotop = NULL;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeChild = NULL;
  int i,j,k;
  CBasicEntity* pCurEntity = NULL;
  string previousFileName = "";

  pNodeBiotop = pXmlDoc->FirstChild(XML_NODE_BIOTOP);
  if (pNodeBiotop==NULL)
  {
    TiXmlElement newNode(XML_NODE_BIOTOP);
    pNodeBiotop = pXmlDoc->InsertEndChild(newNode);
  }

  if ((pNodeBiotop != NULL) && (pNodeBiotop->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNodeBiotop;
    pElement->SetAttribute( XML_ATTR_LABEL,      m_Label);
    pElement->SetAttribute( XML_ATTR_SIZE_X,     m_Dimension.x);
    pElement->SetAttribute( XML_ATTR_SIZE_Y,     m_Dimension.y);
    pElement->SetAttribute( XML_ATTR_SIZE_LAYER, m_nbLayer);
    pElement->SetAttribute( XML_ATTR_BIO_TIME  , convertBioTimeToCount(m_BioTime));
  }

  // Clear previous parameters
  while (pNodeBiotop->FirstChild(XML_NODE_PARAMETER) != NULL)
    pNodeBiotop->RemoveChild(pNodeBiotop->FirstChild(XML_NODE_PARAMETER));

  // Save parameters
  for (i=0; i<m_tParam.size(); i++)
  {
    TiXmlElement newParam(XML_NODE_PARAMETER);
    pNodeChild = pNodeBiotop->InsertEndChild(newParam);
    if (pNodeChild != NULL) 
    {
      pElement = (TiXmlElement*)pNodeChild;
      pElement->SetAttribute(XML_ATTR_NAME, getParameter(i)->getLabel());
      pElement->SetDoubleAttribute(XML_ATTR_VALUE, getParameter(i)->getVal());
    }
  }

  // Grid
  pNode = pNodeBiotop->FirstChild(XML_NODE_GRID);
  if (pNode==NULL)
  {
    // Create Grid node
    TiXmlElement newNode(XML_NODE_GRID);
    pNode = pNodeBiotop->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous childs
    while (pNode->FirstChild() != NULL)
      pNode->RemoveChild(pNode->FirstChild());
  }

  if ((pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Save layers
    for (k=0; k<m_nbLayer; k++)
    {
      TiXmlElement newLayer(XML_NODE_LAYER);
      pNodeChild = pNode->InsertEndChild(newLayer);
      if (pNodeChild != NULL) 
      {
        string strData = "";
        for (i=0;i<m_Dimension.x;i++)
        {
          for (j=0;j<m_Dimension.y;j++)
          {
            strData += FormatString("%02u",(BYTE)m_tBioGrid[i][j][k].layerType); 
          }
        }
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_INDEX, k);
        pElement->SetAttribute(XML_ATTR_RAW_DATA, strData);
      }
    }
  }

  // Entities
  pNode = pNodeBiotop->FirstChild(XML_NODE_ENTITIES);
  if (pNode==NULL)
  {
    // Create entities node
    TiXmlElement newNode(XML_NODE_ENTITIES);
    pNode = pNodeBiotop->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous childs
    while (pNode->FirstChild() != NULL)
      pNode->RemoveChild(pNode->FirstChild());
  }

  if (saveEntities && (pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Save childs
    for (i=0; i<getNbOfEntities(); i++)
    {
      TiXmlElement newEntityNode(XML_NODE_ENTITY);
      pNodeChild = pNode->InsertEndChild(newEntityNode);
      if (pNodeChild != NULL) 
      {
        pCurEntity = getEntityByIndex(i);
        string entityFileName = pCurEntity->getLabel() + ".xml";
       
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_COORD_X,   pCurEntity->getStepCoord().x);
        pElement->SetAttribute(XML_ATTR_COORD_Y,   pCurEntity->getStepCoord().y);
        pElement->SetAttribute(XML_ATTR_LAYER,     pCurEntity->getLayer());
        pElement->SetAttribute(XML_ATTR_DIRECTION, pCurEntity->getDirection());
        pElement->SetAttribute(XML_ATTR_FILE_NAME, entityFileName);

        if (entityFileName != previousFileName)
        {
          // Don't save twice the same file
          pCurEntity->saveInXmlFile(pathNameForEntities + entityFileName);
          previousFileName = entityFileName;
        }
      }
    }
  }

  return true;
}


bool CBiotop::loadFromXmlFile(string fileName, string pathName)
{

  TiXmlDocument xmlDoc(pathName + fileName);
  if (xmlDoc.LoadFile() == false)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Error loading biotop file: %s\n", fileName.c_str());
    CYBIOCORE_LOG_FLUSH;
    return false;
  }

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Load: file %s path %s\n", fileName.c_str(), pathName.c_str());
  loadFromXmlFile(&xmlDoc, pathName);
  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - File load complete\n");
  CYBIOCORE_LOG_FLUSH;
  return true;
}

bool CBiotop::loadFromXmlFile(TiXmlDocument *pXmlDoc, string pathNameForEntities)
{
  TiXmlElement* pElement;
  TiXmlNode* pNode = NULL;

  TiXmlNode* pNodeBiotop = pXmlDoc->FirstChild(XML_NODE_BIOTOP);
  if ((pNodeBiotop != NULL) && (pNodeBiotop->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeBiotop;
    int sizeX, sizeY, nbLayer;
    string timeCountStr;

    if ( pElement->QueryStringAttribute(XML_ATTR_LABEL,  &m_Label) == TIXML_NO_ATTRIBUTE)
      m_Label = "Unset";
    if ( pElement->QueryIntAttribute(XML_ATTR_SIZE_X,  &sizeX) == TIXML_NO_ATTRIBUTE)
      sizeX = 10;
    if ( pElement->QueryIntAttribute(XML_ATTR_SIZE_Y,  &sizeY) == TIXML_NO_ATTRIBUTE)
      sizeY = 10;
    if ( pElement->QueryIntAttribute(XML_ATTR_SIZE_LAYER,  &nbLayer) == TIXML_NO_ATTRIBUTE)
      nbLayer = 3;
    if ( pElement->QueryStringAttribute(XML_ATTR_BIO_TIME,  &timeCountStr) == TIXML_NO_ATTRIBUTE)
      timeCountStr = "0";

    // Clear Previous Biotop
    deleteAllEntities();
    deleteAllMeasures();
    deleteGrid(); 

    m_Dimension.x = sizeX;
    m_Dimension.y = sizeY;
    m_nbLayer = nbLayer;
    m_BioTime = convertCountToBioTime(atoi(timeCountStr.c_str()));
    m_DefaultFilePath = pathNameForEntities;

    buildGrid(m_Dimension.x, m_Dimension.y, m_nbLayer);

    // Parameters management
    string paramName;
    double paramVal;
    CGenericParam* pParam;
    pNode = pNodeBiotop->FirstChild(XML_NODE_PARAMETER);
    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_PARAMETER))
      {
        pElement = (TiXmlElement*)pNode; 
        if ( pElement->QueryDoubleAttribute(XML_ATTR_VALUE,  &paramVal) == TIXML_NO_ATTRIBUTE)
          paramVal = 0;
        
        if ( pElement->QueryStringAttribute(XML_ATTR_NAME,  &paramName) != TIXML_NO_ATTRIBUTE)
        {
          pParam = getParameterByName(paramName);
          if (pParam != NULL)
            pParam->setVal(paramVal);
        }
      }
      pNode = pNode->NextSibling();
    }

    // Load Grid data
    int i,j;
    int layerIndex;
    string dataStr;
    pNode = pNodeBiotop->FirstChild(XML_NODE_GRID);
    if (pNode!=NULL)
      pNode = pNode->FirstChild(XML_NODE_LAYER);

    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_LAYER))
      {
        pElement = (TiXmlElement*)pNode; 
        if ( pElement->QueryIntAttribute(XML_ATTR_INDEX,  &layerIndex) == TIXML_NO_ATTRIBUTE)
          layerIndex = 0;
        if ( pElement->QueryStringAttribute(XML_ATTR_RAW_DATA,  &dataStr) != TIXML_NO_ATTRIBUTE)
        {
          char curTypeStr[3];
          curTypeStr[2] = 0;
          const char* bufferStr = dataStr.c_str();
          for (i=0;i<m_Dimension.x;i++)
          {
            for (j=0;j<m_Dimension.y;j++)
            {
              curTypeStr[0] = *bufferStr;
              bufferStr++;
              curTypeStr[1] = *bufferStr;
              bufferStr++;
              m_tBioGrid[i][j][layerIndex].layerType = (LayerType_e)atoi(curTypeStr);
            }
          }
        }
      }
      pNode = pNode->NextSibling();
    }

    initGridEntity();

    // Load entities
    Point_t gridCoord;
    Point_t stepCoord;
    int layer, direction;
    entityIdType resuId;
    string entityFileName,previousFileName = "";
    CBasicEntity* pEntity = NULL;
    pNode = pNodeBiotop->FirstChild(XML_NODE_ENTITIES);
    if (pNode!=NULL)
      pNode = pNode->FirstChild(XML_NODE_ENTITY);

    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_ENTITY))
      {
        pElement = (TiXmlElement*)pNode; 
        if ( pElement->QueryIntAttribute(XML_ATTR_COORD_X,  &stepCoord.x) == TIXML_NO_ATTRIBUTE)
          stepCoord.x = -1;
        if ( pElement->QueryIntAttribute(XML_ATTR_COORD_Y,  &stepCoord.y) == TIXML_NO_ATTRIBUTE)
          stepCoord.y = -1;
        if ( pElement->QueryIntAttribute(XML_ATTR_LAYER,  &layer) == TIXML_NO_ATTRIBUTE)
          layer = -1;
        if ( pElement->QueryIntAttribute(XML_ATTR_DIRECTION,  &direction) == TIXML_NO_ATTRIBUTE)
          direction = 0;
        if ( pElement->QueryStringAttribute(XML_ATTR_FILE_NAME,  &entityFileName) != TIXML_NO_ATTRIBUTE)
        {
          gridCoord.x  = stepCoord.x / NB_STEPS_PER_GRID_SQUARE;
          gridCoord.y  = stepCoord.y / NB_STEPS_PER_GRID_SQUARE;
          if ( (entityFileName == previousFileName) && (pEntity) )
          {
            resuId = createAndAddCloneEntity(pEntity->getId(), gridCoord, layer);
            pEntity = getEntityById(resuId);
            if (pEntity)
            {
              pEntity->setDirection(direction);
              pEntity->jumpToStepCoord(stepCoord);
              pEntity->checkIfhasMovedAndClear();
              pEntity->checkIfhasChangedAndClear();
            }
          }
          else
          { 
            resuId = createAndAddEntity(entityFileName, pathNameForEntities, gridCoord);
            if (resuId != -1)
            {
              pEntity = getEntityById(resuId);
            }
            else
            {
              pEntity = NULL;
            }
            if (pEntity)
            {
              pEntity->setDirection(direction);
            }
          }
          previousFileName = entityFileName;
        }
      }
      pNode = pNode->NextSibling();
    }

  }

  return true;
}

//===========================================================================
// Specific behaviors
//===========================================================================
void CBiotop::spreadWaterPuddlesByRain(int coverRate)
{
  CGenome* pGenome1 = new CGenome(CLASS_NONE, "");

  pGenome1->loadFromXmlFile("..\\DataScriptMammal\\water_puddle.xml");

  Point_t coord = { 10,10 };
  entityIdType waterId;

  int nbPuddles = m_Dimension.x * m_Dimension.y * coverRate / 100000;
  for (int i = 0; i < nbPuddles; i++)
  {
    CGenome* pGenome = new CGenome(*pGenome1);
    coord.x = getRandInt(m_Dimension.x) + 2;
    coord.y = getRandInt(m_Dimension.y) + 2;
    waterId = createAndAddEntity("water_puddle", coord, 1, pGenome);
  }

  delete pGenome1;

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Rain : %d puddles\n", coverRate);
}

//===========================================================================
// CPU Marker
//===========================================================================

void CBiotop::resetCpuMarker()
{
  for (int i = 0; i < BIOTOP_CPUMARKER_LAST; i++)
  {
    m_CpuMonitoring[i].cpuTimeCumulated = 0;
  }
}

void CBiotop::logCpuMarkerStart(BiotopCpuMarkerType_e markerId)
{
  m_CpuMonitoring[markerId].startTime = std::chrono::system_clock::now();
}

void CBiotop::logCpuMarkerEnd(BiotopCpuMarkerType_e markerId)
{
  m_CpuMonitoring[markerId].endTime = std::chrono::system_clock::now();
  std::chrono::duration<double> diffTime = m_CpuMonitoring[markerId].endTime - m_CpuMonitoring[markerId].startTime;
  m_CpuMonitoring[markerId].cpuTimeCumulated += diffTime.count();
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

Point_t CBiotop::getDimension() 
{
  return (m_Dimension);
}

double CBiotop::getFertility(Point_t coord) 
{
  // coord may be used later
  return m_pFertilityRate->getVal();    
}
   
void CBiotop::setFertilityRate(int fertilityRate) 
{
  m_pFertilityRate->setVal(fertilityRate);    
}
     
double CBiotop::getRadioactivityRate() 
{
  // TBD: coord may be used later
  return (m_pRadioactivity->getVal());    
}

double CBiotop::getTemperature(Point_t coord, int layer)
{
  double computedTemperature = m_pTemperature->getVal();
  CBasicEntity* pCurEntity = NULL;
  FoundEntity_t* pFoundIds = NULL;
  int nbIds = findEntities(pFoundIds, coord, 1, true);

  // Give bonus malus on temperature according to entities around
  for (int ind=0;ind<nbIds;ind++)
  {
    pCurEntity = pFoundIds[ind].pEntity;
    if (pCurEntity->getClass() >= CLASS_ANIMAL_FIRST)
      computedTemperature += 2.0;
    else if (pCurEntity->getClass() >= CLASS_VEGETAL_FIRST)
      computedTemperature -= 0.4;
    else if (pCurEntity->getClass() == CLASS_WATER)
      computedTemperature -= 0.6;
  }

  // Check if shadow
  Point_t southPos = coord;
  southPos.y -= 1;
  pCurEntity = findTopLevelEntity(southPos);
  if ((pCurEntity != NULL) && (pCurEntity->getLayer() >= layer))
    computedTemperature += m_pSunlightRate->getVal()/20.0 - 5.0;
  else
    computedTemperature += m_pSunlightRate->getVal()/10.0 - 5.0;

  // TBD: Altitude may be used later

  return (computedTemperature);
}
 
double CBiotop::getSunlight()
{
  // TBD: Shadow may be used later
  return (m_pSunlightRate->getVal());
}

LayerType_e CBiotop::getLayerType(Point_t coord, int layer)
{
  LayerType_e resu = LAYER_UNDER_GROUND; // default value
  if (isCoordValid(coord,layer))
  {
    resu =  m_tBioGrid[coord.x][coord.y][layer].layerType;
  }

  return (resu);
}

int CBiotop::getNbLayer(void)
{
  return(m_nbLayer);
}

CGenericParam* CBiotop::getParamFertility()
{
  return (m_pFertilityRate);
}
CGenericParam* CBiotop::getParamSunlight()
{
  return (m_pSunlightRate);
}
CGenericParam* CBiotop::getParamRadioactivity()
{
  return (m_pRadioactivity);
} 
CGenericParam* CBiotop::getParamTemperature()
{
  return (m_pTemperature);
} 

CGenericParam* CBiotop::getParameter(int id)
{
  if ( (id<0) || (id>m_tParam.size()) )
  {
    return (NULL);
  }
  else
  {
    return (m_tParam[id]);
  }
}

CGenericParam* CBiotop::getParameterByName(string& paramName)
{
  for (unsigned int id=0; id<m_tParam.size(); id++)
  {
    if (m_tParam[id]->getLabel() == paramName)
      return (m_tParam[id]);
  }

  return (NULL);
}

double CBiotop::getOdorTrace(Point_t coord, OdorType_e odor) 
{
  // coord may be used later
  return (m_tBioSquare[coord.x][coord.y].odorTrace[odor]);    
}

bool CBiotop::getOdorLevels(Point_t coord, int range, double odorLevel[NUMBER_ODORS], entityIdType excludedEntityId)
{
  // Init to 0
  int i;
  for (i=0; i<NUMBER_ODORS; i++)
    odorLevel[i]=0;

  // Check input coord
  if (!isCoordValid(coord,1))
    return false;

  Point_t coordWind = coord;
  if (m_WindStrenght>0)
  {
    coordWind.x -= vectorDirection[m_WindDirection].x;
    coordWind.y -= vectorDirection[m_WindDirection].y;
  }

  // Find entities
  CBasicEntity* pCurEntity = NULL;
  FoundEntity_t* pFoundIds = NULL;

  int nbIds = findEntities(pFoundIds, coordWind, range);
  for (int ind=0;ind<nbIds;ind++)
  {
    pCurEntity = pFoundIds[ind].pEntity;
    if ((pCurEntity!=NULL) && (pCurEntity->getId() != excludedEntityId))
    {
      for (int odor=0; odor<NUMBER_ODORS; odor++)
      {
        // Add odor for entities
        if (pCurEntity->getOdor() == (ODOR_FIRST_TYPE+odor) )
        {
          odorLevel[odor] += MAX_SENSOR_VAL / ((double)pFoundIds[ind].distance + 2); // 1/R
        }
      }
    }
  }

  // Add trace odor
  for (i=0; i<NUMBER_ODORS; i++)
  {
    // Add odor trace
    odorLevel[i] += getOdorTrace(coord, (OdorType_e)(ODOR_FIRST_TYPE+i));

    // Use first threshold
    if (odorLevel[i] > MAX_SENSOR_VAL)
      odorLevel[i] = MAX_SENSOR_VAL;
  }

  return true;
}

COLORREF CBiotop::getCustomColor(Point_t coord)
{
  // coord may be used later
  return (m_tBioSquare[coord.x][coord.y].customColor);    
}

WORD CBiotop::getMapId(Point_t coord)
{
  // coord may be used later
  if ( (coord.x < m_Dimension.x) && (coord.y < m_Dimension.y) )
    return (m_tBioSquare[coord.x][coord.y].mapId);
  else
    return 0;
}

BiotopSquare_t** CBiotop::getpBioSquare()
{
  return (m_tBioSquare);
}

string CBiotop::getLabel()
{
  return (m_Label);
}

int CBiotop::getWindDirection()
{
  return m_WindDirection;
}

void CBiotop::setWindDirection(int direction)
{
  m_WindDirection = direction%8;
}

int CBiotop::getWindStrenght()
{
  return m_WindStrenght;
}

void CBiotop::setWindStrenght(int strenght)
{
  m_WindStrenght = strenght;
}