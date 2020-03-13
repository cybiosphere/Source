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
  m_pSubCaptorWeightRate = new double[1];
  m_pSubCaptorWeightRate[0] = weightRate;
  m_BonusRate.resize(1);
  m_BonusRate[0] = 100;
  m_pStimulationValues = new sensorValType[1];
  m_pStimulationValues[0] = 0;
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
CSensor::CSensor(int subCaptorNumber, double* pSubCaptorWeightRate, DWORD uniqueId)
{
  m_Label = "Basic Sensor";
  m_SubCaptorNumber = subCaptorNumber;
  m_pSubCaptorWeightRate = new double[subCaptorNumber];
  m_pStimulationValues = new sensorValType[subCaptorNumber];
  m_BonusRate.resize(subCaptorNumber);
  m_UniqueId  = uniqueId;

  for (int i=0; i<m_SubCaptorNumber; i++)
  {
    m_pSubCaptorWeightRate[i] = pSubCaptorWeightRate[i];
    m_pStimulationValues[i] = 0;
    m_BonusRate[i] = 100;
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
CSensor::CSensor(int subCaptorNumber, double SubCaptorsWeightRate, DWORD uniqueId)
{
  m_Label = "Basic Sensor";
  m_SubCaptorNumber = subCaptorNumber;
  m_pSubCaptorWeightRate = new double[subCaptorNumber];
  m_pStimulationValues = new sensorValType[subCaptorNumber];
  m_BonusRate.resize(subCaptorNumber);
  m_UniqueId  = uniqueId;

  for (int i=0; i<m_SubCaptorNumber; i++)
  {
    m_pSubCaptorWeightRate[i] = SubCaptorsWeightRate;
    m_pStimulationValues[i] = 0;
    m_BonusRate[i] = 100;
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
  delete [] m_pSubCaptorWeightRate;
  delete [] m_pStimulationValues;
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
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Should be overloaded
//---------------------------------------------------------------------------
int CSensor::UpdateAndGetStimulationTable(sensorValType*& pStimulationVal)
{
  // Default sensor: detect nothing
  for (int i=0; i<m_SubCaptorNumber; i++)
  {
    m_pStimulationValues[i] = 0;
  }
  pStimulationVal = m_pStimulationValues;
  return m_SubCaptorNumber;
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
sensorValType CSensor::GetSubCaptorStimulationLevel(int captorIndex)
{
  if ((0<=captorIndex)&&(captorIndex<m_SubCaptorNumber))
    return (m_pStimulationValues[captorIndex]*100/m_pSubCaptorWeightRate[captorIndex]);
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
int CSensor::GetStimulationTable(sensorValType*& pStimulationVal)
{
  pStimulationVal = m_pStimulationValues;
  return m_SubCaptorNumber;
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
bool CSensor::IsSexSpecific(int captorIndex)
{
  return false;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================
int CSensor::GetSubCaptorNumber()
{
  return (m_SubCaptorNumber);
}

string CSensor::GetLabel()
{
  return (m_Label);
}

DWORD CSensor::GetUniqueId()
{
  return (m_UniqueId);
}

int CSensor::GetBonusRate(int subCaptorInd)
{
  return (m_BonusRate[subCaptorInd]);
}

bool CSensor::SetBonusRate(int subCaptorInd, int bonus)
{
  if (GetSubCaptorNumber() >= subCaptorInd)
    return false;

  m_BonusRate[subCaptorInd] = bonus;
  return true;
}

bool CSensor::SetBonusRate(int tableBonusSize, const std::vector<int>& pTableBonus)
{
  if (GetSubCaptorNumber() != tableBonusSize)
    return false;

  for (int i=0; i<tableBonusSize; i++)
  {
    // Check if bonus is present on subcaptor
    if (pTableBonus[i] != 100)
    {
      if (m_BonusRate[i] == 100)
      {
        // No previous bonus on this sub captor
        m_BonusRate[i] = pTableBonus[i];
      }
      else
      {
        // Bonus already present on this sub captor. Give avarage value
        m_BonusRate[i] = (m_BonusRate[i] + pTableBonus[i]) / 2;
      }
    }
  }
  return true;
}

void CSensor::SetBonusRateToNeutral()
{
  for (int i=0; i< m_BonusRate.size(); i++)
  {
    m_BonusRate[i] = 100;
  }
}