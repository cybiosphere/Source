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
// FILE: CReactionAttack.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action for attack
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  24/10/2005 : Creation
//
//===========================================================================

#if !defined(REACTIONATTACK_INCLUDED_)
#define REACTIONATTACK_INCLUDED_

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
class DLL_CYBIOCORE_API CReactionAttack : public CReaction   
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
    CBrainAnimal* m_pBrain;

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
    int           m_relativeLayer; // entity relative layer

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CReactionAttack(CBrainAnimal*  pBrain, double successFactor, double failureFactor, int relLayer=0);

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  void ExecuteAction(ReactionIntensityType_e intensity);

};

#endif // !defined(REACTIONATTACK_INCLUDED_)
