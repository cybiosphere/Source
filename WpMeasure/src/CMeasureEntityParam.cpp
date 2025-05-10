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
// FILE: CMeasureEntityParam.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#include "CMeasureEntityParam.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

CMeasureEntityParam::CMeasureEntityParam(CBasicEntity* pEntity, size_t paramIndex, int period, int id)
:CMeasureParam(pEntity->getParameter(paramIndex), period, id, MEASURE_TYPE_PARAMETER_ENTITY)
{
  m_pEntity = pEntity;
  m_paramIndex = paramIndex;

  if ( (pEntity!=NULL) && (pEntity->getParameter(paramIndex)!=NULL) )
  {
    m_Label = pEntity->getLabel() + " - " + pEntity->getParameter(paramIndex)->getLabel();
  }
  else
  {
    m_Label = "NULL parameter";
  }
}

CMeasureEntityParam::~CMeasureEntityParam()
{
}

//===========================================================================
// Measurement management
//===========================================================================

bool CMeasureEntityParam::IsStillValid()
{
  if (m_pEntity->isToBeRemoved())
    return false;
  else
    return true;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

size_t CMeasureEntityParam::GetSubTypeId()
{
  return m_paramIndex;
}

CBasicEntity* CMeasureEntityParam::GetpEntity()
{
  return m_pEntity;
}