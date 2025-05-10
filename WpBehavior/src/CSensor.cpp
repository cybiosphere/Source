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
// FILE: CSensor.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a basic sensor
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CSensor.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensor::CSensor
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    double weightRate: {0..100} - Sum of all weights should be 100
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensor::CSensor(double weightRate, DWORD uniqueId)
{
  m_Label = "Basic Sensor";
  m_SubCaptorNumber = 1;
  m_tSubCaptorWeightRate.resize(1);
  m_tSubCaptorWeightRate[0] = weightRate;
  m_tBonusRate.resize(1);
  m_tBonusRate[0] = 100;
  m_tStimulationValues.resize(1);
  m_tStimulationValues[0] = 0;
  m_UniqueId = uniqueId;
}

//---------------------------------------------------------------------------
// METHOD:       CSensor::CSensor
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    int subCaptorNumber: Number of doubles processed by Sensor
//               double* pSubCaptorWeightRate: {0..100} - table of weight
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensor::CSensor(size_t subCaptorNumber, std::vector<double>& tSubCaptorWeightRate, DWORD uniqueId)
{
  m_Label = "Basic Sensor";
  m_SubCaptorNumber = subCaptorNumber;
  m_tSubCaptorWeightRate.resize(subCaptorNumber);
  m_tStimulationValues.resize(subCaptorNumber);
  m_tBonusRate.resize(subCaptorNumber);
  m_UniqueId  = uniqueId;

  for (size_t i=0; i<m_SubCaptorNumber; i++)
  {
    m_tSubCaptorWeightRate[i] = tSubCaptorWeightRate[i];
    m_tStimulationValues[i] = 0;
    m_tBonusRate[i] = 100;
  }
}

//---------------------------------------------------------------------------
// METHOD:       CSensor::CSensor
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    int subCaptorNumber: Number of doubles processed by Sensor
//               double pSubCaptorsWeightRate: {0..100} - weight applied to all
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensor::CSensor(size_t subCaptorNumber, double SubCaptorsWeightRate, DWORD uniqueId)
{
  m_Label = "Basic Sensor";
  m_SubCaptorNumber = subCaptorNumber;
  m_tSubCaptorWeightRate.resize(subCaptorNumber);
  m_tStimulationValues.resize(subCaptorNumber);
  m_tBonusRate.resize(subCaptorNumber);
  m_UniqueId  = uniqueId;

  for (size_t i=0; i<m_SubCaptorNumber; i++)
  {
    m_tSubCaptorWeightRate[i] = SubCaptorsWeightRate;
    m_tStimulationValues[i] = 0;
    m_tBonusRate[i] = 100;
  }
}

//---------------------------------------------------------------------------
// METHOD:       CSensor::~CSensor
//  
// DESCRIPTION:  Destructor 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensor::~CSensor()
{
}

//===========================================================================
// Virtual
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensor::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to entity and environment 
//               The Weight of each captor should be taken into account here
//
// ARGUMENTS:    none
//   
// RETURN VALUE: m_tStimulationValues vector
//  
// REMARKS:      Should be overloaded
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensor::UpdateAndGetStimulationTable()
{
  // Default sensor: detect nothing
  for (size_t i=0; i<m_SubCaptorNumber; i++)
  {
    m_tStimulationValues[i] = 0;
  }
  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensor::GetSubCaptorStimulationLevel
//  
// DESCRIPTION:  Return sub-captor stimulation level WITHOUT weighting rate
//
// ARGUMENTS:    int captorIndex : index of the sub captor (def = 0)
//   
// RETURN VALUE: sensorValType: Sensor stimulation not using weight
//  
// REMARKS:      
//---------------------------------------------------------------------------
sensorValType CSensor::GetSubCaptorStimulationLevel(size_t captorIndex)
{
  if (captorIndex < m_SubCaptorNumber)
    return (m_tStimulationValues[captorIndex] * 100 / m_tSubCaptorWeightRate[captorIndex]);
  else
    return (0);
}

//---------------------------------------------------------------------------
// METHOD:       CSensor::GetStimulationTable
//  
// DESCRIPTION:  Get Stimulation table of sensor already updated by entity 
//
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensor::GetStimulationTable()
{
  return m_tStimulationValues;
}

//---------------------------------------------------------------------------
// METHOD:       CSensor::IsSexSpecific
//  
// DESCRIPTION:  Return true if the sub-captor is used a different way 
//               according to sex
//
// ARGUMENTS:    int captorIndex : index of the sub captor (def = 0)
//   
// RETURN VALUE: true: sex specific, false: no sexual differenciation
//  
// REMARKS:      to be overloaded
//---------------------------------------------------------------------------
bool CSensor::IsSexSpecific(size_t captorIndex)
{
  return false;
}

//===========================================================================
// Shared method
//===========================================================================
void CSensor::applySubCaptorWeightRate()
{
  for (size_t i = 0; i < m_SubCaptorNumber; i++)
  {
    if (m_tStimulationValues[i] != 0) // CPU optim: StimulationValue is often 0
    {
      // Use weight
      m_tStimulationValues[i] = m_tStimulationValues[i] * m_tSubCaptorWeightRate[i] / 100.0;
      // Don't go over Max!
      if (m_tStimulationValues[i] > MAX_SENSOR_VAL)
        m_tStimulationValues[i] = MAX_SENSOR_VAL;
      if (m_tStimulationValues[i] < -MAX_SENSOR_VAL)
        m_tStimulationValues[i] = -MAX_SENSOR_VAL;
    }
  }
}


//===========================================================================
// Get / Set for attributes
//===========================================================================
size_t CSensor::GetSubCaptorNumber()
{
  return m_SubCaptorNumber;
}

string CSensor::GetLabel()
{
  return m_Label;
}

DWORD CSensor::GetUniqueId()
{
  return m_UniqueId;
}

int CSensor::GetBonusRate(size_t subCaptorInd)
{
  return (m_tBonusRate[subCaptorInd]);
}

bool CSensor::SetBonusRate(size_t subCaptorInd, int bonus)
{
  if (GetSubCaptorNumber() >= subCaptorInd)
    return false;

  m_tBonusRate[subCaptorInd] = bonus;
  return true;
}

bool CSensor::SetBonusRate(const std::vector<int>& tBonus)
{
  if (GetSubCaptorNumber() != tBonus.size())
    return false;

  for (size_t i = 0; i < tBonus.size(); i++)
  {
    // Check if bonus is present on subcaptor
    if (tBonus[i] != 100)
    {
      if (m_tBonusRate[i] == 100)
      {
        // No previous bonus on this sub captor
        m_tBonusRate[i] = tBonus[i];
      }
      else
      {
        // Bonus already present on this sub captor. Give avarage value
        m_tBonusRate[i] = (m_tBonusRate[i] + tBonus[i]) / 2;
      }
    }
  }
  return true;
}

void CSensor::SetBonusRateToNeutral()
{
  for (size_t i=0; i< m_tBonusRate.size(); i++)
  {
    m_tBonusRate[i] = 100;
  }
}