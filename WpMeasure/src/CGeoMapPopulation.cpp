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
// FILE: CGeoMapPopulation.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a geographic map to store population         
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  16/09/2011 : Creation
//
//===========================================================================

#include "CGeoMapPopulation.h"
#include <math.h>

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CGeoMapPopulation::CGeoMapPopulation
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CGeoMapPopulation::CGeoMapPopulation(CBiotop* pBiotop, Point_t gridBiotopSize, std::string specieName, size_t maxNumberRecords)
  : CGeoMap({ gridBiotopSize.x / 2, gridBiotopSize.y / 2 }, gridBiotopSize, gridBiotopSize, maxNumberRecords)
{
  m_MaxNumberRecords = maxNumberRecords;
  m_tTimeStamps.resize(maxNumberRecords);
  m_pBiotop = pBiotop;
  m_specieName = specieName;
}

CGeoMapPopulation::~CGeoMapPopulation()
{
}


//===========================================================================
// public methods
//===========================================================================
bool CGeoMapPopulation::MemorizePopulationMap(size_t dayIndex)
{
  size_t indexRecord = GetNewTabIndex(dayIndex);

  for (size_t i = 0; i < m_GeoMapSize.x; i++)
  {
    for (size_t j = 0; j < m_GeoMapSize.y; j++)
    {
      m_pMemoryMap[i][j][dayIndex] = CountEntitiesInMapSquare(m_specieName, i, j);
    }
  }

  return true;
}

size_t CGeoMapPopulation::GetPopulationInSquareMap(size_t dayIndex, Point_t geoMapPos)
{
  // If pose out of teritory map, give negative weight -100
  size_t population = 0;

  if ((geoMapPos.x < m_GeoMapSize.x) && (geoMapPos.y < m_GeoMapSize.y))
  {
    population = m_pMemoryMap[geoMapPos.x][geoMapPos.y][dayIndex];
  }

  return population;
}

string CGeoMapPopulation::GetSpecieName()
{
  return m_specieName;
}

//===========================================================================
// private methods
//===========================================================================
size_t CGeoMapPopulation::GetNewTabIndex(size_t dayIndex)
{
  size_t i;
  // Check if dayIndex already exist
  for (i = 0; i < m_MaxNumberRecords; i++)
  {
    if (dayIndex == m_tTimeStamps[i])
      return i;
  }
  // Check if there are free id in table
  for (i = 0; i < m_MaxNumberRecords; i++)
  {
    if (m_tTimeStamps[i] == 0)
    {
      m_tTimeStamps[i] = dayIndex;
      return i;
    }
  }
  return invalidIndex;
}

size_t CGeoMapPopulation::CountEntitiesInMapSquare(std::string specieName, size_t geoMapPosX, size_t geoMapPosY)
{
  size_t countEntities = 0;
  Point_t gridCoord;
  gridCoord.x = geoMapPosX * NB_GRID_PER_GEOMAP_SQUARE;
  gridCoord.y = geoMapPosY * NB_GRID_PER_GEOMAP_SQUARE;
  const BiotopFoundIds_t& biotopFoundIds = m_pBiotop->findEntitiesInSquare(gridCoord, NB_GRID_PER_GEOMAP_SQUARE, true);
  const std::vector<FoundEntity_t>& tFoundIds = biotopFoundIds.tFoundIds;
  for (size_t i = 0; i < biotopFoundIds.nbFoundIds; i++)
  {
    if (tFoundIds[i].pEntity->getSpecieName() == specieName)
    {
      countEntities++;  
    }
  }
  return countEntities;
}