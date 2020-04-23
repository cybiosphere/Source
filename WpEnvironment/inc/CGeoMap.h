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
// FILE: CGeoMap.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a geographic map to store purpose success
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  16/09/2011 : Creation
//
//===========================================================================

#if !defined(GEOMAP_INCLUDED_)
#define GEOMAP_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CBrainAnimal.h"

//===========================================================================
// Definitions            
//===========================================================================
#define NB_GRID_PER_GEOMAP_SQUARE 25


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGeoMap  
{
//===========================================================================
// Attributes 
//===========================================================================
protected:  // Fred remettre en private ?
  RelativePos_t m_GeoCoordStart;
  Point_t       m_GeoMapSize;
  size_t        m_nbRecords;

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
protected: // Fred remettre en private ?
  short***      m_pMemoryMap;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CGeoMap(Point_t gridCoordCenterPos, Point_t gridBiotopSize, Point_t gridMapSize, size_t nbRecords);
  ~CGeoMap();

  //---------------------------------------------------------------------------
  // public methods
  //---------------------------------------------------------------------------
public:
  bool GridCoordToGeoMapCoord(Point_t gridPos, Point_t& geoMapPos, bool giveEdgePositionWhenOut = false);

//---------------------------------------------------------------------------
// protected methods
//---------------------------------------------------------------------------
protected:
  void ConvergeAllRecordsToNeutral();
  void ClearRecordOnFullMap(size_t recordIndex);
  string buildStringDataFromGeoMapRecord(size_t recordIndex);
  bool buildGeoMapRecordFromStringData(size_t recordIndex, string rawData);

//---------------------------------------------------------------------------
// private methods
//---------------------------------------------------------------------------
private:


};

#endif // !defined(GEOMAP_INCLUDED_)
