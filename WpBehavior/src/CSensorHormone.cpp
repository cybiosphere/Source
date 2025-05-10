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
// FILE: CSensorHormone.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used for smell sens
//         It is a set of sub-sensors dedicated for the pheromones:
//         A weight table must be used to set savour sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#include "CSensorHormone.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorHormone::CSensorHormone
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
CSensorHormone::CSensorHormone(CBrainAnimal* pBrain, std::vector<double>& tWeightRate)
:CSensor(NUMBER_HORMONES, tWeightRate, UID_BASE_SENS_HORMONE )
{
  m_pBrain = pBrain;
  m_Label = "Hormone";
}

CSensorHormone::~CSensorHormone()
{
}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorHormone::UpdateAndGetStimulationTable
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
const std::vector<sensorValType>& CSensorHormone::UpdateAndGetStimulationTable()
{
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();

  // Process level
  if (pAnimal->getPheromone() >= PHEROMONE_FIRST_TYPE)
    m_tStimulationValues[PheromoneTypeToIndex(pAnimal->getPheromone())] = MAX_SENSOR_VAL;

  applySubCaptorWeightRate();

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorHormone::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorHormone::GetSubCaptorLabel(size_t index)
{
  if (index>GetSubCaptorNumber())
    return ("bad index");
  else
  {
    PheromoneType_e pheromoneId = PHEROMONE_FIRST_TYPE;
    for (size_t i=0; (i<index)&&(i<PHEROMONE_NUMBER_TYPE); i++)
      pheromoneId = (PheromoneType_e)(pheromoneId+1);
    return (CBasicEntity::getPheromoneStrName(pheromoneId) );
  }
}

//---------------------------------------------------------------------------
// METHOD:       CSensorHormone::IsSexSpecific
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
bool CSensorHormone::IsSexSpecific(size_t captorIndex)
{
  return true;
}