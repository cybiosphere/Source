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
// FILE: CAnimal.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a thinking and moving entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined(CANIMAL_INCLUDED_)
#define CANIMAL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CBasicEntity.h" 
#include "CBrainAnimal.h"
#include "CFeelingFear.h"

//===========================================================================
// Definitions            
//===========================================================================
typedef enum
{
  BRAIN_BEHAVIOR_UNSET = -1,
  BRAIN_BEHAVIOR_NONE,
  BRAIN_BEHAVIOR_FIRST_TYPE,
  BRAIN_BEHAVIOR_BABY_STAY_HOME = BRAIN_BEHAVIOR_FIRST_TYPE,
  BRAIN_BEHAVIOR_NUMBER_TYPE
} BrainBehaviorType_e;

#define VIGILANCE_RATE_MAX     100.0
#define VIGILANCE_RATE_NORMAL   80.0
#define VIGILANCE_RATE_LOW      60.0
#define VIGILANCE_RATE_SLEEP     1.0


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CAnimal : public CBasicEntity 
{
//===========================================================================
// Attributes 
//===========================================================================
private:
  int  m_HeadDirection;	// [0..7] Looking direction of head in the Biotop (Used for turn head action)
  int  m_BusySecondCounter;
  bool m_bIsSleeping;
  int  m_tTasteLevel[TASTE_NUMBER_TYPE];
  double m_ConsumedEnergy;
  CFeelingFear* m_pFeelingFear;

  // Forbiden action can be used to punish an action sytematically, whatever is the evoluton of feeling welfare.
  choiceIndType m_forbidenActionInd;
  int           m_forbidenActionCount;

//---------------------------------------------------------------------------
// Parameters Ids
//---------------------------------------------------------------------------
private:
  size_t m_id_Age;
  size_t m_id_Decomposition;
  size_t m_id_ReproductionRate;
  size_t m_id_Health;
  size_t m_id_Hunger;
  size_t m_id_Thirst;
  size_t m_id_StomachFilling;
  size_t m_id_Libido;
  size_t m_id_Suffering;
  size_t m_id_Pleasure;
  size_t m_id_Tiredness;
  size_t m_id_GrowthSpeed;
  size_t m_id_FatWeight;
  size_t m_id_AttackFactor;
  size_t m_id_Curiosity;
  size_t m_id_Learning;
  size_t m_id_CurrentSpeed;
  size_t m_id_Fear;
  size_t m_id_Vigilance;
  size_t m_id_ResistanceToPoison;

//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public: 
  CAnimal(string label, Point_t initCoord, size_t layer, CGenome* pGenome);
  CAnimal(string label, CAnimal& model);
  CAnimal(string label, CAnimal& mother,CAnimal& father);
  ~CAnimal();

//---------------------------------------------------------------------------
// Genetic settings
//---------------------------------------------------------------------------
protected: 
  bool setParamFromGene (CGene* pGen);
  bool completeParamsWithDefault();
  bool setPhysicWelfareFromGene (CGene* pGen);
  bool completePhysicWelfareWithDefault();
  bool setBrainReactionFromGene (CGene* pGen);
  bool completeBrainSensorWithDefault();
  bool completeBrainReactionWithDefault();
  bool setBrainSizeFromGene (CGene* pGen);
  bool setBrainInstinctFromGene (CGene* pGen);
  bool setBrainConfigFromGene (CGene* pGen);
  bool completeBrainInstinctWithDefault(void);
  bool setFeelingFromGene (CGene* pGen);
  bool setPurposeFromGene (CGene* pGen);

public: 
  bool setBrainInstinctInGenes(void);
  CSensor* getTemporarySensorFromGene (CGene* pGen);

//---------------------------------------------------------------------------
// Genetic description
//---------------------------------------------------------------------------
public:
  size_t getExpectedBrainSensorWeightSize (CGene* pGen);

protected:
  string buildPhysicWellfareString(CGene* pGen);
  string buildSensorString(CGene* pGen);
  string buildReactionString(CGene* pGen);
  string buildBrainSizeString(CGene* pGen);
  string buildBrainInstinctString(CGene* pGen);
  string buildBrainConfigString(CGene* pGen);
  string buildFeelingWellfareString(CGene* pGen);
  string buildPurposeString(CGene* pGen);

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:   
  void nextSecond();
  void nextHour();
  void nextDay(bool forceGrowth = false);
  int  getAge();
  int  getDecompositionTime();

//---------------------------------------------------------------------------
// Heatlth management
//---------------------------------------------------------------------------
  bool checkVitalNeedsOk();

protected:
  void balanceWeightAndMetabolism(bool forceGrowth);
  std::string getDeathCauseString();

//---------------------------------------------------------------------------
// Weight management
//---------------------------------------------------------------------------
protected:
  void increaseFatWeight(double weightToAdd);
  void consumeFatWeight(double weightToRemove);
  void useFatWeightToGrow(double weightToTransfer);

//---------------------------------------------------------------------------
// Parameters management
//---------------------------------------------------------------------------
protected:
  void changeHungerRate(double variation);
  void changeThirstRate(double variation);
  void changeLibidoRate(double variation);
  bool changeHealthRate(double variation, CBasicEntity* pAggresor=NULL, bool suffering = true);
  void changeSufferingRate(double variation);
  void changePleasureRate(double variation);
  void convergePleasureRateToNeutral(double variation);
  void changeTirednessRate(double variation);
  void forceTirednessRate(double newRate);
  void changeFearRate(double variation);
public:
  void changeCurrentSpeed(double variation);
  void forceCurrentSpeed(double newSpeed);
  void stopCurrentSpeed();
  void changeStomachFillingRate(double variation);

//---------------------------------------------------------------------------
// Behavior  
//---------------------------------------------------------------------------
public:  
  virtual feedbackValType forceNextAction(choiceIndType myChoice);
  virtual choiceIndType   predictNextAction();

  virtual bool ExecuteTurnRightAction(unsigned int nDegree);
  virtual bool ExecuteTurnLeftAction(unsigned int nDegree);
  virtual bool ExecuteMoveForwardAction(double successSatisfactionFactor, double failureFrustrationFactor, int nbSteps);
  virtual bool ExecuteMoveBackwardAction(double successSatisfactionFactor, double failureFrustrationFactor, int nbSteps);
  virtual bool ExecuteEatAction(int relLayer, double successSatisfactionFactor, double failureFrustrationFactor);
  virtual bool ExecuteDrinkAction(double successSatisfactionFactor, double failureFrustrationFactor);
  virtual bool ExecuteCopulateAction(double successSatisfactionFactor, double failureFrustrationFactor);
  virtual bool ExecuteAttackAction(int relLayer, int stepRange, double successSatisfactionFactor, double failureFrustrationFactor, ReactionIntensityType_e intensity);
  virtual bool ExecuteSleepAction(void);

  void wakeUp(void);
  void lookForward(void);
  void turnHeadLeft(void);
  void turnHeadRight(void);

  void setForbidenActionInd(choiceIndType actionInd);
  string getForbidenActionLabel();
  int getForbidenActionCount();
  bool SetCurrentReactionIndex(size_t index);

protected:
  bool checkConsumeClass(ClassType_e eatenClass);
  void logDeathCause(std::string deathCauseStr = "\n");

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:   
  double getReproductionRate();
  double getLifeExpectation();
  double getRotenTimeExpectation();
  double getHungerRate();
  double getThirstRate();
  double getHealthRate();
  double getStomachFillingRate();
  double getLibidoRate();
  double getSufferingRate();
  double getPleasureRate();
  double getTirednessRate();
  double getCuriosityRate();
  double getLearningRate();
  void   setCuriosityToNominalRatio(double ratio);
  void   setLearningToNominalRatio(double ratio);
  void   setGrowthSpeedToNominalRatio(double ratio);
  void   setAttackFactorToNominalRatio(double ratio);
  double getAttackFactor();
  bool   isSleeping();
  double getFatWeight();
  int    getCurrentSpeed();
  int    getHeadDirection();
  int*   getpTasteLevelTable();
  double getFearRate();
  double getVigilance();
  void   setVigilance(double rate);
  void setHungerRate(double rate);
  void setThirstRate(double rate);
  void setLibidoRate(double rate);
  void setHealthRate(double rate);
  void setSufferingRate(double rate);
  void setPleasureRate(double rate);
  void setTirednessRate(double rate);
  void setFatWeight(double fatWeight);
  void setFearRate(double rate);
  void setStomachFillingRate(double rate);
  double getResistanceToPoison();
  void consumeEnergy(double unit);
}; // end ClAnimal

#endif // !defined(CANIMAL_INCLUDED_)



