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
// FILE: CFeelingWelfare.cpp
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
 
#include "CFeelingWelfare.h"
#include "CBiotop.h"

//===========================================================================
// Constructors / Destructors 
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CFeelingWelfare::CFeelingWelfare
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
CFeelingWelfare::CFeelingWelfare(CBasicEntity* pEntity)
{
  m_pEntity = pEntity;
  m_CurSatisfaction = 0;

  // Sensitivity table
  m_tSensitivity.resize(0);
}

//---------------------------------------------------------------------------
// METHOD:       CFeelingWelfare::~CFeelingWelfare
//  
// DESCRIPTION:  Destructor 
// 
// ARGUMENTS:    None
//               
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CFeelingWelfare::~CFeelingWelfare()
{
  for (size_t i = 0; i < m_tSensitivity.size(); i++)
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
// METHOD:       CFeelingWelfare::ComputeAndGetFeelingWelfare
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
double CFeelingWelfare::ComputeAndGetFeelingWelfare()
{
  m_CurSatisfaction = 0;

  for (auto pSensitivity : m_tSensitivity)
  {
    const std::vector<sensorValType>& vectStimulation{ pSensitivity->m_pSens->UpdateAndGetStimulationTable() };
    for (size_t j = 0; j < vectStimulation.size(); j++)
    {
      m_CurSatisfaction += pSensitivity->m_pSensitivityTableMask[j] * vectStimulation[j];
    }
  }

  if (m_CurSatisfaction > 100.0)
    m_CurSatisfaction = 100.0;

  if (m_CurSatisfaction < -100.0)
    m_CurSatisfaction = -100.0;

  return m_CurSatisfaction;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

bool CFeelingWelfare::AddSensitivity(CSensor* pSens, size_t tableMaskSize, double* pTableMask)
{
  if (pSens->GetSubCaptorNumber() != tableMaskSize)
  {
    CYBIOCORE_LOG_TIME(m_pEntity->getBiotop()->getBiotopTime());
    CYBIOCORE_LOG("ANIMAL - ERROR : %s incorect sensitivity size %d for sensor %s\n", m_pEntity->getLabel().c_str(), tableMaskSize, pSens->GetLabel().c_str());
    return false;
  }

  Sensitivity_t* pSensitivity = new Sensitivity_t;

  if (pSensitivity == NULL)
    return false;

  pSensitivity->m_pSens = pSens;
  pSensitivity->m_pSensitivityTableMask = pTableMask;

  m_tSensitivity.push_back(pSensitivity);

  return true;
}

double CFeelingWelfare::GetCurFeelingWelfare()
{
  return m_CurSatisfaction;
}

double CFeelingWelfare::GetSensorFeelingImpact(size_t index, std::string &infoStr)
{
  double satisfaction = 0;
  Sensitivity_t* pSensitivity = NULL;

  if (index < m_tSensitivity.size())
  {
    pSensitivity = m_tSensitivity[index];
    const std::vector<sensorValType>& vectStimulation { pSensitivity->m_pSens->UpdateAndGetStimulationTable() };
    for (size_t j = 0; j < vectStimulation.size(); j++)
    {
      satisfaction = pSensitivity->m_pSensitivityTableMask[j] * vectStimulation[j];
    }
    infoStr = pSensitivity->m_pSens->GetLabel();
  }
  else
  {
    infoStr = "Bad index";
  }

  return satisfaction;
}

