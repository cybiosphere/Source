/*
https://github.com/cybiosphere
copyright (c) 2005-2014 Frederic RIBERT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions

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
#include <omp.h>
#include "CEntityFactory.h"

RelativePos_t vectorDirection[8] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

//===========================================================================
// Constructors / Destructors
//===========================================================================
CBiotop::CBiotop(size_t dimX, size_t dimY, size_t dimZ, size_t nbOmpThreads, string logFileName)
{
  // Initialisation
  m_Label = "Default biotop";
  m_Dimension.x  = dimX;
  m_Dimension.y  = dimY;
  m_nbLayer      = dimZ;
  setGlobalGridDimension(dimX, dimY);
  setGlobalGridCoordOffset({ 0, 0 });
  m_IndexLastAnimal = 0;
  setBiotopTime(0, 12, 0, 0);
  m_NextHourTimeOffset = 0;
  m_WindDirection = 1;
  m_WindStrenght  = 1; // 0,1 or 2
  m_DefaultFilePath = get_working_path();

  m_tParam.resize(0);
  m_pFertilityRate = new CCyclicParam(10, 50, DEFAULT_SEASON_PERIOD_IN_DAYS * NUMBER_HOURS_PER_DAY, "Avarage fertility", PARAM_ENVIRONMENT);
  m_tParam.push_back(m_pFertilityRate);
  m_pSunlightRate = new CCyclicParam(0, 100, NUMBER_HOURS_PER_DAY, "Sunlight", PARAM_ENVIRONMENT);
  m_tParam.push_back(m_pSunlightRate);
  m_pRadioactivity = new CCustomParam(0, 1, 1, 100, "Radioactivity", PARAM_ENVIRONMENT);
  m_tParam.push_back(m_pRadioactivity);
  m_pTemperature = new CCyclicParam(20, 26, DEFAULT_SEASON_PERIOD_IN_DAYS * NUMBER_HOURS_PER_DAY, "Avarage Temperature", PARAM_ENVIRONMENT); // TBD use global min/max temperature
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

  m_NbOmpThreads = nbOmpThreads;
  m_BiotopFoundIdsArray.resize(m_NbOmpThreads);
  for (size_t i = 0; i < m_BiotopFoundIdsArray.size(); i++)
  {
    m_BiotopFoundIdsArray[i].nbFoundIds = 0;
  }

  resetCpuMarker();

  m_tRandomEntitiesGeneration.resize(MAX_NUMBER_RANDOM_ENTITIES);
  for (size_t i = 0; i < m_tRandomEntitiesGeneration.size(); i++)
  {
    m_tRandomEntitiesGeneration[i].pModelEntity = NULL;
  }
  // Set rain as default random entity
  addEntitySpawner(0, "water_puddle.xml", "../DataScriptMammal/", 50, 5, true);

  m_bColorizeSearch = false;

  CYBIOCORE_LOG_INIT(logFileName.c_str());
  CYBIOCORE_LOG("BIOTOP - Biotop created. Number OMP threads is %u\n", m_NbOmpThreads);
}

CBiotop::~CBiotop()
{
  clearAllElements();
  deleteAllParameters();
  delete m_pWaterGlobalEntity;
  delete m_pGrassGlobalEntity;
  delete m_pRockGlobalEntity;
  CYBIOCORE_LOG_CLOSE;
}

void CBiotop::clearAllElements(void)
{
  deleteAllEntities();
  deleteAllMeasures();
  deleteAllPopulationMaps();
  deleteGrid();
  clearEventMaps();
}

void CBiotop::resetResizeAndAddDefaultEntities(size_t dimX, size_t dimY)
{
  clearAllElements();
  m_Dimension.x = dimX;
  m_Dimension.y = dimY;
  m_nbLayer = 3;
  setGlobalGridDimension(dimX, dimY);
  setGlobalGridCoordOffset({ 0, 0 });
  buildGrid(m_Dimension.x, m_Dimension.y, m_nbLayer);
  setDefaultMapAndEntities();
}

void CBiotop::setDefaultMapAndEntities(void)
{
  initGridEntity();
  initGridDefaultLayerType();
  initGridDefaultAltitude();
  setDefaultEntitiesForNewBiotop();
}


CBiotop* CBiotop::extractNewBiotopFromArea(const Point_t& startCoord, size_t dimX, size_t dimY)
{
  if ((startCoord.x > m_Dimension.x) || (startCoord.y > m_Dimension.y)
    || ((startCoord.x + dimX) > m_Dimension.x) || ((startCoord.y + dimY) > m_Dimension.y))
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("ERROR  - Try to exctract Arean with invalid coord startX=%u sizeX=%u\n", startCoord.x, dimX);
    return NULL;
  }

  CBiotop* pExctactedBiotop = new CBiotop(dimX, dimY, m_nbLayer, m_NbOmpThreads, "CybioCoreExctracted.log");

  if (pExctactedBiotop == NULL)
    return NULL;

  for (size_t i = 0; i < dimX; i++)
  {
    for (size_t j = 0; j < dimY; j++)
    {
      for (size_t layerIndex = 0; layerIndex < m_nbLayer; layerIndex++)
      {
        pExctactedBiotop->m_tBioGrid[i][j][layerIndex].layerType = m_tBioGrid[i + startCoord.x][j + startCoord.y][layerIndex].layerType;
        pExctactedBiotop->m_tBioGrid[i][j][layerIndex].pEntity = NULL;
      }
    }
  }

  initGridEntity();

  pExctactedBiotop->setBiotopTime(getBiotopTime().seconds, getBiotopTime().hours, getBiotopTime().days, getBiotopTime().years);
  pExctactedBiotop->setGlobalGridDimension(m_Dimension.x, m_Dimension.y);
  pExctactedBiotop->setGlobalGridCoordOffset(startCoord);
  // TODO: copy climate, wind, measures?, spwners?....

  // Copy entities
  CBasicEntity* pCurEntity{NULL};
  CBasicEntity* pCloneEntity{ NULL };
  Point_t curCoord{ invalidCoord, invalidCoord };
  Point_t newStepCoord{ invalidCoord, invalidCoord };
  for (size_t i = 0; i < getNbOfEntities(); i++)
  {
    pCurEntity = getEntityByIndex(i);
    if (pCurEntity == NULL)
    {
      return NULL;
    }
    
    curCoord = pCurEntity->getGridCoord();
    if ((curCoord.x >= startCoord.x) && (curCoord.y >= startCoord.y)
      && (curCoord.x < (startCoord.x + dimX)) && (curCoord.y < (startCoord.y + dimY)))
    {
      newStepCoord = pCurEntity->getGlobalStepCoord();
      pCloneEntity = CEntityFactory::createCloneEntity(pCurEntity);
      pExctactedBiotop->addEntityWithPresetId(pCurEntity->getId(), pCloneEntity, newStepCoord, true, pCurEntity->getLayer());
    }
  }

  return pExctactedBiotop;
}

//===========================================================================
// Entities management
//===========================================================================

bool CBiotop::addEntity(CBasicEntity* pEntity, Point_t globalGridCoord, size_t layer)
{
  if ((pEntity == NULL) || !isGlobalGridCoordValidAndFree(globalGridCoord, layer))
  {
    //CYBIOCORE_LOG_TIME(m_BioTime);
    //CYBIOCORE_LOG("ERROR  - addEntity Failed NULL entity or coord not free\n");
    return false;
  }

  if (pEntity->isAnimal())
  {
    if (getNbOfAnimals() >= MAX_NUMBER_ANIMALS)
    {
      CYBIOCORE_LOG_TIME(m_BioTime);
      CYBIOCORE_LOG("ERROR  - Too many animals. Cannot add entity name %s\n", pEntity->getLabel().c_str());
      return false;
    }
  }
  else if (getNbOfEntities() >= MAX_NUMBER_NON_ANIMAL_ENTITIES)
  {
    return false;
  }

  // Set new id
  m_IdLastEntity++;
  pEntity->setId(m_IdLastEntity);

  // Attach it to biotop
  if (pEntity->attachToBiotop(this, getStepCoordFromGridCoord(globalGridCoord), layer) == false)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("ERROR  - attachToBiotop failed for entity name %s\n", pEntity->getLabel().c_str());
    return false;
  }

  if (pEntity->isAnimal())
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

  markEntityWithGene(pEntity);

  return true;
}

bool CBiotop::addEntityWithPresetId(entityIdType idEntity, CBasicEntity* pEntity, Point_t globalStepCoord, bool chooseLayer, size_t newLayer)
{
  size_t layer;
  if (chooseLayer)
    layer = newLayer;
  else
    layer = pEntity->getLayer();

  if (getNbOfEntities() > MAX_NUMBER_ENTITIES)
    return false;

  if ( !isGlobalGridCoordValidAndFree(getGridCoordFromStepCoord(globalStepCoord), layer) )
    return false;

  //check ID:
  if (getEntityById(idEntity) != NULL)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("ERROR  - getEntityById failed for entity id %lu\n", idEntity);
    return false;
  }

  pEntity->setId(idEntity);

  // Attach it to biotop
  if (pEntity->attachToBiotop(this, globalStepCoord, layer) == false)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("ERROR  - attachToBiotop failed for entity name %s\n", pEntity->getLabel().c_str());
    return false;
  }

  if (pEntity->isAnimal())
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

  markEntityWithGene(pEntity);

  return true;
}

CBasicEntity* CBiotop::createAndAddEntity(string name, Point_t globalGridCoord, size_t layer, CGenome* pGenome)
{
  // Check coords
  if (!isGlobalGridCoordValidAndFree(globalGridCoord, layer))
     return NULL;

  // Create entity
  CBasicEntity* pNewEntity = CEntityFactory::createEntity(name,pGenome);
  if (pNewEntity == NULL)
    return NULL;

  /*if (pNewEntity->isLiving())
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - New entity from genome : specie %s name %s\n", pNewEntity->getSpecieName().c_str(), name.c_str());
  }*/

  // Put it in the biotop (with check coord);
  if (addEntity(pNewEntity, globalGridCoord, layer) == false)
  {
    delete pNewEntity;
    return NULL;
  }

  return pNewEntity;
}

CBasicEntity* CBiotop::createAndAddEntity(string fileName, string pathName, Point_t globalGridCoord, size_t layer)
{
  string fileNameWithPath = pathName + fileName;
  TiXmlDocument *pXmlDoc = new TiXmlDocument(fileNameWithPath);
  if (!pXmlDoc->LoadFile())
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Error loading entity file: %s\n", fileNameWithPath.c_str());
    delete pXmlDoc;
    return NULL;
  }
  CBasicEntity* pNewEntity = createAndAddEntity(pXmlDoc, globalGridCoord, layer);
  delete pXmlDoc;
  return pNewEntity;
}

CBasicEntity* CBiotop::createAndAddEntity(TiXmlDocument *pXmlDoc, Point_t globalGridCoord, size_t layer)
{
  // Create entity
  CBasicEntity* pNewEntity = CEntityFactory::createEntity(pXmlDoc);
  if (pNewEntity == NULL)
    return NULL;

  // Check coords
  if (layer == invalidCoord)
    layer = pNewEntity->getDefaultLayer();

  if (!isGlobalGridCoordValidAndFree(globalGridCoord, layer))
  {
    delete pNewEntity;
    return NULL;
  }
  
  if (pNewEntity->isLiving())
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - New entity: specie %s name %s\n", pNewEntity->getSpecieName().c_str(), pNewEntity->getLabel().c_str());
  }

  // Put it in the biotop (with check coord);
  if (addEntity(pNewEntity, globalGridCoord, layer) == false)
  {
    delete pNewEntity;
    return NULL;
  }

  return pNewEntity;
}

CBasicEntity* CBiotop::createAndAddCloneEntity(entityIdType idModelEntity, Point_t cloneGlobalCoord, size_t cloneLayer, string cloneName)
{
  CBasicEntity* pModelEntity = getEntityById(idModelEntity);
  CBasicEntity* pNewEntity = NULL;

  size_t layer = cloneLayer;
  string name = cloneName;

  if (pModelEntity == NULL)
    return NULL;

  if (name == "")
    name = pModelEntity->getLabel();

  // Check coords
  if (!isGlobalGridCoordValidAndFree(cloneGlobalCoord, layer))
     return NULL;

  pNewEntity = CEntityFactory::createCloneEntity(pModelEntity);

  if (pNewEntity==NULL)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Error copy clone Entity : %s\n", pModelEntity->getLabel().c_str());
    return NULL;
  }

  // Put it in the biotop (with check coord);
  if (addEntity(pNewEntity, cloneGlobalCoord, layer) == false)
  {
    delete pNewEntity;
    return NULL;
  }

  // Desynchronize clone from model using Id
  pNewEntity->setHourCounter(pNewEntity->getId() % NUMBER_HOURS_PER_DAY);

  return pNewEntity;
}

bool CBiotop::resetEntityGenome(entityIdType idEntity, CGenome* pNewEntityGenome)
{
  CBasicEntity* pOldEntity = getEntityById(idEntity);

  if (pOldEntity == NULL)
    return false;

  // Keep old entity data
  string oldLabel = pOldEntity->getLabel();
  Point_t oldStepCoord = pOldEntity->getGlobalStepCoord();
  size_t oldLayer = pOldEntity->getLayer();
  entityIdType oldId = pOldEntity->getId();

  // Destroy Old entity
  size_t prevIndex = deleteEntity(pOldEntity, false);

  // Create new entity
  CBasicEntity* pNewEntity = CEntityFactory::createEntity(oldLabel,pNewEntityGenome);
  if (pNewEntity==NULL)
    return false;

  // Set old id
  pNewEntity->setId(oldId);

  // Attach it to biotop
  if (pNewEntity->attachToBiotop(this, oldStepCoord, oldLayer) == false)
    return false;

  // Add it to list
  m_tEntity.insert(m_tEntity.begin() + prevIndex, 1, pNewEntity);
  if (pNewEntity->isAnimal())
  {
    m_IndexLastAnimal++;
  }

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Entity genome updated : specie %s name %s\n", pNewEntity->getSpecieName().c_str(), pNewEntity->getLabel().c_str());

  // Enter 1st day and change LifeStage
  pNewEntity->nextDay();
  addBiotopEvent(BIOTOP_EVENT_ENTITY_MODIFIED, pNewEntity);

  return true;
}

bool CBiotop::replaceEntityByAnother(entityIdType idEntity, CBasicEntity* pNewEntity)
{
  CBasicEntity* pOldEntity = getEntityById(idEntity);

  if ((pOldEntity==NULL) || (pNewEntity==NULL))
    return false;

  // Keep old entity data
  Point_t oldCoord = pOldEntity->getGlobalGridCoord();
  size_t oldLayer    = pOldEntity->getLayer();
  entityIdType oldId = pOldEntity->getId();

  // Destroy Old entity
  pOldEntity->autoRemove(false);

  addEntity(pNewEntity, oldCoord, oldLayer);
  pNewEntity->setId(oldId);

  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - Entity full update : specie %s name %s coordX %d\n", pNewEntity->getSpecieName().c_str(), pNewEntity->getLabel().c_str(), oldCoord.x);
  addBiotopEvent(BIOTOP_EVENT_ENTITY_MODIFIED, pNewEntity);

  return true;
}

size_t CBiotop::deleteEntity(CBasicEntity* pEntity, bool displayLog)
{
  if (pEntity == NULL)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - ERROR Try to remove NULL entity\n");
    return invalidIndex;
  }
  if (isCoordValid(pEntity->getGridCoord(), pEntity->getLayer()))
  {
    m_tBioGrid[pEntity->getGridCoord().x][pEntity->getGridCoord().y][pEntity->getLayer()].pEntity = NULL;
  }
  for (size_t i = 0; i < getNbOfEntities(); i++)
  {
    if (m_tEntity[i] == pEntity)
    {
      if (pEntity->isAnimal())
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
      return i;
    }
  }
  // If not found
  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - ERROR Try to remove entity not found\n");
  return invalidIndex;
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
}

void CBiotop::deleteAllPopulationMaps(void)
{
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
    printf("ID=%3d Direction=%3u \n", pEntity->getId(), pEntity->getDirection());
  }
}

void CBiotop::setDefaultEntitiesForNewBiotop(void)
{
  // Seed the random generator
  // srand( (unsigned)time(NULL) );

  Point_t coord{ 0, 0 };

  buildWoodenFence({ 1, 1 }, { 1, m_Dimension.y - 2 });
  buildWoodenFence({ 1, 1 }, { m_Dimension.x - 2, 1 });
  buildWoodenFence({ 1, m_Dimension.y - 2 }, { m_Dimension.x - 1, m_Dimension.y - 2 });
  buildWoodenFence({ m_Dimension.x - 2, 1 }, { m_Dimension.x - 2, m_Dimension.y - 1 });

  size_t i;
  for (i = 0; i < 2; i++)
  {
    coord.x = 2 + getRandInt(60);
    coord.y = 2 + getRandInt(40);
    buildRock(coord);
  }
}

void CBiotop::buildWoodenFence(const Point_t& startCoord, const Point_t& endCoord)
{
  std::string fenceFileName = "wooden_fence.xml";
  std::string pathName = m_DefaultFilePath + "/../DataScriptMammal/";
  Point_t coord;
  CBasicEntity* pEntity;
  if (startCoord.x == endCoord.x)
  {
    coord.x = startCoord.x;
    for (size_t y = std::min(startCoord.y, endCoord.y); y < std::max(startCoord.y, endCoord.y); y++)
    {
      coord.y = y;
      pEntity = createAndAddEntity(fenceFileName, pathName, coord);
      if (pEntity)
      {
        pEntity->setDirection(2);
      }
    }
  }
  else if (startCoord.y == endCoord.y)
  {
    coord.y = startCoord.y;
    for (size_t x = std::min(startCoord.x, endCoord.x); x < std::max(startCoord.x, endCoord.x); x++)
    {
      coord.x = x;
      pEntity = createAndAddEntity(fenceFileName, pathName, coord);
      if (pEntity)
      {
        pEntity->setDirection(0);
      }
    }
  }
}

void CBiotop::buildRock(const Point_t& coord)
{
  std::string rockFileName = "rock.xml";
  std::string pathName = m_DefaultFilePath + "/../DataScriptMammal/";
  CBasicEntity* pEntity = createAndAddEntity(rockFileName, pathName, coord);
}

size_t CBiotop::getNbOfEntities()
{
  return m_tEntity.size();
}

size_t CBiotop::getNbOfAnimals()
{
  return m_IndexLastAnimal;
}

size_t CBiotop::getNbOfLivingAnimals()
{
  CBasicEntity* pEntity;
  size_t i;
  size_t nbLivingAnimals = 0;
  for (i = 0; i < getNbOfAnimals(); i++)
  {
    pEntity = m_tEntity[i];
    if ((pEntity != NULL) && pEntity->isAlive())
    {
      nbLivingAnimals++;
    }
  }
  return nbLivingAnimals;
}

size_t CBiotop::getNbOfVegetals()
{
  size_t tempCount = 0;
  for (CBasicEntity* pEntity : m_tEntity)
  {
    if (pEntity->isVegetal() && (pEntity->getId() > 0))
      tempCount++;
  }
  return tempCount;
}

size_t CBiotop::getNbOfMinerals()
{
  size_t tempCount = 0;
  for (CBasicEntity* pEntity : m_tEntity)
  {
    if (pEntity->isMineral() && (pEntity->getId() > 0))
      tempCount++;
  }
  return tempCount;
}

size_t CBiotop::getNbOfSpecieEntities(string& specieName)
{
  size_t tempCount = 0;
  bool femaleFound = false;
  bool maleFound = false;
  for (CBasicEntity* pEntity : m_tEntity)
  {
    if ((pEntity->getSpecieName() == specieName) && (pEntity->isAlive() || (pEntity->getStatus() == STATUS_STATIC)))
    {
      if (pEntity->getTypeOfReproduction() == REPRODUCT_SEXUAL)
      {
        if (pEntity->getSex() == SEX_MALE)
          maleFound = true;
        else if (pEntity->getSex() == SEX_FEMALE)
          femaleFound = true;
      }
      tempCount++;
    }
  }
  if (maleFound && !femaleFound)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Specie going to extinction: %s: No more female\n", specieName.c_str());
  }
  else if (femaleFound && !maleFound)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Specie going to extinction: %s: No more male\n", specieName.c_str());
  }
  return tempCount;
}

double CBiotop::getSpecieBiomass(string& SpecieName)
{
  double tempCount = 0;
  for (CBasicEntity* pEntity : m_tEntity)
  {
    if ((pEntity->getSpecieName() == SpecieName) && (pEntity->isAlive() || (pEntity->getStatus() == STATUS_STATIC)))
      tempCount += pEntity->getWeight();
  }
  return tempCount;
}

size_t CBiotop::getNbOfMarkedEntities(void)
{
  size_t tempCount = 0;
  for (CBasicEntity* pEntity : m_tEntity)
  {
    if (pEntity->isMarked())
      tempCount++;
  }
  return tempCount;
}

//===========================================================================
// Entities access
//===========================================================================

CBasicEntity* CBiotop::getEntityById(entityIdType idEntity)
{
  // Check if it is water
  if (idEntity==ENTITY_ID_WATER)
  {
    return m_pWaterGlobalEntity;
  }

  if (idEntity==ENTITY_ID_GRASS)
  {
    return m_pGrassGlobalEntity;
  }

  if (idEntity == ENTITY_ID_ROCK)
  {
    return m_pRockGlobalEntity;
  }

  for (CBasicEntity* pEntity : m_tEntity)
  {
    if (pEntity->getId() == idEntity)
    {
        return pEntity;
    }
  }
  // If not found
  return NULL;
}

CBasicEntity* CBiotop::getEntityByIndex(size_t index)
{
  return (index < getNbOfEntities()) ? m_tEntity[index] : NULL;
}

CBasicEntity* CBiotop::getEntityByName(string& entityName)
{
  for (CBasicEntity* pEntity : m_tEntity)
  {
    if (pEntity->getLabel() == entityName)
    {
        return pEntity;
    }
  }
  return NULL;
}

size_t CBiotop::getEntityTableIndex(CBasicEntity* pEntity)
{
  for (size_t i=0; i<getNbOfEntities(); i++)
  {
    if (m_tEntity[i] == pEntity)
    {
        return i;
    }
  }
  return invalidIndex;
}

void CBiotop::colorizeSearchNoCheckCoord(const Point_t& coord)
{
  if (m_bColorizeSearch)
  {
    m_tBioSquare[coord.x][coord.y].customColor -= 0x00001010; //blue
  }
}

CBasicEntity* CBiotop::findEntity(const Point_t& searchCoord, size_t layer)
{
  if (isCoordValid(searchCoord,layer))
  {
#ifdef WIN32
    // CPU optim: Color is only for Gui. No need in console mode
    colorizeSearchNoCheckCoord(searchCoord);
#endif
    return m_tBioGrid[searchCoord.x][searchCoord.y][layer].pEntity;   
  }
  return NULL;
}

CBasicEntity* CBiotop::findEntityNoCheckCoord(const Point_t& searchCoord, size_t layer)
{
  return m_tBioGrid[searchCoord.x][searchCoord.y][layer].pEntity;
}

void CBiotop::putEntityInList(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, size_t layer, bool includeWater)
{
  CBasicEntity* pCurEntity = findEntity(searchCoord, layer);
  if ((pCurEntity != NULL) && !pCurEntity->isToBeRemoved() && (includeWater || (pCurEntity->getId() != ENTITY_ID_WATER)))
  {
    if (foundIds.nbFoundIds >= MAX_FOUND_ENTITIES)
      return;
    foundIds.tFoundIds[foundIds.nbFoundIds].pEntity = pCurEntity;
    foundIds.tFoundIds[foundIds.nbFoundIds].distance = distanceToSet;
    foundIds.nbFoundIds++;
  }
}

void CBiotop::putEntityInListOptim(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, size_t layer, bool includeWater)
{
  CBasicEntity* pCurEntity = findEntity(searchCoord, layer);
  if ((pCurEntity != NULL) && !pCurEntity->isToBeRemoved() && (includeWater || (pCurEntity->getId() != ENTITY_ID_WATER)))
  {
    if (foundIds.nbFoundIds >= MAX_FOUND_ENTITIES)
      return;
    if ((foundIds.nbFoundIds < 10) || pCurEntity->isAnimal() ||
       ((foundIds.tFoundIds[foundIds.nbFoundIds - 1].pEntity != NULL)
       && (foundIds.tFoundIds[foundIds.nbFoundIds - 1].pEntity->getEntitySignature() != pCurEntity->getEntitySignature())))
    {
      foundIds.tFoundIds[foundIds.nbFoundIds].pEntity = pCurEntity;
      foundIds.tFoundIds[foundIds.nbFoundIds].distance = distanceToSet;
      foundIds.nbFoundIds++;
    }
  }
}

// CPU Optim: No check coord!
void CBiotop::putEntitiesInListAllLayers(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, bool includeWater)
{
  constexpr size_t startlayer{ 1 }; // CPU optim: start at layer 1 (layer 0 is underground not detected by default)
  BiotopCube_t* pBiotopCube = &m_tBioGrid[searchCoord.x][searchCoord.y][startlayer];
  CBasicEntity* pCurEntity = NULL;
  for (size_t z = startlayer; z < m_nbLayer; z++)
  {
    pCurEntity = pBiotopCube->pEntity;
    if ((pCurEntity != NULL) && !pCurEntity->isToBeRemoved() && (includeWater || (pCurEntity->getId() != ENTITY_ID_WATER)))
    {
      if (foundIds.nbFoundIds >= MAX_FOUND_ENTITIES)
        return;
      foundIds.tFoundIds[foundIds.nbFoundIds].pEntity = pCurEntity;
      foundIds.tFoundIds[foundIds.nbFoundIds].distance = distanceToSet;
      foundIds.nbFoundIds++;
    }
    pBiotopCube++;
  }
#ifdef WIN32
  // CPU optim: Color is only for Gui. No need in console mode
  colorizeSearchNoCheckCoord(searchCoord);
#endif
}

void CBiotop::putEntitiesInListAllLayersOptim(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, bool includeWater)
{
  constexpr size_t startlayer{ 1 }; // CPU optim: start at layer 1 (layer 0 is underground not detected by default)
  BiotopCube_t* pBiotopCube = &m_tBioGrid[searchCoord.x][searchCoord.y][startlayer];
  CBasicEntity* pCurEntity = NULL;
  for (size_t z = startlayer; z < m_nbLayer; z++)
  {
    pCurEntity = pBiotopCube->pEntity;
    if ((pCurEntity != NULL) && !pCurEntity->isToBeRemoved() && (includeWater || (pCurEntity->getId() != ENTITY_ID_WATER)))
    {
      if (foundIds.nbFoundIds >= MAX_FOUND_ENTITIES)
        return;
      if ((foundIds.nbFoundIds < 10) || pCurEntity->isAnimal() ||
         ((foundIds.tFoundIds[foundIds.nbFoundIds - 1].pEntity != NULL) 
          && (foundIds.tFoundIds[foundIds.nbFoundIds - 1].pEntity->getEntitySignature() != pCurEntity->getEntitySignature())))
      {
        foundIds.tFoundIds[foundIds.nbFoundIds].pEntity = pCurEntity;
        foundIds.tFoundIds[foundIds.nbFoundIds].distance = distanceToSet;
        foundIds.nbFoundIds++;
      }
    }
    pBiotopCube++;
  }
#ifdef WIN32
  // CPU optim: Color is only for Gui. No need in console mode
  colorizeSearchNoCheckCoord(searchCoord);
#endif
}

void CBiotop::findEntitiesInRow(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t startCoord, size_t lenght, bool includeWater)
{
  if (startCoord.y < m_Dimension.y)
  {
    size_t startCoordX = (startCoord.x < m_Dimension.x) ? startCoord.x : 0;
    size_t endCoordX = startCoord.x + lenght;
    if (endCoordX > m_Dimension.x)
    {
      endCoordX = m_Dimension.x;
    }
    for (size_t i = startCoordX; i < endCoordX; i++)
    {
      putEntitiesInListAllLayers(foundIds, distanceToSet, { i, startCoord.y }, includeWater);
    }
  }
}

void CBiotop::findEntitiesInColumn(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t startCoord, size_t lenght, bool includeWater)
{
  if (startCoord.x < m_Dimension.x)
  {
    size_t startCoordY = (startCoord.y < m_Dimension.y) ? startCoord.y : 0;
    size_t endCoordY = startCoord.y + lenght;
    if (endCoordY > m_Dimension.y)
    {
      endCoordY = m_Dimension.y;
    }
    for (size_t j = startCoordY; j < endCoordY; j++)
    {
      putEntitiesInListAllLayers(foundIds, distanceToSet, { startCoord.x, j }, includeWater);
    }
  }
}

const BiotopFoundIds_t& CBiotop::findEntitiesInSquare(const Point_t& bottomLeftCoord, size_t squareSize, bool includeWater)
{
  Point_t curCoord;
  size_t i;
  int threadNum = omp_get_thread_num();
  BiotopFoundIds_t& foundIds = m_BiotopFoundIdsArray[threadNum];
  foundIds.nbFoundIds = 0;

  curCoord.x = bottomLeftCoord.x;
  curCoord.y = bottomLeftCoord.y;

  for (i = 0; i < squareSize; i++)
  {
    findEntitiesInColumn(foundIds, i, curCoord, squareSize, includeWater);
    curCoord.x++;
  }

  return foundIds;
}

const BiotopFoundIds_t& CBiotop::findEntities(const Point_t& startCoord, size_t distance, bool includeWater)
{
  Point_t curCoord;
  int threadNum = omp_get_thread_num();
  BiotopFoundIds_t& foundIds = m_BiotopFoundIdsArray[threadNum];
  foundIds.nbFoundIds = 0;

  findEntitiesInColumn(foundIds, 0, startCoord, 1, includeWater);

  // Do circles around center
  for (size_t k = 1; k < distance; k++)
  {
    curCoord.x = startCoord.x - k;
    curCoord.y = startCoord.y - k;
    findEntitiesInColumn(foundIds, k, curCoord, 2 * k + 1, includeWater);

    curCoord.x++;
    findEntitiesInRow(foundIds, k, curCoord, 2 * k - 1, includeWater);

    curCoord.x = startCoord.x + k;
    findEntitiesInColumn(foundIds, k, curCoord, 2 * k + 1, includeWater);

    curCoord.x = startCoord.x - k + 1;
    curCoord.y = startCoord.y + k;
    findEntitiesInRow(foundIds, k, curCoord, 2 * k - 1, includeWater);

    if (foundIds.nbFoundIds >= MAX_FOUND_ENTITIES)
      break;
  }

  return foundIds;
}

//  startCoord entity not include
const BiotopFoundIds_t& CBiotop::findEntities(const Point_t& centerCoord, UCHAR sectorBmp, int distance, size_t layer, bool includeWater)
{
  Point_t curCoord;
  int i, j;
  int startCoordx = (int)centerCoord.x;
  int startCoordy = (int)centerCoord.y;
  int threadNum = omp_get_thread_num();
  BiotopFoundIds_t& foundIds = m_BiotopFoundIdsArray[threadNum];
  foundIds.nbFoundIds = 0;

  if ( (sectorBmp & SECTOR_EE_BIT_MASK) !=0 )
  {
    int starty = startCoordy;
    int endy = startCoordy;
    int offset = 0;
    for (i = startCoordx + 1; i <= startCoordx + distance;i++)
    {
      for (j = starty; j <= endy; j++)
      {
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, i - startCoordx, curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, cybio_max(i - startCoordx, j - startCoordy), curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, j - startCoordy, curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, cybio_max(startCoordx - i, j - startCoordy), curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, startCoordx - i, curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, cybio_max(startCoordx - i, startCoordy - j), curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, startCoordy - j, curCoord, layer, includeWater);
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
        curCoord.x = i;
        curCoord.y = j;
        putEntityInListOptim(foundIds, cybio_max(i - startCoordx, startCoordy - j), curCoord, layer, includeWater);
      }
      // Enlarge y range
      offset = (offset+1)%2;
      starty -= offset;
      endy -= 2;
      if (endy < startCoordy - distance)
        endy = startCoordy - distance;
    }
  }
  return foundIds;
}

//  find all entities betwen 2 range, on any layer except layer0 (under ground)
const BiotopFoundIds_t& CBiotop::findFarEntities(const Point_t& centerCoord, UCHAR sectorBmp, int rangeMin, int rangeMax, bool includeWater)
{
  Point_t curCoord;
  int entityDist;
  int i, j;
  int startCoordx = (int)centerCoord.x;
  int startCoordy = (int)centerCoord.y;
  int threadNum = omp_get_thread_num();
  BiotopFoundIds_t& foundIds = m_BiotopFoundIdsArray[threadNum];
  foundIds.nbFoundIds = 0;

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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
          if (isCoordValid(curCoord))
          {
            putEntitiesInListAllLayersOptim(foundIds, entityDist, curCoord, includeWater);
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
  return foundIds;
}

bool CBiotop::isCoordValidAndFree(const Point_t& coord, size_t layer)
{
  return (isCoordValid(coord, layer) && (findEntityNoCheckCoord(coord, layer) == NULL));
}

bool CBiotop::isGlobalGridCoordValidAndFree(const Point_t& globalCoord, size_t layer)
{
  return isCoordValidAndFree(getGridCoordFromGlobalGridCoord(globalCoord), layer);
}

bool CBiotop::isCoordValid(const Point_t& coord, size_t layer)
{
  return ((coord.x < m_Dimension.x) && (coord.y < m_Dimension.y) && (layer < m_nbLayer));
}

bool CBiotop::isCoordValid(const Point_t& coord)
{
  return ((coord.x < m_Dimension.x) && (coord.y < m_Dimension.y));
}

Point_t CBiotop::getGridCoordFromStepCoord(const Point_t& stepCoord)
{
  return { stepCoord.x / NB_STEPS_PER_GRID_SQUARE, stepCoord.y / NB_STEPS_PER_GRID_SQUARE };
}

Point_t CBiotop::getStepCoordFromGridCoord(const Point_t& gridCoord)
{
  return { gridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE / 2, 
           gridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE / 2 };
}

Point_t CBiotop::getGlobalGridCoordFromGridCoord(const Point_t& localGridCoord)
{
  return { localGridCoord.x + m_GlobalGridCoordOffset.x, localGridCoord.y + m_GlobalGridCoordOffset.y };
}

Point_t CBiotop::getGridCoordFromGlobalGridCoord(const Point_t& globalGridCoord)
{
  return { globalGridCoord.x - m_GlobalGridCoordOffset.x, globalGridCoord.y - m_GlobalGridCoordOffset.y };
}

Point_t CBiotop::getGlobalStepCoordFromStepCoord(const Point_t& localStepCoord)
{
  return { localStepCoord.x + m_GlobalStepCoordOffset.x, localStepCoord.y + m_GlobalStepCoordOffset.y };
}

Point_t CBiotop::getStepCoordFromGlobalStepCoord(const Point_t& globalStepCoord)
{
  return { globalStepCoord.x - m_GlobalStepCoordOffset.x, globalStepCoord.y - m_GlobalStepCoordOffset.y };
}

CBasicEntity* CBiotop::findTopLevelEntity(const Point_t& searchCoord)
{
  CBasicEntity* pFoundEntity = NULL;

  if (!isCoordValid(searchCoord))
    return NULL;

  for (int layer = (int)m_nbLayer - 1; layer >= 0; layer--)
  {
    pFoundEntity = findEntityNoCheckCoord(searchCoord, layer);
    if (pFoundEntity!=NULL)
    {
      return pFoundEntity;
    }
  }
  // No entity found => return NULL
  return NULL;
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

  return resu;
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

  return resu;
}

void CBiotop::markAllEntitiesWithGene(CGene& modelGene, bool markDominantAlleleOnly)
{
  m_GeneToMark = modelGene;
  m_bMarkDominantAlleleOnly = markDominantAlleleOnly;
  for (CBasicEntity* pCurEntity : m_tEntity)
  {
    markEntityWithGene(pCurEntity);
  }
}

void CBiotop::markEntityWithGene(CBasicEntity* pEntity)
{
  if (m_GeneToMark.getGeneType() != GENE_GENERIC)
  {
    if (pEntity->getGenome()->findGeneInGenome(m_GeneToMark, m_bMarkDominantAlleleOnly).second != invalidIndex)
      pEntity->setMarked(true);
    else
      pEntity->setMarked(false);
  }
}

void CBiotop::clearMarksOnAllEntities()
{
  m_GeneToMark.setAsNeutral();
  m_bMarkDominantAlleleOnly = false;
  for (CBasicEntity* pCurEntity : m_tEntity)
  {
    pCurEntity->setMarked(false);
  }
}

//===========================================================================
// Time management
//===========================================================================

void CBiotop::nextSecond(bool doIncreaseTime)
{
  logCpuMarkerStart(BIOTOP_CPUMARKER_TOTAL);

  if (doIncreaseTime)
  {
    m_BioTime.seconds++;
    resetBiotopEventsMapCurrent();
    if (m_BioTime.seconds >= NUMBER_SECONDS_PER_HOUR)
    {
      // Next hour for biotop
      nextHour();
      generateRandomEntities();
      memorizeAllPopulationMaps();
    }
  }

  if (m_BioTime.seconds == m_NextHourTimeOffset)
  {
    decreaseOdorMap();
    nextHourForAllEntities();
    resetCpuMarker();
  }

  // Loop on all animals for basic action
  nextSecondForAllAnimals();

  // Trigger measurement
  if (doIncreaseTime)
  {
    triggerMeasuresNextSecond();
  }

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
  // Update time
  m_BioTime.seconds = 0;
  m_BioTime.hours++;
  // Cyclic parameters updates
  m_pSunlightRate->nextStep();
  m_pFertilityRate->nextStep();
  m_pTemperature->nextStep();
  if (m_BioTime.hours >= NUMBER_HOURS_PER_DAY)
  {
    nextDay();
  }
}

void CBiotop::nextDay(void)
{
  size_t nbLivingAnimals = getNbOfLivingAnimals();
  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - nextDay CPU total=%f animals=%f custom1=%f custom2=%f for %d living animals\n",
    m_CpuMonitoring[BIOTOP_CPUMARKER_TOTAL].cpuTimeCumulated,
    m_CpuMonitoring[BIOTOP_CPUMARKER_ANIMALS].cpuTimeCumulated,
    m_CpuMonitoring[BIOTOP_CPUMARKER_CUSTOM1].cpuTimeCumulated,
    m_CpuMonitoring[BIOTOP_CPUMARKER_CUSTOM2].cpuTimeCumulated,
    nbLivingAnimals);
  if (nbLivingAnimals > 0)
  {
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - nextDay CPU: average CPU per living animal=%f\n", m_CpuMonitoring[BIOTOP_CPUMARKER_ANIMALS].cpuTimeCumulated / nbLivingAnimals);
  }

  m_BioTime.hours = 0;
  m_BioTime.days++;
  if (m_BioTime.days >= NUMBER_DAYS_PER_YEAR)
  {
    m_BioTime.days = 0;
    m_BioTime.years++;
  }

  // Automatic metric records
  if ((m_BioTime.days % 2) == 1)
  {
    saveAllRecordsInFiles();
  }
  if ((m_AutoSaveSpecieName != "") && ((m_BioTime.days % 100) == 0))
  {
    saveAllEntitiesOfSpecie(m_AutoSaveSpecieName);
  }
}

void CBiotop::nextHourForAllEntities(void)
{
  CBasicEntity* pEntity = NULL;
  // loop from top to bottom to clear entities each hour
  for (int i = (int)getNbOfEntities() - 1; i >= 0; i--)
  {
    pEntity = m_tEntity[i];
    if (pEntity != NULL)
    {
      if (pEntity->isToBeRemoved())
      {
        deleteEntity(pEntity, false);
      }
      else
      {
        pEntity->nextHour();
      }
    }
  }
}

void CBiotop::nextSecondForAllAnimals(void)
{
  // Loop on all animals for basic action
  logCpuMarkerStart(BIOTOP_CPUMARKER_ANIMALS);
  if (getNbOfAnimals() < m_NbOmpThreads)
  {
    nextSecondForAllAnimalsSingleProcess();
  }
  else
  {
    nextSecondForAllAnimalsMultiProcess();
  }
  logCpuMarkerEnd(BIOTOP_CPUMARKER_ANIMALS);
}

void CBiotop::nextSecondForAllAnimalsSingleProcess(void)
{
  CBasicEntity* pEntity = NULL;
  size_t i;
  for (i = 0; i < getNbOfAnimals(); i++)
  {
    pEntity = m_tEntity[i];
    if (pEntity != NULL)
    {
      pEntity->nextSecond();
    }
  }
}

void CBiotop::nextSecondForAllAnimalsMultiProcess(void)
{
  CBasicEntity* pEntity = NULL;
  int i;
#pragma omp parallel for ordered private(i) num_threads(m_NbOmpThreads)
  for (i = 0; i < getNbOfAnimals(); i++)
  {
    pEntity = m_tEntity[i];
    if (pEntity != NULL)
    {
      pEntity->nextSecond();
    }
  }
#pragma omp barrier
}

void CBiotop::decreaseOdorMap()
{
  for (size_t i = 0; i < m_Dimension.x; i++)
  {
    for (size_t j = 0; j < m_Dimension.y; j++)
    {
      for (size_t odor = 0; odor < NUMBER_ODORS; odor++)
      {
        if (m_tBioSquare[i][j].odorTrace[odor] < 2)
          m_tBioSquare[i][j].odorTrace[odor] = 0;
        else
          m_tBioSquare[i][j].odorTrace[odor] = m_tBioSquare[i][j].odorTrace[odor] / 2;
      }
    }
  }
}

void CBiotop::memorizeAllPopulationMaps()
{
  for (auto pGeoMap : m_tGeoMapSpecies)
  {
    pGeoMap->MemorizePopulationMap(m_BioTime.days);
  }
}

void CBiotop::generateRandomEntities()
{
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
        fertilityFactor = getFertility({ 1, 1 });
        coverRate = coverRate * fertilityFactor / 100.0;
      }
      if (randomEntity.avaragePeriodicity > 0)
      {
        periodicityFactor = 100.0 / ((double)(randomEntity.avaragePeriodicity) * NUMBER_HOURS_PER_DAY);
      }
      if (testChance(periodicityFactor, fertilityFactor))
      {
        spawnEntitiesRandomly(randomEntity.pModelEntity, cybio_round(coverRate));
      }
    }
  }
}

const BiotopTime_t& CBiotop::getBiotopTime(void)
{
  return m_BioTime;
}

void CBiotop::setBiotopTime(int seconds, int hours, int days, int years)
{
  m_BioTime.seconds = seconds;
  m_BioTime.hours = hours;
  m_BioTime.days = days;
  m_BioTime.years = years;
  resetBiotopEventsMapCurrent();
}

void CBiotop::setNextHourTimeOffset(unsigned char nextHourTimeOffset)
{
  m_NextHourTimeOffset = nextHourTimeOffset;
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
  if (m_nbLayer < 3)
  {
    CYBIOCORE_LOG("CBiotop::initGridDefaultLayerType: Error, not enough layers\n");
    return;
  }

  size_t i, j;
  // Put Ground Everywhere:
  for (i=0;i<m_Dimension.x;i++)
  {
    for (j=0;j<m_Dimension.y;j++)
    {
      if (testChance(0)) // choose here de % of rocky ground
      {
        setGridGroundTypeRock(i, j);
      }
      else
      {
        setGridGroundTypeEarth(i, j);
      }
    }
  }

  // Add a River in the middle
  //for (j = 1; j < m_Dimension.y / 2; j++)
  //{
  //  setGridGroundTypeWater(m_Dimension.x / 2 - 1, j);
  //  setGridGroundTypeDeepWater(m_Dimension.x / 2, j);
  //  setGridGroundTypeWater(m_Dimension.x / 2 + 1, j);
  //}

  // Add lake in the center
  for (i=(m_Dimension.x/2-5); i<(m_Dimension.x/2+5); i++)
  {
    for (j=(m_Dimension.y/2-5); j<(m_Dimension.y/2+5); j++)
    {
      setGridGroundTypeWater(i, j);
    }
  }
  for (i=(m_Dimension.x/2-2); i<(m_Dimension.x/2+2); i++)
  {
    for (j=(m_Dimension.y/2-2); j<(m_Dimension.y/2+2); j++)
    {
      setGridGroundTypeDeepWater(i, j);
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
    for (size_t j = 0; j < m_Dimension.y; j++)
    {
      for (size_t k = 0; k < m_nbLayer; k++)
      {
        m_tBioGrid[i][j][k].pEntity = NULL;
      }

      if (m_nbLayer > 1)
      {
        // If water present: set water global entity
        if ((m_tBioGrid[i][j][1].layerType == LAYER_OVER_WATER) || (m_tBioGrid[i][j][1].layerType == LAYER_OVER_WET_GROUND))
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
      }

      // reset odor traces
      for (size_t odor = 0; odor < NUMBER_ODORS; odor++)
      {
        m_tBioSquare[i][j].odorTrace[odor] = 0;
      }

      // reset custom color
      m_tBioSquare[i][j].customColor = 0x00FFFFFF; //No color
    }
  }

  updateGridAllEntities();
  updateGridFertilityBonus();
}

void CBiotop::setGridGroundTypeEarth(size_t gridCoordX, size_t gridCoordY)
{
  if ((gridCoordX >= m_Dimension.x) || (gridCoordY >= m_Dimension.y))
  return;

  if (m_tBioGrid[gridCoordX][gridCoordY][1].pEntity != NULL)
  {
    m_tBioGrid[gridCoordX][gridCoordY][1].pEntity->autoRemove();
  }
  m_tBioGrid[gridCoordX][gridCoordY][0].layerType = LAYER_UNDER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].layerType = LAYER_OVER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][2].layerType = LAYER_OVER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].pEntity = NULL;
}

void CBiotop::setGridGroundTypeDeepWater(size_t gridCoordX, size_t gridCoordY)
{
  if ((gridCoordX >= m_Dimension.x) || (gridCoordY >= m_Dimension.y))
    return;

  if (m_tBioGrid[gridCoordX][gridCoordY][1].pEntity != NULL)
  {
    m_tBioGrid[gridCoordX][gridCoordY][1].pEntity->autoRemove();
  }
  m_tBioGrid[gridCoordX][gridCoordY][0].layerType = LAYER_UNDER_WATER;
  m_tBioGrid[gridCoordX][gridCoordY][1].layerType = LAYER_OVER_WATER;
  m_tBioGrid[gridCoordX][gridCoordY][2].layerType = LAYER_IN_AIR;
  m_tBioGrid[gridCoordX][gridCoordY][1].pEntity = m_pWaterGlobalEntity;
}

void CBiotop::setGridGroundTypeWater(size_t gridCoordX, size_t gridCoordY)
{
  if ((gridCoordX >= m_Dimension.x) || (gridCoordY >= m_Dimension.y))
    return;

  if (m_tBioGrid[gridCoordX][gridCoordY][1].pEntity != NULL)
  {
    m_tBioGrid[gridCoordX][gridCoordY][1].pEntity->autoRemove();
  }
  m_tBioGrid[gridCoordX][gridCoordY][0].layerType = LAYER_UNDER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].layerType = LAYER_OVER_WET_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][2].layerType = LAYER_OVER_WET_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].pEntity = m_pWaterGlobalEntity;
}

void CBiotop::setGridGroundTypeGrass(size_t gridCoordX, size_t gridCoordY)
{
  if ((gridCoordX >= m_Dimension.x) || (gridCoordY >= m_Dimension.y))
    return;

  if (m_tBioGrid[gridCoordX][gridCoordY][1].pEntity != NULL)
  {
    m_tBioGrid[gridCoordX][gridCoordY][1].pEntity->autoRemove();
  }
  m_tBioGrid[gridCoordX][gridCoordY][0].layerType = LAYER_UNDER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].layerType = LAYER_GLOBAL_GRASS;
  m_tBioGrid[gridCoordX][gridCoordY][2].layerType = LAYER_OVER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].pEntity = m_pGrassGlobalEntity;
}

void CBiotop::setGridGroundTypeRock(size_t gridCoordX, size_t gridCoordY)
{
  if ((gridCoordX >= m_Dimension.x) || (gridCoordY >= m_Dimension.y))
    return;

  if (m_tBioGrid[gridCoordX][gridCoordY][1].pEntity != NULL)
  {
    m_tBioGrid[gridCoordX][gridCoordY][1].pEntity->autoRemove();
  }
  m_tBioGrid[gridCoordX][gridCoordY][0].layerType = LAYER_UNDER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].layerType = LAYER_GLOBAL_ROCK;
  m_tBioGrid[gridCoordX][gridCoordY][2].layerType = LAYER_OVER_GROUND;
  m_tBioGrid[gridCoordX][gridCoordY][1].pEntity = m_pRockGlobalEntity;
}

void CBiotop::updateGridFertilityBonus(void)
{
  CBasicEntity* pCurEntity = NULL;
  int currentBonus;
  for (size_t i = 0; i < m_Dimension.x; i++)
  {
    for (size_t j = 0; j < m_Dimension.y; j++)
    {
      m_tBioSquare[i][j].fertilityBonus = 0;
      const BiotopFoundIds_t& biotopFoundIds = findEntities({ i, j }, 10, true);
      const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;
      for (size_t ind = 0; ind < biotopFoundIds.nbFoundIds; ind++)
      {
        pCurEntity = tFoundIds[ind].pEntity;
        if (pCurEntity->isDrinkable())
        {
          currentBonus = 16 - (int)tFoundIds[ind].distance;
          if (currentBonus > m_tBioSquare[i][j].fertilityBonus)
          {
            m_tBioSquare[i][j].fertilityBonus = currentBonus;
          }
        }
      }  
    }
  }
}

void CBiotop::ModifyGridFertilityBonus(const Point_t& coord, char bonusToAdd)
{
  if (isCoordValid(coord))
  {
    m_tBioSquare[coord.x][coord.y].fertilityBonus += bonusToAdd;
  }
}

void CBiotop::updateGridEntity(CBasicEntity* pCurEntity)
{
  ASSERT(pCurEntity != NULL);
  Point_t tmpCoord;
  size_t tmpLayer;

  if (pCurEntity->checkIfhasMovedAndClear())
#pragma omp critical
  {
    tmpCoord = pCurEntity->getPrevGridCoord();
    tmpLayer = pCurEntity->getPrevLayer();
    if (isCoordValid(tmpCoord, tmpLayer))
    {
      // memorize odor trace
      if (pCurEntity->getOdor() > ODOR_NONE)
      {
        m_tBioSquare[tmpCoord.x][tmpCoord.y].odorTrace[OdorTypeToIndex(pCurEntity->getOdor())] = MAX_ODOR_TRACE_VAL;
      }
      m_tBioGrid[tmpCoord.x][tmpCoord.y][tmpLayer].pEntity = NULL;
    }
    tmpCoord = pCurEntity->getGridCoord();
    tmpLayer = pCurEntity->getLayer();
    if (isCoordValid(tmpCoord, tmpLayer))
    {
      m_tBioGrid[tmpCoord.x][tmpCoord.y][tmpLayer].pEntity = pCurEntity;
    }
  }
}

int CBiotop::getGridDistance(const Point_t& gridCoord1, const Point_t& gridCoord2)
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
    if ( (pCurEntity) && (pCurEntity->checkIfhasMovedAndClear()) )
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
    if (pCurEntity)
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

  return true;
}

bool CBiotop::addMeasureEntityParam(CBasicEntity* pEntity, int paramId, int period, int id)
{
  if ( (pEntity==NULL) || (period<0) || (pEntity->getParameter(paramId)==NULL) )
    return false;

  CMeasureParam* pNewMeas = new CMeasureEntityParam(pEntity, paramId, period, id);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);
  return true;
}

bool CBiotop::addMeasureFeeling(CBasicEntity* pEntity, int period, int id, MeasureFeelingType_e type)
{
  if ((pEntity==NULL)||(period<0))
    return false;

  CMeasureFeeling* pNewMeas = new CMeasureFeeling(pEntity, period, id, type);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);
  return true;
}

bool CBiotop::addMeasureReaction(CBasicEntity* pEntity, int reactionInd, int period, int id, MeasureReactionType_e type)
{
  if ((pEntity==NULL)||(period<0))
    return false;

  CMeasureReaction* pNewMeas = new CMeasureReaction(pEntity, reactionInd, period, id, type);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);
  return true;
}

bool CBiotop::addMeasurePopulation(int period, int id, MeasurePopulationType_e type, int maxVal, string SpecieName)
{
  CMeasurePopulation* pNewMeas = new CMeasurePopulation(this, period, id, type, maxVal, SpecieName);
  m_tMeasures.push_back(pNewMeas);
  pNewMeas->StartMeasurement(m_BioTime);
  return true;
}

CMeasure* CBiotop::getMeasureById(int id)
{
  CMeasure* foundMeas = NULL;
  for (auto pMeasure : m_tMeasures)
  {
    if (pMeasure->GetId() == id)
      foundMeas = pMeasure;
  }
  return foundMeas;
}

CMeasure* CBiotop::getMeasureByIndex(size_t index)
{
  return m_tMeasures[index];
}

size_t CBiotop::getNbOfMeasures(void)
{
  return m_tMeasures.size();
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
  return resu;
}

void CBiotop::saveAllMeasuresInDataFile(string fileNameWithPath)
{
  for (size_t i = 0; i<m_tMeasures.size(); i++)
  {
    m_tMeasures[i]->saveInFile(fileNameWithPath);
  }
}

timeCountType CBiotop::getNextSmallestTimeCountInAllMeasures(timeCountType previousSmallestTimeCount)
{
  if (m_tMeasures.size() == 0)
    return MAX_TIMECOUNT_VALUE;

  timeCountType curentMinCount = m_tMeasures[0]->getNextSmallestTimeCount(previousSmallestTimeCount);
  timeCountType nextMinCount;
  for (size_t i = 1; i < m_tMeasures.size(); i++)
  {
    nextMinCount = m_tMeasures[i]->getNextSmallestTimeCount(previousSmallestTimeCount);
    curentMinCount = cybio_min(nextMinCount, curentMinCount);
  }
  // Check if end of measure is reached
  if (curentMinCount == previousSmallestTimeCount)
    curentMinCount = MAX_TIMECOUNT_VALUE;

  return curentMinCount;
}


void CBiotop::saveAllMeasuresInCsvFile(string fileNameWithPath)
{
  // First line
  string savedMeasureString = "Time";
  for (size_t i = 0; i < m_tMeasures.size(); i++)
  {
    savedMeasureString += "," + m_tMeasures[i]->GetLabel();
  }
  savedMeasureString += "\n";

  // Loop on data
  size_t maxIterations = m_tMeasures.size() * MAX_MEASUREMENT_DATA_SIZE;
  size_t numberIterations = 0;
  timeCountType currentTimeCount = getNextSmallestTimeCountInAllMeasures(0);
  MeasureData_t invalidMeasure{ 0.0, MAX_TIMECOUNT_VALUE };
  while ((currentTimeCount != MAX_TIMECOUNT_VALUE) && (numberIterations < maxIterations))
  {
    savedMeasureString += FormatString("%u", currentTimeCount);
    for (size_t i = 0; i < m_tMeasures.size(); i++)
    {
      MeasureData_t& curentData = invalidMeasure;
      if (m_tMeasures[i]->getMeasureFromTimeStamp(currentTimeCount, curentData))
      {
        savedMeasureString += "," + FormatString("%f", curentData.value);
      }
      else
      {
        savedMeasureString += ",";
      }
    }
    currentTimeCount = getNextSmallestTimeCountInAllMeasures(currentTimeCount);
    savedMeasureString += "\n";
    numberIterations++;
  }

  ofstream f1;
  f1.open(fileNameWithPath.c_str(), std::ofstream::out | std::ofstream::app);
  f1.write(savedMeasureString.c_str(), savedMeasureString.length());
  f1.close();
}

bool CBiotop::addGeoMapSpeciePopulation(std::string specieName)
{
  CGeoMapPopulation* pGeoMapPopulation = new CGeoMapPopulation(this, getGlobalGridDimension(), specieName);
  m_tGeoMapSpecies.push_back(pGeoMapPopulation);
  // Store initial population map
  pGeoMapPopulation->MemorizePopulationMap(m_BioTime.days);
  return true;
}

size_t CBiotop::getNbOfGeoMapSpecie(void)
{
  return m_tGeoMapSpecies.size();
}

CGeoMapPopulation* CBiotop::getGeoMapSpecieByIndex(size_t index)
{
  return (index < m_tGeoMapSpecies.size()) ? m_tGeoMapSpecies[index] : NULL;
}

void CBiotop::saveAllGeoMapsInFile(string fileNameWithPath)
{
  for (size_t indexRecord = 0; indexRecord < getNbOfGeoMapSpecie(); indexRecord++)
  {
    getGeoMapSpecieByIndex(indexRecord)->saveInXmlFile(fileNameWithPath);
  }
}

void CBiotop::saveAllRecordsInFiles()
{
  string measureFileName{ m_DefaultFilePath + "/Measures.csv" };
  string geomapFileName{ m_DefaultFilePath + "/GeoMaps.xml" };

  std::remove(measureFileName.c_str());
  std::remove(geomapFileName.c_str());

  saveAllMeasuresInCsvFile(measureFileName);
  saveAllGeoMapsInFile(geomapFileName);
}

void CBiotop::saveAllEntitiesOfSpecie(string specieName)
{
  string path = m_DefaultFilePath + "/AutoSave/";
  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP  - Save all entities of specie name %s path %s\n", specieName.c_str(), path.c_str());

  for (CBasicEntity* pCurEntity : m_tEntity)
  {
    if (pCurEntity->getSpecieName() == specieName)
    {
      string entityFileName{ path + pCurEntity->getLabel() + ".xml"};
      pCurEntity->saveInXmlFile(entityFileName);
    }
  }
}

//===========================================================================
// Event management
//===========================================================================
bool CBiotop::addBiotopEvent(EntityEventList_e entityEventList, CBasicEntity* pEntity)
{
  if ((pEntity == NULL) || (pEntity->getId() == ENTITY_ID_INVALID))
    return false;

  std::map<entityIdType, BiotopEvent_t>* tEventMap{ &getBiotopEventMapCurrent() };

#pragma omp critical
  {
    auto search = tEventMap->find(pEntity->getId());
    if (search != tEventMap->end())
    {
      search->second.eventList.set(entityEventList);
      search->second.pEntity = pEntity;
      search->second.markAsReadByGui = false;
      search->second.markAsReadByNetwork = false;
    }
    else
    {
      BiotopEvent_t newEvent;
      newEvent.eventList.set(entityEventList);
      newEvent.pEntity = pEntity;
      newEvent.markAsReadByGui = false;
      newEvent.markAsReadByNetwork = false;
      tEventMap->insert({ pEntity->getId(), newEvent });

      // Avoid overload by cleaning oldest events
      if (tEventMap->size() > 100000)
      {
        tEventMap->erase(tEventMap->begin());
      }
    }
  }
  return true;
}

BiotopEventPair CBiotop::getNextUnreadGuiBiotopEvent()
{
  std::map<entityIdType, BiotopEvent_t>* prevMap{ &getBiotopEventMapPrevious() };
  for (std::map<entityIdType, BiotopEvent_t>::iterator it = prevMap->begin(); it != prevMap->end(); ++it)
  {
    if (!it->second.markAsReadByGui)
    {
      it->second.markAsReadByGui = true;
      return { it->first, it->second };
    }
  }
  std::map<entityIdType, BiotopEvent_t>* curMap{ &getBiotopEventMapCurrent() };
  for (std::map<entityIdType, BiotopEvent_t>::iterator it = curMap->begin(); it != curMap->end(); ++it)
  {
    if (!it->second.markAsReadByGui)
    {
      it->second.markAsReadByGui = true;
      return { it->first, it->second };
    }
  }
  return { ENTITY_ID_INVALID, {} };
}

BiotopEventPair CBiotop::getNextUnreadNetworkBiotopEvent()
{
  std::map<entityIdType, BiotopEvent_t>* prevMap{ &getBiotopEventMapPrevious() };
  for (std::map<entityIdType, BiotopEvent_t>::iterator it = prevMap->begin(); it != prevMap->end(); ++it)
  {
    if (!it->second.markAsReadByNetwork)
    {
      it->second.markAsReadByNetwork = true;
      return { it->first, it->second };
    }
  }
  std::map<entityIdType, BiotopEvent_t>* curMap{ &getBiotopEventMapCurrent() };
  for (std::map<entityIdType, BiotopEvent_t>::iterator it = curMap->begin(); it != curMap->end(); ++it)
  {
    if (!it->second.markAsReadByNetwork)
    {
      it->second.markAsReadByNetwork = true;
      return { it->first, it->second };
    }
  }
  return { ENTITY_ID_INVALID, {} };
}

bool CBiotop::resetBiotopEventsMapCurrent()
{
  getBiotopEventMapCurrent().clear();
  return true;
}

std::map<entityIdType, BiotopEvent_t>& CBiotop::getBiotopEventMapCurrent()
{
  return (m_BioTime.seconds % 2 == 0) ? m_tEventsEven : m_tEventsOdd;
}

std::map<entityIdType, BiotopEvent_t>& CBiotop::getBiotopEventMapPrevious()
{
  return (m_BioTime.seconds % 2 == 0) ? m_tEventsOdd : m_tEventsEven;
}

void CBiotop::clearEventMaps()
{
  m_tEventsOdd.clear();
  m_tEventsEven.clear();
}

//===========================================================================
// Save/Load in file
//===========================================================================
bool CBiotop::saveInXmlFile(string fileName, string pathName, bool saveEntities)
{
  bool resu = false;
  TiXmlDocument *pXmlDoc = new TiXmlDocument(pathName + fileName);

  if (pXmlDoc==NULL)
    return false;

  saveInXmlFile(pXmlDoc, pathName, true, saveEntities);

  resu = pXmlDoc->SaveFile();
  delete pXmlDoc;
  return resu;
}

bool CBiotop::saveInXmlFile(TiXmlDocument *pXmlDoc, string pathNameForEntities, bool saveEntityList, bool saveEntities)
{
  TiXmlElement* pElement;
  TiXmlNode* pNodeBiotop = NULL;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeChild = NULL;
  size_t i;
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
    if ((m_GlobalGridDimension.x != m_Dimension.x) || (m_GlobalGridDimension.y != m_Dimension.y))
    {
      pElement->SetAttribute(XML_ATTR_GLOBALSIZE_X, (int)m_GlobalGridDimension.x);
      pElement->SetAttribute(XML_ATTR_GLOBALSIZE_Y, (int)m_GlobalGridDimension.y);
    }
    if ((m_GlobalGridCoordOffset.x > 0) || (m_GlobalGridCoordOffset.y > 0))
    {
      pElement->SetAttribute(XML_ATTR_OFFSET_X, (int)m_GlobalGridCoordOffset.x);
      pElement->SetAttribute(XML_ATTR_OFFSET_Y, (int)m_GlobalGridCoordOffset.y);
    }
  }

  // Clear previous parameters
  while (pNodeBiotop->FirstChild(XML_NODE_PARAMETER) != NULL)
    pNodeBiotop->RemoveChild(pNodeBiotop->FirstChild(XML_NODE_PARAMETER));

  // Save parameters
  for (i=0; i<m_tParam.size(); i++)
  {
    getParameter(i)->saveInXmlFile(pNodeBiotop, true);
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

  if (saveEntityList && (pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
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
        pElement->SetAttribute(XML_ATTR_COORD_X,   (int)pCurEntity->getGlobalStepCoord().x);
        pElement->SetAttribute(XML_ATTR_COORD_Y,   (int)pCurEntity->getGlobalStepCoord().y);
        pElement->SetAttribute(XML_ATTR_LAYER,     (int)pCurEntity->getLayer());
        pElement->SetAttribute(XML_ATTR_DIRECTION, pCurEntity->getDirection());
        pElement->SetAttribute(XML_ATTR_FILE_NAME, entityFileName);

        if (saveEntities && (entityFileName != previousFileName))
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

  m_Label = fileName;
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
    int sizeX, sizeY, nbLayer, offsetX, offsetY, globalSizeX, globalSizeY;
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
    if (pElement->QueryIntAttribute(XML_ATTR_GLOBALSIZE_X, &globalSizeX) == TIXML_NO_ATTRIBUTE)
      globalSizeX = sizeX;
    if (pElement->QueryIntAttribute(XML_ATTR_GLOBALSIZE_Y, &globalSizeY) == TIXML_NO_ATTRIBUTE)
      globalSizeY = sizeY;
    if (pElement->QueryIntAttribute(XML_ATTR_OFFSET_X, &offsetX) == TIXML_NO_ATTRIBUTE)
      offsetX = 0;
    if (pElement->QueryIntAttribute(XML_ATTR_OFFSET_Y, &offsetY) == TIXML_NO_ATTRIBUTE)
      offsetY = 0;

    // Clear Previous Biotop
    clearAllElements();

    m_Dimension.x = sizeX;
    m_Dimension.y = sizeY;
    m_nbLayer = nbLayer;
    m_BioTime = convertCountToBioTime(atoi(timeCountStr.c_str()));
    setGlobalGridDimension(globalSizeX, globalSizeY);
    setGlobalGridCoordOffset({ (size_t)offsetX , (size_t)offsetY });
    buildGrid(m_Dimension.x, m_Dimension.y, m_nbLayer);

    // Parameters management
    string paramName;
    CGenericParam* pParam;
    pNode = pNodeBiotop->FirstChild(XML_NODE_PARAMETER);
    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_PARAMETER))
      {
        pElement = (TiXmlElement*)pNode;
        if ( pElement->QueryStringAttribute(XML_ATTR_NAME,  &paramName) != TIXML_NO_ATTRIBUTE)
        {
          pParam = getParameterByName(paramName);
          if (pParam != NULL)
          {
            pParam->loadFromXmlFile(pNode);
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
    string entityFileName,previousFileName = "";
    CBasicEntity* pEntity = NULL;
    CBasicEntity* pCloneEntity = NULL;
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
            pCloneEntity = createAndAddCloneEntity(pEntity->getId(), gridCoord, layer);
            if (pCloneEntity)
            {
              pCloneEntity->setDirection(direction);
              pCloneEntity->jumpToStepCoord(stepCoord, false);
              // randomize age to prevent threshold effect
              pCloneEntity->quickAgeing(1 + getRandInt((int)(pCloneEntity->getLifeExpectation() * 0.8)));
            }
          }
          else
          {
            pEntity = createAndAddEntity(entityFileName, pathNameForEntities, gridCoord, layer);
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
  CBasicEntity* pEntity = CEntityFactory::createEntity(pathName + entityFileName);
  addEntitySpawner(index, pEntity, intensityRate, avaragePeriod, isProportionalToFertility);
  if (index < m_tRandomEntitiesGeneration.size())
  {
    m_tRandomEntitiesGeneration[index].entityFileName = entityFileName;
    m_tRandomEntitiesGeneration[index].entityPathName = pathName;
    CYBIOCORE_LOG_TIME(m_BioTime);
    CYBIOCORE_LOG("BIOTOP - Add entity spawner : %s index%d \n", entityFileName.c_str(), index);
    return true;
  }
  CYBIOCORE_LOG_TIME(m_BioTime);
  CYBIOCORE_LOG("BIOTOP - FAILURE entity spawner : %s index%d \n", entityFileName.c_str(), index);
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
      CBasicEntity* pNewEntity = CEntityFactory::createCloneEntity(pModelEntity);
      this->addEntity(pNewEntity, coord, pModelEntity->getLayer());
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
// Climate management
//===========================================================================

void CBiotop::setClimateModel(const ClimateType_e newClimateType, const int newPeriod)
{
  int seasonPeriod = (newPeriod > 0) ? newPeriod : (int)getParamFertility()->getPeriod();

  switch (newClimateType)
  {
  case CLIMATE_TROPICAL:
    {
      getParamFertility()->reconfigure(60, 100, seasonPeriod);
      getParamTemperature()->reconfigure(20, 28, seasonPeriod);
      break;
    }
  case CLIMATE_SEMI_ARID:
  {
    getParamFertility()->reconfigure(10, 50, seasonPeriod);
    getParamTemperature()->reconfigure(20, 26, seasonPeriod);
    break;
  }
  case CLIMATE_ARID:
  {
    getParamFertility()->reconfigure(0, 30, seasonPeriod);
    getParamTemperature()->reconfigure(20, 30, seasonPeriod);
    break;
  }
  case CLIMATE_TEMPERATE:
  {
    getParamFertility()->reconfigure(30, 80, seasonPeriod);
    getParamTemperature()->reconfigure(5, 25, seasonPeriod);
    break;
  }
  case CLIMATE_CONTINENTAL:
  {
    getParamFertility()->reconfigure(20, 80, seasonPeriod);
    getParamTemperature()->reconfigure(10, 25, seasonPeriod);
    break;
  }
  case CLIMATE_POLAR:
  {
    getParamFertility()->reconfigure(10, 30, seasonPeriod);
    getParamTemperature()->reconfigure(-10, 10, seasonPeriod);
    break;
  }
  default:
    {
      break;
    }
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

const Point_t& CBiotop::getDimension() const
{
  return m_Dimension;
}

const Point_t& CBiotop::getGlobalGridDimension() const
{
  return m_GlobalGridDimension;
}

double CBiotop::getFertility(const Point_t& coord)
{
  if (isCoordValid(coord))
  {
    double fertility = m_pFertilityRate->getVal() + m_tBioSquare[coord.x][coord.y].fertilityBonus;
    fertility = (fertility > 100) ? 100 : fertility;
    fertility = (fertility < 0) ? 0 : fertility;
    return fertility;
  }
  return m_pFertilityRate->getVal();
}

void CBiotop::setFertilityRate(int fertilityRate)
{
  m_pFertilityRate->setVal(fertilityRate);
}

double CBiotop::getRadioactivityRate()
{
  return m_pRadioactivity->getVal();
}

double CBiotop::getTemperature(const Point_t& coord, size_t layer)
{
  double computedTemperature = m_pTemperature->getVal();

#ifdef NON_OPTIMIZED_CPU_MODE
  ClassType_e entityClass{CLASS_UNSET};
  const BiotopFoundIds_t& biotopFoundIds = findEntities(coord, 1, true);
  const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;

  // Give bonus malus on temperature according to entities around
  for (size_t ind = 0; ind < biotopFoundIds.nbFoundIds; ind++)
  {
    entityClass = tFoundIds[ind].pEntity->getClass();
    if (entityClass >= CLASS_ANIMAL_FIRST)
      computedTemperature += 2.0;
    else if (entityClass >= CLASS_VEGETAL_FIRST)
      computedTemperature -= 1.0;
    else if (entityClass == CLASS_WATER)
      computedTemperature -= 2.0;
  }
#endif

  // Check if shadow
  Point_t southPos = coord;
  southPos.y -= 1;
  CBasicEntity* pCurEntity = findTopLevelEntity(southPos);
  if ((pCurEntity != NULL) && (pCurEntity->getLayer() >= layer))
    computedTemperature += m_pSunlightRate->getVal()/20.0 - 5.0;
  else
    computedTemperature += m_pSunlightRate->getVal()/10.0 - 5.0;

  // TBD: Altitude may be used later

  return computedTemperature;
}

double CBiotop::getSunlight()
{
  // TBD: Shadow may be used later
  return m_pSunlightRate->getVal();
}

LayerType_e CBiotop::getLayerType(const Point_t& coord, size_t layer)
{
  return (isCoordValid(coord, layer)) ? m_tBioGrid[coord.x][coord.y][layer].layerType : LAYER_UNDER_GROUND;
}

size_t CBiotop::getNbLayer(void)
{
  return m_nbLayer;
}

CCyclicParam* CBiotop::getParamFertility()
{
  return m_pFertilityRate;
}

CCyclicParam* CBiotop::getParamSunlight()
{
  return m_pSunlightRate;
}

CGenericParam* CBiotop::getParamRadioactivity()
{
  return m_pRadioactivity;
}

CCyclicParam* CBiotop::getParamTemperature()
{
  return m_pTemperature;
}

CGenericParam* CBiotop::getParameter(size_t id)
{
  return (id > m_tParam.size()) ? NULL : m_tParam[id];
}

CGenericParam* CBiotop::getParameterByName(string& paramName)
{
  for (auto pParam : m_tParam)
  {
    if (pParam->getLabel() == paramName)
      return pParam;
  }
  return NULL;
}

double CBiotop::getOdorTrace(const Point_t& coord, size_t odorIndex)
{
  return isCoordValid(coord) ? ((double)m_tBioSquare[coord.x][coord.y].odorTrace[odorIndex] / MAX_ODOR_TRACE_VAL) : 0;
}

bool CBiotop::getOdorLevels(const Point_t& coord, int range, double odorLevel[NUMBER_ODORS], entityIdType excludedEntityId)
{
  // Init to 0
  int i;
  for (i=0; i<NUMBER_ODORS; i++)
    odorLevel[i]=0;

  Point_t coordWind = coord;
  if (m_WindStrenght>0)
  {
    coordWind.x -= vectorDirection[m_WindDirection].x;
    coordWind.y -= vectorDirection[m_WindDirection].y;
  }

  // Find entities
  CBasicEntity* pCurEntity = NULL;

  const BiotopFoundIds_t& biotopFoundIds = findEntities(coordWind, range);
  const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;

  for (size_t ind = 0; ind < biotopFoundIds.nbFoundIds; ind++)
  {
    pCurEntity = tFoundIds[ind].pEntity;
    if ((pCurEntity!=NULL) && !pCurEntity->isToBeRemoved() && (pCurEntity->getOdor() > ODOR_NONE) && (pCurEntity->getId() != excludedEntityId))
    {
      odorLevel[OdorTypeToIndex(pCurEntity->getOdor())] += MAX_SENSOR_VAL / ((double)tFoundIds[ind].distance + 2); // 1/R
    }
  }

  // Add trace odor
  for (i=0; i<NUMBER_ODORS; i++)
  {
    // Add odor trace
    odorLevel[i] += getOdorTrace(coord, i);

    // Use first threshold
    if (odorLevel[i] > MAX_SENSOR_VAL)
      odorLevel[i] = MAX_SENSOR_VAL;
  }

  return true;
}

COLORREF CBiotop::getCustomColor(const Point_t& coord)
{
  // coord may be used later
  return isCoordValid(coord) ? (m_tBioSquare[coord.x][coord.y].customColor) : 0;
}

BiotopSquare_t** CBiotop::getpBioSquare()
{
  return m_tBioSquare;
}

string CBiotop::getLabel()
{
  return m_Label;
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

BiotopRandomEntitiyGeneration_t& CBiotop::getRandomEntitiyGeneration(size_t index)
{
  return m_tRandomEntitiesGeneration[index];
}

size_t CBiotop::getNumberOfRandomEntitiyGeneration()
{
  return m_tRandomEntitiesGeneration.size();
}

void CBiotop::SetColorizeSearchMode(bool bColorizeSearch)
{
  m_bColorizeSearch = bColorizeSearch;
}

CGene& CBiotop::getGeneToMark()
{
  return m_GeneToMark;
}

bool CBiotop::getMarkDominantAlleleOnly()
{
  return m_bMarkDominantAlleleOnly;
}

void CBiotop::setGlobalGridDimension(size_t dimX, size_t dimY)
{
  m_GlobalGridDimension.x = dimX;
  m_GlobalGridDimension.y = dimY;
}

void CBiotop::setGlobalGridCoordOffset(const Point_t& startingCoord)
{
  m_GlobalGridCoordOffset = startingCoord;
  m_GlobalStepCoordOffset.x = startingCoord.x * NB_STEPS_PER_GRID_SQUARE;
  m_GlobalStepCoordOffset.y = startingCoord.y * NB_STEPS_PER_GRID_SQUARE;
}

void CBiotop::setAutoSaveSpecieName(string specieName)
{
  m_AutoSaveSpecieName = specieName;
}

