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
// FILE: CAnimal.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a thinking and moving entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CAnimal.h"
#include "CBiotop.h" 
#include "CBrainAnimal.h"
#include "Definitions.h"
#include "CPhysicalWelfare.h"

//===========================================================================
// Definitions 
//===========================================================================
const char* BrainBehaviorTypeNameList[BRAIN_BEHAVIOR_NUMBER_TYPE] =
{
  "None",
  "Baby stay home"
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimal::CAnimal
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
CAnimal::CAnimal(string label, Point_t initCoord, size_t layer, CGenome* pGenome)
{ 
  // Input values
  jumpToGridCoord(initCoord, layer);
  m_pGenome         = pGenome;
  m_pBrain          = new CBrainAnimal(this);
  m_pPhysicWelfare  = new CPhysicalWelfare(this);

  // Default values          
  m_Status      = STATUS_ALIVE; 
  
  m_Label       = label;
  m_Generation	= 0;
  m_TotalChildNumber = 0;
  m_HeadDirection = 0;

  // Parameter id pre-init
  m_id_Age              = invalidIndex; 
  m_id_Decomposition    = invalidIndex; 
  m_id_ReproductionRate = invalidIndex; 
  m_id_Health           = invalidIndex;
  m_id_Hunger           = invalidIndex;
  m_id_Thirst           = invalidIndex;
  m_id_StomachFilling   = invalidIndex;
  m_id_Libido           = invalidIndex;
  m_id_Suffering        = invalidIndex;
  m_id_Pleasure         = invalidIndex;
  m_id_Tiredness        = invalidIndex;
  m_id_GrowthSpeed      = invalidIndex;
  m_id_FatWeight        = invalidIndex;
  m_id_AttackFactor     = invalidIndex;
  m_id_Curiosity        = invalidIndex;
  m_id_Learning         = invalidIndex;
  m_id_CurrentSpeed     = invalidIndex;
  m_id_Fear             = invalidIndex;
  m_id_Vigilance        = invalidIndex;

  m_BusySecondCounter = 2;
  m_bIsSleeping = false;
  m_pFeelingFear = new CFeelingFear(this);
  for (size_t i=0; i<TASTE_NUMBER_TYPE; i++)
    m_tTasteLevel[i] = 0;

  m_forbidenActionInd = invalidIndex;
  m_forbidenActionCount = 0;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::CAnimal
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CAnimal& model : Single parent constructor for cloning
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------    
CAnimal::CAnimal(string label, CAnimal& model)
{ 
  m_Label         = label;
  // inherited
  jumpToGridCoord(model.getGridCoord(), true,  model.getLayer());
  m_Generation		= model.m_Generation + 1;
  m_pGenome       = new CGenome(*model.m_pGenome);
  // reset
  m_pBrain          = new CBrainAnimal(this);
  m_pPhysicWelfare  = new CPhysicalWelfare(this);
  m_Status          = STATUS_ALIVE; 
  m_TotalChildNumber = 0;
  m_HeadDirection = 0;

  // Parameter id pre-init
  m_id_Age              = invalidIndex; 
  m_id_Decomposition    = invalidIndex; 
  m_id_ReproductionRate = invalidIndex; 
  m_id_Health           = invalidIndex;
  m_id_Hunger           = invalidIndex;
  m_id_Thirst           = invalidIndex;
  m_id_StomachFilling   = invalidIndex;
  m_id_Libido           = invalidIndex;
  m_id_Suffering        = invalidIndex;
  m_id_Pleasure         = invalidIndex;
  m_id_Tiredness        = invalidIndex;
  m_id_GrowthSpeed      = invalidIndex;
  m_id_FatWeight        = invalidIndex;
  m_id_AttackFactor     = invalidIndex;
  m_id_Curiosity        = invalidIndex;
  m_id_Learning         = invalidIndex;
  m_id_CurrentSpeed     = invalidIndex;
  m_id_Fear             = invalidIndex;
  m_id_Vigilance        = invalidIndex;

  m_BusySecondCounter = 2;
  m_bIsSleeping = false;
  m_pFeelingFear = new CFeelingFear(this);
  for (size_t i=0; i<TASTE_NUMBER_TYPE; i++)
    m_tTasteLevel[i] = 0;

  m_forbidenActionInd = invalidIndex;
  m_forbidenActionCount = 0;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::CAnimal
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CAnimal& mother,father : Parents for sexual reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CAnimal::CAnimal(string label, CAnimal& mother,CAnimal& father)
{ 
  m_Label         = label;
  // inherited
  jumpToGridCoord(mother.getGridCoord(), true, mother.getLayer()); 
  m_Generation		= mother.m_Generation + 1;
  m_pGenome       = new CGenome(*mother.m_pGenome, *father.m_pGenome, 1.0); // Fred: default crossover rate 1%
  // reset
  m_pPhysicWelfare  = new CPhysicalWelfare(this);
  m_pBrain          = new CBrainAnimal(this);
  m_Status          = STATUS_ALIVE; 
  m_TotalChildNumber = 0;
  m_HeadDirection = 0;

  // Parameter id pre-init
  m_id_Age              = invalidIndex; 
  m_id_Decomposition    = invalidIndex; 
  m_id_ReproductionRate = invalidIndex; 
  m_id_Health           = invalidIndex;
  m_id_Hunger           = invalidIndex;
  m_id_Thirst           = invalidIndex;
  m_id_StomachFilling   = invalidIndex;
  m_id_Libido           = invalidIndex;
  m_id_Suffering        = invalidIndex;
  m_id_Pleasure         = invalidIndex;
  m_id_Tiredness        = invalidIndex;
  m_id_GrowthSpeed      = invalidIndex;
  m_id_FatWeight        = invalidIndex;
  m_id_AttackFactor     = invalidIndex;
  m_id_Curiosity        = invalidIndex;
  m_id_Learning         = invalidIndex;
  m_id_CurrentSpeed     = invalidIndex;
  m_id_Fear             = invalidIndex;
  m_id_Vigilance        = invalidIndex;

  m_BusySecondCounter = 2;
  m_bIsSleeping = false;
  m_pFeelingFear = new CFeelingFear(this);
  for (size_t i=0; i<TASTE_NUMBER_TYPE; i++)
    m_tTasteLevel[i] = 0;

  m_forbidenActionInd = invalidIndex;
  m_forbidenActionCount = 0;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::~CAnimal
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
CAnimal::~CAnimal()
{
  if (m_pFeelingFear != NULL)
    delete m_pFeelingFear;
}

//===========================================================================
// Genetic settings
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimal::setParamFromGene 
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
bool CAnimal::setParamFromGene (CGene* pGen)
{
  if (CBasicEntity::setParamFromGene (pGen) == true)
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
  case GENE_PARAM_AGE:
    {
      if (m_id_Age != invalidIndex) delete(getParameter(m_id_Age)); // delete if already set
      m_id_Age = addParameterFromGene(pGen, PARAM_DURATION);
      resu = true;
      break;
    }
  case GENE_PARAM_DECOMPOSITION:
    {
      if (m_id_Decomposition != invalidIndex) delete(getParameter(m_id_Decomposition)); // delete if already set
      m_id_Decomposition = addParameterFromGene(pGen, PARAM_DURATION);
      resu = true;
      break;
    }
  case GENE_PARAM_REPRO_RATE:
    {
      if (m_id_ReproductionRate != invalidIndex) delete(getParameter(m_id_ReproductionRate)); // delete if already set
      m_id_ReproductionRate = addParameterFromGene(pGen, PARAM_REPRODUCTION);
      resu = true;
      break;
    }
  case GENE_PARAM_HEALTH:
    {
      if (m_id_Health != invalidIndex) delete(getParameter(m_id_Health)); // delete if already set
      m_id_Health = addParameterFromGene(pGen, PARAM_FEELING);
      resu = true;
      break;
    }
  case GENE_PARAM_GROWTH_SPEED:
    {
      if (m_id_GrowthSpeed != invalidIndex) delete(getParameter(m_id_GrowthSpeed)); // delete if already set
      m_id_GrowthSpeed = addParameterFromGene(pGen, PARAM_PHYSIC);
      resu = true;
      break;
    }
  case GENE_PARAM_FAT_WEIGHT:
    {
      if (m_id_FatWeight != invalidIndex) delete(getParameter(m_id_FatWeight)); // delete if already set
      m_id_FatWeight = addParameterFromGene(pGen, PARAM_PHYSIC);
      resu = true;
      break;
    }
  case GENE_PARAM_ATTACK_FACTOR:
    {
      if (m_id_AttackFactor != invalidIndex) delete(getParameter(m_id_AttackFactor)); // delete if already set
      m_id_AttackFactor = addParameterFromGene(pGen, PARAM_PHYSIC);
      resu = true;
      break;
    }
  case GENE_PARAM_SPEED:
    {
      if (m_id_CurrentSpeed != invalidIndex) delete(getParameter(m_id_CurrentSpeed)); // delete if already set
      m_id_CurrentSpeed = addParameterFromGene(pGen, PARAM_PHYSIC);
      resu = true;
      break;
    }
  case GENE_PARAM_CURIOSITY:
    {
      if (m_id_Curiosity != invalidIndex) delete(getParameter(m_id_Curiosity)); // delete if already set
      m_id_Curiosity = addParameterFromGene(pGen, PARAM_BEHAVIOR);
      resu = true;
      break;
    }    
  case GENE_PARAM_LEARNING:
    {
      if (m_id_Learning != invalidIndex) delete(getParameter(m_id_Learning)); // delete if already set
      m_id_Learning = addParameterFromGene(pGen, PARAM_BEHAVIOR);
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
  // If resu is false, param is not valid for CAnimal, but it may be used by inherited class !
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::completeParamsWithDefault 
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
bool CAnimal::completeParamsWithDefault()
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value

  // CAnimal specific
  if (m_id_Protection == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,10,10,100,"Protection factor",PARAM_PHYSIC,GENE_PARAM_PROTECTION);
    m_id_Protection       = addParameter(pParam);
  }

  // In base class
  CBasicEntity::completeParamsWithDefault();

  // CAnimal specific
  if (m_id_Age == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,100,"Age",PARAM_DURATION,GENE_PARAM_AGE);
    m_id_Age              = addParameter(pParam);
  }
  if (m_id_Decomposition == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,5,"Decomposition",PARAM_DURATION,GENE_PARAM_DECOMPOSITION);
    m_id_Decomposition    = addParameter(pParam);
  }
  if (m_id_ReproductionRate == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,10,10,100,"Reproduction rate",PARAM_REPRODUCTION,GENE_PARAM_REPRO_RATE);
    m_id_ReproductionRate = addParameter(pParam);
  } 
  if (m_id_Health == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,100,100,100,"Health rate",PARAM_FEELING,GENE_PARAM_HEALTH);
    m_id_Health = addParameter(pParam);
  } 
  if (m_id_GrowthSpeed == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,100,100,1000,"Growth speed rate",PARAM_PHYSIC,GENE_PARAM_GROWTH_SPEED);
    m_id_GrowthSpeed = addParameter(pParam);
  } 
  if (m_id_Hunger == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,20,0,100,"Hunger rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Hunger = addParameter(pParam);
  } 
  if (m_id_Thirst == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,20,0,100,"Thirst rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Thirst = addParameter(pParam);
  } 
  if (m_id_StomachFilling == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,20,0,100,"Stomach filling",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_StomachFilling = addParameter(pParam);
  } 
  if (m_id_Libido == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,100,"Libido rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Libido = addParameter(pParam);
  } 
  if (m_id_Suffering == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,10,10,100,"Suffering rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Suffering = addParameter(pParam);
  }
  if (m_id_Pleasure == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,50,50,100,"Pleasure rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Pleasure = addParameter(pParam);
  }
  if (m_id_Tiredness == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,10,10,100,"Tiredness rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Tiredness = addParameter(pParam);
  }
  if (m_id_CurrentSpeed == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,10,200,"Speed",PARAM_PHYSIC,GENE_PARAM_SPEED);
    m_id_CurrentSpeed = addParameter(pParam);
  }
  if (m_id_FatWeight == invalidIndex)
  {
    double fatWeight = getWeight()/10;
    double fatMax = getMaxWeight()*0.9; // fat is max 90 of total weight
    CGenericParam* pParam = new CGenericParam(0,fatWeight,fatWeight,fatMax,"Fat weight",PARAM_PHYSIC,GENE_PARAM_FAT_WEIGHT);
    m_id_FatWeight = addParameter(pParam);
  } 
  if (m_id_AttackFactor == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,10,10,100,"Attack factor",PARAM_PHYSIC,GENE_PARAM_ATTACK_FACTOR);
    m_id_AttackFactor = addParameter(pParam);
  } 
  if (m_id_Curiosity == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,10,10,100,"Curiosity rate",PARAM_BEHAVIOR,GENE_PARAM_CURIOSITY);
    m_id_Curiosity = addParameter(pParam);
  }
  if (m_id_Learning == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,100,100,100,"Learning rate",PARAM_BEHAVIOR,GENE_PARAM_LEARNING);
    m_id_Learning = addParameter(pParam);
  } 

  if (m_id_Fear == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,100,"Fear rate",PARAM_FEELING, GENE_PARAM_UNKNOWN);
    m_id_Fear = addParameter(pParam);
  } 

  if (m_id_Vigilance == invalidIndex)
  {
    CGenericParam* pParam = new CGenericParam(0, 100, 100, 100, "Vigilance rate", PARAM_BEHAVIOR, GENE_PARAM_UNKNOWN);
    m_id_Vigilance = addParameter(pParam);
  }

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::setPhysicWelfareFromGene 
//  
// DESCRIPTION:  Set a Physic Welfare sensitivity according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not for physical welfare
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimal::setPhysicWelfareFromGene (CGene* pGen)
{
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_PHY_WELFARE))
  {
    return (false);
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<4*sizeof(WORD))
  {
    // not enought data to config param
    return (false);
  }

  double sensitivity = pGen->getElementValue(0);
  double min = pGen->getElementValue(1);
  double nominal = pGen->getElementValue(2);
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
      m_pPhysicWelfare->SetTemperatureSensitivity(min - 50.0, max - 50.0, sensitivity);
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
// METHOD:       CAnimal::completePhysicWelfareWithDefault 
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
bool CAnimal::completePhysicWelfareWithDefault(void)
{
  if (m_pPhysicWelfare == NULL)
    return (false);

  if (m_pPhysicWelfare->IsHabitatSensSet() == false)
  {
    m_pPhysicWelfare->SetHabitatSensitivity(1.0);
  }

  if (m_pPhysicWelfare->IsTemperSensSet() == false)
  {
    m_pPhysicWelfare->SetTemperatureSensitivity(5.0, 35.0, 1.0);
  }

  // TBD: temporary. must be confirmed
  m_pPhysicWelfare->SetRecoveryBonus(0.02);

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getTemporarySensorFromGene 
//  
// DESCRIPTION:  Get a temporary CSensor built according to Gene
// 
// ARGUMENTS:    CGene* pGen : Sensor gene containing at least 5 Words
//   
// RETURN VALUE: CSensor* : Temporary CSensor that should be delete if not used
//                          NULL if failure on parsing gene
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
CSensor* CAnimal::getTemporarySensorFromGene (CGene* pGen)
{
  if (CBasicEntity::getTemporarySensorFromGene (pGen) != NULL)
  {
    // The parameter has already been taken into account by basic entity
    return (NULL);
  }
  
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_SENSOR))
  {
    return (NULL);
  }
  // We are sure Gene is a sensor
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<4*sizeof(WORD))
  {
    // not enought data to config sensor
    return (NULL);
  }

  CSensor* pSensor = NULL;
  std::vector<double> tWeight;
  size_t nbWeight = len/sizeof(WORD) - 4;
  // Set Weight table
  tWeight.resize(nbWeight);
  for (size_t i = 0; i < nbWeight; i++)
  {
    tWeight[i] = pGen->computeWeightFromData(pData[4+i]);
  }

  int scaledVal1{ 0 }, scaledVal2{ 0 }, scaledVal3{ 0 }, scaledVal4{ 0 };
  size_t numParam = pGen->getNumElements();
  if (numParam > 0) scaledVal1 = pGen->getElementRoundValue(0);
  if (numParam > 1) scaledVal2 = pGen->getElementRoundValue(1);
  if (numParam > 2) scaledVal3 = pGen->getElementRoundValue(2);
  if (numParam > 3) scaledVal4 = pGen->getElementRoundValue(3);

  switch(pGen->getGeneSubType())
  {
  case GENE_SENS_TACTILE:
    {
      if (nbWeight == (size_t)(scaledVal2 - scaledVal1 + 1))
      {
        pSensor = new CSensorTactile((CBrainAnimal*)m_pBrain, tWeight, scaledVal1, scaledVal2);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_VIEW:
    {
      if (nbWeight == (size_t)(scaledVal4 * VIEW_SIZE_PER_FOCUS))
      {
        pSensor = new CSensorView((CBrainAnimal*)m_pBrain, tWeight, scaledVal1, scaledVal2, (ViewAngleType_e)scaledVal3, scaledVal4);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_SMELL:
    {
      if ( nbWeight == NUMBER_ODORS )
      {
        pSensor = new CSensorSmell((CBrainAnimal*)m_pBrain, tWeight, scaledVal1);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_PHEROMONE:
    {
      if ( nbWeight == NUMBER_PHEROMONES )
      {
        pSensor = new CSensorPheromone((CBrainAnimal*)m_pBrain, tWeight, scaledVal1);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_HORMONE:
    {
      if ( nbWeight == NUMBER_HORMONES )
      {
        pSensor = new CSensorHormone((CBrainAnimal*)m_pBrain, tWeight);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_EAR:
    {
      if ( nbWeight==8 )
      {
        pSensor = new CSensorEar((CBrainAnimal*)m_pBrain, tWeight, scaledVal1);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_HUNGER:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorHunger((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_THIRST:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorThirst((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_STOMACH_LOAD:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorStomachOverload((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }  
  case GENE_SENS_PLEASURE:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorPleasure((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_SUFFER:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorSuffering((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_TIREDNESS:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorTiredness((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    } 
  case GENE_SENS_LIBIDO:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorLibido((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_SPEED:
    {
      if (nbWeight==NUMBER_SPEED_SUB_CAPTORS)
      {
        pSensor = new CSensorSpeed((CBrainAnimal*)m_pBrain, tWeight);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_OBSCURITY:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorObscurity((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_VIEW_FAR:
    {
      if ( nbWeight==(cybio_round(scaledVal4)*VIEW_SIZE_PER_FOCUS) )
      {
        pSensor = new CSensorViewFar((CBrainAnimal*)m_pBrain, tWeight, scaledVal1, scaledVal2, (ViewAngleType_e)scaledVal3, scaledVal4);
        resu = true;
      }
      break;
    }
  case GENE_SENS_COMPASS:
    {
      if ( nbWeight==8 )
      {
        pSensor = new CSensorCompass((CBrainAnimal*)m_pBrain, tWeight);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_TASTE:
    {
      if ( nbWeight == NUMBER_TASTES )
      {
        pSensor = new CSensorTaste((CBrainAnimal*)m_pBrain, tWeight);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_FEAR:
    {
      if (nbWeight==1)
      {
        pSensor = new CSensorFear((CBrainAnimal*)m_pBrain, tWeight[0]);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_TEMPERATURE:
    {
      if (nbWeight==NUMBER_TEMPERATURE_SUB_CAPTORS)
      {
        pSensor = new CSensorTemperature((CBrainAnimal*)m_pBrain, tWeight);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_COMPOSITE:
    {
      if (nbWeight>0)
      {
        DWORD sensUid = (DWORD)scaledVal1 * 65536 + (DWORD)scaledVal2; //TODO : should use getElementRawValue but need to swap rawData first
        pSensor = new CSensorComposite((CBrainAnimal*)m_pBrain, tWeight, nbWeight, sensUid, scaledVal3);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_ORIENTATION:
    {
      if (nbWeight==NUMBER_ORIENTATION_SUB_CAPTORS)
      {
        pSensor = new CSensorOrientation((CBrainAnimal*)m_pBrain, tWeight);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }

  case GENE_SENS_VIEW_IDENTIFY:
    {
      if ( nbWeight==(cybio_round(scaledVal4)*VIEW_IDENTIFY_SIZE_PER_FOCUS) )
      {
        pSensor = new CSensorViewIdentify((CBrainAnimal*)m_pBrain, tWeight, scaledVal1, scaledVal2, (ViewAngleType_e)scaledVal3, scaledVal4);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  case GENE_SENS_VIEW_IDENT_FAR:
    {
      if ( nbWeight==(cybio_round(scaledVal4)*VIEW_IDENTIFY_SIZE_PER_FOCUS) )
      {
        pSensor = new CSensorViewIdentifyFar((CBrainAnimal*)m_pBrain, tWeight, scaledVal1, scaledVal2, (ViewAngleType_e)scaledVal3, scaledVal4);
        resu = true;
      }
      else
      {
        // Format error
        resu = false;
      }
      break;
    }
  default:
    {
      // Unknown
      resu = false;
      break;
    }
  }

  // If resu is false, sensor is not valid for animaly, but it may be used by inherited class !
  return (pSensor);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::getExpectedBrainSensorWeightSize 
//  
// DESCRIPTION:  Get the expected size of weight required for a gen used in
//               setBrainSensorFromGene.
// 
// ARGUMENTS:    CGene* pGen : Sensor gene containing at least 5 Words
//   
// RETURN VALUE: int : required weight size to accept gene in setBrainSensor
//                     0 mean pGen not a valid sensor gene
//  
// REMARKS:       
//---------------------------------------------------------------------------
size_t  CAnimal::getExpectedBrainSensorWeightSize (CGene* pGen)
{
  size_t nbWeight = CBasicEntity::getExpectedBrainSensorWeightSize (pGen);

  if ( nbWeight > 0)
  {
    // The parameter has already been taken into account by basic entity
    return (nbWeight);
  }
  
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_SENSOR))
  {
    // error in pGen
    return (0);
  }

  // We are sure Gene is a sensor
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<4*sizeof(WORD))
  {
    // not enought data to config sensor
    return (0);
  }

  int scaledVal1{ 0 }, scaledVal2{ 0 }, scaledVal3{ 0 }, scaledVal4{ 0 };
  size_t numParam = pGen->getNumElements();
  if (numParam > 0) scaledVal1 = pGen->getElementRoundValue(0);
  if (numParam > 1) scaledVal2 = pGen->getElementRoundValue(1);
  if (numParam > 2) scaledVal3 = pGen->getElementRoundValue(2);
  if (numParam > 3) scaledVal4 = pGen->getElementRoundValue(3);

  switch(pGen->getGeneSubType())
  {
  case GENE_SENS_TACTILE:
    {
      nbWeight = (size_t)(scaledVal2 - scaledVal1 + 1);
      break;
    }
  case GENE_SENS_VIEW:
  case GENE_SENS_VIEW_FAR:
    {
      nbWeight = (size_t)scaledVal4 * VIEW_SIZE_PER_FOCUS;
      break;
    }
  case GENE_SENS_SMELL:
    {
      nbWeight = NUMBER_ODORS;
      break;
    }
  case GENE_SENS_PHEROMONE:
    {
      nbWeight = NUMBER_PHEROMONES;
      break;
    }
  case GENE_SENS_HORMONE:
    {
      nbWeight = NUMBER_HORMONES;
      break;
    }
  case GENE_SENS_EAR:
  case GENE_SENS_COMPASS:
    {
      nbWeight = 8;
      break;
    }
  case GENE_SENS_HUNGER:
  case GENE_SENS_THIRST:
  case GENE_SENS_STOMACH_LOAD:
  case GENE_SENS_PLEASURE:
  case GENE_SENS_SUFFER:
  case GENE_SENS_TIREDNESS:
  case GENE_SENS_LIBIDO:
  case GENE_SENS_OBSCURITY:
  case GENE_SENS_FEAR:
    {
      nbWeight = 1;
      break;
    }
  case GENE_SENS_TEMPERATURE:
    {
      nbWeight = NUMBER_TEMPERATURE_SUB_CAPTORS;
      break;
    }
  case GENE_SENS_SPEED:
    {
      nbWeight = NUMBER_SPEED_SUB_CAPTORS;
      break;
    }
  case GENE_SENS_TASTE:
    {
      nbWeight = NUMBER_TASTES;
      break;
    }
  case GENE_SENS_COMPOSITE:
    {
      DWORD sensUid = (DWORD)(pData[0])*65536 + pData[1]; //TODO : should use getElementRawValue but need to swap rawData first
      CSensor* pSens = m_pBrain->GetSensorByUniqueId(sensUid);
      if (pSens != NULL)
        nbWeight = pSens->GetSubCaptorNumber();
      else
        nbWeight = 1;
      break;
    }
  case GENE_SENS_ORIENTATION:
    {
      nbWeight = NUMBER_ORIENTATION_SUB_CAPTORS;
      break;
    }
  case GENE_SENS_VIEW_IDENTIFY:
  case GENE_SENS_VIEW_IDENT_FAR:
    {
      nbWeight = (size_t)scaledVal4 * VIEW_IDENTIFY_SIZE_PER_FOCUS;
      break;
    }
  default:
    {
      // Unknown
      nbWeight = 0;
      break;
    }
  }
  // Last verification
  if (nbWeight == invalidIndex)
    nbWeight = 0;

  // If resu is false, sensor is not valid for animaly, but it may be used by inherited class !
  return (nbWeight);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::setBrainReactionFromGene 
//  
// DESCRIPTION:  Set a reaction attached to brain according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity reaction
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CAnimal::setBrainReactionFromGene (CGene* pGen)
{
  if (CBasicEntity::setBrainReactionFromGene (pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return (true);
  }
  
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_REACTION))
  {
    return (false);
  }
  // We are sure Gene is a reaction
  bool resu = false;
  size_t len = pGen->getData().size();
  if (len<4*sizeof(WORD))
  {
    // not enought data to config reaction
    return (false);
  }

  CReaction* pReaction = NULL;
  double success,failure;
  success  = pGen->getElementValue(0);
  failure  = pGen->getElementValue(1);

  switch(pGen->getGeneSubType())
  {
  case GENE_REACT_NOTHING:
    {
      pReaction = new CReactionNothing((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_SLEEP:
    {
      pReaction = new CReactionSleep((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_MOVEFORWARD:
    {
      pReaction = new CReactionVarySpeed((CBrainAnimal*)m_pBrain,success,failure, pGen->getElementRoundValue(2));
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_STEPBACKWARD:
    {
      pReaction = new CReactionStepBackward((CBrainAnimal*)m_pBrain,success,failure, pGen->getElementRoundValue(2));
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_TURNLEFT:
    {
      pReaction = new CReactionTurnLeft((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_TURNRIGHT:
    {
      pReaction = new CReactionTurnRight((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_EAT:
    {
      pReaction = new CReactionEat((CBrainAnimal*)m_pBrain,success,failure, pGen->getElementRoundValue(2));
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_DRINK:
    {
      pReaction = new CReactionDrink((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_COPULATE:
    {
      pReaction = new CReactionCopulate((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_ATTACK:
    {
      pReaction = new CReactionAttack((CBrainAnimal*)m_pBrain,success,failure, pGen->getElementRoundValue(2));
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_TURNHEADLEFT:
    {
      pReaction = new CReactionTurnHeadLeft((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_TURNHEADRIGHT:
    {
      pReaction = new CReactionTurnHeadRight((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_RUMINATE:
    {
      pReaction = new CReactionRuminate((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
      resu = true;
      break;
    }
  case GENE_REACT_HIDE:
    {
      pReaction = new CReactionHide((CBrainAnimal*)m_pBrain,success,failure);
      m_pBrain->AttachReaction(pReaction);
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
  // If resu is false, reaction is not valid for animal, but it may be used by inherited class !
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::completeBrainSensorWithDefault 
//  
// DESCRIPTION:  Complete BrainReaction if unset by genome 
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimal::completeBrainSensorWithDefault(void)
{

  CSensor* pSens = NULL;
  pSens = new CSensorMemoryAction((CBrainAnimal*)m_pBrain,10.0);
  if (pSens!=NULL)
	  m_pBrain->AttachSensor(pSens); 
  pSens = new CSensorMemoryFeedback((CBrainAnimal*)m_pBrain,50.0);
  if (pSens!=NULL)
    m_pBrain->AttachSensor(pSens);
  pSens = new CSensorStatic((CBrainAnimal*)m_pBrain,30.0);
  if (pSens!=NULL)
    m_pBrain->AttachSensor(pSens);
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::completeBrainReactionWithDefault 
//  
// DESCRIPTION:  Complete BrainReaction if unset by genome 
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimal::completeBrainReactionWithDefault(void)
{
  if (m_pBrain->GetNumberReaction() == 0)
  {
    CReaction* pReaction = new CReactionNothing((CBrainAnimal*)m_pBrain,0,0);
    m_pBrain->AttachReaction(pReaction);    
  }

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::setBrainSizeFromGene 
//  
// DESCRIPTION:  Resize Brain initial matrix according to brain Gene
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity Brain Instinct
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CAnimal::setBrainSizeFromGene (CGene* pGen)
{
  if (CBasicEntity::setBrainSizeFromGene (pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return (true);
  }
  
  if ( (pGen==NULL) || (pGen->getGeneType()!=GENE_BRAIN_SIZE) )
  {
    return (false);
  }

  // We are sure Gene is a brain size gene
  bool resu = false;
  size_t len = pGen->getData().size();
  if (len<1*sizeof(WORD))
  {
    // not enought data to config brain
    return (false);
  }

  int brainSize = pGen->getElementRoundValue(0);

  switch(pGen->getGeneSubType())
  {
  case GENE_BRAIN_SIZE_TERRITORY:
    {
      m_pBrain->SetGeoMapSize(brainSize);
      resu = true;
      break;
    }
  case GENE_BRAIN_SIZE_HIST_IN:
    {
      m_pBrain->SetNumberInputHistory(brainSize);
      resu = true;
      break;
    }
  case GENE_BRAIN_SIZE_HIST_EXP:
    {
      m_pBrain->SetNumberExperienceHistory(brainSize);
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
// METHOD:       CAnimal::setBrainInstinctFromGene 
//  
// DESCRIPTION:  Build Brain initial matrix according to brain Gene
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity Brain Instinct
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CAnimal::setBrainInstinctFromGene (CGene* pGen)
{
  if (CBasicEntity::setBrainInstinctFromGene (pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return (true);
  }
  
  if ((pGen==NULL) || ((pGen->getGeneSubType() != GENE_BRAIN_LINE) && (pGen->getGeneSubType() != GENE_BRAIN_IDENTIFY_LINE)))
  {
    return (false);
  }

  // We are sure Gene is a Brain line
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size() * sizeof(BYTE) / sizeof(WORD) - 1;
  // Process Line Id
  GeneSubTypeBrainLine_e subType = (GeneSubTypeBrainLine_e)pGen->getGeneSubType();
  int lineId = pGen->getElementRoundValue(0);
  // Build Line
  if (subType == GENE_BRAIN_LINE)
  {
    resu = m_pBrain->GetDecisionNeuronTable()->buildNeuronLineFromRawData(lineId,len,pData+1);
  }
  else if (subType == GENE_BRAIN_IDENTIFY_LINE)
  {
    resu = m_pBrain->GetIdentifyNeuronTable()->buildNeuronLineFromRawData(lineId,len,pData+1);
  }
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setBrainConfigFromGene 
//  
// DESCRIPTION:  Set Brain caracteristics according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity caracter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimal::setBrainConfigFromGene (CGene* pGen)
{
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_BRAIN_CONFIG))
  {
    return (false);
  }
  // We are sure Gene is a caracteristic
  bool resu = false;
  size_t len = pGen->getData().size();
  // For the moment, only size 1 is supported
  if (len < 1)
  {
    return (false);
  }

  switch(pGen->getGeneSubType())
  {
  case GENE_BRAIN_BEHAVIOR:
    if ((BrainBehaviorType_e)pGen->getElementRoundValue(0) == BRAIN_BEHAVIOR_BABY_STAY_HOME)
      m_pBrain->SetBabyStayHome(true);
    resu = true;
    break;
   default:
    // Unknown
    resu = false;
    break;
  }
  // If resu is false, caract is not valid for basic entity, but it may be used by inherited class !
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::completeBrainInstinctWithDefault 
//  
// DESCRIPTION:  Complete Brain matrix unset by genome with default values.
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CAnimal::completeBrainInstinctWithDefault(void)
{
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::setFeelingFromGene 
//  
// DESCRIPTION:  Set a Feeling Welfare sensitivity according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a feeling
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimal::setFeelingFromGene (CGene* pGen)
{
  if ((m_pBrain==NULL)||(pGen==NULL)||(pGen->getGeneType() != GENE_FEELING))
  {
    return (false);
  }
  // We are sure Gene is a Feeling
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if ( len < (sizeof(DWORD)+sizeof(WORD)) )
  {
    // not enought data to config param
    return (false);
  }

  // Get associated Sensor
  DWORD sensUid = pGen->getElementRawValue(0);
  CSensor* pSens = m_pBrain->GetSensorByUniqueId(sensUid);
  if ( pSens == NULL )
  {
    return (false);
  }

  GeneSubTypeFeeling_e subType = (GeneSubTypeFeeling_e)pGen->getGeneSubType();

  switch(subType)
  {
  case GENE_FEEL_WELFARE:
    {
      size_t lenSensiTable = len / sizeof(WORD) - 2;
      double* pSensiTable = new double[lenSensiTable];
      for (size_t i=0; i<lenSensiTable; i++)
        pSensiTable[i] = pGen->computeSensitivityFromData(pData[2+i]);

      resu = m_pBrain->AddFeelingWelfareSensitivity(pSens,lenSensiTable,pSensiTable);
      if (resu == false)
        delete [] pSensiTable;

      break;
    }
  case GENE_FEEL_FEAR:
    {
      size_t lenSensiTable = len / sizeof(WORD) - 2;
      double* pSensiTable = new double[lenSensiTable];
      for (size_t i=0; i<lenSensiTable; i++)
        pSensiTable[i] = pGen->computeSensitivityFromData(pData[2+i]);

      resu = m_pFeelingFear->AddSensitivity(pSens,lenSensiTable,pSensiTable);
      if (resu == false)
        delete [] pSensiTable;

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
// METHOD:       CAnimal::setPurposeFromGene 
//  
// DESCRIPTION:  Set a new purpose according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not supported
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CAnimal::setPurposeFromGene (CGene* pGen)
{
  if ((m_pBrain==NULL)||(pGen==NULL)||(pGen->getGeneType() != GENE_PURPOSE))
  {
    return (false);
  }

  // We are sure Gene is a purpose
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();

  GeneSubTypePurpose_e subType = (GeneSubTypePurpose_e)pGen->getGeneSubType();

  switch(subType)
  {
  case GENE_PURPOSE_TRIGGER_UP:
  case GENE_PURPOSE_TRIGGER_DOWN:
    {
      if ( len < (sizeof(DWORD)+4*sizeof(WORD)) )
      {
        // not enought data to config param
        return (false);
      }
      // Get associated Sensor
      DWORD sensUid = pGen->getElementRawValue(4);
      CSensor* pSens = m_pBrain->GetSensorByUniqueId(sensUid);
      if ( pSens == NULL )
      {
        return (false);
      }
      int duration           = pGen->getElementRoundValue(0);
      int subCaptorIndex     = pGen->getElementRoundValue(1);
      int startRateThreshold = pGen->getElementRoundValue(2);
      int stopRateThreshold  = pGen->getElementRoundValue(3);
      char* pLabel = (char*)pData + 12;

      PurposeTriggerType_e trigType = PURPOSE_TRIG_UP;
      if (subType == GENE_PURPOSE_TRIGGER_DOWN)
        trigType = PURPOSE_TRIG_DOWN;

      CPurpose* pPurpose = new CPurpose(pLabel, duration, pSens, subCaptorIndex, startRateThreshold, stopRateThreshold, trigType);
      if ( pPurpose != NULL )
      {
        m_pBrain->AttachPurpose(pPurpose);
        resu = true;
      }  
      break;
    }
  case GENE_PURPOSE_SENSOR:
    {
      if ( len < (2*sizeof(DWORD)) )
      {
        // not enought data to config param
        return (false);
      }
      DWORD purposeUid = pGen->getElementRawValue(0);
      DWORD sensUid = pGen->getElementRawValue(1);
      CPurpose* pPurpose = m_pBrain->GetPurposeByUniqueId(purposeUid);
      CSensor* pSensor   = m_pBrain->GetSensorByUniqueId(sensUid);

      if ( (pPurpose == NULL) || (pSensor == NULL) )
      {
        // Bad data to config param
        return (false);
      }

      size_t  bonusTableSize = (len - (2*sizeof(DWORD))) / sizeof(WORD);
      std::vector<int> bonusTable(bonusTableSize);
      for (int index=0; index<bonusTableSize; index++)
      {
        bonusTable[index] = pGen->computeBonusFromData(pData[4+index]);
      }
      resu = pPurpose->AddSensorBonus(pSensor, bonusTable);
      break;
    }
  case GENE_PURPOSE_REACTION:
    {
      if ( len < (2*sizeof(DWORD)+sizeof(WORD)) )
      {
        // not enought data to config param
        return (false);
      }
      DWORD purposeUid = pGen->getElementRawValue(0);
      DWORD reactionUid = pGen->getElementRawValue(1);
      CPurpose* pPurpose = m_pBrain->GetPurposeByUniqueId(purposeUid);
      CReaction* pReaction = m_pBrain->GetReactionByUniqueId(reactionUid);
      int bonus = pGen->getElementRoundValue(2);
      if (pPurpose && pReaction)
        resu = pPurpose->AddReactionBonus(pReaction,bonus);
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
// METHOD:       CAnimal::setBrainInstinctInGenes 
//  
// DESCRIPTION:  Build Brain Instinct from current Brain (aquired kwnoledge genetic transmition)
// 
// ARGUMENTS:    
//   
// RETURN VALUE: bool : error
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CAnimal::setBrainInstinctInGenes(void) // Idee FRED: Deplacer dans CGenome...
{
  getGenome()->setBrainIdentifyInGenes(getBrain());
  getGenome()->setBrainInstinctInGenes(getBrain());
  return true;
}


//===========================================================================
// Genetic description
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildPhysicWellfareString 
//  
// DESCRIPTION:  Give info string on Physic Wellfare interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildPhysicWellfareString(CGene* pGen)
{
  string welfareStr = CBasicEntity::buildPhysicWellfareString(pGen);
  string tempStr;
  if (welfareStr != STRING_GENE_UNUSED)
  {
    // The gene has already been taken into account by basic entity
    return (welfareStr);
  }

  if ((pGen==NULL)||(pGen->getGeneType() != GENE_PHY_WELFARE))
  {
    return (welfareStr);
  }
  // We are sure Gene is a parameter
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if ((len < sizeof(WORD)) || (pGen->getNumElements() < 4))
  {
    // not enought data to config param
    return (welfareStr);
  }

  double sensitivity = pGen->getElementValue(0);
  double scaledVal1 = pGen->getElementValue(1);
  double scaledVal2 = pGen->getElementValue(2);
  double scaledVal3 = pGen->getElementValue(3);

  switch(pGen->getGeneSubType())
  {
  case GENE_PHYS_SENS_HABITAT:
    {
      welfareStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.2f %%", sensitivity);
      welfareStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
  case GENE_PHYS_SENS_TEMPER:
    {
      welfareStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.2f %%", sensitivity);
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
      // keep STRING_GENE_UNUSED
      break;
    }
  }

  return(welfareStr);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildSensorString 
//  
// DESCRIPTION:  Give info string on Sensor interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildSensorString(CGene* pGen)
{
  string paramStr = CBasicEntity::buildSensorString(pGen);
  string tempStr;
  if (paramStr != STRING_GENE_UNUSED)
  {
    // The parameter has already been taken into account by basic entity
    return (paramStr);
  }

  if ((pGen==NULL)||(pGen->getGeneType() != GENE_SENSOR))
  {
    return (paramStr);
  }
  // We are sure Gene is a parameter
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<4*sizeof(WORD))
  {
    // not enought data to config param
    return (paramStr);
  }
 
  int scaledVal1{ 0 }, scaledVal2{ 0 }, scaledVal3{ 0 }, scaledVal4{ 0 };
  size_t numParam = pGen->getNumElements();
  if (numParam > 0) scaledVal1 = pGen->getElementRoundValue(0);
  if (numParam > 1) scaledVal2 = pGen->getElementRoundValue(1);
  if (numParam > 2) scaledVal3 = pGen->getElementRoundValue(2);
  if (numParam > 3) scaledVal4 = pGen->getElementRoundValue(3);

  switch(pGen->getGeneSubType())
  {
  case GENE_SENS_TACTILE:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%d ", scaledVal1 );
      paramStr += pGen->getElementStrName(0) + tempStr;
      tempStr = FormatString("=%d ", scaledVal2);
      paramStr += pGen->getElementStrName(1) + tempStr;
      break;
    }
  case GENE_SENS_VIEW:
  case GENE_SENS_VIEW_FAR:
  case GENE_SENS_VIEW_IDENTIFY:
  case GENE_SENS_VIEW_IDENT_FAR:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%d ", scaledVal1);
      paramStr += pGen->getElementStrName(0) + tempStr;
      // Fred TBD : use angle string
      tempStr = FormatString("=%d ", scaledVal2);
      paramStr += pGen->getElementStrName(1) + tempStr;
      tempStr = FormatString("=%d ", scaledVal3);
      paramStr += pGen->getElementStrName(2) + tempStr;
      tempStr = FormatString("=%d ", scaledVal4);
      paramStr += pGen->getElementStrName(3) + tempStr;
      break;
    }
  case GENE_SENS_SMELL:
  case GENE_SENS_PHEROMONE:
  case GENE_SENS_EAR:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%d ", scaledVal1);
      paramStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
 
  case GENE_SENS_HUNGER:
  case GENE_SENS_THIRST:
  case GENE_SENS_STOMACH_LOAD:
  case GENE_SENS_PLEASURE:
  case GENE_SENS_SUFFER:
  case GENE_SENS_TIREDNESS:
  case GENE_SENS_LIBIDO:
  case GENE_SENS_SPEED:
  case GENE_SENS_OBSCURITY:
  case GENE_SENS_COMPASS:
  case GENE_SENS_TASTE:
  case GENE_SENS_FEAR:
  case GENE_SENS_TEMPERATURE:
  case GENE_SENS_ORIENTATION:
  case GENE_SENS_HORMONE:
    {
      paramStr = pGen->getLabel();
      break;
    }
  case GENE_SENS_COMPOSITE:
    {
      DWORD sensUid = (DWORD)scaledVal1 * 65536 + (DWORD)scaledVal2; //TODO : should use getElementRawValue but need to swap rawData first 
      CSensor* pSens = m_pBrain->GetSensorByUniqueId(sensUid);
      CGenericParam* pParam = getParameter(scaledVal3);
      paramStr = pGen->getLabel();

      if (pParam!=NULL)
        paramStr = paramStr + " " + pParam->getLabel();
      else
        paramStr = paramStr + " Bad param ";

      if (pSens!=NULL)
        paramStr = paramStr + " x " + pSens->GetLabel();
      else
        paramStr = paramStr + " x Bad sensor ";

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

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildReactionString 
//  
// DESCRIPTION:  Give info string on Reaction interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildReactionString(CGene* pGen)
{
  string paramStr = CBasicEntity::buildSensorString(pGen);
  string tempStr;
  if (paramStr != STRING_GENE_UNUSED)
  {
    // The parameter has already been taken into account by basic entity
    return (paramStr);
  }

  if ((pGen==NULL)||(pGen->getGeneType() != GENE_REACTION))
  {
    return (paramStr);
  }
  // We are sure Gene is a parameter
  size_t len = pGen->getData().size();
  if (len<4*sizeof(WORD))
  {
    // not enought data to config param
    return (paramStr);
  }

  switch(pGen->getGeneSubType())
  {
  case GENE_REACT_NOTHING:
  case GENE_REACT_SLEEP:
  case GENE_REACT_STEPBACKWARD:
  case GENE_REACT_TURNLEFT:
  case GENE_REACT_TURNRIGHT:
  case GENE_REACT_DRINK:
  case GENE_REACT_COPULATE:
  case GENE_REACT_TURNHEADLEFT:
  case GENE_REACT_TURNHEADRIGHT:
  case GENE_REACT_RUMINATE:
  case GENE_REACT_HIDE:
  case GENE_REACT_EAT:
  case GENE_REACT_ATTACK:
  case GENE_REACT_MOVEFORWARD:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%d ", pGen->getElementRoundValue(0));
      paramStr += pGen->getElementStrName(0) + tempStr;
      tempStr = FormatString("=%d ", pGen->getElementRoundValue(1));
      paramStr += pGen->getElementStrName(1) + tempStr;
      if (pGen->getNumElements() > 2)
      {
        tempStr = FormatString("=%d ", pGen->getElementRoundValue(2));
        paramStr += pGen->getElementStrName(2) + tempStr;
      }
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

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildBrainSizeString 
//  
// DESCRIPTION:  Give info string on Brain Size interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildBrainSizeString(CGene* pGen)
{
  string brainStr = CBasicEntity::buildPhysicWellfareString(pGen);
  string tempStr;
  if (brainStr != STRING_GENE_UNUSED)
  {
    // The gene has already been taken into account by basic entity
    return (brainStr);
  }

  if ( (pGen==NULL) || (pGen->getGeneType()!=GENE_BRAIN_SIZE) )
  {
    return (brainStr);
  }

  // We are sure Gene is a brain size gene
  size_t len = pGen->getData().size();
  if (len<1*sizeof(WORD))
  {
    // not enought data to config brain
    return (brainStr);
  }

  int brainSize = pGen->getElementRoundValue(0);

  switch(pGen->getGeneSubType())
  {
  case GENE_BRAIN_SIZE_TERRITORY:
  case GENE_BRAIN_SIZE_HIST_IN:
  case GENE_BRAIN_SIZE_HIST_EXP:
    {
      brainStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%d", brainSize );
      brainStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
  default:
    {
      // keep STRING_GENE_UNUSED
      break;
    }
  }

  return(brainStr);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildBrainInstinct 
//  
// DESCRIPTION:  Give info string on Brain Instinct interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildBrainInstinctString(CGene* pGen)
{
  string brainStr = CBasicEntity::buildBrainInstinctString(pGen);
  string tempStr;
  if (brainStr != STRING_GENE_UNUSED)
  {
    // The gene has already been taken into account by basic entity
    return (brainStr);
  }

  // We are sure Gene is a brain size gene
  size_t len = pGen->getData().size();
  if (len<1*sizeof(WORD))
  {
    // not enought data to config brain
    return (brainStr);
  }

  switch(pGen->getGeneSubType())
  {
  case GENE_BRAIN_LINE:
  case GENE_BRAIN_IDENTIFY_LINE:
    {
      brainStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%d", pGen->getElementRoundValue(0));
      brainStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
  default:
    {
      // keep STRING_GENE_UNUSED
      break;
    }
  }

  return(brainStr);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildBrainConfigString 
//  
// DESCRIPTION:  Give info string on Brain Instinct interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildBrainConfigString(CGene* pGen)
{
  string caractStr = STRING_GENE_UNUSED;
  string tempStr;

  if ((pGen==NULL)||(pGen->getGeneType() != GENE_BRAIN_CONFIG))
  {
    return (caractStr);
  }
  // We are sure Gene is a caracteristic
  size_t len = pGen->getData().size();
  if (len<1)
  {
    return (caractStr);
  }

  BYTE data1 = pGen->getElementRoundValue(0);

  switch (pGen->getGeneSubType())
  {
  case GENE_BRAIN_BEHAVIOR:
    {
      if (data1 < BRAIN_BEHAVIOR_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += BrainBehaviorTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  default:
    {
      // keep STRING_GENE_UNUSED
      break;
    }
  }

  return (caractStr);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildFeelingWellfareString 
//  
// DESCRIPTION:  Give info string on Feeling Wellfare interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildFeelingWellfareString(CGene* pGen)
{
  string welfareStr = CBasicEntity::buildPhysicWellfareString(pGen);
  string tempStr;
  if (welfareStr != STRING_GENE_UNUSED)
  {
    // The gene has already been taken into account by basic entity
    return (welfareStr);
  }

  if ((m_pBrain==NULL)||(pGen==NULL)||(pGen->getGeneType() != GENE_FEELING))
  {
    return (welfareStr);
  }
  // We are sure Gene is a parameter
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if ( len < (sizeof(DWORD)+sizeof(WORD)) )
  {
    // not enought data to config param
    return (welfareStr);
  }

  // Get associated Sensor
  DWORD sensUid = pGen->getElementRawValue(0);
  CSensor* pSens = m_pBrain->GetSensorByUniqueId(sensUid);
  if ( pSens == NULL )
  {
    return (welfareStr);
  }

  GeneSubTypeFeeling_e subType = (GeneSubTypeFeeling_e)pGen->getGeneSubType();

  switch(subType)
  {
  case GENE_FEEL_WELFARE:
  case GENE_FEEL_FEAR:
    {
      welfareStr = pGen->getLabel() + " : ";
      welfareStr += pGen->getElementStrName(0) + pSens->GetLabel();
      break;
    }
  default:
    {
      // keep STRING_GENE_UNUSED
      break;
    }
  }

  return(welfareStr);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::buildPurposeString 
//  
// DESCRIPTION:  Give info string on Purpose interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CAnimal::buildPurposeString(CGene* pGen)
{
  string purposeStr = CBasicEntity::buildPurposeString(pGen);
  string tempStr;
  if (purposeStr != STRING_GENE_UNUSED)
  {
    // The gene has already been taken into account by basic entity
    return (purposeStr);
  }

  if ((m_pBrain==NULL)||(pGen==NULL)||(pGen->getGeneType() != GENE_PURPOSE))
  {
    return (purposeStr);
  }
  // We are sure Gene is a parameter
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();

  GeneSubTypePurpose_e subType = (GeneSubTypePurpose_e)pGen->getGeneSubType();

  switch(subType)
  {
  case GENE_PURPOSE_TRIGGER_UP:
  case GENE_PURPOSE_TRIGGER_DOWN:
    {
      if ( len < (sizeof(DWORD)+4*sizeof(WORD)) )
        break;
      // Get associated Sensor
      DWORD sensUid = pGen->getElementRawValue(4);
      CSensor* pSens = m_pBrain->GetSensorByUniqueId(sensUid);
      if ( pSens == NULL )
        break;
      int duration           = pGen->getElementRoundValue(0);
      int subCaptorIndex     = pGen->getElementRoundValue(1);
      int startRateThreshold = pGen->getElementRoundValue(2);
      int stopRateThreshold  = pGen->getElementRoundValue(3);
      char* pLabel = (char*)pData + 12;

      tempStr = " <"; tempStr += pLabel; tempStr += ">";
      purposeStr = pGen->getLabel() + tempStr + " on " + pSens->GetLabel() + " ";
      purposeStr += pSens->GetSubCaptorLabel(subCaptorIndex) + " : ";     
      tempStr = FormatString("=%d ", duration );
      purposeStr += pGen->getElementStrName(0) + tempStr;
      tempStr = FormatString("=%d ", startRateThreshold );
      purposeStr += pGen->getElementStrName(2) + tempStr;
      tempStr = FormatString("=%d ", stopRateThreshold );
      purposeStr += pGen->getElementStrName(3) + tempStr;
      break;
    }
  case GENE_PURPOSE_SENSOR:
    {
      if ( len < (2*sizeof(DWORD)) )
        break;

      DWORD purposeUid = pGen->getElementRawValue(0);
      DWORD sensUid = pGen->getElementRawValue(1);
      CPurpose* pPurpose = m_pBrain->GetPurposeByUniqueId(purposeUid);
      CSensor*  pSensor = m_pBrain->GetSensorByUniqueId(sensUid);
      if (pPurpose && pSensor)
        purposeStr = pGen->getLabel() + " for " + pPurpose->GetLabel() + " on " + pSensor->GetLabel();
      break;
    }
  case GENE_PURPOSE_REACTION:
    {
      if ( len < (2*sizeof(DWORD)+sizeof(WORD)) )
        break;

      DWORD purposeUid = pGen->getElementRawValue(0);
      DWORD reactionUid = pGen->getElementRawValue(1);
      CPurpose* pPurpose = m_pBrain->GetPurposeByUniqueId(purposeUid);
      CReaction* pReaction = m_pBrain->GetReactionByUniqueId(reactionUid);
      int bonus = pGen->getElementRoundValue(2);
      tempStr = FormatString("=%d ", bonus );
      if (pPurpose && pReaction)
        purposeStr = pGen->getLabel() + " for " + pPurpose->GetLabel() + " on " + pReaction->GetLabel() + pGen->getElementStrName(2) + tempStr;
      break;
    }
  default:
    {
      // Unknown
      break;
    }
  }

  return(purposeStr);
}

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimal::nextSecond
//  
// DESCRIPTION:  Basic action trigger: Action / reaction / memorisation
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed highly impacted by this method
//---------------------------------------------------------------------------
void CAnimal::nextSecond() 
{
  if (isAlive() && isLocalAutoControlled())
  {
    int currentSpeed = getCurrentSpeed();

    // Speed management step1 (inertia on initial speed)
    if (currentSpeed>0)
      ExecuteMoveForwardAction(0,0,currentSpeed/4);
    else if (currentSpeed<0)
      ExecuteMoveBackwardAction(0,0,currentSpeed/4);

    // Taste
    for (size_t i=0; i<TASTE_NUMBER_TYPE; i++)
      m_tTasteLevel[i] = m_tTasteLevel[i]/2;

    bool resu;
    choiceIndType myChoice;
    ReactionIntensityType_e myIntensity;
    feedbackValType myFeedback;
    size_t prevReactIndex = m_pBrain->GetCurrentReactionIndex();

    resu = m_pBrain->PollAllSensors();	
    if (resu && !isUserControlled())
    {
      // Fear
      getParameter(m_id_Fear)->setVal(m_pFeelingFear->ComputeAndGetFeelingFear());
      // Brain process
      m_pBrain->NextSecond();   
      if (m_BusySecondCounter<1)
      {
        m_pBrain->HistorizeInput();
        myChoice = (choiceIndType)m_pBrain->ComputeAndGetDecision(getCuriosityRate(), myIntensity);
        if (myChoice != invalidIndex)
        {
          m_pBrain->HistorizeDecision(myChoice);
          myFeedback = m_pBrain->ComputeFeedback(myChoice, myIntensity);
          // if action is forbiden, systematically give max negative feedback
          if (myChoice == m_forbidenActionInd)
          {
            myFeedback = -MAX_FEEDBACK_VAL;
            m_forbidenActionCount++;
          }
          resu = m_pBrain->MemorizeExperience(myFeedback,getLearningRate());
          m_BusySecondCounter = m_pBrain->GetReactionByIndex(m_pBrain->GetCurrentReactionIndex())->GetUnitTimeInSecond();
        }
      }
      else
      {
        doNothing();
      }
    }

    // if reaction change, set HasChanged flag
    if (m_pBrain->GetCurrentReactionIndex() != prevReactIndex)
    {
      m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_BEHAVIOR_CHANGE, this);
    }

    // Speed management step2 (new speed)
    currentSpeed = getCurrentSpeed();
    if (currentSpeed>0)
      ExecuteMoveForwardAction(0,0,3*currentSpeed/4);
    else if (currentSpeed<0)
      ExecuteMoveBackwardAction(0,0,3*currentSpeed/4);

    m_BusySecondCounter--;

    // Compute health according to physical welfare
    double healthVar = m_pPhysicWelfare->ComputeAndGetHealthVariation();
    if ((healthVar!=0) && (getParameter(m_id_Health)->getVal()>0) && (changeHealthRate(healthVar) == false))
    {
      if (checkVitalNeedsOk())
      {
        logDeathCause(FormatString("due to bad physical welfare (coordx %d coordy %d)\n", getGridCoord().x, getGridCoord().y));
      }
      else
      {
        logDeathCause(FormatString("due to vital needs (coordx %d coordy %d)\n", getGridCoord().x, getGridCoord().y));
      }
    }

    // Decraese slowly Pleasure, suffer, Tiredness
    convergePleasureRateToNeutral(0.1);
    changeSufferingRate(-0.1);
    changeTirednessRate(-0.2);
  }
  else
  {
    doNothing();
  }
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::nextHour
//  
// DESCRIPTION:  Secondary action: parameters update and checkup
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CAnimal::nextHour() 
{
  if (isAlive() && !isRemoteControlled())
  {
    if (isSleeping())
    {
      changeHungerRate(1);
      changeThirstRate(1);
    }
    else
    {
      if (getConsumeClass() == CONSUM_VEGETAL)
      {
        changeHungerRate(4);
      }
      else
      {
        changeHungerRate(2);
      }
      changeThirstRate(2);
    }
    changeStomachFillingRate(-25);
    // reset injury malus: entity has survived
    m_pPhysicWelfare->SetInjuryMalus(0); 
  }

  CBasicEntity::nextHour();
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::nextDay
//  
// DESCRIPTION:  Living day trigger called by CBasicEntity every 24 hours
// 
// ARGUMENTS:    forceGrowth : can be use to force growth without using ressource (fat)
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CAnimal::nextDay(bool forceGrowth)
{
  if (!isRemoteControlled())
  {
    // get older 
    if (isAlive())
    {
      if (m_pBrain != NULL)
        m_pBrain->NextDay();
      getParameter(m_id_Age)->changeVal(1);
      balanceWeightAndMetabolism(forceGrowth);
      if (getParameter(m_id_Age)->isMaxReached())
      {
        autoKill();
        logDeathCause("due to old age\n");
      }
    }
    else
    {
      if (isDead())
      {
        getParameter(m_id_Decomposition)->changeVal(1);
        changeFatWeight(-0.2);
        if (getParameter(m_id_Decomposition)->isMaxReached())
        {
          autoRemove();
        }
      }
    }
  }
  CBasicEntity::nextDay(forceGrowth);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::getAge
//  
// DESCRIPTION:  Return Age in days
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      
//---------------------------------------------------------------------------
int CAnimal::getAge()
{
  return ((int)getParameter(m_id_Age)->getVal());
}

int CAnimal::getDecompositionTime()
{
  return ((int)getParameter(m_id_Decomposition)->getVal());
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::checkVitalNeedsOk
//  
// DESCRIPTION:  Return true if vital needs are all Ok, 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: true : OK, false Ko
//  
// REMARKS:      Called every second by Physical Welfare
//---------------------------------------------------------------------------
bool CAnimal::checkVitalNeedsOk()
{
  bool resu = true;

  if (getThirstRate()>99.8)
    resu = false;

  if (getFatWeight()<0.02)
    resu = false;

  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::balanceWeightAndMetabolism
//  
// DESCRIPTION:  Balance weight and energy used every 1 day
// 
// ARGUMENTS:    forceGrowth : force growth without using fat
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CAnimal::balanceWeightAndMetabolism(bool forceGrowth)
{
  // Transfer mass from fat to body according to growth speed
  double growthWeight = getParameter(m_id_GrowthSpeed)->getVal()/1000;

  if (forceGrowth)
  {
    changeWeight(growthWeight);
    changeFatWeight(growthWeight/10); // give 10% fat weight
  }
  else
  {
    // Use Fat for growth
    getParameter(m_id_FatWeight)->changeVal(-growthWeight);
    // Use Fat for basal metabolism: consume 1% of weight per day
    changeFatWeight(-getWeight()/100);
  }

}

//===========================================================================
// Parameters management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeHungerRate
//  
// DESCRIPTION:  Make animal more hungry
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeHungerRate(double variation)
{
  getParameter(m_id_Hunger)->changeVal(variation);

  //if (getParameter(m_id_Hunger)->isMaxReached())
  //{
    // Hunger is just an indicator. 
    // Fat rate is the critical ressource for health
  //}
  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeThirstRate
//  
// DESCRIPTION:  Make animal more Thirsty
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeThirstRate(double variation)
{
  getParameter(m_id_Thirst)->changeVal(variation);
  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeStomachFillingRate
//  
// DESCRIPTION:  Fill stomach
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeStomachFillingRate(double variation)
{
  getParameter(m_id_StomachFilling)->changeVal(variation);

  if (getParameter(m_id_StomachFilling)->isMaxReached())
  {
    if (changeHealthRate(-1) == false)
    {
      logDeathCause("due to stomach overload\n");
    }
  }
  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeLibidoRate
//  
// DESCRIPTION:  Change animal libido
// 
// ARGUMENTS:    double variation : rate change 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeLibidoRate(double variation)
{
  getParameter(m_id_Libido)->changeVal(variation);
  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeHealthRate
//  
// DESCRIPTION:  Make animal more Thirsty
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: True: entity is still alive. False: entity dead
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CAnimal::changeHealthRate(double variation, CBasicEntity* pAggresor, bool suffering)
{
  getParameter(m_id_Health)->changeVal(variation);

  if (suffering)
  {
    // By default, suffering amplifies Health variation
    changeSufferingRate(-10*variation);
  }

  if((variation<0) && (pAggresor!=NULL))
  {
    // Reenforce danger recognition
    getBrain()->MemorizeIdentificationExperience(-variation, getLearningRate(), pAggresor, IDENTIFICATION_DANGER);
  }

  if (getParameter(m_id_Health)->isMinReached())
  {
    autoKill();
    return false;
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeSufferingRate
//  
// DESCRIPTION:  double animal suffering
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeSufferingRate(double variation)
{
  getParameter(m_id_Suffering)->changeVal(variation);

  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changePleasureRate
//  
// DESCRIPTION:  Modify animal pleasure
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changePleasureRate(double variation)
{
  getParameter(m_id_Pleasure)->changeVal(variation);

  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::convergePleasureRateToNeutral
//  
// DESCRIPTION:  Modify animal pleasure
// 
// ARGUMENTS:    double variation : rate variation (must be > 0) 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::convergePleasureRateToNeutral(double variation)
{
  CGenericParam* pPleasureParam = getParameter(m_id_Pleasure);
  if (pPleasureParam->getVal() > 50.0)
  {
    if (pPleasureParam->getVal() > (50.0 + variation))
    {
      pPleasureParam->changeVal(-variation);
    }
    else
    {
      pPleasureParam->forceVal(50.0);
    }
  }
  else if (pPleasureParam->getVal() < 50.0)
  {
    if (pPleasureParam->getVal() < (50.0 - variation))
    {
      pPleasureParam->changeVal(variation);
    }
    else
    {
      pPleasureParam->forceVal(50.0);
    }
  }
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeTirednessRate
//  
// DESCRIPTION:  Modify animal Tiredness
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeTirednessRate(double variation)
{
  getParameter(m_id_Tiredness)->changeVal(variation);

  if (getParameter(m_id_Tiredness)->isMaxReached())
  {
    if (changeHealthRate(-1) == false)
    {
      logDeathCause("due to burnout\n");
    }
  }
  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeFatWeight
//  
// DESCRIPTION:  Increase both weight and fat weight in authorized range
// 
// ARGUMENTS:    double variation : mass change
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeFatWeight(double variation)
{
  // Change total weight in autorized range
  double realVariation = changeWeight(variation);

  // Change fat rate
  CGenericParam* pParam = getParameter(m_id_FatWeight);
  pParam->changeVal(realVariation);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeCurrentSpeed
//  
// DESCRIPTION:  Change animal speed
// 
// ARGUMENTS:    double variation : rate change 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeCurrentSpeed(double variation)
{
  CGenericParam* pSpeed = getParameter(m_id_CurrentSpeed);

  pSpeed->changeVal(variation);

  // Reduce max speed for babies and injured animals
  if ((getpPhysicalWelfare()->GetInjuryMalus()>1) || (getCurrentLifeStage()->getStageType()<STAGE_2))
  {
    double maxSpeed = pSpeed->getMax() * 0.7; // 70% of max speed
    if (pSpeed->getVal() > maxSpeed)
      pSpeed->forceVal(maxSpeed);
  }

  // Stop noise if stop speed
  if (pSpeed->getVal() == 0)
    setNoise(0);

  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::forceCurrentSpeed
//  
// DESCRIPTION:  Force animal speed value
// 
// ARGUMENTS:    double newSpeed : new absolute speed 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::forceCurrentSpeed(double newSpeed)
{
  CGenericParam* pSpeed = getParameter(m_id_CurrentSpeed);

  pSpeed->forceVal(newSpeed);

  // Stop noise if stop speed
  if (pSpeed->getVal() == 0)
    setNoise(0);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::stopCurrentSpeed
//  
// DESCRIPTION:  Change animal libido
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::stopCurrentSpeed()
{
  getParameter(m_id_CurrentSpeed)->setVal(0);
  setNoise(0);
  return;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::changeFearRate
//  
// DESCRIPTION:  Modify animal pleasure
// 
// ARGUMENTS:    double variation : rate increase 
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::changeFearRate(double variation)
{
  getParameter(m_id_Fear)->changeVal(variation);

  return;
}

//===========================================================================
// Behavior  
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CAnimal::forceNextAction
//  
// DESCRIPTION:  Basic action trigger: User defined action / reaction / memorisation
// 
// ARGUMENTS:    choiceIndType myChoice: Id of the desired action
//   
// RETURN VALUE: feedbackValType : Feedback result from user action
//  
// REMARKS:      To be used for tutorial phase
//---------------------------------------------------------------------------
feedbackValType CAnimal::forceNextAction(choiceIndType myChoice) 
{
  bool resu;
  choiceIndType autoChoice;
  ReactionIntensityType_e autoIntensity;
  feedbackValType myFeedback = 0;

  if (isAlive())
  {
    int currentSpeed = getCurrentSpeed();
    // Speed management step1
    if (currentSpeed>0)
      ExecuteMoveForwardAction(0,0,currentSpeed/4);
    else if (currentSpeed<0)
      ExecuteMoveBackwardAction(0,0,currentSpeed/4);

    // Taste
    for (size_t i=0; i<TASTE_NUMBER_TYPE; i++)
      m_tTasteLevel[i] = m_tTasteLevel[i]/2;

    resu = m_pBrain->PollAllSensors();	
    if (resu)
    {
      // Fear
      getParameter(m_id_Fear)->setVal(m_pFeelingFear->ComputeAndGetFeelingFear());
      // Brain process
      m_pBrain->NextSecond();   
	    m_pBrain->HistorizeInput();
      autoChoice = (choiceIndType)m_pBrain->ComputeAndGetDecision(getCuriosityRate(), autoIntensity);
      if (myChoice != invalidIndex)
      {
        m_pBrain->HistorizeDecision(myChoice);
        myFeedback = m_pBrain->ComputeFeedback(myChoice, REACTION_INTENSITY_MEDIUM);  // FRED TBD: user define intensity
        m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_BEHAVIOR_CHANGE, this);
        //TRACE("Force Choice:%d  Auto choice:%d Feeback:%f\n",myChoice,autoChoice,myFeedback);
      }
      else
      {
        m_pBrain->HistorizeDecision(autoChoice);
        myFeedback = m_pBrain->ComputeFeedback(autoChoice, autoIntensity);
      }
    }

    // Speed management step2
    currentSpeed = getCurrentSpeed();
    if (currentSpeed>0)
      ExecuteMoveForwardAction(0,0,3*currentSpeed/4);
    else if (currentSpeed<0)
      ExecuteMoveBackwardAction(0,0,3*currentSpeed/4);

    // Compute health according to physical welfare
    double healthVar = m_pPhysicWelfare->ComputeAndGetHealthVariation();
    if (changeHealthRate(healthVar) == false)
    {
      if (checkVitalNeedsOk())
      {
        logDeathCause("due to bad physical welfare\n");
      }
      else
      {
        logDeathCause("due to vital needs\n");
      }
    }

    // Decraese slowly suffer, center slowly pleasure
    convergePleasureRateToNeutral(0.1);
    changeSufferingRate(-0.1);
    changeTirednessRate(-0.2);
  }
  else
  {
    doNothing();
  }
  return (myFeedback);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::predictNextAction
//  
// DESCRIPTION:  Predict action without any memorization
// 
// ARGUMENTS:    choiceIndType myChoice: Id of the desired action
//   
// RETURN VALUE: choiceIndType : Id of the decided action
//  
// REMARKS:      To be used for tutorial phase
//---------------------------------------------------------------------------
choiceIndType CAnimal::predictNextAction()
{
  bool resu;
  choiceIndType autoChoice = invalidIndex;
  ReactionIntensityType_e autoIntensity;

  if (isAlive())
  {
    resu = m_pBrain->PollAllSensors();	
    if (resu)
    {
      autoChoice = (choiceIndType)m_pBrain->ComputeAndGetDecision(getCuriosityRate(), autoIntensity);
    }
  }

  return (autoChoice);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteTurnRightAction
//  
// DESCRIPTION:  Action turn right
// 
// ARGUMENTS:    None    
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteTurnRightAction(unsigned int nDegree)
{
  setNoise(10);
  changeTirednessRate(0.2);
  turnRight(nDegree);
  lookForward();
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteTurnLeftAction
//  
// DESCRIPTION:  Action turn left
// 
// ARGUMENTS:    None    
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteTurnLeftAction(unsigned int nDegree)
{
  setNoise(10);
  changeTirednessRate(0.2);
  turnLeft(nDegree);
  lookForward();
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteMoveForwardAction
//  
// DESCRIPTION:  Action move forward
// 
// ARGUMENTS:    success/failureSatisfactionFactor, : factor for pleasure management 
//               nbSteps : >0 integer. Number of steps forward     
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteMoveForwardAction(double successSatisfactionFactor, double failureFrustrationFactor, int nbSteps)
{
  double pleasureRate = 0;
  CBasicEntity* pWater = NULL;
  double injuryLevel=0;
  // Check if we are on the edge of water
  Point_t curCoord;
  int i,j, totalLayerType=0;
  for (i=-1; i<2; i++)
  {
    for (j=-1; j<2; j++)
    {
        curCoord.x = getGridCoord().x + i;
        curCoord.y = getGridCoord().y + j;
        totalLayerType += m_pBiotop->getLayerType(curCoord,1);
    }
  }
  if (totalLayerType != 9 * m_pBiotop->getLayerType(getGridCoord(),1) )
  {
    turnToCenterDir();      // Center in direction pos to avoid border issue
  }

  if (CBasicEntity::moveLinear(nbSteps))
  {
    // Stop speed in water
    LayerType_e layerType = m_pBiotop->getLayerType(getGridCoord(), 1);
    if ((layerType == LAYER_OVER_WET_GROUND) || (layerType == LAYER_OVER_WATER))
    {
      stopCurrentSpeed();
      pWater = m_pBiotop->findEntity(getGridCoord(),0);
    }

    if (checkHabitat())
    {
      pleasureRate = successSatisfactionFactor;
    }
    else
    {
      pleasureRate = -failureFrustrationFactor;
      if (changeHealthRate(-2.0, pWater) == false)
      {
        logDeathCause("due to water\n");
      }
    }
  }
  else
  {
    moveToGridEdgePos();  // Center in grid pos to avoid corner blocked issue
    turnToCenterDir();      // Center in direction pos to avoid border  issue
    if (CBasicEntity::moveLinear(nbSteps/2))
    {
      if (checkHabitat())
      {
        pleasureRate = successSatisfactionFactor;
      }
      else
      {
        pleasureRate = -failureFrustrationFactor;
        if (changeHealthRate(-2.0, pWater) == false)
        {
          logDeathCause("due to water2\n");
        }  
      }
    }
    else
    {
      pleasureRate = -failureFrustrationFactor;
      stopCurrentSpeed();

      // Find hit entity
      RelativePos_t relPos = {1,0};
      Point_t newCoord = getGridCoordRelative(relPos);
      CBasicEntity* pHitEntity = m_pBiotop->findEntity(newCoord,getLayer());

      if (pHitEntity != NULL)
      {
        // Hurt moving entity
        if (pHitEntity->getProtection() > getProtection())
        {
          injuryLevel = (double)nbSteps / (double)NB_STEPS_PER_GRID_SQUARE*(pHitEntity->getProtection() - getProtection()) / 100.0;
          if (changeHealthRate(-injuryLevel) == false)
          {
            logDeathCause(FormatString("due to run in obstacle %s\n", pHitEntity->getLabel().c_str()));
          }
        }

        // Hurt hit entity
        if (getAttackFactor() > pHitEntity->getProtection())
        {
          if ((pHitEntity->getClass() >= CLASS_ANIMAL_FIRST) && (pHitEntity->isAlive()))
          {
            injuryLevel = (double)nbSteps / (double)NB_STEPS_PER_GRID_SQUARE*(getAttackFactor() - pHitEntity->getProtection()) / 100.0;
            if (((CAnimal*)pHitEntity)->changeHealthRate(-injuryLevel, this) == false)
            {
              ((CAnimal*)pHitEntity)->logDeathCause(FormatString("due to shock by animal %s\n", getLabel().c_str()));
            }
          }
        }
      }
    }  
  }

  changePleasureRate(pleasureRate);
  changeTirednessRate(nbSteps * 0.02);

  if (injuryLevel>0)
    return (false);
  else
    return (true); 
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteMoveBackwardAction
//  
// DESCRIPTION:  Action move forward
// 
// ARGUMENTS:    success/failureSatisfactionFactor, : factor for pleasure management   
//               nbSteps : >0 integer. Number of steps backward      
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteMoveBackwardAction(double successSatisfactionFactor, double failureFrustrationFactor, int nbSteps)
{
  double pleasureRate = 0;

  if (CBasicEntity::moveLinear(-nbSteps))
  {
    if (checkHabitat())
    {
      pleasureRate = successSatisfactionFactor;
    }
    else
    {
      pleasureRate = -failureFrustrationFactor;
      changeHealthRate(-2.0);   
    }
  }
  else
  {
    pleasureRate = -failureFrustrationFactor;
    changeHealthRate(-nbSteps/NB_STEPS_PER_GRID_SQUARE);
    moveToGridEdgePos();
  }

  changePleasureRate(pleasureRate);
  changeTirednessRate(nbSteps * 0.02);

  if (pleasureRate<0)
    return (false);
  else
    return (true); 
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::checkConsumeClass
//  
// DESCRIPTION:  Check if food class match with consume class
// 
// ARGUMENTS:    ClassType_e eatenClass: class of eaten entity
//   
// RETURN VALUE: bool : 1 food compliant / 0 food not compliant 
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CAnimal::checkConsumeClass (ClassType_e eatenClass)
{
  if ( ( (getConsumeClass() == CONSUM_VEGETAL)
       && (eatenClass >= CLASS_VEGETAL_FIRST)
       && (eatenClass <= CLASS_VEGETAL_LAST) )
   ||( (getConsumeClass() == CONSUM_OMNI)
       && (eatenClass >= CLASS_VEGETAL_FIRST)
       && (eatenClass <= CLASS_ANIMAL_LAST) )
   ||( (getConsumeClass() == CONSUM_MEAT)
       && (eatenClass >= CLASS_ANIMAL_FIRST)
       && (eatenClass <= CLASS_ANIMAL_LAST) ) )
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::logDeathCause
//  
// DESCRIPTION:  Display death cause
// 
// ARGUMENTS:    string deathCauseStr: string that will follow "is dead ". End with "\n"
//   
// RETURN VALUE: None 
//  
// REMARKS:      
//---------------------------------------------------------------------------
void CAnimal::logDeathCause(std::string deathCauseStr)
{
  if (m_pBiotop != NULL)
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("ANIMAL - Death : specie %s name %s is dead ", getSpecieName().c_str(), getLabel().c_str());
    CYBIOCORE_LOG(deathCauseStr.c_str());
  }
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteEatAction
//  
// DESCRIPTION:  Action other entity
// 
// ARGUMENTS:    int relLayer: {-m_nbLayer..m_nbLayer} - relative layer to perform eat
//               success/failureSatisfactionFactor, : factor for pleasure management
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteEatAction(int relLayer, double successSatisfactionFactor, double failureFrustrationFactor)
{
  double pleasureRate = 0;
  RelativePos_t relPos = {1,0};
  double eatenWeight = 0;
  double initialWeight = 0;
  ClassType_e eatenClass;

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
    if (pEatenEntity->isComestible())
    {
      eatenClass = pEatenEntity->getClass();
      // If pEatenEntity was still alive, it will not survive...
      if ( (eatenClass >= CLASS_ANIMAL_FIRST) && (eatenClass <= CLASS_ANIMAL_LAST) )
      {
        pEatenEntity->autoKill();
      }

      // Taste
      m_tTasteLevel[pEatenEntity->getTaste()] = (int)MAX_SENSOR_VAL;

      initialWeight = getWeight();
      // Each attempt, try to eat 0.1% of own weight 
      eatenWeight = - pEatenEntity->changeWeight(-initialWeight/1000.0);

      double toxicityRate = pEatenEntity->getToxicity();

      if (toxicityRate>0.01)
      {
        // If toxic, be injured
        if (changeHealthRate(-toxicityRate) == false)
        {
          logDeathCause(FormatString("due to poison when eating %s\n", pEatenEntity->getLabel().c_str()));
        }
      }

      if ( checkConsumeClass(eatenClass) )
      {
        // Enjoy the lunch
        // TBD Pleasure should depend of taste 
        pleasureRate = (getHungerRate() - toxicityRate) * successSatisfactionFactor / 100.0; 
        // Reenforce food recognition
        getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pEatenEntity, IDENTIFICATION_FOOD);
      

        if ( (eatenClass >= CLASS_VEGETAL_FIRST) && (eatenClass <= CLASS_VEGETAL_LAST) )
        {
          // 10% of this eaten food is digested:
          changeFatWeight(eatenWeight/10.0);
          changeHungerRate(-0.4);
          changeThirstRate(-0.1);
          changeStomachFillingRate(0.5);
        }
        else if (eatenClass >= CLASS_ANIMAL_FIRST)
        {
          // Meat contains more calories
          // 50% of this eaten food is digested:
          changeFatWeight(eatenWeight/2.0);
          changeHungerRate(-0.5);
          changeThirstRate(-0.1);
          changeStomachFillingRate(0.5);

          // If too much meet is consumed, eaten animal becomes carrion,
          if (pEatenEntity->getWeight() < (pEatenEntity->getMinWeight()*1.1))
          {
            (CAnimal*)pEatenEntity->setCurrentLifeStages(STAGE_6);
          }
        }
      }
      else
      {
        // It is not the expected food        
        pleasureRate = (getHungerRate()-80.0) / 2 - toxicityRate; // Under 80%, no benefits
        changeHungerRate(-0.1);
        changeThirstRate(-0.1);
        // No possible digestion
        // Reenforce food recognition
        getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pEatenEntity, IDENTIFICATION_FOOD);
      }
    }
    else
    {
      // Too strong to be eaten: med frustration
      pleasureRate = -failureFrustrationFactor;
      // Reenforce food recognition
      getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pEatenEntity, IDENTIFICATION_FOOD);
    }
  }

  changePleasureRate(pleasureRate);

  if (pleasureRate<0)
    return (false);
  else
    return (true); 
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::drinkAndGetSatisfactionRate
//  
// DESCRIPTION:  Action drink 
// 
// ARGUMENTS:    success/failureSatisfactionFactor, : factor for pleasure management
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteDrinkAction(double successSatisfactionFactor, double failureFrustrationFactor)
{
  double pleasureRate = 0;
  RelativePos_t relPos = {1,0};

  moveToGridEdgePos();
  Point_t newCoord = getGridCoordRelative(relPos);

  CBasicEntity* pDrunkEntity = m_pBiotop->findEntity(newCoord,1);
  if ( (pDrunkEntity!=NULL) && (pDrunkEntity->isDrinkable()==true) )// water !
  {
    pDrunkEntity->changeWeight(-getWeight()/1000.0);

    // satisfaction
    pleasureRate = getThirstRate() * successSatisfactionFactor / 100.0; // Average between thirst and succes factor
    changeThirstRate(-1);
    changeStomachFillingRate(0.5);
    // Reenforce drink recognition
    getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pDrunkEntity, IDENTIFICATION_DRINK);
  }
  else
  {
    // Nothing to drink: frustration
    pleasureRate = -failureFrustrationFactor;
    // Reenforce drink recognition
    getBrain()->MemorizeIdentificationExperience(pleasureRate, getLearningRate(), pDrunkEntity, IDENTIFICATION_DRINK);
  }

  changePleasureRate(pleasureRate);

  if (pleasureRate<0)
    return (false);
  else
    return (true); 
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteCopulateAction
//  
// DESCRIPTION:  Action copulate
// 
// ARGUMENTS:    success/failureSatisfactionFactor, : factor for pleasure management
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteCopulateAction(double successSatisfactionFactor, double failureFrustrationFactor)
{
  // Reproduction is class specific
  // Has to be inherited
  return (false);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteAttackAction
//  
// DESCRIPTION:  Action attack other entity
// 
// ARGUMENTS:    int relLayer: {-m_nbLayer..m_nbLayer} - relative layer to attack
//               success/failureSatisfactionFactor, : factor for pleasure management
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteAttackAction(int relLayer, int stepRange, double successSatisfactionFactor, double failureFrustrationFactor, ReactionIntensityType_e intensity)
{
  double pleasureRate = 0;

  // Jump before attack
  CBasicEntity::moveLinear(stepRange);
  RelativePos_t relPos = {1,0};
  double weightFactor;
  int attackScore,defenseScore, diffScore;

  moveToGridEdgePos();
  Point_t newCoord = getGridCoordRelative(relPos);

  CBasicEntity* pAttackedEntity = m_pBiotop->findEntity(newCoord,getLayer()+relLayer);
  if ( (pAttackedEntity==NULL) || ( (pAttackedEntity!=NULL) && (pAttackedEntity->isDrinkable()==true) ) )// Rq: Water (id=0) cannot be attacked
  {
    // Nothing to attack: small frustration
    pleasureRate = -failureFrustrationFactor; // -1
  }
  else
  {
    weightFactor = getWeight() / pAttackedEntity->getWeight();
    attackScore  = (int)(weightFactor*getAttackFactor()) + getRandInt(5);
    defenseScore = (int)(pAttackedEntity->getProtection()) + getRandInt(5);

    // Pleasure to fight
    pleasureRate = successSatisfactionFactor; // +3

    diffScore = attackScore - defenseScore;

    if (diffScore > 0)
    {
      // Attack success 
      ClassType_e attackedClass = pAttackedEntity->getClass();
      // If pAttackedEntity is  alive, it will be hurt
      if ( (attackedClass >= CLASS_ANIMAL_FIRST) 
        && (attackedClass <= CLASS_ANIMAL_LAST)
        && pAttackedEntity->isAlive() )
      {
        // For adults allow critical attack
        if ( (diffScore>10) && (getCurrentLifeStage()->getStageType()==STAGE_3) && (getRandInt(4)==1) && (intensity > REACTION_INTENSITY_LOW) )
        {
          diffScore = 8*diffScore;
        }

        if (((CAnimal*)pAttackedEntity)->changeHealthRate(-diffScore) == false)
        {
          ((CAnimal*)pAttackedEntity)->logDeathCause(FormatString("killed by %s\n", getLabel().c_str()));
        }
        // Animal is hurt and can loose health for a duration
        pAttackedEntity->getpPhysicalWelfare()->SetInjuryMalus((double)getRandInt(attackScore-defenseScore)/10.0);
        // Attacked animal is slow down
        ((CAnimal*)pAttackedEntity)->changeCurrentSpeed(-diffScore);

         // Reenforce prey recognition
        getBrain()->MemorizeIdentificationExperience(successSatisfactionFactor/5, getLearningRate(), pAttackedEntity, IDENTIFICATION_PREY);
        // Reenforce predator recognition
        ((CAnimal*)pAttackedEntity)->getBrain()->MemorizeIdentificationExperience(5, getLearningRate(), this, IDENTIFICATION_PREDATOR);
      }
    }
    else
    {
      // Reduce prey recognition
      getBrain()->MemorizeIdentificationExperience(-2, getLearningRate(), pAttackedEntity, IDENTIFICATION_PREY);
      // Attack failure hurts
      if (changeHealthRate((attackScore-defenseScore)/4, pAttackedEntity) == false)  
      {
        logDeathCause(FormatString("while trying to attack %s\n", pAttackedEntity->getLabel().c_str()));
      }
    }
  }

  changePleasureRate(pleasureRate);

  changeTirednessRate(0.2);

  if (pleasureRate<0)
    return (false);
  else
    return (true); 
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::ExecuteSleepAction
//  
// DESCRIPTION:  Action sleep
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CAnimal::ExecuteSleepAction()
{
  doNothing();
  // Set asleep
  m_bIsSleeping = true;
  // bonus
  changeTirednessRate(-0.2);
  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::wakeUp
//  
// DESCRIPTION:  Stop sleeping
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
void CAnimal::wakeUp(void)
{
  lookForward();
  m_bIsSleeping = false;
  setCamouflageToNominalRatio(100);
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::lookForward
//  
// DESCRIPTION:  Stop sleeping
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::lookForward(void)
{
  m_HeadDirection = getDirection();
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::turnHeadLeft
//  
// DESCRIPTION:  Stop sleeping
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::turnHeadLeft(void)
{
  m_HeadDirection = (getDirection()+1)%8;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::turnHeadRight
//  
// DESCRIPTION:  Stop sleeping
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::turnHeadRight(void)
{
  m_HeadDirection = (getDirection()+7)%8;
}

//---------------------------------------------------------------------------
// METHOD:       CAnimal::setForbidenActionInd
//  
// DESCRIPTION:  Set a forbiden action that will be punished systematically
// 
// ARGUMENTS:    choiceIndType actionInd
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CAnimal::setForbidenActionInd(choiceIndType actionInd)
{
  if (actionInd < m_pBrain->GetNumberReaction())
  {
    m_forbidenActionInd = actionInd;
    m_forbidenActionCount = 0;
  }
  else
  {
    m_forbidenActionInd = invalidIndex;
    m_forbidenActionCount = 0;
  }
}

string CAnimal::getForbidenActionLabel()
{
  if (m_forbidenActionInd == invalidIndex)
    return "None";
  else
    return m_pBrain->GetReactionByIndex(m_forbidenActionInd)->GetLabel();
}

int CAnimal::getForbidenActionCount()
{
  return m_forbidenActionCount;
}

bool CAnimal::SetCurrentReactionIndex(size_t reactIndex)
{
  if (m_pBrain->GetCurrentReactionIndex() != reactIndex)
  {
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_BEHAVIOR_CHANGE, this);
    return getBrain()->SetCurrentReactionIndex(reactIndex);
  }
  return true;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================
    
double CAnimal::getReproductionRate() 
{
  return (getParameter(m_id_ReproductionRate)->getVal());
}
    
double CAnimal::getLifeExpectation() 
{
  return (getParameter(m_id_Age)->getMax());
}

double CAnimal::getRotenTimeExpectation() 
{
  return (getParameter(m_id_Decomposition)->getMax());
}

double CAnimal::getHungerRate()
{
  return (getParameter(m_id_Hunger)->getVal());
}

double CAnimal::getThirstRate()
{
  return (getParameter(m_id_Thirst)->getVal());
}

double CAnimal::getHealthRate()
{
  return (getParameter(m_id_Health)->getVal());
}

double CAnimal::getStomachFillingRate()
{
  return (getParameter(m_id_StomachFilling)->getVal());
}

double CAnimal::getLibidoRate()
{
  return (getParameter(m_id_Libido)->getVal());
}

double CAnimal::getSufferingRate()
{
  return (getParameter(m_id_Suffering)->getVal());
}

double CAnimal::getPleasureRate()
{
  return (getParameter(m_id_Pleasure)->getVal());
}

double CAnimal::getTirednessRate()
{
  return (getParameter(m_id_Tiredness)->getVal());
}

double CAnimal::getCuriosityRate()
{
  return (getParameter(m_id_Curiosity)->getVal());
}

double CAnimal::getLearningRate()
{
  return (getParameter(m_id_Learning)->getVal());
}

double CAnimal::getFatWeight()
{
  return (getParameter(m_id_FatWeight)->getVal());
}

void CAnimal::setCuriosityToNominalRatio(double ratio)
{
  double newCuriosity = getParameter(m_id_Curiosity)->getValNominal() * ratio / 100.0;
  getParameter(m_id_Curiosity)->setVal(newCuriosity);
}

void CAnimal::setLearningToNominalRatio(double ratio)
{
  double newLearning = getParameter(m_id_Learning)->getValNominal() * ratio / 100.0;
  getParameter(m_id_Learning)->setVal(newLearning);
}

void CAnimal::setGrowthSpeedToNominalRatio(double ratio)
{
  double newGrowth = getParameter(m_id_GrowthSpeed)->getValNominal() * ratio / 100.0;
  getParameter(m_id_GrowthSpeed)->setVal(newGrowth);
}

void CAnimal::setAttackFactorToNominalRatio(double ratio)
{
  double newAttack = getParameter(m_id_AttackFactor)->getValNominal() * ratio / 100.0;
  getParameter(m_id_AttackFactor)->setVal(newAttack);
}

double CAnimal::getAttackFactor()
{
  return (getParameter(m_id_AttackFactor)->getVal());
}

bool CAnimal::isSleeping()
{
  return (m_bIsSleeping);
}

int CAnimal::getCurrentSpeed()
{
  return ((int)getParameter(m_id_CurrentSpeed)->getVal());
}

int CAnimal::getHeadDirection()
{
  return (m_HeadDirection);
}

int* CAnimal::getpTasteLevelTable()
{
  return (m_tTasteLevel);
}

double CAnimal::getFearRate()
{
  return (getParameter(m_id_Fear)->getVal());
}

double CAnimal::getVigilance()
{
  return (getParameter(m_id_Vigilance)->getVal());
}

void CAnimal::setVigilance(double rate)
{
  getParameter(m_id_Vigilance)->setVal(rate);
}

void CAnimal::setHungerRate(double rate)
{
  getParameter(m_id_Hunger)->setVal(rate);
}

void CAnimal::setThirstRate(double rate)
{
  getParameter(m_id_Thirst)->setVal(rate);
}

void CAnimal::setLibidoRate(double rate)
{
  getParameter(m_id_Libido)->setVal(rate);
}

void CAnimal::setHealthRate(double rate)
{
  getParameter(m_id_Health)->setVal(rate);
}

void CAnimal::setSufferingRate(double rate)
{
  getParameter(m_id_Suffering)->setVal(rate);
}

void CAnimal::setPleasureRate(double rate)
{
  getParameter(m_id_Pleasure)->setVal(rate);
}

void CAnimal::setTirednessRate(double rate)
{
  getParameter(m_id_Tiredness)->setVal(rate);
}

void CAnimal::setFatWeight(double fatWeight)
{
  getParameter(m_id_FatWeight)->setVal(fatWeight);
}

void CAnimal::setFearRate(double rate)
{
  getParameter(m_id_Fear)->setVal(rate);
}

void CAnimal::setStomachFillingRate(double rate)
{
  getParameter(m_id_StomachFilling)->setVal(rate);
}