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
// FILE: CBrain.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an animal brain
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  21/08/2005 : Creation
//
//===========================================================================

#include "CBrain.h"
#include "CFeelingWelfare.h"
#include "CBiotop.h"
#include "CGeoMapPurpose.h"
#include <fstream>

//===========================================================================
// Definitions            
//===========================================================================

#define MEMORIZATION_FEEDBACK_THRESHOLD 0.05
#define MAX_CHOICE_BUFFER_SIZE_PER_CHOICE 6
#define TERRITORY_SIZE_SMALL 600
#define TERRITORY_SIZE_DEFAULT 800
#define TERRITORY_SIZE_BIG 1000

const char* IdentificationTypeNameList[IDENTIFICATION_NUMBER_TYPE] =
{
  "Neutral",
  "Danger",
  "Food",
  "Drink",
  "Home",
  "Baby",
  "Mother",
  "Friend",
  "Sex Partner",
  "Dominant",
  "Prey",
  "Predator"
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrain::CBrain()
{
  m_pEntity = NULL;

  // Input Vector attributes
  m_nInputSensors = 0;
  // Historical used to take decision 
  m_nInputHistory = 1;
  // Historical used to compute experience 
  m_nExperienceHistory = 1;
  // Size of geomap for purpose memory 
  m_TerritorySize = TERRITORY_SIZE_DEFAULT;

  m_historyWeight = 0.6;

  // Current Input containing current and historized sensations
  m_vCurrentDecisionInput.SetSize(0,1);
  // Current choice vector
  m_vCurrentDecisionChoice.SetSize(0,1);
  m_pChoiceDecisionBuffer = NULL;

  // Memorized inputs
  m_mInputDecisionHistory.SetSize(0,0);
  // Memorized feedbacks
  m_mDecisionHistory.SetSize(0,0);

  // Sensors table
  m_tSensors.resize(0);
  // Reaction table
  m_tReactions.resize(0);
  // Purpose table
  m_tPurposes.resize(0);

  m_pCurrentPurpose = NULL;
  m_pGeoMap = NULL;
  m_pFeelingWelfare = new CFeelingWelfare(m_pEntity);
  m_PreviousFeelingWelfare = 0;
  m_CurrentFeelingWelfare = 0;
  m_CurrentReactionChoice = 0;
  m_tBrainMemorizedEntityIdentities.resize(0);

  m_pSensorPheromone = NULL;
  m_pSensorSmell = NULL;

  m_bBabyStayHome = false;
  m_bIdentificationUsed=false;

  clearBrainFocusedEntityInfo();
}

void CBrain::clearBrainFocusedEntityInfo(void)
{
  m_FocusedEntityInfo.previousEntityId = ENTITY_ID_INVALID;
  m_FocusedEntityInfo.newEntityId = ENTITY_ID_INVALID;
  m_FocusedEntityInfo.computedWeight = 0;
  m_FocusedEntityInfo.captorUid = UID_UNSET;
  m_FocusedEntityInfo.subcaptorIndex = invalidIndex;
  m_FocusedEntityInfo.subcaptorsSize = 0;
}

CBrain::~CBrain()
{
  m_vCurrentDecisionInput.SetSize(0,0);
  m_vCurrentDecisionChoice.SetSize(0,0);
  m_mInputDecisionHistory.SetSize(0,0);
  m_mDecisionHistory.SetSize(0,0);

  DeleteAllPurposes();
  DeleteAllSensors();
  DeleteAllReactions();
  DeleteAllBrainMemorizedEntityIdentities();
  if (m_pFeelingWelfare!=NULL)
    delete m_pFeelingWelfare;
  if (m_pChoiceDecisionBuffer!=NULL)
    delete [] m_pChoiceDecisionBuffer;
  if(m_pGeoMap!=NULL)
    delete m_pGeoMap;
}

//---------------------------------------------------------------------------
// global management
//---------------------------------------------------------------------------

CBasicEntity* CBrain::GetEntity()
{
  return (m_pEntity);
}

void CBrain::NextSecond()
{
  bool purposeEnd = true;
  int curPriority = -1;

  if (m_pCurrentPurpose!=NULL)
  {
    if (m_pGeoMap == NULL)
      CreateGeoMapArroudCurrentPosition();

    purposeEnd = m_pCurrentPurpose->CheckSucces();
    if (purposeEnd)
    {
      // Purpose is a success. Fill map
      if (m_pCurrentPurpose->IsMemorizeSuccess())
      {
        m_pGeoMap->MemorizePurposeSuccessPos(m_pCurrentPurpose->GetUniqueId(), m_pEntity->getGlobalGridCoord(), 10);
      }
    }
    else
    { 
      // Check timeout
      purposeEnd = m_pCurrentPurpose->IncreaseCountAndCheckEnd();
      // Memorize failure position
      if ((purposeEnd) && (m_pGeoMap != NULL) && m_pCurrentPurpose->IsMemorizeSuccess())
      {
        m_pGeoMap->MemorizePurposeSuccessPos(m_pCurrentPurpose->GetUniqueId(), m_pEntity->getGlobalGridCoord(), -1);
      }
    }
  }

  if (!purposeEnd)
  {
    curPriority = m_pCurrentPurpose->GetPriority();
  }

  sensorValType curlevel;
  sensorValType maxlevel = 0;
  size_t selectId = invalidIndex;

  // TBD Select 1 purpose randomely among max levels or specify this order in biorules
  for (size_t i=0; i<m_tPurposes.size(); i++)
  {
    // Select new purpose according to highest sensor level
    if ( (m_tPurposes[i]->GetPriority() > curPriority) && (m_tPurposes[i]->IsStartThresholdReached()) )
      curlevel = m_tPurposes[i]->GetSensorLevel() + m_tPurposes[i]->GetLevelBonus() + MAX_SENSOR_VAL * m_tPurposes[i]->GetPriority();
    else 
      curlevel = 0;

    if (maxlevel<curlevel)
    {
      maxlevel = curlevel;
      selectId = i;
    }
  }

  if (selectId != invalidIndex)
  {
    // Stop previous purpose
    if (m_pCurrentPurpose!=NULL)
    {  
      m_pCurrentPurpose->StopPeriod();
    }
    m_pCurrentPurpose = m_tPurposes[selectId];
    // Start new purpose
    m_pCurrentPurpose->StartPeriod();
  }
  else if ( (purposeEnd) && (m_pCurrentPurpose!=NULL) )
  {
    m_pCurrentPurpose->StopPeriod();
    m_pCurrentPurpose = NULL;
  }
}


void CBrain::NextDay()
{
  if (m_pGeoMap!=NULL)
    m_pGeoMap->NextDay();
}

//---------------------------------------------------------------------------
// Feeling/sensors/reactions/geomap management
//---------------------------------------------------------------------------

bool CBrain::SetNumberInputHistory (size_t nbHistory)
{
  if (nbHistory == 0)
    return false;

  m_nInputHistory = nbHistory;

  return true;
}

bool CBrain::SetNumberExperienceHistory (size_t nbHistory)
{
  if (nbHistory == 0)
  {
    return false;
  }

  m_nExperienceHistory = nbHistory;

  return true;
}

size_t CBrain::AttachSensor (CSensor* pSensor)
{
  m_tSensors.push_back(pSensor);

  if ((pSensor->GetUniqueId()&UID_BASE_MASK) == UID_BASE_SENS_PHEROMONE)
    m_pSensorPheromone = (CSensorPheromone*)pSensor;
  else if ((pSensor->GetUniqueId()&UID_BASE_MASK) == UID_BASE_SENS_SMELL)
    m_pSensorSmell = (CSensorSmell*)pSensor;
  else if ((pSensor->GetUniqueId()&UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY)
    m_bIdentificationUsed = true;

  return (m_tSensors.size()-1);
}

size_t CBrain::AttachReaction (CReaction* pReaction)
{
  m_tReactions.push_back(pReaction);
  return (m_tReactions.size()-1);
}

size_t CBrain::AttachPurpose (CPurpose* pPurpose)
{
  m_tPurposes.push_back(pPurpose);
  return (m_tPurposes.size()-1);
}

bool CBrain::DeleteAllSensors ()
{
  for (size_t i = 0; i < m_tSensors.size(); i++)
  {
    if (m_tSensors[i] != NULL)
      delete m_tSensors[i]; 
  }
  m_tSensors.clear();
  return true;
}

bool CBrain::DeleteAllReactions ()
{
  for (size_t i = 0; i < m_tReactions.size(); i++)
  {
    if (m_tReactions[i] != NULL)
      delete m_tReactions[i];  
  }
  m_tReactions.clear();
  return true;
}

bool CBrain::DeleteAllPurposes ()
{
  for (size_t i = 0; i < m_tPurposes.size(); i++)
  {
    if (m_tPurposes[i] != NULL)
      delete m_tPurposes[i]; 
  }
  m_tPurposes.clear();
  return true;
}

bool CBrain::DeleteAllBrainMemorizedEntityIdentities ()
{
  // loop from top to bottom 
  for (size_t i = 0; i < m_tBrainMemorizedEntityIdentities.size(); i++)
  {
    if (m_tBrainMemorizedEntityIdentities[i] != NULL)
      delete m_tBrainMemorizedEntityIdentities[i]; 
  }
  m_tBrainMemorizedEntityIdentities.clear();
  return true;
}

size_t CBrain::GetNumberSensor ()
{
  return (m_tSensors.size());
}

size_t CBrain::GetNumberReaction ()
{
  return (m_tReactions.size());
}

size_t CBrain::GetNumberPurpose ()
{
  return (m_tPurposes.size());
}

CSensor* CBrain::GetSensorByIndex (size_t index)
{
  return (m_tSensors[index]);
}

CSensor* CBrain::GetSensorByUniqueId(DWORD uniqueId)
{
  for (size_t i=0; i<m_tSensors.size(); i++) 
  {
    if (m_tSensors[i]->GetUniqueId() == uniqueId)
      return m_tSensors[i];
  }
  return NULL;
}

CReaction* CBrain::GetReactionByIndex (size_t index)
{
  return (m_tReactions[index]);
}

CReaction* CBrain::GetReactionByUniqueId(DWORD uniqueId)
{
  for (size_t i=0; i<m_tReactions.size(); i++) 
  {
    if (m_tReactions[i]->GetUniqueId() == uniqueId)
      return m_tReactions[i];
  }
  return NULL;
}

CPurpose* CBrain::GetPurposeByIndex (size_t index)
{
  return (m_tPurposes[index]);
}

CPurpose* CBrain::GetPurposeByUniqueId(DWORD uniqueId)
{
  for (size_t i=0; i<m_tPurposes.size(); i++) 
  {
    if (m_tPurposes[i]->GetUniqueId() == uniqueId)
      return m_tPurposes[i];
  }
  return NULL;
}

CPurpose* CBrain::GetPurposeByLabel(string label)
{
  for (size_t i = 0; i < m_tPurposes.size(); i++)
  {
    if (m_tPurposes[i]->GetLabel() == label)
      return m_tPurposes[i];
  }
  return NULL;
}

CPurpose* CBrain::GetCurrentPurpose (void)
{
  return (m_pCurrentPurpose);
}

size_t CBrain::GetCurrentPurposeIndex(void)
{
  for (size_t i = 0; i < m_tPurposes.size(); i++)
  {
    if (m_tPurposes[i] == m_pCurrentPurpose)
      return i;
  }
  return (invalidIndex);
}

CPurpose*  CBrain::GetPurposeByTriggerSensor(DWORD sensorUniqueId, size_t sensorSubCaptorIndex)
{
  return GetPurposeByUniqueId(CPurpose::ComputeUniqueId(sensorUniqueId, sensorSubCaptorIndex));
}

CGeoMapPurpose* CBrain::GetGeographicMap(void)
{
  return m_pGeoMap;
}

void CBrain::ForceCurrentPurpose(size_t purposeIndex)
{
  if (purposeIndex != invalidIndex)
  {
    if (m_pCurrentPurpose != m_tPurposes[purposeIndex])
    {
      // Stop previous purpose
      if (m_pCurrentPurpose != NULL)
      {
        m_pCurrentPurpose->StopPeriod();
      }
      m_pCurrentPurpose = m_tPurposes[purposeIndex];
      // Start new purpose
      m_pCurrentPurpose->StartPeriod();
    }
  }
  else if (m_pCurrentPurpose != NULL)
  {
    m_pCurrentPurpose->StopPeriod();
    m_pCurrentPurpose = NULL;
  }
}

bool CBrain::PollAllSensors (void)
{
  sensorValType curSensorVal = 0;
  int bonusRate;
  size_t cpt = 0;
  size_t i;

  for (auto pSensor : m_tSensors)
  {
    const std::vector<sensorValType>& vectSensorVal{ pSensor->UpdateAndGetStimulationTable()};
    for (i = 0; i < vectSensorVal.size(); i++)
    {
      // Fill pass input (shift all InputSensors blocks down, and *m_historyWeight to give less weight to pass)
      for (int hist=(int)m_nInputHistory-1; hist>0; hist--)
      {
        m_vCurrentDecisionInput(hist*m_nInputSensors+cpt,0) = m_vCurrentDecisionInput((hist-1)*m_nInputSensors+cpt,0)*m_historyWeight;
      }

      // Fill new InputSensors and add purpose bonus
      curSensorVal = vectSensorVal[i];
      if (curSensorVal != 0) // CPU optim: curSensorVal is often 0
      {
        bonusRate = pSensor->GetBonusRate(i);
        if (bonusRate != 100)
        {
          curSensorVal = curSensorVal * (double)bonusRate / 100.0;
        }
        if (curSensorVal > MAX_SENSOR_VAL)
        {
          curSensorVal = MAX_SENSOR_VAL;
        }
        if (curSensorVal < -MAX_SENSOR_VAL)
        {
          curSensorVal = -MAX_SENSOR_VAL;
        }
      }
      m_vCurrentDecisionInput(cpt,0) = curSensorVal;
      cpt++;
    }
  }

  // Give an additional bonus to focused entity (120%)
  if (m_FocusedEntityInfo.computedWeight > 0)
  {
    cpt = GetBrainMatrixRowIndex (m_FocusedEntityInfo.captorUid, 1, m_FocusedEntityInfo.subcaptorIndex, 0);
    if (cpt != invalidIndex)
    {
      for (i=0; i<m_FocusedEntityInfo.subcaptorsSize;i++)
      {
        curSensorVal = m_vCurrentDecisionInput(cpt, 0);
        if (curSensorVal != 0) // CPU optim: curSensorVal is often 0
        {
          curSensorVal = curSensorVal * 1.2;
          if (curSensorVal > MAX_SENSOR_VAL)
          {
            curSensorVal = MAX_SENSOR_VAL;
          }
          if (curSensorVal < -MAX_SENSOR_VAL)
          {
            curSensorVal = -MAX_SENSOR_VAL;
          }
          m_vCurrentDecisionInput(cpt, 0) = curSensorVal;
        }
        cpt++;
      }
    }
  }

#ifdef WIN32
  // CPU optim: Update Identification on focused entity for GUI. No need in console mode
  if (m_FocusedEntityInfo.newEntityId != ENTITY_ID_INVALID)
  {
    CBasicEntity* pFocusedEntity = GetEntity()->getBiotop()->getEntityById(m_FocusedEntityInfo.newEntityId);
    if (pFocusedEntity != NULL)
    {
      // Update Identification on focused entity for GUI 
      ComputeAndGetIdentification(pFocusedEntity);
    }
  }
#endif

  // reset m_FocusedEntityInfo
  clearBrainFocusedEntityInfo();
  m_FocusedEntityInfo.previousEntityId = m_FocusedEntityInfo.newEntityId;
  return true;
}

entityIdType CBrain::getBrainFocusedEntityId(void)
{
  return(m_FocusedEntityInfo.previousEntityId);
}

void CBrain::proposeNewFocusedEntityCandidate(CBasicEntity* pEntity, double computedWeight, double specialWeightBonus,
                                              DWORD captorUid, size_t subcaptorIndex, size_t subcaptorsSize)
{
  if ((pEntity != NULL) && (computedWeight > m_FocusedEntityInfo.computedWeight))
  {
    m_FocusedEntityInfo.newEntityId = pEntity->getId();
    m_FocusedEntityInfo.computedWeight = computedWeight + 1 + specialWeightBonus; // Add 1 to give advantage to first found entity
    m_FocusedEntityInfo.captorUid = captorUid;
    m_FocusedEntityInfo.subcaptorIndex = subcaptorIndex;
    m_FocusedEntityInfo.subcaptorsSize = subcaptorsSize;
  }
}

void CBrain::ForceBrainFocusedEntityId(entityIdType focusedEntityId)
{
  m_FocusedEntityInfo.previousEntityId = focusedEntityId;
}

void CBrain::ResetReactionsFailureSuccessFactor()
{
  for (size_t i=0; i<m_tReactions.size(); i++) 
  {
    m_tReactions[i]->ResetSuccessFailureFactor();
  }
}

double CBrain::GetViewedEntityWeight(CBasicEntity* pEntity)
{
  double foundWeight = 0;
  for (auto pSensor : m_tSensors)
  {
    if ( (pSensor->GetUniqueId() & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY )
    {
      CSensorViewIdentify* pViewSens = (CSensorViewIdentify*)pSensor;
      foundWeight = pViewSens->GetViewedEntityWeight(pEntity);
      if (foundWeight>0)
        return foundWeight;
    }
    if ( (pSensor->GetUniqueId() & UID_BASE_MASK) == UID_BASE_SENS_VIEW_FAR )
    {
      CSensorViewFar* pViewSens = (CSensorViewFar*)pSensor;
      foundWeight = pViewSens->GetViewedEntityWeight(pEntity);
      if (foundWeight>0)
        return foundWeight;
    }
    if ( (pSensor->GetUniqueId() & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY_FAR )
    {
      CSensorViewIdentifyFar* pViewSens = (CSensorViewIdentifyFar*)pSensor;
      foundWeight = pViewSens->GetViewedEntityWeight(pEntity);
      if (foundWeight>0)
        return foundWeight;
    }
  }
  return foundWeight;
}


//---------------------------------------------------------------------------
// Brain file management
//---------------------------------------------------------------------------
bool CBrain::saveInXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  xmlDoc.LoadFile();
  saveInTiXmlFile(&xmlDoc);
  resu = xmlDoc.SaveFile();
  return resu;
}

bool CBrain::saveInTiXmlFile(TiXmlDocument *pXmlDoc)
{
  TiXmlNode* pNodeEntity = NULL;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeChild = NULL;
  TiXmlElement* pElement = NULL;
  int i;

  if (pXmlDoc==NULL) 
    return false;

  pNodeEntity = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if (pNodeEntity==NULL)
  {
    TiXmlElement newNode(XML_NODE_ENTITY);
    pNodeEntity = pXmlDoc->InsertEndChild(newNode);
  }

  // Actions feedback
  pNode = pNodeEntity->FirstChild(XML_NODE_ACTION_LEVEL);
  if (pNode==NULL)
  {
    // Create action level node
    TiXmlElement newNode(XML_NODE_ACTION_LEVEL);
    pNode = pNodeEntity->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous actions
    while (pNode->FirstChild() != NULL)
      pNode->RemoveChild(pNode->FirstChild());
  }

  for (i=0; i<GetNumberReaction(); i++)
  {
    TiXmlElement newAction(XML_NODE_ACTION);
    pNodeChild = pNode->InsertEndChild(newAction);
    if (pNodeChild != NULL) 
    {
      pElement = (TiXmlElement*)pNodeChild;
      pElement->SetAttribute(XML_ATTR_INDEX, i);
      pElement->SetDoubleAttribute(XML_ATTR_SUCCESS, GetReactionByIndex(i)->GetSuccessSatisfactionFactor());
      pElement->SetDoubleAttribute(XML_ATTR_FRUSTRATION, GetReactionByIndex(i)->GetFailureFrustrationFactor());
    }
  }

  // memorized visual entities
  pNode = pNodeEntity->FirstChild(XML_NODE_MEMO_ENTITY);
  if (pNode==NULL)
  {
    // Create action level node
    TiXmlElement newNode(XML_NODE_MEMO_ENTITY);
    pNode = pNodeEntity->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous memo
    while (pNode->FirstChild() != NULL)
      pNode->RemoveChild(pNode->FirstChild());
  }

  for (i=0; i<m_tBrainMemorizedEntityIdentities.size(); i++)
  {
    TiXmlElement newMemo(XML_NODE_MEMO);
    pNodeChild = pNode->InsertEndChild(newMemo);
    if (pNodeChild != NULL) 
    {
      pElement = (TiXmlElement*)pNodeChild;
      pElement->SetAttribute(XML_ATTR_INDEX, i);
      pElement->SetAttribute(XML_ATTR_IDENTITY,  m_tBrainMemorizedEntityIdentities[i]->identity);
      pElement->SetAttribute(XML_ATTR_SIGNATURE, m_tBrainMemorizedEntityIdentities[i]->entitySignature);
    }
  }

  // Save Neurone tables
  GetDecisionNeuronTable()->saveInXmlFile(pXmlDoc);
  GetIdentifyNeuronTable()->saveInXmlFile(pXmlDoc);

  return true;
}

bool CBrain::loadFromXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  resu = xmlDoc.LoadFile();
  loadFromTiXmlFile(&xmlDoc);
  return resu;
}

bool CBrain::loadFromTiXmlFile(TiXmlDocument *pXmlDoc)
{
  bool resu = false;

  if (pXmlDoc==NULL)
    return resu;

  resu = GetIdentifyNeuronTable()->loadFromXmlFile(pXmlDoc);
  resu = GetDecisionNeuronTable()->loadFromXmlFile(pXmlDoc);

  if (resu == true)
  {
    TiXmlElement* pElement;
    TiXmlNode* pNode = NULL;
    int index;
    CReaction* pReaction = NULL;

    // Actions feedback
    double succes, frustration;
    pNode = pXmlDoc->FirstChild(XML_NODE_ENTITY);
    if (pNode!=NULL)
      pNode = pNode->FirstChild(XML_NODE_ACTION_LEVEL);
    if (pNode!=NULL)
    {
      pNode = pNode->FirstChild(XML_NODE_ACTION);
      while (pNode != NULL)
      {
        if (pNode->Type() == TiXmlNode::TINYXML_ELEMENT)
        {
          pElement = (TiXmlElement*)pNode;
          succes = 0.0;
          frustration = 0.0;
          pElement->QueryDoubleAttribute(XML_ATTR_SUCCESS,  &succes);
          pElement->QueryDoubleAttribute(XML_ATTR_FRUSTRATION,  &frustration);

          if ( pElement->QueryIntAttribute(XML_ATTR_INDEX,  &index) != TIXML_NO_ATTRIBUTE)
          {
            pReaction = GetReactionByIndex(index);
            if (pReaction)
            {
              pReaction->SetSuccessSatisfactionFactor(succes);
              pReaction->SetFailureFrustrationFactor(frustration);
            }
          }
        }
        pNode = pNode->NextSibling();
      }
    }

    // Memorized entities
    entitySignatureType   entitySignature;
    IdentificationType_e  identity;
    //double readDoubleValue;
    int readIntValue;

    pNode = pXmlDoc->FirstChild(XML_NODE_ENTITY);
    if (pNode!=NULL)
      pNode = pNode->FirstChild(XML_NODE_MEMO_ENTITY);
    if (pNode!=NULL)
    {
      pNode = pNode->FirstChild(XML_NODE_MEMO);
      while (pNode != NULL)
      {
        if (pNode->Type() == TiXmlNode::TINYXML_ELEMENT)
        {
          pElement = (TiXmlElement*)pNode;
          entitySignature = 0;
          identity = IDENTIFICATION_NEUTRAL;
          pElement->QueryIntAttribute(XML_ATTR_SIGNATURE, &readIntValue);
          entitySignature = (entitySignatureType)readIntValue;
          pElement->QueryIntAttribute(XML_ATTR_IDENTITY, &readIntValue);
          identity = (IdentificationType_e)readIntValue;
 
          if ( pElement->QueryIntAttribute(XML_ATTR_INDEX,  &index) != TIXML_NO_ATTRIBUTE)
          {
            CheckIfEntityIdentityNotMemorized(entitySignature, identity);
          }
        }
        pNode = pNode->NextSibling();
      }
    }

  }

  return resu;
}


bool CBrain::exportDecisionInCsvFile(string fileNameWithPath)
{
  size_t i, j;
  string tmpStr;
  string savedBrain = " ;";

  // First line
  for (j=0; j<m_mDecisionNeuronTable.GetNeuronTableColumnCount(); j++)
  {
    savedBrain += GetColumnLabel(j) + ";";
  }
  savedBrain += "\n";

  // Next lines
  for (i=0; i<m_mDecisionNeuronTable.GetNeuronTableRowCount(); i++)
  {
    // First column
    savedBrain += GetRowLabel(i) + ";";
    // Next columns
    for (j=0; j<m_mDecisionNeuronTable.GetNeuronTableColumnCount();j++)
    {
      tmpStr = FormatString("%02.8f", m_mDecisionNeuronTable.GetNeuronTableData(i,j));
      savedBrain += tmpStr + ";";
    }
    savedBrain += "\n";
  }

  ofstream f1;
  f1.open(fileNameWithPath.c_str());
  f1.write(savedBrain.c_str(), savedBrain.length());
  f1.close(); 

  return true;
}


bool CBrain::importDecisionFromCsvFile(string fileNameWithPath)
{
  string loadedBrain;
  char* pbuf;
  std::streamoff begin, end;
  std::streamoff fLength;
  ifstream f1;
  bool resu = false;

  f1.open( fileNameWithPath.c_str());
  begin = f1.tellg();
  f1.seekg(0, ios::end);
  end = f1.tellg();
  fLength = (end - begin);
  f1.seekg(0, ios::beg);

  if ( (fLength==0) || (fLength>3000000) )
  {
    return (false);
  }

  pbuf = new char[fLength+1];
  memset(pbuf,0,fLength+1);
  f1.read(pbuf,fLength);
  loadedBrain = pbuf;
  f1.close();

  delete[] pbuf;

  if (loadedBrain.length()>0)
  {
    std::vector<std::string> vectColumnLabel;
    std::vector<std::string> vectRowLabel;
    size_t nbColumnsInFile, nbRowsInFile;

    string readStr;
    size_t indexStart = 0, indexEnd = 0, indexStartLine = 0, indexEndLine = 0;
    indexEndLine = loadedBrain.find("\n",indexStartLine);

    // skip 1st column that should be empty
    indexStart = loadedBrain.find(";",0) + 1;
    if ((indexStart<2) || (indexStart>3))
      return false; // format error, first element should be empty

    // parse 1st row. count nb of columns
    while (indexStart<indexEndLine)
    {
      indexEnd = loadedBrain.find(";",indexStart);
      readStr = loadedBrain.substr(indexStart,indexEnd-indexStart);
      vectColumnLabel.push_back(readStr);
      indexStart = indexEnd + 1;
    }
    nbColumnsInFile = vectColumnLabel.size();
    indexStart++; // go to next line

    // parse 1st column. count nb of rows
    while (indexStart<loadedBrain.length())
    {
      indexEnd = loadedBrain.find(";",indexStart);
      readStr = loadedBrain.substr(indexStart,indexEnd-indexStart);
      vectRowLabel.push_back(readStr);
      indexEnd = loadedBrain.find("\n",indexStart);
      indexStart = indexEnd + 1;
    }
    nbRowsInFile = vectRowLabel.size();

    // Build and file data matrix
    CNeuronMatrix dataMatrixInFile;
    size_t rowIndex=0, columnIndex=0;
    double avarageValInFile; 
    dataMatrixInFile.InitializeNeuronMatrixNeutral("", nbRowsInFile, nbColumnsInFile);
    dataMatrixInFile.ResetNeuronMatrixNeutral();
    avarageValInFile = dataMatrixInFile.GetNeuronTableData(0,0);
    string readLine;

    indexStartLine = indexEndLine + 1;
    while (indexStartLine<loadedBrain.length())
    {
      indexStartLine = loadedBrain.find(";",indexStartLine) + 1; // Goto first data, skip first column
      indexEndLine = loadedBrain.find("\n",indexStartLine);

      readLine = loadedBrain.substr(indexStartLine,indexEndLine-indexStartLine);
      indexStart = 0;

      for (columnIndex=0; columnIndex<nbColumnsInFile; columnIndex++)
      {
        indexEnd = readLine.find(";",indexStart);
        readStr = readLine.substr(indexStart,indexEnd-indexStart);
        dataMatrixInFile.SetNeuronTableData(rowIndex, columnIndex,atof(readStr.c_str())); 
        indexStart = indexEnd + 1;
      }

      indexStartLine = indexEndLine + 1;
      rowIndex ++;
    }

    // Now, set file data into DecisionNeuronTable
    CNeuronMatrix* pBrainNeuronMatrix = GetDecisionNeuronTable();
    double avarageValInBrain; 
    pBrainNeuronMatrix->ResetNeuronMatrixNeutral();
    avarageValInBrain = pBrainNeuronMatrix->GetNeuronTableData(0,0);

    // Column mapping in case of actions from file don't match brain
    size_t i, j;
    std::vector<size_t> columnMaping;
    columnMaping.resize(GetNumberReaction());
    for (i = 0; i < columnMaping.size(); i++)
    {
      columnMaping[i] = -1;
      for(j=0; j<nbColumnsInFile; j++)
      {
        if (GetColumnLabel(i).find(vectColumnLabel[j], 0) == 0)
        {
          columnMaping[i] = j;
          break;
        }
      }
    }

    // For each line in brain try to find corresponding line in csv file
    double scale = avarageValInBrain / avarageValInFile;
    double newValue;
    for (i=0; i<m_mDecisionNeuronTable.GetNeuronTableRowCount(); i++)
    {
      for(j=0; j<nbRowsInFile; j++)
      {
        if (GetRowLabel(i).find(vectRowLabel[j], 0) == 0)
        {
          for (columnIndex=0; columnIndex<GetNumberReaction(); columnIndex++)
          {
            newValue = scale * dataMatrixInFile.GetNeuronTableData(j, columnMaping[columnIndex]);
            pBrainNeuronMatrix->SetNeuronTableData(i, columnIndex, newValue);
          }
          break;
        }
      }
    }

    pBrainNeuronMatrix->NormalizeNeuronMatrix();

    // FOR THE MOMENT SIMPLE OVERWRITE WITHOUT LABEL CHECK
    /*CNeuronMatrix* pBrainNeuronMatrix = GetDecisionNeuronTable();
    for (rowIndex=0; rowIndex<nbRowsInFile; rowIndex++)
      for (columnIndex=0; columnIndex<nbColumnsInFile; columnIndex++)
        pBrainNeuronMatrix->SetNeuronTableData(rowIndex, columnIndex, dataMatrixInFile(rowIndex, columnIndex));*/
  }

  return (resu);
}

bool CBrain::exportIdentificationInCsvFile(string fileNameWithPath)
{
  size_t i, j;
  string tmpStr;
  string savedBrain = " ;";

  // First line
  for (j = 0; j < m_mIdentifyNeuronTable.GetNeuronTableColumnCount(); j++)
  {
    savedBrain += getIdentificationLabel(j) + ";";
  }
  savedBrain += "\n";

  // Next lines
  for (i = 0; i < m_mIdentifyNeuronTable.GetNeuronTableRowCount(); i++)
  {
    // First column
    savedBrain += getIdentifyInputLabel(i) + ";";
    // Next columns
    for (j = 0; j < m_mIdentifyNeuronTable.GetNeuronTableColumnCount(); j++)
    {
      tmpStr = FormatString("%02.3f", m_mIdentifyNeuronTable.GetNeuronTableData(i, j));
      savedBrain += tmpStr + ";";
    }
    savedBrain += "\n";
  }

  ofstream f1;
  f1.open(fileNameWithPath.c_str());
  f1.write(savedBrain.c_str(), savedBrain.length());
  f1.close();

  return true;
}

//---------------------------------------------------------------------------
// Brain decision process
//---------------------------------------------------------------------------

CNeuronMatrix* CBrain::GetDecisionNeuronTable()
{
  return (&m_mDecisionNeuronTable);
}

bool CBrain::InitializeNeuronTableNeutral()
{
  // Initialize decision table
  m_nInputSensors = 0;
  for (auto pSensor : m_tSensors)
  {
    if (pSensor != NULL)
    {
      m_nInputSensors += pSensor->GetSubCaptorNumber();
    }
    else
    {
      // At this point, all sensors must be attached properly
      return (false);
    }
  }
  size_t nOutputReactions = m_tReactions.size();
  m_vCurrentDecisionInput.SetSize(m_nInputSensors*m_nInputHistory,1);
  for (size_t i=0; i<m_vCurrentDecisionInput.RowNo(); i++)
  {
    m_vCurrentDecisionInput(i,0) = 0;
  }
  m_mDecisionNeuronTable.InitializeNeuronMatrixNeutral(XML_NODE_BRAIN, m_nInputSensors*m_nInputHistory,nOutputReactions);
  m_mInputDecisionHistory.SetSize(m_nInputSensors*m_nInputHistory,m_nExperienceHistory);
  m_vCurrentDecisionChoice.SetSize(nOutputReactions,1);
  if (m_pChoiceDecisionBuffer!=NULL)
    delete [] m_pChoiceDecisionBuffer;
  m_pChoiceDecisionBuffer = new size_t[MAX_CHOICE_BUFFER_SIZE_PER_CHOICE*nOutputReactions];
  m_mDecisionHistory.SetSize(nOutputReactions,m_nExperienceHistory);

  // Initialize identification table
  m_nInputIdentification = IDENTIFY_INPUT_SIZE;
  m_vCurrentIdentifyInput.SetSize(m_nInputIdentification,1);
  m_vCurrentIdentifyOutput.SetSize(IDENTIFICATION_NUMBER_TYPE,1);
  m_vCurrentIdentificationChoice.SetSize(IDENTIFICATION_NUMBER_TYPE,1);
  m_mIdentifyNeuronTable.InitializeNeuronMatrixNeutral(XML_NODE_IDENTIFY, m_nInputIdentification,IDENTIFICATION_NUMBER_TYPE);
  ReenforceFullIdentity(4, IDENTIFICATION_NEUTRAL); // Initialize identification to neutral
  return true;
}

bool CBrain::HistorizeInput (void)
{
  for (int hist=(int)m_nExperienceHistory-1; hist>0; hist--)
  {
    // Fill input history (shift all Input vect on righ, and *m_historyWeight to give less weight to pass)
    for (size_t i=0; i<m_mInputDecisionHistory.RowNo(); i++)
    {
      m_mInputDecisionHistory(i,hist) = m_mInputDecisionHistory(i,hist-1)*m_historyWeight;
    }
  }
  // Insert current input
  for (size_t i=0; i<m_mInputDecisionHistory.RowNo(); i++)
  {
    m_mInputDecisionHistory(i,0) = m_vCurrentDecisionInput(i,0);
  }
  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CBrain::SetBonusAndGetVectorChoiceThresholds
//  
// DESCRIPTION:  compute decision thresholds 
//
//               maxVal         -----------------------------------
//               midThreshold   ----------------------------------- - 75% - curiosity
//               lowThreshold   ----------------------------------- | - 50% - 2x curiosity
//                                                                  | |
//               avarageVal     ----------------------------------- V V
// 
// ARGUMENTS:    in : double curiosityRate
//               out: neuroneValType &maxVal, neuroneValType &midThreshold, neuroneValType &lowThreshold
//   
// RETURN VALUE: bool error case
//  
// REMARKS:      None
//---------------------------------------------------------------------------  
bool CBrain::SetBonusAndGetVectorChoiceThresholds(double curiosityRate, neuroneValType &maxVal, neuroneValType &midThreshold, neuroneValType &lowThreshold)
{
  size_t i;
  size_t nOutputReactions = m_tReactions.size();
  int bonusRate;
  neuroneValType avarageVal = 0;

  // Find max value
  maxVal = -100;
  for (i=0; i<nOutputReactions; i++)
  {
    // Add purpose bonus
    bonusRate = m_tReactions[i]->GetBonusRate();
    if (bonusRate!=100)
    {
      m_vCurrentDecisionChoice(i,0) = m_vCurrentDecisionChoice(i,0) * (double)bonusRate / 100.0;
    }
    
    // compute avarage
    avarageVal += m_vCurrentDecisionChoice(i,0);
    // compute max
    if (m_vCurrentDecisionChoice(i,0) > maxVal)
    {
      // New max
      maxVal = m_vCurrentDecisionChoice(i,0);
    }
  }

  // compute avarage
  avarageVal = avarageVal/nOutputReactions;

  // Process threshold value for selection 
  // (Allows to take a decison not the best choice, but among the good choices
  lowThreshold = avarageVal + (maxVal-avarageVal) * (50.0-(double)curiosityRate*2.0) / 100.0;
  midThreshold = avarageVal + (maxVal-avarageVal) * (75.0-(double)curiosityRate) / 100.0;

  return (true);
}

choiceIndType CBrain::ComputeAndGetDecision (double curiosityRate, ReactionIntensityType_e &intensity)
{
  size_t i, j;
  neuroneValType maxVal = -100;
  neuroneValType curVal = -100;
  neuroneValType thresholdVal = 0;
  neuroneValType thresholdMidVal = 0;
  neuroneValType avarageVal = 0;
  choiceIndType  resuIndex = invalidIndex;
  size_t cptMaxVal = 0;
  size_t nOutputReactions = m_tReactions.size();

  m_mDecisionNeuronTable.ComputeVectorChoice(m_vCurrentDecisionInput, m_vCurrentDecisionChoice);

  SetBonusAndGetVectorChoiceThresholds(curiosityRate, maxVal, thresholdMidVal, thresholdVal);

  // Count values over threshold
  for (i=0; i<nOutputReactions; i++)
  {
    curVal = m_vCurrentDecisionChoice(i,0);
    avarageVal += curVal;

    if (curVal == maxVal) // Give MAX_CHOICE_BUFFER_SIZE_PER_CHOICE chances to maxVal
    {
      for (j=0;j<MAX_CHOICE_BUFFER_SIZE_PER_CHOICE;j++)
      {
        m_pChoiceDecisionBuffer[cptMaxVal] = i;
        cptMaxVal++;
      }
    }
    else if (curVal > thresholdMidVal) // Give 2 chances 
    {
      for (j=0;j<2;j++)
      {
        m_pChoiceDecisionBuffer[cptMaxVal] = i;
        cptMaxVal++;
      }
    }
    else if (curVal > thresholdVal) // Give 1 chance
    {
      for (j=0;j<1;j++)
      {
        m_pChoiceDecisionBuffer[cptMaxVal] = i;
        cptMaxVal++;
      }
    }
  }

  avarageVal = avarageVal/nOutputReactions;

  if (cptMaxVal>0) // check if no error
  {
    // Select an index with value over threshold
    size_t randomInd = getRandInt(cptMaxVal-1);
    resuIndex = m_pChoiceDecisionBuffer[randomInd];

    // Compute intensity
    double ratio = m_vCurrentDecisionChoice(resuIndex,0) / avarageVal;
    intensity = REACTION_INTENSITY_MEDIUM;
    if (ratio<1.02)
      intensity = REACTION_INTENSITY_LOW;
    else if (ratio>1.12) // Fred: a regler
      intensity = REACTION_INTENSITY_HIGH;
  }
  else
  {
    CYBIOCORE_LOG("BRAIN - ERROR ComputeAndGetDecision : Choice error for entity name %s \n", this->GetEntity()->getLabel().c_str());
  }
  return resuIndex;
}

feedbackValType CBrain::ComputeFeedback (choiceIndType myChoice, ReactionIntensityType_e myIntensity)
{
  feedbackValType resu;
  if (m_tReactions[myChoice] == NULL)
  {
    return 0;
  }
  m_CurrentReactionChoice = myChoice;

  m_tReactions[myChoice]->ExecuteAction(myIntensity);
  m_tReactions[myChoice]->IncreaseExecuteCount();

  m_PreviousFeelingWelfare = m_CurrentFeelingWelfare;
  m_CurrentFeelingWelfare  = m_pFeelingWelfare->ComputeAndGetFeelingWelfare();

  // Feedback is the difference between current feedback and previous...
  resu = (m_CurrentFeelingWelfare - m_PreviousFeelingWelfare) * MAX_FEEDBACK_VAL / 100.0;

  if (resu>MEMORIZATION_FEEDBACK_THRESHOLD)
    m_tReactions[myChoice]->IncreaseSuccessCount();

  if (resu<-MEMORIZATION_FEEDBACK_THRESHOLD)
    m_tReactions[myChoice]->IncreaseFailureCount();

  return (resu);
}

feedbackValType CBrain::GetCurrentFeedback ()
{
  // Feedback is the difference between current feedback and previous...
  feedbackValType resu = (m_CurrentFeelingWelfare - m_PreviousFeelingWelfare) * MAX_FEEDBACK_VAL / 100.0;

  // Filter
  if ((-MEMORIZATION_FEEDBACK_THRESHOLD<resu)&&(resu<MEMORIZATION_FEEDBACK_THRESHOLD))
    resu = 0;

  return (resu);
}

bool CBrain::HistorizeDecision (choiceIndType index)
{
  for (int hist=(int)m_nExperienceHistory-1; hist>0; hist--)
  {
    // Fill input history (shift all Input vect on righ)
    for (size_t i=0; i<m_mDecisionHistory.RowNo(); i++)
    {
      m_mDecisionHistory(i,hist) = m_mDecisionHistory(i,hist-1);
    }
  }
    // Insert current input
  for (size_t i=0; i<m_mDecisionHistory.RowNo(); i++)
  {
    m_mDecisionHistory(i,0) = 0;
  }
  m_mDecisionHistory(index,0) = 1;

  return true;
}

bool CBrain::MemorizeExperience (feedbackValType currentFeedback, double learningRate)
{
  // Small feedback should not be memorize because it is most of the time non significative but occurs very often
  if ((currentFeedback < MEMORIZATION_FEEDBACK_THRESHOLD) && (currentFeedback > -MEMORIZATION_FEEDBACK_THRESHOLD))
    return false;

  if (currentFeedback > MAX_FEEDBACK_VAL)
  {
    CYBIOCORE_LOG_TIME(GetEntity()->getBiotop()->getBiotopTime());
    CYBIOCORE_LOG("BRAIN - warning MemorizeExperience :currentFeedback too big: %f\n", currentFeedback);
    currentFeedback = MAX_FEEDBACK_VAL;
  }
  else if (currentFeedback < -MAX_FEEDBACK_VAL)
  {
    CYBIOCORE_LOG_TIME(GetEntity()->getBiotop()->getBiotopTime());
    CYBIOCORE_LOG("BRAIN - warning MemorizeExperience :currentFeedback too big negative: %f\n", currentFeedback);
    currentFeedback = -MAX_FEEDBACK_VAL;
  }
  // coef is the coefficient of effect of the previous action
  // The effect of an action cannot exceed 1% when learningRate is at 100%
  neuroneValType coef = currentFeedback*learningRate/MAX_FEEDBACK_VAL/MAX_SENSOR_VAL/10000; 
 
  m_mDecisionNeuronTable.MemorizeExperience(coef, &m_mInputDecisionHistory, &m_mDecisionHistory);

  return true;
}


CMatrix* CBrain::GetChoiceVect()
{
  return (&m_vCurrentDecisionChoice);
}

CMatrix* CBrain::GetInputVect()
{
  return (&m_vCurrentDecisionInput);
}

double CBrain::GetPreviousFeelingWelfare()
{
  return (m_PreviousFeelingWelfare);
}

double CBrain::GetCurrentFeelingWelfare()
{
  return (m_CurrentFeelingWelfare);
}

CFeelingWelfare* CBrain::GetpFeelingWelfare()
{
  return (m_pFeelingWelfare);
}

size_t CBrain::GetCurrentReactionIndex(void)
{
  return ((size_t)m_CurrentReactionChoice);
}

bool CBrain::SetCurrentReactionIndex(size_t index)
{
  bool resu = false;
  if (index < GetNumberReaction())
  {
    m_CurrentReactionChoice = index;
    resu = true;
  }
  return (resu);
}

size_t CBrain::GetReactionIndexByLabel(string label)
{
  for (size_t i=0; i<m_tReactions.size(); i++) 
  {
    if (m_tReactions[i]->GetLabel() == label)
      return i;
  }
  return invalidIndex;
}

size_t CBrain::GetChoiceVectSize()
{
  
  return (m_vCurrentDecisionChoice.RowNo()); 
}

neuroneValType CBrain::GetChoiceVectData(size_t pos)
{
  return (m_vCurrentDecisionChoice(pos,0));
}

size_t CBrain::GetInputVectSize()
{
  return (m_vCurrentDecisionInput.RowNo());
}

neuroneValType CBrain::GetInputVectData(size_t pos)
{
  return (m_vCurrentDecisionInput(pos,0));
}

string CBrain::GetColumnLabel(size_t index)
{
  if (index >= GetNumberReaction())
  {
    return ("Out of range");
  }
  else
  {
    return (GetReactionByIndex(index)->GetLabel());
  }
}

string CBrain::GetRowLabel(size_t index)
{
  if (index >= m_mDecisionNeuronTable.GetNeuronTableRowCount())
  {
    return ("Out of range");
  }
  else
  {
    size_t offset = index / m_nInputSensors;
    size_t pos = offset * m_nInputSensors;
    for (size_t sensId=0; sensId<GetNumberSensor(); sensId++)
    {
      if ( (pos <= index) && (index < pos+GetSensorByIndex(sensId)->GetSubCaptorNumber()) )
      {
        string label;
        label = FormatString("T-%d ", offset);
        label += GetSensorByIndex(sensId)->GetLabel() + " " + GetSensorByIndex(sensId)->GetSubCaptorLabel(index-pos);

        // FRED temp
        if (IsDecisionRowSexSpecific(index))
          label += " (sex)";

        return (label);
      }
      pos += GetSensorByIndex(sensId)->GetSubCaptorNumber();
    }
  }
  return ("Not found");
}


size_t CBrain::GetBrainMatrixColumnIndex(DWORD reactionUidBase, size_t reactionUidRange, size_t foundIndex)
{
  size_t foundCount = 0;
  for (size_t reacId=0; reacId<GetNumberReaction(); reacId++)
  {
    DWORD uid = GetReactionByIndex(reacId)->GetUniqueId();
    if ( ( uid >= reactionUidBase) && (uid < (reactionUidBase + reactionUidRange)) )
    {
      if (foundCount == foundIndex)
        return (reacId);
      else
        foundCount++;
    }
  }
  return (invalidIndex);
}

size_t CBrain::GetBrainMatrixRowIndex(DWORD sensorUidbase, size_t sensorUidRange, size_t subCaptorIndex, 
                                            size_t timeHistory, size_t foundIndex)
{
  size_t pos = 0;
  size_t foundCount = 0;
  for (size_t sensId = 0; sensId<GetNumberSensor(); sensId++)
  {
    CSensor* pSens = GetSensorByIndex(sensId);
    if ( ( pSens->GetUniqueId() >= sensorUidbase) && (pSens->GetUniqueId() < (sensorUidbase + sensorUidRange)) )
    {
      size_t resu = timeHistory * m_nInputSensors + pos + subCaptorIndex;
      if (resu < m_mDecisionNeuronTable.GetNeuronTableRowCount())
      {
        if (foundCount == foundIndex)
          return resu;
        else
          foundCount++;
      }
      else
      {
        return (invalidIndex);
      }
    }
    pos += pSens->GetSubCaptorNumber();
  }
  return (invalidIndex);
}


bool CBrain::IsDecisionRowSexSpecific(size_t rowIndex)
{
  size_t index = rowIndex % m_nInputSensors;

  if (rowIndex >= m_mDecisionNeuronTable.GetNeuronTableRowCount())
  {
    return false;
  }
  else
  {
    size_t pos = 0;
    for (size_t sensId=0; sensId<GetNumberSensor(); sensId++)
    {
      if ( (pos <= index) && (index < pos+GetSensorByIndex(sensId)->GetSubCaptorNumber()) )
      {
        return (GetSensorByIndex(sensId)->IsSexSpecific(index-pos));
      }
      pos += GetSensorByIndex(sensId)->GetSubCaptorNumber();
    }
  }

  return false;
}


bool CBrain::ChangeDecisionNeuronTableVal(size_t row, size_t col, double variation, bool normalize)
{
  return m_mDecisionNeuronTable.ChangeNeuronTableVal( row, col, variation, normalize);
}

bool CBrain::AddFeelingWelfareSensitivity(CSensor* pSens, size_t tableSensiSize, double* pTableSensi)
{
  return ( m_pFeelingWelfare->AddSensitivity(pSens,tableSensiSize,pTableSensi) );
}


//---------------------------------------------------------------------------
// Brain identification process
//---------------------------------------------------------------------------

bool CBrain::IsIdentificationUsed()
{
  return m_bIdentificationUsed;
}

CNeuronMatrix* CBrain::GetIdentifyNeuronTable()
{
  return (&m_mIdentifyNeuronTable);
}

size_t CBrain::GetIdentifyMatrixRowIndex(DWORD sensorUidbase, size_t subCaptorIndex)
{
  size_t index;
  switch (sensorUidbase)
  {
  case UID_BASE_SENS_VIEW:
    index = subCaptorIndex;
    break;

  case UID_BASE_SENS_SMELL:
    index = VIEW_SIZE_PER_FOCUS + subCaptorIndex;
    break;

  case UID_BASE_SENS_PHEROMONE:
    index = VIEW_SIZE_PER_FOCUS + NUMBER_ODORS + subCaptorIndex;
    break;

  default:
    index = invalidIndex;
    break;
  }
  return index;
}

bool CBrain::ChangeIdentifyNeuronTableVal(size_t row, size_t col, double variation, bool normalize)
{
  return m_mIdentifyNeuronTable.ChangeNeuronTableVal( row, col, variation, normalize);

}

string CBrain::getIdentificationLabel(size_t columnIndex)
{
  string label;
  label = IdentificationTypeNameList[columnIndex];
  return (label);
}

string CBrain::getIdentifyInputLabel(size_t rowIndex)
{
  string captorStr;
  
  if (rowIndex>IDENTIFY_INPUT_SIZE)
    captorStr = "captor error";
  else if (rowIndex >= (VIEW_SIZE_PER_FOCUS + NUMBER_ODORS + NUMBER_PHEROMONES))
  {
    captorStr = "non static";
  }
  else if (rowIndex>=(VIEW_SIZE_PER_FOCUS + NUMBER_ODORS))
  {
    if (m_pSensorPheromone != NULL)
      captorStr ="phero " + m_pSensorPheromone->GetSubCaptorLabel(rowIndex - VIEW_SIZE_PER_FOCUS - NUMBER_ODORS);
    else
      captorStr = "no Pheromone captor";
  }
  else if (rowIndex>=(VIEW_SIZE_PER_FOCUS))
  {
    if (m_pSensorSmell != NULL)
      captorStr = "odor " + m_pSensorSmell->GetSubCaptorLabel(rowIndex - VIEW_SIZE_PER_FOCUS);
    else
      captorStr = "no Smell captor";
  }
  else if (rowIndex>(3+VIEW_NUMBER_COLORS+VIEW_NUMBER_FORMS+VIEW_NUMBER_TEXTURES))
    captorStr = "attr " + CBasicEntity::getPhyAttributeStrName((PhyAttributeType_e)(rowIndex + PHY_ATTRIBUTE_NUMBER_TYPE - VIEW_SIZE_PER_FOCUS));
  else if (rowIndex>(3+VIEW_NUMBER_COLORS+VIEW_NUMBER_FORMS))
    captorStr = "text " + CBasicEntity::getTextureStrName(indexToTextureType(rowIndex - 4 - VIEW_NUMBER_COLORS - VIEW_NUMBER_FORMS));
  else if (rowIndex>(3+VIEW_NUMBER_COLORS))
    captorStr = "form " + CBasicEntity::getFormStrName(indexToFormType(rowIndex - 4 - VIEW_NUMBER_COLORS));
  else if (rowIndex>3)
    captorStr = "colr " + CBasicEntity::getColorStrName(indexToColorType(rowIndex - 4));
  else if (rowIndex==3)
    captorStr = "relative speed escape";
  else if (rowIndex==2)
    captorStr = "relative speed approach";
  else if (rowIndex==1)
    captorStr = "smaller";
  else
    captorStr = "bigger";
 
  // FRED temp
  if (IsIdentifyRowSexSpecific(rowIndex))
    captorStr += " (sex)";

  return captorStr;
}


bool CBrain::IsIdentifyRowSexSpecific(size_t rowIndex)
{
  if (rowIndex>IDENTIFY_INPUT_SIZE)
  {
    return false;
  }
  else if (rowIndex >= (VIEW_SIZE_PER_FOCUS + NUMBER_ODORS + NUMBER_PHEROMONES))
  {
    return false;
  }
  else if (rowIndex>=(VIEW_SIZE_PER_FOCUS + NUMBER_ODORS))
  {
    if (m_pSensorPheromone != NULL)
      return m_pSensorPheromone->IsSexSpecific(rowIndex - VIEW_SIZE_PER_FOCUS - NUMBER_ODORS);
  }
  else if (rowIndex>=(VIEW_SIZE_PER_FOCUS))
  {
    if (m_pSensorSmell != NULL)
      return m_pSensorSmell->IsSexSpecific(rowIndex - VIEW_SIZE_PER_FOCUS);
  }
  // For visual captors, TBD

  return false;
}


//---------------------------------------------------------------------------
// METHOD:       CBrain::GetVectorIdentifyThresholds
//  
// DESCRIPTION:  compute Identification thresholds 
//
//               maxVal         -----------------------------------
//               highThreshold  ----------------------------------- 95%
//               midThreshold   ----------------------------------- | 75% - curiosity
//               lowThreshold   ----------------------------------- | | 50% - 2x curiosity
//                                                                  | | |
//               avarageVal     ----------------------------------- V V V
// 
// ARGUMENTS:    in : NA
//               out: neuroneValType &maxVal, neuroneValType &midThreshold, neuroneValType &lowThreshold
//   
// RETURN VALUE: bool error case
//  
// REMARKS:      None
//---------------------------------------------------------------------------  
bool CBrain::GetVectorIdentifyThresholds (neuroneValType &highThreshold, neuroneValType &midThreshold, neuroneValType &lowThreshold)
{
  size_t i;
  neuroneValType maxVal = 0;
  neuroneValType avarageVal = 0;

  // Find max value
  for (i=0; i<IDENTIFICATION_NUMBER_TYPE; i++)
  {   
    // compute avarage
    avarageVal += m_vCurrentIdentifyOutput(i,0);
    // compute max
    if (m_vCurrentIdentifyOutput(i,0) > maxVal)
    {
      // New max
      maxVal = m_vCurrentIdentifyOutput(i,0);
    }
  }

  // compute avarage
  avarageVal = avarageVal/IDENTIFICATION_NUMBER_TYPE;

  // Process threshold value for selection 
  // (Allows to take a decison not the best choice, but among the good choices
  lowThreshold  = avarageVal + (maxVal-avarageVal) * 50.0 / 100.0;
  midThreshold  = avarageVal + (maxVal-avarageVal) * 75.0 / 100.0;
  highThreshold = avarageVal + (maxVal-avarageVal) * 95.0 / 100.0;

  return (true);
}

CMatrix* CBrain::ComputeAndGetIdentification(CBasicEntity* pEntity, bool useOdors)
{
  size_t i;
  neuroneValType highThreshold, midThreshold, lowThreshold;

  // Initialize m_vCurrentIdentificationChoice
  for (i = 0; i<IDENTIFICATION_NUMBER_TYPE; i++)
  {
    m_vCurrentIdentificationChoice(i, 0) = 0;
  }

  // If error, just raise NEUTRAL
  if ((pEntity == NULL) || (pEntity->isToBeRemoved()))
  {
    CYBIOCORE_LOG("BRAIN - ERROR ComputeAndGetIdentification : pEntity is NULL or ToBeRemoved\n");
    m_vCurrentIdentificationChoice(IDENTIFICATION_NEUTRAL, 0) = MAX_SENSOR_VAL;
  }

  UpdateIdentifyInputVector(pEntity, useOdors);
  m_mIdentifyNeuronTable.ComputeVectorChoice(m_vCurrentIdentifyInput, m_vCurrentIdentifyOutput);
  GetVectorIdentifyThresholds(highThreshold, midThreshold, lowThreshold);
  
  // If vector is flat, just raise NEUTRAL
  if (highThreshold == lowThreshold)
  {
    m_vCurrentIdentificationChoice(IDENTIFICATION_NEUTRAL,0) = MAX_SENSOR_VAL;
  }
  else
  {
    double count=0;
    for (i=0; i<IDENTIFICATION_NUMBER_TYPE; i++)
    { 
      if (m_vCurrentIdentifyOutput(i,0) >= highThreshold)
      {
        m_vCurrentIdentificationChoice(i,0) = MAX_SENSOR_VAL;
        count+=16;
      }
      else if (m_vCurrentIdentifyOutput(i,0) >= midThreshold)
      {
        m_vCurrentIdentificationChoice(i,0) = MAX_SENSOR_VAL/4;
        count+=4;
      }
      else if (m_vCurrentIdentifyOutput(i,0) >= lowThreshold)
      {
        m_vCurrentIdentificationChoice(i,0) = MAX_SENSOR_VAL/16;
        count+=1;
      }
    }
    // Adjust weight
    for (i=0; i<IDENTIFICATION_NUMBER_TYPE; i++)
    { 
      m_vCurrentIdentificationChoice(i,0) = m_vCurrentIdentificationChoice(i,0) * 16.0 / count;
    }
  }

  return &m_vCurrentIdentificationChoice;
}

void CBrain::UpdateIdentifyInputVector(CBasicEntity* pEntity, bool useOdors)
{
  size_t i, offset = 0;
  if ((pEntity == NULL) || (pEntity->isToBeRemoved()))
  {
    for (i = 0; i < m_nInputIdentification; i++)
      m_vCurrentIdentifyInput(i,0) = 0;
    return;
  }

  // Size big
  double weightRatio = pEntity->getWeight() / m_pEntity->getWeight();
  if (weightRatio <= 1)
    m_vCurrentIdentifyInput(offset,0) = 0;
  else if (weightRatio > MAX_SENSOR_VAL)
    m_vCurrentIdentifyInput(offset,0) = MAX_SENSOR_VAL;
  else
    m_vCurrentIdentifyInput(offset, 0) = weightRatio;
  offset++;
  // Size small
  weightRatio = m_pEntity->getWeight() / pEntity->getWeight();
  if (weightRatio <= 1)
    m_vCurrentIdentifyInput(offset, 0) = 0;
  else if (weightRatio > MAX_SENSOR_VAL)
    m_vCurrentIdentifyInput(offset, 0) = MAX_SENSOR_VAL;
  else
    m_vCurrentIdentifyInput(offset, 0) = weightRatio;
  offset++;
  // Relative speed escape
  int relativeSpeed = m_pEntity->getRelativeSpeed(pEntity);
  if (relativeSpeed > 0)
    m_vCurrentIdentifyInput(offset,0) = relativeSpeed;
  else
    m_vCurrentIdentifyInput(offset,0) = 0;
  offset++;
  // Relative speed approach
  if (relativeSpeed < 0)
    m_vCurrentIdentifyInput(offset, 0) = -relativeSpeed;
  else
    m_vCurrentIdentifyInput(offset, 0) = 0;
  offset++;
  // Color:
  for (int color = COLOR_CARACTER_FIRST_TYPE; color < COLOR_CARACTER_NUMBER_TYPE; color++)
  {
    if (pEntity->getColorType() == color)
      m_vCurrentIdentifyInput(offset,0) = MAX_SENSOR_VAL ;
    else
      m_vCurrentIdentifyInput(offset,0) = 0;
    offset++;
  }
  // Form:
  for (int form = FORM_FIRST_TYPE; form < FORM_NUMBER_TYPE; form++)
  {
    if (pEntity->getForm() == form)
      m_vCurrentIdentifyInput(offset,0) = MAX_SENSOR_VAL;
    else
      m_vCurrentIdentifyInput(offset,0) = 0;
    offset++;
  }
  // Texture:
  for (int textu = TEXTURE_FIRST_TYPE; textu < TEXTURE_NUMBER_TYPE; textu++)
  {
    if (pEntity->getTexture() == textu)
      m_vCurrentIdentifyInput(offset,0) = MAX_SENSOR_VAL;
    else
      m_vCurrentIdentifyInput(offset,0) = 0;
    offset++;
  }
  // Physical attribute:
  for (int attribu = PHY_ATTRIBUTE_FIRST_TYPE; attribu < PHY_ATTRIBUTE_NUMBER_TYPE; attribu++)
  {
    if (pEntity->isPhyAttributePresent((PhyAttributeType_e)attribu))
      m_vCurrentIdentifyInput(offset,0) = MAX_SENSOR_VAL;
    else
      m_vCurrentIdentifyInput(offset,0) = 0;
    offset++;
  }

  if (useOdors)
  {
    for (i=0; i<NUMBER_ODORS; i++)
    {
      m_vCurrentIdentifyInput(offset,0) = m_pSensorSmell->GetSubCaptorStimulationLevel(i) / 2; // reduce weight because not directional
      offset++;
    }
    for (i=0; i<NUMBER_PHEROMONES; i++)
    {
      m_vCurrentIdentifyInput(offset,0) = m_pSensorPheromone->GetSubCaptorStimulationLevel(i) / 2; // reduce weight because not directional
      offset++;
    }
  }
  else
  {
    offset += NUMBER_ODORS + NUMBER_PHEROMONES;
  }

  // Set as non static animals when dead or not sleeping nor hiding
  m_vCurrentIdentifyInput(offset, 0) = 0;
  DWORD reactionUid = 0;
  if ((pEntity->getBrain() != NULL) && pEntity->isAlive())
  {
    reactionUid = pEntity->getBrain()->GetReactionByIndex(pEntity->getBrain()->GetCurrentReactionIndex())->GetUniqueId();
    if (((reactionUid & UID_BASE_MASK) != UID_BASE_REACT_SLEEP) && ((reactionUid & UID_BASE_MASK) != UID_BASE_REACT_HIDE))
    {
      m_vCurrentIdentifyInput(offset, 0) = MAX_SENSOR_VAL;
    }
  }
}

CMatrix* CBrain::GetIdentifyInputVect()
{
  return (&m_vCurrentIdentifyInput);
}

CMatrix* CBrain::GetIdentifyOutputVect()
{
  return (&m_vCurrentIdentifyOutput);
}

bool CBrain::CheckIfEntityIdentityNotMemorized(entitySignatureType entitySignature, IdentificationType_e identity)
{
  BrainMemorizedEntityIdentity_t* pEntityMem=NULL;

  for (size_t i=0; i<m_tBrainMemorizedEntityIdentities.size(); i++) 
  {
    if ((m_tBrainMemorizedEntityIdentities[i]->identity == identity) && (m_tBrainMemorizedEntityIdentities[i]->entitySignature == entitySignature))
      return false;
  }

  // limit size
  if (m_tBrainMemorizedEntityIdentities.size() == MAX_MEMORIZED_IDENTITIES)
  {
    if (m_tBrainMemorizedEntityIdentities[0] != NULL)
      delete m_tBrainMemorizedEntityIdentities[0]; 
    m_tBrainMemorizedEntityIdentities.erase(m_tBrainMemorizedEntityIdentities.begin());
  }

  // Memorize
  pEntityMem = new BrainMemorizedEntityIdentity_t;
  pEntityMem->entitySignature = entitySignature;
  pEntityMem->identity        = identity;
  m_tBrainMemorizedEntityIdentities.push_back(pEntityMem);

  return true;
}


bool CBrain::MemorizeIdentificationExperience(feedbackValType currentFeedback,double learningRate, CBasicEntity* pEntity, IdentificationType_e identity)
{
  // skip small memorizations
  if (((currentFeedback<1) && (currentFeedback>-1)) || (pEntity==NULL) || (CheckIfEntityIdentityNotMemorized(pEntity->getEntitySignature(), identity)==false) )
    return false;

  if (currentFeedback > MAX_FEEDBACK_VAL)
    currentFeedback = MAX_FEEDBACK_VAL;
  else if (currentFeedback < -MAX_FEEDBACK_VAL)
    currentFeedback = -MAX_FEEDBACK_VAL;

  // Prepare input
  UpdateIdentifyInputVector(pEntity, true);
  // Prepare outpu
  for (size_t i=0; i<IDENTIFICATION_NUMBER_TYPE; i++)
  { 
    m_vCurrentIdentificationChoice(i,0) = 0;
  }
  m_vCurrentIdentificationChoice(identity,0) = MAX_FEEDBACK_VAL;

  neuroneValType coef = currentFeedback*learningRate/MAX_FEEDBACK_VAL/MAX_SENSOR_VAL/10000; 

  CYBIOCORE_LOG_TIME(GetEntity()->getBiotop()->getBiotopTime());
  CYBIOCORE_LOG("BRAIN  - MemorizeIdentificationExperience : name %s currentFeedback=%.1f learningRate=%.1f identity=%s entity=%s coef=%1.4f\n", 
             GetEntity()->getLabel().c_str(), currentFeedback, learningRate, IdentificationTypeNameList[identity], pEntity->getLabel().c_str(), coef);

  return m_mIdentifyNeuronTable.MemorizeExperience(coef, &m_vCurrentIdentifyInput, &m_vCurrentIdentificationChoice);
}

bool CBrain::ReenforceFullIdentity(feedbackValType currentFeedback, IdentificationType_e identity)
{

  if (currentFeedback > MAX_FEEDBACK_VAL)
    currentFeedback = MAX_FEEDBACK_VAL;
  else if (currentFeedback < -MAX_FEEDBACK_VAL)
    currentFeedback = -MAX_FEEDBACK_VAL;

  // Prepare input
  size_t i;
  for (i=0; i<m_nInputIdentification; i++)
  { 
    m_vCurrentIdentifyInput(i,0) = MAX_SENSOR_VAL;
  }
  // Prepare output
  for (i=0; i<IDENTIFICATION_NUMBER_TYPE; i++)
  { 
    m_vCurrentIdentificationChoice(i,0) = 0;
  }
  m_vCurrentIdentificationChoice(identity,0) = MAX_FEEDBACK_VAL;

  neuroneValType coef = currentFeedback/MAX_FEEDBACK_VAL/MAX_SENSOR_VAL/100; 

  return m_mIdentifyNeuronTable.MemorizeExperience(coef, &m_vCurrentIdentifyInput, &m_vCurrentIdentificationChoice);
}


string CBrain::getIdentityStrName(IdentificationType_e type)
{
  return (IdentificationTypeNameList[type]);
}

//---------------------------------------------------------------------------
// Brain special behaviors
//---------------------------------------------------------------------------

void CBrain::SetBabyStayHome(bool stay)
{
  m_bBabyStayHome = stay;
}

bool CBrain::IsBabyStayHome()
{
  return m_bBabyStayHome;
}

bool CBrain::SetTerritorySize(size_t geoMapSize)
{
  if (m_pGeoMap == NULL)
  {
    m_TerritorySize = geoMapSize;
    return true;
  }
  else
  {
    return false;
  }
}

void CBrain::UpdateDefaultTerritorySize()
{
  if ((m_TerritorySize == TERRITORY_SIZE_DEFAULT) && (m_pEntity != NULL))
  {
    if (m_pEntity->getMaxWeight() < 5.0)
      m_TerritorySize = TERRITORY_SIZE_SMALL;
    else if ((m_pEntity->getMaxWeight() > 20.0) || (m_pEntity->getConsumeClass() == CONSUM_MEAT))
      m_TerritorySize = TERRITORY_SIZE_BIG;
  }
}

void CBrain::CreateGeoMapArroudCurrentPosition()
{
  constexpr size_t maxNumberMemorizedPurpose{ 6 };
  if ((m_pGeoMap == NULL) && (m_pEntity != NULL) && (m_pEntity->getBiotop() != NULL))
  {
    m_pGeoMap = new CGeoMapPurpose(this, m_pEntity->getGlobalGridCoord(), m_pEntity->getBiotop()->getGlobalGridDimension(), m_TerritorySize, maxNumberMemorizedPurpose);
  }
}

bool CBrain::SetHomePurposePositionInGeoMap()
{
  CPurpose* pPurpose = NULL;

  if (m_bBabyStayHome==false)
    return false;

  // Delete memory map if exist and current position is outside
  Point_t geoMapPos;
  if ((m_pGeoMap != NULL) && (m_pGeoMap->GridCoordToGeoMapCoord(m_pEntity->getGlobalGridCoord(), geoMapPos) == false))
  {
    delete m_pGeoMap;
    m_pGeoMap = NULL;
  }

  if (m_pGeoMap == NULL)
  {
    CreateGeoMapArroudCurrentPosition();
  }
  else
  {
    // Clear previous home positions
    pPurpose = GetPurposeByTriggerSensor(UID_BASE_SENS_HORMONE, PheromoneTypeToIndex(PHEROMONE_BABY));
    if (pPurpose != NULL)
    {
      m_pGeoMap->ClearPurposeSuccessOnFullMap(pPurpose->GetUniqueId());
    }
    pPurpose = GetPurposeByTriggerSensor(UID_BASE_SENS_HORMONE, PheromoneTypeToIndex(PHEROMONE_MATERNAL));
    if (pPurpose != NULL)
    {
      m_pGeoMap->ClearPurposeSuccessOnFullMap(pPurpose->GetUniqueId());
    }
  }

  // Memorize home position
  if (m_pGeoMap!=NULL)
  {
    // Check if baby
    if (m_pEntity->getPheromone() == PHEROMONE_BABY)
    {
      // the place where baby were born is the new home for baby
      pPurpose = GetPurposeByTriggerSensor(UID_BASE_SENS_HORMONE, PheromoneTypeToIndex(PHEROMONE_BABY));
      if (pPurpose != NULL)
      {
        m_pGeoMap->MemorizePurposeSuccessPos(pPurpose->GetUniqueId(), m_pEntity->getGlobalGridCoord(), 1000);
      }
    }
    // Check if mother
    else if (m_pEntity->getPheromone() == PHEROMONE_MATERNAL)
    {
      // the place where baby were born is the new home for mother
      pPurpose = GetPurposeByTriggerSensor(UID_BASE_SENS_HORMONE, PheromoneTypeToIndex(PHEROMONE_MATERNAL));
      if (pPurpose != NULL)
      {
        m_pGeoMap->MemorizePurposeSuccessPos(pPurpose->GetUniqueId(), m_pEntity->getGlobalGridCoord(), 800);
      }
    }
  }

  return true;
}