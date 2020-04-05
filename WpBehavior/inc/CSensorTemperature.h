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
// FILE: CSensorTemperature.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for hunger
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined(SENSORTEMPERATURE_INCLUDED_)
#define SENSORTEMPERATURE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CSensor.h"

//===========================================================================
// Definitions            
//===========================================================================
class CBrainAnimal;

typedef enum
{
  TEMPERATURE_TOO_HOT  =     0,
  TEMPERATURE_TOO_COLD,
  TEMPERATURE_PLEASANT
} SensorTemperatureType_e;


#define NUMBER_TEMPERATURE_SUB_CAPTORS 3

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensorTemperature : public CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

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
	CSensorTemperature(CBrainAnimal* pBrain, std::vector<double>& tWeightRate);
	virtual ~CSensorTemperature();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  const std::vector<sensorValType>& UpdateAndGetStimulationTable();
  string GetSubCaptorLabel(size_t index);

};

#endif // !defined(SENSORTEMPERATURE_INCLUDED_)
