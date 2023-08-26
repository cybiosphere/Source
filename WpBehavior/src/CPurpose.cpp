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
// FILE: CPurpose.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a purpose for a brain.
//         It associates a sensor to a set of prefered reactions
//         When the current puprose ended (sensor < stop threshold or timeout),
//         the most stimulized sensor over start threshold is selected.        
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  21/12/2005 : Creation
//
//===========================================================================
 
#include "CPurpose.h"
#include "CBrainAnimal.h"

//===========================================================================
// Constructors / Destructors 
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPurpose::CPurpose
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    string label : user name
//               int MinuteDuration : duration in minute
//               CSensor* pSens : Should be a single captor associated to a parameter.
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CPurpose::CPurpose(string label, int minuteDuration, CSensor* pSens, size_t subCaptorIndex,
                   int startRateThreshold, int stopRateThreshold, PurposeTriggerType_e type)
{
  int i;
  m_Label           = label;
  m_SecondDuration  = minuteDuration * NUMBER_SECONDS_PER_MINUTE;
  m_SecondCounter   = 0;

  m_pSensor         = pSens;
  m_subCaptorIndex  = subCaptorIndex;

  m_StartRateThreshold  = startRateThreshold;  
  m_StopRateThreshold   = stopRateThreshold;
  m_TriggerType         = type;

  m_bMemorizeSuccess = false;
  m_LevelBonus = 0;
  m_nbSensorBonus = 0;
  for (i=0; i<SENSOR_TABLE_SIZE; i++)
  {
    m_tSensorBonus[i].pSensor = NULL;
    m_tSensorBonus[i].bonusTable.clear();
  }

  m_nbReactionBonus = 0;
  for (i=0; i<REACTION_TABLE_SIZE; i++)
  {
    m_tReactionBonus[i].pReaction = NULL;
    m_tReactionBonus[i].bonusRate = 100;
  }

  // Try to give a priority according to trigger sens (TBD should be genetic info)
  switch (pSens->GetUniqueId() & UID_BASE_MASK)
  {
  case UID_BASE_SENS_HORMONE:     
      if (subCaptorIndex == PheromoneTypeToIndex(PHEROMONE_BABY))
        m_Priority = 8; // For baby purpose
      else
        m_Priority = 3; // For mother purpose
      m_bMemorizeSuccess = true;
      break;
  case UID_BASE_SENS_TIREDNESS: 
  case UID_BASE_SENS_FEAR:
      m_Priority = 6;
      break;
  case UID_BASE_SENS_STOMACH_FULL:
      m_Priority = 5;
      break;
  case UID_BASE_SENS_THIRST:
      m_Priority = 4;
      m_bMemorizeSuccess = true;
      break;
  case UID_BASE_SENS_HUNGER:
      m_Priority = 3;
      m_LevelBonus = 2.5;
      m_bMemorizeSuccess = true;
    break;
  case UID_BASE_SENS_LIBIDO:    // For Reproduction
  case UID_BASE_SENS_PHEROMONE:
      m_Priority = 2;
      break;
  case UID_BASE_SENS_SMELL:     // For Regroup
      m_Priority = 1;
      m_bMemorizeSuccess = true; // TBC (help to regroup)
      break;
  case UID_BASE_SENS_OBSCURITY: 
  default:
      m_Priority = 0;
      break;
  }

  m_UniqueId = ComputeUniqueId (pSens->GetUniqueId(), subCaptorIndex);
}

CPurpose::~CPurpose()
{
}

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPurpose::CheckSucces
//  
// DESCRIPTION:  Check if purpose sensor has reached expected level
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool: true if threshold reached
//  
// REMARKS:      Global application speed highly impacted by this method
//---------------------------------------------------------------------------
bool CPurpose::CheckSucces(void)
{
  if (((m_TriggerType == PURPOSE_TRIG_UP)   && (GetSensorLevel()<(m_StopRateThreshold*MAX_SENSOR_VAL/100.0)))
   || ((m_TriggerType == PURPOSE_TRIG_DOWN) && (GetSensorLevel()>(m_StopRateThreshold*MAX_SENSOR_VAL/100.0))))
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CPurpose::increaseCountAndCheckEnd
//  
// DESCRIPTION:  Decrease m_SecondCounter and check if = 0
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool: true if end reached (timeout)
//  
// REMARKS:      Global application speed highly impacted by this method
//---------------------------------------------------------------------------
bool CPurpose::IncreaseCountAndCheckEnd(void)
{
  m_SecondCounter++;

  if (m_SecondCounter>=m_SecondDuration) 
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CPurpose::startPeriod
//  
// DESCRIPTION:  Begin a new period with this purpose
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CPurpose::StartPeriod(void)
{
  size_t i;
  // Reset counter
  m_SecondCounter = 0;

  // Set specific bonus on each sensor
  CSensor* curSens = NULL;
  for (i=0; i<m_nbSensorBonus; i++)
  {
    curSens = m_tSensorBonus[i].pSensor; 
    if (curSens!=NULL)
    {
      curSens->SetBonusRate(m_tSensorBonus[i].bonusTable);
    }
  }

  // Set specific bonus on each reaction
  CReaction* curReact = NULL;
  for (i=0; i<m_nbReactionBonus; i++)
  {
    curReact = m_tReactionBonus[i].pReaction; 
    if (curReact!=NULL)
    {
      curReact->SetBonusRate(m_tReactionBonus[i].bonusRate);
    }
  }
}

//---------------------------------------------------------------------------
// METHOD:       CPurpose::stopPeriod
//  
// DESCRIPTION:  Stop current purpose
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CPurpose::StopPeriod(void)
{
  size_t i;
  // Reset bonus to neutral on each sensor
  CSensor* curSens = NULL;
  for (i=0; i<m_nbSensorBonus; i++)
  {
    curSens = m_tSensorBonus[i].pSensor;
    if (curSens!=NULL)
    {
      curSens->SetBonusRateToNeutral();
    }
  }

  // Reset bonus to neutral on each reaction
  CReaction* curReact = NULL;
  for (i=0; i<m_nbReactionBonus; i++)
  {
    curReact = m_tReactionBonus[i].pReaction;
    if (curReact!=NULL)
    {
      curReact->SetBonusRateToNeutral();
    }
  }
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

bool CPurpose::AddSensorBonus (CSensor* pSensor, std::vector<int>& bonusTable)
{
  if ((m_nbSensorBonus>SENSOR_TABLE_SIZE-1)||(bonusTable.empty()))
  {
    return (false);
  }
  
  m_tSensorBonus[m_nbSensorBonus].pSensor    = pSensor;
  m_tSensorBonus[m_nbSensorBonus].bonusTable = std::move(bonusTable);
  m_nbSensorBonus++;
  return (true);
}

bool CPurpose::AddReactionBonus (CReaction* pReaction,int bonusRate)
{
  if (m_nbReactionBonus>REACTION_TABLE_SIZE-1)
  {
    return (false);
  }
  
  m_tReactionBonus[m_nbReactionBonus].pReaction = pReaction;
  m_tReactionBonus[m_nbReactionBonus].bonusRate = bonusRate;
  m_nbReactionBonus++;
  return (true);
}

sensorValType CPurpose::GetSensorLevel()
{
  sensorValType  resu = 0;
  if (m_pSensor)
  {
    resu = m_pSensor->GetSubCaptorStimulationLevel(m_subCaptorIndex);
  }
  return (resu);
}

bool CPurpose::IsStartThresholdReached()
{
  sensorValType  resu = GetSensorLevel();
  if (((m_TriggerType == PURPOSE_TRIG_UP)   && (resu > (m_StartRateThreshold*MAX_SENSOR_VAL/100.0)))
   || ((m_TriggerType == PURPOSE_TRIG_DOWN) && (resu < (m_StartRateThreshold*MAX_SENSOR_VAL/100.0))))
    return (true);
  else
    return (false);
}

string CPurpose::GetLabel()
{
  return (m_Label);
}

DWORD CPurpose::GetUniqueId()
{
  return (m_UniqueId);
}

DWORD CPurpose::ComputeUniqueId(DWORD triggerSensUid, size_t triggerSubCaptorIndex)
{
  return (triggerSensUid + (DWORD)triggerSubCaptorIndex);
}

int CPurpose::GetPriority()
{
  return (m_Priority);
}

double CPurpose::GetLevelBonus()
{
  return (m_LevelBonus);
}

bool CPurpose::IsMemorizeSuccess()
{
  return m_bMemorizeSuccess;
}