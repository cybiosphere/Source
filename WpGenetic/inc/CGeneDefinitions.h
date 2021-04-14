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
// FILE: CGeneDefinitions.h
//   
// GENERAL DESCRIPTION:
//         This CLASS lists all gene definitions and elements
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined( CGENE_DEFINITIONS_INCLUDED_)
#define CGENE_DEFINITIONS_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//===========================================================================
// Includes 
//===========================================================================
#include "Definitions.h"
#include "CaracterDefinitions.h"
#include <vector>

//===========================================================================
// Definitions            
//===========================================================================

typedef enum 
{
  GENE_DOMINANCE_HIGHEST = 0,
  GENE_DOMINANCE_LOWEST,
  GENE_DOMINANCE_RANDOM,
  GENE_DOMINANCE_NUMBER_TYPE
} GeneDominanceType_e;

typedef struct
{
  string name;
  size_t hexaSize;   // Hexa data size in byte
  size_t hexaOffset; // Hexa data offset in byte
  double valMin;
  double valMax;
  GeneDominanceType_e dominance;
  bool isConfigurable;
  double defaultValue; // Value used for not configurable parameter
} GeneParamDefinition_t;

typedef enum
{
  GENE_GENERIC =     0,
  GENE_CARACTER,
  GENE_PARAMETER,
  GENE_LIFESTAGE,
  GENE_PHY_WELFARE,
  GENE_SENSOR, 
  GENE_REACTION,
  GENE_BRAIN_SIZE,
  GENE_BRAIN_INIT,
  GENE_FEELING,
  GENE_PURPOSE,
  GENE_BRAIN_CONFIG,
  GENE_NUMBER_TYPE
} GeneType_e;

typedef enum
{
  GENE_MUTE_RANDOM_BIT     = 0,// Mutation change 1 bit randomly. Bit weight not taken into account 
  GENE_MUTE_INCREMENTAL_1  = 1,// Mutation can just increase on decrease by block of 1 byte
  GENE_MUTE_INCREMENTAL_2  = 2,// Mutation can just increase on decrease by block of 2 bytes
  GENE_MUTE_NUMBER_TYPE
} GeneMuteType_e;


typedef enum
{        
  GENE_GENERIC_UNKNOWN = 0,
} GeneSubTypeUnset_e;

typedef enum
{
  // Caracteristics
  GENE_CARACT_UNKNOWN = 0,
  GENE_CARACT_COLOR = 1,
  GENE_CARACT_ODOR,
  GENE_CARACT_PHEROMONE,
  GENE_CARACT_TASTE,
  GENE_CARACT_FORM,
  GENE_CARACT_REPRO_TYPE,
  GENE_CARACT_HABITAT,
  GENE_CARACT_CONSUME_TYPE,
  GENE_CARACT_MOVE_TYPE,
  GENE_CARACT_TEXTURE,
  GENE_CARACT_PHY_ATTRIBUTE
} GeneSubTypeCaracter_e;

typedef enum
{
  GENE_PARAM_UNKNOWN = 0,

  GENE_PARAM_WEIGHT = 14, // keep historical value
  GENE_PARAM_TOXICITY,
  GENE_PARAM_PROTECTION,
  GENE_PARAM_CAMOUFLAGE,
  GENE_PARAM_AGE,
  GENE_PARAM_DECOMPOSITION,
  GENE_PARAM_REPRO_RATE,
  GENE_PARAM_RESISTANCE_TO_POISON,
  GENE_PARAM_GROWTH_SPEED,
  GENE_PARAM_REPRO_RANGE,
  GENE_PARAM_FAT_WEIGHT,
  GENE_PARAM_ATTACK_FACTOR,
  GENE_PARAM_SPEED,
  GENE_PARAM_CURIOSITY,
  GENE_PARAM_LEARNING,
  GENE_PARAM_METABOLISM,
  GENE_PARAM_POLLEN_RANGE,
  GENE_PARAM_GESTA_TIME,
  GENE_PARAM_GESTA_NB,
  GENE_PARAM_RESISTANCE_TO_CONSUMPTION
} GeneSubTypeParam_e;

typedef enum
{
  GENE_STAGE_UNKNOWN = 0,

  GENE_STAGE_0 = 35, // keep historical value
  GENE_STAGE_1,
  GENE_STAGE_2,
  GENE_STAGE_3,
  GENE_STAGE_4,
  GENE_STAGE_5,
  GENE_STAGE_6
} GeneSubTypeLifeStage_e;

typedef enum
{
  GENE_PHYS_SENS_UNKNOWN = 0,

  GENE_PHYS_SENS_TEMPER = 42, // keep historical value
  GENE_PHYS_SENS_HABITAT,
  GENE_PHYS_SENS_FERTILITY,
  GENE_PHYS_SENS_SUNLIGHT
} GeneSubTypePhySensi_e;

typedef enum
{
  GENE_SENS_UNKNOWN = 0,

  GENE_SENS_TACTILE = 48, // keep historical value
  GENE_SENS_VIEW,
  GENE_SENS_SMELL,
  GENE_SENS_PHEROMONE,
  GENE_SENS_EAR,
  GENE_SENS_HUNGER,
  GENE_SENS_THIRST,
  GENE_SENS_STOMACH_LOAD,
  GENE_SENS_PLEASURE,
  GENE_SENS_SUFFER,
  GENE_SENS_TIREDNESS,
  GENE_SENS_OBSCURITY,
  GENE_SENS_LIBIDO,
  GENE_SENS_SPEED,
  GENE_SENS_VIEW_FAR,
  GENE_SENS_COMPASS,
  GENE_SENS_TASTE,
  GENE_SENS_FEAR,
  GENE_SENS_TEMPERATURE,
  GENE_SENS_ORIENTATION,
  GENE_SENS_VIEW_IDENTIFY,
  GENE_SENS_VIEW_IDENT_FAR,
  GENE_SENS_HORMONE,
  GENE_SENS_COMPOSITE
} GeneSubTypeSensor_e;

typedef enum
{
  GENE_REACT_UNKNOWN = 0,

  GENE_REACT_NOTHING = 72, // keep historical value
  GENE_REACT_SLEEP,
  GENE_REACT_MOVEFORWARD,
  GENE_REACT_STEPBACKWARD,
  GENE_REACT_TURNLEFT,
  GENE_REACT_TURNRIGHT,
  GENE_REACT_EAT,
  GENE_REACT_DRINK,
  GENE_REACT_COPULATE,
  GENE_REACT_ATTACK,
  GENE_REACT_TURNHEADLEFT,
  GENE_REACT_TURNHEADRIGHT,
  GENE_REACT_RUMINATE,
  GENE_REACT_HIDE
} GeneSubTypeReaction_e;

typedef enum
{
  GENE_BRAIN_SIZE_UNKNOWN = 0,
  GENE_BRAIN_SIZE_TERRITORY,

  GENE_BRAIN_SIZE_HIST_IN = 88, // keep historical value
  GENE_BRAIN_SIZE_HIST_EXP
} GeneSubTypeBrainSize_e;

typedef enum
{
  GENE_BRAIN_LINE_UNKNOWN = 0,

  GENE_BRAIN_LINE = 90, // keep historical value
  GENE_BRAIN_IDENTIFY_LINE = 97 // keep historical value
} GeneSubTypeBrainLine_e;

typedef enum
{
  GENE_FEEL_UNKNOWN = 0,

  GENE_FEEL_WELFARE = 91, // keep historical value
  GENE_FEEL_FEAR
} GeneSubTypeFeeling_e;

typedef enum
{
  GENE_PURPOSE_UNKNOWN = 0,

  GENE_PURPOSE_TRIGGER_UP = 93, // keep historical value
  GENE_PURPOSE_SENSOR,
  GENE_PURPOSE_REACTION,
  GENE_PURPOSE_TRIGGER_DOWN
} GeneSubTypePurpose_e;

typedef enum
{
  GENE_BRAIN_BEHAVIOR_UNKNOWN = 0,

  GENE_BRAIN_BEHAVIOR = 98 // keep historical value
} GeneSubTypeBrainBehavior_e;



//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGeneDefinitions
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data:
//---------------------------------------------------------------------------
public:
  GeneType_e geneType;
  int geneSubType;
  string label;
  GeneMuteType_e muteType;
  std::vector<GeneParamDefinition_t> elements;

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CGeneDefinitions(GeneType_e type, int subType, string name, GeneMuteType_e muteType, std::vector<GeneParamDefinition_t> paramList);
  virtual ~CGeneDefinitions();

  double getElementDefaultValue(size_t index) const;

}; // end CGeneDefinitions

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGeneList
{
  //===========================================================================
  // Attributes 
  //===========================================================================
private:
  static array< std::vector<CGeneDefinitions>, GENE_NUMBER_TYPE> geneListArray;
  
  static std::vector<CGeneDefinitions> geneListGeneric;
  static std::vector<CGeneDefinitions> geneListCaracter;
  static std::vector<CGeneDefinitions> geneListParameter;
  static std::vector<CGeneDefinitions> geneListLifeStage;
  static std::vector<CGeneDefinitions> geneListPhyWellfare;
  static std::vector<CGeneDefinitions> geneListSensor;
  static std::vector<CGeneDefinitions> geneListReaction;
  static std::vector<CGeneDefinitions> geneListBrainSize;
  static std::vector<CGeneDefinitions> geneListBrainInstinct;
  static std::vector<CGeneDefinitions> geneListFeeling;
  static std::vector<CGeneDefinitions> geneListPurpose;
  static std::vector<CGeneDefinitions> geneListBrainConfig;


  //===========================================================================
  // Methods 
  //===========================================================================
public:
  static const CGeneDefinitions* getDefinitions(GeneType_e type, int subType);
  static size_t getNumberOfGeneSubtype(GeneType_e type);
  static const CGeneDefinitions* getDefinitionsByIndex(GeneType_e type, size_t subTypeIndex);
  static string getGeneTypeStrName(GeneType_e type);
  static string getGeneMuteTypeStrName(GeneMuteType_e muteType);

};

#endif // !defined(CGENE_DEFINITIONS_INCLUDED_)



