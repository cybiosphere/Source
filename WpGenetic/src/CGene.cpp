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
 
#include "CGene.h"

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

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::CGene()
//  
// DESCRIPTION:  Default constructor
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      To be used before SetAs...
//---------------------------------------------------------------------------
CGene::CGene()
{
  m_GeneType    = GENE_GENERIC;
  m_GeneSubType = GENE_GENERIC_UNKNOWN;
  m_MuteRate    = 0;
  m_MuteType    = GENE_MUTE_RANDOM_BIT;
  m_DataLength  = 0; 
  m_pRawData = NULL;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::CGene()
//  
// DESCRIPTION:  Constructor.
// 
// ARGUMENTS:    pData: Pointer on dataLen BYTE size memory allocated
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CGene::CGene(GeneType_e type, GeneSubType_e subType, int dataLen, BYTE* pData, GeneMuteType_e muteType, int muteRate)
{
  m_GeneType = type;
  m_GeneSubType = subType;
  m_MuteRate = muteRate;
  m_MuteType = muteType;
  m_DataLength = dataLen; 
  m_pRawData = pData;
}

//---------------------------------------------------------------------------
// METHOD:       CGene::CGene()
//  
// DESCRIPTION:  Copy constructor
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      To be used for reproduction
//---------------------------------------------------------------------------
CGene::CGene(CGene& model)
{
  m_GeneType    = model.m_GeneType;
  m_GeneSubType = model.m_GeneSubType;
  m_MuteRate    = model.m_MuteRate;
  m_MuteType    = model.m_MuteType;
  m_DataLength  = model.m_DataLength; 
  m_pRawData    = new BYTE[m_DataLength];
  memcpy(m_pRawData,model.m_pRawData,m_DataLength);
}

CGene::~CGene()
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;
}

//===========================================================================
// Configuration
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsCaracter()
//  
// DESCRIPTION:  Configure gene to describe a caracter
// 
// ARGUMENTS:    pData: Pointer on dataLen BYTE size data. Note that pData is copied
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsCaracter(GeneSubType_e subType, int muteRate, GeneMuteType_e muteType, int dataLen, BYTE* pData)
{
  // Clean previous config
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_CARACTER;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = muteType;
  m_DataLength  = dataLen; 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    memcpy(m_pRawData,pData,m_DataLength);
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsParameter()
//  
// DESCRIPTION:  Configure gene to describe a parameter
// 
// ARGUMENTS:    min [0..1000] generic scale: Real min compute according to subType 
//               nominalVal [0..1000] generic scale: Real val compute according to subType 
//               max [0..1000] generic scale: Real max compute according to subType 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsParameter(GeneSubType_e subType, int muteRate, long min, long nominalVal, long max)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_PARAMETER;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = 3*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for parameter is [0..1000]
    pWordData[0] = (min * 0xFFFF)/1000;
    pWordData[1] = (nominalVal * 0xFFFF)/1000;
    pWordData[2] = (max * 0xFFFF)/1000;
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsLifeStage()
//  
// DESCRIPTION:  Configure gene to describe a life stage
// 
// ARGUMENTS:    durationRatio [0..1000] generic scale: 10 x "% of life or death" period 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsLifeStage(GeneSubType_e subType, int muteRate, long durationRatio)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_LIFESTAGE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for LifeStage is [1..1000]
    pWordData[0] = (durationRatio * 0xFFFF)/1000;
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsPhysicWelfare()
//  
// DESCRIPTION:  Configure gene to describe a life stage
// 
// ARGUMENTS:    sensitivity [0..1000] generic scale: 10 x "% of sensitivity" 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPhysicWelfare(GeneSubType_e subType, int muteRate, long sensitivity, long min, long nominalVal, long max)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_PHY_WELFARE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = 4*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for sensitivity is [1..1000]
    pWordData[0] = (sensitivity * 0xFFFF)/1000;
    // Generic scale for parameter is [0..1000]
    pWordData[1] = (min * 0xFFFF)/1000;
    pWordData[2] = (nominalVal * 0xFFFF)/1000;
    pWordData[3] = (max * 0xFFFF)/1000;
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsSensor()
//  
// DESCRIPTION:  Configure gene to describe a sensor
// 
// ARGUMENTS:    nbWeight size of pWeight according to subType
//               pWeight table [-1000..1000] generic scale: 10 x "% Weight" 
//               dataX [0..1000] generic scale: Real val compute according to subType
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsSensor (GeneSubType_e subType, int muteRate, int nbWeight, short* pWeight, 
                         long data1, long data2, long data3, long data4)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_SENSOR;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = (4+nbWeight)*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for all values is [0..1000]
    pWordData[0] = (data1 * 0xFFFF)/1000;
    pWordData[1] = (data2 * 0xFFFF)/1000;
    pWordData[2] = (data3 * 0xFFFF)/1000;
    pWordData[3] = (data4 * 0xFFFF)/1000;
    for (int i=0; i<nbWeight; i++)
    {
      pWordData[4+i] = (WORD)(((DWORD)(pWeight[i]+1000) * 0xFFFF)/2000);
    }
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsSensorComposite()
//  
// DESCRIPTION:  Configure gene to describe a composite sensor
// 
// ARGUMENTS:    nbWeight size of pWeight according to subType
//               pWeight table [-1000..1000] generic scale: 10 x "% Weight" 
//               raw sensorUId and paramId 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsSensorComposite (int muteRate, int nbWeight, short* pWeight, DWORD sensorUId, int paramId)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_SENSOR;
  m_GeneSubType = GENE_SENS_COMPOSITE;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = (4+nbWeight)*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for all values is [0..1000]
    pWordData[0] = sensorUId>>16;
    pWordData[1] = sensorUId&0xFFFF;
    pWordData[2] = (paramId * 0xFFFF)/1000;
    pWordData[3] = 0;
    for (int i=0; i<nbWeight; i++)
    {
      pWordData[4+i] = (WORD)(((DWORD)(pWeight[i]+1000) * 0xFFFF)/2000);
    }
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsReaction()
//  
// DESCRIPTION:  Configure gene to describe a reaction
// 
// ARGUMENTS:    success [0..1000] generic scale: 10 x "% of success" 
//               failure [0..1000] generic scale: 10 x "% of failure" 
//               dataX [0..1000] generic scale: Real val compute according to subType
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsReaction ( GeneSubType_e subType, int muteRate, long success, long failure, 
                            long data1, long data2)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_REACTION;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = 4*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for all values is [0..1000]
    pWordData[0] = (success * 0xFFFF)/1000;
    pWordData[1] = (failure * 0xFFFF)/1000;
    pWordData[2] = (data1 * 0xFFFF)/1000;
    pWordData[3] = (data2 * 0xFFFF)/1000;
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainSize()
//  
// DESCRIPTION:  Configure gene to describe a brain configuration size
// 
// ARGUMENTS:    brainSize [0..100]: size to configure brain
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainSize (GeneSubType_e subType, int muteRate, int brainSize)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_BRAIN_SIZE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Generic scale for brainSize is [1..100]
    pWordData[0] = (brainSize * 0xFFFF)/100;
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainInstinctLine()
//  
// DESCRIPTION:  Configure gene to describe a brain matrix line
// 
// ARGUMENTS:    pData: Pointer on dataLen WORD size data. Note that pData is copied  
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainInstinctLine (int muteRate, int lineId, int nbColumn, WORD* pData)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_BRAIN_INIT;
  m_GeneSubType = GENE_BRAIN_LINE;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = (nbColumn+1)*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Set 1st Word as lineId. Generic scale is [0..10000]
    pWordData[0] = (lineId* 0xFFFF)/10000;
    // Set all other words as brain line data
    for (int i=0;i<nbColumn;i++)
    {
      pWordData[i+1] = pData[i];
    }
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainIdentificationLine()
//  
// DESCRIPTION:  Configure gene to describe a brain identification matrix line
// 
// ARGUMENTS:    pData: Pointer on dataLen WORD size data. Note that pData is copied  
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainIdentificationLine (int muteRate, int lineId, int nbColumn, WORD* pData)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_BRAIN_INIT;
  m_GeneSubType = GENE_BRAIN_IDENTIFY_LINE;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = (nbColumn+1)*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD* pWordData = (WORD*)m_pRawData;
    // Set 1st Word as lineId. Generic scale is [0..10000]
    pWordData[0] = (lineId* 0xFFFF)/10000;
    // Set all other words as brain line data
    for (int i=0;i<nbColumn;i++)
    {
      pWordData[i+1] = pData[i];
    }
    return (true);
  }
  else
  {
    return (false);
  }
}


//---------------------------------------------------------------------------
// METHOD:       CGene::setAsFeeling()
//  
// DESCRIPTION:  Configure gene to describe a sensor
// 
// ARGUMENTS:    nbSensi size of pSensi according to Sensor UID
//               pSensi table [-1000..1000] generic scale: 10 x "% Sensibility" 
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsFeeling(GeneSubType_e subType, int muteRate, DWORD sensorUId, int nbSensi, short* pSensi)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_FEELING;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = sizeof(DWORD) + nbSensi*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD*  pWordData  = (WORD*)m_pRawData;
    DWORD* pDWordData = (DWORD*)m_pRawData;  
    pDWordData[0] = sensorUId;
    // Generic scale for all values is [0..1000]
    for (int i=0; i<nbSensi; i++)
    {
      pWordData[2+i] = (WORD)(((DWORD)(pSensi[i]+1000) * 0xFFFF)/2000);
    }
    return (true);
  }
  else
  {
    return (false);
  }
}


//---------------------------------------------------------------------------
// METHOD:       CGene::setAsNewPurpose()
//  
// DESCRIPTION:  Configure gene to describe a purpose
// 
// ARGUMENTS:    sensorUId : Sensor UID
//               duration  : [0..1000] in minute  
//               startThreshold : [0..1000]  10 x "% start rate"
//               subCaptorIndex : [0..20]
//               stopThreshold  : [0..1000]  10 x "% stop rate"
//               labelLen  : label to identify purpose
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPurposeTrigger(GeneSubType_e subType, int muteRate, 
                                DWORD sensorUId, int duration, int subCaptorIndex, 
                                int startThreshold, int stopThreshold, int labelLen, char* pLabel)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_PURPOSE;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = sizeof(DWORD) + 4*sizeof(WORD) + labelLen*sizeof(BYTE); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD*  pWordData  = (WORD*)m_pRawData;
    DWORD* pDWordData = (DWORD*)m_pRawData;  
    BYTE*  pByteData  = (BYTE*)m_pRawData;
    pWordData[0]  = (WORD)(((DWORD)duration * 0xFFFF)/1000);
    pWordData[1]  = (WORD)(((DWORD)subCaptorIndex * 0xFFFF)/100);
    pWordData[2]  = (WORD)(((DWORD)startThreshold * 0xFFFF)/1000);
    pWordData[3]  = (WORD)(((DWORD)stopThreshold * 0xFFFF)/1000);
    pDWordData[2] = sensorUId;

    for (int i=0; i<labelLen; i++)
    {
      pByteData[12+i] = pLabel[i];
    }
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsPurposeSensorBonus()
//  
// DESCRIPTION:  Configure gene to describe a purpose reaction bonus
// 
// ARGUMENTS:    purposeUId : Purpose UID
//               sensorUId  : Sensor UID
//               subCaptorIndex : [0..20]
//               bonus       : [0..2000] generic scale: 10 x multiplicator factor on sensor
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPurposeSensorBonus(int muteRate, DWORD purposeUId, DWORD sensorUId, int nbBonus, short* pBonusTable)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_PURPOSE;
  m_GeneSubType = GENE_PURPOSE_SENSOR;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = 2*sizeof(DWORD) + nbBonus*sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD*  pWordData  = (WORD*)m_pRawData;
    DWORD* pDWordData = (DWORD*)m_pRawData;  
    pDWordData[0] = purposeUId;
    pDWordData[1] = sensorUId;
    for (int i=0; i<nbBonus; i++)
    {
      pWordData[4+i] = (WORD)(((DWORD)(pBonusTable[i]) * 0xFFFF)/2000);
    }

    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsPurposeReactionBonus()
//  
// DESCRIPTION:  Configure gene to describe a purpose reaction bonus
// 
// ARGUMENTS:    purposeUId  : Purpose UID
//               reactionUId : Reaction UID
//               bonus       : [0..2000] generic scale: 10 x bonus rate
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsPurposeReactionBonus(int muteRate, DWORD purposeUId, DWORD reactionUId, int bonus)
{
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_PURPOSE;
  m_GeneSubType = GENE_PURPOSE_REACTION;
  m_MuteRate    = muteRate;
  m_MuteType    = GENE_MUTE_INCREMENTAL_2;
  m_DataLength  = 2*sizeof(DWORD) + sizeof(WORD); 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    WORD*  pWordData  = (WORD*)m_pRawData;
    DWORD* pDWordData = (DWORD*)m_pRawData;  
    pDWordData[0] = purposeUId;
    pDWordData[1] = reactionUId;
    pWordData[4]  = (WORD)(((DWORD)bonus * 0xFFFF)/2000);
    return (true);
  }
  else
  {
    return (false);
  }
}

//---------------------------------------------------------------------------
// METHOD:       CGene::setAsBrainConfig()
//  
// DESCRIPTION:  Configure gene to describe a caracter
// 
// ARGUMENTS:    pData: Pointer on dataLen BYTE size data. Note that pData is copied
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      Erase previous data and resize memory
//---------------------------------------------------------------------------
bool CGene::setAsBrainConfig(GeneSubType_e subType, int muteRate, GeneMuteType_e muteType, int dataLen, BYTE* pData)
{
  // Clean previous config
  if (m_pRawData!=NULL)
    delete [] m_pRawData;

  m_GeneType    = GENE_BRAIN_CONFIG;
  m_GeneSubType = subType;
  m_MuteRate    = muteRate;
  m_MuteType    = muteType;
  m_DataLength  = dataLen; 
  m_pRawData    = new BYTE[m_DataLength];
  if (m_pRawData!=NULL)
  {
    memcpy(m_pRawData,pData,m_DataLength);
    return (true);
  }
  else
  {
    return (false);
  }
}

//===========================================================================
// Raw data conversion
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::buildStringDataFromGene()
//  
// DESCRIPTION:  Convert Gene to string: Type/Subtype/Len[MuteType/MuteRate/Data]
// 
// ARGUMENTS:    None  
//   
// RETURN VALUE: CString: result
//  
// REMARKS:      None
//---------------------------------------------------------------------------
string CGene::buildStringDataFromGene()
{
  string rawData = "";
  string tempStr = "";
  CGene* pGene = NULL;
  int curLen = 0;

  tempStr = FormatString("%02X%04X%04X%02X%02X",
                  m_GeneType,m_GeneSubType,
                  m_DataLength+2, // include mute type and mute rate
                  m_MuteType,m_MuteRate);
  rawData += tempStr;
  for (int j=0;j<m_DataLength;j++)
  {
    tempStr = FormatString("%02X",m_pRawData[j]);
    rawData += tempStr;
  }

  return (rawData);
}

//---------------------------------------------------------------------------
// METHOD:       CGene::buildGeneFromStringData()
//  
// DESCRIPTION:  Convert string to Gene
// 
// ARGUMENTS:    CString rawData: string Type/Subtype/Len[MuteType/MuteRate/Data]  
//   
// RETURN VALUE: bool: result
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CGene::buildGeneFromStringData(string rawData)
{
  string tmpStr = "";
  BYTE* pData;

  if (rawData.length() < 14)
  {
    return (false);
  }

  int geneType;
  int geneSubType;
  int dataLen; 
  int muteType; 
  int muteRate;
  int tmpData;
  tmpStr = rawData.substr(0,14);
  sscanf( tmpStr.c_str(),"%02X%04X%04X%02X%02X",&geneType,&geneSubType,&dataLen,&muteType,&muteRate);
  m_GeneType    = (GeneType_e)geneType;
  m_GeneSubType = (GeneSubType_e)geneSubType;
  m_DataLength  = dataLen - 2; // mute type and rate are include in data
  m_MuteType    = (GeneMuteType_e)muteType;
  m_MuteRate    = muteRate;

  pData = new BYTE[m_DataLength];
  for (int j=0;j<m_DataLength;j++)
  {
    tmpStr = rawData.substr(14+2*j, 2);
    sscanf (tmpStr.c_str(),"%02X",&tmpData);
    pData[j] = (BYTE)tmpData;
  }

  m_pRawData    = pData;

  return (true);
}

//===========================================================================
// Genetic
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGene::getDominanceFactor()
//  
// DESCRIPTION:  Return a Dominance factor to determine the dominant gene
// 
// ARGUMENTS:    None 
//   
// RETURN VALUE: int: Dominance factor = Sum of all BYTES
//  
// REMARKS:      None
//---------------------------------------------------------------------------
int CGene::getDominanceFactor(void)
{
  int resu=0;
  
  // Each BYTE has same weight
  for (int i=0;i<m_DataLength;i++)
  {
    resu = resu + m_pRawData[i];
  }

  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CGene::tryMutation()
//  
// DESCRIPTION:  Perform a mutation on gene randomely, according to MuteRate
// 
// ARGUMENTS:    None 
//   
// RETURN VALUE: bool: true if mutation occured
//  
// REMARKS:      None
//---------------------------------------------------------------------------
bool CGene::tryMutation()
{
  bool resu = false;
  if (testChance(m_MuteRate))
  {
    switch (m_MuteType)
    {
    case GENE_MUTE_RANDOM_BIT:
      {
        // Do mutation randomly. 
        // Effect efficiency depends of the weight of the bit muted.
        int dataByteId = getRandInt(m_DataLength-1);
        int dataBitId = getRandInt(7);
        m_pRawData[dataByteId] ^= (1<<dataBitId);
        resu = true;
        break;
      }
    case GENE_MUTE_INCREMENTAL_1:
      { 
        // Select 1 byte and increment or decrement it by 1.
        int dataByteId = getRandInt(m_DataLength-1);
        int addVal = getRandInt(2) - 1;
        if ( ((addVal<0)&&(m_pRawData[dataByteId]>0)) || ((addVal>0)&&(m_pRawData[dataByteId]<0xFF)) )
        {
          m_pRawData[dataByteId] += addVal;
          resu = true;
        }
        else
        {
          resu = false;
        }
        break;
      }
    case GENE_MUTE_INCREMENTAL_2:
      {
        // Select 1 word and increment or decrement it by 1.
        int dataByteId = getRandInt(m_DataLength-1)/2;
        int addVal = (getRandInt(2) - 1) * 66; // 66 in order to increment of 1 per 1000
        WORD* pWordData = (WORD*)m_pRawData;
        if ( ( (addVal<0)&&(pWordData[dataByteId]>65) ) || ( (addVal>0)&&(pWordData[dataByteId]<0xFFFF) ) )
        {
          pWordData[dataByteId] += addVal;
          resu = true;
        }
        else
        {
          resu = false;
        }
        break;
      }
    default:
      {
        resu = false;
        break;
      }
    }
  }
  else
  {
    resu = false;
  }
  return (resu);
}

//===========================================================================
// Display 
//===========================================================================

string CGene::getLabel()
{
  string label = GeneSubTypeNameList[m_GeneSubType].Name;
  return (label);
}

string CGene::getTypeLabel()
{
  string label;
  label = GeneTypeNameList[m_GeneType];
  return (label);
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

int CGene::getData(BYTE** pData)
{
  *pData = m_pRawData;
  return (m_DataLength);
}

int CGene::getDataLen()
{
  return (m_DataLength);
}

GeneType_e CGene::getGeneType()
{
  return (m_GeneType);
}

GeneSubType_e CGene::getGeneSubType()
{
  return (m_GeneSubType);
}

GeneMuteType_e CGene::getMuteType()
{
  return (m_MuteType);
}

BYTE CGene::getMuteRate()
{
  return (m_MuteRate);
}

string CGene::getGeneTypeStrName(GeneType_e type)
{
  string typeName;
  typeName = GeneTypeNameList[type];
  return(typeName);
}

string CGene::getGeneSubTypeStrName(GeneSubType_e type)
{
  string typeName;
  typeName = GeneSubTypeNameList[type].Name;
  return(typeName);
}

string CGene::getGeneMuteTypeStrName(GeneMuteType_e muteType)
{
  string typeName;
  typeName = GeneMuteTypeNameList[muteType];
  return(typeName);
}

GeneType_e CGene::getExpectedGeneType (GeneSubType_e subType)
{
  return(GeneSubTypeNameList[subType].type);
}