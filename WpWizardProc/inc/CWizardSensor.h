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

// CWizardSensor.h: interface for the CBrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(WIZARD_SENSOR_INCLUDED_)
#define WIZARD_SENSOR_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Definitions.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"

#include <afx.h>
#include <afxtempl.h>

#define MAX_NB_LAYER_VIEW 10
#define VIEW_FAR_MIN_RANGE 8

typedef struct 
{
  int                   layer;
  DWORD                 range0;     // View close
  ViewAngleType_e       angle0;
  int                   nbFocus0;
  int                   nbWeight0;
  short*                pWeight0;
  DWORD                 range1;     // View medium
  ViewAngleType_e       angle1;
  int                   nbFocus1;
  int                   nbWeight1;
  short*                pWeight1;
} GeneDescSensorView_t;


class CWizardSensor  
{
public:
	CWizardSensor(CAnimal* pAnimal, int nPreyNumber, CBasicEntity** tPreyList, int nPredatorNumber, CBasicEntity** tPredatorList);
	virtual ~CWizardSensor();

    // High level fonctions
    bool AddGenesVisualSensors(int chromoIdx, int mutationRate, int range, ViewAngleType_e angle, int nbFocus, int variation);
    bool AddGenesVisualIdentifySensors(int chromoIdx, int mutationRate, int range, ViewAngleType_e angle, int nbFocus, int variation);
    bool AddGenesSmellSensor(int chromoIdx, int mutationRate, int range, int variation);
    bool AddGenesGenericSensor(GeneSubType_e type, int defChromoIdx, int sexChromoIdx, int mutationRate, 
                               short defWeight, int variation, int param1=-1, int param2=-1);

private:
    CAnimal*        m_pAnimal;
    int             m_nPreyNumber;
    CBasicEntity**  m_tPreyList;
    int             m_nPredatorNumber;
    CBasicEntity**  m_tPredatorList;

    GeneDescSensorView_t  m_tViewDesc[MAX_NB_LAYER_VIEW]; // MAX_NB_LAYER

protected:
    bool InitViewDescLayer(CBasicEntity* pEntity, int range, ViewAngleType_e angle, int nbFocus, bool isIdentifiedView=false);
    bool AddBonusOnViewWeight(short* pWeight, int nbFocus, short bonusRate, CBasicEntity* pTarget, bool isMoving);
    bool AddBonusOnIdentifyViewWeight(short* pWeight, int nbFocus, short bonusRate, IdentificationType_e identity, 
                                      bool isMoving=false, bool isDirLft=false, bool isDirRight=false);
};

#endif // !defined(WIZARD_SENSOR_INCLUDED_)
