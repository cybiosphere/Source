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
// FILE: CSensorSpeed.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for hunger
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CSensorSpeed.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorSpeed::CSensorSpeed
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
CSensorSpeed::CSensorSpeed(CBrainAnimal* pBrain, std::vector<double>& tWeightRate)
:CSensor(NUMBER_SPEED_SUB_CAPTORS,tWeightRate, UID_BASE_SENS_SPEED)
{
  m_pBrain = pBrain;
  m_Label = "Movt";
}

CSensorSpeed::~CSensorSpeed()
{

}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorSpeed::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to entity libido 
//               The Weight of captor is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorSpeed::UpdateAndGetStimulationTable()
{ 
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  int curSpeed = m_pBrain->getAnimal()->getCurrentSpeed();

  if (curSpeed == 0)
    m_tStimulationValues[0] = MAX_SENSOR_VAL;
  else if (abs(curSpeed) < NB_STEPS_PER_GRID_SQUARE)
    m_tStimulationValues[1] = MAX_SENSOR_VAL * (5 + curSpeed/2) / 10;
  else
    m_tStimulationValues[2] = MAX_SENSOR_VAL * (5 + curSpeed/NB_STEPS_PER_GRID_SQUARE) / 10;

  applySubCaptorWeightRate();

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorSpeed::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorSpeed::GetSubCaptorLabel(int index)
{
  string label;

  switch(index)
  {
  case 0:  label = "stopped";   break;
  case 1:  label = "walking";   break;
  case 2:  label = "running";   break;
  default: label = "bad index"; break;
  }

  return label;
}