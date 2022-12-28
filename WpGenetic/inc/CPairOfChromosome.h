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
// FILE: CPairOfChromosome.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a pair of chromosome
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined( CPAIREOFCHROMOSOME_INCLUDED_)
#define CPAIREOFCHROMOSOME_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//#include <afxwin.h>
//#include <afxtempl.h>

#include "CChromosome.h"

class DLL_CYBIOCORE_API CPairOfChromosome
{
  ///////////////////////////////////////
  // constructors

public:
  CPairOfChromosome(size_t number);
  CPairOfChromosome(CPairOfChromosome& model);
  CPairOfChromosome(CPairOfChromosome& mother, CPairOfChromosome& father, double crossoverRate);
  virtual ~CPairOfChromosome();

  ///////////////////////////////////////
  // attributes

private:
  size_t m_IdNumber;
  CChromosome* m_pPaterChromosome;
  CChromosome* m_pMaterChromosome;

  ///////////////////////////////////////
  // methods

public:
  CChromosome* getPaterChromosome ();
  CChromosome* getMaterChromosome ();

  size_t getNumGenes();
  CGene* getDominantAllele (size_t index);
  bool tryMutation(int rate);

  bool setAsSexualMale(void);
  bool setAsSexualFemale(void);
  bool setAsNeutral(void);

  bool getCrossedChromosomeStr(CPairOfChromosome& pPair, string& crossedStr);

  ChromosomeType_e getSex(void);
  size_t getIdNumber(void);

private:

}; // end CPairOfChromosome

#endif // !defined(CPAIREOFCHROMOSOME_INCLUDED_)



