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
// FILE: CLifeStage.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a life stage for an entity.
//         Behavior and parameters of any entity are updated when stage change
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CLifeStage.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CLifeStage::CLifeStage
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    
//               
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CLifeStage::CLifeStage(string label, LifeStage_e type, int dayDuration)
{
  m_Label               = label;
  m_StageType           = type;
  m_TotalDayDuration    = dayDuration;
  m_CurrentDayCounter   = 0;
}

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CLifeStage::increaseDayCountAndCheckEnd
//  
// DESCRIPTION:  Increase m_CurrentDayCounter and check if = m_TotalDayDuration
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool: false if end not 
//  
// REMARKS:      Global application speed highly impacted by this method
//---------------------------------------------------------------------------
bool CLifeStage::increaseDayCountAndCheckEnd(void)
{
  m_CurrentDayCounter++;
  if (m_CurrentDayCounter >= m_TotalDayDuration)
  {
    m_CurrentDayCounter = m_TotalDayDuration;
    return true;
  }
  return false;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

LifeStage_e CLifeStage::getStageType(void)
{
  return m_StageType;
}

string CLifeStage::getLabel(void)
{
  return m_Label;
}

int CLifeStage::getTotalDayDuration(void)
{
  return m_TotalDayDuration;
}

void CLifeStage::setTotalDayDuration(int newDuration)
{
 m_TotalDayDuration = newDuration;
}

int CLifeStage::getCurDayCount(void)
{
  return m_CurrentDayCounter;
}

void CLifeStage::setCurDayCount(int newDayCount)
{
  m_CurrentDayCounter = newDayCount;
}