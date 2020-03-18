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
// FILE: CSensorMemoryFeedback.cpp
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

#include "CSensorMemoryFeedback.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorMemoryFeedback::CSensorMemoryFeedback
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorMemoryFeedback::CSensorMemoryFeedback(CBrainAnimal* pBrain, double weightRate)
:CSensor(pBrain->GetNumberReaction(), weightRate, UID_BASE_SENS_MEMORY_FB)
{
  m_pBrain = pBrain;
  m_Label = "Memory feedback";
}

CSensorMemoryFeedback::~CSensorMemoryFeedback()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorMemoryFeedback::UpdateAndGetStimulationTable
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
const std::vector<sensorValType>& CSensorMemoryFeedback::UpdateAndGetStimulationTable()
{
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  int reactInd = m_pBrain->GetCurrentReactionIndex();

  if ( (reactInd>=0) && (reactInd<GetSubCaptorNumber()) )
  {
    double curFeedback = m_pBrain->GetCurrentFeedback();
    // give more weight to negative feedbacks according to consecutive failures
    if (curFeedback<=0)
    {
      curFeedback = curFeedback - m_pBrain->GetReactionByIndex(reactInd)->GetConsecutiveFailures();
    }
    m_tStimulationValues[reactInd] = curFeedback * MAX_SENSOR_VAL / MAX_FEEDBACK_VAL;
  }

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorMemoryFeedback::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorMemoryFeedback::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) || (index>m_pBrain->GetNumberReaction()) )
    return ("bad index");
  else
  {
    return (m_pBrain->GetReactionByIndex(index)->GetLabel());
  }
}
