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
//         This CLASS lists all gene definitions and parameters
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

// Don't forget to update GeneDefinitionList with new gene sub-types !
typedef enum
{
// Subtype    ID            // len // data
  GENE_GENERIC_UNKNOWN  = 0,//  X  // X
// Caracteristics     
  GENE_CARACT_COLOR        ,//  4  // 00 bb gg rr
  GENE_CARACT_ODOR         ,//  1  // enum
  GENE_CARACT_PHEROMONE    ,//  1  // enum  // Note: Can be set in gene as def value, but change dynamically according to entity live
  GENE_CARACT_TASTE        ,//  1  // enum
  GENE_CARACT_FORM         ,//  1  // enum
  GENE_CARACT_REPRO_TYPE   ,//  1  // enum
  GENE_CARACT_HABITAT      ,//  1  // enum
  GENE_CARACT_CONSUME_TYPE ,//  1  // enum
  GENE_CARACT_MOVE_TYPE    ,//  1  // enum
  GENE_CARACT_TEXTURE      ,//  1  // enum
  GENE_CARACT_PHY_ATTRIBUTE,//  1  // enum
  GENE_CARACT_RFU1         ,//  1  // enum
  GENE_CARACT_RFU2         ,//  1  // enum
// Parameters                      // Min    /  Nominal  /  Max  
  GENE_PARAM_WEIGHT        ,//  6  // [1..]  =  [1..]    < [2..1000]
  GENE_PARAM_TOXICITY      ,//  6  // 0         [0..100]   100
  GENE_PARAM_PROTECTION    ,//  6  // 0         [0..100]   100
  GENE_PARAM_CAMOUFLAGE    ,//  6  // 0         [0..100]   100
  GENE_PARAM_AGE           ,//  6  // 0         0          [1..10000]
  GENE_PARAM_DECOMPOSITION ,//  6  // 0         0          [1..1000] 
  GENE_PARAM_REPRO_RATE    ,//  6  // 0         [0..100]   100
  GENE_PARAM_HEALTH        ,//  6  // 0         [0..100]   100
  GENE_PARAM_GROWTH_SPEED  ,//  6  // 0         [0..10000] 10000        // mU growth per day
  GENE_PARAM_REPRO_RANGE   ,//  6  // 0         [0..10]    10
  GENE_PARAM_FAT_WEIGHT    ,//  6  // [0..]  =  [0..]    < [2..1000]
  GENE_PARAM_ATTACK_FACTOR ,//  6  // 0         [0..100]   100
  GENE_PARAM_SPEED         ,//  6  // 0         [0..100]   100
  GENE_PARAM_CURIOSITY     ,//  6  // 0         [0..100]   100
  GENE_PARAM_LEARNING      ,//  6  // 0         [0..100]   100
  GENE_PARAM_METABOLISM    ,//  6  // 0         [0..100]   100  // Obsolete
  GENE_PARAM_POLLEN_RANGE  ,//  6  // 0         [0..100]   100
  GENE_PARAM_GESTA_TIME    ,//  6  // 0         0          [1..1000] 
  GENE_PARAM_GESTA_NB      ,//  6  // [0..100]< [0..100] < [0..100]
  GENE_PARAM_RFU1          ,//  6  // 0         [0..100]   100
  GENE_PARAM_RFU2          ,//  6  // 0         [0..100]   100
// Life Stages
  GENE_STAGE_0             ,//  2  // duration ratio [0.1..100]
  GENE_STAGE_1             ,//  2  // duration ratio [0.1..100]
  GENE_STAGE_2             ,//  2  // duration ratio [0.1..100]
  GENE_STAGE_3             ,//  2  // duration ratio [0.1..100]
  GENE_STAGE_4             ,//  2  // duration ratio [0.1..100]
  GENE_STAGE_5             ,//  2  // duration ratio [0.1..100]
  GENE_STAGE_6             ,//  2  // duration ratio [0.1..100]
// Welfare physical sensitivity       Sensitivity    /  Min    /  Nominal  /  Max  
  GENE_PHYS_SENS_TEMPER    ,//  2  // ratio [0..100]  [-50..50]      X      [-50..50]
  GENE_PHYS_SENS_HABITAT   ,//  2  // ratio [0..100]     X           X          X
  GENE_PHYS_SENS_FERTILITY ,//  2  // ratio [0..100]  [0..100]   [0..100]   [0..100]
  GENE_PHYS_SENS_SUNLIGHT  ,//  2  // ratio [0..100]  [0..100]   [0..100]   [0..100]   // Obsolete
  GENE_PHYS_SENS_RFU1      ,//  2  // ratio [0..100]     X           X          X 
  GENE_PHYS_SENS_RFU2      ,//  2  // ratio [0..100]     X           X          X 
// Sensors                  //         Nb Weight   data1      data2      data3      data4 
  GENE_SENS_TACTILE        ,// N+8 // data2-data1 [-10..10]  [-10..10]     X          X
  GENE_SENS_VIEW           ,// N+8 // data4xSize  [0..100]   [0..10]    [0..4]     [0..10] 
  GENE_SENS_SMELL          ,// N+8 // 7           [0..100]      X          X          X
  GENE_SENS_PHEROMONE      ,// N+8 // N           [0..100]      X          X          X
  GENE_SENS_EAR            ,// N+8 // 8           [0..100]      X          X          X       
  GENE_SENS_HUNGER         ,// N+8 // 1              X          X          X          X
  GENE_SENS_THIRST         ,// N+8 // 1              X          X          X          X
  GENE_SENS_STOMACH_LOAD   ,// N+8 // 1              X          X          X          X
  GENE_SENS_PLEASURE       ,// N+8 // 1              X          X          X          X
  GENE_SENS_SUFFER         ,// N+8 // 1              X          X          X          X
  GENE_SENS_TIREDNESS      ,// N+8 // 1              X          X          X          X
  GENE_SENS_OBSCURITY      ,// N+8 // 1              X          X          X          X
  GENE_SENS_LIBIDO         ,// N+8 // 1              X          X          X          X
  GENE_SENS_SPEED          ,// N+8 // 1              X          X          X          X
  GENE_SENS_VIEW_FAR       ,// N+8 // Nb reac =>!!!  X          X          X          X
  GENE_SENS_COMPASS        ,// N+8 // 8              X          X          X          X
  GENE_SENS_TASTE          ,// N+8 // Nb taste       X          X          X          X
  GENE_SENS_FEAR           ,// N+8 // 1              X          X          X          X
  GENE_SENS_TEMPERATURE    ,// N+8 // 1              X          X          X          X
  GENE_SENS_ORIENTATION    ,// N+8 // 1              X          X          X          X
  GENE_SENS_VIEW_IDENTIFY  ,// N+8 // 1              X          X          X          X
  GENE_SENS_VIEW_IDENT_FAR ,// N+8 // 8              X          X          X          X
  GENE_SENS_HORMONE        ,// N+8 // N              X          X          X          X
  GENE_SENS_COMPOSITE      ,// N+8 // N           [0..0xFF]  [0..0xFF]  [0..100]      X
// Reactions                            //  Success  Failure    data1      data2      data3 
  GENE_REACT_NOTHING       ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_SLEEP         ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_MOVEFORWARD   ,// 10  // [0..100] [0..100]  [-20..20]      X          X
  GENE_REACT_STEPBACKWARD  ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_TURNLEFT      ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_TURNRIGHT     ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_EAT           ,// 10  // [0..100] [0..100]  [-10..10]      X          X
  GENE_REACT_DRINK         ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_COPULATE      ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_ATTACK        ,// 10  // [0..100] [0..100]  [-10..10]      X          X
  GENE_REACT_TURNHEADLEFT  ,// 10  // [0..100] [0..100]  [-10..10]      X          X
  GENE_REACT_TURNHEADRIGHT ,// 10  // [0..100] [0..100]  [-10..10]      X          X
  GENE_REACT_RUMINATE      ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_HIDE          ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_RFU1          ,// 10  // [0..100] [0..100]      X          X          X
  GENE_REACT_RFU2          ,// 10  // [0..100] [0..100]      X          X          X
// Brain size
  GENE_BRAIN_SIZE_HIST_IN  ,//  n  // [1..100]
  GENE_BRAIN_SIZE_HIST_EXP ,//  n  // [1..100]
// Brain Instinct
  GENE_BRAIN_LINE          ,// 1+n // [0..10000] [-1..1]...[-1..1]
// Feeling
  GENE_FEEL_WELFARE        ,//  N  // ratio [0..100] : 0 not sensitive / 100 max sensitivity
  GENE_FEEL_FEAR           ,//  N  // ratio [0..100] : 0 not sensitive / 100 max sensitivity
// Purpose
  GENE_PURPOSE_TRIGGER_UP  ,//  N  //
  GENE_PURPOSE_SENSOR      ,//  N  //
  GENE_PURPOSE_REACTION    ,//  N  //
  GENE_PURPOSE_TRIGGER_DOWN,//  N  //
// Brain Identification
  GENE_BRAIN_IDENTIFY_LINE , // 1+n // [0..10000] [-1..1]...[-1..1]
// Brain Config
  GENE_BRAIN_BEHAVIOR,       //  1  // enum
// End marker
  GENE_NUMBER_SUBTYPE
} GeneSubType_e;


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
  GeneType_e     geneType;
  GeneSubType_e  geneSubType;
  string         label;
  GeneMuteType_e muteType;
  vector<GeneParamDefinition_t> parameters;

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CGeneDefinitions(GeneType_e type, GeneSubType_e subType, string name, GeneMuteType_e muteType, vector<GeneParamDefinition_t> paramList);
  virtual ~CGeneDefinitions();

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
  static array< vector<CGeneDefinitions>, GENE_NUMBER_TYPE> geneListArray;
  
  static vector<CGeneDefinitions> geneListGeneric;
  static vector<CGeneDefinitions> geneListCaracter;
  static vector<CGeneDefinitions> geneListParameter;
  static vector<CGeneDefinitions> geneListLifeStage;
  static vector<CGeneDefinitions> geneListPhyWellfare;
  static vector<CGeneDefinitions> geneListSensor;
  static vector<CGeneDefinitions> geneListReaction;
  static vector<CGeneDefinitions> geneListBrainSize;
  static vector<CGeneDefinitions> geneListBrainInstinct;
  static vector<CGeneDefinitions> geneListFeeling;
  static vector<CGeneDefinitions> geneListPurpose;
  static vector<CGeneDefinitions> geneListBrainConfig;


  //===========================================================================
  // Methods 
  //===========================================================================
public:
  static const CGeneDefinitions* getDefinitions(GeneType_e type, GeneSubType_e subType);
  static size_t getNumberOfGeneSubtype(GeneType_e type);
  static const CGeneDefinitions* getDefinitionsByIndex(GeneType_e type, size_t subTypeIndex);
  static string getGeneTypeStrName(GeneType_e type);
  static string getGeneMuteTypeStrName(GeneMuteType_e muteType);
};

#endif // !defined(CGENE_DEFINITIONS_INCLUDED_)



