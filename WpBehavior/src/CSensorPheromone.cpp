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
// FILE: CSensorPheromone.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used for smell sens
//         It is a set of 4 sub-sensors dedicated for the 4 pheromones:
//         A 4 sized weight table must be used to set savour sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#include "CSensorPheromone.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CVirus.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorPheromone::CSensorPheromone
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[NUMBER_PHEROMONES] : table of weight for each pheromone
//               int range: Distance scanned to find pheromone
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorPheromone::CSensorPheromone(CBrainAnimal* pBrain, std::vector<double>& tWeightRate, int range)
:CSensor(NUMBER_PHEROMONES, tWeightRate, UID_BASE_SENS_PHEROMONE /*+ range*/)
{
  m_pBrain = pBrain;
  m_Label = "Pheromone";
  m_nRange = range;
}

CSensorPheromone::~CSensorPheromone()
{
}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorPheromone::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to smelled entities 
//               The Weight of captors is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorPheromone::UpdateAndGetStimulationTable()
{
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();
  CBasicEntity* pCurEntity = NULL;

  // Process level
  RelativePos_t relPos = {1, 0};
  const BiotopFoundIds_t& biotopFoundIds = pAnimal->getBiotop()->findEntities(pAnimal->getGridCoordRelative(relPos), m_nRange);
  const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;

  for (size_t ind = 0; ind < biotopFoundIds.nbFoundIds; ind++)
  {
    pCurEntity = tFoundIds[ind].pEntity;
    if ((pCurEntity!=NULL) && (pCurEntity!=pAnimal)) // Do not take into account my own pheromon
    {
      if (pAnimal->getGenome()->checkSpecieCompatibility(pCurEntity->getGenome()) == true)
      {
        if (pCurEntity->getPheromone() > PHEROMONE_NONE)
          m_tStimulationValues[PheromoneTypeToIndex(pCurEntity->getPheromone())] += MAX_SENSOR_VAL / (tFoundIds[ind].distance + 1) / (tFoundIds[ind].distance + 1); // 1/R2

        // Check possible infection by parasite
        if (pCurEntity->hasParasite())
        {
          // Todo: check parasite transmition mode (air)
          CVirus* pParasite = (CVirus*)pCurEntity->getParasite();
          if (tFoundIds[ind].distance <= pParasite->getReproductionRange())
          {
            pAnimal->tryInfectionByParasite(pParasite);
          }
        }
      }
    }
  }

  applySubCaptorWeightRate();
  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorPheromone::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorPheromone::GetSubCaptorLabel(size_t index)
{
  if (index>GetSubCaptorNumber())
    return ("bad index");
  else
  {
    PheromoneType_e pheromoneId = (PheromoneType_e)(index + 1);
    return (CBasicEntity::getPheromoneStrName(pheromoneId));
  }
}

//---------------------------------------------------------------------------
// METHOD:       CSensorPheromone::IsSexSpecific
//  
// DESCRIPTION:  Return true if the sub-captor is used a different way 
//               according to sex
//
// ARGUMENTS:    int captorIndex : index of the sub captor (def = 0)
//   
// RETURN VALUE: true: sex specific, false: no sexual differenciation
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CSensorPheromone::IsSexSpecific(size_t captorIndex)
{
  return true;
}