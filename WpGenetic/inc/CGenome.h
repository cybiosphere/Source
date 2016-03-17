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
// FILE: CGenome.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents the whole genetic caracteristic of a creature
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined( CGENOME_INCLUDED_)
#define CGENOME_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//#include <afxwin.h>
//#include <afxtempl.h>

#include "CPairOfChromosome.h"
#include "CybioXmlDef.h"

class CBrain;

typedef enum
{
 SEX_UNSET = -1,
 SEX_NONE,
 SEX_MALE,
 SEX_FEMALE,    
} SexType_e;

typedef enum
{
  CLASS_UNSET = -1,
  CLASS_NONE  =  0,

  // Mineral classs
  CLASS_MINERAL_FIRST,        
  CLASS_ROCK = CLASS_MINERAL_FIRST,       // Any block
  CLASS_WATER,                            // Water entity
  CLASS_MINERAL_LAST  = CLASS_WATER,

  // Vegetal classs
  CLASS_VEGETAL_FIRST,                     
  CLASS_PHYCOPHYTA = CLASS_VEGETAL_FIRST, // Containing alga 
  CLASS_MYCOPHYTA,                        // Containing mushrooms
  CLASS_BRYOPHYTA,                        // Containing the mosses 
  CLASS_PTERIDOPHYTA,                     // Containing ferns 
  CLASS_SPERMATOPHYTA,                    // Containing plants with seeds
  CLASS_VEGETAL_LAST = CLASS_SPERMATOPHYTA,

  // Animal classs
  CLASS_ANIMAL_FIRST,
  CLASS_MAMMAL = CLASS_ANIMAL_FIRST,
  CLASS_REPTILE,
  CLASS_ARTHROPOD,
  CLASS_AVE,
  CLASS_ANIMAL_LAST = CLASS_AVE,

} ClassType_e;

typedef unsigned long specieSignatureType;


class DLL_CYBIOCORE_API CGenome
{
  ///////////////////////////////////////
  // constructors

public:
  CGenome(ClassType_e classType, string specieName);
  CGenome(CGenome& model);
  CGenome(CGenome& mother, CGenome& father, double crossoverRate);
  virtual ~CGenome();

  ///////////////////////////////////////
  // attributes

private:
  ClassType_e m_class;
  string m_SpecieName; 
  int m_SexualPairIndex;
  bool m_bSexualDimorphism;
  std::vector<CPairOfChromosome*> m_tPair;

  ///////////////////////////////////////
  // methods

public:
  int addPair();
  bool removePair(int index);
  int getNumPair();
  string getOwnClassStrName();
  static string getClassStrName(ClassType_e classType);
  CPairOfChromosome* getPair(int index);
  CPairOfChromosome* getPairSexual();
  CPairOfChromosome* getPairBrain();
  bool setSexMale  (int indexPairSex, bool sexualDimorph = false);
  bool setSexFemale(int indexPairSex, bool sexualDimorph = false);
  bool setSexNeutral(int indexPairSex);
  SexType_e getSexType();
  bool tryMutation(int rate);
  ClassType_e getClass();
  string getSpecieName();
  
  specieSignatureType getSpecieSignature();
  bool checkSpecieCompatibility(CGenome* pOther);

  bool saveInFile(string fileNameWithPath);
  bool loadFromFile(string fileNameWithPath);

  bool saveInXmlFile(string fileNameWithPath);
  bool saveInXmlFile(TiXmlDocument *pXmlDoc);
  bool loadFromXmlFile(string fileNameWithPath);
  bool loadFromXmlFile(TiXmlDocument *pXmlDoc);

  bool setBrainInstinctInGenes(CBrain* pBrain);
  bool setBrainIdentifyInGenes(CBrain* pBrain);

private:
  void deleteAllPairs(void);

}; // end CGenome

#endif // !defined(CGENOME_INCLUDED_)



