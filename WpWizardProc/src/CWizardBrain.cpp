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

// Brain.cpp: implementation of the CBrain class. 
//
//////////////////////////////////////////////////////////////////////

#include "CWizardBrain.h"
#include "CBrainAnimal.h"


BrainMatrixBonus_t DefaultBrainBonusList [] = 
{
    { UID_BASE_SENS_STOMACH_FULL ,  1,  0,  UID_BASE_REACT_EAT               ,  20,  0, -100}, // Don't eat too much
    { UID_BASE_SENS_STOMACH_FULL ,  1,  0,  UID_BASE_REACT_DRINK             ,  20,  0, -100}, // Don't drink too much
    { UID_BASE_SENS_SPEED        ,  1,  2,  UID_BASE_REACT_NOTHING           ,   1,  0,  -20}, // Don't stop immediately when running
    { UID_BASE_SENS_SPEED        ,  1,  2,  UID_BASE_REACT_SLEEP             ,   1,  0,  -80}, // Don't sleep when running
    { UID_BASE_SENS_SPEED        ,  1,  2,  UID_BASE_REACT_STEPBACKWARD      ,   1,  0,  -80}, // Don't step back when running
    { UID_BASE_SENS_SPEED        ,  1,  2,  UID_BASE_REACT_EAT               ,  20,  0,  -80}, // Don't eat when running
    { UID_BASE_SENS_SPEED        ,  1,  2,  UID_BASE_REACT_DRINK             ,  20,  0,  -80}, // Don't drink when running
    { UID_BASE_SENS_SPEED        ,  1,  0,  UID_BASE_REACT_TURNLEFT          ,   1,  0,  -10}, // Don't turn around when stopped
    { UID_BASE_SENS_SPEED        ,  1,  0,  UID_BASE_REACT_TURNRIGHT         ,   1,  0,  -10}, // Don't turn around when stopped
    { UID_BASE_SENS_TIREDNESS    ,  1,  0,  UID_BASE_REACT_MOVEFORWARD + 12  , 200,  0,  -60}, // Don't run if tired
    { UID_BASE_SENS_TIREDNESS    ,  1,  0,  UID_BASE_REACT_NOTHING           ,   1,  0,   30}, // Stay idle if tired
    { UID_BASE_SENS_HUNGER       ,  1,  0,  UID_BASE_REACT_EAT               ,  20,  0,   30}, // Eat more when hungry
    { UID_BASE_SENS_HUNGER       ,  1,  0,  UID_BASE_REACT_MOVEFORWARD       ,  11,  0,   30}, // Walk to find food when hungry
    { UID_BASE_SENS_HUNGER       ,  1,  0,  UID_BASE_REACT_NOTHING           ,   1,  0,   -4}, // Don't stay idle when hungry
    { UID_BASE_SENS_THIRST       ,  1,  0,  UID_BASE_REACT_DRINK             ,  20,  0,   40}, // Drink more when thirsty
    { UID_BASE_SENS_THIRST       ,  1,  0,  UID_BASE_REACT_MOVEFORWARD       ,  11,  0,   20}, // Walk to find water when thirsty
    { UID_BASE_SENS_THIRST       ,  1,  0,  UID_BASE_REACT_NOTHING           ,   1,  0,   -4}, // Don't stay idle when thirsty
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_NOTHING           ,   1,  0,   20}, // By default: nothing more prio
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_MOVEFORWARD       ,  11,  0,   10}, // By default: walk
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_HIDE              ,   1,  0,  -10}, // By default: don't don't hide
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_MOVEFORWARD + 12  , 200,  0,  -30}, // By default: don't don't run
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_EAT               ,  20,  0,  -60}, // By default: don't eat without food
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_DRINK             ,  20,  0,  -60}, // By default: don't drink without water
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_ATTACK            ,  20,  0,  -60}, // By default: don't attack without target
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_STEPBACKWARD      ,  20,  0,  -60}, // By default: don't step back
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_RUMINATE          ,   1,  0,  -60}, // By default: don't ruminate 
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_SLEEP             ,   1,  0,  -80}, // By default: don't sleep 
    { UID_BASE_SENS_STATIC       ,  1,  0,  UID_BASE_REACT_COPULATE          ,   1,  0,  -80}, // By default: don't copul without female
    { UID_BASE_SENS_TEMPERATURE  ,  1,  0,  UID_BASE_REACT_MOVEFORWARD       ,  11,  0,   10}, // Walk when temperature hot (too find freshness)
    { UID_BASE_SENS_TEMPERATURE  ,  1,  1,  UID_BASE_REACT_MOVEFORWARD       ,  11,  0,   10}, // Walk when temperature cold (too find heat)
    { UID_BASE_SENS_TEMPERATURE  ,  1,  2,  UID_BASE_REACT_NOTHING           ,   1,  0,    2}, // Release when temperature is good
    { UID_BASE_SENS_EAR          ,  1,  3,  UID_BASE_REACT_TURNHEADLEFT      ,   1,  0,   30}, // Turn head left when hear noise
    { UID_BASE_SENS_EAR          ,  1,  5,  UID_BASE_REACT_TURNHEADLEFT      ,   1,  0,   40}, // Turn head left when hear noise
    { UID_BASE_SENS_EAR          ,  1,  7,  UID_BASE_REACT_TURNHEADLEFT      ,   1,  0,   40}, // Turn head left when hear noise
    { UID_BASE_SENS_EAR          ,  1,  4,  UID_BASE_REACT_TURNHEADRIGHT     ,   1,  0,   30}, // Turn head right when hear noise
    { UID_BASE_SENS_EAR          ,  1,  6,  UID_BASE_REACT_TURNHEADRIGHT     ,   1,  0,   40}, // Turn head right when hear noise
    { UID_BASE_SENS_EAR          ,  1,  7,  UID_BASE_REACT_TURNHEADRIGHT     ,   1,  0,   40}, // Turn head right when hear noise
    { UID_BASE_SENS_EAR          ,  1,  7,  UID_BASE_REACT_MOVEFORWARD + 12  , 200,  0,   20}, // Run when hear noise in the back
    { UID_BASE_SENS_FEAR         ,  1,  0,  UID_BASE_REACT_SLEEP             ,   1,  0,  -60}, // Don't sleep when affraid
    { UID_BASE_SENS_FEAR         ,  1,  0,  UID_BASE_REACT_NOTHING           ,   1,  0,  -20}, // Do something when affraid
    { UID_BASE_SENS_FEAR         ,  1,  0,  UID_BASE_REACT_EAT               ,  20,  0,  -40}, // Don't eat when affraid
    { UID_BASE_SENS_FEAR         ,  1,  0,  UID_BASE_REACT_DRINK             ,  20,  0,  -40}, // Don't drink when affraid
    { UID_BASE_SENS_FEAR         ,  1,  0,  UID_BASE_REACT_HIDE              ,   1,  0,   20}, // Hide when affraid
    { UID_BASE_SENS_FEAR         ,  1,  0,  UID_BASE_REACT_MOVEFORWARD + 12  , 200,  0,   20}, // Be ready to run when affraid
    { UID_BASE_SENS_OBSCURITY    ,  1,  0,  UID_BASE_REACT_SLEEP             ,   1,  0,   40}, // Sleep when darkness (diurn only!)
    { UID_BASE_SENS_ORIENTATION  ,  1,  0,  UID_BASE_REACT_MOVEFORWARD       ,  11,  0,   50}, // Walk when orientation guide front
    { UID_BASE_SENS_ORIENTATION  ,  1,  1,  UID_BASE_REACT_TURNLEFT          ,   1,  0,   40}, // Turn when orientation guide left
    { UID_BASE_SENS_ORIENTATION  ,  1,  2,  UID_BASE_REACT_TURNRIGHT         ,   1,  0,   40}  // Turn when orientation guide right
};

int DEFAULT_BRAIN_BONUS_LIST_SIZE = sizeof(DefaultBrainBonusList) / sizeof(BrainMatrixBonus_t);


IdentificationMatrixBonus_t DefaultIdentificationBonusList [] = 
{
    { UID_BASE_SENS_PHEROMONE, PHEROMONE_BABY-PHEROMONE_FIRST_TYPE    , IDENTIFICATION_BABY  ,       100}, //  Baby has a specific pheromon
    { UID_BASE_SENS_PHEROMONE, PHEROMONE_MATERNAL-PHEROMONE_FIRST_TYPE, IDENTIFICATION_MOTHER,       100}, //  Baby has a specific pheromon
    { UID_BASE_SENS_PHEROMONE, PHEROMONE_BABY-PHEROMONE_FIRST_TYPE    , IDENTIFICATION_MOTHER  ,     -30}, //  Mother has no pheromon baby
    { UID_BASE_SENS_PHEROMONE, PHEROMONE_MATERNAL-PHEROMONE_FIRST_TYPE, IDENTIFICATION_BABY,         -30}, //  Baby has no pheromon mother
    { UID_BASE_SENS_VIEW     , 0                                      , IDENTIFICATION_BABY,         -60}, //  Baby is never bigger
    { UID_BASE_SENS_VIEW     , 1                                      , IDENTIFICATION_MOTHER,       -60}, //  Mother is never smaller
    { UID_BASE_SENS_VIEW     , 0                                      , IDENTIFICATION_SEX_PARTNER,  -20}, //  patner is not bigger
    { UID_BASE_SENS_VIEW     , 1                                      , IDENTIFICATION_SEX_PARTNER,  -20}, //  patner is not smaller
    { UID_BASE_SENS_VIEW     , 0                                      , IDENTIFICATION_PREY,         -40}, //  prey is not bigger (danger)
};

int DEFAULT_IDENTIFICATION_BONUS_LIST_SIZE = sizeof(DefaultIdentificationBonusList) / sizeof(IdentificationMatrixBonus_t);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWizardBrain::CWizardBrain(CAnimal* pAnimal)
{
  m_pAnimal = pAnimal;
  m_pBrain = m_pAnimal->getBrain();
}


CWizardBrain::~CWizardBrain()
{
  // loop from top to bottom 
  for (int i=m_tBrainMatrixBonus.GetSize()-1; i>=0; i--) 
  {
    if (m_tBrainMatrixBonus.GetAt(i) != NULL)
      delete m_tBrainMatrixBonus.GetAt(i); 
    m_tBrainMatrixBonus.RemoveAt(i);
  }
}


bool CWizardBrain::ApplyBonusListOnBrain()
{
  if (m_pBrain == NULL)
    return false;

  ApplyIdentifyBonusList();
  ApplyBrainBonusList();

  CompleteViewSecondaryEntities();

  return true;
}


bool CWizardBrain::ApplyBrainBonus(BrainMatrixBonus_t* pBonus)
{
  if (m_pBrain == NULL)
    return false;

  int row, column;
  int foundRowCount = 0;
  int foundColCount = 0;
  bool resu = false;

  row = m_pBrain->GetBrainMatrixRowIndex( pBonus->sensorUniqueId, pBonus->sensorUniqueIdRange, pBonus->subCaptorIndex, pBonus->timeHistory, foundRowCount);
  //string titi = m_pBrain->GetRowLabel(row);

  while (row >= 0)
  {
    foundColCount = 0;
    column = m_pBrain->GetBrainMatrixColumnIndex( pBonus->reactionUniqueId, pBonus->reactionUniqueIdRange, foundColCount);
    while (column >= 0)
    {
      resu = m_pBrain->ChangeDecisionNeuronTableVal(row, column, 0.01*pBonus->bonusRate, false);
      foundColCount++;
      column = m_pBrain->GetBrainMatrixColumnIndex( pBonus->reactionUniqueId, pBonus->reactionUniqueIdRange, foundColCount);
    }
    foundRowCount++; 
    row = m_pBrain->GetBrainMatrixRowIndex( pBonus->sensorUniqueId, pBonus->sensorUniqueIdRange, pBonus->subCaptorIndex, pBonus->timeHistory, foundRowCount);
  }

  return (resu);
}

bool CWizardBrain::ApplyIdentifyBonus(IdentificationMatrixBonus_t* pBonus)
{
  if (m_pBrain == NULL)
    return false;

  int row, column;
  int foundRowCount = 0;
  int foundColCount = 0;
  bool resu = false;

  row = m_pBrain->GetIdentifyMatrixRowIndex(pBonus->sensorUniqueId, pBonus->subCaptorIndex);
  column = pBonus->identity;

  resu = m_pBrain->ChangeIdentifyNeuronTableVal(row, column, 0.01*pBonus->bonusRate, false);

  return (resu);
}


bool CWizardBrain::ApplyMemoryFeedbackBonus(int level)
{
  if (m_pBrain == NULL)
    return false;

  int row;
  row = m_pBrain->GetBrainMatrixRowIndex( UID_BASE_SENS_MEMORY_FB, 1, 0, 0, 0);

  // By default, continue action when receiving pleasure and stop it if welfare decreased.
  for (int i=0; i<m_pBrain->GetNumberReaction(); i++)
  {
    m_pBrain->ChangeDecisionNeuronTableVal(row+i, i, 0.01*level, false);
  }

  return true;
}


bool CWizardBrain::ApplyMemoryActionBonus(int level)
{
  if (m_pBrain == NULL)
    return false;

  int row      = m_pBrain->GetBrainMatrixRowIndex( UID_BASE_SENS_MEMORY_ACT, 1, 0, 0, 0);
  int rowPrevT = m_pBrain->GetBrainMatrixRowIndex( UID_BASE_SENS_MEMORY_ACT, 1, 0, 1, 0);
  
  int columnTl = m_pBrain->GetBrainMatrixColumnIndex( UID_BASE_REACT_TURNLEFT,  1, 0);
  int columnTr = m_pBrain->GetBrainMatrixColumnIndex( UID_BASE_REACT_TURNRIGHT, 1, 0);
  int columnRn = m_pBrain->GetBrainMatrixColumnIndex( UID_BASE_REACT_MOVEFORWARD + 12, 200, 0); // run
  int columnWk = m_pBrain->GetBrainMatrixColumnIndex( UID_BASE_REACT_MOVEFORWARD , 11, 0);      // walk
  int columnSp = m_pBrain->GetBrainMatrixColumnIndex( UID_BASE_REACT_SLEEP,   1, 0);
  int columnNo = m_pBrain->GetBrainMatrixColumnIndex( UID_BASE_REACT_NOTHING,   1, 0);

  // Don't turn right after turn left...
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnTl, columnTr, -0.06*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnTr, columnTl, -0.06*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(rowPrevT + columnTl, columnTr, -0.04*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(rowPrevT + columnTr, columnTl, -0.04*level, false);

  // Don't slow down if just accelerate...
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnRn, columnWk, -0.02*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnRn, columnNo, -0.04*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnNo, columnRn, -0.04*level, false);

  // Don't walk and run continuousely...
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnRn, columnRn, 0.04*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnWk, columnWk, 0.04*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnRn, columnWk, 0.02*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(rowPrevT + columnRn, columnRn, 0.02*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(rowPrevT + columnWk, columnWk, 0.02*level, false);

  // Sleep: sleep again or use nothing as wakeup
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnSp, columnSp,  0.04*level, false);
  m_pBrain->ChangeDecisionNeuronTableVal(row + columnSp, columnNo,  0.04*level, false);

  // FRED TBC...

  return true;
}

int CWizardBrain::ApplyDefaultBonusOnBrain()
{
  if (m_pBrain == NULL)
    return false;
  
  int i;
  int bonusCount = 0;
  int row = 0;
  int column = 0;
  IdentificationMatrixBonus_t* pBonusIdentify;
  BrainMatrixBonus_t* pBonus;

  // reinitialize matrix. (fred: TBC)
  m_pBrain->InitializeNeuronTableNeutral();

  // Apply identification bonus in list
  for (i=0; i<DEFAULT_IDENTIFICATION_BONUS_LIST_SIZE; i++)
  {
    pBonusIdentify = &DefaultIdentificationBonusList[i];
    if ( ApplyIdentifyBonus(pBonusIdentify) )
      bonusCount++;
  }

  // Apply decision bonus in list
  for (i=0; i<DEFAULT_BRAIN_BONUS_LIST_SIZE; i++)
  {
    pBonus = &DefaultBrainBonusList[i];
    if ( ApplyBrainBonus(pBonus) )
      bonusCount++;
  }

  // Apply other default bonus
  ApplyMemoryFeedbackBonus(10);
  ApplyMemoryActionBonus(10);

  // Normalize matrix
  m_pBrain->GetIdentifyNeuronTable()->NormalizeNeuronMatrix();
  m_pBrain->GetDecisionNeuronTable()->NormalizeNeuronMatrix();
  return bonusCount;
}


bool CWizardBrain::AddBrainBonusInList( DWORD sensorUId, int offset, int suboffset, DWORD reactionUniqueId, int reactionRange, int bonus)
{
  if ( (offset<0) || (suboffset<0) )
    return false;

  BrainMatrixBonus_t* pBrainBonus = new BrainMatrixBonus_t;
  pBrainBonus->sensorUniqueId        = sensorUId;
  pBrainBonus->sensorUniqueIdRange   = 1; // sensorUid is the exact Uid => range is 1
  pBrainBonus->subCaptorIndex        = offset + suboffset;
  pBrainBonus->reactionUniqueId      = reactionUniqueId;
  pBrainBonus->reactionUniqueIdRange = reactionRange;
  pBrainBonus->timeHistory           = 0; 
  pBrainBonus->bonusRate             = bonus; 

  m_tBrainMatrixBonus.Add(pBrainBonus);

  return true;
}

bool CWizardBrain::AddIdentifyBonusInList( DWORD sensorUId, int offset, IdentificationType_e  identity, int bonus)
{
  if (offset<0)
    return false;

  IdentificationMatrixBonus_t* pIdentifyBonus = new IdentificationMatrixBonus_t;
  pIdentifyBonus->sensorUniqueId  = sensorUId;
  pIdentifyBonus->subCaptorIndex  = offset;
  pIdentifyBonus->identity        = identity;
  pIdentifyBonus->bonusRate       = bonus; 

  m_tIdentificationMatrixBonus.Add(pIdentifyBonus);

  return true;
}

int CWizardBrain::ApplyBrainBonusList()
{
  if (m_pBrain == NULL)
    return false;

  int bonusCount = 0;
  int row = 0;
  int column = 0;
  BrainMatrixBonus_t* pBonus;

  // Apply bonus
  for (int i=0; i<m_tBrainMatrixBonus.GetSize(); i++)
  {
    pBonus = m_tBrainMatrixBonus[i];
    if ( ApplyBrainBonus(pBonus) )
      bonusCount++;
  }

  // Normalize matrix
  m_pBrain->GetDecisionNeuronTable()->NormalizeNeuronMatrix();
  return bonusCount;
}

int CWizardBrain::ApplyIdentifyBonusList()
{
  if (m_pBrain == NULL)
    return false;

  int bonusCount = 0;
  int row = 0;
  int column = 0;
  IdentificationMatrixBonus_t* pBonus;

  // Apply bonus
  for (int i=0; i<m_tIdentificationMatrixBonus.GetSize(); i++)
  {
    pBonus = m_tIdentificationMatrixBonus[i];
    if ( ApplyIdentifyBonus(pBonus) )
      bonusCount++;
  }

  // Normalize matrix
  m_pBrain->GetIdentifyNeuronTable()->NormalizeNeuronMatrix();
  return bonusCount;
}


bool CWizardBrain::AddVisualObstacle ( int level,
                                       int layer,
                                       FormType_e form, 
                                       ColorCaracterType_e color, 
                                       TextureType_e texture, 
                                       PhyAttributeType_e attribute,
                                       bool justInFront)
{
  if ( (m_pBrain==NULL) || (level==0) )
    return false; 

  CSensor* pCurSens;
  DWORD curSensUid;

  for (int i=0; i<m_pBrain->GetNumberSensor(); i++)
  {
    pCurSens   = m_pBrain->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW ) || ( (curSensUid & 0xFFFF0000) == (UID_BASE_SENS_COMPOSITE + UID_BASE_SENS_VIEW/256) ) )
    {
      CSensorView* pViewSens = NULL;

      if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW )
      {
        pViewSens = (CSensorView*)pCurSens;
      }
      else
      {
        // This is the composite view/speed sensor
        pViewSens = (CSensorView*)( ((CSensorComposite*)pCurSens)->GetBaseSens() );
      }

      if (pViewSens->GetLayer() == layer)
      {
        // Prepare captor offsets
        int offsetEntityF0   = pViewSens->GetSubCaptorIndexForDirection(0,0);
        int offsetEntityFL0  = pViewSens->GetSubCaptorIndexForDirection(1,0);
        int offsetEntityFR0  = pViewSens->GetSubCaptorIndexForDirection(2,0); 
        int offsetEntityL0   = pViewSens->GetSubCaptorIndexForDirection(3,0); 
        int offsetEntityR0   = pViewSens->GetSubCaptorIndexForDirection(4,0); 
        int offsetEntityBL0  = pViewSens->GetSubCaptorIndexForDirection(5,0); 
        int offsetEntityBR0  = pViewSens->GetSubCaptorIndexForDirection(6,0); 
        int offsetEntityB0   = pViewSens->GetSubCaptorIndexForDirection(7,0); 
        int subOffsetForm    = -1;
        int subOffsetColor   = -1;
        int subOffsetTexture = -1;
        int subOffsetAttrib  = -1;
        int subOffsetProxi   = -1;

        if (form >= FORM_FIRST_TYPE)
          subOffsetForm    = pViewSens->GetSubCaptorSubIndexForForm(form);
        if (color >= COLOR_CARACTER_FIRST_TYPE)
          subOffsetColor   = pViewSens->GetSubCaptorSubIndexForColor(color);
        if (texture >= TEXTURE_FIRST_TYPE)
          subOffsetTexture = pViewSens->GetSubCaptorSubIndexForTexture(texture);
        if (attribute >= PHY_ATTRIBUTE_FIRST_TYPE)
          subOffsetAttrib = pViewSens->GetSubCaptorSubIndexForPhyAttribute(attribute);
        
        subOffsetProxi = pViewSens->GetSubCaptorSubIndexForProximity();

        // If no parameter set, use proximity
        if ( (subOffsetForm==-1) && (subOffsetColor==-1) && (subOffsetTexture==-1) && (subOffsetAttrib==-1) )
        {
          subOffsetForm = subOffsetProxi;
        }

        if (pViewSens->GetRange() == 1)
        {

          // Adjacent: Stop and don't run nor walk !
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_NOTHING,   1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_NOTHING,   1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_NOTHING,   1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_NOTHING,   1,  level/8);

          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);

          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD, 11, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD, 11, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD, 11, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD, 11, -level);

          // Turn
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1,  level/4);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1,  level/4);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1,  level/8);

          if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW )
          {
            // On the Left: Avoid turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, -level/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, -level/8);

            // On the Right: Avoid turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -level/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -level/8);
          }
          else
          {
            // On the Left: Don't turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, -level/2);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, -level/4);

            // On the Right: Don't turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -level/2);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -level/4);
          }
        }
        else if (!justInFront)
        {
          // In front: Don't run
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/4);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxi,   UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);

          // Turn
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  100, level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 100, level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 100, level/10);
        }
      }
    }
  }

  return true;
}



bool CWizardBrain::AddVisualIdentifyObstacle ( int level,
                                               int layer,
                                               IdentificationType_e identity, 
                                               bool isMoving, bool isDirLeft, bool isDirRight,
                                               bool justInFront)
{
  if ( (m_pBrain==NULL) || (level==0) )
    return false; 

  CSensor* pCurSens;
  DWORD curSensUid;

  for (int i=0; i<m_pBrain->GetNumberSensor(); i++)
  {
    pCurSens   = m_pBrain->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY ) || ( (curSensUid & 0xFFFF0000) == (UID_BASE_SENS_COMPOSITE + UID_BASE_SENS_VIEW_IDENTIFY/256) ) )
    {
      CSensorViewIdentify* pViewSens = NULL;

      if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY )
      {
        pViewSens = (CSensorViewIdentify*)pCurSens;
      }
      else
      {
        // This is the composite view/speed sensor
        pViewSens = (CSensorViewIdentify*)( ((CSensorComposite*)pCurSens)->GetBaseSens() );
      }

      if (pViewSens->GetLayer() == layer)
      {
        // Prepare captor offsets
        int offsetEntityF0   = pViewSens->GetSubCaptorIndexForDirection(0,0);
        int offsetEntityFL0  = pViewSens->GetSubCaptorIndexForDirection(1,0);
        int offsetEntityFR0  = pViewSens->GetSubCaptorIndexForDirection(2,0); 
        int offsetEntityL0   = pViewSens->GetSubCaptorIndexForDirection(3,0); 
        int offsetEntityR0   = pViewSens->GetSubCaptorIndexForDirection(4,0); 
        int offsetEntityBL0  = pViewSens->GetSubCaptorIndexForDirection(5,0); 
        int offsetEntityBR0  = pViewSens->GetSubCaptorIndexForDirection(6,0); 
        int offsetEntityB0   = pViewSens->GetSubCaptorIndexForDirection(7,0); 
        int subOffsetProxy    = -1;
        int subOffsetMove     = -1;
        int subOffsetdirLeft  = -1;
        int subOffsetdirRight = -1;

        subOffsetProxy = CSensorViewIdentify::GetSubCaptorSubIndexForProximity(identity);
        if (isMoving)
        {
          subOffsetMove  = CSensorViewIdentify::GetSubCaptorSubIndexForMoving(identity);
        }
        if (isDirLeft)
        {
          subOffsetdirLeft  = CSensorViewIdentify::GetSubCaptorSubIndexForDirLeft(identity);
        }
        if (isDirRight)
        {
          subOffsetdirRight  = CSensorViewIdentify::GetSubCaptorSubIndexForDirRight(identity);
        }

        if (pViewSens->GetRange() == 1)
        {

          // Adjacent: Stop and don't run nor walk !
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_NOTHING,   1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_NOTHING,   1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_NOTHING,   1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_NOTHING,   1,  level/8);

          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_MOVEFORWARD + 12, 200, -level);

          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_MOVEFORWARD, 11, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_MOVEFORWARD, 11, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_MOVEFORWARD, 11, -level);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_MOVEFORWARD, 11, -level);

          // Turn
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_TURNLEFT,  1,  level/4);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_TURNLEFT,  1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_TURNLEFT,  1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_TURNLEFT,  1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_TURNRIGHT, 1,  level/4);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_TURNRIGHT, 1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_TURNRIGHT, 1,  level/8);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_TURNRIGHT, 1,  level/8);

          if ( (curSensUid & UID_BASE_MASK) == GENE_SENS_VIEW_IDENTIFY )
          {
            // On the Left: Avoid turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,    UID_BASE_REACT_TURNLEFT, 1, -level/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetMove,     UID_BASE_REACT_TURNLEFT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft,  UID_BASE_REACT_TURNLEFT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirRight, UID_BASE_REACT_TURNLEFT, 1, -level/8);

            // On the Right: Avoid turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,    UID_BASE_REACT_TURNRIGHT, 1, -level/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetMove,     UID_BASE_REACT_TURNRIGHT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirLeft,  UID_BASE_REACT_TURNRIGHT, 1, -level/8);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight, UID_BASE_REACT_TURNRIGHT, 1, -level/8);
          }
          else
          {
            // On the Left: Don't turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,    UID_BASE_REACT_TURNLEFT, 1, -level/2);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetMove,     UID_BASE_REACT_TURNLEFT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft,  UID_BASE_REACT_TURNLEFT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirRight, UID_BASE_REACT_TURNLEFT, 1, -level/4);

            // On the Right: Don't turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,    UID_BASE_REACT_TURNRIGHT, 1, -level/2);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetMove,     UID_BASE_REACT_TURNRIGHT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirLeft,  UID_BASE_REACT_TURNRIGHT, 1, -level/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight, UID_BASE_REACT_TURNRIGHT, 1, -level/4);
          }
        }
        else if (!justInFront)
        {
          // In front: Don't run
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/4);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_MOVEFORWARD + 12, 200, -level/6);

          // Turn
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_TURNLEFT,  100, level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_TURNLEFT,  100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_TURNLEFT,  100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_TURNLEFT,  100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,    UID_BASE_REACT_TURNRIGHT, 100, level/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,     UID_BASE_REACT_TURNRIGHT, 100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft,  UID_BASE_REACT_TURNRIGHT, 100, level/10);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight, UID_BASE_REACT_TURNRIGHT, 100, level/10);
        }
      }
    }
  }

  return true;
}



bool CWizardBrain::AddVisualBonus ( int bonusRate,
                                    int layer,
                                    FormType_e form, 
                                    ColorCaracterType_e color, 
                                    TextureType_e texture, 
                                    PhyAttributeType_e attribute,
                                    bool isMoving,
                                    bool doRun,
                                    DWORD targetReacUid,
                                    int targetReacRange)
{
  if ( (m_pBrain==NULL) || (bonusRate==0) )
    return false; 

  CSensor* pCurSens;
  DWORD curSensUid;

  // By default: walk
  DWORD moveReacUniqueId = UID_BASE_REACT_MOVEFORWARD;  
  int   moveReacRange = 11;

  if (doRun)
  {
    moveReacUniqueId = UID_BASE_REACT_MOVEFORWARD + 12;  
    moveReacRange = 200;
  }

  for (int i=0; i<m_pBrain->GetNumberSensor(); i++)
  {
    pCurSens   = m_pBrain->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW )
    {
      CSensorView* pViewSens = (CSensorView*)pCurSens;

      if (pViewSens->GetLayer() == layer)
      {
        // Prepare captor offsets
        int offsetEntityF0   = pViewSens->GetSubCaptorIndexForDirection(0,0);
        int offsetEntityFL0  = pViewSens->GetSubCaptorIndexForDirection(1,0);
        int offsetEntityFR0  = pViewSens->GetSubCaptorIndexForDirection(2,0); 
        int offsetEntityL0   = pViewSens->GetSubCaptorIndexForDirection(3,0); 
        int offsetEntityR0   = pViewSens->GetSubCaptorIndexForDirection(4,0); 
        int offsetEntityBL0  = pViewSens->GetSubCaptorIndexForDirection(5,0); 
        int offsetEntityBR0  = pViewSens->GetSubCaptorIndexForDirection(6,0); 
        int offsetEntityB0   = pViewSens->GetSubCaptorIndexForDirection(7,0); 
        int subOffsetForm    = -1;
        int subOffsetColor   = -1;
        int subOffsetTexture = -1;
        int subOffsetAttrib  = -1;
        int subOffsetIsMove  = -1;

        if (form >= FORM_FIRST_TYPE)
          subOffsetForm    = pViewSens->GetSubCaptorSubIndexForForm(form);
        if (color >= COLOR_CARACTER_FIRST_TYPE)
          subOffsetColor   = pViewSens->GetSubCaptorSubIndexForColor(color);
        if (texture >= TEXTURE_FIRST_TYPE)
          subOffsetTexture = pViewSens->GetSubCaptorSubIndexForTexture(texture);
        if (attribute >= PHY_ATTRIBUTE_FIRST_TYPE)
          subOffsetAttrib = pViewSens->GetSubCaptorSubIndexForPhyAttribute(attribute);
        if (isMoving)
          subOffsetIsMove = pViewSens->GetSubCaptorSubIndexForMoving();

        if (pViewSens->GetRange() == 1)
        {
          if (bonusRate>0)
          {
            // Adjacent attraction
            // In front: Do action
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    targetReacUid, targetReacRange, bonusRate);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   targetReacUid, targetReacRange, bonusRate/2);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, targetReacUid, targetReacRange, bonusRate/2);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  targetReacUid, targetReacRange, bonusRate/2);
            // Don't run/walk
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD + 12, 200, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD + 12, 200, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD + 12, 200, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD + 12, 200, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD, 11, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD, 11, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD, 11, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD, 11, -bonusRate/4);

            // On the Left: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            // On the Right: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            // On the back: Turn
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
          }
          else
          {
            // Adjacent repulsion
            // In front: Turn
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            // Front Left: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            // Front Right: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            // Otherwise: Accelerate
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
          }
        }
        else
        {
          if (bonusRate>0)
          {
            // Close attraction
            // In front: Walk
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            if (doRun)
            {
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
            }

            // On the Left: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
            // On the Right: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            // On the back: Turn
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
          }
          else
          {
            // Close repulsion
            // In front: Turn
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            // Front Left: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            // Front Right: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            // Otherwise: Accelerate
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/4);
          }
        }
      }
    }
    else if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_FAR )
    {
      CSensorViewFar* pViewSens = (CSensorViewFar*)pCurSens;

      // Prepare captor offsets
      int offsetEntityF0   = pViewSens->GetSubCaptorIndexForDirection(0,0);
      int offsetEntityFL0  = pViewSens->GetSubCaptorIndexForDirection(1,0);
      int offsetEntityFR0  = pViewSens->GetSubCaptorIndexForDirection(2,0); 
      int offsetEntityL0   = pViewSens->GetSubCaptorIndexForDirection(3,0); 
      int offsetEntityR0   = pViewSens->GetSubCaptorIndexForDirection(4,0); 
      int offsetEntityBL0  = pViewSens->GetSubCaptorIndexForDirection(5,0); 
      int offsetEntityBR0  = pViewSens->GetSubCaptorIndexForDirection(6,0); 
      int offsetEntityB0   = pViewSens->GetSubCaptorIndexForDirection(7,0); 
      int subOffsetForm    = -1;
      int subOffsetColor   = -1;
      int subOffsetTexture = -1;
      int subOffsetAttrib  = -1;
      int subOffsetIsMove  = -1;

      if (form >= FORM_FIRST_TYPE)
        subOffsetForm    = pViewSens->GetSubCaptorSubIndexForForm(form);
      if (color >= COLOR_CARACTER_FIRST_TYPE)
        subOffsetColor   = pViewSens->GetSubCaptorSubIndexForColor(color);
      if (texture >= TEXTURE_FIRST_TYPE)
        subOffsetTexture = pViewSens->GetSubCaptorSubIndexForTexture(texture);
      if (attribute >= PHY_ATTRIBUTE_FIRST_TYPE)
        subOffsetAttrib = pViewSens->GetSubCaptorSubIndexForPhyAttribute(attribute);
      if (isMoving)
        subOffsetIsMove = pViewSens->GetSubCaptorSubIndexForMoving();

      if (bonusRate>0)
      {
        // far attraction
        // In front: walk
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    moveReacUniqueId, moveReacRange, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   moveReacUniqueId, moveReacRange, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, moveReacUniqueId, moveReacRange, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  moveReacUniqueId, moveReacRange, bonusRate/4);
        // On the Left: Turn left
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        // On the Right: Turn right
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        // On the back: Turn
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
      }
      else
      {
        // far repulsion
        // In front: Turn
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        // Front Left: Turn right
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetForm,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetColor,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetTexture, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetAttrib,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetIsMove,  UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        // Front Right: Turn left
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetForm,    UID_BASE_REACT_TURNLEFT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetColor,   UID_BASE_REACT_TURNLEFT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetTexture, UID_BASE_REACT_TURNLEFT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetAttrib,  UID_BASE_REACT_TURNLEFT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetIsMove,  UID_BASE_REACT_TURNLEFT, 1, -bonusRate/6);
        // Otherwise: Accelerate
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetForm,    moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetColor,   moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetTexture, moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetAttrib,  moveReacUniqueId, moveReacRange, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetIsMove,  moveReacUniqueId, moveReacRange, -bonusRate/6);
      }
    }
  }

  return true;
}

bool CWizardBrain::AddVisualIdentifyBonus ( int bonusRate,
                                            int layer,
                                            IdentificationType_e identity, 
                                            bool isMoving, bool isDirLeft, bool isDirRight,
                                            bool doRun,
                                            DWORD targetReacUid,
                                            int targetReacRange)
{
  if ( (m_pBrain==NULL) || (bonusRate==0) )
    return false; 

  CSensor* pCurSens;
  DWORD curSensUid;

  // By default: walk
  DWORD moveReacUniqueId = UID_BASE_REACT_MOVEFORWARD;  
  int   moveReacRange = 11;

  if (doRun)
  {
    moveReacUniqueId = UID_BASE_REACT_MOVEFORWARD + 12;  
    moveReacRange = 200;
  }

  for (int i=0; i<m_pBrain->GetNumberSensor(); i++)
  {
    pCurSens   = m_pBrain->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY )
    {
      CSensorViewIdentify* pViewSens = (CSensorViewIdentify*)pCurSens;

      if (pViewSens->GetLayer() == layer)
      {
        // Prepare captor offsets
        int offsetEntityF0   = pViewSens->GetSubCaptorIndexForDirection(0,0);
        int offsetEntityFL0  = pViewSens->GetSubCaptorIndexForDirection(1,0);
        int offsetEntityFR0  = pViewSens->GetSubCaptorIndexForDirection(2,0); 
        int offsetEntityL0   = pViewSens->GetSubCaptorIndexForDirection(3,0); 
        int offsetEntityR0   = pViewSens->GetSubCaptorIndexForDirection(4,0); 
        int offsetEntityBL0  = pViewSens->GetSubCaptorIndexForDirection(5,0); 
        int offsetEntityBR0  = pViewSens->GetSubCaptorIndexForDirection(6,0); 
        int offsetEntityB0   = pViewSens->GetSubCaptorIndexForDirection(7,0); 
        int subOffsetProxy    = -1;
        int subOffsetMove     = -1;
        int subOffsetdirLeft  = -1;
        int subOffsetdirRight = -1;

        subOffsetProxy = CSensorViewIdentify::GetSubCaptorSubIndexForProximity(identity);
        if (isMoving)
        {
          subOffsetMove = CSensorViewIdentify::GetSubCaptorSubIndexForMoving(identity);
        }
        if (isDirLeft)
        {
          subOffsetdirLeft = CSensorViewIdentify::GetSubCaptorSubIndexForDirLeft(identity);
        }
        if (isDirRight)
        {
          subOffsetdirRight = CSensorViewIdentify::GetSubCaptorSubIndexForDirRight(identity);
        }

        if (pViewSens->GetRange() == 1)
        {
          if (bonusRate>0)
          {
            // Adjacent attraction
            // In front: Do action
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   targetReacUid, targetReacRange, bonusRate);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    targetReacUid, targetReacRange, bonusRate/2);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, targetReacUid, targetReacRange, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,targetReacUid, targetReacRange, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT,  1   , bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1   , bonusRate/8);

            // Don't run/walk
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_MOVEFORWARD + 12, 200, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_MOVEFORWARD, 11, -bonusRate/4);

            // On the Left: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/2);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/3);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);

            // On the Right: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/2);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/3);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);

            // On the back: Turn
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT,  1, bonusRate/5);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetMove,    UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/5);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
          }
          else
          {
            // Adjacent repulsion
            // In front: Turn
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            // Front Left: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            // Front Right: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetMove,    UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);

            // Otherwise: Accelerate
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
          }
        }
        else
        {
          if (bonusRate>0)
          {
            // Close attraction
            // In front: Walk
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT,     1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT,    1, bonusRate/8);
            if (doRun)
            {
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
              AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
            }

            // On the Left: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);

            // On the Right: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);

            // On the back: Turn
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
            AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
          }
          else
          {
            // Close repulsion
            // In front: Turn
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
            // Front Left: Turn right
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
            // Front Right: Turn left
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetMove,    UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight,UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
            // Otherwise: Accelerate
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
            AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
          }
        }
      }
    }
      else if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY_FAR )
    {
      CSensorViewIdentifyFar* pViewSens = (CSensorViewIdentifyFar*)pCurSens;

      // Prepare captor offsets
      int offsetEntityF0   = pViewSens->GetSubCaptorIndexForDirection(0,0);
      int offsetEntityFL0  = pViewSens->GetSubCaptorIndexForDirection(1,0);
      int offsetEntityFR0  = pViewSens->GetSubCaptorIndexForDirection(2,0); 
      int offsetEntityL0   = pViewSens->GetSubCaptorIndexForDirection(3,0); 
      int offsetEntityR0   = pViewSens->GetSubCaptorIndexForDirection(4,0); 
      int offsetEntityBL0  = pViewSens->GetSubCaptorIndexForDirection(5,0); 
      int offsetEntityBR0  = pViewSens->GetSubCaptorIndexForDirection(6,0); 
      int offsetEntityB0   = pViewSens->GetSubCaptorIndexForDirection(7,0); 
      int subOffsetProxy    = -1;
      int subOffsetMove     = -1;
      int subOffsetdirLeft  = -1;
      int subOffsetdirRight = -1;

      subOffsetProxy = CSensorViewIdentifyFar::GetSubCaptorSubIndexForProximity(identity);
      if (isMoving)
      {
        subOffsetMove = CSensorViewIdentifyFar::GetSubCaptorSubIndexForMoving(identity);
      }
      if (isDirLeft)
      {
        subOffsetdirLeft = CSensorViewIdentifyFar::GetSubCaptorSubIndexForDirLeft(identity);
      }
      if (isDirRight)
      {
        subOffsetdirRight = CSensorViewIdentifyFar::GetSubCaptorSubIndexForDirRight(identity);
      }

      if (bonusRate>0)
      {
        // far attraction
        // In front: Walk
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_MOVEFORWARD, 11, bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT,     1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT,    1, bonusRate/8);
        if (doRun)
        {
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
          AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_MOVEFORWARD + 12, 200, bonusRate/6);
        }

        // On the Left: Turn left
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT, 1, bonusRate/10);

        // On the Right: Turn right
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/8);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);

        // On the back: Turn
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetdirLeft, UID_BASE_REACT_TURNLEFT,  1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
        AddBrainBonusInList(curSensUid, offsetEntityB0, subOffsetdirRight,UID_BASE_REACT_TURNRIGHT, 1, bonusRate/10);
      }
      else
      {
        // far repulsion
        // In front: Turn
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirRight,UID_BASE_REACT_TURNLEFT,  1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        AddBrainBonusInList(curSensUid, offsetEntityF0, subOffsetdirLeft, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/6);
        // Front Left: Turn right
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetProxy,   UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetMove,    UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityFL0, subOffsetdirLeft, UID_BASE_REACT_TURNRIGHT, 1, -bonusRate/4);
        // Front Right: Turn left
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetProxy,   UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetMove,    UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityFR0, subOffsetdirRight,UID_BASE_REACT_TURNLEFT, 1, -bonusRate/4);
        // Otherwise: Accelerate
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityL0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityR0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBL0, subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityBR0, subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetProxy,   moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetMove,    moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetdirLeft, moveReacUniqueId, moveReacRange, -bonusRate/4);
        AddBrainBonusInList(curSensUid, offsetEntityB0,  subOffsetdirRight,moveReacUniqueId, moveReacRange, -bonusRate/4);
      }
    }
  }

  return true;
}

bool CWizardBrain::AddOdorBonus (int bonusRate, OdorType_e odor, DWORD targetReacUid, int targetReacRange)
{
  BrainMatrixBonus_t* pBrainBonus = new BrainMatrixBonus_t;
  pBrainBonus->sensorUniqueId        = UID_BASE_SENS_SMELL;
  pBrainBonus->sensorUniqueIdRange   = 1000; 
  pBrainBonus->subCaptorIndex        = odor-ODOR_FIRST_TYPE;
  pBrainBonus->reactionUniqueId      = targetReacUid;
  pBrainBonus->reactionUniqueIdRange = targetReacRange;
  pBrainBonus->timeHistory           = 0; 
  pBrainBonus->bonusRate             = bonusRate; 

  m_tBrainMatrixBonus.Add(pBrainBonus);

  return true;
}

bool CWizardBrain::AddPheromoneBonus (int bonusRate, PheromoneType_e phero, DWORD targetReacUid, int targetReacRange)
{
  BrainMatrixBonus_t* pBrainBonus = new BrainMatrixBonus_t;
  pBrainBonus->sensorUniqueId        = UID_BASE_SENS_PHEROMONE;
  pBrainBonus->sensorUniqueIdRange   = 1000; 
  pBrainBonus->subCaptorIndex        = phero-PHEROMONE_FIRST_TYPE;
  pBrainBonus->reactionUniqueId      = targetReacUid;
  pBrainBonus->reactionUniqueIdRange = targetReacRange;
  pBrainBonus->timeHistory           = 0; 
  pBrainBonus->bonusRate             = bonusRate; 

  m_tBrainMatrixBonus.Add(pBrainBonus);

  return true;
}

bool CWizardBrain::AddHormoneBonus (int bonusRate, PheromoneType_e phero, DWORD targetReacUid, int targetReacRange)
{
  BrainMatrixBonus_t* pBrainBonus = new BrainMatrixBonus_t;
  pBrainBonus->sensorUniqueId        = UID_BASE_SENS_HORMONE;
  pBrainBonus->sensorUniqueIdRange   = 1; 
  pBrainBonus->subCaptorIndex        = phero-PHEROMONE_FIRST_TYPE;
  pBrainBonus->reactionUniqueId      = targetReacUid;
  pBrainBonus->reactionUniqueIdRange = targetReacRange;
  pBrainBonus->timeHistory           = 0; 
  pBrainBonus->bonusRate             = bonusRate; 

  m_tBrainMatrixBonus.Add(pBrainBonus);

  return true;
}

bool CWizardBrain::AddDefaultSexualBonus(int bonusRate)
{
  if (m_pAnimal == NULL)
    return false;

  if (m_pAnimal->getSex() == SEX_MALE)
  {
    AddBrainBonusInList(UID_BASE_SENS_LIBIDO, 0, 0, UID_BASE_REACT_COPULATE, 1, bonusRate/2);
    AddHormoneBonus( bonusRate/2, PHEROMONE_SEXUAL_MALE, UID_BASE_REACT_COPULATE, 1);

    // Stay with female, not male nor childs
    AddPheromoneBonus( bonusRate, PHEROMONE_SEXUAL_FEMALE, UID_BASE_REACT_COPULATE,     1 );
    AddPheromoneBonus(-bonusRate, PHEROMONE_MATERNAL,      UID_BASE_REACT_COPULATE,     1 );
    AddPheromoneBonus(-bonusRate, PHEROMONE_BABY,          UID_BASE_REACT_COPULATE,     1 );
    AddPheromoneBonus( bonusRate, PHEROMONE_BABY,          UID_BASE_REACT_MOVEFORWARD, 11 );
    AddPheromoneBonus( bonusRate, PHEROMONE_SEXUAL_MALE,   UID_BASE_REACT_MOVEFORWARD, 11 );

    AddPheromoneIdentificationBonus( bonusRate+10, PHEROMONE_SEXUAL_FEMALE, IDENTIFICATION_SEX_PARTNER);
    AddPheromoneIdentificationBonus(-bonusRate,    PHEROMONE_MATERNAL,      IDENTIFICATION_SEX_PARTNER);
    AddPheromoneIdentificationBonus(-bonusRate,    PHEROMONE_BABY,          IDENTIFICATION_SEX_PARTNER);
    AddPheromoneIdentificationBonus(-bonusRate,    PHEROMONE_SEXUAL_MALE,   IDENTIFICATION_SEX_PARTNER);

    AddVisualBonus(bonusRate/5, m_pAnimal->getLayer(), m_pAnimal->getForm(), m_pAnimal->getColorType(),
                   m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), false, false, UID_BASE_REACT_COPULATE, 1);
    AddVisualIdentifyBonus (bonusRate/2, m_pAnimal->getLayer(), IDENTIFICATION_SEX_PARTNER, false, false, false, false, UID_BASE_REACT_COPULATE, 1);
    AddVisualIdentificationBonus ( 25, m_pAnimal->getForm(), m_pAnimal->getColorType(), m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), false, false, false, IDENTIFICATION_SEX_PARTNER);
  }
  else
  {
    // Stay with male, not female
    AddPheromoneBonus(bonusRate,   PHEROMONE_SEXUAL_MALE,   UID_BASE_REACT_NOTHING, 1);
    AddPheromoneBonus(bonusRate,   PHEROMONE_SEXUAL_FEMALE, UID_BASE_REACT_MOVEFORWARD, 11 );

    AddPheromoneIdentificationBonus( bonusRate+10, PHEROMONE_SEXUAL_MALE,   IDENTIFICATION_SEX_PARTNER);
    AddPheromoneIdentificationBonus(-bonusRate,    PHEROMONE_SEXUAL_FEMALE, IDENTIFICATION_SEX_PARTNER);
    AddPheromoneIdentificationBonus(-bonusRate,    PHEROMONE_MATERNAL,      IDENTIFICATION_SEX_PARTNER);
    AddPheromoneIdentificationBonus(-bonusRate,    PHEROMONE_BABY,          IDENTIFICATION_SEX_PARTNER);

    AddVisualBonus(bonusRate/8, m_pAnimal->getLayer(), m_pAnimal->getForm(), m_pAnimal->getColorType(),
                   m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), true, false);
    AddVisualIdentifyBonus (bonusRate/2, m_pAnimal->getLayer(), IDENTIFICATION_SEX_PARTNER, false, false, false, false);
    AddVisualIdentificationBonus ( 25, m_pAnimal->getForm(), m_pAnimal->getColorType(), m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), false, false, false, IDENTIFICATION_SEX_PARTNER);
  }

  return true;
}


bool CWizardBrain::AddDefaultMammalBonus(int bonusRate)
{
  if (m_pAnimal == NULL)
    return false;

  // Drink milk when baby
  AddHormoneBonus  ( bonusRate/2, PHEROMONE_BABY,     UID_BASE_REACT_EAT+10, 1);
  AddPheromoneBonus( bonusRate,   PHEROMONE_MATERNAL, UID_BASE_REACT_EAT+10, 1);
  // Already in default // AddPheromoneIdentificationBonus( bonusRate, PHEROMONE_MATERNAL, IDENTIFICATION_MOTHER);

  // Slow down when mother is closed
  AddPheromoneBonus(-bonusRate/2, PHEROMONE_MATERNAL, UID_BASE_REACT_MOVEFORWARD + 12, 200);
  AddPheromoneBonus(-bonusRate/4, PHEROMONE_MATERNAL, UID_BASE_REACT_MOVEFORWARD     ,  11);

  if (m_pAnimal->getSex() == SEX_FEMALE)
  {
    // Slow down when mother
    AddHormoneBonus  (-bonusRate/2, PHEROMONE_MATERNAL, UID_BASE_REACT_MOVEFORWARD + 12, 200);
    AddHormoneBonus  (-bonusRate/4, PHEROMONE_MATERNAL, UID_BASE_REACT_MOVEFORWARD     ,  11);
    AddPheromoneBonus(-bonusRate/2, PHEROMONE_BABY,     UID_BASE_REACT_MOVEFORWARD + 12, 200);
    AddPheromoneBonus(-bonusRate/4, PHEROMONE_BABY,     UID_BASE_REACT_MOVEFORWARD     ,  11);

   // Already in default // AddPheromoneIdentificationBonus( bonusRate, PHEROMONE_BABY, IDENTIFICATION_BABY);
    AddVisualIdentificationBonus ( 25, m_pAnimal->getForm(), m_pAnimal->getColorType(), m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), false, false, true, IDENTIFICATION_BABY);

    // Attraction for mother milk (to be amplified by purpose)
    AddVisualBonus(bonusRate/2, m_pAnimal->getLayer(), m_pAnimal->getForm(), m_pAnimal->getColorType(),
                 m_pAnimal->getTexture(), PHY_ATTRIBUTE_UDDER, false, true, UID_BASE_REACT_EAT+10, 1);
    AddVisualIdentifyBonus (bonusRate, m_pAnimal->getLayer(), IDENTIFICATION_MOTHER, true, true, true, false, UID_BASE_REACT_EAT+10, 1);
    AddVisualIdentificationBonus ( 20, m_pAnimal->getForm(), m_pAnimal->getColorType(), m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), false, true, false, IDENTIFICATION_MOTHER);
    AddVisualIdentificationBonus ( 40, FORM_UNSET, COLOR_CARACTER_UNSET, TEXTURE_UNSET, PHY_ATTRIBUTE_UDDER, false, false, false, IDENTIFICATION_MOTHER);
  }
  else
  {
    // Attraction for mother milk (to be amplified by purpose)
    AddVisualBonus(bonusRate/4, m_pAnimal->getLayer(), m_pAnimal->getForm(), m_pAnimal->getColorType(),
                 m_pAnimal->getTexture(), PHY_ATTRIBUTE_UDDER, false, true, UID_BASE_REACT_EAT+10, 1);
    AddVisualIdentifyBonus (bonusRate, m_pAnimal->getLayer(), IDENTIFICATION_MOTHER, true, true, true, false, UID_BASE_REACT_EAT+10, 1);
    AddVisualIdentificationBonus ( 20, m_pAnimal->getForm(), m_pAnimal->getColorType(), m_pAnimal->getTexture(), m_pAnimal->getMainPhyAttribute(), false, true, false, IDENTIFICATION_MOTHER);
    AddVisualIdentificationBonus ( 40, FORM_UNSET, COLOR_CARACTER_UNSET, TEXTURE_UNSET, PHY_ATTRIBUTE_UDDER, false, false, false, IDENTIFICATION_MOTHER);
  }

  return true;
}

bool CWizardBrain::CompleteViewSecondaryEntities(void)
{
  if (m_pAnimal == NULL)
    return false;

  CSensor* pCurSens;
  DWORD curSensUid;

  for (int i=0; i<m_pBrain->GetNumberSensor(); i++)
  {
    pCurSens   = m_pBrain->GetSensorByIndex(i);
    curSensUid = pCurSens->GetUniqueId(); 
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW )
    {
      CSensorView* pViewSens = (CSensorView*)pCurSens;

      for (int direction=0; direction<8; direction++)
      {
        for (int index=1; index<VIEW_MAX_NUMBER_FOCUSSED_ENTITIES; index++)
        {
          int offsetEntity = pViewSens->GetSubCaptorIndexForDirection(direction,index);

          if (offsetEntity > -1)
          {
            int offsetEntitySource = pViewSens->GetSubCaptorIndexForDirection(direction,0);
            
            for (int j=0; j<VIEW_SIZE_PER_FOCUS; j++)
            {
              int rowBase = m_pBrain->GetBrainMatrixRowIndex( curSensUid, 1, 0, 0, 0);  
              for (int col=0; col <m_pBrain->GetDecisionNeuronTable()->GetNeuronTableColumnCount(); col++)
              {
                m_pBrain->GetDecisionNeuronTable()->SetNeuronTableData( rowBase+offsetEntity+j, col, m_pBrain->GetDecisionNeuronTable()->GetNeuronTableData(rowBase+offsetEntitySource+j, col) );
              }
            }
          }
        }
      }
    }
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY )
    {
      CSensorViewIdentify* pViewSens = (CSensorViewIdentify*)pCurSens;

      for (int direction=0; direction<8; direction++)
      {
        for (int index=1; index<VIEW_MAX_NUMBER_FOCUSSED_ENTITIES; index++)
        {
          int offsetEntity = pViewSens->GetSubCaptorIndexForDirection(direction,index);

          if (offsetEntity > -1)
          {
            int offsetEntitySource = pViewSens->GetSubCaptorIndexForDirection(direction,0);
            
            for (int j=0; j<VIEW_IDENTIFY_SIZE_PER_FOCUS; j++)
            {
              int rowBase = m_pBrain->GetBrainMatrixRowIndex( curSensUid, 1, 0, 0, 0);  
              for (int col=0; col <m_pBrain->GetDecisionNeuronTable()->GetNeuronTableColumnCount(); col++)
              {
                m_pBrain->GetDecisionNeuronTable()->SetNeuronTableData( rowBase+offsetEntity+j, col, m_pBrain->GetDecisionNeuronTable()->GetNeuronTableData(rowBase+offsetEntitySource+j, col) );
              }
            }
          }
        }
      }
    }
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_FAR )
    {
      CSensorViewFar* pViewSens = (CSensorViewFar*)pCurSens;

      for (int direction=0; direction<8; direction++)
      {
        for (int index=1; index<VIEW_MAX_NUMBER_FOCUSSED_ENTITIES; index++)
        {
          int offsetEntity = pViewSens->GetSubCaptorIndexForDirection(direction,index);

          if (offsetEntity > -1)
          {
            int offsetEntitySource = pViewSens->GetSubCaptorIndexForDirection(direction,0);
            
            for (int j=0; j<VIEW_SIZE_PER_FOCUS; j++)
            {
              int rowBase = m_pBrain->GetBrainMatrixRowIndex( curSensUid, 1, 0, 0, 0);  
              for (int col=0; col <m_pBrain->GetDecisionNeuronTable()->GetNeuronTableColumnCount(); col++)
              {
                m_pBrain->GetDecisionNeuronTable()->SetNeuronTableData( rowBase+offsetEntity+j, col, m_pBrain->GetDecisionNeuronTable()->GetNeuronTableData(rowBase+offsetEntitySource+j, col) );
              }
            }
          }
        }
      }
    }
    if ( (curSensUid & UID_BASE_MASK) == UID_BASE_SENS_VIEW_IDENTIFY_FAR )
    {
      CSensorViewIdentifyFar* pViewSens = (CSensorViewIdentifyFar*)pCurSens;

      for (int direction=0; direction<8; direction++)
      {
        for (int index=1; index<VIEW_MAX_NUMBER_FOCUSSED_ENTITIES; index++)
        {
          int offsetEntity = pViewSens->GetSubCaptorIndexForDirection(direction,index);

          if (offsetEntity > -1)
          {
            int offsetEntitySource = pViewSens->GetSubCaptorIndexForDirection(direction,0);
            
            for (int j=0; j<VIEW_IDENTIFY_SIZE_PER_FOCUS; j++)
            {
              int rowBase = m_pBrain->GetBrainMatrixRowIndex( curSensUid, 1, 0, 0, 0);  
              for (int col=0; col <m_pBrain->GetDecisionNeuronTable()->GetNeuronTableColumnCount(); col++)
              {
                m_pBrain->GetDecisionNeuronTable()->SetNeuronTableData( rowBase+offsetEntity+j, col, m_pBrain->GetDecisionNeuronTable()->GetNeuronTableData(rowBase+offsetEntitySource+j, col) );
              }
            }
          }
        }
      }
    }
  }

  return true;
}


bool CWizardBrain::AddVisualIdentificationBonus( int bonusRate, FormType_e form, ColorCaracterType_e color, 
                                                 TextureType_e texture, PhyAttributeType_e attribute, 
                                                 bool isMoving, bool isBigger, bool isSmaller,
                                                 IdentificationType_e  identity)
{
  // Prepare captor offsets
  int subOffsetForm      = -1;
  int subOffsetColor     = -1;
  int subOffsetTexture   = -1;
  int subOffsetAttrib    = -1;
  int subOffsetIsMove    = -1;
  int subOffsetIsBigger  = -1;
  int subOffsetIsSmaller = -1;

  if (form >= FORM_FIRST_TYPE)
    subOffsetForm    = CSensorView::GetSubCaptorSubIndexForForm(form);
  if (color >= COLOR_CARACTER_FIRST_TYPE)
    subOffsetColor   = CSensorView::GetSubCaptorSubIndexForColor(color);
  if (texture >= TEXTURE_FIRST_TYPE)
    subOffsetTexture = CSensorView::GetSubCaptorSubIndexForTexture(texture);
  if (attribute >= PHY_ATTRIBUTE_FIRST_TYPE)
    subOffsetAttrib = CSensorView::GetSubCaptorSubIndexForPhyAttribute(attribute);
  if (isMoving)
    subOffsetIsMove = CSensorView::GetSubCaptorSubIndexForMoving();
  if (isBigger)
    subOffsetIsBigger = CSensorView::GetSubCaptorSubIndexForSizeBig();
  if (isSmaller)
    subOffsetIsSmaller = CSensorView::GetSubCaptorSubIndexForSizeSmall();

  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetForm,      identity, bonusRate);
  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetColor,     identity, bonusRate);
  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetTexture,   identity, bonusRate);
  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetAttrib,    identity, bonusRate);
  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetIsMove,    identity, bonusRate/2);
  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetIsBigger,  identity, bonusRate/3);
  AddIdentifyBonusInList(UID_BASE_SENS_VIEW, subOffsetIsSmaller, identity, bonusRate/3);

  return true;
}

bool CWizardBrain::AddOdorIdentificationBonus( int bonusRate, OdorType_e odor, IdentificationType_e  identity)
{
  return AddIdentifyBonusInList(UID_BASE_SENS_SMELL, odor-ODOR_FIRST_TYPE, identity, bonusRate);
}

bool CWizardBrain::AddPheromoneIdentificationBonus( int bonusRate, PheromoneType_e phero, IdentificationType_e  identity)
{
  return AddIdentifyBonusInList(UID_BASE_SENS_PHEROMONE, phero-PHEROMONE_FIRST_TYPE, identity, bonusRate);
}