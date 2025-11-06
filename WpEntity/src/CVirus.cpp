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
// FILE: CVirus.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a virus entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CVirus.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVirus::CVirus
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    string label : String containing label
//               Point_t initCoord, int layer : Born coordinate
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CVirus::CVirus(string label, CGenome* pGenome)
{
  // Input values 
  m_pGenome = pGenome;
  m_pBrain = NULL;

  // Default values   
  m_Status = STATUS_STATIC;
  m_Label = label;
  m_Generation = 0;
  m_TotalChildNumber = 0;

  initVirusParamIds();
}

void CVirus::initVirusParamIds()
{
  m_id_ReproductionRate = invalidCharIndex;
  m_id_ReproductionRange = invalidCharIndex;
}

//---------------------------------------------------------------------------
// METHOD:       CVirus::setParamFromGene 
//  
// DESCRIPTION:  Set parameter according to genome.
//               Automatically called by setEntityFromGenome for each genes
// 
// ARGUMENTS:    CGene* pGen : 1 gene from genome
//   
// RETURN VALUE: bool : false if gene not a CVegetable parameter
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CVirus::setParamFromGene(CGene* pGen)
{
  if (CBasicEntity::setParamFromGene(pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return true;
  }

  if ((pGen == NULL) || (pGen->getGeneType() != GENE_PARAMETER))
  {
    return false;
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  size_t len = rawData.size();
  if (len < 3 * sizeof(WORD))
  {
    // not enought data to config param
    return false;
  }

  switch (pGen->getGeneSubType())
  {
  case GENE_PARAM_REPRO_RATE:
  {
    if (m_id_ReproductionRate != invalidCharIndex) delete(getParameterNoCheck(m_id_ReproductionRate)); // delete if already set
    m_id_ReproductionRate = addParameterFromGene(pGen, PARAM_REPRODUCTION);
    resu = true;
    break;
  }
  case GENE_PARAM_REPRO_RANGE:
  {
    if (m_id_ReproductionRange != invalidCharIndex) delete(getParameterNoCheck(m_id_ReproductionRange)); // delete if already set
    m_id_ReproductionRange = addParameterFromGene(pGen, PARAM_REPRODUCTION);
    resu = true;
    break;
  }
  default:
  {
    // Unknown
    resu = false;
    break;
  }
  }
  // If resu is false, param is not valid for CVegetable, but it may be used by inherited class !
  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CVirus::completeParamsWithDefault 
//  
// DESCRIPTION:  Complete parameters unset by genome with default values.
//               Automatically called by setEntityFromGenome
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool : false if an error occured...
//  
// REMARKS:      Should be called by all derived method but not elsewhere
//---------------------------------------------------------------------------
bool CVirus::completeParamsWithDefault()
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value

  // In base class
  CBasicEntity::completeParamsWithDefault();

  // CVegetable specific
  if (m_id_ReproductionRate == invalidCharIndex)
  {
    m_id_ReproductionRate = addParameterFromGeneDefinition(PARAM_REPRODUCTION, GENE_PARAM_REPRO_RATE);
  }
  if (m_id_ReproductionRange == invalidCharIndex)
  {
    m_id_ReproductionRange = addParameterFromGeneDefinition(PARAM_REPRODUCTION, GENE_PARAM_REPRO_RANGE);
  }

  return true;
}

string CVirus::buildLifeStageString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return defStr;
}

double CVirus::getReproductionRate()
{
  return getParameterNoCheck(m_id_ReproductionRate)->getVal();
}

double CVirus::getReproductionRange()
{
  return getParameterNoCheck(m_id_ReproductionRange)->getVal();
}
