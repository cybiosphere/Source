#include "event_definitions.h"
#include "event_manager.h"
#include "API/Core/Zip/zlib_compression.h"
#include "API/Core/Text/console_logger.h"
#include "CMeasureFactory.h"
#include "CEntityFactory.h"

namespace clan
{
  bool event_manager::buildEventsCreateBiotop(CBiotop* pBiotop, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    pBiotop->saveInXmlFile(&xmlDoc, "", false);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (buildEventsLongString(labelEventLoadMap, xmlZipBuffer, 1, 0, 0, 0, 0, eventVector));
  }

  void event_manager::handleEventCreateBiotop(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int transactionId = 0;
    int custom1, custom2, custom3, custom4;
    DataBuffer bufferOutput;
    bool bufferIsReady = handleEventsLongString(e, m_tBiotopBufferEvent, bufferOutput, transactionId, custom1, custom2, custom3, custom4);
    if (bufferIsReady)
    {
      DataBuffer xmlBuffer = ZLibCompression::decompress(bufferOutput, false);
      TiXmlDocument xmlDoc;
      xmlDoc.Parse(xmlBuffer.get_data());
      pBiotop->loadFromXmlFile(&xmlDoc, "");
      log_event("events", "Loading biotop: %1, Size %2,%3,%4", pBiotop->getLabel(), pBiotop->getDimension().x, pBiotop->getDimension().y, pBiotop->getNbLayer());
    }
  }

  bool event_manager::buildEventsAddEntity(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    pEntity->saveInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (buildEventsLongString(labelEventAddEntity, xmlZipBuffer, pEntity->getId(),
                                     pEntity->getStepCoord().x, pEntity->getStepCoord().y, pEntity->getLayer(), pEntity->getStepDirection(),
                                     eventVector));
  }


  CBasicEntity* event_manager::handleEventAddEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    int transactionId = 0; // contains entityId
    int custom1, custom2, custom3, custom4;
    DataBuffer bufferOutput;
    bool bufferIsReady = handleEventsLongString(e, m_tEntityBufferEvent, bufferOutput, transactionId, custom1, custom2, custom3, custom4);
    if (bufferIsReady)
    {
      return addEntityWithZipBuffer(bufferOutput, transactionId, custom1, custom2, custom3, custom4, pBiotop, setAsRemoteControl);
    }
    return NULL;
  }

  bool event_manager::buildEventsAddCloneEntities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, std::vector<NetGameEvent>& eventVector)
  {
    static constexpr int maxNumEntitiesPerEvent = 500;
    int nbEvents = vectPositions.size() / maxNumEntitiesPerEvent + 1;
    int nbRemainingEntityPos = vectPositions.size();
    int index = 0;
    for (int indexEvent = 0; indexEvent < nbEvents; indexEvent++)
    {
      NetGameEvent newEvent(labelEventAddCloneEntity);
      int nbEntityPosInEvent = cybio_min(maxNumEntitiesPerEvent, nbRemainingEntityPos);
      newEvent.add_argument(nbEntityPosInEvent);
      newEvent.add_argument((int)modelEntityId);
      for (int i = 0; i < nbEntityPosInEvent;  i++)
      {
        newEvent.add_argument((int)(vectPositions[index].entityId));
        newEvent.add_argument(vectPositions[index].layer);
        newEvent.add_argument(vectPositions[index].stepCoordX);
        newEvent.add_argument(vectPositions[index].stepCoordY);
        newEvent.add_argument(vectPositions[index].stepDirection);
        nbRemainingEntityPos--;
        index++;
      }
      eventVector.push_back(std::move(newEvent));
    }
    return true;
  }
  
  bool event_manager::handleEventAddCloneEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    bool resu = false;
    Point_t stepCoord;
    int nbEntityPosInEvent = e.get_argument(0);
    int modelEntityId = e.get_argument(1);
    int index = 2;
    CBasicEntity* pModelEntity = pBiotop->getEntityById(modelEntityId);
    //log_event("events", "handleEventAddCloneEntity nbEntityPosInEvent: %1 modelId=%2", nbEntityPosInEvent, modelEntityId);
    if (pModelEntity == NULL)
    {
      return false;
    }

    for (int i = 0; i < nbEntityPosInEvent; i++)
    {
      int entityId = e.get_argument(index);
      int layer = e.get_argument(index + 1);
      int stepCoordx = e.get_argument(index + 2);
      int stepCoordy = e.get_argument(index + 3);
      int direction = e.get_argument(index + 4);
      stepCoord.x = stepCoordx;
      stepCoord.y = stepCoordy;
      index += 5;

      CBasicEntity* pClonedNewEntity = CEntityFactory::createCloneEntity(pModelEntity);
      pClonedNewEntity->setStepDirection(direction);
      if (entityId != ENTITY_ID_INVALID)
      {
        if (pBiotop->addEntityWithPresetId(entityId, pClonedNewEntity, stepCoord, true, layer) == ENTITY_ID_INVALID)
        {
          delete pClonedNewEntity;
          return false;
        }
      }
      else
      {
        if (pBiotop->addEntity(pClonedNewEntity, CBasicEntity::getGridCoordFromStepCoord(stepCoord), layer) == ENTITY_ID_INVALID)
        {
          delete pClonedNewEntity;
          return false;
        }
      }
      pClonedNewEntity->setRemoteControlled(setAsRemoteControl);
    }
  }

   bool event_manager::buildEventsUpdateEntityData(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    pEntity->saveInXmlFile(&xmlDoc);
    pEntity->saveBrainPurposeMapInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (buildEventsLongString(labelEventUpdateEntityData, xmlZipBuffer, pEntity->getId(),
      pEntity->getStepCoord().x, pEntity->getStepCoord().y, pEntity->getLayer(), pEntity->getStepDirection(),
      eventVector));
  }

   void event_manager::handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop)
   {
     int transactionId = 0; // contains entityId
     int custom1, custom2, custom3, custom4;
     DataBuffer bufferOutput;
     bool bufferIsReady = handleEventsLongString(e, m_tEntityBufferEvent, bufferOutput, transactionId, custom1, custom2, custom3, custom4);
     if (bufferIsReady)
     {
       updateEntityWithZipBuffer(bufferOutput, transactionId, pBiotop);
     }
   }

  NetGameEvent event_manager::buildEventUpdateEntityPos(CBasicEntity* pEntity, std::string eventLabel)
  {
    NetGameEvent newEvent(eventLabel);
    newEvent.add_argument((int)pEntity->getId());
    newEvent.add_argument(pEntity->getLabel());
    newEvent.add_argument(pEntity->getStatus());
    newEvent.add_argument((int)pEntity->getStepCoord().x);
    newEvent.add_argument((int)pEntity->getStepCoord().y);
    newEvent.add_argument((int)pEntity->getLayer());
    newEvent.add_argument(pEntity->getStepDirection());
    newEvent.add_argument((int)pEntity->isImmortal());
    newEvent.add_argument((int)pEntity->getColorRgb());
    newEvent.add_argument((int)pEntity->getForm());
    newEvent.add_argument((int)pEntity->getTexture());
    newEvent.add_argument((int)pEntity->getOdor());
    newEvent.add_argument((int)pEntity->getPheromone());
    newEvent.add_argument((int)pEntity->getAttributePresenceMask());
    //log_event("events", "Send event update entity position: entityID %1 label %2", pEntity->getId(), pEntity->getLabel());
    // Add parameters
    for (int i = 0; i < pEntity->getNumParameter(); i++)
    {
      newEvent.add_argument((float)pEntity->getParameter(i)->getVal());
    }
    // Add other usefull info
    if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
    {
      CAnimal* pAnimal = (CAnimal*)pEntity;
      newEvent.add_argument((int)pAnimal->getBrain()->GetCurrentReactionIndex());
      newEvent.add_argument((int)pAnimal->getBrain()->GetCurrentPurposeIndex());
      newEvent.add_argument((int)pEntity->getCurrentLifeStage()->getStageType());
    }
    return (std::move(newEvent));
  }

  CBasicEntity* event_manager::handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool forceEntityUpdate)
  {
    if (e.get_argument_count() < 8)
    {
      log_event("events", "handleEventUpdateEntityPosition: ERROR bad number of arguments: %1", e.get_argument_count());
      return false;
    }
    int entityId = e.get_argument(0);
    std::string  entityLabel = e.get_argument(1);
    int status = e.get_argument(2);
    Point_t position;
    int positionx = e.get_argument(3);
    int positiony = e.get_argument(4);
    position.x = positionx;
    position.y = positiony;
    int layer = e.get_argument(5);
    int direction = e.get_argument(6);
    int isImmortal = e.get_argument(7);
    int color = e.get_argument(8);
    int form = e.get_argument(9);
    int texture = e.get_argument(10);
    int odor = e.get_argument(11);
    int pheromon = e.get_argument(12);
    int AttributeMask = e.get_argument(13);
    int index = 14;

    // Check if entity exists
    CBasicEntity* pEntity = pBiotop->getEntityById(entityId);
    // If entity exist, update only if remoteControlled or manual mode
    if ((pEntity != NULL) && (forceEntityUpdate || pEntity->isRemoteControlled()))
    {
      float paramValue = 0;    
      //log_event("events", "Biotop update entity position: entityID %1 label %2", (int)entityId, entityLabel);
      if (pEntity->getLabel() != entityLabel)
        log_event("events", "Biotop update entity position: entityID %1 label mistmatch %2 expected %3", entityId, pEntity->getLabel(), entityLabel);
      pEntity->setStatus((StatusType_e)status);
      pEntity->jumpToStepCoord(position, true, layer);
      pEntity->setStepDirection(direction, true);
      pEntity->setImmortal(isImmortal);
      pEntity->setColor((COLORREF)color);
      pEntity->setForm((FormType_e)form);
      pEntity->setTexture((TextureType_e)texture);
      pEntity->setOdor((OdorType_e)odor);
      pEntity->setPheromone((PheromoneType_e)pheromon);
      pEntity->setAttributePresenceMask((DWORD)AttributeMask); 
      // Update parameters
      for (int i = 0; i < pEntity->getNumParameter(); i++)
      {
        paramValue = e.get_argument(index);
        pEntity->getParameter(i)->forceVal(paramValue);
        index++;
      }
      // Additional infos
      if ((pEntity->getClass() >= CLASS_ANIMAL_FIRST) && (e.get_argument_count() > index + 1))
      {
        CAnimal* pAnimal = (CAnimal*)pEntity;
        int reactIndex = e.get_argument(index);
        index++;
        int purposeIndex = e.get_argument(index);
        index++;
        int lifeStage = e.get_argument(index);
        pAnimal->SetCurrentReactionIndex(reactIndex);
        pAnimal->getBrain()->ForceCurrentPurpose(purposeIndex);
        pAnimal->setCurrentLifeStages((LifeStage_e)lifeStage);
      }
    }
    return pEntity;
  }

	NetGameEvent event_manager::buildEventRemoveEntity(CBasicEntity* pEntity, entityIdType entityId)
	{
    NetGameEvent newEvent(labelEventRemoveEntity);
    newEvent.add_argument((int)entityId);
    newEvent.add_argument(pEntity->getLabel());
    return (std::move(newEvent));
	} 

  bool event_manager::handleEventRemoveEntity(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    std::string  entityLabel = e.get_argument(1);
    CBasicEntity* pEntity = pBiotop->getEntityById(entityId);

    if ((pEntity != NULL) && (entityLabel == pEntity->getLabel()))
    {
      log_event("events", "Biotop remove entity: entityID %1 label %2", entityId, pEntity->getLabel());
      pEntity->autoRemove();
      return true;
    }
    else
    {
      //log_event("events", "Biotop remove entity: Error entityID %1 label expected %2", entityId, entityLabel);
      return false;
    }
  }

  NetGameEvent event_manager::buildEventChangeBiotopSpeed(const float newBiotopSpeed, const bool isManualMode)
  {
    NetGameEvent newEvent(labelEventChangeBiotopSpeed);
    newEvent.add_argument(newBiotopSpeed);
    newEvent.add_argument((int)isManualMode);
    return (std::move(newEvent));
  }

  bool event_manager::handleEventChangeBiotopSpeed(const NetGameEvent& e, float& newBiotopSpeed, bool& isManualMode)
  {
    newBiotopSpeed = e.get_argument(0);
    int manualMode = e.get_argument(1);
    isManualMode = manualMode;
    log_event("events", "Biotop speed change: %1 isManual%2", newBiotopSpeed, isManualMode);
    return true;
  }

  NetGameEvent event_manager::buildEventForceEntityAction(const entityIdType entityId, const int actionIndex)
  {
    NetGameEvent newEvent(labelEventForceEntityAction);
    newEvent.add_argument((int)entityId);
    newEvent.add_argument(actionIndex);
    return (std::move(newEvent));
  }

  bool event_manager::handleEventForceEntityAction(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    int actionIndex = e.get_argument(1);
    log_event("events", "ForceEntityAction: entity%1 actionIndex%2", entityId, actionIndex);
    return pBiotop->forceEntityAction(entityId, actionIndex);
  }

  bool event_manager::buildEventsCreateMeasure(CMeasure* pMeasure, std::vector<NetGameEvent>& eventVector)
  {
    std::string dataString = pMeasure->buildStringDataFromMeasure();

    // Compress data string
    DataBuffer dataBuffer(dataString.c_str(), dataString.length());
    DataBuffer dataZipBuffer = ZLibCompression::compress(dataBuffer, false);

    int measureTypeSubType = pMeasure->GetType() + (pMeasure->GetSubTypeId() << 8);
    int entityId = -1;
    if (pMeasure->GetEntity() != NULL)
    {
      entityId = pMeasure->GetEntity()->getId();
    }
    return (buildEventsLongString(labelEventCreateMeasure, dataZipBuffer, pMeasure->GetId(),
      pMeasure->GetPeriod(), measureTypeSubType, pMeasure->GetParameterIndex(), entityId, eventVector));
  }

  void event_manager::handleEventCreateMeasure(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int transactionId = 0; // contains measureId
    int custom1, custom2, custom3, custom4;
    DataBuffer bufferOutput;
    bool bufferIsReady = handleEventsLongString(e, m_tMeasureBufferEvent, bufferOutput, transactionId, custom1, custom2, custom3, custom4);
    if (bufferIsReady)
    {
      createMeasureWithZipBuffer(bufferOutput, pBiotop, transactionId, custom1, custom2, custom3, custom4);
    }
  }

  NetGameEvent event_manager::buildEventReqEntityRefresh(CBasicEntity* pEntity)
  {
    NetGameEvent newEvent(labelEventReqEntityRefresh);
    newEvent.add_argument((int)pEntity->getId());
    newEvent.add_argument(pEntity->getLabel());
    return (std::move(newEvent));
  }

  CBasicEntity* event_manager::handleEventReqEntityRefresh(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    string label = e.get_argument(1);
    log_event("events", "Reqest entity refresh: entity Id=%1 label %2", entityId, label);
    return (pBiotop->getEntityById(entityId));
  }

  bool event_manager::buildEventsAddEntitySpawner(int index, BiotopRandomEntitiyGeneration_t& generator, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    generator.pModelEntity->saveInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (buildEventsLongString(labelEventAddEntitySpawner, xmlZipBuffer, index,
      generator.intensity, generator.avaragePeriodicity, generator.IsProportionalToFertility, 0,
      eventVector));
  }

  void event_manager::handleEventAddEntitySpawner(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int transactionId = 0;
    int custom1, custom2, custom3, custom4;
    DataBuffer bufferOutput;
    bool bufferIsReady = handleEventsLongString(e, m_tEntityBufferEvent, bufferOutput, transactionId, custom1, custom2, custom3, custom4);
    if (bufferIsReady)
    {
      createSpawnerWithZipBuffer(bufferOutput, pBiotop, transactionId, custom1, custom2, custom3);
    }
  }

  bool event_manager::buildEventsCreateGeoMapSpecie(CGeoMapPopulation* pGeoMap, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    pGeoMap->saveInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (buildEventsLongString(labelEventCreateSpecieMap, xmlZipBuffer, pGeoMap->GetSpecieName().size(),
      0, 0, 0, 0, eventVector));
  }

  void event_manager::handleEventCreateGeoMapSpecie(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int transactionId = 0; // contains measureId
    int custom1, custom2, custom3, custom4;
    DataBuffer bufferOutput;
    bool bufferIsReady = handleEventsLongString(e, m_tGeoMapBufferEvent, bufferOutput, transactionId, custom1, custom2, custom3, custom4);
    if (bufferIsReady)
    {
      createGeoMapSpecieWithZipBuffer(bufferOutput, pBiotop);
    }
  }

  NetGameEvent event_manager::buildEventChangeEntityRemoteControl(entityIdType entityId, bool setRemoteControl)
  {
    NetGameEvent newEvent(labelEventChangeRemoteControl);
    newEvent.add_argument((int)entityId);
    newEvent.add_argument((int)setRemoteControl);
    return (std::move(newEvent));
  }

  bool event_manager::handleEventChangeEntityRemoteControl(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    int setRemoteCtrl = e.get_argument(1);
    CBasicEntity* pEntity = pBiotop->getEntityById(entityId);

    if (pEntity != NULL) 
    {
      //log_event("events", "Biotop set entity remote control: entity %1 remote:%2", pEntity->getLabel(), setRemoteCtrl);
      pEntity->setRemoteControlled(setRemoteCtrl);
      return true;
    }
    else
    {
      log_event("events", "Biotop set entity remote control: Error entityID %1", entityId);
      return false;
    }
  }

  bool event_manager::buildEventsLongString(const std::string event_label, const DataBuffer& data, const int transactionId,
                                               const int custom1, const int custom2, const int custom3, const int custom4,
                                               std::vector<NetGameEvent>& eventVector)
  {
    int nbBlocks = data.get_size() / SENT_BUFFER_MAX_SIZE + 1;
    if (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS)
    {
      log_event("events", "event_manager buildEventsLongString: ERROR bad nbBlocks: %1", nbBlocks);
      return false;
    }

    for (int i = 0; i < (nbBlocks - 1); i++)
    {
      NetGameEvent genericEvent(event_label);
      DataBuffer dataBlock(data, i * SENT_BUFFER_MAX_SIZE, SENT_BUFFER_MAX_SIZE);
      genericEvent.add_argument(transactionId);
      genericEvent.add_argument(nbBlocks);
      genericEvent.add_argument(i);
      genericEvent.add_argument(custom1);
      genericEvent.add_argument(custom2);
      genericEvent.add_argument(custom3);
      genericEvent.add_argument(custom4);
      genericEvent.add_argument(dataBlock);
      eventVector.push_back(std::move(genericEvent));
    }

    NetGameEvent genericEvent(event_label);
    DataBuffer dataBlock(data, (nbBlocks - 1) * SENT_BUFFER_MAX_SIZE, (data.get_size() % SENT_BUFFER_MAX_SIZE));
    genericEvent.add_argument(transactionId);
    genericEvent.add_argument(nbBlocks);
    genericEvent.add_argument(nbBlocks - 1);
    genericEvent.add_argument(custom1);
    genericEvent.add_argument(custom2);
    genericEvent.add_argument(custom3);
    genericEvent.add_argument(custom4);
    genericEvent.add_argument(dataBlock);
    eventVector.push_back(std::move(genericEvent));
    return true;
  }

  bool event_manager::handleEventsLongString(const NetGameEvent& e, 
                                             std::vector<LongBufferEvent_t>& tBufferEvent, DataBuffer& bufferOutput,
                                             int& transactionId, int& custom1, int& custom2, int& custom3, int& custom4)
  {
    if (e.get_argument_count() < 8)
    {
      log_event("events", "handleEventsLongString: ERROR bad number of arguments: %1", e.get_argument_count());
      return false;
    }
    int i;
    transactionId = e.get_argument(0); // contains entityId
    int nbBlocks = e.get_argument(1);
    int blocksIndex = e.get_argument(2);
    custom1 = e.get_argument(3);
    custom2 = e.get_argument(4);
    custom3 = e.get_argument(5);
    custom4 = e.get_argument(6);
    DataBuffer bufferBlock = e.get_argument(7);

    if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
    {
      log_event("events", "handleEventsLongString: ERROR bad nbBlocks: %1", nbBlocks);
    }
    else if (nbBlocks == 1)
    {
      bufferOutput = std::move(bufferBlock);
      return true;
    }
    else
    {
      int storeIndex = -1;
      // Find current transaction if exists
      for (i = 0; i < (int)tBufferEvent.size(); i++)
      {
        if (tBufferEvent[i].transactionId == transactionId)
        {
          storeIndex = i;
          break;
        }
      }
      // if context exist, fill context 
      if (storeIndex > -1)
      {
        tBufferEvent[storeIndex].nb_blocks_received++;
        tBufferEvent[storeIndex].buffer[blocksIndex] = bufferBlock;
        if (tBufferEvent[storeIndex].nb_blocks_received == tBufferEvent[storeIndex].nb_blocks)
        {
          DataBuffer fullXmlZipBuffer;
          int fullXmlZipBufferSize = 0;
          int curBufIndex = 0;
          // Process buffer size
          for (i = 0; i < tBufferEvent[storeIndex].nb_blocks; i++)
          {
            fullXmlZipBufferSize += tBufferEvent[storeIndex].buffer[i].get_size();
          }
          fullXmlZipBuffer.set_size(fullXmlZipBufferSize);
          // Copy blocks in single buffer
          for (i = 0; i < tBufferEvent[storeIndex].nb_blocks; i++)
          {
            memcpy(&fullXmlZipBuffer[curBufIndex], &(tBufferEvent[storeIndex].buffer[i][0]), tBufferEvent[storeIndex].buffer[i].get_size());
            curBufIndex += tBufferEvent[storeIndex].buffer[i].get_size();
          }
          // clean tBufferEvent
          tBufferEvent.erase(tBufferEvent.begin() + storeIndex);
          bufferOutput = std::move(fullXmlZipBuffer);
          return true;
        }
      }
      else // (storeIndex==-1) new context creation needed
      {
        LongBufferEvent_t newBufferEvent;
        newBufferEvent.transactionId = transactionId;
        newBufferEvent.nb_blocks = nbBlocks;
        newBufferEvent.nb_blocks_received = 1;
        newBufferEvent.buffer[blocksIndex] = bufferBlock;
        tBufferEvent.insert(tBufferEvent.end(), newBufferEvent);
      }
    }
    return false;
  }

  CBasicEntity* event_manager::addEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, const entityIdType entityId,
                                             const int stepCoordX, const int stepCoordY, const int layer, const int stepDirection,
                                             CBiotop* pBiotop, bool setAsRemoteControl)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());

    CBasicEntity* pNewEntity = CEntityFactory::createEntity(&xmlDoc);
    if (pNewEntity == NULL)
    {
      log_event("events", "ERROR Biotop add entity with NULL entity ");
      return false;
    }
    pNewEntity->setStepDirection(stepDirection);
    log_event("events", "Biotop add entity: %1 state %2 stepCoordX %3 stepCoordY %4 layer %5 ID %6", pNewEntity->getLabel(), pNewEntity->getStatus(), stepCoordX, stepCoordY, layer, (int)entityId);

    Point_t stepCoord{ stepCoordX , stepCoordY };

    if (entityId != ENTITY_ID_INVALID)
    {
      if (pBiotop->addEntityWithPresetId(entityId, pNewEntity, stepCoord, true, layer) == ENTITY_ID_INVALID)
      {
        //log_event("events", "Biotop add entity: Error in addEntityWithPresetId");
        delete pNewEntity;
        return NULL;
      }
    }
    else
    {
      if (pBiotop->addEntity(pNewEntity, CBasicEntity::getGridCoordFromStepCoord(stepCoord), layer) == ENTITY_ID_INVALID)
      {
        log_event("events", "Biotop add entity: Error in addEntity");
        delete pNewEntity;
        return NULL;
      }
    }
    pNewEntity->setRemoteControlled(setAsRemoteControl);
    return pNewEntity;
  }


 bool event_manager::updateEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());

    CBasicEntity* pNewEntity = CEntityFactory::createEntity(&xmlDoc);
    if (pNewEntity == NULL)
    {
      log_event("events", "ERROR Biotop update full entity with NULL entity ID %1", (int)entityId);
      return false;
    }

    log_event("events", "Biotop update full entity: %1 ID %2", pNewEntity->getLabel(), (int)entityId);
    CBasicEntity* pCurEntity;
    bool bFound = false;
    int curStepDirection;
    bool curRemoteCtrl;
    // Update entity with same Id
    pCurEntity = pBiotop->getEntityById(entityId);

    if (pCurEntity != NULL)
    {
      // Do not update entity for pregnant animals
      if (pCurEntity->isLocalAutoControlled() && (pCurEntity->getClass() == CLASS_MAMMAL) && (((CAnimMammal*)pCurEntity)->getGestationBabyNumber() > 0))
      {
        log_event("Client ", "Skip update entity for pregnant animal %1", pCurEntity->getLabel());
        return false;
      }
      curStepDirection = pCurEntity->getStepDirection();
      curRemoteCtrl = pCurEntity->isRemoteControlled();
      pNewEntity->setRemoteControlled(curRemoteCtrl);
      pNewEntity->setStepDirection(curStepDirection);
      if (!pBiotop->replaceEntityByAnother(pCurEntity->getId(), pNewEntity))
      {
        delete pNewEntity;
      }
      pNewEntity->loadPurposeMapFromXmlFile(&xmlDoc);
      bFound = true;
    }
    return (bFound);
  }

  bool event_manager::createMeasureWithZipBuffer(const DataBuffer& dataZipBuffer, CBiotop* pBiotop, const int measureId,
                                                 const int period, const int typeSubType, const int paramId, const int entityId)
  {
    DataBuffer dataBuffer = ZLibCompression::decompress(dataZipBuffer, false);
    CMeasure* pNewMeasure = CMeasureFactory::createMeasure(measureId, period, typeSubType, pBiotop, paramId, entityId);
    if (pNewMeasure != NULL)
    {
      pNewMeasure->buildMeasureDataFromString(dataBuffer.get_data());
      log_event("events", "Biotop create measure: Id%1 period=%2 label=%3", measureId, period, pNewMeasure->GetLabel());
      pBiotop->replaceMeasure(measureId, pNewMeasure);
    }
    return true;
  }

  bool event_manager::createSpawnerWithZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop, const int spawnerId,
                                             const int intensity, const int period, const bool isProportional)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());
    CBasicEntity* pNewEntity = CEntityFactory::createEntity(&xmlDoc);
    if (pNewEntity!=NULL)
    {
      log_event("events", "Add entity spawn: Id%1 period=%2 label=%3", spawnerId, period, pNewEntity->getLabel());
      return (pBiotop->addEntitySpawner(spawnerId, pNewEntity, intensity, period, isProportional));
    }
    else
    {
      log_event("events", "Add entity spawn error: Id%1 period=%2", spawnerId, period);
      return false;
    }
  }

  bool event_manager::createGeoMapSpecieWithZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());

    pBiotop->addGeoMapSpeciePopulation("Unset");
    CGeoMapPopulation* pGeoMapPopu = pBiotop->getGeoMapSpecieByIndex(pBiotop->getNbOfGeoMapSpecie() - 1);
    if (pGeoMapPopu != NULL)
    {
      pGeoMapPopu->loadFromXmlFile(&xmlDoc, 0);
    }
    return true;
  }

}
