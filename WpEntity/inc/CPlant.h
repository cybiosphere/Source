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
// FILE: CPlant.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a plant entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined(CPLANT_INCLUDED_)
#define CPLANT_INCLUDED_

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
class DLL_CYBIOCORE_API CPlant : public CBasicEntity 
{  
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// Parameters Ids
//---------------------------------------------------------------------------
private:
  char m_id_Age;
  char m_id_Decomposition;
  char m_id_ReproductionRate;
  char m_id_Health;
  char m_id_GrowthSpeed;
  char m_id_ReproductionRange;
  char m_id_ResistanceToConsumption;
  COLORREF   m_OriginalColorRgb;           // Color in RGB when plant is healthy (could be in basicEntity)
  double     m_OriginalProtection;         // Protection when plant is healthy
  OdorType_e m_OriginalOdor;               // Odor when plant is healthy

//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:       
  CPlant(string label, size_t layer, CGenome* pGenome);
  CPlant(string label, CPlant& model);
  CPlant(string label, CPlant& mother,CPlant& father);  

private:
  void initPlantParamIds();

//---------------------------------------------------------------------------
// Genetic settings
//---------------------------------------------------------------------------
protected: 
  bool setParamFromGene(CGene* pGen);
  bool completeParamsWithDefault();
  bool setPhysicWelfareFromGene(CGene* pGen);
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
  void nextDay(bool doQuickAgeing = false);
  int  getAge();
  int  getDecompositionTime();
  bool changeHealthRate(double variation);

protected:
  void setHealthRate(double rate);
  void forceAgeValue(int newAge);

//---------------------------------------------------------------------------
// Behavior  
//---------------------------------------------------------------------------
  void autoKill();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  bool isPlant();
  double changeWeight(double variation); // Overload
  double getReproductionRate(); 
  double getLifeExpectation();
  double getRotenTimeExpectation();
  double getReproductionRange();
  void setResistanceToConsumptionToNominalRatio(double ratio);
  void setReproductionRateToNominalRatio(double ratio);

}; // end CPlant

#endif // !defined(CPLANT_INCLUDED_)



