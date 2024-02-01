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
// FILE: CGenericParam.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#include "CGenericParam.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

CGenericParam::CGenericParam(double valMin, double valInit, double valNominal, double valMax, GenericParamType_e type)
{
  m_ValMin     = valMin;
  m_ValCurrent = valInit;
  m_ValNominal = valNominal;
  m_ValMax     = valMax;
  m_Type       = type;

  // Corrections
  if (m_ValMax<m_ValMin)
    m_ValMax = m_ValMin;

  if (m_ValCurrent<m_ValMin)
    m_ValCurrent = m_ValMin;
  if (m_ValCurrent>m_ValMax)
    m_ValCurrent = m_ValMax;

  if (m_ValNominal<m_ValMin)
    m_ValNominal = m_ValMin;
  if (m_ValNominal>m_ValMax)
    m_ValNominal = m_ValMax;
}

CGenericParam::~CGenericParam()
{

}

//===========================================================================
// Save/Load in xml file
//===========================================================================
bool CGenericParam::saveInXmlFile(TiXmlNode* pNode, bool doSaveMinMax)
{
  bool resu = false;
  TiXmlElement newParam(XML_NODE_PARAMETER);
  TiXmlNode* pNodeChild = pNode->InsertEndChild(newParam);
  if (pNodeChild != NULL)
  {
    TiXmlElement* pElement = (TiXmlElement*)pNodeChild;
    pElement->SetAttribute(XML_ATTR_NAME, getLabel());
    pElement->SetDoubleAttribute(XML_ATTR_VALUE, getVal());
    if (doSaveMinMax)
    {
      pElement->SetDoubleAttribute(XML_ATTR_RANGE_MIN, getMin());
      pElement->SetDoubleAttribute(XML_ATTR_RANGE_MAX, getMax());
    }
    resu = true;
  }
  return resu;
}

bool CGenericParam::loadFromXmlFile(TiXmlNode* pNode)
{
  TiXmlElement* pElement = (TiXmlElement*)pNode;
  double paramVal;
  if (pElement->QueryDoubleAttribute(XML_ATTR_VALUE, &paramVal) != TIXML_NO_ATTRIBUTE)
  {
    setVal(paramVal);
  }
  if (pElement->QueryDoubleAttribute(XML_ATTR_RANGE_MIN, &paramVal) != TIXML_NO_ATTRIBUTE)
  {
    setValMin(paramVal);
  }
  if (pElement->QueryDoubleAttribute(XML_ATTR_RANGE_MAX, &paramVal) != TIXML_NO_ATTRIBUTE)
  {
    setValMax(paramVal);
  }

  return true;
}


//===========================================================================
// Get / Set for attributes
//===========================================================================

double CGenericParam::getVal(void)
{
  return (m_ValCurrent);
}

bool CGenericParam::setVal(double newVal)
{
  m_ValCurrent = newVal;
  if (m_ValCurrent>m_ValMax)
  {
    m_ValCurrent=m_ValMax;
    return (false);
  }
  else if (m_ValCurrent<m_ValMin)
  {
    m_ValCurrent=m_ValMin;
    return (false);
  }
  else
  {
    return (true);
  }
}

bool CGenericParam::forceVal(double newVal)
{
  m_ValCurrent = newVal;
  if ((m_ValCurrent>m_ValMax)||(m_ValCurrent<m_ValMin))
  {
    return (false);
  }
  else
  {
    return (true);
  }
}

double CGenericParam::getValNominal(void)
{
  return (m_ValNominal);
}

bool CGenericParam::setValNominal(double newVal)
{
  m_ValNominal = newVal;
  if (m_ValNominal>m_ValMax)
  {
    m_ValNominal=m_ValMax;
    return (false);
  }
  else if (m_ValNominal<m_ValMin)
  {
    m_ValNominal=m_ValMin;
    return (false);
  }
  else
  {
    return (true);
  }
}

bool CGenericParam::setValMax(double newVal)
{
  m_ValMax = newVal;
  if (m_ValNominal>m_ValMax)
  {
    m_ValNominal=m_ValMax;
    return (false);
  }
  else if (m_ValCurrent>m_ValMax)
  {
    m_ValCurrent=m_ValMax;
    return (false);
  }
  else
  {
    return (true);
  }
}

bool CGenericParam::setValMin(double newVal)
{
  m_ValMin = newVal;
  if (m_ValNominal<m_ValMin)
  {
    m_ValNominal=m_ValMax;
    return (false);
  }
  else if (m_ValCurrent<m_ValMin)
  {
    m_ValCurrent=m_ValMin;
    return (false);
  }
  else
  {
    return (true);
  }
}

double CGenericParam::getMin(void)
{
  return (m_ValMin);
}

double CGenericParam::getMax(void)
{
  return (m_ValMax);
}

string CGenericParam::getLabel(void)
{
  return ("Unset");
}

bool CGenericParam::changeVal(double variation)
{
  m_ValCurrent += variation;
  if (m_ValCurrent>m_ValMax)
  {
    m_ValCurrent=m_ValMax;
    return (false);
  }
  else if (m_ValCurrent<m_ValMin)
  {
    m_ValCurrent=m_ValMin;
    return (false);
  }
  else
  {
    return (true);
  }
}

bool CGenericParam::isMaxReached(void)
{
  if (m_ValCurrent==m_ValMax)
    return (true);
  else
    return (false);
}

bool CGenericParam::isMinReached(void)
{
  if (m_ValCurrent==m_ValMin)
    return (true);
  else
    return (false);
}

GenericParamType_e CGenericParam::getType()
{
  return m_Type;
}

GeneSubTypeParam_e CGenericParam::getCodingGeneSubtype()
{
  return GENE_PARAM_UNKNOWN;
}