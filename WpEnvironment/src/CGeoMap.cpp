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
CGeoMap::CGeoMap(Point_t globalGridCoordCenterPos, Point_t globalGridBiotopSize, Point_t gridMapSize, size_t nbRecords)
{
  size_t expectedMapSizeX = (gridMapSize.x - 1) / NB_GRID_PER_GEOMAP_SQUARE + 1;
  size_t expectedMapSizeY = (gridMapSize.y - 1) / NB_GRID_PER_GEOMAP_SQUARE + 1;
  size_t biotopMapSizeX = (size_t)((int)globalGridBiotopSize.x - 1) / NB_GRID_PER_GEOMAP_SQUARE + 1;
  size_t biotopMapSizeY = (size_t)((int)globalGridBiotopSize.y - 1) / NB_GRID_PER_GEOMAP_SQUARE + 1;

  m_GeoMapSize.x = cybio_min(expectedMapSizeX, biotopMapSizeX);
  m_GeoMapSize.y = cybio_min(expectedMapSizeY, biotopMapSizeY);
  m_GeoCoordStart.x = (int)globalGridCoordCenterPos.x / NB_GRID_PER_GEOMAP_SQUARE - (int)m_GeoMapSize.x/2;
  m_GeoCoordStart.y = (int)globalGridCoordCenterPos.y / NB_GRID_PER_GEOMAP_SQUARE - (int)m_GeoMapSize.y/2;
  m_nbRecords = nbRecords;

  // shift to fit in biotop
  if ((m_GeoCoordStart.x + m_GeoMapSize.x) > biotopMapSizeX)
    m_GeoCoordStart.x = (int)biotopMapSizeX - (int)m_GeoMapSize.x;
  if ((m_GeoCoordStart.y + m_GeoMapSize.y) > biotopMapSizeY)
    m_GeoCoordStart.y = (int)biotopMapSizeY - (int)m_GeoMapSize.y;
  if (m_GeoCoordStart.x < 0)
    m_GeoCoordStart.x = 0;
  if (m_GeoCoordStart.y < 0)
    m_GeoCoordStart.y = 0;

  // Build 3D dynamic table
  m_pMemoryMap   = new short**[m_GeoMapSize.x];
  ASSERT( m_pMemoryMap != NULL );
  for (size_t i = 0; i < m_GeoMapSize.x; i++)
  {
    m_pMemoryMap[i]   = new short*[m_GeoMapSize.y];
    ASSERT( m_pMemoryMap[i] != NULL );
    for (size_t j = 0; j < m_GeoMapSize.y; j++)
    {
      m_pMemoryMap[i][j] = new short[m_nbRecords];
      ASSERT( m_pMemoryMap[i][j] != NULL );
      for (size_t k = 0; k < m_nbRecords; k++)
        m_pMemoryMap[i][j][k] = 0;
    }
  }
}

CGeoMap::~CGeoMap()
{
  // Delete 3D dynamic table 
  for (size_t i = 0; i < m_GeoMapSize.x; i++)
  {
    for (size_t j = 0; j < m_GeoMapSize.y; j++)
    {
      delete [] m_pMemoryMap[i][j];
    }
    delete [] m_pMemoryMap[i];
  }
  delete [] m_pMemoryMap;
}


//===========================================================================
// protected methods
//===========================================================================
void CGeoMap::ClearRecordOnFullMap(size_t recordIndex)
{
  if (recordIndex < m_nbRecords)
  {
    for (size_t i = 0; i < m_GeoMapSize.x; i++)
    {
      for (size_t j = 0; j < m_GeoMapSize.y; j++)
      {
        m_pMemoryMap[i][j][recordIndex] = 0;
      }
    }
  }
}

void CGeoMap::ConvergeAllRecordsToNeutral()
{
  for (size_t i = 0; i < m_GeoMapSize.x; i++)
  {
    for (size_t j = 0; j < m_GeoMapSize.y; j++)
    {
      for (size_t k = 0; k < m_nbRecords; k++)
      {
        if (m_pMemoryMap[i][j][k] > 0)
          m_pMemoryMap[i][j][k]--;
        else if (m_pMemoryMap[i][j][k] < 0)
          m_pMemoryMap[i][j][k]++; 

      }
    }
  }
}

string CGeoMap::buildStringDataFromGeoMapRecord(size_t recordIndex)
{
  string rawDataRsp = "";
  string tmpStr;
  if (recordIndex < m_nbRecords)
  {
    for (size_t i = 0; i < m_GeoMapSize.x; i++)
    {
      for (size_t j = 0; j < m_GeoMapSize.y; j++)
      {
        tmpStr = FormatString("%04X", (WORD)m_pMemoryMap[i][j][recordIndex]);
        rawDataRsp += tmpStr;
      }
    }
  }
  return rawDataRsp;
}

bool CGeoMap::buildGeoMapRecordFromStringData(size_t recordIndex, string rawData)
{
  string tmpStr = "0000";
  string rawDataRsp = "";
  size_t strOffset = 0;
  size_t dataLen = rawData.length();
  char* pEnd;
  if (recordIndex < m_nbRecords)
  {
    for (size_t i = 0; i < m_GeoMapSize.x; i++)
    {
      for (size_t j = 0; j < m_GeoMapSize.y; j++)
      {
        if ((strOffset + 4) > dataLen)
        {
          return false;
        }
        tmpStr = rawData.substr(strOffset, 4);
        m_pMemoryMap[i][j][recordIndex] = (short)strtol(tmpStr.c_str(), &pEnd, 16);
        strOffset += 4;
      }
    }
  }
  return true;
}

//===========================================================================
// public methods
//===========================================================================

bool CGeoMap::GridCoordToGeoMapCoord(Point_t globalGridPos, Point_t &geoMapPos, bool giveEdgePositionWhenOut)
{
  bool gridPosIsInsideGeoMap = true;
  Point_t geoMapCoord;
  geoMapCoord.x = globalGridPos.x / NB_GRID_PER_GEOMAP_SQUARE;
  geoMapCoord.y = globalGridPos.y / NB_GRID_PER_GEOMAP_SQUARE;

  if(giveEdgePositionWhenOut)
  {
    if (geoMapCoord.x < m_GeoCoordStart.x)
      geoMapCoord.x = m_GeoCoordStart.x;
    else if (geoMapCoord.x >= (m_GeoCoordStart.x + m_GeoMapSize.x))
      geoMapCoord.x = m_GeoCoordStart.x + m_GeoMapSize.x - 1;

    if (geoMapCoord.y < m_GeoCoordStart.y)
      geoMapCoord.y = m_GeoCoordStart.y;
    else if (geoMapCoord.y >= (m_GeoCoordStart.y + m_GeoMapSize.y))
      geoMapCoord.y = m_GeoCoordStart.y + m_GeoMapSize.y - 1;
  }
  else
  {
    if( (geoMapCoord.x < m_GeoCoordStart.x) || (geoMapCoord.x >= (m_GeoCoordStart.x + m_GeoMapSize.x))
     || (geoMapCoord.y < m_GeoCoordStart.y) || (geoMapCoord.y >= (m_GeoCoordStart.y + m_GeoMapSize.y)) )
    {
      gridPosIsInsideGeoMap = false;
    }
  }

  geoMapPos.x = geoMapCoord.x - m_GeoCoordStart.x;
  geoMapPos.y = geoMapCoord.y - m_GeoCoordStart.y;
  return gridPosIsInsideGeoMap;
}
