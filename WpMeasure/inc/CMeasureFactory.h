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
// FILE: CMeasureFactory.h
//   
// GENERAL DESCRIPTION:
//         This CLASS is used to build Measure classes
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#if !defined(CMEASUREFACTORY_INCLUDED_)
#define CMEASUREFACTORY_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "Definitions.h"
#include "CMeasure.h"
#include "CMeasureBiotopParam.h"
#include "CMeasureEntityParam.h"
#include "CMeasureFeeling.h"
#include "CMeasureParam.h"
#include "CMeasurePopulation.h"
#include "CMeasureReaction.h"

//===========================================================================
//                                    CLASS            
//===========================================================================

class CMeasureFactory
{
public:
  static CMeasure* createMeasure(int id, int period, unsigned int typeSubtype, CBiotop* pBiotop, int paramId = -1, int entityId = -1)
  {
    CMeasure* pNewMeasure = NULL;
    MeasureType_e  type = (MeasureType_e)(typeSubtype & 0xFF);
    unsigned char subtype = (typeSubtype >> 8) & 0xFF;
    switch (type)
    {
    case MEASURE_TYPE_PARAMETER_ENTITY:
      if (pBiotop != NULL)
      {
        CBasicEntity* pEntity = pBiotop->getEntityById(entityId);
        pNewMeasure = new CMeasureEntityParam(pEntity, paramId, period, id);
      }
      break;
    case MEASURE_TYPE_PARAMETER_BIOTOP:
      if (pBiotop != NULL)
      {
        pNewMeasure = new CMeasureBiotopParam(pBiotop, paramId, period, id);
      }
      break;
    case MEASURE_TYPE_FEELING:
      if (pBiotop != NULL)
      {
        CBasicEntity* pEntity = pBiotop->getEntityById(entityId);
        pNewMeasure = new CMeasureFeeling(pEntity, period, id, (MeasureFeelingType_e)subtype);
      }
      break;
    case MEASURE_TYPE_REACTION:
      if (pBiotop != NULL)
      {
        CBasicEntity* pEntity = pBiotop->getEntityById(entityId);
        pNewMeasure = new CMeasureReaction(pEntity, paramId, period, id, (MeasureReactionType_e)subtype);
      }
      break;
    case MEASURE_TYPE_POPULATION:
      if (pBiotop != NULL)
      {
        pNewMeasure = new CMeasurePopulation(pBiotop, period, id, (MeasurePopulationType_e)subtype, 1000, "tobedefined");
      }
      break;
    }
    return pNewMeasure;
  };

};

#endif // !defined(CMEASUREFACTORY_INCLUDED_)
