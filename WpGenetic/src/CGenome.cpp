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
// FILE: CGenome.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents whole genetic caracteristic of a creature
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CGenome.h"
#include <fstream>
#include "CBrain.h"

///////////////////////////////////////
// constructors

CGenome::CGenome(ClassType_e classType, string specieName)
{
  m_class = classType;
  m_SpecieName = specieName;
  m_SexualPairIndex = invalidIndex;
  m_bSexualDimorphism = false;
  m_tPair.resize(0);
}

CGenome::CGenome(CGenome& model)
{
  m_class = model.m_class;
  m_SpecieName = model.m_SpecieName;
  m_SexualPairIndex = model.m_SexualPairIndex;
  m_bSexualDimorphism = model.m_bSexualDimorphism;
  CPairOfChromosome* tempPair = NULL;
  for (size_t  i = 0; i < model.m_tPair.size(); i++)
  {
    tempPair = new CPairOfChromosome(*model.m_tPair[i]);
    m_tPair.push_back(tempPair);
  }
}

CGenome::CGenome(CGenome& mother, CGenome& father, double crossoverRate)
{
  m_class = mother.m_class;
  m_SpecieName = mother.m_SpecieName;
  m_SexualPairIndex = mother.m_SexualPairIndex;
  m_bSexualDimorphism = mother.m_bSexualDimorphism;
  CPairOfChromosome* tempPair = NULL;
  for (size_t i = 0; i < mother.m_tPair.size(); i++)
  {
    tempPair = new CPairOfChromosome(*mother.m_tPair[i], *father.m_tPair[i], crossoverRate);
    m_tPair.push_back(tempPair);
  }
}

CGenome::~CGenome()
{
  deleteAllPairs();
}


///////////////////////////////////////
// methods

size_t CGenome::addPair()
{
  size_t pairId = m_tPair.size();
  CPairOfChromosome* pPair = new CPairOfChromosome(pairId);
  if (pPair != NULL)
  {
    m_tPair.push_back(pPair);
    return (m_tPair.size() - 1);
  }
  else
    return (invalidIndex);
}

bool CGenome::removePairFromIndex(size_t index)
{
  if (index > (m_tPair.size() - 1))
    return false;

  m_tPair.erase(m_tPair.begin() + index);
  return true;
}

bool CGenome::removePair(CPairOfChromosome* pPaire)
{
  for (size_t i = 0; i < m_tPair.size(); i++)
  {
    if (pPaire == m_tPair[i])
    {
      return removePairFromIndex(i);
    }
  }
  return false;
}

size_t CGenome::getNumPair()
{
  return (m_tPair.size());
}

CPairOfChromosome* CGenome::getPair(size_t id)
{
  if (id > m_tPair.size())
  {
    return NULL;
  }
  else
  {
    return (m_tPair[id]);
  }
}

CPairOfChromosome* CGenome::getPairSexual()
{
  if (m_SexualPairIndex >= m_tPair.size())
  {
    return NULL;
  }
  else
  {
    return (m_tPair[m_SexualPairIndex]);
  }
}

CPairOfChromosome* CGenome::getPairBrain()
{
  if (m_SexualPairIndex>m_tPair.size())
  {
    return NULL;
  }
  else
  {
    return (m_tPair[(m_tPair.size() - 1)]); // return last pair
  }
}

void CGenome::deleteAllPairs() 
{
  for (size_t i = 0; i < m_tPair.size(); i++)
  {
    if (m_tPair[i] != NULL)
      delete (m_tPair[i]);   
  }
  m_tPair.clear();
} 

bool CGenome::setSexMale(size_t indexPairSex, bool sexualDimorph)
{
  if (indexPairSex>m_tPair.size())
  {
    return false;
  }

  if (m_SexualPairIndex != invalidIndex)
  {
    m_tPair[m_SexualPairIndex]->setAsNeutral();
  }

  bool resu = m_tPair[indexPairSex]->setAsSexualMale();
  if (resu)
    m_SexualPairIndex = indexPairSex;
  else
    m_SexualPairIndex = invalidIndex;

  m_bSexualDimorphism = sexualDimorph;

  return resu;
}

bool CGenome::setSexFemale(size_t indexPairSex, bool sexualDimorph)
{
  if (indexPairSex>m_tPair.size())
  {
    return false;
  }

  if (m_SexualPairIndex != invalidIndex)
  {
    m_tPair[m_SexualPairIndex]->setAsNeutral();
  }

  bool resu = m_tPair[indexPairSex]->setAsSexualFemale();
  if (resu)
    m_SexualPairIndex = indexPairSex;
  else
    m_SexualPairIndex = invalidIndex;

  m_bSexualDimorphism = sexualDimorph;

  return resu;
}

bool CGenome::setSexNeutral(size_t indexPairSex)
{
  if (indexPairSex>m_tPair.size())
  {
    return false;
  }

  bool resu = m_tPair[indexPairSex]->setAsNeutral();
  m_SexualPairIndex = invalidIndex;

  return resu;
}

SexType_e CGenome::getSexType()
{
  if (m_SexualPairIndex == invalidIndex)
  {
    return SEX_NONE;
  }
  
  ChromosomeType_e chromoType = m_tPair[m_SexualPairIndex]->getSex();
  switch (chromoType)
  {
  case CHROMOSOME_SEX_MALE:
    return SEX_MALE;
    break;
  case CHROMOSOME_SEX_FEMALE:
    return SEX_FEMALE;
    break;
  default:
    return SEX_NONE;
    break;
  }
}

bool CGenome::tryMutation(int rate)
{
  bool resu = false;
  for (size_t i = 0; i < m_tPair.size(); i++)
  {
    if (m_tPair[i]->tryMutation(rate))
    {
      // At least 1 mute
      resu = true;
    }
  }
  return resu;
}

ClassType_e CGenome::getClass()
{
  return m_class;
}

string CGenome::getSpecieName()
{
  return m_SpecieName;
}

string CGenome::getOwnClassStrName()
{
  return (getClassStrName(m_class));
}

string CGenome::getClassStrName(ClassType_e classType)
{
  string classeName;

  switch(classType)
  {
    case CLASS_UNSET:
      classeName = "Not set";
      break;
    case CLASS_NONE:
      classeName = "Unknown";
      break;
    case CLASS_SOLID:
      classeName = "Solid";
      break;
    case CLASS_WATER:
      classeName = "Water";
      break;
    case CLASS_PHYCOPHYTA:
      classeName = "Phycophyta";
      break;
    case CLASS_MYCOPHYTA:
      classeName = "Mycophyta";
      break;
    case CLASS_BRYOPHYTA:
      classeName = "Bryophyta";
      break;
    case CLASS_PTERIDOPHYTA:
      classeName = "Pteridophyta";
      break;
    case CLASS_SPERMATOPHYTA:
      classeName = "Spermatophyta";
      break;
    case CLASS_MAMMAL:
      classeName = "Mammal";
      break;
    case CLASS_REPTILE:
      classeName = "Reptile";
      break;
    case CLASS_ARTHROPOD:
      classeName = "Arthropod";
      break;
    case CLASS_AVE:
      classeName = "Ave";
      break;
    case CLASS_VIRUS:
      classeName = "Virus";
      break;
    default:
      classeName = "Bad Class value";
      break;
  }

  return classeName;
}


specieSignatureType CGenome::getSpecieSignature()
{
  // Specie is determine by class, nb of pair, .... 
  // TBD : complete Specie signature
  //return (1000000*m_class + 10000*m_tPair.size());
  
  specieSignatureType signature = 1000000*m_class;
  for (size_t i = 0; i < m_tPair.size(); i++)
  {
    signature += (specieSignatureType)(m_tPair[i]->getMaterChromosome()->getNumGene() * (7 * i + 1));
  }
  return signature;

}

bool CGenome::checkSpecieCompatibility(CGenome* pOther)
{
  bool resu = false;
  if (pOther == NULL)
  {
    resu = false;
  }
  else if (fabs((double)getSpecieSignature() - (double)pOther->getSpecieSignature())<2)
  {
    resu = true;
  }
  else
  {
    resu = false;
  }
  return resu;
}


bool CGenome::saveInXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  xmlDoc.LoadFile();
  saveInXmlFile(&xmlDoc);
  resu = xmlDoc.SaveFile();
  return resu;
}

bool CGenome::saveInXmlFile(TiXmlDocument* pXmlDoc)
{
  TiXmlNode* pNodeEntity = NULL;

  if (pXmlDoc == NULL)
    return false;

  pNodeEntity = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if (pNodeEntity == NULL)
  {
    TiXmlElement newNode(XML_NODE_ENTITY);
    pNodeEntity = pXmlDoc->InsertEndChild(newNode);
  }

  return saveInXmlNode(pNodeEntity);
}

bool CGenome::saveInXmlNode(TiXmlNode * pNodeEntity)
{
  TiXmlElement* pElement;
  TiXmlNode* pNodeGenome = NULL;
  TiXmlNode* pNodePair = NULL;
  TiXmlNode* pNodeGene = NULL;

  if (pNodeEntity == NULL)
  {
    return false;
  }

  pNodeGenome = pNodeEntity->FirstChild(XML_NODE_GENOME);
  if (pNodeGenome == NULL)
  {
    // Create Genome node
    TiXmlElement newNode(XML_NODE_GENOME);
    pNodeGenome = pNodeEntity->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous Genome
    while (pNodeGenome->FirstChild() != NULL)
      pNodeGenome->RemoveChild(pNodeGenome->FirstChild());
  }

  // Set attributes
  if ((pNodeGenome != NULL) && (pNodeGenome->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    pElement = (TiXmlElement*)pNodeGenome;
    pElement->SetAttribute( XML_ATTR_CLASS_NAME, getOwnClassStrName());
    pElement->SetAttribute( XML_ATTR_CLASS_ID, (int)m_class);
    pElement->SetAttribute( XML_ATTR_SPECIE, m_SpecieName);
    pElement->SetAttribute( XML_ATTR_DIMORPHISM, (int)m_bSexualDimorphism);
  }

  size_t i,j;
  CChromosome* pCurChroM = NULL;
  CChromosome* pCurChroP = NULL;
  for (i = 0; i < getNumPair(); i++)
  {
    TiXmlElement newNodePair(XML_NODE_PAIR);
    pNodePair = pNodeGenome->InsertEndChild(newNodePair);

    if ((pNodePair != NULL) && (pNodePair->Type() == TiXmlNode::TINYXML_ELEMENT))
    {
      // Set attributes
      pElement = (TiXmlElement*)pNodePair;
      pElement->SetAttribute( XML_ATTR_TYPE, (int)getPair(i)->getSex());

      pCurChroM = getPair(i)->getMaterChromosome();
      pCurChroP = getPair(i)->getPaterChromosome();

      for (j = 0; j < pCurChroM->getNumGene(); j++)
      {
        TiXmlElement newNodeGeneM(XML_NODE_GENE);
        pNodeGene = pNodePair->InsertEndChild(newNodeGeneM);
        if (pNodeGene != NULL) 
        {
          pElement = (TiXmlElement*)pNodeGene;
          pElement->SetAttribute( XML_ATTR_GENE_DATA_M, pCurChroM->getGene(j)->buildStringDataFromGene());
          pElement->SetAttribute( XML_ATTR_GENE_DATA_P, pCurChroP->getGene(j)->buildStringDataFromGene());
        }
      }
    }
  }

  return true;
}

bool CGenome::loadFromXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  resu = xmlDoc.LoadFile();
  loadFromXmlFile(&xmlDoc);
  return resu;
}

bool CGenome::loadFromXmlFile(TiXmlDocument* pXmlDoc)
{
  if (pXmlDoc == NULL)
    return false;

  return loadFromXmlNode(pXmlDoc->FirstChild(XML_NODE_ENTITY));
}

bool CGenome::loadFromXmlNode(TiXmlNode* pNodeEntity)
{
  TiXmlElement* pElement;
  TiXmlNode* pNodeGenome = NULL;
  TiXmlNode* pNodePair = NULL;
  TiXmlNode* pNodeGene = NULL;
  bool resu = false;

  if (pNodeEntity != NULL)
    pNodeGenome = pNodeEntity->FirstChild(XML_NODE_GENOME);

  if ((pNodeGenome != NULL) && (pNodeGenome->Type() == TiXmlNode::TINYXML_ELEMENT))
  {
    resu = true;
    pElement = (TiXmlElement*)pNodeGenome;
    int classType, dimorphism;
    string specieName;

    if ( pElement->QueryStringAttribute(XML_ATTR_SPECIE, &specieName) == TIXML_NO_ATTRIBUTE)
      specieName = "Unset";

    if ( pElement->QueryIntAttribute(XML_ATTR_CLASS_ID, &classType) == TIXML_NO_ATTRIBUTE)
      classType = 0;

    if ( pElement->QueryIntAttribute(XML_ATTR_DIMORPHISM, &dimorphism) == TIXML_NO_ATTRIBUTE)
      dimorphism = 0;

    // Clear previous genome
    deleteAllPairs();
    m_tPair.resize(0); 
    m_SexualPairIndex = invalidIndex;
    m_class = (ClassType_e)classType;
    m_SpecieName = specieName;
    m_bSexualDimorphism = (bool)dimorphism;

    string geneStrM;
    string geneStrP;
    size_t indexPair;
    int type;
    size_t geneIndex;
    CGene* pGene;
    CChromosome* pCurChro;

    pNodePair = pNodeGenome->FirstChild();
    while (pNodePair != NULL)
    {
      if ((pNodePair->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNodePair->ValueStr() == XML_NODE_PAIR))
      {
        // Add a pair of chromosomes
        indexPair = addPair();

        pElement = (TiXmlElement*)pNodePair;
        if ( pElement->QueryIntAttribute(XML_ATTR_TYPE,  &type) != TIXML_NO_ATTRIBUTE)
        {
          switch(type)
          {
          case CHROMOSOME_SEX_MALE:
            setSexMale(indexPair);
            break;
          case CHROMOSOME_SEX_FEMALE:
            setSexFemale(indexPair);
            break;
          default:
            getPair(indexPair)->setAsNeutral();
            break;
          }
        }

        pNodeGene = pNodePair->FirstChild();
        while (pNodeGene != NULL)
        {
          if ((pNodeGene->Type() == TiXmlNode::TINYXML_ELEMENT) && (pNodeGene->ValueStr() == XML_NODE_GENE))
          {
            pElement = (TiXmlElement*)pNodeGene;
            if ( pElement->QueryStringAttribute(XML_ATTR_GENE_DATA_M, &geneStrM) != TIXML_NO_ATTRIBUTE)
            {
              pCurChro = getPair(indexPair)->getMaterChromosome();
              geneIndex = pCurChro->addGene();
              pGene = pCurChro->getGene(geneIndex);
              pGene->buildGeneFromStringData(geneStrM);
            }
            if ( pElement->QueryStringAttribute(XML_ATTR_GENE_DATA_P, &geneStrP) != TIXML_NO_ATTRIBUTE)
            {
              pCurChro = getPair(indexPair)->getPaterChromosome();
              geneIndex = pCurChro->addGene();
              pGene = pCurChro->getGene(geneIndex);
              pGene->buildGeneFromStringData(geneStrP);
            }
          }
          pNodeGene = pNodeGene->NextSibling();
        }
      }
      pNodePair = pNodePair->NextSibling();
    }
  }

  return resu;
}

//---------------------------------------------------------------------------
// METHOD:       CGenome::setBrainInstinctInGenes 
//  
// DESCRIPTION:  Build Brain Instinct from current Brain (aquired kwnoledge genetic transmition)
// 
// ARGUMENTS:    
//   
// RETURN VALUE: bool : error
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CGenome::setBrainInstinctInGenes(CBrain* pBrain)
{
  if (pBrain == NULL)
    return false;

  WORD* pData = NULL;
  int lineSize;
  size_t indexGen;
  CGene *pGen1M, *pGen1P;
  size_t i;
  
  // clean previous Brain instinct genes (decision tables)
  int numPairs = (int)getNumPair();
  for (int pairId = (numPairs - 1); pairId > -1; pairId--)
  {
    CPairOfChromosome* pCurPair = getPair(pairId);
    CChromosome* pCurChromoM = pCurPair->getMaterChromosome();
    CChromosome* pCurChromoP = pCurPair->getPaterChromosome();

    int numGenes = (int)pCurChromoM->getNumGene();
    for (int geneId = (numGenes - 1); geneId > -1; geneId--)
    {
      if (pCurChromoM->getGene(geneId)->getGeneSubType() == GENE_BRAIN_LINE)
      {
        pCurChromoM->removeGeneFromIndex(geneId);
        pCurChromoP->removeGeneFromIndex(geneId);
      }
    }
  }
 
  CPairOfChromosome* pPaireBrain = getPairBrain();
  CPairOfChromosome* pPaireSex   = getPairSexual();

  CChromosome* pChromoBrainM = pPaireBrain->getMaterChromosome();
  CChromosome* pChromoBrainP = pPaireBrain->getPaterChromosome();

  CChromosome* pChromoSexM = pPaireSex->getMaterChromosome();
  CChromosome* pChromoSexP = pPaireSex->getPaterChromosome();

  // Store Decision table
  for (i = 0; i < pBrain->GetDecisionNeuronTable()->GetNeuronTableRowCount(); i++)
  {
    if (pBrain->IsDecisionRowSexSpecific(i))
    {
      indexGen = pChromoSexM->addGene();
      pGen1M = pChromoSexM->getGene(indexGen);
      indexGen = pChromoSexP->addGene();
      pGen1P = pChromoSexP->getGene(indexGen);
    }
    else
    {
      indexGen = pChromoBrainM->addGene();
      pGen1M = pChromoBrainM->getGene(indexGen);
      indexGen = pChromoBrainP->addGene();
      pGen1P = pChromoBrainP->getGene(indexGen);
    }

    lineSize = pBrain->GetDecisionNeuronTable()->buildRawDataFromNeuronLine(i,pData);
    pGen1M->setAsBrainInstinctLine(10,i,lineSize,pData);
    pGen1P->setAsBrainInstinctLine(10,i,lineSize,pData);
    if (pData)
      delete [] pData;
  }

  return true;
}

//---------------------------------------------------------------------------
// METHOD:       CGenome::setBrainIdentifyInGenes 
//  
// DESCRIPTION:  Build Brain Instinct from current Brain (aquired kwnoledge genetic transmition)
// 
// ARGUMENTS:    
//   
// RETURN VALUE: bool : error
//  
// REMARKS:      
//---------------------------------------------------------------------------
bool CGenome::setBrainIdentifyInGenes(CBrain* pBrain)
{
  if (pBrain == NULL)
    return false;

  WORD* pData = NULL;
  size_t lineSize;
  size_t indexGen;
  CGene *pGen1M, *pGen1P;
  size_t i;
  
  // clean previous Brain instinct genes (both identification and decision tables)
  int numPairs = (int)getNumPair();
  for (int pairId = (numPairs - 1); pairId > -1; pairId--)
  {
    CPairOfChromosome* pCurPair = getPair(pairId);
    CChromosome* pCurChromoM = pCurPair->getMaterChromosome();
    CChromosome* pCurChromoP = pCurPair->getPaterChromosome();

    int numGenes = (int)pCurChromoM->getNumGene();
    for (int geneId = (numGenes - 1); geneId > -1; geneId--)
    {
      if (pCurChromoM->getGene(geneId)->getGeneSubType() == GENE_BRAIN_IDENTIFY_LINE)
      {
        pCurChromoM->removeGeneFromIndex(geneId);
        pCurChromoP->removeGeneFromIndex(geneId);
      }
    }
  }
 
  CPairOfChromosome* pPaireBrain = getPairBrain();
  CPairOfChromosome* pPaireSex   = getPairSexual();

  CChromosome* pChromoBrainM = pPaireBrain->getMaterChromosome();
  CChromosome* pChromoBrainP = pPaireBrain->getPaterChromosome();

  CChromosome* pChromoSexM = pPaireSex->getMaterChromosome();
  CChromosome* pChromoSexP = pPaireSex->getPaterChromosome();

  // Store Identification table
  for (i = 0; i < pBrain->GetIdentifyNeuronTable()->GetNeuronTableRowCount(); i++)
  {
    if (pBrain->IsIdentifyRowSexSpecific(i))
    {
      indexGen = pChromoSexM->addGene();
      pGen1M = pChromoSexM->getGene(indexGen);
      indexGen = pChromoSexP->addGene();
      pGen1P = pChromoSexP->getGene(indexGen);
    }
    else
    {
      indexGen = pChromoBrainM->addGene();
      pGen1M = pChromoBrainM->getGene(indexGen);
      indexGen = pChromoBrainP->addGene();
      pGen1P = pChromoBrainP->getGene(indexGen);
    }

    lineSize = pBrain->GetIdentifyNeuronTable()->buildRawDataFromNeuronLine(i,pData);
    pGen1M->setAsBrainIdentificationLine(10,i,lineSize,pData);
    pGen1P->setAsBrainIdentificationLine(10,i,lineSize,pData);
    if (pData)
      delete [] pData;
  }

  return true;
}

std::pair<size_t, size_t> CGenome::findGeneInGenome(CGene& modelGene, bool findDominantAlleleOnly)
{
  std::string modelGeneString = modelGene.buildStringDataFromGene();
  CPairOfChromosome* pPaire = NULL;

  for (size_t paireIndex = 0; paireIndex < m_tPair.size(); paireIndex++)
  {
    pPaire = m_tPair[paireIndex];
    for (size_t geneIndex = 0; geneIndex < pPaire->getNumGenes(); geneIndex++)
    {
      if (findDominantAlleleOnly)
      {
        if (pPaire->getDominantAllele(geneIndex)->buildStringDataFromGene() == modelGeneString)
          return { paireIndex , geneIndex };
      }
      else
      {
        if (pPaire->getMaterChromosome()->getGene(geneIndex)->buildStringDataFromGene() == modelGeneString)
          return { paireIndex , geneIndex };
        else if (pPaire->getPaterChromosome()->getGene(geneIndex)->buildStringDataFromGene() == modelGeneString)
          return { paireIndex , geneIndex };
      }
    }
  }

  return { invalidIndex , invalidIndex };
}

bool  CGenome::isVegetalGenome(void)
{
  return ((m_class >= CLASS_VEGETAL_FIRST) && (m_class <= CLASS_VEGETAL_LAST));
}

bool  CGenome::isAnimalGenome(void)
{
  return ((m_class >= CLASS_ANIMAL_FIRST) && (m_class <= CLASS_ANIMAL_LAST));
}

bool CGenome::isParasiteGenome(void)
{
  return ((m_class >= CLASS_PARASITE_FIRST) && (m_class <= CLASS_PARASITE_LAST));
}