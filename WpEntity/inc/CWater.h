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
// FILE: CWater.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic water element
//         This entity should not be attached to biotop. 1 instance only 
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined( CWATER_INCLUDED_)
#define CWATER_INCLUDED_

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
class DLL_CYBIOCORE_API CWater:public CBasicEntity 
{
//===========================================================================
// Attributes 
//===========================================================================
  bool m_bInfiniteContent;

//===========================================================================
// methods 
//===========================================================================
protected:
  bool completeCaractsWithDefault();

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------     
public:
  CWater();
  CWater(string label, Point_t initCoord, CGenome* pPseudoGenome);
  CWater(string label, CWater& model);
  double  changeWeight(double variation); // Overload
  ClassType_e getClass();
  void nextHour();
};// end CWater

#endif // !defined(CWATER_INCLUDED_)


