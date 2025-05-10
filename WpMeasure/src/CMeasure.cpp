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
// FILE: CMeasure.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic Mesure
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#include "CMeasure.h"

//===========================================================================
// Definitions            
//===========================================================================

const char* MeasureTypeNameList[MEASURE_NUMBER_TYPE] =
{
  "Generic",
  "Parameter entity",
  "Parameter biotop",
  "Feeling entity",
  "Action entity",
  "Population biotop",
};

const char* EventTypeNameList[EVENT_NUMBER_TYPE] =
{
  "NONE",
  "UNDER",
  "EGAL",
  "OVER"
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

CMeasure::CMeasure(int period, int id, double rangeMin, double rangeMax, MeasureType_e type)
{
  m_Period   = period;
  m_Id       = id;
  m_Type     = type;
  m_RangeMin = rangeMin;
  m_RangeMax = rangeMax;

  m_bIsRecording  = false;
  m_CounterPeriod = 0;
  m_StartBioTime.seconds = 0;
  m_StartBioTime.hours   = 0;
  m_StartBioTime.days    = 0;
  m_StartBioTime.years   = 0;
  m_TotalMeasNbFromStart = 0;
  m_IndexCurData = invalidIndex;
  m_paramIndex = invalidIndex;
  m_pEntity = NULL;

  m_EventType = EVENT_TYPE_NONE;
  m_EventThreshold = 0;

  for (int i=0; i<MAX_MEASUREMENT_DATA_SIZE; i++)
  {
    m_tCurValTable[i].value = 0;
    m_tCurValTable[i].timeCount = 0;
  }
}

CMeasure::~CMeasure()
{
}

//===========================================================================
// Command
//===========================================================================
void CMeasure::StartMeasurement(BiotopTime_t startTime)
{
  m_bIsRecording  = true;
  m_CounterPeriod = 0;
  // For new measures, set initial time
  if (m_TotalMeasNbFromStart == 0)
  {
    m_StartBioTime.seconds = startTime.seconds;
    m_StartBioTime.hours = startTime.hours;
    m_StartBioTime.days = startTime.days;
    m_StartBioTime.years = startTime.years;
    m_IndexCurData = invalidIndex;
  }
  NextSecond();
}

void CMeasure::StopMeasurement()
{
  m_bIsRecording  = false;
}

//===========================================================================
// Time management
//===========================================================================
bool CMeasure::NextSecond()
{
  if (m_bIsRecording)
  {
    if (m_CounterPeriod == 0)
    {
      timeCountType prevTime;
      if (m_IndexCurData != invalidIndex)
        prevTime = m_tCurValTable[m_IndexCurData].timeCount;
      else
        prevTime =  convertBioTimeToCount(m_StartBioTime) - m_Period + 1;

      m_IndexCurData = (m_IndexCurData+1)%MAX_MEASUREMENT_DATA_SIZE;
      m_tCurValTable[m_IndexCurData].value = GetCurrentValue();
      m_tCurValTable[m_IndexCurData].timeCount = prevTime + m_Period;
      m_TotalMeasNbFromStart++;
    }
    m_CounterPeriod = (m_CounterPeriod + 1) % m_Period;
  }

  return (IsStillValid());
}


bool CMeasure::IsStillValid()
{
  return true;
}

//===========================================================================
// Event management
//===========================================================================
bool CMeasure::SetEvent(EventType_e type, double threshold)
{
  if ( (threshold<m_RangeMin) || (threshold>m_RangeMax) )
    return false;

  m_EventType = type;
  m_EventThreshold = threshold;

  return true;
}

bool CMeasure::CheckEvent(void)
{
  bool resu = false;

  switch(m_EventType)
  {
  case EVENT_TYPE_UNDER_THRESHOLD:
    {
      resu = (GetCurrentValue()<m_EventThreshold);
      break;
    }
  case EVENT_TYPE_EGAL_THRESHOLD:
    {
      resu = ( fabs(GetCurrentValue()-m_EventThreshold)<0.01 );
      break;
    }
  case EVENT_TYPE_OVER_THRESHOLD:
    {
      resu = (GetCurrentValue()>m_EventThreshold);
      break;
    }
  default:
    {
      resu = false;
      break;
    }
  }

  return resu;
}

//===========================================================================
// Raw data conversion
//===========================================================================
string CMeasure::buildStringDataFromMeasure()
{
  size_t i;
  string tmpStr;
  string strMeasure = GetLabel() + "\n";

  if (m_TotalMeasNbFromStart > MAX_MEASUREMENT_DATA_SIZE) // Reorder ring buffer when buffer is full
  {
    for (i = m_IndexCurData + 1; i < MAX_MEASUREMENT_DATA_SIZE; i++)
    {
      tmpStr = FormatString("%u;", m_tCurValTable[i].timeCount);
      strMeasure += tmpStr;
    }
  }
  for (i = 0; i <= m_IndexCurData; i++)
  {
    tmpStr = FormatString("%u;", m_tCurValTable[i].timeCount);
    strMeasure += tmpStr;
  }
  strMeasure += "\n";

  if (m_TotalMeasNbFromStart > MAX_MEASUREMENT_DATA_SIZE) // Reorder ring buffer when buffer is full
  {
    for (i = m_IndexCurData + 1; i < MAX_MEASUREMENT_DATA_SIZE; i++)
    {
      tmpStr = FormatString("%.2f;", m_tCurValTable[i].value);
      strMeasure += tmpStr;
    }
  }
  for (i = 0; i <= m_IndexCurData; i++)
  {
    tmpStr = FormatString("%.2f;", m_tCurValTable[i].value);
    strMeasure += tmpStr;
  }
  strMeasure += "\n\n";

  return strMeasure;
}

std::vector<std::string> CMeasure::split(const std::string& s, char delimiter)
{
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

bool CMeasure::buildMeasureDataFromString(string dataString)
{
  size_t endFirstLine = dataString.find("\n");
  size_t endSencondLine = dataString.find("\n", endFirstLine + 1);
  size_t endThirdLine = dataString.find("\n", endSencondLine + 1);
  string label = dataString.substr(0, endFirstLine);
  string timeStampDataStr = dataString.substr(endFirstLine + 1, endSencondLine - endFirstLine - 1);
  string userDataStr = dataString.substr(endSencondLine + 1, endThirdLine - endSencondLine - 1);

  auto vectorTimeStampDataStr = split(timeStampDataStr, ';');
  auto vectorUserDataStr = split(userDataStr, ';');
  size_t nbPoints = cybio_min(vectorTimeStampDataStr.size(), vectorUserDataStr.size());
  for (size_t i = 0; i < nbPoints; i++)
  {
    m_tCurValTable[i].timeCount = stoi(vectorTimeStampDataStr[i]);
    m_tCurValTable[i].value = stof(vectorUserDataStr[i]);
    m_IndexCurData++;
  }
  m_TotalMeasNbFromStart = (timeCountType)nbPoints;
  AutoUpdateRange();
  return true;
}

//===========================================================================
// Save in File
//===========================================================================
bool CMeasure::saveInFile(string fileNameWithPath)
{
  string savedMeasure = buildStringDataFromMeasure();
  ofstream f1;
  f1.open(fileNameWithPath.c_str(), std::ofstream::out | std::ofstream::app);
  f1.write(savedMeasure.c_str(), savedMeasure.length());
  f1.close();
  return true;
}

//===========================================================================
// data access
//===========================================================================
timeCountType CMeasure::getNextSmallestTimeCount(timeCountType previousSmallestTimeCount)
{
  timeCountType nextMinTimeCount = MAX_TIMECOUNT_VALUE;
  timeCountType tempTimeCount = 0;
  size_t numData = cybio_min(m_TotalMeasNbFromStart, (timeCountType)MAX_MEASUREMENT_DATA_SIZE);
  for (size_t i = 0; i < numData; i++)
  {
    tempTimeCount = m_tCurValTable[i].timeCount;
    if ((tempTimeCount > previousSmallestTimeCount) && (tempTimeCount < nextMinTimeCount))
      nextMinTimeCount = tempTimeCount;
  }
  return nextMinTimeCount;
}

bool CMeasure::getMeasureFromTimeStamp(timeCountType timeCount, MeasureData_t& measureData)
{
  size_t numData = cybio_min(m_TotalMeasNbFromStart, (timeCountType)MAX_MEASUREMENT_DATA_SIZE);
  for (size_t i = 0; i < numData; i++)
  {
    if (m_tCurValTable[i].timeCount == timeCount)
    {
      measureData = m_tCurValTable[i];
      return true;
    }
  }
  return false;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================
int CMeasure::GetId()
{
  return m_Id;
}

string CMeasure::GetLabel()
{
  return m_Label;
}

MeasureType_e CMeasure::GetType()
{
  return m_Type;
}

size_t CMeasure::GetSubTypeId()
{
  return 0;
}

MeasureData_t* CMeasure::GetPMeasureData()
{
  return m_tCurValTable;
}

size_t CMeasure::GetIndexCurData()
{
  return m_IndexCurData;
}

timeCountType CMeasure::GetTotalMeasureNumberFromStart()
{
  return m_TotalMeasNbFromStart;
}

double CMeasure::GetRangeMin()
{
  return m_RangeMin;
}

double CMeasure::GetRangeMax()
{
  return m_RangeMax;
}

bool CMeasure::AutoUpdateRange()
{
  bool resu = false;
  for (int i=0; i<MAX_MEASUREMENT_DATA_SIZE; i++)
  {
    if ( m_tCurValTable[i].timeCount != 0 )
    {
      if (m_tCurValTable[i].value > m_RangeMax)
      {
        m_RangeMax = 120 * ( floor(m_tCurValTable[i].value / 100.0) + 1 ); // RangeMax 20% over real max
        resu = true;
      }
      if (m_tCurValTable[i].value < m_RangeMin)
      {
        m_RangeMin = 120 * ( floor(m_tCurValTable[i].value / 100.0) - 1 ); // RangeMin 20% under real min
        resu = true;
      }
    }
  }  
  return resu;
}

string CMeasure::getMeasureTypeStrName(MeasureType_e type)
{
  string typeName;
  typeName = MeasureTypeNameList[type];
  return typeName;
}

int CMeasure::GetPeriod()
{
  return m_Period;
}

bool CMeasure::IsRecording()
{
  return m_bIsRecording;
}

EventType_e CMeasure::GetEventType()
{
  return m_EventType;
}

double CMeasure::GetEventThreshold()
{
  return m_EventThreshold;
}

string CMeasure::getEventTypeStrName(EventType_e type)
{
  string typeName;
  typeName = EventTypeNameList[type];
  return typeName;
}

size_t CMeasure::GetParameterIndex()
{
  return m_paramIndex;
}

CBasicEntity* CMeasure::GetEntity()
{
  return m_pEntity;
}