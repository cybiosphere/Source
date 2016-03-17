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

// CWizardPurpose.cpp: implementation of the CBrain class. 
//
//////////////////////////////////////////////////////////////////////

#include "CWizardPurpose.h"
#include "CBrainAnimal.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWizardPurpose::CWizardPurpose(CAnimal* pAnimal)
{
  m_pAnimal = pAnimal;
}


CWizardPurpose::~CWizardPurpose()
{
  // loop from top to bottom 
  for (int i=m_tPurposeDefinition.GetSize()-1; i>=0; i--) 
  {
    if (m_tPurposeDefinition.GetAt(i) != NULL)
      delete m_tPurposeDefinition.GetAt(i); 
    // Fred TBD: delete tSensorBonus.pBonusTable
    m_tPurposeDefinition.RemoveAt(i);
  }
}


bool CWizardPurpose::AddPurposeInList ( int                  chromosomePaireIndex,
                                        CString              purposeName,
                                        int                  purposeDuration,
                                        PurposeTriggerType_e triggerType,
                                        DWORD                triggerSensorUniqueId,
                                        int                  triggerSensorUniqueIdRange,
                                        int                  triggerSubCaptorIndex,
                                        int                  triggerStartThreshold,
                                        int                  triggerStopThreshold)
{
  PurposeDefinition_t* pPurpose = new PurposeDefinition_t;
  pPurpose->chromosomePaireIndex        = chromosomePaireIndex;
  pPurpose->purposeDuration             = purposeDuration;
  pPurpose->purposeName                 = purposeName;
  pPurpose->triggerType                 = triggerType;
  pPurpose->triggerSensorUniqueId       = triggerSensorUniqueId;
  pPurpose->triggerSensorUniqueIdRange  = triggerSensorUniqueIdRange;
  pPurpose->triggerSubCaptorIndex       = triggerSubCaptorIndex;
  pPurpose->triggerStartThreshold       = triggerStartThreshold;
  pPurpose->triggerStopThreshold        = triggerStopThreshold;
  pPurpose->nbSensorBonus               = 0;
  pPurpose->nbReactionBonus             = 0;

  // TBD: Could check that purposeName does not already exist
  m_tPurposeDefinition.Add(pPurpose);

  return true;
}

bool CWizardPurpose::AddPurposeSensorBonus( CString purposeName, 
                                            DWORD sensorUniqueId, int sensorUniqueIdRange,
                                            int nbBonus, short* pBonusTable )
{
  bool resu = false;
  PurposeDefinition_t *pCurPurpose = NULL;

  for (int i=0; i<m_tPurposeDefinition.GetSize(); i++)
  {
    pCurPurpose = m_tPurposeDefinition.GetAt(i);
    if ( ( pCurPurpose->purposeName == purposeName) && (pCurPurpose->nbSensorBonus < (WZ_MAX_SENSOR_BONUS_PURPOSE-1)) )
    {
      pCurPurpose->tSensorBonus[pCurPurpose->nbSensorBonus].sensorUniqueId = sensorUniqueId;
      pCurPurpose->tSensorBonus[pCurPurpose->nbSensorBonus].sensorUniqueIdRange = sensorUniqueIdRange;
      pCurPurpose->tSensorBonus[pCurPurpose->nbSensorBonus].nbBonus = nbBonus;
      pCurPurpose->tSensorBonus[pCurPurpose->nbSensorBonus].pBonusTable = pBonusTable;
      pCurPurpose->nbSensorBonus += 1;
      resu = true;
    }
  }

  return resu;
}


bool CWizardPurpose::AddPurposeReactionBonus( CString purposeName, 
                                              DWORD reactionUniqueId, int reactionUniqueIdRange,
                                              int bonusRateX10 )
{
  bool resu = false;
  PurposeDefinition_t *pCurPurpose = NULL;

  for (int i=0; i<m_tPurposeDefinition.GetSize(); i++)
  {
    pCurPurpose = m_tPurposeDefinition.GetAt(i);
    if ( ( pCurPurpose->purposeName == purposeName) && (pCurPurpose->nbReactionBonus < (WZ_MAX_REACTION_BONUS_PURPOSE-1)) )
    {
      pCurPurpose->tReactionBonus[pCurPurpose->nbReactionBonus].reactionUniqueId = reactionUniqueId;
      pCurPurpose->tReactionBonus[pCurPurpose->nbReactionBonus].reactionUniqueIdRange = reactionUniqueIdRange;
      pCurPurpose->tReactionBonus[pCurPurpose->nbReactionBonus].bonus = bonusRateX10;
      pCurPurpose->nbReactionBonus += 1;
      resu = true;
    }
  }  
  return resu;
}

// bonus = 10 x mutiplicator factor
bool CWizardPurpose::AddPurposeVisualBonus( CString purposeName, int bonus, int layer, 
                                            FormType_e form, ColorCaracterType_e color, 
                                            TextureType_e texture, PhyAttributeType_e attribute, bool isMoving)
{
  if (m_pAnimal == NULL)
    return false;

  int i,j;
  CSensor* pCurSens;
  DWORD curSensUid;
  short* pCurBonusTable = NULL;
  int nbViewBlocks = 0;

  for (i=0; i<m_pAnimal->getBrain()->GetNumberSensor(); i++)
  {
    pCurSens   = m_pAnimal->getBrain()->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW )
    {
      CSensorView* pViewSens = (CSensorView*)pCurSens;

      if (pViewSens->GetLayer() == layer)
      {
         pCurBonusTable = new short[pViewSens->GetSubCaptorNumber()];
         for (j=0; j<pViewSens->GetSubCaptorNumber(); j++)
         {
            pCurBonusTable[j] = 10; // by default: multiplicator 1.0 this could be changed by a shrink parameter
         }

        int subOffsetForm    = -1;
        int subOffsetColor   = -1;
        int subOffsetTexture = -1;
        int subOffsetAttrib  = -1;
        int subOffsetIsMove  = -1;
        int offset = 0;
        if (form >= FORM_FIRST_TYPE)
          subOffsetForm    = pViewSens->GetSubCaptorSubIndexForForm(form);
        if (color >= COLOR_CARACTER_FIRST_TYPE)
          subOffsetColor   = pViewSens->GetSubCaptorSubIndexForColor(color);
        if (texture >= TEXTURE_FIRST_TYPE)
          subOffsetTexture = pViewSens->GetSubCaptorSubIndexForTexture(texture);
        if (attribute >= PHY_ATTRIBUTE_FIRST_TYPE)
          subOffsetAttrib = pViewSens->GetSubCaptorSubIndexForPhyAttribute(attribute);
        if (isMoving)
          subOffsetIsMove = pViewSens->GetSubCaptorSubIndexForMoving();

        nbViewBlocks = pViewSens->GetSubCaptorNumber()/VIEW_SIZE_PER_FOCUS;

        for (j=0; j<nbViewBlocks; j++)
        {
          if (subOffsetForm>-1)
            pCurBonusTable[offset + subOffsetForm]    = bonus;
          if (subOffsetColor>-1)
            pCurBonusTable[offset + subOffsetColor]   = bonus;
          if (subOffsetTexture>-1)
            pCurBonusTable[offset + subOffsetTexture] = bonus;
          if (subOffsetAttrib>-1)
            pCurBonusTable[offset + subOffsetAttrib]  = bonus;
          if (subOffsetIsMove>-1)
            pCurBonusTable[offset + subOffsetIsMove]  = bonus;
          offset += VIEW_SIZE_PER_FOCUS;
        }

        AddPurposeSensorBonus(purposeName, pViewSens->GetUniqueId(), 1, pViewSens->GetSubCaptorNumber(), pCurBonusTable); 
      }
    }
    else if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_FAR )
    {
      CSensorViewFar* pViewSens = (CSensorViewFar*)pCurSens;

      pCurBonusTable = new short[pViewSens->GetSubCaptorNumber()];
      for (j=0; j<pViewSens->GetSubCaptorNumber(); j++)
      {
         pCurBonusTable[j] = 10; // by default: multiplicator 1.0 
      }

      int subOffsetForm    = -1;
      int subOffsetColor   = -1;
      int subOffsetTexture = -1;
      int subOffsetAttrib  = -1;
      int subOffsetIsMove  = -1;
      int offset = 0;
      if (form >= FORM_FIRST_TYPE)
        subOffsetForm    = pViewSens->GetSubCaptorSubIndexForForm(form);
      if (color >= COLOR_CARACTER_FIRST_TYPE)
        subOffsetColor   = pViewSens->GetSubCaptorSubIndexForColor(color);
      if (texture >= TEXTURE_FIRST_TYPE)
        subOffsetTexture = pViewSens->GetSubCaptorSubIndexForTexture(texture);
      if (attribute >= PHY_ATTRIBUTE_FIRST_TYPE)
        subOffsetAttrib = pViewSens->GetSubCaptorSubIndexForPhyAttribute(attribute);
      if (isMoving)
        subOffsetIsMove = pViewSens->GetSubCaptorSubIndexForMoving();

      nbViewBlocks = pViewSens->GetSubCaptorNumber()/VIEW_SIZE_PER_FOCUS;

      for (j=0; j<nbViewBlocks; j++)
      {
        if (subOffsetForm>-1)
          pCurBonusTable[offset + subOffsetForm]    = bonus;
        if (subOffsetColor>-1)
          pCurBonusTable[offset + subOffsetColor]   = bonus;
        if (subOffsetTexture>-1)
          pCurBonusTable[offset + subOffsetTexture] = bonus;
        if (subOffsetAttrib>-1)
          pCurBonusTable[offset + subOffsetAttrib]  = bonus;
        if (subOffsetIsMove>-1)
          pCurBonusTable[offset + subOffsetIsMove]  = bonus;
        offset += VIEW_SIZE_PER_FOCUS;
      }

      AddPurposeSensorBonus(purposeName, pViewSens->GetUniqueId(), 1, pViewSens->GetSubCaptorNumber(), pCurBonusTable); 
    }
  }

  return true;
}

// bonus = 10 x mutiplicator factor
bool CWizardPurpose::AddPurposeVisualIdentifyBonus( CString purposeName, int bonus, int layer, 
                                                    IdentificationType_e identity, 
                                                    bool isMoving, bool isDirLeft, bool isDirRight)
{
  if (m_pAnimal == NULL)
    return false;

  int i,j;
  CSensor* pCurSens;
  DWORD curSensUid;
  short* pCurBonusTable = NULL;
  int nbViewBlocks = 0;

  for (i=0; i<m_pAnimal->getBrain()->GetNumberSensor(); i++)
  {
    pCurSens   = m_pAnimal->getBrain()->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY )
    {
      CSensorViewIdentify* pViewSens = (CSensorViewIdentify*)pCurSens;

      if (pViewSens->GetLayer() == layer)
      {
        pCurBonusTable = new short[pViewSens->GetSubCaptorNumber()];
        for (j=0; j<pViewSens->GetSubCaptorNumber(); j++)
        {
          pCurBonusTable[j] = 10; // by default: multiplicator 1.0 this could be changed by a shrink parameter
        }

        int subOffsetProxy    = -1;
        int subOffsetMove     = -1;
        int subOffsetdirLeft  = -1;
        int subOffsetdirRight = -1;
        int offset = 0;

        subOffsetProxy = CSensorViewIdentify::GetSubCaptorSubIndexForProximity(identity);
        if (isMoving)
        {
          subOffsetMove  = CSensorViewIdentify::GetSubCaptorSubIndexForMoving(identity);
        }
        if (isDirLeft)
        {
          subOffsetdirLeft  = CSensorViewIdentify::GetSubCaptorSubIndexForDirLeft(identity);
        }
        if (isDirRight)
        {
          subOffsetdirRight  = CSensorViewIdentify::GetSubCaptorSubIndexForDirRight(identity);
        }

        nbViewBlocks = pViewSens->GetSubCaptorNumber()/VIEW_IDENTIFY_SIZE_PER_FOCUS;

        for (j=0; j<nbViewBlocks; j++)
        {
          if (subOffsetProxy>-1)
            pCurBonusTable[offset + subOffsetProxy]    = bonus;
          if (subOffsetMove>-1)
            pCurBonusTable[offset + subOffsetMove]     = bonus;
          if (subOffsetdirLeft>-1)
            pCurBonusTable[offset + subOffsetdirLeft]  = bonus;
          if (subOffsetdirRight>-1)
            pCurBonusTable[offset + subOffsetdirRight] = bonus;
          offset += VIEW_IDENTIFY_SIZE_PER_FOCUS;
        }

        AddPurposeSensorBonus(purposeName, pViewSens->GetUniqueId(), 1, pViewSens->GetSubCaptorNumber(), pCurBonusTable); 
      }
    }
    else if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY_FAR )
    {
      CSensorViewIdentifyFar* pViewSens = (CSensorViewIdentifyFar*)pCurSens;

      pCurBonusTable = new short[pViewSens->GetSubCaptorNumber()];
      for (j=0; j<pViewSens->GetSubCaptorNumber(); j++)
      {
         pCurBonusTable[j] = 10; // by default: multiplicator 1.0 
      }

      int subOffsetProxy    = -1;
      int subOffsetMove     = -1;
      int subOffsetdirLeft  = -1;
      int subOffsetdirRight = -1;
      int offset = 0;

      subOffsetProxy = CSensorViewIdentifyFar::GetSubCaptorSubIndexForProximity(identity);
      if (isMoving)
      {
        subOffsetMove  = CSensorViewIdentifyFar::GetSubCaptorSubIndexForMoving(identity);
      }
      if (isDirLeft)
      {
        subOffsetdirLeft  = CSensorViewIdentifyFar::GetSubCaptorSubIndexForDirLeft(identity);
      }
      if (isDirRight)
      {
        subOffsetdirRight  = CSensorViewIdentifyFar::GetSubCaptorSubIndexForDirRight(identity);
      }

      nbViewBlocks = pViewSens->GetSubCaptorNumber()/VIEW_IDENTIFY_SIZE_PER_FOCUS;

      for (j=0; j<nbViewBlocks; j++)
      {
        if (subOffsetProxy>-1)
          pCurBonusTable[offset + subOffsetProxy]    = bonus;
        if (subOffsetMove>-1)
          pCurBonusTable[offset + subOffsetMove]     = bonus;
        if (subOffsetdirLeft>-1)
          pCurBonusTable[offset + subOffsetdirLeft]  = bonus;
        if (subOffsetdirRight>-1)
          pCurBonusTable[offset + subOffsetdirRight] = bonus;
        offset += VIEW_IDENTIFY_SIZE_PER_FOCUS;
      }

      AddPurposeSensorBonus(purposeName, pViewSens->GetUniqueId(), 1, pViewSens->GetSubCaptorNumber(), pCurBonusTable); 
    }
  }

  return true;
}

bool CWizardPurpose::AddPurposeOdorBonus ( CString purposeName, int bonus, OdorType_e odor)
{
  bool resu = false;
  short* pCurBonusTable = new short[NUMBER_ODORS];

  for (int i=0; i<NUMBER_ODORS; i++)
  {
     pCurBonusTable[i] = 10; // by default: multiplicator 1.0 this could be changed by a shrink parameter
  }
  if (odor >= ODOR_FIRST_TYPE)
  {
    pCurBonusTable[odor-ODOR_FIRST_TYPE] = bonus;  
    AddPurposeSensorBonus(purposeName, UID_BASE_SENS_SMELL, 1000, NUMBER_ODORS, pCurBonusTable);
    resu = true;
  }
  return resu;
}


bool CWizardPurpose::AddPurposePheromoneBonus ( CString purposeName, int bonus, PheromoneType_e phero)
{
  bool resu = false;
  short* pCurBonusTable = new short[NUMBER_PHEROMONES];

  for (int i=0; i<NUMBER_PHEROMONES; i++)
  {
     pCurBonusTable[i] = 10; // by default: multiplicator 1.0 this could be changed by a shrink parameter
  }
  if (phero >= PHEROMONE_FIRST_TYPE)
  {
    pCurBonusTable[phero-PHEROMONE_FIRST_TYPE] = bonus;  
    AddPurposeSensorBonus(purposeName, UID_BASE_SENS_PHEROMONE, 1000, NUMBER_PHEROMONES, pCurBonusTable);
    resu = true;
  }
  return resu;
}


bool CWizardPurpose::AddPurposeHormoneBonus (CString purposeName, int bonus, PheromoneType_e phero)
{
  bool resu = false;
  short* pCurBonusTable = new short[NUMBER_PHEROMONES];

  for (int i=0; i<NUMBER_PHEROMONES; i++)
  {
     pCurBonusTable[i] = 10; // by default: multiplicator 1.0 this could be changed by a shrink parameter
  }
  if (phero >= PHEROMONE_FIRST_TYPE)
  {
    pCurBonusTable[phero-PHEROMONE_FIRST_TYPE] = bonus;  
    AddPurposeSensorBonus(purposeName, UID_BASE_SENS_HORMONE, 1000, NUMBER_PHEROMONES, pCurBonusTable);
    resu = true;
  }
  return resu;
}


bool CWizardPurpose::AddPurposeSensorGene(int mutationRate, int paireIndex, DWORD purposeUid, PurposeSensorBonus_t sensBonus)
{
  int i;
  int geneIndex;
  CPairOfChromosome* pCurPaire = m_pAnimal->getGenome()->getPair(paireIndex);
  CGene* pCurGene = NULL;
  DWORD sensorUid;
  CSensor* pSensor = NULL;

  for (i=0; i<m_pAnimal->getBrain()->GetNumberSensor(); i++)
  {
    pSensor = m_pAnimal->getBrain()->GetSensorByIndex(i);
    if ( (pSensor->GetUniqueId() >= sensBonus.sensorUniqueId) 
      && (pSensor->GetUniqueId() < (sensBonus.sensorUniqueId + sensBonus.sensorUniqueIdRange)) ) 
    {
      sensorUid = pSensor->GetUniqueId();
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsPurposeSensorBonus(mutationRate, purposeUid, sensorUid, sensBonus.nbBonus, sensBonus.pBonusTable); 

      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsPurposeSensorBonus(mutationRate, purposeUid, sensorUid, sensBonus.nbBonus, sensBonus.pBonusTable); 
    }
  }

  return true;
}


bool CWizardPurpose::AddPurposeReactionGene(int mutationRate, int paireIndex, DWORD purposeUid, PurposeReactionBonus_t reactionBonus)
{
  int i;
  int geneIndex;
  CPairOfChromosome* pCurPaire = m_pAnimal->getGenome()->getPair(paireIndex);
  CGene* pCurGene = NULL;
  DWORD reactionUid;
  CReaction* pReaction = NULL;

  for (i=0; i<m_pAnimal->getBrain()->GetNumberReaction(); i++)
  {
    pReaction = m_pAnimal->getBrain()->GetReactionByIndex(i);
    if ( (pReaction->GetUniqueId() >= reactionBonus.reactionUniqueId) 
      && (pReaction->GetUniqueId() < (reactionBonus.reactionUniqueId + reactionBonus.reactionUniqueIdRange)) ) 
    {
      reactionUid = pReaction->GetUniqueId();

      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsPurposeReactionBonus(mutationRate, purposeUid, reactionUid, reactionBonus.bonus); 

      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsPurposeReactionBonus(mutationRate, purposeUid, reactionUid, reactionBonus.bonus); 
    }
  }

  return true;
}


bool CWizardPurpose::AddAllPurposeGenesToAnimal(int mutationRate)
{
  if (m_pAnimal == NULL)
    return false;
  
  int i,j;
  CGenome* pGenome = m_pAnimal->getGenome();
  CPairOfChromosome* pCurPaire = NULL;
  PurposeDefinition_t* pCurPurpose = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;
  GeneSubType_e curGeneSubType;
  DWORD sensorUid  = UID_UNSET;
  DWORD purposeUid = UID_UNSET;
  CSensor* pSensor = NULL;

  for (i=0; i<m_tPurposeDefinition.GetSize(); i++)
  {
    pCurPurpose = m_tPurposeDefinition.GetAt(i);
    pCurPaire = pGenome->getPair(pCurPurpose->chromosomePaireIndex); 
    
    if (pCurPaire!=NULL)
    {
      if (pCurPurpose->triggerType == PURPOSE_TRIG_DOWN)
        curGeneSubType = GENE_PURPOSE_TRIGGER_DOWN;
      else
        curGeneSubType = GENE_PURPOSE_TRIGGER_UP;

      sensorUid  = UID_UNSET;
      for (j=0; j<m_pAnimal->getBrain()->GetNumberSensor(); j++)
      {
        pSensor = m_pAnimal->getBrain()->GetSensorByIndex(j);
        if ( (pSensor->GetUniqueId() >= pCurPurpose->triggerSensorUniqueId) 
          && (pSensor->GetUniqueId() < (pCurPurpose->triggerSensorUniqueId + pCurPurpose->triggerSensorUniqueIdRange)) ) 
        {
          sensorUid = pSensor->GetUniqueId();
          break;
        }
      }
     
      // Skip purpose if sensor was not found
      if (sensorUid == UID_UNSET)
        continue;

      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsPurposeTrigger(curGeneSubType, mutationRate, sensorUid, 
                                    pCurPurpose->purposeDuration, pCurPurpose->triggerSubCaptorIndex, 
                                    pCurPurpose->triggerStartThreshold, pCurPurpose->triggerStopThreshold,
                                    pCurPurpose->purposeName.GetLength()+1, pCurPurpose->purposeName.GetBuffer(0));
                                    
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsPurposeTrigger(curGeneSubType, mutationRate, sensorUid, 
                                    pCurPurpose->purposeDuration, pCurPurpose->triggerSubCaptorIndex, 
                                    pCurPurpose->triggerStartThreshold, pCurPurpose->triggerStopThreshold,
                                    pCurPurpose->purposeName.GetLength()+1, pCurPurpose->purposeName.GetBuffer(0));
        
      purposeUid = CPurpose::ComputeUniqueId (sensorUid, pCurPurpose->triggerSubCaptorIndex);

      for (j=0; j<pCurPurpose->nbSensorBonus; j++)
      {
        AddPurposeSensorGene(mutationRate, pCurPurpose->chromosomePaireIndex, purposeUid, pCurPurpose->tSensorBonus[j]);
      }

      for (j=0; j<pCurPurpose->nbReactionBonus; j++)
      {
        AddPurposeReactionGene(mutationRate, pCurPurpose->chromosomePaireIndex, purposeUid, pCurPurpose->tReactionBonus[j]);
      }
    }
  }

  return true;
}