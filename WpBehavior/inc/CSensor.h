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
// FILE: CSensor.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a basic sensor
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined(CSENSOR_INCLUDED_)
#define CSENSOR_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>
#include "Matrix.h"
#include "CReaction.h"
//#include "CBasicEntity.h"

//===========================================================================
// Definitions            
//===========================================================================
typedef double sensorValType;  // from -MAX_SENSOR_VAL to MAX_SENSOR_VAL 
#define MAX_SENSOR_VAL    10.0


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  int                        m_SubCaptorNumber;
  string                     m_Label; // User defined name
  std::vector<double>        m_tSubCaptorWeightRate;
  std::vector<sensorValType> m_tStimulationValues;
  std::vector<int>           m_tBonusRate; // Table of Purpose factor: from 0 to 100 :malus, over 100 bonus 

private:
  DWORD           m_UniqueId;    // Global identifier

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CSensor(double weightRate, DWORD uniqueId);
  CSensor(int subCaptorNumber, double* pSubCaptorWeightRate, DWORD uniqueId);
  CSensor(int subCaptorNumber, double SubCaptorsWeightRate, DWORD uniqueId);
  virtual ~CSensor();

//---------------------------------------------------------------------------
// Virtual
//---------------------------------------------------------------------------
  virtual const std::vector<sensorValType>& UpdateAndGetStimulationTable();
  virtual string GetSubCaptorLabel(int index) = 0;
  virtual bool   IsSexSpecific(int captorIndex);

//---------------------------------------------------------------------------
// Shared method
//---------------------------------------------------------------------------
protected:
  void applySubCaptorWeightRate();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  int           GetSubCaptorNumber();
  string        GetLabel();
  DWORD         GetUniqueId();
  int           GetBonusRate(int subCaptorInd);
  bool          SetBonusRate(int subCaptorInd, int bonus);
  bool          SetBonusRate(int tableBonusSize, const std::vector<int>& pTableBonus);
  void          SetBonusRateToNeutral();
  sensorValType GetSubCaptorStimulationLevel(int captorIndex = 0);
  const std::vector<sensorValType>& GetStimulationTable();

};

#endif // !defined(CSENSOR_INCLUDED_)
