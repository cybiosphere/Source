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
// FILE: CBasicEntity.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS Represents a basic entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================
 
#include "CBasicEntity.h"
#include "CPhysicalWelfare.h"
#include "CBiotop.h" 
#include "CEntityFactory.h"
#include "CGeoMapPurpose.h"

//===========================================================================
// Definitions            
//===========================================================================

const char* ReproTypeNameList[REPRODUCT_NUMBER_TYPE] =
{
  "None",
  "Clonage",
  "Sexual"
};

const char* ColorTypeNameList[COLOR_CARACTER_NUMBER_TYPE] =
{
  "White",
  "Black",
  "Grey",
  "Red",
  "Blue",
  "Green",
  "Yellow",
  "Orange",
  "Pink",
  "Beige",
  "Brown"
};

const char* OdorTypeNameList[ODOR_NUMBER_TYPE] =
{
  "None",
  "Aromatic",
  "Fragrant",
  "Alliaceous",
  "Ambrosial",
  "Hircine",
  "Repulsive",
  "Nauseous"
};

const char* PheromoneTypeNameList[PHEROMONE_NUMBER_TYPE] =
{
  "None",
  "Sexual male",
  "Sexual female",
  "Maternal",
  "Baby",
  "Dominant"
};

const char* FormTypeNameList[FORM_NUMBER_TYPE] =
{
  "Unknown",
  "Plan smooth",
  "Plan rough",
  "Roundish",
  "Geometric",
  "Vegetal long leef",
  "Vegetal bush",
  "Vegetal tree",
  "Quadriped thin",
  "Quadriped long neck",
  "Quadriped fat",
  "Biped",
  "Carrion"
};

const char* TasteTypeNameList[TASTE_NUMBER_TYPE] =
{
  "None",
  "Sweet",
  "Acid",
  "Salty",
  "Bitter",
  "Umani"
};

const char* HabitatTypeNameList[HABITAT_NUMBER_TYPE] =
{
  "Earth",
  "Amphibian",
  "Water",
  "Air"
};

const char* ConsumeTypeNameList[CONSUM_NUMBER_TYPE] =
{
  "None",
  "Vegetarian",
  "Omnivor",
  "Carnivor"
};

const char* MoveTypeNameList[MOVE_NUMBER_TYPE] =
{
  "None",
  "Walk"
};

const char* TextureTypeNameList[TEXTURE_NUMBER_TYPE] =
{
  "None",
  "Stripped",
  "Spotted",
  "Reflect"
};

const char* PhyAttributeTypeNameList[PHY_ATTRIBUTE_NUMBER_TYPE] =
{
  "None",
  "Udder",
  "Horns",
  "Tusks",
  "Mane"
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::CBasicEntity
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CBasicEntity::CBasicEntity()
{
  // system initialisation
  m_pBiotop         = NULL;
  m_pBrain          = NULL;
  m_pGenome         = NULL;
  m_pPhysicWelfare  = NULL;
  m_tParam.resize(0);
  m_tGestationChilds.resize(0);
  m_tLifeStage.resize(0);
  m_bHasMoved   = true;
  m_indCurrentLifeStage = invalidIndex;
  m_HourCounter = 0;
  m_Id = ENTITY_ID_INVALID;
  m_Direction   = 0;
  m_StepDirection   = 0;
  m_Layer = invalidCoord;
  m_DefaultLayer = invalidCoord;
  m_PrevLayer = invalidCoord;
  m_GridCoord.x = invalidCoord;
  m_GridCoord.y = invalidCoord;
  m_StepCoord.x = invalidCoord;
  m_StepCoord.y = invalidCoord;
  m_GuiGridCoord.x = invalidCoord;
  m_GuiGridCoord.y = invalidCoord;
  m_GuiStepCoord.x = invalidCoord;
  m_GuiStepCoord.y = invalidCoord;
  m_PrevGridCoord.x = invalidCoord;
  m_PrevGridCoord.y = invalidCoord;
  m_PrevStepCoord.x = invalidCoord;
  m_PrevStepCoord.y = invalidCoord;
  m_PrevStepDirection = -1;
  m_bIsImmortal = false;
  m_bIsDrinkable = false;
  m_bIsMarked = false;

  // Caracter pre-init
  m_ColorRgb            = COLOR_UNSET;
  m_ColorCaracter       = COLOR_CARACTER_UNSET;
  m_Sex                 = SEX_UNSET;
  m_Odor                = ODOR_UNSET;
  m_TypeOfReproduction  = REPRODUCT_UNSET;
  m_Silhouette          = FORM_UNSET;
  m_Texture             = TEXTURE_UNSET;
  m_Taste               = TASTE_UNSET;
  m_Habitat             = HABITAT_UNSET;
  m_ConsumeClass        = CONSUM_UNSET;
  m_MoveType            = MOVE_UNSET;
  m_Pheromone           = PHEROMONE_UNSET;

  // Parameter id pre-init
  m_id_Weight     = invalidCharIndex;
  m_id_Toxicity   = invalidCharIndex;
  m_id_Protection = invalidCharIndex;
  m_id_Camouflage = invalidCharIndex;
  m_id_Noise      = invalidCharIndex;

  m_EntitySignature = 0;
  m_Control = CONTROL_LOCAL_AUTO;
  m_Generation = 0;
  m_TotalChildNumber = 0;
  m_Status = STATUS_STATIC;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::~CBasicEntity
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
CBasicEntity::~CBasicEntity()
{
  deleteAllParameters();
  deleteAllLifeStages();
  deleteAllGestationChilds();

  if (m_pBrain != NULL)
    delete m_pBrain;

  if (m_pGenome != NULL)
    delete m_pGenome;

  if (m_pPhysicWelfare != NULL)
    delete m_pPhysicWelfare;
}


//===========================================================================
// Genetic settings
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setEntityFromGenome 
//  
// DESCRIPTION:  1- Perform random mutation on genome
//               2- Set all entity caracteristcs according to genome 
// 
// ARGUMENTS:    double mutationRate [0..100]
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      This base class method can be used for all child classes
//---------------------------------------------------------------------------
bool CBasicEntity::setEntityFromGenome(double mutationRate)
{
  bool resu = false;
  size_t i,j;
  if (m_pGenome==NULL)
  {
    // Set mandatory caracters with default values
    completeCaractsWithDefault();
    // Set mandatory parameters with default values
    completeParamsWithDefault();
    return true;
  }

  if (mutationRate>0)
  {
    m_pGenome->tryMutation((int)floor(mutationRate));
  }

  // 1- Set Sex according to genome
  resu = setSexFromGenome();

  // 2- Set other caracters and parameters according to genome
  CGene* pGen = NULL;
  for (i=0; i<m_pGenome->getNumPair();i++)
  {
    for (j=0; j<m_pGenome->getPair(i)->getNumGenes();j++)
    {
      pGen = m_pGenome->getPair(i)->getDominantAllele(j);
      switch (pGen->getGeneType())
      {
      case GENE_CARACTER:
        resu = setCaractFromGene(pGen);
        break;
      case GENE_PARAMETER:
        resu = setParamFromGene(pGen);
        break;
      default:
        break;
      }
    }
  }
  // Set mandatory caracters forgotten by genome with default values
  completeCaractsWithDefault();
  // Set mandatory parameters forgotten by genome with default values
  completeParamsWithDefault();

  // 3- Set parameter/caracter dependant object according to genome
  // As initialisation of parameter must be finished, don't handle them in prev loop!
  for (i=0; i<m_pGenome->getNumPair();i++)
  {
    for (j=0; j<m_pGenome->getPair(i)->getNumGenes();j++)
    {
      pGen = m_pGenome->getPair(i)->getDominantAllele(j);
      switch (pGen->getGeneType())
      {
      case GENE_LIFESTAGE:
        resu = setLifeStageFromGene(pGen);
        break;
      case GENE_PHY_WELFARE:
        resu = setPhysicWelfareFromGene(pGen);
        break;       
      default:
        break;
      }
    }
  }
  completeLifeStagesWithDefault();
  completePhysicWelfareWithDefault();

  // If life stage defined, initialise the 1st one
  if (m_tLifeStage.size()>0)
  {
    m_indCurrentLifeStage = 0;
    CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
    enterInNewLifeStage(pLifeStage);
  }

  // 4- Set Brain Reactions and Sensors and size according to genome
  for (i=0; i<m_pGenome->getNumPair();i++)
  {
    for (j=0; j<m_pGenome->getPair(i)->getNumGenes();j++)
    {
      pGen = m_pGenome->getPair(i)->getDominantAllele(j);
      switch (pGen->getGeneType())
      {
      case GENE_SENSOR:
        resu = setBrainSensorFromGene(pGen);
        break;
      case GENE_REACTION:
        resu = setBrainReactionFromGene(pGen);
        break;
      case GENE_BRAIN_SIZE:
        resu = setBrainSizeFromGene(pGen);
        break;
      default:
        break;
      }
    }
  }

  completeBrainSensorWithDefault();
  completeBrainReactionWithDefault();

  // 5- Set Brain instinct (init matrix) according to genome
  // As initialisation of sensors and reaction must be finished, don't handle GENE_BRAIN_INIT in prev loop!
  // First, initialize brain with neutral values
  if (m_pBrain != NULL)
  {
    m_pBrain->InitializeNeuronTableNeutral();
    m_pBrain->UpdateDefaultTerritorySize();
  }

  resu = false;
  for (i=0; i<m_pGenome->getNumPair();i++)
  {
    for (j=0; j<m_pGenome->getPair(i)->getNumGenes();j++)
    {
      pGen = m_pGenome->getPair(i)->getDominantAllele(j);
      switch (pGen->getGeneType())
      {
      case GENE_BRAIN_INIT:
        resu = setBrainInstinctFromGene(pGen);
        break;
      case GENE_FEELING:
        resu = setFeelingFromGene(pGen);
        break;
      case GENE_PURPOSE:
        resu = setPurposeFromGene(pGen);
        break;
      case GENE_BRAIN_CONFIG:
        resu = setBrainConfigFromGene(pGen);
        break;
      default:
        break;
      }
    }
  }

  if (resu == false)
  {
    // initialize unset values in matrix to neutral
    completeBrainInstinctWithDefault();
  }

  // To finish, normalize brain table
  if (m_pBrain != NULL)
  {
    m_pBrain->GetDecisionNeuronTable()->NormalizeNeuronMatrix();
  }

  // Set Signature
  computeEntitySignature();

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setSexFromGenome 
//  
// DESCRIPTION:  Set sex property according to genome
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be call by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::setSexFromGenome(void)
{
  m_Sex = m_pGenome->getSexType();
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setCaractFromGene 
//  
// DESCRIPTION:  Set caracteristics according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity caracter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::setCaractFromGene (CGene* pGen)
{
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_CARACTER))
  {
    return false;
  }
  // We are sure Gene is a caracteristic
  bool resu = false;
  
  switch(pGen->getGeneSubType())
  {
  case GENE_CARACT_COLOR:
    if (pGen->getData().size() == sizeof(COLORREF))
    {
      COLORREF colorData = *(COLORREF*)pGen->getData().data();
      setColor(colorData);
      resu = true;
    }
    break;
  case GENE_CARACT_ODOR:
    m_Odor = (OdorType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_PHEROMONE:
    m_Pheromone = (PheromoneType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_TASTE:
    m_Taste = (TasteType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_FORM:
    m_Silhouette = (FormType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_REPRO_TYPE:
    m_TypeOfReproduction = (ReproType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_HABITAT:
    m_Habitat = (HabitatType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_CONSUME_TYPE:
    m_ConsumeClass = (ConsumeType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_MOVE_TYPE:
    m_MoveType = (MoveType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_TEXTURE:
    m_Texture = (TextureType_e)pGen->getElementFloorValue(0);
    resu = true;
    break;
  case GENE_CARACT_PHY_ATTRIBUTE:
    setAttribute((PhyAttributeType_e)pGen->getElementFloorValue(0));
    resu = true;
    break;
  default:
    // Unknown
    resu = false;
    break;
  }
  // If resu is false, caract is not valid for basic entity, but it may be used by inherited class !
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeCaractsWithDefault 
//  
// DESCRIPTION:  Complete caracters unset by genome with default values.
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::completeCaractsWithDefault(void)
{
  if (m_ColorRgb == COLOR_UNSET)
    setColor(COLOR_DEFAULT);

  if (m_Sex == SEX_UNSET)
    m_Sex = SEX_NONE;

  if (m_Odor == ODOR_UNSET)
    m_Odor = ODOR_NONE;

  if (m_TypeOfReproduction == REPRODUCT_UNSET)
    m_TypeOfReproduction = REPRODUCT_NONE;

  if (m_Silhouette == FORM_UNSET)
    m_Silhouette = FORM_UNKNOWN;

  if (m_Texture == TEXTURE_UNSET)
    m_Texture = TEXTURE_NONE;

  if (m_Taste == TASTE_UNSET)
    m_Taste = TASTE_NONE;

  if (m_Habitat == HABITAT_UNSET)
    m_Habitat = HABITAT_EARTH;

  if (m_ConsumeClass == CONSUM_UNSET)
    m_ConsumeClass = CONSUM_NONE;

  if (m_MoveType == MOVE_UNSET)
    m_MoveType = MOVE_NONE;

  if (m_Pheromone == PHEROMONE_UNSET)
    m_Pheromone = PHEROMONE_NONE;

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setParamFromGene 
//  
// DESCRIPTION:  Set parameter according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity parameter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::setParamFromGene (CGene* pGen)
{
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_PARAMETER))
  {
    return false;
  }
  // We are sure Gene is a parameter
  bool resu = false;
  size_t len = pGen->getData().size();
  if (len<3*sizeof(WORD))
  {
    // not enought data to config param
    return false;
  }

  switch(pGen->getGeneSubType())
  {
  case GENE_PARAM_WEIGHT:
    {
      if (m_id_Weight != invalidCharIndex) delete(getParameterNoCheck(m_id_Weight)); // delete if already set
      m_id_Weight = addParameterFromGene(pGen, PARAM_PHYSICAL);
      resu = true;
      break;
    }
  case GENE_PARAM_TOXICITY:
    {
      if (m_id_Toxicity != invalidCharIndex) delete(getParameterNoCheck(m_id_Toxicity)); // delete if already set
      m_id_Toxicity = addParameterFromGene(pGen, PARAM_PHYSICAL);
      resu = true;
      break;
    }
  case GENE_PARAM_PROTECTION:
    {
      if (m_id_Protection != invalidCharIndex) delete(getParameterNoCheck(m_id_Protection)); // delete if already set
      m_id_Protection = addParameterFromGene(pGen, PARAM_PHYSICAL);
      resu = true;
      break;
    }
  case GENE_PARAM_CAMOUFLAGE:
    {
      if (m_id_Camouflage != invalidCharIndex) delete(getParameterNoCheck(m_id_Camouflage)); // delete if already set
      m_id_Camouflage = addParameterFromGene(pGen, PARAM_PHYSICAL);
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
  // If resu is false, param is not valid for basic entity, but it may be used by inherited class !
  return resu;
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeParamsWithDefault 
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
bool CBasicEntity::completeParamsWithDefault(void)
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value
  if (m_id_Weight == invalidCharIndex)
  {
    m_id_Weight = addParameterFromGeneDefinition(PARAM_PHYSICAL, GENE_PARAM_WEIGHT);
  }
  if (m_id_Toxicity == invalidCharIndex)
  {
    m_id_Toxicity = addParameterFromGeneDefinition(PARAM_PHYSICAL, GENE_PARAM_TOXICITY);
  }
  if (m_id_Protection == invalidCharIndex)
  {
    m_id_Protection = addParameterFromGeneDefinition(PARAM_PHYSICAL, GENE_PARAM_PROTECTION);
  }
  if (m_id_Camouflage == invalidCharIndex)
  {
    m_id_Camouflage = addParameterFromGeneDefinition(PARAM_PHYSICAL, GENE_PARAM_CAMOUFLAGE);
  }
  if (m_id_Noise == invalidCharIndex)
  {
    m_id_Noise = addParameterCustom(0, 0, 0, 100, "NoiseRate", PARAM_ENVIRONMENT);
  }
  
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setLifeStageFromGene 
//  
// DESCRIPTION:  Set a Life stage according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity life stage
//  
// REMARKS:      Should NOT be called by derived method. 
//               All stages must be supported at the same inheritage level
//---------------------------------------------------------------------------
bool CBasicEntity::setLifeStageFromGene (CGene* pGen)
{
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeLifeStagesWithDefault 
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
bool CBasicEntity::completeLifeStagesWithDefault(void)
{
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setPhysicWelfareFromGene 
//  
// DESCRIPTION:  Set a Physic Welfare sensitivity according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity life stage
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::setPhysicWelfareFromGene (CGene* pGen)
{
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completePhysicWelfareWithDefault 
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
bool CBasicEntity::completePhysicWelfareWithDefault(void)
{
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setBrainSensorFromGene 
//  
// DESCRIPTION:  Set a sensor attached to brain according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity Sensor
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CBasicEntity::setBrainSensorFromGene (CGene* pGen)
{
  CSensor* pSensor = getTemporarySensorFromGene(pGen);
  if ((pSensor == NULL) || (m_pBrain == NULL))
    return false;

  m_pBrain->AttachSensor(pSensor);
  return true;
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
CSensor* CBasicEntity::getTemporarySensorFromGene (CGene* pGen)
{
  return NULL;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getExpectedBrainSensorWeightSize 
//  
// DESCRIPTION:  Get the expected size of weight required for a gen used in
//               setBrainSensorFromGene.
// 
// ARGUMENTS:    CGene* pGen : Sensor gene containing at least 5 Words
//   
// RETURN VALUE: int : required weight size to accept gene in setBrainSensor
//                     0 mean pGen sensor gene unused
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
size_t  CBasicEntity::getExpectedBrainSensorWeightSize (CGene* pGen)
{
  return 0;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setBrainReactionFromGene 
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
bool CBasicEntity::setBrainReactionFromGene (CGene* pGen)
{
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeBrainSensorWithDefault 
//  
// DESCRIPTION:  Complete Brain sensor if unset by genome 
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::completeBrainSensorWithDefault(void)
{
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeBrainReactionWithDefault 
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
bool CBasicEntity::completeBrainReactionWithDefault(void)
{
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setBrainSizeFromGene 
//  
// DESCRIPTION:  Set a brain size according to gene.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity reaction
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CBasicEntity::setBrainSizeFromGene (CGene* pGen)
{
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setBrainInstinctFromGene 
//  
// DESCRIPTION:  Build Brain initial matrix according to brain Gene
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity Brain Instinct
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CBasicEntity::setBrainInstinctFromGene (CGene* pGen)
{
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setBrainConfigFromGene 
//  
// DESCRIPTION:  Build Brain initial matrix according to brain config
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity Brain config
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
bool CBasicEntity::setBrainConfigFromGene (CGene* pGen)
{
  return false;
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::completeBrainInstinctWithDefault 
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
bool CBasicEntity::completeBrainInstinctWithDefault(void)
{
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setFeelingFromGene 
//  
// DESCRIPTION:  Set a Feeling Welfare sensitivity according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity life stage
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::setFeelingFromGene (CGene* pGen)
{
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setPurposeFromGene 
//  
// DESCRIPTION:  Set a new purpose according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CBasicEntity life stage
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CBasicEntity::setPurposeFromGene (CGene* pGen)
{
  return false;
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::computeEntitySignature 
//  
// DESCRIPTION:  Compute entity signature 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      2 entities with the same signature cannot be differencied
//---------------------------------------------------------------------------
void CBasicEntity::computeEntitySignature()
{
  m_EntitySignature = 0;

  if (m_Silhouette > FORM_UNKNOWN)
    m_EntitySignature += 100000000 * m_Silhouette;
  if (m_ColorCaracter > COLOR_CARACTER_UNSET)
    m_EntitySignature += 1000000 * m_ColorCaracter;
  if (m_Texture > TEXTURE_NONE)
    m_EntitySignature += 100000 * m_Texture;
  if (m_Odor > ODOR_NONE)
    m_EntitySignature += 1000 * m_Odor;
  if (m_Pheromone > PHEROMONE_NONE)
    m_EntitySignature += 100 * m_Pheromone;

  m_EntitySignature += getAttributePresenceMask();

}


//===========================================================================
// Genetic description
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getGeneDescriptionString 
//  
// DESCRIPTION:  Give info string on gene interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CBasicEntity::getGeneDescriptionString(CGene* pGen)
{
  string descStr = STRING_GENE_UNUSED;;

  switch(pGen->getGeneType())
  {
  case GENE_CARACTER:
    descStr = buildCaracterString(pGen);
    break;

  case GENE_PARAMETER:
    descStr = buildParameterString(pGen);
    break;

  case GENE_LIFESTAGE:
    descStr = buildLifeStageString(pGen);
    break;

  case GENE_PHY_WELFARE:
    descStr = buildPhysicWellfareString(pGen);
    break;

  case GENE_SENSOR:
    descStr = buildSensorString(pGen);
    break;

  case GENE_REACTION:
    descStr = buildReactionString(pGen);
    break;

  case GENE_BRAIN_SIZE:
    descStr = buildBrainSizeString(pGen);
    break;

  case GENE_BRAIN_INIT:
    descStr = buildBrainInstinctString(pGen);
    break;

  case GENE_FEELING:
    descStr = buildFeelingWellfareString(pGen);
    break;

  case GENE_PURPOSE:
    descStr = buildPurposeString(pGen);
    break;

  case GENE_BRAIN_CONFIG:
    descStr = buildBrainConfigString(pGen);
    break;

  default:
    // keep STRING_GENE_UNUSED
    break;
  }

  return descStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildCaracterString 
//  
// DESCRIPTION:  Give info string on caracter interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildCaracterString(CGene* pGen)
{
  string caractStr = STRING_GENE_UNUSED;
  string tempStr;

  if ((pGen==NULL)||(pGen->getGeneType() != GENE_CARACTER))
  {
    return caractStr;
  }
  // We are sure Gene is a caracteristic
  size_t len = pGen->getData().size();
  if ((len < 1) || (pGen->getNumElements() < 1))
  {
    return caractStr;
  }

  BYTE data1 = pGen->getElementFloorValue(0);

  switch (pGen->getGeneSubType())
  {
  case GENE_CARACT_COLOR:
    {
      if (len>=sizeof(DWORD))
      {
        tempStr = FormatString("=%03d ", pGen->getElementFloorValue(0));
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
        tempStr = FormatString("=%03d ", pGen->getElementFloorValue(1));
        caractStr += pGen->getElementStrName(1) + tempStr;
        tempStr = FormatString("=%03d ", pGen->getElementFloorValue(2));
        caractStr += pGen->getElementStrName(2) + tempStr;
        tempStr = FormatString("=%03d ", pGen->getElementFloorValue(3));
        caractStr += pGen->getElementStrName(3) + tempStr;
        COLORREF rgbCol = *(COLORREF*)pGen->getData().data();
        tempStr = " ("; tempStr += ColorTypeNameList[convertRgbColorInCaracter(rgbCol)]; tempStr +=")";
        caractStr += tempStr;
      }
      break;
    }
  case GENE_CARACT_ODOR:
    {
      if (data1<ODOR_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += OdorTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_PHEROMONE:
    {
      if (data1<PHEROMONE_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += PheromoneTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_TASTE:
    {
      if (data1<TASTE_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += TasteTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_FORM:
    {
      if (data1<FORM_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += FormTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_REPRO_TYPE:
    {
      if (data1<REPRODUCT_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += ReproTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_HABITAT:
    {
      if (data1<HABITAT_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += HabitatTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_CONSUME_TYPE:
    {
      if (data1<CONSUM_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += ConsumeTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_MOVE_TYPE:
    {
      if (data1<MOVE_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += MoveTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_TEXTURE:
    {
      if (data1<TEXTURE_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += TextureTypeNameList[data1]; tempStr += " ";
        caractStr = pGen->getLabel() + " : " + pGen->getElementStrName(0) + tempStr;
      }
      break;
    }
  case GENE_CARACT_PHY_ATTRIBUTE:
    {
      if (data1<PHY_ATTRIBUTE_NUMBER_TYPE)
      {
        tempStr = "="; tempStr += PhyAttributeTypeNameList[data1]; tempStr += " ";
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

  return caractStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildParameterString 
//  
// DESCRIPTION:  Give info string on parameter interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildParameterString(CGene* pGen)
{
  string paramStr = STRING_GENE_UNUSED;
  string tempStr;

  if ((pGen==NULL)||(pGen->getGeneType() != GENE_PARAMETER))
  {
    return paramStr;
  }
  // We are sure Gene is a parameter
  auto& rawData = pGen->getData();
  size_t len = rawData.size();
  if ((len<3*sizeof(WORD)) || (pGen->getNumElements() < 3))
  {
    // not enought data to config param
    return paramStr;
  }

  double scaledVal0,scaledVal1,scaledVal2;
  
  paramStr = pGen->getLabel() + " : ";
  if (pGen->getElementIsConfigurable(0))
  {
    scaledVal0 = pGen->getElementValue(0);
    tempStr = FormatString("=%6.2f ", scaledVal0);
    paramStr += pGen->getElementStrName(0) + tempStr;
  }
  if (pGen->getElementIsConfigurable(1))
  {
    scaledVal1 = pGen->getElementValue(1);
    tempStr = FormatString("=%6.2f ", scaledVal1);
    paramStr += pGen->getElementStrName(1) + tempStr;
  }
  if (pGen->getElementIsConfigurable(2))
  {
    scaledVal2 = pGen->getElementValue(2);
    tempStr = FormatString("=%6.2f ", scaledVal2);
    paramStr += pGen->getElementStrName(2) + tempStr;
  }

  return paramStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildLifeStageString 
//  
// DESCRIPTION:  Give info string on Life Stage interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildLifeStageString(CGene* pGen)
{
  string paramStr = STRING_GENE_UNUSED;
  string tempStr;

  if ((pGen == NULL) || (pGen->getGeneType() != GENE_LIFESTAGE))
  {
    return (paramStr);
  }
  // We are sure Gene is a parameter
  auto& rawData = pGen->getData();
  size_t len = rawData.size();
  if (len < sizeof(WORD))
  {
    // not enought data to config param
    return (paramStr);
  }

  double scaledVal1 = pGen->getElementValue(0);

  switch (pGen->getGeneSubType())
  {
    case GENE_STAGE_0:
    case GENE_STAGE_1:
    case GENE_STAGE_2:
    case GENE_STAGE_3:
    case GENE_STAGE_4:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.1f%% of life duration", scaledVal1);
      paramStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
    case GENE_STAGE_5:
    case GENE_STAGE_6:
    {
      paramStr = pGen->getLabel() + " : ";
      tempStr = FormatString("=%6.1f%% of death duration", scaledVal1);
      paramStr += pGen->getElementStrName(0) + tempStr;
      break;
    }
    default:
    {
      // keep STRING_GENE_UNUSED
      break;
    }
  }

  return paramStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildPhysicWellfareString 
//  
// DESCRIPTION:  Give info string on Physic Wellfare interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildPhysicWellfareString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildSensorString 
//  
// DESCRIPTION:  Give info string on Sensor interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildSensorString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildReactionString 
//  
// DESCRIPTION:  Give info string on Reaction interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildReactionString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildBrainSizeString 
//  
// DESCRIPTION:  Give info string on Brain Size interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildBrainSizeString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildBrainInstinct 
//  
// DESCRIPTION:  Give info string on Brain Instinct interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildBrainInstinctString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildBrainConfigString 
//  
// DESCRIPTION:  Give info string on Brain Instinct interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildBrainConfigString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildFeelingWellfareString 
//  
// DESCRIPTION:  Give info string on Physic Wellfare interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildFeelingWellfareString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::buildPurposeString 
//  
// DESCRIPTION:  Give info string on Purpose interpretation.
// 
// ARGUMENTS:    CGene* pGen: gene reference
//   
// RETURN VALUE: string : description string.
//  
// REMARKS:      Should be called by all derived method but not elsewhere 
//---------------------------------------------------------------------------
string CBasicEntity::buildPurposeString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

//===========================================================================
// Biotop Connection
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::attachToBiotop
//  
// DESCRIPTION:  Link the entity to the biotop 
// 
// ARGUMENTS:    CBiotop* pBiotop
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::attachToBiotop(CBiotop* pBiotop, Point_t globalStepCoord, size_t layer)
{   
  if (pBiotop != NULL)
  {
    m_pBiotop = pBiotop;
    if (jumpToGlobalStepCoord(globalStepCoord, true, layer))
    {
      m_pBiotop->updateGridEntity(this);
      getAndUpdateGuiGridCoord();
      getAndUpdateGuiStepCoord();
      turnToCenterDir();
      m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_ADDED, this);
      m_DefaultLayer = m_Layer;
      defaultActionWhenAttachedToBiotop();
      return true;
    }
  }
  return false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::detachFromBiotop
//  
// DESCRIPTION:  Disconnect the entity from biotop 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CBasicEntity::detachFromBiotop() 
{
  m_pBiotop = NULL;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::isAttachedToBiotop
//  
// DESCRIPTION:  Check if entity is already attached
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::isAttachedToBiotop()
{
  return (m_pBiotop != NULL);
}

bool CBasicEntity::moveToLayerIfPossible(size_t newLayer)
{
  if (newLayer != m_Layer)
  {
    return jumpToStepCoord(m_StepCoord, true, newLayer, true);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::autoRemove
//  
// DESCRIPTION:  Put the entity in up to be deleted mode
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CBasicEntity::autoRemove(bool addRemoveEvent)
{
  if (m_Id < ENTITY_ID_FIRST_USER_ENTITY)
    return;

  defaultActionWhenRemovedFromBiotop();
  m_Status = STATUS_TOBEREMOVED;
  Point_t coord{ invalidCoord, invalidCoord };
  jumpToGridCoord(coord, true, invalidCoord);
  if ((m_pBiotop != NULL) && addRemoveEvent)
  {
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_REMOVED, this);
  }
  // To avoid problem until biotop removal (TBC)
  m_Id = ENTITY_ID_INVALID;
}


//===========================================================================
// Caracter management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::convertRgbColorInCaracter
//  
// DESCRIPTION:  Give a color caracter according to RGB color
// 
// ARGUMENTS:    COLOREF rbg
//   
// RETURN VALUE: ColorCaracterType_e : Color caracter
//  
// REMARKS:      None
//---------------------------------------------------------------------------
ColorCaracterType_e CBasicEntity::convertRgbColorInCaracter(COLORREF rbg)
{
  double r,g,b,h,s,v,l;

  r = double( (rbg&0xFF) );
  g = double( (rbg&0xFF00)>>8 );
  b = double( (rbg&0xFF0000)>>16 ); 

  PIX_RGB_TO_HSVL_COMMON(r,g,b,h,s,v,l,1);

  // Check on L => Black and White
  if (l < 20)
    return COLOR_CARACTER_BLACK;
  if (l > 230)
    return COLOR_CARACTER_WHITE;

  // Check on S => Grey
  if (s < 25)
    return COLOR_CARACTER_GREY;

  // Check on H => Colors
  if ( (h < 15) || (h > 345) )
  {
    if ( (l < 90) || ( (s < 60) && (l < 140) ) )
      return COLOR_CARACTER_BROWN;
    else if (l > 150)
      return COLOR_CARACTER_PINK;
    else 
      return COLOR_CARACTER_RED;
  }
  else if (h < 45)
  {
    if (l < 80)
      return COLOR_CARACTER_BROWN;
    else if (l > 180)
      return COLOR_CARACTER_BEIGE;
    else 
      return COLOR_CARACTER_ORANGE;
  }
  else if (h < 80)
  {
    if (l < 100)
      return COLOR_CARACTER_BROWN;
    else if (l > 200)
      return COLOR_CARACTER_BEIGE;
    else 
      return COLOR_CARACTER_YELLOW;
  }
  else if (h < 160)
  {
    return COLOR_CARACTER_GREEN;
  }
  else if (h < 260)
  {
    return COLOR_CARACTER_BLUE;
  }
  else
  {
    return COLOR_CARACTER_PINK;
  }
}


//===========================================================================
// Parameters management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::addParameter
//  
// DESCRIPTION:  Add a generic parameter in the list
// 
// ARGUMENTS:    CGenericParam* pParam : new parameter
//   
// RETURN VALUE: int : ID of the parameter in the list
//  
// REMARKS:      None
//---------------------------------------------------------------------------
int CBasicEntity::addParameter(CGenericParam* pParam)
{
  m_tParam.push_back(pParam);
  return ((int)m_tParam.size()-1);
}

int CBasicEntity::addParameterFromGene(CGene* pGen, GenericParamType_e paramType)
{
  double minVal, initVal, maxVal;
  pGen->getElementIsConfigurable(0) ? minVal = pGen->getElementValue(0) : minVal = pGen->getElementDefaultValue(0);
  pGen->getElementIsConfigurable(1) ? initVal = pGen->getElementValue(1) : initVal = pGen->getElementDefaultValue(1);
  pGen->getElementIsConfigurable(2) ? maxVal = pGen->getElementValue(2) : maxVal = pGen->getElementDefaultValue(2);
  CGenericParam* pParam = new CGeneticParam(minVal, initVal, initVal, maxVal,  paramType, pGen->getDefinitions());
  return addParameter(pParam);
}

int CBasicEntity::addParameterFromGeneDefinition(GenericParamType_e paramType, GeneSubTypeParam_e geneSubTypeParam)
{
  const CGeneDefinitions* pGenDefinition = CGeneList::getDefinitions(GENE_PARAMETER, geneSubTypeParam);
  double minVal, initVal, maxVal;
  minVal = pGenDefinition->getElementDefaultValue(0);
  initVal = pGenDefinition->getElementDefaultValue(1);
  maxVal = pGenDefinition->getElementDefaultValue(2);
  CGenericParam* pParam = new CGeneticParam(minVal, initVal, initVal, maxVal, paramType, pGenDefinition);
  return addParameter(pParam);
}

int CBasicEntity::addParameterCustom(double valMin, double valInit, double valNominal, double valMax, string label, GenericParamType_e type)
{
  CGenericParam* pParam = new CCustomParam(valMin, valInit, valNominal, valMax, label, type);
  return addParameter(pParam);
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getNumParameter
//  
// DESCRIPTION:  Get the number of parameters in the list
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: int : Number of parameters in the list
//  
// REMARKS:      None
//---------------------------------------------------------------------------
size_t CBasicEntity::getNumParameter()
{
  return m_tParam.size();
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getParameter
//  
// DESCRIPTION:  Get a parameters in the list
// 
// ARGUMENTS:    int id : Id returned by addParameter
//   
// RETURN VALUE: CGenericParam* : Ptr on the parameter
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CGenericParam* CBasicEntity::getParameter(size_t id)
{
  if (id > m_tParam.size())
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("ENTITY - ERROR entity name %s Cannot getParameter index %d\n", getLabel().c_str(), id);
    return NULL;
  }
  else
  {
    return m_tParam[id];
  }
}

CGenericParam* CBasicEntity::getParameterNoCheck(size_t id)
{
  return m_tParam[id];
}

CGenericParam* CBasicEntity::getParameterByName(string paramName)
{
  for (auto pParam : m_tParam)
  {
    if (pParam->getLabel() == paramName)
      return (pParam);
  }
  return NULL;
}

size_t CBasicEntity::getParamIdByName(string paramName)
{
  for (size_t id=0; id<m_tParam.size(); id++)
  {
    if (m_tParam[id]->getLabel() == paramName)
      return id;
  }
    return invalidIndex;
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::deleteAllParameters
//  
// DESCRIPTION:  Delete all parameters in the list
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CBasicEntity::deleteAllParameters() 
{
  for (size_t i = 0; i < m_tParam.size(); i++)
  {
    if (m_tParam[i] != NULL)
      delete m_tParam[i]; 
  }
  m_tParam.clear();
}

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::nextSecond
//  
// DESCRIPTION:  Basic action trigger called by biotop every time unit
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed highly impacted by this method
//---------------------------------------------------------------------------
void CBasicEntity::nextSecond()
{
  // NOP 
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::nextHour
//  
// DESCRIPTION:  Secondary action trigger called by biotop every hour
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CBasicEntity::nextHour()
{
  if (!isRemoteControlled())
  {
    m_HourCounter++;
    if (m_HourCounter > NUMBER_HOURS_PER_DAY)
    {
      nextDay();
      m_HourCounter = 0;
    }
  }
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::nextDay
//  
// DESCRIPTION:  Living day trigger called by CBasicEntity every day
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CBasicEntity::nextDay(bool doQuickAgeing)
{
  if (m_indCurrentLifeStage < m_tLifeStage.size())
  {
    CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
    if (pLifeStage->increaseDayCountAndCheckEnd())
    {
      if (m_indCurrentLifeStage<(m_tLifeStage.size()-1))
      {
        m_indCurrentLifeStage++;
        pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
        enterInNewLifeStage(pLifeStage);
        m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
      }      
    }
  }
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getAge
//  
// DESCRIPTION:  Return Age
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Should be overiden by derived object
//---------------------------------------------------------------------------
int CBasicEntity::getAge()
{
  return 0;
}

double CBasicEntity::getLifeExpectation()
{
  return 0;
}

int CBasicEntity::getDecompositionTime()
{
  return 0;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::checkVitalNeedsOk
//  
// DESCRIPTION:  Return true if vital needs are all Ok, 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: true : OK, false Ko
//  
// REMARKS:      Called every second by Physical Welfare
//---------------------------------------------------------------------------
bool CBasicEntity::checkVitalNeedsOk()
{
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::quickAgeing
//  
// DESCRIPTION:  Get older quickely
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      
//---------------------------------------------------------------------------
void CBasicEntity::quickAgeing(int nbDays)
{
  for(int i=0; i<nbDays; i++)
  {
    nextDay(true);
  }
}


//===========================================================================
// Life stages management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::addLifeStage
//  
// DESCRIPTION:  Add a life stage in the list. Sort according to StageType
//               If same StageType already present, do not insert
// 
// ARGUMENTS:    CLifeStage* pLifeStage : new life stage
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::addLifeStage(CLifeStage* pLifeStage)
{
  size_t index = 0;
  bool resu = false;

  while ( (index<m_tLifeStage.size()) 
        &&(pLifeStage->getStageType() > m_tLifeStage[index]->getStageType()) )
  {
    index++;
  }
  
  // if stage already exist, don't insert it
  if ( (index>=m_tLifeStage.size())
     ||(pLifeStage->getStageType() != m_tLifeStage[index]->getStageType()) )
  {
    m_tLifeStage.insert(m_tLifeStage.begin()+index, 1, pLifeStage);
    resu = true;
  }

  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::deleteAllLifeStages
//  
// DESCRIPTION:  Delete all Life Stages in the list
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CBasicEntity::deleteAllLifeStages() 
{
  for (size_t i = 0; i < m_tLifeStage.size(); i++)
  {
    if (m_tLifeStage[i] != NULL)
      delete (m_tLifeStage[i]); 
  }
  m_tLifeStage.clear();
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::setCurrentLifeStages
//  
// DESCRIPTION:  Set m_indCurrentLifeStage to desired stage
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: Success
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::setCurrentLifeStages(LifeStage_e newStage) 
{
  if (m_indCurrentLifeStage >= m_tLifeStage.size())
    return false;

  if (newStage == m_tLifeStage[m_indCurrentLifeStage]->getStageType())
    return true;

  bool resu = false;
  for (size_t i=0; i<m_tLifeStage.size(); i++)
  {
    if (newStage == m_tLifeStage[i]->getStageType())
    {
      m_indCurrentLifeStage = i;
      CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
      enterInNewLifeStage(pLifeStage);
      m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
      resu = true;
    }
  }
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::enterInNewLifeStage
//  
// DESCRIPTION:  Change parameters according to new stage
// 
// ARGUMENTS:    CLifeStage* pLifeStage : new life stage
//   
// RETURN VALUE: None
//  
// REMARKS:      Pure virtual - Should be overloaded to have effect
//---------------------------------------------------------------------------
void CBasicEntity::enterInNewLifeStage(CLifeStage* pLifeStage)
{

}

//===========================================================================
// Behavior
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::forceNextAction
//  
// DESCRIPTION:  Basic action trigger
// 
// ARGUMENTS:    choiceIndType myChoice: Id of the desired action
//   
// RETURN VALUE: feedbackValType : Feedback result from user action
//  
// REMARKS:      Should be overloaded by entities with a brain
//---------------------------------------------------------------------------
feedbackValType CBasicEntity::forceNextAction(choiceIndType myChoice)
{
  doNothing();
  return 0;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::predictNextAction
//  
// DESCRIPTION:  Predict action without any memorization
// 
// ARGUMENTS:    choiceIndType myChoice: Id of the desired action
//   
// RETURN VALUE: choiceIndType : Id of the decided action
//  
// REMARKS:      Should be overloaded by entities with a brain
//---------------------------------------------------------------------------
choiceIndType CBasicEntity::predictNextAction()
{
  choiceIndType autoChoice = invalidIndex;
  return autoChoice;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::moveLinear
//  
// DESCRIPTION:  Action move forward
// 
// ARGUMENTS:    int nbSteps: >0 = move forward, <0 = move backward  
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CBasicEntity::moveLinear(int nbSteps)
{ 
  // Update prev step coord
  m_PrevStepCoord = m_StepCoord;

  RelativePos_t relPos = {nbSteps,0};
  Point_t newStepCoord = getStepCoordRelative(relPos);
  size_t newGridCoordX, newGridCoordY;

  newGridCoordX = getGridPosFromStepPos(newStepCoord.x);
  newGridCoordY = getGridPosFromStepPos(newStepCoord.y);

  int nbGridMove = cybio_max( abs((int)newGridCoordX - (int)m_GridCoord.x), abs((int)newGridCoordY - (int)m_GridCoord.y) );

  Point_t nextGridCoord = m_GridCoord;
  bool resu = true;
  for (int i=0; i<nbGridMove; i++)
  {
    if (newGridCoordX > nextGridCoord.x)
       nextGridCoord.x++;
    else if (newGridCoordX < nextGridCoord.x)
       nextGridCoord.x--; 

    if (newGridCoordY > nextGridCoord.y)
       nextGridCoord.y++;
    else if (newGridCoordY < nextGridCoord.y)
       nextGridCoord.y--; 

    resu = jumpToGridCoord(nextGridCoord, false);

    if (!resu)
      break;
  }

  if (resu)
  {
    // No trouble found: goto exact coord
    m_StepCoord = newStepCoord;
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_MOVED, this);
  }

  setNoise(nbSteps);

  return resu;
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::jumpToGridCoord
//  
// DESCRIPTION:  Action move to any coordonate
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      coord -1,-1 can be used to move entity out of the game
//---------------------------------------------------------------------------
bool CBasicEntity::jumpToGridCoord(const Point_t& newGridCoord, bool chooseLayer, size_t newLayer)
{
  bool resu;
  size_t nextLayer = chooseLayer ? newLayer : m_Layer;

  if (m_pBiotop==NULL)
  {
    m_GridCoord = newGridCoord;
    m_bHasMoved  = true;
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_PrevGridCoord = m_GridCoord;
    m_PrevStepCoord = m_StepCoord;
    m_Layer = nextLayer;
    m_PrevLayer = m_Layer;
    resu = true;
  }
  else if ( m_pBiotop->isCoordValidAndFree(newGridCoord, nextLayer)  // valid
         || ( (newGridCoord.x == invalidCoord) && (newGridCoord.y == invalidCoord) ) )  // or out
  {
    // newCoord valid ... Move
    m_PrevGridCoord = m_GridCoord;
    m_PrevStepCoord = m_StepCoord;
    m_GridCoord = newGridCoord;
    m_bHasMoved  = true;
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_PrevLayer = m_Layer; 
    m_Layer = nextLayer;
    resu = true;
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_MOVED, this);
    m_pBiotop->updateGridEntity(this);
  }
  else
  {
    // Move out of the local biotop: quiet auto remove (without event)
    if (!m_pBiotop->isCoordValid(newGridCoord))
    {
      CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
      CYBIOCORE_LOG("ENTITY - Remove entity %s moving out of local map.\n", getLabel().c_str());
      autoRemove(false);
    }
    resu = false;
  }
  return resu;
}

bool CBasicEntity::jumpToGlobalGridCoord(const Point_t& newGlobalGridCoord, bool chooseLayer, size_t newLayer)
{
  if (m_pBiotop == NULL)
  {
    CYBIOCORE_LOG("ERROR jumpToGlobalGridCoord entity not ATTACHED");
    return jumpToGridCoord(newGlobalGridCoord, chooseLayer, newLayer);
  }
  else
  {
    return jumpToGridCoord(m_pBiotop->getGridCoordFromGlobalGridCoord(newGlobalGridCoord), chooseLayer, newLayer);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::jumpToStepCoord
//  
// DESCRIPTION:  Action move to any coordonate
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      coord -1,-1 can be used to move entity out of the game
//---------------------------------------------------------------------------
bool CBasicEntity::jumpToStepCoord(const Point_t& newStepCoord, bool chooseLayer, size_t newLayer, bool addMoveEvent)
{
  if (addMoveEvent && ((m_PrevStepCoord.x != m_StepCoord.x) || (m_PrevStepCoord.y != m_StepCoord.y)))
  {
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_MOVED, this);
  }

  m_PrevStepCoord = m_StepCoord;
  // Update grid coord if needed
  Point_t newGridCoord{ newStepCoord.x / NB_STEPS_PER_GRID_SQUARE, newStepCoord.y / NB_STEPS_PER_GRID_SQUARE };
  if ( (m_GridCoord.x != newGridCoord.x) || (m_GridCoord.y != newGridCoord.y) || chooseLayer)
  {
    if (!jumpToGridCoord(newGridCoord, chooseLayer, newLayer))
      return false;
  }

  // Update new step coord
  m_StepCoord = newStepCoord;

  return true;
}

bool CBasicEntity::jumpToGlobalStepCoord(const Point_t& newGlobalStepCoord, bool chooseLayer, size_t newLayer, bool addMoveEvent)
{
  if (m_pBiotop == NULL)
  {
    CYBIOCORE_LOG("ERROR jumpToGlobalStepCoord entity not ATTACHED");
    return jumpToStepCoord(newGlobalStepCoord, chooseLayer, newLayer, addMoveEvent);
  }
  else
  {
    return jumpToStepCoord(m_pBiotop->getStepCoordFromGlobalStepCoord(newGlobalStepCoord), chooseLayer, newLayer, addMoveEvent);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::moveToGridCenterPos
//  
// DESCRIPTION:  Move entity in the center of grid square
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      can be used before or after any action
//---------------------------------------------------------------------------
bool CBasicEntity::moveToGridCenterPos()
{
  // Update prev step coord
  m_PrevStepCoord = m_StepCoord;
  // Update new step coord
  m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2;   // center in square
  m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2;   // center in square
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::moveToGridEdgePos
//  
// DESCRIPTION:  Move entity in the center of grid square according to direction
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      can be used before or after any action
//---------------------------------------------------------------------------
bool CBasicEntity::moveToGridEdgePos()
{
  // Update prev step coord
  m_PrevStepCoord = m_StepCoord;

  switch (m_Direction)
  {
  case 0:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE - 1; // on edge
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2;   // center in square
    break;
  case 1:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE - 2; // in corner
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE - 2; // in corner
    break;
  case 2:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2;   // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE - 1; // on edge
    break;
  case 3:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + 2; // in corner
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE - 2; // in corner
    break;
  case 4:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + 1; // on edge
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    break;
  case 5:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + 2; // in corner
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + 2; // in corner
    break;
  case 6:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + 1; // on edge
    break;
  case 7:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE - 2; // in corner
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + 2; // in corner
    break;
  default:
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    break;
  }

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::turnToCenterDir
//  
// DESCRIPTION:  Allign StepDirection on grid direction
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CBasicEntity::turnToCenterDir()
{
  m_PrevStepDirection = m_StepDirection;
  // Go exactely in view direction
  m_StepDirection = 45*m_Direction;
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::turnRight
//  
// DESCRIPTION:  Action turn right
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CBasicEntity::turnRight(size_t nDegree)
{
  setStepDirection(m_StepDirection + 360 - (int)nDegree, true);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::turnLeft
//  
// DESCRIPTION:  Action turn left
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true if move was possible
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
bool CBasicEntity::turnLeft(size_t nDegree)
{
  setStepDirection(m_StepDirection + (int)nDegree, true);
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::doNothing
//  
// DESCRIPTION:  Action NOP
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Directly called by CReaction
//---------------------------------------------------------------------------
void CBasicEntity::doNothing()
{

}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::autoKill
//  
// DESCRIPTION:  Put entity in dead mode
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CBasicEntity::autoKill()
{
  if ( isAlive() && !m_bIsImmortal )
  {
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    setStatus(STATUS_DEAD);
    if (!checkHabitat())
    {
      autoRemove();
    }
  }
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::checkHabitat
//  
// DESCRIPTION:  Check if habitat compatible with current layer type.
//               Update m_bCannotBreathe to true if not compatible
//
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : true Habitat compliant, false bad Habitat
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::checkHabitat(void)
{
  if (m_pBiotop != NULL)
  {
    LayerType_e curLayer = m_pBiotop->getLayerType(m_GridCoord, m_Layer);
    if ((m_Habitat == HABITAT_EARTH) && (curLayer != LAYER_OVER_GROUND) && (curLayer != LAYER_OVER_WET_GROUND))
    {
      return false;
    }
    else if ((m_Habitat == HABITAT_WATER) && (curLayer != LAYER_UNDER_WATER))
    {
      return false;
    }
  }
  return true;
}

//===========================================================================
// Gestation Babies management management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::deleteAllGestationChilds
//  
// DESCRIPTION:  Delete all childs in gestation in the list
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
void CBasicEntity::deleteAllGestationChilds() 
{
  for (size_t i = 0; i < m_tGestationChilds.size(); i++)
  {
    if (m_tGestationChilds[i] != NULL)
      delete (m_tGestationChilds[i]); 
  }
  m_tGestationChilds.clear();
}


//===========================================================================
// Save/Load in XML file
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::saveInXmlFile
//  
// DESCRIPTION:  Save all entity data
// 
// ARGUMENTS:    string fileNameWithPath
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::saveInXmlFile(string fileName, string newLabel)
{
  bool resu = false;

  if (fileName == ".xml")
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("ENTITY - WARNING Trying to save entity %s in empty file name .xml\n", getLabel().c_str());
  }

  TiXmlDocument *pXmlDoc = new TiXmlDocument(fileName);
  saveInXmlFile(pXmlDoc, newLabel);
  resu = pXmlDoc->SaveFile();
  delete pXmlDoc;
  return resu;
}


bool CBasicEntity::saveInXmlFile(TiXmlDocument* pXmlDoc, string newLabel)
{
  if (pXmlDoc == NULL)
    return false;

  pXmlDoc->Clear();
  bool resu = addEntityInXmlFile(pXmlDoc, newLabel, this, false);

  for (size_t i = 0; i < m_tGestationChilds.size(); i++)
  {
    CBasicEntity* pFetus = m_tGestationChilds[i];
    addEntityInXmlFile(pXmlDoc, "", pFetus, true);
  }

  return resu;
}

bool CBasicEntity::addEntityInXmlFile(TiXmlDocument * pXmlDoc, string newLabel, CBasicEntity * pEntity, bool setAsFetusEntity)
{
  bool resu = false;
  TiXmlElement* pElement;
  TiXmlNode* pNodeEntity = NULL;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeChild = NULL;
  string tempLabel;
  string labelEntityNode;
  size_t i;

  if ((pXmlDoc==NULL) || (pEntity==NULL))
    return false;

  setAsFetusEntity? labelEntityNode = XML_NODE_FETUS_ENTITY: labelEntityNode = XML_NODE_ENTITY;
  TiXmlElement newNode(labelEntityNode);
  pNodeEntity = pXmlDoc->InsertEndChild(newNode);

  if (newLabel == "")
    tempLabel = pEntity->getLabel();
  else
    tempLabel = newLabel;

  if ((pNodeEntity != NULL) && (pNodeEntity->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNodeEntity;
    pElement->SetAttribute( XML_ATTR_LABEL, tempLabel);
    pElement->SetAttribute( XML_ATTR_GENERATION, pEntity->m_Generation);
    pElement->SetAttribute( XML_ATTR_STATUS, pEntity->m_Status);
    pElement->SetAttribute( XML_ATTR_LAYER, (int)pEntity->m_DefaultLayer);
    pElement->SetAttribute( XML_ATTR_DIRECTION, pEntity->m_Direction);
    pElement->SetAttribute( XML_ATTR_HOUR_COUNT, pEntity->m_HourCounter);
    pElement->SetAttribute( XML_ATTR_IMMORTAL, (int)pEntity->m_bIsImmortal);
  }

  // Create Life stages node
  TiXmlElement newNodeStages(XML_NODE_LIFE_STAGES);
  pNode = pNodeEntity->InsertEndChild(newNodeStages);

  if ((pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNode;
    pElement->SetAttribute( XML_ATTR_CURRENT_IND, (int)pEntity->m_indCurrentLifeStage);

    for (i=0; i< pEntity->getNbLifeStages(); i++)
    {
      TiXmlElement newLifeStage(XML_NODE_LIFE_STAGE);
      pNodeChild = pNode->InsertEndChild(newLifeStage);
      if (pNodeChild != NULL) 
      {
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_INDEX, (int)i);
        pElement->SetAttribute(XML_ATTR_DAY_COUNT, pEntity->getLifeStage(i)->getCurDayCount());
      }
    }
  }

  // Save parameters
  for (i=0; i<pEntity->getNumParameter(); i++)
  {
    // Save Min, Max for entities without genome definitions
    bool doSaveMinMax = ((pEntity->getGenome() == NULL) || (pEntity->getGenome()->getNumPair() == 0)) ? true : false;
    pEntity->getParameter(i)->saveInXmlFile(pNodeEntity, doSaveMinMax);
  }

  // Set caracters
  TiXmlElement newCaract(XML_NODE_CARACTER);
  newCaract.SetAttribute(XML_ATTR_NAME, "ColorRGB");
  newCaract.SetAttribute(XML_ATTR_VALUE, pEntity->m_ColorRgb);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Odor");
  newCaract.SetAttribute(XML_ATTR_VALUE, (int)pEntity->m_Odor);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Pheromone");
  newCaract.SetAttribute(XML_ATTR_VALUE, (int)pEntity->m_Pheromone);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Form");
  newCaract.SetAttribute(XML_ATTR_VALUE,(int)pEntity->m_Silhouette);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Texture");
  newCaract.SetAttribute(XML_ATTR_VALUE, (int)pEntity->m_Texture);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Attribute");
  newCaract.SetAttribute(XML_ATTR_VALUE, pEntity->m_PhyAttribute.getPresenceMask());
  pNodeEntity->InsertEndChild(newCaract);

  // Gestation
  TiXmlElement newNodeGestation(XML_NODE_GESTATION);
  pNode = pNodeEntity->InsertEndChild(newNodeGestation);
  if ((pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNode;
    pElement->SetAttribute(XML_ATTR_TOTAL_CHILDS, pEntity->m_TotalChildNumber);
  }

  if (pEntity->m_pGenome!=NULL)
  {
    pEntity->m_pGenome->saveInXmlNode(pNodeEntity);
  }

  if ((pEntity->m_pBrain!=NULL) && (!setAsFetusEntity))
  {
    pEntity->m_pBrain->saveInTiXmlFile(pXmlDoc);
  }

  return resu;
}


bool CBasicEntity::saveBrainPurposeMapInXmlFile(TiXmlDocument* pXmlDoc)
{
  if ((m_pBrain != NULL) && (m_pBrain->GetGeographicMap() != NULL))
  {
    return m_pBrain->GetGeographicMap()->saveInXmlFile(pXmlDoc);
  }
  return false;
}


//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getDefaultLayerFromXmlFile
//  
// DESCRIPTION:  Get entity default layer from xlm file
// 
// ARGUMENTS:    string fileNameWithPath
//               int& layer: result
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::getDefaultLayerFromXmlFile(string fileName, int& layer)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileName);
  resu = xmlDoc.LoadFile();
  getDefaultLayerFromXmlFile(&xmlDoc, layer);
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getDefaultLayerFromXmlFile
//  
// DESCRIPTION:  Get entity default layer from xlm file
// 
// ARGUMENTS:    string fileNameWithPath
//               int& layer: result
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::getDefaultLayerFromXmlFile(TiXmlDocument *pXmlDoc, int& layer)
{
  if (pXmlDoc==NULL)
    return false;

  TiXmlNode* pNode = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if(pNode->Type() == TiXmlNode::TINYXML_ELEMENT)
  {
    TiXmlElement* pElement = (TiXmlElement*)pNode;
    if ( pElement->QueryIntAttribute(XML_ATTR_LAYER,  &layer) == TIXML_NO_ATTRIBUTE)
      return false;
  }

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getEntityNameFromXmlFile
//  
// DESCRIPTION:  Get entity name
// 
// ARGUMENTS:    string fileNameWithPath
//               stringt& name: result
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::getEntityNameFromXmlFile(string fileName, string& name)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileName);
  resu = xmlDoc.LoadFile();
  getEntityNameFromXmlFile(&xmlDoc, name);
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getEntityNameFromXmlFile
//  
// DESCRIPTION:  Get entity name
// 
// ARGUMENTS:    string fileNameWithPath
//               stringt& name: result
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::getEntityNameFromXmlFile(TiXmlDocument* pXmlDoc, string& name)
{
  if (pXmlDoc == NULL)
    return false;
  TiXmlNode* pNode = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  return getEntityNameFromXmlNode(pNode, name);
}

bool CBasicEntity::getEntityNameFromXmlNode(TiXmlNode * pNodeEntity, string & name)
{
  if((pNodeEntity !=NULL)&&(pNodeEntity->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    TiXmlElement* pElement = (TiXmlElement*)pNodeEntity;
    if ( pElement->QueryStringAttribute(XML_ATTR_LABEL,  &name) == TIXML_NO_ATTRIBUTE)
      return false;
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::loadDataFromXmlFile
//  
// DESCRIPTION:  Load entity data
// 
// ARGUMENTS:    string fileNameWithPath
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::loadDataFromXmlFile(string fileName)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileName);
  resu = xmlDoc.LoadFile();
  loadDataFromXmlFile(&xmlDoc);
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::loadDataFromXmlFile
//  
// DESCRIPTION:  Load entity data
// 
// ARGUMENTS:    TiXmlDocument *pXmlDoc
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::loadDataFromXmlFile(TiXmlDocument *pXmlDoc)
{
  TiXmlElement* pElement;
  TiXmlNode* pNode = NULL;

  if (pXmlDoc==NULL)
    return false;

  TiXmlNode* pNodeEntity = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if ((pNodeEntity != NULL) && (pNodeEntity->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeEntity;
    int direction, defLayer, status, immortal;

    if ( pElement->QueryStringAttribute(XML_ATTR_LABEL,  &m_Label) == TIXML_NO_ATTRIBUTE)
      m_Label = "Unset";

    if ( pElement->QueryIntAttribute(XML_ATTR_GENERATION,  &m_Generation) == TIXML_NO_ATTRIBUTE)
      m_Generation = 0;

    if ( pElement->QueryIntAttribute(XML_ATTR_STATUS,  &status) == TIXML_NO_ATTRIBUTE)
      status = STATUS_STATIC;
    m_Status = (status < STATUS_TOBEREMOVED) ? (StatusType_e)status : STATUS_DEAD;

    if ( pElement->QueryIntAttribute(XML_ATTR_DIRECTION,  &direction) == TIXML_NO_ATTRIBUTE)
      direction = 0;
    setDirection(direction);

    if (pElement->QueryIntAttribute(XML_ATTR_LAYER, &defLayer) == TIXML_NO_ATTRIBUTE)
      defLayer = 1;
    m_DefaultLayer = defLayer;

    if ( pElement->QueryIntAttribute(XML_ATTR_HOUR_COUNT,  &m_HourCounter) == TIXML_NO_ATTRIBUTE)
      m_HourCounter = 0;

    if ( pElement->QueryIntAttribute(XML_ATTR_IMMORTAL,  &immortal) == TIXML_NO_ATTRIBUTE)
      immortal = 0;

    if (immortal!=0)
      m_bIsImmortal = true;

    // Life stage management
    size_t prevLifeStage = m_indCurrentLifeStage;
    int readIndLifeStage;
    pNode = pNodeEntity->FirstChild(XML_NODE_LIFE_STAGES);
    if ((pNode!=NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
    {
      pElement = (TiXmlElement*)pNode;
      if ( pElement->QueryIntAttribute(XML_ATTR_CURRENT_IND,  &readIndLifeStage) == TIXML_NO_ATTRIBUTE)
        m_indCurrentLifeStage = 0;
      else
        m_indCurrentLifeStage = readIndLifeStage;

      if ( (m_indCurrentLifeStage != prevLifeStage) && (getNbLifeStages()>0) && (m_indCurrentLifeStage<getNbLifeStages()) )
      {
        CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
        enterInNewLifeStage(pLifeStage);
      }

      int indexRead, dayCount;
      size_t index;
      pNode = pNode->FirstChild();
      while (pNode != NULL)
      {
        if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_LIFE_STAGE))
        {
          pElement = (TiXmlElement*)pNode;
          if (pElement->QueryIntAttribute(XML_ATTR_INDEX, &indexRead) == TIXML_NO_ATTRIBUTE)
            index = invalidIndex;
          else
            index = indexRead;
          if ( pElement->QueryIntAttribute(XML_ATTR_DAY_COUNT,  &dayCount) == TIXML_NO_ATTRIBUTE)
            dayCount = 0;
          if (index < getNbLifeStages())
            getLifeStage(index)->setCurDayCount(dayCount);
        }
        pNode = pNode->NextSibling();
      }
    }

    // Parameters management
    string paramName;
    CGenericParam* pParam;
    pNode = pNodeEntity->FirstChild(XML_NODE_PARAMETER);
    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_PARAMETER))
      {
        pElement = (TiXmlElement*)pNode;
        if (pElement->QueryStringAttribute(XML_ATTR_NAME, &paramName) != TIXML_NO_ATTRIBUTE)
        {
          pParam = getParameterByName(paramName);
          if (pParam != NULL)
          {
            pParam->loadFromXmlFile(pNode);
          }
        }
      }
      pNode = pNode->NextSibling();
    }

    // Caracters management
    string caractName;
    string caractVal;
    pNode = pNodeEntity->FirstChild(XML_NODE_CARACTER);
    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_CARACTER))
      {
        pElement = (TiXmlElement*)pNode; 
        if ( pElement->QueryStringAttribute(XML_ATTR_VALUE,  &caractVal) == TIXML_NO_ATTRIBUTE)
          caractVal = "0";
        
        if ( pElement->QueryStringAttribute(XML_ATTR_NAME,  &caractName) != TIXML_NO_ATTRIBUTE)
        {
          if (caractName == "ColorRGB")
            m_ColorRgb = atoi(caractVal.c_str());
          else if (caractName == "Odor")
            m_Odor = (OdorType_e)atoi(caractVal.c_str()); 
          else if (caractName == "Pheromone")
            m_Pheromone =(PheromoneType_e)atoi(caractVal.c_str()); 
          else if (caractName == "Form")
            m_Silhouette = (FormType_e)atoi(caractVal.c_str());
          else if (caractName == "Texture")
            m_Texture = (TextureType_e)atoi(caractVal.c_str());
          else if (caractName == "Attribute")
            m_PhyAttribute.setPresenceMask((DWORD)atoi(caractVal.c_str())); 
        }
      }
      pNode = pNode->NextSibling();
    }

    // Gestation Babies
    pNode = pNodeEntity->FirstChild(XML_NODE_GESTATION);
    if ((pNode!=NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
    {
      pElement = (TiXmlElement*)pNode;
      if ( pElement->QueryIntAttribute(XML_ATTR_TOTAL_CHILDS,  &m_TotalChildNumber) == TIXML_NO_ATTRIBUTE)
        m_TotalChildNumber = 0;
    }
  }

  loadBabiesFromXmlFile(pXmlDoc);
  return true;
}

bool CBasicEntity::loadBabiesFromXmlFile(TiXmlDocument* pXmlDoc)
{
  if (pXmlDoc == NULL)
    return false;

  TiXmlNode* pNodeEntity = pXmlDoc->FirstChild(XML_NODE_FETUS_ENTITY);
  while (pNodeEntity!=NULL)
  {
    CGenome* pBabyGenome;
    string babyName;
    CBasicEntity* pFetusEntity;
    pBabyGenome = new CGenome(CLASS_NONE, "");
    if (pBabyGenome != NULL)
    {
      pBabyGenome->loadFromXmlNode(pNodeEntity);
      getEntityNameFromXmlNode(pNodeEntity, babyName);
      pFetusEntity = CEntityFactory::createEntity(babyName, pBabyGenome);
      m_tGestationChilds.push_back(pFetusEntity);
    }
    pNodeEntity = pNodeEntity->NextSibling(XML_NODE_FETUS_ENTITY);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getGenomeFromXmlFile
//  
// DESCRIPTION:  Load all entity data except params,brain values and babies.
//               Genetic reconstruction is required.
// 
// ARGUMENTS:    string fileNameWithPath
//               CGenome genome   
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      Reserved for non attached entities
//---------------------------------------------------------------------------
bool CBasicEntity::getGenomeFromXmlFile(string fileName, CGenome& genome)
{
  return genome.loadFromXmlFile(fileName);
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::getGenomeFromXmlFile
//  
// DESCRIPTION:  Load all entity data except params,brain values and babies.
//               Genetic reconstruction is required.
// 
// ARGUMENTS:    string fileNameWithPath
//               CGenome genome   
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      Reserved for non attached entities
//---------------------------------------------------------------------------
bool CBasicEntity::getGenomeFromXmlFile(TiXmlDocument *pXmlDoc, CGenome& genome)
{
  return genome.loadFromXmlFile(pXmlDoc);
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::loadBrainFromXmlFile
//  
// DESCRIPTION:  Load all entity brain.
// 
// ARGUMENTS:    string fileNameWithPath
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::loadBrainFromXmlFile(string fileName)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileName);
  resu = xmlDoc.LoadFile();
  loadBrainFromXmlFile(&xmlDoc);
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::loadBrainFromXmlFile
//  
// DESCRIPTION:  Load all entity brain.
// 
// ARGUMENTS:    string fileNameWithPath
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::loadBrainFromXmlFile(TiXmlDocument *pXmlDoc)
{
  return (m_pBrain == NULL) ? false : m_pBrain->loadFromTiXmlFile(pXmlDoc);
}

bool CBasicEntity::loadPurposeMapFromXmlFile(TiXmlDocument* pXmlDoc)
{
  if (m_pBrain == NULL)
    return false;
  m_pBrain->CreateGeoMapArroudCurrentPosition();
  return m_pBrain->GetGeographicMap()->loadFromXmlFile(pXmlDoc);
}

//===========================================================================
// Get / Set for attributes
//===========================================================================
    
const Point_t& CBasicEntity::getGridCoord()
{
  return m_GridCoord;
}

Point_t CBasicEntity::getGlobalGridCoord()
{
  if (m_pBiotop != NULL)
  {
    return m_pBiotop->getGlobalGridCoordFromGridCoord(m_GridCoord);
  }
  else
  {
    CYBIOCORE_LOG("ERROR getGlobalGridCoord entity not ATTACHED");
    return m_GridCoord;
  }
}

const Point_t& CBasicEntity::getPrevGridCoord()
{
  return m_PrevGridCoord;
}

const Point_t& CBasicEntity::getPrevStepCoord()
{
  return m_PrevStepCoord;
}

Point_t CBasicEntity::getAndUpdateGuiGridCoord()
{
  Point_t prevGridCoord = m_GuiGridCoord;
  m_GuiGridCoord = m_GridCoord;
  return prevGridCoord;
}

Point_t CBasicEntity::getAndUpdateGuiStepCoord()
{
  Point_t prevStepCoord = m_GuiStepCoord;
  m_GuiStepCoord = m_StepCoord;
  return prevStepCoord;
}

Point_t CBasicEntity::getGridCoordRelative(const RelativePos_t& relativeCoord)
{
  Point_t position;
  //  Optimisation
  if ( (abs(relativeCoord.x) < 3) && (relativeCoord.y == 0) )
  {
    // In line cells
    switch (m_Direction)
    {
    case 0:
      position.x = m_GridCoord.x + relativeCoord.x;
      position.y = m_GridCoord.y;
      break;
    case 1:
      position.x = m_GridCoord.x + relativeCoord.x;
      position.y = m_GridCoord.y + relativeCoord.x;
      break;
    case 2:
      position.x = m_GridCoord.x;
      position.y = m_GridCoord.y + relativeCoord.x;
      break;
    case 3:
      position.x = m_GridCoord.x - relativeCoord.x;
      position.y = m_GridCoord.y + relativeCoord.x;
      break;
    case 4:
      position.x = m_GridCoord.x - relativeCoord.x;
      position.y = m_GridCoord.y;
      break;
    case 5:
      position.x = m_GridCoord.x - relativeCoord.x;
      position.y = m_GridCoord.y - relativeCoord.x;
      break;
    case 6:
      position.x = m_GridCoord.x ;
      position.y = m_GridCoord.y - relativeCoord.x;
      break;
    case 7:
      position.x = m_GridCoord.x + relativeCoord.x;
      position.y = m_GridCoord.y - relativeCoord.x;
      break;
    default:
      position.x = m_GridCoord.x + relativeCoord.x;
      position.y = m_GridCoord.y;
      break;
    }
  }
  else
  {
    // Distant cell: use trigo
    double aRad = (double)m_Direction * CYBIO_PI / 4.0;
    position.x= m_GridCoord.x + cybio_round(cos(aRad)*relativeCoord.x - sin(aRad)*relativeCoord.y);
    position.y= m_GridCoord.y + cybio_round(sin(aRad)*relativeCoord.x + cos(aRad)*relativeCoord.y);
  }

  return position;
}

const Point_t& CBasicEntity::getStepCoord()
{
  return m_StepCoord;
}

Point_t CBasicEntity::getGlobalStepCoord()
{
  return (m_pBiotop != NULL) ? m_pBiotop->getGlobalStepCoordFromStepCoord(m_StepCoord) : m_StepCoord;
}

Point_t CBasicEntity::getStepCoordRelative(const RelativePos_t& relativeCoord)
{
  Point_t position;
  int posX, posY;
  // Distant cell: use trigo
  double aRad = (double)m_StepDirection * CYBIO_PI / 180.0;
  posX = (int)m_StepCoord.x + cybio_round(cos(aRad)*relativeCoord.x - sin(aRad)*relativeCoord.y);
  posY = (int)m_StepCoord.y + cybio_round(sin(aRad)*relativeCoord.x + cos(aRad)*relativeCoord.y);

  // avoid <0 coord
  position.x = (posX < 0) ? 0 : posX;
  position.y = (posY < 0) ? 0 : posY;

  return position;
}

size_t CBasicEntity::getLayer() 
{
  return m_Layer;
}

size_t CBasicEntity::getPrevLayer()
{
  return m_PrevLayer;
}

size_t CBasicEntity::getDefaultLayer()
{
  return m_DefaultLayer;
}

int CBasicEntity::getDirection()
{
  return m_Direction;
}

void CBasicEntity::setDirection(int direction)
{
  int newDirection = direction % 8;
  if (m_Direction != newDirection)
  {
    m_Direction = newDirection; // avoid invalid direction
    m_PrevStepDirection = m_StepDirection;
    m_StepDirection = 45 * m_Direction;
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_MOVED, this);
  }
}

int CBasicEntity::getStepDirection()
{
  return m_StepDirection;
}

void  CBasicEntity::setStepDirection(int stepDirection, bool addMoveEvent)
{
  m_PrevStepDirection = m_StepDirection;
  m_StepDirection = stepDirection % 360;
  if ((addMoveEvent) && (m_StepDirection != m_PrevStepDirection))
  {
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_MOVED, this);
  }
  m_Direction = ((m_StepDirection + 22) / 45) % 8;
}

int CBasicEntity::getPrevStepDirection()
{
  return m_PrevStepDirection;
}

entityIdType CBasicEntity::getId() 
{
  return m_Id;
}

void CBasicEntity::setId(entityIdType id) 
{
  m_Id = id;
}

COLORREF CBasicEntity::getColorRgb() 
{
  return m_ColorRgb;
}

void CBasicEntity::setColor(COLORREF newColor)
{
  if (newColor != m_ColorRgb)
  {
    m_ColorRgb = newColor;
    m_ColorCaracter = convertRgbColorInCaracter(newColor);
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
    computeEntitySignature();
  }
}

FormType_e CBasicEntity::getForm()
{
  return m_Silhouette;
}

void CBasicEntity::setForm(FormType_e newForm)
{
  if (newForm != m_Silhouette)
  {
    m_Silhouette = newForm;
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
    computeEntitySignature();
  }
}
 
TextureType_e CBasicEntity::getTexture()
{
  return m_Texture;
}

void CBasicEntity::setTexture(TextureType_e newText)
{
  if (newText != m_Texture)
  {
    m_Texture = newText;
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
    computeEntitySignature();
  }
}

const string& CBasicEntity::getLabel() 
{
  return m_Label;
}

void CBasicEntity::setLabel(string newname)
{
  m_Label = newname;
}

bool CBasicEntity::isComestible() 
{
  // Entity with protection < 10 % can be eaten anyway, even if still alive
  // Otherwise, entity must be killed before with attack action
  return (getProtection() < 9.9);
}  

bool CBasicEntity::isDrinkable()
{
  return m_bIsDrinkable;  
}

bool CBasicEntity::isAlive()
{
  return m_Status == STATUS_ALIVE;
}  

bool CBasicEntity::isDead()
{
  return m_Status == STATUS_DEAD;
}

void CBasicEntity::setUserControlled(bool active)
{
  m_Control = (active == true) ? CONTROL_LOCAL_USER : CONTROL_LOCAL_AUTO;
}

bool CBasicEntity::isUserControlled()
{
  return m_Control == CONTROL_LOCAL_USER;
}

bool CBasicEntity::isLocalAutoControlled()
{
  return m_Control == CONTROL_LOCAL_AUTO;
}

bool CBasicEntity::isRemoteControlled()
{
  return m_Control == CONTROL_REMOTE_AUTO;
}

void CBasicEntity::setRemoteControlled(bool active)
{
  m_Control = (active == true) ? CONTROL_REMOTE_AUTO : CONTROL_LOCAL_AUTO;
}

double CBasicEntity::getToxicity() 
{
  return getParameterNoCheck(m_id_Toxicity)->getVal();
}

void CBasicEntity::setToxicity(double newRate) 
{
  getParameterNoCheck(m_id_Toxicity)->setVal(newRate);
}

OdorType_e CBasicEntity::getOdor() 
{
  return m_Odor;
}

void CBasicEntity::setOdor(OdorType_e newOdor)
{
  if (newOdor != m_Odor)
  {
    m_Odor = newOdor;
    computeEntitySignature();
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
  }
}

TasteType_e CBasicEntity::getTaste() 
{
  return m_Taste;
}

void CBasicEntity::setTaste(TasteType_e newTaste)
{
  m_Taste = newTaste;
}


PheromoneType_e CBasicEntity::getPheromone()
{
  return m_Pheromone;
}

void CBasicEntity::setPheromone(PheromoneType_e pheroType)
{
  if (pheroType != m_Pheromone)
  {
    m_Pheromone = pheroType;
    computeEntitySignature();
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
  }
}

int CBasicEntity::getGeneration() 
{
  return m_Generation;
}

void CBasicEntity::setGeneration(int newGeneration)
{
  m_Generation = newGeneration;
}

ReproType_e CBasicEntity::getTypeOfReproduction()
{
  return m_TypeOfReproduction;
}

void CBasicEntity::setTypeOfReproduction(ReproType_e newType)
{
  m_TypeOfReproduction = newType;
}

SexType_e CBasicEntity::getSex()
{
  return m_Sex;
}

string CBasicEntity::getSexString()
{
  switch (m_Sex)
  {
  case SEX_NONE:
    return "None";
  case SEX_MALE:
    return "Male";
  case SEX_FEMALE:
    return "Female";
  default:
    return "Unset";
  }
}

CBiotop* CBasicEntity::getBiotop() 
{
  return m_pBiotop;
}

CBrain* CBasicEntity::getBrain() 
{
  return m_pBrain;
}

CGenome* CBasicEntity::getGenome()
{
  return m_pGenome;
}

double CBasicEntity::getNoise()
{
  return getParameterNoCheck(m_id_Noise)->getVal();
}

void CBasicEntity::setNoise(int rate)
{
  getParameterNoCheck(m_id_Noise)->setVal(rate);
}

double CBasicEntity::getWeight()
{
  return getParameterNoCheck(m_id_Weight)->getVal();
}

double CBasicEntity::getMaxWeight()
{
  return getParameterNoCheck(m_id_Weight)->getMax();
}

double CBasicEntity::getMinWeight()
{
  return getParameterNoCheck(m_id_Weight)->getMin();
}

double CBasicEntity::getSizeRate()
{
  // return the scale of the entity compared to its max size
  double rate = 0;
  if (getMaxWeight()>0)
    rate=pow( getWeight()/getMaxWeight(), 0.33333333 );
  return rate;
}

double CBasicEntity::getCamouflage()
{
  if (getCurrentSpeed() == 0)
    return (getParameterNoCheck(m_id_Camouflage)->getVal());
  else
    return (getParameterNoCheck(m_id_Camouflage)->getVal()/2);
}

void CBasicEntity::setCamouflageToNominalRatio(double ratio)
{
  double newCuriosity = getParameterNoCheck(m_id_Camouflage)->getValNominal() * ratio / 100.0;
  getParameterNoCheck(m_id_Camouflage)->setVal(newCuriosity);
}

bool CBasicEntity::checkIfhasMoved()
{
  return m_bHasMoved;
}

bool CBasicEntity::checkIfhasMovedAndClear()
{
  bool resu = m_bHasMoved;
  m_bHasMoved = false;
  return resu;
}

double CBasicEntity::getProtection() 
{
  return getParameterNoCheck(m_id_Protection)->getVal();
}

void CBasicEntity::setProtection(double newRate)
{
  getParameterNoCheck(m_id_Protection)->setVal(newRate);
}

ConsumeType_e CBasicEntity::getConsumeClass()
{
  return m_ConsumeClass;
}

ClassType_e CBasicEntity::getClass()
{
  return (m_pGenome != NULL) ? m_pGenome->getClass() : CLASS_UNSET;
}

double CBasicEntity::changeWeight(double variation)
{
  CGenericParam* pParam = getParameterNoCheck(m_id_Weight);
  double initVal = pParam->getVal();
  pParam->changeVal(variation);
  double resuVariation = pParam->getVal() - initVal;
  if (pParam->isMinReached())
  {
    autoRemove();
  }
  // return effective variation
  return resuVariation;
}

void CBasicEntity::forceWeight(double newWeight)
{
  getParameterNoCheck(m_id_Weight)->forceVal(newWeight);
}

CLifeStage* CBasicEntity::getCurrentLifeStage() 
{
  return (m_indCurrentLifeStage < m_tLifeStage.size()) ? m_tLifeStage[m_indCurrentLifeStage] : NULL;
}

CLifeStage* CBasicEntity::getLifeStage(size_t index)
{ 
  return (index < m_tLifeStage.size()) ? m_tLifeStage[index] : NULL;
}

size_t CBasicEntity::getNbLifeStages()
{
  return m_tLifeStage.size();
}

string CBasicEntity::getColorStrName(ColorCaracterType_e type)
{
  return ColorTypeNameList[type];
}

string CBasicEntity::getOdorStrName(OdorType_e type)
{
  return OdorTypeNameList[type];
}

string CBasicEntity::getTasteStrName(TasteType_e type)
{
  return TasteTypeNameList[type];
}

string CBasicEntity::getPheromoneStrName(PheromoneType_e type)
{
  return PheromoneTypeNameList[type];
}

string CBasicEntity::getFormStrName(FormType_e type)
{
  return FormTypeNameList[type];
}

string CBasicEntity::getTextureStrName(TextureType_e type)
{
  return TextureTypeNameList[type];
}

string CBasicEntity::getPhyAttributeStrName(PhyAttributeType_e type)
{
  return PhyAttributeTypeNameList[type];
}

string CBasicEntity::getHabitatStrName(HabitatType_e type)
{
  return HabitatTypeNameList[type];
}

string CBasicEntity::getConsumeStrName(ConsumeType_e type)
{
  return ConsumeTypeNameList[type];
}

string CBasicEntity::getMoveStrName(MoveType_e type)
{
  return MoveTypeNameList[type];
}

string CBasicEntity::getReproStrName(ReproType_e type)
{
  return ReproTypeNameList[type];
}

string CBasicEntity::getRelativePosStrName(size_t index)
{
  string directionStr;

  switch (index)
  {
  case 0:
    directionStr = "F  ";
    break;
  case 1:
    directionStr = "FL ";
    break;
  case 2:
    directionStr = "FR ";
    break;
  case 3:
    directionStr = "L  ";
    break;
  case 4:
    directionStr = "R  ";
    break;
  case 5:
    directionStr = "BL ";
    break;
  case 6:
    directionStr = "BR ";
    break;
  case 7:
    directionStr = "B  ";
    break;
  default:
    directionStr = "bad index";
    break;
  }

  return directionStr;
}

string CBasicEntity::getAbsolutePosStrName(size_t index)
{
  string directionStr;

  switch (index)
  {
  case 0:
    directionStr = "E  ";
    break;
  case 1:
    directionStr = "NE ";
    break;
  case 2:
    directionStr = "N ";
    break;
  case 3:
    directionStr = "NW  ";
    break;
  case 4:
    directionStr = "W  ";
    break;
  case 5:
    directionStr = "SW ";
    break;
  case 6:
    directionStr = "S ";
    break;
  case 7:
    directionStr = "SE  ";
    break;
  default:
    directionStr = "bad index";
    break;
  }

  return directionStr;
}

void CBasicEntity::setImmortal(bool isImmortal)
{
  m_bIsImmortal = isImmortal;
}

bool CBasicEntity::isImmortal()
{
  return m_bIsImmortal;
}

StatusType_e CBasicEntity::getStatus()
{
  return m_Status;
}

void CBasicEntity::setStatus(StatusType_e newStatus)
{
  if (m_Status != newStatus)
  {
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
    if (newStatus < m_Status)
    {
      CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
      CYBIOCORE_LOG("ENTITY - WARNING entity name %s unexpected status change from %d to %d\n", getLabel().c_str(), m_Status, newStatus);
    }
    m_Status = newStatus;
  }
}

void CBasicEntity::setMarked(bool marked)
{
  m_bIsMarked = marked;
}

bool CBasicEntity::isMarked()
{
  return m_bIsMarked;
}

void CBasicEntity::setHourCounter(int newHour)
{
  m_HourCounter = newHour;
}

CPhysicalWelfare* CBasicEntity::getpPhysicalWelfare()
{
  return m_pPhysicWelfare;
}

string CBasicEntity::getSpecieName()
{
  return (m_pGenome != NULL) ? m_pGenome->getSpecieName() : m_Label;
}

entitySignatureType CBasicEntity::getEntitySignature()
{
  return m_EntitySignature;
}

int CBasicEntity::getCurrentSpeed()
{
  return 0;
}

int  CBasicEntity::getRelativeSpeed(CBasicEntity* pReference)
{
  int relativeSpeed = 0;
  if (m_pBiotop != NULL)
  {
    if (m_pBiotop->isCoordValid(pReference->getGridCoord(), pReference->getLayer()))
    {
      int distInitial = m_pBiotop->getGridDistance(this->getGridCoord(), pReference->getGridCoord());
      RelativePos_t relPos{ (int)getGridPosFromStepPos(getCurrentSpeed()), 0 };
      RelativePos_t relPosRef{ (int)getGridPosFromStepPos(pReference->getCurrentSpeed()), 0 };
      int distFuture = m_pBiotop->getGridDistance(getGridCoordRelative(relPos), pReference->getGridCoordRelative(relPosRef));
      relativeSpeed = distInitial - distFuture;
    }
    else
    {
      // In case of global entity or bad reference, just return own speed
      RelativePos_t relPos{ (int)getGridPosFromStepPos(getCurrentSpeed()), 0 };
      relativeSpeed = m_pBiotop->getGridDistance(getGridCoordRelative(relPos), getGridCoord());
    }
  }
  return relativeSpeed;
}


ColorCaracterType_e CBasicEntity::getColorType()
{
  return m_ColorCaracter;
}

bool CBasicEntity::isPhyAttributePresent(PhyAttributeType_e type)
{
  return m_PhyAttribute.isCaractPresent(AttributeTypeToIndex(type));
}

void CBasicEntity::setAttribute(PhyAttributeType_e newAttrib)
{
  if ((newAttrib>=PHY_ATTRIBUTE_FIRST_TYPE) && !(m_PhyAttribute.isCaractPresent(AttributeTypeToIndex(newAttrib))))
  {
    m_PhyAttribute.setCaractPresent(AttributeTypeToIndex(newAttrib));
    computeEntitySignature();
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
  }
}

void CBasicEntity::removeAttribute(PhyAttributeType_e delAttrib)
{
  if ((delAttrib>=PHY_ATTRIBUTE_FIRST_TYPE) && (m_PhyAttribute.isCaractPresent(AttributeTypeToIndex(delAttrib))))
  {
    m_PhyAttribute.setCaractAbsent(AttributeTypeToIndex(delAttrib));
    computeEntitySignature();
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE, this);
  }
}

PhyAttributeType_e  CBasicEntity::getMainPhyAttribute()
{
  for (PhyAttributeType_e attrib=PHY_ATTRIBUTE_HORNS; attrib<PHY_ATTRIBUTE_NUMBER_TYPE; attrib=(PhyAttributeType_e)((int)attrib+1))
  {
    if (isPhyAttributePresent(attrib))
      return attrib;
  }
  return PHY_ATTRIBUTE_NONE;
}

DWORD CBasicEntity::getAttributePresenceMask()
{
  return m_PhyAttribute.getPresenceMask();
}

void CBasicEntity::setAttributePresenceMask(DWORD mask)
{
  m_PhyAttribute.setPresenceMask(mask);
}

size_t CBasicEntity::getGridPosFromStepPos(size_t stepCoord)
{
  return (stepCoord / NB_STEPS_PER_GRID_SQUARE);
}

bool CBasicEntity::isAnimal()
{
  return false;
}

bool CBasicEntity::isVegetal()
{
  return false;
}

bool CBasicEntity::isMineral()
{
  return false;
}

bool CBasicEntity::isLiving()
{
  return !isMineral();
}