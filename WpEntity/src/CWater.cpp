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
// FILE: CWater.cpp
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
 
#include "CWater.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CWater::CWater
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CWater::CWater()
{
  // Input values
  
  // Default values 
  Point_t coord = {invalidCoord, invalidCoord};
  jumpToGridCoord(coord, true, 1); // Def layer is 1
  m_pGenome       = NULL;
  m_pBrain        = NULL;

  m_Status      = STATUS_STATIC;
  m_Label       = "Water";
  m_Generation  = 0;
  m_TotalChildNumber = 0;
  m_bIsDrinkable = true;
  m_bInfiniteContent = true;

  completeCaractsWithDefault();
  completeParamsWithDefault();
  CBasicEntity::changeWeight(getMaxWeight());
}

//---------------------------------------------------------------------------
// METHOD:       CWater::CWater
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CWater::CWater(string label, Point_t initCoord, CGenome* pPseudoGenome)
{
  // Input values
  
  // Default values 
  jumpToGridCoord(initCoord, true, 1); // Def layer is 1
  m_pGenome       = pPseudoGenome;
  m_pBrain        = NULL;

  m_Status      = STATUS_STATIC;
  m_Label       = label;
  m_Generation  = 0;
  m_TotalChildNumber = 0;
  m_bIsDrinkable = true;
  m_bInfiniteContent = false;
}

//---------------------------------------------------------------------------
// METHOD:       CWater::CWater
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CMineral& model : Single parent constructor for cloning
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------     
CWater::CWater(string label, CWater& model) 
{ 
  m_Label         = label;
  // inherited
  jumpToGridCoord(model.getGridCoord(), true, model.getLayer()); 
  m_Generation		= model.m_Generation + 1;
  m_pGenome       = new CGenome(*model.m_pGenome);

  // reset
  m_pBrain          = NULL;
  m_Status      = STATUS_STATIC;
  m_Generation  = 0;
  m_TotalChildNumber = 0;
  m_bIsDrinkable = true;
  m_bInfiniteContent = false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeCaractsWithDefault 
//  
// DESCRIPTION:  Overide all settings
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CWater::completeCaractsWithDefault()
{
  CBasicEntity::completeCaractsWithDefault();
  setOdor(ODOR_NONE);
  setColor(0x00EA0F00); // Color
  setForm(FORM_PLAN_SMOOTH);
  setTexture(TEXTURE_REFLECT);

  return (true);
}

bool CWater::isMineral()
{
  return true;
}

double CWater::changeWeight(double variation)
{
  if (m_bInfiniteContent)
  {
    // Do nothing: infinite content
    return (variation);
  }
  else
  {
    return(CBasicEntity::changeWeight(variation));
  }
}

ClassType_e  CWater::getClass()
{
  return CLASS_WATER;
}

void CWater::nextHour()
{
  // Decrease slowly content
  changeWeight(-0.1);
}