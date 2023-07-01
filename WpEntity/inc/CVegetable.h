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
// FILE: CVegetable.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a vegetal entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined( CVEGETABLE_INCLUDED_)
#define CVEGETABLE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//===========================================================================
// Includes 
//===========================================================================
#include "CBasicEntity.h" 

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CVegetable : public CBasicEntity 
{  
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// Parameters Ids
//---------------------------------------------------------------------------
private:
  size_t m_id_Age;
  size_t m_id_Decomposition;
  size_t m_id_ReproductionRate;
  size_t m_id_Health;
  size_t m_id_GrowthSpeed;
  size_t m_id_ReproductionRange;
  size_t m_id_ResistanceToConsumption;
  COLORREF   m_OriginalColorRgb;           // Color in RGB when vegetal is healthy (could be in basicEntity)
  double     m_OriginalProtection;         // Protection when vegetal is healthy
  OdorType_e m_OriginalOdor;               // Odor when vegetal is healthy

//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:       
  CVegetable(string label, Point_t initCoord, size_t layer, CGenome* pGenome);
  CVegetable(string label, CVegetable& model);
  CVegetable(string label, CVegetable& mother,CVegetable& father);  

//---------------------------------------------------------------------------
// Genetic settings
//---------------------------------------------------------------------------
protected: 
  bool setParamFromGene (CGene* pGen);
  bool completeParamsWithDefault();
  bool setPhysicWelfareFromGene (CGene* pGen);
  bool completePhysicWelfareWithDefault();

//---------------------------------------------------------------------------
// Genetic description
//---------------------------------------------------------------------------
protected:
  string buildPhysicWellfareString(CGene* pGen);

//---------------------------------------------------------------------------
// Biotop Connection
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  void nextHour();
  void nextDay(bool forceGrowth = false);
  int  getAge();
  int  getDecompositionTime();

protected:
  bool changeHealthRate(double variation);
  void setHealthRate(double rate);

//---------------------------------------------------------------------------
// Behavior  
//---------------------------------------------------------------------------
  void autoKill();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  double changeWeight(double variation); // Overload
  double getReproductionRate(); 
  double getLifeExpectation();
  double getRotenTimeExpectation();
  double getReproductionRange();
  void setResistanceToConsumptionToNominalRatio(double ratio);
  void setReproductionRateToNominalRatio(double ratio);

}; // end CVegetable

#endif // !defined(CVEGETABLE_INCLUDED_)



