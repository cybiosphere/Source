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
// FILE: CChromosome.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a set of basic genes
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CChromosome.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CChromosome::CChromosome(int number)
//  
// DESCRIPTION:  Default constructor
// 
// ARGUMENTS:    int number: Id of the chromosome
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CChromosome::CChromosome(size_t number)
{
  m_IdNumber = number;
  m_tGene.resize(0);
  m_ChromosomeType = CHROMOSOME_NEUTRAL;
}

//---------------------------------------------------------------------------
// METHOD:       CChromosome::CChromosome(CChromosome& model)
//  
// DESCRIPTION:  Copy constructor
// 
// ARGUMENTS:    CChromosome& model
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CChromosome::CChromosome(CChromosome& model)
{
  m_IdNumber       = model.m_IdNumber;
  m_ChromosomeType = model.m_ChromosomeType;
  CGene* tempGen   = NULL;
  for (size_t i=0; i<model.m_tGene.size(); i++)
  {
    tempGen = new CGene(*model.m_tGene[i]);
    m_tGene.push_back(tempGen);
  }
}

CChromosome::~CChromosome()
{
  deleteAllGenes();
}


//===========================================================================
// Gene table mgt
//===========================================================================

size_t CChromosome::addGene()
{
  CGene* pGene = new CGene();
  if (pGene != NULL)
  {
    m_tGene.push_back(pGene);
    return (m_tGene.size()-1);
  }
  else
    return (invalidIndex);
}

bool CChromosome::removeGeneFromIndex(size_t index)
{
  if (index>(m_tGene.size()-1))
    return false;

  m_tGene.erase(m_tGene.begin()+index);
  return true;
}

bool CChromosome::removeGene(CGene* pGene)
{
  for (size_t i = 0; i < m_tGene.size(); i++)
  {
    if (pGene == m_tGene[i])
    {
      return removeGeneFromIndex(i);
    }
  }
  return false;
}

size_t CChromosome::getNumGene(void)
{
  return (m_tGene.size());
}

CGene* CChromosome::getGene(size_t id)
{
  if (id>=m_tGene.size())  {
    return (NULL);
  }
  else
  {
    return (m_tGene[id]);
  }
}

void CChromosome::deleteAllGenes() 
{
  for (size_t i = 0; i < m_tGene.size(); i++)
  {
      delete m_tGene[i];
  }
  m_tGene.clear();
} 

//===========================================================================
// Raw data conversion
//===========================================================================

string CChromosome::buildStringDataFromGenes()
{
  string rawData = "";
  CGene* pGene = NULL;

  for (size_t i=0;i<m_tGene.size();i++)
  {
    pGene = m_tGene[i];
    rawData = rawData + pGene->buildStringDataFromGene();
  }

  return (rawData);
}

bool CChromosome::buildGenesFromStringData(string rawData)
{
  if (m_tGene.size() > 0)
  {
    return (false);
  }
  size_t geneIndex;
  CGene* pGene = NULL;
  string tmpStr;
  int geneType;    // ! sscanf needs int as argument, do not use BYTE
  int geneSubType; // ! sscanf needs int as argument, do not use BYTE
  int dataLength;  // ! sscanf needs int as argument, do not use BYTE
  int offset = 0;

  size_t remainingLen = rawData.length();
  while (remainingLen>0)
  { 
    tmpStr = rawData.substr(offset,10);
    sscanf(tmpStr.c_str(),"%02X%04X%04X",&geneType,&geneSubType,&dataLength);  
    tmpStr = rawData.substr(offset, 2*dataLength + 10);
    geneIndex = addGene();
    if (geneIndex != invalidIndex)
    {
      pGene = getGene(geneIndex);
      pGene->buildGeneFromStringData(tmpStr);
    }
    remainingLen -= (2*dataLength + 10);
    offset += (2*dataLength + 10);
  }

  return (true);
}

//===========================================================================
// Genetic
//===========================================================================

bool CChromosome::tryMutation(int rate)
{
  bool resu = false;
  for (size_t i=0; i<m_tGene.size(); i++)
  {
    if (testChance(rate))
    {
      if (m_tGene[i]->tryMutation())
      {
        // At least 1 mute
        resu = true;
      }
    }
  }
  return (resu);
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

size_t CChromosome::getIdNumber(void)
{
  return m_IdNumber;
}

string CChromosome::getLabel(void)
{
  string label;
  switch (m_ChromosomeType)
  {
  case CHROMOSOME_SEX_MALE:
    label = FormatString("%2d Y", m_IdNumber); // TBD: rq may be W/Z for birds
    break;
  case CHROMOSOME_SEX_FEMALE:
    label = FormatString("%2d X", m_IdNumber); // TBD: rq may be W/Z for birds
    break;
  default:
    label = FormatString("%2d", m_IdNumber);
    break;
  }
  return label;
}

void CChromosome::setChromosomeType(ChromosomeType_e newType)
{
  m_ChromosomeType = newType;
}

ChromosomeType_e CChromosome::getChromosomeType()
{
  return(m_ChromosomeType);
}




