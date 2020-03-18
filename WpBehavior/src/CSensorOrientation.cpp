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
// FILE: CSensorOrientation.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for hunger
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CSensorOrientation.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CGeoMap.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorOrientation::CSensorOrientation
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
CSensorOrientation::CSensorOrientation(CBrainAnimal* pBrain, double pWeightRate[NUMBER_ORIENTATION_SUB_CAPTORS])
:CSensor(NUMBER_ORIENTATION_SUB_CAPTORS, pWeightRate, UID_BASE_SENS_ORIENTATION)
{
  m_pBrain = pBrain;
  m_Label = "Guide";
}

CSensorOrientation::~CSensorOrientation()
{

}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorOrientation::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to entity orientation 
//               The Weight of captor is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorOrientation::UpdateAndGetStimulationTable()
{ 
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CPurpose* pPurpose = m_pBrain->GetCurrentPurpose();
  CGeoMap*  pMap = m_pBrain->GetGeographicMap();

  if ( (pPurpose != NULL) && (pMap != NULL) && (pPurpose->IsMemorizeSuccess()) )
  {
    int targetDirection;
    int curDirection = m_pBrain->getAnimal()->getDirection();
    GeoMapIntensityType_e intensity = pMap->GetClosestSuccessPos(pPurpose->GetUniqueId(), m_pBrain->getAnimal()->getGridCoord(), targetDirection);
    if (intensity>FOUND_INTENSITY_NULL)
    {
      if (targetDirection == curDirection)
      {
        m_tStimulationValues[0] = MAX_SENSOR_VAL*intensity/FOUND_INTENSITY_HIGH;
      }
      else if (((360 + (targetDirection-curDirection)*45) % 360) < 180)
      {
        m_tStimulationValues[1] = MAX_SENSOR_VAL*intensity/FOUND_INTENSITY_HIGH;
      }
      else
      {
        m_tStimulationValues[2] = MAX_SENSOR_VAL*intensity/FOUND_INTENSITY_HIGH;
      }

      applySubCaptorWeightRate();
    }
  }

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorOrientation::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorOrientation::GetSubCaptorLabel(int index)
{
  string label;

  switch(index)
  {
  case 0:  label = "front";    break;
  case 1:  label = "left";     break;
  case 2:  label = "right";    break;
  default: label = "bad index";break;
  }

  return label;
}