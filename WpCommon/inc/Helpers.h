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


#if !defined(HELPERS_INCLUDED_)
#define HELPERS_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CBiotop.h"

//===========================================================================
// Functions            
//===========================================================================

inline size_t computeMaxSpeedStepfactor(CBiotop* pBiotop, bool isMaxSpeed)
{
  if (pBiotop == NULL)
    return 1;
  size_t nbAnimals = pBiotop->getNbOfAnimals();
  if ((isMaxSpeed) && (nbAnimals < 100))
  {
    return (nbAnimals == 0) ? 100 : 10 - nbAnimals / 10;
  }
  else
  {
    return 1;
  }
}

#endif // !defined(HELPERS_INCLUDED_)
