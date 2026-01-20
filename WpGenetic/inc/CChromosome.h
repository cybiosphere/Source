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
// FILE: CChromosome.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a set of basic genes
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined( CCHROMOSOME_INCLUDED_)
#define CCHROMOSOME_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//===========================================================================
// Includes 
//===========================================================================

#include <algorithm> // for std::swap
#include "CGene.h"

//===========================================================================
// Definitions            
//===========================================================================

typedef enum
{
 CHROMOSOME_NEUTRAL =     0,
 CHROMOSOME_SEX_MALE,
 CHROMOSOME_SEX_FEMALE,    
} ChromosomeType_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CChromosome
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data:
//---------------------------------------------------------------------------
private:
  size_t m_IdNumber;
  ChromosomeType_e m_ChromosomeType;
  std::vector<CGene*> m_tGene;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CChromosome(size_t number);
  CChromosome(const CChromosome& model);          // note const
  CChromosome& operator=(const CChromosome& rhs); // copy-assignment
  void swap(CChromosome& other) noexcept;
  virtual ~CChromosome();

//---------------------------------------------------------------------------
// Gene table mgt
//---------------------------------------------------------------------------
public:
  size_t addGene();
  bool removeGeneFromIndex(size_t index);
  bool removeGene(CGene* pGene);
  size_t getNumGene(void);
  CGene* getGene(size_t index);
  void deleteAllGenes(void);

//---------------------------------------------------------------------------
// Raw data conversion
//---------------------------------------------------------------------------
public:
  string buildStringDataFromGenes();
  bool buildGenesFromStringData(string rawData);

//---------------------------------------------------------------------------
// Genetic
//---------------------------------------------------------------------------
public:
  bool tryMutation(int rate);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:  
  size_t getIdNumber(void);
  string getLabel(void);
  void setChromosomeType(ChromosomeType_e newType);
  ChromosomeType_e getChromosomeType();

}; // end CChromosome

#endif // !defined(CCHROMOSOME_INCLUDED_)



