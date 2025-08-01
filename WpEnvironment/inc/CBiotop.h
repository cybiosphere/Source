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
// FILE: CBiotop.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a biotop environment
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/09/2002 : Creation
//
//===========================================================================

#if !defined(CBIOTOP_INCLUDED_)
#define CBIOTOP_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include <chrono>
#include <bitset>
#include "Definitions.h" 
#include "CBasicEntity.h" 
#include "CWater.h"
#include "CGrass.h"
#include "CRock.h"
#include "CCyclicParam.h"
// Measures
#include "CMeasureEntityParam.h"
#include "CMeasureBiotopParam.h"
#include "CMeasureFeeling.h"
#include "CMeasurePopulation.h"
#include "CMeasureReaction.h"
#include "CGeoMapPopulation.h"
// For odor mgt
#include "CSensorSmell.h"

//===========================================================================
// Definitions            
//===========================================================================
class CBasicEntity;
class CGeoMapPopulation;

#define MAX_NUMBER_OMP_THREADS 4

#define MAX_NUMBER_ANIMALS    2000
#define MAX_NUMBER_NON_ANIMAL_ENTITIES 200000
#define MAX_NUMBER_ENTITIES (MAX_NUMBER_ANIMALS + MAX_NUMBER_NON_ANIMAL_ENTITIES)

#define MAX_NUMBER_RANDOM_ENTITIES 3

// Sectors arround entity X:
//  44443332222
//  55443332211
//  55554321111
//  55555X11111  
//  55556781111
//  55667778811
//  66667778888

#define SECTOR_EE_BIT_MASK 0x01  // Sector 1
#define SECTOR_NE_BIT_MASK 0x02  // Sector 2
#define SECTOR_NN_BIT_MASK 0x04  // Sector 3
#define SECTOR_NW_BIT_MASK 0x08  // Sector 4
#define SECTOR_WW_BIT_MASK 0x10  // Sector 5
#define SECTOR_SW_BIT_MASK 0x20  // Sector 6
#define SECTOR_SS_BIT_MASK 0x40  // Sector 7
#define SECTOR_SE_BIT_MASK 0x80  // Sector 8

typedef enum
{
  LAYER_OVER_GROUND =     0,
  LAYER_UNDER_GROUND,
  LAYER_OVER_WATER,
  LAYER_UNDER_WATER, 
  LAYER_OVER_WET_GROUND,
  LAYER_IN_AIR,
  LAYER_GLOBAL_GRASS,
  LAYER_GLOBAL_ROCK
} LayerType_e;

typedef struct 
{
  //entityIdType id;
  CBasicEntity* pEntity;
  size_t distance;
} FoundEntity_t;

typedef struct 
{
  LayerType_e layerType;
  CBasicEntity* pEntity;
} BiotopCube_t;

#define MAX_ODOR_TRACE_VAL    100

typedef struct 
{
  BYTE     odorTrace[NUMBER_ODORS];
  char     fertilityBonus;
  COLORREF customColor;                 // Value that can be used by GUI to display user defined colors on the map
  WORD     mapId;                       // Value that can be used by GUI to display paterns on the map
  WORD     altitude;
} BiotopSquare_t;

typedef enum
{
  BIO_PARAM_FERTILITY     =     0,
  BIO_PARAM_SUNLIGHT      =     1,
  BIO_PARAM_RADIOACTIVITY =     2,
  BIO_PARAM_TEMPERATURE   =     3,
  BIO_PARAM_NUMBERT_TYPE  =     4
} BiotopParameterType_e;

typedef enum
{
  ENTITY_ID_INVALID =  -1,
  ENTITY_ID_WATER =     0,
  ENTITY_ID_GRASS,
  ENTITY_ID_ROCK,
  ENTITY_ID_FIRST_USER_ENTITY
} ReservedEntityIdType_e;

typedef enum
{
  BIOTOP_EVENT_ENTITY_ADDED = 0,
  BIOTOP_EVENT_ENTITY_MODIFIED,
  BIOTOP_EVENT_ENTITY_REMOVED,
  BIOTOP_EVENT_ENTITY_MOVED,
  BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE,
  BIOTOP_EVENT_ENTITY_BEHAVIOR_CHANGE,
} EntityEventList_e;

typedef struct
{
  std::bitset<8>    eventList;
  CBasicEntity*     pEntity;
  bool              markAsReadByGui;
  bool              markAsReadByNetwork;
} BiotopEvent_t;

using BiotopEventPair = std::pair<entityIdType, BiotopEvent_t>;

typedef enum
{
  BIOTOP_CPUMARKER_TOTAL = 0,
  BIOTOP_CPUMARKER_ANIMALS,
  BIOTOP_CPUMARKER_CUSTOM1,
  BIOTOP_CPUMARKER_CUSTOM2,
  BIOTOP_CPUMARKER_LAST
} BiotopCpuMarkerType_e;

typedef struct
{
  std::chrono::time_point<std::chrono::system_clock> startTime;
  std::chrono::time_point<std::chrono::system_clock> endTime;
  double cpuTimeCumulated;
} BiotopCpuMonitoring_t;

typedef struct
{
  string entityPathName;
  string entityFileName; // including .xml extension
  CBasicEntity* pModelEntity;
  int intensity; // scale 0..100
  int avaragePeriodicity; // avarage number of days between 2 occurences
  bool IsProportionalToFertility; // If true, both intensity and avaragePeriodicity are modulated by fertility rate
} BiotopRandomEntitiyGeneration_t;

typedef struct
{
  entityIdType entityId;
  size_t layer;
  size_t stepCoordX;
  size_t stepCoordY;
  int stepDirection;
} BiotopEntityPosition_t;


using BiotopFoundIdsList = std::array<FoundEntity_t, MAX_FOUND_ENTITIES>;

typedef struct
{
  size_t nbFoundIds;
  BiotopFoundIdsList tFoundIds;
} BiotopFoundIds_t;

typedef enum
{
  CLIMATE_TROPICAL = 0,
  CLIMATE_SEMI_ARID,
  CLIMATE_ARID,
  CLIMATE_TEMPERATE,
  CLIMATE_CONTINENTAL,
  CLIMATE_POLAR
} ClimateType_e;

//===========================================================================
//                                    CLASS            
//=========================================================================== 
class DLL_CYBIOCORE_API CBiotop 
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
private:
  string         m_Label;
  Point_t        m_Dimension;
  Point_t        m_GlobalGridDimension;
  Point_t        m_GlobalGridCoordOffset;
  Point_t        m_GlobalStepCoordOffset;
  size_t         m_nbLayer;
  entityIdType   m_IdLastEntity;
  int            m_IndexLastAnimal;
  BiotopTime_t   m_BioTime;
  unsigned char  m_NextHourTimeOffset; // Time offset for nextHourForAllEntities to avoid entity generation colisions between instances
  CCyclicParam*  m_pFertilityRate;     // 0 to 100 
  CCyclicParam*  m_pSunlightRate;      // 0 to 100  
  CGenericParam* m_pRadioactivity;     // 0 to 100
  CCyclicParam*  m_pTemperature;       // -50 to 50

  BiotopCube_t***  m_tBioGrid;    // Contain the info for all volumes in biotop
  BiotopSquare_t** m_tBioSquare;  // Contain the info for all surfaces in biotop

  std::array<BiotopFoundIds_t, MAX_NUMBER_OMP_THREADS>  m_BiotopFoundIdsArray;  // structure to store temporarily found entities 

  //        3   2   1    
  //          \ | /          
  //        4 --+-->0     
  //          / | \       
  //        5   6   7      
  //       m_WindDirection
  int m_WindDirection;
  int m_WindStrenght; // 0,1 or 2

  string m_DefaultFilePath;

  BiotopCpuMonitoring_t m_CpuMonitoring[BIOTOP_CPUMARKER_LAST];
  bool m_bColorizeSearch; 
  CGene m_GeneToMark{};
  bool m_bMarkDominantAlleleOnly{ false };
  string m_AutoSaveSpecieName{ "" };

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
  std::vector<CBasicEntity*> m_tEntity;
  CWater* m_pWaterGlobalEntity;
  CGrass* m_pGrassGlobalEntity;
  CRock* m_pRockGlobalEntity;
  std::vector<CMeasure*> m_tMeasures;
  std::vector <CGeoMapPopulation*> m_tGeoMapSpecies;
  std::vector<CGenericParam*> m_tParam;
  std::map<entityIdType, BiotopEvent_t> m_tEventsOdd;
  std::map<entityIdType, BiotopEvent_t> m_tEventsEven;
  std::vector<BiotopRandomEntitiyGeneration_t> m_tRandomEntitiesGeneration{};

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
  CBiotop(size_t dimX, size_t dimY, size_t dimZ, string logFileName = "CybioCore.log") ;
  virtual ~CBiotop();

  CBiotop* extractNewBiotopFromArea(const Point_t& startCoord, size_t dimX, size_t dimY);
  void resetResizeAndAddDefaultEntities(size_t dimX, size_t dimY);


//---------------------------------------------------------------------------
// Entities management
//---------------------------------------------------------------------------
private:
  size_t deleteEntity(CBasicEntity* pEntity, bool displayLog = true);
  void deleteAllParameters(void);
  void clearAllElements(void);

public:
  bool addEntity(CBasicEntity* pEntity, Point_t globalGridCoord, size_t newLayer = invalidCoord);
  bool addEntityWithPresetId(entityIdType idEntity, CBasicEntity* pEntity, Point_t globalStepCoord, bool chooseLayer, size_t newLayer);
  CBasicEntity* createAndAddEntity(string name, Point_t globalGridCoord, size_t layer, CGenome* pGenome);
  CBasicEntity* createAndAddEntity(string fileNameWithPath, string pathName, Point_t globalGridCoord, size_t layer = invalidCoord);
  CBasicEntity* createAndAddEntity(TiXmlDocument *pXmlDoc, Point_t globalGridCoord, size_t layer);
  CBasicEntity* createAndAddCloneEntity(entityIdType idModelEntity, Point_t cloneGlobalCoord, size_t cloneLayer, string cloneName = "");

  bool resetEntityGenome(entityIdType idEntity, CGenome* pNewEntityGenome);
  bool replaceEntityByAnother(entityIdType idEntity, CBasicEntity* pNewEntity);

  void displayEntities(void);
  void setDefaultMapAndEntities(void);
  void buildWoodenFence(const Point_t& startCoord, const Point_t& endCoord);
  void buildRock(const Point_t& coord);

  void deleteAllEntities(void);
  void deleteAllMeasures(void);
  void deleteAllPopulationMaps(void);

  size_t getNbOfEntities(void);
  size_t getNbOfAnimals(void);
  size_t getNbOfLivingAnimals(void);
  size_t getNbOfVegetals(void);
  size_t getNbOfMinerals(void);
  size_t getNbOfSpecieEntities(string& SpecieName);
  double getSpecieBiomass(string& SpecieName);
  size_t getNbOfMarkedEntities(void);

  feedbackValType forceEntityAction(entityIdType idEntity, choiceIndType myChoice);
  choiceIndType predictEntityAction(entityIdType idEntity);

  void markAllEntitiesWithGene(CGene& modelGene, bool markDominantAlleleOnly);
  void clearMarksOnAllEntities();

private:
  void markEntityWithGene(CBasicEntity* pEntity);
  void setDefaultEntitiesForNewBiotop(void);

//---------------------------------------------------------------------------
// Entities access
//---------------------------------------------------------------------------
public:
  CBasicEntity* getEntityById(entityIdType idEntity);
  CBasicEntity* getEntityByIndex(size_t index);
  CBasicEntity* getEntityByName(string& entityName);
  size_t        getEntityTableIndex(CBasicEntity* pEntity);
  CBasicEntity* findEntity(const Point_t& searchCoord, size_t Layer);
  CBasicEntity* findEntityNoCheckCoord(const Point_t& searchCoord, size_t layer);
  const BiotopFoundIds_t& findEntitiesInSquare(const Point_t& bottomLeftCoord, size_t squareSize, bool includeWater = false);
  const BiotopFoundIds_t& findEntities(const Point_t& startCoord, size_t distance, bool includeWater = false);
  const BiotopFoundIds_t& findEntities(const Point_t& startCoord, UCHAR sectorBmp, int distance, size_t layer, bool includeWater = false);
  const BiotopFoundIds_t& findFarEntities(const Point_t& startCoord, UCHAR sectorBmp, int rangeMin, int rangeMax, bool includeWater);
  CBasicEntity* findTopLevelEntity(const Point_t& searchCoord);
  bool isCoordValidAndFree(const Point_t& coord, size_t layer);
  bool isGlobalGridCoordValidAndFree(const Point_t& globalCoord, size_t layer);
  bool isCoordValid(const Point_t& coord, size_t layer);
  bool isCoordValid(const Point_t& coord);

  static Point_t getGridCoordFromStepCoord(const Point_t& stepCoord);
  static Point_t getStepCoordFromGridCoord(const Point_t& gridCoord);
  Point_t getGlobalGridCoordFromGridCoord(const Point_t& localGridCoord);
  Point_t getGridCoordFromGlobalGridCoord(const Point_t& globalGridCoord);
  Point_t getGlobalStepCoordFromStepCoord(const Point_t& localStepCoord);
  Point_t getStepCoordFromGlobalStepCoord(const Point_t& globalStepCoord);

private:
  void colorizeSearchNoCheckCoord(const Point_t& coord);
  void putEntityInList(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, size_t layer, bool includeWater);
  void putEntityInListOptim(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, size_t layer, bool includeWater);
  void putEntitiesInListAllLayers(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, bool includeWater);
  void putEntitiesInListAllLayersOptim(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t searchCoord, bool includeWater);
  void findEntitiesInRow(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t startCoord, size_t lenght, bool includeWater = false);
  void findEntitiesInColumn(BiotopFoundIds_t& foundIds, size_t distanceToSet, Point_t startCoord, size_t lenght, bool includeWater = false);

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  void nextSecond(bool doIncreaseTime = true);
  void triggerMeasuresNextSecond(void);
  const BiotopTime_t& getBiotopTime(void);
  void setBiotopTime(int seconds, int hours, int days, int years);
  void setNextHourTimeOffset(unsigned char nextHourTimeOffset);

private:
  void nextHour(void);
  void nextDay(void);
  void nextHourForAllEntities(void);
  void nextSecondForAllAnimals(void);
  void nextSecondForAllAnimalsSingleProcess(void);
  void nextSecondForAllAnimalsMultiProcess(void);

//---------------------------------------------------------------------------
// Grid management
//---------------------------------------------------------------------------
public:
  void ModifyGridFertilityBonus(const Point_t& coord, char bonusToAdd);
  void updateGridEntity(CBasicEntity* pEntity);
  int getGridDistance(const Point_t& gridCoord1, const Point_t& gridCoord2);

  void setGridGroundTypeEarth(size_t gridCoordX, size_t gridCoordY);
  void setGridGroundTypeDeepWater(size_t gridCoordX, size_t gridCoordY);
  void setGridGroundTypeWater(size_t gridCoordX, size_t gridCoordY);
  void setGridGroundTypeGrass(size_t gridCoordX, size_t gridCoordY);
  void setGridGroundTypeRock(size_t gridCoordX, size_t gridCoordY);

private:
  void initGridDefaultLayerType(void);
  void initGridDefaultAltitude(void);
  void initGridEntity(void);
  void buildGrid(size_t dimX, size_t dimY, size_t dimZ);
  void deleteGrid(void);
  void updateGridAllEntities(void);
  void updateGridFertilityBonus(void);

//---------------------------------------------------------------------------
// Measure management
//---------------------------------------------------------------------------
public:
  bool      addMeasureBiotopParam(BiotopParameterType_e type, int period, int id);
  bool      addMeasureEntityParam(CBasicEntity* pEntity, int paramId, int period, int id);
  bool      addMeasureFeeling(CBasicEntity* pEntity, int period, int id, MeasureFeelingType_e type);
  bool      addMeasureReaction(CBasicEntity* pEntity, int reactionInd, int period, int id, MeasureReactionType_e type);
  bool      addMeasurePopulation(int period, int id, MeasurePopulationType_e type, int maxVal, string SpecieName = "");
  CMeasure* getMeasureById(int id);
  CMeasure* getMeasureByIndex(size_t index);
  size_t    getNbOfMeasures(void);
  int       getUnusedMeasureId(int maxMeasureId);
  void      replaceMeasure(int id, CMeasure* pMeasure);
  bool      checkMeasureEvents();
  void      saveAllMeasuresInDataFile(string fileNameWithPath);
  void      saveAllMeasuresInCsvFile(string fileNameWithPath);
  bool      addGeoMapSpeciePopulation(std::string specieName);
  size_t    getNbOfGeoMapSpecie(void);
  CGeoMapPopulation* getGeoMapSpecieByIndex(size_t index);
  void      saveAllGeoMapsInFile(string fileNameWithPath);
  void      saveAllRecordsInFiles();
  void      saveAllEntitiesOfSpecie(string specieName);

  private:
  timeCountType getNextSmallestTimeCountInAllMeasures(timeCountType previousSmallestTimeCount);

//---------------------------------------------------------------------------
// Event management
//---------------------------------------------------------------------------
public:
  bool addBiotopEvent(EntityEventList_e entityEventList, CBasicEntity* pEntity);
  BiotopEventPair getNextUnreadGuiBiotopEvent();
  BiotopEventPair getNextUnreadNetworkBiotopEvent();
  bool resetBiotopEventsMapCurrent();
private:
  std::map<entityIdType, BiotopEvent_t>& getBiotopEventMapCurrent();
  std::map<entityIdType, BiotopEvent_t>& getBiotopEventMapPrevious();
  void clearEventMaps();

//---------------------------------------------------------------------------
// Save/Load in file
//---------------------------------------------------------------------------
public:
  bool saveInXmlFile(string fileName, string pathName, bool saveEntities = true);
  bool saveInXmlFile(TiXmlDocument *pXmlDoc, string pathNameForEntities, bool saveEntityList = true, bool saveEntities = false);
  bool loadFromXmlFile(string fileName, string pathName);
  bool loadFromXmlFile(TiXmlDocument *pXmlDoc, string pathNameForEntities);

//---------------------------------------------------------------------------
// Specific behaviors
//---------------------------------------------------------------------------
public:
  bool addEntitySpawner(size_t index, string entityFileName, string pathName, int intensityRate, int avaragePeriod, bool isProportionalToFertility);
  bool addEntitySpawner(size_t index, CBasicEntity* pModelEntity, int intensityRate, int avaragePeriod, bool isProportionalToFertility);
  void spawnEntitiesRandomly(CBasicEntity* pModelEntity, int coverRate);
private:
  void generateRandomEntities();
  void decreaseOdorMap();
  void memorizeAllPopulationMaps();

//---------------------------------------------------------------------------
// Climate management
//---------------------------------------------------------------------------
public:
  void setClimateModel(const ClimateType_e newClimateType, const int newPeriod = 0);

//---------------------------------------------------------------------------
// CPU Marker
//---------------------------------------------------------------------------
public:
  void resetCpuMarker();
  void logCpuMarkerStart(BiotopCpuMarkerType_e markerId);
  void logCpuMarkerEnd(BiotopCpuMarkerType_e markerId);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  const Point_t& getDimension(void) const;
  const Point_t& getGlobalGridDimension(void) const;
  double getFertility(const Point_t& coord);
  void   setFertilityRate(int fertility);
  double getRadioactivityRate(); 
  double getTemperature(const Point_t& coord, size_t layer);
  double getSunlight(); 
  LayerType_e getLayerType(const Point_t& coord, size_t layer);
  size_t getNbLayer(void);
  CCyclicParam* getParamFertility();
  CCyclicParam* getParamSunlight();
  CGenericParam* getParamRadioactivity(); 
  CCyclicParam* getParamTemperature();
  CGenericParam* getParameter(size_t id);
  CGenericParam* getParameterByName(string& label);
  double getOdorTrace(const Point_t& coord, size_t odorIndex);
  bool getOdorLevels(const Point_t& coord, int range, double odorLevel[NUMBER_ODORS], entityIdType excludedEntityId = ENTITY_ID_INVALID);
  COLORREF getCustomColor(const Point_t& coord);
  BiotopSquare_t** getpBioSquare();
  string getLabel();
  int  getWindDirection();
  void setWindDirection(int direction);
  int  getWindStrenght(); 
  void setWindStrenght(int strenght);
  BiotopRandomEntitiyGeneration_t& getRandomEntitiyGeneration(size_t index);
  size_t getNumberOfRandomEntitiyGeneration();
  void SetColorizeSearchMode(bool bColorizeSearch);
  CGene& getGeneToMark();
  bool getMarkDominantAlleleOnly();
  void setGlobalGridDimension(size_t dimX, size_t dimY);
  void setGlobalGridCoordOffset(const Point_t& startingCoord);
  void setAutoSaveSpecieName(string specieName);

}; // end CBiotop

#endif // !defined(CBIOTOP_INCLUDED_)




