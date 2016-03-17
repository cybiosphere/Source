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
// FILE: CSensorTactile.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for touching
//         It is a set of n sub-sensors dedicated for n layers in front of animal
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/10/2005 : Creation
//
//===========================================================================

#include "CSensorTactile.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorTactile::CSensorTactile
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[NUMBER_ODORS] : table of weight for each odor
//               int range: Distance scanned to find odor
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorTactile::CSensorTactile(CBrainAnimal* pBrain, double pWeightRate[], int relBottomLayer, int relTopLayer)
:CSensor(relTopLayer-relBottomLayer+1, pWeightRate, UID_BASE_SENS_TACTILE + 100*relBottomLayer + relTopLayer)
{
  m_pBrain = pBrain;
  m_Label = FormatString("Tactile");  
  m_relBottomLayer = relBottomLayer;
  m_relTopLayer = relTopLayer;
}

CSensorTactile::~CSensorTactile()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorTactile::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to touched entities 
//               The Weight of captors is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
int CSensorTactile::UpdateAndGetStimulationTable(sensorValType*& pStimulationVal)
{
  int i;
  for (i=0; i<m_SubCaptorNumber; i++)
  {
    m_pStimulationValues[i] = 0;
  }

  CAnimal* pAnimal = m_pBrain->getAnimal();
  Point_t relPos = {1,0};
  Point_t relCoord = pAnimal->getGridCoordRelative(relPos);
  CBasicEntity* pTouchedEntity;

  // Process level
  for (int layer=0; layer<(m_relTopLayer-m_relBottomLayer+1); layer++)
  {
    pTouchedEntity = pAnimal->getBiotop()->findEntity(relCoord,pAnimal->getLayer()+m_relBottomLayer+layer);
    if (pTouchedEntity != NULL)
    {
      m_pStimulationValues[layer] = MAX_SENSOR_VAL; // bool 0 or MAX_SENSOR_VAL
    } 
  }
  
  for (i=0; i<m_SubCaptorNumber; i++)
  {
    // Use weight
    m_pStimulationValues[i] = m_pStimulationValues[i] * m_pSubCaptorWeightRate[i] / 100.0;
    // Don't go over Max!
    if (m_pStimulationValues[i] > MAX_SENSOR_VAL)
      m_pStimulationValues[i] = MAX_SENSOR_VAL;
  }

  pStimulationVal = m_pStimulationValues;
  return m_SubCaptorNumber;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorTactile::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorTactile::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) )
    return ("bad index");
  else
  {
    string label;
    label = FormatString("position %d",m_relBottomLayer+index);
    return (label);
  }
}