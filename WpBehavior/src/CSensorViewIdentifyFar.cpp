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
// FILE: CSensorViewIdentifyFar.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for view
//         It is a set of Y = totalNbFocusObjects * VIEW_IDENTIFY_SIZE_PER_FOCUS 
//         sub-sensors dedicated, for each FocusObjects to:
//         Size,Proximity,Red,Green,Blue,Reflection,FORM_0,..,FORM_N      
//         A Y sized weight table must be used to set visible aspect sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  10/10/2005 : Creation
//
//===========================================================================

#include "CSensorViewIdentifyFar.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentifyFar::CSensorViewIdentifyFar
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[] : table of weight for each focus:
//                 Size,Proximity,Red,Green,Blue,Reflection,FORM_0,..,FORM_N 
//               int range: Distance scanned to find entities
//               int layer: layer scanned to find entities
//               ViewIdentifyFarAngleType_e angle: angle around direction scan to find entities
//               int totalNbFocusObjects: max number of seen entities
//               bool distanceEvaluation: Allow/prevent Proximity process
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorViewIdentifyFar::	CSensorViewIdentifyFar(CBrainAnimal* pBrain, 
                          std::vector<double>& tWeightRate,
                          int rangeMin,
                          int rangeMax,
                          ViewAngleType_e angle,
                          int totalNbFocusObjects,
                          bool distanceEvaluation)
:CSensor(totalNbFocusObjects * VIEW_IDENTIFY_SIZE_PER_FOCUS, tWeightRate, UID_BASE_SENS_VIEW_IDENTIFY_FAR + angle )
{
  m_pBrain = pBrain;
  m_nRangeMin = rangeMin;
  m_nRangeMax = rangeMax;
  m_Angle = angle;
  m_bDistanceEval = distanceEvaluation;
  m_nFocusObjectsSect1 = 0; 
  m_nFocusObjectsSect2 = 0; 
  m_nFocusObjectsSect3 = 0; 
  m_nFocusObjectsSect4 = 0; 
  m_nFocusObjectsSect5 = 0;
  m_nFocusObjectsSect6 = 0;
  m_nFocusObjectsSect7 = 0;
  m_nFocusObjectsSect8 = 0;
  m_Label = "View far";
  m_pFollowedEntity = NULL;

  // Add adjacent sectors for turn head management
  switch (m_Angle)
  {
  case VIEW_ANGLE_45:
    m_nFocusObjectsSect2 = totalNbFocusObjects/3;
    m_nFocusObjectsSect3 = totalNbFocusObjects/3;
    m_nFocusObjectsSect1 = totalNbFocusObjects - 2*m_nFocusObjectsSect2; // maximize sect1
    break;

  case VIEW_ANGLE_135:
    m_nFocusObjectsSect2 = totalNbFocusObjects/3;
    m_nFocusObjectsSect3 = totalNbFocusObjects/3;
    m_nFocusObjectsSect4 = totalNbFocusObjects/5; // minimize sect
    m_nFocusObjectsSect5 = totalNbFocusObjects/5; // minimize sect
    m_nFocusObjectsSect1 = totalNbFocusObjects - 2*m_nFocusObjectsSect2 - 2*m_nFocusObjectsSect4; // maximize sect1
    break;

  case VIEW_ANGLE_225:
    m_nFocusObjectsSect2 = totalNbFocusObjects/5;
    m_nFocusObjectsSect3 = totalNbFocusObjects/5;
    m_nFocusObjectsSect4 = totalNbFocusObjects/5;
    m_nFocusObjectsSect5 = totalNbFocusObjects/5;
    m_nFocusObjectsSect6 = totalNbFocusObjects/7; // minimize sect
    m_nFocusObjectsSect7 = totalNbFocusObjects/7; // minimize sect
    m_nFocusObjectsSect1 = totalNbFocusObjects - 4*m_nFocusObjectsSect2 - 2*m_nFocusObjectsSect6; // maximize sect1
    break;

  case VIEW_ANGLE_315:
  case VIEW_ANGLE_360:
    m_nFocusObjectsSect2 = totalNbFocusObjects/7;
    m_nFocusObjectsSect3 = totalNbFocusObjects/7;
    m_nFocusObjectsSect4 = totalNbFocusObjects/7;
    m_nFocusObjectsSect5 = totalNbFocusObjects/7;
    m_nFocusObjectsSect6 = totalNbFocusObjects/7;
    m_nFocusObjectsSect7 = totalNbFocusObjects/7;
    m_nFocusObjectsSect8 = totalNbFocusObjects/8; // minimize sect
    m_nFocusObjectsSect1 = totalNbFocusObjects - 6*m_nFocusObjectsSect2 - m_nFocusObjectsSect8; // maximize sect1
    break;

  default: 
    break;
  }
}

CSensorViewIdentifyFar::~CSensorViewIdentifyFar()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentifyFar::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to viewed entities
//               The Weight of captors is taken into account here 
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorViewIdentifyFar::UpdateAndGetStimulationTable()
{
  int i;
  CAnimal* pAnimal = m_pBrain->getAnimal();
  pAnimal->getBiotop()->logCpuMarkerStart(BIOTOP_CPUMARKER_CUSTOM1);

  // Reinit m_tStimulationValues
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);
 
  if (pAnimal->isSleeping())
  {
    // No stimulation when sleeping...
    return m_tStimulationValues;
  }

  bool tScanDirections[8] = {false,false,false,false,false,false,false,false};
  int viewDirection = pAnimal->getHeadDirection();
  for (i = 0; i<(m_Angle+1); i++)
  {
    tScanDirections[(viewDirection+i)%8]   = true;
    tScanDirections[(viewDirection+8-i)%8] = true;
  }

  // Prepare followed entity
  if (m_pFollowedEntity!=NULL)
  {
    m_followedEntityWeight.pEntity = NULL;
    m_followedEntityWeight.index = invalidIndex;
    m_followedEntityWeight.signature = 0;
    m_followedEntityWeight.computedWeight = 0;
  }

  size_t offset = 0;
  int direction = pAnimal->getDirection(); 
  if (tScanDirections[direction] && m_nFocusObjectsSect1)
    Scan45degSector(offset, m_nFocusObjectsSect1,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect1;
  direction = (pAnimal->getDirection()+1) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect2)
    Scan45degSector(offset, m_nFocusObjectsSect2,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect2;
  direction = (pAnimal->getDirection()+7) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect3)
    Scan45degSector(offset, m_nFocusObjectsSect3,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect3;
  direction = (pAnimal->getDirection()+2) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect4)
    Scan45degSector(offset, m_nFocusObjectsSect4,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect4;
  direction = (pAnimal->getDirection()+6) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect5)
    Scan45degSector(offset, m_nFocusObjectsSect5,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect5;
  direction = (pAnimal->getDirection()+3) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect6)
    Scan45degSector(offset, m_nFocusObjectsSect6,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect6;
  direction = (pAnimal->getDirection()+5) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect7)
    Scan45degSector(offset, m_nFocusObjectsSect7,direction);

  offset += VIEW_IDENTIFY_SIZE_PER_FOCUS * m_nFocusObjectsSect7;
  direction = (pAnimal->getDirection()+4) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect8)
    Scan45degSector(offset, m_nFocusObjectsSect8,direction);
  
  pAnimal->getBiotop()->logCpuMarkerEnd(BIOTOP_CPUMARKER_CUSTOM1);
  return m_tStimulationValues;
}

//===========================================================================
// other functions
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentifyFar::Scan45degSector
//  
// DESCRIPTION:  Stimulate a table of ensor according to viewed in 45o direction 
// 
// ARGUMENTS:    sensorValType* pStimulationVal : Pointer on result table
//               int maxNumFocusObject : number of entities to be set
//               int direction : {0..7} , int layer : {0..max layer}
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
bool CSensorViewIdentifyFar::Scan45degSector(size_t stimulationTabOffset,
                                     size_t maxNumFocusObject,
                                     int direction)
{
  CBasicEntity* pCurEntity = NULL;
  CAnimal* pAnimal = m_pBrain->getAnimal();
  CBiotop* pBiotop = pAnimal->getBiotop();
  UCHAR visionSectorBmp = 0x01 << direction;
  size_t offset = 0;
  size_t i,j;
  double maxComputedWeight;
  size_t maxWeightViewTabIndex;
  int identityIdx;
  CMatrix* pFoundIdentitiesMatrix = NULL;
  int relativeSpeed;
  int relativeAngle;
  double curWeight;
  double viewChance;
  entitySignatureType previousEntitySignature = 0;

  // Find entities according to angle, distance and layer:
  const BiotopFoundIds_t& biotopFoundIds = pBiotop->findFarEntities(pAnimal->getGridCoord(), visionSectorBmp, m_nRangeMin, m_nRangeMax, true);
  const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;

  for (i = 0; i < biotopFoundIds.nbFoundIds; i++)
  {
    pCurEntity = tFoundIds[i].pEntity;
    curWeight = 0;

    if (pCurEntity == NULL)
    {
      viewChance = 0;
    }
    else if (pCurEntity->getId() != m_pBrain->getBrainFocusedEntityId())
    {
      // view chance depends on vigilance, target camouflage and sunlight. TBD can include view accuity of entity and distance
      viewChance = pAnimal->getVigilance() - 2*pCurEntity->getCamouflage();
      if (pBiotop->getSunlight() < 20)
        viewChance = viewChance/2; 
    }
    else
    {
      viewChance = 100;
    }
    // Reduce again chance for very low luminosity
    if (pBiotop->getSunlight() < 10)
        viewChance = viewChance/2;

    if ((pCurEntity!=NULL)&&(testChance(viewChance)))
    {
      m_pEntityViewIdentifyFarTab[i].index = i;
      m_pEntityViewIdentifyFarTab[i].pEntity = pCurEntity;
      m_pEntityViewIdentifyFarTab[i].signature = pCurEntity->getEntitySignature();
      offset = 0;

      if (pCurEntity->getEntitySignature() != previousEntitySignature) // CPU optim
      {
        pFoundIdentitiesMatrix = m_pBrain->ComputeAndGetIdentification(pCurEntity, false);
        previousEntitySignature = pCurEntity->getEntitySignature();
      }
      else if (i > maxNumFocusObject)  // CPU optim
      {
        // If same entity already taken into account and too much entities in list, just skip it
        m_pEntityViewIdentifyFarTab[i].index = invalidIndex;
        m_pEntityViewIdentifyFarTab[i].computedWeight = 0;
        m_pEntityViewIdentifyFarTab[i].signature = 0;
        m_pEntityViewIdentifyFarTab[i].pEntity = NULL;
        continue;
      }

      relativeSpeed = pAnimal->getRelativeSpeed(pCurEntity);
      relativeAngle = (pAnimal->getDirection() + 8 - pCurEntity->getDirection()) % 8 - 4;

      for (identityIdx=0; identityIdx<IDENTIFICATION_STATIC_NUMBER_TYPE; identityIdx++)
      {
        curWeight = (*pFoundIdentitiesMatrix)(identityIdx,0);
        if ((curWeight!=0) && m_bDistanceEval) // iclude distance in weight
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = (curWeight + curWeight / tFoundIds[i].distance);
        else
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = curWeight; 
        offset++;
      }
      for (identityIdx=IDENTIFICATION_STATIC_NUMBER_TYPE; identityIdx<IDENTIFICATION_NUMBER_TYPE; identityIdx++)
      {
        curWeight = (*pFoundIdentitiesMatrix)(identityIdx,0);
        if (curWeight!=0)
        {
          // 1 presence and distance
          if (m_bDistanceEval) // iclude distance in weight
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = (curWeight + curWeight / tFoundIds[i].distance);
          else
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = curWeight;
          offset++;
          // 2 Relative speed escape
          if (relativeSpeed > 0)
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = relativeSpeed;
          else
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
          // 3 Relative speed approach
          if (relativeSpeed < 0)
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = -relativeSpeed;
          else
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
          // 4 direction left
          if (relativeAngle>0) 
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = curWeight;
          else
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
          // 5 direction right
          if ((relativeAngle < 0) && (relativeAngle != -4))
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = curWeight;
          else
            m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
        }
        else
        {
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;    
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;    
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
          m_pEntityViewIdentifyFarTab[i].weightTab[offset] = 0;
          offset++;
        }
      }

      // Compute weight and computedWeight sum
      m_pEntityViewIdentifyFarTab[i].computedWeight = 0;
      for (j=0; j<VIEW_IDENTIFY_SIZE_PER_FOCUS; j++)
      {
        if (m_pEntityViewIdentifyFarTab[i].weightTab[j] != 0)
        {
          // Use weight
          m_pEntityViewIdentifyFarTab[i].weightTab[j] = m_pEntityViewIdentifyFarTab[i].weightTab[j] * m_tSubCaptorWeightRate[stimulationTabOffset + j] / 100.0;
          // Don't go over Max!
          if (m_pEntityViewIdentifyFarTab[i].weightTab[j] > MAX_SENSOR_VAL)
            m_pEntityViewIdentifyFarTab[i].weightTab[j] = MAX_SENSOR_VAL;
        }
        m_pEntityViewIdentifyFarTab[i].computedWeight += m_pEntityViewIdentifyFarTab[i].weightTab[j] * m_tBonusRate[stimulationTabOffset+j];
      }

      // Fill followed entity
      if (m_pFollowedEntity==pCurEntity)
      {
        m_followedEntityWeight.pEntity = pCurEntity;
        m_followedEntityWeight.index = i;
        m_followedEntityWeight.signature = m_pEntityViewIdentifyFarTab[i].signature;
        m_followedEntityWeight.computedWeight = m_pEntityViewIdentifyFarTab[i].computedWeight;
      }
    }
    else
    {
      m_pEntityViewIdentifyFarTab[i].index = invalidIndex;
      m_pEntityViewIdentifyFarTab[i].computedWeight = 0;
      m_pEntityViewIdentifyFarTab[i].signature = 0;
      m_pEntityViewIdentifyFarTab[i].pEntity = NULL;
    }
  }

  // Now choose the most significant entities
  offset = stimulationTabOffset;

  for (j=0; j<maxNumFocusObject; j++)
  {
    // 1 Find max weight
    maxComputedWeight = 0;
    maxWeightViewTabIndex = invalidIndex;
    for (i = 0; i < biotopFoundIds.nbFoundIds; i++)
    {
      if (m_pEntityViewIdentifyFarTab[i].pEntity != NULL)
      {
        // Give 10% bonus to previousely selected entity
        if (m_pEntityViewIdentifyFarTab[i].pEntity->getId() == m_pBrain->getBrainFocusedEntityId())
          m_pEntityViewIdentifyFarTab[i].computedWeight += m_pEntityViewIdentifyFarTab[i].computedWeight / 10;

        if (m_pEntityViewIdentifyFarTab[i].computedWeight > maxComputedWeight)
        {
          maxComputedWeight = m_pEntityViewIdentifyFarTab[i].computedWeight;
          maxWeightViewTabIndex = i;
        }
      }
    }

    // 2 Exit if no more entities
    if (maxComputedWeight == 0)
      break;

    // 3 Update global brain focused entity if weight over previous
    double weightBonus = (pAnimal->getHeadDirection() == direction) ? 1 : 0;
    m_pBrain->proposeNewFocusedEntityCandidate(m_pEntityViewIdentifyFarTab[maxWeightViewTabIndex].pEntity, maxComputedWeight, weightBonus,
                                               GetUniqueId(), offset, VIEW_IDENTIFY_SIZE_PER_FOCUS);

    // 4 Fill m_tStimulationValues
    for (i=0; i<VIEW_IDENTIFY_SIZE_PER_FOCUS; i++)
    {
      m_tStimulationValues[offset] = m_pEntityViewIdentifyFarTab[maxWeightViewTabIndex].weightTab[i];
      offset++;
    }

    // 5 Remove identical entities
    for (i = 0; i < biotopFoundIds.nbFoundIds; i++)
    {
      if (m_pEntityViewIdentifyFarTab[i].signature == m_pEntityViewIdentifyFarTab[maxWeightViewTabIndex].signature)
        m_pEntityViewIdentifyFarTab[i].computedWeight = 0;
    }
  }

  return (true);
}


//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentifyFar::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorViewIdentifyFar::GetSubCaptorLabel(size_t index)
{
  if (index>GetSubCaptorNumber())
    return ("bad index");
  else
  {
    size_t offset = index / VIEW_IDENTIFY_SIZE_PER_FOCUS;
    size_t pos = index % VIEW_IDENTIFY_SIZE_PER_FOCUS;

    string directionStr; // according to offset
    directionStr = FormatString("N%d ", offset);
    size_t index = 0;
    size_t count=m_nFocusObjectsSect1;

    do 
    {
      if ( offset < count )
      {
        index = 0;
        break;
      }
      count += m_nFocusObjectsSect2;
      if ( offset < count )
      {
        index = 1;
        break;
      }
      count += m_nFocusObjectsSect3;
      if ( offset < count )
      {
        index = 2;
        break;
      }
      count += m_nFocusObjectsSect4;
      if ( offset < count )
      {
        index = 3;
        break;
      }
      count += m_nFocusObjectsSect5;
      if ( offset < count )
      {
        index = 4;
        break;
      }
      count += m_nFocusObjectsSect6;
      if ( offset < count )
      {
        index = 5;
        break;
      }
      count += m_nFocusObjectsSect7;
      if ( offset < count )
      {
        index = 6;
        break;
      }
      count += m_nFocusObjectsSect8;
      if ( offset < count )
      {
        index = 7;
        break;
      }
    } while (false);

    directionStr += CBasicEntity::getRelativePosStrName(index);

    string captorStr = ""; // according to pos

    if (pos>VIEW_IDENTIFY_SIZE_PER_FOCUS)
      captorStr = "captor error";
    else if (pos<IDENTIFICATION_STATIC_NUMBER_TYPE)
      captorStr = CBrain::getIdentityStrName((IdentificationType_e)pos)  + " proxi";
    else
    {
      size_t subindex  = IDENTIFICATION_STATIC_NUMBER_TYPE + (pos-IDENTIFICATION_STATIC_NUMBER_TYPE) / VIEW_IDENTIFY_SIZE_PER_DYNAMIC;
      size_t suboffset = (pos-IDENTIFICATION_STATIC_NUMBER_TYPE) % VIEW_IDENTIFY_SIZE_PER_DYNAMIC;
      switch (suboffset)
      {
      case 0:
        captorStr = CBrain::getIdentityStrName((IdentificationType_e)subindex) + " proxi"; break;
      case 1:
        captorStr = CBrain::getIdentityStrName((IdentificationType_e)subindex) + " relative speed approach"; break;
      case 2:
        captorStr = CBrain::getIdentityStrName((IdentificationType_e)subindex) + " relative speed escape"; break;
      case 3:
        captorStr = CBrain::getIdentityStrName((IdentificationType_e)subindex) + " dir left"; break;
      case 4:
        captorStr = CBrain::getIdentityStrName((IdentificationType_e)subindex) + " dir right"; break;
      default:
        captorStr = "error";
      }

    }

    return (directionStr + captorStr);
  }
}


size_t CSensorViewIdentifyFar::GetSubCaptorIndexForDirection(int relativeDirection, size_t entityIndex)
{
  size_t index = invalidIndex;

  switch (relativeDirection)
  {
  case 0:
    if (entityIndex<m_nFocusObjectsSect1)
      index = entityIndex * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 1:
    if (entityIndex<m_nFocusObjectsSect2)
      index = (m_nFocusObjectsSect1 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 2:
    if (entityIndex<m_nFocusObjectsSect3)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 3:
    if (entityIndex<m_nFocusObjectsSect4)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 4:
    if (entityIndex<m_nFocusObjectsSect5)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 5:
    if (entityIndex<m_nFocusObjectsSect6)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + m_nFocusObjectsSect5 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 6:
    if (entityIndex<m_nFocusObjectsSect7)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + m_nFocusObjectsSect5 + m_nFocusObjectsSect6 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  case 7:
    if (entityIndex<m_nFocusObjectsSect8)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + m_nFocusObjectsSect5 + m_nFocusObjectsSect6 + m_nFocusObjectsSect7 + entityIndex) * VIEW_IDENTIFY_SIZE_PER_FOCUS;
    break;

  default:
    break;
  }

  return index;
}


size_t CSensorViewIdentifyFar::GetSubCaptorSubIndexForProximity(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return invalidIndex;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return (size_t)identity;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC);
}

size_t CSensorViewIdentifyFar::GetSubCaptorSubIndexForEscapeSpeed(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return invalidIndex;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return invalidIndex;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 1;
}

size_t CSensorViewIdentifyFar::GetSubCaptorSubIndexForApproachSpeed(IdentificationType_e identity)
{
  if (identity >= IDENTIFICATION_NUMBER_TYPE)
    return invalidIndex;
  else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
    return invalidIndex;
  else
    return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity - IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 2;
}

size_t CSensorViewIdentifyFar::GetSubCaptorSubIndexForDirLeft(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return invalidIndex;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return invalidIndex;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 3;
}


size_t CSensorViewIdentifyFar::GetSubCaptorSubIndexForDirRight(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return invalidIndex;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return invalidIndex;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 4;
}

double CSensorViewIdentifyFar::GetViewedEntityWeight(CBasicEntity* pEntity)
{
  m_pFollowedEntity = pEntity;
  UpdateAndGetStimulationTable();
  m_pFollowedEntity = NULL; // Don't follow anymore
  return (m_followedEntityWeight.computedWeight);
}

//---------------------------------------------------------------------------
// METHOD:       CSensorPheromone::IsSexSpecific
//  
// DESCRIPTION:  Return true if the sub-captor is used a different way 
//               according to sex
//
// ARGUMENTS:    int captorIndex : index of the sub captor (def = 0)
//   
// RETURN VALUE: true: sex specific, false: no sexual differenciation
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CSensorViewIdentifyFar::IsSexSpecific(size_t captorIndex)
{
  size_t index = captorIndex % VIEW_IDENTIFY_SIZE_PER_FOCUS;
  if ( (index == GetSubCaptorSubIndexForProximity(IDENTIFICATION_SEX_PARTNER))
    || (index == GetSubCaptorSubIndexForEscapeSpeed(IDENTIFICATION_SEX_PARTNER))
    || (index == GetSubCaptorSubIndexForApproachSpeed(IDENTIFICATION_SEX_PARTNER))
    || (index == GetSubCaptorSubIndexForDirLeft(IDENTIFICATION_SEX_PARTNER))
    || (index == GetSubCaptorSubIndexForDirRight(IDENTIFICATION_SEX_PARTNER))
    || (index == GetSubCaptorSubIndexForProximity(IDENTIFICATION_BABY))
    || (index == GetSubCaptorSubIndexForEscapeSpeed(IDENTIFICATION_BABY))
    || (index == GetSubCaptorSubIndexForApproachSpeed(IDENTIFICATION_BABY))
    || (index == GetSubCaptorSubIndexForDirLeft(IDENTIFICATION_BABY))
    || (index == GetSubCaptorSubIndexForDirRight(IDENTIFICATION_BABY)) )
    return true;
  else
    return false;
}
