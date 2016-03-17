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
// FILE: CBrainAnimal.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an animal brain
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  06/11/2005 : Creation
//
//===========================================================================

#if !defined(CBRAINANIMAL_INCLUDED_)
#define CBRAINANIMAL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================

#include "CBrain.h"
#include "CSensorView.h"
#include "CSensorHunger.h"
#include "CSensorSmell.h"
#include "CSensorEar.h"
#include "CSensorThirst.h"
#include "CSensorStomachOverload.h"
#include "CSensorTactile.h"
#include "CSensorSuffering.h"
#include "CSensorPleasure.h"
#include "CSensorPheromone.h"
#include "CSensorTiredness.h"
#include "CSensorLibido.h"
#include "CSensorSpeed.h"
#include "CSensorObscurity.h"
#include "CSensorMemoryAction.h"
#include "CSensorMemoryFeedback.h"
#include "CSensorViewFar.h"
#include "CSensorCompass.h"
#include "CSensorTaste.h"
#include "CSensorFear.h"
#include "CSensorTemperature.h"
#include "CSensorStatic.h"
#include "CSensorHormone.h"
#include "CSensorComposite.h"
#include "CSensorOrientation.h"
#include "CSensorViewIdentify.h"
#include "CSensorViewIdentifyFar.h"
#include "CReactionNothing.h"
#include "CReactionSleep.h"
#include "CReactionVarySpeed.h"
#include "CReactionStepBackward.h"
#include "CReactionTurnLeft.h"
#include "CReactionTurnRight.h"
#include "CReactionAttack.h"
#include "CReactionEat.h"
#include "CReactionDrink.h"
#include "CReactionCopulate.h"
#include "CReactionTurnHeadLeft.h"
#include "CReactionTurnHeadRight.h"
#include "CReactionRuminate.h"
#include "CReactionHide.h"

//===========================================================================
// Definitions            
//===========================================================================

#define UID_BASE_MASK                0xFF000000

// Base value to define sensor UniqueId. Other significative values should be added on LSB
#define UID_BASE_SENS_STATIC            0x00000000
#define UID_BASE_SENS_TACTILE           0x01000000
#define UID_BASE_SENS_VIEW              0x02000000
#define UID_BASE_SENS_SMELL             0x03000000
#define UID_BASE_SENS_PHEROMONE         0x04000000
#define UID_BASE_SENS_EAR               0x05000000
#define UID_BASE_SENS_HUNGER            0x06000000
#define UID_BASE_SENS_THIRST            0x07000000
#define UID_BASE_SENS_STOMACH_FULL      0x08000000
#define UID_BASE_SENS_PLEASURE          0x09000000
#define UID_BASE_SENS_SUFFER            0x0A000000
#define UID_BASE_SENS_TIREDNESS         0x0B000000
#define UID_BASE_SENS_OBSCURITY         0x0C000000
#define UID_BASE_SENS_LIBIDO            0x0D000000
#define UID_BASE_SENS_MEMORY_ACT        0x0E000000
#define UID_BASE_SENS_MEMORY_FB         0x0F000000
#define UID_BASE_SENS_SPEED             0x10000000
#define UID_BASE_SENS_VIEW_FAR          0x11000000
#define UID_BASE_SENS_COMPASS           0x12000000
#define UID_BASE_SENS_TASTE             0x13000000
#define UID_BASE_SENS_FEAR              0x14000000
#define UID_BASE_SENS_TEMPERATURE       0x15000000
#define UID_BASE_SENS_WIND              0x16000000
#define UID_BASE_SENS_HORMONE           0x17000000
#define UID_BASE_SENS_COMPOSITE         0x18000000
#define UID_BASE_SENS_ORIENTATION       0x19000000
#define UID_BASE_SENS_VIEW_IDENTIFY     0x1A000000
#define UID_BASE_SENS_VIEW_IDENTIFY_FAR 0x1B000000

// Base value to define recation UniqueId. Other significative values should be added on LSB
#define UID_BASE_REACT_NOTHING          0x01000000
#define UID_BASE_REACT_SLEEP            0x02000000
#define UID_BASE_REACT_MOVEFORWARD      0x03000000
#define UID_BASE_REACT_STEPBACKWARD     0x04000000
#define UID_BASE_REACT_TURNLEFT         0x05000000
#define UID_BASE_REACT_TURNRIGHT        0x06000000
#define UID_BASE_REACT_EAT              0x07000000
#define UID_BASE_REACT_DRINK            0x08000000
#define UID_BASE_REACT_COPULATE         0x09000000
#define UID_BASE_REACT_ATTACK           0x0A000000
#define UID_BASE_REACT_TURNHEADLEFT     0x0B000000
#define UID_BASE_REACT_TURNHEADRIGHT    0x0C000000
#define UID_BASE_REACT_RUMINATE         0x0D000000
#define UID_BASE_REACT_HIDE             0x0E000000


class CAnimal;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CBrainAnimal : public CBrain
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CBrainAnimal(CAnimal* pAnimal);
	virtual ~CBrainAnimal();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
  CAnimal* getAnimal(void);

};

#endif // !defined(CBRAINANIMAL_INCLUDED_)
