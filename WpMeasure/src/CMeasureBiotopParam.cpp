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
// FILE: CMeasureBiotopParam.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#include "CMeasureBiotopParam.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

CMeasureBiotopParam::CMeasureBiotopParam(CBiotop* pBiotop, size_t paramIndex, int period, int id)
:CMeasureParam(pBiotop->getParameter(paramIndex), period, id, MEASURE_TYPE_PARAMETER_BIOTOP)
{
  m_pBiotop = pBiotop;
  m_paramIndex = paramIndex;

  if ( (pBiotop!=NULL) && (pBiotop->getParameter(paramIndex)!=NULL) )
  {
    m_Label = "Biotop - " + pBiotop->getParameter(paramIndex)->getLabel();
  }
  else
  {
    m_Label = "NULL parameter";
  }
}

CMeasureBiotopParam::~CMeasureBiotopParam()
{
}

//===========================================================================
// Measurement management
//===========================================================================


//===========================================================================
// Get / Set for attributes
//===========================================================================

size_t CMeasureBiotopParam::GetSubTypeId()
{
  return m_paramIndex;
}