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
// FILE: CGeoMapPurpose.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a geographic map to store purpose success
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  16/09/2011 : Creation
//
//===========================================================================

#if !defined(GEOMAPPURPOSE_INCLUDED_)
#define GEOMAPPURPOSE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CBrainAnimal.h"
#include "CGeoMap.h"

//===========================================================================
// Definitions            
//===========================================================================
enum class GeoMapIntensityType_e
{
  FOUND_INTENSITY_NULL = 0,
  FOUND_INTENSITY_LOW,
  FOUND_INTENSITY_MEDIUM,
  FOUND_INTENSITY_HIGH,
};

class CBrainAnimal;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGeoMapPurpose : public CGeoMap
{
//===========================================================================
// Attributes 
//===========================================================================
private:
  size_t        m_NbPurposeRec;

  // attributes used to manage timeout on target direction in GeoMap
  Point_t       m_curTargetMapPos;
  size_t        m_curPurposeUidIdx;
  size_t        m_curTargetTimout;

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
  CBrain*       m_pBrain;
  std::vector<DWORD> m_tPurposeUniqueId;


//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CGeoMapPurpose(CBrain* pBrain, Point_t gridCoordCenterPos, Point_t gridBiotopSize, size_t gridMapSize, size_t nbPurposeRec);
  ~CGeoMapPurpose();

//---------------------------------------------------------------------------
// public methods
//---------------------------------------------------------------------------
public:

  bool MemorizePurposeSuccessPos(DWORD purposeUid, Point_t gridPos, int weight);
  GeoMapIntensityType_e GetClosestSuccessPos(DWORD purposeUid, Point_t gridCenterPos, int &absoluteDirection);
  void ClearPurposeSuccessOnFullMap(DWORD purposeUid);
  void NextDay();

  int GetSuccessWeight(size_t purposeIndex, Point_t geoMapPos);
  DWORD GettPurposeUniqueId (size_t index);

  bool saveInXmlFile(TiXmlDocument* pXmlDoc);
  bool loadFromXmlFile(TiXmlDocument* pXmlDoc);

//---------------------------------------------------------------------------
// private methods
//---------------------------------------------------------------------------
private:
  size_t GetPurposeUidTabIndex(DWORD purposeUid);
  bool MemorizePurposeSuccessGeoPos(size_t purposeIndex, Point_t geoMapPos, int weight);

};

#endif // !defined(GEOMAPPURPOSE_INCLUDED_)
