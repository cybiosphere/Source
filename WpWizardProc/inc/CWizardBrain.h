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

// CBrainWizard.h: interface for the CBrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(WIZARD_BRAIN_H_INCLUDED_)
#define WIZARD_BRAIN_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Definitions.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include <afxtempl.h>

typedef struct 
{
  DWORD  sensorUniqueId;
  int    sensorUniqueIdRange;
  int    subCaptorIndex;
  DWORD  reactionUniqueId;
  int    reactionUniqueIdRange;
  int    timeHistory;   // from 0 to -10
  int    bonusRate;  // from -100 to 100 :-100 max malus, 100 max bonus

} BrainMatrixBonus_t;

typedef struct 
{
  DWORD                 sensorUniqueId; // view/smell/pheromon
  int                   subCaptorIndex;
  IdentificationType_e  identity;
  int                   bonusRate;  // from -100 to 100 :-100 max malus, 100 max bonus

} IdentificationMatrixBonus_t;

class CWizardBrain  
{
public:
	CWizardBrain(CAnimal* pAnimal);
	virtual ~CWizardBrain();

    int  ApplyDefaultBonusOnBrain();
    bool ApplyBonusListOnBrain();

    bool AddBrainBonusInList( DWORD sensorUId, int offset, int suboffset, DWORD reactionUniqueId, int reactionRange, int bonus);
    bool AddIdentifyBonusInList( DWORD sensorUId, int offset, IdentificationType_e  identity, int bonus);

    bool AddVisualObstacle(  int level, size_t layer,
                             FormType_e form, ColorCaracterType_e color, 
                             TextureType_e texture, PhyAttributeType_e attribute, bool justInFront = false);

    bool AddVisualIdentifyObstacle ( int level,
                                     size_t layer,
                                     IdentificationType_e identity, 
                                     bool isMoving, bool isDirLft, bool isDirRight,
                                     bool justInFront);

    bool AddVisualBonus ( int bonusRate, size_t layer,
                          FormType_e form, ColorCaracterType_e color, 
                          TextureType_e texture, PhyAttributeType_e attribute, bool isMoving, bool doRun = false,
                          DWORD targetReacUid = UID_BASE_REACT_NOTHING, int targetReacRange = 1); // default: stop in front of target


    bool AddVisualIdentifyBonus ( int bonusRate,
                                  size_t layer,
                                  IdentificationType_e identity, 
                                  bool isMoving = false, bool isDirLeft = false, bool isDirRight = false,
                                  bool doRun = false,
                                  DWORD targetReacUid = UID_BASE_REACT_NOTHING,
                                  int targetReacRange = 1);

    bool AddOdorBonus ( int bonusRate, OdorType_e odor, DWORD targetReacUid, int targetReacRange);
    bool AddPheromoneBonus (int bonusRate, PheromoneType_e phero, DWORD targetReacUid, int targetReacRange);
    bool AddHormoneBonus (int bonusRate, PheromoneType_e phero, DWORD targetReacUid, int targetReacRange);

    bool AddDefaultSexualBonus(int bonusRate);
    bool AddDefaultMammalBonus(int bonusRate);

    bool AddVisualIdentificationBonus( int bonusRate, FormType_e form, ColorCaracterType_e color, 
                                       TextureType_e texture, PhyAttributeType_e attribute,  
                                       bool isMoving, bool isBigger, bool isSmaller,
                                       IdentificationType_e  identity);
    bool AddOdorIdentificationBonus( int bonusRate, OdorType_e odor, IdentificationType_e  identity);
    bool AddPheromoneIdentificationBonus (int bonusRate, PheromoneType_e phero, IdentificationType_e  identity);

private:
    CBrain*   m_pBrain;
    CAnimal*  m_pAnimal;

    CArray<BrainMatrixBonus_t*,BrainMatrixBonus_t*> m_tBrainMatrixBonus;
    CArray<IdentificationMatrixBonus_t*,IdentificationMatrixBonus_t*> m_tIdentificationMatrixBonus;

    bool ApplyBrainBonus(BrainMatrixBonus_t* pBonus);
    bool ApplyIdentifyBonus(IdentificationMatrixBonus_t* pBonus);
    bool ApplyMemoryFeedbackBonus(int level);
    bool ApplyMemoryActionBonus(int level);

    int  ApplyBrainBonusList();
    int  ApplyIdentifyBonusList();
    bool CompleteViewSecondaryEntities(void);


protected:


};

#endif // !defined(WIZARD_BRAIN_H_INCLUDED_)
