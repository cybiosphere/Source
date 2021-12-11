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
// FILE: CGeoMapPopulation.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a geographic map to store purpose success
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  16/09/2011 : Creation
//
//===========================================================================

#if !defined(GEOMAPPOPULATION_INCLUDED_)
#define GEOMAPPOPULATION_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CGeoMap.h"
#include "CBiotop.h"

//===========================================================================
// Definitions            
//===========================================================================


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGeoMapPopulation : public CGeoMap
{
//===========================================================================
// Attributes 
//===========================================================================
private:
  size_t   m_CurrentNumberRecords;
  CBiotop* m_pBiotop;
  std::vector<size_t> m_tTimeStamps;
  std::string m_specieName;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CGeoMapPopulation(CBiotop* pBiotop, Point_t gridBiotopSize, std::string specieName, size_t maxNumberRecords = 500);
  ~CGeoMapPopulation();

//---------------------------------------------------------------------------
// public methods
//---------------------------------------------------------------------------
public:
  bool MemorizePopulationMap(size_t dayIndex);
  size_t GetPopulationInSquareMap(size_t dayIndex, Point_t geoMapPos);
  string GetSpecieName();

  //---------------------------------------------------------------------------
  // File management
  //---------------------------------------------------------------------------
  bool saveInXmlFile(string fileNameWithPath);
  bool saveInXmlFile(TiXmlDocument* pXmlDoc);
  static size_t getNumberSpeciesStoredInFile(string fileNameWithPath);
  bool loadFromXmlFile(string fileNameWithPath, size_t indexOfRecordInFile);
  bool loadFromXmlFile(TiXmlDocument* pXmlDoc, size_t indexOfRecordInFile);

//---------------------------------------------------------------------------
// private methods
//---------------------------------------------------------------------------
private:
  size_t GetTableIndex(size_t dayIndex);
  size_t CountEntitiesInMapSquare(std::string specieName, size_t geoMapPosX, size_t geoMapPosY);

};

#endif // !defined(GEOMAPPOPULATION_INCLUDED_)
