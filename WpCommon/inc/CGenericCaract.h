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
// FILE: CGenericCaract.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic caracter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  06/11/2011 : Creation
//
//===========================================================================

#if !defined(CGENERICCARACT_INCLUDED_)
#define CGENERICCARACT_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>

#include "Definitions.h"
#include "CGene.h"

//===========================================================================
// Definitions            
//===========================================================================
#define CARACT_PRESENCE_MASK_EMPTY 0x00000000

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CGenericCaract  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  string m_Label;        // User defined name
  DWORD  m_PresenceMask;


//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CGenericCaract(string label, DWORD initPresenceMask = CARACT_PRESENCE_MASK_EMPTY);
	virtual ~CGenericCaract();

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  DWORD getPresenceMask(void);
  void  setPresenceMask(DWORD newMask);
  bool  isCaractPresent(unsigned char index);
  void  setCaractPresent(unsigned char index);
  void  setCaractAbsent(unsigned char index);
  bool  isPresenceMaskEmpty();
  void  removeAllCaracters();
};

#endif // !defined(CGENERICCARACT_INCLUDED_)
