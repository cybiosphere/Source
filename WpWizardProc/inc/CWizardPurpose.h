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

// CWizardPurpose.h: interface for the CBrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(WIZARD_PURPOSE_H_INCLUDED_)
#define WIZARD_PURPOSE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Definitions.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"

#include <afx.h>
#include <afxtempl.h>


#define WZ_MAX_SENSOR_BONUS_PURPOSE     10
#define WZ_MAX_REACTION_BONUS_PURPOSE   10

typedef struct 
{
  DWORD                 sensorUniqueId;
  int                   sensorUniqueIdRange;
  int                   nbBonus;
  short*                pBonusTable;
} PurposeSensorBonus_t;

typedef struct 
{
  DWORD                 reactionUniqueId;
  int                   reactionUniqueIdRange;
  int                   bonus;
} PurposeReactionBonus_t;

typedef struct 
{
  int                       chromosomePaireIndex;
  CString                   purposeName;
  int                       purposeDuration;
  PurposeTriggerType_e      triggerType;
  DWORD                     triggerSensorUniqueId;
  int                       triggerSensorUniqueIdRange;
  int                       triggerSubCaptorIndex;
  int                       triggerStartThreshold;
  int                       triggerStopThreshold;
  int                       nbSensorBonus;
  PurposeSensorBonus_t      tSensorBonus[WZ_MAX_SENSOR_BONUS_PURPOSE];
  int                       nbReactionBonus;
  PurposeReactionBonus_t    tReactionBonus[WZ_MAX_REACTION_BONUS_PURPOSE];
} PurposeDefinition_t;


class CWizardPurpose  
{
public:
	CWizardPurpose(CAnimal* pAnimal);
	virtual ~CWizardPurpose();

    bool AddAllPurposeGenesToAnimal(int mutationRate);

    bool AddPurposeInList ( int                  chromosomePaireIndex,
                            CString              purposeName,
                            int                  purposeDuration,
                            PurposeTriggerType_e triggerType,
                            DWORD                triggerSensorUniqueId,
                            int                  triggerSensorUniqueIdRange,
                            int                  triggerSubCaptorIndex,
                            int                  triggerStartThreshold,
                            int                  triggerStopThreshold);

    // Generic fonctions
    bool AddPurposeSensorBonus( CString purposeName, 
                                DWORD sensorUniqueId, int sensorUniqueIdRange,
                                int nbBonus, short* pBonusTable );

    bool AddPurposeReactionBonus( CString purposeName, 
                                  DWORD reactionUniqueId, int reactionUniqueIdRange,
                                  int bonusRateX10 );

    // High level fonctions
    bool AddPurposeVisualBonus( CString purposeName, int bonus, size_t layer,
                                FormType_e form, ColorCaracterType_e color, 
                                TextureType_e texture, PhyAttributeType_e attribute, bool isMoving);
    bool AddPurposeVisualIdentifyBonus( CString purposeName, int bonus, size_t layer,
                                        IdentificationType_e identity, 
                                        bool isMoving, bool isDirLeft, bool isDirRight);
    bool AddPurposeOdorBonus ( CString purposeName, int bonus, OdorType_e odor);
    bool AddPurposePheromoneBonus ( CString purposeName, int bonus, PheromoneType_e phero);
    bool AddPurposeHormoneBonus (CString purposeName, int bonus, PheromoneType_e phero);


private:
    CAnimal*  m_pAnimal;

    CArray<PurposeDefinition_t*,PurposeDefinition_t*> m_tPurposeDefinition;

protected:

    bool AddPurposeSensorGene(int mutationRate, int paireIndex, DWORD purposeUid, PurposeSensorBonus_t sensBonus);  
    bool AddPurposeReactionGene(int mutationRate, int paireIndex, DWORD purposeUid, PurposeReactionBonus_t reactionBonus); 


};

#endif // !defined(WIZARD_PURPOSE_H_INCLUDED_)
