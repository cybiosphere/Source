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

CGenericParam::CGenericParam(double valMin,double valInit,double valNominal,double valMax,string label,GenericParamType_e type, GeneSubType_e codingGeneSubType)
{
  m_ValMin     = valMin;
  m_ValCurrent = valInit;
  m_ValNominal = valNominal;
  m_ValMax     = valMax;
  m_Label      = label;
  m_Type       = type;

  m_CodingGeneSubType = codingGeneSubType;

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
  return (m_Label);
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

GeneSubType_e CGenericParam::getCodingGeneSubtype()
{
  return m_CodingGeneSubType;
}