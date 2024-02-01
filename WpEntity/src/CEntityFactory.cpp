/*
https://github.com/cybiosphere
copyright (c) 2020 Frederic RIBERT

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
// FILE: CEntityFactory.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS is used to build CEntity classes
//     
// (C) COPYRIGHT 2020.  All Rights Reserved.
//
//  20/08/2020 : Creation
//
//===========================================================================
 
#include "CEntityFactory.h"


CBasicEntity* CEntityFactory::createEntity(string name, CGenome* pGenome)
{
  CBasicEntity* pNewEntity = NULL;

  // Set defaut coord
  Point_t coord = { invalidCoord, invalidCoord };
  size_t layer = invalidCoord;

  // Create the BasicEntity derived object
  ClassType_e entityClass = (pGenome != NULL) ? pGenome->getClass() : CLASS_SOLID;
  switch (entityClass)
  {
    // Vegetable classs
  case CLASS_PHYCOPHYTA:
  case CLASS_MYCOPHYTA:
  case CLASS_BRYOPHYTA:
  case CLASS_PTERIDOPHYTA:
    pNewEntity = new CVegetable(name, coord, layer, pGenome);
    break;

  case CLASS_SPERMATOPHYTA:
    pNewEntity = new CVegetSpermatophyta(name, coord, layer, pGenome);
    break;

    // Animal classs
  case CLASS_MAMMAL:
    pNewEntity = new CAnimMammal(name, coord, layer, pGenome);

    break;

  case CLASS_REPTILE:
  case CLASS_ARTHROPOD:
  case CLASS_AVE:
    pNewEntity = new CAnimal(name, coord, layer, pGenome);
    break;

  case CLASS_WATER:
    pNewEntity = new CWater(name, coord, pGenome);
    break;

  default:
    pNewEntity = new CMineral(name, coord, layer, pGenome);

  }

  if (pNewEntity == NULL)
    return (NULL);

  // Initialise the new entity
  pNewEntity->setEntityFromGenome();

  return (pNewEntity);
}

CBasicEntity* CEntityFactory::createEntity(string fileNameWithPath)
{
  TiXmlDocument* pXmlDoc = new TiXmlDocument(fileNameWithPath);
  if (!pXmlDoc->LoadFile())
  {
    CYBIOCORE_LOG("CHECK - Error reading entity file: %s\n", fileNameWithPath.c_str());
    delete pXmlDoc;
    return NULL;
  }
  CBasicEntity* pNewEntity = CEntityFactory::createEntity(pXmlDoc);
  delete pXmlDoc;
  return (pNewEntity);
}

CBasicEntity* CEntityFactory::createEntity(TiXmlDocument* pXmlDoc)
{
  string name;
  CGenome* pTempGenome = new CGenome(CLASS_NONE, "");
  int startLayer;
  CBasicEntity::getDefaultLayerFromXmlFile(pXmlDoc, startLayer);
  CBasicEntity::getEntityNameFromXmlFile(pXmlDoc, name);

  if (CBasicEntity::getGenomeFromXmlFile(pXmlDoc, *pTempGenome) == false)
  {
    delete pTempGenome;
    pTempGenome = NULL;
  }

  CBasicEntity* pNewEntity = createEntity(name, pTempGenome);
  pNewEntity->jumpToGridCoord({ invalidCoord, invalidCoord }, true, startLayer);

  if (pNewEntity != NULL)
  {
    pNewEntity->loadDataFromXmlFile(pXmlDoc);
    pNewEntity->loadBrainFromXmlFile(pXmlDoc);
  }
  else if (pTempGenome != NULL)
  {
    delete pTempGenome;
  }

  return (pNewEntity);
}


CBasicEntity* CEntityFactory::createCloneEntity(CBasicEntity* pModelEntity)
{
  if (pModelEntity == NULL)
    return (NULL);

  CGenome* pGenome = NULL;
  if (pModelEntity->getGenome() != NULL)
    pGenome = new CGenome(*pModelEntity->getGenome());

  CBasicEntity* pNewEntity = createEntity(pModelEntity->getLabel(), pGenome);

  if (pNewEntity == NULL)
  {
    if (pGenome != NULL)
      delete pGenome;

    return (NULL);
  }

  // quick aging
  pNewEntity->quickAgeing(pModelEntity->getAge());

  // If dead quick aging in dead mode
  if (pModelEntity->getStatus() == STATUS_DEAD)
  {
    pNewEntity->autoKill();
    pNewEntity->quickAgeing(pModelEntity->getDecompositionTime());
  }

  // Copy caracters
  pNewEntity->setOdor(pModelEntity->getOdor());
  pNewEntity->setForm(pModelEntity->getForm());
  pNewEntity->setTaste(pModelEntity->getTaste());
  pNewEntity->setTypeOfReproduction(pModelEntity->getTypeOfReproduction());
  pNewEntity->setPheromone(pModelEntity->getPheromone());
  pNewEntity->setColor(pModelEntity->getColorRgb());
  pNewEntity->setTexture(pModelEntity->getTexture());
  
  // Copy parameters
  CGenericParam* pParam;
  CGenericParam* pModelParam;
  for (size_t i = 0; i < pNewEntity->getNumParameter(); i++)
  {
    pParam = pNewEntity->getParameter(i);
    pModelParam = pModelEntity->getParameter(i);
    pParam->setValMin(pModelParam->getMin());
    pParam->setValMax(pModelParam->getMax());
    pParam->setValNominal(pModelParam->getValNominal());
    pParam->forceVal(pModelParam->getVal());
  }

  // Copy other infos
  pNewEntity->setStatus(pModelEntity->getStatus());
  pNewEntity->setGeneration(pModelEntity->getGeneration());

  return pNewEntity;
}