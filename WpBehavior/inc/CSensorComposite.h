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
// FILE: CSensorComposite.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents sensor used to combine an existing sensor 
//         with a parameter value. 
// 
//
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#if !defined(SENSOR_COMPOSITE_H)
#define SENSOR_COMPOSITE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CSensor.h"
#include "CAnimal.h"

//===========================================================================
// Definitions            
//===========================================================================
class CBrainAnimal;


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensorComposite : public CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
  DWORD    m_BaseSensUId;
  int      m_ParamIndex;
  CSensor* m_pBaseSens;
  CGenericParam* m_pParam;
//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
  CBrainAnimal* m_pBrain;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CSensorComposite(CBrainAnimal* pBrain, double* pWeightRate, int subCaptorNumber, DWORD baseSensUId, int paramIndex);
	virtual ~CSensorComposite();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  int UpdateAndGetStimulationTable(sensorValType*& pStimulationVal);
  string GetSubCaptorLabel(int index);

//---------------------------------------------------------------------------
// other functions
//---------------------------------------------------------------------------
public:
  CSensor* GetBaseSens();

};

#endif // !defined(SENSOR_COMPOSITE_H)
