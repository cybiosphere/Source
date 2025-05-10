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
// FILE: CReactionTurnRight.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action turn right
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  24/10/2005 : Creation
//
//===========================================================================

#include "CReactionTurnRight.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionTurnRight::CReactionTurnRight
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
CReactionTurnRight::CReactionTurnRight(CBrainAnimal*  pBrain, double successFactor, double failureFactor)
:CReaction(successFactor, failureFactor, UID_BASE_REACT_TURNRIGHT, 1)
{
  m_pBrain = pBrain;
  m_Label  = "Turn_Right";
}

CReactionTurnRight::~CReactionTurnRight()
{
}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionTurnRight::ExecuteAction
//  
// DESCRIPTION:  Evaluate feedback after action 
// 
// ARGUMENTS:    ReactionIntensityType_e intensity
//   
// RETURN VALUE: None
//  
// REMARKS:      Virtual overload
//---------------------------------------------------------------------------
void CReactionTurnRight::ExecuteAction(ReactionIntensityType_e intensity)
{
  m_pBrain->getAnimal()->wakeUp();
  m_pBrain->getAnimal()->setVigilance(VIGILANCE_RATE_LOW);

  // reduce speed
  double curSpeed = m_pBrain->getAnimal()->getCurrentSpeed();
  if (curSpeed < 5)
  {
    m_pBrain->getAnimal()->stopCurrentSpeed();
  }
  else
  {
    m_pBrain->getAnimal()->changeCurrentSpeed(-curSpeed/3.0);
  }

  switch(intensity)
  {
  case REACTION_INTENSITY_LOW:
    m_pBrain->getAnimal()->ExecuteTurnRightAction(15);
    break;
  case REACTION_INTENSITY_HIGH:
    m_pBrain->getAnimal()->ExecuteTurnRightAction(45);
    break;
  default:
    m_pBrain->getAnimal()->ExecuteTurnRightAction(30);
    break;
  }
}
