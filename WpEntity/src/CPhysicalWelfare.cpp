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
// FILE: CPhysicalWelfare.cpp
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
 
#include "CPhysicalWelfare.h"
#include "CBiotop.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPhysicalWelfare::CPhysicalWelfare
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    
//               
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CPhysicalWelfare::CPhysicalWelfare(CBasicEntity* pEntity)
{
  m_pEntity = pEntity;

  // Default init
  m_TemperMin = 0;
  m_TemperMax = 30;
  m_TemperSensitivity = 0; 
  m_TemperSensSet = false;
  m_HabitatSensitivity = 0;
  m_HabitatSensSet = false;
  m_FertilMin = 0;
  m_FertilMax = 0;
  m_FertilSensitivity = 0; 
  m_FertilSensSet = false;
  m_SunlightMin = 0;
  m_SunlightMax = 0;
  m_SunlightSensitivity = 0;
  m_SunlightSensSet = false;
  m_DiseaseMalus = 0;
  m_InjuryMalus = 0;
  m_RecoveryBonus = 0;
}

//---------------------------------------------------------------------------
// Computation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// METHOD:       CPhysicalWelfare::ComputeAndGetHealthVariation
//  
// DESCRIPTION:  Get health variation due to environment effect on entity
// 
// ARGUMENTS:    
//               
//   
// RETURN VALUE: double : -100.0 = immediate death .. +100.0 = max recovery
//  
// REMARKS:      Can be called every sec for animal, every hour for vegetable
//---------------------------------------------------------------------------
double CPhysicalWelfare::ComputeAndGetHealthVariation()
{
  double variation = 0;

  variation += ComputeTemperatureHealthVariation();
  variation += ComputeHabitatHealthVariation();
  variation += ComputeFertilityHealthVariation();
  variation += ComputeSunlightHealthVariation();
  variation -= m_DiseaseMalus;
  variation -= m_InjuryMalus;
  variation += m_RecoveryBonus;

  // Vital needs not reached => quick death
  if (!m_pEntity->checkVitalNeedsOk())
    variation -= 10.0;

  return (variation);
}

double CPhysicalWelfare::ComputeTemperatureHealthVariation()
{
  double variation = 0;
  if (m_TemperSensitivity != 0)
  {
    double temper = m_pEntity->getBiotop()->getTemperature(m_pEntity->getGridCoord(), m_pEntity->getLayer());

    if (temper < m_TemperMin)
      variation = - (m_TemperMin - temper) * m_TemperSensitivity / 100.0;
    else if (temper > m_TemperMax)
      variation = - (temper - m_TemperMax) * m_TemperSensitivity / 100.0;
  }
  return variation;
}

double CPhysicalWelfare::ComputeHabitatHealthVariation()
{
  double variation = 0;
  if (m_HabitatSensitivity != 0)
  {
    bool compliant = m_pEntity->checkHabitat();

    if (compliant == false)
      variation = - m_HabitatSensitivity;
  }
  return variation;
}

double CPhysicalWelfare::ComputeFertilityHealthVariation()
{
  double variation = 0;
  if (m_FertilSensitivity != 0)
  {
    double fertil = m_pEntity->getBiotop()->getFertility(m_pEntity->getGridCoord());

    if (fertil < m_FertilMin)
      variation = - (m_FertilMin - fertil) * m_FertilSensitivity / 100.0;
    else if (fertil > m_FertilMax)
      variation = - (fertil - m_FertilMax) * m_FertilSensitivity / 100.0;
  }
  return variation;
}

double CPhysicalWelfare::ComputeSunlightHealthVariation()
{
  double variation = 0;
  if (m_SunlightSensitivity != 0)
  {
    double sunlight = m_pEntity->getBiotop()->getSunlight();

    if (sunlight < m_SunlightMin)
      variation = - (m_SunlightMin - sunlight) * m_SunlightSensitivity / 100.0;
    else if (sunlight > m_SunlightMax)
      variation = - (sunlight - m_SunlightMax) * m_SunlightSensitivity / 100.0;
  }
  return variation;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

void CPhysicalWelfare::SetTemperatureSensitivity (double temperMin, double temperMax, double sensitivity)
{
  m_TemperMin = temperMin;
  m_TemperMax = temperMax;
  m_TemperSensitivity = sensitivity;
  m_TemperSensSet = true;
}

void CPhysicalWelfare::SetHabitatSensitivity     (double sensitivity)
{
  m_HabitatSensitivity = sensitivity;
  m_HabitatSensSet = true;
}

void CPhysicalWelfare::SetFertilitySensitivity   (double fertilMin, double fertilMax, double sensitivity)
{
  m_FertilMin = fertilMin;
  m_FertilMax = fertilMax;
  m_FertilSensitivity = sensitivity;
  m_FertilSensSet = true;
}

void CPhysicalWelfare::SetSunlightSensitivity    (double sunMin, double sunMax, double sensitivity)
{
  m_SunlightMin = sunMin;
  m_SunlightMax = sunMax;
  m_SunlightSensitivity = sensitivity;
  m_SunlightSensSet = true;
}

void CPhysicalWelfare::SetDiseaseMalus (double malus)
{
  m_DiseaseMalus = malus;
}

double CPhysicalWelfare::GetDiseaseMalus  (void)
{
  return m_DiseaseMalus;
}

void CPhysicalWelfare::SetInjuryMalus  (double malus)
{
  m_InjuryMalus = malus;
}

double CPhysicalWelfare::GetInjuryMalus  (void)
{
  return m_InjuryMalus;
}

void CPhysicalWelfare::SetRecoveryBonus  (double bonus)
{
  m_RecoveryBonus = bonus;
}

double CPhysicalWelfare::GetRecoveryBonus(void)
{
  return m_RecoveryBonus;
}

bool CPhysicalWelfare::IsTemperSensSet()
{
  return(m_TemperSensSet);
}

bool CPhysicalWelfare::IsHabitatSensSet()
{
  return(m_HabitatSensSet);
}

bool CPhysicalWelfare::IsFertilSensSet()
{
  return(m_FertilSensSet);
}

bool CPhysicalWelfare::IsSunlightSensSet()
{
  return(m_SunlightSensSet);
}

double CPhysicalWelfare::GetTemperatureMin()
{
  return(m_TemperMin);
}

double CPhysicalWelfare::GetTemperatureMax()
{
  return(m_TemperMax);
}