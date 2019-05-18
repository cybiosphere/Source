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
// FILE: CFeelingWelfare.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents the feeling welfare of an animal
//         This is used to compute a parameter involved in satisfaction evaluation
//         when the animal perform an action (especially moves or does nothing)
//         It can take into account any sensor output
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  01/03/2006 : Creation
//
//===========================================================================

#if !defined( CFEELINGWELFARE_INCLUDED_)
#define CFEELINGWELFARE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxtempl.h>

#include "CBasicEntity.h"

//===========================================================================
// Definitions            
//===========================================================================

typedef struct 
{
  CSensor* m_pSens;
  double*  m_pSensitivityTableMask;   // array size should be pSens->m_SubCaptorNumber
} Sensitivity_t;


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CFeelingWelfare
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:

  CBasicEntity* m_pEntity;

  // FearSensitivity_t table
  std::vector<Sensitivity_t*> m_tSensitivity;

  double m_CurSatisfaction;

//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------  
public:
  CFeelingWelfare(CBasicEntity* pEntity);
	virtual ~CFeelingWelfare();

//---------------------------------------------------------------------------
// Computation
//---------------------------------------------------------------------------
public:
  double ComputeAndGetFeelingWelfare();
  double GetSensorFeelingImpact(int index, std::string &infoStr);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  bool AddSensitivity(CSensor* pSens, int tableMaskSize, double* pTableMask);

  double GetCurFeelingWelfare();


};// end CFeelingWelfare

#endif // !defined(CFEELINGWELFARE_INCLUDED_)


