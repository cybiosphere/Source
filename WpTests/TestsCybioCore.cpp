#include "TestsCybioCore.h"

void TestsCybioCore::launchAllTests()
{
  testBiotopCreation();
  testAnimalBehavior();
  testAnimalSensors();
}

void TestsCybioCore::testBiotopCreation()
{
  for (size_t nbLayer = 0; nbLayer < 5; nbLayer++)
  {
    printf("testBiotopCreation %u layers\n", nbLayer);
    CBiotop* pBiotop = new CBiotop(100, 100, nbLayer);
    pBiotop->initGridEntity();
    pBiotop->initGridDefaultLayerType();
    pBiotop->initGridDefaultAltitude();
    pBiotop->setDefaultEntitiesForTest();

    EXPECT_EQ(pBiotop->getDimension().x, 100);
    EXPECT_EQ(pBiotop->getDimension().y, 100);
    EXPECT_EQ(pBiotop->getNbOfEntities(), ((nbLayer > 2) ? 390 : 0));
    EXPECT_EQ(pBiotop->getNbOfMinerals(), ((nbLayer > 2) ? 390 : 0));
    EXPECT_EQ(pBiotop->getNbOfVegetals(), 0);
    EXPECT_EQ(pBiotop->getNbOfAnimals(), 0);
    EXPECT_EQ(pBiotop->getNbOfLivingAnimals(), 0);
    delete pBiotop;
  }
}
  
void TestsCybioCore::testAnimalBehavior()
{
  printf("testAnimalBehavior\n");
  CBiotop* pBiotop = new CBiotop(400, 400, 3);
  pBiotop->initGridEntity();
  pBiotop->initGridDefaultLayerType();
  pBiotop->initGridDefaultAltitude();
  pBiotop->setDefaultEntitiesForTest();

  CAnimal* pGazelle = (CAnimal*)pBiotop->createAndAddEntity(pathForEntityFiles + "gazelleTest.xml", pathForEntityFiles, {110, 100});
  printf("Animal %s created\n", pGazelle->getLabel().c_str());
  EXPECT_EQ(pGazelle->getGlobalGridCoord().x, 110);
  EXPECT_EQ(pGazelle->getGlobalGridCoord().y, 100);
  EXPECT_EQ(pGazelle->getStepCoord().x, 1105);
  EXPECT_EQ(pGazelle->getStepCoord().y, 1005);

  pGazelle->predictNextAction();
  EXPECT_EQ(pGazelle->getBrain()->getBrainFocusedEntityId(), ENTITY_ID_INVALID);

  CBasicEntity* pRock = pBiotop->createAndAddEntity(pathForEntityFiles + "rock.xml", pathForEntityFiles, { 116, 100 });
  pGazelle->predictNextAction();
  CBasicEntity* pFocussEntity = pBiotop->getEntityById(pGazelle->getBrain()->getBrainFocusedEntityId());
  EXPECT_NE(pFocussEntity, NULL);
  if (pFocussEntity != NULL)
  {
    EXPECT_EQ(pFocussEntity->getId(), pRock->getId());
    printf("%s focus is %s\n", pGazelle->getLabel().c_str(), pFocussEntity->getLabel().c_str());
  }

  CBasicEntity* pGrass = pBiotop->createAndAddEntity(pathForEntityFiles + "grassDry.xml", pathForEntityFiles, { 125, 100 });
  pGazelle->predictNextAction();
  pFocussEntity = pBiotop->getEntityById(pGazelle->getBrain()->getBrainFocusedEntityId());
  EXPECT_NE(pFocussEntity, NULL);
  if (pFocussEntity != NULL)
  {
    EXPECT_EQ(pFocussEntity->getId(), pRock->getId());
    printf("%s focus is %s\n", pGazelle->getLabel().c_str(), pFocussEntity->getLabel().c_str());
  }

  EXPECT_STREQ(pGazelle->getBrain()->GetCurrentPurpose()->GetLabel(), "regroup");
  pGazelle->setHungerRate(60);
  pGazelle->predictNextAction();
  EXPECT_STREQ(pGazelle->getBrain()->GetCurrentPurpose()->GetLabel(), "feed");

  pGazelle->predictNextAction();
  pGazelle->predictNextAction(); // 2 times to be sure grass was seen
  pFocussEntity = pBiotop->getEntityById(pGazelle->getBrain()->getBrainFocusedEntityId());
  EXPECT_NE(pFocussEntity, NULL);
  if (pFocussEntity != NULL)
  {
    EXPECT_EQ(pFocussEntity->getId(), pGrass->getId());
    printf("%s focus is %s\n", pGazelle->getLabel().c_str(), pFocussEntity->getLabel().c_str());
  }

  for (int i = 0; i < 100; i++)
  {
    pBiotop->nextSecond();
  }
  EXPECT_LT(pGazelle->getHungerRate(), 59);

  printf("%s hunger rate is now %f\n", pGazelle->getLabel().c_str(), pGazelle->getHungerRate());

  delete pBiotop;
}

void TestsCybioCore::testAnimalSensors()
{
  printf("testAnimalSensors\n");
  CBiotop* pBiotop = new CBiotop(200, 200, 3);
  pBiotop->initGridEntity();
  pBiotop->initGridDefaultLayerType();
  pBiotop->initGridDefaultAltitude();
  pBiotop->setDefaultEntitiesForTest();

  CAnimal* pGazelle = (CAnimal*)pBiotop->createAndAddEntity(pathForEntityFiles + "gazelleTest.xml", pathForEntityFiles, { 110, 100 });
  printf("Animal %s created\n", pGazelle->getLabel().c_str());

  printf("Verify sensor range center position\n", pGazelle->getLabel().c_str());
  verifySensorsRange(pBiotop, pGazelle, 2783, 3720, 3481, 361);

  printf("Verify sensor range South West corner position\n", pGazelle->getLabel().c_str());
  pGazelle->jumpToGridCoord({ 2, 2 }, false);
  verifySensorsRange(pBiotop, pGazelle, 903, 1088, 992, 156);

  printf("Verify sensor range North East corner position\n", pGazelle->getLabel().c_str());
  pGazelle->jumpToGridCoord({ 197, 197 }, false);
  verifySensorsRange(pBiotop, pGazelle, 646, 1088, 1056, 132);
  delete pBiotop;
}
void TestsCybioCore::verifySensorsRange(CBiotop* pBiotop, CAnimal* pAnimal,
                                        size_t expectGridViewFar, size_t expectGridEar, size_t expectGridSmell, size_t expectGridPhero)
{
  pBiotop->SetColorizeSearchMode(true);
  CSensor* pSensor = NULL;
  for (int ind = 0; ind < pAnimal->getBrain()->GetNumberSensor(); ind++)
  {
    clearMap(pBiotop);
    pSensor = pAnimal->getBrain()->GetSensorByIndex(ind);
    if (pSensor->GetLabel().find("View far") != std::string::npos)
    {
      pSensor->UpdateAndGetStimulationTable();
      EXPECT_EQ(countColorizedGrid(pBiotop), expectGridViewFar);
    }
    else if (pSensor->GetLabel().find("Ear") != std::string::npos)
    {
      pSensor->UpdateAndGetStimulationTable();
      EXPECT_EQ(countColorizedGrid(pBiotop), expectGridEar);
    }
    else if (pSensor->GetLabel().find("Smell") != std::string::npos)
    {
      pSensor->UpdateAndGetStimulationTable();
      EXPECT_EQ(countColorizedGrid(pBiotop), expectGridSmell);
    }
    else if (pSensor->GetLabel().find("Pheromone") != std::string::npos)
    {
      pSensor->UpdateAndGetStimulationTable();
      EXPECT_EQ(countColorizedGrid(pBiotop), expectGridPhero);
    }
  }
  pBiotop->SetColorizeSearchMode(false);
}

void TestsCybioCore::clearMap(CBiotop* pBiotop)
{
  int i, j;
  BiotopSquare_t** tBioSquare;
  tBioSquare = pBiotop->getpBioSquare();
  for (i = 0; i < pBiotop->getDimension().x; i++)
  {
    for (j = 0; j < pBiotop->getDimension().y; j++)
    {
      tBioSquare[i][j].customColor = 0x00FFFFFF;
    }
  }
}

size_t TestsCybioCore::countColorizedGrid(CBiotop* pBiotop)
{
  int i, j;
  size_t count = 0;
  BiotopSquare_t** tBioSquare;
  tBioSquare = pBiotop->getpBioSquare();
  for (i = 0; i < pBiotop->getDimension().x; i++)
  {
    for (j = 0; j < pBiotop->getDimension().y; j++)
    {
      if (tBioSquare[i][j].customColor != 0x00FFFFFF)
      {
        count++;
      }
    }
  }
  return count;
}