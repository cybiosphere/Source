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
// FILE: CMineral.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a mineral entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined( CMINERAL_INCLUDED_)
#define CMINERAL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
 
#include "CBasicEntity.h"

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CMineral : public CBasicEntity 
{
     
//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------  
public:
  CMineral(string label, Point_t initCoord, size_t layer, CGenome* pPseudoGenome);
  string buildLifeStageString(CGene* pGen);

  bool isMineral();
};// end CMineral

#endif // !defined(CMINERAL_INCLUDED_)


