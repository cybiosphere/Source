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
// FILE: CGene.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a basic gene
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================
 
#include "CGeneDefinitions.h"

//===========================================================================
// Definitions            
//===========================================================================

const char* GeneTypeNameList[GENE_NUMBER_TYPE] =
{
  "Generic",
  "Caracter",
  "Parameter",
  "Lifestage",
  "Physical wellfare",
  "Sensor",
  "Reaction",
  "Brain size",
  "Brain instinct",
  "Feeling component",
  "Purpose",
  "Brain config"
};

const char* GeneMuteTypeNameList[GENE_MUTE_NUMBER_TYPE] =
{
  "Random bit",
  "Incremental 1 byte",
  "Incremental 2 bytes"
};

vector<CGeneDefinitions> CGeneList::geneListGeneric =
{
  { GENE_GENERIC, GENE_GENERIC_UNKNOWN, "generic", GENE_MUTE_RANDOM_BIT, { } }
};

vector<CGeneDefinitions> CGeneList::geneListCaracter =
{
  { GENE_CARACTER, GENE_CARACT_COLOR, "Color", GENE_MUTE_RANDOM_BIT,
  { { "R", 1, 0, 0, 255, GENE_DOMINANCE_HIGHEST, true, 0 },
    { "G", 1, 1, 0, 255, GENE_DOMINANCE_HIGHEST, true, 0 },
    { "B", 1, 2, 0, 255, GENE_DOMINANCE_HIGHEST, true, 0 },
    { "A", 1, 3, 0, 255, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_ODOR, "Odor", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, ODOR_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_PHEROMONE, "Pheromone", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, PHEROMONE_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_TASTE, "Taste", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, TASTE_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_FORM, "Form", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, FORM_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_REPRO_TYPE, "Reproduction", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, REPRODUCT_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_HABITAT, "Habitat", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, HABITAT_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_CONSUME_TYPE, "Consumption", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, CONSUM_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_MOVE_TYPE, "Move mode", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, MOVE_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_TEXTURE, "Texture", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, TEXTURE_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_CARACTER, GENE_CARACT_PHY_ATTRIBUTE, "Physical attribute", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, PHY_ATTRIBUTE_NUMBER_TYPE, GENE_DOMINANCE_HIGHEST, true, 0 } } }
};

vector<CGeneDefinitions> CGeneList::geneListParameter =
{
  { GENE_PARAMETER, GENE_PARAM_WEIGHT, "Weight", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 1 },
      { "Init", 2, 2, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 2 },
      { "Max", 2, 4, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 10 } } },

  { GENE_PARAMETER, GENE_PARAM_TOXICITY, "Toxicity", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_PROTECTION, "Protection", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 5 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_CAMOUFLAGE, "Camouflage", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 1 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_AGE, "Age", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 10000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Start", 2, 2, 0, 10000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Max", 2, 4, 0, 10000, GENE_DOMINANCE_HIGHEST, true, 10000 } } },

  { GENE_PARAMETER, GENE_PARAM_DECOMPOSITION, "Decomposition", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Start", 2, 2, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Max", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, true, 10000 } } },

  { GENE_PARAMETER, GENE_PARAM_REPRO_RATE, "Reproduction rate", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 10 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_RESISTANCE_TO_POISON, "Resistance to poison", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_GROWTH_SPEED, "Growth speed", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "mU day", 2, 2, 0, 1000, GENE_DOMINANCE_HIGHEST, true, 1 },
      { "Max", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 1000 } } },

  { GENE_PARAMETER, GENE_PARAM_REPRO_RANGE, "Reproduction range", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 10, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Range", 2, 2, 0, 10, GENE_DOMINANCE_HIGHEST, true, 2 },
      { "Max", 2, 4, 0, 10, GENE_DOMINANCE_HIGHEST, false, 10 } } },

  { GENE_PARAMETER, GENE_PARAM_FAT_WEIGHT, "Fat weight", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Init", 2, 2, 0.1, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0.1, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 1 } } },

  { GENE_PARAMETER, GENE_PARAM_ATTACK_FACTOR, "Attack", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 10 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_SPEED, "Speed", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 200, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Speed", 2, 2, 0, 200, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Steps sec", 2, 4, 0, 200, GENE_DOMINANCE_HIGHEST, true, 10 } } },

  { GENE_PARAMETER, GENE_PARAM_CURIOSITY, "Curiosity rate", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 10 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_LEARNING, "Learning rate", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 100 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_POLLEN_RANGE, "Pollen range", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Range", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 2 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_GESTA_TIME, "Gestation time", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Start", 2, 2, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Duration", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, true, 2 } } },

  { GENE_PARAMETER, GENE_PARAM_GESTA_NB, "Gestation size", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Usual", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 1 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, true, 2 } } },

  { GENE_PARAMETER, GENE_PARAM_RESISTANCE_TO_CONSUMPTION, "Resistance to consumption", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 60 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } }
};


vector<CGeneDefinitions> CGeneList::geneListLifeStage =
{
  { GENE_LIFESTAGE, GENE_STAGE_0, "Life stage 0", GENE_MUTE_INCREMENTAL_2,
    { { "Life time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_LIFESTAGE, GENE_STAGE_1, "Life stage 1", GENE_MUTE_INCREMENTAL_2,
    { { "Life time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_LIFESTAGE, GENE_STAGE_2, "Life stage 2", GENE_MUTE_INCREMENTAL_2,
    { { "Life time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_LIFESTAGE, GENE_STAGE_3, "Life stage 3", GENE_MUTE_INCREMENTAL_2,
    { { "Life time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_LIFESTAGE, GENE_STAGE_4, "Life stage 4", GENE_MUTE_INCREMENTAL_2,
    { { "Life time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_LIFESTAGE, GENE_STAGE_5, "Dead stage 5", GENE_MUTE_INCREMENTAL_2,
    { { "Dead time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_LIFESTAGE, GENE_STAGE_6, "Dead stage 6", GENE_MUTE_INCREMENTAL_2,
    { { "Dead time", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true , 0} } }
};

vector<CGeneDefinitions> CGeneList::geneListPhyWellfare =
{
  { GENE_PHY_WELFARE, GENE_PHYS_SENS_TEMPER, "Sensitivity temperature", GENE_MUTE_INCREMENTAL_2,
    { { "Rate", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Min", 2, 2, -50, 50, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Ideal", 2, 4, -50, 50, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 6, -50, 50, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_PHY_WELFARE, GENE_PHYS_SENS_HABITAT, "Sensitivity habitat", GENE_MUTE_INCREMENTAL_2,
    { { "Rate", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Min", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Ideal", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Max", 2, 6, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 } } },

  { GENE_PHY_WELFARE, GENE_PHYS_SENS_FERTILITY, "Sensitivity fertility", GENE_MUTE_INCREMENTAL_2,
    { { "Rate", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Min", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Ideal", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 6, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } }
};

vector<CGeneDefinitions> CGeneList::geneListSensor =
{
  { GENE_SENSOR, GENE_SENS_TACTILE, "Tactile", GENE_MUTE_INCREMENTAL_2,
    { { "Level min", 2, 0, -10, 10, GENE_DOMINANCE_HIGHEST, true },
      { "Level max", 2, 2, -10, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_VIEW, "View", GENE_MUTE_INCREMENTAL_2,
    { { "Range", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Layer", 2, 2, 0, 10, GENE_DOMINANCE_HIGHEST, true },
      { "Angle", 2, 4, 0, VIEW_ANGLE_360, GENE_DOMINANCE_HIGHEST, true },
      { "Nb focus", 2, 6, 0, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_SMELL, "Smell", GENE_MUTE_INCREMENTAL_2,
    { { "Range", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_PHEROMONE, "Pheromone detector", GENE_MUTE_INCREMENTAL_2,
    { { "Range", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_EAR, "Ear", GENE_MUTE_INCREMENTAL_2,
    { { "Range", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_HUNGER, "Hunger", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_THIRST, "Thirst", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_STOMACH_LOAD, "Stomach overload", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_PLEASURE, "Pleasure", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_SUFFER, "Suffer", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_TIREDNESS, "Tiredness", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_OBSCURITY, "Obscurity", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_LIBIDO, "Libido", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_SPEED, "Speed", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_VIEW_FAR, "View far", GENE_MUTE_INCREMENTAL_2,
    { { "Range min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Range max", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Angle", 2, 4, 0, VIEW_ANGLE_360, GENE_DOMINANCE_HIGHEST, true },
      { "Nb focus", 2, 6, 0, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_SPEED, "Speed", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_TASTE, "Taste", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_COMPASS, "Compass", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_FEAR, "Fear", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_TEMPERATURE, "Temperature", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_ORIENTATION, "Orientation", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_VIEW_IDENTIFY, "View identify", GENE_MUTE_INCREMENTAL_2,
    { { "Range", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Layer", 2, 2, 0, 10, GENE_DOMINANCE_HIGHEST, true },
      { "Angle", 2, 4, 0, VIEW_ANGLE_360, GENE_DOMINANCE_HIGHEST, true },
      { "Nb focus", 2, 6, 0, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_VIEW_IDENT_FAR, "View identify far", GENE_MUTE_INCREMENTAL_2,
    { { "Range min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Range max", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Angle", 2, 4, 0, VIEW_ANGLE_360, GENE_DOMINANCE_HIGHEST, true },
      { "Nb focus", 2, 6, 0, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_SENSOR, GENE_SENS_HORMONE, "Hormone", GENE_MUTE_INCREMENTAL_2, { } },

  { GENE_SENSOR, GENE_SENS_COMPOSITE, "Composite", GENE_MUTE_INCREMENTAL_2,
    { { "Sens Uid Hi", 2, 0, 0, 0xFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Sens Uid Lo", 2, 2, 0, 0xFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "ParamId", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, false } } }
};

vector<CGeneDefinitions> CGeneList::geneListReaction =
{
  { GENE_REACTION, GENE_REACT_NOTHING, "Nothing", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_SLEEP, "Sleep", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_MOVEFORWARD, "Move forward", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Steps sec", 2, 4, 0, 200, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_STEPBACKWARD, "Step back", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Steps", 2, 4, 0, 30, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_TURNLEFT, "Turn left", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_TURNRIGHT, "Turn right", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_EAT, "Eat", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Layer", 2, 4, -10, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_DRINK, "Drink", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_COPULATE, "Copulate", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_ATTACK, "Attack", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Layer", 2, 4, -10, 10, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_TURNHEADLEFT, "Turn head left", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_TURNHEADRIGHT, "Turn head right", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_RUMINATE, "Ruminate", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_REACTION, GENE_REACT_HIDE, "Hide", GENE_MUTE_INCREMENTAL_2,
    { { "Success", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Failure", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true } } }
};

vector<CGeneDefinitions> CGeneList::geneListBrainSize =
{
  { GENE_BRAIN_SIZE, GENE_BRAIN_SIZE_TERRITORY, "Territory size", GENE_MUTE_INCREMENTAL_2,
    { { "Size", 2, 0, 0, 5000, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_BRAIN_SIZE, GENE_BRAIN_SIZE_HIST_IN, "History input", GENE_MUTE_INCREMENTAL_2,
    { { "Size", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_BRAIN_SIZE, GENE_BRAIN_SIZE_HIST_EXP, "History experience", GENE_MUTE_INCREMENTAL_2,
    { { "Size", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true } } }
};

vector<CGeneDefinitions> CGeneList::geneListBrainInstinct =
{
  { GENE_BRAIN_INIT, GENE_BRAIN_LINE, "Brain line", GENE_MUTE_INCREMENTAL_2,
    { { "Line", 2, 0, 0, 10000, GENE_DOMINANCE_HIGHEST, false } } },

  { GENE_BRAIN_INIT, GENE_BRAIN_IDENTIFY_LINE, "Identification line", GENE_MUTE_INCREMENTAL_2,
    { { "Line", 2, 0, 0, 10000, GENE_DOMINANCE_HIGHEST, false } } }
};

vector<CGeneDefinitions> CGeneList::geneListFeeling =
{
  { GENE_FEELING, GENE_FEEL_WELFARE, "Wellfare feeling", GENE_MUTE_INCREMENTAL_2,
    { { "Sensor ", 4, 0, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Sensivity", 2, 4, -100, 100, GENE_DOMINANCE_HIGHEST, false } } },

  { GENE_FEELING, GENE_FEEL_FEAR, "Fear feeling", GENE_MUTE_INCREMENTAL_2,
    { { "Sensor ", 4, 0, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Sensivity", 2, 4, -100, 100, GENE_DOMINANCE_HIGHEST, false } } }
};

vector<CGeneDefinitions> CGeneList::geneListPurpose =
{
  { GENE_PURPOSE, GENE_PURPOSE_TRIGGER_UP, "Purpose trigger up", GENE_MUTE_INCREMENTAL_2,
    { { "Time", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, true },
      { "Index", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Start", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Stop", 2, 6, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Sensor ", 4, 8, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false } } },

  { GENE_PURPOSE, GENE_PURPOSE_SENSOR, "Sensor bonus", GENE_MUTE_INCREMENTAL_2,
    { { "Purpose ", 4, 0, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Sensor ", 4, 4, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Bonus", 2, 8, 0, 20000, GENE_DOMINANCE_HIGHEST, false } } },

  { GENE_PURPOSE, GENE_PURPOSE_REACTION, "Reaction bonus", GENE_MUTE_INCREMENTAL_2,
    { { "Purpose ", 4, 0, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Reaction ", 4, 4, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false },
      { "Bonus", 2, 8, 0, 200, GENE_DOMINANCE_HIGHEST, true } } },

  { GENE_PURPOSE, GENE_PURPOSE_TRIGGER_DOWN, "Purpose trigger down", GENE_MUTE_INCREMENTAL_2,
    { { "Time", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, true },
      { "Index", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Start", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Stop", 2, 6, 0, 100, GENE_DOMINANCE_HIGHEST, true },
      { "Sensor ", 4, 8, 0, 0xFFFFFFFF, GENE_DOMINANCE_HIGHEST, false } } },
};

vector<CGeneDefinitions> CGeneList::geneListBrainConfig =
{
  { GENE_BRAIN_CONFIG, GENE_BRAIN_BEHAVIOR, "Behavior", GENE_MUTE_INCREMENTAL_1,
    { { "Type", 1, 0, 0, 10, GENE_DOMINANCE_HIGHEST, true } } }
};

array< vector<CGeneDefinitions>, GENE_NUMBER_TYPE> CGeneList::geneListArray =
{ 
  CGeneList::geneListGeneric , 
  CGeneList::geneListCaracter ,
  CGeneList::geneListParameter ,
  CGeneList::geneListLifeStage,
  CGeneList::geneListPhyWellfare,
  CGeneList::geneListSensor,
  CGeneList::geneListReaction,
  CGeneList::geneListBrainSize,
  CGeneList::geneListBrainInstinct,
  CGeneList::geneListFeeling,
  CGeneList::geneListPurpose,
  CGeneList::geneListBrainConfig
};



//===========================================================================
//                          CGeneDefinitions            
//===========================================================================

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGeneDefinitions::CGeneDefinitions()
//  
// DESCRIPTION:  Default constructor
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      To be used before SetAs...
//---------------------------------------------------------------------------
CGeneDefinitions::CGeneDefinitions(GeneType_e type, int subType, string name, GeneMuteType_e muteType, vector<GeneParamDefinition_t> elementList):
 geneType{type}, geneSubType{subType}, label{name}, muteType{muteType}, elements{ elementList }
{
}

CGeneDefinitions::~CGeneDefinitions()
{
}

double CGeneDefinitions::getElementDefaultValue(size_t index) const
{
  double defaultVal = 0;
  if (index < elements.size())
  {
    defaultVal = elements[index].defaultValue;
  }
  return defaultVal;
}


//===========================================================================
//                          CGeneList            
//===========================================================================

const CGeneDefinitions* CGeneList::getDefinitions(GeneType_e type, int subType)
{
  if (type < geneListArray.size())  // can be removed later...
  {
    for (CGeneDefinitions& def : geneListArray[type])
    {
      if (def.geneSubType == subType)
        return (&def);
    }
  }
  return (&geneListArray[GENE_GENERIC][0]);
}

size_t CGeneList::getNumberOfGeneSubtype(GeneType_e type)
{
  size_t nbSubType = 0;
  if (type < geneListArray.size())  // can be removed later...
  {
    nbSubType = geneListArray[type].size();
  }
  return nbSubType;
}

const CGeneDefinitions* CGeneList::getDefinitionsByIndex(GeneType_e type, size_t subTypeIndex)
{
  if (type < geneListArray.size())  // can be removed later...
  {
    if (subTypeIndex < geneListArray[type].size())
    {
        return (&geneListArray[type][subTypeIndex]);
    }
  }
  return (&geneListArray[GENE_GENERIC][0]);
}

string CGeneList::getGeneTypeStrName(GeneType_e type)
{
  string typeName;
  typeName = GeneTypeNameList[type];
  return(typeName);
}

string CGeneList::getGeneMuteTypeStrName(GeneMuteType_e muteType)
{
  string typeName;
  typeName = GeneMuteTypeNameList[muteType];
  return(typeName);
}
