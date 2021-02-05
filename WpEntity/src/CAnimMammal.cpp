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
// FILE: CAnimMammal.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a mammal entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/09/2002 : Creation
//
//===========================================================================

#include "CAnimMammal.h"
#include "CBiotop.h" 
#include "Definitions.h"
#include "CGeoMapPurpose.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::CAnimMammal
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    string label : String containing name
//               Point_t initCoord, int layer : Born coordinate
//               CGenome* pGenome : Ptr on a valid Genome object
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CAnimMammal::CAnimMammal(string label, Point_t initCoord, size_t layer, CGenome* pGenome):
CAnimal(label, initCoord, layer, pGenome)
{  
  // Default values          
  m_Status      = STATUS_ALIVE; 
  
  // Parameter id pre-init
  m_id_GestationTime        = invalidIndex;
  m_id_GestationNumberRange = invalidIndex;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::CAnimMammal
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CAnimal& model : monoparental constructor for cloange
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CAnimMammal::CAnimMammal(string label, CAnimMammal& model):
CAnimal(label, model)
{  
  // Default values          
  m_Status      = STATUS_ALIVE; 
  
  // Parameter id pre-init
  m_id_GestationTime        = invalidIndex;
  m_id_GestationNumberRange = invalidIndex;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::CAnimMammal
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CAnimal& model : biparental constructor for reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CAnimMammal::CAnimMammal(string label, CAnimMammal& mother,CAnimMammal& father):
CAnimal(label, mother, father)
{  
  // Default values          
  m_Status      = STATUS_ALIVE; 
  
  // Parameter id pre-init
  m_id_GestationTime        = invalidIndex;
  m_id_GestationNumberRange = invalidIndex;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::~CAnimMammal
//  
// DESCRIPTION:  Destructor 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      All associated objects are delete...
//               Even if created from the outside
//---------------------------------------------------------------------------  
CAnimMammal::~CAnimMammal()
{
  
}

//===========================================================================
// Genetic settings
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::setParamFromGene 
//  
// DESCRIPTION:  Set parameter according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CAnimal parameter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimMammal::setParamFromGene (CGene* pGen)
{
  if (CAnimal::setParamFromGene (pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return (true);
  }
  
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_PARAMETER))
  {
    return (false);
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<3*sizeof(WORD))
  {
    // not enought data to config param
    return (false);
  }
    
  switch(pGen->getGeneSubType())
  {
  case GENE_PARAM_GESTA_TIME:
    {
      if (m_id_GestationTime != invalidIndex) delete(getParameter(m_id_GestationTime)); // delete if already set
      m_id_GestationTime = addParameterFromGene(pGen, PARAM_REPRODUCTION);
      resu = true;
      break;
    }
  case GENE_PARAM_GESTA_NB:
    {
      if (m_id_GestationNumberRange != invalidIndex) delete(getParameter(m_id_GestationNumberRange)); // delete if already set
      m_id_GestationNumberRange = addParameterFromGene(pGen, PARAM_REPRODUCTION);
      setGestationBabyNumber(0);
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
  // If resu is false, param is not valid for CAnimMammal, but it may be used by inherited class !
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::completeParamsWithDefault 
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
bool CAnimMammal::completeParamsWithDefault()
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value
  
  // In base class
  CAnimal::completeParamsWithDefault();
  
  // CAnimMammal specific
  if (m_id_GestationTime == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,200,"Gestation time",PARAM_REPRODUCTION,GENE_PARAM_GESTA_TIME);
    m_id_GestationTime    = addParameter(pParam);
  }
  if (m_id_GestationNumberRange == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,3,10,"Gestation size",PARAM_REPRODUCTION,GENE_PARAM_GESTA_NB);
    pParam->forceVal(0);
    m_id_GestationNumberRange = addParameter(pParam);
  }
  
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::setLifeStageFromGene 
//  
// DESCRIPTION:  Set a Life stage according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CAnimMammal life stage
//  
// REMARKS:      Should NOT be called by derived method. 
//               All stages must be supported at the same inheritage level
//---------------------------------------------------------------------------
bool CAnimMammal::setLifeStageFromGene (CGene* pGen)
{
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_LIFESTAGE))
  {
    return (false);
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<sizeof(WORD))
  {
    // not enought data to config param
    return (false);
  }
  
  CLifeStage* pLifeStage = NULL;
  int duration;
  double lifeDuration = getLifeExpectation();
  double deathDuration = getRotenTimeExpectation();
  double scaledRate = pGen->getElementValue(0);

  switch(pGen->getGeneSubType())
  {
  case GENE_STAGE_0:
    {
      duration  = 0; // Age during gestation is not counted.
      pLifeStage = new CLifeStage("Fetus",STAGE_0,duration);
      if (!addLifeStage(pLifeStage))
        delete pLifeStage;
      else
        resu = true;
      break;
    }
  case GENE_STAGE_1:
    {
      duration  = (int)(scaledRate*lifeDuration);
      pLifeStage = new CLifeStage("Baby",STAGE_1,duration);
      if (!addLifeStage(pLifeStage))
        delete pLifeStage;
      else
        resu = true;
      break;
    }
  case GENE_STAGE_2:
    {
      duration  = (int)(scaledRate*lifeDuration);
      pLifeStage = new CLifeStage("Child",STAGE_2,duration);
      if (!addLifeStage(pLifeStage))
        delete pLifeStage;
      else
        resu = true;
      break;
    }
  case GENE_STAGE_3:
    {
      duration  = (int)(scaledRate*lifeDuration);
      pLifeStage = new CLifeStage("Adult",STAGE_3,duration);
      if (!addLifeStage(pLifeStage))
        delete pLifeStage;
      else
        resu = true;
      break;
    }
  case GENE_STAGE_4:
    {
      duration  = (int)(scaledRate*lifeDuration);
      pLifeStage = new CLifeStage("Old",STAGE_4,duration);
      if (!addLifeStage(pLifeStage))
        delete pLifeStage;
      else
        resu = true;
      break;
    }
  case GENE_STAGE_5:
    {
      duration  = (int)(scaledRate*deathDuration);
      pLifeStage = new CLifeStage("Dead",STAGE_5,duration);
      if (!addLifeStage(pLifeStage))
        delete pLifeStage;
      else
        resu = true;
      break;
    }
  case GENE_STAGE_6:
    {
      duration  = (int)(scaledRate*deathDuration);
      pLifeStage = new CLifeStage("Roten",STAGE_6,duration);
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
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::completeLifeStagesWithDefault 
//  
// DESCRIPTION:  Complete Life stages unset by genome with default values.
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should NOT be called by derived method. 
//               All stages must be supported at the same inheritage level
//---------------------------------------------------------------------------
bool CAnimMammal::completeLifeStagesWithDefault(void)
{
  CLifeStage* pLifeStage = NULL;
  int duration;
  double lifeDuration = getLifeExpectation();
  double deathDuration = getRotenTimeExpectation();
  
  duration  = 0; // Age during gestation is not counted.
  pLifeStage = new CLifeStage("Fetus",STAGE_0,0);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  duration  = (int)(0.05*lifeDuration);
  pLifeStage = new CLifeStage("Baby",STAGE_1,duration);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  duration  = (int)(0.15*lifeDuration);
  pLifeStage = new CLifeStage("Child",STAGE_2,duration);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  duration  = (int)(0.70*lifeDuration);
  pLifeStage = new CLifeStage("Adult",STAGE_3,duration);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  duration  = (int)(0.10*lifeDuration);
  pLifeStage = new CLifeStage("Old",STAGE_4,duration);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  duration  = (int)(0.4*deathDuration);
  pLifeStage = new CLifeStage("Dead",STAGE_5,duration);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  duration  = (int)(0.6*deathDuration);
  pLifeStage = new CLifeStage("Roten",STAGE_6,duration);
  if (!addLifeStage(pLifeStage))
    delete(pLifeStage); // stage was already set by genome
  
  // Adjust lasts stage duration so that sums = lifeDuration and deathDuration
  int configLifeDuration = 0;
  int i;
  for (i=1;i<5;i++)
  {
    configLifeDuration += getLifeStage(i)->getTotalDayDuration();
  }
  if (lifeDuration > configLifeDuration)
  {
    // Enlarge Old stage
    getLifeStage(4)->setTotalDayDuration( getLifeStage(4)->getTotalDayDuration() 
      + (int)lifeDuration - configLifeDuration + 1 );
  }
  int configDeathDuration = 0;
  for (i=5;i<7;i++)
  {
    configDeathDuration += getLifeStage(i)->getTotalDayDuration();
  }
  if (deathDuration > configDeathDuration)
  {
    // Enlarge Roten stage
    getLifeStage(6)->setTotalDayDuration( getLifeStage(6)->getTotalDayDuration() 
      + (int)deathDuration - configDeathDuration + 1 );
  }
  
  return (true);
}


//===========================================================================
// Genetic description
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::buildLifeStageString 
//  
// DESCRIPTION:  Give info string on Life Stage interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimMammal::buildLifeStageString(CGene* pGen)
{
  string paramStr = STRING_GENE_UNUSED;
  string tempStr;
  
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_LIFESTAGE))
  {
    return (paramStr);
  }
  // We are sure Gene is a parameter
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<sizeof(WORD))
  {
    // not enought data to config param
    return (paramStr);
  }
 
  double scaledVal1 = pGen->getElementValue(0);
  
  switch(pGen->getGeneSubType())
  {
  case GENE_STAGE_0:
  case GENE_STAGE_1:
  case GENE_STAGE_2:
  case GENE_STAGE_3:
  case GENE_STAGE_4:
  case GENE_STAGE_5:
  case GENE_STAGE_6:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.2f%%", scaledVal1);
      paramStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
  default:
    {
      // keep STRING_GENE_UNUSED
      break;
    }
  }
  
  return (paramStr);
}


//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::nextHour
//  
// DESCRIPTION:  Secondary action: parameters update and checkup
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CAnimMammal::nextHour() 
{
  if (isAlive() && !isRemoteControlled())
  {
    if (getCurrentLifeStage()->getStageType() == STAGE_3)
    {
      if(getSex() == SEX_MALE)
      {
        changeLibidoRate(10.0);
      }
      else if (getSex() == SEX_FEMALE)
      {
        changeLibidoRate(1.0);
        if (getLibidoRate()>50)
        {
          setPheromone(PHEROMONE_SEXUAL_FEMALE);
          removeAttribute(PHY_ATTRIBUTE_UDDER);
        }
      }
    }
  }
  
  CAnimal::nextHour();
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::nextDay
//  
// DESCRIPTION:  Living day trigger called by CBasicEntity every 24 hours
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CAnimMammal::nextDay(bool forceGrowth)
{
  // get older
  if (isAlive() && !isRemoteControlled())
  {
    if (getGestationBabyNumber()>0)
    {
      increaseGestationTime(1); 
    }
  }
  
  CAnimal::nextDay(forceGrowth);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::increaseGestationTime
//  
// DESCRIPTION:  Increase gestation time and check parameters
// 
// ARGUMENTS:    int variation : duration in day 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimMammal::increaseGestationTime(int variation)
{
  if (isAlive())
  {
    getParameter(m_id_GestationTime)->changeVal(1);
    if (getParameter(m_id_GestationTime)->isMaxReached())
    {
      deliverAllBabies();
    }
  }
}

//===========================================================================
// Life stages management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::enterInNewLifeStage
//  
// DESCRIPTION:  Change parameters according to new stage
// 
// ARGUMENTS:    CLifeStage* pLifeStage : new life stage
//   
// RETURN VALUE: None
//  
// REMARKS:       
//---------------------------------------------------------------------------
void CAnimMammal::enterInNewLifeStage(CLifeStage* pLifeStage)
{
  switch(pLifeStage->getStageType())
  {
  case STAGE_0:
    {
      m_Status = STATUS_STATIC;
      break;
    }
  case STAGE_1:
    {
      // baby settings
      m_Status = STATUS_ALIVE;
      setCuriosityToNominalRatio(49);
      setLearningToNominalRatio(60);
      setGrowthSpeedToNominalRatio(200);
      setAttackFactorToNominalRatio(20);
      setPheromone(PHEROMONE_BABY);
      break;
    }
  case STAGE_2:
    {
      // child settings
      setCuriosityToNominalRatio(51);
      setLearningToNominalRatio(80);
      setGrowthSpeedToNominalRatio(100);
      setAttackFactorToNominalRatio(80);
      setPheromone(PHEROMONE_NONE);
      m_pBrain->ResetReactionsFailureSuccessFactor();
      break;
    }
  case STAGE_3:
    {
      // adult settings
      setCuriosityToNominalRatio(50);
      setLearningToNominalRatio(60);
      setGrowthSpeedToNominalRatio(0);
      setAttackFactorToNominalRatio(100);
      if (getSex() == SEX_MALE)
        setPheromone(PHEROMONE_SEXUAL_MALE);
      break;
    }
  case STAGE_4:
    {
      // old settings
      setCuriosityToNominalRatio(50);
      setLearningToNominalRatio(40);
      setGrowthSpeedToNominalRatio(0);
      setAttackFactorToNominalRatio(100);
      setPheromone(PHEROMONE_NONE);
      removeAttribute(PHY_ATTRIBUTE_UDDER);
      break;
    }
  case STAGE_5:
    {
      setCuriosityToNominalRatio(0);
      m_pBrain->SetCurrentReactionIndex(0);
      // Defense decrease a lot
      double newProtection = getProtection() / 5.0;
      setProtection(newProtection);
      setPheromone(PHEROMONE_NONE);
      removeAttribute(PHY_ATTRIBUTE_UDDER);
      setOdor(ODOR_REPULSIVE);
      break;
    }
  case STAGE_6:
    {
      //setToxicity(10);
      setOdor(ODOR_NAUSEOUS);
      setPheromone(PHEROMONE_NONE);
      setForm(FORM_CARRION); // Change form (should be skeleton)
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
// METHOD:       CAnimMammal::autoKill
//  
// DESCRIPTION:  Put entity in dead mode and in stage 5
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimMammal::autoKill()
{
  stopCurrentSpeed();
  CBasicEntity::autoKill();
  if (!isImmortal())
  {
    setCurrentLifeStages(STAGE_5);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::ExecuteEatAction
//  
// DESCRIPTION:  For baby, drink milk on mother, otherwise eat normally
// 
// ARGUMENTS:    int relLayer: {-m_nbLayer..m_nbLayer} - relative layer to perform eat
//               success/failureSatisfactionFactor, : factor for pleasure management
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimMammal::ExecuteEatAction(int relLayer, double successSatisfactionFactor, double failureFrustrationFactor)
{
  if ( getPheromone() != PHEROMONE_BABY )
  {
    return CAnimal::ExecuteEatAction(relLayer, successSatisfactionFactor, failureFrustrationFactor);
  }
  else // Specific baby behavior: drink milk
  {
    double pleasureRate = 0;
    RelativePos_t relPos = {1,0};
    double eatenWeight = 0;
    
    moveToGridEdgePos();
    Point_t newCoord = getGridCoordRelative(relPos);
    
    CBasicEntity* pEatenEntity = m_pBiotop->findEntity(newCoord,getLayer()+relLayer);
    if ( (pEatenEntity==NULL) || ( (pEatenEntity!=NULL) && (pEatenEntity->getId()==ENTITY_ID_WATER) ) )// Rq: Water (id=0) cannot be eaten
    {
      // Nothing to eat: small frustration
      pleasureRate = -failureFrustrationFactor;
    }
    else
    {
      // Check that suckling entity is a mother female of the same specie 
      if ( (m_pGenome->checkSpecieCompatibility(pEatenEntity->getGenome()) == true)
        &&(pEatenEntity->isAlive()) && (pEatenEntity->getPheromone() == PHEROMONE_MATERNAL) )
      {
        eatenWeight = getWeight()/1000;
        // Remove fat from mother (milk generation)
        ((CAnimMammal*)pEatenEntity)->changeFatWeight(eatenWeight);
        ((CAnimMammal*)pEatenEntity)->changeLibidoRate(-1); // Prevent mother to have other babies
        // Enjoy the lunch
        pleasureRate = getHungerRate() * successSatisfactionFactor / 100.0; 
        // 25% of milk is digested:
        changeFatWeight(eatenWeight/4.0);
        changeHungerRate(-0.2);
        changeThirstRate(-0.2);
        changeStomachFillingRate(0.5);
        // Reenforce mother recognition
        getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pEatenEntity, IDENTIFICATION_MOTHER);
      }
      else
      {
        // No milk :-(
        pleasureRate = -failureFrustrationFactor;
        // Reenforce mother recognition
        getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pEatenEntity, IDENTIFICATION_MOTHER);
      }
    } 
    
    changePleasureRate(pleasureRate);

    if (pleasureRate<0)
      return (false);
    else
      return (true);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::ExecuteCopulateAction
//  
// DESCRIPTION:  Action copulate
// 
// ARGUMENTS:    success/failureSatisfactionFactor, : factor for pleasure management
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimMammal::ExecuteCopulateAction(double successSatisfactionFactor, double failureFrustrationFactor)
{
  double pleasureRate = 0;
  RelativePos_t relPos = {1,0};
  Point_t newCoord = getGridCoordRelative(relPos);
  
  moveToGridEdgePos();
  
  CBasicEntity* pPartnerEntity = m_pBiotop->findEntity(newCoord,getLayer());
  if ( (pPartnerEntity==NULL) || (getSex()==SEX_FEMALE) )
  {
    // No partner: small frustration
    pleasureRate = -failureFrustrationFactor;  // -1
  }
  else
  {
    if ( (m_pGenome->checkSpecieCompatibility(pPartnerEntity->getGenome()) == true)
      && pPartnerEntity->isAlive() && (pPartnerEntity->getSex() == SEX_FEMALE) && pPartnerEntity->isLocalAutoControlled() )
    {
      if ( ((CAnimMammal*)pPartnerEntity)->reproductWith(this) )
      {
        pleasureRate = getLibidoRate() + successSatisfactionFactor;  // +10
        changeLibidoRate(-20.0);
        // Reenforce partner recognition
        getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pPartnerEntity, IDENTIFICATION_SEX_PARTNER);
      }
      else
      {
        // Female is not fecond, but not so bad...
        pleasureRate = getLibidoRate();  //
        changeLibidoRate(-20.0);
      }
    }
    else
    {
      // Not valid partner: med frustration
      pleasureRate = -failureFrustrationFactor;  // -1
      changeHealthRate(-0.1); // It hurts !
    }
  }
  
  changePleasureRate(pleasureRate);
  
  changeTirednessRate(2);
  
  if (pleasureRate<0)
    return (false);
  else
    return (true); 
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::reproductWith
//  
// DESCRIPTION:  Action of reproduction for a mammal
// 
// ARGUMENTS:    CAnimMammal* partner : Ptr on the female partner
//   
// RETURN VALUE: bool : false if an error occured
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CAnimMammal::reproductWith(CAnimMammal* partner)
{
  // TBD: Check also if reproduction age is reached
  if ((getGestationBabyNumber()!=0) || (getLibidoRate()<50))
  {
    return (false);
  }
 
  // Chose a number between 0 to 10
  int childNb = getRandInt(10);
  // If out of range, give nominal value
  if ( (childNb>getGestationMaxNumber()) || (childNb<getGestationMinNumber()) )
  {
    childNb = getGestationNominalNumber();
  }
  setGestationBabyNumber(childNb);

  CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
  CYBIOCORE_LOG("MAMMAL - Reproduction : %d babies %s expected by %s with %s\n", 
                 childNb, getSpecieName().c_str(), getLabel().c_str(), partner->getLabel().c_str());

  CBasicEntity* pGestationChild = NULL;
  deleteAllGestationChilds();
  for(int i=0; i<childNb; i++)
  {
    string childLabel;
    childLabel = FormatString("j%d", m_TotalChildNumber);
    childLabel = m_Label + childLabel;
    pGestationChild = new CAnimMammal(childLabel,*this,*partner);
    m_TotalChildNumber++;
    // Caracter from genetic after mutation:
    pGestationChild->setEntityFromGenome(m_pBiotop->getRadioactivityRate());
    m_tGestationChilds.push_back(pGestationChild);
  }
  // Pleasure for female
  changePleasureRate( getLibidoRate() );
  
  // Stop sexual pheromon and libido
  changeLibidoRate(-100.0);
  setPheromone(PHEROMONE_NONE);
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::deliverAllBabies
//  
// DESCRIPTION:  Action of delivering for a pregnant mammal female
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CAnimMammal::deliverAllBabies()
{
  int nbChildsAlive = 0;
  CBasicEntity* pGestationChild = NULL;

  if (m_pBiotop!=NULL)
  {
    for (size_t i = 0; i < m_tGestationChilds.size(); i++)
    {
      pGestationChild = m_tGestationChilds[i];
      if (pGestationChild != NULL)
      {
        int xOfset = getRandInt(2) - 1;
        int yOfset = getRandInt(2) - 1;
        Point_t newCoord = {getGridCoord().x + xOfset, getGridCoord().y + yOfset};
        pGestationChild->changeWeight(0.2); // ensure weight is more than min
        entityIdType resuId = m_pBiotop->addEntity(pGestationChild, newCoord, getLayer()); 
        if (resuId == ENTITY_ID_INVALID)
        {
          delete (pGestationChild);
        }
        else
        {
          nbChildsAlive++;
        }
      } 
    }
    m_tGestationChilds.clear();

    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("MAMMAL - Reproduction : %d babies %s born alive from mother %s\n", 
                  nbChildsAlive, getSpecieName().c_str(), getLabel().c_str());
  }
  setGestationBabyNumber(0);
  getParameter(m_id_GestationTime)->forceVal(0);
  // Set maternal pheromon reset libido
  changeLibidoRate(-100.0);
  setPheromone(PHEROMONE_MATERNAL);
  setAttribute(PHY_ATTRIBUTE_UDDER);
  
  if (nbChildsAlive>0)
  {
    getBrain()->SetHomePurposePositionInGeoMap();
  }

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimMammal::checkBabyNumberAndAdjustGestationChildsTable
//  
// DESCRIPTION:  Check the coherency between GestationNumber parameter and m_tGestationChilds
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CAnimMammal::checkCoherenceAndAdjustBabyNumberParam()
{
  int nbBabies = getGestationBabyNumber();
  if ((nbBabies == 0) && (m_tGestationChilds.size() > 0))
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("MAMMAL - Reproduction : %s gestation childs deleted but not delivered\n", getLabel().c_str());
    deleteAllGestationChilds();
    return false;
  }
  else if (nbBabies != m_tGestationChilds.size())
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("MAMMAL - Reproduction : %s gestation baby number forced to fit to gestation table\n", getLabel().c_str());
    setGestationBabyNumber(m_tGestationChilds.size());
    return false;
  }
  return true;
}


//===========================================================================
// Get / Set for attributes
//===========================================================================

int CAnimMammal::getGestationMaxNumber()
{
  return ((int)getParameter(m_id_GestationNumberRange)->getMax());
}

int CAnimMammal::getGestationMinNumber()
{
  return ((int)getParameter(m_id_GestationNumberRange)->getMin());
}

int CAnimMammal::getGestationNominalNumber()
{
  return (max((int)getParameter(m_id_GestationNumberRange)->getValNominal(), 1));
}

int CAnimMammal::getGestationBabyNumber()
{
  return ((int)getParameter(m_id_GestationNumberRange)->getVal());
}

void CAnimMammal::setGestationBabyNumber(int nbBaby)
{
  getParameter(m_id_GestationNumberRange)->forceVal(nbBaby);
}


