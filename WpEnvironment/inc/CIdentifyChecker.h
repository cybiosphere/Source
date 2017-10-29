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
// FILE: CIdentifyChecker.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a cyclic parameter
//     
// (C) COPYRIGHT 2010.  All Rights Reserved.
//
//  26/05/2010 : Creation
//
//===========================================================================

#if !defined(CIDENTIFYCHECKER_INCLUDED_)
#define CIDENTIFYCHECKER_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================

#include "CBiotop.h"
#include <fstream>


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CIdentifyChecker
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
   CBasicEntity*  m_pEntity;
   ifstream       m_curCheckerFile; 
   string         m_CheckerPath;
   string         m_currentCheck;
   string         m_resultInfo;

   int m_successScore;
   int m_totalScore;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CIdentifyChecker(CBasicEntity* pEntity);
	virtual ~CIdentifyChecker();

public:
  bool ReadCheckerFile(string fileName, string pathName);
  bool StartCheck();
  bool NextCheck();

  int GetSuccessScore()  {return m_successScore;};
  int GetTotalScore()    {return m_totalScore;};
  string GetCurrentCheck() {return m_currentCheck;};
  string GetResultInfo() { return m_resultInfo;};

private:
  bool checkIdentify(int level, string identityStr, std::vector<std::pair<std::string, int>> vectorExpected);
};

#endif // !defined(CIDENTIFYCHECKER_INCLUDED_)
