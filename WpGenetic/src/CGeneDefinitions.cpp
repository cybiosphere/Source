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
/*
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

GeneDefinition_t GeneSubTypeNameList[GENE_NUMBER_SUBTYPE] =
{
  { GENE_GENERIC     , "generic"                },
  { GENE_CARACTER    , "color"                  },
  { GENE_CARACTER    , "odor"                   },
  { GENE_CARACTER    , "pheromone"              },
  { GENE_CARACTER    , "taste"                  },
  { GENE_CARACTER    , "form"                   },
  { GENE_CARACTER    , "reproduction"           },
  { GENE_CARACTER    , "habitat"                },
  { GENE_CARACTER    , "consumption"            },
  { GENE_CARACTER    , "move mode"              },
  { GENE_CARACTER    , "texture"                },
  { GENE_CARACTER    , "physical attribute"     },
  { GENE_CARACTER    , "RFU1"                   },
  { GENE_CARACTER    , "RFU2"                   },
  { GENE_PARAMETER   , "weight"                 },
  { GENE_PARAMETER   , "toxicity"               },
  { GENE_PARAMETER   , "protection"             },
  { GENE_PARAMETER   , "camouflage"             },
  { GENE_PARAMETER   , "age"                    },
  { GENE_PARAMETER   , "decomposition"          },
  { GENE_PARAMETER   , "repro rate"             },
  { GENE_PARAMETER   , "born health"            },
  { GENE_PARAMETER   , "growth speed"           },
  { GENE_PARAMETER   , "reproduction range"     },
  { GENE_PARAMETER   , "fat weight"             },
  { GENE_PARAMETER   , "attack"                 },
  { GENE_PARAMETER   , "speed"                  },
  { GENE_PARAMETER   , "curiosity"              },
  { GENE_PARAMETER   , "learning"               },
  { GENE_PARAMETER   , "metabolism"             },
  { GENE_PARAMETER   , "pollen range"           },
  { GENE_PARAMETER   , "gestation time"         },
  { GENE_PARAMETER   , "baby number"            },
  { GENE_PARAMETER   , "RFU1"                   },
  { GENE_PARAMETER   , "RFU2"                   },
  { GENE_LIFESTAGE   , "life stage 0"           },
  { GENE_LIFESTAGE   , "life stage 1"           },
  { GENE_LIFESTAGE   , "life stage 2"           },
  { GENE_LIFESTAGE   , "life stage 3"           },
  { GENE_LIFESTAGE   , "life stage 4"           },
  { GENE_LIFESTAGE   , "life stage 5"           },
  { GENE_LIFESTAGE   , "life stage 6"           },
  { GENE_PHY_WELFARE , "sensitivity temperature"},
  { GENE_PHY_WELFARE , "sensitivity habitat"    },
  { GENE_PHY_WELFARE , "sensitivity fertility"  },
  { GENE_PHY_WELFARE , "sensitivity sunlight"   },
  { GENE_PHY_WELFARE , "RFU1"                   },
  { GENE_PHY_WELFARE , "RFU2"                   },
  { GENE_SENSOR      , "tactile"                },
  { GENE_SENSOR      , "view"                   },
  { GENE_SENSOR      , "smell"                  },
  { GENE_SENSOR      , "pheromone detector"     },
  { GENE_SENSOR      , "ear"                    }, // GENE_SENS_EAR
  { GENE_SENSOR      , "hunger"                 }, // GENE_SENS_HUNGER
  { GENE_SENSOR      , "thirst"                 }, // GENE_SENS_THIRST
  { GENE_SENSOR      , "stomach overload"       }, // GENE_SENS_STOMACH_LOAD
  { GENE_SENSOR      , "pleasure"               }, // GENE_SENS_PLEASURE
  { GENE_SENSOR      , "suffer"                 }, // GENE_SENS_SUFFER
  { GENE_SENSOR      , "tiredness"              }, // GENE_SENS_TIREDNESS
  { GENE_SENSOR      , "obscurity"              }, // GENE_SENS_OBSCURITY
  { GENE_SENSOR      , "libido"                 }, // GENE_SENS_LIBIDO
  { GENE_SENSOR      , "speed"                  }, // GENE_SENS_SPEED
  { GENE_SENSOR      , "view far"               }, // GENE_SENS_VIEW_FAR
  { GENE_SENSOR      , "compass"                }, // GENE_SENS_COMPASS
  { GENE_SENSOR      , "taste"                  }, // GENE_SENS_TASTE
  { GENE_SENSOR      , "fear"                   }, // GENE_SENS_FEAR
  { GENE_SENSOR      , "temperature"            }, // GENE_SENS_TEMPERATURE
  { GENE_SENSOR      , "orientation"            }, // GENE_SENS_ORIENTATION
  { GENE_SENSOR      , "view identify"          }, // GENE_SENS_VIEW_IDENTIFY
  { GENE_SENSOR      , "view identify far"      }, // GENE_SENS_VIEW_IDENT_FAR
  { GENE_SENSOR      , "Hormone level"          }, // GENE_SENS_HORMONE
  { GENE_SENSOR      , "composite"              }, // GENE_SENS_COMPOSITE
  { GENE_REACTION    , "nothing"                },
  { GENE_REACTION    , "sleep"                  },
  { GENE_REACTION    , "move forward"           },
  { GENE_REACTION    , "step back"              },
  { GENE_REACTION    , "turn left"              },
  { GENE_REACTION    , "turn right"             },
  { GENE_REACTION    , "eat"                    },
  { GENE_REACTION    , "drink"                  },
  { GENE_REACTION    , "copulate"               },
  { GENE_REACTION    , "attack"                 },
  { GENE_REACTION    , "turn head left"         },
  { GENE_REACTION    , "turn head right"        },
  { GENE_REACTION    , "ruminate"               },
  { GENE_REACTION    , "hide"                   },
  { GENE_REACTION    , "RFU1"                   },
  { GENE_REACTION    , "RFU2"                   },
  { GENE_BRAIN_SIZE  , "history input"          },
  { GENE_BRAIN_SIZE  , "history experience"     },
  { GENE_BRAIN_INIT  , "brain line"             },
  { GENE_FEELING     , "wellfare sensor"        },
  { GENE_FEELING     , "fear sensor"            },
  { GENE_PURPOSE     , "purpose trigger up"     }, // GENE_PURPOSE_TRIGGER_UP
  { GENE_PURPOSE     , "sensor bonus"           }, // GENE_PURPOSE_SENSOR
  { GENE_PURPOSE     , "reaction bonus"         }, // GENE_PURPOSE_REACTION
  { GENE_PURPOSE     , "purpose trigger down"   }, // GENE_PURPOSE_TRIGGER_DOWN
  { GENE_BRAIN_INIT  , "identification line"    }, // GENE_BRAIN_IDENTIFY_LINE
  { GENE_BRAIN_CONFIG, "behavior"               }  // GENE_BRAIN_BEHAVIOR
};

const char* GeneMuteTypeNameList[GENE_MUTE_NUMBER_TYPE] =
{
  "Random bit",
  "Incremental 1 byte",
  "Incremental 2 bytes"
};
*/

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
    { { "Min", 2, 0, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Init", 2, 2, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_PARAMETER, GENE_PARAM_TOXICITY, "Toxicity", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_PROTECTION, "Protection", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_CAMOUFLAGE, "Camouflage", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
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
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_HEALTH, "Health", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Start", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_GROWTH_SPEED, "Growth speed", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "mU day", 2, 2, 0, 1000, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 1000 } } },

  { GENE_PARAMETER, GENE_PARAM_REPRO_RANGE, "Reproduction range", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 10, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Range", 2, 2, 0, 10, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 10, GENE_DOMINANCE_HIGHEST, false, 10 } } },

  { GENE_PARAMETER, GENE_PARAM_FAT_WEIGHT, "Fat weight", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Init", 2, 2, 0.1, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0.1, MAX_VAL_WEIGHT, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_PARAMETER, GENE_PARAM_ATTACK_FACTOR, "Attack", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_SPEED, "Speed", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 200, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Speed", 2, 2, 0, 200, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Steps sec", 2, 4, 0, 200, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_PARAMETER, GENE_PARAM_CURIOSITY, "Curiosity rate", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_LEARNING, "Learning rate", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Rate", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_POLLEN_RANGE, "Pollen range", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Range", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, false, 100 } } },

  { GENE_PARAMETER, GENE_PARAM_GESTA_TIME, "Gestation time", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Start", 2, 2, 0, 1000, GENE_DOMINANCE_HIGHEST, false, 0 },
      { "Duration", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, true, 0 } } },

  { GENE_PARAMETER, GENE_PARAM_GESTA_NB, "Gestation baby number", GENE_MUTE_INCREMENTAL_2,
    { { "Min", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Usual", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 },
      { "Max", 2, 4, 0, 100, GENE_DOMINANCE_HIGHEST, true, 0 } } }
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
    { { "Sens Uid Hi", 2, 0, 0, 100, GENE_DOMINANCE_HIGHEST, false },
      { "Sens Uid Lo", 2, 2, 0, 100, GENE_DOMINANCE_HIGHEST, false },
      { "ParamId", 2, 4, 0, 1000, GENE_DOMINANCE_HIGHEST, true } } }
};

array< vector<CGeneDefinitions>, 6> CGeneList::geneListArray = 
{ 
  CGeneList::geneListGeneric , 
  CGeneList::geneListCaracter ,
  CGeneList::geneListParameter ,
  CGeneList::geneListLifeStage,
  CGeneList::geneListPhyWellfare,
  CGeneList::geneListSensor 
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
CGeneDefinitions::CGeneDefinitions(GeneType_e type, GeneSubType_e subType, string name, GeneMuteType_e muteType, vector<GeneParamDefinition_t> paramList):
 geneType{type}, geneSubType{subType}, label{name}, muteType{muteType}, parameters{paramList}
{
}


CGeneDefinitions::~CGeneDefinitions()
{
}


//===========================================================================
//                          CGeneList            
//===========================================================================

const CGeneDefinitions* CGeneList::getDefinitions(GeneType_e type, GeneSubType_e subType)
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