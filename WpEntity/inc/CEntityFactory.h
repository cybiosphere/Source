/*
https://github.com/cybiosphere
copyright (c) 2020 Frederic RIBERT

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
// FILE: CEntityFactory.h
//   
// GENERAL DESCRIPTION:
//         This CLASS is used to build CEntity classes
//     
// (C) COPYRIGHT 2020.  All Rights Reserved.
//
//  20/01/2020 : Creation
//
//===========================================================================

#if !defined(CENTITYFACTORY_INCLUDED_)
#define CENTITYFACTORY_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CMineral.h"
#include "CWater.h"
#include "CVegetable.h"
#include "CVegetSpermatophyta.h"
#include "CAnimal.h"
#include "CAnimMammal.h"
#include "CVirus.h"

//===========================================================================
//                                    CLASS            
//===========================================================================

class DLL_CYBIOCORE_API CEntityFactory
{
public:
  static CBasicEntity* createEntity(string name, CGenome* pGenome);
  static CBasicEntity* createEntity(string fileNameWithPath);
  static CBasicEntity* createEntity(TiXmlDocument* pXmlDoc);
  static CBasicEntity* createCloneEntity(CBasicEntity* pModelEntity);

};

#endif // !defined(CENTITYFACTORY_INCLUDED_)
