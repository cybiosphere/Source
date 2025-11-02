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
// FILE: CReactionVarySpeed.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action step forward
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  18/09/2008 : Creation
//
//===========================================================================

#include "CReactionVarySpeed.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionVarySpeed::CReactionVarySpeed
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double successFactor: {0..100} - feedback weight in case of success
//               double failureFactor: {0..100} - feedback weight in case of failure
//               int stepSpeed :       >0 : acceleration; <0 deceleration; =0 force stop 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CReactionVarySpeed::CReactionVarySpeed(CBrainAnimal*  pBrain, double successFactor, double failureFactor, int stepSpeed)
:CReaction(successFactor, failureFactor, UID_BASE_REACT_MOVEFORWARD + stepSpeed, 1)
{
  m_pBrain    = pBrain;
  m_stepSpeed = stepSpeed;
  if (m_stepSpeed > 12)
    m_Label = "Run";
  else if (m_stepSpeed <= 0)
    m_Label = "BadSpeed";
  else
    m_Label = "Walk";
}

CReactionVarySpeed::~CReactionVarySpeed()
{
}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionVarySpeed::ExecuteAction
//  
// DESCRIPTION:  Evaluate feedback after action 
// 
// ARGUMENTS:    ReactionIntensityType_e intensity
//   
// RETURN VALUE: None
//  
// REMARKS:      Virtual overload
//---------------------------------------------------------------------------
void CReactionVarySpeed::ExecuteAction(ReactionIntensityType_e intensity)
{
  bool resu = true;
  m_pBrain->getAnimal()->wakeUp();
  m_pBrain->getAnimal()->setVigilance(VIGILANCE_RATE_NORMAL);

  double accelerationRate = 50 + (intensity - 2) * 10;  // Use value from new parameter
  double targetSpeed = m_stepSpeed;

  int stepVariation = cybio_round((targetSpeed - (double)m_pBrain->getAnimal()->getCurrentSpeed()) * accelerationRate / 100.0);

  if (stepVariation != 0)
  {
    m_pBrain->getAnimal()->changeCurrentSpeed(stepVariation);

    // perform additional steps : start now to move
    if (stepVariation > 0)
      resu = m_pBrain->getAnimal()->ExecuteMoveForwardAction(m_SuccessSatisfactionFactor, m_FailureFrustrationFactor, stepVariation);
  }

  if (resu)   
    m_ConsecutiveFailures = 0;
  else
    m_ConsecutiveFailures++;  
}