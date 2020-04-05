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
// FILE: CMeasureReaction.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#if !defined(CMEASUREREACTION_INCLUDED_)
#define CMEASUREREACTION_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>

#include "Definitions.h"
#include "CBrain.h"
#include "CMeasure.h"

//===========================================================================
// Definitions            
//===========================================================================

typedef enum
{
  MEASURE_ACTION_SUCCESS  = 0,
  MEASURE_ACTION_FAILURE  = 1,
  MEASURE_ACTION_NB_TYPE  = 2
} MeasureReactionType_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CMeasureReaction : public CMeasure
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  CReaction* m_pReaction;
  MeasureReactionType_e m_SubType;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CMeasureReaction(CBasicEntity* pEntity, size_t reactionInd, int period, int id, MeasureReactionType_e type);
	virtual ~CMeasureReaction();

//---------------------------------------------------------------------------
// Measurement management
//---------------------------------------------------------------------------
public:
  double GetCurrentValue();
  bool IsStillValid();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  static string GetMeasureReactionTypeStrName(MeasureReactionType_e type);
  size_t GetSubTypeId();
  size_t GetReactionIndex();
};

#endif // !defined(CMEASUREREACTION_INCLUDED_)
