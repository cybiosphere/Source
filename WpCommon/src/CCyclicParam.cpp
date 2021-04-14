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
// FILE: CCyclicParam.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CCyclicParam.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

CCyclicParam::CCyclicParam(double valMin, double valMax, int nbStepPerCycle, string label, GenericParamType_e type)
:CGenericParam(valMin, valMax, valMax, valMax, type)
{
  m_Label = label;
  m_nbStepPerCycle = nbStepPerCycle;
  m_CurPhase = 0;
  m_PhaseStep = 2*CYBIO_PI/nbStepPerCycle;
}

CCyclicParam::~CCyclicParam()
{

}

//===========================================================================
// Save/Load in xml file
//===========================================================================
bool CCyclicParam::saveInXmlFile(TiXmlNode* pNode)
{
  bool resu = false;
  TiXmlElement newParam(XML_NODE_PARAMETER);
  TiXmlNode* pNodeChild = pNode->InsertEndChild(newParam);
  if (pNodeChild != NULL)
  {
    TiXmlElement* pElement = (TiXmlElement*)pNodeChild;
    pElement->SetAttribute(XML_ATTR_NAME, getLabel());
    pElement->SetDoubleAttribute(XML_ATTR_VALUE, getVal());
    pElement->SetDoubleAttribute(XML_ATTR_PHASE, getCurrentPhase());
    pElement->SetDoubleAttribute(XML_ATTR_RANGE_MIN, getMin());
    pElement->SetDoubleAttribute(XML_ATTR_RANGE_MAX, getMax());
    resu = true;
  }
  return resu;
}

bool CCyclicParam::loadFromXmlFile(TiXmlNode* pNode)
{
  TiXmlElement* pElement = (TiXmlElement*)pNode;
  double paramVal, paramPhase, paramMin, paramMax;

  if (pElement->QueryDoubleAttribute(XML_ATTR_VALUE, &paramVal) != TIXML_NO_ATTRIBUTE)
  {
    setVal(paramVal);
  }
  if (pElement->QueryDoubleAttribute(XML_ATTR_PHASE, &paramPhase) != TIXML_NO_ATTRIBUTE)
  {
    setCurrentPhase(paramPhase);
  }
  if (pElement->QueryDoubleAttribute(XML_ATTR_RANGE_MIN, &paramMin) != TIXML_NO_ATTRIBUTE)
  {
    setValMin(paramMin);
  }
  if (pElement->QueryDoubleAttribute(XML_ATTR_RANGE_MAX, &paramMax) != TIXML_NO_ATTRIBUTE)
  {
    setValMax(paramMax);
  }

  return true;
}

//===========================================================================
// Cycle management
//===========================================================================
void CCyclicParam::NextStep(void)
{
  m_CurPhase += m_PhaseStep;
  if (m_CurPhase>2*CYBIO_PI)
  {
    m_CurPhase = 0;
  }
  setVal( (getMax()-getMin()) * (cos(m_CurPhase)+1) / 2 + getMin() );
}

double CCyclicParam::getCurrentPhase()
{
  return m_CurPhase;
}

void CCyclicParam::setCurrentPhase(double phase)
{
  m_CurPhase = phase;
}

//===========================================================================
// Get / Set for attributes
//===========================================================================

string CCyclicParam::getLabel(void)
{
  return m_Label;
}