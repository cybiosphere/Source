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
// FILE: CSensorTaste.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used for smell sens
//         It is a set of 7 sub-sensors dedicated for the 7 odors:
//         AROMATIC(0),...NAUSEOUS(6)
//         A 7 sized weight table must be used to set savour sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#include "CSensorTaste.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorTaste::CSensorTaste
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[NUMBER_TASTES] : table of weight for each taste
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorTaste::CSensorTaste(CBrainAnimal* pBrain, double pWeightRate[NUMBER_TASTES])
:CSensor(NUMBER_TASTES, pWeightRate, UID_BASE_SENS_TASTE)
{
  m_pBrain = pBrain;
  m_Label = "Taste";
}

CSensorTaste::~CSensorTaste()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorTaste::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to tasted entities 
//               The Weight of captors is taken into account here
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
int CSensorTaste::UpdateAndGetStimulationTable(sensorValType*& pStimulationVal)
{
  int* pTasteLeveltable = m_pBrain->getAnimal()->getpTasteLevelTable();
  int i;
  for (i=0; i<m_SubCaptorNumber; i++)
  {
    m_pStimulationValues[i] = pTasteLeveltable[i+TASTE_NONE];
  }

  for (i=0; i<m_SubCaptorNumber; i++)
  {
    // Use weight
    m_pStimulationValues[i] = m_pStimulationValues[i] * m_pSubCaptorWeightRate[i] / 100.0;
    // Don't go over Max!
    if (m_pStimulationValues[i] > MAX_SENSOR_VAL)
      m_pStimulationValues[i] = MAX_SENSOR_VAL;
  }

  pStimulationVal = m_pStimulationValues;
  return m_SubCaptorNumber;
}

//---------------------------------------------------------------------------
// METHOD:       CSensorTaste::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorTaste::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) )
    return ("bad index");
  else
  {
    TasteType_e tasteId = TASTE_NONE;
    tasteId = (TasteType_e)(tasteId+1);
    for (int i=0; (i<index)&&(i<TASTE_NUMBER_TYPE); i++)
      tasteId = (TasteType_e)(tasteId+1);
    return (CBasicEntity::getTasteStrName(tasteId) );
  }
}
