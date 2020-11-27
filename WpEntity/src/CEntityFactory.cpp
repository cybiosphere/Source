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
  if (pGenome == NULL)
    return (NULL);

  CBasicEntity* pNewEntity = NULL;

  // Set defaut coord
  Point_t coord = { invalidCoord, invalidCoord };
  size_t layer = invalidCoord;

  // Create the BasicEntity derived object
  switch (pGenome->getClass())
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
  CBasicEntity::getGenomeFromXmlFile(pXmlDoc, *pTempGenome);
  CBasicEntity::getEntityNameFromXmlFile(pXmlDoc, name);

  CBasicEntity* pNewEntity = createEntity(name, pTempGenome);
  pNewEntity->jumpToGridCoord({ invalidCoord, invalidCoord }, true, startLayer);

  if (pNewEntity != NULL)
  {
    pNewEntity->loadDataFromXmlFile(pXmlDoc);
    pNewEntity->loadBrainFromXmlFile(pXmlDoc);
  }
  else
  {
    delete pTempGenome;
  }

  return (pNewEntity);
}


CBasicEntity* CEntityFactory::createCloneEntity(CBasicEntity* pModelEntity)
{
  CBasicEntity* pNewEntity = NULL;
  string name = pModelEntity->getLabel();

  if (pModelEntity->getGenome() == NULL)
  {
    pNewEntity = new CBasicEntity(); // FRED Should use constructor with copy caracters
    if (pNewEntity == NULL)
      return (NULL);
    pNewEntity->setLabel(name);
  }
  else
  {
    // Create the BasicEntity derived object
    switch (pModelEntity->getGenome()->getClass())
    {
      // Vegetable classs
    case CLASS_PHYCOPHYTA:
    case CLASS_MYCOPHYTA:
    case CLASS_BRYOPHYTA:
    case CLASS_PTERIDOPHYTA:
      pNewEntity = new CVegetable(name, *(CVegetable*)pModelEntity);
      break;

    case CLASS_SPERMATOPHYTA:
      pNewEntity = new CVegetSpermatophyta(name, *(CVegetSpermatophyta*)pModelEntity);
      break;

      // Animal classs
    case CLASS_MAMMAL:
      pNewEntity = new CAnimMammal(name, *(CAnimMammal*)pModelEntity);
      break;

    case CLASS_REPTILE:
    case CLASS_ARTHROPOD:
    case CLASS_AVE:
      pNewEntity = new CAnimal(name, *(CAnimal*)pModelEntity);
      break;

    case CLASS_WATER:
      pNewEntity = new CWater(name, *(CWater*)pModelEntity);
      break;

    default:
      pNewEntity = new CMineral(name, *(CMineral*)pModelEntity);

    }

    if (pNewEntity == NULL)
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
  for (size_t i = 0; i < pNewEntity->getNumParameter(); i++)
    pNewEntity->getParameter(i)->forceVal(pModelEntity->getParameter(i)->getVal());

  // Copie Status
  pNewEntity->setStatus(pModelEntity->getStatus());

  return pNewEntity;
}