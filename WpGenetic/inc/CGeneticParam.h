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
// FILE: CGeneticParam.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a parameter built by gene
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  08/06/2007 : Creation
//
//===========================================================================

#if !defined(CGENETICPARAM_INCLUDED_)
#define CGENETICPARAM_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>

#include "CGenericParam.h"

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGeneticParam : public CGenericParam
{
  //===========================================================================
  // Attributes 
  //===========================================================================

  //---------------------------------------------------------------------------
  // internal proprieties:
  //---------------------------------------------------------------------------
protected:
  const CGeneDefinitions* m_pGeneDefinitions;

  //===========================================================================
  // Methods 
  //===========================================================================

  //---------------------------------------------------------------------------
  // Constructors / Destructors
  //---------------------------------------------------------------------------
public:
  CGeneticParam(double valMin, double valInit, double valNominal, double valMax, GenericParamType_e type, const CGeneDefinitions* pGeneDefinitions);
  virtual ~CGeneticParam();

  //--------------------------------------------------------------------------- 
  // Get / Set for attributes
  //---------------------------------------------------------------------------
public:
  string getLabel(void);
  GeneSubTypeParam_e getCodingGeneSubtype();

};

#endif // !defined(CGENETICPARAM_INCLUDED_)
