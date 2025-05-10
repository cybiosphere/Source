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
// FILE: CSensorTemperature.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for hunger
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CSensorTemperature.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CPhysicalWelfare.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorTemperature::CSensorTemperature
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double weightRate: {0..100} - Sum of all weights should be 100
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorTemperature::CSensorTemperature(CBrainAnimal* pBrain, std::vector<double>& tWeightRate)
:CSensor(NUMBER_TEMPERATURE_SUB_CAPTORS,tWeightRate, UID_BASE_SENS_TEMPERATURE)
{
  m_pBrain = pBrain;
  m_Label  = "Temperature";
}

CSensorTemperature::~CSensorTemperature()
{
}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorTemperature::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to entity hunger 
//               The Weight of captor is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorTemperature::UpdateAndGetStimulationTable()
{
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();
  CPhysicalWelfare* pPhyWelfare = pAnimal->getpPhysicalWelfare();
  double biotopTemperature = pAnimal->getBiotop()->getTemperature(pAnimal->getGridCoord(), pAnimal->getLayer());

  if(biotopTemperature > pPhyWelfare->GetTemperatureMax())
    m_tStimulationValues[TEMPERATURE_TOO_HOT]  = (biotopTemperature - pPhyWelfare->GetTemperatureMax()) * MAX_SENSOR_VAL/5;

  if(biotopTemperature < pPhyWelfare->GetTemperatureMin())
    m_tStimulationValues[TEMPERATURE_TOO_COLD] = (pPhyWelfare->GetTemperatureMin() - biotopTemperature) * MAX_SENSOR_VAL/5;

  LayerType_e curLayer = pAnimal->getBiotop()->getLayerType(pAnimal->getGridCoord(), 1);
  if (curLayer == LAYER_OVER_WET_GROUND)
    m_tStimulationValues[TEMPERATURE_COLD_WATER] = MAX_SENSOR_VAL/4;
  else if(curLayer == LAYER_OVER_WATER)
    m_tStimulationValues[TEMPERATURE_COLD_WATER] = MAX_SENSOR_VAL;

  applySubCaptorWeightRate();

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorTemperature::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorTemperature::GetSubCaptorLabel(size_t index)
{
  string label;

  switch(index)
  {
  case TEMPERATURE_TOO_HOT:  label = "too hot";   break;
  case TEMPERATURE_TOO_COLD:  label = "too cold";   break;
  case TEMPERATURE_COLD_WATER:  label = "cold water";   break;
  default: label = "bad index"; break;
  }

  return label;
}