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
// FILE: CMeasurePopulation.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a population metric
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/01/2007 : Creation
//
//===========================================================================

#include "CMeasurePopulation.h"
#include "CBiotop.h"

//===========================================================================
// Definitions            
//===========================================================================

const char* MeasurePopulationTypeNameList[MEASURE_POPULATION_NUMBER_TYPE] =
{
  "Total",
  "Animals",
  "Vegetals",
  "Minerals",
  "Specific",
  "Specific biomass",
  "Marked entities"
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

CMeasurePopulation::CMeasurePopulation(CBiotop* pBiotop, int period, int id, MeasurePopulationType_e type, int maxVal, string SpecieName)
:CMeasure(period, id, 0, maxVal, MEASURE_TYPE_POPULATION)
{
  m_pBiotop = pBiotop;
  m_SubType = type;
  m_SpecieName = SpecieName;

  switch(m_SubType)
  {
  case  MEASURE_POPULATION_TOTAL:
    m_Label = "Population total"; 
    break;
  case  MEASURE_POPULATION_ANIMAL:
    m_Label = "Population animal"; 
    break;
  case  MEASURE_POPULATION_VEGETAL:
    m_Label = "Population vegetal"; 
    break;
  case  MEASURE_POPULATION_MINERAL:
    m_Label = "Population mineral"; 
    break;
  case  MEASURE_POPULATION_SPECIFIC:
    m_Label = "Population : " + m_SpecieName;
    break;
  case  MEASURE_POPULATION_SPECIFIC_BIOMASS:
    m_Label = "Population biomass : " + m_SpecieName;
    break;
  case  MEASURE_POPULATION_MARKED_ENTITIES:
    m_Label = "Population marked entities";
    break;
  default:
    m_Label = "error";
    break;
  }

}

CMeasurePopulation::~CMeasurePopulation()
{
}

//===========================================================================
// Measurement management
//===========================================================================
double CMeasurePopulation::GetCurrentValue()
{
  if (m_pBiotop == NULL)
    return 0;

  size_t val =0;

  switch(m_SubType)
  {
  case  MEASURE_POPULATION_TOTAL:
    val = m_pBiotop->getNbOfEntities();
    break;
  case  MEASURE_POPULATION_ANIMAL:
    val = m_pBiotop->getNbOfAnimals();
    break;
  case  MEASURE_POPULATION_VEGETAL:
    val = m_pBiotop->getNbOfVegetals(); 
    break;
  case  MEASURE_POPULATION_MINERAL:
    val = m_pBiotop->getNbOfMinerals();
    break;
  case  MEASURE_POPULATION_SPECIFIC:
    val = m_pBiotop->getNbOfSpecieEntities(m_SpecieName);
    break;
  case  MEASURE_POPULATION_SPECIFIC_BIOMASS:
    val = cybio_round(m_pBiotop->getSpecieBiomass(m_SpecieName));
    break;
  case  MEASURE_POPULATION_MARKED_ENTITIES:
    val = m_pBiotop->getNbOfMarkedEntities();
    break;
  default:
    val = 0;
    break;
  }

  return (double)val;
}

//===========================================================================
// Raw data conversion
//===========================================================================
bool CMeasurePopulation::buildMeasureDataFromString(string dataString)
{
  bool resu = CMeasure::buildMeasureDataFromString(dataString);
  string label = dataString.substr(0, dataString.find("\n"));
  if ((label.find("Population : ") != string::npos) && (m_SubType == MEASURE_POPULATION_SPECIFIC))
  {
    // Update specie name
    m_SpecieName = label.substr(13);
    m_Label = "Population : " + m_SpecieName;
  }
  else if ((label.find("Population biomass : ") != string::npos) && (m_SubType == MEASURE_POPULATION_SPECIFIC_BIOMASS))
  {
    // Update specie name
    m_SpecieName = label.substr(21);
    m_Label = "Population biomass : " + m_SpecieName;
  }
  return resu;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

string CMeasurePopulation::GetMeasurePopulationTypeStrName(MeasurePopulationType_e type)
{
  string typeName;
  typeName = MeasurePopulationTypeNameList[type];
  return typeName;
}

size_t CMeasurePopulation::GetSubTypeId()
{
  return m_SubType;
}

string  CMeasurePopulation::GetSpecieName()
{
  return m_SpecieName;
}