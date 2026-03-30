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
// FILE: CPlant.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a plant entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CPlant.h"
#include "CBiotop.h" 
#include "Definitions.h"
#include "CPhysicalWelfare.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPlant::CPlant
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    string label : String containing label
//               CGenome* pGenome : Ptr on a valid Genome object
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CPlant::CPlant(string label, size_t layer, CGenome* pGenome)
{ 
  // Input values
  setLayerAndDefaultLayer(layer);
  m_pGenome = pGenome;
  m_pBrain = NULL;
  m_pPhysicWelfare = new CPhysicalWelfare(this);

  // Default values          
  m_Status = STATUS_ALIVE; 
  m_Label = label;
  m_Generation = 0;
  m_TotalChildNumber = 0;

  // Parameter id pre-init
  initPlantParamIds();

  // Give default values for attributes
  setColor(0x00A6E0F9);
  setTaste(TASTE_SWEET);
  setOdor(ODOR_FRAGRANT);
  setTypeOfReproduction(REPRODUCT_CLONAGE);
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::CPlant
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CPlant& model : Single parent constructor for cloning
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------     
CPlant::CPlant(string label, CPlant& model) 
{ 
  m_Label = label;
  // inherited
  setLayerAndDefaultLayer(model.getLayer());
  m_Generation = model.m_Generation + 1;
  m_pGenome = new CGenome(*model.m_pGenome);

  // reset
  m_pBrain = NULL;
  m_Status = STATUS_ALIVE; 
  m_pPhysicWelfare = new CPhysicalWelfare(this);
  m_TotalChildNumber = 0;

  // Parameter id pre-init
  initPlantParamIds();

  // Give default values for attributes
  setOdor(model.getOdor());
  setForm(model.getForm());
  setTaste(model.getTaste());
  setTypeOfReproduction(model.getTypeOfReproduction());
  setPheromone(model.getPheromone());
  setColor(model.getColorRgb());
  setTexture(model.getTexture());
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::CPlant
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CPlant& mother,father : Parents for sexual reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------     
CPlant::CPlant(string label, CPlant& mother,CPlant& father) 
{ 
  m_Label = label;
  // inherited
  setLayerAndDefaultLayer(mother.getLayer());
  m_Generation = mother.m_Generation + 1;
  m_pGenome = new CGenome(*mother.m_pGenome, *father.m_pGenome, 1.0);

  // reset
  m_pBrain = NULL;
  m_Status = STATUS_ALIVE; 
  m_pPhysicWelfare = new CPhysicalWelfare(this);
  m_TotalChildNumber = 0;

  // Parameter id pre-init
  initPlantParamIds();
}  

void CPlant::initPlantParamIds()
{
  m_id_Age = invalidCharIndex;
  m_id_Decomposition = invalidCharIndex;
  m_id_ReproductionRate = invalidCharIndex;
  m_id_Health = invalidCharIndex;
  m_id_GrowthSpeed = invalidCharIndex;
  m_id_ReproductionRange = invalidCharIndex;
  m_id_ResistanceToConsumption = invalidCharIndex;
}

//===========================================================================
// Genetic settings
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPlant::setParamFromGene 
//  
// DESCRIPTION:  Set parameter according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CPlant parameter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CPlant::setParamFromGene(CGene* pGen)
{
  if (CBasicEntity::setParamFromGene(pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return true;
  }
  
  if ((pGen == NULL) || (pGen->getGeneType() != GENE_PARAMETER))
  {
    return false;
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  size_t len = rawData.size();
  if (len < 3 * sizeof(WORD))
  {
    // not enought data to config param
    return false;
  }

  switch(pGen->getGeneSubType())
  {
  case GENE_PARAM_AGE:
    {
      if (m_id_Age != invalidCharIndex) delete(getParameterNoCheck(m_id_Age)); // delete if already set
      m_id_Age = addParameterFromGene(pGen, PARAM_DURATION);
      resu = true;
      break;
    }
  case GENE_PARAM_DECOMPOSITION:
    {
      if (m_id_Decomposition != invalidCharIndex) delete(getParameterNoCheck(m_id_Decomposition)); // delete if already set
      m_id_Decomposition = addParameterFromGene(pGen, PARAM_DURATION);
      resu = true;
      break;
    }
  case GENE_PARAM_REPRO_RATE:
    {
      if (m_id_ReproductionRate != invalidCharIndex) delete(getParameterNoCheck(m_id_ReproductionRate)); // delete if already set
      m_id_ReproductionRate = addParameterFromGene(pGen, PARAM_REPRODUCTION);
      resu = true;
      break;
    }
  case GENE_PARAM_GROWTH_SPEED:
    {
      if (m_id_GrowthSpeed != invalidCharIndex) delete(getParameterNoCheck(m_id_GrowthSpeed)); // delete if already set
      m_id_GrowthSpeed = addParameterFromGene(pGen, PARAM_BIOLOGICAL);
      resu = true;
      break;
    }
  case GENE_PARAM_REPRO_RANGE:
    {
      if (m_id_ReproductionRange != invalidCharIndex) delete(getParameterNoCheck(m_id_ReproductionRange)); // delete if already set
      m_id_ReproductionRange = addParameterFromGene(pGen, PARAM_REPRODUCTION);
      resu = true;
      break;
    }
  case GENE_PARAM_RESISTANCE_TO_CONSUMPTION:
  {
    if (m_id_ResistanceToConsumption != invalidCharIndex) delete(getParameterNoCheck(m_id_ResistanceToConsumption)); // delete if already set
    m_id_ResistanceToConsumption = addParameterFromGene(pGen, PARAM_PHYSICAL);
    resu = true;
    break;
  }
  default:
    {
      // Unknown
      resu = false;
      break;
    }
  }
  // If resu is false, param is not valid for CPlant, but it may be used by inherited class !
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::completeParamsWithDefault 
//  
// DESCRIPTION:  Complete parameters unset by genome with default values.
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CPlant::completeParamsWithDefault()
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value

  // In base class
  CBasicEntity::completeParamsWithDefault();

  // CPlant specific
  if (m_id_Age == invalidCharIndex)
  {
    m_id_Age = addParameterFromGeneDefinition(PARAM_DURATION, GENE_PARAM_AGE);
  }
  if (m_id_Decomposition == invalidCharIndex)
  {
    m_id_Decomposition = addParameterFromGeneDefinition(PARAM_DURATION, GENE_PARAM_DECOMPOSITION);
  }
  if (m_id_ReproductionRate == invalidCharIndex)
  {
    m_id_ReproductionRate = addParameterFromGeneDefinition(PARAM_REPRODUCTION, GENE_PARAM_REPRO_RATE);
  }
  if (m_id_Health == invalidCharIndex)
  {
    m_id_Health = addParameterCustom(0, 100, 100, 100, "Health rate", PARAM_FEELING);
  }
  if (m_id_GrowthSpeed == invalidCharIndex)
  {
    m_id_GrowthSpeed = addParameterFromGeneDefinition(PARAM_BIOLOGICAL, GENE_PARAM_GROWTH_SPEED);
  }
  if (m_id_ReproductionRange == invalidCharIndex)
  {
    m_id_ReproductionRange = addParameterFromGeneDefinition(PARAM_REPRODUCTION, GENE_PARAM_REPRO_RANGE);
  }
  if (m_id_ResistanceToConsumption == invalidCharIndex)
  {
    m_id_ResistanceToConsumption = addParameterFromGeneDefinition(PARAM_PHYSICAL, GENE_PARAM_RESISTANCE_TO_CONSUMPTION);
  }

  // keep info to restore when plant is healthy
  m_OriginalColorRgb = getColorRgb();
  m_OriginalProtection = getProtection();
  m_OriginalOdor = getOdor();

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::setPhysicWelfareFromGene 
//  
// DESCRIPTION:  Set a Physic Welfare sensitivity according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity Physic Welfare
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CPlant::setPhysicWelfareFromGene(CGene* pGen)
{
  if ((pGen == NULL) || (pGen->getGeneType() != GENE_PHY_WELFARE))
  {
    return false;
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  size_t len = rawData.size();
  if ((len < sizeof(WORD)) || (pGen->getNumElements() < 4))
  {
    // not enought data to config param
    return false;
  }

  double sensitivity = pGen->getElementValue(0);
  double min = pGen->getElementValue(1);
  double max = pGen->getElementValue(3);

  switch(pGen->getGeneSubType())
  {
  case GENE_PHYS_SENS_HABITAT:
    {
      m_pPhysicWelfare->SetHabitatSensitivity(sensitivity);
      resu = true;
      break;
    }
  case GENE_PHYS_SENS_TEMPER:
    {
      m_pPhysicWelfare->SetTemperatureSensitivity(min, max, sensitivity);
      resu = true;
      break;
    }
  case GENE_PHYS_SENS_FERTILITY:
    {
      m_pPhysicWelfare->SetFertilitySensitivity(min, max, sensitivity);
      resu = true;
      break;
    }
  default:
    {
      // Unknown
      resu = false;
      break;
    }
  }

  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::completePhysicWelfareWithDefault 
//  
// DESCRIPTION:  Complete Physic Welfare sensitivity unset by genome 
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CPlant::completePhysicWelfareWithDefault(void)
{
  if (m_pPhysicWelfare == NULL)
    return false;

  if (m_pPhysicWelfare->IsHabitatSensSet() == false)
  {
    m_pPhysicWelfare->SetHabitatSensitivity(100.0); // immediate death
  }

  if (m_pPhysicWelfare->IsTemperSensSet() == false)
  {
    m_pPhysicWelfare->SetTemperatureSensitivity(0.0, 40.0, 10.0);
  }

  if (m_pPhysicWelfare->IsFertilSensSet() == false)
  {
    m_pPhysicWelfare->SetFertilitySensitivity(20.0, 100.0, 10.0);
  }

  // TBD: temporary. must be confirmed
  m_pPhysicWelfare->SetRecoveryBonus(1.0);

  return true;
}

//===========================================================================
// Genetic description
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPlant::buildPhysicWellfareString 
//  
// DESCRIPTION:  Give info string on Physic Wellfare interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CPlant::buildPhysicWellfareString(CGene* pGen)
{
  string welfareStr = CBasicEntity::buildPhysicWellfareString(pGen);
  string tempStr;
  if (welfareStr != STRING_GENE_UNUSED)
  {
    // The gene has already been taken into account by basic entity
    return welfareStr;
  }

  if ((pGen == NULL) || (pGen->getGeneType() != GENE_PHY_WELFARE))
  {
    return welfareStr;
  }
  // We are sure Gene is a parameter
  auto rawData = pGen->getData();
  size_t len = rawData.size();
  if ((len < sizeof(WORD)) || (pGen->getNumElements() < 4))
  {
    // not enought data to config param
    return welfareStr;
  }

  double sensitivity = pGen->getElementValue(0);
  double scaledVal1  = pGen->getElementValue(1);
  double scaledVal2  = pGen->getElementValue(2);
  double scaledVal3  = pGen->getElementValue(3);

  switch(pGen->getGeneSubType())
  {
  case GENE_PHYS_SENS_HABITAT:
    {
      welfareStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.2f %%", sensitivity );
      welfareStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
  case GENE_PHYS_SENS_TEMPER:
  case GENE_PHYS_SENS_FERTILITY:
    {
      welfareStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.2f %%", sensitivity );
      welfareStr += pGen->getElementStrName(0) + tempStr;
      tempStr = FormatString("=%6.2f C", scaledVal1);
      welfareStr += pGen->getElementStrName(1) + tempStr;
      tempStr = FormatString("=%6.2f C", scaledVal2);
      welfareStr += pGen->getElementStrName(2) + tempStr;
      tempStr = FormatString("=%6.2f C", scaledVal3);
      welfareStr += pGen->getElementStrName(3) + tempStr;
      break;
    }
  default:
    {
      // Unknown
      break;
    }
  }

  return welfareStr;
}

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CPlant::nextHour
//  
// DESCRIPTION:  Secondary action: parameters update and checkup
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CPlant::nextHour() 
{
  if (isAlive() && isLocalAutoControlled())
  {
    // Compute health according to fertility and temper
    double healthVar = m_pPhysicWelfare->ComputeAndGetHealthVariation();
    if (changeHealthRate(healthVar) == false)
    {
      CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
      CYBIOCORE_LOG("PLANT  - Death : specie %s name %s is dead due to bad environment conditions\n", 
                     getSpecieName().c_str(), getLabel().c_str());
    }
  }

  CBasicEntity::nextHour();
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::nextDay
//  
// DESCRIPTION:  Living day trigger called by CBasicEntity every day
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CPlant::nextDay(bool doQuickAgeing)
{
  // get older
  if (isAlive())
  {
    // retore def values (new leafs are available)
    setColor(m_OriginalColorRgb);
    setProtection(m_OriginalProtection);
    setOdor(m_OriginalOdor);

    double growthWeight = getParameterNoCheck(m_id_GrowthSpeed)->getVal() / 100;
    changeWeight(growthWeight);

    getParameterNoCheck(m_id_Age)->changeVal(1);
    if (getParameterNoCheck(m_id_Age)->isMaxReached())
    {
      autoKill();
    }
  }
  else
  {
    getParameterNoCheck(m_id_Decomposition)->changeVal(1);
    changeWeight(-0.2);
    if (getParameterNoCheck(m_id_Decomposition)->isMaxReached())
    {
      autoRemove();
    }
  }
  CBasicEntity::nextDay(doQuickAgeing);
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::getAge
//  
// DESCRIPTION:  Return Age in days
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      
//---------------------------------------------------------------------------
int CPlant::getAge()
{
  return ((int)getParameterNoCheck(m_id_Age)->getVal());
}

void CPlant::forceAgeValue(int newAge)
{
  getParameterNoCheck(m_id_Age)->setVal(newAge);
}

int CPlant::getDecompositionTime()
{
  return ((int)getParameterNoCheck(m_id_Decomposition)->getVal());
}

//---------------------------------------------------------------------------
// METHOD:       CPlant::changeHealthRate
//  
// DESCRIPTION:  Make animal more Thirsty
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: True: entity is still alive. False: entity dead
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CPlant::changeHealthRate(double variation)
{
  getParameterNoCheck(m_id_Health)->changeVal(variation);

  if (getParameterNoCheck(m_id_Health)->isMinReached())
  {
    autoKill();
    return false;
  }
  return true;
}

void CPlant::setHealthRate(double rate)
{
  getParameterNoCheck(m_id_Health)->setVal(rate);
}

//===========================================================================
// Behavior  
//===========================================================================
void CPlant::autoKill()
{
  setColor(0x00008080);  // Color Brown when plant is dead
  CBasicEntity::autoKill();
}

//===========================================================================
// Get / Set for attributes
//===========================================================================
bool CPlant::isPlant()
{
  return true;
}

double CPlant::getReproductionRate() 
{
  return getParameterNoCheck(m_id_ReproductionRate)->getVal();
}
    
double CPlant::getLifeExpectation() 
{
  return getParameterNoCheck(m_id_Age)->getMax();
}

double CPlant::getRotenTimeExpectation() 
{
  return getParameterNoCheck(m_id_Decomposition)->getMax();
}

double CPlant::getReproductionRange()
{
  return getParameterNoCheck(m_id_ReproductionRange)->getVal();
}


double CPlant::changeWeight(double variation)
{
  if (variation > 0)
  {
    return CBasicEntity::changeWeight(variation);
  }
  else // variation<0
  {
    CGenericParam* pParam = getParameterNoCheck(m_id_Weight);
    double curVal, minVal, remaining;
    curVal = pParam->getVal();
    minVal  = pParam->getMin();
    remaining = curVal-minVal;

    if ((-variation > remaining) && testChance(getParameterNoCheck(m_id_ResistanceToConsumption)->getVal()))
    {
      // Plant is totaly eaten but can survive
      // change temporaly color and protection
      setColor(0x00008080);  // Color Brown when plant has no more leafs
      setProtection(50.0);   // Protection when plant has no more leafs 
      setOdor(ODOR_NONE);    // Odor when plant has no more leafs
      changeHealthRate(-10);
      return CBasicEntity::changeWeight(-remaining + 0.01);
    }
    else
    {
      return CBasicEntity::changeWeight(variation);
    }
  }
}

void CPlant::setResistanceToConsumptionToNominalRatio(double ratio)
{
  double newResistance = getParameterNoCheck(m_id_ResistanceToConsumption)->getValNominal() * ratio / 100.0;
  getParameterNoCheck(m_id_ResistanceToConsumption)->setVal(newResistance);
}

void CPlant::setReproductionRateToNominalRatio(double ratio)
{
  double newReproRate = getParameterNoCheck(m_id_ReproductionRate)->getValNominal() * ratio / 100.0;
  getParameterNoCheck(m_id_ReproductionRate)->setVal(newReproRate);
}