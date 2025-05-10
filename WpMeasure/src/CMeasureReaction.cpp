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
// FILE: CMeasureReaction.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#include "CMeasureReaction.h"

//===========================================================================
// Definitions            
//===========================================================================

const char* MeasureReactionTypeNameList[MEASURE_ACTION_NB_TYPE] =
{
  "Success",
  "Failure"
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

CMeasureReaction::CMeasureReaction(CBasicEntity* pEntity, size_t reactionInd, int period, int id, MeasureReactionType_e type)
:CMeasure(period, id, 0, 100.0, MEASURE_TYPE_REACTION)
{
  m_paramIndex = reactionInd;

  if ( (pEntity != NULL) && (pEntity->getBrain() != NULL) && (pEntity->getBrain()->GetReactionByIndex(reactionInd) != NULL))
  {
    m_SubType = type;
    m_pReaction = pEntity->getBrain()->GetReactionByIndex(reactionInd);
  }
  else
  {
    m_SubType = MEASURE_ACTION_NB_TYPE;
    m_pReaction = NULL;
  }

  switch(m_SubType)
  {
  case  MEASURE_ACTION_SUCCESS:
    m_Label = pEntity->getLabel() + " - Reaction " + m_pReaction->GetLabel() + " success rate"; 
    break;
  case  MEASURE_ACTION_FAILURE:
    m_Label = pEntity->getLabel() + " - Reaction " + m_pReaction->GetLabel() + " failure rate"; 
    break;
  default:
    m_Label = "error";
    break;
  }

}

CMeasureReaction::~CMeasureReaction()
{
}

//===========================================================================
// Measurement management
//===========================================================================
double CMeasureReaction::GetCurrentValue()
{
  if (m_pReaction==NULL)
    return 0;

  double val = 0;
  switch(m_SubType)
  {
  case  MEASURE_ACTION_SUCCESS:
    val = m_pReaction->GetSuccessRate();
    break;
  case  MEASURE_ACTION_FAILURE:
    val = m_pReaction->GetFailureRate(); 
    break;
  default:
    val =0;
    break;
  }
  return val;
}

bool CMeasureReaction::IsStillValid()
{
  return (m_pEntity->isToBeRemoved()) ? false : true;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

string CMeasureReaction::GetMeasureReactionTypeStrName(MeasureReactionType_e type)
{
  string typeName;
  typeName = MeasureReactionTypeNameList[type];
  return typeName;
}

size_t CMeasureReaction::GetSubTypeId()
{
  return m_SubType;
}


size_t CMeasureReaction::GetReactionIndex()
{
  return m_paramIndex;
}