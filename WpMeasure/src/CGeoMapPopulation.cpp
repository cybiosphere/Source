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
  m_tTimeStamps.resize(maxNumberRecords);
  m_pBiotop = pBiotop;
  m_specieName = specieName;
  m_CurrentNumberRecords = 0;
}

CGeoMapPopulation::~CGeoMapPopulation()
{
}


//===========================================================================
// public methods
//===========================================================================
bool CGeoMapPopulation::MemorizePopulationMap(size_t dayIndex)
{
  size_t indexRecord = GetTableIndex(dayIndex);
  for (size_t i = 0; i < m_GeoMapSize.x; i++)
  {
    for (size_t j = 0; j < m_GeoMapSize.y; j++)
    {
      m_pMemoryMap[i][j][indexRecord] = (short)CountEntitiesInMapSquare(m_specieName, i, j);
    }
  }

  return true;
}

size_t CGeoMapPopulation::GetPopulationInSquareMap(size_t dayIndex, Point_t geoMapPos)
{
  size_t indexRecord = GetTableIndex(dayIndex);
  size_t population = 0;

  if ((geoMapPos.x < m_GeoMapSize.x) && (geoMapPos.y < m_GeoMapSize.y))
  {
    population = m_pMemoryMap[geoMapPos.x][geoMapPos.y][indexRecord];
  }

  return population;
}

string CGeoMapPopulation::GetSpecieName()
{
  return m_specieName;
}

//===========================================================================
// File management
//===========================================================================
bool CGeoMapPopulation::saveInXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  xmlDoc.LoadFile();
  saveInXmlFile(&xmlDoc);
  resu = xmlDoc.SaveFile();
  return resu;
}

bool CGeoMapPopulation::saveInXmlFile(TiXmlDocument* pXmlDoc)
{
  TiXmlElement* pElement;
  TiXmlNode* pNodeBiotop = NULL;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeChild = NULL;
  size_t index;
  string previousFileName = "";
  int nbSpeciesInFile = 0;

  pNodeBiotop = pXmlDoc->FirstChild(XML_NODE_BIOTOP);
  if (pNodeBiotop == NULL)
  {
    TiXmlElement newNode(XML_NODE_BIOTOP);
    pNodeBiotop = pXmlDoc->InsertEndChild(newNode);
  }

  if ((pNodeBiotop != NULL) && (pNodeBiotop->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeBiotop;
    // Get previous number of species in file
    pElement->QueryIntAttribute(XML_ATTR_NB_SPECIES, &nbSpeciesInFile);
    // Set attributes
    pElement->SetAttribute(XML_ATTR_LABEL, m_pBiotop->getLabel());
    pElement->SetAttribute(XML_ATTR_SIZE_X, (int)m_pBiotop->getDimension().x);
    pElement->SetAttribute(XML_ATTR_SIZE_Y, (int)m_pBiotop->getDimension().y);
    pElement->SetAttribute(XML_ATTR_SIZE_LAYER, (int)m_pBiotop->getNbLayer());
    pElement->SetAttribute(XML_ATTR_BIO_TIME, convertBioTimeToCount(m_pBiotop->getBiotopTime()));
    nbSpeciesInFile++;
    pElement->SetAttribute(XML_ATTR_NB_SPECIES, (int)nbSpeciesInFile);
  }

  // Create new Records node
  TiXmlElement newNode(XML_NODE_RECORDS);
  pNode = pNodeBiotop->InsertEndChild(newNode);
  if ((pNode != NULL) && (pNode->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    // Set attributes
    pElement = (TiXmlElement*)pNode;
    pElement->SetAttribute(XML_ATTR_SPECIE, m_specieName);

    // Save record data
    for (index = 0; index < m_CurrentNumberRecords; index++)
    {
      TiXmlElement newPopulationNode(XML_NODE_POPULATION);
      pNodeChild = pNode->InsertEndChild(newPopulationNode);
      if (pNodeChild != NULL)
      {
        pElement = (TiXmlElement*)pNodeChild;
        pElement->SetAttribute(XML_ATTR_BIO_DAY, (int)m_tTimeStamps[index]);
        pElement->SetAttribute(XML_ATTR_RAW_DATA, buildStringDataFromGeoMapRecord(index));
      }
    }
  }
  return true;
}

size_t CGeoMapPopulation::getNumberSpeciesStoredInFile(string fileNameWithPath)
{
  int nbSpecies = 0;
  TiXmlDocument xmlDoc(fileNameWithPath);
  xmlDoc.LoadFile();
  TiXmlElement* pElement;

  TiXmlNode* pNodeBiotop = xmlDoc.FirstChild(XML_NODE_BIOTOP);
  if ((pNodeBiotop != NULL) && (pNodeBiotop->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeBiotop;
    pElement->QueryIntAttribute(XML_ATTR_NB_SPECIES, &nbSpecies);
  }

  return (size_t)nbSpecies;
}

bool CGeoMapPopulation::loadFromXmlFile(string fileNameWithPath, size_t indexOfRecordInFile)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  resu = xmlDoc.LoadFile();
  loadFromXmlFile(&xmlDoc, indexOfRecordInFile);
  return resu;
}

bool CGeoMapPopulation::loadFromXmlFile(TiXmlDocument* pXmlDoc, size_t indexOfRecordInFile)
{
  bool recordFound = false;
  TiXmlElement* pElement;
  TiXmlNode* pNode = NULL;
  TiXmlNode* pNodeRecord = NULL;

  TiXmlNode* pNodeBiotop = pXmlDoc->FirstChild(XML_NODE_BIOTOP);
  if ((pNodeBiotop != NULL) && (pNodeBiotop->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeBiotop;
    int sizeX, sizeY, nbLayer;
    string timeCountStr;

    pElement->QueryIntAttribute(XML_ATTR_SIZE_X, &sizeX);
    pElement->QueryIntAttribute(XML_ATTR_SIZE_Y, &sizeY);
    pElement->QueryIntAttribute(XML_ATTR_SIZE_LAYER, &nbLayer);
    pElement->QueryStringAttribute(XML_ATTR_BIO_TIME, &timeCountStr);

    if ((sizeX > m_pBiotop->getDimension().x) || (sizeY > m_pBiotop->getDimension().y) || (nbLayer > m_pBiotop->getNbLayer()))
    {
      return false;
    }

    // Record management
    string specieName;
    int dayOfRecord;
    string rawData;
    pNodeRecord = pNodeBiotop->FirstChild(XML_NODE_RECORDS);
    if (pNodeRecord != NULL)
    {
      for (size_t index = 0; index < indexOfRecordInFile; index++)
        pNodeRecord = pNodeRecord->NextSibling();

      if ((pNodeRecord->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNodeRecord->ValueStr() == XML_NODE_RECORDS))
      {
        pElement = (TiXmlElement*)pNodeRecord;
        if (pElement->QueryStringAttribute(XML_ATTR_SPECIE, &specieName) != TIXML_NO_ATTRIBUTE)
        {
          m_specieName = specieName;
          pNode = pNodeRecord->FirstChild(XML_NODE_POPULATION);
          while (pNode != NULL)
          {
            if ((pNode->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNode->ValueStr() == XML_NODE_POPULATION))
            {
              pElement = (TiXmlElement*)pNode;
              pElement->QueryIntAttribute(XML_ATTR_BIO_DAY, &dayOfRecord);
              if (pElement->QueryStringAttribute(XML_ATTR_RAW_DATA, &rawData) != TIXML_NO_ATTRIBUTE)
              {
                size_t indexRecord = GetTableIndex(dayOfRecord);
                buildGeoMapRecordFromStringData(indexRecord, rawData);
              }
            }
            pNode = pNode->NextSibling();
          }
        }
      }  
    }
  }
  return recordFound;
}

//===========================================================================
// private methods
//===========================================================================
size_t CGeoMapPopulation::GetTableIndex(size_t dayIndex)
{
  size_t i;
  // Check if dayIndex already exist
  for (i = 0; i < m_CurrentNumberRecords; i++)
  {
    if (dayIndex == m_tTimeStamps[i])
      return i;
  }
  // Check if there are free id in table
  for (i = 0; i < m_nbRecords; i++)
  {
    if (m_tTimeStamps[i] == 0)
    {
      m_tTimeStamps[i] = dayIndex;
      m_CurrentNumberRecords++;
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
  const BiotopFoundIdsList& tFoundIds = biotopFoundIds.tFoundIds;
  for (size_t i = 0; i < biotopFoundIds.nbFoundIds; i++)
  {
    if (tFoundIds[i].pEntity->getSpecieName() == specieName)
    {
      countEntities++;  
    }
  }
  return countEntities;
}