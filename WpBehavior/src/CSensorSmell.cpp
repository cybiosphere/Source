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
// FILE: CSensorSmell.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used for smell sens
//         It is a set of 7 sub-sensors dedicated for the 7 odors:
//         AROMATIC(0),...NAUSEOUS(6)
//         A 7 sized weight table must be used to set savour sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#include "CSensorSmell.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorSmell::CSensorSmell
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
CSensorSmell::CSensorSmell(CBrainAnimal* pBrain, std::vector<double>& tWeightRate, int range)
:CSensor(NUMBER_ODORS, tWeightRate, UID_BASE_SENS_SMELL /*+ range*/)
{
  m_pBrain = pBrain;
  m_Label = "Smell";
  m_nRange = range;
}

CSensorSmell::~CSensorSmell()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorSmell::UpdateAndGetStimulationTable
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
const std::vector<sensorValType>& CSensorSmell::UpdateAndGetStimulationTable()
{
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();

  // Process level
  Point_t relPos = {1,0};
  Point_t frontPos = pAnimal->getGridCoordRelative(relPos);

  // Get odor in biotop, excluding own odor
  pAnimal->getBiotop()->getOdorLevels(frontPos, m_nRange, &(m_tStimulationValues[0]), pAnimal->getId());
  
  applySubCaptorWeightRate();

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorSmell::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorSmell::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) )
    return ("bad index");
  else
  {
    OdorType_e odorId = ODOR_FIRST_TYPE;
    for (int i=0; (i<index)&&(i<ODOR_NUMBER_TYPE); i++)
      odorId = (OdorType_e)(odorId+1);
    return (CBasicEntity::getOdorStrName(odorId) );
  }
}
