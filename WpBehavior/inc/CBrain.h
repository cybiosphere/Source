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

// Brain.h: interface for the CBrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRAIN_H__AA2E81DE_1CD7_4BEF_B88B_0B04AD788087__INCLUDED_)
#define AFX_BRAIN_H__AA2E81DE_1CD7_4BEF_B88B_0B04AD788087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Includes 
//===========================================================================
#include "Definitions.h"
#include "Matrix.h"

typedef size_t choiceIndType;

#include "CNeuronMatrix.h"
#include "CSensor.h"
#include "CReaction.h"
#include "CBasicEntity.h"
#include "CPurpose.h"
#include "CGenome.h"
#include "CybioXmlDef.h"

//===========================================================================
// Definitions            
//===========================================================================

#define UID_UNSET                    0xFFFFFFFF

#define IDENTIFY_INPUT_SIZE (VIEW_SIZE_PER_FOCUS + NUMBER_ODORS + NUMBER_PHEROMONES + 1)

class CNeuronMatrix;
class CSensor;
class CReaction;
class CPurpose;
class CBasicEntity;
class CFeelingWelfare;
class CGeoMapPurpose;
class CSensorPheromone;
class CSensorSmell;


typedef struct 
{
  entityIdType previousEntityId;
  entityIdType newEntityId;
  double       computedWeight;
  DWORD        captorUid;
  size_t       subcaptorIndex;
  size_t       subcaptorsSize;
} BrainFocusedEntityView_t;


typedef enum
{
  IDENTIFICATION_NEUTRAL = 0, // static
  IDENTIFICATION_DANGER,      // static
  IDENTIFICATION_FOOD,        // static
  IDENTIFICATION_DRINK,       // static
  IDENTIFICATION_HOME,        // static
  IDENTIFICATION_BABY,
  IDENTIFICATION_MOTHER,
  IDENTIFICATION_FRIEND, 
  IDENTIFICATION_SEX_PARTNER,
  IDENTIFICATION_DOMINANT,
  IDENTIFICATION_PREY,
  IDENTIFICATION_PREDATOR,
  IDENTIFICATION_NUMBER_TYPE
} IdentificationType_e;

typedef struct 
{
  entitySignatureType   entitySignature;
  IdentificationType_e  identity;
} BrainMemorizedEntityIdentity_t;

#define MAX_MEMORIZED_IDENTITIES 20

#define IDENTIFICATION_STATIC_NUMBER_TYPE IDENTIFICATION_BABY
#define IDENTIFICATION_MOBILE_NUMBER_TYPE (IDENTIFICATION_NUMBER_TYPE - IDENTIFICATION_BABY)


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CBrain  
{

//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// global management
//---------------------------------------------------------------------------
protected:
  CBasicEntity* m_pEntity;

//---------------------------------------------------------------------------
// Feeling/sensors/reactions/geomap management
//---------------------------------------------------------------------------
private:
  CFeelingWelfare* m_pFeelingWelfare;
  double m_PreviousFeelingWelfare;
  double m_CurrentFeelingWelfare;

  // Sensors table
  std::vector<CSensor*> m_tSensors;
  // Reaction table
  std::vector<CReaction*> m_tReactions;
  // Purpose table
  std::vector<CPurpose*> m_tPurposes;

  CPurpose* m_pCurrentPurpose;

  CSensorSmell*     m_pSensorSmell;     // keep a reference to smell sensor
  CSensorPheromone* m_pSensorPheromone; // keep a reference to pheromon sensor

  CGeoMapPurpose* m_pGeoMap;

  // Attention is focused on 1 particular entity
  BrainFocusedEntityView_t m_FocusedEntityInfo;

//---------------------------------------------------------------------------
// Brain decision process
//---------------------------------------------------------------------------
private:
  // DIMENSSIONS
  // Total number of inputs from sensors. 1 sensor may control several inputs
  size_t m_nInputSensors;
  // Historical used to take decision 
  size_t m_nInputHistory;
  // Historical used to compute experience 
  size_t m_nExperienceHistory;
  // Size of geomap for purpose memory 
  size_t m_GeoMapSize;

  // VMATRIX/VECTORS for decision neuronal table (compute final action choice)
  // Current Input containing current and historized sensations
  CMatrix m_vCurrentDecisionInput;
  // Current choice vector
  CMatrix m_vCurrentDecisionChoice;
  // buffer used to store different choice according to 
  int* m_pChoiceDecisionBuffer; 

  // Neuronal matrix table for decision
  CNeuronMatrix m_mDecisionNeuronTable;
  // Memorized inputs
  CMatrix m_mInputDecisionHistory;
  // Memorized feedbacks
  CMatrix m_mDecisionHistory;

  choiceIndType m_CurrentReactionChoice;

  double m_historyWeight; // [0..2] : <1 give less importance to past >1 give more importance to past than present

//---------------------------------------------------------------------------
// Brain identification process
//---------------------------------------------------------------------------
private:
  bool m_bIdentificationUsed;
  // VMATRIX/VECTORS for identification neuronal table (Associate category to all visible entities)
  // Neuronal matrix table for identification
  CNeuronMatrix m_mIdentifyNeuronTable;
  // Current Input
  size_t  m_nInputIdentification;
  CMatrix m_vCurrentIdentifyInput;
  // Current choice vector
  CMatrix m_vCurrentIdentifyOutput; // raw vector result
  CMatrix m_vCurrentIdentificationChoice; // threhold vector result
  std::vector<BrainMemorizedEntityIdentity_t*> m_tBrainMemorizedEntityIdentities;
public:
  static string  getIdentityStrName(IdentificationType_e type);

//---------------------------------------------------------------------------
// Brain special behaviors
//---------------------------------------------------------------------------
private:
    bool m_bBabyStayHome;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CBrain();
	virtual ~CBrain();

//---------------------------------------------------------------------------
// global management
//---------------------------------------------------------------------------
public:
  CBasicEntity* GetEntity();
  void NextSecond();
  void NextDay();


//---------------------------------------------------------------------------
// Feeling/sensors/reactions/purpose/geomap management
//---------------------------------------------------------------------------
protected:
  bool DeleteAllSensors (void);
  bool DeleteAllReactions (void);
  bool DeleteAllPurposes(void);
  bool DeleteAllBrainMemorizedEntityIdentities(void);

public:
  size_t AttachSensor(CSensor*   pSensor);
  size_t AttachReaction(CReaction* pReaction);
  size_t AttachPurpose(CPurpose*  pPurpose);
  size_t GetNumberSensor();
  size_t GetNumberReaction();
  size_t GetNumberPurpose();

  CSensor*   GetSensorByIndex(size_t index);
  CSensor*   GetSensorByUniqueId(DWORD uniqueId);
  CReaction* GetReactionByIndex(size_t index);
  CReaction* GetReactionByUniqueId(DWORD uniqueId);
  CPurpose*  GetPurposeByIndex(size_t index);
  CPurpose*  GetPurposeByUniqueId(DWORD uniqueId);
  CPurpose*  GetPurposeByLabel(string label);
  CPurpose*  GetCurrentPurpose(void);
  size_t GetCurrentPurposeIndex(void);
  CPurpose*  GetPurposeByTriggerSensor(DWORD sensorUniqueId, size_t sensorSubCaptorIndex);
  CGeoMapPurpose* GetGeographicMap(void);
  void       ForceCurrentPurpose(size_t purposeIndex);

  bool PollAllSensors (void);
  bool AddFeelingWelfareSensitivity(CSensor* pSens, size_t tableSensiSize, double* pTableSensi);

  
  entityIdType getBrainFocusedEntityId(void);
  void proposeNewFocusedEntityCandidate(CBasicEntity* pEntity, double computedWeight, double specialWeightBonus,
                                        DWORD captorUid, size_t subcaptorIndex, size_t subcaptorsSize);
  void ForceBrainFocusedEntityId(entityIdType focusedEntityId);
  void clearBrainFocusedEntityInfo(void);

  void ResetReactionsFailureSuccessFactor();

  double GetViewedEntityWeight(CBasicEntity* pEntity);


//---------------------------------------------------------------------------
// Brain file management
//---------------------------------------------------------------------------
  bool saveInXmlFile(string fileNameWithPath);
  bool saveInTiXmlFile(TiXmlDocument *pXmlDoc);
  bool loadFromXmlFile(string fileNameWithPath);
  bool loadFromTiXmlFile(TiXmlDocument *pXmlDoc);

  bool exportDecisionInCsvFile(string fileNameWithPath);
  bool importDecisionFromCsvFile(string fileNameWithPath);

  bool exportIdentificationInCsvFile(string fileNameWithPath);

//---------------------------------------------------------------------------
// Brain decision process
//---------------------------------------------------------------------------
public:
  CNeuronMatrix* GetDecisionNeuronTable();
  bool SetNumberInputHistory (size_t nbHistory);
  bool SetNumberExperienceHistory (size_t nbHistory);
  size_t  GetCurrentReactionIndex(void);
  bool SetCurrentReactionIndex(size_t index);
  size_t  GetReactionIndexByLabel(string label);
  double GetPreviousFeelingWelfare();
  double GetCurrentFeelingWelfare();
  CFeelingWelfare* GetpFeelingWelfare();
 
  bool InitializeNeuronTableNeutral();
  bool HistorizeInput (void);
  bool GetVectorChoiceThresholds (double curiosityRate, neuroneValType &maxVal, neuroneValType &midThreshold, neuroneValType &lowThreshold);
  choiceIndType ComputeAndGetDecision(double curiosityRate, ReactionIntensityType_e &intensity);
  bool HistorizeDecision (choiceIndType index);
  feedbackValType ComputeFeedback (choiceIndType myChoice, ReactionIntensityType_e myIntensity);
  feedbackValType GetCurrentFeedback ();
  bool MemorizeExperience(feedbackValType currentFeedback,double learningRate);

  CMatrix* GetChoiceVect();
  CMatrix* GetInputVect();
  size_t GetChoiceVectSize();
  neuroneValType GetChoiceVectData(size_t pos);
  size_t GetInputVectSize();
  neuroneValType GetInputVectData(size_t pos);

  string GetColumnLabel(size_t index);
  string GetRowLabel(size_t index);

  size_t GetBrainMatrixColumnIndex(DWORD reactionUidBase, size_t reactionUidRange, size_t foundIndex = 0);
  size_t GetBrainMatrixRowIndex(DWORD sensorUidbase, size_t sensorUidRange, size_t subCaptorIndex, size_t timeHistory, size_t foundIndex = 0);
  size_t GetIdentifyMatrixRowIndex(DWORD sensorUidbase, size_t subCaptorIndex);
  bool IsDecisionRowSexSpecific(size_t rowIndex);

  bool ChangeDecisionNeuronTableVal(size_t row, size_t col, double variation, bool normalize=true);


//---------------------------------------------------------------------------
// Brain identification process
//---------------------------------------------------------------------------
public:
  CNeuronMatrix* GetIdentifyNeuronTable();

  // Identification methodes
  string getIdentificationLabel(size_t columnIndex);
  string getIdentifyInputLabel(size_t rowIndex);
  bool IsIdentifyRowSexSpecific(size_t rowIndex);

  CMatrix* GetIdentifyInputVect();
  CMatrix* GetIdentifyOutputVect();
  bool GetVectorIdentifyThresholds (neuroneValType &maxVal, neuroneValType &midThreshold, neuroneValType &lowThreshold);
  CMatrix* ComputeAndGetIdentification(CBasicEntity* pEntity, bool useOdors = true);
  
  bool CheckIfEntityIdentityNotMemorized(entitySignatureType entitySignature, IdentificationType_e identity);
  bool MemorizeIdentificationExperience(feedbackValType currentFeedback,double learningRate, CBasicEntity* pEntity, IdentificationType_e identity);
  bool ChangeIdentifyNeuronTableVal(size_t row, size_t col, double variation, bool normalize=true);
  bool IsIdentificationUsed();

protected:
  void UpdateIdentifyInputVector(CBasicEntity* pEntity, bool useOdors);
  bool ReenforceFullIdentity(feedbackValType currentFeedback, IdentificationType_e identity);


//---------------------------------------------------------------------------
// Brain special behaviors
//---------------------------------------------------------------------------
public:
  bool SetGeoMapSize(size_t geoMapSize);
  void CreateGeoMapArroudCurrentPosition();
  void SetBabyStayHome(bool stay);
  bool IsBabyStayHome();
  bool SetHomePurposePositionInGeoMap();

};

#endif // !defined(AFX_BRAIN_H__AA2E81DE_1CD7_4BEF_B88B_0B04AD788087__INCLUDED_)
