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
// FILE: CGeoMapPurpose.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a geographic map to store purpose success
//         Map position level is increased when purpose success is reached.
//         Map position level is decreased when purpose success in not reach at timeout
//           or when target geographic position cannot be reach at timeout      
//         Map level erosion is performed every day on all positions for all purpose                 
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  16/09/2011 : Creation
//
//===========================================================================

#include "CGeoMapPurpose.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include <math.h>

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGeoMapPurpose::CGeoMapPurpose
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CGeoMapPurpose::CGeoMapPurpose(CBrain* pBrain, Point_t gridCoordCenterPos, Point_t gridBiotopSize, size_t gridMapSize, size_t nbPurposeRec)
  : CGeoMap(gridCoordCenterPos, gridBiotopSize, { gridMapSize, gridMapSize }, nbPurposeRec)
{
  m_pBrain = pBrain;
  m_NbPurposeRec = nbPurposeRec;

  // attributes used to manage timeout on target direction in GeoMap
  m_curTargetMapPos.x = invalidCoord;
  m_curTargetMapPos.y = invalidCoord;
  m_curPurposeUidIdx  = 0xFFFFFFFF;
  m_curTargetTimout   = 0;

  m_tPurposeUniqueId.resize(nbPurposeRec);
  for (size_t idx=0; idx<nbPurposeRec; idx++)
    m_tPurposeUniqueId[idx] = 0;
}

CGeoMapPurpose::~CGeoMapPurpose()
{
}


//===========================================================================
// public methods
//===========================================================================
bool CGeoMapPurpose::MemorizePurposeSuccessPos(DWORD purposeUid, Point_t gridPos, int weight)
{
  Point_t geoMapCoord;

  if (GridCoordToGeoMapCoord(gridPos, geoMapCoord) == true)
  {
    size_t UidIdx = GetPurposeUidTabIndex(purposeUid);
    MemorizePurposeSuccessGeoPos(UidIdx, geoMapCoord, weight);
  }
  return true;
}

void CGeoMapPurpose::ClearPurposeSuccessOnFullMap(DWORD purposeUid)
{
  size_t UidIdx = GetPurposeUidTabIndex(purposeUid);
  ClearRecordOnFullMap(UidIdx);
}

GeoMapIntensityType_e CGeoMapPurpose::GetClosestSuccessPos(DWORD purposeUid, Point_t gridCenterPos, int &absoluteDirection )
{
  size_t range = 0;
  int maxWeight = 0;
  int curWeight = 0;
  int initialWeight = 0;
  Point_t foundMapPos;
  foundMapPos.x = invalidCoord;
  foundMapPos.y = invalidCoord;
  absoluteDirection = -1;
  Point_t centerMapCoord;
  int delta = 0;

  bool foundSuccess = false;
  GeoMapIntensityType_e foundIntensity = GeoMapIntensityType_e::FOUND_INTENSITY_NULL;

  size_t UidIdx = GetPurposeUidTabIndex(purposeUid);
  if (UidIdx != invalidIndex)
  {
    Point_t currentMapCoord;
    if (GridCoordToGeoMapCoord(gridCenterPos, centerMapCoord) == true)
    {
      curWeight = GetSuccessWeight(UidIdx, centerMapCoord);
      if (curWeight>0)
        return foundIntensity; // Success already exists on current position. no need to guide anywhere else

      maxWeight = curWeight;
      initialWeight = curWeight;
      for (range=1; range<m_GeoMapSize.x; range++)
      {
        for(size_t i=0; i<(2*range); i++)
        {
          currentMapCoord.x = centerMapCoord.x - range + i + 1;
          currentMapCoord.y = centerMapCoord.y - range;
          curWeight = GetSuccessWeight(UidIdx, currentMapCoord);
          if ((curWeight>maxWeight) && (currentMapCoord.x>=0) && (currentMapCoord.y>=0))
          {
            foundMapPos.x = currentMapCoord.x;
            foundMapPos.y = currentMapCoord.y;
            maxWeight = curWeight;
          }
          currentMapCoord.x = centerMapCoord.x - range;
          currentMapCoord.y = centerMapCoord.y - range + i;
          curWeight = GetSuccessWeight(UidIdx, currentMapCoord);
          if ((curWeight>maxWeight) && (currentMapCoord.x>=0) && (currentMapCoord.y>=0))
          {
            foundMapPos.x = currentMapCoord.x;
            foundMapPos.y = currentMapCoord.y;
            maxWeight = curWeight;
          }
          currentMapCoord.x = centerMapCoord.x + range;
          currentMapCoord.y = centerMapCoord.y - range + i + 1;
          curWeight = GetSuccessWeight(UidIdx, currentMapCoord);
          if ((curWeight>maxWeight) && (currentMapCoord.y>=0))
          {
            foundMapPos.x = currentMapCoord.x;
            foundMapPos.y = currentMapCoord.y;
            maxWeight = curWeight;
          }
          currentMapCoord.x = centerMapCoord.x - range + i;
          currentMapCoord.y = centerMapCoord.y + range;
          curWeight = GetSuccessWeight(UidIdx, currentMapCoord);
          if ((curWeight>maxWeight) && (currentMapCoord.x>=0))
          {
            foundMapPos.x = currentMapCoord.x;
            foundMapPos.y = currentMapCoord.y;
            maxWeight = curWeight;
          }
        }
        // Exit at first found success
        if (maxWeight>initialWeight)
        {
          foundSuccess = true;
          delta = maxWeight-initialWeight;
          if (delta > 12)
          {
            foundIntensity = GeoMapIntensityType_e::FOUND_INTENSITY_HIGH;
            break;
          }
          else if (delta > 2)
          {
            foundIntensity = GeoMapIntensityType_e::FOUND_INTENSITY_MEDIUM;
            break; 
          }
          else
          {
            foundIntensity = GeoMapIntensityType_e::FOUND_INTENSITY_LOW;
            // No break: try to find better
          }
        }
      }
    }
    else // Entity is out of GeoMap: Go back to territory
    {
      foundSuccess = true;
      GridCoordToGeoMapCoord(gridCenterPos, foundMapPos, true);
      foundIntensity = GeoMapIntensityType_e::FOUND_INTENSITY_MEDIUM;
    }
  }

  if (foundSuccess)
  {
    RelativePos_t vectorDir;
    vectorDir.x = (int)foundMapPos.x - (int)centerMapCoord.x;
    vectorDir.y = (int)foundMapPos.y - (int)centerMapCoord.y;

    if ( (vectorDir.x != 0) || (vectorDir.y != 0) )
    {
      double norm= sqrt((double)(vectorDir.x * vectorDir.x + vectorDir.y * vectorDir.y));
      double adj= (double)(vectorDir.x) / norm;
      double angle = acos(adj);
      if (vectorDir.y < 0)
        angle = -angle;

      absoluteDirection = cybio_round(angle * 4 / CYBIO_PI + 8) % 8;
    }
    else
    {
      foundSuccess = false;
      foundIntensity = GeoMapIntensityType_e::FOUND_INTENSITY_NULL;
    }
  }

  // Manage timeout on target direction in GeoMap
  if (foundSuccess)
  {
    if ((m_curPurposeUidIdx == UidIdx) && (m_curTargetMapPos.x == foundMapPos.x) && (m_curTargetMapPos.y == foundMapPos.y))
    {
      m_curTargetTimout--;
      if (m_curTargetTimout<1)
      {
        // Give a negative weight to target position to switch to another place
        MemorizePurposeSuccessGeoPos(m_curPurposeUidIdx, m_curTargetMapPos, -1);
        m_curTargetTimout = range * 240; // Give 4 minutes per range to reach target position
      }
    }
    else
    {
      // Set new target position info
      m_curTargetMapPos.x = foundMapPos.x;
      m_curTargetMapPos.y = foundMapPos.y;
      m_curPurposeUidIdx  = UidIdx;
      m_curTargetTimout   = range * 240; // Give 4 minutes per range to reach target position
    }
  }

  return foundIntensity;
}


void CGeoMapPurpose::NextDay()
{
  ConvergeAllRecordsToNeutral();
}

//===========================================================================
// private methods
//===========================================================================

size_t CGeoMapPurpose::GetPurposeUidTabIndex(DWORD purposeUid)
{
  size_t i;
  // Check if purpose already exist
  for (i=0; i<m_NbPurposeRec; i++)
  {
    if (purposeUid == m_tPurposeUniqueId[i])
      return i;
  }

  // Check if there are free id in table
  for (i=0; i<m_NbPurposeRec; i++)
  {
    if (m_tPurposeUniqueId[i] == 0)
    {
      m_tPurposeUniqueId[i] = purposeUid;
      return i;
    }
  }
  return invalidIndex;
}

DWORD CGeoMapPurpose::GettPurposeUniqueId (size_t index)
{
  if (index >= m_NbPurposeRec)
    return 0;
  else
    return m_tPurposeUniqueId[index];
}

int CGeoMapPurpose::GetSuccessWeight(size_t purposeIndex, Point_t geoMapPos)
{
  // If pose out of teritory map, give negative weight -100
  int weight = -100;

  if ((geoMapPos.x<m_GeoMapSize.x) && (geoMapPos.y<m_GeoMapSize.y))
  {
    weight = m_pMemoryMap[geoMapPos.x][geoMapPos.y][purposeIndex];
  }

  return weight;
}


bool CGeoMapPurpose::MemorizePurposeSuccessGeoPos(size_t purposeIndex, Point_t geoMapPos, int weight)
{
  bool resu = false;
  short newWeight;

  if ((geoMapPos.x < m_GeoMapSize.x) && (geoMapPos.y < m_GeoMapSize.y) && (purposeIndex < m_NbPurposeRec))
  {
    // Allow negative values to avoid staying static in a bad place
    newWeight = m_pMemoryMap[geoMapPos.x][geoMapPos.y][purposeIndex] + weight;
    // Force range [-100..1000]
   if (newWeight > 1000)
     newWeight = 1000;
   else if (newWeight < -100)
     newWeight = -100;

   m_pMemoryMap[geoMapPos.x][geoMapPos.y][purposeIndex] = newWeight;
    resu = true;
  }
  
  return resu;
}