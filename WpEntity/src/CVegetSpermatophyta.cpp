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
// FILE: CVegetSpermatophyta.cpp
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
 
#include "CVegetSpermatophyta.h"
#include "CBiotop.h" 
#include "Definitions.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::CVegetSpermatophyta
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    string label : String containing label
//               Point_t initCoord, int layer : Born coordinate
//               CGenome* pGenome : Ptr on a valid Genome object
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CVegetSpermatophyta::CVegetSpermatophyta(string label, Point_t initCoord, size_t layer, CGenome* pGenome):
CVegetable(label, initCoord, layer, pGenome)
{
  // Default values          
  m_Status = STATUS_ALIVE; 

  // Parameter id pre-init
  m_id_PollenRange = invalidCharIndex;;
}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::CVegetSpermatophyta
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CVegetSpermatophyta& model : Clonage reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CVegetSpermatophyta::CVegetSpermatophyta(string label, CVegetSpermatophyta& model):
CVegetable(label, model)
{ 
  // Default values          
  m_Status = STATUS_ALIVE; 

  // Parameter id pre-init
  m_id_PollenRange = invalidCharIndex;;
}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::CVegetSpermatophyta
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CVegetSpermatophyta& mother,father : Parents for sexual reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CVegetSpermatophyta::CVegetSpermatophyta(string label, CVegetSpermatophyta& mother,CVegetSpermatophyta& father):
CVegetable(label, mother, father)
{ 
  // Default values          
  m_Status = STATUS_ALIVE; 

  // Parameter id pre-init
  m_id_PollenRange = invalidCharIndex;;
}
  
//===========================================================================
// Genetic settings
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::setParamFromGene 
//  
// DESCRIPTION:  Set parameter according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CVegetable parameter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CVegetSpermatophyta::setParamFromGene(CGene* pGen)
{
  if (CVegetable::setParamFromGene(pGen) == true)
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
  case GENE_PARAM_POLLEN_RANGE:
    {
      if (m_id_PollenRange != invalidCharIndex) delete(getParameterNoCheck(m_id_PollenRange)); // delete if already set
      m_id_PollenRange = addParameterFromGene(pGen, PARAM_REPRODUCTION);
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
  // If resu is false, param is not valid for CVegetSpermatophyta, but it may be used by inherited class !
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::completeParamsWithDefault 
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
bool CVegetSpermatophyta::completeParamsWithDefault()
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value

  // In base class
  CVegetable::completeParamsWithDefault();

  // CVegetable specific
  if (m_id_PollenRange == invalidCharIndex)
  {
    m_id_PollenRange = addParameterFromGeneDefinition(PARAM_REPRODUCTION, GENE_PARAM_POLLEN_RANGE);
  }

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::setLifeStageFromGene 
//  
// DESCRIPTION:  Set a Life stage according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a life stage
//  
// REMARKS:      Should NOT be called by derived method. 
//               All stages must be supported at the same inheritage level
//---------------------------------------------------------------------------
bool CVegetSpermatophyta::setLifeStageFromGene(CGene* pGen)
{
  if ((pGen == NULL) || (pGen->getGeneType() != GENE_LIFESTAGE))
  {
    return false;
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  size_t len = rawData.size();
  if (len < sizeof(WORD))
  {
    // not enought data to config param
    return false;
  }

  CLifeStage* pLifeStage = NULL;
  int duration;
  double lifeDuration = getLifeExpectation();
  double deathDuration = getRotenTimeExpectation();
  double scaledRate = pGen->getElementValue(0) / 100.0;

  switch (pGen->getGeneSubType())
  {
  case GENE_STAGE_0:
  {
    duration = 0; // Age during gestation is not counted.
    pLifeStage = new CLifeStage("Seed", STAGE_0, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
      resu = true;
    break;
  }
  case GENE_STAGE_1:
  {
    duration = (int)(scaledRate * lifeDuration);
    pLifeStage = new CLifeStage("juvenile", STAGE_1, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
      resu = true;
    break;
  }
  case GENE_STAGE_2:
  {
    duration = (int)(scaledRate * lifeDuration);
    pLifeStage = new CLifeStage("Adult", STAGE_2, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
      resu = true;
    break;
  }
  case GENE_STAGE_3:
  {
    duration = (int)(scaledRate * lifeDuration);
    pLifeStage = new CLifeStage("Adult", STAGE_3, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
      resu = true;
    break;
  }
  case GENE_STAGE_4:
  {
    duration = (int)(scaledRate * lifeDuration);
    pLifeStage = new CLifeStage("Old", STAGE_4, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
      resu = true;
    break;
  }
  case GENE_STAGE_5:
  {
    duration = (int)(scaledRate * deathDuration);
    pLifeStage = new CLifeStage("Dead", STAGE_5, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
      resu = true;
    break;
  }
  case GENE_STAGE_6:
  {
    duration = (int)(scaledRate * deathDuration);
    pLifeStage = new CLifeStage("Roten", STAGE_6, duration);
    if (!addLifeStage(pLifeStage))
      delete pLifeStage;
    else
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

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::nextDay
//  
// DESCRIPTION:  Daily action: reproduction
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CVegetSpermatophyta::nextDay(bool doQuickAgeing)
{
  if (isAlive() && !doQuickAgeing)
  {
    tryToReproduceOnceADay();
  }
  CVegetable::nextDay(doQuickAgeing);
}

void CVegetSpermatophyta::tryToReproduceOnceADay()
{
  if (m_pBiotop && testChance(getReproductionRate(), m_pBiotop->getFertility(getGridCoord())))
  {
    if (getTypeOfReproduction() == REPRODUCT_CLONAGE)
    {
      autoClone();
    }
    else if ((getTypeOfReproduction() == REPRODUCT_SEXUAL) && (getSex() == SEX_FEMALE))
    {
      CLifeStage* pCurrentLifeStage = getCurrentLifeStage();
      if ((pCurrentLifeStage == NULL) || (pCurrentLifeStage->getStageType() > STAGE_1))
      {
        CBasicEntity* pFoundEntity = NULL;
        const BiotopFoundIds_t& biotopFoundIds = getBiotop()->findEntities(getGridCoord(), (int)getReproductionRange());
        const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;

        for (size_t i = 0; i < biotopFoundIds.nbFoundIds; i++)
        {
          pFoundEntity = tFoundIds[i].pEntity;
          if ((pFoundEntity->getSex() == SEX_MALE) && (m_pGenome->checkSpecieCompatibility(pFoundEntity->getGenome()) == true))
          {
            // TBD: Select 1 random compatible male among different parters
            reproductWith((CVegetSpermatophyta*)tFoundIds[i].pEntity);
            break;
          }
        }
      }
    }
  }
}

void CVegetSpermatophyta::defaultActionWhenAttachedToBiotop(void)
{
  if (m_pBiotop && (getLayer() > 1))
  {
    // Decrease fertility around trees
    Point_t tmpCoord;
    tmpCoord.x = getGridCoord().x - 1;
    tmpCoord.y = getGridCoord().y;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.y--;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.x++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.x++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.y++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.y++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.x--;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
    tmpCoord.x--;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, -10);
  }
}

void CVegetSpermatophyta::defaultActionWhenRemovedFromBiotop(void)
{
  if (m_pBiotop && (getLayer() > 1))
  {
    // Restore fertility
    Point_t tmpCoord;
    tmpCoord.x = getGridCoord().x - 1;
    tmpCoord.y = getGridCoord().y;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.y--;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.x++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.x++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.y++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.y++;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.x--;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
    tmpCoord.x--;
    m_pBiotop->ModifyGridFertilityBonus(tmpCoord, 10);
  }
}

//===========================================================================
// Life stages management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::enterInNewLifeStage
//  
// DESCRIPTION:  Change parameters according to new stage
// 
// ARGUMENTS:    CLifeStage* pLifeStage : new life stage
//   
// RETURN VALUE: None
//  
// REMARKS:       
//---------------------------------------------------------------------------
void CVegetSpermatophyta::enterInNewLifeStage(CLifeStage* pLifeStage)
{
  switch (pLifeStage->getStageType())
  {
    case STAGE_0:
    {
      m_Status = STATUS_STATIC;
      break;
    }
    case STAGE_1:
    {
      // entity is initially created on default layer then forced to layer1 as juvenile
      defaultActionWhenRemovedFromBiotop();
      if (!moveToLayerIfPossible(1))
      {
        CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
        CYBIOCORE_LOG("VEGETAL - Layer not free for juvenil life stage. Entity  %s removed\n", getLabel().c_str());
        defaultActionWhenAttachedToBiotop();
        autoRemove();
      }
      else
      {
        m_Status = STATUS_ALIVE;
        setResistanceToConsumptionToNominalRatio(10);
        setReproductionRateToNominalRatio(0);
       }
      break;
    }
    case STAGE_2:
    {
      if (!moveToLayerIfPossible(getDefaultLayer()))
      {
        CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
        CYBIOCORE_LOG("VEGETAL - Layer not free for adult life stage. Entity  %s removed\n", getLabel().c_str());
        autoRemove();
      }
      else
      {
        setResistanceToConsumptionToNominalRatio(100);
        setReproductionRateToNominalRatio(100);
        defaultActionWhenAttachedToBiotop();
      }
      break;
    }
    case STAGE_3:
    {
      break;
    }
    case STAGE_4:
    {
      break;
    }
    case STAGE_5:
    {
      break;
    }
    case STAGE_6:
    {
      break;
    }
    default:
    {
      break;
    }
  }
}

//===========================================================================
// Behavior  
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::reproductWith
//  
// DESCRIPTION:  Clone entity
// 
// ARGUMENTS:    CVegetSpermatophyta* partner : male entity to make child
//   
// RETURN VALUE: bool: Success
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CVegetSpermatophyta::reproductWith(CVegetSpermatophyta* partner)
{
  string childLabel;
  childLabel = FormatString("j%d", m_TotalChildNumber);
  childLabel = m_Label + childLabel;
  CVegetSpermatophyta* pChildEntity = new CVegetSpermatophyta(childLabel,*this,*partner);
  m_TotalChildNumber++;
  // Caracter from genetic after mutation:
  pChildEntity->setEntityFromGenome(m_pBiotop->getRadioactivityRate());

  int range = (int)getReproductionRange();
  int xOfset = getRandInt(2 * range) - range;
  int yOfset = getRandInt(2 * range) - range;
  Point_t newCoord = {getGridCoord().x + xOfset, getGridCoord().y + yOfset};
  pChildEntity->changeWeight(0.1); // ensure weight is more than min
  entityIdType resuId = m_pBiotop->addEntity(pChildEntity, newCoord, getLayer());

  if (resuId == ENTITY_ID_INVALID)
  {
    delete pChildEntity;
  }
  else if (!pChildEntity->checkHabitat())
  {
    pChildEntity->autoRemove(); // Cannot growth in water
  }

  return true;
}


//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::autoClone
//  
// DESCRIPTION:  Clone entity
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool: Success
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CVegetSpermatophyta::autoClone()
{
  string childLabel;
  childLabel = m_Label;
  CVegetSpermatophyta* pChildEntity = new CVegetSpermatophyta(childLabel,*this);
  m_TotalChildNumber++;
  // Caracter from genetic after mutation:
  pChildEntity->setEntityFromGenome(m_pBiotop->getRadioactivityRate());

  // Copy parameters
  CGenericParam* pParam;
  CGenericParam* pModelParam;
  for (size_t i = 0; i < pChildEntity->getNumParameter(); i++)
  {
    pParam = pChildEntity->getParameter(i);
    pModelParam = getParameter(i);
    pParam->setValMin(pModelParam->getMin());
    pParam->setValMax(pModelParam->getMax());
    pParam->setValNominal(pModelParam->getValNominal());
    pParam->setVal(pModelParam->getVal());
  }

  // Reset some paramters
  pChildEntity->forceAgeValue(0);
  pChildEntity->forceWeight(pChildEntity->getMinWeight() + 0.1); // ensure weight is more than min

  int range = (int)getReproductionRange();
  int xOfset = getRandInt(2*range) - range;
  int yOfset = getRandInt(2*range) - range;
  Point_t newCoord = {getGridCoord().x + xOfset, getGridCoord().y + yOfset};
  entityIdType resuId = m_pBiotop->addEntity(pChildEntity, newCoord, getLayer());

  if (resuId == ENTITY_ID_INVALID)
  {
    delete pChildEntity;
  }
  else if (!pChildEntity->checkHabitat())
  {
    pChildEntity->autoRemove(); // Cannot growth in water
  }

  return true;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

double CVegetSpermatophyta::getPollenRange()
{
  return getParameterNoCheck(m_id_PollenRange)->getVal();
}




