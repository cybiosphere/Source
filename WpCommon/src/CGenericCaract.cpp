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
// FILE: CGenericCaract.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic caracter
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  06/11/2011 : Creation
//
//===========================================================================

#include "CGenericCaract.h"

//===========================================================================
// Constructors / Destructors
//===========================================================================

CGenericCaract::CGenericCaract(string label, DWORD initPresenceMask)
{
  m_Label = label;
  m_PresenceMask = initPresenceMask;
}

CGenericCaract::~CGenericCaract()
{

}

//===========================================================================
// Get / Set
//===========================================================================
DWORD CGenericCaract::getPresenceMask(void)
{
  return m_PresenceMask;
}

void  CGenericCaract::setPresenceMask(DWORD newMask)
{
  m_PresenceMask = newMask;
}

bool  CGenericCaract::isCaractPresent(unsigned char index)
{
  if ((m_PresenceMask & (1<<index)) == 0)
    return false;
  else
    return true;
}

void  CGenericCaract::setCaractPresent(unsigned char index)
{
  m_PresenceMask |= 1<<index;
}

void  CGenericCaract::setCaractAbsent(unsigned char index)
{
  m_PresenceMask &= ~(1 << index);
}

bool  CGenericCaract::isPresenceMaskEmpty()
{
  if (m_PresenceMask == CARACT_PRESENCE_MASK_EMPTY)
    return true;
  else
    return false;
}

void CGenericCaract::removeAllCaracters()
{
  m_PresenceMask = CARACT_PRESENCE_MASK_EMPTY;
}