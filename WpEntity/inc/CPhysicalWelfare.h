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
// FILE: CPhysicalWelfare.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents the physical welfare of a living entity.
//         This is used to compute a parameter involved in health management
//         It can take into account biotop (temperature, water...) and special
//         malus such as disease, injury, ... or bonus such as sleep, rest...
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  01/03/2006 : Creation
//
//===========================================================================

#if !defined( CPHYSICALWELFARE_INCLUDED_)
#define CPHYSICALWELFARE_INCLUDED_

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


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CPhysicalWelfare
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:

  CBasicEntity* m_pEntity;

  // Sensitivity: 0 = neutral (no injury) 100 = maximal (quick death)
  // Temperature sensitivity
  double        m_TemperMin;
  double        m_TemperMax;
  double        m_TemperSensitivity; // Health down in case of temperature out of range
  bool          m_TemperSensSet;     // Check if init done

  // Environement sensitivity
  double        m_HabitatSensitivity; // Health down in case of bad layer type
  bool          m_HabitatSensSet;      // Check if init done

  // Fertility sensitivity
  double        m_FertilMin;
  double        m_FertilMax;
  double        m_FertilSensitivity; // Health down in case of fertility out of range
  bool          m_FertilSensSet;     // Check if init done

  // Sunlight sensitivity
  double        m_SunlightMin;
  double        m_SunlightMax;
  double        m_SunlightSensitivity; // Health down in case of sunlight out of range
  bool          m_SunlightSensSet;       // Check if init done

  double        m_DiseaseMalus;
  double        m_InjuryMalus;
  double        m_RecoveryBonus;


//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------  
public:
  CPhysicalWelfare(CBasicEntity* pEntity);

//---------------------------------------------------------------------------
// Computation
//---------------------------------------------------------------------------
public:
  double ComputeAndGetHealthVariation();


//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:

  void SetTemperatureSensitivity (double temperMin, double temperMax, double sensitivity);
  void SetHabitatSensitivity     (double sensitivity);
  void SetFertilitySensitivity   (double fertilMin, double fertilMax, double sensitivity);
  void SetSunlightSensitivity    (double sunMin, double sunMax, double sensitivity);

  void   SetDiseaseMalus  (double malus);
  double GetDiseaseMalus  (void);
  void   SetInjuryMalus   (double malus);
  double GetInjuryMalus   (void);
  void   SetRecoveryBonus (double bonus);

  bool IsTemperSensSet();
  bool IsHabitatSensSet();
  bool IsFertilSensSet();
  bool IsSunlightSensSet();

  double GetTemperatureMin();
  double GetTemperatureMax();

};// end CPhysicalWelfare

#endif // !defined(CPHYSICALWELFARE_INCLUDED_)


