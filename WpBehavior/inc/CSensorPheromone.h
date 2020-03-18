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
// FILE: CSensorPheromone.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for smelling
//         It is a set of 7 sub-sensors dedicated for 4 pheromones:
//         AROMATIC(0),...NAUSEOUS(6)
//         A 7 sized weight table must be used to set savour sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  25/09/2005 : Creation
//
//===========================================================================

#if !defined(SENSORPHEROMONE_H)
#define SENSORPHEROMONE_H

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

#define NUMBER_PHEROMONES (PHEROMONE_NUMBER_TYPE-1)

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensorPheromone : public CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
  int m_nRange;

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
	CSensorPheromone(CBrainAnimal* pBrain, double pWeightRate[NUMBER_PHEROMONES], int range);
	virtual ~CSensorPheromone();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  const std::vector<sensorValType>& UpdateAndGetStimulationTable();
  string GetSubCaptorLabel(int index);
  bool IsSexSpecific(int captorIndex);
};

#endif // !defined(SENSORPHEROMONE_H)
