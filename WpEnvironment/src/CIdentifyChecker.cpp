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
// FILE: CIdentifyChecker.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2010.  All Rights Reserved.
//
//  26/05/2010 : Creation
//
//===========================================================================
#include "CIdentifyChecker.h"
#include "CAnimal.h"

//===========================================================================
// Definitions            
//===========================================================================


//===========================================================================
// Constructors / Destructors
//===========================================================================

CIdentifyChecker::CIdentifyChecker(CBasicEntity* pEntity)
{
   m_pEntity = pEntity;
   m_successScore = 0;
   m_totalScore = 0;
   m_resultInfo = "";
}


CIdentifyChecker::~CIdentifyChecker()
{
  if (m_curCheckerFile.is_open())
  {
    m_curCheckerFile.close();
  }
}


bool CIdentifyChecker::ReadCheckerFile(string fileName, string pathName)
{
  bool resu = false;

  // Check if file already opened
  if (m_curCheckerFile.is_open())
  {
    m_curCheckerFile.clear();
    m_curCheckerFile.close();
  }
  
  m_CheckerPath = pathName;
  string fileNameWithPath = pathName + fileName;

  // Open file
  m_curCheckerFile.open(fileNameWithPath.c_str(), ios::in);

  // Check if file opened properly
  resu = m_curCheckerFile.is_open();

  m_successScore = 0;
  m_totalScore = 0;
  m_currentCheck = "";

  if (resu)
  {
    CYBIOCORE_LOG("CHECK - New check loaded: %s\n", fileName.c_str());
  }
  else
  {
    CYBIOCORE_LOG("CHECK - ERROR: Fail loading new check file: %s\n", fileName.c_str());
  }

  return (resu);
}


bool CIdentifyChecker::StartCheck()
{
  // Run Scenario
  m_successScore = 0;
  m_totalScore = 0;
  m_currentCheck = "";
  m_resultInfo = "";

  bool cmdFound = true;

  while (cmdFound)
  {
    cmdFound = NextCheck();
  }

  return (true);
}

bool CIdentifyChecker::checkIdentify (int level, string identityStr, std::vector<std::pair<std::string, int>> vectorExpected)
{
  bool found = false;
  for (std::pair<std::string, int>& pairChk : vectorExpected)
  {
    if (identityStr == pairChk.first)
    {
      if (level == pairChk.second)
      {
        found = true;
        m_resultInfo += FormatString("CHECK - Identity %s level %d: OK\n", identityStr.c_str(), level);
        CYBIOCORE_LOG("CHECK - Identity %s level %d: OK\n", identityStr.c_str(), level);
        m_successScore++;
      }
      else
      {
        found = true;
        m_resultInfo += FormatString("CHECK - Identity %s level %d: WARN (expected level = %d)\n", identityStr.c_str(), level, pairChk.second);
        CYBIOCORE_LOG("CHECK - Identity %s level %d: WARN (expected level = %d)\n", identityStr.c_str(), level, pairChk.second);
      }
    }
  }

  if (!found)
  {
    m_resultInfo += FormatString("CHECK - Identity %s level %d: ERR\n", identityStr.c_str(), level);
    CYBIOCORE_LOG("CHECK - Identity %s level %d: ERR\n", identityStr.c_str(), level);
  }

  m_totalScore++;

  return true;
}

bool CIdentifyChecker::NextCheck()
{
  if (!m_curCheckerFile.is_open())
    return false;

  string curLine;
  bool resu = true;
  int startInd;

  getline(m_curCheckerFile, curLine);

  if ( m_curCheckerFile.rdstate() !=  ios::goodbit )
    return false;

  // Check if line is comment
  startInd = curLine.find("#", 0);
  if ( startInd > -1)
  {
    // NO action but command found
    return true;
  }

  // memorize command
  m_currentCheck = curLine;

  // Parse Check line: FileName; identity1 val1; identity2 val2;....
  std::vector<std::pair<std::string, int>> vectorIdentify;
  int indexStart = 0, indexEnd = 0, indexEndLine = 0, indexSpace = 0, readLevel = 0;
  indexEndLine = curLine.length();
  string readStr, readIdentity;
  indexEnd = curLine.find(";", indexStart);

  if (indexEnd < 1)
    return false;

  readStr = curLine.substr(indexStart, indexEnd - indexStart);

  string fileNameWithPath = m_CheckerPath + readStr;
  TiXmlDocument *pXmlDoc = new TiXmlDocument(fileNameWithPath);
  if (!pXmlDoc->LoadFile())
  {
    CYBIOCORE_LOG("CHECK - Error reading entity file: %s\n", fileNameWithPath.c_str());
    delete pXmlDoc;
    return true;
  }
  CBasicEntity* pTargetEntity = CBiotop::createEntity(pXmlDoc,"");
  delete pXmlDoc;

  if (pTargetEntity == NULL)
  {
    CYBIOCORE_LOG("CHECK - Error: cmd: %s \n", curLine.c_str());
    return true;
  }

  indexStart = indexEnd + 1;
  // parse 1st row. count nb of columns
  while (indexStart<indexEndLine)
  {
    indexEnd = curLine.find(";", indexStart);
    readStr = curLine.substr(indexStart, indexEnd - indexStart);
    indexSpace = readStr.find(" ");
    readIdentity = readStr.substr(0, indexSpace);
    readLevel = atoi(readStr.substr(indexSpace + 1, 1).c_str());
    vectorIdentify.push_back(std::make_pair(readIdentity, readLevel));
    indexStart = indexEnd + 1;
  }

  m_pEntity->getBrain()->ComputeAndGetIdentification(pTargetEntity, false);
  CMatrix* pOutVector = m_pEntity->getBrain()->GetIdentifyOutputVect();
  double highThreshold = 0, midThreshold = 0, lowThreshold = 0;
  m_pEntity->getBrain()->GetVectorIdentifyThresholds(highThreshold, midThreshold, lowThreshold);

  m_resultInfo += FormatString("CHECK - entity=%s\n", pTargetEntity->getLabel().c_str());
  CYBIOCORE_LOG("CHECK - entity=%s\n", pTargetEntity->getLabel().c_str());

  double curVal = 0;
  for (int i = 0; i < pOutVector->RowNo(); i++)
  {
    curVal = (*pOutVector)(i, 0);
    if (curVal >= highThreshold)
      checkIdentify(1, m_pEntity->getBrain()->getIdentificationLabel(i).c_str(), vectorIdentify);
    else if (curVal > midThreshold)
      checkIdentify(2, m_pEntity->getBrain()->getIdentificationLabel(i).c_str(), vectorIdentify);
    else if (curVal > lowThreshold)
      checkIdentify(3, m_pEntity->getBrain()->getIdentificationLabel(i).c_str(), vectorIdentify);
  }

  return true;
}

