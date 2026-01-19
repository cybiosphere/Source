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
// FILE: CPairOfChromosome.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a set of basic caracteristics of a creature
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CPairOfChromosome.h"

///////////////////////////////////////
// constructors

CPairOfChromosome::CPairOfChromosome(size_t number)
  : m_IdNumber(number)
  , m_PaterChromosome(number)
  , m_MaterChromosome(number)
{
}

CPairOfChromosome::CPairOfChromosome(CPairOfChromosome& model)
  : m_IdNumber(model.m_IdNumber)
  , m_PaterChromosome(model.m_PaterChromosome)
  , m_MaterChromosome(model.m_MaterChromosome)
{
  // copy constructor: members copied via their copy constructors
}

CPairOfChromosome::CPairOfChromosome(CPairOfChromosome& mother, CPairOfChromosome& father, double crossoverRate)
  : m_IdNumber(mother.m_IdNumber)
  , m_PaterChromosome(m_IdNumber)
  , m_MaterChromosome(m_IdNumber)
{
  // inherite 1 chromosome from mother
  if ((mother.m_MaterChromosome.getChromosomeType() == CHROMOSOME_NEUTRAL) && testChance(crossoverRate))
  {
    string crossedStr;
    getCrossedChromosomeStr(mother, crossedStr);
    CYBIOCORE_LOG_TIME_NOT_AVAILABLE;
    CYBIOCORE_LOG("GENETIC- Crossover on mother pair %u\n", m_IdNumber);
    m_MaterChromosome.buildGenesFromStringData(crossedStr);
  }
  else if (testChance(50.0))
  {
    m_MaterChromosome = mother.m_MaterChromosome;
  }
  else
  {
    m_MaterChromosome = mother.m_PaterChromosome;
  }

  // inherite 1 chromosome from father
  if ((father.m_MaterChromosome.getChromosomeType() == CHROMOSOME_NEUTRAL) && testChance(crossoverRate))
  {
    string crossedStr;
    getCrossedChromosomeStr(father, crossedStr);
    CYBIOCORE_LOG_TIME_NOT_AVAILABLE;
    CYBIOCORE_LOG("GENETIC- Crossover on father pair %u\n", m_IdNumber);
    m_PaterChromosome.buildGenesFromStringData(crossedStr);
  }
  else if (testChance(50.0))
  {
    m_PaterChromosome = father.m_MaterChromosome;
  }
  else
  {
    m_PaterChromosome = father.m_PaterChromosome;
  }
}

CPairOfChromosome::~CPairOfChromosome()
{
  // members are value objects now; no explicit delete required
}


///////////////////////////////////////
// methods

CChromosome& CPairOfChromosome::getPaterChromosome()
{
  return m_PaterChromosome;
}

CChromosome& CPairOfChromosome::getMaterChromosome()
{
  return m_MaterChromosome;
}

size_t CPairOfChromosome::getNumGenes()
{
  size_t materNumGen = m_MaterChromosome.getNumGene();
  size_t paterNumGen = m_PaterChromosome.getNumGene();

  return (paterNumGen > materNumGen) ? paterNumGen : materNumGen;
}

CGene* CPairOfChromosome::getDominantAllele(size_t index)
{
  CGene* pResu = NULL;
  CGene* pMaterGene = m_MaterChromosome.getGene(index);
  CGene* pPaterGene = m_PaterChromosome.getGene(index);

  if ((pMaterGene == NULL) && (pPaterGene == NULL))
  {
    pResu = NULL;
  }
  else if (pMaterGene == NULL)
  {
    pResu = pPaterGene;
  }
  else if (pPaterGene == NULL)
  {
    pResu = pMaterGene;
  }
  else if (m_PaterChromosome.getChromosomeType() == CHROMOSOME_SEX_MALE) // Rule: sexual male chromo always dominant
  {
    pResu = pPaterGene;
  }
  else
  {
    // Rq: No check on Gene compatibility.
    //     No codominance supported. Dominant allele has the smallest factor
    if (pPaterGene->getRecessiveFactor() < pMaterGene->getRecessiveFactor())
      pResu = pPaterGene;
    else
      pResu = pMaterGene;
  }

  return pResu;
}

bool CPairOfChromosome::tryMutation(int rate)
{
  if ((m_PaterChromosome.tryMutation(rate)) || (m_MaterChromosome.tryMutation(rate)))
    return true;
  else
    return false;
}

bool CPairOfChromosome::setAsSexualMale(void)
{
  m_MaterChromosome.setChromosomeType(CHROMOSOME_SEX_FEMALE);
  m_PaterChromosome.setChromosomeType(CHROMOSOME_SEX_MALE);
  return true;
}

bool CPairOfChromosome::setAsSexualFemale(void)
{
  m_MaterChromosome.setChromosomeType(CHROMOSOME_SEX_FEMALE);
  m_PaterChromosome.setChromosomeType(CHROMOSOME_SEX_FEMALE);
  return true;
}

bool CPairOfChromosome::setAsNeutral(void)
{
  m_MaterChromosome.setChromosomeType(CHROMOSOME_NEUTRAL);
  m_PaterChromosome.setChromosomeType(CHROMOSOME_NEUTRAL);
  return true;
}

ChromosomeType_e CPairOfChromosome::getSex(void)
{
  return (m_PaterChromosome.getChromosomeType());
}

size_t CPairOfChromosome::getIdNumber(void)
{
  return m_IdNumber;
}

bool CPairOfChromosome::getCrossedChromosomeStr(CPairOfChromosome& pPair, string& crossedStr)
{
  string motherStr;
  string fatherStr;

  motherStr = pPair.m_MaterChromosome.buildStringDataFromGenes();
  fatherStr = pPair.m_PaterChromosome.buildStringDataFromGenes();

  if (motherStr.size() != fatherStr.size())
  {
    CYBIOCORE_LOG_TIME_NOT_AVAILABLE;
    CYBIOCORE_LOG("GENETIC- ERROR getCrossedChromosomeStr: lenghtFather=%u lenghMother=%u\n", fatherStr.size(), motherStr.size());
    crossedStr = motherStr;
    return false;
  }

  size_t splitIndex = getRandInt(motherStr.length() - 2) + 1;
  crossedStr = motherStr.substr(0, splitIndex) + fatherStr.substr(splitIndex);

  return true;
}