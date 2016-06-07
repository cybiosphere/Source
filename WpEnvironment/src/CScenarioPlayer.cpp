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
// FILE: CScenarioPlayer.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a generic parameter
//     
// (C) COPYRIGHT 2010.  All Rights Reserved.
//
//  26/05/2010 : Creation
//
//===========================================================================

#include "CScenarioPlayer.h"
#include "CAnimal.h"
#include "CGeoMap.h"

//===========================================================================
// Definitions            
//===========================================================================

CommandHandler_t ScenarioCmdNameList[SCENARIO_CMD_NUMBER_TYPE] =
{
// cmd name                      cmd function                                 parameters
  {"RUN_BIOTOP",                 CScenarioPlayer::CmdRunBiotop},           // Time in second.This cmd needs to stay at first
  {"LOAD_BIOTOP",                CScenarioPlayer::CmdLoadBiotop},          // Biotop file name 
  {"CHANGE_BIOTOP_PARAM",        CScenarioPlayer::CmdChangeBiotopParam},   // Param name + value
  {"ADD_ENTITY",                 CScenarioPlayer::CmdAddEntity},           // Entity file name + coord
  {"CHANGE_PARAM",               CScenarioPlayer::CmdChangeParam},         // Entity name + Param name + value
  {"SAVE_ENTITY",                CScenarioPlayer::CmdSaveEntity},          // Entity file name
  {"SAVE_BRAIN",                 CScenarioPlayer::CmdSaveBrain},           // Entity brain file name
  {"QUICK_AGEING",               CScenarioPlayer::CmdQuickAgeing},         // Entity file name + day number
  {"MOVE_ENTITY",                CScenarioPlayer::CmdMoveEntity},          // Entity name + coord
  {"CHECK_PARAM_OVER",           CScenarioPlayer::CmdCheckParamOver},      // Entity name + Param name + value
  {"CHECK_PARAM_UNDER",          CScenarioPlayer::CmdCheckParamUnder},     // Entity name + Param name + value
  {"SET_IMMORTAL",               CScenarioPlayer::CmdSetImmortal},         // Entity name
  {"DELETE_ENTITY",              CScenarioPlayer::CmdDeleteEntity},        // Entity name  
  {"SET_FORBIDEN_ACTION",        CScenarioPlayer::CmdSetForbidenAction},   // Entity name + Action name
  {"CHECK_FORBID_ACT_CNT_UNDER", CScenarioPlayer::CmdChkForbidActCntUnder} // Entity name + value
};

//===========================================================================
// Constructors / Destructors
//===========================================================================

CScenarioPlayer::CScenarioPlayer(CBiotop* pBiotop)
{
  m_pBiotop = pBiotop;
  m_successScore = 0;
  m_totalScore = 0;
  m_remainingTimeToRunBiotop = 0;
}


CScenarioPlayer::~CScenarioPlayer()
{
  if (m_curScenarioFile.is_open())
  {
    m_curScenarioFile.close();
  }
}


bool CScenarioPlayer::ReadScenarioFile(string fileName, string pathName)
{
  bool resu = false;

  // Check if file already opened
  if (m_curScenarioFile.is_open())
  {
    m_curScenarioFile.clear();
    m_curScenarioFile.close();
  }
  
  m_ScenarioPath = pathName;
  string fileNameWithPath = pathName + fileName;

  // Open file
  m_curScenarioFile.open(fileNameWithPath.c_str(), ios::in);

  // Check if file opened properly
  resu = m_curScenarioFile.is_open();

  m_successScore = 0;
  m_totalScore = 0;
  m_remainingTimeToRunBiotop = 0;

  if (resu)
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("SCENAR - New scenario loaded: %s\n", fileName.c_str());
  }
  else
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("SCENAR - ERROR: Fail loading new scenario: %s\n", fileName.c_str());
  }

  return (resu);
}


bool CScenarioPlayer::PlayScenario()
{
  // Run Scenario
  m_successScore = 0;
  m_totalScore = 0;
  m_remainingTimeToRunBiotop = 0;

  bool cmdFound = true;

  while (cmdFound)
  {
    cmdFound = NextCmdNextSecond();
  }

  return (true);
}


bool CScenarioPlayer::NextCmdNextSecond()
{
  if (!m_curScenarioFile.is_open())
    return false;

  // If running biotop, don't parse new cmd
  if (m_remainingTimeToRunBiotop>0)
  {
    m_remainingTimeToRunBiotop--;
    m_pBiotop->nextSecond();
    return true;
  }

  string curLine;
  bool resu = true;
  int startInd;

  getline(m_curScenarioFile, curLine);

  if ( m_curScenarioFile.rdstate() !=  ios::goodbit )
    return false;

  // Check if line is comment
  startInd = curLine.find("#", 0);
  if ( startInd > -1)
  {
    // NO action but command found
    return true;
  }

  // Run biotop cmd
  startInd = curLine.find(ScenarioCmdNameList[0].commandName, 0);
  if ( startInd > -1)
  {
    int cmdSize = strlen(ScenarioCmdNameList[0].commandName);
    int duration = atoi(GetParamFromString(curLine.substr(startInd+cmdSize+1),0).c_str());
    m_remainingTimeToRunBiotop = duration;
    return true;
  }

  // Other cmds
  for(int i=1; i<SCENARIO_CMD_NUMBER_TYPE; i++)
  {
    startInd = curLine.find(ScenarioCmdNameList[i].commandName, 0);
    if ( startInd > -1)
    {
      int cmdSize = strlen(ScenarioCmdNameList[i].commandName);
      resu = ScenarioCmdNameList[i].commandFonction(m_pBiotop, m_ScenarioPath, curLine.substr(startInd+cmdSize+1), &m_successScore, &m_totalScore);
      break;
    }
  }

  if (resu && ( startInd > -1))
  {
    CYBIOCORE_LOG_TIME(m_pBiotop->getBiotopTime());
    CYBIOCORE_LOG("SCENAR - cmd: %s\n", curLine.c_str());
    CYBIOCORE_LOG_FLUSH;
  }

  return resu;
}

bool CScenarioPlayer::ExecuteCmd(CBiotop* pBiotop, string commandString, string biotopFilesPath, int &successScore, int &totalScore)
{
  bool resu = true;
  int startInd = -1;
  for(int i=1; i<SCENARIO_CMD_NUMBER_TYPE; i++)
  {
    startInd = commandString.find(ScenarioCmdNameList[i].commandName, 0);
    if ( startInd > -1)
    {
      int cmdSize = strlen(ScenarioCmdNameList[i].commandName);
      resu = ScenarioCmdNameList[i].commandFonction(pBiotop, biotopFilesPath, commandString.substr(startInd+cmdSize+1), &successScore, &totalScore);
      break;
    }
  }
  return resu;
}

string CScenarioPlayer::GetParamFromString(string commandParam,int paramIndex)
{
  int indexStart=0;
  bool bracketFound = false;

  if (commandParam.c_str()[0] == '\"')
    bracketFound = true;

  for(int i=0;i<paramIndex;i++)
  {
    if (bracketFound)
    {
      indexStart = commandParam.find("\"",indexStart+1) + 2;
      bracketFound = false;
    }
    else
    {
      indexStart = commandParam.find(" ",indexStart+1) + 1;
    }

    if (commandParam.c_str()[indexStart] == '\"')
      bracketFound = true;
  }

  if (indexStart<0)
    return ("");

  int indexEnd;
  if (commandParam.c_str()[indexStart] == '\"')
  {
    indexStart++;
    indexEnd = commandParam.find("\"",indexStart+1);
  }
  else
  {
    indexEnd = commandParam.find(" ",indexStart+1);
  } 
  
  if (indexEnd<0)
    indexEnd = commandParam.size();

  return (commandParam.substr(indexStart, indexEnd-indexStart));
}


bool CScenarioPlayer::CmdLoadBiotop(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  string firstParam = GetParamFromString(commandParam, 0);

  if (firstParam.find(".bio",0) < 0)
    return (false);

  int indexName = firstParam.rfind("\\",firstParam.size());

  if (indexName<0)
    indexName = firstParam.rfind("/",firstParam.size());

  if (indexName>0)
  {
    int sizeName = firstParam.size() - indexName - 1;
    pBiotop->loadFromXmlFile(firstParam.substr(indexName+1, sizeName), firstParam.substr(0,indexName+1));
  }
  else //No path
  {
    pBiotop->loadFromXmlFile(firstParam, path);
  }
  return (true);
}


bool CScenarioPlayer::CmdAddEntity(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  entityIdType entityId = -1;
  CBasicEntity* pEntity;

  string firstParam = GetParamFromString(commandParam, 0);

  if (firstParam.find(".xml",0) < 0)
    return (false);

  int indexName = firstParam.rfind("\\",firstParam.size());

  if (indexName<0)
    indexName = firstParam.rfind("/",firstParam.size());

  int indexEndName = firstParam.find(".xml",0) + 3;

  Point_t coord;
  coord.x = atoi(GetParamFromString(commandParam,1).c_str());
  coord.y = atoi(GetParamFromString(commandParam,2).c_str());

  if (indexName>0)
  {
    int sizeName = firstParam.size() - indexName - 1;
    entityId = pBiotop->createAndAddEntity(firstParam.substr(indexName+1, sizeName), firstParam.substr(0,indexName+1), coord);
  }
  else //No path
  {
    entityId = pBiotop->createAndAddEntity(firstParam, path, coord);
  }

  pEntity = pBiotop->getEntityById(entityId);

  return (true);
}


bool CScenarioPlayer::CmdRunBiotop(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  int duration = atoi(GetParamFromString(commandParam,0).c_str());

  for (int i=0;i<duration;i++)
    pBiotop->nextSecond();

  return (true);
}


bool CScenarioPlayer::CmdChangeBiotopParam(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string paramName  = GetParamFromString(commandParam, 0);
  double paramValue = atof(GetParamFromString(commandParam,1).c_str());

  CGenericParam* pParam = pBiotop->getParameterByName(paramName);
  if (pParam!=NULL)
  {
      pParam->forceVal(paramValue);
      resu = true;
  }

  return (true); // return always true for not stopping scenario
}


bool CScenarioPlayer::CmdChangeParam(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  string paramName  = GetParamFromString(commandParam, 1);
  double paramValue = atof(GetParamFromString(commandParam,2).c_str());

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    CGenericParam* pParam = pEntity->getParameterByName(paramName);
    if (pParam != NULL)
    {
      pParam->forceVal(paramValue);
      resu = true;
    }
  }

  return (true); // return always true for not stopping scenario
}


bool CScenarioPlayer::CmdSaveEntity(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  CBasicEntity *pEnt = NULL;
  string firstParam = GetParamFromString(commandParam, 0);
  string nameWithPath;

  if (firstParam.find(".xml",0) < 0)
    return (false);

  int indexName = firstParam.rfind("\\",firstParam.size());

  if (indexName<0)
    indexName = firstParam.rfind("/",firstParam.size());

  int indexEndName = firstParam.find(".xml",0) + 3;

  if (indexName>0)
  {
    int sizeNameNoExt = firstParam.size() - indexName - 5;
    CBasicEntity *pEnt = pBiotop->getEntityByName(firstParam.substr(indexName+1, sizeNameNoExt));
    nameWithPath = firstParam;
  }
  else // No path
  {
    int sizeNameNoExt = firstParam.size() - 4;
    pEnt = pBiotop->getEntityByName(firstParam.substr(0, sizeNameNoExt));
    nameWithPath = path + firstParam;
  }

  if (pEnt != NULL)
  {
    int sizeName = firstParam.size() - indexName - 1;
    pEnt->saveInXmlFile(nameWithPath.c_str(), firstParam.substr(0,indexName+1));
  }

  return (true);
}

bool CScenarioPlayer::CmdSaveBrain(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  CBasicEntity *pEnt = NULL;
  string firstParam = GetParamFromString(commandParam, 0);
  string nameWithPath;

  if (firstParam.find(".xml",0) < 0)
    return (false);

  int indexName = firstParam.rfind("\\",firstParam.size());

  if (indexName<0)
    indexName = firstParam.rfind("/",firstParam.size());

  int indexEndName = firstParam.find(".xml",0) + 3;

  if (indexName>0)
  {
    int sizeNameNoExt = firstParam.size() - indexName - 5;
    pEnt = pBiotop->getEntityByName(firstParam.substr(indexName+1, sizeNameNoExt));
    nameWithPath = firstParam;
  }
  else // No path
  {
    int sizeNameNoExt = firstParam.size() - 4;
    pEnt = pBiotop->getEntityByName(firstParam.substr(0, sizeNameNoExt));
    nameWithPath = path + firstParam;
  }

  if (pEnt != NULL)
  {
    int sizeName = firstParam.size() - indexName - 1;
    pEnt->getBrain()->saveInXmlFile(nameWithPath.c_str());
  }

  return (true);
}

bool CScenarioPlayer::CmdQuickAgeing(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  int paramValue = atoi(GetParamFromString(commandParam,1).c_str());

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    // Save current brain
    TiXmlDocument xmlDocSavedBrain;
    pEntity->getBrain()->saveInTiXmlFile(&xmlDocSavedBrain);

    // Reset entity
    entityIdType oldId = pEntity->getId();
    CGenome* pNewGenome = new CGenome(*pEntity->getGenome());
    pBiotop->resetEntityGenome(oldId, pNewGenome);
    CBasicEntity* pNewEntity = pBiotop->getEntityById(oldId);

    // Go to required age
    pNewEntity->quickAgeing(paramValue);

    // Reconfigure original brain
    pNewEntity->getBrain()->loadFromTiXmlFile(&xmlDocSavedBrain);
  }

  return (true); // return always true for not stopping scenario
}

bool CScenarioPlayer::CmdMoveEntity(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  Point_t coord;
  coord.x = atoi(GetParamFromString(commandParam,1).c_str());
  coord.y = atoi(GetParamFromString(commandParam,2).c_str());
  int direction = atoi(GetParamFromString(commandParam,3).c_str()); 

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    pEntity->jumpToGridCoord(coord);
    resu = true;
    if ( (direction>=0) && (direction<=7) )
      pEntity->setDirection(direction);

    // Set home position if needed
    if (pEntity->getBrain() != NULL)
    {
      pEntity->getBrain()->SetHomePurposePositionInGeoMap();
    }
  }

  return (true); // return always true for not stopping scenario
}

bool CScenarioPlayer::CmdCheckParamOver(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  string paramName  = GetParamFromString(commandParam, 1);
  double paramValue = atof(GetParamFromString(commandParam,2).c_str());
  CGenericParam* pParam = NULL;

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    pParam = pEntity->getParameterByName(paramName);
    if (pParam != NULL)
    {
      if (pParam->getVal() >= paramValue)
      {
        (*pSuccessScore)++; // Check has succeed
        CYBIOCORE_LOG_TIME(pBiotop->getBiotopTime());
        CYBIOCORE_LOG("SCENAR - Success chk%d: parameter %s value=%f is over %f\n", *pTotalScore, pParam->getLabel().c_str(), pParam->getVal(), paramValue);
        resu = true;
      }
    }
  }

  if ((!resu) && (pParam != NULL))
  {
    CYBIOCORE_LOG_TIME(pBiotop->getBiotopTime());
    CYBIOCORE_LOG("SCENAR - Failure chk%d: parameter %s value=%f is under %f\n", *pTotalScore, pParam->getLabel().c_str(), pParam->getVal(), paramValue);
  }

  (*pTotalScore)++;
  return (true); // return always true for not stopping scenario
}

bool CScenarioPlayer::CmdCheckParamUnder(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  string paramName  = GetParamFromString(commandParam, 1);
  double paramValue = atof(GetParamFromString(commandParam,2).c_str());
  CGenericParam* pParam = NULL;

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    pParam = pEntity->getParameterByName(paramName);
    if (pParam != NULL)
    {
      if (pParam->getVal() <= paramValue)
      {
        (*pSuccessScore)++; // Check has succeed
        CYBIOCORE_LOG_TIME(pBiotop->getBiotopTime());
        CYBIOCORE_LOG("SCENAR - Success chk%d: parameter %s value=%f is under %f\n", *pTotalScore, pParam->getLabel().c_str(), pParam->getVal(), paramValue);
        resu = true;
      }
    }
  }

  if ((!resu) && (pParam != NULL))
  {
    CYBIOCORE_LOG_TIME(pBiotop->getBiotopTime());
    CYBIOCORE_LOG("SCENAR - Failure chk%d: parameter %s value=%f is over %f\n", *pTotalScore, pParam->getLabel().c_str(), pParam->getVal(), paramValue);
  }

  (*pTotalScore)++;
  return (true); // return always true for not stopping scenario
}

bool CScenarioPlayer::CmdSetImmortal(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    pEntity->setImmortal(true);
    resu = true;
  }

  return (true); // return always true for not stopping scenario
}

bool CScenarioPlayer::CmdDeleteEntity(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  Point_t coord;
  coord.x = atoi(GetParamFromString(commandParam,1).c_str());
  coord.y = atoi(GetParamFromString(commandParam,2).c_str());

  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if (pEntity != NULL)
  {
    pEntity->autoRemove();
    resu = true;
  }

  return (true); // return always true for not stopping scenario
}


bool CScenarioPlayer::CmdSetForbidenAction   (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  string actionName = GetParamFromString(commandParam, 1);
  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if ( (pEntity != NULL) && (pEntity->getBrain() != NULL) )
  {
    int actionInd = pEntity->getBrain()->GetReactionIndexByLabel(actionName);
    ((CAnimal*)pEntity)->setForbidenActionInd(actionInd);
    resu = true;
  }

  return (true); // return always true for not stopping scenario
}


bool CScenarioPlayer::CmdChkForbidActCntUnder(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore)
{
  bool resu = false;
  string entityName = GetParamFromString(commandParam, 0);
  int maxCount = atoi(GetParamFromString(commandParam,1).c_str());
  CBasicEntity* pEntity = pBiotop->getEntityByName(entityName);

  if ( (pEntity != NULL) && (pEntity->getBrain() != NULL) )
  {
    if (((CAnimal*)pEntity)->getForbidenActionCount() < maxCount)
    {
      (*pSuccessScore)++; // Check has succeed
      CYBIOCORE_LOG_TIME(pBiotop->getBiotopTime());
      CYBIOCORE_LOG("SCENAR - Success chk%d: Forbid action %s called =%d under %d\n", *pTotalScore, ((CAnimal*)pEntity)->getForbidenActionLabel().c_str(), ((CAnimal*)pEntity)->getForbidenActionCount(), maxCount);
      resu = true;
    }
  }

  if ((!resu) && (pEntity != NULL) && (pEntity->getBrain() != NULL))
  {
    CYBIOCORE_LOG_TIME(pBiotop->getBiotopTime());
    CYBIOCORE_LOG("SCENAR - Failure chk%d: Forbid action %s called =%d over %d\n", *pTotalScore, ((CAnimal*)pEntity)->getForbidenActionLabel().c_str(), ((CAnimal*)pEntity)->getForbidenActionCount(), maxCount);
  }

  (*pTotalScore)++;
  return (true); // return always true for not stopping scenario
}

bool CScenarioPlayer::IsScenarioFileOpened()
{
  return (bool)m_curScenarioFile.is_open();
}


