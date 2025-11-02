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
// FILE: CGene.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a basic gene
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================
 
#include "CGene.h"

//===========================================================================
// Definitions            
//===========================================================================


//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::CGene()
//  
// DESCRIPTION:  Default constructor
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      To be used before SetAs...
//---------------------------------------------------------------------------
CGene::CGene():
  m_GeneType{ GENE_GENERIC }, 
  m_GeneSubType{ GENE_GENERIC_UNKNOWN },
  m_MuteRate{ 0 },
  m_MuteType{ GENE_MUTE_RANDOM_BIT },
  m_RawData{},
  m_pDefinitions{ CGeneList::getDefinitions(GENE_GENERIC, GENE_GENERIC_UNKNOWN) }
{
}

//---------------------------------------------------------------------------
// METHOD:       CGene::CGene()
//  
// DESCRIPTION:  Copy constructor
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      To be used for reproduction
//---------------------------------------------------------------------------
CGene::CGene(CGene& model):
  m_GeneType{ model.m_GeneType },
  m_GeneSubType{ model.m_GeneSubType },
  m_MuteRate{ model.m_MuteRate },
  m_MuteType{ model.m_MuteType },
  m_RawData{ model.m_RawData },
  m_pDefinitions{ model.m_pDefinitions }
{
}

CGene::~CGene()
{
}

//===========================================================================
// Configuration
//===========================================================================

bool CGene::setAsNeutral(void)
{
  // Clean previous config
  m_RawData.clear();
  m_GeneType = GENE_GENERIC;
  m_GeneSubType = GENE_GENERIC_UNKNOWN;
  m_MuteRate = 0;
  m_MuteType = GENE_MUTE_RANDOM_BIT;
  m_pDefinitions = CGeneList::getDefinitions(GENE_GENERIC, GENE_GENERIC_UNKNOWN);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsCaracter()
//  
// DESCRIPTION:  Configure gene to describe a caracter
// 
// ARGUMENTS:    pData: Pointer on dataLen BYTE size data. Note that pData is copied
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsCaracter(GeneSubTypeCaracter_e subType, int muteRate, GeneMuteType_e muteType, int dataLen, BYTE* pRawData)
{
  // Clean previous config
  m_RawData.clear();

  m_GeneType    = GENE_CARACTER;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = muteType;
  m_pDefinitions = CGeneList::getDefinitions(GENE_CARACTER, subType);
  m_RawData.resize(dataLen);
  memcpy(m_RawData.data(), pRawData, dataLen);
  return true;
}

bool CGene::setAsCaracterUsingDefinition(GeneSubTypeCaracter_e subType, int muteRate, DWORD value)
{
  // Clean previous config
  m_RawData.clear();

  m_GeneType = GENE_CARACTER;
  m_GeneSubType = subType;
  m_pDefinitions = CGeneList::getDefinitions(GENE_CARACTER, subType);
  m_MuteRate = muteRate;
  m_MuteType = m_pDefinitions->muteType;
  setRawDataSizeWithDefinition();
  setElementValue(0, (double)value + 0.5);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsParameter()
//  
// DESCRIPTION:  Configure gene to describe a parameter
// 
// ARGUMENTS:    min [0..1000] generic scale: Real min compute according to subType 
//               nominalVal [0..1000] generic scale: Real val compute according to subType 
//               max [0..1000] generic scale: Real max compute according to subType 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsParameter(GeneSubTypeParam_e subType, int muteRate, long min, long nominalVal, long max)
{
  m_RawData.clear();

  m_GeneType    = GENE_PARAMETER;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_pDefinitions = CGeneList::getDefinitions(GENE_PARAMETER, subType);
  m_RawData.resize(3 * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for parameter is [0..1000]
  pWordData[0] = encodeLongOnWord(min, 1000);
  pWordData[1] = encodeLongOnWord(nominalVal, 1000);
  pWordData[2] = encodeLongOnWord(max, 1000);

  return true;
}

bool CGene::setAsParameterUsingDefinition(GeneSubTypeParam_e subType, int muteRate, double min, double nominalVal, double max)
{
  m_RawData.clear();

  m_GeneType    = GENE_PARAMETER;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_PARAMETER, subType);
  m_MuteType = m_pDefinitions->muteType;
  setRawDataSizeWithDefinition();
  setElementValue(0, min);
  setElementValue(1, nominalVal);
  setElementValue(2, max);

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsLifeStage()
//  
// DESCRIPTION:  Configure gene to describe a life stage
// 
// ARGUMENTS:    durationRatio [0..1000] generic scale: 10 x "% of life or death" period 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsLifeStage(GeneSubTypeLifeStage_e subType, int muteRate, long durationRatio)
{
  m_RawData.clear();

  m_GeneType    = GENE_LIFESTAGE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_LIFESTAGE, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for LifeStage is [1..1000]
  pWordData[0] = encodeLongOnWord(durationRatio, 1000);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsPhysicWelfare()
//  
// DESCRIPTION:  Configure gene to describe a life stage
// 
// ARGUMENTS:    sensitivity [0..1000] generic scale: 10 x "% of sensitivity" 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPhysicWelfare(GeneSubTypePhySensi_e subType, int muteRate, long sensitivity, long min, long nominalVal, long max)
{
  m_RawData.clear();

  m_GeneType    = GENE_PHY_WELFARE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_PHY_WELFARE, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(4 * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for sensitivity is [1..1000]
  pWordData[0] = encodeLongOnWord(sensitivity, 1000);
  // Generic scale for parameter is [0..1000]
  pWordData[1] = encodeLongOnWord(min, 1000);
  pWordData[2] = encodeLongOnWord(nominalVal, 1000);
  pWordData[3] = encodeLongOnWord(max, 1000);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsSensor()
//  
// DESCRIPTION:  Configure gene to describe a sensor
// 
// ARGUMENTS:    nbWeight size of pWeight according to subType
//               pWeight table [-1000..1000] generic scale: 10 x "% Weight" 
//               dataX [0..1000] generic scale: Real val compute according to subType
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsSensor(GeneSubTypeSensor_e subType, int muteRate, int nbWeight, short* pWeight,
                         long data1, long data2, long data3, long data4)
{
  m_RawData.clear();

  m_GeneType    = GENE_SENSOR;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_SENSOR, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize((4 + nbWeight) * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for all values is [0..1000]
  pWordData[0] = encodeLongOnWord(data1, 1000);
  pWordData[1] = encodeLongOnWord(data2, 1000);
  pWordData[2] = encodeLongOnWord(data3, 1000);
  pWordData[3] = encodeLongOnWord(data4, 1000);
  for (int i = 0; i < nbWeight; i++)
  {
    pWordData[4+i] = encodeLongOnWordSigned(pWeight[i], 1000);
  }
  return true;
}

bool CGene::setAsSensorUsingDefinition(GeneSubTypeSensor_e subType, int muteRate, int nbWeight, short* pWeight,
  double data1, double data2, double data3, double data4)
{
  m_RawData.clear();

  m_GeneType = GENE_SENSOR;
  m_GeneSubType = subType;
  m_MuteRate = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_SENSOR, subType);
  m_MuteType = m_pDefinitions->muteType;
  m_RawData.resize((4 + nbWeight) * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  setElementValue(0, data1);
  setElementValue(1, data2);
  setElementValue(2, data3);
  setElementValue(3, data4);

  for (int i = 0; i < nbWeight; i++)
  {
    pWordData[4 + i] = encodeLongOnWordSigned(pWeight[i], 1000);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsSensorComposite()
//  
// DESCRIPTION:  Configure gene to describe a composite sensor
// 
// ARGUMENTS:    nbWeight size of pWeight according to subType
//               pWeight table [-1000..1000] generic scale: 10 x "% Weight" 
//               raw sensorUId and paramId 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsSensorComposite(int muteRate, int nbWeight, short* pWeight, DWORD sensorUId, int paramId)
{
  m_RawData.clear();

  m_GeneType    = GENE_SENSOR;
  m_GeneSubType = GENE_SENS_COMPOSITE;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_SENSOR, GENE_SENS_COMPOSITE);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize((4 + nbWeight) * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for all values is [0..1000]
  pWordData[0] = sensorUId>>16;
  pWordData[1] = sensorUId&0xFFFF;
  pWordData[2] = encodeLongOnWord(paramId, 1000);
  pWordData[3] = 0;
  for (int i = 0; i < nbWeight; i++)
  {
    pWordData[4+i] = encodeLongOnWordSigned(pWeight[i], 1000);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsReaction()
//  
// DESCRIPTION:  Configure gene to describe a reaction
// 
// ARGUMENTS:    success [0..1000] generic scale: 10 x "% of success" 
//               failure [0..1000] generic scale: 10 x "% of failure" 
//               dataX [0..1000] generic scale: Real val compute according to subType
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsReaction(GeneSubTypeReaction_e subType, int muteRate, long success, long failure,
                            long data1, long data2)
{
  m_RawData.clear();

  m_GeneType    = GENE_REACTION;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_REACTION, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(4 * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for all values is [0..1000]
  pWordData[0] = encodeLongOnWord(success, 1000);
  pWordData[1] = encodeLongOnWord(failure, 1000);
  pWordData[2] = encodeLongOnWord(data1, 1000);
  pWordData[3] = encodeLongOnWord(data2, 1000);
  return true;
}

bool CGene::setAsReactionUsingDefinition(GeneSubTypeReaction_e subType, int muteRate, double success, double failure, double data1, double data2)
{
  m_RawData.clear();

  m_GeneType = GENE_REACTION;
  m_GeneSubType = subType;
  m_MuteRate = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_REACTION, subType);
  m_MuteType = m_pDefinitions->muteType;
  m_RawData.resize(4 * sizeof(WORD));
  setElementValue(0, success);
  setElementValue(1, failure);
  setElementValue(2, data1);
  setElementValue(3, data1);

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainSize()
//  
// DESCRIPTION:  Configure gene to describe a brain configuration size
// 
// ARGUMENTS:    brainSize [0..100]: size to configure brain
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainSize(GeneSubTypeBrainSize_e subType, int muteRate, size_t brainSize)
{
  m_RawData.clear();

  m_GeneType    = GENE_BRAIN_SIZE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_BRAIN_SIZE, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Generic scale for brainSize is [1..100]
  pWordData[0] = encodeLongOnWord((long)brainSize, 100);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainInstinctLine()
//  
// DESCRIPTION:  Configure gene to describe a brain matrix line
// 
// ARGUMENTS:    pData: Pointer on dataLen WORD size data. Note that pData is copied  
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainInstinctLine(int muteRate, size_t lineId, size_t nbColumn, WORD* pData)
{
  m_RawData.clear();

  m_GeneType    = GENE_BRAIN_INIT;
  m_GeneSubType = GENE_BRAIN_LINE;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_BRAIN_INIT, GENE_BRAIN_LINE);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize((nbColumn + 1) * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Set 1st Word as lineId. Generic scale is [0..10000]
  pWordData[0] = encodeLongOnWord((long)lineId, 10000);
  // Set all other words as brain line data
  for (size_t i = 0; i < nbColumn; i++)
  {
    pWordData[i+1] = pData[i];
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainIdentificationLine()
//  
// DESCRIPTION:  Configure gene to describe a brain identification matrix line
// 
// ARGUMENTS:    pData: Pointer on dataLen WORD size data. Note that pData is copied  
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainIdentificationLine(int muteRate, size_t lineId, size_t nbColumn, WORD* pData)
{
  m_RawData.clear();

  m_GeneType    = GENE_BRAIN_INIT;
  m_GeneSubType = GENE_BRAIN_IDENTIFY_LINE;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_BRAIN_INIT, GENE_BRAIN_IDENTIFY_LINE);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize((nbColumn + 1) * sizeof(WORD));
  WORD* pWordData = (WORD*)m_RawData.data();
  // Set 1st Word as lineId. Generic scale is [0..10000]
  pWordData[0] = encodeLongOnWord((long)lineId, 10000);
  // Set all other words as brain line data
  for (size_t i = 0; i < nbColumn; i++)
  {
    pWordData[i + 1] = pData[i];
  }
  return true;
}


//---------------------------------------------------------------------------
// METHOD:       CGene::setAsFeeling()
//  
// DESCRIPTION:  Configure gene to describe a sensor
// 
// ARGUMENTS:    nbSensi size of pSensi according to Sensor UID
//               pSensi table [-1000..1000] generic scale: 10 x "% Sensibility" 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsFeeling(GeneSubTypeFeeling_e subType, int muteRate, DWORD sensorUId, int nbSensi, short* pSensi)
{
  m_RawData.clear();

  m_GeneType    = GENE_FEELING;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_FEELING, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(sizeof(DWORD) + nbSensi * sizeof(WORD));
  WORD*  pWordData  = (WORD*)m_RawData.data();
  DWORD* pDWordData = (DWORD*)m_RawData.data();
  pDWordData[0] = sensorUId;
  // Generic scale for all values is [0..1000]
  for (int i = 0; i < nbSensi; i++)
  {
    pWordData[2 + i] = encodeLongOnWordSigned(pSensi[i], 1000);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsNewPurpose()
//  
// DESCRIPTION:  Configure gene to describe a purpose
// 
// ARGUMENTS:    sensorUId : Sensor UID
//               duration  : [0..1000] in minute  
//               startThreshold : [0..1000]  10 x "% start rate"
//               subCaptorIndex : [0..20]
//               stopThreshold  : [0..1000]  10 x "% stop rate"
//               labelLen  : label to identify purpose
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPurposeTrigger(GeneSubTypePurpose_e subType, int muteRate,
                                DWORD sensorUId, int duration, int subCaptorIndex, 
                                int startThreshold, int stopThreshold, int labelLen, char* pLabel)
{
  m_RawData.clear();

  m_GeneType    = GENE_PURPOSE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_PURPOSE, subType);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(sizeof(DWORD) + 4 * sizeof(WORD) + labelLen * sizeof(BYTE));
  WORD*  pWordData  = (WORD*)m_RawData.data();
  DWORD* pDWordData = (DWORD*)m_RawData.data();
  BYTE*  pByteData  = (BYTE*)m_RawData.data();
  pWordData[0]  = encodeLongOnWord(duration, 1000);
  pWordData[1]  = encodeLongOnWord(subCaptorIndex, 100);
  pWordData[2]  = encodeLongOnWord(startThreshold, 1000);
  pWordData[3]  = encodeLongOnWord(stopThreshold, 1000);
  pDWordData[2] = sensorUId;

  for (int i = 0; i < labelLen; i++)
  {
    pByteData[12 + i] = pLabel[i];
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsPurposeSensorBonus()
//  
// DESCRIPTION:  Configure gene to describe a purpose reaction bonus
// 
// ARGUMENTS:    purposeUId : Purpose UID
//               sensorUId  : Sensor UID
//               subCaptorIndex : [0..20]
//               bonus       : [0..2000] generic scale: 10 x multiplicator factor on sensor
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPurposeSensorBonus(int muteRate, DWORD purposeUId, DWORD sensorUId, int nbBonus, short* pBonusTable)
{
  m_RawData.clear();

  m_GeneType    = GENE_PURPOSE;
  m_GeneSubType = GENE_PURPOSE_SENSOR;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_PURPOSE, GENE_PURPOSE_SENSOR);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(2 * sizeof(DWORD) + nbBonus * sizeof(WORD));
  WORD*  pWordData  = (WORD*)m_RawData.data();
  DWORD* pDWordData = (DWORD*)m_RawData.data();
  pDWordData[0] = purposeUId;
  pDWordData[1] = sensorUId;
  for (int i = 0; i < nbBonus; i++)
  {
    pWordData[4+i] = encodeLongOnWord(pBonusTable[i], 2000);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsPurposeReactionBonus()
//  
// DESCRIPTION:  Configure gene to describe a purpose reaction bonus
// 
// ARGUMENTS:    purposeUId  : Purpose UID
//               reactionUId : Reaction UID
//               bonus       : [0..2000] generic scale: 10 x bonus rate
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPurposeReactionBonus(int muteRate, DWORD purposeUId, DWORD reactionUId, int bonus)
{
  m_RawData.clear();

  m_GeneType    = GENE_PURPOSE;
  m_GeneSubType = GENE_PURPOSE_REACTION;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_PURPOSE, GENE_PURPOSE_REACTION);
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_RawData.resize(2 * sizeof(DWORD) + sizeof(WORD));
  WORD*  pWordData  = (WORD*)m_RawData.data();
  DWORD* pDWordData = (DWORD*)m_RawData.data();
  pDWordData[0] = purposeUId;
  pDWordData[1] = reactionUId;
  pWordData[4]  = encodeLongOnWord(bonus, 2000);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainConfig()
//  
// DESCRIPTION:  Configure gene to describe a caracter
// 
// ARGUMENTS:    pData: Pointer on dataLen BYTE size data. Note that pData is copied
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainConfig(GeneSubTypeBrainBehavior_e subType, int muteRate, GeneMuteType_e muteType, int dataLen, BYTE* pData)
{
  // Clean previous config
  m_RawData.clear();

  m_GeneType    = GENE_BRAIN_CONFIG;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_pDefinitions = CGeneList::getDefinitions(GENE_BRAIN_CONFIG, subType);
  m_MuteType    = muteType;
  m_RawData.resize(dataLen);
  memcpy(m_RawData.data(), pData, dataLen);
  return true;
}

bool CGene::setAsBrainConfigUsingDefinition(GeneSubTypeBrainBehavior_e subType, int muteRate, DWORD value)
{
  // Clean previous config
  m_RawData.clear();

  m_GeneType = GENE_BRAIN_CONFIG;
  m_GeneSubType = subType;
  m_pDefinitions = CGeneList::getDefinitions(GENE_BRAIN_CONFIG, subType);
  m_MuteRate = muteRate;
  m_MuteType = m_pDefinitions->muteType;
  setRawDataSizeWithDefinition();
  setElementValue(0, (double)value + 0.5);
  return true;
}


//===========================================================================
// Use Gene definition  
//===========================================================================
const CGeneDefinitions* CGene::getDefinitions()
{
  return m_pDefinitions;
}

size_t CGene::getNumElements()
{
  return m_pDefinitions->elements.size();
}

double CGene::getElementValue(size_t index)
{
  double value = 0;
  if (index < getNumElements())
  {
    const GeneParamDefinition_t& def = m_pDefinitions->elements[index];
    BYTE* pDataParam = m_RawData.data() + def.hexaOffset;

    if (def.hexaSize == 1)
    {
      value = (double)pDataParam[0] * (def.valMax - def.valMin) / 255.0 + def.valMin;
    }
    else if (def.hexaSize == 2)
    {
      WORD* pDataWord = (WORD*)pDataParam;
      value = (double)pDataWord[0] * (def.valMax - def.valMin) / 65535.0 + def.valMin;
    }
  }
  return value;
}

bool CGene::setElementValue(size_t index, double value)
{
  bool resu = false;
  if (index < getNumElements())
  {
    const GeneParamDefinition_t& def = m_pDefinitions->elements[index];
    BYTE* pDataParam = m_RawData.data() + def.hexaOffset;

    if (def.hexaSize == 1)
    {
      pDataParam[0] = (BYTE)((value - def.valMin) * 255.0 / (def.valMax - def.valMin));
      resu = true;
    }
    else if (def.hexaSize == 2)
    {
      WORD* pDataWord = (WORD*)pDataParam;
      pDataWord[0] = (WORD)((value - def.valMin) * 65535.0 / (def.valMax - def.valMin));
      resu = true;
    }
    else
    {
      resu = setElementRawValue(index, (DWORD)value);
    }
  }
  return resu;
}

double CGene::getElementScale(size_t index)
{
  double value = 0;
  if (index < getNumElements())
  {
    const GeneParamDefinition_t& def = m_pDefinitions->elements[index];
    if (def.hexaSize == 1)
    {
      value = (def.valMax - def.valMin) / 255;
    }
    else if (def.hexaSize == 2)
    {
      value = (def.valMax - def.valMin) / 65535;
    }
  }
  return value;
}

int CGene::getElementRoundValue(size_t index)
{
  return cybio_round(getElementValue(index));
}

int CGene::getElementFloorValue(size_t index)
{
  return (int)(getElementValue(index));
}

DWORD CGene::getElementRawValue(size_t index)
{
  DWORD value = 0;
  if (index < getNumElements())
  {
    const GeneParamDefinition_t& def = m_pDefinitions->elements[index];
    BYTE* pDataParam = m_RawData.data() + def.hexaOffset;

    if (def.hexaSize == 1)
    {
      value = pDataParam[0];
    }
    else if (def.hexaSize == 2)
    {
      WORD* pDataWord = (WORD*)pDataParam;
      value = pDataWord[0];
    }
    else if (def.hexaSize == 4)
    {
      DWORD* pDataDWord = (DWORD*)pDataParam;
      value = pDataDWord[0];
    }
  }
  return value;
}

bool CGene::setElementRawValue(size_t index, DWORD value)
{
  bool resu = false;
  if (index < getNumElements())
  {
    const GeneParamDefinition_t& def = m_pDefinitions->elements[index];
    BYTE* pDataParam = m_RawData.data() + def.hexaOffset;

    if (def.hexaSize == 1)
    {
      pDataParam[0] = (BYTE)value;
      resu = true;
    }
    else if (def.hexaSize == 2)
    {
      WORD* pDataWord = (WORD*)pDataParam;
      pDataWord[0] = (WORD)value;
      resu = true;
    }
    else
    {
      DWORD* pDataDWord = (DWORD*)pDataParam;
      pDataDWord[0] = value;
      resu = true;
    }
  }
  return resu;
}

string CGene::getElementStrName(size_t index)
{
  string name = "Unset";
  if (index < getNumElements())
  {
    name = m_pDefinitions->elements[index].name;
  }
  return name;
}

bool CGene::getElementIsConfigurable(size_t index)
{
  bool resu = false;
  if (index < getNumElements())
  {
    resu = m_pDefinitions->elements[index].isConfigurable;
  }
  return resu;
}

double CGene::getElementDefaultValue(size_t index)
{
  return m_pDefinitions->getElementDefaultValue(index);
}

bool CGene::setRawDataSizeWithDefinition()
{
  size_t dataSize = 0;
  for (auto& param : m_pDefinitions->elements)
  {
    dataSize += param.hexaSize;
  }
  m_RawData.resize(dataSize);
  return true;
}

double CGene::computeWeightFromData(WORD rawData)
{
  return cybio_round((double)rawData * 200.0 / 65536.0 - 100.0);
}

double CGene::computeSensitivityFromData(WORD rawData)
{
  return cybio_round((double)rawData * 200.0 / 65536.0 - 100.0);
}

int CGene::computeBonusFromData(WORD rawData)
{
  return cybio_round((double)rawData * 20000.0 / 65536.0);
}


//===========================================================================
// Raw data conversion
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::buildStringDataFromGene()
//  
// DESCRIPTION:  Convert Gene to string: Type/Subtype/Len[MuteType/MuteRate/Data]
// 
// ARGUMENTS:    None  
//   
// RETURN VALUE: CString: result
//  
// REMARKS:      None
//---------------------------------------------------------------------------
string CGene::buildStringDataFromGene()
{
  string rawData = "";
  string tempStr = "";

  tempStr = FormatString("%02X%04X%04X%02X%02X",
                  m_GeneType,m_GeneSubType,
                  m_RawData.size()+2, // include mute type and mute rate
                  m_MuteType,m_MuteRate);
  rawData += tempStr;
  for (size_t j = 0; j < m_RawData.size(); j++)
  {
    tempStr = FormatString("%02X",m_RawData[j]);
    rawData += tempStr;
  }

  return rawData;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::buildGeneFromStringData()
//  
// DESCRIPTION:  Convert string to Gene
// 
// ARGUMENTS:    CString rawData: string Type/Subtype/Len[MuteType/MuteRate/Data]  
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CGene::buildGeneFromStringData(string rawData)
{
  if (rawData.length() < 14)
  {
    return false;
  }
  m_GeneType = static_cast<GeneType_e>(std::stoi(rawData.substr(0, 2), nullptr, 16));
  m_GeneSubType = std::stoi(rawData.substr(2, 4), nullptr, 16);
  int dataLen = std::stoi(rawData.substr(6, 4), nullptr, 16);
  m_MuteType = static_cast<GeneMuteType_e>(std::stoi(rawData.substr(10, 2), nullptr, 16));
  m_MuteRate = std::stoi(rawData.substr(12, 2), nullptr, 16);
  m_pDefinitions = CGeneList::getDefinitions(m_GeneType, m_GeneSubType);
  dataLen -= 2; // mute type and rate are included in data
  m_RawData.resize(dataLen);
  for (int j = 0; j < dataLen; ++j)
  {
    m_RawData[j] = static_cast<BYTE>(std::stoi(rawData.substr(14 + 2 * j, 2), nullptr, 16));
  }
  return true;
}

//===========================================================================
// Genetic
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::getRecessiveFactor()
//  
// DESCRIPTION:  Return a Recessive factor to determine the dominant allele
// 
// ARGUMENTS:    None 
//   
// RETURN VALUE: int: Dominance factor = Sum of all BYTES
//  
// REMARKS:      None
//---------------------------------------------------------------------------
int CGene::getRecessiveFactor(void)
{
  int resu=0;
  
  // Each BYTE has same weight
  for (size_t i = 0; i < m_RawData.size(); i++)
  {
    resu = resu + m_RawData[i];
  }

  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::tryMutation()
//  
// DESCRIPTION:  Perform a mutation on gene randomely, according to MuteRate
// 
// ARGUMENTS:    None 
//   
// RETURN VALUE: bool: true if mutation occured
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CGene::tryMutation()
{
  bool resu = false;
  if (testChance(m_MuteRate))
  {
    switch (m_MuteType)
    {
    case GENE_MUTE_RANDOM_BIT:
      {
        // Do mutation randomly. 
        // Effect efficiency depends of the weight of the bit muted.
        size_t dataByteId = getRandInt(m_RawData.size()-1);
        int dataBitId = getRandInt(7);
        m_RawData[dataByteId] ^= (1<<dataBitId);
        CYBIOCORE_LOG_TIME_NOT_AVAILABLE;
        CYBIOCORE_LOG("GENETIC- Mutation type Random bit on gene: %s\n", getLabel().c_str());
        resu = true;
        break;
      }
    case GENE_MUTE_INCREMENTAL_1:
      { 
        // Select 1 byte and increment or decrement it by 1.
        size_t dataByteId = getRandInt(m_RawData.size()-1);
        int addVal = getRandInt(2) - 1;
        if ( ((addVal<0)&&(m_RawData[dataByteId]>0)) || ((addVal>0)&&(m_RawData[dataByteId]<0xFF)) )
        {
          m_RawData[dataByteId] += addVal;
          CYBIOCORE_LOG_TIME_NOT_AVAILABLE;
          CYBIOCORE_LOG("GENETIC- Mutation type Increment byte +1 or -1 on gene: %s\n", getLabel().c_str());
          resu = true;
        }
        else
        {
          resu = false;
        }
        break;
      }
    case GENE_MUTE_INCREMENTAL_2:
      {
        // Select 1 word and increment or decrement it by 1.
        size_t dataByteId = getRandInt(m_RawData.size()-1)/2;
        int addVal = (getRandInt(2) - 1) * 66; // 66 in order to increment of 1 per 1000
        WORD* pWordData = (WORD*)m_RawData.data();
        if ( ( (addVal<0)&&(pWordData[dataByteId]>-addVal) ) || ( (addVal>0)&&(pWordData[dataByteId]<(0xFFFF-addVal)) ) )
        {
          pWordData[dataByteId] += addVal;
          CYBIOCORE_LOG_TIME_NOT_AVAILABLE;
          CYBIOCORE_LOG("GENETIC- Mutation type Increment 2bytes +1 or -1 on gene: %s\n", getLabel().c_str());
          resu = true;
        }
        else
        {
          resu = false;
        }
        break;
      }
    default:
      {
        resu = false;
        break;
      }
    }
  }
  else
  {
    resu = false;
  }
  return resu;
}

//===========================================================================
// Display 
//===========================================================================

string CGene::getLabel()
{
  return (m_pDefinitions->label);
}


//===========================================================================
// Get / Set for attributes
//===========================================================================

std::vector<BYTE>& CGene::getData()
{
  return m_RawData;
}

size_t CGene::getDataLen()
{
  return m_RawData.size();
}

GeneType_e CGene::getGeneType()
{
  return m_GeneType;
}

int CGene::getGeneSubType()
{
  return m_GeneSubType;
}

GeneMuteType_e CGene::getMuteType()
{
  return m_MuteType;
}

BYTE CGene::getMuteRate()
{
  return m_MuteRate;
}

