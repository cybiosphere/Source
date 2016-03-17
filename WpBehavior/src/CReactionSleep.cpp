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
// FILE: CReactionSleep.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action "NOP"
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  24/10/2005 : Creation
//
//===========================================================================

#include "CReactionSleep.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionSleep::CReactionSleep
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
CReactionSleep::CReactionSleep(CBrainAnimal*  pBrain, double successFactor, double failureFactor)
:CReaction(successFactor, failureFactor, UID_BASE_REACT_SLEEP, 3)
{
  m_pBrain = pBrain;
  m_Label  = "Sleep";
}

CReactionSleep::~CReactionSleep()
{

}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionSleep::ExecuteAction
//  
// DESCRIPTION:  Evaluate feedback after action 
// 
// ARGUMENTS:    ReactionIntensityType_e intensity
//   
// RETURN VALUE: None 
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CReactionSleep::ExecuteAction(ReactionIntensityType_e intensity)
{
  m_pBrain->getAnimal()->stopCurrentSpeed();
  m_pBrain->getAnimal()->ExecuteSleepAction();
  return;
}