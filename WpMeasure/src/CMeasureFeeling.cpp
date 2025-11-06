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
// FILE: CMeasureFeeling.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#include "CMeasureFeeling.h"

//===========================================================================
// Definitions            
//===========================================================================

const char* MeasureFeelingTypeNameList[MEASURE_FEELING_NB_TYPE] =
{
  "Current",
  "Delta"
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

CMeasureFeeling::CMeasureFeeling(CBasicEntity* pEntity, int period, int id, MeasureFeelingType_e type)
:CMeasure(period, id, -100.0, 100.0, MEASURE_TYPE_FEELING)
{
  m_pEntity = pEntity;

  if ((pEntity != NULL) && (pEntity->getBrain() != NULL))
  {
    m_SubType = type;
    m_pBrain = pEntity->getBrain();
  }
  else
  {
    m_SubType = MEASURE_FEELING_NB_TYPE;
    m_pBrain = NULL;
  }

  switch(m_SubType)
  {
  case  MEASURE_FEELING_CUR:
    m_Label = pEntity->getLabel() + "- Feeling";
    break;

  case  MEASURE_FEELING_DIF:
    m_Label = pEntity->getLabel() + "- Delta feeling";
    break;

  default:
    m_Label = "error";
    break;
  }
  
}

CMeasureFeeling::~CMeasureFeeling()
{
}

//===========================================================================
// Measurement management
//===========================================================================
double CMeasureFeeling::GetCurrentValue()
{
  if (m_pBrain == NULL)
    return 0;

  double val;

  switch(m_SubType)
  {
  case  MEASURE_FEELING_CUR:
    val = m_pBrain->GetCurrentFeelingWelfare();
    break;

  case  MEASURE_FEELING_DIF:
    val = m_pBrain->GetCurrentFeelingWelfare() - m_pBrain->GetPreviousFeelingWelfare();
    break;

  default:
    val = 0;
    break;
  }

  return val;
}

bool CMeasureFeeling::IsStillValid()
{
  return (m_pEntity->isToBeRemoved()) ? false : true;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

string CMeasureFeeling::GetMeasureFeelingTypeStrName(MeasureFeelingType_e type)
{
  string typeName;
  typeName = MeasureFeelingTypeNameList[type];
  return typeName;
}

size_t CMeasureFeeling::GetSubTypeId()
{
  return m_SubType;
}

CBasicEntity* CMeasureFeeling::GetpEntity()
{
  return m_pEntity;
}