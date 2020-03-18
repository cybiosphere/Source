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
// FILE: CSensorPleasure.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for Pleasure
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CSensorPleasure.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorPleasure::CSensorPleasure
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
CSensorPleasure::CSensorPleasure(CBrainAnimal* pBrain, double weightRate)
:CSensor(weightRate, UID_BASE_SENS_PLEASURE)
{
  m_pBrain = pBrain;
  m_Label = "Pleasure";
}


//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorPleasure::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to entity pleasure 
//               The Weight of captor is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorPleasure::UpdateAndGetStimulationTable()
{
  m_tStimulationValues[0] = (double)(m_pBrain->getAnimal()->getPleasureRate()) * MAX_SENSOR_VAL / 100.0;
  applySubCaptorWeightRate();

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorPleasure::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorPleasure::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) )
    return ("bad index");
  else
    return ("level");
}