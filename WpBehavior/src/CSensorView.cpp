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
// FILE: CSensorView.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for view
//         It is a set of Y = totalNbFocusObjects * VIEW_SIZE_PER_FOCUS 
//         sub-sensors dedicated, for each FocusObjects to:
//         Size,Proximity,Red,Green,Blue,Reflection,FORM_0,..,FORM_N      
//         A Y sized weight table must be used to set visible aspect sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  10/10/2005 : Creation
//
//===========================================================================

#include "CSensorView.h"
#include "CBiotop.h"

//===========================================================================
// Global
//===========================================================================
// Buffer used to store focused entity. Always init before use so that it can be shared between all.
static EntityViewWeight_t m_pEntityViewTab[MAX_FOUND_ENTITIES];

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorView::CSensorView
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
CSensorView::	CSensorView(CBrainAnimal* pBrain, 
                          std::vector<double>& tWeightRate,
                          int range,
                          size_t layer,
                          ViewAngleType_e angle,
                          int totalNbFocusObjects,
                          bool distanceEvaluation)
:CSensor(totalNbFocusObjects * VIEW_SIZE_PER_FOCUS, tWeightRate, UID_BASE_SENS_VIEW + 10*(DWORD)layer + angle )
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
    m_Label = FormatString("View old adj L%d", layer);
  else
    m_Label = FormatString("View old L%d", layer);

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

CSensorView::~CSensorView()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorView::UpdateAndGetStimulationTable
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
const std::vector<sensorValType>& CSensorView::UpdateAndGetStimulationTable()
{
  int i;
  // Reinit m_tStimulationValues
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();
 
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

  size_t offset = 0;
  int direction = pAnimal->getDirection(); 
  if (tScanDirections[direction] && m_nFocusObjectsSect1)
    Scan45degSector(offset, m_nFocusObjectsSect1,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect1;
  direction = (pAnimal->getDirection()+1) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect2)
    Scan45degSector(offset, m_nFocusObjectsSect2,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect2;
  direction = (pAnimal->getDirection()+7) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect3)
    Scan45degSector(offset, m_nFocusObjectsSect3,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect3;
  direction = (pAnimal->getDirection()+2) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect4)
    Scan45degSector(offset, m_nFocusObjectsSect4,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect4;
  direction = (pAnimal->getDirection()+6) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect5)
    Scan45degSector(offset, m_nFocusObjectsSect5,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect5;
  direction = (pAnimal->getDirection()+3) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect6)
    Scan45degSector(offset, m_nFocusObjectsSect6,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect6;
  direction = (pAnimal->getDirection()+5) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect7)
    Scan45degSector(offset, m_nFocusObjectsSect7,direction);

  offset += VIEW_SIZE_PER_FOCUS * m_nFocusObjectsSect7;
  direction = (pAnimal->getDirection()+4) % 8;
  if (tScanDirections[direction] && m_nFocusObjectsSect8)
    Scan45degSector(offset, m_nFocusObjectsSect8,direction);
  
  return m_tStimulationValues;
}

//===========================================================================
// other functions
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorView::Scan45degSector
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
bool CSensorView::Scan45degSector(size_t stimulationTabOffset,
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
  double viewChance;
  double distanceWeight;

  // Find entities according to angle, distance and layer:
  const std::vector<FoundEntity_t>& tFoundIds = pBiotop->findEntities(pAnimal->getGridCoord(), visionSectorBmp, m_nRange, m_Layer, true);

  // get focused entity
  BrainFocusedEntityView_t* pBrainFocused = m_pBrain->getpBrainFocusedEntityInfo();

  for (i = 0; i < tFoundIds.size(); i++)
  {
    pCurEntity = tFoundIds[i].pEntity;
    distanceWeight = 0;
     
    if ((pCurEntity != pBrainFocused->pPreviousEntity) && (m_nRange>1))
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
      m_pEntityViewTab[i].index = i;
      m_pEntityViewTab[i].pEntity = pCurEntity;
      m_pEntityViewTab[i].signature = pCurEntity->getEntitySignature();

      offset = 0;
      // Size big
      m_pEntityViewTab[i].weightTab[offset] = pCurEntity->getWeight() / pAnimal->getWeight();
      if (m_pEntityViewTab[i].weightTab[offset] < 1)
        m_pEntityViewTab[i].weightTab[offset] = 0;
      else if (m_pEntityViewTab[i].weightTab[offset] > MAX_SENSOR_VAL)
        m_pEntityViewTab[i].weightTab[offset] = MAX_SENSOR_VAL;
      offset++;
      // Size small
      m_pEntityViewTab[i].weightTab[offset] = pAnimal->getWeight() / pCurEntity->getWeight();
      if (m_pEntityViewTab[i].weightTab[offset] < 1)
        m_pEntityViewTab[i].weightTab[offset] = 0;
      else if (m_pEntityViewTab[i].weightTab[offset] > MAX_SENSOR_VAL)
        m_pEntityViewTab[i].weightTab[offset] = MAX_SENSOR_VAL;
      offset++;
      // Distance
      if (m_bDistanceEval)
        m_pEntityViewTab[i].weightTab[offset] = MAX_SENSOR_VAL/(tFoundIds[i].distance);
      distanceWeight = m_pEntityViewTab[i].weightTab[offset];
      offset++;
      // Movement
      m_pEntityViewTab[i].weightTab[offset] = pCurEntity->getCurrentSpeed() * MAX_SENSOR_VAL / 100.0;
      offset++;
      // Color:
      for (int colo=0; colo<VIEW_NUMBER_COLORS; colo++)
      {
        if ( pCurEntity->getColorType() == (COLOR_CARACTER_FIRST_TYPE+colo) )
          m_pEntityViewTab[i].weightTab[offset] = MAX_SENSOR_VAL ;
        else
          m_pEntityViewTab[i].weightTab[offset] = 0;
        offset++;
      }
      // Form:
      for (int form=0; form<VIEW_NUMBER_FORMS; form++)
      {
        if ( pCurEntity->getForm() == (FORM_FIRST_TYPE+form) )
          m_pEntityViewTab[i].weightTab[offset] = (3*MAX_SENSOR_VAL + distanceWeight) / 4.0;
        else
          m_pEntityViewTab[i].weightTab[offset] = 0;
        offset++;
      }
      // Texture:
      for (int textu=0; textu<VIEW_NUMBER_TEXTURES; textu++)
      {
        if (pCurEntity->getTexture() == (TEXTURE_FIRST_TYPE+textu) )
          m_pEntityViewTab[i].weightTab[offset] = (3*MAX_SENSOR_VAL + distanceWeight) / 4.0;
        else
          m_pEntityViewTab[i].weightTab[offset] = 0;
        offset++;
      }
      // Physical attribute:
      for (int attribu=0; attribu<VIEW_NUMBER_PHY_ATTRIBUT; attribu++)
      {
        if (pCurEntity->isPhyAttributePresent((PhyAttributeType_e)(PHY_ATTRIBUTE_FIRST_TYPE+attribu)))
          m_pEntityViewTab[i].weightTab[offset] = (3*MAX_SENSOR_VAL + distanceWeight) / 4.0;
        else
          m_pEntityViewTab[i].weightTab[offset] = 0;
        offset++;
      }

      // Compute weight and computedWeight sum
      m_pEntityViewTab[i].computedWeight = 0;
      for (j=0; j<VIEW_SIZE_PER_FOCUS; j++)
      {
        // Use weight
        m_pEntityViewTab[i].weightTab[j] = m_pEntityViewTab[i].weightTab[j] * m_tSubCaptorWeightRate[stimulationTabOffset+j] / 100.0;
        // Don't go over Max!
        if (m_pEntityViewTab[i].weightTab[j]  > MAX_SENSOR_VAL)
          m_pEntityViewTab[i].weightTab[j] = MAX_SENSOR_VAL;

        m_pEntityViewTab[i].computedWeight += m_pEntityViewTab[i].weightTab[j] * m_tBonusRate[stimulationTabOffset+j];
      }
    }
    else
    {
      m_pEntityViewTab[i].index = invalidIndex;
      m_pEntityViewTab[i].computedWeight = 0;
      m_pEntityViewTab[i].signature = 0;
      m_pEntityViewTab[i].pEntity = NULL;
    }
  }

  // Now choose the most significant entities
  offset = stimulationTabOffset;

  for (j=0; j<maxNumFocusObject; j++)
  {
    // 1 Find max weight
    maxComputedWeight = 0;
    maxWeightViewTabIndex = invalidIndex;
    for (i = 0; i < tFoundIds.size(); i++)
    {
      // Give 10% bonus to previousely selected entity
      if (m_pEntityViewTab[i].pEntity == pBrainFocused->pPreviousEntity)
        m_pEntityViewTab[i].computedWeight += m_pEntityViewTab[i].computedWeight/10;

      if (m_pEntityViewTab[i].computedWeight>maxComputedWeight)
      {
        maxComputedWeight = m_pEntityViewTab[i].computedWeight;
        maxWeightViewTabIndex = i;
      }
    }

    // 2 Exit if no more entities
    if (maxComputedWeight == 0)
      break;

    // 3 Update global brain focused entity if weight over previous
    if (maxComputedWeight > pBrainFocused->computedWeight)
    {
      pBrainFocused->pNewEntity      = m_pEntityViewTab[maxWeightViewTabIndex].pEntity;
      if (pAnimal->getHeadDirection() == direction)
        pBrainFocused->computedWeight  = maxComputedWeight + 2; // Add 2 to give advantage to front entity
      else
        pBrainFocused->computedWeight  = maxComputedWeight + 1; // Add 1 to give advantage to first found entity
      pBrainFocused->captorUid       = GetUniqueId();
      pBrainFocused->subcaptorIndex  = offset;
      pBrainFocused->subcaptorsSize  = VIEW_SIZE_PER_FOCUS;
    }

    // 4 Fill m_tStimulationValues
    for (i=0; i<VIEW_SIZE_PER_FOCUS; i++)
    {
      m_tStimulationValues[offset] = m_pEntityViewTab[maxWeightViewTabIndex].weightTab[i];
      offset++;
    }

    // 5 Remove identical entities
    for (i = 0; i < tFoundIds.size(); i++)
    {
      if (m_pEntityViewTab[i].signature == m_pEntityViewTab[maxWeightViewTabIndex].signature)
        m_pEntityViewTab[i].computedWeight = 0;
    }
  }

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CSensorView::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: string: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorView::GetSubCaptorLabel(size_t index)
{
  if (index>GetSubCaptorNumber())
    return ("bad index");
  else
  {
    size_t offset = index / VIEW_SIZE_PER_FOCUS;
    size_t pos = index % VIEW_SIZE_PER_FOCUS;

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

    if (pos>VIEW_SIZE_PER_FOCUS)
      captorStr = "captor error";
    else if (pos>(3+VIEW_NUMBER_COLORS+VIEW_NUMBER_FORMS+VIEW_NUMBER_TEXTURES))
      captorStr = "attr " + CBasicEntity::getPhyAttributeStrName( (PhyAttributeType_e)(pos+PHY_ATTRIBUTE_NUMBER_TYPE-VIEW_SIZE_PER_FOCUS) );
    else if (pos>(3+VIEW_NUMBER_COLORS+VIEW_NUMBER_FORMS))
      captorStr = "text " + CBasicEntity::getTextureStrName( (TextureType_e)(pos-4-VIEW_NUMBER_COLORS-VIEW_NUMBER_FORMS+TEXTURE_FIRST_TYPE) );
    else if (pos>(3+VIEW_NUMBER_COLORS))
      captorStr = "form " + CBasicEntity::getFormStrName( (FormType_e)(pos-4-VIEW_NUMBER_COLORS+FORM_FIRST_TYPE) );
    else if (pos>3)
      captorStr = "colr " + CBasicEntity::getColorStrName( (ColorCaracterType_e)(pos-4+COLOR_CARACTER_FIRST_TYPE) );
    else if (pos==3)
      captorStr = "moving";
    else if (pos==2)
      captorStr = "proximity";
    else if (pos==1)
      captorStr = "smaller";
    else
      captorStr = "bigger";

    return (directionStr + captorStr);
  }
}


size_t CSensorView::GetSubCaptorIndexForDirection(int relativeDirection, size_t entityIndex)
{
  size_t index = invalidIndex;

  switch (relativeDirection)
  {
  case 0:
    if (entityIndex<m_nFocusObjectsSect1)
      index = entityIndex * VIEW_SIZE_PER_FOCUS;
    break;

  case 1:
    if (entityIndex<m_nFocusObjectsSect2)
      index = (m_nFocusObjectsSect1 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  case 2:
    if (entityIndex<m_nFocusObjectsSect3)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  case 3:
    if (entityIndex<m_nFocusObjectsSect4)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  case 4:
    if (entityIndex<m_nFocusObjectsSect5)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  case 5:
    if (entityIndex<m_nFocusObjectsSect6)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + m_nFocusObjectsSect5 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  case 6:
    if (entityIndex<m_nFocusObjectsSect7)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + m_nFocusObjectsSect5 + m_nFocusObjectsSect6 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  case 7:
    if (entityIndex<m_nFocusObjectsSect8)
      index = (m_nFocusObjectsSect1 + m_nFocusObjectsSect2 + m_nFocusObjectsSect3 + m_nFocusObjectsSect4 + m_nFocusObjectsSect5 + m_nFocusObjectsSect6 + m_nFocusObjectsSect7 + entityIndex) * VIEW_SIZE_PER_FOCUS;
    break;

  default:
    break;
  }

  return index;
}

size_t CSensorView::GetSubCaptorSubIndexForSizeBig()
{
  return 0;
}

size_t CSensorView::GetSubCaptorSubIndexForSizeSmall()
{
  return 1;
}

size_t CSensorView::GetSubCaptorSubIndexForProximity()
{
  return 2;
}

size_t CSensorView::GetSubCaptorSubIndexForMoving()
{
  return 3;
}

size_t CSensorView::GetSubCaptorSubIndexForColor(ColorCaracterType_e color)
{
  return (4 + color - COLOR_CARACTER_FIRST_TYPE);
}

size_t CSensorView::GetSubCaptorSubIndexForForm(FormType_e form)
{
  return (4 + VIEW_NUMBER_COLORS + form - FORM_FIRST_TYPE);
}

size_t CSensorView::GetSubCaptorSubIndexForTexture(TextureType_e texture)
{
  return (4 + VIEW_NUMBER_COLORS + VIEW_NUMBER_FORMS + texture - TEXTURE_FIRST_TYPE);
}

size_t CSensorView::GetSubCaptorSubIndexForPhyAttribute(PhyAttributeType_e attribute)
{
  return (4 + VIEW_NUMBER_COLORS + VIEW_NUMBER_FORMS + VIEW_NUMBER_TEXTURES + attribute - PHY_ATTRIBUTE_FIRST_TYPE);
}

int CSensorView::GetRange()
{
  return m_nRange;
}

size_t CSensorView::GetLayer()
{
  return m_Layer;
}