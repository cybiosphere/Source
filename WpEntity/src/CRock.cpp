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
// FILE: CRock.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic water element
//         This entity should not be attached to biotop. 1 instance only 
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CRock.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CRock::CRock
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CRock::CRock()
{
  // Input values
  
  // Default values 
  Point_t coord = {invalidCoord, invalidCoord};
  jumpToGridCoord(coord, true, invalidCoord);
  m_pGenome       = NULL;
  m_pBrain        = NULL;

  m_Status      = STATUS_STATIC;
  m_Label       = "Generic rock";
  m_Generation  = 0;
  m_TotalChildNumber = 0;
 
  setColor(0x008A8F8A); // Color
  setForm(FORM_PLAN_ROUGH);
  setOdor(ODOR_NONE);

  completeCaractsWithDefault();
  completeParamsWithDefault();
  CBasicEntity::changeWeight(getMaxWeight());

  m_pGenome = new CGenome(CLASS_SOLID, "Rock_ground");
}

CRock::~CRock()
{
}

bool CRock::isMineral()
{
  return true;
}

double CRock::changeWeight(double variation)
{
  return (0);
}
