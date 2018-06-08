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
// FILE: CMeasure.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#if !defined(CMEASURE_INCLUDED_)
#define CMEASURE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>

#include "Definitions.h"

//===========================================================================
// Definitions            
//===========================================================================

#define MAX_MEASUREMENT_DATA_SIZE 1000

typedef struct 
{
  double value;
  timeCountType timeCount;
} MeasureData_t;

typedef enum
{
  MEASURE_TYPE_GENERIC = 0,
  MEASURE_TYPE_PARAMETER_ENTITY,
  MEASURE_TYPE_PARAMETER_BIOTOP,
  MEASURE_TYPE_FEELING,    
  MEASURE_TYPE_REACTION,
  MEASURE_TYPE_POPULATION,
  MEASURE_NUMBER_TYPE 
} MeasureType_e;

typedef enum
{
  EVENT_TYPE_NONE = 0,
  EVENT_TYPE_UNDER_THRESHOLD,
  EVENT_TYPE_EGAL_THRESHOLD,
  EVENT_TYPE_OVER_THRESHOLD,    
  EVENT_NUMBER_TYPE 
} EventType_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CMeasure  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  int            m_Period;        // Period in second
  int            m_Id;
  MeasureType_e  m_Type;
  double         m_RangeMin;
  double         m_RangeMax;
  string         m_Label;
  bool           m_bIsRecording;
  timeCountType  m_CounterPeriod;
  BiotopTime_t   m_StartBioTime;
  timeCountType  m_TotalMeasNbFromStart;
  MeasureData_t  m_tCurValTable[MAX_MEASUREMENT_DATA_SIZE];
  int            m_IndexCurData;

  EventType_e   m_EventType;
  double        m_EventThreshold;


//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CMeasure(int period, int id, double rangeMin, double rangeMax, MeasureType_e type);
	virtual ~CMeasure();

//---------------------------------------------------------------------------
// Command
//---------------------------------------------------------------------------
public:
  void StartMeasurement(BiotopTime_t startTime);
  void StopMeasurement();

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  bool NextSecond();

//---------------------------------------------------------------------------
// Measurement management
//---------------------------------------------------------------------------
public:
  virtual double GetCurrentValue() = 0;
  virtual bool IsStillValid();

//---------------------------------------------------------------------------
// Event management
//---------------------------------------------------------------------------
public:
  bool SetEvent(EventType_e type, double threshold);
  bool CheckEvent(void);

//---------------------------------------------------------------------------
// Save in File
//---------------------------------------------------------------------------
  bool saveInFile(string fileNameWithPath);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  int GetId();
  string GetLabel();
  MeasureType_e GetType();
  virtual int GetSubTypeId();
  MeasureData_t* GetPMeasureData();
  int GetIndexCurData();
  double GetRangeMin();
  double GetRangeMax();
  bool AutoUpdateRange();
  static string getMeasureTypeStrName(MeasureType_e type);
  int    GetPeriod();
  bool IsRecording();
  EventType_e GetEventType();
  double GetEventThreshold();
  static string getEventTypeStrName(EventType_e type);
};

#endif // !defined(CMEASURE_INCLUDED_)
