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
// FILE: CPurpose.h
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

#if !defined( CPURPOSE_INCLUDED_)
#define CPURPOSE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "Definitions.h"
#include "CSensor.h"
#include "CReaction.h"

//===========================================================================
// Definitions            
//===========================================================================

typedef enum
{
  PURPOSE_TRIG_UP = 0,
  PURPOSE_TRIG_DOWN
} PurposeTriggerType_e;

class CSensor;
class CReaction;

typedef struct 
{
  CReaction*  pReaction;
  int         bonusRate;  // from 0 to 100 :malus, over 100 bonus
} ReactionBonus_t;

typedef struct 
{
  CSensor*    pSensor;
  int         bonusTableSize;
  int*        pBonusTable;  // from 0 to 100 :malus, over 100 bonus
} SensorBonus_t;

#define SENSOR_TABLE_SIZE 10
#define REACTION_TABLE_SIZE 10

//#define START_SENSOR_THRESHOLD 3.0 // 30% of MAX_SENSOR_VAL 
//#define STOP_SENSOR_THRESHOLD  2.0 // 20% of MAX_SENSOR_VAL 

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CPurpose
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
private:
  string                m_Label;           // User defined name
  int                   m_SecondDuration;
  int                   m_SecondCounter;

  CSensor*              m_pSensor;
  int                   m_subCaptorIndex;
  int                   m_StartRateThreshold;  // purpose is launched when m_pSensor level reach this max rate
  int                   m_StopRateThreshold;   // purpose is stopped when m_pSensor level reach this max rate 
  PurposeTriggerType_e  m_TriggerType;
  
  DWORD                 m_UniqueId;            // Global identifier
  int                   m_Priority;            // Used to select active purpose (from 0:low to 5:max)
  bool                  m_bMemorizeSuccess;    // Used to specify that success geographic position can be memorized

  int                   m_nbSensorBonus;
  SensorBonus_t*        m_tSensorBonus;
  int                   m_nbReactionBonus;
  ReactionBonus_t*      m_tReactionBonus;


//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------  
public:
  CPurpose(string label, int minuteDuration, 
           CSensor* pSens, int subCaptorIndex, 
           int startRateThreshold, int stopRateThreshold, PurposeTriggerType_e type);

  virtual ~CPurpose();

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  bool CheckSucces(void);
  bool IncreaseCountAndCheckEnd(void);
  void StartPeriod(void);
  void StopPeriod(void);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  bool AddSensorBonus (CSensor* pSensor, int bonusTableSize, int* pBonusTable);
  bool AddReactionBonus (CReaction* pReaction,int bonusRate);
  sensorValType GetSensorLevel();
  bool IsStartThresholdReached();
  string GetLabel(void);
  DWORD GetUniqueId();
  int GetPriority();
  static DWORD ComputeUniqueId(DWORD triggerSensUid, int triggerSubCaptorIndex);
  bool IsMemorizeSuccess();

};// end CPurpose

#endif // !defined(CPURPOSE_INCLUDED_)


