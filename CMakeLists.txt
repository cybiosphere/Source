cmake_minimum_required(VERSION 3.00)
project(cybiosphere)

set(SOURCE Evolution.cpp 
    ./WpCommon/src/Definitions.cpp 
    ./WpCommon/src/CCustomParam.cpp
    ./WpCommon/src/CGenericParam.cpp
    ./WpCommon/src/CGenericCaract.cpp
    ./WpCommon/src/CCyclicParam.cpp
    ./WpBehavior/src/CBrain.cpp
    ./WpBehavior/src/CBrainAnimal.cpp
    ./WpBehavior/src/CFeelingFear.cpp
    ./WpBehavior/src/CFeelingWelfare.cpp
    ./WpBehavior/src/CGeoMapPurpose.cpp
    ./WpBehavior/src/CNeuronMatrix.cpp
    ./WpBehavior/src/CPurpose.cpp
    ./WpBehavior/src/CReaction.cpp
    ./WpBehavior/src/CReactionAttack.cpp
    ./WpBehavior/src/CReactionCopulate.cpp
    ./WpBehavior/src/CReactionDrink.cpp
    ./WpBehavior/src/CReactionEat.cpp
    ./WpBehavior/src/CReactionHide.cpp
    ./WpBehavior/src/CReactionNothing.cpp
    ./WpBehavior/src/CReactionRuminate.cpp
    ./WpBehavior/src/CReactionSleep.cpp
    ./WpBehavior/src/CReactionStepBackward.cpp
    ./WpBehavior/src/CReactionTurnHeadLeft.cpp
    ./WpBehavior/src/CReactionTurnHeadRight.cpp
    ./WpBehavior/src/CReactionTurnLeft.cpp
    ./WpBehavior/src/CReactionTurnRight.cpp
    ./WpBehavior/src/CReactionVarySpeed.cpp
    ./WpBehavior/src/CSensor.cpp
    ./WpBehavior/src/CSensorCompass.cpp
    ./WpBehavior/src/CSensorComposite.cpp
    ./WpBehavior/src/CSensorEar.cpp
    ./WpBehavior/src/CSensorFear.cpp
    ./WpBehavior/src/CSensorHormone.cpp
    ./WpBehavior/src/CSensorHunger.cpp
    ./WpBehavior/src/CSensorLibido.cpp
    ./WpBehavior/src/CSensorMemoryAction.cpp
    ./WpBehavior/src/CSensorMemoryFeedback.cpp
    ./WpBehavior/src/CSensorObscurity.cpp
    ./WpBehavior/src/CSensorOrientation.cpp
    ./WpBehavior/src/CSensorPheromone.cpp
    ./WpBehavior/src/CSensorPleasure.cpp
    ./WpBehavior/src/CSensorSmell.cpp
    ./WpBehavior/src/CSensorSpeed.cpp
    ./WpBehavior/src/CSensorStatic.cpp
    ./WpBehavior/src/CSensorStomachOverload.cpp
    ./WpBehavior/src/CSensorSuffering.cpp
    ./WpBehavior/src/CSensorTactile.cpp
    ./WpBehavior/src/CSensorTaste.cpp
    ./WpBehavior/src/CSensorTemperature.cpp
    ./WpBehavior/src/CSensorThirst.cpp
    ./WpBehavior/src/CSensorTiredness.cpp
    ./WpBehavior/src/CSensorView.cpp
    ./WpBehavior/src/CSensorViewFar.cpp
    ./WpBehavior/src/CSensorViewIdentify.cpp
    ./WpBehavior/src/CSensorViewIdentifyFar.cpp	
    ./WpEntity/src/CAnimal.cpp
    ./WpEntity/src/CAnimMammal.cpp
    ./WpEntity/src/CBasicEntity.cpp
    ./WpEntity/src/CEntityFactory.cpp
    ./WpEntity/src/CGrass.cpp
    ./WpEntity/src/CLifeStage.cpp
    ./WpEntity/src/CMineral.cpp
    ./WpEntity/src/CPhysicalWelfare.cpp
    ./WpEntity/src/CRock.cpp
    ./WpEntity/src/CVegetable.cpp
    ./WpEntity/src/CVegetSpermatophyta.cpp
    ./WpEntity/src/CWater.cpp
    ./WpEnvironment/src/CBiotop.cpp
    ./WpEnvironment/src/CGeoMap.cpp
    ./WpEnvironment/src/CIdentifyChecker.cpp
    ./WpEnvironment/src/CScenarioPlayer.cpp
    ./WpGenetic/src/CChromosome.cpp
    ./WpGenetic/src/CGene.cpp
    ./WpGenetic/src/CGeneDefinitions.cpp
    ./WpGenetic/src/CGeneticParam.cpp
    ./WpGenetic/src/CGenome.cpp
    ./WpGenetic/src/CPairOfChromosome.cpp
    ./WpMeasure/src/CGeoMapPopulation.cpp	
    ./WpMeasure/src/CMeasure.cpp
    ./WpMeasure/src/CMeasureBiotopParam.cpp
    ./WpMeasure/src/CMeasureEntityParam.cpp
    ./WpMeasure/src/CMeasureFeeling.cpp
    ./WpMeasure/src/CMeasureParam.cpp
    ./WpMeasure/src/CMeasurePopulation.cpp
    ./WpMeasure/src/CMeasureReaction.cpp
    ./WpTinyXml/src/tinystr.cpp
    ./WpTinyXml/src/tinyxml.cpp
    ./WpTinyXml/src/tinyxmlerror.cpp
    ./WpTinyXml/src/tinyxmlparser.cpp)

add_compile_definitions(TIXML_USE_STL CYBIOCORE_LOG_IN_FILE)
add_link_options(-pthread -fopenmp)
	
add_executable(${PROJECT_NAME} ${SOURCE})

set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/WpCommon/inc
                       ${CMAKE_CURRENT_SOURCE_DIR}/WpEntity/inc
                       ${CMAKE_CURRENT_SOURCE_DIR}/WpGenetic/inc
                       ${CMAKE_CURRENT_SOURCE_DIR}/WpBehavior/inc
                       ${CMAKE_CURRENT_SOURCE_DIR}/WpMeasure/inc
                       ${CMAKE_CURRENT_SOURCE_DIR}/WpEnvironment/inc
                       ${CMAKE_CURRENT_SOURCE_DIR}/WpTinyXml/inc)

target_include_directories(${PROJECT_NAME} PUBLIC "${COMMON_INCLUDE_DIR}")

