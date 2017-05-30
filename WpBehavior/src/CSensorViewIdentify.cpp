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
// FILE: CSensorViewIdentify.cpp
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

#include "CSensorViewIdentify.h"
#include "CBiotop.h"

//===========================================================================
// Global
//===========================================================================
// Buffer used to store focused entity. Always init before use so that it can be shared between all.
static EntityViewIdentifyWeight_t m_pEntityViewIdentifyTab[MAX_FOUND_ENTITIES];

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentify::CSensorViewIdentify
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[] : table of weight for each focus:
//                 Size,Proximity,Red,Green,Blue,Reflection,FORM_0,..,FORM_N 
//               int range: Distance scanned to find entities
//               int layer: layer scanned to find entities
//               ViewAngleType_e angle: angle around direction scan to find entities
//               int totalNbFocusObjects: max number of seen entities
//               bool distanceEvaluation: Allow/prevent Proximity process
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorViewIdentify::	CSensorViewIdentify(CBrainAnimal* pBrain, 
                          double* pWeightRate,
                          int range,
                          int layer,
                          ViewAngleType_e angle,
                          int totalNbFocusObjects,
                          bool distanceEvaluation)
:CSensor(totalNbFocusObjects * VIEW_IDENTIFY_SIZE_PER_FOCUS, pWeightRate, UID_BASE_SENS_VIEW_IDENTIFY + 10*layer + angle )
{
  m_pBrain = pBrain;
  m_nRange = range;
  m_Layer = layer;
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
  if (range == 1)
    m_Label = FormatString("View adj L%d", layer);
  else
    m_Label = FormatString("View L%d", layer);
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

CSensorViewIdentify::~CSensorViewIdentify()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentify::UpdateAndGetStimulationTable
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
int CSensorViewIdentify::UpdateAndGetStimulationTable(sensorValType*& pStimulationVal)
{
    int i;

  // Reinit m_pStimulationValues
  for (i=0; i<m_SubCaptorNumber; i++)
  {
    m_pStimulationValues[i] = 0;
  }

  CAnimal* pAnimal = m_pBrain->getAnimal();
 
  if (pAnimal->isSleeping())
  {
    // No stimulation when sleeping...
    pStimulationVal = m_pStimulationValues;
    return m_SubCaptorNumber;
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
    m_followedEntityWeight.index = -1;
    m_followedEntityWeight.signature = 0;
    m_followedEntityWeight.computedWeight = 0;
  }

  int offset = 0;
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
  
  pStimulationVal = m_pStimulationValues;
  return m_SubCaptorNumber;
}

//===========================================================================
// other functions
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentify::Scan45degSector
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
bool CSensorViewIdentify::Scan45degSector(int stimulationTabOffset,
                                  int maxNumFocusObject,
                                  int direction)
{
  FoundEntity_t* pFoundIds = NULL;
  CBasicEntity* pCurEntity = NULL;
  CAnimal* pAnimal = m_pBrain->getAnimal();
  CBiotop* pBiotop = pAnimal->getBiotop();
  UCHAR visionSectorBmp = 0x01 << direction;
  int offset = 0;
  int count = 0;
  int i,j;
  int identityIdx;
  double maxComputedWeight;
  int maxWeightViewTabIndex;
  CMatrix* pFoundIdentitiesMatrix;
  int relativeAngle;
  double distanceWeight;
  double curWeight;
  double viewChance;

  // Find entities according to angle, distance and layer:
  int nbIds = pBiotop->findEntities(pFoundIds, pAnimal->getGridCoord(), visionSectorBmp, m_nRange, m_Layer, true);

  // get focused entity
  BrainFocusedEntityView_t* pBrainFocused = m_pBrain->getpBrainFocusedEntityInfo();

  for (i=0; i<nbIds; i++)
  {
    pCurEntity = pFoundIds[i].pEntity;
    distanceWeight = 0;
    curWeight = 0;

    if ((pCurEntity != pBrainFocused->pEntity) && (m_nRange>1)) // keep seeing focused and just in front entities
    {
      // view chance depends of camouflage and sunlight. TBD can include view accuity of entity and distance
      viewChance = 100.0 - pCurEntity->getCamouflage(); 
      if (pBiotop->getSunlight() < 10)
        viewChance = viewChance/2; 
    }
    else
    {
      viewChance = 100;
    }

    if ((pCurEntity!=NULL)&&(testChance(viewChance)))
    {
      m_pEntityViewIdentifyTab[i].index = i;
      m_pEntityViewIdentifyTab[i].pEntity = pCurEntity;
      m_pEntityViewIdentifyTab[i].signature = pCurEntity->getEntitySignature();

      offset = 0;
      pFoundIdentitiesMatrix = m_pBrain->ComputeAndGetIdentification(pCurEntity);

      for (identityIdx=0; identityIdx<IDENTIFICATION_STATIC_NUMBER_TYPE; identityIdx++)
      {
        curWeight = (*pFoundIdentitiesMatrix)(identityIdx,0);
        if ((curWeight!=0) && m_bDistanceEval) // iclude distance in weight
          m_pEntityViewIdentifyTab[i].weightTab[offset] = (curWeight + curWeight/pFoundIds[i].distance);
        else
          m_pEntityViewIdentifyTab[i].weightTab[offset] = curWeight; 
        offset++;
      }
      for (identityIdx=IDENTIFICATION_STATIC_NUMBER_TYPE; identityIdx<IDENTIFICATION_NUMBER_TYPE; identityIdx++)
      {
        curWeight = (*pFoundIdentitiesMatrix)(identityIdx,0);
        if (curWeight!=0)
        {
          // 1 presence and distance
          if (m_bDistanceEval) // iclude distance in weight
            m_pEntityViewIdentifyTab[i].weightTab[offset] = (curWeight + curWeight/pFoundIds[i].distance); 
          else
            m_pEntityViewIdentifyTab[i].weightTab[offset] = curWeight;
          offset++;
          // 2 Relative speed escape
          int relativeSpeed = pAnimal->getRelativeSpeed(pCurEntity);
          if (relativeSpeed > 0)
            m_pEntityViewIdentifyTab[i].weightTab[offset] = relativeSpeed;
          else
            m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
          // 3 Relative speed approach
          if (relativeSpeed < 0)
            m_pEntityViewIdentifyTab[i].weightTab[offset] = -relativeSpeed;
          else
            m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
          // 4 direction left
          relativeAngle = (pAnimal->getDirection() + 8 - pCurEntity->getDirection())%8 - 4;
          if (relativeAngle>0) 
            m_pEntityViewIdentifyTab[i].weightTab[offset] = curWeight;
          else
            m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
          // 5 direction right
          if ((relativeAngle < 0) && (relativeAngle != -4))
            m_pEntityViewIdentifyTab[i].weightTab[offset] = curWeight;
          else
            m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
        }
        else
        {
          m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
          m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;    
          m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;    
          m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
          m_pEntityViewIdentifyTab[i].weightTab[offset] = 0;
          offset++;
        }
      }

      // Compute weight and computedWeight sum
      m_pEntityViewIdentifyTab[i].computedWeight = 0;
      for (j=0; j<VIEW_IDENTIFY_SIZE_PER_FOCUS; j++)
      {
        // Use weight
        m_pEntityViewIdentifyTab[i].weightTab[j] = m_pEntityViewIdentifyTab[i].weightTab[j] * m_pSubCaptorWeightRate[stimulationTabOffset+j] / 100.0;
        // Don't go over Max!
        if (m_pEntityViewIdentifyTab[i].weightTab[j]  > MAX_SENSOR_VAL)
          m_pEntityViewIdentifyTab[i].weightTab[j] = MAX_SENSOR_VAL;

        m_pEntityViewIdentifyTab[i].computedWeight += m_pEntityViewIdentifyTab[i].weightTab[j] * m_pBonusRate[stimulationTabOffset+j];
      }

      // Fill followed entity
      if (m_pFollowedEntity==pCurEntity)
      {
        m_followedEntityWeight.pEntity = pCurEntity;
        m_followedEntityWeight.index = i;
        m_followedEntityWeight.signature = m_pEntityViewIdentifyTab[i].signature;
        m_followedEntityWeight.computedWeight = m_pEntityViewIdentifyTab[i].computedWeight;
      }
    }
    else
    {
      m_pEntityViewIdentifyTab[i].index = -1;
      m_pEntityViewIdentifyTab[i].computedWeight = 0;
      m_pEntityViewIdentifyTab[i].signature = 0;
      m_pEntityViewIdentifyTab[i].pEntity = NULL;
    }
  }

  // Now choose the most significant entities
  offset = stimulationTabOffset;

  for (j=0; j<maxNumFocusObject; j++)
  {
    // 1 Find max weight
    maxComputedWeight = 0;
    maxWeightViewTabIndex = -1;
    for (i=0; i<nbIds; i++)
    {
      // Give 10% bonus to previousely selected entity
      if (m_pEntityViewIdentifyTab[i].pEntity == pBrainFocused->pEntity)
        m_pEntityViewIdentifyTab[i].computedWeight += m_pEntityViewIdentifyTab[i].computedWeight/10;

      if (m_pEntityViewIdentifyTab[i].computedWeight>maxComputedWeight)
      {
        maxComputedWeight = m_pEntityViewIdentifyTab[i].computedWeight;
        maxWeightViewTabIndex = i;
      }
    }

    // 2 Exit if no more entities
    if (maxComputedWeight == 0)
      break;

    // 3 Update global brain focused entity if weight over previous
    if (maxComputedWeight > pBrainFocused->computedWeight)
    {
      pBrainFocused->pEntity         = m_pEntityViewIdentifyTab[maxWeightViewTabIndex].pEntity;
      if (pAnimal->getHeadDirection() == direction)
        pBrainFocused->computedWeight  = maxComputedWeight + 2; // Add 2 to give advantage to front entity
      else
        pBrainFocused->computedWeight  = maxComputedWeight + 1; // Add 1 to give advantage to first found entity
      pBrainFocused->captorUid       = GetUniqueId();
      pBrainFocused->subcaptorIndex  = offset;
      pBrainFocused->subcaptorsSize  = VIEW_IDENTIFY_SIZE_PER_FOCUS;
    }

    // 4 Fill m_pStimulationValues
    for (i=0; i<VIEW_IDENTIFY_SIZE_PER_FOCUS; i++)
    {
      m_pStimulationValues[offset] = m_pEntityViewIdentifyTab[maxWeightViewTabIndex].weightTab[i];
      if (m_pStimulationValues[offset] > MAX_SENSOR_VAL)
      {
        CYBIOCORE_LOG("SENSOR - warning Scan45degSector :m_pStimulationValues offset%d too big: %f\n", offset, m_pStimulationValues[offset]);
        m_pStimulationValues[offset] = MAX_SENSOR_VAL;
      }
      else if (m_pStimulationValues[offset] < -MAX_SENSOR_VAL)
      {
        CYBIOCORE_LOG("SENSOR - warning Scan45degSector :m_pStimulationValues offset%d too big: %f\n", offset, m_pStimulationValues[offset]);
        m_pStimulationValues[offset] = -MAX_SENSOR_VAL;
      }
      offset++;
    }

    // 5 Remove identical entities
    for (i=0; i<nbIds; i++)
    {
      if (m_pEntityViewIdentifyTab[i].signature == m_pEntityViewIdentifyTab[maxWeightViewTabIndex].signature)
        m_pEntityViewIdentifyTab[i].computedWeight = 0;
    }
  }

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CSensorViewIdentify::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: string: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorViewIdentify::GetSubCaptorLabel(int index)
{
  if ( (index<0) || (index>GetSubCaptorNumber()) )
    return ("bad index");
  else
  {
    int offset = index / VIEW_IDENTIFY_SIZE_PER_FOCUS;
    int pos = index % VIEW_IDENTIFY_SIZE_PER_FOCUS;

    string directionStr; // according to offset
    directionStr = FormatString("N%d ", offset);
    int index = 0;
    int count=m_nFocusObjectsSect1;

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
      int subindex  = IDENTIFICATION_STATIC_NUMBER_TYPE + (pos-IDENTIFICATION_STATIC_NUMBER_TYPE) / VIEW_IDENTIFY_SIZE_PER_DYNAMIC;
      int suboffset = (pos-IDENTIFICATION_STATIC_NUMBER_TYPE) % VIEW_IDENTIFY_SIZE_PER_DYNAMIC;
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


int CSensorViewIdentify::GetSubCaptorIndexForDirection(int relativeDirection, int entityIndex)
{
  int index = -1;

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


int CSensorViewIdentify::GetSubCaptorSubIndexForProximity(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return -1;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return (int)identity;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC);
}

int CSensorViewIdentify::GetSubCaptorSubIndexForEscapeSpeed(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return -1;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return -1;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 1;
}

int CSensorViewIdentify::GetSubCaptorSubIndexForApproachSpeed(IdentificationType_e identity)
{
  if (identity >= IDENTIFICATION_NUMBER_TYPE)
    return -1;
  else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
    return -1;
  else
    return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity - IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 1;
}

int CSensorViewIdentify::GetSubCaptorSubIndexForDirLeft(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return -1;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return -1;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 3;
}


int CSensorViewIdentify::GetSubCaptorSubIndexForDirRight(IdentificationType_e identity)
{
    if (identity>=IDENTIFICATION_NUMBER_TYPE)
      return -1;
    else if (identity<IDENTIFICATION_STATIC_NUMBER_TYPE)
      return -1;
    else
      return (IDENTIFICATION_STATIC_NUMBER_TYPE + (identity-IDENTIFICATION_STATIC_NUMBER_TYPE) * VIEW_IDENTIFY_SIZE_PER_DYNAMIC) + 4;
}


int CSensorViewIdentify::GetRange()
{
  return m_nRange;
}

int CSensorViewIdentify::GetLayer()
{
  return m_Layer;
}

double CSensorViewIdentify::GetViewedEntityWeight(CBasicEntity* pEntity)
{
  sensorValType* pStimulationVal = NULL;
  m_pFollowedEntity = pEntity;
  UpdateAndGetStimulationTable(pStimulationVal);
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
bool CSensorViewIdentify::IsSexSpecific(int captorIndex)
{
  int index = captorIndex % VIEW_IDENTIFY_SIZE_PER_FOCUS;
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
