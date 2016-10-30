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

// Don't forget to update GeneDefinitionList with new gene sub-types !
// Note: scale set to 0 mean parameter not configurable
// FRED TBD : should be dispatch between all inherited object level
GeneInterpreterDefinition_t GeneDefinitionList [GENE_NUMBER_SUBTYPE] = 
{
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_GENERIC_UNKNOWN
  {                          1.0, "R"        ,                         1.0, "G"        ,                         1.0, "B"        ,                         1.0, "UV"      },// GENE_CARACT_COLOR
  {       ODOR_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_ODOR
  {  PHEROMONE_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_PHEROMONE
  {      TASTE_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_TASTE
  {       FORM_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_FORM
  {  REPRODUCT_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_REPRO_TYPE
  {    HABITAT_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_HABITAT
  {     CONSUM_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_CONSUME_TYPE
  {       MOVE_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_MOVE_TYPE
  {    TEXTURE_NUMBER_TYPE/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_TEXTURE
  {PHY_ATTRIBUTE_NUMBER_TYPE/256.0,"Type"    ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_CARACT_PHY_ATTRIBUTE
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_GENERIC_RFU1
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""        },// GENE_GENERIC_RFU2

  {       MAX_VAL_WEIGHT/65536.0, "Min"      ,      MAX_VAL_WEIGHT/65536.0, "Init"     ,      MAX_VAL_WEIGHT/65536.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_WEIGHT
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_TOXICITY
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_PROTECTION
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_CAMOUFLAGE
  {                          0.0, "Min"      ,             10000.0/65536.0, "Start"    ,             10000.0/65536.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_AGE
  {                          0.0, "Min"      ,                         0.0, ""         ,              1000.0/65536.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_DECOMPOSITION
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_REPRO_RATE
  {                          0.0, "Min"      ,               100.0/65536.0, "Start"    ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_HEALTH
  {                          0.0, "Min"      ,             10000.0/65536.0, "mU day"   ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_GROWTH_SPEED
  {                          0.0, "Min"      ,                10.0/65536.0, "Range"    ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_REPRO_RANGE
  {                          0.0, "Min"      ,      MAX_VAL_WEIGHT/65536.0, "Init"     ,      MAX_VAL_WEIGHT/65536.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_FAT_WEIGHT
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_ATTACK_FACTOR
  {                          0.0, "Min"      ,                         0.0, "Speed"    ,               200.0/65536.0, "Steps sec",                         0.0, ""         },// GENE_PARAM_SPEED
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_CURIOSITY
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_LEARNING
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_METABOLISM
  {                          0.0, "Min"      ,               100.0/65536.0, "Range"    ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_POLLEN_RANGE
  {                          0.0, "Min"      ,                         0.0, "Rate"     ,              1000.0/65536.0, "duration" ,                         0.0, ""         },// GENE_PARAM_GESTA_TIME
  {                100.0/65536.0, "Min"      ,               100.0/65536.0, "Usual"    ,               100.0/65536.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_GESTA_NB
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_REPRO_RFU1
  {                          0.0, "Min"      ,               100.0/65536.0, "Rate"     ,                         0.0, "Max"      ,                         0.0, ""         },// GENE_PARAM_REPRO_RFU2

  {                100.0/65536.0, "Life time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_0
  {                100.0/65536.0, "Life time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_1
  {                100.0/65536.0, "Life time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_2
  {                100.0/65536.0, "Life time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_3
  {                100.0/65536.0, "Life time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_4
  {                100.0/65536.0, "Dead time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_5
  {                100.0/65536.0, "Dead time",                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_STAGE_6

  {                100.0/65536.0, "Rate"     ,               100.0/65536.0, "Min"      ,               100.0/65536.0, "Ideal"    ,               100.0/65536.0, "Max"      },// GENE_PHYS_SENS_TEMPER
  {                100.0/65536.0, "Rate"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_PHYS_SENS_HABITAT
  {                100.0/65536.0, "Rate"     ,               100.0/65536.0, "Min"      ,               100.0/65536.0, "Ideal"    ,               100.0/65536.0, "Max"      },// GENE_PHYS_SENS_FERTILITY
  {                100.0/65536.0, "Rate"     ,               100.0/65536.0, "Min"      ,               100.0/65536.0, "Ideal"    ,               100.0/65536.0, "Max"      },// GENE_PHYS_SENS_SUNLIGHT
  {                100.0/65536.0, "Rate"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_PHYS_SENS_RFU1
  {                100.0/65536.0, "Rate"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_PHYS_SENS_RFU2

  {                 20.0/65536.0, "level min",                20.0/65536.0, "level max",                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_TACTILE
  {                100.0/65536.0, "range"    ,                10.0/65536.0, "layer"    ,                 4.0/65536.0, "angle"    ,                10.0/65536.0, "Nb focus" },// GENE_SENS_VIEW
  {                100.0/65536.0, "range"    ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_SMELL
  {                100.0/65536.0, "range"    ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_PHEROMONE
  {                100.0/65536.0, "range"    ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_EAR
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_HUNGER
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_THIRST
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_STOMACH_LOAD
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_PLEASURE
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_SUFFER
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_TIREDNESS
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_OBSCURITY
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_LIBIDO
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_SPEED
  {                100.0/65536.0, "range min",               100.0/65536.0, "range max",                 4.0/65536.0, "angle"    ,                10.0/65536.0, "Nb focus" },// GENE_SENS_VIEW_FAR
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_COMPASS
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_TASTE
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_FEAR
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_TEMPERATURE
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_ORIENTATION
  {                100.0/65536.0, "range"    ,                10.0/65536.0, "layer"    ,                 4.0/65536.0, "angle"    ,                10.0/65536.0, "Nb focus" },// GENE_SENS_VIEW_IDENTIFY
  {                100.0/65536.0, "range min",               100.0/65536.0, "range max",                 4.0/65536.0, "angle"    ,                10.0/65536.0, "Nb focus" },// GENE_SENS_VIEW_IDENT_FAR
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_SENS_HORMONE
  {                          1.0, "SensUidH" ,                         1.0, "SensUidL" ,              1000.0/65536.0, "paramId"  ,                         0.0, ""         },// GENE_SENS_COMPOSITE

  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_NOTHING
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_SLEEP
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,               200.0/65536.0, "steps sec",                         0.0, ""         },// GENE_REACT_VARYSPEED
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                30.0/65536.0, "steps"    ,                         0.0, ""         },// GENE_REACT_STEPBACKWARD
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_TURNLEFT
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_TURNRIGHT
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                20.0/65536.0, "layer"    ,                         0.0, ""         },// GENE_REACT_EAT
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_DRINK
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_COPULATE
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                20.0/65536.0, "layer"    ,                 3.0/65536.0, "range"    },// GENE_REACT_ATTACK
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_TURNHEADLEFT
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_TURNHEADRIGHT
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_RUMINATE
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_HIDE
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_RFU1
  {                100.0/65536.0, "success"  ,               100.0/65536.0, "failure"  ,                         0.0, ""         ,                         0.0, ""         },// GENE_REACT_RFU2

  {                100.0/65536.0, "Size"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_BRAIN_SIZE_HIST_IN
  {                100.0/65536.0, "Size"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_BRAIN_SIZE_HIST_EXP

  {              10000.0/65536.0, "Line"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_BRAIN_LINE

  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_FEEL_WELFARE
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_FEEL_FEAR

  {               1000.0/65536.0, "time"     ,               100.0/65536.0, "index"    ,               100.0/65536.0, "start"    ,               100.0/65536.0, "stop"     },// GENE_PURPOSE_TRIGGER_UP
  {                          0.0, ""         ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_PURPOSE_SENSOR
  {                200.0/65536.0, "bonus"    ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_PURPOSE_REACTION
  {               1000.0/65536.0, "time"     ,               100.0/65536.0, "index"    ,               100.0/65536.0, "start"    ,               100.0/65536.0, "stop"     },// GENE_PURPOSE_TRIGGER_DOWN
  {              10000.0/65536.0, "Line"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_BRAIN_IDENTIFY_LINE
  {                     10/256.0, "Type"     ,                         0.0, ""         ,                         0.0, ""         ,                         0.0, ""         },// GENE_BRAIN_BEHAVIOR
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
  m_IsAttached  = false;
  m_bHasChanged = false;
  m_bHasMoved   = true;
  m_indCurrentLifeStage = -1;
  m_HourCounter = 0;
  m_Id = -1;
  m_Direction   = 0;
  m_StepDirection   = 0;
  m_Layer = -1;
  m_PrevLayer = -1;
  m_GridCoord.x = -1;
  m_GridCoord.y = -1;
  m_StepCoord.x = -1;
  m_StepCoord.y = -1;
  m_PrevGridCoord.x = -1;
  m_PrevGridCoord.y = -1;
  m_PrevStepCoord.x = -1;
  m_PrevStepCoord.y = -1;
  m_bIsImmortal = false;
  m_bIsDrinkable = false;

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

  m_pPhyAttribute = new CGenericCaract("Attribute");


  // Parameter id pre-init
  m_id_Weight     = -1; 
  m_id_Toxicity   = -1;
  m_id_Protection = -1;   
  m_id_Camouflage = -1;
  m_id_Noise      = -1;

  m_EntitySignature = 0;
  m_Control = CONTROL_LOCAL_AUTO;

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
  
  if (m_pPhyAttribute!= NULL)
    delete m_pPhyAttribute;
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
  int i,j;
  if (m_pGenome==NULL)
  {
    // Set mandatory caracters with default values
    completeCaractsWithDefault();
    // Set mandatory parameters with default values
    completeParamsWithDefault();
    return (true);
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
    for (j=0; j<m_pGenome->getPair(i)->getNumAllele();j++)
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
    for (j=0; j<m_pGenome->getPair(i)->getNumAllele();j++)
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
    for (j=0; j<m_pGenome->getPair(i)->getNumAllele();j++)
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
  }

  resu = false;
  for (i=0; i<m_pGenome->getNumPair();i++)
  {
    for (j=0; j<m_pGenome->getPair(i)->getNumAllele();j++)
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

  return (true);
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
  return (true);
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
    return (false);
  }
  // We are sure Gene is a caracteristic
  bool resu = false;
  BYTE* pData;
  int len = pGen->getData(&pData);
  DWORD scaledVal;
  GeneSubType_e subType = pGen->getGeneSubType();

  // For the moment, only size 1 and 4 are supported
  if (len==sizeof(DWORD))
  {
    scaledVal = ( (BYTE)( (double)(pData[0]) * getGeneScaleData1(subType) ) )
              + ( (BYTE)( (double)(pData[1]) * getGeneScaleData2(subType) ) << 8 )
              + ( (BYTE)( (double)(pData[2]) * getGeneScaleData3(subType) ) << 16 )
              + ( (BYTE)( (double)(pData[3]) * getGeneScaleData4(subType) ) << 24 );
  }
  else if (len==sizeof(BYTE))
  {
    scaledVal = ( (BYTE)( (double)(pData[0]) * getGeneScaleData1(subType) ) );
  }
  else
  {
    return (false);
  }

  switch(subType)
  {
  case GENE_CARACT_COLOR:
    setColor((COLORREF)scaledVal);
    resu = true;
    break;
  case GENE_CARACT_ODOR:
    m_Odor = (OdorType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_PHEROMONE:
    m_Pheromone = (PheromoneType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_TASTE:
    m_Taste = (TasteType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_FORM:
    m_Silhouette = (FormType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_REPRO_TYPE:
    m_TypeOfReproduction = (ReproType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_HABITAT:
    m_Habitat = (HabitatType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_CONSUME_TYPE:
    m_ConsumeClass = (ConsumeType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_MOVE_TYPE:
    m_MoveType = (MoveType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_TEXTURE:
    m_Texture = (TextureType_e)scaledVal;
    resu = true;
    break;
  case GENE_CARACT_PHY_ATTRIBUTE:
    setAttribute((PhyAttributeType_e)scaledVal);
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

  return (true);
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
    return (false);
  }
  // We are sure Gene is a parameter
  bool resu = false;
  WORD* pData;
  int len = pGen->getData((BYTE**)&pData);
  if (len<3*sizeof(WORD))
  {
    // not enought data to config param
    return (false);
  }

  CGenericParam* pParam = NULL;
  double minVal,initVal,maxVal;
  double scaledVal1,scaledVal2,scaledVal3;
  GeneSubType_e subType = pGen->getGeneSubType();

  scaledVal1 = (double)pData[0] * getGeneScaleData1(subType);
  scaledVal2 = (double)pData[1] * getGeneScaleData2(subType);
  scaledVal3 = (double)pData[2] * getGeneScaleData3(subType);

  switch(subType)
  {
  case GENE_PARAM_WEIGHT:
    {
      if (m_id_Weight != -1) delete(getParameter(m_id_Weight)); // delete if already set
      minVal  = scaledVal1;
      initVal = scaledVal2;
      maxVal  = scaledVal3;
      pParam = new CGenericParam(minVal,initVal,initVal,maxVal,"Weight",PARAM_PHYSIC,subType);
      m_id_Weight = addParameter(pParam);
      resu = true;
      break;
    }
  case GENE_PARAM_TOXICITY:
    {
      if (m_id_Toxicity != -1) delete(getParameter(m_id_Toxicity)); // delete if already set
      minVal  = 0.0;
      initVal = scaledVal2;
      maxVal  = 100.0;
      pParam = new CGenericParam(minVal,initVal,initVal,maxVal,"Toxicity rate",PARAM_PHYSIC,subType);
      m_id_Toxicity = addParameter(pParam);
      resu = true;
      break;
    }
  case GENE_PARAM_PROTECTION:
    {
      if (m_id_Protection != -1) delete(getParameter(m_id_Protection)); // delete if already set
      minVal  = 0.0;
      initVal = scaledVal2;
      maxVal  = 100.0;
      pParam = new CGenericParam(minVal,initVal,initVal,maxVal,"Protection factor",PARAM_PHYSIC,subType);
      m_id_Protection = addParameter(pParam);
      resu = true;
      break;
    }
  case GENE_PARAM_CAMOUFLAGE:
    {
      if (m_id_Camouflage != -1) delete(getParameter(m_id_Camouflage)); // delete if already set
      minVal  = 0.0;
      initVal = scaledVal2;
      maxVal  = 100.0;
      pParam = new CGenericParam(minVal,initVal,initVal,maxVal,"Camouflage factor",PARAM_PHYSIC,subType);
      m_id_Camouflage = addParameter(pParam);
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
  return (resu);
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
  if (m_id_Weight == -1)
  {
    CGenericParam* pParam = new CGenericParam(1,2,2,20,"Weight",PARAM_PHYSIC,GENE_PARAM_WEIGHT);
    m_id_Weight           = addParameter(pParam);
  }
  if (m_id_Toxicity == -1)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,100,"Toxicity rate",PARAM_PHYSIC,GENE_PARAM_TOXICITY);
    m_id_Toxicity         = addParameter(pParam);
  }
  if (m_id_Protection == -1)
  {
    CGenericParam* pParam = new CGenericParam(0,5,5,100,"Protection factor",PARAM_PHYSIC,GENE_PARAM_PROTECTION);
    m_id_Protection       = addParameter(pParam);
  }
  if (m_id_Camouflage == -1)
  {
    CGenericParam* pParam = new CGenericParam(0,1,1,100,"Camouflage factor",PARAM_PHYSIC,GENE_PARAM_CAMOUFLAGE);
    m_id_Camouflage       = addParameter(pParam);
  }
  if (m_id_Noise == -1)
  {
    CGenericParam* pParam = new CGenericParam(0,0,0,100,"NoiseRate",PARAM_ENVIRONMENT,GENE_GENERIC_UNKNOWN);
    m_id_Noise       = addParameter(pParam);
  }
  
  return (true);
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
  return (false);
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
  return (true);
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
  return (false);
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
  return (true);
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
    return(false);

  m_pBrain->AttachSensor(pSensor);
  return(true);
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
  return (NULL);
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
int  CBasicEntity::getExpectedBrainSensorWeightSize (CGene* pGen)
{
  return (0);
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
  return (false);
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
  return (true);
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
  return (true);
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
  return (false);
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
  return (false);
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
  return (false);
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
  return (true);
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
  return (false);
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
  return (false);
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

  return (descStr);
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
    return (caractStr);
  }
  // We are sure Gene is a caracteristic
  BYTE* pData;
  int len = pGen->getData(&pData);
  if (len<1)
  {
    return (caractStr);
  }

  GeneSubType_e subType = pGen->getGeneSubType();
  BYTE data1 = (BYTE)( (double)(pData[0]) * getGeneScaleData1(subType) );

  switch (subType)
  {
  case GENE_CARACT_COLOR:
    {
      if (len>=sizeof(DWORD))
      {
        tempStr = FormatString("=%03d ", (BYTE)( (double)(pData[0]) * getGeneScaleData1(subType) ) );
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
        tempStr = FormatString("=%03d ", (BYTE)( (double)(pData[1]) * getGeneScaleData2(subType) ) );
        caractStr += getGeneNameData2(subType) + tempStr;
        tempStr = FormatString("=%03d ", (BYTE)( (double)(pData[2]) * getGeneScaleData3(subType) ) );
        caractStr += getGeneNameData3(subType) + tempStr;
        tempStr = FormatString("=%03d ", (BYTE)( (double)(pData[3]) * getGeneScaleData4(subType) ) );
        caractStr += getGeneNameData4(subType) + tempStr;
        COLORREF rgbCol = (COLORREF)pData[0] + (COLORREF)(pData[1]<<8) + (COLORREF)(pData[2]<<16);
        tempStr = " ("; tempStr += ColorTypeNameList[convertRgbColorInCaracter(rgbCol)]; tempStr +=")";
        caractStr += tempStr;
      }
      break;
    }
  case GENE_CARACT_ODOR:
    {
      if ( (data1>-1) && (data1<ODOR_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += OdorTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_PHEROMONE:
    {
      if ( (data1>-1) && (data1<PHEROMONE_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += PheromoneTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_TASTE:
    {
      if ( (data1>-1) && (data1<TASTE_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += TasteTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_FORM:
    {
      if ( (data1>-1) && (data1<FORM_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += FormTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_REPRO_TYPE:
    {
      if ( (data1>-1) && (data1<REPRODUCT_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += ReproTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_HABITAT:
    {
      if ( (data1>-1) && (data1<HABITAT_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += HabitatTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_CONSUME_TYPE:
    {
      if ( (data1>-1) && (data1<CONSUM_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += ConsumeTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_MOVE_TYPE:
    {
      if ( (data1>-1) && (data1<MOVE_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += MoveTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_TEXTURE:
    {
      if ( (data1>-1) && (data1<TEXTURE_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += TextureTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
      }
      break;
    }
  case GENE_CARACT_PHY_ATTRIBUTE:
    {
      if ( (data1>-1) && (data1<PHY_ATTRIBUTE_NUMBER_TYPE) )
      {
        tempStr = "="; tempStr += PhyAttributeTypeNameList[data1]; tempStr += " ";
        caractStr = getGeneNameString(pGen) + " : " + getGeneNameData1(subType) + tempStr;
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
    return (paramStr);
  }
  // We are sure Gene is a parameter
  WORD* pData;
  int len = pGen->getData((BYTE**)&pData);
  if (len<3*sizeof(WORD))
  {
    // not enought data to config param
    return (paramStr);
  }

  CGenericParam* pParam = NULL;
  double scaledVal1,scaledVal2,scaledVal3;
  GeneSubType_e subType = pGen->getGeneSubType();

  scaledVal1 = (double)pData[0] * getGeneScaleData1(subType);
  scaledVal2 = (double)pData[1] * getGeneScaleData2(subType);
  scaledVal3 = (double)pData[2] * getGeneScaleData3(subType);

  switch(subType)
  {
  case GENE_PARAM_WEIGHT:
  case GENE_PARAM_TOXICITY:
  case GENE_PARAM_PROTECTION:
  case GENE_PARAM_CAMOUFLAGE:
    {
      paramStr = getGeneNameString(pGen) + " : ";
      if (getGeneScaleData1(subType)!=0)
      {
        tempStr = FormatString("=%6.2f ", scaledVal1 );
        paramStr += getGeneNameData1(subType) + tempStr;
      }
      if (getGeneScaleData2(subType)!=0)
      {
        tempStr = FormatString("=%6.2f ", scaledVal2 );
        paramStr += getGeneNameData2(subType) + tempStr;
      }
      if (getGeneScaleData3(subType)!=0)
      {
        tempStr = FormatString("=%6.2f ", scaledVal3 );
        paramStr += getGeneNameData3(subType) + tempStr;
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
  string defStr = STRING_GENE_UNUSED;
  return(defStr);
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
  return(defStr);
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
  return(defStr);
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
  return(defStr);
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
  return(defStr);
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
  return(defStr);
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
  return(defStr);
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
  return(defStr);
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
  return(defStr);
}


string CBasicEntity::getGeneNameString(CGene* pGen)
{
  string name = pGen->getLabel();
  return(name);
}

string CBasicEntity::getGeneNameData1(GeneSubType_e subType)
{
  string name = GeneDefinitionList[subType].data1Name;
  return(name);
}

string CBasicEntity::getGeneNameData2(GeneSubType_e subType)
{
  string name = GeneDefinitionList[subType].data2Name;
  return(name);
}

string CBasicEntity::getGeneNameData3(GeneSubType_e subType)
{
  string name = GeneDefinitionList[subType].data3Name;
  return(name);
}

string CBasicEntity::getGeneNameData4(GeneSubType_e subType)
{
  string name = GeneDefinitionList[subType].data4Name;
  return(name);
}

double CBasicEntity::getGeneScaleData1 (GeneSubType_e subType)
{
  return(GeneDefinitionList[subType].data1Scale);
}

double CBasicEntity::getGeneScaleData2 (GeneSubType_e subType)
{
  return(GeneDefinitionList[subType].data2Scale);
}

double CBasicEntity::getGeneScaleData3 (GeneSubType_e subType)
{
  return(GeneDefinitionList[subType].data3Scale);
}

double CBasicEntity::getGeneScaleData4 (GeneSubType_e subType)
{
  return(GeneDefinitionList[subType].data4Scale);
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
void CBasicEntity::attachToBiotop(CBiotop* pBiotop) 
{   
  m_pBiotop = pBiotop;

  if (m_pBiotop != NULL)
  {
    m_IsAttached = true;
    m_pBiotop->updateGridEntity(this);
  }
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
  m_IsAttached = false;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::isToBeRemoved
//  
// DESCRIPTION:  Check if entity is not up to be deleted 
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CBasicEntity::isToBeRemoved()
{
  if (m_Status == STATUS_TOBEREMOVED)
    return (true);
  else 
    return (false);
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
  return (m_IsAttached);
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
void CBasicEntity::autoRemove()
{
  m_Status = STATUS_TOBEREMOVED;
  Point_t coord = {-1,-1};
  jumpToGridCoord(coord, -1);

  if (m_pBiotop!=NULL)
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_REMOVED, this);

  // To avoid problem until biotop removal (TBC)
  m_Id = -1; 
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
  return (m_tParam.size()-1);
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
unsigned int CBasicEntity::getNumParameter()
{
  return(m_tParam.size());
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
CGenericParam* CBasicEntity::getParameter(unsigned int id)
{
  if ( (id<0) || (id>m_tParam.size()) )
  {
    return (NULL);
  }
  else
  {
    return (m_tParam[id]);
  }
}

CGenericParam* CBasicEntity::getParameterByName(string paramName)
{
  for (unsigned int id=0; id<m_tParam.size(); id++)
  {
    if (m_tParam[id]->getLabel() == paramName)
      return (m_tParam[id]);
  }

  return (NULL);
}

int CBasicEntity::getParamIdByName(string paramName)
{
  for (unsigned int id=0; id<m_tParam.size(); id++)
  {
    if (m_tParam[id]->getLabel() == paramName)
      return (id);
  }

  return (-1);
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
  // loop from top to bottom 
  for (int i=m_tParam.size()-1; i>=0; i--) 
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
// DESCRIPTION:  Secondary action trigger called by biotop every 3600 time units
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CBasicEntity::nextHour()
{
  m_HourCounter ++;
  if (m_HourCounter > 24)
  {
    nextDay();
    m_HourCounter = 0;
  }
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::nextDay
//  
// DESCRIPTION:  Living day trigger called by CBasicEntity every 24 hours
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CBasicEntity::nextDay(bool forceGrowth)
{
  if (m_indCurrentLifeStage>-1)
  {
    CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
    if (pLifeStage->increaseDayCountAndCheckEnd())
    {
      if (m_indCurrentLifeStage<(m_tLifeStage.size()-1))
      {
        m_indCurrentLifeStage++;
        pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
        enterInNewLifeStage(pLifeStage);
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
  return (0);
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
  return (true);
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
  unsigned int index = 0;
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

  return (resu);
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
  // loop from top to bottom 
  for (int i=m_tLifeStage.size()-1; i>=0; i--) 
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
  bool resu = false;
  for (unsigned int i=0; i<m_tLifeStage.size(); i++)
  {
    if (newStage == m_tLifeStage[i]->getStageType())
    {
      m_indCurrentLifeStage = i;
      CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
      enterInNewLifeStage(pLifeStage);
      m_bHasChanged = true;
      resu = true;
    }
  }
  return (resu);
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
  return (0);
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
  choiceIndType autoChoice = -1;
  return (autoChoice);
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

  Point_t relPos = {nbSteps,0};
  Point_t newStepCoord = getStepCoordRelative(relPos);
  Point_t newGridCoord;

  newGridCoord.x = newStepCoord.x / NB_STEPS_PER_GRID_SQUARE;
  newGridCoord.y = newStepCoord.y / NB_STEPS_PER_GRID_SQUARE; 

  int nbGridMove = max( abs(newGridCoord.x - m_GridCoord.x), abs(newGridCoord.y - m_GridCoord.y) );

  Point_t nextGridCoord = m_GridCoord;
  bool resu = true;
  for (int i=0; i<nbGridMove; i++)
  {
    if (newGridCoord.x > nextGridCoord.x)
       nextGridCoord.x++;
    else if (newGridCoord.x < nextGridCoord.x)
       nextGridCoord.x--; 

    if (newGridCoord.y > nextGridCoord.y)
       nextGridCoord.y++;
    else if (newGridCoord.y < nextGridCoord.y)
       nextGridCoord.y--; 

    resu = jumpToGridCoord(nextGridCoord);

    if (!resu)
      break;
  }

  if (resu)
  {
    // No trouble found: goto exact coord
    m_StepCoord = newStepCoord;
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
bool CBasicEntity::jumpToGridCoord(Point_t newGridCoord, int newLayer)
{
  bool resu;

  if (m_pBiotop==NULL)
  {
    m_PrevGridCoord.x = -1;
    m_PrevGridCoord.y = -1;
    m_PrevStepCoord.x = -1;
    m_PrevStepCoord.y = -1;
    m_GridCoord = newGridCoord;
    m_bHasMoved  = true;
    m_bHasChanged = true;
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_PrevLayer = -1; 
    if (newLayer>-2)
    {    
      m_Layer = newLayer;
    }
    resu = true;
  }
  else if ( m_pBiotop->isCoordValidAndFree(newGridCoord,m_Layer)  // valid
         || ( (newGridCoord.x == -1) && (newGridCoord.y == -1) )  // or out
         || (m_Layer == -1) )
  {
    // newCoord valid ... Move
    m_PrevGridCoord = m_GridCoord;
    m_PrevStepCoord = m_StepCoord;
    m_GridCoord = newGridCoord;
    m_bHasMoved  = true;
    m_bHasChanged = true;
    m_StepCoord.x = m_GridCoord.x * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_StepCoord.y = m_GridCoord.y * NB_STEPS_PER_GRID_SQUARE + NB_STEPS_PER_GRID_SQUARE/2; // center in square
    m_PrevLayer = m_Layer; 
    if (newLayer>-2)
    { 
      m_Layer = newLayer;
    }    
    resu = true;
    m_pBiotop->addBiotopEvent(BIOTOP_EVENT_ENTITY_MOVED, this);
    m_pBiotop->updateGridEntity(this);
  }
  else
  {
    // newCoord not valid... Don't move
    resu = false;
  }
  return (resu);
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
bool CBasicEntity::jumpToStepCoord(Point_t newStepCoord, int newLayer)
{
  // Update prev step coord
  m_PrevStepCoord = m_StepCoord;

  // Update grid coord if needed
  Point_t newGridCoord;
  newGridCoord.x = newStepCoord.x / NB_STEPS_PER_GRID_SQUARE;
  newGridCoord.y = newStepCoord.y / NB_STEPS_PER_GRID_SQUARE;
  if ( (m_GridCoord.x != newGridCoord.x) || (m_GridCoord.y != newGridCoord.y) )
  {
    if (!jumpToGridCoord(newGridCoord, newLayer))
      return (false);
  }

  // Update new step coord
  m_StepCoord = newStepCoord;

  return (true);
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
  return (true);
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

  return (true);
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
  return (true);
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
bool CBasicEntity::turnRight(unsigned int nDegree)
{
  m_PrevStepDirection = m_StepDirection;
  m_StepDirection += 360 - nDegree;
  m_StepDirection = m_StepDirection%360;
  m_Direction = ((m_StepDirection+22)/45)%8;
  m_bHasChanged = true;
  return (true);
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
bool CBasicEntity::turnLeft(unsigned int nDegree)
{
  m_PrevStepDirection = m_StepDirection;
  m_StepDirection += nDegree;
  m_StepDirection = m_StepDirection%360;
  m_Direction = ((m_StepDirection+22)/45)%8;
  m_bHasChanged = true;
  return (true);
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
    m_Status = STATUS_DEAD;
    m_bHasChanged = true;

    if (!checkHabitat())
      autoRemove();

    setCurrentLifeStages(STAGE_5);
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
  bool resu = false;

  LayerType_e curLayer = m_pBiotop->getLayerType(m_GridCoord,m_Layer);

  if ( (m_Habitat == HABITAT_EARTH) && (curLayer != LAYER_OVER_GROUND) && (curLayer != LAYER_OVER_WET_GROUND) )
  {
    resu = false;
  }
  else if ( (m_Habitat == HABITAT_WATER) && (curLayer != LAYER_UNDER_WATER) )
  {
    resu = false;
  }
  else
  {
    resu = true;
  }

  return (resu);
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
  // loop from top to bottom 
  for (int i=m_tGestationChilds.size()-1; i>=0; i--) 
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
    int titi = 10;
  }

  TiXmlDocument *pXmlDoc = new TiXmlDocument(fileName);
  saveInXmlFile(pXmlDoc, newLabel);
  resu = pXmlDoc->SaveFile();
  delete pXmlDoc;
  return resu;
}


bool CBasicEntity::saveInXmlFile(TiXmlDocument *pXmlDoc, string newLabel)
{
  bool resu = false;
  TiXmlElement* pElement;
  TiXmlNode* pNodeEntity = NULL;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeChild = NULL;
  string tempLabel;
  int i;

  if (pXmlDoc==NULL) 
    return false;

  pNodeEntity = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if (pNodeEntity==NULL)
  {
    TiXmlElement newNode(XML_NODE_ENTITY);
    pNodeEntity = pXmlDoc->InsertEndChild(newNode);
  }

  if (newLabel == "")
    tempLabel = m_Label;
  else
    tempLabel = newLabel;

  if ((pNodeEntity != NULL) && (pNodeEntity->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNodeEntity;
    pElement->SetAttribute( XML_ATTR_LABEL,      tempLabel);
    pElement->SetAttribute( XML_ATTR_GENERATION, m_Generation);
    pElement->SetAttribute( XML_ATTR_STATUS,     m_Status);
    pElement->SetAttribute( XML_ATTR_LAYER,      m_Layer);
    pElement->SetAttribute( XML_ATTR_DIRECTION,  m_Direction);
    pElement->SetAttribute( XML_ATTR_HOUR_COUNT, m_HourCounter);
    pElement->SetAttribute( XML_ATTR_IMMORTAL,   (int)m_bIsImmortal);
  }

  pNode = pNodeEntity->FirstChild(XML_NODE_LIFE_STAGES);
  if (pNode==NULL)
  {
    // Create Life stages node
    TiXmlElement newNode(XML_NODE_LIFE_STAGES);
    pNode = pNodeEntity->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous Life stages
    while (pNode->FirstChild() != NULL)
      pNode->RemoveChild(pNode->FirstChild());
  }

  if ((pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNode;
    pElement->SetAttribute( XML_ATTR_CURRENT_IND, m_indCurrentLifeStage);

    for (i=0; i<getNbLifeStages(); i++)
    {
      TiXmlElement newLifeStage(XML_NODE_LIFE_STAGE);
      pNodeChild = pNode->InsertEndChild(newLifeStage);
      if (pNodeChild != NULL) 
      {
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_INDEX, i);
        pElement->SetAttribute(XML_ATTR_DAY_COUNT, getLifeStage(i)->getCurDayCount());
      }
    }
  }

  // Clear previous parameters
  while (pNodeEntity->FirstChild(XML_NODE_PARAMETER) != NULL)
    pNodeEntity->RemoveChild(pNodeEntity->FirstChild(XML_NODE_PARAMETER));

  // Save parameters
  for (i=0; i<getNumParameter(); i++)
  {
    TiXmlElement newParam(XML_NODE_PARAMETER);
    pNodeChild = pNodeEntity->InsertEndChild(newParam);
    if (pNodeChild != NULL) 
    {
      pElement = (TiXmlElement*)pNodeChild;
      pElement->SetAttribute(XML_ATTR_NAME, getParameter(i)->getLabel());
      pElement->SetDoubleAttribute(XML_ATTR_VALUE, getParameter(i)->getVal());
    }
  }

  // Clear previous caracters
  while (pNodeEntity->FirstChild(XML_NODE_CARACTER) != NULL)
    pNodeEntity->RemoveChild(pNodeEntity->FirstChild(XML_NODE_CARACTER));

  // Set caracters
  TiXmlElement newCaract(XML_NODE_CARACTER);
  newCaract.SetAttribute(XML_ATTR_NAME, "ColorRGB");
  newCaract.SetAttribute(XML_ATTR_VALUE, m_ColorRgb);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Odor");
  newCaract.SetAttribute(XML_ATTR_VALUE, (int)m_Odor);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Pheromone");
  newCaract.SetAttribute(XML_ATTR_VALUE, (int)m_Pheromone);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Form");
  newCaract.SetAttribute(XML_ATTR_VALUE,(int)m_Silhouette);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Texture");
  newCaract.SetAttribute(XML_ATTR_VALUE, (int)m_Texture);
  pNodeEntity->InsertEndChild(newCaract);
  newCaract.SetAttribute(XML_ATTR_NAME, "Attribute");
  newCaract.SetAttribute(XML_ATTR_VALUE, m_pPhyAttribute->getPresenceMask());
  pNodeEntity->InsertEndChild(newCaract);

  // Gestation
  pNode = pNodeEntity->FirstChild(XML_NODE_GESTATION);
  if (pNode==NULL)
  {
    // Create gestation node
    TiXmlElement newNode(XML_NODE_GESTATION);
    pNode = pNodeEntity->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous childs
    while (pNode->FirstChild() != NULL)
      pNode->RemoveChild(pNode->FirstChild());
  }

  if ((pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNode;
    pElement->SetAttribute(XML_ATTR_TOTAL_CHILDS, m_TotalChildNumber);

    // Save childs
    for (i=0; i<m_tGestationChilds.size(); i++)
    {
      CBasicEntity* pBaby = m_tGestationChilds[i];
      string babyFileName = pBaby->getLabel() + ".xml";
      pBaby->saveInXmlFile(babyFileName);
      TiXmlElement newBaby(XML_NODE_ENTITY);
      pNodeChild = pNode->InsertEndChild(newBaby);
      if (pNodeChild != NULL) 
      {
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_FILE_NAME, babyFileName);
      }
    }
  }

  if (m_pGenome!=NULL)
  {
    m_pGenome->saveInXmlFile(pXmlDoc);
  }

  if (m_pBrain!=NULL)
  {
    m_pBrain->saveInTiXmlFile(pXmlDoc);
  }

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
bool CBasicEntity::getEntityNameFromXmlFile(TiXmlDocument *pXmlDoc, string& name)
{
  if (pXmlDoc==NULL)
    return false;

  TiXmlNode* pNode = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if((pNode!=NULL)&&(pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    TiXmlElement* pElement = (TiXmlElement*)pNode;
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
//               stringt& name: result
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::loadDataFromXmlFile(string fileName, string pathNameForBabies)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileName);
  resu = xmlDoc.LoadFile();
  loadDataFromXmlFile(&xmlDoc, pathNameForBabies);
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CBasicEntity::loadDataFromXmlFile
//  
// DESCRIPTION:  Load entity data
// 
// ARGUMENTS:    string fileNameWithPath
//               stringt& name: result
//   
// RETURN VALUE: bool: success
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CBasicEntity::loadDataFromXmlFile(TiXmlDocument *pXmlDoc, string pathNameForBabies)
{
  TiXmlElement* pElement;
  TiXmlNode* pNode = NULL;

  if (pXmlDoc==NULL)
    return false;

  TiXmlNode* pNodeEntity = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if ((pNodeEntity != NULL) && (pNodeEntity->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeEntity;
    int direction, status, immortal;

    if ( pElement->QueryStringAttribute(XML_ATTR_LABEL,  &m_Label) == TIXML_NO_ATTRIBUTE)
      m_Label = "Unset";

    if ( pElement->QueryIntAttribute(XML_ATTR_GENERATION,  &m_Generation) == TIXML_NO_ATTRIBUTE)
      m_Generation = 0;

    if ( pElement->QueryIntAttribute(XML_ATTR_STATUS,  &status) == TIXML_NO_ATTRIBUTE)
      status = 0;
    m_Status = (StatusType_e)status;

    if ( pElement->QueryIntAttribute(XML_ATTR_DIRECTION,  &direction) == TIXML_NO_ATTRIBUTE)
      direction = 0;
    setDirection(direction);

    if ( pElement->QueryIntAttribute(XML_ATTR_HOUR_COUNT,  &m_HourCounter) == TIXML_NO_ATTRIBUTE)
      m_HourCounter = 0;

    if ( pElement->QueryIntAttribute(XML_ATTR_IMMORTAL,  &immortal) == TIXML_NO_ATTRIBUTE)
      immortal = 0;

    if (immortal!=0)
      m_bIsImmortal = true;

    // Life stage management
    int prevLifeStage = m_indCurrentLifeStage;
    pNode = pNodeEntity->FirstChild(XML_NODE_LIFE_STAGES);
    if ((pNode!=NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
    {
      pElement = (TiXmlElement*)pNode;
      if ( pElement->QueryIntAttribute(XML_ATTR_CURRENT_IND,  &m_indCurrentLifeStage) == TIXML_NO_ATTRIBUTE)
        m_indCurrentLifeStage = 0;

      if ( (m_indCurrentLifeStage != prevLifeStage) && (getNbLifeStages()>0) && (m_indCurrentLifeStage<getNbLifeStages()) )
      {
        CLifeStage* pLifeStage = m_tLifeStage[m_indCurrentLifeStage];
        enterInNewLifeStage(pLifeStage);
      }

      int index, dayCount;
      pNode = pNode->FirstChild();
      while (pNode != NULL)
      {
        if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_LIFE_STAGE))
        {
          pElement = (TiXmlElement*)pNode;
          if ( pElement->QueryIntAttribute(XML_ATTR_INDEX,  &index) == TIXML_NO_ATTRIBUTE)
            index = -1;
          if ( pElement->QueryIntAttribute(XML_ATTR_DAY_COUNT,  &dayCount) == TIXML_NO_ATTRIBUTE)
            dayCount = 0;
          if ((index>-1) && (index<getNbLifeStages()))
            getLifeStage(index)->setCurDayCount(dayCount);
        }
        pNode = pNode->NextSibling();
      }
    }

    // Parameters management
    string paramName;
    double paramVal;
    CGenericParam* pParam;
    pNode = pNodeEntity->FirstChild(XML_NODE_PARAMETER);
    while (pNode != NULL)
    {
      if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_PARAMETER))
      {
        pElement = (TiXmlElement*)pNode; 
        if ( pElement->QueryDoubleAttribute(XML_ATTR_VALUE,  &paramVal) == TIXML_NO_ATTRIBUTE)
          paramVal = 0;
        
        if ( pElement->QueryStringAttribute(XML_ATTR_NAME,  &paramName) != TIXML_NO_ATTRIBUTE)
        {
          pParam = getParameterByName(paramName);
          if (pParam != NULL)
            pParam->setVal(paramVal);
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
            m_pPhyAttribute->setPresenceMask((DWORD)atoi(caractVal.c_str())); 
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

      // Load babies
      CGenome* pBabyGenome;
      CBasicEntity* pEntity;
      string fileStr, babName;
      pNode = pNode->FirstChild(XML_NODE_ENTITY);
      while (pNode != NULL)
      {
        pElement = (TiXmlElement*)pNode;
        if ((pElement!=NULL) &&  (pElement->QueryStringAttribute(XML_ATTR_FILE_NAME, &fileStr) != TIXML_NO_ATTRIBUTE))
        { 
          string fileNameWithPathStr = pathNameForBabies + fileStr;
          pBabyGenome = new CGenome(CLASS_NONE,"");
          getGenomeFromXmlFile(fileNameWithPathStr,*pBabyGenome);
          getEntityNameFromXmlFile(fileNameWithPathStr,babName);    
          pEntity = m_pBiotop->createEntity(babName,pBabyGenome);
          if (pEntity)
          {
            pEntity->loadBrainFromXmlFile(fileNameWithPathStr);
            pEntity->loadDataFromXmlFile(fileNameWithPathStr, pathNameForBabies);
            m_tGestationChilds.push_back(pEntity);
          }
        }
        pNode = pNode->NextSibling();
      }
    }
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
  bool resu = genome.loadFromXmlFile(fileName);
  return resu;
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
  bool resu = genome.loadFromXmlFile(pXmlDoc);
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
  if (m_pBrain == NULL)
    return false;

  bool resu = m_pBrain->loadFromTiXmlFile(pXmlDoc);

  return (resu);
}

//===========================================================================
// Get / Set for attributes
//===========================================================================
    
Point_t CBasicEntity::getGridCoord()
{
  return (m_GridCoord);
}

Point_t CBasicEntity::getPrevGridCoord()
{
  return (m_PrevGridCoord);
}

Point_t CBasicEntity::getPrevStepCoord()
{
  return (m_PrevStepCoord);
}

Point_t CBasicEntity::getGuiGridCoord()
{
  return (m_GuiGridCoord);
}

Point_t CBasicEntity::getGridCoordRelative(Point_t relativeCoord)
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

  return (position);
}

Point_t CBasicEntity::getStepCoord()
{
  return (m_StepCoord);
}

Point_t CBasicEntity::getStepCoordRelative(Point_t relativeCoord)
{
  Point_t position;
 
  // Distant cell: use trigo
  double aRad = (double)m_StepDirection * CYBIO_PI / 180.0;
  position.x= m_StepCoord.x + cybio_round(cos(aRad)*relativeCoord.x - sin(aRad)*relativeCoord.y);
  position.y= m_StepCoord.y + cybio_round(sin(aRad)*relativeCoord.x + cos(aRad)*relativeCoord.y);

  // avoid <0 coord
  if (position.x < 0)
    position.x = 0;
  if (position.y < 0)
    position.y = 0;

  return (position);
}

int CBasicEntity::getLayer() 
{
  return (m_Layer);
}

int CBasicEntity::getPrevLayer() 
{
  return (m_PrevLayer);
}

int CBasicEntity::getDirection()
{
  return (m_Direction);
}

void CBasicEntity::setDirection(int direction)
{
  m_Direction = direction%8; // avoid invalid direction
  m_PrevStepDirection = m_StepDirection;
  m_StepDirection = 45*m_Direction;
  m_bHasChanged = true;
}

int CBasicEntity::getStepDirection()
{
  return (m_StepDirection);
}

void  CBasicEntity::setStepDirection(int stepDirection)
{
  m_Direction = (stepDirection/45)%8; // avoid invalid direction
  m_PrevStepDirection = m_StepDirection;
  m_StepDirection = stepDirection;
  m_bHasChanged = true;
}

int CBasicEntity::getPrevStepDirection()
{
  return (m_PrevStepDirection);
}

entityIdType CBasicEntity::getId() 
{
  return (m_Id);
}

void CBasicEntity::setId(entityIdType id) 
{
  m_Id = id;
}

COLORREF CBasicEntity::getColorRgb() 
{
  return (m_ColorRgb);
}

void CBasicEntity::setColor(COLORREF newColor)
{
  m_ColorRgb = newColor;
  m_ColorCaracter = convertRgbColorInCaracter(newColor);
  m_bHasChanged = true;
  computeEntitySignature();
}

FormType_e CBasicEntity::getForm()
{
  return (m_Silhouette);
}

void CBasicEntity::setForm(FormType_e newForm)
{
  m_Silhouette = newForm;
  m_bHasChanged = true;
  computeEntitySignature();
}
 
TextureType_e CBasicEntity::getTexture()
{
  return (m_Texture);
}

void CBasicEntity::setTexture(TextureType_e newText)
{
  m_Texture = newText;
  m_bHasChanged = true;
  computeEntitySignature();
}

string CBasicEntity::getLabel() 
{
  return (m_Label);
}

bool CBasicEntity::setLabel(string newname)
{
  m_Label = newname;
  return (true);
}

bool CBasicEntity::isComestible() 
{
  bool resu;
  // Entity with protection < 10 % can be eaten anyway, even if still alive
  // Otherwise, entity must be killed before with attack action
  if (getProtection()<10)
  {
    resu = true;
  }
  else
  {
    resu = false;
  }
  return (resu);
}  

bool CBasicEntity::isDrinkable()
{
  return (m_bIsDrinkable);  
}

bool CBasicEntity::isAlive()
{
  if (m_Status == STATUS_ALIVE)
    return (true);
  else 
    return (false);
}  

void CBasicEntity::setUserControlled(bool active)
{
  if (active == true)
    m_Control = CONTROL_LOCAL_USER;
  else 
    m_Control = CONTROL_LOCAL_AUTO;
}

bool CBasicEntity::isUserControlled()
{
  if (m_Control == CONTROL_LOCAL_USER)
    return (true);
  else 
    return (false);
}

bool CBasicEntity::isLocalAutoControlled()
{
  if (m_Control == CONTROL_LOCAL_AUTO)
    return (true);
  else 
    return (false);
}

void CBasicEntity::setRemoteControlled(bool active)
{
  if (active == true)
    m_Control = CONTROL_REMOTE_AUTO;
  else 
    m_Control = CONTROL_LOCAL_AUTO;
}


double CBasicEntity::getToxicity() 
{
  return (getParameter(m_id_Toxicity)->getVal());
}

void CBasicEntity::setToxicity(double newRate) 
{
  getParameter(m_id_Toxicity)->setVal(newRate);
}

OdorType_e CBasicEntity::getOdor() 
{
  return (m_Odor);
}

TasteType_e CBasicEntity::getTaste() 
{
  return (m_Taste);
}

void CBasicEntity::setOdor(OdorType_e newOdor)
{
  m_Odor = newOdor;
  computeEntitySignature();
}

PheromoneType_e CBasicEntity::getPheromone()
{
  return (m_Pheromone);
}

void CBasicEntity::setPheromone(PheromoneType_e pheroType)
{
  m_Pheromone = pheroType;
  computeEntitySignature();
}

int CBasicEntity::getGeneration() 
{
  return (m_Generation);
}

ReproType_e CBasicEntity::getTypeOfReproduction()
{
  return (m_TypeOfReproduction);
}

SexType_e CBasicEntity::getSex()
{
  return (m_Sex);
}

CBiotop* CBasicEntity::getBiotop() 
{
  return (m_pBiotop);
}

CBrain* CBasicEntity::getBrain() 
{
  return (m_pBrain);
}

CGenome* CBasicEntity::getGenome()
{
  return (m_pGenome);
}

double CBasicEntity::getNoise()
{
  return (getParameter(m_id_Noise)->getVal());
}

void CBasicEntity::setNoise(int rate)
{
  getParameter(m_id_Noise)->setVal(rate);
}

double CBasicEntity::getWeight()
{
  return (getParameter(m_id_Weight)->getVal());
}

double CBasicEntity::getMaxWeight()
{
  return (getParameter(m_id_Weight)->getMax());
}

double CBasicEntity::getMinWeight()
{
  return (getParameter(m_id_Weight)->getMin());
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
    return (getParameter(m_id_Camouflage)->getVal());
  else
    return (getParameter(m_id_Camouflage)->getVal()/2);
}

void CBasicEntity::setCamouflageToNominalRatio(double ratio)
{
  double newCuriosity = getParameter(m_id_Camouflage)->getValNominal() * ratio / 100.0;
  getParameter(m_id_Camouflage)->setVal(newCuriosity);
}

bool CBasicEntity::checkIfhasMoved()
{
  return (m_bHasMoved);
}

bool CBasicEntity::checkIfhasChanged()
{
  return (m_bHasChanged);
}

bool CBasicEntity::checkIfhasMovedAndClear()
{
  bool resu = m_bHasMoved;
  m_bHasMoved = false;
  return (resu);
}

bool CBasicEntity::checkIfhasChangedAndClear()
{
  bool resu = m_bHasChanged;
  m_GuiGridCoord = m_GridCoord;
  m_bHasChanged = false;
  return (resu);
}

void  CBasicEntity::forceHasChanged()
{
  m_bHasChanged = true;
}

double CBasicEntity::getProtection() 
{
  return (getParameter(m_id_Protection)->getVal());
}

void CBasicEntity::setProtection(double newRate)
{
  getParameter(m_id_Protection)->setVal(newRate);
}

ConsumeType_e CBasicEntity::getConsumeClass()
{
  return (m_ConsumeClass);
}

ClassType_e CBasicEntity::getClass()
{
  if (m_pGenome!=NULL)
  {
    return (m_pGenome->getClass());
  }
  else
  {
    return (CLASS_UNSET);
  }
}

double CBasicEntity::changeWeight(double variation)
{
  CGenericParam* pParam = getParameter(m_id_Weight);
  double initVal;
  double resuVariation;

  initVal = pParam->getVal();
  pParam->changeVal(variation);
  resuVariation = pParam->getVal() - initVal;

  if (pParam->isMinReached())
  {
    autoRemove();
  }
  
  // return effective variation
  return (resuVariation);
}

void CBasicEntity::forceWeight(double newWeight)
{
  CGenericParam* pParam = getParameter(m_id_Weight);
  pParam->forceVal(newWeight);
  if (pParam->isMinReached())
  {
    autoRemove();
  }
}

CLifeStage* CBasicEntity::getCurrentLifeStage() 
{
  if ( (-1<m_indCurrentLifeStage) && (m_indCurrentLifeStage<m_tLifeStage.size()) )
    return (m_tLifeStage[m_indCurrentLifeStage]);
  else
    return (NULL);
}

CLifeStage* CBasicEntity::getLifeStage(int index)
{ 
  if ( (index>-1) && (index < m_tLifeStage.size()) )
    return (m_tLifeStage[index]);
  else
    return (NULL);
}

unsigned int CBasicEntity::getNbLifeStages()
{
  return (m_tLifeStage.size());
}

string CBasicEntity::getColorStrName(ColorCaracterType_e type)
{
  return (ColorTypeNameList[type]);
}

string CBasicEntity::getOdorStrName(OdorType_e type)
{
  return (OdorTypeNameList[type]);
}

string CBasicEntity::getTasteStrName(TasteType_e type)
{
  return (TasteTypeNameList[type]);
}

string CBasicEntity::getPheromoneStrName(PheromoneType_e type)
{
  return (PheromoneTypeNameList[type]);
}

string CBasicEntity::getFormStrName(FormType_e type)
{
  return (FormTypeNameList[type]);
}

string CBasicEntity::getTextureStrName(TextureType_e type)
{
  return (TextureTypeNameList[type]);
}

string CBasicEntity::getPhyAttributeStrName(PhyAttributeType_e type)
{
  return (PhyAttributeTypeNameList[type]);
}

string CBasicEntity::getHabitatStrName(HabitatType_e type)
{
  return (HabitatTypeNameList[type]);
}

string CBasicEntity::getConsumeStrName(ConsumeType_e type)
{
  return (ConsumeTypeNameList[type]);
}

string CBasicEntity::getMoveStrName(MoveType_e type)
{
  return (MoveTypeNameList[type]);
}

string CBasicEntity::getReproStrName(ReproType_e type)
{
  return (ReproTypeNameList[type]);
}

string CBasicEntity::getRelativePosStrName(int index)
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

string CBasicEntity::getAbsolutePosStrName(int index)
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
  return (m_bIsImmortal);
}

StatusType_e CBasicEntity::getStatus()
{
  return m_Status;
}

void CBasicEntity::setStatus(StatusType_e newStatus)
{
  if (m_Status != newStatus)
    m_bHasChanged = true;
  m_Status = newStatus;
}

CPhysicalWelfare* CBasicEntity::getpPhysicalWelfare()
{
  return(m_pPhysicWelfare);
}

string CBasicEntity::getSpecieName()
{
  if (m_pGenome!=NULL)
    return(m_pGenome->getSpecieName());
  else
    return(m_Label);
}

entitySignatureType CBasicEntity::getEntitySignature()
{
  return m_EntitySignature;
}

int CBasicEntity::getCurrentSpeed()
{
  return(0);
}

ColorCaracterType_e CBasicEntity::getColorType()
{
  return(m_ColorCaracter);
}

bool CBasicEntity::isPhyAttributePresent(PhyAttributeType_e type)
{
  return(m_pPhyAttribute->isCaractPresent(type-PHY_ATTRIBUTE_FIRST_TYPE));
}

void CBasicEntity::setAttribute(PhyAttributeType_e newAttrib)
{
  if (newAttrib>=PHY_ATTRIBUTE_FIRST_TYPE)
  {
    m_pPhyAttribute->setCaractPresent(newAttrib-PHY_ATTRIBUTE_FIRST_TYPE);
    computeEntitySignature();
  }
}

void CBasicEntity::removeAttribute(PhyAttributeType_e delAttrib)
{
  if (delAttrib>=PHY_ATTRIBUTE_FIRST_TYPE)
  {
    m_pPhyAttribute->setCaractAbsent(delAttrib-PHY_ATTRIBUTE_FIRST_TYPE);
    computeEntitySignature();
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
  return m_pPhyAttribute->getPresenceMask();
}

Point_t CBasicEntity::getGridCoordFromStepCoord(Point_t stepCoord)
{
  Point_t gridCoord;
  gridCoord.x = stepCoord.x / NB_STEPS_PER_GRID_SQUARE; 
  gridCoord.y = stepCoord.y / NB_STEPS_PER_GRID_SQUARE;
  return gridCoord;
}