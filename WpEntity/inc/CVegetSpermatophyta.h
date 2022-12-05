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
// FILE: CVegetSpermatophyta.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a a Spermatophyta entity
//         => a vegetable using flowers for reproduction
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined( CVEGETSPERMATOPHYTA_INCLUDED_)
#define CVEGETSPERMATOPHYTA_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//=========================================================================== 
#include "CVegetable.h" 

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CVegetSpermatophyta : public CVegetable 
{  
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// Parameters Ids
//---------------------------------------------------------------------------
private:
  size_t m_id_PollenRange;

//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:       
  CVegetSpermatophyta(string label, Point_t initCoord, size_t layer, CGenome* pGenome);
  CVegetSpermatophyta(string label, CVegetSpermatophyta& model);
  CVegetSpermatophyta(string label, CVegetSpermatophyta& mother,CVegetSpermatophyta& father);

//---------------------------------------------------------------------------
// Genetic settings
//---------------------------------------------------------------------------
protected:
  bool setParamFromGene (CGene* pGen);
  bool completeParamsWithDefault();
  bool setLifeStageFromGene(CGene* pGen);

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  void nextHour(); 

//---------------------------------------------------------------------------
// Life stages management
//---------------------------------------------------------------------------
protected:
  void enterInNewLifeStage(CLifeStage* pLifeStage);

//---------------------------------------------------------------------------
// Behavior  
//---------------------------------------------------------------------------
protected:
  bool reproductWith(CVegetSpermatophyta* partner);
  bool autoClone();
  void tryToReproduceOnceADay();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  double getPollenRange();


}; // end CVegetSpermatophyta

#endif // !defined(CVEGETSPERMATOPHYTA_INCLUDED_)



