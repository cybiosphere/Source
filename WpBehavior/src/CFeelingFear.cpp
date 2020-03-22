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
// FILE: CFeelingFear.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents the feeling welfare of an animal
//         This is used to compute a parameter involved in satisfaction evaluation
//         when the animal perform an action (especially moves or does nothing)
//         It can take into account any sensor output
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  01/03/2006 : Creation
//
//===========================================================================
 
#include "CFeelingFear.h"

//===========================================================================
// Constructors / Destructors 
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CFeelingFear::CFeelingFear
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBasicEntity* pEntity
//               
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CFeelingFear::CFeelingFear(CBasicEntity* pEntity)
{
  m_pEntity = pEntity;
  m_CurFear = 0;

  // Sensitivity table
  m_tSensitivity.resize(0);
}

//---------------------------------------------------------------------------
// METHOD:       CFeelingFear::~CFeelingFear
//  
// DESCRIPTION:  Destructor 
// 
// ARGUMENTS:    None
//               
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CFeelingFear::~CFeelingFear()
{
  // loop from top to bottom 
  for (int i=m_tSensitivity.size()-1; i>=0; i--) 
  {
    if (m_tSensitivity[i] != NULL)
    {
      delete m_tSensitivity[i]->m_pSensitivityTableMask;
      delete m_tSensitivity[i];
    }
  }
  m_tSensitivity.clear();
}

//---------------------------------------------------------------------------
// Computation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// METHOD:       CFeelingFear::ComputeAndGetFeelingFear
//  
// DESCRIPTION:  Get feeling welfare due to environment effect on animal
// 
// ARGUMENTS:    
//               
//   
// RETURN VALUE: double : -100.0 = worst feeling .. +100.0 = best feeling
//  
// REMARKS:      Can be called every sec for animal, every hour for vegetable
//---------------------------------------------------------------------------
double CFeelingFear::ComputeAndGetFeelingFear()
{
  // First decrease fear
  m_CurFear -= 1;
  // Add new fear factors
  for (auto pSensitivity: m_tSensitivity)
  {
    const std::vector<sensorValType>& vectStimulation{ pSensitivity->m_pSens->GetStimulationTable() };
    for (int j = 0; j < vectStimulation.size(); j++)
    {
      m_CurFear += pSensitivity->m_pSensitivityTableMask[j] * vectStimulation[j];
    }
  }

  if (m_CurFear>100.0)
    m_CurFear = 100.0;

  if (m_CurFear<0.0)
    m_CurFear = -0.0;

  return (m_CurFear);
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

bool CFeelingFear::AddSensitivity(CSensor* pSens, int tableMaskSize, double* pTableMask)
{
  if (pSens->GetSubCaptorNumber() != tableMaskSize)
    return false;

  FearSensitivity_t* pSensitivity = new FearSensitivity_t;

  if (pSensitivity == NULL)
    return false;

  pSensitivity->m_pSens = pSens;
  pSensitivity->m_pSensitivityTableMask = pTableMask;

  m_tSensitivity.push_back(pSensitivity);

  return true;
}

double CFeelingFear::GetCurFeelingFear()
{
  return (m_CurFear);
}
