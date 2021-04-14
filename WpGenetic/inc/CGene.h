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
// FILE: CGene.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a basic gene
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined( CGENE_INCLUDED_)
#define CGENE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>
//#include <afxtempl.h>
#include "Definitions.h"
#include "CGeneDefinitions.h"
#include <vector>

//===========================================================================
// Definitions            
//===========================================================================

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGene 
{

//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data:
//---------------------------------------------------------------------------
private:
  GeneType_e m_GeneType;
  int m_GeneSubType;
  BYTE m_MuteRate;
  GeneMuteType_e m_MuteType;
  std::vector<BYTE> m_RawData;
  const CGeneDefinitions* m_pDefinitions;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CGene();
  CGene(CGene& model);
  virtual ~CGene();

//---------------------------------------------------------------------------
// Configuration
//---------------------------------------------------------------------------
public:
  bool setAsCaracter (GeneSubTypeCaracter_e subType, int muteRate, GeneMuteType_e muteType,int dataLen, BYTE* pRawData);
  bool setAsCaracterUsingDefinition(GeneSubTypeCaracter_e subType, int muteRate, DWORD value);
  bool setAsParameter(GeneSubTypeParam_e subType, int muteRate, long min, long nominalVal, long max);
  bool setAsParameterUsingDefinition(GeneSubTypeParam_e subType, int muteRate, double min, double nominalVal, double max);
  bool setAsLifeStage(GeneSubTypeLifeStage_e subType, int muteRate, long duration);
  bool setAsPhysicWelfare(GeneSubTypePhySensi_e subType, int muteRate, long sensitivity, long min, long nominalVal, long max);
  bool setAsSensor   (GeneSubTypeSensor_e subType, int muteRate, int nbWeight, short* pWeight,
    long data1 = 0, long data2 = 0, long data3 = 0, long data4 = 0);
  bool setAsSensorUsingDefinition(GeneSubTypeSensor_e subType, int muteRate, int nbWeight, short* pWeight,
    double data1 = 0, double data2 = 0, double data3 = 0, double data4 = 0);
  bool setAsSensorComposite(int muteRate, int nbWeight, short* pWeight, DWORD sensorUId, int paramId);
  bool setAsReaction (GeneSubTypeReaction_e subType, int muteRate, long success, long failure, long data1 = 0, long data2 = 0);
  bool setAsReactionUsingDefinition(GeneSubTypeReaction_e subType, int muteRate, double success, double failure, double data1 = 0, double data2 = 0);
  bool setAsBrainSize (GeneSubTypeBrainSize_e subType, int muteRate, size_t brainSize);
  bool setAsBrainInstinctLine (int muteRate, size_t lineId, size_t nbColumn, WORD* pData);
  bool setAsBrainIdentificationLine (int muteRate, size_t lineId, size_t nbColumn, WORD* pData);
  bool setAsFeeling(GeneSubTypeFeeling_e subType, int muteRate, DWORD sensorUId, int nbSensi, short* pSensi);

  bool setAsPurposeTrigger(GeneSubTypePurpose_e subType, int muteRate, DWORD sensorUId, int duration, int subCaptorIndex, int startThreshold, int stopThreshold, int labelLen, char* pLabel);
  bool setAsPurposeSensorBonus(int muteRate, DWORD purposeUId, DWORD sensorUId, int nbBonus, short* pBonusTable); 
  bool setAsPurposeReactionBonus(int muteRate, DWORD purposeUId, DWORD reactionUId, int bonus); 
  bool setAsBrainConfig(GeneSubTypeBrainBehavior_e subType,int muteRate, GeneMuteType_e muteType, int dataLen, BYTE* pData);
  bool setAsBrainConfigUsingDefinition(GeneSubTypeBrainBehavior_e subType, int muteRate, DWORD value);

//---------------------------------------------------------------------------
// Raw data conversion
//---------------------------------------------------------------------------
  string buildStringDataFromGene();
  bool buildGeneFromStringData(string rawData);

//---------------------------------------------------------------------------
// Genetic
//---------------------------------------------------------------------------
  int getDominanceFactor();
  bool tryMutation();

//---------------------------------------------------------------------------
// Display 
//---------------------------------------------------------------------------
  string getLabel();

 //---------------------------------------------------------------------------
 // Internal processing 
 //---------------------------------------------------------------------------
private:
  inline WORD encodeLongOnWord(long longValue, long rangeMax) { return (WORD)((longValue * 0xFFFF) / rangeMax);}
  inline WORD encodeLongOnWordSigned(long longValue, long rangeMax) { return (WORD)(((longValue + rangeMax) * 0xFFFF) / (2 * rangeMax));}

//---------------------------------------------------------------------------
// Use Gene definition 
//---------------------------------------------------------------------------
public:
  const CGeneDefinitions* getDefinitions();
  size_t getNumElements();
  double getElementValue(size_t index);
  bool setElementValue(size_t index, double value);
  double getElementScale(size_t index);
  int getElementRoundValue(size_t index);
  int getElementFloorValue(size_t index);
  DWORD getElementRawValue(size_t index);
  bool setElementRawValue(size_t index, DWORD value);
  string getElementStrName(size_t index);
  bool getElementIsConfigurable(size_t index);
  double getElementDefaultValue(size_t index);
  bool setRawDataSizeWithDefinition();

  double computeWeightFromData(WORD rawData);
  double computeSensitivityFromData(WORD rawData);
  int computeBonusFromData(WORD rawData);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  std::vector<BYTE>& getData();
  size_t getDataLen();
  GeneType_e getGeneType();
  int getGeneSubType();
  GeneMuteType_e getMuteType();
  BYTE getMuteRate();

}; // end CGene

#endif // !defined(CGENE_INCLUDED_)



