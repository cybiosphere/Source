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
// FILE: CMineral.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a mineral entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================
 
#include "CMineral.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CMineral::CMineral
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
CMineral::CMineral(string label, Point_t initCoord, size_t layer, CGenome* pPseudoGenome)
{
  // Input values 
  jumpToGridCoord(initCoord, layer);
  m_pGenome       = pPseudoGenome;
  m_pBrain        = NULL;

  // Default values   
  m_Status      = STATUS_STATIC;
  m_Label       = label;
  m_Generation  = 0;
  m_TotalChildNumber = 0;

}

//---------------------------------------------------------------------------
// METHOD:       CMineral::CMineral
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CMineral& model : Single parent constructor for cloning
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------     
CMineral::CMineral(string label, CMineral& model) 
{ 
  m_Label         = label;
  // inherited
  jumpToGridCoord(model.getGridCoord(), true, model.getLayer()); 
  m_Generation		= model.m_Generation + 1;
  m_pGenome       = new CGenome(*model.m_pGenome);

  // reset
  m_pBrain          = NULL;
  m_Status      = STATUS_STATIC;
  m_Generation  = 0;
  m_TotalChildNumber = 0;
}

string CMineral::buildLifeStageString(CGene* pGen)
{
  string defStr = STRING_GENE_UNUSED;
  return(defStr);
}