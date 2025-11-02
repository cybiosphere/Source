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
// FILE: CReaction.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a possible reaction for a brain.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  21/12/2005 : Creation
//
//===========================================================================

#include "CReaction.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReaction::CReaction
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    successSatisfactionFactor: 0..100 pleasure factor in case of success
//               failureFrustrationFactor:  0..100 pleasure factor in case of failure
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CReaction::CReaction(double successSatisfactionFactor, double failureFrustrationFactor, DWORD uniqueId, int unitTime)
{
  m_SuccessSatisfactionFactor = successSatisfactionFactor;
  m_FailureFrustrationFactor  = failureFrustrationFactor;

  if (m_SuccessSatisfactionFactor < 0)
    m_SuccessSatisfactionFactor = 0;
  if (m_SuccessSatisfactionFactor > 100.0)
    m_SuccessSatisfactionFactor = 100.0;
  if (m_FailureFrustrationFactor < 0)
    m_FailureFrustrationFactor = 0;
  if (m_FailureFrustrationFactor > 100.0)
    m_FailureFrustrationFactor = 100.0;

  m_InitialSuccessSatisfactionFactor = m_SuccessSatisfactionFactor;
  m_InitialFailureFrustrationFactor  = m_FailureFrustrationFactor;
  m_ConsecutiveFailures = 0;

  m_BonusRate = 100;
  m_UniqueId  = uniqueId;
  m_UnitTimeInSecond = unitTime;

  ResetCounters();
}

CReaction::~CReaction()
{
}

//===========================================================================
// Action management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CReaction::ExecuteAction
//  
// DESCRIPTION:  Execute an action and compute feedback feeling
// 
// ARGUMENTS:    ReactionIntensityType_e intensity
//   
// RETURN VALUE: void
//  
// REMARKS:      Should be overloaded by inherited classes
//---------------------------------------------------------------------------
void CReaction::ExecuteAction(ReactionIntensityType_e intensity)
{
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

string CReaction::GetLabel()
{
  return m_Label;
}

DWORD CReaction::GetUniqueId()
{
  return m_UniqueId;
}

double CReaction::GetFailureFrustrationFactor()
{
  return m_FailureFrustrationFactor;
}

double CReaction::GetSuccessSatisfactionFactor()
{
  return m_SuccessSatisfactionFactor;
}

void CReaction::SetFailureFrustrationFactor(double newFrustration)
{
  m_FailureFrustrationFactor  = newFrustration;

  if (m_FailureFrustrationFactor < 0)
    m_FailureFrustrationFactor = 0;
  if (m_FailureFrustrationFactor > 100.0)
    m_FailureFrustrationFactor = 100.0;
}

void CReaction::SetSuccessSatisfactionFactor(double newSatisfaction)
{
  m_SuccessSatisfactionFactor = newSatisfaction;

  if (m_SuccessSatisfactionFactor < 0)
    m_SuccessSatisfactionFactor = 0;
  if (m_SuccessSatisfactionFactor > 100.0)
    m_SuccessSatisfactionFactor = 100.0;
}

int CReaction::GetBonusRate()
{
  return m_BonusRate;
}

void CReaction::SetBonusRate(int rate)
{
  m_BonusRate = rate;
}

void CReaction::SetBonusRateToNeutral()
{
  m_BonusRate = 100;
}

void CReaction::IncreaseSuccessCount(void)
{
  // Decrease SuccessSatisfactionFactor on each success (long time satiety)
  if (m_SuccessSatisfactionFactor > 0.1)
    m_SuccessSatisfactionFactor -= 0.1;

  m_SuccessCounter++;
}

void CReaction::IncreaseFailureCount(void)
{
  // Decrease FailureFrustrationFactor on each failure (long time limitation)
  if (m_FailureFrustrationFactor > 0.1)
    m_FailureFrustrationFactor -= 0.1;

  m_FailureCounter++;
}

void CReaction::IncreaseExecuteCount(void)
{
  m_ExecuteCounter++;
}

double CReaction::GetSuccessRate(void)
{
  return (m_ExecuteCounter != 0) ? 100.0 * (double)m_SuccessCounter / (double)(m_ExecuteCounter) : -1.0;
}

double CReaction::GetFailureRate(void)
{
  return (m_ExecuteCounter != 0) ? 100.0 * (double)m_FailureCounter / (double)(m_ExecuteCounter) : -1.0;
}

void CReaction::ResetCounters(void)
{
  m_SuccessCounter = 0;
  m_FailureCounter = 0;
  m_ExecuteCounter = 0;
}

void CReaction::ResetSuccessFailureFactor(void)
{
  m_SuccessSatisfactionFactor = m_InitialSuccessSatisfactionFactor;
  m_FailureFrustrationFactor = m_InitialFailureFrustrationFactor;
}

int CReaction::GetUnitTimeInSecond(void)
{
  return m_UnitTimeInSecond;
}

int CReaction::GetConsecutiveFailures(void)
{
  return m_ConsecutiveFailures;
}