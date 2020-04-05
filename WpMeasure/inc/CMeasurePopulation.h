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
// FILE: CMeasurePopulation.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a population metric
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#if !defined(CMEASURE_POPULATION_INCLUDED_)
#define CMEASURE_POPULATION_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>

#include "Definitions.h"
#include "CMeasure.h"

class CBiotop;

//===========================================================================
// Definitions            
//===========================================================================

typedef enum
{
  MEASURE_POPULATION_TOTAL       = 0,
  MEASURE_POPULATION_ANIMAL      = 1,
  MEASURE_POPULATION_VEGETAL     = 2,
  MEASURE_POPULATION_MINERAL     = 3,
  MEASURE_POPULATION_SPECIFIC    = 4,
  MEASURE_POPULATION_NUMBER_TYPE = 5
} MeasurePopulationType_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CMeasurePopulation : public CMeasure
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  CBiotop* m_pBiotop;
  MeasurePopulationType_e m_SubType;
  string m_SpecieName;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CMeasurePopulation(CBiotop* pBiotop, int period, int id, MeasurePopulationType_e type, int maxVal, string SpecieName = "");
	virtual ~CMeasurePopulation();

//---------------------------------------------------------------------------
// Measurement management
//---------------------------------------------------------------------------
public:
  double GetCurrentValue();

//---------------------------------------------------------------------------
// Raw data conversion
//---------------------------------------------------------------------------
public:
  bool buildMeasureDataFromString(string dataString);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  static string GetMeasurePopulationTypeStrName(MeasurePopulationType_e type);
  size_t GetSubTypeId();
  string GetSpecieName();

};

#endif // !defined(CMEASURE_POPULATION_INCLUDED_)
