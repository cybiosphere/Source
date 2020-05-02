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

RelativePos_t vectorDirection[8] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

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
  setBiotopTime(0, 12, 0, 0);
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

  m_pRockGlobalEntity = new CRock();
  // Set object caract
  m_pRockGlobalEntity->setEntityFromGenome(0);
  m_pRockGlobalEntity->setId(ENTITY_ID_ROCK);

  m_IdLastEntity = ENTITY_ID_FIRST_USER_ENTITY;
  m_tMeasures.resize(0);
  m_tGeoMapSpecies.resize(0);
  m_BiotopFoundIds.tFoundIds.resize(MAX_FOUND_ENTITIES);
  m_BiotopFoundIds.nbFoundIds = 0;

  resetCpuMarker();

  m_tRandomEntitiesGeneration.resize(MAX_NUMBER_RANDOM_ENTITIES);
  for (size_t i = 0; i < m_tRandomEntitiesGeneration.size(); i++)
  {
    m_tRandomEntitiesGeneration[i].pModelEntity = NULL;
  }
  // Set rain as default random entity
  addEntitySpawner(0, "water_puddle.xml", "../DataScriptMammal/", 50, 5, true);

  m_bColorizeSearch = false;

  CYBIOCORE_LOG_INIT;
  CYBIOCORE_LOG("BIOTOP - Biotop created\n");
}
   
CBiotop::~CBiotop() 
{
  deleteAllMeasures();
  deleteAllParameters();
  deleteAllEntities();
  deleteGrid();  // Delete 3D dynamic table

  delete m_pWaterGlobalEntity;
  delete m_pGrassGlobalEntity;
  delete m_pRockGlobalEntity;

  CYBIOCORE_LOG_CLOSE;
}
//===========================================================================
// Entities management
//===========================================================================

entityIdType CBiotop::addEntity(CBasicEntity* pEntity, Point_t coord, bool chooseLayer, size_t newLayer)
{
  if ((pEntity == NULL) || (getNbOfEntities() > MAXIMUM_NB_ENTITIES))
    return (ENTITY_ID_INVALID);

  size_t layer;
  if (chooseLayer)
    layer = newLayer;
  else
    layer = pEntity->getLayer();
  
  if ( !isCoordValidAndFree(coord,layer) )
     return (ENTITY_ID_INVALID);

  // Set new id
  m_IdLastEntity++;
  pEntity->setId(m_IdLastEntity);
  // Go to position
  pEntity->jumpToGridCoord(coord, true, layer);
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

bool CBiotop::addRemoteCtrlEntity(entityIdType idEntity, CBasicEntity* pEntity, Point_t stepCoord, bool chooseLayer, size_t newLayer)
{
  size_t layer;
  if (chooseLayer)
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
  pEntity->jumpToStepCoord(stepCoord, true, layer);
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
  Point_t coord = { invalidCoord, invalidCoord };
  size_t layer = invalidCoord;

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

CBasicEntity* CBiotop::createEntity(string fileName, string pathName)
{
  string fileNameWithPath = pathName + fileName;
  TiXmlDocument* pXmlDoc = new TiXmlDocument(fileNameWithPath);
  if (!pXmlDoc->LoadFile())
  {
    CYBIOCORE_LOG("CHECK - Error reading entity file: %s\n", fileNameWithPath.c_str());
    delete pXmlDoc;
    return NULL;
  }
  CBasicEntity* pNewEntity = CBiotop::createEntity(pXmlDoc, pathName);
  delete pXmlDoc;
  return (pNewEntity);
}

CBasicEntity* CBiotop::createEntity(TiXmlDocument *pXmlDoc, string pathNameForBabies)
{
  string name;
  CGenome* pTempGenome = new CGenome(CLASS_NONE,"");
  int startLayer;
  CBasicEntity::getDefaultLayerFromXmlFile(pXmlDoc, startLayer);
  CBasicEntity::getGenomeFromXmlFile(pXmlDoc, *pTempGenome);
  CBasicEntity::getEntityNameFromXmlFile(pXmlDoc, name);

  CBasicEntity* pNewEntity = createEntity(name, pTempGenome);
  pNewEntity->jumpToGridCoord({invalidCoord, invalidCoord}, true, startLayer);

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
  string name = pModelEntity->getLabel();

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

    // If dead quick aging in dead mode
    if (pModelEntity->getStatus() == STATUS_DEAD)
    {
      pNewEntity->autoKill();
      pNewEntity->quickAgeing(pModelEntity->getDecompositionTime());
    }

  }

  // Copie parameters
  for (size_t i=0; i<pNewEntity->getNumParameter(); i++)
    pNewEntity->getParameter(i)->forceVal(pModelEntity->getParameter(i)->getVal());

  // Copie Status
  pNewEntity->setStatus(pModelEntity->getStatus());

  return pNewEntity;
}


entityIdType CBiotop::createAndAddEntity(string name, Point_t coord, size_t layer, CGenome* pGenome)
{
  // Check coords
  if ( !isCoordValidAndFree(coord,layer) )
     return (ENTITY_ID_INVALID);

  // Create entity
  CBasicEntity* pNewEntity = createEntity(name,pGenome);
  if (pNewEntity==NULL)
    return (ENTITY_ID_INVALID);

  if (pGenome->getClass() > CLASS_MINERAL_LAST)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - New entity : specie %s name %s\n", pGenome->getSpecieName().c_str(), name.c_str());
  }

  // Put it in the biotop (with check coord);
  entityIdType resuId = addEntity(pNewEntity, coord, true, layer);
  if (resuId == ENTITY_ID_INVALID)
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
    delete pXmlDoc;
    return ENTITY_ID_INVALID;
  }
  newEntityId = createAndAddEntity(pXmlDoc, pathName, coord);
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
     return (ENTITY_ID_INVALID);

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
    return ENTITY_ID_INVALID;
  }

  return newEntityId;
}


entityIdType CBiotop::createAndAddCloneEntity(entityIdType idModelEntity, Point_t cloneCoord, size_t cloneLayer, string cloneName)
{
  CBasicEntity* pModelEntity = getEntityById(idModelEntity);
  CBasicEntity* pNewEntity = NULL;

  size_t layer = cloneLayer;
  string name = cloneName;

  if (name == "")
    name = pModelEntity->getLabel();

  // Check coords
  if ( !isCoordValidAndFree(cloneCoord,layer) )
     return (ENTITY_ID_INVALID);

  pNewEntity = createCloneEntity(pModelEntity);

  if (pNewEntity==NULL)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Error copy clone Entity : %s\n", pModelEntity->getLabel().c_str());
    return (ENTITY_ID_INVALID);
  }

  // Put it in the biotop (with check coord);
  entityIdType resuId = addEntity(pNewEntity, cloneCoord, true, layer);
  if (resuId == ENTITY_ID_INVALID)
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
  size_t oldLayer     = pOldEntity->getLayer();
  entityIdType oldId = pOldEntity->getId();

  // Destroy Old entity
  size_t prevIndex = deleteEntity(oldId, false);

  // Create new entity
  CBasicEntity* pNewEntity = createEntity(oldLabel,pNewEntityGenome);
  if (pNewEntity==NULL)
    return (false);

  // Set coord
  pNewEntity->jumpToGridCoord(oldCoord, true, oldLayer); 

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

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Entity genome updated : specie %s name %s\n", pNewEntity->getSpecieName().c_str(), pNewEntity->getLabel().c_str());

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
  size_t oldLayer    = pOldEntity->getLayer();
  entityIdType oldId = pOldEntity->getId();

  // Destroy Old entity
  pOldEntity->autoRemove();

  addEntity(pNewEntity, oldCoord, true, oldLayer);
  pNewEntity->setId(oldId);

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Entity full update : specie %s name %s\n", pNewEntity->getSpecieName().c_str(), pNewEntity->getLabel().c_str());

  addBiotopEvent(BIOTOP_EVENT_ENTITY_MODIFIED, pNewEntity);

  return true;
}


size_t CBiotop::deleteEntity(entityIdType idEntity, bool displayLog)
{ 
  CBasicEntity* pEntity = NULL;
  for (size_t i = 0; i < getNbOfEntities(); i++)     
  { 
    pEntity = m_tEntity[i];
    if (pEntity->getId() == idEntity)
    {
      if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
      {
        m_IndexLastAnimal--;
      }
      if (displayLog)
      {
        CYBIOCORE_LOG_TIME(m_BioTime);
        CYBIOCORE_LOG("BIOTOP - Entity removed : specie %s name %s\n", pEntity->getSpecieName().c_str(), pEntity->getLabel().c_str());
      }
      delete (m_tEntity[i]); 
      m_tEntity.erase(m_tEntity.begin()+i);
      return (i);
    }
  }
  // If not found
  return (invalidIndex);
}
 

void CBiotop::deleteAllEntities() 
{
  // loop from top to bottom 
  for (size_t i = 0; i < m_tEntity.size(); i++)
  {
    if (m_tEntity[i] != NULL)
      delete (m_tEntity[i]); 
  }
  m_tEntity.clear();
  m_IndexLastAnimal = 0;
}          


void CBiotop::deleteAllMeasures() 
{
  for (size_t i = 0; i < m_tMeasures.size(); i++)
  {
    if (m_tMeasures[i] != NULL)
      delete (m_tMeasures[i]); 
  }
  m_tMeasures.clear();

  for (size_t i = 0; i < m_tGeoMapSpecies.size(); i++)
  {
    if (m_tGeoMapSpecies[i] != NULL)
      delete (m_tGeoMapSpecies[i]);
  }
  m_tGeoMapSpecies.clear();
}   


void CBiotop::deleteAllParameters() 
{
  // loop from top to bottom 
  for (size_t i= 0; i < m_tParam.size(); i++)
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
    printf("x=%2d, y=%2d ", (int)pEntity->getGridCoord().x, (int)pEntity->getGridCoord().y);
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

  pGenome6->loadFromXmlFile("../DataScriptMammal/rock.xml");
  pGenome8->loadFromXmlFile("../DataScriptMammal/wooden_fence.xml");

  pGenome1->loadFromXmlFile("../DataScriptMammal/grassDry.xml");
  pGenome2->loadFromXmlFile("../DataScriptMammal/grassDry2.xml");

  Point_t coord = {10,10};
  string name;
  entityIdType pierreId, grassId, fenceId;

  size_t i;
  for (i=0; i<2; i++)
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
    if (fenceId != ENTITY_ID_INVALID)
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
    if (fenceId != ENTITY_ID_INVALID)
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
    grassId = createAndAddEntity("grassDry",coord,1,pGenome);
  }

  addMeasurePopulation(14400,7,MEASURE_POPULATION_VEGETAL,1000);

  addMeasureBiotopParam(BIO_PARAM_SUNLIGHT,3600,9);

  delete pGenome1;
  delete pGenome2;
  delete pGenome6;
  delete pGenome8;
}


size_t CBiotop::getNbOfEntities()
{
  return (m_tEntity.size());
}  

size_t CBiotop::getNbOfAnimals()
{
  return (m_IndexLastAnimal);
}  

size_t CBiotop::getNbOfVegetals()
{
  size_t tempCount = 0;
  ClassType_e curClass;

  for (CBasicEntity* pEntity : m_tEntity)
  {
    curClass = pEntity->getClass();
    if ( (curClass >= CLASS_VEGETAL_FIRST) && (curClass <= CLASS_VEGETAL_LAST) && (pEntity->getId()>0) )
      tempCount++;
  } 

  return (tempCount);
}  

size_t CBiotop::getNbOfMinerals()
{
  size_t tempCount = 0;
  ClassType_e curClass;

  for (CBasicEntity* pEntity : m_tEntity)
  {
    curClass = pEntity->getClass();
    if ( (curClass >= CLASS_MINERAL_FIRST) && (curClass <= CLASS_MINERAL_LAST) && (pEntity->getId()>0) )
      tempCount++;
  } 

  return (tempCount);
}  

size_t CBiotop::getNbOfSpecieEntities(string& SpecieName)
{
  size_t tempCount = 0;
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
 
  if (idEntity == ENTITY_ID_ROCK)
  {
    return (m_pRockGlobalEntity);
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

CBasicEntity* CBiotop::getEntityByIndex(size_t index)
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

size_t CBiotop::getEntityTableIndex(CBasicEntity* pEntity)
{
  for (size_t i=0; i<getNbOfEntities(); i++)   
  {   
    if (m_tEntity[i] == pEntity)
    {
        return (i);
    }
  }
  return (invalidIndex);
}
   
CBasicEntity* CBiotop::findEntity(Point_t searchCoord, size_t layer)
{ 
  CBasicEntity* pFoundEntity = NULL;

  if ( isCoordValid(searchCoord,layer) )
  {
    pFoundEntity = m_tBioGrid[searchCoord.x][searchCoord.y][layer].pEntity;
    if (m_bColorizeSearch)
    {
      m_tBioSquare[searchCoord.x][searchCoord.y].customColor -= 0x00001010; //blue
    }
  }
  
  return (pFoundEntity);
}

const BiotopFoundIds_t& CBiotop::findEntitiesInSquare(Point_t bottomLeftCoord, size_t squareSize, bool includeWater)
{
  int nbFoundIds = 0;
  CBasicEntity* pCurEntity = NULL;
  Point_t curCoord;
  size_t i,j;
  size_t layer;
  std::vector<FoundEntity_t>& tFoundIds = m_BiotopFoundIds.tFoundIds;
  curCoord.x = bottomLeftCoord.x;
  curCoord.y = bottomLeftCoord.y;

  for (i = 0; i < squareSize; i++)
  {
    for (j = 0; j < squareSize; j++)
    {
      for (layer = 0; layer < m_nbLayer; layer++)
      {
        pCurEntity = findEntity(curCoord, layer);
        if ((pCurEntity != NULL) && (includeWater || (pCurEntity->getId() != ENTITY_ID_WATER)))
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = i + j;
          nbFoundIds++;
        }
      }
      curCoord.y++;
    }
    curCoord.y = bottomLeftCoord.y;
    curCoord.x++;
  }

  m_BiotopFoundIds.nbFoundIds = nbFoundIds;
  return (m_BiotopFoundIds);
}


const BiotopFoundIds_t& CBiotop::findEntities(Point_t startCoord, size_t distance, bool includeWater)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;
  int k, i, segment;
  size_t layer;
  std::vector<FoundEntity_t>& tFoundIds = m_BiotopFoundIds.tFoundIds;

  // Entities in center pos
  for (layer = 0; layer < m_nbLayer; layer++)
  {
    pCurEntity = findEntity(startCoord,layer);
    if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
    {
      tFoundIds[nbFoundIds].pEntity = pCurEntity;
      tFoundIds[nbFoundIds].distance = 0;
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
          if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
          {
            tFoundIds[nbFoundIds].pEntity = pCurEntity;
            tFoundIds[nbFoundIds].distance = k;
            nbFoundIds++;
          }
        }
      }
    }
  }
  m_BiotopFoundIds.nbFoundIds = nbFoundIds;
  return (m_BiotopFoundIds);
}


//  startCoord entity not include
const BiotopFoundIds_t& CBiotop::findEntities(Point_t centerCoord, UCHAR sectorBmp, int distance, size_t layer, bool includeWater)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;
  int i, j;
  int startCoordx = (int)centerCoord.x;
  int startCoordy = (int)centerCoord.y;
  std::vector<FoundEntity_t>& tFoundIds = m_BiotopFoundIds.tFoundIds;

  if ( (sectorBmp & SECTOR_EE_BIT_MASK) !=0 )
  {
    int starty = startCoordy;
    int endy = startCoordy;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + distance;i++)
    {
      for (j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = i - startCoordx;
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
    int starty = startCoordy + 1;
    int endy = startCoordy + 1;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + distance;i++)
    {
      for (j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = cybio_max(i - startCoordx, j - startCoordy);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy>startCoordy + distance)
        endy = startCoordy + distance;
    }
  }
  if ( (sectorBmp & SECTOR_NN_BIT_MASK) !=0 )
  {
    int startx = startCoordx;
    int endx = startCoordx;
    int offset = 0;
    for (j = startCoordy + 1; j <= startCoordy + distance; j++)
    {
      for (i = startx; i <= endx; i++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = j - startCoordy;
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
    int starty = startCoordy + 1;
    int endy = startCoordy + 1;
    int offset = 0;
    for (i = startCoordx - 1; i >= startCoordx - distance ; i--)
    {
      for (j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = cybio_max(startCoordx - i, j - startCoordy);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy > startCoordy + distance)
        endy = startCoordy + distance;
    }
  }
  if ( (sectorBmp & SECTOR_WW_BIT_MASK) !=0 )
  {
    int starty = startCoordy;
    int endy = startCoordy;
    int offset = 0;
    for (i = startCoordx - 1; i >= startCoordx - distance; i--)
    {
      for (j = starty; j <= endy; j++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = startCoordx - i;
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
    int starty = startCoordy - 1;
    int endy = startCoordy - 1;
    int offset = 0;
    for (i = startCoordx - 1; i >= startCoordx - distance; i--)
    {
      for (j = starty; j >= endy; j--)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = cybio_max(startCoordx - i, startCoordy - j);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy < startCoordy - distance)
        endy = startCoordy - distance;
    }
  }
  if ( (sectorBmp & SECTOR_SS_BIT_MASK) !=0 )
  {
    int startx = startCoordx;
    int endx = startCoordx;
    int offset = 0;
    for (j = startCoordy - 1; j >= startCoordy - distance; j--)
    {
      for (i = startx; i <= endx; i++)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = startCoordy - j;
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
    int starty = startCoordy - 1;
    int endy = startCoordy - 1;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + distance; i++)
    {
      for (j = starty; j >= endy; j--)
      {
        if (nbFoundIds>=MAX_FOUND_ENTITIES)
          break;
        curCoord.x = i;
        curCoord.y = j;
        pCurEntity = findEntity(curCoord,layer);
        if ( (pCurEntity!=NULL) && (includeWater||(pCurEntity->getId() != ENTITY_ID_WATER)) )
        {
          tFoundIds[nbFoundIds].pEntity = pCurEntity;
          tFoundIds[nbFoundIds].distance = cybio_max(i - startCoordx, startCoordy - j);
          nbFoundIds++;
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy < startCoordy - distance)
        endy = startCoordy - distance;
    }
  }
  m_BiotopFoundIds.nbFoundIds = nbFoundIds;
  return (m_BiotopFoundIds);
}

//  find all entities betwen 2 range, on any layer except layer0 (under ground)
const BiotopFoundIds_t& CBiotop::findFarEntities(Point_t centerCoord, UCHAR sectorBmp, int rangeMin, int rangeMax)
{
  int nbFoundIds = 0;
  CBasicEntity*  pCurEntity = NULL;
  Point_t curCoord;
  int entityDist;
  int i, j;
  int startCoordx = (int)centerCoord.x;
  int startCoordy = (int)centerCoord.y;
  std::vector<FoundEntity_t>& tFoundIds = m_BiotopFoundIds.tFoundIds;

  if ( (sectorBmp & SECTOR_EE_BIT_MASK) !=0 )
  {
    int starty = startCoordy;
    int endy = startCoordy;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + rangeMax;i++)
    {
      entityDist = i - startCoordx;
      if (entityDist>=rangeMin)
      {
        for (j = starty; j <= endy; j++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            {
              if ( pCurEntity!=NULL ) 
              {
                tFoundIds[nbFoundIds].pEntity = pCurEntity;
                tFoundIds[nbFoundIds].distance = entityDist;
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
    int starty = startCoordy + 1;
    int endy = startCoordy + 1;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + rangeMax;i++)
    {
      for (j = starty; j <= endy; j++)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = cybio_max(i - startCoordx, j - startCoordy);
        if (entityDist>=rangeMin)
        {
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy > startCoordy + rangeMax)
        endy = startCoordy + rangeMax;
    }
  }
  if ( (sectorBmp & SECTOR_NN_BIT_MASK) !=0 )
  {
    int startx = startCoordx;
    int endx = startCoordx;
    int offset = 0;
    for (j = startCoordy + 1; j <= startCoordy + rangeMax;j++)
    {
      entityDist = j - startCoordy;
      if (entityDist>=rangeMin)
      {
        for (i = startx; i <= endx; i++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
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
    int starty = startCoordy + 1;
    int endy = startCoordy + 1;
    int offset = 0;
    for (i = startCoordx - 1; i >= startCoordx - rangeMax; i--)
    {
      for (j = starty; j <= endy; j++)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = cybio_max(startCoordx - i, j - startCoordy);
        if (entityDist>=rangeMin)
        {
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty += offset;
      endy += 2;
      if (endy > startCoordy + rangeMax)
        endy = startCoordy + rangeMax;
    }
  }
  if ( (sectorBmp & SECTOR_WW_BIT_MASK) !=0 )
  {
    int starty = startCoordy;
    int endy = startCoordy;
    int offset = 0;
    for (i = startCoordx - 1; i >= startCoordx - rangeMax; i--)
    {
      entityDist = startCoordx - i;
      if (entityDist>=rangeMin)
      {
        for (j = starty; j <= endy; j++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
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
    int starty = startCoordy - 1;
    int endy = startCoordy - 1;
    int offset = 0;
    for (i = startCoordx - 1; i >= startCoordx - rangeMax; i--)
    {
      for (j = starty; j >= endy; j--)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = cybio_max(startCoordx - i, startCoordy - j);
        if (entityDist>=rangeMin)
        {
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy < startCoordy - rangeMax)
        endy = startCoordy - rangeMax;
    }
  }
  if ( (sectorBmp & SECTOR_SS_BIT_MASK) !=0 )
  {
    int startx = startCoordx;
    int endx = startCoordx;
    int offset = 0;
    for (j = startCoordy - 1; j >= startCoordy - rangeMax; j--)
    {
      entityDist = startCoordy - j;
      if (entityDist>=rangeMin)
      {
        for (i = startx; i <= endx; i++)
        {
          curCoord.x = i;
          curCoord.y = j;
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
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
    int starty = startCoordy - 1;
    int endy = startCoordy - 1;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + rangeMax;i++)
    {
      for (j = starty; j >= endy; j--)
      {
        curCoord.x = i;
        curCoord.y = j;
        entityDist = cybio_max(i - startCoordx, startCoordy - j);
        if (entityDist>=rangeMin)
        {
          for (size_t layer = 1; layer < m_nbLayer; layer++)
          {
            if (nbFoundIds>=MAX_FOUND_ENTITIES)
              break;
            pCurEntity = findEntity(curCoord,layer);
            if ( pCurEntity!=NULL )
            {
              tFoundIds[nbFoundIds].pEntity = pCurEntity;
              tFoundIds[nbFoundIds].distance = entityDist;
              nbFoundIds++;
            }
          }
        }
      }
      // Enlarge y range 
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy < startCoordy - rangeMax)
        endy = startCoordy - rangeMax;
    }
  }
  m_BiotopFoundIds.nbFoundIds = nbFoundIds;
  return (m_BiotopFoundIds);
}

bool CBiotop::isCoordValidAndFree(Point_t coord, size_t layer)
{
  if ( (coord.x >= m_Dimension.x) || (coord.y >= m_Dimension.y) || (layer >= m_nbLayer)
     || (findEntity(coord, layer) != NULL) ) // water is consider as not free
  {
    return (false);
  }
  else 
  {
    return (true);
  }
}

bool CBiotop::isCoordValid(Point_t coord, size_t layer)
{
  if ( (coord.x < m_Dimension.x) && (coord.y < m_Dimension.y) && (layer < m_nbLayer) )
  {
    return (true);
  }
  else 
  {
    return (false);
  }
}

CBasicEntity* CBiotop::findTopLevelEntity(Point_t searchCoord) 
{
  CBasicEntity* pFoundEntity = NULL;
   
  for (int layer = (int)m_nbLayer - 1; layer >= 0; layer--)
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

  // Force entity action
  if (pEntity != NULL)
  {
    resu = pEntity->forceNextAction(myChoice);
  }

  // Loop on all other entities for basic action  
  /*for (size_t i=0; i<getNbOfAnimals(); i++)    
  {
    pEntity = m_tEntity[i];
    if ((pEntity != NULL)&&(pEntity->getId()!=idEntity))
    {
      pEntity->nextSecond();
    }
  }*/

  // Trigger measurement
  triggerMeasuresNextSecond();

  return(resu);
}

choiceIndType CBiotop::predictEntityAction(entityIdType idEntity)
{
  choiceIndType resu = invalidIndex;
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
  m_BioTime.seconds++;
  logCpuMarkerStart(BIOTOP_CPUMARKER_TOTAL);

  if (m_BioTime.seconds>=3600)
  {
    // Next hour for biotop
    nextHour();

    // loop from top to bottom to clear entities each hour
    for (i = (int)getNbOfEntities() - 1; i >= 0; i--)   
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
  triggerMeasuresNextSecond();

  logCpuMarkerEnd(BIOTOP_CPUMARKER_TOTAL);
}

void CBiotop::triggerMeasuresNextSecond(void)
{
  bool isValid;
  for (auto pMeasure : m_tMeasures)
  {
    isValid = pMeasure->NextSecond();
    if (!isValid)
    {
      pMeasure->StopMeasurement();
    }
  }
}

void CBiotop::nextHour(void)
{
  for (size_t i=0;i<m_Dimension.x;i++)
  {
    for (size_t j=0;j<m_Dimension.y;j++)
    {
      for (size_t odor=0;odor<ODOR_NUMBER_TYPE;odor++)
      {
        if (m_tBioSquare[i][j].odorTrace[odor]<0.01)
          m_tBioSquare[i][j].odorTrace[odor] = 0;
        else
          m_tBioSquare[i][j].odorTrace[odor] = m_tBioSquare[i][j].odorTrace[odor]/2.0;
      }
    }
  }

  // Random entities generation
  for (size_t ind = 0; ind < MAX_NUMBER_RANDOM_ENTITIES; ind++)
  {
    BiotopRandomEntitiyGeneration_t& randomEntity = m_tRandomEntitiesGeneration[ind];
    if (randomEntity.pModelEntity != NULL)
    {
      double coverRate = randomEntity.intensity + getRandInt(10);
      double fertilityFactor = 100;
      double periodicityFactor = 100;
      if (randomEntity.IsProportionalToFertility)
      {
        fertilityFactor = getFertility({1, 1});
        coverRate = coverRate * fertilityFactor / 100.0;
      }
      if (randomEntity.avaragePeriodicity > 0)
      {
        periodicityFactor = 100.0 / ((double)(randomEntity.avaragePeriodicity) * 24.0);
      }
      if (testChance(periodicityFactor, fertilityFactor))
      {
        spawnEntitiesRandomly(randomEntity.pModelEntity, cybio_round(coverRate));
      }
    }
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

    // Save geomap with monitored specie populations
    for (auto pGeoMap : m_tGeoMapSpecies)
    {
      pGeoMap->MemorizePopulationMap(m_BioTime.days);
    }

  }
}


BiotopTime_t CBiotop::getBiotopTime(void)
{
  return (m_BioTime);
}

void CBiotop::setBiotopTime(int seconds, int hours, int days, int years)
{
  m_BioTime.seconds = seconds;
  m_BioTime.hours = hours;
  m_BioTime.days = days;
  m_BioTime.years = years;
}

//===========================================================================
// Grid management
//===========================================================================
void CBiotop::buildGrid(size_t dimX, size_t dimY, size_t dimZ)
{
  // Build 3D dynamic table
  m_tBioSquare = new BiotopSquare_t*[dimX];
  m_tBioGrid   = new BiotopCube_t**[dimX];
  ASSERT( m_tBioGrid != NULL );
  for (size_t i=0;i<dimX;i++)
  {
    m_tBioSquare[i] = new BiotopSquare_t[dimY];
    m_tBioGrid[i]   = new BiotopCube_t*[dimY];
    ASSERT( m_tBioGrid[i] != NULL );
    for (size_t j=0;j<dimY;j++)
    {
      m_tBioGrid[i][j] = new BiotopCube_t[dimZ];
      ASSERT( m_tBioGrid[i][j] != NULL );
    }
  }
}

void CBiotop::deleteGrid(void)
{
  // Delete 3D dynamic table 
  for (size_t i=0;i<m_Dimension.x;i++)
  {
    for (size_t j=0;j<m_Dimension.y;j++)
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
  size_t i, j;

  // Put Ground Everywhere:
  for (i=0;i<m_Dimension.x;i++)
  {
    for (j=0;j<m_Dimension.y;j++)
    {
      m_tBioGrid[i][j][0].layerType = LAYER_UNDER_GROUND;
      if (testChance(30))
      {
        m_tBioGrid[i][j][1].layerType = LAYER_GLOBAL_ROCK;
      }
      else
      {
        m_tBioGrid[i][j][1].layerType = LAYER_OVER_GROUND;
      }
      m_tBioGrid[i][j][2].layerType = LAYER_OVER_GROUND;
    }
  }

  // Add a River in the middle
  for (j = 1; j < m_Dimension.y / 2; j++)
  {
    m_tBioGrid[m_Dimension.x / 2][j][0].layerType = LAYER_UNDER_WATER;
    m_tBioGrid[m_Dimension.x / 2][j][1].layerType = LAYER_OVER_WATER;
    m_tBioGrid[m_Dimension.x / 2][j][2].layerType = LAYER_IN_AIR;
    m_tBioGrid[m_Dimension.x / 2 + 1][j][0].layerType = LAYER_UNDER_GROUND;
    m_tBioGrid[m_Dimension.x / 2 + 1][j][1].layerType = LAYER_OVER_WET_GROUND;
    m_tBioGrid[m_Dimension.x / 2 + 1][j][2].layerType = LAYER_OVER_GROUND;
    m_tBioGrid[m_Dimension.x / 2 - 1][j][0].layerType = LAYER_UNDER_GROUND;
    m_tBioGrid[m_Dimension.x / 2 - 1][j][1].layerType = LAYER_OVER_WET_GROUND;
    m_tBioGrid[m_Dimension.x / 2 - 1][j][2].layerType = LAYER_OVER_GROUND;
  }

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
  for (size_t i=0;i<m_Dimension.x;i++)
  {
    for (size_t j=0;j<m_Dimension.y;j++)
    {
      m_tBioSquare[i][j].altitude = 0;
    }
  }
}

void CBiotop::initGridEntity(void)
{
  // 
  for (size_t i=0;i<m_Dimension.x;i++)
  {
    for (size_t j=0;j<m_Dimension.y;j++)
    {
      for (size_t k=0;k<m_nbLayer;k++)
      {
        m_tBioGrid[i][j][k].pEntity = NULL;
      }

      // If water present: set water global entity
      if ( (m_tBioGrid[i][j][1].layerType == LAYER_OVER_WATER) || (m_tBioGrid[i][j][1].layerType == LAYER_OVER_WET_GROUND) )
      {
        //m_tBioGrid[i][j][0].pEntity = m_pWaterGlobalEntity;
        m_tBioGrid[i][j][1].pEntity = m_pWaterGlobalEntity;
      }
      else if (m_tBioGrid[i][j][1].layerType == LAYER_GLOBAL_GRASS) 
      {
        m_tBioGrid[i][j][1].pEntity = m_pGrassGlobalEntity;
      }
      else if (m_tBioGrid[i][j][1].layerType == LAYER_GLOBAL_ROCK)
      {
        m_tBioGrid[i][j][1].pEntity = m_pRockGlobalEntity;
      }

      // reset odor traces
      for (size_t odor=0; odor<ODOR_NUMBER_TYPE; odor++)
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
  size_t tmpLayer;
   
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
  return cybio_max(abs((int)gridCoord1.x - (int)gridCoord2.x), abs((int)gridCoord1.y - (int)gridCoord2.y));
}

void CBiotop::updateGridAllEntities(void)
{
  Point_t tmpCoord;
  size_t tmpLayer;

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

  for (auto pMeasure : m_tMeasures)
  {
    if (pMeasure->GetId() == id)
      foundMeas = pMeasure;
  }

  return (foundMeas);
}

CMeasure* CBiotop::getMeasureByIndex(size_t index)
{
  return (m_tMeasures[index]);
}

size_t CBiotop::getNbOfMeasures(void)
{
  return (m_tMeasures.size());
}

int CBiotop::getUnusedMeasureId(int maxMeasureId)
{
  for (int i = 0; i < maxMeasureId; i++)
  {
    if (getMeasureById(i) == NULL)
      return i;
  }
  return -1;
}

void CBiotop::replaceMeasure(int id, CMeasure* pMeasure)
{
  if (pMeasure==NULL)
    return;

  CMeasure* foundMeas = NULL;
  size_t foundIndex = invalidIndex;

  for (size_t i=0; i<m_tMeasures.size(); i++) 
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
  for (size_t i=0; i<m_tMeasures.size(); i++) 
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
  for (size_t i = 0; i<m_tMeasures.size(); i++)
  {
    m_tMeasures[i]->saveInFile(fileNameWithPath);
  }
}

bool CBiotop::addGeoMapSpeciePopulation(std::string specieName)
{
  CGeoMapPopulation* pGeoMapPopulation = new CGeoMapPopulation(this, getDimension(), specieName);
  m_tGeoMapSpecies.push_back(pGeoMapPopulation);
  // Store initial population map
  pGeoMapPopulation->MemorizePopulationMap(m_BioTime.days);
  return true;
}

size_t CBiotop::getNbOfGeoMapSpecie(void)
{
  return (m_tGeoMapSpecies.size());
}

CGeoMapPopulation* CBiotop::getGeoMapSpecieByIndex(size_t index)
{
  CGeoMapPopulation* pGeoMap = NULL;
  if (index < m_tGeoMapSpecies.size())
    pGeoMap = m_tGeoMapSpecies[index];
  return (pGeoMap);
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

size_t CBiotop::getNbOfBiotopEvents(void)
{
  return m_tEvents.size();
}

BiotopEvent_t CBiotop::getBiotopEvent(size_t index)
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
  int i;
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
    pElement->SetAttribute( XML_ATTR_SIZE_X,     (int)m_Dimension.x);
    pElement->SetAttribute( XML_ATTR_SIZE_Y,     (int)m_Dimension.y);
    pElement->SetAttribute( XML_ATTR_SIZE_LAYER, (int)m_nbLayer);
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
      pElement->SetDoubleAttribute(XML_ATTR_PHASE, getParameter(i)->getCurrentPhase());
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
    for (size_t k=0; k<m_nbLayer; k++)
    {
      TiXmlElement newLayer(XML_NODE_LAYER);
      pNodeChild = pNode->InsertEndChild(newLayer);
      if (pNodeChild != NULL) 
      {
        string strData = "";
        for (size_t cx = 0; cx < m_Dimension.x; cx++)
        {
          for (size_t cy = 0; cy < m_Dimension.y; cy++)
          {
            strData += FormatString("%02u",(BYTE)m_tBioGrid[cx][cy][k].layerType); 
          }
        }
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_INDEX, (int)k);
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
        pElement->SetAttribute(XML_ATTR_COORD_X,   (int)pCurEntity->getStepCoord().x);
        pElement->SetAttribute(XML_ATTR_COORD_Y,   (int)pCurEntity->getStepCoord().y);
        pElement->SetAttribute(XML_ATTR_LAYER,     (int)pCurEntity->getLayer());
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
    CYBIOCORE_LOG("BIOTOP - Error loading biotop file: %s %s\n", pathName.c_str(), fileName.c_str());
    return false;
  }

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Load: file %s path %s\n", fileName.c_str(), pathName.c_str());
  loadFromXmlFile(&xmlDoc, pathName);
  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - File load complete\n");
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
    double paramPhase;
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
          {
            pParam->setVal(paramVal);
            if (pElement->QueryDoubleAttribute(XML_ATTR_PHASE, &paramPhase) != TIXML_NO_ATTRIBUTE)
            {
              pParam->setCurrentPhase(paramPhase);
            }
          }
        }
      }
      pNode = pNode->NextSibling();
    }

    // Load Grid data
    size_t i,j;
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
    int stepCoordX, stepCoordY;
    Point_t stepCoord;
    int layerInFile;
    size_t layer;
    int direction;
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
        if ( pElement->QueryIntAttribute(XML_ATTR_COORD_X,  &stepCoordX) == TIXML_NO_ATTRIBUTE)
          stepCoord.x = invalidCoord;
        else
          stepCoord.x = stepCoordX;

        if ( pElement->QueryIntAttribute(XML_ATTR_COORD_Y,  &stepCoordY) == TIXML_NO_ATTRIBUTE)
          stepCoord.y = invalidCoord;
        else
          stepCoord.y = stepCoordY;

        if ( pElement->QueryIntAttribute(XML_ATTR_LAYER,  &layerInFile) == TIXML_NO_ATTRIBUTE)
          layer = invalidCoord;
        else
          layer = layerInFile;

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
              pEntity->jumpToStepCoord(stepCoord, false);
              pEntity->checkIfhasMovedAndClear();
              pEntity->checkIfhasChangedAndClear();
            }
          }
          else
          { 
            resuId = createAndAddEntity(entityFileName, pathNameForEntities, gridCoord);
            if (resuId != ENTITY_ID_INVALID)
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
bool CBiotop::addEntitySpawner(size_t index, string entityFileName, string pathName, int intensityRate, int avaragePeriod, bool isProportionalToFertility)
{
  CBasicEntity* pEntity = createEntity(entityFileName, pathName);
  addEntitySpawner(index, pEntity, intensityRate, avaragePeriod, isProportionalToFertility);
  if (index < m_tRandomEntitiesGeneration.size())
  {
    m_tRandomEntitiesGeneration[index].entityFileName = entityFileName;
    m_tRandomEntitiesGeneration[index].entityPathName = pathName;
    return true;
  }
  return false;
}

bool CBiotop::addEntitySpawner(size_t index, CBasicEntity* pModelEntity, int intensityRate, int avaragePeriod, bool isProportionalToFertility)
{
  if (index < m_tRandomEntitiesGeneration.size())
  {
    if (m_tRandomEntitiesGeneration[index].pModelEntity != NULL)
    {
      //clean previous entity
      delete m_tRandomEntitiesGeneration[index].pModelEntity;
    }
    m_tRandomEntitiesGeneration[index].entityFileName = "";
    m_tRandomEntitiesGeneration[index].entityPathName = "";
    m_tRandomEntitiesGeneration[index].pModelEntity = pModelEntity;
    m_tRandomEntitiesGeneration[index].intensity = intensityRate;
    m_tRandomEntitiesGeneration[index].avaragePeriodicity = avaragePeriod;
    m_tRandomEntitiesGeneration[index].IsProportionalToFertility = isProportionalToFertility;
    return true;
  }
  else
  {
    return false;
  }
}

void CBiotop::spawnEntitiesRandomly(CBasicEntity* pModelEntity, int coverRate)
{
  Point_t coord;
  if (pModelEntity != NULL)
  {
    size_t nbEntities = m_Dimension.x * m_Dimension.y * coverRate / 100000;
    for (size_t i = 0; i < nbEntities; i++)
    {
      coord.x = getRandInt(m_Dimension.x) + 2;
      coord.y = getRandInt(m_Dimension.y) + 2;
      CBasicEntity* pNewEntity = createCloneEntity(pModelEntity);
      this->addEntity(pNewEntity, coord, true, pModelEntity->getLayer());
    }
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Spread entities randomly : %d %ss\n", nbEntities, pModelEntity->getLabel().c_str());
  }
  else
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - ERROR: cannot spread entities\n");
  }
}

//===========================================================================
// CPU Marker
//===========================================================================

void CBiotop::resetCpuMarker()
{
  for (size_t i = 0; i < BIOTOP_CPUMARKER_LAST; i++)
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

double CBiotop::getTemperature(Point_t coord, size_t layer)
{
  double computedTemperature = m_pTemperature->getVal();
  CBasicEntity* pCurEntity = NULL;
  const BiotopFoundIds_t& biotopFoundIds = findEntities(coord, 1, true);
  const std::vector<FoundEntity_t>& tFoundIds = biotopFoundIds.tFoundIds;

  // Give bonus malus on temperature according to entities around
  for (size_t ind = 0; ind < biotopFoundIds.nbFoundIds; ind++)
  {
    pCurEntity = tFoundIds[ind].pEntity;
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

LayerType_e CBiotop::getLayerType(Point_t coord, size_t layer)
{
  LayerType_e resu = LAYER_UNDER_GROUND; // default value
  if (isCoordValid(coord,layer))
  {
    resu =  m_tBioGrid[coord.x][coord.y][layer].layerType;
  }

  return (resu);
}

size_t CBiotop::getNbLayer(void)
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

CGenericParam* CBiotop::getParameter(size_t id)
{
  if (id > m_tParam.size())
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
  for (auto pParam : m_tParam)
  {
    if (pParam->getLabel() == paramName)
      return (pParam);
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

  const BiotopFoundIds_t& biotopFoundIds = findEntities(coord, range);
  const std::vector<FoundEntity_t>& tFoundIds = biotopFoundIds.tFoundIds;

  for (size_t ind = 0; ind < biotopFoundIds.nbFoundIds; ind++)
  {
    pCurEntity = tFoundIds[ind].pEntity;
    if ((pCurEntity!=NULL) && (pCurEntity->getId() != excludedEntityId))
    {
      for (size_t odor=0; odor<NUMBER_ODORS; odor++)
      {
        // Add odor for entities
        if (pCurEntity->getOdor() == (ODOR_FIRST_TYPE+odor) )
        {
          odorLevel[odor] += MAX_SENSOR_VAL / ((double)tFoundIds[ind].distance + 2); // 1/R
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

BiotopRandomEntitiyGeneration_t& CBiotop::getRandomEntitiyGeneration(int index)
{
  return (m_tRandomEntitiesGeneration[index]);
}

size_t CBiotop::getNumberOfRandomEntitiyGeneration()
{
  return m_tRandomEntitiesGeneration.size();
}

void CBiotop::SetColorizeSearchMode(bool bColorizeSearch)
{
  m_bColorizeSearch = bColorizeSearch;
}