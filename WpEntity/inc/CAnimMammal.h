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
// FILE: CAnimMammal.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a mammal entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/09/2002 : Creation
//
//===========================================================================

#if !defined(CANIMMAMMAL_INCLUDED_)
#define CANIMMAMMAL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//===========================================================================
// Includes 
//===========================================================================
#include "CAnimal.h" 

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CAnimMammal : public CAnimal 
{  
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// Parameters Ids
//---------------------------------------------------------------------------
private:
  int m_id_GestationTime;
  int m_id_GestationNumberRange;


//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:       
  CAnimMammal(string label, Point_t initCoord, int layer, CGenome* pGenome);
  CAnimMammal(string label, CAnimMammal& model);
  CAnimMammal(string label, CAnimMammal& mother,CAnimMammal& father);

  virtual ~CAnimMammal();

//---------------------------------------------------------------------------
// Genetic settings
//---------------------------------------------------------------------------
protected: 
  bool setParamFromGene (CGene* pGen);
  bool setLifeStageFromGene (CGene* pGen);
  bool completeParamsWithDefault();
  bool completeLifeStagesWithDefault();

//---------------------------------------------------------------------------
// Genetic description
//---------------------------------------------------------------------------
protected:
  string buildParameterString(CGene* pGen);
  string buildLifeStageString(CGene* pGen);

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public: 
  void nextHour(); 
  void nextDay(bool forceGrowth = false);
protected:
  void increaseGestationTime(int variation);

//---------------------------------------------------------------------------
// Life stages management
//---------------------------------------------------------------------------
protected:
  void enterInNewLifeStage(CLifeStage* pLifeStage);

//---------------------------------------------------------------------------
// Behavior  
//---------------------------------------------------------------------------
public: 
  void autoKill();
  bool ExecuteEatAction(int relLayer, double successSatisfactionFactor, double failureFrustrationFactor);
  bool ExecuteCopulateAction(double successSatisfactionFactor, double failureFrustrationFactor);
  bool deliverAllBabies();
  
protected:
  bool reproductWith(CAnimMammal* partner);


//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public: 
  int getGestationMaxNumber();
  int getGestationMinNumber();
  int getGestationNominalNumber();
  int getGestationBabyNumber();
  void setGestationBabyNumber(int nbBaby);
}; // end CAnimMammal

#endif // !defined(CANIMMAMMAL_INCLUDED_)



