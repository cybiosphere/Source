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
// FILE: CBasicEntity.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a basic entity
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2002 : Creation
//
//===========================================================================

#if !defined(CBASICENTITY_INCLUDED_)
#define CBASICENTITY_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxwin.h>
//#include <afxtempl.h>

#include "Definitions.h"
#include "CBrain.h"
#include "CGenericParam.h"
#include "CGenericCaract.h"
#include "CGenome.h"
#include "CLifeStage.h"
#include "CybioXmlDef.h"

//===========================================================================
// Definitions            
//===========================================================================
class CBiotop;
class CBrain;
class CPhysicalWelfare;

#define COLOR_UNSET   0xFFFFFFFF
#define COLOR_DEFAULT 0x000F0F0F

#define MAX_VAL_WEIGHT  1000.0
#define STRING_GENE_UNUSED "Unused gene"
#define NB_STEPS_PER_GRID_SQUARE 10

typedef long int entityIdType;

typedef enum
{
  STATUS_STATIC = 0,
  STATUS_ALIVE,
  STATUS_DEAD,    
  STATUS_TOBEREMOVED
} StatusType_e;

typedef enum
{
  CONTROL_LOCAL_AUTO = 0,
  CONTROL_LOCAL_USER,
  CONTROL_REMOTE_AUTO
} ControlType_e;

typedef enum
{
  REPRODUCT_UNSET = -1,
  REPRODUCT_NONE,
  REPRODUCT_CLONAGE,
  REPRODUCT_SEXUAL, 
  REPRODUCT_NUMBER_TYPE,  
} ReproType_e;

typedef enum
{
  COLOR_CARACTER_UNSET = -1,
  COLOR_CARACTER_FIRST_TYPE,
  COLOR_CARACTER_WHITE = COLOR_CARACTER_FIRST_TYPE,
  COLOR_CARACTER_BLACK,
  COLOR_CARACTER_GREY,
  COLOR_CARACTER_RED,
  COLOR_CARACTER_BLUE,
  COLOR_CARACTER_GREEN,
  COLOR_CARACTER_YELLOW,
  COLOR_CARACTER_ORANGE,
  COLOR_CARACTER_PINK,
  COLOR_CARACTER_BEIGE,
  COLOR_CARACTER_BROWN,
  COLOR_CARACTER_NUMBER_TYPE
} ColorCaracterType_e;

typedef enum
{
  TEXTURE_UNSET = -1,
  TEXTURE_NONE,
  TEXTURE_FIRST_TYPE,
  TEXTURE_STRIPPED = TEXTURE_FIRST_TYPE,
  TEXTURE_SPOTTED,
  TEXTURE_REFLECT, 
  TEXTURE_NUMBER_TYPE
} TextureType_e;

typedef enum
{
  PHY_ATTRIBUTE_UNSET = -1,
  PHY_ATTRIBUTE_NONE,
  PHY_ATTRIBUTE_FIRST_TYPE,
  PHY_ATTRIBUTE_UDDER = PHY_ATTRIBUTE_FIRST_TYPE,
  PHY_ATTRIBUTE_HORNS,
  PHY_ATTRIBUTE_TUSKS,
  PHY_ATTRIBUTE_MANE,
  PHY_ATTRIBUTE_NUMBER_TYPE
} PhyAttributeType_e;

typedef enum
{
  ODOR_UNSET = -1,
  ODOR_NONE,
  ODOR_FIRST_TYPE,
  ODOR_AROMATIC = ODOR_FIRST_TYPE,
  ODOR_FRAGRANT,
  ODOR_ALLIACEOUS,
  ODOR_AMBROSIAL,
  ODOR_HIRCINE,
  ODOR_REPULSIVE,
  ODOR_NAUSEOUS,
  ODOR_NUMBER_TYPE
} OdorType_e;

typedef enum
{
  PHEROMONE_UNSET = -1,
  PHEROMONE_NONE,
  PHEROMONE_FIRST_TYPE,
  PHEROMONE_SEXUAL_MALE = PHEROMONE_FIRST_TYPE,
  PHEROMONE_SEXUAL_FEMALE,
  PHEROMONE_MATERNAL,
  PHEROMONE_BABY,
  PHEROMONE_DOMINANT,
  PHEROMONE_NUMBER_TYPE
} PheromoneType_e;

typedef enum
{
  FORM_UNSET = -1,
  FORM_FIRST_TYPE,
  FORM_UNKNOWN = FORM_FIRST_TYPE,
  FORM_PLAN_SMOOTH,
  FORM_PLAN_ROUGH,
  FORM_ROUNDISH,
  FORM_GEOMETRIC,
  FORM_VEGET_LONG_LEEF,
  FORM_VEGET_BUSH,
  FORM_VEGET_TREE,
  FORM_QUADRIPED_THIN,
  FORM_QUADRIPED_LONG_NECK,
  FORM_QUADRIPED_FAT,
  FORM_BIPED,
  FORM_CARRION,
// TBD: complete form type
  FORM_NUMBER_TYPE  
} FormType_e;

typedef enum
{
  TASTE_UNSET = -1,
  TASTE_NONE,
  TASTE_SWEET,
  TASTE_ACID,
  TASTE_SALTY,
  TASTE_BITTER,
  TASTE_UMANI,
  TASTE_NUMBER_TYPE,  
} TasteType_e;

typedef enum
{
  HABITAT_UNSET = -1,
  HABITAT_FIRST_TYPE,
  HABITAT_EARTH = HABITAT_FIRST_TYPE,
  HABITAT_AMPHIBIAN, 
  HABITAT_WATER, 
  HABITAT_AIR, 
  HABITAT_NUMBER_TYPE,  
} HabitatType_e;

typedef enum
{
  CONSUM_UNSET = -1,
  CONSUM_NONE,
  CONSUM_VEGETAL, 
  CONSUM_OMNI,
  CONSUM_MEAT,
  CONSUM_NUMBER_TYPE,  
} ConsumeType_e;

typedef enum
{
  MOVE_UNSET = -1,
  MOVE_NONE,
  MOVE_WALK, 
// TBD: complete move type
  MOVE_NUMBER_TYPE  
} MoveType_e;

typedef struct 
{
  double        data1Scale;
  const char*   data1Name;
  double        data2Scale;
  const char*   data2Name;
  double        data3Scale;
  const char*   data3Name;
  double        data4Scale;
  const char*   data4Name;
} GeneInterpreterDefinition_t;


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CBasicEntity 
{
//===========================================================================
// Attributes 
//===========================================================================

protected:
  
  StatusType_e        m_Status;        // status (alive, static)
  ControlType_e       m_Control;       // status (alive, static)
  string              m_Label;         // User defined name
  bool                m_bIsImmortal;   // Immortal mode used for beginers training
  bool                m_bIsDrinkable;
  int                 m_Generation;	   // Generation
  int                 m_TotalChildNumber; // Number of descendants
  entitySignatureType m_EntitySignature; // Entity signature used for identification 

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
private:
  //
  //        3   2   1        135 90 45
  //          \ | /            \ | / 
  //        4 --+-->0      180 --*-->0
  //          / | \            / | \ 
  //        5   6   7       225 270 315
  //       m_Direction    m_StepDirection

  int m_Direction;	    // [0..7] Direction in the Biotop
  int m_StepDirection;  // [0..360] Detailed direction in the Biotop

  //
  //        /______/ 2   First layer for entities
  //        /______/ 1   Ground/Grass/Water
  //        /______/ 0   Under Ground/Water
  //         m_Layer

  int m_Layer;          // Layer in the biotop
  int m_PrevLayer;	    // Previous layer in the biotop  

  //                  
  // m_StepCoord.y m_GridCoord.y ^
  //                             |       .
  //     NB_STEPS..............1 +...............
  //            .                +       .              
  //            2                +   X   .           
  //            1                +       .            
  //            0..............0 +-+-+-+-+-------->         
  //                             0 1 2 . NB_STEPS    m_StepCoord.x  
  //                             0       1           m_GridCoord.x

  Point_t m_GridCoord;      // Coordinate in the Biotop grid
  Point_t m_StepCoord;      // Detailed coord that share grid into NB_STEPS_PER_GRID_SQUARE steps
  Point_t m_PrevGridCoord;  // Previous coordinate in the Biotop used by biotop grid
  Point_t m_GuiGridCoord;   // Coordinate in the Biotop that can be used by GUI

  bool         m_IsAttached;  // Is attached to the biotop ?
  entityIdType m_Id;          // Unique ID (Set by Biotop)

  int       m_HourCounter;    // internal Day Counter
  bool      m_bHasMoved;      // Indicates if coord has changed. Must be clear after use
  bool      m_bHasChanged;    // Indicates if display has to be refresh. Must be clear after use

//---------------------------------------------------------------------------
// Caracteristics
//---------------------------------------------------------------------------
private:
  COLORREF            m_ColorRgb;           // Color in RGB
  ColorCaracterType_e m_ColorCaracter;      // Color definition
  SexType_e           m_Sex;                // male or female?
  OdorType_e          m_Odor;               // Odor in Linnaeus classification
  PheromoneType_e     m_Pheromone;          // Pheromone: inter-specie chemical communication. !
  ReproType_e         m_TypeOfReproduction; // Possible type of reproduction
  FormType_e          m_Silhouette;         // Form in the most significant orientation
  TextureType_e       m_Texture;            // Texture 
  TasteType_e         m_Taste;              // acid...
  HabitatType_e       m_Habitat;            // earth/water
  ConsumeType_e       m_ConsumeClass;       // 
  MoveType_e          m_MoveType;           // Moving capabilities

  CGenericCaract*     m_pPhyAttribute;      // Physical attribut 


//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
protected:
  CBiotop*          m_pBiotop;
  CBrain*           m_pBrain;   
  CGenome*          m_pGenome;
  CPhysicalWelfare* m_pPhysicWelfare;

//---------------------------------------------------------------------------
// Parameters
//---------------------------------------------------------------------------
protected:
  std::vector<CGenericParam*> m_tParam;

  // Parameters Id
  int m_id_Weight;
  int m_id_Toxicity;
  int m_id_Protection;
  int m_id_Camouflage;
  int m_id_Noise;

//---------------------------------------------------------------------------
// Life Stages
//---------------------------------------------------------------------------
private:
  std::vector<CLifeStage*> m_tLifeStage;
  int m_indCurrentLifeStage;

//---------------------------------------------------------------------------
// Gestation childs
//---------------------------------------------------------------------------
protected:  
  std::vector<CBasicEntity*> m_tGestationChilds;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CBasicEntity();
  virtual ~CBasicEntity();

//---------------------------------------------------------------------------
// Genetic settings
//---------------------------------------------------------------------------
public:
  bool setEntityFromGenome(double mutationRate = 0);

protected:
  virtual bool setSexFromGenome ();
  virtual bool setCaractFromGene (CGene* pGen);
  virtual bool completeCaractsWithDefault();
  virtual bool setParamFromGene (CGene* pGen);
  virtual bool completeParamsWithDefault();
  virtual bool setLifeStageFromGene (CGene* pGen);
  virtual bool completeLifeStagesWithDefault();
  virtual bool setPhysicWelfareFromGene (CGene* pGen);
  virtual bool completePhysicWelfareWithDefault();
  bool setBrainSensorFromGene (CGene* pGen);
  virtual bool setBrainReactionFromGene (CGene* pGen);
  virtual bool completeBrainSensorWithDefault();
  virtual bool completeBrainReactionWithDefault();
  virtual bool setBrainSizeFromGene (CGene* pGen);
  virtual bool setBrainInstinctFromGene (CGene* pGen);
  virtual bool setBrainConfigFromGene (CGene* pGen);
  virtual bool completeBrainInstinctWithDefault();
  virtual bool setFeelingFromGene (CGene* pGen);
  virtual bool setPurposeFromGene (CGene* pGen);
  void         computeEntitySignature();

public:
  virtual CSensor* getTemporarySensorFromGene (CGene* pGen);

//---------------------------------------------------------------------------
// Genetic description
//---------------------------------------------------------------------------
public:
  string getGeneDescriptionString(CGene* pGen);
  string getGeneNameString(CGene* pGen);

  static string getGeneNameData1 (GeneSubType_e subType);
  static string getGeneNameData2 (GeneSubType_e subType);
  static string getGeneNameData3 (GeneSubType_e subType);
  static string getGeneNameData4 (GeneSubType_e subType);

  static double getGeneScaleData1 (GeneSubType_e subType);
  static double getGeneScaleData2 (GeneSubType_e subType);
  static double getGeneScaleData3 (GeneSubType_e subType);
  static double getGeneScaleData4 (GeneSubType_e subType);

  virtual int  getExpectedBrainSensorWeightSize (CGene* pGen);

protected:
  virtual string buildCaracterString(CGene* pGen);
  virtual string buildParameterString(CGene* pGen);
  virtual string buildLifeStageString(CGene* pGen);
  virtual string buildPhysicWellfareString(CGene* pGen);
  virtual string buildSensorString(CGene* pGen);
  virtual string buildReactionString(CGene* pGen);
  virtual string buildBrainSizeString(CGene* pGen);
  virtual string buildBrainInstinctString(CGene* pGen);
  virtual string buildFeelingWellfareString(CGene* pGen);
  virtual string buildPurposeString(CGene* pGen);
  virtual string buildBrainConfigString(CGene* pGen);

//---------------------------------------------------------------------------
// Biotop Connection
//---------------------------------------------------------------------------
public:
  void attachToBiotop(CBiotop* Biotop);
  void detachFromBiotop();
  bool isToBeRemoved();
  void autoRemove();
  bool isAttachedToBiotop();

//---------------------------------------------------------------------------
// Caracter management
//---------------------------------------------------------------------------
  static ColorCaracterType_e convertRgbColorInCaracter(COLORREF rbg);

//---------------------------------------------------------------------------
// Parameters management
//---------------------------------------------------------------------------
protected:
  int addParameter(CGenericParam* pParam);
public:
  unsigned int getNumParameter();
  CGenericParam* getParameter(unsigned int id);
  CGenericParam* getParameterByName(string paramName);
  int getParamIdByName(string paramName);

private:
  void deleteAllParameters();

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  virtual void nextSecond();  // Called by biotop every time unit
  virtual void nextHour();    // Called by biotop every 3600 time units  
  virtual void nextDay(bool forceGrowth = false);     // Called every 24h at birth hour  
  virtual int  getAge();
  virtual bool checkVitalNeedsOk(); // Called by biotop every time unit

  void quickAgeing(int nbDays);   // Get older quickely

//---------------------------------------------------------------------------
// Life stages management
//---------------------------------------------------------------------------
public: 
  bool setCurrentLifeStages(LifeStage_e newStage); 
protected: 
  bool addLifeStage(CLifeStage* pLifeStage);
  virtual void enterInNewLifeStage(CLifeStage* pLifeStage);
private:
  void deleteAllLifeStages();

//---------------------------------------------------------------------------
// Behavior  
//---------------------------------------------------------------------------
public:
  virtual feedbackValType forceNextAction(choiceIndType myChoice);
  virtual choiceIndType   predictNextAction();

  virtual void autoKill();
  virtual bool moveLinear(int nbSteps);
  virtual bool turnRight(unsigned int nDegree);
  virtual bool turnLeft(unsigned int nDegree);
  virtual void doNothing();

  bool jumpToGridCoord(Point_t newGridCoord, int newLayer=-2);
  bool jumpToStepCoord(Point_t newStepCoord, int newLayer=-2);
  bool moveToGridEdgePos();
  bool moveToGridCenterPos();
  bool turnToCenterDir();

  bool checkHabitat(void);

//---------------------------------------------------------------------------
// Gestation childs management management
//---------------------------------------------------------------------------
private:
  void deleteAllGestationChilds(); 

//---------------------------------------------------------------------------
// Save/Load in file
//---------------------------------------------------------------------------
public:
  bool saveInFile(string fileName, string pathName, string newLabel="");
  static bool getGenomeFromFile(string fileName, string pathName, CGenome& genome);
  static bool getDefaultLayerFromFile(string fileName, string pathName, int& layer);
  static bool getEntityNameFromFile(string fileName, string pathName, string& name);
  bool loadInfoFromFile(string fileName, string pathName);
  bool loadParametersFromFile(string fileName, string pathName);
  bool loadBrainFromFile(string fileName, string pathName);

//---------------------------------------------------------------------------
// Save/Load in XML file
//---------------------------------------------------------------------------
public:
  bool saveInXmlFile(string fileName, string newLabel="");
  bool saveInXmlFile(TiXmlDocument *pXmlDoc, string newLabel="");
  static bool getGenomeFromXmlFile(string fileName, CGenome& genome);
  static bool getGenomeFromXmlFile(TiXmlDocument *pXmlDoc, CGenome& genome);
  static bool getDefaultLayerFromXmlFile(string fileName, int& layer);
  static bool getDefaultLayerFromXmlFile(TiXmlDocument *pXmlDoc, int& layer);
  static bool getEntityNameFromXmlFile(string fileName, string& name);
  static bool getEntityNameFromXmlFile(TiXmlDocument *pXmlDoc, string& name);
  bool loadDataFromXmlFile(string fileName);
  bool loadDataFromXmlFile(TiXmlDocument *pXmlDoc);
  bool loadBrainFromXmlFile(string fileName);
  bool loadBrainFromXmlFile(TiXmlDocument *pXmlDoc);
  

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:

  virtual double  changeWeight(double variation);
  void  forceWeight(double newWeight);

  Point_t         getGridCoord();
  Point_t         getPrevGridCoord();
  Point_t         getGuiGridCoord();
  Point_t         getGridCoordRelative(Point_t relativeCoord);
  Point_t         getStepCoord();
  Point_t         getStepCoordRelative(Point_t relativeCoord);
  int             getLayer();
  int             getPrevLayer();
  int             getDirection();
  void            setDirection(int direction);
  int             getStepDirection();
  void            setStepDirection(int stepDirection);
  entityIdType    getId();
  void            setId(entityIdType id);
  COLORREF        getColorRgb();
  void            setColor(COLORREF newColor);
  FormType_e      getForm();
  void            setForm(FormType_e newForm);
  TextureType_e   getTexture();
  void            setTexture(TextureType_e newText);
  string          getLabel();
  bool            setLabel(string newname);
  bool            isComestible();
  bool            isDrinkable();
  bool            isAlive();
  void            setUserControlled(bool active);
  bool            isUserControlled();
  bool            isLocalAutoControlled();
  void            setRemoteControlled(bool active);
  double          getToxicity();
  void            setToxicity(double newRate);
  OdorType_e      getOdor();
  TasteType_e     getTaste();
  void            setOdor(OdorType_e newOdor);
  PheromoneType_e getPheromone();
  void            setPheromone(PheromoneType_e pheroType);
  int             getGeneration();
  ReproType_e     getTypeOfReproduction();
  SexType_e       getSex();
  CGenome*        getGenome();
  double          getNoise();
  void            setNoise(int rate);
  double          getWeight();
  double          getMaxWeight();
  double          getMinWeight();
  double          getSizeRate();
  double          getCamouflage();
  void            setCamouflageToNominalRatio(double ratio);
  CBiotop*        getBiotop();
  CBrain*         getBrain();
  bool            checkIfhasMoved();
  bool            checkIfhasChanged();
  bool            checkIfhasMovedAndClear();
  bool            checkIfhasChangedAndClear();
  void            forceHasChanged();
  double          getProtection();
  void            setProtection(double newRate);
  ConsumeType_e   getConsumeClass();
  virtual ClassType_e getClass();
  CLifeStage*     getCurrentLifeStage();
  CLifeStage*     getLifeStage(int index);
  unsigned int    getNbLifeStages();
  void            setImmortal(bool isImmortal);
  bool            isImmortal();
  StatusType_e    getStatus();
  void            setStatus(StatusType_e newStatus);

  ColorCaracterType_e getColorType();
  bool                isPhyAttributePresent(PhyAttributeType_e type);
  void                setAttribute(PhyAttributeType_e newAttrib);
  void                removeAttribute(PhyAttributeType_e delAttrib);
  PhyAttributeType_e  getMainPhyAttribute();
  DWORD               getAttributePresenceMask();
  CPhysicalWelfare*   getpPhysicalWelfare();
  string              getSpecieName();
  entitySignatureType getEntitySignature();
  
      
  virtual int    getCurrentSpeed();

  static string  getColorStrName(ColorCaracterType_e type);
  static string  getOdorStrName(OdorType_e type);
  static string  getPheromoneStrName(PheromoneType_e type);
  static string  getFormStrName(FormType_e type);
  static string  getTextureStrName(TextureType_e type);
  static string  getPhyAttributeStrName(PhyAttributeType_e type);
  static string  getRelativePosStrName(int index);
  static string  getAbsolutePosStrName(int index);
  static string  getTasteStrName(TasteType_e type);
  static string  getHabitatStrName(HabitatType_e type);
  static string  getConsumeStrName(ConsumeType_e type);
  static string  getMoveStrName(MoveType_e type);
  static string  getReproStrName(ReproType_e type);
  static Point_t getGridCoordFromStepCoord(Point_t stepCoord);

};

#endif // !defined(CBASICENTITY_INCLUDED_)


