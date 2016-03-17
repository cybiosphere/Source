//===========================================================================
// FILE: CReactionStepForward.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action step forward
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  24/10/2005 : Creation
//
//===========================================================================

#if !defined(REACTIONSTEPFORWARD_INCLUDED_)
#define REACTIONSTEPFORWARD_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================

#include "CReaction.h"
#include "CBrainAnimal.h"

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CReactionStepForward : public CReaction  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
    int           m_stepSpeed; // max step number per second

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
    CBrainAnimal* m_pBrain;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CReactionStepForward(CBrainAnimal*  pBrain, double successFactor, double failureFactor, int stepSpeed);
	virtual ~CReactionStepForward();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
  void ExecuteAction(ReactionIntensityType_e intensity);

};

#endif // !defined(REACTIONSTEPFORWARD_INCLUDED_)
