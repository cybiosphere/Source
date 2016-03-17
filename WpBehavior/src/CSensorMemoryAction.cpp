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
// FILE: CSensorMemoryAction.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used for smell sens
//         It is a set of n sub-sensors dedicated for each action executed
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#include "CSensorMemoryAction.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorMemoryAction::CSensorMemoryAction
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorMemoryAction::CSensorMemoryAction(CBrainAnimal* pBrain, double weightRate)
:CSensor(pBrain->GetNumberReaction(), weightRate, UID_BASE_SENS_MEMORY_ACT)
{
  m_pBrain = pBrain;
  m_Label = "Memory action";
}

CSensorMemoryAction::~CSensorMemoryAction()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorMemoryAction::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to previous action 
//               The Weight of captors is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
int CSensorMemoryAction::UpdateAndGetStimulationTable(sensorValType*& pStimulationVal)
{
  for (int i=0; i<m_SubCaptorNumber; i++)
  {
    m_pStimulationValues[i] = 0;
  }

  int reactInd = m_pBrain->GetCurrentReactionIndex();

  if ( (reactInd>=0) && (reactInd<GetSubCaptorNumber()) )
    m_pStimulationValues[reactInd] = m_pSubCaptorWeightRate[reactInd] * MAX_SENSOR_VAL / 100.0;

  pStimulationVal = m_pStimulationValues;
  return m_SubCaptorNumber;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorMemoryAction::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorMemoryAction::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) || (index>m_pBrain->GetNumberReaction()) )
    return ("bad index");
  else
  {
    return (m_pBrain->GetReactionByIndex(index)->GetLabel());
  }
}
