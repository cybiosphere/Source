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
// FILE: CSensorComposite.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used to combine an existing sensor 
//         with a parameter value. 
//          
//          
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#include "CSensorComposite.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorComposite::CSensorComposite
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[NUMBER_ODORS] : table of weight for each odor
//               int range: Distance scanned to find odor
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorComposite::CSensorComposite(CBrainAnimal* pBrain, std::vector<double>& tWeightRate, size_t subCaptorNumber, DWORD baseSensUId, size_t paramIndex)
:CSensor(subCaptorNumber, tWeightRate, UID_BASE_SENS_COMPOSITE + (baseSensUId & UID_BASE_MASK)/256 + (baseSensUId & 0xFF) + ((DWORD)paramIndex * 256) )
{
  m_pBrain      = pBrain;
  m_Label       = "Comp";
  m_BaseSensUId = baseSensUId;
  m_ParamIndex  = paramIndex;
  m_pBaseSens   = NULL;
  m_pParam      = NULL;
}

CSensorComposite::~CSensorComposite()
{
}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorComposite::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to smelled entities 
//               The Weight of captors is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorComposite::UpdateAndGetStimulationTable()
{
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  if (m_pBaseSens == NULL)
    m_pBaseSens = m_pBrain->GetSensorByUniqueId(m_BaseSensUId);

  if (m_pParam == NULL)
    m_pParam = m_pBrain->getAnimal()->getParameter(m_ParamIndex);

  if ((m_pBaseSens == NULL) || (m_pParam == NULL))
  {
    // No sensor or param found => no stimulation
    return m_tStimulationValues;
  }

  double factor = m_pParam->getVal();
  const std::vector<sensorValType>& vectorBaseStimulation{ m_pBaseSens->UpdateAndGetStimulationTable() };
  
  for (size_t i = 0; i < m_SubCaptorNumber; i++)
  {
    // Compute stimulation
    if (vectorBaseStimulation[i] != 0) // CPU Optim: vectorBaseStimulation[i] is often 0
      m_tStimulationValues[i] = vectorBaseStimulation[i] * factor * m_tSubCaptorWeightRate[i] / 100.0;
    else
      m_tStimulationValues[i] = 0;
  }

  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorComposite::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorComposite::GetSubCaptorLabel(size_t index)
{
  if (m_pBaseSens == NULL)
    m_pBaseSens = m_pBrain->GetSensorByUniqueId(m_BaseSensUId);

  if (m_pParam == NULL)
    m_pParam = m_pBrain->getAnimal()->getParameter(m_ParamIndex);

  if ((m_pBaseSens == NULL) || (m_pParam == NULL))
  {
    return "error";
  }

  if (index > GetSubCaptorNumber())
    return ("bad index");
  else
  {
    return (m_pParam->getLabel() + " x " + m_pBaseSens->GetLabel() + " " + m_pBaseSens->GetSubCaptorLabel(index) );
  }
}

//===========================================================================
// Other functions
//===========================================================================
CSensor* CSensorComposite::GetBaseSens()
{
  if (m_pBaseSens == NULL)
    m_pBaseSens = m_pBrain->GetSensorByUniqueId(m_BaseSensUId);

  return m_pBaseSens;
}