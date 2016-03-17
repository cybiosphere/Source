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
// FILE: CReactionEat.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an action for eat
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined(CREACTIONEAT_INCLUDED_)
#define CREACTIONEAT_INCLUDED_

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
class DLL_CYBIOCORE_API CReactionEat : public CReaction  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
    int           m_relativeLayer; // entity relative layer

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
	CReactionEat(CBrainAnimal*  pBrain, double successFactor, double failureFactor, int relLayer=0);
	virtual ~CReactionEat();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
  void ExecuteAction(ReactionIntensityType_e intensity);

};

#endif // !defined(CREACTIONEAT_INCLUDED_)
