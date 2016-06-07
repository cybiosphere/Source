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
// FILE: CScenarioPlayer.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a cyclic parameter
//     
// (C) COPYRIGHT 2010.  All Rights Reserved.
//
//  26/05/2010 : Creation
//
//===========================================================================

#if !defined(CSCENARIOPLAYER_INCLUDED_)
#define CSCENARIOPLAYER_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================

#include "CBiotop.h"
#include <fstream>

#define SCENARIO_CMD_NUMBER_TYPE 15

typedef struct 
{
  const char*   commandName;
  bool (*commandFonction)(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
} CommandHandler_t;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CScenarioPlayer
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
   CBiotop*  m_pBiotop;
   ifstream  m_curScenarioFile; 
   string    m_ScenarioPath;

public:
   int m_successScore;
   int m_totalScore;
   int m_remainingTimeToRunBiotop;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CScenarioPlayer(CBiotop* pBiotop);
	virtual ~CScenarioPlayer();

public:
  bool ReadScenarioFile(string fileName, string pathName);
  bool PlayScenario();
  bool NextCmdNextSecond();
  bool IsScenarioFileOpened();

  static bool ExecuteCmd(CBiotop* pBiotop, string commandString, string biotopFilesPath, int &successScore, int &totalScore);
  static string GetParamFromString(string commandParam,int paramIndex);

  static bool CmdLoadBiotop          (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdAddEntity           (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdRunBiotop           (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdChangeBiotopParam   (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdChangeParam         (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdSaveEntity          (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdSaveBrain           (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdQuickAgeing         (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdMoveEntity          (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdCheckParamOver      (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdCheckParamUnder     (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdSetImmortal         (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdDeleteEntity        (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdSetForbidenAction   (CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);
  static bool CmdChkForbidActCntUnder(CBiotop* pBiotop, string path, string commandParam, int* pSuccessScore, int* pTotalScore);

  int GetSuccessScore() {return m_successScore;};
  int GetTotalScore()   {return m_totalScore;};
};

#endif // !defined(CSCENARIOPLAYER_INCLUDED_)
