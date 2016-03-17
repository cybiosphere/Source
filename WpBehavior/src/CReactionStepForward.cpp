//===========================================================================
// FILE: CReactionStepForward.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action step forward
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  24/10/2005 : Creation
//
//===========================================================================

#include "CReactionStepForward.h"
#include "CAnimal.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionStepForward::CReactionStepForward
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
CReactionStepForward::CReactionStepForward(CBrainAnimal*  pBrain, double successFactor, double failureFactor, int stepSpeed)
:CReaction(successFactor, failureFactor, UID_BASE_REACT_STEPFORWARD,1)
{
  m_pBrain    = pBrain;
  m_stepSpeed = stepSpeed;
  if (m_stepSpeed>30)
    m_Label.Format("Run");
  else
    m_Label.Format("Walk");
}

CReactionStepForward::~CReactionStepForward()
{

}

//===========================================================================
// Virtual overload
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReactionStepForward::ExecuteAction
//  
// DESCRIPTION:  Evaluate feedback after action 
// 
// ARGUMENTS:    ReactionIntensityType_e intensity
//   
// RETURN VALUE: None
//  
// REMARKS:      Virtual overload
//---------------------------------------------------------------------------
void CReactionStepForward::ExecuteAction(ReactionIntensityType_e intensity)
{
  m_pBrain->getAnimal()->wakeUp();
  if (m_pBrain->getAnimal()->ExecuteMoveForwardAction(m_SuccessSatisfactionFactor, m_FailureFrustrationFactor, m_stepSpeed))
    m_ConsecutiveFailures = 0;
  else
    m_ConsecutiveFailures++;  
  return;
}