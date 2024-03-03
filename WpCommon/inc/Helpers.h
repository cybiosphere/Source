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

inline string getIpServerAddrFromIniFile(const string& fileIni)
{
  string ServerAddrStr;
  char resuBuffer[512];
  getStringSectionFromFile("CYBIOSPHERE", "ServerAddr", "localhost", resuBuffer, 512, fileIni);
  ServerAddrStr = resuBuffer;
  return ServerAddrStr;
}

inline string getIpServerLoginFromIniFile(const string& fileIni)
{
  string LogInStr;
  char resuBuffer[512];
  getStringSectionFromFile("CYBIOSPHERE", "Login", "Player", resuBuffer, 512, fileIni);
  LogInStr = resuBuffer;
  return LogInStr;
}

inline string getIpPortStrFromIniFile(const string& fileIni)
{
  string serverPortStr;
  char resuBuffer[512];
  if (getStringSectionFromFile("CYBIOSPHERE", "ServerPort", "4556", resuBuffer, 512, fileIni) > 0)
  {
    serverPortStr = resuBuffer;
  }
  else
  {
    serverPortStr = "4556";
  }
  return serverPortStr;
}

inline void createBiotopAndScenarioFromIniFile(const string& fileIni, CBiotop** pNewBiotop, CScenarioPlayer** pNewScenarioPlayer)
{
  CBiotop* pBiotop{ nullptr };
  CScenarioPlayer* pScenarioPlayer{ nullptr };
  char resuBuffer[512];
  string resuStr;

  int resu = getStringSectionFromFile("CYBIOSPHERE", "Biotop", "", resuBuffer, 512, fileIni);
  resuStr = resuBuffer;
  if (resuStr != "")
  {
    string resuDataPath;
    pBiotop = new CBiotop(0, 0, 0);
    bool resu = getStringSectionFromFile("CYBIOSPHERE", "DataPath", "", resuBuffer, 512, fileIni);
    resuDataPath = resuBuffer;
    if (resuDataPath != "")
      pBiotop->loadFromXmlFile(resuStr, resuDataPath);
    else
      pBiotop->loadFromXmlFile(resuStr, "..\\dataXml\\");
  }
  else
  {
    pBiotop = new CBiotop(50, 50, 3);
    pBiotop->initGridDefaultLayerType();
    pBiotop->initGridDefaultAltitude();
    pBiotop->initGridEntity();
    pBiotop->setDefaultEntitiesForTest();
  }

  resu = getStringSectionFromFile("CYBIOSPHERE", "Scenario", "", resuBuffer, 512, fileIni);
  resuStr = resuBuffer;
  if (resuStr != "")
  {
    string resuDataPath;
    pScenarioPlayer = new CScenarioPlayer(pBiotop);
    bool resu = getStringSectionFromFile("CYBIOSPHERE", "DataPath", "", resuBuffer, 512, fileIni);
    resuDataPath = resuBuffer;
    if (resuDataPath != "")
      pScenarioPlayer->ReadScenarioFile(resuStr, resuDataPath);
    else
      pScenarioPlayer->ReadScenarioFile(resuStr, "..\\dataXml\\");

    // Start reading scenario twice to update biotop
    pScenarioPlayer->NextCmdNextSecond();
    pScenarioPlayer->NextCmdNextSecond();
  }

  *pNewBiotop = pBiotop;
  *pNewScenarioPlayer = pScenarioPlayer;
}

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
