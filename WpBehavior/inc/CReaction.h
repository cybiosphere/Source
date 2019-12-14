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
// FILE: CReaction.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a possible reaction for a brain.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  21/12/2005 : Creation
//
//===========================================================================

#if !defined( CREACTION_INCLUDED_)
#define CREACTION_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>
#include "Definitions.h"
#include "Matrix.h"


//===========================================================================
// Definitions            
//===========================================================================

class CBrain;

typedef double feedbackValType; // from -MAX_FEEDBACK_VAL to MAX_FEEDBACK_VAL 

#define MAX_FEEDBACK_VAL  10.0

typedef enum
{
  REACTION_INTENSITY_LOW = 0,
  REACTION_INTENSITY_MEDIUM,
  REACTION_INTENSITY_HIGH,    
} ReactionIntensityType_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CReaction  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  string    m_Label;                    // User defined name
  double    m_SuccessSatisfactionFactor;// 0..100 pleasure factor in case of success
  double    m_FailureFrustrationFactor; // 0..100 pleasure factor in case of failure
  double    m_InitialSuccessSatisfactionFactor;// 0..100 Initial pleasure factor in case of success
  double    m_InitialFailureFrustrationFactor; // 0..100 Initial pleasure factor in case of failure
  int       m_ConsecutiveFailures;

private:
  int       m_BonusRate;                // Purpose factor: from 0 to 100 :malus, over 100 bonus 
  long      m_SuccessCounter;           // Use to evaluate success rate
  long      m_FailureCounter;           // Use to evaluate failure rate
  long      m_ExecuteCounter;           // Use to evaluate failure rate
  DWORD     m_UniqueId;                 // Global identifier
  int       m_UnitTimeInSecond;         // Minimum time used for an action

//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------  
public:
	CReaction(double successSatisfactionFactor, double failureFrustrationFactor, DWORD uniqueId, int unitTime);
  virtual ~CReaction();

//---------------------------------------------------------------------------
// Action management
//---------------------------------------------------------------------------   
    virtual void ExecuteAction(ReactionIntensityType_e intensity);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
  string  GetLabel();
  DWORD   GetUniqueId();
  double  GetFailureFrustrationFactor();
  double  GetSuccessSatisfactionFactor();
  void    SetFailureFrustrationFactor(double newFrustration);
  void    SetSuccessSatisfactionFactor(double newSatisfaction);
  int     GetBonusRate();
  void    SetBonusRate(int rate);
  void    SetBonusRateToNeutral();
  void    IncreaseSuccessCount(void);
  void    IncreaseFailureCount(void);
  void    IncreaseExecuteCount(void);
  double  GetSuccessRate(void);
  double  GetFailureRate(void);
  void    ResetCounters(void);
  void    ResetSuccessFailureFactor(void);
  int     GetUnitTimeInSecond(void);
  int     GetConsecutiveFailures(void);
};

#endif // !defined(CREACTION_INCLUDED_)
