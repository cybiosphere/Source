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

  getStringSectionFromFile("CYBIOSPHERE", "Biotop", "", resuBuffer, 512, fileIni);
  string biotopName = resuBuffer;
  clearWindowsEolIfNeeded(biotopName);

  if (biotopName != "")
  {
    string resuDataPath;
    pBiotop = new CBiotop(0, 0, 0);
    getStringSectionFromFile("CYBIOSPHERE", "DataPath", "", resuBuffer, 512, fileIni);
    resuDataPath = resuBuffer;
    clearWindowsEolIfNeeded(resuDataPath);
    if (resuDataPath != "")
      pBiotop->loadFromXmlFile(biotopName, resuDataPath);
    else
      pBiotop->loadFromXmlFile(biotopName, "..\\dataXml\\");
  }
  else
  {
    pBiotop = new CBiotop(100, 100, 3);
    pBiotop->setDefaultMapAndEntities();
  }

  pScenarioPlayer = new CScenarioPlayer(pBiotop);
  getStringSectionFromFile("CYBIOSPHERE", "Scenario", "", resuBuffer, 512, fileIni);
  string scenarioName = resuBuffer;
  clearWindowsEolIfNeeded(scenarioName);

  if (scenarioName != "")
  {
    string resuDataPath;
    getStringSectionFromFile("CYBIOSPHERE", "DataPath", "", resuBuffer, 512, fileIni);
    resuDataPath = resuBuffer;
    clearWindowsEolIfNeeded(resuDataPath);
    if (resuDataPath != "")
      pScenarioPlayer->ReadScenarioFile(scenarioName, resuDataPath);
    else
      pScenarioPlayer->ReadScenarioFile(scenarioName, "..\\dataXml\\");

    // Start reading scenario twice to update biotop
    pScenarioPlayer->NextCmdNextSecond();
    pScenarioPlayer->NextCmdNextSecond();
  }

  string resuSpecieName;
  getStringSectionFromFile("CYBIOSPHERE", "AutoSaveSpecie", "", resuBuffer, 512, fileIni);
  resuSpecieName = resuBuffer;
  clearWindowsEolIfNeeded(resuSpecieName);
  if (resuSpecieName != "")
    pBiotop->setAutoSaveSpecieName(resuSpecieName);

  *pNewBiotop = pBiotop;
  *pNewScenarioPlayer = pScenarioPlayer;
}

inline double getStartupSpeedFromIniFile(const string& fileIni)
{
  char resuBuffer[512];
  getStringSectionFromFile("CYBIOSPHERE", "StartupSpeed", "1", resuBuffer, 512, fileIni);
  return atoi(resuBuffer);
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
