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
// FILE: CSensorCompass.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for Ear
//         It is a set of Y = 8 sub-sensors dedicated, for each direction
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  10/10/2008 : Creation
//
//===========================================================================

#include "CSensorCompass.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorCompass::CSensorCompass
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[] : table of weight for each sector
//               int range: Distance scanned to find entities
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorCompass::	CSensorCompass(CBrainAnimal* pBrain, double* pWeightRate)
:CSensor(8, pWeightRate, UID_BASE_SENS_COMPASS)
{
  m_pBrain = pBrain;
  m_Label = "Compass";
}

CSensorCompass::~CSensorCompass()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorCompass::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to Eared entities
//               The Weight of captors is taken into account here 
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorCompass::UpdateAndGetStimulationTable()
{
  // Reinit m_tStimulationValues
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();
  m_tStimulationValues[pAnimal->getDirection()] = MAX_SENSOR_VAL;
  applySubCaptorWeightRate();

  return m_tStimulationValues;
}

//===========================================================================
// other functions
//===========================================================================


//---------------------------------------------------------------------------
// METHOD:       CSensorCompass::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorCompass::GetSubCaptorLabel(int index)
{
  return (CBasicEntity::getAbsolutePosStrName(index));
}