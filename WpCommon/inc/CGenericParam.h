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
// FILE: CGenericParam.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined(CGENERICPARAM_INCLUDED_)
#define CGENERICPARAM_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>

#include "Definitions.h"
#include "CybioXmlDef.h"
#include "CGene.h"

//===========================================================================
// Definitions            
//===========================================================================
typedef enum
{
  PARAM_GENERIC = 0,
  PARAM_ENVIRONMENT,
  PARAM_DURATION,
  PARAM_PHYSIC,
  PARAM_BEHAVIOR,
  PARAM_FEELING,
  PARAM_REPRODUCTION
} GenericParamType_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGenericParam  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  double m_ValCurrent;
  double m_ValNominal;
  double m_ValMin;
  double m_ValMax;
  string m_Label;        // User defined name
  GenericParamType_e m_Type;
  GeneSubType_e      m_CodingGeneSubType;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CGenericParam(double valMin,double valInit,double valNominal,double valMax,string label,
                  GenericParamType_e type, GeneSubType_e codingGeneSubType = GENE_GENERIC_UNKNOWN);
	virtual ~CGenericParam();

 //---------------------------------------------------------------------------
// Save/Load in xml file
//---------------------------------------------------------------------------
  virtual bool saveInXmlFile(TiXmlNode* pNode);
  virtual bool loadFromXmlFile(TiXmlNode* pNode);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  double getVal(void);
  bool setVal(double newVal);
  bool forceVal(double newVal);
  double getValNominal(void);
  bool setValNominal(double newVal);
  bool setValMax(double newVal);
  bool setValMin(double newVal);
  double getMin(void);
  double getMax(void);
  string getLabel(void);
  bool changeVal(double variation);
  bool isMaxReached(void);
  bool isMinReached(void);
  GenericParamType_e getType();
  GeneSubType_e getCodingGeneSubtype();
  virtual double getCurrentPhase() { return 0; };
  virtual void setCurrentPhase(double phase) { };
};

#endif // !defined(CGENERICPARAM_INCLUDED_)
