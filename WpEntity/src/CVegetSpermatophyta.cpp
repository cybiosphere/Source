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
// FILE: CVegetSpermatophyta.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a a Spermatophyta entity
//         => a vegetable using flowers for reproduction
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CVegetSpermatophyta.h"
#include "CBiotop.h" 
#include "Definitions.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::CVegetSpermatophyta
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    string label : String containing label
//               Point_t initCoord, int layer : Born coordinate
//               CGenome* pGenome : Ptr on a valid Genome object
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CVegetSpermatophyta::CVegetSpermatophyta(string label, Point_t initCoord, size_t layer, CGenome* pGenome):
CVegetable(label, initCoord, layer, pGenome)
{
  // Default values          
  m_Status      = STATUS_ALIVE; 

  // Parameter id pre-init
  m_id_PollenRange      = invalidIndex;; 
}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::CVegetSpermatophyta
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CVegetSpermatophyta& model : Clonage reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CVegetSpermatophyta::CVegetSpermatophyta(string label, CVegetSpermatophyta& model):
CVegetable(label, model)
{ 
  // Default values          
  m_Status      = STATUS_ALIVE; 

  // Parameter id pre-init
  m_id_PollenRange      = invalidIndex;; 

}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::CVegetSpermatophyta
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CVegetSpermatophyta& mother,father : Parents for sexual reproduction
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//--------------------------------------------------------------------------- 
CVegetSpermatophyta::CVegetSpermatophyta(string label, CVegetSpermatophyta& mother,CVegetSpermatophyta& father):
CVegetable(label, mother, father)
{ 
  // Default values          
  m_Status      = STATUS_ALIVE; 

  // Parameter id pre-init
  m_id_PollenRange      = invalidIndex;; 

}
  
//===========================================================================
// Genetic settings
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::setParamFromGene 
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
bool CVegetSpermatophyta::setParamFromGene (CGene* pGen)
{
  if (CVegetable::setParamFromGene (pGen) == true)
  {
    // The parameter has already been taken into account by basic entity
    return (true);
  }
  
  if ((pGen==NULL)||(pGen->getGeneType() != GENE_PARAMETER))
  {
    return (false);
  }
  // We are sure Gene is a parameter
  bool resu = false;
  auto rawData = pGen->getData();
  WORD* pData = (WORD*)rawData.data();
  size_t len = rawData.size();
  if (len<3*sizeof(WORD))
  {
    // not enought data to config param
    return (false);
  }

  switch(pGen->getGeneSubType())
  {
  case GENE_PARAM_POLLEN_RANGE:
    {
      if (m_id_PollenRange != invalidIndex) delete(getParameter(m_id_PollenRange)); // delete if already set
      m_id_PollenRange = addParameterFromGene(pGen, PARAM_REPRODUCTION);
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
  // If resu is false, param is not valid for CVegetSpermatophyta, but it may be used by inherited class !
  return (resu);
}

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::completeParamsWithDefault 
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
bool CVegetSpermatophyta::completeParamsWithDefault()
{
  // Check if all mandatory parameters were set by genome
  // If not, use default value

  // In base class
  CVegetable::completeParamsWithDefault();

  // CVegetable specific
  if (m_id_PollenRange == invalidIndex)
  {
    m_id_PollenRange = addParameterFromGeneDefinition(PARAM_REPRODUCTION, GENE_PARAM_POLLEN_RANGE);
  }

  return (true);
}

//===========================================================================
// Time management
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::nextHour
//  
// DESCRIPTION:  Secondary action: parameters update and checkup
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: None
//  
// REMARKS:      Global application speed impacted by this method
//---------------------------------------------------------------------------
void CVegetSpermatophyta::nextHour() 
{
  if (isAlive() && isLocalAutoControlled())
  {
    // reproduction
    // TBD: Check also if reproduction age is reached
    if (testChance(getReproductionRate(),m_pBiotop->getFertility(getGridCoord()) / 24.0))
    {
      if ((getTypeOfReproduction() == REPRODUCT_SEXUAL) && (getSex() == SEX_FEMALE))
      {
        CBasicEntity *pFoundEntity = NULL;
        const BiotopFoundIds_t & biotopFoundIds = getBiotop()->findEntities(getGridCoord(), (int)getReproductionRange());
        const std::vector<FoundEntity_t>& tFoundIds = biotopFoundIds.tFoundIds;
 
        for (size_t i = 0; i < biotopFoundIds.nbFoundIds; i++)
        {
          pFoundEntity = tFoundIds[i].pEntity;
          ASSERT (pFoundEntity!=NULL);
          if ( (m_pGenome->checkSpecieCompatibility(pFoundEntity->getGenome()) == true) 
             &&(pFoundEntity->getSex() == SEX_MALE) )
          {
            // TBD: Select 1 random compatible male among different parters
            reproductWith((CVegetSpermatophyta*)tFoundIds[i].pEntity);
            break;
          }
        }
      }
      else if (getTypeOfReproduction() == REPRODUCT_CLONAGE)
      {
        autoClone();
      }
    }
  }

  CVegetable::nextHour();
}
        
//===========================================================================
// Behavior  
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::reproductWith
//  
// DESCRIPTION:  Clone entity
// 
// ARGUMENTS:    CVegetSpermatophyta* partner : male entity to make child
//   
// RETURN VALUE: bool: Success
//  
// REMARKS:      None
//---------------------------------------------------------------------------

bool CVegetSpermatophyta::reproductWith(CVegetSpermatophyta* partner)
{
  string childLabel;
  childLabel = FormatString("j%d", m_TotalChildNumber);
  childLabel = m_Label + childLabel;
  CVegetSpermatophyta* pChildEntity = new CVegetSpermatophyta(childLabel,*this,*partner);
  m_TotalChildNumber++;
  // Caracter from genetic after mutation:
  pChildEntity->setEntityFromGenome(m_pBiotop->getRadioactivityRate());

  int range = (int)getReproductionRange();
  int xOfset = getRandInt(2*range) - range;
  int yOfset = getRandInt(2*range) - range;
  Point_t newCoord = {getGridCoord().x + xOfset, getGridCoord().y + yOfset};
  pChildEntity->changeWeight(0.1); // ensure weight is more than min
  entityIdType resuId = m_pBiotop->addEntity(pChildEntity, newCoord, getLayer());

  if (resuId == ENTITY_ID_INVALID)
  {
    delete pChildEntity;
  }
  else if (!pChildEntity->checkHabitat())
  {
    pChildEntity->autoRemove(); // Cannot growth in water
  }

  return (true);
}


//---------------------------------------------------------------------------
// METHOD:       CVegetSpermatophyta::autoClone
//  
// DESCRIPTION:  Clone entity
// 
// ARGUMENTS:    None
//   
// RETURN VALUE: bool: Success
//  
// REMARKS:      None
//---------------------------------------------------------------------------

bool CVegetSpermatophyta::autoClone()
{
  string childLabel;
  childLabel = m_Label;
  CVegetSpermatophyta* pChildEntity = new CVegetSpermatophyta(childLabel,*this);
  m_TotalChildNumber++;
  // Caracter from genetic after mutation:
  pChildEntity->setEntityFromGenome(m_pBiotop->getRadioactivityRate());

  int range = (int)getReproductionRange();
  int xOfset = getRandInt(2*range) - range;
  int yOfset = getRandInt(2*range) - range;
  Point_t newCoord = {getGridCoord().x + xOfset, getGridCoord().y + yOfset};
  pChildEntity->changeWeight(0.1); // ensure weight is more than min
  entityIdType resuId = m_pBiotop->addEntity(pChildEntity, newCoord, getLayer());

  if (resuId == ENTITY_ID_INVALID)
  {
    delete pChildEntity;
  }
  else if (!pChildEntity->checkHabitat())
  {
    pChildEntity->autoRemove(); // Cannot growth in water
  }

  return (true);
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

double CVegetSpermatophyta::getPollenRange()
{
  return (getParameter(m_id_PollenRange)->getVal());
}




