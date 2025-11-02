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
// FILE: CReactionDrink.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action for drink
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CReactionDrink.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionDrink::CReactionDrink
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double successFactor: {0..100} - feedback weight in case of success
//               double failureFactor: {0..100} - feedback weight in case of failure
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CReactionDrink::CReactionDrink(CBrainAnimal*  pBrain, double successFactor, double failureFactor)
:CReaction(successFactor, failureFactor, UID_BASE_REACT_DRINK, 2)
{
  m_pBrain = pBrain;
  m_Label  = "Drink";
}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionDrink::ExecuteAction
//  
// DESCRIPTION:  Evaluate feedback after action 
// 
// ARGUMENTS:    ReactionIntensityType_e intensity
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CReactionDrink::ExecuteAction(ReactionIntensityType_e intensity)
{
  m_pBrain->getAnimal()->wakeUp();
  m_pBrain->getAnimal()->stopCurrentSpeed();
  m_pBrain->getAnimal()->setVigilance(VIGILANCE_RATE_LOW);
  if (m_pBrain->getAnimal()->ExecuteDrinkAction(m_SuccessSatisfactionFactor, m_FailureFrustrationFactor))
    m_ConsecutiveFailures = 0;
  else
    m_ConsecutiveFailures++;  
}
