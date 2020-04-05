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

// CWizardSensor.cpp: implementation of the CBrain class. 
//
//////////////////////////////////////////////////////////////////////

#include "CWizardSensor.h"
#include "CBrainAnimal.h"
#include "CWater.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWizardSensor::CWizardSensor(CAnimal* pAnimal, int nPreyNumber, CBasicEntity** tPreyList, int nPredatorNumber, CBasicEntity** tPredatorList)
{
  m_pAnimal         = pAnimal;
  m_nPreyNumber     = nPreyNumber;
  m_tPreyList       = tPreyList;
  m_nPredatorNumber = nPredatorNumber;
  m_tPredatorList   = tPredatorList;

  for (int i=0;i<MAX_NB_LAYER_VIEW;i++)
  {
    m_tViewDesc[i].layer = invalidCoord;
    m_tViewDesc[i].nbWeight0 = 0;
    m_tViewDesc[i].pWeight0  = NULL;
    m_tViewDesc[i].nbWeight1 = 0;
    m_tViewDesc[i].pWeight1  = NULL;
  }

}


CWizardSensor::~CWizardSensor()
{

}

bool CWizardSensor::InitViewDescLayer(CBasicEntity* pEntity, int range, ViewAngleType_e angle, int nbFocus, bool isIdentifiedView)
{
  int nbWeight;
  int i;
  size_t layer = pEntity->getLayer();
  if ( ( m_tViewDesc[layer].layer == invalidCoord) && (pEntity != NULL) )
  {
    m_tViewDesc[layer].layer  = pEntity->getLayer();
    m_tViewDesc[layer].range0 = 1;
    m_tViewDesc[layer].angle0 = VIEW_ANGLE_135;
    m_tViewDesc[layer].nbFocus0 = min(3, nbFocus);

    if (isIdentifiedView == false)
      nbWeight = m_tViewDesc[layer].nbFocus0 * VIEW_SIZE_PER_FOCUS;
    else
      nbWeight = m_tViewDesc[layer].nbFocus0 * VIEW_IDENTIFY_SIZE_PER_FOCUS;

    m_tViewDesc[layer].nbWeight0 = nbWeight;
    m_tViewDesc[layer].pWeight0 = new short[nbWeight];
    for(i=0;i<nbWeight;i++)
      m_tViewDesc[layer].pWeight0[i] = 100; // stand for 10.0

    m_tViewDesc[layer].layer  = pEntity->getLayer();
    m_tViewDesc[layer].range1 = min(VIEW_FAR_MIN_RANGE, range);
    m_tViewDesc[layer].angle1 = angle;
    m_tViewDesc[layer].nbFocus1 = nbFocus;

    if (isIdentifiedView == false)
      nbWeight = m_tViewDesc[layer].nbFocus1 * VIEW_SIZE_PER_FOCUS;
    else
      nbWeight = m_tViewDesc[layer].nbFocus1 * VIEW_IDENTIFY_SIZE_PER_FOCUS;

    m_tViewDesc[layer].nbWeight1 = nbWeight;
    m_tViewDesc[layer].pWeight1 = new short[nbWeight];
    for(i=0;i<nbWeight;i++)
      m_tViewDesc[layer].pWeight1[i] = 100; // stand for 10.0
  }

  return true;
}

bool CWizardSensor::AddBonusOnViewWeight(short* pWeight, int nbFocus, short bonusRate, CBasicEntity* pTarget, bool isMoving)
{
  size_t subOffsetForm    = invalidIndex;
  size_t subOffsetColor   = invalidIndex;
  size_t subOffsetTexture = invalidIndex;
  size_t subOffsetAttrib  = invalidIndex;
  size_t subOffsetIsMove  = invalidIndex;
  size_t i,offset = 0;
  size_t count = 0;

  if (pTarget->getForm() >= FORM_FIRST_TYPE)
  {
    subOffsetForm    = CSensorView::GetSubCaptorSubIndexForForm(pTarget->getForm());
    count++;
  }
  if (pTarget->getColorType() >= COLOR_CARACTER_FIRST_TYPE)
  {
    subOffsetColor   = CSensorView::GetSubCaptorSubIndexForColor(pTarget->getColorType());
    count++;
  }
  if (pTarget->getTexture() >= TEXTURE_FIRST_TYPE)
  {
    subOffsetTexture = CSensorView::GetSubCaptorSubIndexForTexture(pTarget->getTexture());
    count++;
  }
  if (pTarget->getMainPhyAttribute() >= PHY_ATTRIBUTE_FIRST_TYPE)
  {
    subOffsetAttrib  = CSensorView::GetSubCaptorSubIndexForPhyAttribute(pTarget->getMainPhyAttribute());
    count++;
  }
  if (isMoving)
  {
    subOffsetIsMove  = CSensorView::GetSubCaptorSubIndexForMoving();
  }

  for (i=0; i<nbFocus; i++)
  {
    if (subOffsetForm != invalidIndex)
      pWeight[offset + subOffsetForm]    += bonusRate / count;
    if (subOffsetColor != invalidIndex)
      pWeight[offset + subOffsetColor]   += bonusRate / count;
    if (subOffsetTexture != invalidIndex)
      pWeight[offset + subOffsetTexture] += bonusRate / count;
    if (subOffsetAttrib != invalidIndex)
      pWeight[offset + subOffsetAttrib]  += bonusRate / count;
    if (isMoving)
      pWeight[offset + subOffsetIsMove]  += bonusRate / count / 2;
    offset += VIEW_SIZE_PER_FOCUS;
  }

  return true;  
}

bool CWizardSensor::AddBonusOnIdentifyViewWeight(short* pWeight, int nbFocus, short bonusRate, IdentificationType_e identity, 
                                                 bool isMoving, bool isDirLft, bool isDirRight)
{
  size_t subOffsetProxy    = invalidIndex;
  size_t subOffsetEscape   = invalidIndex;
  size_t subOffsetAppraoch = invalidIndex;
  size_t subOffsetdirLeft  = invalidIndex;
  size_t subOffsetdirRight = invalidIndex;

  int i,offset = 0;
  int count = 0;

  subOffsetProxy = CSensorViewIdentify::GetSubCaptorSubIndexForProximity(identity);
  count++;

  if (isMoving)
  {
    subOffsetEscape = CSensorViewIdentify::GetSubCaptorSubIndexForEscapeSpeed(identity);
    count++;
    subOffsetAppraoch = CSensorViewIdentify::GetSubCaptorSubIndexForApproachSpeed(identity);
    count++;
  }
  if (isDirLft)
  {
    subOffsetdirLeft  = CSensorViewIdentify::GetSubCaptorSubIndexForDirLeft(identity);
    count++;
  }
  if (isDirRight)
  {
    subOffsetdirRight  = CSensorViewIdentify::GetSubCaptorSubIndexForDirRight(identity);
    count++;
  }

  for (i=0; i<nbFocus; i++)
  {
    if (subOffsetProxy != invalidIndex)
      pWeight[offset + subOffsetProxy]    += bonusRate / count;
    if (subOffsetEscape != invalidIndex)
      pWeight[offset + subOffsetEscape]   += bonusRate / count;
    if (subOffsetAppraoch != invalidIndex)
      pWeight[offset + subOffsetAppraoch] += bonusRate / count;
    if (subOffsetdirLeft != invalidIndex)
      pWeight[offset + subOffsetdirLeft]  += bonusRate / count / 2;
    if (subOffsetdirRight != invalidIndex)
      pWeight[offset + subOffsetdirRight] += bonusRate / count / 2;
    offset += VIEW_IDENTIFY_SIZE_PER_FOCUS;
  }

  return true;  
}



bool CWizardSensor::AddGenesVisualSensors(int chromoIdx, int mutationRate, int range, ViewAngleType_e angle, int nbFocus, int variation)
{
  int i;
  size_t layer;
  CWater water;

  // Find all view layers (entity layer + water layer + prey layer + predator layer
  InitViewDescLayer(m_pAnimal, range, angle, nbFocus);  
  InitViewDescLayer(&water, range, angle, nbFocus);
  for (i=0;i<m_nPreyNumber;i++)
  {
    InitViewDescLayer(m_tPreyList[i], range, angle, nbFocus);
  }
  for (i=0;i<m_nPredatorNumber;i++)
  {
    InitViewDescLayer(m_tPredatorList[i], range, angle, nbFocus);
  }

  // Init View Far
  int nbWeightViewFar = nbFocus * VIEW_SIZE_PER_FOCUS;
  short* pWeightViewFar = new short[nbWeightViewFar];
  for(i=0;i<nbWeightViewFar;i++)
    pWeightViewFar[i] = 100; // stand for 10.0

  // Add bonus to all sensor Weight table
  layer = m_pAnimal->getLayer();
  AddBonusOnViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 200.0 + getRandInt(variation), m_pAnimal, false);
  AddBonusOnViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 180.0 + getRandInt(variation), m_pAnimal, false);
  AddBonusOnViewWeight(pWeightViewFar, nbFocus, 160.0 + getRandInt(variation), m_pAnimal, true);

  layer = 1;
  AddBonusOnViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 480.0 + getRandInt(variation), &water, false);
  AddBonusOnViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 360.0 + getRandInt(variation), &water, false);
  AddBonusOnViewWeight(pWeightViewFar, nbFocus, 160.0 + getRandInt(variation), &water, false);

  for (i=0;i<m_nPreyNumber;i++)
  {
    layer = m_tPreyList[i]->getLayer();
    AddBonusOnViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 200.0 + getRandInt(variation), m_tPreyList[i], false);
    AddBonusOnViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 180.0 + getRandInt(variation), m_tPreyList[i], false);
    AddBonusOnViewWeight(pWeightViewFar, nbFocus, 160.0 + getRandInt(variation), m_tPreyList[i], false);
  }

  for (i=0;i<m_nPredatorNumber;i++)
  {
    layer = m_tPredatorList[i]->getLayer();
    AddBonusOnViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 300.0 + getRandInt(variation), m_tPredatorList[i], false);
    AddBonusOnViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 300.0 + getRandInt(variation), m_tPredatorList[i], false);
    AddBonusOnViewWeight(pWeightViewFar, nbFocus, 200.0 + getRandInt(variation), m_tPredatorList[i], true);
  }


  // Add genes
  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;
  double scale1,scale2,scale3,scale4;

  // Add sensors view
  scale1 = m_pAnimal->getGeneScaleData1(GENE_SENS_VIEW) * 65536.0;
  if (scale1!=0)
    scale1 = 1000.0 / scale1;
  scale2 = m_pAnimal->getGeneScaleData2(GENE_SENS_VIEW) * 65536.0;
  if (scale2!=0)
    scale2 = 1000.0 / scale2;
  scale3 = m_pAnimal->getGeneScaleData3(GENE_SENS_VIEW) * 65536.0;
  if (scale3!=0)
    scale3 = 1000.0 / scale3;
  scale4 = m_pAnimal->getGeneScaleData4(GENE_SENS_VIEW) * 65536.0;
  if (scale1!=0)
    scale4 = 1000.0 / scale4;

  for(i=0;i<MAX_NB_LAYER_VIEW;i++)
  {
    if (m_tViewDesc[i].layer != invalidCoord)
    {
      // view adjacent
      pCurPaire = m_pAnimal->getGenome()->getPair(chromoIdx);
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW, mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, 
                            m_tViewDesc[i].range0 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle0 * scale3, m_tViewDesc[i].nbFocus0 * scale4);
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW, mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, 
                            m_tViewDesc[i].range0 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle0 * scale3, m_tViewDesc[i].nbFocus0 * scale4);

      // Add composite view/speed
      DWORD sensorUID = UID_BASE_SENS_VIEW + 10*m_tViewDesc[i].layer + m_tViewDesc[i].angle0;
      int paramId = m_pAnimal->getParamIdByName("Speed");      
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensorComposite(mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, sensorUID, paramId); 
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensorComposite(mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, sensorUID, paramId); 

      // view medium
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW, mutationRate, m_tViewDesc[i].nbWeight1, m_tViewDesc[i].pWeight1, 
                            m_tViewDesc[i].range1 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle1 * scale3, m_tViewDesc[i].nbFocus1 * scale4);
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW, mutationRate, m_tViewDesc[i].nbWeight1, m_tViewDesc[i].pWeight1, 
                            m_tViewDesc[i].range1 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle1 * scale3, m_tViewDesc[i].nbFocus1 * scale4); 
    }
  }

  // Add sensors view far
  if (range > VIEW_FAR_MIN_RANGE)
  {
      scale1 = m_pAnimal->getGeneScaleData1(GENE_SENS_VIEW_FAR) * 65536.0;
      if (scale1!=0)
        scale1 = 1000.0 / scale1;
      scale2 = m_pAnimal->getGeneScaleData2(GENE_SENS_VIEW_FAR) * 65536.0;
      if (scale2!=0)
        scale2 = 1000.0 / scale2;
      scale3 = m_pAnimal->getGeneScaleData3(GENE_SENS_VIEW_FAR) * 65536.0;
      if (scale3!=0)
        scale3 = 1000.0 / scale3;
      scale4 = m_pAnimal->getGeneScaleData4(GENE_SENS_VIEW_FAR) * 65536.0;
      if (scale1!=0)
        scale4 = 1000.0 / scale4;

      pCurPaire = m_pAnimal->getGenome()->getPair(chromoIdx);
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_FAR, mutationRate, nbWeightViewFar, pWeightViewFar, 
                            (VIEW_FAR_MIN_RANGE+1) * scale1, range * scale2, angle * scale3, nbFocus * scale4);
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_FAR, mutationRate, nbWeightViewFar, pWeightViewFar, 
                            (VIEW_FAR_MIN_RANGE+1) * scale1, range * scale2, angle * scale3, nbFocus * scale4);
  }

  delete[] pWeightViewFar;
  return true;
}

bool CWizardSensor::AddGenesVisualIdentifySensors(int chromoIdx, int mutationRate, int range, ViewAngleType_e angle, int nbFocus, int variation)
{
  int i;
  size_t layer;
  CWater water;

  // Find all view layers (entity layer + water layer + prey layer + predator layer
  InitViewDescLayer(m_pAnimal, range, angle, nbFocus, true);  
  InitViewDescLayer(&water, range, angle, nbFocus, true);
  for (i=0;i<m_nPreyNumber;i++)
  {
    InitViewDescLayer(m_tPreyList[i], range, angle, nbFocus, true);
  }
  for (i=0;i<m_nPredatorNumber;i++)
  {
    InitViewDescLayer(m_tPredatorList[i], range, angle, nbFocus, true);
  }

  // Init View Far
  int nbWeightViewFar = nbFocus * VIEW_IDENTIFY_SIZE_PER_FOCUS;
  short* pWeightViewFar = new short[nbWeightViewFar];
  for(i=0;i<nbWeightViewFar;i++)
    pWeightViewFar[i] = 100; // stand for 10.0

  // Add bonus to all sensor Weight table
  layer = m_pAnimal->getLayer();
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0,  50.0 + getRandInt(variation), IDENTIFICATION_NEUTRAL);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 200.0 + getRandInt(variation), IDENTIFICATION_DANGER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_BABY);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_MOTHER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_FRIEND);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_SEX_PARTNER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  40.0 + getRandInt(variation), IDENTIFICATION_NEUTRAL);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 180.0 + getRandInt(variation), IDENTIFICATION_DANGER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_BABY);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_MOTHER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_FRIEND);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_SEX_PARTNER);

  layer = 1;
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 200.0 + getRandInt(variation), IDENTIFICATION_DANGER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_DRINK);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 180.0 + getRandInt(variation), IDENTIFICATION_DANGER);
  AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_DRINK);

  for (i=0;i<m_nPreyNumber;i++)
  {
    layer = m_tPreyList[i]->getLayer();
    if (layer != invalidCoord)
    {
      AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_FOOD);
      AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 100.0 + getRandInt(variation), IDENTIFICATION_PREY, true);
      AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_FOOD);
      AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1,  90.0 + getRandInt(variation), IDENTIFICATION_PREY, true);
    }
  }

  for (i=0;i<m_nPredatorNumber;i++)
  {
    layer = m_tPredatorList[i]->getLayer();
    if (layer != invalidCoord)
    {
      AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight0, m_tViewDesc[layer].nbFocus0, 200.0 + getRandInt(variation), IDENTIFICATION_PREDATOR, true);
      AddBonusOnIdentifyViewWeight(m_tViewDesc[layer].pWeight1, m_tViewDesc[layer].nbFocus1, 200.0 + getRandInt(variation), IDENTIFICATION_PREDATOR, true);
    }
  }

  // Add genes
  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;
  double scale1,scale2,scale3,scale4;

  // Add sensors view
  scale1 = m_pAnimal->getGeneScaleData1(GENE_SENS_VIEW_IDENTIFY) * 65536.0;
  if (scale1!=0)
    scale1 = 1000.0 / scale1;
  scale2 = m_pAnimal->getGeneScaleData2(GENE_SENS_VIEW_IDENTIFY) * 65536.0;
  if (scale2!=0)
    scale2 = 1000.0 / scale2;
  scale3 = m_pAnimal->getGeneScaleData3(GENE_SENS_VIEW_IDENTIFY) * 65536.0;
  if (scale3!=0)
    scale3 = 1000.0 / scale3;
  scale4 = m_pAnimal->getGeneScaleData4(GENE_SENS_VIEW_IDENTIFY) * 65536.0;
  if (scale1!=0)
    scale4 = 1000.0 / scale4;

  for(i=0;i<MAX_NB_LAYER_VIEW;i++)
  {
    if (m_tViewDesc[i].layer != invalidCoord)
    {
      // view adjacent
      pCurPaire = m_pAnimal->getGenome()->getPair(chromoIdx);
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_IDENTIFY, mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, 
                            m_tViewDesc[i].range0 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle0 * scale3, m_tViewDesc[i].nbFocus0 * scale4);
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_IDENTIFY, mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, 
                            m_tViewDesc[i].range0 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle0 * scale3, m_tViewDesc[i].nbFocus0 * scale4);

      // Add composite view/speed
      DWORD sensorUID = UID_BASE_SENS_VIEW_IDENTIFY + 10*m_tViewDesc[i].layer + m_tViewDesc[i].angle0;
      int paramId = m_pAnimal->getParamIdByName("Speed");      
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensorComposite(mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, sensorUID, paramId); 
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensorComposite(mutationRate, m_tViewDesc[i].nbWeight0, m_tViewDesc[i].pWeight0, sensorUID, paramId); 

      // view medium
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_IDENTIFY, mutationRate, m_tViewDesc[i].nbWeight1, m_tViewDesc[i].pWeight1, 
                            m_tViewDesc[i].range1 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle1 * scale3, m_tViewDesc[i].nbFocus1 * scale4);
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_IDENTIFY, mutationRate, m_tViewDesc[i].nbWeight1, m_tViewDesc[i].pWeight1, 
                            m_tViewDesc[i].range1 * scale1, m_tViewDesc[i].layer * scale2,
                            m_tViewDesc[i].angle1 * scale3, m_tViewDesc[i].nbFocus1 * scale4); 
    }
  }

  if (range > VIEW_FAR_MIN_RANGE)
  {
      scale1 = m_pAnimal->getGeneScaleData1(GENE_SENS_VIEW_IDENT_FAR) * 65536.0;
      if (scale1!=0)
        scale1 = 1000.0 / scale1;
      scale2 = m_pAnimal->getGeneScaleData2(GENE_SENS_VIEW_IDENT_FAR) * 65536.0;
      if (scale2!=0)
        scale2 = 1000.0 / scale2;
      scale3 = m_pAnimal->getGeneScaleData3(GENE_SENS_VIEW_IDENT_FAR) * 65536.0;
      if (scale3!=0)
        scale3 = 1000.0 / scale3;
      scale4 = m_pAnimal->getGeneScaleData4(GENE_SENS_VIEW_IDENT_FAR) * 65536.0;
      if (scale1!=0)
        scale4 = 1000.0 / scale4;

      pCurPaire = m_pAnimal->getGenome()->getPair(chromoIdx);
      geneIndex = pCurPaire->getMaterChromosome()->addGene();
      pCurGene = pCurPaire->getMaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_IDENT_FAR, mutationRate, nbWeightViewFar, pWeightViewFar, 
                            (VIEW_FAR_MIN_RANGE+1) * scale1, range * scale2, angle * scale3, nbFocus * scale4);
      geneIndex = pCurPaire->getPaterChromosome()->addGene();
      pCurGene = pCurPaire->getPaterChromosome()->getGene(geneIndex);
      pCurGene->setAsSensor(GENE_SENS_VIEW_IDENT_FAR, mutationRate, nbWeightViewFar, pWeightViewFar, 
                            (VIEW_FAR_MIN_RANGE+1) * scale1, range * scale2, angle * scale3, nbFocus * scale4);
  }

  delete[] pWeightViewFar;
  return true;
}

bool CWizardSensor::AddGenesSmellSensor(int chromoIdx, int mutationRate, int range, int variation)
{
  int i;
  short* pWeightSmell = new short[NUMBER_ODORS];
  for(i=0;i<NUMBER_ODORS;i++)
    pWeightSmell[i] = 200; // stand for 10.0

  for (i=0;i<m_nPreyNumber;i++)
  {  
    if(m_tPreyList[i]->getOdor() >= ODOR_FIRST_TYPE)
      pWeightSmell[m_tPreyList[i]->getOdor()-ODOR_FIRST_TYPE] += 50 - i + getRandInt(variation);
  }
  for (i=0;i<m_nPredatorNumber;i++)
  {
    if(m_tPredatorList[i]->getOdor() >= ODOR_FIRST_TYPE)
      pWeightSmell[m_tPredatorList[i]->getOdor()-ODOR_FIRST_TYPE] += 100 - i + getRandInt(variation);
  }

  // Add weight for "strong" odors
  pWeightSmell[ODOR_REPULSIVE-ODOR_FIRST_TYPE] += 100 + getRandInt(variation);
  pWeightSmell[ODOR_NAUSEOUS-ODOR_FIRST_TYPE]  += 200 + getRandInt(variation);

  // Add genes
  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;
  double scale1;

  scale1 = m_pAnimal->getGeneScaleData1(GENE_SENS_SMELL) * 65536.0;
  if (scale1!=0)
    scale1 = 1000.0 / scale1;

  pCurPaire = m_pAnimal->getGenome()->getPair(chromoIdx);
  geneIndex = pCurPaire->getMaterChromosome()->addGene();
  pCurGene  = pCurPaire->getMaterChromosome()->getGene(geneIndex);
  pCurGene->setAsSensor(GENE_SENS_SMELL, mutationRate, NUMBER_ODORS, pWeightSmell, range * scale1);
  geneIndex = pCurPaire->getPaterChromosome()->addGene();
  pCurGene  = pCurPaire->getPaterChromosome()->getGene(geneIndex);
  pCurGene->setAsSensor(GENE_SENS_SMELL, mutationRate, NUMBER_ODORS, pWeightSmell, range * scale1);

  delete[] pWeightSmell;
  return true;
}


bool CWizardSensor::AddGenesGenericSensor(GeneSubType_e geneSubType, int defChromoIdx, int sexChromoIdx, 
                                          int mutationRate, short defWeight,int variation, int param1, int param2)
{
  int i;
  CGene tmpGene;
  tmpGene.setAsSensor(geneSubType,0,0,NULL,0,0,0,0);

  int chromoIdx = defChromoIdx;

  int weightSize = m_pAnimal->getExpectedBrainSensorWeightSize(&tmpGene);
  // if weight <1, not a good sensor gene
  if (weightSize < 1)
      return false;

  short* weightTab = new short[weightSize];
  for (i=0; i<weightSize; i++)
    weightTab[i] = defWeight + getRandInt(variation); // Def value

  // Add genes
  CPairOfChromosome* pCurPaire = NULL;
  int geneIndex;
  CGene* pCurGene = NULL;
  double scale1, scale2;

  scale1 = m_pAnimal->getGeneScaleData1(geneSubType) * 65536.0;
  if (scale1!=0)
    scale1 = 1000.0 / scale1;
  scale2 = m_pAnimal->getGeneScaleData2(geneSubType) * 65536.0;
  if (scale2!=0)
    scale2 = 1000.0 / scale2;

  // Check if sexual sensor
  tmpGene.setAsSensor(geneSubType, mutationRate, weightSize, weightTab, param1 * scale1, param2 * scale2);
  CSensor* pTmpSens = m_pAnimal->getTemporarySensorFromGene (&tmpGene);
  if ( pTmpSens && pTmpSens->IsSexSpecific(0) )
    chromoIdx = sexChromoIdx;
  if (pTmpSens)
    delete pTmpSens;

  pCurPaire = m_pAnimal->getGenome()->getPair(chromoIdx);
  geneIndex = pCurPaire->getMaterChromosome()->addGene();
  pCurGene  = pCurPaire->getMaterChromosome()->getGene(geneIndex);
  pCurGene->setAsSensor(geneSubType, mutationRate, weightSize, weightTab, param1 * scale1, param2 * scale2);
  geneIndex = pCurPaire->getPaterChromosome()->addGene();
  pCurGene  = pCurPaire->getPaterChromosome()->getGene(geneIndex);
  pCurGene->setAsSensor(geneSubType, mutationRate, weightSize, weightTab, param1 * scale1, param2 * scale2);

  delete[] weightTab;

  return true;
}
