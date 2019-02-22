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
// FILE: CGeoMap.cpp
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

#include "CGeoMap.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include <math.h>

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGeoMap::CGeoMap
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CGeoMap::CGeoMap(CBrain* pBrain, Point_t gridCoordCenterPos, int gridMapSize, int nbPurposeRec)
{
  m_pBrain = pBrain;

  m_GeoMapSize = (gridMapSize-1) / NB_GRID_PER_GEOMAP_SQUARE + 1;
  m_GeoCoordStart.x = gridCoordCenterPos.x / NB_GRID_PER_GEOMAP_SQUARE - m_GeoMapSize/2;
  m_GeoCoordStart.y = gridCoordCenterPos.y / NB_GRID_PER_GEOMAP_SQUARE - m_GeoMapSize/2;
  if (m_GeoCoordStart.x <0)
    m_GeoCoordStart.x = 0;
  if (m_GeoCoordStart.y <0)
    m_GeoCoordStart.y = 0;
  m_NbPurposeRec = nbPurposeRec;

  // attributes used to manage timeout on target direction in GeoMap
  m_curTargetMapPos.x = -1;
  m_curTargetMapPos.y = -1;
  m_curPurposeUidIdx  = -1;
  m_curTargetTimout   = 0;

  m_tPurposeUniqueId = new DWORD[nbPurposeRec];
  for (int idx=0; idx<nbPurposeRec; idx++)
    m_tPurposeUniqueId[idx] = 0;

  // Build 3D dynamic table
  m_pMemoryMap   = new short**[m_GeoMapSize];
  ASSERT( m_pMemoryMap != NULL );
  for (int i=0;i<m_GeoMapSize;i++)
  {
    m_pMemoryMap[i]   = new short*[m_GeoMapSize];
    ASSERT( m_pMemoryMap[i] != NULL );
    for (int j=0;j<m_GeoMapSize;j++)
    {
      m_pMemoryMap[i][j] = new short[nbPurposeRec];
      ASSERT( m_pMemoryMap[i][j] != NULL );
      for (int k=0; k<nbPurposeRec; k++)
        m_pMemoryMap[i][j][k] = 0;
    }
  }
}

CGeoMap::~CGeoMap()
{
  delete [] m_tPurposeUniqueId;

  // Delete 3D dynamic table 
  for (int i=0; i<m_GeoMapSize; i++)
  {
    for (int j=0; j<m_GeoMapSize; j++)
    {
      delete [] m_pMemoryMap[i][j];
    }
    delete [] m_pMemoryMap[i];
  }
  delete [] m_pMemoryMap;
}


//===========================================================================
// public methods
//===========================================================================
bool CGeoMap::MemorizePurposeSuccessPos(DWORD purposeUid, Point_t gridPos, int weight)
{
  Point_t geoMapCoord;

  if (GridCoordToGeoMapCoord(gridPos, geoMapCoord) == true)
  {
    int UidIdx = GetPurposeUidTabIndex(purposeUid);
    MemorizePurposeSuccessGeoPos(UidIdx, geoMapCoord, weight);
  }
  return true;
}

void CGeoMap::ClearPurposeSuccessOnFullMap(DWORD purposeUid)
{
  int UidIdx = GetPurposeUidTabIndex(purposeUid);
  if ((UidIdx >= 0) && (UidIdx < m_NbPurposeRec))
  {
    for (int i = 0; i < m_GeoMapSize; i++)
    {
      for (int j = 0; j < m_GeoMapSize; j++)
      {
        m_pMemoryMap[i][j][UidIdx] = 0;
      }
    }
  }
}

GeoMapIntensityType_e CGeoMap::GetClosestSuccessPos(DWORD purposeUid, Point_t gridCenterPos, int &absoluteDirection )
{
  int range = 0;
  int maxWeight = 0;
  int curWeight = 0;
  int initialWeight = 0;
  Point_t foundMapPos;
  foundMapPos.x = -1;
  foundMapPos.y = -1;
  absoluteDirection = -1;
  Point_t centerMapCoord;
  int delta = 0;

  bool foundSuccess = false;
  GeoMapIntensityType_e foundIntensity = FOUND_INTENSITY_NULL;

  int UidIdx = GetPurposeUidTabIndex(purposeUid);
  if (UidIdx > -1)
  {
    Point_t currentMapCoord;
    if (GridCoordToGeoMapCoord(gridCenterPos, centerMapCoord) == true)
    {
      curWeight = GetSuccessWeight(UidIdx, centerMapCoord);
      if (curWeight>0)
        return foundIntensity; // Success already exists on current position. no need to guide anywhere else

      maxWeight = curWeight;
      initialWeight = curWeight;
      for (range=1; range<m_GeoMapSize; range++)
      {
        for(int i=0; i<(2*range); i++)
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
            foundIntensity = FOUND_INTENSITY_HIGH;
            break;
          }
          else if (delta > 2)
          {
            foundIntensity = FOUND_INTENSITY_MEDIUM;
            break; 
          }
          else
          {
            foundIntensity = FOUND_INTENSITY_LOW;
            // No break: try to find better
          }
        }
      }
    }
    else // Entity is out of GeoMap: Go back to territory
    {
      foundSuccess = true;
      GridCoordToGeoMapCoord(gridCenterPos, foundMapPos, true);
      foundIntensity = FOUND_INTENSITY_MEDIUM;
    }
  }

  if (foundSuccess)
  {
    Point_t vectorDir;
    vectorDir.x = foundMapPos.x - centerMapCoord.x;
    vectorDir.y = foundMapPos.y - centerMapCoord.y;

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
      foundIntensity = FOUND_INTENSITY_NULL;
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


void CGeoMap::NextDay()
{
  for (int i=0; i<m_GeoMapSize; i++)
  {
    for (int j=0; j<m_GeoMapSize; j++)
    {
      for (int k=0; k<m_NbPurposeRec; k++)
      {
        if (m_pMemoryMap[i][j][k] > 0)
          m_pMemoryMap[i][j][k]--;
        else if (m_pMemoryMap[i][j][k] < 0)
          m_pMemoryMap[i][j][k]++; 

      }
    }
  }
}

//===========================================================================
// private methods
//===========================================================================

bool CGeoMap::GridCoordToGeoMapCoord(Point_t gridPos, Point_t &geoMapPos, bool giveEdgePositionWhenOut)
{
  bool gridPosIsInsideGeoMap = true;
  Point_t geoMapCoord;
  geoMapCoord.x = gridPos.x / NB_GRID_PER_GEOMAP_SQUARE;
  geoMapCoord.y = gridPos.y / NB_GRID_PER_GEOMAP_SQUARE;

  if(giveEdgePositionWhenOut)
  {
    if (geoMapCoord.x < m_GeoCoordStart.x)
      geoMapCoord.x = m_GeoCoordStart.x;
    else if (geoMapCoord.x >= (m_GeoCoordStart.x + m_GeoMapSize))
      geoMapCoord.x = m_GeoCoordStart.x + m_GeoMapSize - 1;

    if (geoMapCoord.y < m_GeoCoordStart.y)
      geoMapCoord.y = m_GeoCoordStart.y;
    else if (geoMapCoord.y >= (m_GeoCoordStart.y + m_GeoMapSize))
      geoMapCoord.y = m_GeoCoordStart.y + m_GeoMapSize - 1;
  }
  else
  {
    if( (geoMapCoord.x < m_GeoCoordStart.x) || (geoMapCoord.x >= (m_GeoCoordStart.x + m_GeoMapSize))
     || (geoMapCoord.y < m_GeoCoordStart.y) || (geoMapCoord.y >= (m_GeoCoordStart.y + m_GeoMapSize)) )
    {
      gridPosIsInsideGeoMap = false;
    }
  }

  geoMapPos.x = geoMapCoord.x - m_GeoCoordStart.x;
  geoMapPos.y = geoMapCoord.y - m_GeoCoordStart.y;
  return gridPosIsInsideGeoMap;
}

int CGeoMap::GetPurposeUidTabIndex(DWORD purposeUid)
{
  int i;
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
  return -1;
}

DWORD CGeoMap::GettPurposeUniqueId (int index)
{
  if ((index<0) || (index>=m_NbPurposeRec))
    return 0;
  else
    return m_tPurposeUniqueId[index];
}

int CGeoMap::GetSuccessWeight(int purposeIndex, Point_t geoMapPos)
{
  // If pose out of teritory map, give negative weight -100
  int weight = -100;

  if (purposeIndex > -1)
  {
    if ((geoMapPos.x>=0) && (geoMapPos.y>=0) && (geoMapPos.x<m_GeoMapSize) && (geoMapPos.y<m_GeoMapSize))
    {
      weight = m_pMemoryMap[geoMapPos.x][geoMapPos.y][purposeIndex];
    }
  }
  return weight;
}


bool CGeoMap::MemorizePurposeSuccessGeoPos(int purposeIndex, Point_t geoMapPos, int weight)
{
  bool resu = false;
  short newWeight;

  if ((geoMapPos.x>=0) && (geoMapPos.y>=0) && (geoMapPos.x<m_GeoMapSize) && (geoMapPos.y<m_GeoMapSize) 
    && (purposeIndex>=0) && (purposeIndex < m_NbPurposeRec))
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