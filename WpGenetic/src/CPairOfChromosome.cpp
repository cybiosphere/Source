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
{
  m_IdNumber = number;
  m_pPaterChromosome = new CChromosome(number);
  m_pMaterChromosome = new CChromosome(number);
}

CPairOfChromosome::CPairOfChromosome(CPairOfChromosome& model)
{
  // inherite both chromosome from model (clonage)
  m_IdNumber = model.m_IdNumber;
  m_pMaterChromosome = new CChromosome(*model.m_pMaterChromosome);
  m_pPaterChromosome = new CChromosome(*model.m_pPaterChromosome);
}

CPairOfChromosome::CPairOfChromosome(CPairOfChromosome& mother, CPairOfChromosome& father, double crossoverRate)
{
  m_IdNumber = mother.m_IdNumber;

  // inherite 1 chromosome from mother
  if ((mother.m_pMaterChromosome->getChromosomeType() == CHROMOSOME_NEUTRAL) && testChance(crossoverRate))
  {
    string crossedStr;
    m_pMaterChromosome = new CChromosome(m_IdNumber);
    getCrossedChromosomeStr(mother, crossedStr);
    CYBIOCORE_LOG("                 GENETIC- Crossover on mother pair %u\n", m_IdNumber);
    m_pMaterChromosome->buildGenesFromStringData(crossedStr);
  }
  else if (testChance(50.0))
  {
    m_pMaterChromosome = new CChromosome(*mother.m_pMaterChromosome);
  }
  else
  {
    m_pMaterChromosome = new CChromosome(*mother.m_pPaterChromosome);
  }

  // inherite 1 chromosome from father
  if ((father.m_pMaterChromosome->getChromosomeType() == CHROMOSOME_NEUTRAL) && testChance(crossoverRate))
  {
    string crossedStr;
    m_pPaterChromosome = new CChromosome(m_IdNumber);
    getCrossedChromosomeStr(father, crossedStr);
    CYBIOCORE_LOG("                 GENETIC- Crossover on father pair %u\n", m_IdNumber);
    m_pPaterChromosome->buildGenesFromStringData(crossedStr);
  }
  else if (testChance(50.0))
  {
    m_pPaterChromosome = new CChromosome(*father.m_pMaterChromosome);
  }
  else
  {
    m_pPaterChromosome = new CChromosome(*father.m_pPaterChromosome);
  }
}

CPairOfChromosome::~CPairOfChromosome()
{
  if (m_pPaterChromosome!=NULL)
    delete m_pPaterChromosome;

  if (m_pMaterChromosome!=NULL)
    delete m_pMaterChromosome;
}


///////////////////////////////////////
// methods

CChromosome* CPairOfChromosome::getPaterChromosome ()
{
  return (m_pPaterChromosome);
}

CChromosome* CPairOfChromosome::getMaterChromosome ()
{
  return (m_pMaterChromosome);
}

size_t CPairOfChromosome::getNumGenes()
{
  size_t materNumGen = 0;
  size_t paterNumGen = 0;

  if (m_pMaterChromosome!=NULL)
     materNumGen = m_pMaterChromosome->getNumGene();
  if (m_pPaterChromosome!=NULL)
     paterNumGen = m_pPaterChromosome->getNumGene();

  if (paterNumGen>materNumGen)
    return (paterNumGen);
  else
    return (materNumGen);
}

CGene* CPairOfChromosome::getDominantAllele(size_t index)
{
  CGene* pResu = NULL;
  CGene* pMaterGene = NULL;
  CGene* pPaterGene = NULL;

  if (m_pMaterChromosome!=NULL)
     pMaterGene = m_pMaterChromosome->getGene(index);
  if (m_pPaterChromosome!=NULL)
     pPaterGene = m_pPaterChromosome->getGene(index);

  if ((pMaterGene == NULL)&&(pPaterGene == NULL))
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
  else if (m_pPaterChromosome->getChromosomeType() == CHROMOSOME_SEX_MALE) // Rule: sexual male chromo always dominant
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

  return (pResu);
}

bool CPairOfChromosome::tryMutation(int rate)
{
  if ((m_pPaterChromosome->tryMutation(rate))||(m_pMaterChromosome->tryMutation(rate)))
    return (true);
  else
    return (false);
}

bool CPairOfChromosome::setAsSexualMale(void)
{
  if (m_pMaterChromosome!=NULL)
    m_pMaterChromosome->setChromosomeType(CHROMOSOME_SEX_FEMALE);
  if (m_pPaterChromosome!=NULL)
    m_pPaterChromosome->setChromosomeType(CHROMOSOME_SEX_MALE);
  return (true);
}

bool CPairOfChromosome::setAsSexualFemale(void)
{
  if (m_pMaterChromosome!=NULL)
    m_pMaterChromosome->setChromosomeType(CHROMOSOME_SEX_FEMALE);
  if (m_pPaterChromosome!=NULL)
    m_pPaterChromosome->setChromosomeType(CHROMOSOME_SEX_FEMALE);
  return (true);
}

bool CPairOfChromosome::setAsNeutral(void)
{
  if (m_pMaterChromosome!=NULL)
    m_pMaterChromosome->setChromosomeType(CHROMOSOME_NEUTRAL);
  if (m_pPaterChromosome!=NULL)
    m_pPaterChromosome->setChromosomeType(CHROMOSOME_NEUTRAL);
  return (true);
}

ChromosomeType_e CPairOfChromosome::getSex(void)
{
  return (m_pPaterChromosome->getChromosomeType());
}

size_t CPairOfChromosome::getIdNumber(void)
{
    return m_IdNumber;
}

bool CPairOfChromosome::getCrossedChromosomeStr(CPairOfChromosome& pPair, string& crossedStr)
{
  string motherStr;
  string fatherStr;

  motherStr = pPair.m_pMaterChromosome->buildStringDataFromGenes();
  fatherStr = pPair.m_pPaterChromosome->buildStringDataFromGenes();

  size_t cutIndex = getRandInt(motherStr.length());

  crossedStr = motherStr.substr(0,cutIndex) + fatherStr.substr(cutIndex);

  return (true);
}